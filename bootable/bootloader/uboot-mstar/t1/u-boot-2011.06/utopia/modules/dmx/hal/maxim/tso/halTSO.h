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
// file   halTSO.h
// @brief  TS I/O (TSO) HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_TSO_H__
#define __HAL_TSO_H__

#include "MsCommon.h"

#include "regTSO.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

#define _TsoPid                      ((REG_Pid*)(REG_PIDFLT_BASE))

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------


////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////

// TSO in mux select
#define HAL_TSOIN_MUX_TS0                       0x0000UL
#define HAL_TSOIN_MUX_TS1                       0x0001UL
#define HAL_TSOIN_MUX_TS2                       0x0002UL
#define HAL_TSOIN_MUX_TS3                       0x0003UL
#define HAL_TSOIN_MUX_TS4                       0x0004UL
#define HAL_TSOIN_MUX_TS5                       0x0005UL
#define HAL_TSOIN_MUX_TSCB                      0xFFFFUL   //not supported
#define HAL_TSOIN_MUX_TSDEMOD0                  0x0007UL
#define HAL_TSOIN_MUX_TSDEMOD1                  0xFFFFUL   //not supported
#define HAL_TSOIN_MUX_MEM                       0x0008UL
#define HAL_TSOIN_MUX_MEM1                      0x0009UL

// TSO output mux select
#define HAL_TSOOUT_MUX_NONE                     0x0000UL
#define HAL_TSOOUT_MUX_TS1                      0x0001UL
#define HAL_TSOOUT_MUX_TS3                      0x0003UL


// TSO In Clk select
#define HAL_TSO_CLKIN_TS0                       0x0000UL
#define HAL_TSO_CLKIN_TS1                       0x0001UL
#define HAL_TSO_CLKIN_TS2                       0x0002UL
#define HAL_TSO_CLKIN_TS3                       0x0004UL
#define HAL_TSO_CLKIN_TSCB                      0xFFFFUL  //not supported
#define HAL_TSO_CLKIN_TSDEMOD0                  0x0003UL
#define HAL_TSO_CLKIN_TSDEMOD1                  0xFFFFUL  //not supported
#define HAL_TSO_CLKIN_TS4                       0x0005UL
#define HAL_TSO_CLKIN_TS5                       0x0006UL


// Pre TsoOut Select
#define HAL_PRE_TSO_OUT_SEL_TS0IN               0x0000UL
#define HAL_PRE_TSO_OUT_SEL_TS1IN               0x0001UL
#define HAL_PRE_TSO_OUT_SEL_TS2IN               0x0002UL
#define HAL_PRE_TSO_OUT_SEL_TS3IN               0x0003UL
#define HAL_PRE_TSO_OUT_SEL_TS4IN               0x0004UL
#define HAL_PRE_TSO_OUT_SEL_TS5IN               0x0005UL
#define HAL_PRE_TSO_OUT_SEL_TSCBIN              0xFFFFUL  //not supported
#define HAL_PRE_TSO_OUT_SEL_TSDEMOD0            0xFFFFUL  //not supported

// TSO ClkOut DivClk Src Select
#define HAL_TSO_OUT_DIV_SEL_172M_2N             0x0000UL
#define HAL_TSO_OUT_DIV_SEL_288M_2N             0x0800UL
#define HAL_TSO_OUT_DIV_SEL_216M_N              0xFFFFUL  //not supported

// TSO ClkOut Select
#define HAL_TSO_OUT_SEL_TSO_OUT_DIV2N           0x0000UL
#define HAL_TSO_OUT_SEL_TSO_OUT_62MHz           0x0400UL
#define HAL_TSO_OUT_SEL_TSO_OUT_54MHz           0x0800UL
#define HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT         0x0C00UL
#define HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8    0x1000UL
#define HAL_TSO_OUT_SEL_TSO_OUT_27MHz           0x1400UL
#define HAL_TSO_OUT_SEL_TSO_OUT_FROM_DEMOD      0x1C00UL
#define HAL_TSO_OUT_SEL_TSO_OUT_DIV             0xFFFFUL  //not supported

//TSO Channel IF
#define HAL_TSO_TSIF_LIVE1                      1UL
#define HAL_TSO_TSIF_LIVE2                      5UL
#define HAL_TSO_TSIF_LIVE3                      6UL
#define HAL_TSO_TSIF_FILE1                      HAL_TSO_TSIF_LIVE2
#define HAL_TSO_TSIF_FILE2                      HAL_TSO_TSIF_LIVE3
#define HAL_TSO_TSIF_LIVE_MMT                   7UL


//TSO SVQ RX packet mode
#define HAL_TSO_SVQRX_MODE_OPENCABLE            0UL
#define HAL_TSO_SVQRX_MODE_CIP                  1UL
#define HAL_TSO_SVQRX_MODE_192                  2UL

