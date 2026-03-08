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
/// @file   drvMVOP.h
/// @brief  MVOP Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_STBDC_H_
#define _DRV_STBDC_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// Version string.
#define MSIF_STBDC_LIB_CODE              {'S','T','B','_'}    //Lib code
#define MSIF_STBDC_LIBVER                {'0','1'}            //LIB version
#define MSIF_STBDC_BUILDNUM              {'0','1'}            //Build Number
#define MSIF_STBDC_CHANGELIST            {'0','0','1','3','4','8','7','2'} //P4 ChangeList Number

#define STBDC_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_STBDC_LIB_CODE,                 /* IP__                                             */  \
    MSIF_STBDC_LIBVER,                   /* 0.0 ~ Z.Z                                        */  \
    MSIF_STBDC_BUILDNUM,                 /* 00 ~ 99                                          */  \
    MSIF_STBDC_CHANGELIST,               /* CL#                                              */  \
    MSIF_OS


#define _SUPPORT_IMG_OFFSET_

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_STBDC_OK    = 0,
    E_STBDC_FAIL  = 1,
    E_STBDC_INVALID_PARAM = 2
} STBDC_Result;

/// MVOP tile format
typedef enum
{
    E_STBDC_TILE_8x32  = 0,
    E_STBDC_TILE_16x32 = 1,
    E_STBDC_TILE_NONE  = 2
} STBDC_TileFormat;




typedef struct
{
    MS_PHYADDR u32LumaAddr;
    MS_PHYADDR u32ChromaAddr;
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 u16Pitch;
    MS_BOOL bYUV422;    //!< YUV422 or YUV420
    MS_BOOL bProgressive;   //!< Progressive or Interlace
    MS_BOOL bBottomField;         // +S3, Field 0 or 1
    MS_U16 u16CropX;
    MS_U16 u16CropY;
    MS_U16 u16CropWidth;
    MS_U16 u16CropHeight;
} STBDC_InputCfg;
//---------------------



typedef enum
{
    STBDC_PATTERN_NORMAL     = 0,
    STBDC_PATTERN_COLORBAR   = 1,
    STBDC_PATTERN_FRAMECOLOR = 2,
    STBDC_PATTERN_YCBCR      = 3,
    STBDC_PATTERN_Y          = 4,
    STBDC_PATTERN_CB         = 5,
    STBDC_PATTERN_CR         = 6,
    STBDC_PATTERN_DEFAULT    = 7   //new pattern = (last parrtern + 1) % MVOP_PATTERN_DEFAULT
} STBDC_Pattern;

typedef enum
{
    E_STBDC_INPUT_DRAM = 0,
    E_STBDC_INPUT_HVD = 1,    //SVD for T2
    E_STBDC_INPUT_MVD  = 2,
    E_STBDC_INPUT_JPD  = 3,
    E_STBDC_INPUT_UNKNOWN = -1
} STBDC_InputSel;

/// MVOP VideoStat data structure
typedef struct
{
    MS_U16 u16HorSize;
    MS_U16 u16VerSize;
    MS_U16 u16FrameRate;
    MS_U8 u8AspectRate;
    MS_U8 u8Interlace;
    MS_U16 u16HorOffset;
    MS_U16 u16VerOffset;
} STBDC_VidStat;


///MVOP driver status
typedef struct _STBDC_DrvStatus
{
    MS_BOOL bIsInit;
    MS_BOOL bIsEnable;
    MS_BOOL bIsUVShift;
    MS_BOOL bIsBlackBG;
    MS_BOOL bIsMonoMode;
    MS_BOOL bIsSetTiming;
} STBDC_DrvStatus;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------





#ifdef __cplusplus
}
#endif

#endif // _DRV_MVOP_H_
