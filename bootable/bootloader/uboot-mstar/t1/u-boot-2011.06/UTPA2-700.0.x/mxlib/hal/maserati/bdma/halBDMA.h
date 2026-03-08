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
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_BDMA_H_
#define _HAL_BDMA_H_

////////////////////////////////////////////////////////////////////////////////
/// @file HALbdma.h
/// @brief DRAM byte DMA control hal
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "drvBDMA.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define HAL_BDMA_MS(x)          (3955 * (x))
//two-level fifo
#define HAL_BDMA_DMY_WRCNT      (4)
//Alignment 16
#define HAL_BDMA_ALIGN(x)       (((x)+15) & ~15)
#define BDMA_MEM_FILL           (ENABLE)
#define BDMA_FLASH_COPY         (ENABLE)
#define BDMA_DUMMY_WRCNT        (ENABLE)
#define BDMA_DEV_DATA_WIDTH     (ENABLE)   //device data width
#define BDMA_DMDMCU             (ENABLE)
#define BDMA_TSP                (DISABLE)
#define BDMA_DSP                (ENABLE)
#define BDMA_HK51_1KSRAM        (ENABLE)

#define BDMA_MIN_SIZE           (0x1000)
typedef enum _BDMA_HalDev  //map to BDMA_Dev
{
    E_BDMA_HALDEV_MIU0
    ,E_BDMA_HALDEV_MIU1
    ,E_BDMA_HALDEV_SEARCH
    ,E_BDMA_HALDEV_CRC32
    ,E_BDMA_HALDEV_MEM_FILL
    ,E_BDMA_HALDEV_FLASH
    ,E_BDMA_HALDEV_VDMCU
    ,E_BDMA_HALDEV_DMDMCU
    ,E_BDMA_HALDEV_DSP
    ,E_BDMA_HALDEV_TSP
    ,E_BDMA_HALDEV_1KSRAM_HK51
    ,E_BDMA_HALDEV_MIU2
    ,E_BDMA_HALDEV_MIU3
    ,E_BDMA_HALDEV_NOT_SUPPORT
}BDMA_HalDev;

typedef enum _Hal_BDMA_Ch
{
    E_BDMA_CH_START
    ,E_BDMA_CH0 = E_BDMA_CH_START
    ,E_BDMA_CH1
    ,E_BDMA_CH_END
}BDMA_ChNum;

typedef enum _Hal_BDMA_ACT
{
    E_BDMA_ACT_COPY_MEM
    ,E_BDMA_ACT_COPY_FLASH
    ,E_BDMA_ACT_MEM_FILL
    ,E_BDMA_ACT_SEARCH
    ,E_BDMA_ACT_CRC32
    ,E_BDMA_ACT_MAX
}BDMA_Act;

//Data width
typedef enum _BDMA_Dw
{
    E_BDMA_DW_1BYTE = 0x00
    ,E_BDMA_DW_2BYTE = 0x10
    ,E_BDMA_DW_4BYTE = 0x20
    ,E_BDMA_DW_8BYTE = 0x30
    ,E_BDMA_DW_16BYTE = 0x40
    ,E_BDMA_DW_MAX = E_BDMA_DW_16BYTE
}BDMA_Dw;

//Data width
typedef enum _BDMA_Miu_Alias
{
    E_BDMA_MIU0_ALIAS = 0x00
    ,E_BDMA_MIU1_ALIAS
    ,E_BDMA_MIU2_ALIAS
    ,E_BDMA_MIU3_ALIAS
    ,E_BDMA_MIU_ALIAS_DW_MAX
}BDMA_Miu_Alias;

////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
void HAL_BDMA_Dump_AllReg(void);
void HAL_BDMA_SetIOMapBase(MS_VIRT phy64Base);
MS_U8 HAL_BDMA_ReadByte(MS_PHY phy64RegAddr);
MS_U8 HAL_BDMA_GetDevDw(BDMA_Dev eDev);
MS_U8 HAL_BDMA_GetDevId(BDMA_Dev eDev);
MS_U16 HAL_BDMA_Read2Byte(MS_PHY phy64RegAddr);
MS_U32 HAL_BDMA_Read4Byte(MS_PHY phy64RegAddr);
MS_U32 HAL_BDMA_GetLen(BDMA_Act eAct, MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetSrcAddr(BDMA_Act eAct, MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetDstAddr(BDMA_Act eAct, MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetCmd0(MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetCmd1(MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetCmd2(MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetCRC32(MS_U8 u8Ch);
MS_VIRT HAL_BDMA_GetIOMapBase(void);
MS_U32 HAL_BDMA_GetMatched(MS_PHY phy64SrcAddr, MS_U8 u8Ch);
MS_BOOL HAL_DMA_Is_FlashReady(void);
MS_BOOL HAL_BDMA_WriteRegBit(MS_PHY phy64RegAddr, MS_U8 u8Mask, MS_BOOL bEnable);
MS_BOOL HAL_BDMA_WriteByte(MS_PHY phy64RegAddr, MS_U8 u8Val);
MS_BOOL HAL_BDMA_Write2Byte(MS_PHY phy64u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_BDMA_Write4Byte(MS_PHY phy64u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_BDMA_Stop(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Queued(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Busy(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Int(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Found(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Done(BDMA_Act eAct, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Clear_Status(BDMA_Act eAct, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Enable_INT(MS_BOOL bEnable, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_MOBF(MS_BOOL bEnable, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetSrcAddr(BDMA_Act eAct, MS_PHY phy64RegAddr, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetDstAddr(BDMA_Act eAct, MS_PHY phy64RegAddr, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetLen(BDMA_Act eAct, MS_U32 u32Len, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetSrcDev(MS_U8 u8DevCfg, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetDstDev(MS_U8 u8DevCfg, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetCmd0(MS_U32 u32Cmd, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetCmd1(MS_U32 u32Cmd, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetCmd2(MS_U32 u32Cmd, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Set_Addr_Dec(MS_BOOL bDec, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Set_CRC_Reflect(MS_BOOL bReflect, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetDmyWrCnt(MS_U8 u8Cnt, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_TrigOn(MS_U8 u8Src, MS_U8 u8Dst, MS_U8 u8Ch);
void HAL_BDMA_SetSPIOffsetForMCU(void);
#endif  //_MHAL_BDMA_H_

