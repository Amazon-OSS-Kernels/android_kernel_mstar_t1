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
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvPVR_IframeLUT.h
/// @brief  Secure PVR Iframe LUT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_PVR_IframeLUT_H_
#define _DRV_PVR_IframeLUT_H_

#include "MsTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    E_PVR_IFRAMELUT_RESULT_FAIL = 0,
    E_PVR_IFRAMELUT_RESULT_OK,
    E_PVR_IFRAMELUT_RESULT_INVALID_PARAM,
    E_PVR_IFRAMELUT_RESULT_FUNC_ERROR,
    E_PVR_IFRAMELUT_RESULT_NOT_SUPPORT,
} EN_PVR_IFRAMELUT_RESULT;

typedef enum
{
    E_PVR_IFRAMELUT_DBG_RELEASE = 0,
    E_PVR_IFRAMELUT_DBG_ERR,
    E_PVR_IFRAMELUT_DBG_INFO,
} EN_PVR_IFRAMELUT_DBGLEVEL;

typedef enum
{
    E_PVR_IFRAMELUT_VDEC_TYPE_MPEG = 1,
    E_PVR_IFRAMELUT_VDEC_TYPE_H264,
    E_PVR_IFRAMELUT_VDEC_TYPE_HEVC,
    E_PVR_IFRAMELUT_VDEC_TYPE_AVS,
    E_PVR_IFRAMELUT_VDEC_TYPE_INVALID,
} EN_PVR_IFRAMELUT_VDEC_TYPE;

typedef struct DLL_PACKED
{
    MS_U32 u32CfgDataSize;
    MS_U32 u32PvrEng;
    MS_U32 u32BufferStartAddr;
    MS_U32 u32BufferSize;
    EN_PVR_IFRAMELUT_VDEC_TYPE eVdecType;
    // MS_U32 u32TSCEPath;
} PVR_IFRAMELUT_Cfg;

EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_Init(void);
EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_Open(PVR_IFRAMELUT_Cfg *pstPVR_IframeLUTCfg);
EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_SetVdecType(MS_U32 u32PvrEng, EN_PVR_IFRAMELUT_VDEC_TYPE eVdecType);
EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_GetWritePtr(MS_U32 u32PvrEng, MS_U32 *pu32WritePtr);
EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_Close(MS_U32 u32PvrEng);
EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_Exit(void);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif // _DRV_PVR_IframeLUT_H_
