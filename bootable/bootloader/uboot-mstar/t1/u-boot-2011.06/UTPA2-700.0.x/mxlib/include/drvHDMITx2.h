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
///
/// @file   drvHDMITx.h
/// @brief  HDMI Tx Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HDMITX_H_
#define _DRV_HDMITX_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "halHDMITx2.h"


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
#define HDMITX_ISR_ENABLE              1
#else
#define HDMITX_ISR_ENABLE              1
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//*********************//
//        DVI / HDMI   //
//*********************//

/*
       Bit1:
          - 0: DVI
          - 1: HDMI
       Bit0:
          - 0: without HDCP
          - 1: with HDCP
*/

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef MDRV_HDMITX_C
#define INTERFACED
#else
#define INTERFACED extern
#endif

INTERFACED MS_BOOL MDrv_HDMITx2_Init(void);

// HDMI Tx clock power On/Off
INTERFACED void MDrv_HDMITx2_Power_OnOff(MS_BOOL bEnable);

// This routing set initialization for HDMITX2 moudle
INTERFACED void MDrv_HDMITx2_SetInitTiming(MsHDMITX_VIDEO_TIMING TFlag1);

#ifdef __cplusplus
}
#endif

#endif // _DRV_HDMITX_H_

