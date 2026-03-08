/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// file    Mdrv_ld_Compensation.h
/// @brief  local dimming Compensation table
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MDRV_LD_COMPENSATION_H_
#define _MDRV_LD_COMPENSATION_H_

#include "MsCommon.h"
#include "mhal_xc_chip_config.h"

#if  (LD_ENABLE==1)
extern MS_U8 Tbl_LD_Compensation_LG60inchCinema_TB32[];
extern MS_U8 Tbl_LD_Compensation_LG37inch_LR10[];
extern MS_U8 Tbl_LD_Compensation_LG42inch_LR16[];
extern MS_U8 Tbl_LD_Compensation_LG55inch_LR12[];
extern MS_U8 Tbl_LD_Compensation_CMO42inch_LR16[] ;
#endif

#endif
