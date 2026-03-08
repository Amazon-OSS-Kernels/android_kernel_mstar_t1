/*
 * Contains CPU feature definitions
 *
 * Copyright (C) 2015 ARM Ltd.
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

#define pr_fmt(fmt) "CPU features: " fmt

#include <linux/bsearch.h>
#include <linux/cpumask.h>
#include <linux/sort.h>
#include <linux/stop_machine.h>
#include <linux/types.h>
#include <asm/cpu.h>
#include <asm/cputype.h>
#include <asm/cpufeature.h>
#include <asm/cpu_ops.h>
#include <asm/processor.h>
#include <asm/sysreg.h>

unsigned long elf_hwcap __read_mostly;
EXPORT_SYMBOL_GPL(elf_hwcap);

#ifdef CONFIG_COMPAT
#define COMPAT_ELF_HWCAP_DEFAULT        \
		(COMPAT_HWCAP_HALF|COMPAT_HWCAP_THUMB|\
		COMPAT_HWCAP_FAST_MULT|COMPAT_HWCAP_EDSP|\
		COMPAT_HWCAP_TLS|COMPAT_HWCAP_VFP|\
		COMPAT_HWCAP_VFPv3|COMPAT_HWCAP_VFPv4|\
		COMPAT_HWCAP_NEON|COMPAT_HWCAP_IDIV)
unsigned int compat_elf_hwcap __read_mostly = COMPAT_ELF_HWCAP_DEFAULT;
unsigned int compat_elf_hwcap2 __read_mostly;
#endif

DECLARE_BITMAP(cpu_hwcaps, ARM64_NCAPS);

static bool mixed_endian_el0 = true;

bool cpu_supports_mixed_endian_el0(void)
{
	return id_aa64mmfr0_mixed_endian_el0(read_cpuid(ID_AA64MMFR0_EL1));
}

bool system_supports_mixed_endian_el0(void)
{
	return mixed_endian_el0;
}

static void update_mixed_endian_el0_support(struct cpuinfo_arm64 *info)
{
	mixed_endian_el0 &= id_aa64mmfr0_mixed_endian_el0(info->reg_id_aa64mmfr0);
}

#define ARM64_FTR_BITS(STRICT, TYPE, SHIFT, WIDTH, SAFE_VAL) \
	{						\
		.strict = STRICT,			\
		.type = TYPE,				\
		.shift = SHIFT,				\
		.width = WIDTH,				\
		.safe_val = SAFE_VAL,			\
	}

#define ARM64_FTR_END					\
	{						\
		.width = 0,				\
	}

static struct arm64_ftr_bits ftr_id_aa64isar0[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 32, 32, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64ISAR0_RDM_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 24, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64ISAR0_ATOMICS_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64ISAR0_CRC32_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64ISAR0_SHA2_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64ISAR0_SHA1_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64ISAR0_AES_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 0, 4, 0),	/* RAZ */
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_id_aa64pfr0[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 32, 32, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 28, 4, 0),
	ARM64_FTR_BITS(FTR_NONSTRICT, FTR_LOWER_SAFE, ID_AA64PFR0_CSV2_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64PFR0_GIC_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64PFR0_ASIMD_SHIFT, 4, ID_AA64PFR0_ASIMD_NI),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64PFR0_FP_SHIFT, 4, ID_AA64PFR0_FP_NI),
	/* Linux doesn't care about the EL3 */
	ARM64_FTR_BITS(FTR_NONSTRICT, FTR_EXACT, ID_AA64PFR0_EL3_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64PFR0_EL2_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64PFR0_EL1_SHIFT, 4, ID_AA64PFR0_EL1_64BIT_ONLY),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64PFR0_EL0_SHIFT, 4, ID_AA64PFR0_EL0_64BIT_ONLY),
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_id_aa64mmfr0[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 32, 32, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR0_TGRAN4_SHIFT, 4, ID_AA64MMFR0_TGRAN4_NI),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR0_TGRAN64_SHIFT, 4, ID_AA64MMFR0_TGRAN64_NI),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR0_TGRAN16_SHIFT, 4, ID_AA64MMFR0_TGRAN16_NI),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR0_BIGENDEL0_SHIFT, 4, 0),
	/* Linux shouldn't care about secure memory */
	ARM64_FTR_BITS(FTR_NONSTRICT, FTR_EXACT, ID_AA64MMFR0_SNSMEM_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR0_BIGENDEL_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR0_ASID_SHIFT, 4, 0),
	/*
	 * Differing PARange is fine as long as all peripherals and memory are mapped
	 * within the minimum PARange of all CPUs
	 */
	ARM64_FTR_BITS(FTR_NONSTRICT, FTR_LOWER_SAFE, ID_AA64MMFR0_PARANGE_SHIFT, 4, 0),
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_id_aa64mmfr1[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 32, 32, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64MMFR1_PAN_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR1_LOR_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR1_HPD_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR1_VHE_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR1_VMIDBITS_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64MMFR1_HADBS_SHIFT, 4, 0),
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_ctr[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 31, 1, 1),	/* RAO */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 28, 3, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_HIGHER_SAFE, 24, 4, 0),	/* CWG */
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 20, 4, 0),	/* ERG */
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 16, 4, 1),	/* DminLine */
	/*
	 * Linux can handle differing I-cache policies. Userspace JITs will
	 * make use of *minLine
	 */
	ARM64_FTR_BITS(FTR_NONSTRICT, FTR_EXACT, 14, 2, 0),	/* L1Ip */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 4, 10, 0),	/* RAZ */
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 0, 4, 0),	/* IminLine */
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_id_mmfr0[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 28, 4, 0),	/* InnerShr */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 24, 4, 0),	/* FCSE */
	ARM64_FTR_BITS(FTR_NONSTRICT, FTR_LOWER_SAFE, 20, 4, 0),	/* AuxReg */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 16, 4, 0),	/* TCM */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 12, 4, 0),	/* ShareLvl */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 8, 4, 0),	/* OuterShr */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 4, 4, 0),	/* PMSA */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 0, 4, 0),	/* VMSA */
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_id_aa64dfr0[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 32, 32, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64DFR0_CTX_CMPS_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64DFR0_WRPS_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, ID_AA64DFR0_BRPS_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64DFR0_PMUVER_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64DFR0_TRACEVER_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_AA64DFR0_DEBUGVER_SHIFT, 4, 0x6),
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_mvfr2[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 8, 24, 0),	/* RAZ */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 4, 4, 0),		/* FPMisc */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 0, 4, 0),		/* SIMDMisc */
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_dczid[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 5, 27, 0),	/* RAZ */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 4, 1, 1),		/* DZP */
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 0, 4, 0),	/* BS */
	ARM64_FTR_END,
};


