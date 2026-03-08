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

#ifndef _HAL_PM_H_
#define _HAL_PM_H_

////////////////////////////////////////////////////////////////////////////////
/// @file halPM.h
/// @brief PM control hal
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    PM_RTCIDX_0 = 0,
    PM_RTCIDX_1 = 1,
    PM_RTCIDX_MAX = PM_RTCIDX_1,

} PM_RtcIndex;
////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_PM_ReadByte(MS_U32 u32RegAddr);
MS_U16 HAL_PM_Read2Byte(MS_U32 u32RegAddr);
MS_U32 HAL_PM_Read4Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_PM_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_PM_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_PM_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_PM_ReadRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask);
MS_BOOL HAL_PM_WriteRegBit(MS_U32 u32RegAddr, MS_BOOL bEnable, MS_U8 u8Mask);
MS_BOOL HAL_PM_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk);
void HAL_PM_SetIOMapBase(MS_VIRT virtBase);
MS_VIRT HAL_PM_GetIOMapBase(void);
//void HAL_PM_Dump_AllReg(void);
void HAL_PM_RtcInit(MS_U8 u8RtcIndex, MS_U32 u32RtcCtrlWord);
void HAL_PM_RTC_DisableInit(MS_U8 u8RtcIndex);
void HAL_PM_RtcSetCounter(MS_U8 u8RtcIndex, MS_U32 u32RtcSetCounter);
MS_U32 HAL_PM_RtcGetCounter(MS_U8 u8RtcIndex);
void HAL_PM_RtcSetMatchCounter(MS_U8 u8RtcIndex, MS_U32 u32RtcSetMatchCounter);
MS_U32 HAL_PM_RtcGetMatchCounter(MS_U8 u8RtcIndex);
void HAL_PM_SetSPIOffsetForMCU(MS_U32 BANK);
void HAL_PM_SetSRAMOffsetForMCU(void);
MS_BOOL HAL_PM_SetDRAMOffsetForMCU(MS_U32 u32Offset);
MS_U8 HAL_PM_PowerOnMode(void);
MS_U8 HAL_PM_GetWakeupSource(void);
MS_U8 HAL_PM_GetWakeupKey(void);
void HAL_PM_Disable51(void);
MS_BOOL HAL_PM_isRunning(void);
void HAL_PM_SetBackGroundTrigger(MS_BOOL bTrigger);
MS_BOOL HAL_PM_GetBackGroundActive(void);

void HAL_PM_GPIO4_SetPower(MS_BOOL bOn);

void HAL_PM_SetScratchPad(MS_U8 u8PadNum,MS_U16 u16Val);
MS_BOOL HAL_PM_GetScratchPad(MS_U8 u8PadNum,MS_U16 *u16Val);

MS_BOOL HAL_PM_GetVersion(MS_U8 *u8Val);
MS_U16 HAL_PM_GetSRAMSize(void);
MS_U8 HAL_PM_GetIRPowerOnKey(void);
void HAL_PM_SetWakupDevice(MS_U16 u16WakeSrc);
#endif  //_HAL_PM_H_

