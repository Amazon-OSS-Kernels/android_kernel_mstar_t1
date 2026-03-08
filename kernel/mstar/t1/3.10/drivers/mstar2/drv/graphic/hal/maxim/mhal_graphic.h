/**
* Copyright (c) 2016 – 2017 MStar Semiconductor, Inc.
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
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mhal_graphic.h
// @brief  Graphic Driver Interface
// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////////////////////*/

/*-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------*/
#define MAX_GOP0_GWIN                           2
#define MAX_GOP1_GWIN                           2UL
#define MAX_GOP2_GWIN                           1UL
#define MAX_GOP3_GWIN                           1UL
#define MAX_GOP4_GWIN                           1UL
#define MAX_GOP5_GWIN                           0UL

#define GOP_BIT0    0x01
#define GOP_BIT1    0x02
#define GOP_BIT2    0x04
#define GOP_BIT3    0x08
#define GOP_BIT4    0x10
#define GOP_BIT5    0x20
#define GOP_BIT6    0x40
#define GOP_BIT7    0x80
#define GOP_BIT8    0x0100
#define GOP_BIT9    0x0200
#define GOP_BIT10   0x0400
#define GOP_BIT11   0x0800
#define GOP_BIT12   0x1000
#define GOP_BIT13   0x2000
#define GOP_BIT14   0x4000
#define GOP_BIT15   0x8000

#define GOP_REG_WORD_MASK                           0xffff
#define GOP_REG_HW_MASK                             0xff00
#define GOP_REG_LW_MASK                             0x00ff

#define GOP_FIFO_BURST_ALL                          (GOP_BIT8|GOP_BIT9|GOP_BIT10|GOP_BIT11|GOP_BIT12)

#define GOP_FIFO_BURST_MASK                         (GOP_BIT8|GOP_BIT9|GOP_BIT10|GOP_BIT11|GOP_BIT12)
#define GOP_FIFO_THRESHOLD                          0xF0

#define GOP_YUV_TRANSPARENT_ENABLE              GOP_BIT5
#define GOP_RGB_TRANSPARENT_ENABLE              GOP_BIT11

#define GOP_WordUnit                                32
#define GOP_STRETCH_WIDTH_UNIT                2
#define SCALING_MULITPLIER                      0x1000

#define _fb_phy_addr  0x38F00000
#define PANEL_WIDTH         3840
#define PANEL_HEIGHT        2160
/*-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------*/

void _HAL_GOP_Write16Reg(MS_U32 u32addr, MS_U16 u16val, MS_U16 mask);
void _HAL_GOP_Read16Reg(MS_U32 u32addr, MS_U16 *pu16ret);
void _HAL_GOP_Write32Reg(MS_U32 u32addr, MS_U32 u32val);
void _HAL_GOP_Init(MS_U8 u8GOP);
void _HAL_GOP_GWIN_SetDstPlane(MS_U8 u8GOP, EN_DRV_GOP_DST_TYPE eDsttype, MS_BOOL bOnlyCheck);
void _HAL_GOP_SetBlending(MS_U8 u8GOP, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef);
MS_U8 _HAL_GOP_GetBnkOfstByGop(MS_U8 gop, MS_U32 *pBnkOfst);
void _HAL_GOP_SetHMirror(MS_U8 u8GOP, MS_BOOL bEnable);
void _HAL_GOP_SetVMirror(MS_U8 u8GOP, MS_BOOL bEnable);
void _HAL_GOP_OutputColor_EX(MS_U8 u8GOP, EN_DRV_GOP_OUTPUT_COLOR type);
void _HAL_GOP_MIUSel(MS_U8 u8GOP, E_DRV_GOP_SEL_TYPE MiuSel);
void _HAL_GOP_EnableTransClr_EX(MS_U8 u8GOP, EN_DRV_GOP_TRANSCLR_FMT eFmt, MS_BOOL bEnable);
void _HAL_GOP_SetGwinInfo(MS_U8 u8GOP, MS_U8 u8win, DRV_GWIN_INFO WinInfo);
void _HAL_GOP_SetStretchWin(MS_U8 u8GOP, MS_U16 u16x, MS_U16 u16y, MS_U16 u16width, MS_U16 u16height);
void _HAL_GOP_SetHScale(MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 u16src, MS_U16 u16dst);
void _HAL_GOP_SetVScale(MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 u16src, MS_U16 u16dst);
void _HAL_GOP_GWIN_Enable(MS_U8 u8GOP, MS_U8 u8win, MS_BOOL bEnable);
void _HAL_GOP_UpdateReg(MS_U8 u8Gop);
void _HAL_GOP_SetForceWrite(MS_BOOL bEnable);