//TSO timestamp base
#define HAL_TSO_TIMESTAMP_90K                   0UL
#define HAL_TSO_TIMESTAMP_27M                   1UL

#define MIU_BUS                                 4UL

//TSO PVR SRC
#define HAL_TSO_PVR_NONE                        0UL
#define HAL_TSO_PVR_SVQ                         1UL
#define HAL_TSO_PVR_MMT                         2UL

typedef struct stDrvTSOOutClk
{
    MS_U16  u16OutClk;
    MS_U16  u16OutDivSrc;
    MS_U16  u16OutDivNum;
    MS_U16  u16PreTsoOutClk;
    MS_BOOL bClkInvert;
    MS_BOOL bEnable;
}HalTSOOutClk;

//
// General API
void        HAL_TSO_SetBank(MS_VIRT virtBankAddr);
void        HAL_TSO_Init(void);
void        HAL_TSO_Reset_All(MS_U8 u8Eng);
void        HAL_TSO_Reset(MS_U8 u8Eng);
void        HAL_TSO_Reset_SubItem(MS_U8 u8Eng, MS_U16 u16RstItem);
void        HAL_TSO_HWInt_Enable(MS_U8 u8Eng, MS_BOOL bEnable, MS_U16 u16init);
void        HAL_TSO_HWInt_Clear(MS_U8 u8Eng, MS_U16 u16Int);
MS_U16      HAL_TSO_HWInt_Status(MS_U8 u8Eng);
void        HAL_TSO_PowerCtrl(MS_BOOL bOn);

void        HAL_TSO_Recover_TSOutMode(MS_U8 u8Eng);
MS_BOOL     HAL_TSO_OutPad(MS_U8 u8Eng, MS_U16* pu16OutPad, MS_BOOL bSet);

MS_BOOL     HAL_TSO_SelPad(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16 u16InPadSel, MS_BOOL bParallel);
MS_BOOL     HAL_TSO_Set_InClk(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16 u16ClkSel, MS_BOOL bClkInvert, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_GetInputTSIF_Status(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16* pu16Pad, MS_BOOL* pbClkInvert, MS_BOOL* pbExtSync, MS_BOOL* pbParl);
MS_BOOL     HAL_TSO_OutClk_DefSelect(MS_U8 u8Eng, MS_U16 u16PadSel, MS_BOOL bSet, HalTSOOutClk* pstOutClkSet);
void        HAL_TSO_OutputClk(MS_U8 u8Eng, HalTSOOutClk* pstOutClkSet, MS_BOOL bSet);
MS_BOOL     HAL_TSO_GetOutputClk(MS_U8 u8Eng, HalTSOOutClk* pstOutClkSet);
MS_BOOL     HAL_TSO_Set_TSOOut_Phase_Tune(MS_U8 u8Eng, MS_U16 u16ClkOutPhase, MS_BOOL bPhaseEnable);
MS_BOOL     HAL_TSO_PreTsoOutClk(MS_U8 u8Eng, MS_U16* pu16PreTsoOutSel, MS_BOOL bSet);
MS_BOOL     HAL_TSO_TSOOutDiv(MS_U8 u8Eng, MS_U16* pu16ClkOutDivSrcSel, MS_U16* pu16ClkOutDivNum, MS_BOOL bSet);
MS_BOOL     HAL_TSO_OutClk(MS_U8 u8Eng, MS_U16* pu16ClkOutSel, MS_BOOL* pbClkInvert, MS_BOOL* pbEnable, MS_BOOL bSet);

void        HAL_TSO_Flt_SetPid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U16 u16PID);
void        HAL_TSO_Flt_SetInputSrc(MS_U8 u8Eng, MS_U16 u16FltId, MS_U16 u16InputSrc);
MS_BOOL     HAL_TSO_ReplaceFlt_SetPktPid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U8 u8TsIf, MS_U16 u16OldPid, MS_U16 u16NewPid);
MS_BOOL     HAL_TSO_ReplaceFlt_Enable(MS_U8 u8Eng, MS_U16 u16FltId, MS_BOOL bEnable);

