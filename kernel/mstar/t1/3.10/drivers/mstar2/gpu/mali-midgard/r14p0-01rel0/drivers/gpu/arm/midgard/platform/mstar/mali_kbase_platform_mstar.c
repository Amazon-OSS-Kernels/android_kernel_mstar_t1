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

#include <mali_kbase.h>
#include "mali_kbase_platform_mstar.h"

/* Platform functions */
int mstar_platform_init(struct kbase_device *kbdev)
{
    KBASE_DEBUG_ASSERT(NULL != kbdev);

    init_registers();

#ifdef CONFIG_MALI_MIDGARD_DVFS
    {
        int err = dvfs_init(kbdev);

        if (0 != err)
        {
            return err;
        }
    }
#endif

#ifdef MSTAR_DISABLE_SHADER_CORES
    kbdev->get_num_disabled_cores = get_num_disabled_cores;
#endif

    return 0;
}

void mstar_platform_term(struct kbase_device *kbdev)
{
#ifdef CONFIG_MALI_MIDGARD_DVFS
    dvfs_term(kbdev);
#endif
}

/* PM callbacks */
void mstar_pm_off(struct kbase_device *kbdev)
{
    power_off();
}

int mstar_pm_on(struct kbase_device *kbdev)
{
    power_on();

    /* If the GPU state has been lost then this function must return 1, otherwise it should return 0. */
    return 1; /* TODO: check if the GPU state has been lost */
}

void mstar_pm_suspend(struct kbase_device *kbdev)
{
    power_off();
}

void mstar_pm_resume(struct kbase_device *kbdev)
{
    init_registers();
    power_on();
}

#ifdef CONFIG_MALI_MIDGARD_DVFS

/* XXX: these two values are copied from mali_kbase_pm_metrics.c */
#define KBASE_PM_NO_VSYNC_MIN_UTILISATION 10
#define KBASE_PM_NO_VSYNC_MAX_UTILISATION 40

int kbase_platform_dvfs_event(struct kbase_device *kbdev,
                              u32 utilisation,
                              u32 util_gl_share,
                              u32 util_cl_share[2])
{
    KBASE_DEBUG_ASSERT(NULL != kbdev);

    /* implementation is copied from r6p0-02rel0 */
    if (kbdev->dvfs.enabled)
    {
        if (utilisation < KBASE_PM_NO_VSYNC_MIN_UTILISATION)
        {
            if (NULL != kbdev->dvfs.clock_down)
            {
                kbdev->dvfs.clock_down(kbdev);
            }
        }
        else if (utilisation > KBASE_PM_NO_VSYNC_MAX_UTILISATION)
        {
            if (NULL != kbdev->dvfs.clock_up)
            {
                kbdev->dvfs.clock_up(kbdev);
            }
        }
        else
        {
            /* nop */
        }
    }

    return 0;
}

#endif /* CONFIG_MALI_MIDGARD_DVFS */
