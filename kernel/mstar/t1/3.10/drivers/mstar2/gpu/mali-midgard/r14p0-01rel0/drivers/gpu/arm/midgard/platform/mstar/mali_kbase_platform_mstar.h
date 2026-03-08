/*
 *
 * (C) COPYRIGHT 2014-2016 MStar Semiconductor, Inc. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */

#ifndef _KBASE_PLATFORM_MSTAR_H_
#define _KBASE_PLATFORM_MSTAR_H_

#include <mali_kbase_defs.h>

/* RIU */
#ifdef CONFIG_ARM64
#define MSTAR_RIU_BASE mstar_pm_base
extern ptrdiff_t mstar_pm_base;
#else
#define MSTAR_RIU_BASE 0xfd000000
#endif

#define RIU ((volatile unsigned short*)(MSTAR_RIU_BASE))

/* Platform and PM Callbacks */
int mstar_platform_init(struct kbase_device* kbdev);
void mstar_platform_term(struct kbase_device* kbdev);
void mstar_pm_off(struct kbase_device* kbdev);
int mstar_pm_on(struct kbase_device* kbdev);
void mstar_pm_suspend(struct kbase_device* kbdev);
void mstar_pm_resume(struct kbase_device* kbdev);

/* Internal platform functions */
extern void init_registers(void);
extern void power_on(void);
extern void power_off(void);
extern int dvfs_init(struct kbase_device* kbdev);
extern void dvfs_term(struct kbase_device* kbdev);
#ifdef MSTAR_DISABLE_SHADER_CORES
extern int get_num_disabled_cores(void);
#endif

#endif /* _KBASE_PLATFORM_MSTAR_H_ */
