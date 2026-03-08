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
/// @file   drvCH34.h
/// @brief  CH34 Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CH34_H__
#define __DRV_CH34_H__

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
#define CH34_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'C','H','3','4'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','1'},                          /* 00 ~ 99                                          */  \
    {'0','0','0','0','0','0','0','0'},  /* CL#                                              */  \
    MSIF_OS


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    ///failed
    En_Drv_CH34_FAIL = 0,
    ///success
    En_Drv_CH34_OK,
    ///invalid parameter
    En_Drv_CH34_INVALID_PARAM,
    ///access not allow
    En_Drv_CH34_NotSupport,
}CH34_Drv_Result;

typedef enum
{
    E_CH34_VIDEO_SYS_NTSC = 0,     ///< NTSC
    E_CH34_VIDEO_SYS_PAL,          ///< PAL
    E_CH34_VIDEO_SYS_NUM,
} E_CH34_VIDEO_SYS;

typedef enum
{
    E_CH34_AUDIO_SYS_MONO = 0,     ///< MONO
    E_CH34_AUDIO_SYS_BTSC,         ///< BTSC_STEREO
    E_CH34_AUDIO_SYS_BTSC_SAP,     ///< BTSC_SAP
    E_CH34_AUDIO_SYS_A2,           ///< A2
    E_CH34_AUDIO_SYS_NUM,
} E_CH34_AUDIO_SYS;

typedef enum
{
    E_CH34_CH3 = 0,     ///< ch3
    E_CH34_CH4,          ///< ch4
    E_CH34_NUM,
} E_CH34_CHANNEL;

#define CH34_TABLE_OFFSET                  4
#define CH34_TABLE_MAX_DEPTH               2000

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

CH34_Drv_Result MDrv_Ch34_Init(void);
// CH34_Drv_Result MDrv_Ch34_TurnOnOff(MS_BOOL bOn);
CH34_Drv_Result MDrv_Ch34_SetMode(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys);
// CH34_Drv_Result MDrv_Ch34_WriteCH34Table(MS_U8* pTable);

#ifdef __cplusplus
}
#endif


#endif // __DRV_CH34_H__