static struct arm64_ftr_bits ftr_id_isar5[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_ISAR5_RDM_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 20, 4, 0),	/* RAZ */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_ISAR5_CRC32_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_ISAR5_SHA2_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_ISAR5_SHA1_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_ISAR5_AES_SHIFT, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, ID_ISAR5_SEVL_SHIFT, 4, 0),
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_id_mmfr4[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 8, 24, 0),	/* RAZ */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 4, 4, 0),		/* ac2 */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 0, 4, 0),		/* RAZ */
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_id_pfr0[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 16, 16, 0),	/* RAZ */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 12, 4, 0),	/* State3 */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 8, 4, 0),		/* State2 */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 4, 4, 0),		/* State1 */
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 0, 4, 0),		/* State0 */
	ARM64_FTR_END,
};

/*
 * Common ftr bits for a 32bit register with all hidden, strict
 * attributes, with 4bit feature fields and a default safe value of
 * 0. Covers the following 32bit registers:
 * id_isar[0-4], id_mmfr[1-3], id_pfr1, mvfr[0-1]
 */
static struct arm64_ftr_bits ftr_generic_32bits[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 28, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 24, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 20, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 16, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 12, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 8, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 4, 4, 0),
	ARM64_FTR_BITS(FTR_STRICT, FTR_LOWER_SAFE, 0, 4, 0),
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_generic[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 0, 64, 0),
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_generic32[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 0, 32, 0),
	ARM64_FTR_END,
};

static struct arm64_ftr_bits ftr_aa64raz[] = {
	ARM64_FTR_BITS(FTR_STRICT, FTR_EXACT, 0, 64, 0),
	ARM64_FTR_END,
};

#define ARM64_FTR_REG(id, table)		\
	{					\
		.sys_id = id,			\
		.name = #id,			\
		.ftr_bits = &((table)[0]),	\
	}

static struct arm64_ftr_reg arm64_ftr_regs[] = {

