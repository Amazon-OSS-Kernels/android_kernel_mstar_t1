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

#ifndef _HAL_SYS_DMD_VD_MBX_H_
#define _HAL_SYS_DMD_VD_MBX_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DMD_VD_MBX_CHIP_U3       0UL
#define DMD_VD_MBX_CHIP_T3       1UL
#define DMD_VD_MBX_CHIP_T7_T8_T9 2UL
#define DMD_VD_MBX_CHIP_T12_T13  3UL
#define DMD_VD_MBX_CHIP_VERSION    DMD_VD_MBX_CHIP_T12_T13

#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T7_T8_T9)
#define MBRegBase   0x012600UL
#define DMDMcuBase  0x003480UL
#elif (DMD_VD_MBX_CHIP_VERSION == DMD_VD_MBX_CHIP_T3)
#define MBRegBase   0x010500UL
#define DMDMcuBase  0x003460UL
#else
#define MBRegBase   0x0UL
#define DMDMcuBase  0x0UL
#endif

#if (DMD_VD_MBX_CHIP_VERSION == DMD_VD_MBX_CHIP_T3)
#define DemodCmdWrReg               0x01UL     // write register.
#define DemodCmdRdReg               0x02UL     // read register.
#define DemodCmd                    0x010500UL//0x110500L
#define DemodAdrL                   0x010501UL//0x110501L
#define DemodAdrH                   0x010502UL//0x110502L
#define DemodData                   0x010503UL//0x110503L
#endif

#define BIN_FOR_ATV      BIT_(0)
#define BIN_FOR_DVBT     BIT_(1)
#define BIN_FOR_DVBC     BIT_(2)
#define BIN_FOR_ATSC     BIT_(3)
#define BIN_FOR_ISDBT     BIT_(4)
#define BIN_FOR_DTMB     BIT_(5)
#define BIN_FOR_DVBS        BIT_(6)
#define BIN_FOR_DVBT2				BIT_(7)
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_SYS_DMD_VD_MBX_Init(void);
MS_U8 HAL_SYS_DMD_VD_MBX_GetType(void);
void HAL_SYS_DMD_VD_MBX_SetType(MS_U8 u8Value);
// ATV
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WaitReady(void);
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_ReadByte(MS_U32 u32Reg, MS_U8 *u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WriteByte(MS_U32 u32Reg, MS_U8 u8Val);
// DVBT & DVBC
void HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake(void);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_ReadByte(MS_U16 u16Addr, MS_U8 *u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WriteByte(MS_U32 u16Addr, MS_U8 u8Data);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_ReadDspReg(MS_U32 u16Addr, MS_U8 *u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WriteDspReg(MS_U32 u16Addr, MS_U8 u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_DBG_ReadReg(MS_U16 u16Addr, MS_U8 *u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_DBG_WriteReg(MS_U32 u16Addr, MS_U8 u8Data);
// ATSC
void HAL_SYS_DMD_VD_MBX_ATSC_WriteByte(MS_U16 u16Addr, MS_U8 u8Data);
MS_U8 HAL_SYS_DMD_VD_MBX_ATSC_ReadByte(MS_U16 u16Addr);

#endif // _HAL_SYS_DMD_VD_MBX_H_

