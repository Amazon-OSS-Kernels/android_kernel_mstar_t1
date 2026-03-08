/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>
#include <linux/kernel.h>
#else
#include <stdio.h>
#endif
#ifdef CONFIG_MSTAR_CLKM
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/clkm.h>
#else
#include "drvCLKM.h"
#endif
#endif


#include "MsCommon.h"
#include "gpd_reg.h"

MS_VIRT GPD_REG_BASE;

const MS_U16 gpd_reg_addr[] =
{
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x0 ,
0x1 ,
0x1 ,
0x2 ,
0x3 ,
0x4 ,
0x5 ,
0x6 ,
0x8 ,
0xa ,
0xc ,
0xe ,
0xf ,
0x10,
0x12,
0x13,
0x14,
0x15,
0x15,
0x15,
0x16,
0x17,
0x18,
0x18,
0x18,
0x18,
0x18,
0x18,
0x19,
0x19,
0x1a,
0x1a,
0x1a,
0x1a,
0x1a,
0x1a,
0x1a,
0x1a,
0x1b,
0x1b,
0x1b,
0x1b,
0x1c,
0x1c,
0x1d,
0x1e,
0x1e,
0x20,
0x20,
0x22,
0x22,
0x23,
0x23,
0x23,
0x23,
0x24,
0x24,
0x24,
0x25,
0x26,
0x26,
0x28,
0x28,
0x2a,
0x2a,
0x2c,
0x2c,
0x2d,
0x2e,
0x2e,
0x2f,
0x30,
0x30,
0x31,
0x32,
0x32,
0x33,
0x34,
0x34,
0x35,
0x35,
0x36,
0x36,
0x36,
0x36,
0x37,
0x37,
0x38,
0x38,
0x38,
0x38,
0x39,
0x39,
0x3a,
0x3b,
0x3c,
0x3d,
0x3e,
0x3f,
0x40,
0x41,
0x42,
0x43,
0x44,
0x45,
0x46,
0x47,
0x48,
0x48,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4b,
0x4c,
0x4e,
0x4f,
0x4f,
0x4f,
0x50,
0x51,
0x51,
0x51,
0x52,
0x53,
0x53,
0x53,
0x53,
0x53,
0x54,
0x55,
0x55,
0x55,
0x55,
0x56,
0x58,
0x5a,
0x5c,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5f,
0x5f,
0x5f,
0x5f,
0x5f,
0x5f,
0x5f,
0x60,
0x60,
0x60,
0x60,
0x60,
0x60,
0x61,
0x61,
0x61,
0x61,
0x61,
0x62,
0x64,
0x65,
0x66,
0x66,
0x67,
0x69,
0x6b,
0x6b,
0x6b,
0x6b,
0x6c,
0x6e,
0x70,
0x70
};

const MS_U8 gpd_reg_msb[] =
{
0 ,
1 ,
2 ,
3 ,
4 ,
5 ,
8 ,
9 ,
10,
11,
12,
13,
14,
19,
9,
15,
15,
14,
15,
15,
30,
30,
30,
31,
8 ,
15,
15,
15,
15,
15,
0,
8,
15,
15,
15,
0 ,
1 ,
3 ,
6 ,
10,
15,
5,
12,
0 ,
2 ,
5 ,
6 ,
7 ,
8 ,
9 ,
15,
8,
9,
14,
15,
7 ,
16,
10,
10,
22,
12,
26,
14,
15,
1,
4,
8,
13,
5 ,
12,
20,
13,
9 ,
20,
11,
24,
13,
28,
8 ,
17,
10,
8 ,
17,
10,
8 ,
17,
10,
8 ,
17,
10,
8 ,
17,
6,
11,
4 ,
9 ,
14,
19,
8,
13,
4 ,
9 ,
14,
19,
8,
13,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
14,
29,
0 ,
1 ,
4 ,
5 ,
8 ,
10,
13,
14,
15,
15,
31,
23,
13,
14,
15,
25,
10,
14,
15,
25,
10,
11,
12,
14,
15,
25,
10,
12,
13,
14,
31,
31,
27,
27,
0 ,
1 ,
2 ,
3 ,
4 ,
5 ,
6 ,
7 ,
8 ,
9 ,
10,
11,
12,
13,
14,
15,
0,
1,
2,
3,
4,
5,
6,
4,
9 ,
12,
13,
14,
15,
4,
9,
11,
13,
15,
27,
27,
13,
5 ,
15,
31,
17,
9,
12,
13,
14,
23,
31,
7,
15
};

