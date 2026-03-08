/*
 * CPU kernel entry/exit control
 *
 * Copyright (C) 2013 ARM Ltd.
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

#include <asm/cpu_ops.h>
#include <asm/smp_plat.h>
#include <linux/errno.h>
#include <linux/of.h>
#include <linux/string.h>

#ifdef CONFIG_MP_TRUSTZONE_PATCH
#include <asm/cacheflush.h>
#include <asm/psci.h>
#include <linux/slab.h>
#include <linux/psci.h>
#endif

extern const struct cpu_operations smp_spin_table_ops;
extern const struct cpu_operations cpu_psci_ops;
#ifdef CONFIG_MP_PLATFORM_ARM_64bit_PORTING
extern const struct smp_enable_ops mstar_smp_spin_table;
#endif

const struct cpu_operations *cpu_ops[NR_CPUS];

static const struct cpu_operations *supported_cpu_ops[] __initconst = {
#ifdef CONFIG_SMP
#if defined(CONFIG_MP_PLATFORM_ARM_64bit_PORTING)
        &mstar_smp_spin_table,
#endif
	&smp_spin_table_ops,
#endif
	&cpu_psci_ops,
	NULL,
};

static const struct cpu_operations * __init cpu_get_ops(const char *name)
{
	const struct cpu_operations **ops = supported_cpu_ops;

	while (*ops) {
		if (!strcmp(name, (*ops)->name))
			return *ops;

		ops++;
	}

	return NULL;
}

/*
 * Read a cpu's enable method from the device tree and record it in cpu_ops.
 */
#ifdef CONFIG_MP_TRUSTZONE_PATCH
uint32_t isPSCI = PSCI_RET_NOT_SUPPORTED;
#endif
int __init cpu_read_ops(struct device_node *dn, int cpu)
{
#ifdef CONFIG_MP_TRUSTZONE_PATCH
	uint64_t BA;

	isPSCI = PSCI_RET_NOT_SUPPORTED;
	pr_debug("\033[0;33;31m [CPU_OPS] %s %d %s\033[m\n", __func__, __LINE__, UTOPIA_MODE);
	if (strncmp(UTOPIA_MODE, "optee", 5) == 0) {
		BA = virt_to_phys((uint32_t *)&isPSCI);

		__flush_dcache_area((uint32_t *)&isPSCI, sizeof(uint32_t));
		__asm__ __volatile__(
			"ldr x0,=0xb200585b\n\t"
			"mov x1,%0\n\t"
			"smc #0\n\t"
			:
			: "r"(BA)
			: "x0", "x1"
		);
		__flush_dcache_area((uint32_t *)&isPSCI, sizeof(uint32_t));
		pr_debug("\033[0;33;31m [CPU_OPS] %s %d %x\033[m\n", __func__, __LINE__, isPSCI);
	}
	if (PSCI_RET_SUCCESS == isPSCI) {
		_ms_psci_ops_set();
		cpu_ops[cpu] = cpu_get_ops("psci");
		return 0;
	}
#endif
	const char *enable_method = of_get_property(dn, "enable-method", NULL);
	if (!enable_method) {
		/*
		 * The boot CPU may not have an enable method (e.g. when
		 * spin-table is used for secondaries). Don't warn spuriously.
		 */
		if (cpu != 0)
			pr_err("%s: missing enable-method property\n",
				dn->full_name);
		return -ENOENT;
	}

	cpu_ops[cpu] = cpu_get_ops(enable_method);
	if (!cpu_ops[cpu]) {
		pr_warn("%s: unsupported enable-method property: %s\n",
			dn->full_name, enable_method);
		return -EOPNOTSUPP;
	}

	return 0;
}

void __init cpu_read_bootcpu_ops(void)
{
	struct device_node *dn = NULL;
	u64 mpidr = cpu_logical_map(0);

	while ((dn = of_find_node_by_type(dn, "cpu"))) {
		u64 hwid;
		const __be32 *prop;

		prop = of_get_property(dn, "reg", NULL);
		if (!prop)
			continue;

		hwid = of_read_number(prop, of_n_addr_cells(dn));
		if (hwid == mpidr) {
			cpu_read_ops(dn, 0);
			of_node_put(dn);
			return;
		}
	}
}
