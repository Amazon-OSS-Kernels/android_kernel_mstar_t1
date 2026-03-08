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
/// @file   apiDMS_v2.h
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DMS_V2_H_
#define _DMS_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

/// DMS CMD
typedef enum
{
    //DMS command
    E_DMS_CMD_INIT,
    E_DMS_CMD_SETMEMORY,
    E_DMS_CMD_CREATEWINDOW,
    E_DMS_CMD_SETDIGITALDECODESIGNALINFO,
    E_DMS_CMD_CLEARDIGITALDECODESIGNALINFO,
    E_DMS_CMD_DESTORYWINDOW,
    E_DMS_CMD_SETWINDOW,
    E_DMS_CMD_GETWINDOWINFO,
    E_DMS_CMD_VIDEOFLIP,
    E_DMS_CMD_SETZORDER,
    E_DMS_CMD_VIDEOMUTE,
    E_DMS_CMD_SETMUTECOLOR,
    E_DMS_CMD_VIDEOFREEZE,
    E_DMS_CMD_GETSTATUS,
    E_DMS_CMD_GETCAPS,
    E_DMS_CMD_GETEXISTEDWINDOW,
    E_DMS_CMD_SETOUTPUTLAYER,
    E_DMS_CMD_SET3DMODE,
    E_DMS_CMD_SETCAPTUREINFO,
    E_DMS_CMD_GETCAPTUREBUFFER,
    E_DMS_CMD_RELEASECAPTUREBUFFER,
    E_DMS_CMD_NUM,
    E_DMS_CMD_MAX = E_DMS_CMD_NUM,
}EN_DMS_IOCTL_CMDS;

typedef struct DLL_PACKED
{
    ST_DMS_INITDATA *pstDMS_InitData;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_INIT,*PST_DMS_INIT;

typedef struct DLL_PACKED
{
    ST_DMS_SET_MEMORY_TYPE *pstDMS_SetMemType;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETMEMTYPE,*PST_DMS_SETMEMTYPE;

typedef struct DLL_PACKED
{
    ST_DMS_WINDOW* pstOutputWin;
    ST_DMS_CREATE_WIN_INFO* pstCreateWin_Info;
    MS_U32* pu32WindowID;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_CREATEWIN,*PST_DMS_CREATEWIN;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETDIGIDECINFO,*PST_DMS_SETDIGIDECINFO;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_CLEARDIGIDECINFO,*PST_DMS_CLEARDIGIDECINFO;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_DESTORYWIN,*PST_DMS_DESTORYWIN;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETWIN,*PST_DMS_SETWIN;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETWININFO,*PST_DMS_GETWININFO;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_FLIP,*PST_DMS_FLIP;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_ZORDER_INFO *pstZorderInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_ZORDER,*PST_DMS_ZORDER;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_FREEZE,*PST_DMS_FREEZE;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_MUTE_WINDOW_INFO *pstWindowMute;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_MUTE,*PST_DMS_MUTE;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_MUTE_COLOR_INFO *pstMuteColor;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_MUTECOLOR,*PST_DMS_MUTECOLOR;

typedef struct DLL_PACKED
{
    EN_DMS_STATUS *peStatus;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETSTATUS,*PST_DMS_GETSTATUS;

typedef struct DLL_PACKED
{
    EN_DMS_CAPABILITY *peCapability;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETCAPS,*PST_DMS_GETCAPS;

typedef struct DLL_PACKED
{
    MS_U32 *pu32WindowNums;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETEXISTWIN,*PST_DMS_GETEXISTWIN;

typedef struct DLL_PACKED
{
    ST_DMS_WINDOW *pstLayer;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETOUTPUTLAYER,*PST_DMS_SETOUTPUTLAYER;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_3D_INFO *pst3DInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SET3DMODE,*PST_DMS_SET3DMODE;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_SETCAPTUREINFO,*PST_DMS_SETCAPTUREINFO;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_CAPTURE_INFO *pstCaptureInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_GETCAPTUREBUF,*PST_DMS_GETCAPTUREBUF;

typedef struct DLL_PACKED
{
    MS_U32 u32WindowID;
    ST_DMS_CAPTURE_INFO *pstCaptureInfo;
    EN_DMS_RESULT eReturnValue;
} ST_DMS_RELEASECAPTUREBUF,*PST_DMS_RELEASECAPTUREBUF;

#ifdef __cplusplus
}
#endif

#endif