const MS_U8 gpd_reg_lsb[] =
{
0 ,
1 ,
2 ,
3 ,
4 ,
5 ,
6 ,
9 ,
10,
11,
12,
13,
14,
15,
4,
10,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0,
0,
0,
0,
1,
9,
0,
0,
0 ,
1 ,
2 ,
4 ,
7 ,
11,
0,
6,
0 ,
1 ,
3 ,
6 ,
7 ,
8 ,
9 ,
10,
0,
9,
10,
15,
0 ,
8 ,
1,
0 ,
11,
0 ,
13,
0 ,
15,
0,
2,
5,
9,
0 ,
6 ,
13,
5,
0 ,
10,
0 ,
12,
0 ,
14,
0 ,
9 ,
2,
0 ,
9 ,
2,
0 ,
9 ,
2,
0 ,
9 ,
2,
0 ,
9 ,
2,
7,
0 ,
5 ,
10,
15,
4,
9,
0 ,
5 ,
10,
15,
4,
9,
0,
0,
0,
0,
0,
0,
0,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
15,
0 ,
1 ,
2 ,
5 ,
6 ,
9 ,
11,
14,
15,
0 ,
0 ,
0 ,
8,
14,
15,
0,
10,
11,
15,
0,
10,
11,
12,
13,
15,
0,
10,
11,
13,
14,
0 ,
0 ,
0 ,
0 ,
0 ,
1 ,
2 ,
3 ,
4 ,
5 ,
6 ,
7 ,
8 ,
9 ,
10,
11,
12,
13,
14,
15,
0,
1,
2,
3,
4,
5,
6,
0,
5 ,
10,
13,
14,
15,
0,
5,
10,
13,
14,
0 ,
0 ,
12,
0 ,
6 ,
0,
0,
0,
10,
13,
14,
0,
0 ,
0,
8
};

