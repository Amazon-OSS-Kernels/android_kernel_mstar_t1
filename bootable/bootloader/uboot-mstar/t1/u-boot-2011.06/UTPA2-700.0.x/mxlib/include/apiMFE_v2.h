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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiMFE_v2.h
/// @brief  MFE Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_MFE_V2_H_
#define _API_MFE_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "drv_mfe_st.h"

#include "MsTypes.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MApi_CMD_MFE_Init = 0x0,
    MApi_CMD_MFE_Encode,
    MApi_CMD_MFE_GetVOL,
    MApi_CMD_MFE_CompressOnePicture,
    MApi_CMD_MFE_DeInit,
    MApi_CMD_MFE_PowerOff,
    MApi_CMD_MFE_GetOutBuffer,
    MApi_CMD_MFE_SetBitrateFramerate,
    MApi_CMD_MFE_SetColorFormat,
    MApi_CMD_MFE_ResetSPSPPS,
    MApi_CMD_MFE_SetFrameType,
    MApi_CMD_MFE_SetISR,
    MApi_CMD_MFE_GetHWCap,
    MApi_CMD_MFE_GetOutputInfo,
    MApi_CMD_MFE_SetVUI,
} E_MFE_V2_IOCTL_CMD;

typedef struct _MFE_INIT_PARAM
{
    PVR_Info* mfe_Info;
    MS_BOOL retVal;
}MFE_INIT_PARAM, *PMFE_INIT_PARAM;

typedef struct _MFE_SET_PARAM
{
    PVR_Info* mfe_Info;
    MS_S32 frametype;
    MS_BOOL retVal;
}MFE_SET_PARAM, *PMFE_SET_PARAM;

typedef struct _MFE_GETVOL_PARAM
{
    PVR_Info* mfe_Info;
    void *header_info;
    MS_BOOL retVal;
}MFE_GETVOL_PARAM, *PMFE_GETVOL_PARAM;

typedef struct _MFE_COMPRESSONEPICTURE_PARAM
{
    PVR_Info* mfe_Info;
    MEMMAP_CUR_t YUVPlane;
    MS_BOOL bForceIframe;
    MS_BOOL retVal;
}MFE_COMPRESSONEPICTURE_PARAM, *PMFE_COMPRESSONEPICTURE_PARAM;

typedef struct _MFE_GET_OBUF_PARAM
{
    PVR_Info* mfe_Info;
    MEMMAP_t* outbuf;
    MS_BOOL retVal;
}MFE_GET_OBUF_PARAM, *PMFE_GET_OBUF_PARAM;

typedef struct _MFE_GET_HWCAP_PARAM
{
    PVR_Info* mfe_Info;
    MFE_HwCapV0 *pDrv;
    MS_U32 *HwCap_len;
    MS_BOOL retVal;
}MFE_GET_HWCAP_PARAM, *PMFE_GET_HWCAP_PARAM;

typedef struct _MFE_GETOUTPUT_PARAM
{
    PVR_Info* mfe_Info;
    MS_S32 frametype;
    MS_S32 size;
    MS_BOOL retVal;
}MFE_GETOUTPUT_PARAM, *PMFE_GETOUTPUT_PARAM;

typedef struct _MFE_SETVUI_PARAM
{
    PVR_Info* mfe_Info;
    MS_U32 setLevel;
    MS_BOOL setVUI_aspect_ratio_info_present_flag;
    MS_U32 setVUI_aspect_ratio_idc;
    MS_U32 setVUI_sar_width;
    MS_U32 setVUI_sar_height;
    MS_BOOL retVal;
}MFE_SETVUI_PARAM, *PMFE_SETVUI_PARAM;

void MFERegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 MFE_Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 MFE_Close(void* pInstance);
MS_U32 MFE_Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);


#ifdef __cplusplus
}
#endif

#endif // _API_MFE_V2_H_