	/* Op1 = 0, CRn = 0, CRm = 1 */
	ARM64_FTR_REG(SYS_ID_PFR0_EL1, ftr_id_pfr0),
	ARM64_FTR_REG(SYS_ID_PFR1_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_DFR0_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_MMFR0_EL1, ftr_id_mmfr0),
	ARM64_FTR_REG(SYS_ID_MMFR1_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_MMFR2_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_MMFR3_EL1, ftr_generic_32bits),

	/* Op1 = 0, CRn = 0, CRm = 2 */
	ARM64_FTR_REG(SYS_ID_ISAR0_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_ISAR1_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_ISAR2_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_ISAR3_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_ISAR4_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_ID_ISAR5_EL1, ftr_id_isar5),
	ARM64_FTR_REG(SYS_ID_MMFR4_EL1, ftr_id_mmfr4),

	/* Op1 = 0, CRn = 0, CRm = 3 */
	ARM64_FTR_REG(SYS_MVFR0_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_MVFR1_EL1, ftr_generic_32bits),
	ARM64_FTR_REG(SYS_MVFR2_EL1, ftr_mvfr2),

	/* Op1 = 0, CRn = 0, CRm = 4 */
	ARM64_FTR_REG(SYS_ID_AA64PFR0_EL1, ftr_id_aa64pfr0),
	ARM64_FTR_REG(SYS_ID_AA64PFR1_EL1, ftr_aa64raz),

	/* Op1 = 0, CRn = 0, CRm = 5 */
	ARM64_FTR_REG(SYS_ID_AA64DFR0_EL1, ftr_id_aa64dfr0),
	ARM64_FTR_REG(SYS_ID_AA64DFR1_EL1, ftr_generic),

	/* Op1 = 0, CRn = 0, CRm = 6 */
	ARM64_FTR_REG(SYS_ID_AA64ISAR0_EL1, ftr_id_aa64isar0),
	ARM64_FTR_REG(SYS_ID_AA64ISAR1_EL1, ftr_aa64raz),

	/* Op1 = 0, CRn = 0, CRm = 7 */
	ARM64_FTR_REG(SYS_ID_AA64MMFR0_EL1, ftr_id_aa64mmfr0),
	ARM64_FTR_REG(SYS_ID_AA64MMFR1_EL1, ftr_id_aa64mmfr1),

	/* Op1 = 3, CRn = 0, CRm = 0 */
	ARM64_FTR_REG(SYS_CTR_EL0, ftr_ctr),
	ARM64_FTR_REG(SYS_DCZID_EL0, ftr_dczid),

	/* Op1 = 3, CRn = 14, CRm = 0 */
	ARM64_FTR_REG(SYS_CNTFRQ_EL0, ftr_generic32),
};

static int search_cmp_ftr_reg(const void *id, const void *regp)
{
	return (int)(unsigned long)id - (int)((const struct arm64_ftr_reg *)regp)->sys_id;
}

/*
 * get_arm64_ftr_reg - Lookup a feature register entry using its
 * sys_reg() encoding. With the array arm64_ftr_regs sorted in the
 * ascending order of sys_id , we use binary search to find a matching
 * entry.
 *
 * returns - Upon success,  matching ftr_reg entry for id.
 *         - NULL on failure. It is upto the caller to decide
 *	     the impact of a failure.
 */
static struct arm64_ftr_reg *get_arm64_ftr_reg(u32 sys_id)
{
	return bsearch((const void *)(unsigned long)sys_id,
			arm64_ftr_regs,
			ARRAY_SIZE(arm64_ftr_regs),
			sizeof(arm64_ftr_regs[0]),
			search_cmp_ftr_reg);
}

static u64 arm64_ftr_set_value(struct arm64_ftr_bits *ftrp, s64 reg, s64 ftr_val)
{
	u64 mask = arm64_ftr_mask(ftrp);

	reg &= ~mask;
	reg |= (ftr_val << ftrp->shift) & mask;
	return reg;
}

static s64 arm64_ftr_safe_value(struct arm64_ftr_bits *ftrp, s64 new, s64 cur)
{
	s64 ret = 0;

	switch (ftrp->type) {
	case FTR_EXACT:
		ret = ftrp->safe_val;
		break;
	case FTR_LOWER_SAFE:
		ret = new < cur ? new : cur;
		break;
	case FTR_HIGHER_SAFE:
		ret = new > cur ? new : cur;
		break;
	default:
		BUG();
	}

	return ret;
}

