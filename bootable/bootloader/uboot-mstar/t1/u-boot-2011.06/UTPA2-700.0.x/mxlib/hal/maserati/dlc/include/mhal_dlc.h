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
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
#ifndef MHAL_DLC_H
#define MHAL_DLC_H

#include "dlc_hwreg.h"

#define ENABLE_AVERAGELUMA      0

#define halDlc_FuncEnter()

#define halDlc_FuncExit()

MS_U8   Hal_DLC_init(void);
void    Hal_DLC_init_riu_base( MS_VIRT ptr_riu_base);
MS_BOOL Hal_DLC_GetLumaCurveStatus(void);
void    Hal_DLC_CGC_ResetCGain(void);
MS_U8   Hal_DLC_CGC_GetCGain(void);
void    Hal_DLC_CGC_SetCGain(MS_U8 u8CGain);
void    Hal_DLC_CGC_ResetYGain(void);
MS_U8   Hal_DLC_CGC_GetYGain(void);
void    Hal_DLC_CGC_SetYGain(MS_U8 u8YGain);
MS_U8   Hal_DLC_get_dlc_method(void);
MS_U16  Hal_DLC_get_averageluma_delta(void);
MS_U16  Hal_DLC_get_averageluma_boundary_low(void);
MS_U16  Hal_DLC_get_averageluma_boundary_midlow(void);
MS_U16  Hal_DLC_get_averageluma_boundary_midhigh(void);
MS_U16  Hal_DLC_get_averageluma_boundary_high(void);
MS_BOOL Hal_DLC_get_c_gain_ctrl(void);
MS_U16  Hal_DLC_get_step(void);
MS_U16  Hal_DLC_get_flick_alpha(void);
void    Hal_DLC_set_curve(MS_U8 *pCurve);
MS_U16  Hal_DLC_get_c_gain_strength(void);
void    Hal_DLC_set_curveExt(MS_U16 *pCurve);
MS_U16  Hal_DLC_get_histogram_y_sum(void);
MS_U16  Hal_DLC_get_histogam_pixel_sum(void);
void    Hal_DLC_set_onoff(MS_BOOL bWindow, MS_BOOL ben);
MS_BOOL Hal_DLC_get_histogram_ack(void);
void    Hal_DLC_set_histogram_req(void);
void    Hal_DLC_get_histogram32(MS_U16 *pTable);
void    Hal_DLC_set_curve_sub(MS_U8 *pCurve);

#endif


