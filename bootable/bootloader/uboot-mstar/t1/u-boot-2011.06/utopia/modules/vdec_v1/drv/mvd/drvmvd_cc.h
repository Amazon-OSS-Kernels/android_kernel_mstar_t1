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

////////////////////////////////////////////////////////////////////////////////
///
/// @file  drvmvd_cc.h
/// @brief MPEG-2/4 Video Decoder header file for Closed Caption
////////////////////////////////////////////////////////////////////////////////

#ifndef DRV_MVD_CC_H
#define DRV_MVD_CC_H

////////////////////////////////////////////////////////////////////////////////
// Include List
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
////////////////////////////////////////////////////////////////////////////////
/* MVD INFO */
typedef enum
{
    CC_SELECTOR_708_SW              = 0x0000,
    CC_SELECTOR_CCMVD_RINGBUFFER    = 0x0001

} EN_CC_MVD_INFO;

typedef enum
{
    E_CC_MVD_TYPE_708 = 0x0,
    E_CC_MVD_TYPE_608 = 0x1,
    E_CC_MVD_TYOE_NONE = 0xFF
}EN_CC_MVD_TYPE;

////////////////////////////////////////////////////////////////////////////////
// Type & Structure Declaration
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Function Prototype Declaration
////////////////////////////////////////////////////////////////////////////////
void MDrv_CC_Init(void);
void MDrv_CC_CM_SetMVDRB_HWAddr(MS_U32 u32StartAddress, MS_BOOL u8CC608);
void MDrv_CC_CM_SetParsingType(MS_U8 u8Operation, MS_U16 u16BufferSize, MS_BOOL u8CC608);
void MDrv_CC_CM_DisableParsing(MS_BOOL u8CC608);
MS_U32 MDrv_CC_CM_GetMVDRB_HWAddr(MS_BOOL u8CC608);
//void MDrv_CC_CM_ResumeParsingType(void);
//MS_U8 MDrv_CC_CM_GetMVDRB_WrapCount(MS_BOOL u8CC608);
void MDrv_CC_CM_SyncMVDRB_SWAddr2HWAddr(MS_BOOL u8CC608);

/* Polling Mode */
MS_U8 MDrv_CC_CM_GetOverflowStatus(MS_BOOL u8CC608);
void MDrv_CC_CM_SetMVDRB_SWAddr(MS_U32 u32ReadAddress, MS_BOOL u8CC608);
MS_U32 MDrv_CC_PM_GetMVDRB_WriteAddr(MS_BOOL u8CC608);
MS_U32 MDrv_CC_PM_GetMVDRB_ReadAddr(MS_BOOL u8CC608);
void MDrv_CC_PM_SetMVDRB_ReadAddr(MS_U32 u32EachPacketSize, MS_BOOL u8CC608);
MS_BOOL MDrv_CC_CM_GetMVDDecodeStatus(void);

void MDrv_CC_CM_GetInfo(MS_U32 selector, MS_U8 type, MS_U32 *p1, MS_U32 *p2);
MS_BOOL MDrv_CC_CM_Get708Sw(void);
MS_BOOL MDrv_CC_CM_IsMvdRstDone(MS_U8 type);
MS_BOOL MDrv_CC_InfoEnhanceMode(MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif

