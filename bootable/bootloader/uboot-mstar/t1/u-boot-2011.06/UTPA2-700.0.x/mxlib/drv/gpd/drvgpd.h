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
#ifndef DRVGPD_H
#define DRVGPD_H

#include "gpd.h"

#define DEFAULT_ALPHA 0xFF


#define FALSE  0
#define TRUE   1

#if 1
extern MS_VIRT READBITBASE;
#define read_bits_reg(n) 	(*((volatile MS_U16*)(MS_VIRT)(READBITBASE+((0x10|(n-1))<<2))))
#define show_bits_reg(n) 	(*((volatile MS_U16*)(MS_VIRT)(READBITBASE+((0x20|(n-1))<<2))))
#else
#define READBITBASE1 0xA000543C
#define READBITBASE2 0xA000547C
#define read_bits_reg(n) 	(*((volatile MS_U32*)(READBITBASE1+(n<<2))))
#define show_bits_reg(n) 	(*((volatile MS_U32*)(READBITBASE2+(n<<2))))
#endif

void drv_gpd_set_scaling(MS_U32 enable, MS_U32 mode);
MS_U32 drv_gpd_get_scaling_mode(void);

#if (defined(__arm__))
void drv_eng_always_active(MS_U32 value);
#endif

void drv_gpd_png_en(void);
void drv_gpd_gif_en(void);
MS_U16 read_bits(MS_U32 n);
MS_U16 show_bits(MS_U32 n);

MS_U32 drv_reg_iofifo_state(void);
void drv_gpd_eco_enable(MS_U32 enable);

void drv_gpd_set_zbuf_bas(MS_PHY base);
void drv_gpd_set_cbuf_bas(MS_PHY base);

void set_nop_cmd(MS_U32 num);

MS_U32 drv_gpd_png_state(void);
MS_U32 drv_gpd_bitpos(void);
MS_U32 drv_gpd_ififo_radr(void);
MS_U32 drv_gpd_ififo_empty(void);
MS_U32 drv_gpd_ififo_full(void);
MS_U32 drv_gpd_ofifo_done(void);
MS_U32 drv_gpd_bsadr_full(void);
MS_U32 drv_gpd_png_blk_done(void);
MS_U32 drv_gpd_png_eob(void);

void drv_gpd_Init(void);
void drv_gpd_set_bstart_end(MS_PHY bstart, MS_PHY bend);
void drv_gpd_set_ocolor(MS_U32 ocolor_typ);
void drv_gpd_write_cmap(MS_U32 num, MS_U8 *ptr, MS_U32 trans_idx);
//void drv_gpd_write_cmap(MS_U32 num, MS_U32 *ptr);

void drv_gpd_set_pitch(MS_U32 pitch);
void drv_gpd_set_gif_swidth_sheight(MS_U32 width ,MS_U32 height);
void drv_gpd_set_iwidth_height(MS_U32 iwidth, MS_U32 iheight);
void drv_gpd_set_istart(MS_U32 istart);
void drv_gpd_set_ROI(MS_U32 enable, MS_U32 hstart, MS_U32 vstart, MS_U32 width, MS_U32 height);
void drv_gpd_set_interlace(MS_U8 interlace);
void drv_gpd_set_gif_local_map_size(MS_U32 bpp);

void drv_gpd_set_gif_go(void);
MS_U32 drv_gpd_gif_done(void);
void drv_gpd_set_trans(MS_U16 r, MS_U16 g, MS_U16 b, MS_U8 enable);
void drv_gpd_set_palette(MS_U8 *palette, MS_U16 palette_num, MS_U8 *trans, MS_U16 num_trans);
void drv_gpd_set_pgend(MS_U8 pgend);

void drv_gpd_set_frun_cnt(MS_U32 frun_cnt);
void drv_gpd_set_iccp_dtsize(MS_U8 iccp, MS_U8 dtsize);
void drv_gpd_set_type(MS_U8 color_type, MS_U8 interlace, MS_U8 color_depth);
void drv_gpd_set_default_alpha(MS_U8 alpha);
void drv_gpd_set_sca_dfa(MS_U8 sca, MS_U8 dfa);
void drv_gpd_set_ltsize(MS_U16 ltsize);

void drv_gpd_set_scline_width_progressive(MS_U32 width, MS_U8 bpp);
void drv_gpd_set_scline_height_progressive(MS_U32 height);
void drv_gpd_set_scline_width_interlace(MS_U32 width, MS_U8 bpp);
void drv_gpd_set_scline_height_interlace(MS_U32 height);

void drv_gpd_set_png_go(void);
void drv_gpd_set_blk_go(void);
void drv_gpd_set_png_done(MS_U32 value);
void drv_gpd_set_cmp_type(MS_U8 type);

void drv_gpd_set_lbase(MS_U16 *lbase);
void drv_gpd_set_dbase(MS_U16 *dbase);
void drv_gpd_set_lmincode_valid(MS_U16 lmin_valid);
void drv_gpd_set_dmincode_valid(MS_U16 dmin_valid);
void drv_gpd_set_lmincode(MS_U16 *lmin);
void drv_gpd_set_dmincode(MS_U16 *dmin);

void drv_gpd_set_fixed_ldata(void);
void drv_gpd_set_dynamic_ldata(MS_U16 num, MS_U16 *ldata);
void drv_gpd_set_dynamic_ddata(MS_U16 num, MS_U16 *ddata);
MS_U32 drv_gpd_check_engine(void);
void drv_gpd_set_hipri(MS_U16 hipri);
void drv_gpd_set_access_region(MS_U32 start, MS_U32 end);
MS_U32 drv_gpd_get_clkbase(void);
MS_U32 drv_gpd_get_clkoffset(void);
MS_U32 is_stb(void);
void drv_gpd_init_outside_reg(MS_VIRT BankBaseAddr);
void drv_gpd_power_on(void);
void drv_gpd_power_off(void);
void drv_gpd_init_chip_specific_reg(void);
MS_U8 MDrv_GPD_MIU_Select(MS_U32 u32Addr);
void MDrv_GPD_Reg_Base(MS_U32* u32RIUBase, MS_U32* u32XIUBase);
void MDrv_GPD_MIU_Client(MS_U8* u8Offset, MS_U16* u16BitMask);

// GPD Interrupt Register Function
typedef void (*GPD_IsrFuncCb)(void);

MS_U8 MDrv_GPD_EnableISR(GPD_IsrFuncCb IsrCb);
MS_U8 MDrv_GPD_DisableISR(void);
void MDrv_GPD_SetISRMask(void);
void MDrv_GPD_PrintGPDBuf(void);
void drv_gpd_dbg_dump(void);
void drv_gpd_set_cacheable(MS_U32 u32Cacheable);
MS_U32 drv_gpd_get_cacheable(void);
MS_VIRT drv_gpd_get_RIU_BASE(void);

#endif