const char *gpd_reg_name[] = {
"reg_gif_go_le            ",
"reg_act_chk_le           ",
"reg_gpd_bsaddr_go_le     ",
"reg_gpd_rst_le           ",
"reg_png_go_le            ",
"reg_png_blk_go_le        ",
"reg_gif_ltbl_size      ",
"reg_gif_local_tbl      ",
"reg_gif_done           ",
"reg_ofifo_abort        ",
"reg_ififo_full         ",
"reg_ififo_empty        ",
"reg_bsadr_full         ",
"reg_ififo_diff         ",
"reg_bitpos             ",
"reg_png_dtbl_size      ",
"reg_gif_state          ",
"reg_gpd_pitch          ",
"reg_gpd_iwidth         ",
"reg_gpd_iheight        ",
"reg_gpd_bstart         ",
"reg_gpd_bend           ",
"reg_gpd_istart         ",
"reg_spare1             ",
"reg_gpd_boffset        ",
"reg_iofifo_state       ",
"reg_gpd_roi_hstart     ",
"reg_gpd_roi_vstart     ",
"reg_gpd_roi_width      ",
"reg_gpd_roi_height     ",
"reg_gpd_roi_en         ",
"reg_gpd_default_alpha  ",
"reg_spare2_rst0        ",
"reg_png_trans_r        ",
"reg_png_trans_g        ",
"reg_png_blk_done       ",
"reg_png_mincode1       ",
"reg_png_mincode2       ",
"reg_png_mincode3       ",
"reg_png_mincode4       ",
"reg_png_mincode5       ",
"reg_png_mincode6       ",
"reg_png_mincode7       ",
"reg_gpd_ipm_en         ",
"reg_gpd_ipm_size       ",
"reg_gpd_ocolor         ",
"reg_gpd_en               ",
"reg_ofifo_done         ",
"reg_miu_domain_empty   ",
"reg_mreq_always_active ",
"reg_eng_always_active  ",
"reg_png_ltbl_size      ",
"reg_gpd_act_chk        ",
"reg_png_sca            ",
"reg_png_eob            ",
"reg_png_mincode8       ",
"reg_png_mincode9       ",
"reg_png_mincode10      ",
"reg_png_mincode11      ",
"reg_png_mincode12      ",
"reg_png_mincode13      ",
"reg_png_mincode14      ",
"reg_png_mincode15      ",
"reg_png2_mincode1      ",
"reg_png2_mincode2      ",
"reg_png2_mincode3      ",
"reg_png2_mincode4      ",
"reg_png2_mincode5      ",
"reg_png2_mincode6      ",
"reg_png2_mincode7      ",
"reg_png2_mincode8      ",
"reg_png2_mincode9      ",
"reg_png2_mincode10     ",
"reg_png2_mincode11     ",
"reg_png2_mincode12     ",
"reg_png2_mincode13     ",
"reg_png2_mincode14     ",
"reg_png2_mincode15     ",
"reg_png_lbase2         ",
"reg_png_lbase3         ",
"reg_png_lbase4         ",
"reg_png_lbase5         ",
"reg_png_lbase6         ",
"reg_png_lbase7         ",
"reg_png_lbase8         ",
"reg_png_lbase9         ",
"reg_png_lbase10        ",
"reg_png_lbase11        ",
"reg_png_lbase12        ",
"reg_png_lbase13        ",
"reg_png_lbase14        ",
"reg_png_lbase15        ",
"reg_png_dbase2         ",
"reg_png_dbase3         ",
"reg_png_dbase4         ",
"reg_png_dbase5         ",
"reg_png_dbase6         ",
"reg_png_dbase7         ",
"reg_png_dbase8         ",
"reg_png_dbase9         ",
"reg_png_dbase10        ",
"reg_png_dbase11        ",
"reg_png_dbase12        ",
"reg_png_dbase13        ",
"reg_png_dbase14        ",
"reg_png_dbase15        ",
"reg_png_scline0_width  ",
"reg_png_scline1_width  ",
"reg_png_scline2_width  ",
"reg_png_scline3_width  ",
"reg_png_scline4_width  ",
"reg_png_scline5_width  ",
"reg_png_scline6_width  ",
"reg_png_scline0_height ",
"reg_png_scline1_height ",
"reg_png_scline2_height ",
"reg_png_scline3_height ",
"reg_png_scline4_height ",
"reg_png_scline5_height ",
"reg_png_scline6_height ",
"reg_png_mincode_valid  ",
"reg_png2_mincode_valid ",
"reg_gpd_only_decom_en  ",
"reg_png_done           ",
"reg_png_color_type     ",
"reg_gpd_interlace      ",
"reg_spare6             ",
"reg_png_compress_type  ",
"reg_png_color_depth    ",
"reg_miu_act_chk        ",
"reg_png_trans_en       ",
"reg_png_trans_b        ",
"reg_png_state          ",
"reg_ififo_cnt          ",
"reg_hipri              ",
"reg_gpd_premult_alpha_en ",
"reg_gpd_gif_alpha_mask_en",
"reg_frun_cnt           ",
"reg_png_burst_en       ",
"reg_gif_mask           ",
"reg_deflt_fast_on      ",
"reg_ififo_radr         ",
"reg_gpd_time_out       ",
"reg_gif_code_size_err  ",
"reg_gif_err            ",
"reg_gpd_pgend          ",
"reg_miu64              ",
"reg_gpd2mi_adr         ",
"reg_wait_last_done     ",
"reg_miu_wait_cyc       ",
"reg_fixed_pri          ",
"reg_last_done_md       ",
"reg_gpd_read_data      ",
"reg_io_read_gpd        ",
"reg_cbuf_bas           ",
"reg_zbuf_bas           ",
"reg_bist_fail_lz_psram ",
"reg_bist_fail_bst_psram",
"reg_bist_fail_lit_psram",
"reg_bist_fail_flt_psram",
"reg_bist_fail_lmem     ",
"reg_bist_fail_cmem     ",
"reg_bist_fail_omem     ",
"reg_bist_fail_imem     ",
"reg_bist_fail_dc0_cmem ",
"reg_bist_fail_dc1_cmem ",
"reg_bist_fail_stk_psram",
"reg_bist_fail_dma1     ",
"reg_bist_fail_dma0     ",
"reg_bist_fail_cmd      ",
"reg_bist_fail_data     ",
"reg_bist_fail_cc_cmem  ",
"reg_bist_fail_cc_dmem  ",
"reg_bist_fail_cc_rdmem ",
"reg_bist_fail_cc_wdmem ",
"reg_bist_fail_zc_cmem  ",
"reg_bist_fail_zc_dmem  ",
"reg_bist_fail_zc_rdmem ",
"reg_bist_fail_zc_wdmem ",
"reg_int_mask           ",
"reg_int_rst            ",
"reg_debug_mux          ",
"reg_dram_imi           ",
"reg_gpd_istr_8b        ",
"reg_gif_act_clr        ",
"reg_int_status         ",
"reg_int_sw_force       ",
"reg_cache_hit_cmp      ",
"reg_scale_en           ",
"reg_scale_md           ",
"reg_lb_addr            ",
"reg_ub_addr            ",
"reg_gpd_xiu_byte_sel   ",
"reg_gpd_read_bits      ",
"reg_gpd_reserved1      ",
"reg_gpd_cmem_wdata     ",
"reg_png_ltbl_wdata     ",
"reg_png_dtbl_wdata     ",
"reg_debug2_mux         ",
"reg_wbe_bypass_go_chk  ",
"reg_gpd_sram_sd_en     ",
"reg_gpd_debug          ",
"reg_gpd_debug2         ",
"reg_gpd_version        ",
"reg_gpd_tlb            "
};

