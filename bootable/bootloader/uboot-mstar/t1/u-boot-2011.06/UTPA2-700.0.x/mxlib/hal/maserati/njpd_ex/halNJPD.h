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
/// @file   halNJPD.h
/// @brief  NJPD hal interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_NJPD_H_
#define _HAL_NJPD_H_

//#include "njpeg_def.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//Version
#define NJPD_HAL_VERSION     "NJPD.HAL.00117630"

// VPU MailBox
#define VPU_HI_MBOX0        0
#define VPU_HI_MBOX1        1
#define VPU_RISC_MBOX0      2
#define VPU_RISC_MBOX1      3

//#ifndef HAL_MIU1_BASE
//#define HAL_MIU1_BASE       0x10000000 // 256M
//#endif
#define NJPD_MIU_ADDR_MASK   (HAL_MIU1_BASE - 1)


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// NJPD debug level enum
typedef enum
{
    E_NJPD_DEBUG_HAL_NONE = 0x0
  , E_NJPD_DEBUG_HAL_MSG  = 0x04
  , E_NJPD_DEBUG_HAL_ERR  = 0x08
} NJPD_HalDbgLevel;


typedef enum
{
    E_NJPD_OUTPUT_ORIGINAL  = 0x0
  , E_NJPD_OUTPUT_YC_SWAP   = 0x01
  , E_NJPD_OUTPUT_UV_SWAP   = 0x02
  , E_NJPD_OUTPUT_UV_7BIT   = 0x03
  , E_NJPD_OUTPUT_UV_MSB    = 0x04
} NJPD_OutputFormat;

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


typedef struct
{
    MS_U8* pu8HalVer;
    MS_U8* pu8FwVer;
}NJPD_HAL_Version;

// Dual Decode
typedef struct
{
    HAL_MJPEG_StreamId eStreamId;
    MS_BOOL bUsed;
} HAL_MJPEG_Stream;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void HAL_NJPD_SetMIU(MS_U8 u8Idx);
void HAL_NJPD_JPDSelect(MS_BOOL bNJPD);
void HAL_NJPD_Set_GlobalSetting00(MS_U16 u16Value);
MS_U16 HAL_NJPD_Get_GlobalSetting00(void);
void HAL_NJPD_Set_GlobalSetting01(MS_U16 u16Value);
MS_U16 HAL_NJPD_Get_GlobalSetting01(void);
void HAL_NJPD_Set_GlobalSetting02(MS_U16 u16Value);
MS_U16 HAL_NJPD_Get_GlobalSetting02(void);

void HAL_NJPD_SetMRCBuf0_StartLow(MS_U16 u16Value);
void HAL_NJPD_SetMRCBuf0_StartHigh(MS_U16 u16Value);
void HAL_NJPD_SetMRCBuf0_EndLow(MS_U16 u16Value);
void HAL_NJPD_SetMRCBuf0_EndHigh(MS_U16 u16Value);
void HAL_NJPD_SetMRCBuf1_StartLow(MS_U16 u16Value);
void HAL_NJPD_SetMRCBuf1_StartHigh(MS_U16 u16Value);
void HAL_NJPD_SetMRCBuf1_EndLow(MS_U16 u16Value);
void HAL_NJPD_SetMRCBuf1_EndHigh(MS_U16 u16Value);

