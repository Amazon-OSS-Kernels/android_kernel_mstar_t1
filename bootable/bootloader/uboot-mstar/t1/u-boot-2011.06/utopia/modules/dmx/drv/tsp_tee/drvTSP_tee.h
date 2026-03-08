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
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSP_tee.h
/// @brief  Transport Stream Processer (TSP) Driver Interface for TEE
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_TSP_TEE_H_
#define _DRV_TSP_TEE_H_

#ifdef __cplusplus
extern "C"
{
#endif

MS_BOOL MDrv_TSP_Tee_Init(void);
MS_BOOL MDrv_TSP_Tee_CopyFw(MS_VIRT virtAddr, MS_PHY phyAddr, MS_U32 u32Size);
MS_BOOL MDrv_TSP_Tee_Set_VQBuffer(MS_PHY phyAddr, MS_U32 u32Size);
MS_BOOL MDrv_TSP_Tee_Set_PVRBuffer(MS_U8 u8PvrId, MS_PHY phyAddr0, MS_U32 u32Size0, MS_PHY phyAddr1, MS_U32 u32Size1);
MS_BOOL MDrv_TSP_Tee_Get_PVRWriteAddr(MS_U8 u8PvrId, MS_PHY *pphyAddr);
MS_U32  MDrv_TSP_Tee_FileEngId_Vir2Phy(MS_U8 u8VirEngId);
MS_BOOL MDrv_TSP_Tee_Set_FileinBuffer(MS_U8 u8EngId, MS_PHY phyAddr, MS_U32 u32Size);
MS_BOOL MDrv_TSP_Tee_Set_MMFIBuffer(MS_U8 u8EngId, MS_PHY phyAddr, MS_U32 u32Size);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif // _DRV_TSP_TEE_H_