static int __init sort_cmp_ftr_regs(const void *a, const void *b)
{
	return ((const struct arm64_ftr_reg *)a)->sys_id -
		 ((const struct arm64_ftr_reg *)b)->sys_id;
}

static void __init swap_ftr_regs(void *a, void *b, int size)
{
	struct arm64_ftr_reg tmp = *(struct arm64_ftr_reg *)a;
	*(struct arm64_ftr_reg *)a = *(struct arm64_ftr_reg *)b;
	*(struct arm64_ftr_reg *)b = tmp;
}

static void __init sort_ftr_regs(void)
{
	/* Keep the array sorted so that we can do the binary search */
	sort(arm64_ftr_regs,
		ARRAY_SIZE(arm64_ftr_regs),
		sizeof(arm64_ftr_regs[0]),
		sort_cmp_ftr_regs,
		swap_ftr_regs);
}

/*
 * Initialise the CPU feature register from Boot CPU values.
 * Also initiliases the strict_mask for the register.
 */
static void __init init_cpu_ftr_reg(u32 sys_reg, u64 new)
{
	u64 val = 0;
	u64 strict_mask = ~0x0ULL;
	struct arm64_ftr_bits *ftrp;
	struct arm64_ftr_reg *reg = get_arm64_ftr_reg(sys_reg);

	BUG_ON(!reg);

	for (ftrp  = reg->ftr_bits; ftrp->width; ftrp++) {
		s64 ftr_new = arm64_ftr_value(ftrp, new);

		val = arm64_ftr_set_value(ftrp, val, ftr_new);
		if (!ftrp->strict)
			strict_mask &= ~arm64_ftr_mask(ftrp);
	}
	reg->sys_val = val;
	reg->strict_mask = strict_mask;
}

void __init init_cpu_features(struct cpuinfo_arm64 *info)
{
	/* Before we start using the tables, make sure it is sorted */
	sort_ftr_regs();

	init_cpu_ftr_reg(SYS_CTR_EL0, info->reg_ctr);
	init_cpu_ftr_reg(SYS_DCZID_EL0, info->reg_dczid);
	init_cpu_ftr_reg(SYS_CNTFRQ_EL0, info->reg_cntfrq);
	init_cpu_ftr_reg(SYS_ID_AA64DFR0_EL1, info->reg_id_aa64dfr0);
	init_cpu_ftr_reg(SYS_ID_AA64DFR1_EL1, info->reg_id_aa64dfr1);
	init_cpu_ftr_reg(SYS_ID_AA64ISAR0_EL1, info->reg_id_aa64isar0);
	init_cpu_ftr_reg(SYS_ID_AA64ISAR1_EL1, info->reg_id_aa64isar1);
	init_cpu_ftr_reg(SYS_ID_AA64MMFR0_EL1, info->reg_id_aa64mmfr0);
	init_cpu_ftr_reg(SYS_ID_AA64MMFR1_EL1, info->reg_id_aa64mmfr1);
	init_cpu_ftr_reg(SYS_ID_AA64PFR0_EL1, info->reg_id_aa64pfr0);
	init_cpu_ftr_reg(SYS_ID_AA64PFR1_EL1, info->reg_id_aa64pfr1);
	init_cpu_ftr_reg(SYS_ID_DFR0_EL1, info->reg_id_dfr0);
	init_cpu_ftr_reg(SYS_ID_ISAR0_EL1, info->reg_id_isar0);
	init_cpu_ftr_reg(SYS_ID_ISAR1_EL1, info->reg_id_isar1);
	init_cpu_ftr_reg(SYS_ID_ISAR2_EL1, info->reg_id_isar2);
	init_cpu_ftr_reg(SYS_ID_ISAR3_EL1, info->reg_id_isar3);
	init_cpu_ftr_reg(SYS_ID_ISAR4_EL1, info->reg_id_isar4);
	init_cpu_ftr_reg(SYS_ID_ISAR5_EL1, info->reg_id_isar5);
	init_cpu_ftr_reg(SYS_ID_MMFR0_EL1, info->reg_id_mmfr0);
	init_cpu_ftr_reg(SYS_ID_MMFR1_EL1, info->reg_id_mmfr1);
	init_cpu_ftr_reg(SYS_ID_MMFR2_EL1, info->reg_id_mmfr2);
	init_cpu_ftr_reg(SYS_ID_MMFR3_EL1, info->reg_id_mmfr3);
	init_cpu_ftr_reg(SYS_ID_PFR0_EL1, info->reg_id_pfr0);
	init_cpu_ftr_reg(SYS_ID_PFR1_EL1, info->reg_id_pfr1);
	init_cpu_ftr_reg(SYS_MVFR0_EL1, info->reg_mvfr0);
	init_cpu_ftr_reg(SYS_MVFR1_EL1, info->reg_mvfr1);
	init_cpu_ftr_reg(SYS_MVFR2_EL1, info->reg_mvfr2);

	/* This will be removed later, once we start using the infrastructure */
	update_mixed_endian_el0_support(info);
}

