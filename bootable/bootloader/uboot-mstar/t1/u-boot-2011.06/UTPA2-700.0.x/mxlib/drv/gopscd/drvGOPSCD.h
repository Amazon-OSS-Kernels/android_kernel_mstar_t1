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
#ifndef DRV_GOPSC_EX_H
#define DRV_GOPSC_EX_H

#ifdef DRV_GOPSC_EX_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

// for Utopia 2.0 to Utopia 1.0 compatibility.
void* pu32GOPSCDInst;
void* g_pGOPSCDResource;

typedef enum
{
    ///failed
    En_Drv_GOPSC_FAIL = 0,
    ///success
    En_Drv_GOPSC_OK,
    ///invalid parameter
    En_Drv_GOPSC_INVALID_PARAM,
    ///access not allow
    En_Drv_GOPSC_NotSupport,
}GOPSC_Drv_Result;

GOPSC_Drv_Result MDrv_GOPSC_Init(MS_U32 DeviceId);
GOPSC_Drv_Result MDrv_GOPSC_Enable(MS_U32 DeviceId, MS_BOOL enable);
GOPSC_Drv_Result MDrv_GOPSC_Init_riu_base(MS_U32 DeviceId, MS_VIRT virtriu_base);
GOPSC_Drv_Result MDrv_GOPSC_SetSource(MS_U32 DeviceId, MS_U8 u8SelSrc);
GOPSC_Drv_Result MDrv_GOPSC_SetVScalingMode(MS_U32 DeviceId, MS_U8 mode);
GOPSC_Drv_Result MDrv_GOPSC_SetHVSP(MS_U32 DeviceId, MS_BOOL bHspEnable, MS_BOOL bVspEnable, MS_U16 srcWidth, MS_U16 srcHeight,MS_U16 destWidth, MS_U16 destHeight);
GOPSC_Drv_Result MDrv_GOPSC_SetSkipPixel(MS_U32 DeviceId, MS_U8 bSkipX, MS_U8 bSkipY, MS_U16 xStart, MS_U16 xEnd,MS_U16 yStart, MS_U16 yEnd);
GOPSC_Drv_Result MDrv_GOPSC_SetFieldStartPosition(MS_U32 DeviceId, MS_BOOL bEnable, MS_U16 topFieldOffset, MS_U16 bottomFieldOffset);
GOPSC_Drv_Result MDrv_GOPSC_SetDelayLineNumber(MS_U32 DeviceId, MS_U16 dlyLine);
GOPSC_Drv_Result MDrv_GOPSC_P2IEnable(MS_U32 DeviceId, MS_BOOL enable);
GOPSC_Drv_Result MDrv_GOPSC_HalfMode(MS_U32 DeviceId, MS_BOOL enable);
GOPSC_Drv_Result MDrv_GOPSC_TriggerOnce(MS_U32 DeviceId, MS_U8 FrameNum);
GOPSC_Drv_Result MDrv_GOPSC_SetOutputControlRate(MS_U32 DeviceId, MS_BOOL bEnable,
    MS_U16 SrcWidth, MS_U16 SrcHeight, MS_U16 DestWidth, MS_U16 DestHeight, MS_BOOL bInterlace);
GOPSC_Drv_Result MDrv_GOPSC_SetScalingFormula(MS_U32 DeviceId, MS_U8 Formula);
#undef INTERFACE

#endif
