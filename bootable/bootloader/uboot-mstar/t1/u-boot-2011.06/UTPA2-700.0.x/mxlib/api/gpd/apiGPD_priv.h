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

#ifndef _APIGPD_PRIV_H_
#define _APIGPD_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file apiGPD.h
/// @brief GPD control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "apiGPD.h"

#define SUPPORT_GPD_NUM_MAX     1

typedef void* (*IOCTL_GPD_INIT)(MS_PHY);
typedef MS_S32 (*IOCTL_GPD_INPUT_SOURCE)(gpd_pic_info*, MS_PHY, MS_U32);
typedef MS_S32 (*IOCTL_GPD_OUTPUT_DECODE)(MS_PHY, MS_U32, MS_U32);
typedef MS_S32 (*IOCTL_GPD_OUTPUT_DECODE_ROI)(MS_PHY, MS_U32, MS_U32, gpd_roi_info*);
typedef MS_S32 (*IOCTL_GPD_OUTPUT_DECODE_MGIF)(MS_PHY, MS_U32, MS_U32);
typedef MS_S32 (*IOCTL_GPD_GET_DURATION)(void);
typedef void (*IOCTL_GPD_SET_GIF_MODE)(int);
typedef void (*IOCTL_GPD_SCALING_ENABLE)(MS_U32);
typedef void (*IOCTL_GPD_SCALING_DISABLE)(void);
typedef MS_U32 (*IOCTL_GPD_SET_CONTROL)(gpd_user_cmd, MS_VIRT);
typedef MS_U32 (*IOCTL_GPD_GET_CONTROL)(gpd_get_cmd, MS_VIRT);
typedef MS_U32 (*IOCTL_GPD_GET_CRC_RESULT)(const MS_U8 *, MS_U32);


typedef struct __attribute__((packed))
{
    MS_U8 u8DecoderInUse[SUPPORT_GPD_NUM_MAX];
}GPD_RESOURCE_PRIVATE;

typedef struct _GPD_INSTANCE_PRIVATE
{
    void* pResource;
    // instance variable
    MS_U8                                       Index_of_decoder;
    MS_BOOL                                     bGPDInUse;
    MS_BOOL                                     bGPDSuspend;

    IOCTL_GPD_INIT                              fpGPDInit;
    IOCTL_GPD_INPUT_SOURCE                      fpGPDInputSource;
    IOCTL_GPD_OUTPUT_DECODE                     fpGPDOutputDecode;
    IOCTL_GPD_OUTPUT_DECODE_ROI                 fpGPDOutputDecodeROI;
    IOCTL_GPD_OUTPUT_DECODE_MGIF                fpGPDOutputDecodeMGIF;
    IOCTL_GPD_GET_DURATION                      fpGPDGetDuration;
    IOCTL_GPD_SET_GIF_MODE                      fpGPDSetGIFMode;
    IOCTL_GPD_SCALING_ENABLE                    fpGPDScalingEnable;
    IOCTL_GPD_SCALING_DISABLE                   fpGPDScalingDisable;
    IOCTL_GPD_SET_CONTROL                       fpGPDSetControl;
    IOCTL_GPD_GET_CONTROL                       fpGPDGetControl;
    IOCTL_GPD_GET_CRC_RESULT                    fpGPDGetCRCResult;

}GPD_INSTANCE_PRIVATE;

void GPDRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GPDOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 GPDClose(void* pInstance);
MS_U32 GPDIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 GPDStr(MS_U32 u32PowerState, void* pModule);


void *_MApi_GPD_Init(MS_PHY SrcBuf);
MS_S32 _MApi_GPD_InputSource(gpd_pic_info* param, MS_PHY InputBuf, MS_U32 Size);
MS_S32 _MApi_GPD_OutputDecode(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size);
MS_S32 _MApi_GPD_OutputDecodeROI(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size, gpd_roi_info *roi);
MS_S32 _MApi_GPD_OutputDecodeMGIF(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size);
MS_S32 _MApi_GPD_GetDuration(void);
void _MApi_GPD_SetGIFMode(MS_U32 mode);

void _MApi_GPD_ScalingEnable(MS_U32 mode);
void _MApi_GPD_ScalingDisable(void);
MS_U32 _MApi_GPD_SetControl(gpd_user_cmd cmd_id, MS_VIRT param);
MS_U32 _MApi_GPD_GetControl(gpd_get_cmd cmd_id, MS_VIRT param);
MS_U32 _MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32, MS_U32);

#ifdef __cplusplus
}
#endif
#endif // _APIGPD_PRIV_H_





