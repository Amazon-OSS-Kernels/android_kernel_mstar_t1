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
/// @file   apiGOP.h
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CH34_H_
#define _CH34_H_



#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// GOPSCD Version

/// GOPSCD CMD
typedef enum
{
    //gop property
    MAPI_CMD_CH34_INIT,
    MAPI_CMD_CH34_SETMODE,
    MAPI_CMD_CH34_WRITETABLE,
    MAPI_CMD_CH34_TURNONOFF,
}E_CH34_API_CMD_TYPE;

typedef struct
{
	CH34_Drv_Result bReturnValue;
}CH34_INIT_INFO, *PCH34_INIT_INFO;

typedef struct
{
	E_CH34_CHANNEL channel;
	E_CH34_VIDEO_SYS videosys;
	E_CH34_AUDIO_SYS audiosys;
	CH34_Drv_Result bReturnValue;
}CH34_SET_MODE_INFO, *PCH34_SET_MODE_INFO;

typedef struct
{
	MS_U8* pTable;
	CH34_Drv_Result bReturnValue;
}CH34_WRITE_TABLE_INFO, *PCH34_WRITE_TABLE_INFO;

typedef struct
{
	MS_BOOL bOn;
	CH34_Drv_Result bReturnValue;
}CH34_TURN_ONOFF_INFO, *PCH34_TURN_ONOFF_INFO;

#ifdef __cplusplus
}
#endif

#endif // _API_GOPSCD_H_

