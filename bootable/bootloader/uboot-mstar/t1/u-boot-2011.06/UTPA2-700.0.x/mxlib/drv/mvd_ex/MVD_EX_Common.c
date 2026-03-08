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
// file   MVD_EX_Common.c
// @brief  MPEG Video Decoder (MVD) OSAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_NUTTX

#include "MsCommon.h"
#include "drvMVD_EX.h"


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

MS_BOOL MDrv_MVD_AUTH_IPCheck(MS_U32 u32CodecType,MS_BOOL* bSupportSDModeOnly)
{
    MS_BOOL bIPAUTHReady = FALSE;
    *bSupportSDModeOnly    = TRUE;
#if !defined(SUPPORT_X_MODEL_FEATURE)
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
           if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VC1) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMV) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMDRM_PD) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMDRM_ND) == TRUE))
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
#else
		bIPAUTHReady = TRUE;
		*bSupportSDModeOnly = FALSE;
#endif
    return bIPAUTHReady;
}
#endif
