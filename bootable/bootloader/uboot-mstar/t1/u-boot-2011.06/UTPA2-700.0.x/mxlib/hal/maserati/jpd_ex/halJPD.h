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
/// @file   halJPD.h
/// @brief  JPD hal interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_JPD_H_
#define _HAL_JPD_H_


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//Version
#define JPD_HAL_VERSION     "JPD.HAL.00117630"

// VPU MailBox
#define VPU_HI_MBOX0        0
#define VPU_HI_MBOX1        1
#define VPU_RISC_MBOX0      2
#define VPU_RISC_MBOX1      3

//#ifndef HAL_MIU1_BASE
//#define HAL_MIU1_BASE       0x10000000 // 256M
//#endif
#define JPD_MIU_ADDR_MASK   (HAL_MIU1_BASE - 1)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// JPD debug level enum
typedef enum
{
    E_JPD_DEBUG_HAL_NONE = 0x0
  , E_JPD_DEBUG_HAL_MSG  = 0x04
  , E_JPD_DEBUG_HAL_ERR  = 0x08
} JPD_HalDbgLevel;

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
}JPD_HAL_Version;

// Dual Decode
typedef struct
{
    HAL_MJPEG_StreamId eStreamId;
    MS_BOOL bUsed;
} HAL_MJPEG_Stream;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void HAL_JPD_SelectOJPD(void);
void HAL_JPD_SetMIU(MS_U8 u8Idx);
void HAL_JPD_Set_S_Config(MS_U16 u16Value);
MS_U16 HAL_JPD_Get_S_Config(void);
void HAL_JPD_Set_M_Config(MS_U16 u16Value);
MS_U16 HAL_JPD_Get_M_Config(void);
void HAL_JPD_SetIntEn(MS_U16 u16Value);
void HAL_JPD_SetMRCBufFloorLow(MS_U16 u16Value);
void HAL_JPD_SetMRCBufFloorHigh(MS_U16 u16Value);
void HAL_JPD_SetMRCBufCeilLow(MS_U16 u16Value);
void HAL_JPD_SetMRCBufCeilHigh(MS_U16 u16Value);
void HAL_JPD_SetRCSMAddrLow(MS_U16 u16Value);
void HAL_JPD_SetRCSMAddrHigh(MS_U16 u16Value);
void HAL_JPD_SetMWCBufStAddrLow(MS_U16 u16Value);
void HAL_JPD_SetMWCBufStAddrHigh(MS_U16 u16Value);
void HAL_JPD_SetPic_H(MS_U16 u16Value);
void HAL_JPD_SetPic_V(MS_U16 u16Value);
void HAL_JPD_SetEventFlag(MS_U16 u16Value);
MS_U16 HAL_JPD_GetEventFlag(void);
void HAL_JPD_SetROI_H(MS_U16 u16Value);
void HAL_JPD_SetROI_V(MS_U16 u16Value);
void HAL_JPD_SetROIWidth(MS_U16 u16Value);
void HAL_JPD_SetROIHeight(MS_U16 u16Value);
#if 0 // Unused function
MS_U16 HAL_JPD_GetCurMadrLow(void);
MS_U16 HAL_JPD_GetCurMadrHigh(void);
#endif
void HAL_JPD_SetClock(MS_U16 u16Value);
MS_U16 HAL_JPD_GetClock(void);
void HAL_JPD_PowerOn(void);
void HAL_JPD_PowerOff(void);
void HAL_JPD_InitRegBase(void);
void HAL_JPD_CreateMutex(void);
void HAL_JPD_DeleteMutex(void);
void HAL_JPD_SetRSTIntv(MS_U16 u16Value);
MS_U16 HAL_JPD_GetCurVidx(void);
void HAL_JPD_Set_TID_Addr(MS_U16 u16Value);
void HAL_JPD_Set_TID_Dat(MS_U16 u16Value);
MS_U16 HAL_JPD_Get_TID_Dat(void);
void HAL_JPD_SetDbgLevel(MS_U8 u8DbgLevel);
void HAL_JPD_GetLibVer(JPD_HAL_Version *pHalVer);
void HAL_JPD_Rst(void);
void HAL_JPD_SetMWBuffLineNum(MS_U16 u16Value);
MS_U32 HAL_JPD_GetCurMRCAddr(void);
MS_U16 HAL_JPD_GetCurRow(void);
MS_U16 HAL_JPD_GetCurCol(void);
void HAL_JPD_SetWriteProtect(MS_BOOL enable);
#if (JPD_SUPPORT_AUTO_PROTECT==true)
void HAL_JPD_SetAutoProtect(MS_BOOL enable);
void HAL_JPD_SetWPENAddrHigh(MS_U16 u16Value);
void HAL_JPD_SetWPENAddrLow(MS_U16 u16Value);
#endif
void HAL_JPD_SetSpare(MS_U16 u16Value);
MS_U16 HAL_JPD_GetSpare(void);


#endif // _HAL_JPD_H_

