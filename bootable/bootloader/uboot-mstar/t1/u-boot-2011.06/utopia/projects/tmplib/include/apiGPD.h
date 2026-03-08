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


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   apiGPD.h
/// @brief  GPD driver
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup GPD GPD interface (apiGPD.h)
    \ingroup VDEC

   <b> The supporting features of Mstar GPD HW IP are listed in the following: </b>
   -# GIF/PNG decoder (GPD)
   -# Support up to 8192 x 8192 pixel image(per channel 8 bits), or 4096 x 8192(per channel 16 bits)pixel image
   -# Original png format 1bpp/2bpp/4bpp/8bpp index(palette) mode support
   -# Original png format ARGB8888, RGB565, gray, gray with alpha mode support
   -# Transparency mode support on png (alpha=0)
   -# ARGB8888, RGB565, YUV422(YUYV),YUV422(YVYU),gray, gray with alpha output format support
   -# gif support
   -# scaling down mode on X/Y both directions (1/2,1/4,1/8,1/16) for PNG progressive mode, and (1/8,1/16) for PNG interlace mode and GIF

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html GPD_decode_flow.png

*! \defgroup GPD_Basic GPD Basic APIs
*  \ingroup GPD
*! \defgroup GPD_Debug GPD Debug APIs
*  \ingroup GPD
*! \defgroup GPD_to_be_removed GPD_to_be_removed (to be removed)
*  \ingroup GPD
*/

#ifndef _API_GPD_H_
#define _API_GPD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MSIF_GPD_LIB_CODE      {'G','P','D','_'}
#define MSIF_GPD_LIBVER        {'0','1'}
#define MSIF_GPD_BUILDNUM      {'0','3'}
#define MSIF_GPD_CHANGELIST    {'0','0','6','1','4','5','9','5'}

/// Version string.
#define GPD_API_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_GPD_LIB_CODE,                 /* IP__                                             */  \
    MSIF_GPD_LIBVER,                   /* 0.0 ~ Z.Z                                        */  \
    MSIF_GPD_BUILDNUM,                 /* 00 ~ 99                                          */  \
    MSIF_GPD_CHANGELIST,               /* CL#                                              */  \
    MSIF_OS




#if !defined(TRUE) && !defined(FALSE)
#define TRUE 1
#define FALSE 0
#endif

#define ARGB8888 0
#define YUV422   1
#define RGB565   2
#define YVYU422  3
#define GRAY     4
#define GRAY_ALPHA 5

#ifndef GPD_SUPPORT_MGIF
#define GPD_SUPPORT_MGIF 1
#endif

#ifndef GPD_NO_SUPPORT_MGIF
#define GPD_NO_SUPPORT_MGIF 0
#endif

#define GPD_VERSION 0x12062516
#define WORDALIGN  0x3
#define GPD_CLOCK_OFFSET (0x50 << 1)

#define GPD_UTOPIA20       (1)

typedef struct
{
    MS_PHY    u32PA_ReadBufferAddr;
    MS_VIRT    u32VA_ReadBufferAddr;
    MS_U32    u32ReadBufferSize;
    MS_PHY    u32PA_WriteBufferAddr;
    MS_VIRT    u32VA_WriteBufferAddr;
    MS_U32    u32WriteBufferSize;
    MS_PHY       u32PA_DepthBufferAddr;
    MS_VIRT    u32VA_DepthBufferAddr;
    MS_PHY       u32PA_ColorBufferAddr;
    MS_VIRT       u32VA_ColorBufferAddr;
    MS_VIRT    RegBaseAddr;
    MS_VIRT    ReadBitBaseAddr;
} gpd_addr_info;


typedef struct
{
    char   *filename;
    MS_U32    u32Width;
    MS_U32    u32Height;
    MS_U8     u8BitPP;
    MS_U8     u8Interlace;
    MS_S32    enGPDStatus;    //Error Code
    MS_U8     u8MGIF;
} gpd_pic_info;


typedef struct
{
    MS_U32    hstart;
    MS_U32    vstart;
    MS_U32    width;
    MS_U32    height;

} gpd_roi_info;

typedef struct
{
    MS_PHY u32PA_StartAddr;
    MS_PHY u32PA_EndAddr;

} gpd_access_region;

typedef struct
{
    MS_U32    u32Width;    // input parameter
    MS_U32    ocolor;      // input parameter
    MS_U32    u32Pitch;    // output parameter
} gpd_get_pitch_info;

typedef enum {
    GPD_STATE_MGIF_TERMINATE,
    GPD_STATE_GIF_ERROR,
    GPD_STATE_MGIF_WAIT_BUFFFER,
    GPD_STATE_GIF_SUCCESS
} gpd_state;


typedef enum
{
    E_GPD_USER_CMD_SET_ACCESS_REGION = 0,
    E_GPD_USER_CMD_SET_CACHEABLE,

} gpd_user_cmd;

typedef enum
{
    E_GPD_GET_CMD_GET_PITCH = 0,

} gpd_get_cmd;

