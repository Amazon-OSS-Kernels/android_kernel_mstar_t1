/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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
#ifndef __DRV_DEMOD_NULL_H__
#define __DRV_DEMOD_NULL_H__

MS_BOOL MDrv_Demod_null_init(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_open(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_Close(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_Reset(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_TsOut(MS_U8 u8DemodIndex,MS_BOOL bEnable);
MS_BOOL MDrv_Demod_null_PowerOnOff(MS_U8 u8DemodIndex,MS_BOOL bPowerOn);
MS_BOOL MDrv_Demod_null_SetBW(MS_U8 u8DemodIndex,MS_U32 u32BW);
MS_BOOL MDrv_Demod_null_GetBW(MS_U8 u8DemodIndex,MS_U32 *pu32BW);
MS_BOOL MDrv_Demod_null_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus);
MS_BOOL MDrv_Demod_null_GetSNR(MS_U8 u8DemodIndex,MS_U32 *pu32SNR);
MS_BOOL MDrv_Demod_null_GetBER(MS_U8 u8DemodIndex,float *pfBER);
MS_BOOL MDrv_Demod_null_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal);
MS_BOOL MDrv_Demod_null_Config(MS_U8 u8DemodIndex,MS_U8 *pRegParam);
MS_BOOL MDrv_Demod_null_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam);
MS_BOOL MDrv_Demod_null_Restart(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam);
MS_BOOL MDrv_Demod_null_SetMode(MS_U8 u8DemodIndex,Demod_Mode* pMode);
MS_BOOL MDrv_Demod_null_SetOutoutPath(MS_U8 u8DemodIndex,DEMOD_INTERFACE_MODE path);
DEMOD_INTERFACE_MODE MDrv_Demod_null_GetOutoutPath(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn);

MS_BOOL MDrv_Demod_null_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8 type);
MS_U8 MDrv_Demod_null_GetCurrentDemodType(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_GetPlpBitMap(MS_U8 u8DemodIndex, MS_U8* u8PlpBitMap);
MS_BOOL MDrv_Demod_null_GetPlpGroupID(MS_U8 u8DemodIndex, MS_U8 u8PlpID, MS_U8* u8GroupID);
MS_BOOL MDrv_Demod_null_SetPlpGroupID(MS_U8 u8DemodIndex, MS_U8 u8PlpID, MS_U8 u8GroupID);

MS_BOOL MDrv_Demod_null_SetTsSerial(MS_U8 u8DemodIndex,MS_BOOL bSerial);
MS_BOOL MDrv_Demod_null_TPSGetLock(MS_U8 u8DemodIndex,MS_BOOL *pbLock);
MS_BOOL MDrv_Demod_null_MPEGGetLock(MS_U8 u8DemodIndex,MS_BOOL *pbLock);
MS_BOOL MDrv_Demod_null_GetRollOff(MS_U8 u8DemodIndex,MS_U8 *pRollOff);
MS_BOOL MDrv_Demod_null_GetRFOffset(MS_U8 u8DemodIndex,MS_S16 *ps16RFOff);
MS_BOOL MDrv_Demod_null_BlindScan_Start(MS_U8 u8DemodIndex,MS_U16 u16StartFreq,MS_U16 u16EndFreq);
MS_BOOL MDrv_Demod_null_BlindScan_NextFreq(MS_U8 u8DemodIndex,MS_BOOL* bBlindScanEnd);
MS_BOOL MDrv_Demod_null_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex,MS_U8* u8Progress,MS_U8 *u8FindNum);
MS_BOOL MDrv_Demod_null_BlindScan_Cancel(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_BlindScan_End(MS_U8 u8DemodIndex);
MS_BOOL MDrv_Demod_null_BlindScan_GetChannel(MS_U8 u8DemodIndex, MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable);
MS_BOOL MDrv_Demod_null_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex, MS_U32 *u32CurrentFeq);
MS_BOOL MDrv_Demod_null_DiSEqC_SetTone(MS_U8 u8DemodIndex, MS_BOOL bTone1);
MS_BOOL MDrv_Demod_null_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex, MS_BOOL bLow);
MS_BOOL MDrv_Demod_null_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex, MS_BOOL* bLNBOutLow);
MS_BOOL MDrv_Demod_null_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex, MS_BOOL b22kOn);
MS_BOOL MDrv_Demod_null_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex, MS_BOOL* b22kOn);
MS_BOOL MDrv_Demod_null_DiSEqC_SendCmd(MS_U8 u8DemodIndex, MS_U8* pCmd,MS_U8 u8CmdSize);



#endif // #define __DRV_DEMOD_NULL_H__
