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
/// @file   apiGPD_v2.h
/// @brief  GPD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_GPD_V2_H_
#define _API_GPD_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MApi_CMD_GPD_Init,
    MApi_CMD_GPD_InputSource,
    MApi_CMD_GPD_OutputDecode,
    MApi_CMD_GPD_OutputDecodeROI,
    MApi_CMD_GPD_OutputDecodeMGIF,
    MApi_CMD_GPD_GetDuration,
    MApi_CMD_GPD_SetGIFMode,
    MApi_CMD_GPD_ScalingEnable,
    MApi_CMD_GPD_ScalingDisable,
    MApi_CMD_GPD_SetControl,
    MApi_CMD_GPD_GetControl,
    MApi_CMD_GPD_GetCRCResult
} E_GPD_V2_IOCTL_CMD;


typedef struct _GPD_INIT_PARAM
{
    MS_PHY SrcBuf;
    void* retVal;
}GPD_INIT_PARAM, *PGPD_INIT_PARAM;

typedef struct _GPD_INPUTSOURCE_PARAM
{
    gpd_pic_info* param;
    MS_PHY InputBuf;
    MS_U32 Size;
    MS_S32 retVal;
}GPD_INPUTSOURCE_PARAM, *PGPD_INPUTSOURCE_PARAM;

typedef struct _GPD_OUTPUTDECODE_PARAM
{
    MS_PHY DecodeBuf;
    MS_U32 ocolor;
    MS_U32 Size;
    MS_S32 retVal;
}GPD_OUTPUTDECODE_PARAM, *PGPD_OUTPUTDECODE_PARAM;

typedef struct _GPD_OUTPUTDECODEROI_PARAM
{
    MS_PHY DecodeBuf;
    MS_U32 ocolor;
    MS_U32 Size;
    gpd_roi_info *roi;
    MS_S32 retVal;
}GPD_OUTPUTDECODEROI_PARAM, *PGPD_OUTPUTDECODEROI_PARAM;

typedef struct _GPD_GETDURATION_PARAM
{
    MS_S32 retVal;
}GPD_GETDURATION_PARAM, *PGPD_GETDURATION_PARAM;

typedef struct _GPD_SETGIFMODE_PARAM
{
   MS_U32 mode;
}GPD_SETGIFMODE_PARAM, *PGPD_SETGIFMODE_PARAM;

typedef struct _GPD_SCALINGENABLE_PARAM
{
    MS_U32 mode;
}GPD_SCALINGENABLE_PARAM, *PGPD_SCALINGENABLE_PARAM;


typedef struct _GPD_CONTROL_PARAM
{
    gpd_user_cmd cmd_id;
    MS_VIRT param;
    MS_S32 retVal;
}GPD_CONTROL_PARAM, *PGPD_CONTROL_PARAM;

typedef struct _GPD_GET_CONTROL_PARAM
{
    gpd_get_cmd cmd_id;
    MS_VIRT param;
    MS_S32 retVal;
}GPD_GET_CONTROL_PARAM, *PGPD_GET_CONTROL_PARAM;

typedef struct _GPD_GETCRCRESULT_PARAM
{
    const MS_U8 *pu8Data;
    MS_U32 u32Size;
    MS_U32 u32Width;
    MS_U32 ocolor;
    MS_U32 retVal;
}GPD_GETCRCRESULT_PARAM, *PGPD_GETCRCRESULT_PARAM;


void GPDRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GPD_Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 GPD_Close(void* pInstance);
MS_U32 GPD_Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);


#ifdef __cplusplus
}
#endif

#endif // _API_GPD_V2_H_