typedef enum
{
    E_GPD_SCALE_ONE_TO_TWO,
    E_GPD_SCALE_ONE_TO_FOUR,
    E_GPD_SCALE_ONE_TO_EIGHT,
    E_GPD_SCALE_ONE_TO_SIXTEEN,
} gpd_scale_mode;

#if 1
//-------------------------------------------------------------------------------------------------
/// This function will initialize the parameters of Mstar GPD
/// @ingroup GPD_Basic
/// @param SrcBuf \b IN : internal use buffer
/// @return void
//-------------------------------------------------------------------------------------------------
void *MApi_GPD_Init(MS_PHY SrcBuf);
//-------------------------------------------------------------------------------------------------
/// This function will setup the GPD input picture
/// @ingroup GPD_Basic
/// @param param \b IN : input pic info
/// @param InputBuf \b IN : input pic address
/// @param Size \b IN : input Size
/// @return S32
//-------------------------------------------------------------------------------------------------
MS_S32 MApi_GPD_InputSource(gpd_pic_info* param, MS_PHY InputBuf, MS_U32 Size);
//-------------------------------------------------------------------------------------------------
/// This function will decode the GPD file
/// @ingroup GPD_Basic
/// @param DecodeBuf \b IN : output buffer address
/// @param ocolor \b IN : output color format
/// @param Size \b IN : output buffer Size
/// @return S32
//-------------------------------------------------------------------------------------------------
MS_S32 MApi_GPD_OutputDecode(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size);
//-------------------------------------------------------------------------------------------------
/// This function will decode the GPD file with ROI
/// @ingroup GPD_Basic
/// @param DecodeBuf \b IN : output buffer address
/// @param ocolor \b IN : output color format
/// @param Size \b IN : output buffer Size
/// @param roi \b IN : output buffer ROI
/// @return S32
//-------------------------------------------------------------------------------------------------
MS_S32 MApi_GPD_OutputDecodeROI(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size, gpd_roi_info *roi);
//-------------------------------------------------------------------------------------------------
/// This function will decode the motion GIF file (to be removed)
/// @ingroup GPD_to_be_removed
/// @param DecodeBuf \b IN : output buffer address
/// @param ocolor \b IN : output color format
/// @param Size \b IN : output buffer Size
/// @return S32
//-------------------------------------------------------------------------------------------------
MS_S32 MApi_GPD_OutputDecodeMGIF(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size);

//-------------------------------------------------------------------------------------------------
/// This function will get the GIF duration (to be removed)
/// @ingroup GPD_to_be_removed
/// @param void
/// @return S32
//-------------------------------------------------------------------------------------------------
MS_S32 MApi_GPD_GetDuration(void);
//-------------------------------------------------------------------------------------------------
/// This function will set  the MGIF mode (to be removed)
/// @ingroup GPD_to_be_removed
/// @param void
/// @return S32
//-------------------------------------------------------------------------------------------------
void MApi_GPD_SetGIFMode(MS_U32 mode);


#else

MS_S32 MAdp_GPD_Init(void);
MS_S32 MAdp_GPD_Play(gpd_pic_info* param, MS_U32 ocolor, MS_U8 *srcbuffer, MS_U32 size);
MS_S32 MAdp_GPD_Compare(MS_U32 ocolor_typ);
#endif

//-------------------------------------------------------------------------------------------------
/// This function will enable the scaling function
/// @ingroup GPD_Basic
/// @param mode \b IN : scaling mode
/// @return void
//-------------------------------------------------------------------------------------------------
// void MApi_GPD_ScalingEnable(MS_U32 mode);
//-------------------------------------------------------------------------------------------------
/// This function will disable the scaling function
/// @ingroup GPD_Basic
/// @param void
/// @return void
//-------------------------------------------------------------------------------------------------
// void MApi_GPD_ScalingDisable(void);
//-------------------------------------------------------------------------------------------------
/// This function will set the gpd information
/// @ingroup GPD_Basic
/// @cmd_id \b IN : control ID
/// @param param \b IN : control info
/// @return S32
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_GPD_SetControl(gpd_user_cmd cmd_id, MS_VIRT param);
//-------------------------------------------------------------------------------------------------
/// This function will get the gpd information
/// @ingroup GPD_Basic
/// @cmd_id \b IN : control ID
/// @param param \b IN : control info
/// @return S32
//-------------------------------------------------------------------------------------------------
// MS_U32 MApi_GPD_GetControl(gpd_get_cmd cmd_id, MS_VIRT param);
//-------------------------------------------------------------------------------------------------
/// This function will get the crc information
/// @ingroup GPD_Debug
/// @param pu8Data \b IN : address
/// @param u32Size \b IN : size
/// @param u32Width \b IN : width
/// @param ocolor \b IN : color format
/// @return S32
//-------------------------------------------------------------------------------------------------
// MS_U32 MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, MS_U32 ocolor);

#ifdef __cplusplus
}
#endif


#endif

