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
// file   MVD_EX_Common.c
// @brief  MPEG Video Decoder (MVD) OSAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

#include "MsCommon.h"
#include "drvMVD_EX.h"
#include "halVPU_EX.h"
#include "controller.h"
#include "halMVD_EX.h"
#include "fwHVD_if.h"
#include "../hvd_v3/drvHVD_def.h"


#define IPAUTH_VIDEO_MPEG2          (19)
#define IPAUTH_VIDEO_MPEG2_HD       (20)
#define IPAUTH_VIDEO_MPEG4          (21)
#define IPAUTH_VIDEO_MPEG4_SD       (22)
#define IPAUTH_VIDEO_MPEG4_HD       (23)
#define IPAUTH_VIDEO_DIVX_1080PHD   (24)
#define IPAUTH_VIDEO_DIVX_DRM       (25)
#define IPAUTH_VIDEO_DIVX_PLUS      (26)
#define IPAUTH_VIDEO_VC1            (29)
#define IPAUTH_VIDEO_WMV            (30)
#define IPAUTH_VIDEO_WMDRM_PD       (31)
#define IPAUTH_VIDEO_WMDRM_ND       (32)
#define IPAUTH_VIDEO_FLV            (34)
#define IPAUTH_VIDEO_DIVX_QMOBILE   (35)
#define IPAUTH_VIDEO_DIVX_MOBILE    (36)
#define IPAUTH_VIDEO_DIVX_HT        (37)
#define IPAUTH_VIDEO_DIVX_720PHD    (38)
#if !defined(SUPPORT_X_MODEL_FEATURE)
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);
#endif
extern MS_BOOL bIsSupportDivx;

MS_BOOL MDrv_MVD_AUTH_IPCheck(MS_U32 u32CodecType,MS_BOOL* bSupportSDModeOnly)
{
    MS_BOOL bIPAUTHReady = FALSE;
    *bSupportSDModeOnly    = TRUE;
#if !defined(SUPPORT_X_MODEL_FEATURE) && !defined(MSOS_TYPE_LINUX_KERNEL)
    switch (u32CodecType)
    {
        case E_MVD_CODEC_MPEG2:
           if(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG2_HD) == TRUE)
           {
               bIPAUTHReady = TRUE;
               *bSupportSDModeOnly = FALSE;
           }
           else if(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG2) == TRUE)
           {
               bIPAUTHReady = TRUE;
           }
           break;
       case E_MVD_CODEC_MPEG4:
       case E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER:
           if(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4_HD) == TRUE)
           {
               bIPAUTHReady = TRUE;
               *bSupportSDModeOnly = FALSE;
           }
           else if((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4) == TRUE) || (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4_SD) == TRUE))
           {
               bIPAUTHReady = TRUE;
           }
           break;
       case E_MVD_CODEC_DIVX311:
           if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_1080PHD) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_DRM) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_PLUS) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HT) == TRUE))
           {
               bIPAUTHReady = TRUE;
               *bSupportSDModeOnly = FALSE;
           }
           else if((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_QMOBILE) == TRUE) || (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_MOBILE) == TRUE))
           {
               bIPAUTHReady = TRUE;
           }
           break;
       case E_MVD_CODEC_FLV:
           if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_FLV) == TRUE)
           {
               *bSupportSDModeOnly = FALSE;
               bIPAUTHReady = TRUE;
           }
           break;
       case E_MVD_CODEC_VC1_ADV:
       case E_MVD_CODEC_VC1_MAIN:
           if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VC1) == TRUE)
           {
               *bSupportSDModeOnly =FALSE;
               bIPAUTHReady = TRUE;
           }
           break;
       default:
             bIPAUTHReady = FALSE;;
    }

    if(bIPAUTHReady == FALSE)
    {
        printf("%s fail!!\r\n", __FUNCTION__);
    }
    if((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_1080PHD) == TRUE)
       ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_DRM) == TRUE)
       ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_PLUS) == TRUE)
       ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_QMOBILE) == TRUE)
       ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_MOBILE) == TRUE)
       ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HT) == TRUE))
    {
        bIsSupportDivx = TRUE;
    }
    else
    {
        bIsSupportDivx = FALSE;
    }