static void update_cpu_ftr_reg(u32 sys_reg, u64 new)
{
	struct arm64_ftr_bits *ftrp;
	struct arm64_ftr_reg *reg = get_arm64_ftr_reg(sys_reg);

	BUG_ON(!reg);

	for (ftrp = reg->ftr_bits; ftrp->width; ftrp++) {
		s64 ftr_cur = arm64_ftr_value(ftrp, reg->sys_val);
		s64 ftr_new = arm64_ftr_value(ftrp, new);

		if (ftr_cur == ftr_new)
			continue;
		/* Find a safe value */
		ftr_new = arm64_ftr_safe_value(ftrp, ftr_new, ftr_cur);
		reg->sys_val = arm64_ftr_set_value(ftrp, reg->sys_val, ftr_new);
	}

}

/* Update CPU feature register from non-boot CPU */
void update_cpu_features(struct cpuinfo_arm64 *info)
{
	update_cpu_ftr_reg(SYS_CTR_EL0, info->reg_ctr);
	update_cpu_ftr_reg(SYS_DCZID_EL0, info->reg_dczid);
	update_cpu_ftr_reg(SYS_CNTFRQ_EL0, info->reg_cntfrq);
	update_cpu_ftr_reg(SYS_ID_AA64DFR0_EL1, info->reg_id_aa64dfr0);
	update_cpu_ftr_reg(SYS_ID_AA64DFR1_EL1, info->reg_id_aa64dfr1);
	update_cpu_ftr_reg(SYS_ID_AA64ISAR0_EL1, info->reg_id_aa64isar0);
	update_cpu_ftr_reg(SYS_ID_AA64ISAR1_EL1, info->reg_id_aa64isar1);
	update_cpu_ftr_reg(SYS_ID_AA64MMFR0_EL1, info->reg_id_aa64mmfr0);
	update_cpu_ftr_reg(SYS_ID_AA64MMFR1_EL1, info->reg_id_aa64mmfr1);
	update_cpu_ftr_reg(SYS_ID_AA64PFR0_EL1, info->reg_id_aa64pfr0);
	update_cpu_ftr_reg(SYS_ID_AA64PFR1_EL1, info->reg_id_aa64pfr1);
	update_cpu_ftr_reg(SYS_ID_DFR0_EL1, info->reg_id_dfr0);
	update_cpu_ftr_reg(SYS_ID_ISAR0_EL1, info->reg_id_isar0);
	update_cpu_ftr_reg(SYS_ID_ISAR1_EL1, info->reg_id_isar1);
	update_cpu_ftr_reg(SYS_ID_ISAR2_EL1, info->reg_id_isar2);
	update_cpu_ftr_reg(SYS_ID_ISAR3_EL1, info->reg_id_isar3);
	update_cpu_ftr_reg(SYS_ID_ISAR4_EL1, info->reg_id_isar4);
	update_cpu_ftr_reg(SYS_ID_ISAR5_EL1, info->reg_id_isar5);
	update_cpu_ftr_reg(SYS_ID_MMFR0_EL1, info->reg_id_mmfr0);
	update_cpu_ftr_reg(SYS_ID_MMFR1_EL1, info->reg_id_mmfr1);
	update_cpu_ftr_reg(SYS_ID_MMFR2_EL1, info->reg_id_mmfr2);
	update_cpu_ftr_reg(SYS_ID_MMFR3_EL1, info->reg_id_mmfr3);
	update_cpu_ftr_reg(SYS_ID_PFR0_EL1, info->reg_id_pfr0);
	update_cpu_ftr_reg(SYS_ID_PFR1_EL1, info->reg_id_pfr1);
	update_cpu_ftr_reg(SYS_MVFR0_EL1, info->reg_mvfr0);
	update_cpu_ftr_reg(SYS_MVFR1_EL1, info->reg_mvfr1);
	update_cpu_ftr_reg(SYS_MVFR2_EL1, info->reg_mvfr2);

	update_mixed_endian_el0_support(info);
}

