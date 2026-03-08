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

#ifndef _HAL_HDMITX_H_
#define _HAL_HDMITX_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//*********************//
//             Video   //
//*********************//


typedef enum
{
    E_HDMITX_RES_640x480p       = 0,
    E_HDMITX_RES_720x480i       = 1,
    E_HDMITX_RES_720x576i       = 2,
    E_HDMITX_RES_720x480p       = 3,
    E_HDMITX_RES_720x576p       = 4,
    E_HDMITX_RES_1280x720p_50Hz   = 5,
    E_HDMITX_RES_1280x720p_60Hz   = 6,
    E_HDMITX_RES_1920x1080i_50Hz  = 7,
    E_HDMITX_RES_1920x1080i_60Hz  = 8,
    E_HDMITX_RES_1920x1080p_24Hz  = 9,
    E_HDMITX_RES_1920x1080p_25Hz  = 10,
    E_HDMITX_RES_1920x1080p_30Hz  = 11,
    E_HDMITX_RES_1920x1080p_50Hz  = 12,    
    E_HDMITX_RES_1920x1080p_60Hz  = 13,
    E_HDMITX_RES_MAX              = 14,
} MsHDMITX_VIDEO_TIMING;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#ifdef MHAL_HDMITX_C
#define INTERFACEE
#else
#define INTERFACEE extern
#endif

INTERFACEE void MHal_HDMITx2_DefaultTiming(void);
INTERFACEE void MHal_HDMITx2_SetInitTiming(MsHDMITX_VIDEO_TIMING TFlag1);
INTERFACEE void MHal_HDMITx2_Power_OnOff(MS_BOOL bEnable);

#endif // _HAL_HDMITX_H_

