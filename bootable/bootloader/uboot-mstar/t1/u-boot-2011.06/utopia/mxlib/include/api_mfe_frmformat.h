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

#ifndef _API_MFE_FRMFORMAT_H_
#define _API_MFE_FRMFORMAT_H_

#include "MsTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
MFE_API_MFE_COLORFORMAT_MSTTILE = 0,
MFE_API_MFE_COLORFORMAT_YUYV = 1,
MFE_API_MFE_COLORFORMAT_YVYU = 2,
MFE_API_MFE_COLORFORMAT_UYVY = 3,
MFE_API_MFE_COLORFORMAT_VYUY = 4,
} MFE_API_MFE_COLORFORMAT;

typedef struct _MS_MfeFrameFormat
{
    MS_PHY Cur_PhyY0;
    MS_U8* Cur_VirY0;
    MS_PHY Cur_PhyC0;
    MS_U8* Cur_VirC0;
    MS_PHY Cur_PhyY1;
    MS_U8* Cur_VirY1;
    MS_PHY Cur_PhyC1;
    MS_U8* Cur_VirC1;

} MS_MfeFrameFormat_t;

typedef struct _MS_MfeEXIFInfo
{
    MS_U32 tmp;
} MS_MfeEXIFInfo_t;

typedef struct _MS_MfeAdvInfo
{
    MS_U32 frame_mode;
    MS_U32 has_exif;
    MS_U32 framerate_change;
    MS_U32 new_framerate;
    MFE_API_MFE_COLORFORMAT colorformat;
} MS_MfeAdvInfo_t;

typedef struct _MS_MfeFrameInfo
{
    MS_U8 starcode[5]; //must be 0x00,0x00,0x00,0x01,0x47
    MS_MfeFrameFormat_t inbuf;
    MS_MfeAdvInfo_t     advbuf;
    //MS_MfeEXIFInfo_t exif_info;
} MS_MfeFrameInfo_t;

#ifdef __cplusplus
}
#endif

#endif //_API_MFE_FRMFORMAT_H_