/*
 *
 * Copyright (C) 2016 Amazon Incorporated
 *
 * Haesik Kim <haesikim@lab126.com>
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
#include <linux/rfkill-gpio.h>

static struct rfkill_gpio_platform_data bt_rfkill_platform_data = {
	.name           = "bt_rfkill",
	.reset_gpio     = 18,
	.shutdown_gpio  = -1,
	.type   = 2,
};

struct platform_device mstar_rfkill_device = {
	.name   = "bluetooth_rfkill",
	.id     = -1,
	.dev        = {
		.platform_data = &bt_rfkill_platform_data,
	},
};


static int __init platform_bt_rfkill_init(void)
{
	pr_err("Registering platform bt rfkill\n");
	platform_device_register(&mstar_rfkill_device);
	return 0;
}

device_initcall(platform_bt_rfkill_init);
