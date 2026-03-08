/*
 *
 * Copyright (C) 2016 Amazon Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.

 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/memblock.h>
#include <linux/pstore_ram.h>

#define mstar_ramoops_ADDR		(UL(0x22000000))
/*
 * The pstore rounds down the separate sizes and the total size to be power of
 * two, so we can't simply add the sizes together to get the total size.
 */
#define AMZN_RAM_TOTAL_SIZE			(SZ_1M)
#define AMZN_RAM_OOPS_RECORD_SIZE	(SZ_128K)
#define AMZN_RAM_FTRACE_SIZE		(SZ_128K)
#define AMZN_RAM_PMSG_SIZE			(SZ_128K)
#define AMZN_RAM_CONSOLE_SIZE		(SZ_128K)
#ifdef CONFIG_PSTORE_TVMSG
#define AMZN_RAM_TVMSG_SIZE			(SZ_16K)
#endif

static struct ramoops_platform_data ramoops_data = {
	.mem_size		= AMZN_RAM_TOTAL_SIZE,
	.mem_address	= mstar_ramoops_ADDR,
	.record_size	= AMZN_RAM_OOPS_RECORD_SIZE,
	.console_size	= AMZN_RAM_CONSOLE_SIZE,
	.ftrace_size	= AMZN_RAM_FTRACE_SIZE,
	.pmsg_size		= AMZN_RAM_PMSG_SIZE,
#ifdef CONFIG_PSTORE_TVMSG
	.tvmsg_size		= AMZN_RAM_TVMSG_SIZE,
#endif
	.dump_oops		= 1,
	.ecc_info		= {
		.ecc_size	= 16,
	},
};


static struct platform_device ramoops_dev = {
	.name = "ramoops",
	.dev = {
		.platform_data = &ramoops_data,
	},
};

static __initdata bool mstar_ramoops_inited;

/* device_initcall to register ramconsole device */
static int __init mstar_ramoops_register(void)
{
	int ret;

	if (!mstar_ramoops_inited)
		return -ENODEV;

	ret = platform_device_register(&ramoops_dev);
	if (ret) {
		pr_err("%s: unable to register ram console device:"
			"start=0x%08x, size=0x%08x, ret=%d\n",
			__func__, (u32)ramoops_data.mem_address,
			(u32)ramoops_data.mem_size, ret);
		return ret;
	}

	memblock_add(ramoops_data.mem_address,
		ramoops_data.mem_size);

	return 0;
}
device_initcall(mstar_ramoops_register);

/* setup the ram console device for pstore. */
int __init mstar_ramoops_init(void)
{
	int ret;

	pr_debug("mstar_ramoops_init\n");

	/* call trace_printk once to trigger tracer_alloc_buffers to alloc printk buffer */
	trace_printk("mstar_ramoops_init\n");

	ret = memblock_reserve(ramoops_data.mem_address, ramoops_data.mem_size);
	if (ret) {
		pr_err("Unable to reserve memory for ramoops.\n");
		return ret;
	} else {
		mstar_ramoops_inited = true;
	}

	return ret;
}
EXPORT_SYMBOL(mstar_ramoops_init);
