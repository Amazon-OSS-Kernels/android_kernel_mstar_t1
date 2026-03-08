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
/// @file   apiDMS.h
/// @brief  DMS Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_DMS_H_
#define _API_DMS_H_

#include "MsCommon.h"
#include "apiDMS_Type.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
EN_DMS_RESULT MApi_DMS_Init(ST_DMS_INITDATA *pstDMS_InitData);
EN_DMS_RESULT MApi_DMS_SetMemory(ST_DMS_SET_MEMORY_TYPE* pstDMS_SetMemType);

EN_DMS_RESULT MApi_DMS_CreateWindow(ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID);

EN_DMS_RESULT MApi_DMS_SetDigitalDecodeSignalInfo(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat);

EN_DMS_RESULT MApi_DMS_ClearDigitalDecodeSignalInfo(MS_U32 u32WindowID);

EN_DMS_RESULT MApi_DMS_DestroyWindow(MS_U32 u32WindowID);

EN_DMS_RESULT MApi_DMS_SetWindow(MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info);

EN_DMS_RESULT MApi_DMS_GetWindowInfo(MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info);

EN_DMS_RESULT MApi_DMS_Video_Flip(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat);

EN_DMS_RESULT MApi_DMS_SetZOrder(MS_U32 u32WindowID, ST_DMS_ZORDER_INFO *pstZorderInfo);

EN_DMS_RESULT MApi_DMS_Video_Freeze(MS_U32 u32WindowID, ST_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze);

EN_DMS_RESULT MApi_DMS_Video_Mute(MS_U32 u32WindowID, ST_DMS_MUTE_WINDOW_INFO *pstWindowMute);

EN_DMS_RESULT MApi_DMS_Set_MuteColor(MS_U32 u32WindowID, ST_DMS_MUTE_COLOR_INFO *pstMuteColor);

EN_DMS_RESULT MApi_DMS_GetStatus(EN_DMS_STATUS *peStatus);
EN_DMS_RESULT MApi_DMS_GetCapability(EN_DMS_CAPABILITY *peCapability);

EN_DMS_RESULT MApi_DMS_GetExistedWindows(MS_U32 *pu32WindowNums);
EN_DMS_RESULT MApi_DMS_SetOutputLayer(ST_DMS_WINDOW *pstLayer);
EN_DMS_RESULT MApi_DMS_Set_3D_Mode(MS_U32 u32WindowID, ST_DMS_3D_INFO *pst3DInfo);

EN_DMS_RESULT MApi_DMS_Set_CaptureInfo(MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo);
EN_DMS_RESULT MApi_DMS_Get_CaptureBuffer(MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);
EN_DMS_RESULT MApi_DMS_Release_CaptureBuffer(MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);



#ifdef __cplusplus
}
#endif

#endif

