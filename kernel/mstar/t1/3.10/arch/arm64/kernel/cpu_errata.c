/*
 * Contains CPU specific errata definitions
 *
 * Copyright (C) 2014 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/types.h>
#include <asm/cpu.h>
#include <asm/cputype.h>
#include <asm/cpufeature.h>

static bool __maybe_unused
is_affected_midr_range(const struct arm64_cpu_capabilities *entry, int scope)
{
	return MIDR_IS_CPU_MODEL_RANGE(read_cpuid_id(), entry->midr_model,
				       entry->midr_range_min,
				       entry->midr_range_max);
}

#ifdef CONFIG_HARDEN_BRANCH_PREDICTOR
#include <asm/mmu_context.h>
#include <asm/cacheflush.h>

DEFINE_PER_CPU_READ_MOSTLY(struct bp_hardening_data, bp_hardening_data);

#ifdef CONFIG_KVM
extern char __psci_hyp_bp_inval_start[], __psci_hyp_bp_inval_end[];
extern char __smccc_workaround_1_smc_start[];
extern char __smccc_workaround_1_smc_end[];
extern char __smccc_workaround_1_hvc_start[];
extern char __smccc_workaround_1_hvc_end[];

static void __copy_hyp_vect_bpi(int slot, const char *hyp_vecs_start,
				const char *hyp_vecs_end)
{
	void *dst = lm_alias(__bp_harden_hyp_vecs_start + slot * SZ_2K);
	int i;

	for (i = 0; i < SZ_2K; i += 0x80)
		memcpy(dst + i, hyp_vecs_start, hyp_vecs_end - hyp_vecs_start);

	flush_icache_range((uintptr_t)dst, (uintptr_t)dst + SZ_2K);
}

static void __install_bp_hardening_cb(bp_hardening_cb_t fn,
				      const char *hyp_vecs_start,
				      const char *hyp_vecs_end)
{
	static int last_slot = -1;
	static DEFINE_SPINLOCK(bp_lock);
	int cpu, slot = -1;

	spin_lock(&bp_lock);
	for_each_possible_cpu(cpu) {
		if (per_cpu(bp_hardening_data.fn, cpu) == fn) {
			slot = per_cpu(bp_hardening_data.hyp_vectors_slot, cpu);
			break;
		}
	}

	if (slot == -1) {
		last_slot++;
		BUG_ON(((__bp_harden_hyp_vecs_end - __bp_harden_hyp_vecs_start)
			/ SZ_2K) <= last_slot);
		slot = last_slot;
		__copy_hyp_vect_bpi(slot, hyp_vecs_start, hyp_vecs_end);
	}

	__this_cpu_write(bp_hardening_data.hyp_vectors_slot, slot);
	__this_cpu_write(bp_hardening_data.fn, fn);
	spin_unlock(&bp_lock);
}
#else
#define __psci_hyp_bp_inval_start	NULL
#define __psci_hyp_bp_inval_end		NULL
#define __smccc_workaround_1_smc_start		NULL
#define __smccc_workaround_1_smc_end		NULL
#define __smccc_workaround_1_hvc_start		NULL
#define __smccc_workaround_1_hvc_end		NULL

static void __install_bp_hardening_cb(bp_hardening_cb_t fn,
				      const char *hyp_vecs_start,
				      const char *hyp_vecs_end)
{
	__this_cpu_write(bp_hardening_data.fn, fn);
}
#endif	/* CONFIG_KVM */

static void  install_bp_hardening_cb(const struct arm64_cpu_capabilities *entry,
				     bp_hardening_cb_t fn,
				     const char *hyp_vecs_start,
				     const char *hyp_vecs_end)
{
	u64 pfr0;

	if (!entry->matches(entry, SCOPE_LOCAL_CPU))
		return;

	pfr0 = read_cpuid(ID_AA64PFR0_EL1);
	if (cpuid_feature_extract_unsigned_field(pfr0, ID_AA64PFR0_CSV2_SHIFT))
		return;

	__install_bp_hardening_cb(fn, hyp_vecs_start, hyp_vecs_end);
}

#include <asm/psci.h>
#include <uapi/linux/psci.h>
#include <linux/arm-smccc.h>

static void call_smc_arch_workaround_1(void)
{
	arm_smccc_1_1_smc(ARM_SMCCC_ARCH_WORKAROUND_1, NULL);
}