static volatile MS_VIRT GPD_GET_ADDR(MS_U32 index)
{
    MS_VIRT offset = gpd_reg_addr[index] - (gpd_reg_lsb[index] >> 4);
    return (volatile MS_VIRT)(GPD_REG_BASE + offset * 4);
}

static MS_U32 GPD_GET_MS_U32MASK(MS_U32 index)
{
    if(gpd_reg_msb[index] - gpd_reg_lsb[index] == 31)

      return 0xffffffff;

    else
        return (((MS_U32)1 << (gpd_reg_msb[index] - gpd_reg_lsb[index] + 1)) - 1) << gpd_reg_lsb[index];
}

static MS_U32 GPD_READ_MS_U32(volatile MS_U32 *u32addr)
{
    //volatile MS_U32 *u32addr = (volatile MS_U32 *)u8addr;
    return (u32addr[0] & 0xFFFF) | (u32addr[1] << 16);
}

static void GPD_WRITE_MS_U32(volatile MS_U32 *u32addr, MS_U32 value)
{
    //volatile MS_U32 *u32addr = (volatile MS_U32 *)u8addr;
    u32addr[0] = (value) & 0xFFFF;
    u32addr[1] = (value) >> 16;
}

MS_U32 GPD_GET_MS_U32REG_dbg(MS_U32 index)
{
    volatile MS_VIRT u32addr = GPD_GET_ADDR(index);


    MS_U32 temp = (GPD_READ_MS_U32((volatile MS_U32*)u32addr) & GPD_GET_MS_U32MASK(index)) >> gpd_reg_lsb[index];


    console_printf("Reg read: %s = 0x%08tx, offset = 0x%02tX, lsb=%td, msb=%td\n", gpd_reg_name[index], (ptrdiff_t)temp,
        (ptrdiff_t)gpd_reg_addr[index], (ptrdiff_t)gpd_reg_lsb[index], (ptrdiff_t)gpd_reg_msb[index]);
    return temp;
}

MS_U32 GPD_GET_MS_U32REG(MS_U32 index)
{
    volatile MS_VIRT u32addr = GPD_GET_ADDR(index);


    MS_U32 temp = (GPD_READ_MS_U32((volatile MS_U32*)u32addr) & GPD_GET_MS_U32MASK(index)) >> gpd_reg_lsb[index];


    //console_printf("Reg read: %s = 0x%08X, addr = 0x%08X\n", gpd_reg_name[index], temp, u8addr);
    return temp;
}

void GPD_SET_MS_U32REG(MS_U32 index, MS_U32 value)
{
    volatile MS_VIRT u32addr = GPD_GET_ADDR(index);
    MS_U32 mask = GPD_GET_MS_U32MASK(index);
    MS_U32 temp = GPD_READ_MS_U32((volatile MS_U32*)u32addr);

    temp &= ~mask;

    temp |= ((value) << gpd_reg_lsb[index]) & mask;
    GPD_WRITE_MS_U32((volatile MS_U32*)u32addr, temp);

//    console_printf("Reg write: %s = 0x%08X, offset = 0x%02X, lsb=%d, msb=%d\n", gpd_reg_name[index], (MS_U32)value,
//        (MS_U16)gpd_reg_addr[index], gpd_reg_lsb[index], gpd_reg_msb[index]);
}

MS_U32 hal_gpd_get_clkbase()
{
    // maserati: clkgen1
    return (0x03300 * 2);
}

MS_U32 hal_gpd_get_clkoffset()
{
    // maserati: h0028	h0028	4	0	reg_ckg_gpd
    return (0x50 << 1);
}

