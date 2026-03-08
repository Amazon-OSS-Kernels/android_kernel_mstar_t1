/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

int mstar_platform_dvfs_init(struct kbase_device* kbdev);
void mstar_platform_dvfs_term(struct kbase_device* kbdev);

#endif /* _KBASE_PLATFORM_MSTAR_H_ */