static void call_hvc_arch_workaround_1(void)
{
	arm_smccc_1_1_hvc(ARM_SMCCC_ARCH_WORKAROUND_1, NULL);
}

static int enable_smccc_arch_workaround_1(void *data)
{
	const struct arm64_cpu_capabilities *entry = data;
	bp_hardening_cb_t cb;
	void *smccc_start, *smccc_end;
	struct arm_smccc_res res;

	if (!entry->matches(entry, SCOPE_LOCAL_CPU))
		return 0;

	if (psci_ops.smccc_version == SMCCC_VERSION_1_0)
		return 0;

	switch (psci_ops.conduit) {
	case PSCI_CONDUIT_HVC:
		arm_smccc_1_1_hvc(ARM_SMCCC_ARCH_FEATURES_FUNC_ID,
				  ARM_SMCCC_ARCH_WORKAROUND_1, &res);
		if (res.a0)
			return 0;
		cb = call_hvc_arch_workaround_1;
		smccc_start = __smccc_workaround_1_hvc_start;
		smccc_end = __smccc_workaround_1_hvc_end;
		break;

	case PSCI_CONDUIT_SMC:
		arm_smccc_1_1_smc(ARM_SMCCC_ARCH_FEATURES_FUNC_ID,
				  ARM_SMCCC_ARCH_WORKAROUND_1, &res);
		if (res.a0)
			return 0;
		cb = call_smc_arch_workaround_1;
		smccc_start = __smccc_workaround_1_smc_start;
		smccc_end = __smccc_workaround_1_smc_end;
		break;

	default:
		return 0;
	}

	install_bp_hardening_cb(entry, cb, smccc_start, smccc_end);

	return 0;
}
#endif	/* CONFIG_HARDEN_BRANCH_PREDICTOR */

#define MIDR_RANGE(model, min, max) \
	.def_scope = SCOPE_LOCAL_CPU, \
	.matches = is_affected_midr_range, \
	.midr_model = model, \
	.midr_range_min = min, \
	.midr_range_max = max

#define MIDR_ALL_VERSIONS(model) \
	.def_scope = SCOPE_LOCAL_CPU, \
	.matches = is_affected_midr_range, \
	.midr_model = model, \
	.midr_range_min = 0, \
	.midr_range_max = (MIDR_VARIANT_MASK | MIDR_REVISION_MASK)

const struct arm64_cpu_capabilities arm64_errata[] = {
	{
	/* Cortex-A53 r0p[012] */
		.desc = "ARM errata 826319, 827319, 824069",
		.capability = ARM64_WORKAROUND_CLEAN_CACHE,
		MIDR_RANGE(MIDR_CORTEX_A53, 0x00, 0x02),
	},
#ifdef CONFIG_HARDEN_BRANCH_PREDICTOR
	{
		.capability = ARM64_HARDEN_BRANCH_PREDICTOR,
		MIDR_ALL_VERSIONS(MIDR_CORTEX_A72),
		.enable = enable_smccc_arch_workaround_1,
	},
	{
		.capability = ARM64_HARDEN_BRANCH_PREDICTOR,
		MIDR_ALL_VERSIONS(MIDR_CORTEX_A73),
		.enable = enable_smccc_arch_workaround_1,
	},
	{
		.capability = ARM64_HARDEN_BRANCH_PREDICTOR,
		MIDR_ALL_VERSIONS(MIDR_CORTEX_A75),
		.enable = enable_smccc_arch_workaround_1,
	},
#endif
	{
	}
};

/*
 * The CPU Errata work arounds are detected and applied at boot time
 * and the related information is freed soon after. If the new CPU requires
 * an errata not detected at boot, fail this CPU.
 */
void verify_local_cpu_errata(void)
{
	const struct arm64_cpu_capabilities *caps = arm64_errata;

	for (; caps->matches; caps++) {
		if (cpus_have_cap(caps->capability)) {
			if (caps->enable)
				caps->enable((void *)caps);
		} else if (caps->matches(caps, SCOPE_LOCAL_CPU)) {
			pr_crit("CPU%d: Requires work around for %s, not detected"
					" at boot time\n",
				smp_processor_id(),
				caps->desc ? : "an erratum");
		}
	}
}

void check_local_cpu_errata(void)
{
	update_cpu_capabilities(arm64_errata, "enabling workaround for");
}
