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

#ifndef _GOPSCD_H_
#define _GOPSCD_H_



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
    MAPI_CMD_GOPSCD_INIT,
    MAPI_CMD_GOPSCD_SCALING,
    MAPI_CMD_GOPSCD_SET_INFO,
    MAPI_CMD_GOPSCD_GET_INFO,
    MAPI_CMD_GOPSCD_SET_HVSPSIZE,
    MAPI_CMD_GOPSCD_SET_SKIPPIXEL,
}E_GOPSCD_API_CMD_TYPE;

typedef enum
{
    E_GOPSCD_CMD_DISABLE       = 0x0,
    E_GOPSCD_CMD_ENABLE        = 0x1,
    E_GOPSCD_CMD_SCALING_ONCE  = 0x2,
}E_GOPSCD_SCALING_CMD;

typedef struct
{
	GOPSC_Result bReturnValue;
}GOPSCD_INIT_INFO, *PGOPSCD_INIT_INFO;

typedef struct
{
	E_GOPSCD_SCALING_CMD gopscCmd;
	GOPSC_Result bReturnValue;
	MS_U8 u8FrameCount;
}GOPSCD_SCALING, *PGOPSCD_SCALING;

typedef struct
{
	GOPSC_Info* gopscInfo;
	GOPSC_Result bReturnValue;
}GOPSCD_SET_INFO, *PGOPSCD_SET_INFO;

typedef struct
{
	GOPSC_Info* gopscInfo;
	GOPSC_Result bReturnValue;
}GOPSCD_GET_INFO, *PGOPSCD_GET_INFO;

typedef struct
{
	GOPSC_SizeInfo* sizeInfo;
	MS_U32 flag;
	GOPSC_Result bReturnValue;
}GOPSCD_SET_HVSPSZIE, *PGOPSCD_SET_HVSPSZIE;

typedef struct
{
	GOPSC_SkipPixelInfo* skippixelInfo;
	MS_U32 flag;
	GOPSC_Result bReturnValue;
}GOPSCD_SET_SKIPPIXEL, *PGOPSCD_SET_SKIPPIXEL;

#ifdef __cplusplus
}
#endif

#endif // _API_GOPSCD_H_

