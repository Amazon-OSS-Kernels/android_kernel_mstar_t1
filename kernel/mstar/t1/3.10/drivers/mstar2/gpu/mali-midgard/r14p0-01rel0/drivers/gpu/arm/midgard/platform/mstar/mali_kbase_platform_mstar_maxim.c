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

#include <linux/delay.h>
#include "mstar/mstar_chip.h"

/* REG_GPU */
#define REG_GPU_BASE                0x110800

#define REG_GPU_SOFT_RESET          0x00
#define MSK_GPU_SOFT_RESET          0x0001

#define REG_GPU_SRAM_SD_EN          0x43
#define MSK_GPU_SRAM_SD_EN          0x0008

#define REG_GPU_RREQ_RD_LEN         0x61
#define MSK_GPU_RREQ_RD_LEN         0x00e0
#define SHIFT_GPU_RREQ_RD_LEN       5
#define VAL_GPU_RREQ_RD_LEN_48      0x0
#define VAL_GPU_RREQ_RD_LEN_56      0x1
#define VAL_GPU_RREQ_RD_LEN_64      0x2
#define VAL_GPU_RREQ_RD_LEN_68      0x3
#define VAL_GPU_RREQ_RD_LEN_72      0x4
#define VAL_GPU_RREQ_RD_LEN_76      0x5
#define VAL_GPU_RREQ_RD_LEN_80      0x6
#define VAL_GPU_RREQ_RD_LEN_84      0x7

#define REG_GPU_RIU_EN              0x6a
#define MSK_GPU_RIU_EN              0x0001

#define REG_GPU_MIU2_START          0x6b
#define REG_GPU_MIU2_END            0x6c
#define REG_GPU_MIU0_START          0x75
#define REG_GPU_MIU0_END            0x76
#define REG_GPU_MIU1_START          0x77
#define REG_GPU_MIU1_END            0x78

#ifdef MSTAR_DISABLE_SHADER_CORES
#define REG_GPU_DISABLE_CORES       0x7c
#define MSK_GPU_DISABLE_CORES       0x0001
#endif

/* REG_GPU_PLL */
#define REG_GPU_PLL_BASE            0x160f00

#define REG_G3DPLL                  0x11
#define MSK_G3DPLL                  0xff00

#define REG_SET_0                   0x60
#define REG_SET_1                   0x61

#define REG_SLD                     0x62
#define MSK_SLD                     0x0001

/* REG_CLKGEN1 */
#define REG_CLKGEN1_BASE            0x103300

#define REG_CKG_GPU                 0x20
#define MSK_DISABLE_CLK             0x0001

#define REG_MIU0_BASE               0x101200
#define REG_MIU1_BASE               0x100600
#define REG_MIU2_BASE               0x162000

/* REG_MIU_LEN is set by MIU driver as is:
 * 0:for test, 1:2MB, 2:4MB, 3:8MB,
 * 4:16MB,  5:32MB,  6:64MB, 7:128MB,
 * 8:256MB, 9:512MB, A:1GB,  B:2GB
 */
#define REG_MIU_LEN                 0x69
#define MSK_MIU_LEN                 0xf000
#define SHIFT_MIU_LEN               12

/* RIU mapping */
#define GPU_REG(addr)               RIU[(REG_GPU_BASE) + ((addr) << 1)]
#define GPU_PLL_REG(addr)           RIU[(REG_GPU_PLL_BASE) + ((addr) << 1)]
#define CLKGEN1_REG(addr)           RIU[(REG_CLKGEN1_BASE) + ((addr) << 1)]
#define MIU0_REG(addr)              RIU[(REG_MIU0_BASE) + ((addr) << 1)]
#define MIU1_REG(addr)              RIU[(REG_MIU1_BASE) + ((addr) << 1)]
#define MIU2_REG(addr)              RIU[(REG_MIU2_BASE) + ((addr) << 1)]

/* Platform functions */
void init_registers(void)
{
#define MHZ_TO_REG(MHZ) (0x6c000000 / (MHZ))

    unsigned int clk_reg = MHZ_TO_REG(MALI_MAX_FREQ);

    /* Set GPU clock */
    GPU_PLL_REG(REG_SET_0) = clk_reg & 0xffff;
    GPU_PLL_REG(REG_SET_1) = (clk_reg >> 16) & 0xffff;
    GPU_PLL_REG(REG_SLD) |= MSK_SLD;
    udelay(10);

    /* Set MIU addr */
    GPU_REG(REG_GPU_MIU0_START) = (MSTAR_MIU0_BUS_BASE >> 20) & 0xFFFF;
    GPU_REG(REG_GPU_MIU0_END)   = ((MSTAR_MIU0_BUS_BASE >> 20) & 0xFFFF) + (1 << (MIU0_REG(REG_MIU_LEN) >> SHIFT_MIU_LEN));
    GPU_REG(REG_GPU_MIU1_START) = (MSTAR_MIU1_BUS_BASE >> 20) & 0xFFFF;
    GPU_REG(REG_GPU_MIU1_END)   = ((MSTAR_MIU1_BUS_BASE >> 20) & 0xFFFF )+ (1 << (MIU1_REG(REG_MIU_LEN) >> SHIFT_MIU_LEN));
    GPU_REG(REG_GPU_MIU2_START) = 0xFFFF; /* no MIU2 */
    GPU_REG(REG_GPU_MIU2_END)   = 0xFFFF;

    /* Enable RIU */
#ifdef MSTAR_RIU_ENABLED
    GPU_REG(REG_GPU_RIU_EN) |= MSK_GPU_RIU_EN;
#endif

    /* Set read request length to 64 */
    GPU_REG(REG_GPU_RREQ_RD_LEN) &= ~MSK_GPU_RREQ_RD_LEN;
    GPU_REG(REG_GPU_RREQ_RD_LEN) |= (VAL_GPU_RREQ_RD_LEN_64 << SHIFT_GPU_RREQ_RD_LEN);

    /* Power on G3DPLL */
    GPU_PLL_REG(REG_G3DPLL) &= ~MSK_G3DPLL;
    udelay(1);
}

void power_on(void)
{
    GPU_REG(REG_GPU_SRAM_SD_EN) &= ~MSK_GPU_SRAM_SD_EN; /* enable SRAM */
    CLKGEN1_REG(REG_CKG_GPU)    &= ~MSK_DISABLE_CLK;    /* enable GPU clock */

    /* reset: 0 -> 1 -> 0 */
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    GPU_REG(REG_GPU_SOFT_RESET) |= MSK_GPU_SOFT_RESET;
    udelay(1); /* delay for runtime suspend */
    GPU_REG(REG_GPU_SOFT_RESET) &= ~MSK_GPU_SOFT_RESET;
    udelay(1);
}

void power_off(void)
{
    GPU_REG(REG_GPU_SOFT_RESET) |= MSK_GPU_SOFT_RESET;  /* reset GPU */
    CLKGEN1_REG(REG_CKG_GPU)    |= MSK_DISABLE_CLK;     /* disable GPU clock */
    GPU_REG(REG_GPU_SRAM_SD_EN) |= MSK_GPU_SRAM_SD_EN;  /* disable SRAM */
    udelay(1);
}

#ifdef MSTAR_DISABLE_SHADER_CORES
int get_num_disabled_cores(void)
{
    return GPU_REG(REG_GPU_DISABLE_CORES) & MSK_GPU_DISABLE_CORES;
}
#endif
