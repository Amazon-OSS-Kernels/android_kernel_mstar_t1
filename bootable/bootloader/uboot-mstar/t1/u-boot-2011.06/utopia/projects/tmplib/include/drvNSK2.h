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

#ifndef _DRVNSK2_H_
#define _DRVNSK2_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvNSK2.h
/// @brief
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
//#include "ndstypes.h"
////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////


typedef void (*NSK2_IntNotify)(MS_U32 reason, MS_U32 acpuError);


////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_NSK2_Init(void);
MS_U32 MDrv_NSK2_ColdReset(void);

MS_U32 MDrv_NSK2_Compare(MS_U32 StartAddr, MS_U32 CompareLens, MS_U32 CompareSim,
                         MS_U32 Mask, MS_U32 ExpectResult);
MS_U32 MDrv_NSK2_CompareMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 CompareLens,
                            MS_U32 CompareSim, MS_U32 ExpectResult, void *pGolden);

MS_U32 MDrv_NSK2_CompareKTE(MS_U32 reserved_1, MS_U32 reserved_2, void *pLabel);
MS_U32 MDrv_NSK2_CompareOut(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 HighDWord, MS_U32 LowDWord);

MS_U32 MDrv_NSK2_WriteMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 WriteLens, void *pWriteData);
MS_U32 MDrv_NSK2_WriteSFR(MS_U32 StartAddr, MS_U32 Data);
MS_U32 MDrv_NSK2_WriteESA(MS_U8 ESASelect, MS_U8 ESASubSelect, MS_U8 pid_no);

MS_U32 MDrv_NSK2_WriteTransportKey( MS_U8 SCB, MS_U8 ForceSCB, void *pLabel, MS_U8 pid_no );
MS_U32 MDrv_NSK2_WriteM2MKey(void *pIV, MS_U8 SubAlgo);
MS_U32 MDrv_NSK2_WriteSCPUKey(void);

MS_U32 MDrv_NSK2_SetRNG(MS_U32 reserved_1, MS_U16 RNG_Value);
MS_U32 MDrv_NSK2_BasicInitializationComplete(void);
MS_U32 MDrv_NSK2_DriveKteAck(void);
MS_U32 MDrv_NSK2_FillJTagPswd(void);
MS_U32 MDrv_NSK2_CheckPubOTPConfig(void *pCheck);
MS_U32 MDrv_NSK2_UnlockOTPCtrl(void);

MS_U32 MDrv_NSK2_ReadData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data);
MS_U32 MDrv_NSK2_WriteData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data);
MS_U32 MDrv_NSK2_ReadData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data);
MS_U32 MDrv_NSK2_WriteData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data);
MS_U32 MDrv_NSK2_ReadData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data);
MS_U32 MDrv_NSK2_WriteData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data);

MS_U32 MDrv_NSK2_SetIntNotify(NSK2_IntNotify IntNotify);
MS_U32 MDrv_NSK2_ExcuteCmd(MS_U32 command, MS_U32 control,
                           MS_U32 register_offset, MS_U32 data_size,
                           const MS_U8    *data);



MS_U32 MDrv_NSK2_GetMaxXConn(void);
MS_U32 MDrv_NSK2_GetOTPValue(MS_U32 u32Offset);
MS_U32 MDrv_NSK2_CMChannelNum(void);
MS_U32 MDrv_NSK2_GetOTPFieldValue(MS_U8 u8Tag, MS_U8 *pLength, MS_U32 *pRetValue);
void   MDrv_NSK2_SetDbgLevel(MS_U32 u32Level);
void   MDrv_NSK2_SetPollingCnt(MS_U32 u32Cnt);
void   MDrv_NSK2_GetRNGThroughPut(void *pRngData, MS_U32 u32DataSize, MS_BOOL bDump);
void   MDrv_NSK2_RunFree(MS_BOOL bRunFree);
void   MDrv_NSK2_PushSlowClock ( MS_BOOL HaltClk, MS_U32 TenSecond);
MS_U32 MDrv_NSK2_BasicInitializationComplete(void);
//void   MDrv_NSK2_BurstLen(MS_U32 u32PVREng, MS_U32 u32BurstMode);

#ifdef __cplusplus
}
#endif
#endif//_DRVNSK2_H_