#else
		bIPAUTHReady = TRUE;
		*bSupportSDModeOnly = FALSE;
#endif
    return bIPAUTHReady;
}
#ifdef VDEC3
#define MVD_MAX_STREAMS         16
#else
#define MVD_MAX_STREAMS         2
#endif

#define VPU_MAIL_BOX_SEND_FAIL 1
#define CODEC_KEY_COMPARE_TIMEOUT 2
extern MS_U8 u8CodecCompare[MVD_MAX_STREAMS];
extern HVD_Return HAL_HVD_EX_SetCmd(MS_U32 u32Id, HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg);
#define _VDEC_GET_IDX(x)    (MS_U8)((x)>>24)

#if defined(CHIP_K6LITE) || defined(CHIP_K6)
MS_BOOL MDrv_MVDSendVPUCMD(MS_U32 u32Id,MS_U32 u32CMD,MS_U32 u32Arg)
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






MS_BOOL MDrv_MVDSendCodecKey(MS_U32 u32Id,MS_U8 u8Decoder,MS_U16 u16CodecKey)
{
    MS_U8 u8Idx = _VDEC_GET_IDX(u32Id);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetOffsetIdx(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U32 u32Timeout = 0;
    struct _ctl_info *ctl_ptr = (struct _ctl_info *) MsOS_PA2KSEG1(HAL_VPU_EX_GetFWCodeAddr(u32Id) + CTL_INFO_ADDR);

    if(MDrv_MVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_CMD1,(((VDEC_TAG<<24)&0xff000000))|(((u8Decoder<<16)&0xff0000))|(u16CodecKey&0xffff)) == FALSE)
    {
        HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)MDrv_MVD_GetVpuStreamId(u32Id));
        return FALSE;
    }

    if(MDrv_MVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_MASK,0) == FALSE)
    {
        HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)MDrv_MVD_GetVpuStreamId(u32Id));
        return FALSE;
    }

    MsOS_ReadMemory();
    u32Timeout = MsOS_GetSystemTime() + 500; //timeout 500 ms
    while (CTL_TASK_CMD != ctl_ptr->task_statue[u8VPUIdx])
    {
        if (MsOS_GetSystemTime() > u32Timeout)
        {
            printf("[Info]Case2\n");
            u8CodecCompare[u8Idx] = CODEC_KEY_COMPARE_TIMEOUT;
            HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)MDrv_MVD_GetVpuStreamId(u32Id));
            return FALSE;
        }
        MsOS_ReadMemory();
    }
    return TRUE;
}
#endif

MS_BOOL _MVD_Init(MS_U32 u32Id, MVD_CodecType u8CodecType, MVD_SrcMode u8BSProviderMode, MS_U8 bDisablePESParsing)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    //MVD_PRINT("u8CodecType=0x%x\n", u8CodecType);
    //MVD_PRINT("u8BSProviderMode=0x%x\n", u8BSProviderMode);
    if (HAL_MVD_SetCodecInfo(u8HalIdx, u8CodecType, u8BSProviderMode, bDisablePESParsing) == FALSE)
    {
        printf("%s fail!!\r\n", __FUNCTION__);
        return FALSE;
    }

    #ifndef CONFIG_MBOOT  //Add For GPL (content protection)
    #if defined(CHIP_K6LITE) || defined(CHIP_K6)
    if(MDrv_MVDSendCodecKey(u32Id,MVD_DECODER,u8CodecType) == FALSE)
    {
        return FALSE;
    }
    else
    #endif
    #endif
    {
        return TRUE;
    }
}
#endif
