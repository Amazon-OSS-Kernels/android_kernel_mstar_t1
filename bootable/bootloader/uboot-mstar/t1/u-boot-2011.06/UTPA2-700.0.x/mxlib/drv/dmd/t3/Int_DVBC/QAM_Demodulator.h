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

#ifndef __QAM_DEMODULATOR_H__
#define __QAM_DEMODULATOR_H__

#include "debug.h"
#include "Tuner.h"
#include "msAPI_Global.h"

typedef enum
{
    E_QAM_SERIAL_INTERFACE,
    E_QAM_PARALLEL_INTERFACE
} QAM_TS_INTERFACE;

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
#include "INTERN_DVBC.h"
#else
typedef enum
{
    QAM_FEC_LOCK,
    QAM_PSYNC_LOCK,
    QAM_TPS_LOCK,
    QAM_DCR_LOCK,
    QAM_AGC_LOCK,
    QAM_MODE_DET,

} QAM_LOCK_STATUS;
#endif


//******************************************************************************
// Functions prototype
//******************************************************************************

void devQAM_Init(void);
void devQAM_ControlPowerOnOff(BOOLEAN bPowerOn);
FUNCTION_RESULT devQAM_PowerSave(void);
void devQAM_SetFreqStep(FREQSTEP eFreqStep);
void devQAM_SetFrequency(DWORD dwFrequency, U8 u8QamMode, U16 u16SymbolRate, BOOLEAN bSpecInv);
BOOLEAN devQAM_GetLockStatus(QAM_LOCK_STATUS eStatus);
BYTE devQAM_GetSignalToNoiseRatio(void);
void devQAM_PassThroughI2C(BOOLEAN bEnable);
BOOLEAN devQAM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
BOOLEAN devQAM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
void devQAM_SwitchTransportSteamInferface(QAM_TS_INTERFACE interface);
#if DTV_SCAN_AUTO_FINE_TUNE_ENABLE
BOOLEAN devQAM_GetFrequencyOffset(float *pFreqOff, RF_CHANNEL_BANDWIDTH u8BW);
BOOLEAN devQAM_SetAfcEnable(BOOLEAN bEnable);
#endif
FUNCTION_RESULT devQAM_GetSignal_Vit( DWORD *vit );
FUNCTION_RESULT devQAM_GetSignalStrength(WORD *strength);
FUNCTION_RESULT devQAM_GetSignalQuality(WORD *quality);
void devQAM_TunerProgramming(U8 *cTunerData);
BOOLEAN devQAM_GetSpecInv(void);
#endif // __QAM_DEMODULATOR_H__

