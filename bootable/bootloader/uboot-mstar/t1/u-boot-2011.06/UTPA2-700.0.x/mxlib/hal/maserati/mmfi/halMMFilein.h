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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halMMFilein.h
// @brief  Multimedia File In (MMFILEIN) HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_MMFILEIN_H__
#define __HAL_MMFILEIN_H__

#include "regMMFilein.h"

#define MMFI_VER_1_0                     0x0100UL
#define MMFI_VER_2_0                     0x0200UL
#define MMFI_VERSION                     MMFI_VER_2_0

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void   HAL_MMFI_SetBank(MS_VIRT virtBank);

//
// APIs
//
void        HAL_MMFI_PidFlt_Set(MS_U8 u8Eng, MS_U8 u8Idx, MS_U16 u16PID, MS_U32 u32entype);
void        HAL_MMFI_PidFlt_SetPid(MS_U8 u8Eng, MS_U8 u8Idx, MS_U16 u16PID);
void        HAL_MMFI_PidFlt_Enable(MS_U8 u8Eng, MS_U8 u8Idx, MS_U32 u32entype, MS_BOOL benable);
void        HAL_MMFI_PidFlt_Reset(MS_U8 u8Eng, MS_U8 u8Idx);
void        HAL_MMFI_Set_Filein_ReadAddr(MS_U8 u8Eng, MS_PHY phyAddr);
void        HAL_MMFI_Set_Filein_ReadLen(MS_U8 u8Eng, MS_U32 u32len);
void        HAL_MMFI_Set_Filein_Ctrl(MS_U8 u8Eng, MS_U16 u16ctrl, MS_BOOL bEnable);
MS_U16      HAL_MMFI_Get_Filein_Ctrl(MS_U8 u8Eng);
void        HAL_MMFI_Set_FileinTimer(MS_U8 u8Eng, MS_U8 u8timer);
MS_PHY    HAL_MMFI_Get_Filein_WriteAddr(MS_U8 u8Eng);

MS_U16      HAL_MMFI_CmdQ_FIFO_Get_WRCnt(MS_U8 u8Eng);
MS_BOOL     HAL_MMFI_CmdQ_FIFO_IsFull(MS_U8 u8Eng);
MS_BOOL     HAL_MMFI_CmdQ_FIFO_IsEmpty(MS_U8 u8Eng);
MS_U16      HAL_MMFI_CmdQ_FIFO_Get_WRLevel(MS_U8 u8Eng);
void        HAL_MMFI_Cfg_Enable(MS_U8 u8Eng, MS_U32 u32CfgItem, MS_BOOL benable);
void        HAL_MMFI_Cfg_Set(MS_U8 u8Eng, MS_U32 u32CfglItem);
MS_U32      HAL_MMFI_Cfg_Get(MS_U8 u8Eng);
void        HAL_MMFI_Cfg2_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL benable);
void        HAL_MMFI_Cfg2_Set(MS_U8 u8Eng, MS_U16 u16CfglItem);
MS_U16      HAL_MMFI_Cfg2_Get(MS_U8 u8Eng);
void        HAL_MMFI_Get_TsHeaderInfo(MS_U8 u8Eng, MS_U32 *pu32header);
void        HAL_MMFI_Get_APid_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged);
void        HAL_MMFI_Get_APidB_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged);
void        HAL_MMFI_Get_VPid_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged);
void        HAL_MMFI_Get_VPid3D_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged);
void        HAL_MMFI_LPcr2_Set(MS_U8 u8Eng, MS_U32 u32lpcr2);
MS_U32      HAL_MMFI_LPcr2_Get(MS_U8 u8Eng);
MS_U32      HAL_MMFI_TimeStamp_Get(MS_U8 u8Eng);
void        HAL_MMFI_PktChkSize_Set(MS_U8 u8Eng, MS_U16 u16size);
void        HAL_MMFI_MOBF_Set_FileinKey(MS_U8 u8Eng, MS_U16 u16KeyIndex);
void        HAL_MMFI_MOBF_Enable(MS_U8 u8Eng, MS_BOOL bEnable);
void        HAL_MMFI_AU_RemoveDupPkt(MS_U8 u8Eng, MS_BOOL bEnable);
void        HAL_MMFI_TimeStampClk_Set(MS_U8 u8Eng, MS_U32 u32ClkSrc);
//
// General API
void        HAL_MMFI_Reset(void);
void        HAL_MMFI_Reset_SubItem(MS_U16 u16RstItem);
void        HAL_MMFI_Reset_All(void);
void        HAL_MMFI_HWInt_Enable(MS_BOOL benable, MS_U16 u16init);
void        HAL_MMFI_HWInt_Clear(MS_U16 u16Int);
MS_U32      HAL_MMFI_HWInt_Status(void);

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL     HAL_MMFI_SaveRegs(void);
MS_BOOL     HAL_MMFI_RestoreRegs(void);
#endif //MSOS_TYPE_LINUX_KERNEL


#endif // #ifndef __HAL_MMFILEIN_H__


