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
///
/// file    drvHVD_EX.c
/// @brief  HVD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "drvHVD_Common.h"
#include "drvHVD_EX.h"
#include "drvHVD_def.h"
#include "HVD_EX_Common.h"
#include "controller.h"
#include "fwHVD_if.h"
#include "halVPU_EX.h"
#include "halHVD_EX.h"
#include "drvSYS.h"

#if HVD_ENABLE_AUTO_SET_REG_BASE
#include "drvMMIO.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Functions Prototype
//-------------------------------------------------------------------------------------------------
#define IPAUTH_VIDEO_DIVX_PLUS      (26)
#define IPAUTH_VIDEO_H264           (27)
#define IPAUTH_VIDEO_RM             (28)
#define IPAUTH_VIDEO_AVS            (33)
#define IPAUTH_VIDEO_MVC            (40)
#define IPAUTH_VIDEO_VP8            (61)
#define IPAUTH_VIDEO_HEVC           (75)
#define IPAUTH_VIDEO_DIVX_HEVC_4K    (85)
#define IPAUTH_VIDEO_DIVX_HEVC_1080P (86)
#define IPAUTH_VIDEO_VP9            (91)
#define IPAUTH_VIDEO_DOLBY_HDR      (118)

MS_BOOL bModeSupportDivXPlus = FALSE;
MS_BOOL bModeSupportDivXHEVC_4K = FALSE;
MS_BOOL bModeSupportDivXHEVC_1080P = FALSE;
MS_BOOL bModeSelectHD = FALSE;
static MS_BOOL bAutoIpcheck = FALSE;
#if !defined(SUPPORT_X_MODEL_FEATURE)
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);            //FROM #include "drvIPAUTH.h"
#endif

#define HVD_CODEC_INFO_MASK 0x0000000f

MS_BOOL  _MDrv_HVD_AUTH_IPCheck(MS_U32 u32CodecType)
{
    MS_BOOL bIPAUTHReady = FALSE;
    VDEC_EX_CODEC_CAP_INFO pCodecCapInfo;
#if !defined(SUPPORT_X_MODEL_FEATURE) && !defined(MSOS_TYPE_LINUX_KERNEL)
    u32CodecType = (u32CodecType & HVD_CODEC_INFO_MASK);
    bAutoIpcheck = FALSE;
    switch (u32CodecType)
    {
        case HVD_INIT_HW_AVC:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_H264) == TRUE)
            {
                bModeSelectHD = TRUE;
                bIPAUTHReady  = TRUE;
                bAutoIpcheck  = TRUE;
                if((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_PLUS) == TRUE) ||
                   (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HEVC_4K) == TRUE) ||
                   (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HEVC_1080P) == TRUE) )
                {
                    bModeSupportDivXPlus = TRUE;
                }
            }
            break;
        case HVD_INIT_HW_AVS:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_AVS) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady = TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_RM:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_RM) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_MVC:  /// SUPPORT_MVC
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MVC) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_VP8:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VP8) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_HEVC:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_HEVC) == TRUE)
            {
                bModeSelectHD = TRUE;
                bIPAUTHReady  = TRUE;
                bAutoIpcheck  = TRUE;
                if((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HEVC_4K) == TRUE))
                {
                    bModeSupportDivXHEVC_4K = TRUE;
                }
                if((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HEVC_1080P) == TRUE) )
                {
                    bModeSupportDivXHEVC_1080P = TRUE;
                }
            }
            break;
        case HVD_INIT_HW_VP9:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VP9) == TRUE && HAL_VPU_EX_GetCodecCapInfo( E_HVD_EX_CODEC_TYPE_VP9, &pCodecCapInfo) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_HEVC_DV:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DOLBY_HDR) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        default:
            bIPAUTHReady = FALSE;
            break;
    }

    if(bIPAUTHReady == FALSE)
    {
        HVD_EX_MSG_ERR("[%s]User using invaild key !!!\n",__FUNCTION__);
    }
#else
    bModeSelectHD =TRUE;
    bIPAUTHReady =TRUE;
    bAutoIpcheck =TRUE;