static bool
feature_matches(u64 reg, const struct arm64_cpu_capabilities *entry)
{
	int val = cpuid_feature_extract_field(reg, entry->field_pos);

	return val >= entry->min_field_value;
}

static bool
has_id_aa64pfr0_feature(const struct arm64_cpu_capabilities *entry)
{
	u64 val;

	val = read_cpuid(id_aa64pfr0_el1);
	return feature_matches(val, entry);
}

static bool __maybe_unused
has_id_aa64mmfr1_feature(const struct arm64_cpu_capabilities *entry)
{
	u64 val;

	val = read_cpuid(id_aa64mmfr1_el1);
	return feature_matches(val, entry);
}

static bool has_no_hw_prefetch(const struct arm64_cpu_capabilities *entry)
{
	u32 midr = read_cpuid_id();
	u32 rv_min, rv_max;

	/* Cavium ThunderX pass 1.x and 2.x */
	rv_min = 0;
	rv_max = (1 << MIDR_VARIANT_SHIFT) | MIDR_REVISION_MASK;

	return MIDR_IS_CPU_MODEL_RANGE(midr, MIDR_THUNDERX, rv_min, rv_max);
}

static const struct arm64_cpu_capabilities arm64_features[] = {
	{
		.desc = "GIC system register CPU interface",
		.capability = ARM64_HAS_SYSREG_GIC_CPUIF,
		.def_scope = SCOPE_SYSTEM,
		.matches = has_id_aa64pfr0_feature,
		.field_pos = 24,
		.min_field_value = 1,
	},
#ifdef CONFIG_ARM64_PAN
	{
		.desc = "Privileged Access Never",
		.capability = ARM64_HAS_PAN,
		.def_scope = SCOPE_SYSTEM,
		.matches = has_id_aa64mmfr1_feature,
		.field_pos = 20,
		.min_field_value = 1,
		.enable = cpu_enable_pan,
	},
#endif /* CONFIG_ARM64_PAN */
	{
		.desc = "Software prefetching using PRFM",
		.capability = ARM64_HAS_NO_HW_PREFETCH,
		.matches = has_no_hw_prefetch,
	},
	{},
};

void update_cpu_capabilities(const struct arm64_cpu_capabilities *caps,
			    const char *info)
{
	for (; caps->desc; caps++) {
		if (!caps->matches(caps, caps->def_scope))
			continue;

		if (!cpus_have_cap(caps->capability))
			pr_info("%s %s\n", info, caps->desc);
		cpus_set_cap(caps->capability);
	}
}

/*
 * Run through the enabled capabilities and enable() it on all active
 * CPUs
 */
void enable_cpu_capabilities(const struct arm64_cpu_capabilities *caps)
{
	for (; caps->desc; caps++) {
		if (caps->enable && cpus_have_cap(caps->capability)) {
			/*
			 * Use stop_machine() as it schedules the work allowing
			 * us to modify PSTATE, instead of on_each_cpu() which
			 * uses an IPI, giving us a PSTATE that disappears when
			 * we return.
			 */
			stop_machine(caps->enable, (void *)caps, cpu_online_mask);
		}
	}
}

#ifdef CONFIG_HOTPLUG_CPU

/*
 * Flag to indicate if we have computed the system wide
 * capabilities based on the boot time active CPUs. This
 * will be used to determine if a new booting CPU should
 * go through the verification process to make sure that it
 * supports the system capabilities, without using a hotplug
 * notifier.
 */
static bool sys_caps_initialised;

static inline void set_sys_caps_initialised(void)
{
	sys_caps_initialised = true;
}

/*
 * Park the CPU which doesn't have the capability as advertised
 * by the system.
 */
static void fail_incapable_cpu(char *cap_type,
				 const struct arm64_cpu_capabilities *cap)
{
	int cpu = smp_processor_id();

	pr_crit("CPU%d: missing %s : %s\n", cpu, cap_type, cap->desc);
	/* Mark this CPU absent */
	set_cpu_present(cpu, 0);

	/* Check if we can park ourselves */
	if (cpu_ops[cpu] && cpu_ops[cpu]->cpu_die)
		cpu_ops[cpu]->cpu_die(cpu);
	asm(
	"1:	wfe\n"
	"	wfi\n"
	"	b	1b");
}

