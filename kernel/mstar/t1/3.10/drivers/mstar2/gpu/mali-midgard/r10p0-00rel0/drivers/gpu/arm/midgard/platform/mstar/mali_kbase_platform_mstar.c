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
#include <mali_kbase.h>

#ifdef CONFIG_MALI_MIDGARD_DVFS

/* XXX: these two values are copied from mali_kbase_pm_metrics.c */
#define KBASE_PM_NO_VSYNC_MIN_UTILISATION 10
#define KBASE_PM_NO_VSYNC_MAX_UTILISATION 40

int kbase_platform_dvfs_event(struct kbase_device* kbdev,
                              u32 utilisation,
                              u32 util_gl_share,
                              u32 util_cl_share[2])
{
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