#endif
    return bIPAUTHReady;
}

#ifdef VDEC3
#define HVD_MAX_STREAMS         16
#else
#define HVD_MAX_STREAMS         2
#endif

#define VPU_MAIL_BOX_SEND_FAIL 1
#define CODEC_KEY_COMPARE_TIMEOUT 2
extern MS_U8 u8CodecCompare[HVD_MAX_STREAMS];

extern HVD_EX_Result _HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle);

#define _VDEC_GET_IDX(x)    (MS_U8)((x)>>24)

#if defined(CHIP_K6LITE) || defined(CHIP_K6)
MS_BOOL _MDrv_HVDSendVPUCMD(MS_U32 u32Id,MS_U32 u32CMD,MS_U32 u32Arg)
{
    MS_BOOL bRet = TRUE;
    MS_U8  u8Idx = _VDEC_GET_IDX(u32Id);

    HAL_VPU_EX_Mutex_Lock();
    if(HAL_HVD_EX_SetCmd(u32Id, u32CMD, u32Arg) != E_HVD_RETURN_SUCCESS)
    {
        bRet = FALSE;
    }
    HAL_VPU_EX_Mutex_UnLock();

    if(bRet == FALSE)
    {
        printf("[Info]Case1\n");
        u8CodecCompare[u8Idx] = VPU_MAIL_BOX_SEND_FAIL;
    }

    return bRet;
}

MS_BOOL _MDrv_HVDSendCodecKey(MS_U32 u32Id,MS_U8 u8Decoder,MS_U16 u16CodecKey)
{
    MS_U8 u8Idx = HAL_VPU_EX_GetOffsetIdx(u32Id);
    MS_U32 u32Timeout = 0;
    struct _ctl_info *ctl_ptr = (struct _ctl_info *) MsOS_PA2KSEG1(HAL_VPU_EX_GetFWCodeAddr(u32Id) + CTL_INFO_ADDR);

    if(_MDrv_HVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_CMD1,(((VDEC_TAG<<24)&0xff000000))|(((u8Decoder<<16)&0xff0000))|(u16CodecKey&0xffff)) == FALSE)
    {
        HAL_HVD_EX_DeInit(u32Id);
        return FALSE;
    }

    if(_MDrv_HVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_MASK,0) == FALSE)
    {
        HAL_HVD_EX_DeInit(u32Id);
        return FALSE;
    }

    MsOS_ReadMemory();
    u32Timeout = HVD_GetSysTime_ms() + 500; //timeout 500 ms
    while (CTL_TASK_CMD != ctl_ptr->task_statue[u8Idx])
    {
        if (HVD_GetSysTime_ms() > u32Timeout)
        {
            printf("[Info]Case2\n");
            u8CodecCompare[u8Idx] = CODEC_KEY_COMPARE_TIMEOUT;
            HAL_HVD_EX_DeInit(u32Id);
            return FALSE;
        }
        MsOS_ReadMemory();
    }

    return TRUE;
}
#endif


HVD_EX_Result _HVD_EX_Init_(MS_U32 u32Id,MS_U32 u32ModeFlag)
{
    HVD_EX_Result eRst = E_HVD_EX_FAIL;
    if(bAutoIpcheck)
    {
        eRst = _HVD_EX_Rst(u32Id ,FALSE);
#ifndef CONFIG_MBOOT
#if defined(CHIP_K6LITE) || defined(CHIP_K6)
        MS_U32 u32CodecType = u32ModeFlag & HVD_CODEC_INFO_MASK;
        if(eRst == E_HVD_EX_OK)
        {
            if(u32CodecType != HVD_INIT_HW_MJPEG)
            {
                if(_MDrv_HVDSendCodecKey(u32Id,HVD_DECODER,u32CodecType) == FALSE)
                {
                    eRst = E_HVD_EX_FAIL;
                }
            }
        }
#endif
#endif
    }
    else
    {
        printf("[%s] .. STOP and Contact VDEC team!!!\n",__FUNCTION__);
    }

    return eRst;
}

#endif

