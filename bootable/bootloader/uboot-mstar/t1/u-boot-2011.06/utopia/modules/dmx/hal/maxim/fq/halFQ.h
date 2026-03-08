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
////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halFQ.h
// @brief  FQ HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_FQ_H__
#define __HAL_FQ_H__

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  FQ Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

// FQ define
#define FQ_NUM 1UL

// HW characteristic
#define MIU_FQ(_addr_)                 ((_addr_)>>4UL)

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define _AND_(flag, bit)            ((flag) &   (bit) )
#define _SET_(flag, bit)            ((flag) |   (bit) )
#define _CLR_(flag, bit)            ((flag) & (~(bit)))

//Skip rush path define
#define HAL_FQ_SKIP_RUSH_DATA_PATH_NON         0x00000000UL
#define HAL_FQ_SKIP_APES_RUSH_DATA             0x00000008UL
#define HAL_FQ_SKIP_APES_B_RUSH_DATA           0x00000010UL
#define HAL_FQ_SKIP_VPES_RUSH_DATA             0x00000020UL
#define HAL_FQ_SKIP_SEC_RUSH_DATA              0x00000040UL
#define HAL_FQ_SKIP_ADP_RUSH_DATA              0x00000080UL
#define HAL_FQ_SKIP_PCR_RUSH_DATA              0x00000100UL
#define HAL_FQ_SKIP_RASP_RUSH_DATA             0x00000200UL
#define HAL_FQ_SKIP_PVR1_RUSH_DATA             HAL_FQ_SKIP_RASP_RUSH_DATA
#define HAL_FQ_SKIP_PCR1_RUSH_DATA             0x00001000UL
#define HAL_FQ_SKIP_PVR3_RUSH_DATA             HAL_FQ_SKIP_PCR1_RUSH_DATA
#define HAL_FQ_SKIP_CFG1_MASK                  0x80000000UL
#define HAL_FQ_SKIP_APES_C_RUSH_DATA           0x80000008UL
#define HAL_FQ_SKIP_APES_D_RUSH_DATA           0x80000010UL
#define HAL_FQ_SKIP_V3DPES_RUSH_DATA           0x80000020UL
#define HAL_FQ_SKIP_PVR2_RUSH_DATA             0x80000200UL

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
MS_BOOL HAL_FQ_SetBank(MS_VIRT virtBankAddr);
void    HAL_FQ_PVR_SetBuf(MS_U32 u32FQEng, MS_PHY phyStartAddr, MS_U32 u32BufSize);
void    HAL_FQ_PVR_SetRushAddr(MS_U32 u32FQEng, MS_PHY phyRushAddr);
void    HAL_FQ_PVR_Start(MS_U32 u32FQEng);
void    HAL_FQ_PVR_Stop(MS_U32 u32FQEng);
void    HAL_FQ_Rush_Enable(MS_U32 u32FQEng);
void    HAL_FQ_Bypass(MS_U32 u32FQEng, MS_U8 u8Bypass);
void    HAL_FQ_SWReset(MS_U32 u32FQEng, MS_U8 u8Reset);
void    HAL_FQ_AddrMode(MS_U32 u32FQEng, MS_U8 u8AddrMode);
void    HAL_FQ_SkipRushData(MS_U32 u32FQEng, MS_U32 u32SkipPath);
MS_U32  HAL_FQ_GetPVRTimeStamp(MS_U32 u32FQEng);
void    HAL_FQ_SetPVRTimeStamp(MS_U32 u32FQEng , MS_U32 u32Stamp);
MS_U32  HAL_FQ_GetRead(MS_U32 u32FQEng);
MS_U32  HAL_FQ_GetWrite(MS_U32 u32FQEng);

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL HAL_FQ_SaveRegs(void);
MS_BOOL HAL_FQ_RestoreRegs(void);
#endif


#endif // #ifndef __HAL_FQ_H__
