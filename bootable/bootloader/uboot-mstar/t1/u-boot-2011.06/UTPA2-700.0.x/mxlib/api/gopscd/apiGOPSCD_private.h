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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVGOP_PRIV_H_
#define _DRVGOP_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _GOPSCD_RESOURCE_PRIVATE
{
    GOPSC_Info GOPSCDInfoData;
}GOPSCD_RESOURCE_PRIVATE;

enum
{
    GOPSCD_POOL=0
} eGOPSCDPoolID;

typedef GOPSC_Result              (*IOCTL_GOPSCD_INIT)                               (void*);
typedef GOPSC_Result              (*IOCTL_GOPSCD_SCALING)                            (void*, E_GOPSCD_SCALING_CMD, MS_U8);
typedef GOPSC_Result              (*IOCTL_GOPSCD_SET_INFO)                           (void*, GOPSC_Info*);
typedef GOPSC_Result              (*IOCTL_GOPSCD_GET_INFO)                           (void*, GOPSC_Info*);
typedef GOPSC_Result              (*IOCTL_GOPSCD_SET_HVSPSIZE)                      (void*, const GOPSC_SizeInfo*, MS_U32);
typedef GOPSC_Result              (*IOCTL_GOPSCD_SET_SKIPPIXEL)                     (void*, const GOPSC_SkipPixelInfo*, MS_U32);

typedef struct _GOPSCD_INSTANT_PRIVATE
{
	MS_U32							 u32DeviceID;
    IOCTL_GOPSCD_INIT                fpGOPSCDInit;
    IOCTL_GOPSCD_SCALING        	 fpGOPSCDScaling;
    IOCTL_GOPSCD_SET_INFO  		     fpGOPSCDSetInfo;
    IOCTL_GOPSCD_GET_INFO	         fpGOPSCDGetInfo;
    IOCTL_GOPSCD_SET_HVSPSIZE        fpGOPSCDSetHVSP;
    IOCTL_GOPSCD_SET_SKIPPIXEL       fpGOPSCDSetSkipPixel;
}GOPSCD_INSTANT_PRIVATE;

GOPSC_Result MApi_GOPSCD_Init_U2(void* pInstance);
GOPSC_Result MApi_GOPSCD_Scaling_U2(void* pInstance, E_GOPSCD_SCALING_CMD cmd, MS_U8 FrameNum);
GOPSC_Result MApi_GOPSC_GetGOPSCInfo_U2(void* pInstance, GOPSC_Info* gopscInfo);
GOPSC_Result MApi_GOPSC_SetGOPSCInfo_U2(void* pInstance, GOPSC_Info* gopscInfo);
GOPSC_Result MApi_GOPSC_SetHVSPSize_U2(void* pInstance, const GOPSC_SizeInfo* sizeInfo, MS_U32 flag);
GOPSC_Result MApi_GOPSC_SetSkipPixel_U2(void* pInstance, const GOPSC_SkipPixelInfo* skippixelInfo, MS_U32 flag);


void GOPSCDRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GOPSCDOpen(void** ppInstance, const void* const pAttribute);
MS_U32 GOPSCDClose(void* pInstance);
MS_U32 GOPSCDIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
