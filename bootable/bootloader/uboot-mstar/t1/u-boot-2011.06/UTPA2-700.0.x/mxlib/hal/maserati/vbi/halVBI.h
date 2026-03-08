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
#ifndef _HAL_VBI_H
#define _HAL_VBI_H

 /********************************************************************************/
/*                                                        Functions                                                                   */
/********************************************************************************/
void VBI_TTXInit(MS_VIRT ptrAddr);
MS_BOOL VBI_TTX_CheckCircuitReady(void);
MS_U16 VBI_TTX_GetPacketCount(void);
MS_U16 VBI_GetWSS_Count(void);
MS_U16 VBI_GetVPS_Count(void);
void VBI_TTX_InitSlicer(MS_PHY addr, MS_U16 packetCount);
void VBI_TTX_EnableSlicer(MS_BOOL bEnable);
MS_U8 VBI_TTX_GetHardware_Indication(void);
void VBI_GetVPS_Data(MS_U8 *byte1, MS_U8 *byte2, MS_U8 *byte3, MS_U8 *byte4);
MS_U16 VBI_GetWSS_Data(void);
void VBI_Set_PalNC_VideoStandard(void);
void VBI_Set_Secam_VideoStandard(void);
void VBI_Set_Pal_VideoStandard(void);
void VBI_TTX_EnableInterrupt(MS_BOOL bEnable);
MS_U8 VBI_TTX_ReadIRQ(void);
void VBI_TTX_ClearIRQ(void);
void VBI_TTX_EnableLine(MS_U16 StartLine, MS_U16 EndLine);
void VBI_TTX_DMA_CopyPacket(MS_PHY src_addr, MS_PHY dest_addr, MS_U8 aPacketType);

/*
 *  ClosedCaption
 */
void VBI_CC_Init(MS_VIRT ptrRiuAddr, MS_PHY ptrAddr, MS_U16 u8Len);
void VBI_CC_YPbPr_Init(MS_U8 cvbs_no);
MS_U8 VBI_CC_DataRateSet(MS_U8 *ptable);
MS_U8 VBI_CC_GetPacketCnt(void);
MS_U8 VBI_CC_GetByteFoundIndication(void);
void VBI_SetCCFrameCnt(MS_U8 cnt);
void VBI_CC_EnableSlicer(MS_BOOL bEnable);
void VBI_CC_SetCCLine(MS_U16 StartLine, MS_U16 EndLine, MS_U8 val);
MS_U32 VBI_CC_GetPacket(void);
MS_BOOL VBI_CC_SetSCWindowLen(MS_U8 u8Len);
/*
 * Others
 */
void VBI_SetWssVpsByteNum(MS_U8 cnt);
MS_BOOL VBI_ProtectMemory(MS_BOOL bEnable, MS_PHY phyAddr, MS_U32 u32Size);
void VBI_RegStateStore(void);
void VBI_RegStateRestore(void);

#endif

