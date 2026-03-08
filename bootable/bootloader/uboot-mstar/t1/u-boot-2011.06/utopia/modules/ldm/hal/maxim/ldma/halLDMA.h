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

#ifndef _HAL_LDMA_H_
#define _HAL_LDMA_H_

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

#define LDMA_SPI_TRIGGER_STOP        0
#define LDMA_SPI_TRIGGER_ONE_SHOT    1
#define LDMA_SPI_TRIGGER_EVERY_VSYNC 2

#define LDMA_NOCHECKSUM         0
#define LDMA_CHECKSUM_ALL       1
#define LDMA_CHECKSUM_CMD_DATA  2
#define LDMA_CHECKSUM_DATA_ONLY 3


#define  E_LDMA_GETMENULOAD_BUSY         0
#define  E_LDMA_GETMENULOAD_DONE         1
#define  E_LDMA_GETMENULOAD_ABORRED_ONCE 2

typedef struct
{
    MS_U8 u8CurrentCH;
    MS_U32 u32VirtLdmaBaseAddr;
    MS_U32 u32NONPMRegBaseAddr;
} LDMA_BaseAddr_st;

typedef struct
{
    MS_U16 u16Delay0Cnt0;
    MS_U16 u16Delay0Cnt1;
    MS_U16 u16Delay1Cnt0;
    MS_U16 u16Delay1Cnt1;
} LDMA_TrigDelay_st;

MS_BOOL HAL_LDMA_MMIOConfig(MS_U32 u32NONPMRegBaseAddr, MS_U8 u8Channel);
MS_BOOL HAL_LDMA_SetSPITriggerMode(MS_U8 u8Channel, MS_U8 u8TriggerMode);
MS_BOOL HAL_LDMA_SetMenuloadNumber(MS_U8 u8Channel, MS_U32 u32MenuldNum);
MS_BOOL HAL_LDMA_SetSPICommandFormat(MS_U8 u8Channel,MS_U8 u8CmdLen, MS_U16* pu16CmdBuf);
MS_BOOL HAL_LDMA_SetCheckSumMode(MS_U8 u8Channel,MS_U8 u8SumMode);
MS_BOOL HAL_LDMA_GetStatus(MS_U8 u8Channel,MS_U8 u8Index);
MS_BOOL HAL_LDMA_SetTrigDelayCnt(MS_U8 u8Channel, MS_U16 *pu16DelayCnt);
MS_BOOL HAL_LDMA_EnableCS(MS_U8 u8Channel, MS_BOOL bEnable);
#endif
