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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvHVD_EX.c
/// @brief  HVD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_NUTTX

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

#define IPAUTH_VIDEO_H264           (27)
#define IPAUTH_VIDEO_RM             (28)
#define IPAUTH_VIDEO_AVS            (33)
#define IPAUTH_VIDEO_MVC            (40)
#define IPAUTH_VIDEO_VP8            (61)
#define IPAUTH_VIDEO_HEVC           (75)
#define IPAUTH_VIDEO_DOLBY_HDR      (118)
MS_BOOL bModeSelectHD = FALSE;
static MS_BOOL bAutoIpcheck = FALSE;
#if !defined(SUPPORT_X_MODEL_FEATURE)
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);            //FROM #include "drvIPAUTH.h"
#endif

MS_BOOL  _MDrv_HVD_AUTH_IPCheck(MS_U32 u32CodecType)
{
    MS_BOOL bIPAUTHReady = FALSE;
#if !defined(SUPPORT_X_MODEL_FEATURE) && !defined(MSOS_TYPE_LINUX_KERNEL)
    #define HVD_CODEC_INFO_MASK 0x0000000F
    u32CodecType = (u32CodecType & HVD_CODEC_INFO_MASK);
    bAutoIpcheck = FALSE;
    switch (u32CodecType)
    {
        case HVD_INIT_HW_AVC:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_H264) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady = TRUE;
                bAutoIpcheck =TRUE;
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
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_VP9:
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

extern HVD_EX_Result _HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle);

HVD_EX_Result _HVD_EX_Init_(MS_U32 u32Id)
{
    HVD_EX_Result eRst = E_HVD_EX_FAIL;
    if(bAutoIpcheck)
    {
        eRst = _HVD_EX_Rst(u32Id ,FALSE);
    }else
    {
        printf("[%s] .. STOP and Contact VDEC team!!!\n",__FUNCTION__);
    }

    return eRst;
}

#endif

