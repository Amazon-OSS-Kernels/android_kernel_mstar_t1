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

#ifdef CONFIG_MALI_MIDGARD_DVFS

#include <mali_kbase.h>
#include "mali_kbase_platform_mstar.h"

#include <linux/delay.h>

/* REG_GPU */
#define REG_GPU_PLL_BASE    0x160f00
#define REG_SET_0           0x60
#define REG_SET_1           0x61
#define REG_SLD             0x62
#define MSK_SLD             0x0001

/* RIU mapping */
#define GPU_PLL_REG(addr)   RIU[(REG_GPU_PLL_BASE) + ((addr) << 1)]

/* */
#define MHZ_TO_REG(MHZ)     (0x6c000000 / (MHZ))
#define ADJUST_STEP         50 /* in MHz */

extern int mali_gpu_clock;

/* Helper functions */
static void update_frequency(u32 frequency)
{
    mali_gpu_clock = frequency;

    GPU_PLL_REG(REG_SET_0) = MHZ_TO_REG(frequency) & 0xffff;
    GPU_PLL_REG(REG_SET_1) = (MHZ_TO_REG(frequency) >> 16) & 0xffff;
    GPU_PLL_REG(REG_SLD) |= MSK_SLD;
    udelay(1);
}

/* DVFS callbacks */
static bool dvfs_enable(struct kbase_device* kbdev)
{
    KBASE_DEBUG_ASSERT(NULL != kbdev);

    kbdev->dvfs.current_frequency = kbdev->dvfs.dvfs_max_freq;
    kbdev->dvfs.enabled = 1;

    return true;
}

static bool dvfs_disable(struct kbase_device* kbdev)
{
    KBASE_DEBUG_ASSERT(NULL != kbdev);

    kbdev->dvfs.current_frequency = kbdev->dvfs.dvfs_max_freq; /* restore to the max frequency */
    kbdev->dvfs.enabled = 0;
    update_frequency(kbdev->dvfs.current_frequency);

    return true;
}

static bool dvfs_clock_up(struct kbase_device* kbdev)
{
    u32 new_freq;

    KBASE_DEBUG_ASSERT(NULL != kbdev);

    if (kbdev->dvfs.current_frequency >= kbdev->dvfs.dvfs_max_freq)
    {
        return true;
    }

    new_freq = kbdev->dvfs.current_frequency + ADJUST_STEP;

    if (new_freq > kbdev->dvfs.dvfs_max_freq)
    {
        new_freq = kbdev->dvfs.dvfs_max_freq;
    }
    else if (new_freq < kbdev->dvfs.dvfs_min_freq)
    {
        new_freq = kbdev->dvfs.dvfs_min_freq;
    }

    kbdev->dvfs.current_frequency = new_freq;
    update_frequency(kbdev->dvfs.current_frequency);

    return true;
}

static bool dvfs_clock_down(struct kbase_device* kbdev)
{
    u32 new_freq;

    KBASE_DEBUG_ASSERT(NULL != kbdev);

    if (kbdev->dvfs.current_frequency <= kbdev->dvfs.dvfs_min_freq)
    {
        return true;
    }

    new_freq = (kbdev->dvfs.current_frequency >= ADJUST_STEP) ? (kbdev->dvfs.current_frequency - ADJUST_STEP) : 0;

    if (new_freq < kbdev->dvfs.dvfs_min_freq)
    {
        new_freq = kbdev->dvfs.dvfs_min_freq;
    }
    else if (new_freq > kbdev->dvfs.dvfs_max_freq)
    {
        new_freq = kbdev->dvfs.dvfs_max_freq;
    }

    kbdev->dvfs.current_frequency = new_freq;
    update_frequency(kbdev->dvfs.current_frequency);

    return true;
}

static bool dvfs_enable_boost(struct kbase_device* kbdev)
{
    if (!kbdev)
    {
        return false;
    }

    kbdev->dvfs.boost_enabled= 1;
    kbdev->dvfs.dvfs_max_freq = MALI_BOOST_FREQ;

    return true;
}

static bool dvfs_disable_boost(struct kbase_device* kbdev)
{
    if (!kbdev)
    {
        return false;
    }

    kbdev->dvfs.boost_enabled = 0;
    kbdev->dvfs.dvfs_max_freq = MALI_MAX_FREQ;

    return true;
}

/* Platform functions */
int dvfs_init(struct kbase_device* kbdev)
{
    KBASE_DEBUG_ASSERT(NULL != kbdev);

    memset(&kbdev->dvfs, 0, sizeof(kbdev->dvfs));

    kbdev->dvfs.dvfs_max_freq = MALI_MAX_FREQ;
    kbdev->dvfs.dvfs_min_freq = MALI_MIN_FREQ;

    kbdev->dvfs.enabled = 0;
    kbdev->dvfs.enable = dvfs_enable;
    kbdev->dvfs.disable = dvfs_disable;
    kbdev->dvfs.clock_up = dvfs_clock_up;
    kbdev->dvfs.clock_down = dvfs_clock_down;
    kbdev->dvfs.enable_boost = dvfs_enable_boost;
    kbdev->dvfs.disable_boost = dvfs_disable_boost;

    /* enable after init */
    dvfs_enable(kbdev);

    return 0;
}

void dvfs_term(struct kbase_device* kbdev)
{

}

#endif /* CONFIG_MALI_MIDGARD_DVFS */
