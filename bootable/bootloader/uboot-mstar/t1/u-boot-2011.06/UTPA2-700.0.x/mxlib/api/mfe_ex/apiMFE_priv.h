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

#ifndef _APIMFE_PRIV_H_
#define _APIMFE_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file apiMFE.h
/// @brief MFE control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drv_mfe_st.h"
#include "mfe_common.h"

typedef MS_BOOL (*IOCTL_MFE_INIT)(PVR_Info*);
typedef MS_BOOL (*IOCTL_MFE_ENCODE)(PVR_Info*);
typedef MS_BOOL (*IOCTL_MFE_GETVOL)(PVR_Info*, void*);
typedef MS_BOOL (*IOCTL_MFE_COMPRESSONEPICTURE)(PVR_Info*, MEMMAP_CUR_t, MS_BOOL);
typedef MS_BOOL (*IOCTL_MFE_DEINIT)(PVR_Info*);
typedef MS_BOOL (*IOCTL_MFE_POWEROFF)(PVR_Info*);

typedef MS_BOOL (*IOCTL_MFE_GETOBUF)(PVR_Info* mfe_Info,MEMMAP_t* obuf);

typedef MS_BOOL (*IOCTL_MFE_SETBITRATE_FRAMERATE)(PVR_Info* mfe_Info);
typedef MS_BOOL (*IOCTL_MFE_SETCOLORFORMAT)(PVR_Info* mfe_Info);
typedef MS_BOOL (*IOCTL_MFE_RESETSPSPPS)(PVR_Info* mfe_Info);
typedef MS_BOOL (*IOCTL_MFE_SETFRAMETYPE)(PVR_Info* mfe_Info,MS_S32 frametype);
typedef MS_BOOL (*IOCTL_MFE_SETISR)(PVR_Info* mfe_Info);
typedef MS_BOOL (*IOCTL_MFE_GETHWCAP)(PVR_Info* mfe_Info, MFE_HwCapV0 *pDrv, MS_U32 *len);
typedef MS_BOOL (*IOCTL_MFE_GETOUTPUT_INFO)(PVR_Info *mfe_Info, MS_S32 *frametype, MS_S32 *size);
typedef MS_BOOL (*IOCTL_MFE_SETVUI)(PVR_Info* mfe_Info, VUI_Info* vui_info);

typedef struct __attribute__((packed)) _MFE_INSTANCE_PRIVATE
{
    // instance variable
    IOCTL_MFE_INIT                    fpMFEInit;
    IOCTL_MFE_ENCODE                  fpMFEEncode;
    IOCTL_MFE_GETVOL                  fpMFEGetVOL;
    IOCTL_MFE_COMPRESSONEPICTURE      fpMFECompressOnePicture;
    IOCTL_MFE_DEINIT                  fpMFEDeInit;
    IOCTL_MFE_POWEROFF                fpMFEPowerOff;
    IOCTL_MFE_GETOBUF                 fpMFEGetOBuf;
    IOCTL_MFE_SETBITRATE_FRAMERATE    fpMFESetBitrateFramerate;
    IOCTL_MFE_SETCOLORFORMAT          fpMFESetColorFormat;
    IOCTL_MFE_RESETSPSPPS             fpMFEResetSPSPPS;
    IOCTL_MFE_SETFRAMETYPE            fpMFESetFrameType;
    IOCTL_MFE_SETISR                  fpMFESetISR;
    IOCTL_MFE_GETHWCAP                fpMFEGetHWCap;
    IOCTL_MFE_GETOUTPUT_INFO          fpMFEGetOutputInfo;
    IOCTL_MFE_SETVUI                  fpMFESetVUI;
}MFE_INSTANCE_PRIVATE;

typedef struct __attribute__((packed)) _MFE_RESOURCE_PRIVATE
{
    MS_S32 sAliveInstance;
}MFE_RESOURCE_PRIVATE;

void MFERegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 MFEOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 MFEClose(void* pInstance);
MS_U32 MFEIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);


MS_BOOL _MApi_MFE_Init(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_Encode(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_GetVOL(PVR_Info* mfe_Info, void *header_info);
MS_BOOL _MApi_MFE_CompressOnePicture(PVR_Info* mfe_Info, MEMMAP_CUR_t YUVPlane, MS_BOOL bForceIframe);
MS_BOOL _MApi_MFE_DeInit(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_PowerOff(PVR_Info* mfe_Info);

MS_BOOL _MApi_MFE_GetOBuf(PVR_Info* mfe_Info,MEMMAP_t* obuf);

MS_BOOL _MApi_MFE_SetBitrateFramerate(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_SetColorFormat(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_ResetSPSPPS(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_SetFrameType(PVR_Info* mfe_Info, MS_S32 frametype);
MS_BOOL _MApi_MFE_ISREnable(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_GetHWCap(PVR_Info* mfe_Info, MFE_HwCapV0 *pDrv, MS_U32 *len);
MS_BOOL _MApi_MFE_GetOutputInfo(PVR_Info *mfe_Info, MS_S32 *frametype, MS_S32 *size);
MS_BOOL _MApi_MFE_SetVUI(PVR_Info *mfe_Info, VUI_Info* vui_info);
#ifdef __cplusplus
}
#endif
#endif // _APIMFE_PRIV_H_





