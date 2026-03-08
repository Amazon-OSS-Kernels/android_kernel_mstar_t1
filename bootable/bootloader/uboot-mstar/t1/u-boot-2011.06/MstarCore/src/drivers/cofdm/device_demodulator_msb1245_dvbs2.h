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
#ifndef _MSB124X_DVBS2_H_
#define _MSB124X_DVBS2_H_
#define ENABLE_DEMOD1310_GPIO            0
#define DEMOD124X_PIN11_set_HIGH   MSB124X_GPIO_SetLevel(11,TRUE)//18
#define DEMOD124X_PIN11_set_LOW    MSB124X_GPIO_SetLevel(11,FALSE)//13
#define DEMOD124X_PIN13_set_HIGH   MSB124X_GPIO_SetLevel(13,TRUE)//power off
#define DEMOD124X_PIN13_set_LOW    MSB124X_GPIO_SetLevel(13,FALSE)//on
MS_BOOL  MSB124X_GPIO_Disable(MS_U8 u8Pin);
MS_BOOL MSB124X_GPIO_SetLevel(MS_U8 u8Pin, MS_BOOL bLevel);
MS_BOOL MSB124X_IIC_Bypass_Mode(MS_BOOL enable);
MS_BOOL  MSB124X_I2C_Channel_Set(MS_U8 ch_num);
MS_BOOL  MSB124X_I2C_Channel_Change(MS_U8 ch_num);
MS_BOOL  MSB124X_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data);
MS_BOOL MSB124X_WriteReg(MS_U16 u16Addr, MS_U8 u8Data);
MS_BOOL MSB124X_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data);
MS_BOOL MSB124X_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size);

//MS_BOOL  MSB124X_LoadDSPCode(const MS_U8  *pu8Buf, MS_U16 u16Len);
MS_BOOL  MSB124X_SetFrequency(MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz);
MS_U16	MSB124X_DTV_GetSignalStrength(void);
MS_U32	MSB124X_DTV_GetSignalNoiseRatio(void);
MS_U16	MSB124X_DTV_GetSignalQuality(MS_BOOL bDemodType);
MS_BOOL	MSB124X_DTV_GetLock(void);
MS_BOOL MSB124X_DiSEqC_Init(void);
MS_BOOL  ReadMailbox(MS_U16 u16Addr, MS_U8* u8Data);
MS_BOOL _Load2Sram(MS_U8 *pu8Buf, MS_U16 u16Len);
MS_BOOL MDrv_DMD_MSB1245_LoadDSPCodeToSram(void);
/*
MS_BOOL MDrv_Demod_GetRollOff(MS_U8 *pRollOff);
MS_BOOL MDrv_Demod_GetRFOffset(MS_S16 *ps16RFOff);
MS_BOOL MDrv_Demod_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq);
MS_BOOL MDrv_Demod_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd);
MS_BOOL MDrv_Demod_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum);
MS_BOOL MDrv_Demod_BlindScan_GetChannel(MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable);
MS_BOOL MDrv_Demod_BlindScan_Cancel(void);
MS_BOOL MDrv_Demod_BlindScan_End(void);
MS_BOOL MDrv_Demod_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq);
MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1);
MS_BOOL MDrv_DiSEqC_Set22kOnOff(MS_BOOL b22kOn);
MS_BOOL MDrv_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);
*/
#endif

