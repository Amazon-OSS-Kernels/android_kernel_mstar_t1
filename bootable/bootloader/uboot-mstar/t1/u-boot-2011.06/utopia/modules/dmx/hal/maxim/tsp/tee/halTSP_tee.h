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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halTSP_tee.h
// @brief  Transport Stream Processer (TSP) HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_TSP_TEE_H__
#define __HAL_TSP_TEE_H__

#include "MsCommon.h"

//--------------------------------------------------------------------------------------------------
//  Macro and Define
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Function
//--------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSP_Tee_SetBank(MS_VIRT virtBankAddr);
MS_BOOL HAL_TSP_Tee_Set_FWBuf(MS_PHY phyAddr, MS_U32 u32Size);
MS_BOOL HAL_TSP_Tee_Set_VQBuf(MS_PHY phyAddr, MS_U32 u32Size);
MS_BOOL HAL_TSP_Tee_Set_PvrBuf(MS_U8 u8PvrId, MS_PHY phyAddr0, MS_U32 u32Size0, MS_PHY phyAddr1, MS_U32 u32Size1);
MS_BOOL HAL_TSP_Tee_Get_PVRWriteAddr(MS_U8 u8PvrId, MS_PHY *pphyAddr);
MS_U32  HAL_TSP_Tee_FilePath2Tsif_Mapping(MS_U8 u8FileEng);
MS_BOOL HAL_TSP_Tee_Set_FileinBuf(MS_U8 u8Id, MS_PHY phyAddr, MS_U32 u32Size);
MS_BOOL HAL_TSP_Tee_Set_MMFIBuf(MS_U8 u8Id, MS_PHY phyAddr, MS_U32 u32Size);

#endif // #ifndef __HAL_TSP_TEE_H_
