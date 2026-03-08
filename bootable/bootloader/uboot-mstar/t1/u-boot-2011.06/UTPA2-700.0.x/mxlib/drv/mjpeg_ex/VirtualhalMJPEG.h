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
/// @file   halMJPEG.h
/// @brief  MJPEG hal interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VIRTUALHAL_MJPEG_H_
#define _VIRTUALHAL_MJPEG_H_

// Dual Decode
typedef enum
{
    E_HAL_MJPEG_MAIN_STREAM,
    E_HAL_MJPEG_SUB_STREAM
} HAL_MJPEG_StreamType;

typedef enum
{
    E_HAL_MJPEG_STREAM_NONE = 0x0,

    E_HAL_MJPEG_MAIN_STREAM_BASE = 0x10,
    E_HAL_MJPEG_MAIN_STREAM0 = E_HAL_MJPEG_MAIN_STREAM_BASE,
    E_HAL_MJPEG_MAIN_STREAM_MAX,

    E_HAL_MJPEG_SUB_STREAM_BASE = 0x20,
    E_HAL_MJPEG_SUB_STREAM0 = E_HAL_MJPEG_SUB_STREAM_BASE,
    E_HAL_MJPEG_SUB_STREAM_MAX,

} HAL_MJPEG_StreamId;


// Dual Decode
typedef struct
{
    HAL_MJPEG_StreamId eStreamId;
    MS_BOOL bUsed;
} HAL_MJPEG_Stream;


/*================================ MJPEG =====================================*/
void VirtualHal_MJPEG_StopVPU(void);
void VirtualHal_MJPEG_ReleaseVPU(void);

MS_BOOL VirtualHal_MJPEG_IsVPUMBoxReady(MS_U8 u8MBox);
MS_BOOL VirtualHal_MJPEG_ReadVPUMBox(MS_U8 u8MBox, MS_U32 *u32Msg);
MS_BOOL VirtualHal_MJPEG_SendVPUMBox(MS_U32 u32Id, MS_U8 u8MBox, MS_U32 u32Msg);
MS_BOOL VirtualHal_MJPEG_ClearVPUMBox(MS_U8 u8MBox);
MJPEG_MSTAR_CHIP_ID VirtualHal_MJPEG_Get_Chip_ID(void);

HAL_MJPEG_StreamId VirtualHal_MJPEG_GetFreeStream(HAL_MJPEG_StreamType eStreamType);

#endif // _VIRTUALHAL_MJPEG_H_