static void
verify_local_cpu_features(const struct arm64_cpu_capabilities *caps)
{
		caps = arm64_features;
		for (; caps->desc; caps++) {
		if (!cpus_have_cap(caps->capability))
			continue;
		/*
		* If the new CPU misses an advertised feature, we cannot proceed
		* further, park the cpu.
		*/
		if (!caps->matches(caps, SCOPE_LOCAL_CPU))
			fail_incapable_cpu("arm64_features", caps);
		if (caps->enable)
			caps->enable((void *)caps);
	}
}

/*
 * Run through the enabled system capabilities and enable() it on this CPU.
 * The capabilities were decided based on the available CPUs at the boot time.
 * Any new CPU should match the system wide status of the capability. If the
 * new CPU doesn't have a capability which the system now has enabled, we
 * cannot do anything to fix it up and could cause unexpected failures. So
 * we park the CPU.
 */
void verify_local_cpu_capabilities(void)
{

	/*
	 * If we haven't computed the system capabilities, there is nothing
	 * to verify.
	 */
	if (!sys_caps_initialised)
		return;

	verify_local_cpu_errata();
	verify_local_cpu_features(arm64_features);
}

#else	/* !CONFIG_HOTPLUG_CPU */

static inline void set_sys_caps_initialised(void)
{
}

#endif	/* CONFIG_HOTPLUG_CPU */

static void setup_feature_capabilities(void)
{
	update_cpu_capabilities(arm64_features, "detected feature:");
	enable_cpu_capabilities(arm64_features);
}

/*
 * Check if the current CPU has a given feature capability.
 * Should be called from non-preemptible context.
 */
bool this_cpu_has_cap(unsigned int cap)
{
	const struct arm64_cpu_capabilities *caps;

	if (WARN_ON(preemptible()))
		return false;

	for (caps = arm64_features; caps->matches; caps++)
		if (caps->capability == cap)
			return caps->matches(caps, SCOPE_LOCAL_CPU);

	return false;
}

void __init setup_cpu_features(void)
{
	u64 features, block;

	/* Set the CPU feature capabilies */
	setup_feature_capabilities();

	/* Advertise that we have computed the system capabilities */
	set_sys_caps_initialised();

	/*
	* ID_AA64ISAR0_EL1 contains 4-bit wide signed feature blocks.
	* The blocks we test below represent incremental functionality
	* for non-negative values. Negative values are reserved.
	*/
	features = read_cpuid(ID_AA64ISAR0_EL1);
	block = (features >> 4) & 0xf;
	if (!(block & 0x8)) {
		switch (block) {
		default:
		case 2:
			elf_hwcap |= HWCAP_PMULL;
		case 1:
			elf_hwcap |= HWCAP_AES;
		case 0:
			break;
		}
	}

	block = (features >> 8) & 0xf;
	if (block && !(block & 0x8))
		elf_hwcap |= HWCAP_SHA1;

	block = (features >> 12) & 0xf;
	if (block && !(block & 0x8))
		elf_hwcap |= HWCAP_SHA2;

	block = (features >> 16) & 0xf;
	if (block && !(block & 0x8))
		elf_hwcap |= HWCAP_CRC32;

#ifdef CONFIG_COMPAT
	/*
	* ID_ISAR5_EL1 carries similar information as above, but pertaining to
	* the Aarch32 32-bit execution state.
	*/
	features = read_cpuid(ID_ISAR5_EL1);
	block = (features >> 4) & 0xf;
	if (!(block & 0x8)) {
		switch (block) {
		default:
		case 2:
			compat_elf_hwcap2 |= COMPAT_HWCAP2_PMULL;
		case 1:
			compat_elf_hwcap2 |= COMPAT_HWCAP2_AES;
		case 0:
		break;
		}
	}

	block = (features >> 8) & 0xf;
	if (block && !(block & 0x8))
		compat_elf_hwcap2 |= COMPAT_HWCAP2_SHA1;

	block = (features >> 12) & 0xf;
	if (block && !(block & 0x8))
		compat_elf_hwcap2 |= COMPAT_HWCAP2_SHA2;

	block = (features >> 16) & 0xf;
	if (block && !(block & 0x8))
		compat_elf_hwcap2 |= COMPAT_HWCAP2_CRC32;
#endif
}
