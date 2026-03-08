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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// file    Mdrv_ld_Gamma.h
/// @brief  local dimming gamma table
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_LD_GAMMA_H_
#define _MDRV_LD_GAMMA_H_

#include "MsCommon.h"
#include "mhal_xc_chip_config.h"

#if  (LD_ENABLE==1)
extern const MS_U8 gamma_linear[64];
extern const MS_U8 gamma_0dot6[64];
extern const MS_U8 gamma_0dot4[64];
extern const MS_U8 gamma_1dot2[64];
extern const MS_U8 gamma_1dot4[64];
extern const MS_U8 gamma_0dot9[64];
extern const MS_U8 gamma_0dot8[64];
extern const MS_U8 gamma_0dot75[64];
extern const MS_U8 LGE_sample3[64];
extern const MS_U8 LGE_sampleTHX[64];
extern const MS_U8 gamma_0dot78[64];
extern const MS_U8 gamma_0dot7[64];
extern const MS_U8 gamma_dark_higher[64];
extern const MS_U8 LGE_sample1_adjust[64*9];
extern const MS_U8 LGE_sample2_adjust[64*9];
extern const MS_U8 LGE_sample7_adjust[64*9];
extern const MS_U8 LGE_sample1_0dot81[64];
#endif
#endif
