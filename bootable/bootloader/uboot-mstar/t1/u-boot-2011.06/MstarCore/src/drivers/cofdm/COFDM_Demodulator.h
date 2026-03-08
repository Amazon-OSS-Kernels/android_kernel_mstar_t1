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

#ifndef __COFDM_DEMODULATOR_H__
#define __COFDM_DEMODULATOR_H__

#include <MsTypes.h>
#include <ShareType.h>
#include <msAPI_Tuner.h>

#if 1//((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD))


typedef enum
{
    E_SERIAL_INTERFACE,
    E_PARALLEL_INTERFACE
} TS_INTERFACE;

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
#include <cofdm/MSB123x.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD)
#include <cofdm/MSB123xc.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD)
#include <cofdm/MSB1400.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
#include <cofdm/MSB122x.h>
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD)
#include <cofdm/MSB1233C.h>
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD || (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBS_DEMOD))
#if (defined(CONFIG_KENYA) || defined(CONFIG_KERES) || defined(CONFIG_KIRIN) || defined(CONFIG_KRIS) || defined(CONFIG_KRATOS) || defined(CONFIG_KIWI)|| defined(CONFIG_KAYLA))
typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,

} COFDM_LOCK_STATUS;
#endif
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)
#if defined(CONFIG_KENYA)
typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,

} COFDM_LOCK_STATUS;
#endif
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD)
#if (defined(CONFIG_KERES) || defined(CONFIG_KIRIN)|| defined(CONFIG_KRIS) || defined(CONFIG_KRATOS) || defined(CONFIG_KIWI)|| defined(CONFIG_KAYLA))
typedef enum
{
    COFDM_FEC_LOCK,
    COFDM_PSYNC_LOCK,
    COFDM_TPS_LOCK,
    COFDM_TPS_LOCK_HISTORY,
    COFDM_DCR_LOCK,
    COFDM_AGC_LOCK,
    COFDM_MODE_DET,
    COFDM_LOCK_STABLE_DVBT,
    COFDM_SYNC_LOCK_DVBT,
    COFDM_FAST_LOCK_DVBT,
    COFDM_P1_LOCK,
    COFDM_P1_LOCK_HISTORY,
    COFDM_L1_CRC_LOCK,
    COFDM_DVBT2_NOCH_FLAG,
    COFDM_DVBT_NOCH_FLAG,
} COFDM_LOCK_STATUS;
#endif
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1245_DEMOD)
typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,
} COFDM_LOCK_STATUS;
#endif


//******************************************************************************
// Functions prototype
//******************************************************************************

void devCOFDM_Init(void);
void devCOFDM_ControlPowerOnOff(BOOLEAN bPowerOn);
FUNCTION_RESULT devCOFDM_PowerSave(void);
void devCOFDM_SetFreqStep(FREQSTEP eFreqStep);
void devCOFDM_SetFrequency(MS_U32 dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth, RF_CHANNEL_HP_LP eHpLp, U8 u8PlpID);
BOOLEAN devCOFDM_GetLockStatus(COFDM_LOCK_STATUS eStatus);
U8 devCOFDM_GetSignalToNoiseRatio(void);
void devCOFDM_PassThroughI2C(BOOLEAN bEnable);
BOOLEAN devCOFDM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
BOOLEAN devCOFDM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
#if (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface, RF_CHANNEL_BANDWIDTH BandWidth);
#else
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface);
#endif
#if DTV_SCAN_AUTO_FINE_TUNE_ENABLE
BOOLEAN devCOFDM_GetFrequencyOffset(float *pFreqOff, RF_CHANNEL_BANDWIDTH u8BW);
BOOLEAN devCOFDM_SetAfcEnable(BOOLEAN bEnable);
#endif
FUNCTION_RESULT devCOFDM_GetSignal_Vit( MS_U32 *vit );
FUNCTION_RESULT devCOFDM_GetSignalStrength(MS_U16 *strength);
BOOLEAN devCOFDM_Get_CELL_ID(MS_U16 *wCell_id);
FUNCTION_RESULT devCOFDM_GetSignalQuality(MS_U16 *quality);
BOOLEAN devCOFDM_GetBER(float *ber);
BOOLEAN devCOFDM_Get_Modul_Mode(MS_U16 *Modul_Mode);
BOOLEAN devCOFDM_Is_Hierarchy_On(void);
void devCOFDM_TunerProgramming(U8 *cTunerData);

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD || FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD || FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
U16 devCOFDM_ReadReg(U16 RegAddr);
BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData);
#endif
BOOLEAN devCOFDM_InitFrontend(EN_DEVICE_DEMOD_TYPE enDemodType, MS_U32 u32Frequency, MS_U8 u8BandWidth);
#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD) ||(FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType);
EN_DEVICE_DEMOD_TYPE devCOFDM_GetDemodType(void);
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB1233c_DEMOD) || (FRONTEND_DEMOD_TYPE == MSTAR_MSB123xc_DEMOD))
BOOLEAN devCOFDM_DTV_Serial_Control(BOOLEAN bEnable);
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB1238_DEMOD)||(FRONTEND_DEMOD_TYPE == MSTAR_MSB124x_DEMOD))
MS_BOOL MDrv_Tuner_Init(void);
MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
#else
MS_BOOL MDrv_Tuner_Init(void);
#endif

#if ENABLE_DVB_T2_SYS
MS_U8 devCOFDM_GetPlpIDList(void);
MS_U8 devCOFDM_GetNextPlpID(MS_U8 u8Index);
MS_BOOL devCOFDM_SetPlpID(MS_U8 u8PlpID);
#endif

#endif
#endif // __COFDM_DEMODULATOR_H__

