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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   mdrv_graphic.c
/// @brief  MStar graphic Interface
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
/////////////////////////////////////////////////////////////////////////////////////////////////*/

#define _MDRV_GRAPHIC_C

/*=============================================================================
// Include Files
//=============================================================================*/
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#if defined(CONFIG_MIPS)
#elif defined(CONFIG_ARM)
#include <asm/io.h>
#endif

#include "mdrv_mstypes.h"
#include "mdrv_graphic.h"
#include "mhal_graphic.h"
#include "mhal_graphic_reg.h"

void _MDrv_GRAPHIC_Init(MS_U8 u8GopIdx)
{
	_HAL_GOP_Init(u8GopIdx);
}

void _MDrv_GRAPHIC_SetHMirror(MS_U8 u8GOP, MS_BOOL bEnable)
{
	_HAL_GOP_SetHMirror(u8GOP, bEnable);
}

void _MDrv_GRAPHIC_SetVMirror(MS_U8 u8GOP, MS_BOOL bEnable)
{
	_HAL_GOP_SetVMirror(u8GOP, bEnable);
}

void _MDrv_GRAPHIC_OutputColor_EX(MS_U8 u8GOP, EN_DRV_GOP_OUTPUT_COLOR type)
{
	_HAL_GOP_OutputColor_EX(u8GOP, type);
}

void _MDrv_GRAPHIC_MIUSel(MS_U8 u8GOP, E_DRV_GOP_SEL_TYPE MiuSel)
{
	_HAL_GOP_MIUSel(u8GOP, MiuSel);
}

void _MDrv_GRAPHIC_SetGOPDst(MS_U8 u8GOP, EN_DRV_GOP_DST_TYPE eDsttype, MS_BOOL bOnlyCheck)
{
	_HAL_GOP_GWIN_SetDstPlane(u8GOP, eDsttype, bOnlyCheck);
}

void _MDrv_GRAPHIC_EnableTransClr_EX(MS_U8 u8GOP, EN_DRV_GOP_TRANSCLR_FMT eFmt, MS_BOOL bEnable)
{
	_HAL_GOP_EnableTransClr_EX(u8GOP, eFmt, bEnable);
}

void _MDrv_GRAPHIC_SetBlending(MS_U8 u8GOP, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef)
{
	_HAL_GOP_SetBlending(u8GOP, u8win, bEnable, u8coef);
}

void _MDrv_GOP_SetGwinInfo(MS_U8 u8GOP, MS_U8 u8win, DRV_GWIN_INFO WinInfo)
{
	_HAL_GOP_SetGwinInfo(u8GOP, u8win, WinInfo);
}

void _MDrv_GRAPHIC_SetStretchWin(MS_U8 u8GOP, MS_U16 u16x, MS_U16 u16y, MS_U16 u16width, MS_U16 u16height)
{
	_HAL_GOP_SetStretchWin(u8GOP, u16x, u16y, u16width, u16height);
}

void _MDrv_GRAPHIC_SetHScale(MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 u16src, MS_U16 u16dst)
{
	_HAL_GOP_SetHScale(u8GOP, bEnable, u16src, u16dst);
}

void _MDrv_GRAPHIC_SetVScale(MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 u16src, MS_U16 u16dst)
{
	_HAL_GOP_SetVScale(u8GOP, bEnable, u16src, u16dst);
}

void _MDrv_GRAPHIC_GWIN_Enable(MS_U8 u8GOP, MS_U8 u8win, MS_BOOL bEnable)
{
	_HAL_GOP_GWIN_Enable(u8GOP, u8win, bEnable);
}

void _MDrv_GRAPHIC_UpdateReg(MS_U8 u8Gop)
{
	_HAL_GOP_UpdateReg(u8Gop);
}

void _MDrv_GRAPHIC_SetForceWrite(MS_BOOL bEnable)
{
	_HAL_GOP_SetForceWrite(bEnable);
}

