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

#ifndef _HAL_LDM_H_
#define _HAL_LDM_H_

#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

#define LDMA0 0
#define LDMA1 1

void HAL_LDM_MMIOConfig(MS_U32 u32NONPMRegBaseAddr, MS_U8 u8ClkHz);
void HAL_LDM_Enable(MS_BOOL Enable);
void HAL_LDM_SetMIUPackOffset(MS_U8 u8Channel,MS_U8 u8PackOffset);
void HAL_LDM_SetMIUPackLength(MS_U8 u8Channel,MS_U8 u8PackLength);
void HAL_LDM_SetYoffEnd(MS_U8 u8Channel,MS_U8 u8YoffEnd);
void HAL_LDM_SetDmaEnable(MS_U8 u8Channel, MS_BOOL Enable);
void HAL_LDM_SetBlHeightDMA(MS_U8 u8Height);
void HAL_LDM_SetBlWidthDMA(MS_U8 u8Width);
void HAL_LDM_SetLEDBufBaseOffset(MS_U32 u32DataOffset);
void HAL_LDM_SetLDFAddr(MS_U8 u8AddIndex, MS_U32 u32LDFAddr_l,MS_U32 u32LDFAddr_r);
void HAL_LDM_SetLDBAddr(MS_U8 u8AddIndex, MS_U32 u32LDBAddr_l,MS_U32 u32LDBAddr_r);
void HAL_LDM_SetEdge2DAddr(MS_U32 u32Edge2DAddr);

#endif
