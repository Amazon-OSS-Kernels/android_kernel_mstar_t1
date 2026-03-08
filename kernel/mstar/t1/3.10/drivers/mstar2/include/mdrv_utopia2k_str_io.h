/**
 * Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
 * This program is free software. You can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

/*

 @file   mdrv_utopia2k_str_io.h
 @brief  UTOPIA2K STR interface
 @author MStar Semiconductor Inc.

*/

#ifndef __DRV_UTOPIA2K_STR_IO_H__
#define __DRV_UTOPIA2K_STR_IO_H__

/*
  Header Files
*/
#include "MsTypes.h"

/*
  Define & data type
*/

#define WAKUP_ANDROID_FROM_KERNEL 1

#if WAKUP_ANDROID_FROM_KERNEL
/* From PM51 */
#define WKUP_SRC_NONE    0x00
#define WKUP_SRC_IR    0x01
#define WKUP_SRC_DVI    0x02
#define WKUP_SRC_DVI2    0x03
#define WKUP_SRC_CEC    0x04
#define WKUP_SRC_SAR    0x05
#define WKUP_SRC_ESYNC    0x06
#define WKUP_SRC_SYNC    0x07
#define WKUP_SRC_RTC    0x08
#define WKUP_SRC_RTC2    0x09
#define WKUP_SRC_AVLINK    0x0a
#define WKUP_SRC_UART    0x0b
#define WKUP_SRC_GPIO    0x0c
#define WKUP_SRC_MHL    0x0d
#define WKUP_SRC_WOL    0x0e
#define WKUP_SRC_MAX    0x0f

/* Define unused wakeup source as bluetooth and wifi, but you need
     cooperation with PM51 owner, here's some examples:
*/
#define WKUP_SRC_BT    WKUP_SRC_DVI
#define WKUP_SRC_WIFI    WKUP_SRC_DVI2

#define KEYPAD_POWER_CODE  0x1E
#define IR_POWER_CODE 0x46
#define IR_CUSTOM_1_CODE 0x5F
#define IR_CUSTOM_2_CODE_SEIKI 0x5E   /* Toon Goggles */
#define IR_CUSTOM_3_CODE_SEIKI 0x3B   /* AccuWeather */
#define IR_CUSTOM_2_CODE_PEPPER 0x8C
#define IR_CUSTOM_3_CODE_PEPPER 0x8D

#define PM_REG_BASE             (0x0700 * 2)
#define MCU_REG_BASE            (0x0800 * 2)
#define REG_PM_DUMMY_WAKEUP_SOURCE             ((PM_REG_BASE + 0x39 * 2))
#define WAKEUP_SOURCE  \
	(*((unsigned char *)(mstar_pm_base + \
	((REG_PM_DUMMY_WAKEUP_SOURCE)<<1))))

#define REG_MCU_DUMMY_POWERON_KEY             ((MCU_REG_BASE + (0x53 * 2)))
#define POWERON_KEY  \
	(*((unsigned char *)(mstar_pm_base + \
	((REG_MCU_DUMMY_POWERON_KEY)<<1)+1)))

#endif

enum utopia2k_str_power_mode {
	UTOPIA2K_STR_POWER_SUSPEND = 1,
	UTOPIA2K_STR_POWER_RESUME = 2,
	UTOPIA2K_STR_POWER_MAX,
};

typedef int (*f_utopia_str)(int u32_power_state, void *p_module);

extern ptrdiff_t mstar_pm_base;
/*
  Extern Function
*/
int mdrv_utopia2k_str_setup_function_ptr(void *p_module,
	f_utopia_str fp_str);
int mdrv_utopia2k_str_wait_condition(const char *name,
	MS_U32 mode, MS_U32 stage);
int mdrv_utopia2k_str_send_condition(const char *name,
	MS_U32 mode, MS_U32 stage);

/*
 * ioctl operations
 */
#include <linux/ioctl.h>

#define UTOPIA2K_STR_IOC_MAGIC   'U'

#define UTOPIA2K_STR_IOC_SET_WKUP_SRC    \
	_IOW(UTOPIA2K_STR_IOC_MAGIC, 0x00, unsigned short)
#define UTOPIA2K_STR_IOC_GET_WKUP_SRC    \
	_IOR(UTOPIA2K_STR_IOC_MAGIC, 0x01, unsigned short)
#define UTOPIA2K_STR_IOC_REPORT_EVENT    \
	_IO(UTOPIA2K_STR_IOC_MAGIC, 0x02)
#endif