void hal_gpd_reg_base(MS_U32* u32RIUBase, MS_U32* u32XIUBase)
{
    // maserati: 113Eh, 113Fh
    *u32RIUBase = (0x13E00 * 2);
    *u32XIUBase = (0x13F00 * 2);
}

void hal_gpd_miu_client(MS_U8* u8Offset, MS_U16* u16BitMask)
{
    // maserati: group2, bit 6==> //h007a	h007a	15	0	reg_miu_sel2
    // 0x7a << 1 = 0xf4
    *u8Offset = 0xf4;
    *u16BitMask = 0x40;
}

void hal_gpd_init_outside_reg(MS_VIRT BankBaseAddr)
{
    MS_U32 tmp = 0;
    // maserati: group2, bit 6==> //h0022	h0022	15	0	reg_miu_group2_i64
    tmp = *((volatile MS_U32 *)(MS_VIRT)(BankBaseAddr + 0x1E00*2 + 0x22*2*2));
    tmp |= 0x40;
    console_printf("set i64 reg to 0x%tx\n", (ptrdiff_t)tmp);
    *((volatile MS_U32 *)(MS_VIRT)(BankBaseAddr + 0x1E00*2 + 0x22*2*2)) = tmp;

#ifdef CONFIG_MSTAR_SRAMPD
    console_printf("GPD sram on\n");
//    *((volatile MS_U32 *)(MS_VIRT)(BankBaseAddr + 0x71200*2 + 0x10*2*2)) &= ~0x0008;
    GPD_SET_MS_U32REG(reg_gpd_sram_sd_en, 0);
    MsOS_DelayTask(1);
#endif

}

void hal_gpd_power_on(MS_VIRT BankBaseAddr)
{
    MS_U32 GPD_CLK_BASE, GPD_CLK_OFFSET;
    GPD_CLK_BASE = hal_gpd_get_clkbase();
    GPD_CLK_OFFSET = hal_gpd_get_clkoffset();

#ifdef CONFIG_MSTAR_CLKM
    MS_S32 handle = Drv_Clkm_Get_Handle("g_clk_gpd");
    Drv_Clkm_Set_Clk_Source(handle , "Performance_mode");
#else
#if 1
        console_printf("@@@set gpd_clk to 216MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0;   // 0x28 << 2
#elif 0
        console_printf("@@@set gpd_clk to 192MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0x4;
#else
        console_printf("@@@set gpd_clk to 144MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0xc;
#endif
#endif

}

void hal_gpd_power_off(MS_VIRT BankBaseAddr)
{
#ifdef CONFIG_MSTAR_CLKM
    MS_S32 handle = Drv_Clkm_Get_Handle("g_clk_gpd");
    Drv_Clkm_Clk_Gate_Disable(handle);
#endif

#ifdef CONFIG_MSTAR_SRAMPD
    console_printf("GPD sram off\n");
//    *((volatile MS_U32 *)(MS_VIRT)(BankBaseAddr + 0x71200*2 + 0x10*2*2)) |= 0x0008;
    GPD_SET_MS_U32REG(reg_gpd_sram_sd_en, 1);
    MsOS_DelayTask(1);
#else
    UNUSED(BankBaseAddr);
#endif
}


void hal_gpd_init_chip_specific_reg(void)
{
}

void hal_gpd_SetMIUProtectMask(MS_VIRT BankBaseAddr, MS_U8 bEnable)
{
    MS_U32 tmp = 0;
    // maserati: h0043	h0043	15	0	reg_rq2_mask
    tmp = *((volatile MS_U32 *)(MS_VIRT)(BankBaseAddr + 0x1200*2 + 0x43*2*2));
    if(bEnable==TRUE)
    {
        tmp |= 0x40;
    }
    else
    {
        tmp &=~0x40;
    }
    *((volatile MS_U32 *)(MS_VIRT)(BankBaseAddr + 0x1200*2 + 0x43*2*2)) = tmp;

    tmp = *((volatile MS_U32 *)(MS_VIRT)(BankBaseAddr + 0x0600*2 + 0x43*2*2));
    if(bEnable==TRUE)
    {
        tmp |= 0x40;
    }
    else
    {
        tmp &=~0x40;
    }
    *((volatile MS_U32 *)(MS_VIRT)(BankBaseAddr + 0x0600*2 + 0x43*2*2)) = tmp;

    console_printf("set miu client protect to 0x%tx\n", (ptrdiff_t)tmp);

    return;
}

