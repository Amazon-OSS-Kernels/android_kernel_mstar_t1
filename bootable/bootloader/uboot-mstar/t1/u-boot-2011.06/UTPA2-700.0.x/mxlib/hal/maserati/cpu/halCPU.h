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

#ifndef _HAL_CPU_H_
#define _HAL_CPU_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define CHIPREV_U01     0x00
#define CHIPREV_U02     0x01

#define BIT0            BIT(0)
#define BIT1            BIT(1)
#define BIT2            BIT(2)
#define BIT3            BIT(3)
#define BIT4            BIT(4)
#define BIT5            BIT(5)
#define BIT6            BIT(6)
#define BIT7            BIT(7)
#define BIT8            BIT(8)
#define BIT9            BIT(9)
#define BIT10           BIT(10)
#define BIT11           BIT(11)
#define BIT12           BIT(12)
#define BIT13           BIT(13)
#define BIT14           BIT(14)
#define BIT15           BIT(15)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_COPRO_RegSetBase(MS_VIRT u32Base);
MS_U8 HAL_COPRO_RegReadByte(MS_U32 u32RegAddr);
MS_U16 HAL_COPRO_RegRead2Byte(MS_U32 u32RegAddr);
MS_U32 HAL_COPRO_RegRead4Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_COPRO_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_COPRO_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_COPRO_RegWrite4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);
void HAL_COPRO_Disable(void);
void HAL_COPRO_Enable(MS_U32 u32_ADR);
void HAL_COPRO_Init_Front(void);
void HAL_COPRO_Init_End(MS_U32 u32_ADR);

MS_U8   HAL_CPU_NPM_RegReadByte(MS_U32 u32RegAddr);
MS_BOOL HAL_CPU_NPM_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_U16  HAL_CPU_NPM_RegRead2Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_CPU_NPM_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_U8   HAL_CPU_PM_RegReadByte(MS_U32 u32RegAddr);
MS_BOOL HAL_CPU_PM_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_U16  HAL_CPU_PM_RegRead2Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_CPU_PM_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);

#if 1//frcr2_integration###
void HAL_FRCR2_Disable(void);
void HAL_FRCR2_Enable(MS_U32 u32_ADR);
void HAL_FRCR2_Init_Front(void);
void HAL_FRCR2_Init_End(MS_U32 u32_ADR);
#endif

MS_U32  HAL_CPU_QueryClock(void);

#endif // _HAL_CPU_H_