void        HAL_TSO_Set_Filein_ReadAddr(MS_U8 u8Eng, MS_U8 u8FileEng, MS_PHY phyAddr);
void        HAL_TSO_Set_Filein_ReadLen(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32len);
MS_PHY      HAL_TSO_Get_Filein_ReadAddr(MS_U8 u8Eng, MS_U8 u8FileEng);
void        HAL_TSO_Set_Filein_Ctrl(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U16 u16ctrl);
MS_U16      HAL_TSO_Get_Filein_Ctrl(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_Set_Filein_MOBFKey(MS_U8 u8FileEng, MS_U32 u32Key, MS_BOOL bSecured);
MS_BOOL     HAL_TSO_Filein_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_Set_Filein_MOBFKey(MS_U8 u8FileEng, MS_U32 u32Key, MS_BOOL bSecured);
void        HAL_TSO_FileinTimer_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable);
void        HAL_TSO_Filein_Rate(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U16 u16timer);
void        HAL_TSO_Filein_192Mode_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable);
void        HAL_TSO_Filein_192BlockMode_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable);
MS_U16      HAL_TSO_CmdQ_FIFO_Get_WRCnt(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_CmdQ_FIFO_IsFull(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_CmdQ_FIFO_IsEmpty(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_U8       HAL_TSO_CmdQ_FIFO_Get_WRLevel(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_CmdQ_Reset(MS_U8 u8Eng, MS_U8 u8FileEng);

void        HAL_TSO_RW_ValidBlock_Count(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16ValidBlockCnt);
void        HAL_TSO_RW_InvalidBlock_Count(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16InvalidBlockCnt);
void        HAL_TSO_RW_OutputPktSize(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16PktSize);

void        HAL_TSO_LPcr2_Set(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32lpcr2);
MS_U32      HAL_TSO_LPcr2_Get(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_U32      HAL_TSO_TimeStamp_Get(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8If, MS_U8 u8size);
void        HAL_TSO_Filein_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U8 u8size);

void        HAL_TSO_Cfg1_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL bEnable);
void        HAL_TSO_Cfg4_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_ChIf_Enable(MS_U8 u8Eng, MS_U8 u8ChIf, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16 u16Cfg, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_Get_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16* pu16Cfg, MS_BOOL *pbEnable);
MS_BOOL     HAL_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_U8 u8ChIf, MS_PHY phyBufAddr, MS_U32 u32BufSize);
MS_BOOL     HAL_TSO_ChIf_ClrByteCnt(MS_U8 u8Eng, MS_U8 u8ChIf);
MS_BOOL     HAL_TSO_LocalStreamID(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U8* pu8StrID, MS_BOOL beSet);
MS_BOOL     HAL_TSO_SVQ_TX_Reset(MS_U8 u8Eng, MS_U8 u8ChIf);
MS_BOOL     HAL_TSO_Set_SVQRX_MOBFKey(MS_U8 u8Eng, MS_U32 u32Key, MS_BOOL bSecured);
MS_BOOL     HAL_TSO_Set_SVQRX_PktMode(MS_U8 u8Eng, MS_U16 u16mode);
MS_BOOL     HAL_TSO_Set_SVQRX_ArbitorMode(MS_U8 u8Eng, MS_U16 u16mode, MS_U16 *pu16SvqRxPri);
MS_BOOL     HAL_TSO_Set_SVQ_LocalSysTimestamp(MS_U8 u8Eng, MS_U32 u32systime);
MS_BOOL     HAL_TSO_Get_SVQ_Status(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16* pu16Status);
MS_BOOL     HAL_TSO_GetDelayTime_PreHd2Output(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U32 *pu32time);
MS_BOOL     HAL_TSO_Get_MaxDelta_ChId(MS_U8 u8Eng, MS_U8 *pu8ChIf);
MS_BOOL     HAL_TSO_Sel_LocalSysStampClkBase(MS_U8 u8Eng, MS_U16 u16ClkBase);
MS_U16      HAL_TSO_MMT_Cfg_Map(MS_U16 u16Cfg);
void        HAL_TSO_PVR_WaitFlush(MS_U8 u8PVRId);
void        HAL_TSO_PVR_SetBuffer(MS_U8 u8PVRId, MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1);
MS_PHY      HAL_TSO_PVR_GetBufWrite(MS_U8 u8PVRId);
void        HAL_TSO_PVR_Enable(MS_U8 u8PVRId, MS_BOOL bEnable);
void        HAL_TSO_PVR_PacketMode(MS_U8 u8PVRId, MS_BOOL bSet);
void        HAL_TSO_SetPVRTimeStamp(MS_U8 u8PVRId, MS_U32 u32Stamp);
MS_U32      HAL_TSO_GetPVRTimeStamp(MS_U8 u8PVRId);
void        HAL_TSO_SetPVRTimeStampClk(MS_U8 u8PVRId, MS_U32 u32ClkSrc);
MS_BOOL     HAL_TSO_PVR_IsEnabled(MS_U32 u32EngId);
void        HAL_TSO_PVR_Src(MS_U32 u32Src);

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL     HAL_TSO_SaveRegs(void);
MS_BOOL     HAL_TSO_RestoreRegs(void);
#endif  //MSOS_TYPE_LINUX_KERNEL


#endif // #ifndef __HAL_TSO_H__


