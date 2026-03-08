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

#ifndef _HAL_SYS_H_
#define _HAL_SYS_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define UNUSED( var )       (void)((var) = (var))

#define CHIPREV_U01     0x00UL
#define CHIPREV_U02     0x01UL

#define EFUSE_ADDRESS_MASK            0x007FUL
#define EFUSE_OFFSET_HDCP_FSM0        0x0010UL
#define EFUSE_OFFSET_HDCP_FSM1        0x0028UL
#define EFUSE_OFFSET_HASH_FSM0        0x0002UL // bank 2

//efuse FSM0
#define REG_EFUSE_FSM0_TRIG           0x2000UL
#define REG_EFUSE_FSM0_CTRL           0x2002UL
#define REG_EFUSE_FSM0_RDATA_15_0     0x2060UL
#define REG_EFUSE_FSM0_RDATA_31_16    0x2062UL
#define REG_EFUSE_FSM0_RDATA_47_32    0x2064UL
#define REG_EFUSE_FSM0_RDATA_63_48    0x2066UL

//efuse FSM1
#define REG_EFUSE_FSM1_TRIG           0x204CUL
#define REG_EFUSE_FSM1_CTRL           0x204EUL
#define REG_EFUSE_FSM1_RDATA_15_0     0x2080UL
#define REG_EFUSE_FSM1_RDATA_31_16    0x2082UL
#define REG_EFUSE_FSM1_RDATA_47_32    0x2084UL
#define REG_EFUSE_FSM1_RDATA_63_48    0x2086UL

//efuse 32*32
#define REG_RESERVED4_15_0            0x2050UL   //0X28
#define REG_RESERVED4_31_16           0x2052UL   //0X29

#define REG_EFUSE_128_RD_15_0         0x2058UL   //0X2c
#define REG_EFUSE_128_RD_31_16        0x205AUL   //0X2d

#define REG_EFUSE_32_RD_15_0          0x205CUL   //0X2e
#define REG_EFUSE_32_RD_31_16         0x205EUL   //0X2f

#define FLAG_EFUSE_DATA_BUSY          0x2000UL   //0x28[13], 1: Busy, 0:Idle
                                                 //0x29[13], 1: Busy, 0:Idle

#define MAX_TIMEOUT_COUNT             100UL

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
	MS_U32  u32ChipTopBase;
	MS_U32  u32PmTopBase;      // REG_PM_BASE
} hal_sys_base_t;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void            SYS_Init(MS_PHY phy64baseaddr);
MS_U8           SYS_GetChipRev(void);
MS_U16          SYS_GetChipID(void);
const SYS_Info* SYS_GetInfo(void);
void            SYS_ResetCPU(void);
MS_U8           SYS_VIF_ReadByteByVDMbox(MS_U32 u32Reg);
void            SYS_VIF_WriteByteByVDMbox(MS_U32 u32Reg, MS_U8 u8Val);
void            SYS_VIF_WriteByteMaskByVDMbox(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Mask);
void            SYS_VIF_WriteRegBitByVDMbox(MS_U32 u32Reg, MS_U8 bEnable, MS_U8 u8Mask);
MS_U16          SYS_VIF_Read2ByteByVDMbox(MS_U32 u32Reg);
void            HAL_SYS_RFAGC_Tristate(MS_BOOL bEnable);
void            HAL_SYS_IFAGC_Tristate(MS_BOOL bEnable);
void            HAL_SYS_SetAGCPadMux(SYS_AGC_PAD_SET eAgcPadMux);
MS_BOOL         HAL_SYS_SetPadMux(SYS_PAD_MUX_SET ePadMuxType,SYS_PAD_SEL ePadSel);
MS_BOOL         HAL_SYS_SetPCMCardDetectMode(SYS_PCM_CD_MODE ePCMCDMode);
MS_BOOL         HAL_SYS_PadMuxTableSuspend(void);
MS_BOOL         HAL_SYS_PadMuxTableResume(void);
MS_BOOL         HAL_SYS_DisableDebugPort(void);
MS_BOOL         HAL_SYS_EnableDebugPort(void);
MS_BOOL         HAL_SYS_SetTSOutClockPhase(MS_U16 u16Val);
MS_BOOL         HAL_SYS_SetTSClockPhase(SYS_PAD_MUX_SET ePadMuxType,MS_U16 u16Val);

void            HAL_SYS_SetEfuseIOMapBase(MS_VIRT u32Base);
MS_U16          HAL_SYS_EfuseRead2Byte(MS_U32 u32RegAddr);
MS_U16          HAL_SYS_EfuseWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
void            HAL_SYS_SetOtpIOMapBase(MS_VIRT u32Base);
MS_U32          HAL_SYS_ReadRSAKey(MS_U16 u16ReadAddr);
MS_BOOL         HAL_SYS_Query(E_SYS_QUERY id);
MS_U32          HAL_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_HASH_INFO index);
E_SYS_CHIP_TYPE HAL_SYS_GetChipType(void);
void HAL_SYS_SetChipType(E_SYS_CHIP_TYPE Type);
MS_U16          HAL_SYS_ReadBrickTerminatorStatus(void);
void            HAL_SYS_WriteBrickTerminatorStatus(MS_U16 u16Status);
void            HAL_SYS_GetEfuseDid(MS_U16 *u16efuse_did);
MS_BOOL         HAL_SYS_ReadEfuseHDCPKey(MS_U16 u16ReadAddr, MS_U32 *u32HDCPKey);
MS_U32          HAL_SYS_GetMemcConfg(MS_U32 eSource, MS_U32 eTiming, MS_BOOL *retEnMemc);
MS_U32          HAL_SYS_GetXcByPartConfg (MS_U32 eSource, MS_U32 eInputTiming, MS_U32 eOutputTiming, MS_BOOL *retEn);
void HAL_SYS_EnableWkEventWOL(void);
void HAL_SYS_DisableWkEventWOL(void);
void HAL_SYS_ResetStatusWOL(void);
MS_BOOL HAL_SYS_GetStatusWOL(void);

#endif // _HAL_SYS_H_

