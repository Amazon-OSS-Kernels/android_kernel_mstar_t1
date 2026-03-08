/**
* Copyright (c) 2006 ??2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef __eMMC_HAL_H__
#define __eMMC_HAL_H__

#include "eMMC.h"


#define eMMC_TIMEOUT_RETRY_CNT     5
#define WAIT_FIFOCLK_READY_CNT     0x10000

extern  U32 eMMC_FCIE_WaitEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec);
extern  U32 eMMC_FCIE_PollingEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec);
extern  U32 eMMC_FCIE_FifoClkRdy(void);
//extern void eMMC_FCIE_DumpDebugBus(void);
//extern void eMMC_FCIE_DumpRegisters(void);
extern void eMMC_FCIE_CheckResetDone(void);
extern  U32 eMMC_FCIE_Reset(void);
extern  U32 eMMC_FCIE_Init(void);
//extern void eMMC_FCIE_ErrHandler_Stop(void);
//extern void eMMC_FCIE_ErrHandler_Retry(void);
extern  U32 eMMC_FCIE_SendCmd(U16 u16_Mode, U16 u16_Ctrl, U32 u32_Arg, U8 u8_CmdIdx, U8 u8_RspByteCnt);
extern void eMMC_FCIE_ClearEvents(void);
extern  U32 eMMC_FCIE_WaitD0High(U32 u32_Timeout);
extern void eMMC_FCIE_GetCIFC(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf);
extern void eMMC_FCIE_GetCIFD(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf);
#ifdef IP_FCIE_VERSION_5
extern U32 eMMC_WaitCIFD_Event(U16 u16_WaitEvent, U32  u32_MicroSec);
extern U32 eMMC_WaitGetCIFD(U8 * pu8_DataBuf, U32 u32_ByteCnt);
#endif

//extern void eMMC_FCIE_SetDDRTimingReg(U8 u8_DQS, U8 u8_DelaySel);
//extern  U32 eMMC_FCIE_EnableDDRMode(void);
//extern  U32 eMMC_FCIE_EnableSDRMode(void);
//extern  U32 eMMC_FCIE_DetectDDRTiming(void);
//extern void eMMC_FCIE_ApplyDDRTSet(U8 u8_DDRTIdx);
//extern void eMMC_DumpDDRTTable(void);
//extern  U32 eMMC_LoadDDRTTable(void);

//----------------------------------------
extern  U32 eMMC_Identify(void);
extern  U32 eMMC_CMD0(U32 u32_Arg);
extern  U32 eMMC_CMD1(void);
extern  U32 eMMC_CMD2(void);
extern  U32 eMMC_CMD3_CMD7(U16 u16_RCA, U8 u8_CmdIdx);
extern  U32 eMMC_CMD9(U16 u16_RCA);
extern  U32 eMMC_CSD_Config(void);
extern  U32 eMMC_ExtCSD_Config(void);
//extern  U32 eMMC_CMD8(U8 *pu8_DataBuf);
//extern  U32 eMMC_CMD8_MIU(U8 *pu8_DataBuf);
//extern  U32 eMMC_CMD8_CIFD(U8 *pu8_DataBuf);
#ifdef IP_FCIE_VERSION_5
extern  U32 eMMC_SetBusSpeed(U8 u8_BusSpeed);
#endif
extern  U32 eMMC_SetBusWidth(U8 u8_BusWidth);
extern  U32 eMMC_ModifyExtCSD(U8 u8_AccessMode, U8 u8_ByteIdx, U8 u8_Value);
extern  U32 eMMC_CMD6(U32 u32_Arg);
//extern  U32 eMMC_EraseCMDSeq(U32 u32_eMMCBlkAddr_start, U32 u32_eMMCBlkAddr_end);
//extern  U32 eMMC_CMD35_CMD36(U32 u32_eMMCBlkAddr, U8 u8_CmdIdx);
extern	U32 eMMC_CMD12(U16 u16_RCA);
extern  U32 eMMC_CMD12_NoCheck(U16 u16_RCA);
//extern  U32 eMMC_CMD38(void);
//extern  U32 eMMC_CMD13(U16 u16_RCA);
//extern  U32 eMMC_CMD17(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
//extern  U32 eMMC_CMD17_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
extern  U32 eMMC_CMD17_CIFD(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
//extern  U32 eMMC_CMD24(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
//extern  U32 eMMC_CMD24_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
//extern  U32 eMMC_CMD24_CIFD(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
//extern  U32 eMMC_CMD18_CIFD(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CMD18_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CMD23(U16 u16_BlkCnt);
//extern  U32 eMMC_CMD25(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CMD25_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CheckR1Error(void);
//extern  U32 eMMC_UpFW_Samsung(U8 *pu8_FWBin);


//----------------------------------------
extern  U32 eMMC_FCIE_PollingFifoClkReady(void);
extern  U32 eMMC_FCIE_PollingMIULastDone(void);

#endif // __eMMC_HAL_H__