void HAL_NJPD_SetMRCStart_Low(MS_U16 u16Value);
void HAL_NJPD_SetMRCStart_High(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetMWCBuf_StartLow(void);
MS_U16 HAL_NJPD_GetMWCBuf_StartHigh(void);
MS_U16 HAL_NJPD_GetMWCBuf_WritePtrLow(void);
MS_U16 HAL_NJPD_GetMWCBuf_WritePtrHigh(void);


void HAL_NJPD_SetMWCBuf_StartLow(MS_U16 u16Value);
void HAL_NJPD_SetMWCBuf_StartHigh(MS_U16 u16Value);
void HAL_NJPD_SetPic_H(MS_U16 u16Value);
void HAL_NJPD_SetPic_V(MS_U16 u16Value);

void HAL_NJPD_ClearEventFlag(MS_U16 u16Value);
void HAL_NJPD_ForceEventFlag(MS_U16 u16Value);
void HAL_NJPD_MaskEventFlag(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetEventFlag(void);

void HAL_NJPD_SetROI_H(MS_U16 u16Value);
void HAL_NJPD_SetROI_V(MS_U16 u16Value);
void HAL_NJPD_SetROIWidth(MS_U16 u16Value);
void HAL_NJPD_SetROIHeight(MS_U16 u16Value);
void HAL_NJPD_SetClock(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetClock(void);
void HAL_NJPD_SetClockGate(MS_U16 u16Value);
void HAL_NJPD_PowerOn(void);
void HAL_NJPD_PowerOff(void);
void HAL_NJPD_InitRegBase(void);
void HAL_NJPD_CreateMutex(void);
void HAL_NJPD_DeleteMutex(void);
void HAL_NJPD_SetRSTIntv(MS_U16 u16Value);
void HAL_NJPD_SetDbgLevel(MS_U8 u8DbgLevel);
void HAL_NJPD_GetLibVer(NJPD_HAL_Version *pHalVer);
void HAL_NJPD_Rst(void);
void HAL_NJPD_SetMWBuffLineNum(MS_U16 u16Value);
MS_U32 HAL_NJPD_GetCurMRCAddr(void);
MS_U16 HAL_NJPD_GetCurRow(void);
MS_U16 HAL_NJPD_GetCurCol(void);
void HAL_NJPD_SetWriteProtect(MS_BOOL enable);
void HAL_NJPD_SetAutoProtect(MS_BOOL enable);

void HAL_NJPD_SetMRBurstThd(MS_U16 u16Value);
void HAL_NJPD_Set_MARB06(MS_U16 u16Value);
MS_U16 HAL_NJPD_Get_MARB06(void);
void HAL_NJPD_Set_MARB07(MS_U16 u16Value);
MS_U16 HAL_NJPD_Get_MARB07(void);

void HAL_NJPD_SetWPENUBound_0_L(MS_U16 u16Value);
void HAL_NJPD_SetWPENUBound_0_H(MS_U16 u16Value);
void HAL_NJPD_SetWPENLBound_0_L(MS_U16 u16Value);
void HAL_NJPD_SetWPENLBound_0_H(MS_U16 u16Value);

void HAL_NJPD_SetSpare00(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetSpare00(void);
void HAL_NJPD_SetSpare01(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetSpare01(void);
void HAL_NJPD_SetSpare02(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetSpare02(void);
void HAL_NJPD_SetSpare03(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetSpare03(void);
void HAL_NJPD_SetSpare04(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetSpare04(void);
void HAL_NJPD_SetSpare05(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetSpare05(void);
void HAL_NJPD_SetSpare06(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetSpare06(void);
void HAL_NJPD_SetSpare07(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetSpare07(void);
void HAL_NJPD_SetWriteOneClearReg(MS_U16 u16Value);
void HAL_NJPD_SetWriteOneClearReg_2(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetWriteOneClearReg(void);
void HAL_NJPD_SetHTableStart_Low(MS_U16 u16Value);
void HAL_NJPD_SetHTableStart_High(MS_U16 u16Value);
void HAL_NJPD_SetQTableStart_Low(MS_U16 u16Value);
void HAL_NJPD_SetQTableStart_High(MS_U16 u16Value);
void HAL_NJPD_SetGTableStart_Low(MS_U16 u16Value);
void HAL_NJPD_SetGTableStart_High(MS_U16 u16Value);
void HAL_NJPD_SetHTableSize(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetHTableSize(void);
void HAL_NJPD_SetRIUInterface(MS_U16 u16Value);
MS_U16 HAL_NJPD_GetRIUInterface(void);
MS_U16 HAL_NJPD_TBCReadData_L(void);
MS_U16 HAL_NJPD_TBCReadData_H(void);
void HAL_NJPD_SetIBufReadLength(MS_U8 u8Min, MS_U8 u8Max);
void HAL_NJPD_SetCRCReadMode(void);
void HAL_NJPD_SetCRCWriteMode(void);
MS_U8 HAL_NJPD_GetHandshakeCnt(void);
void HAL_NJPD_Handshake_SWRowCountMode(MS_U8 u8Cnt);
void HAL_NJPD_Handshake_HWRowCountMode(void);
void HAL_NJPD_SetOutputFormat(MS_BOOL bRst, NJPD_OutputFormat eOutputFormat);
void HAL_NJPD_SetMTLBMode(MS_U8 u8MTLB);
void HAL_NJPD_SetVerificationMode(NJPD_VerificationMode VerificationMode);
NJPD_VerificationMode HAL_NJPD_GetVerificationMode(void);
void HAL_NJPD_Debug(void);
void HAL_NJPD_SetNJPDInstance(JPD_Number JPDNum);
MS_BOOL HAL_NJPD_IsNeedToPatch(NJPD_PATCH_INDEX eIndex);
MS_U8 HAL_NJPD_JPDCount(void);

#endif // _HAL_NJPD_H_

