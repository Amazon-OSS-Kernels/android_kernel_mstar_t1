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
/// @file   apiDAC_v2.h
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DAC_V2_H_
#define _DAC_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

/// DAC CMD
typedef enum
{
    //DAC command
    E_DAC_GET_LIBVER,
    E_DAC_GET_INFO,
    E_DAC_GET_STATUS,
    E_DAC_SET_DBG_LEVEL,
    E_DAC_INIT,
    E_DAC_ENABLE,
    E_DAC_SET_CLKINV,
    E_DAC_SET_YPBPR_OUTPUTTIMING,
    E_DAC_SET_OUTPUT_SOURCE,
    E_DAC_SET_OUTPUT_LEVEL,
    E_DAC_SET_OUTPUT_SWAPSEL,
    E_DAC_ONOFF_SD,
    E_DAC_ONOFF_HD,
    E_DAC_CLKSEL,
    E_DAC_DUMP_TABLE,
    E_DAC_EXIT,
    E_DAC_SET_IHALF_OUTPUT,
    E_DAC_SET_QUART_OUTPUT,
    E_DAC_SET_DAC_STATE,
    E_DAC_HOTPLUG_DETECT,
}E_DAC_IOCTL_CMDS;;

typedef struct
{
    const MSIF_Version **ppVersion;
    APIDAC_Result eReturnValue;
} stDAC_GET_LIBVER,*pstDAC_GET_LIBVER;

typedef struct
{
    const DAC_ApiInfo* stReturnValue;
} stDAC_GET_INFO, *pstDAC_GET_INFO;

typedef struct
{
    DAC_ApiStatus* pDacStatus;
    MS_BOOL bReturnValue;
} stDAC_GET_STATUS,*pstDAC_GET_STATUS;

typedef struct
{
    MS_U16 u16DbgSwitch;
    MS_BOOL bReturnValue;
} stDAC_SET_DBG_LEVEL,*pstDAC_SET_DBG_LEVEL;

typedef struct
{
    MS_BOOL bReturnValue;
} stDAC_INIT,*pstDAC_INIT;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
} stDAC_ENABLE, *pstDAC_ENABLE;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
} stDAC_SET_CLKINV, *pstDAC_SET_CLKINV;

typedef struct
{
    E_OUTPUT_VIDEO_TIMING_TYPE eTiming;
} stDAC_SET_YPBPR_OUTPUTTIMING,*pstDAC_SET_YPBPR_OUTPUTTIMING;

typedef struct
{
    E_DAC_OUTPUT_TYPE enOutputType;
    MS_BOOL bIsYPbPr;
} stDAC_SET_OUTPUT_SOURCE,*pstDAC_SET_OUTPUT_SOURCE;

typedef struct
{
    E_DAC_MAX_OUTPUT_LEVEL enLevel;
    MS_BOOL bIsYPbPr;
} stXC_SET_OUTPUT_LEVEL, *pstXC_SET_OUTPUT_LEVEL;

typedef struct
{
    E_DAC_SWAP_SEL enSwap;
    MS_BOOL bIsYPbPr;
} stDAC_SET_OUTPUT_SWAPSEL, *pstDAC_SET_OUTPUT_SWAPSEL;

typedef struct
{
    E_DAC_SD_ENABLE_CTRL enBit;
} stDAC_ONOFF_SD,*pstDAC_ONOFF_SD;

typedef struct
{
    E_DAC_HD_ENABLE_CTRL enBit;
} stDAC_ONOFF_HD, *pstDAC_ONOFF_HD;

typedef struct
{
    E_OUTPUT_VIDEO_TIMING_TYPE eTiming;
    E_OUTPUT_BIT_TYPE ebits;
} stDAC_CLKSEL, *pstDAC_CLKSEL;

typedef struct
{
    MS_U8 *pDACTable;
    MS_U8 u8DACtype;
} stDAC_DUMP_TABLE, *pstDAC_DUMP_TABLE;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
} stDAC_SET_IHALF_OUTPUT, *pstDAC_SET_IHALF_OUTPUT;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
} stDAC_SET_QUART_OUTPUT, *pstDAC_SET_QUART_OUTPUT;

typedef struct
{
    MS_BOOL bEnabled;
    MS_BOOL bIsYPbPr;
} stDAC_SET_DAC_STATE, *pstDAC_SET_DAC_STATE;

typedef struct
{
    E_DAC_DETECT SelDAC;
    E_DAC_DETECT_TYPE DetectType;
    MS_BOOL *State;
    MS_BOOL bReturnValue;
} stDAC_HOTPLUG_DETECT, *pstDAC_HOTPLUG_DETECT;


#ifdef __cplusplus
}
#endif

#endif // _API_DAC_H_

