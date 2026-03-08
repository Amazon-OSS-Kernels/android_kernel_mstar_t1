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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvDMD_DVBT_INTERN.h
/// @brief  DVBT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBT_V2_H_
#define _DRV_DVBT_V2_H_

//#include "MsCommon.h"
//#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBT.h"
#ifdef __cplusplus
extern "C"
{
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
typedef enum {
    DMD_DVBT_DRV_CMD_Init,
    DMD_DVBT_DRV_CMD_Exit,
    DMD_DVBT_DRV_CMD_SetDbgLeve,
    DMD_DVBT_DRV_CMD_GetInfo,
    DMD_DVBT_DRV_CMD_GetLibVer,
    DMD_DVBT_DRV_CMD_GetFWVer,
    DMD_DVBT_DRV_CMD_GetReg,
    DMD_DVBT_DRV_CMD_SetReg,
    DMD_DVBT_DRV_CMD_SetSerialControl,
    DMD_DVBT_DRV_CMD_SetConfig,
    DMD_DVBT_DRV_CMD_SetConfigHPLP,
    DMD_DVBT_DRV_CMD_SetConfigHPLPSetIF,
    DMD_DVBT_DRV_CMD_SetActive,
    DMD_DVBT_DRV_CMD_GetLock,
    DMD_DVBT_DRV_CMD_GetSignalStrength,
    DMD_DVBT_DRV_CMD_GetSignalStrengthWithRFPower,
    DMD_DVBT_DRV_CMD_GetSignalQuality,
    DMD_DVBT_DRV_CMD_GetSignalQualityWithRFPower,
    DMD_DVBT_DRV_CMD_GetSNR,
    DMD_DVBT_DRV_CMD_GetPostViterbiBer,
    DMD_DVBT_DRV_CMD_GetPreViterbiBer,
    DMD_DVBT_DRV_CMD_GetPacketErr,
    DMD_DVBT_DRV_CMD_GetTPSInfo,
    DMD_DVBT_DRV_CMD_GetCellID,
    DMD_DVBT_DRV_CMD_GetFreqOffset,
    DMD_DVBT_DRV_CMD_NORDIGSSITableWrite,
    DMD_DVBT_DRV_CMD_NORDIGSSITableRead,
    DMD_DVBT_DRV_CMD_SetPowerState,
} DMD_DVBT_DRV_COMMAND;
/// For demod init
typedef struct DLL_PACKED _DMD_DVBT_InitData_Transform
{
    // tuner parameter
    MS_U8 u8SarChannel;
    /*
    DMD_RFAGC_SSI *pTuner_RfagcSsi;
    MS_U16 u16Tuner_RfagcSsi_Size;
    DMD_IFAGC_SSI *pTuner_IfagcSsi_LoRef;
    MS_U16 u16Tuner_IfagcSsi_LoRef_Size;
    DMD_IFAGC_SSI *pTuner_IfagcSsi_HiRef;
    MS_U16 u16Tuner_IfagcSsi_HiRef_Size;
    DMD_IFAGC_ERR *pTuner_IfagcErr_LoRef;
    MS_U16 u16Tuner_IfagcErr_LoRef_Size;
    DMD_IFAGC_ERR *pTuner_IfagcErr_HiRef;
    MS_U16 u16Tuner_IfagcErr_HiRef_Size;
    DMD_SQI_CN_NORDIGP1 *pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;
    */

    // register init
    MS_U8 *u8DMD_DVBT_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBT_DSPRegInitSize;
    MS_U8 *u8DMD_DVBT_InitExt; // TODO use system variable type
} DMD_DVBT_InitData_Transform;

typedef struct DLL_PACKED _DVBT_EXIT_PARAM
{
	MS_BOOL ret;
}DVBT_EXIT_PARAM, *PDVBT_EXIT_PARAM;

typedef struct DLL_PACKED _DVBT_SETDBG_LEVEL_PARAM
{
    DMD_DbgLv u8DbgLevel;
    MS_BOOL ret;
} DVBT_SETDBG_LEVEL_PARAM, *PDVBT_SETDBG_LEVEL_PARAM;

typedef struct DLL_PACKED _DVBT_INIT_PARAM
{
    DMD_DVBT_InitData_Transform *DMD_DVBT_InitData;
    MS_U32 u32InitDataLen;
	MS_BOOL ret;
} DVBT_INIT_PARAM, *PDVBT_INIT_PARAM;

typedef struct DLL_PACKED _DVBT_GETINFO_PARAM
{
    DMD_DVBT_INFO_TYPE eInfoType;
	DMD_DVBT_Info* pInfo;
	MS_BOOL ret;
} DVBT_GETINFO_PARAM, *PDVBT_GETINFO_PARAM;

/*
typedef struct _DVBT_GETINFO_RETURN_PARAM
{
    DMD_DVBT_Info* pInfo;
} DVBT_GETINFO_RETURN_PARAM, *PDVBT_GETINFO_RETURN_PARAM;
*/
typedef struct DLL_PACKED _DVBT_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
    MS_BOOL ret;
} DVBT_GETLIBVER_PARAM, *PDVBT_GETLIBVER_PARAM;

typedef struct DLL_PACKED _DVBT_GETFWVER_PARAM
{
    MS_U16 *ver;
   MS_BOOL ret;
} DVBT_GETFWVER_PARAM, *PDVBT_GETFWVER_PARAM;

typedef struct DLL_PACKED _DVBT_GETREG_PARAM
{
    MS_U16 u16Addr;
    MS_U8 *pu8Data;
   MS_BOOL ret;
} DVBT_GETREG_PARAM, *PDVBT_GETREG_PARAM;

typedef struct DLL_PACKED _DVBT_SETREG_PARAM
{
    MS_U16 u16Addr;
    MS_U8 u8Data;
   MS_BOOL ret;
} DVBT_SETREG_PARAM, *PDVBT_SETREG_PARAM;

typedef struct DLL_PACKED _DVBT_SetSerialControl_PARAM
{
    MS_BOOL bEnable;
    MS_BOOL ret;
} DVBT_SetSerialControl_PARAM, *PDVBT_SetSerialControl_PARAM;

typedef struct DLL_PACKED _DVBT_SetConfig_PARAM
{
    DMD_RF_CHANNEL_BANDWIDTH BW;
    MS_BOOL bSerialTS;
    MS_BOOL bPalBG;
    MS_BOOL ret;
} DVBT_SetConfig_PARAM, *PDVBT_SetConfig_PARAM;

typedef struct DLL_PACKED _DVBT_SetConfigHPLP_PARAM
{
    DMD_RF_CHANNEL_BANDWIDTH BW;
    MS_BOOL bSerialTS;
    MS_BOOL bPalBG;
    MS_BOOL bLPSel;
    MS_BOOL ret;
} DVBT_SetConfigHPLP_PARAM, *PDVBT_SetConfigHPLP_PARAM;

typedef struct DLL_PACKED _DVBT_SetConfigHPLPSetIF_PARAM
{
    DMD_RF_CHANNEL_BANDWIDTH BW;
    MS_BOOL bSerialTS;
    MS_BOOL bPalBG;
    MS_BOOL bLPSel;
    MS_U32 u32IFFreq;
    MS_U32 u32FSFreq;
    MS_U8 u8IQSwap;
    MS_BOOL ret;
} DVBT_SetConfigHPLPSetIF_PARAM, *PDVBT_SetConfigHPLPSetIF_PARAM;

typedef struct DLL_PACKED _DVBT_SetActive_PARAM
{
    MS_BOOL bEnable;
    MS_BOOL ret;
} DVBT_SetActive_PARAM, *PDVBT_SetActive_PARAM;

typedef struct DLL_PACKED _DVBT_GetLock_PARAM
{
    DMD_DVBT_GETLOCK_TYPE eType;
    DMD_LOCK_STATUS *eLockStatus;
    MS_BOOL ret;
} DVBT_GetLock_PARAM, *PDVBT_GetLock_PARAM;

typedef struct DLL_PACKED _DVBT_GetSignalStrength_PARAM
{
    MS_U16 *u16Strength;
    MS_BOOL ret;
} DVBT_GetSignalStrength_PARAM, *PDVBT_GetSignalStrength_PARAM;

/*bryan temp mark*/
#if(0)
typedef struct _DVBT_GetSignalStrengthWithRFPower_PARAM
{
    MS_U16 *u16Strength;
    float fRFPowerDbm;
    MS_BOOL ret;
} DVBT_GetSignalStrengthWithRFPower_PARAM, *PDVBT_GetSignalStrengthWithRFPower_PARAM;
#endif

typedef struct DLL_PACKED _DVBT_GetSignalQuality_PARAM
{
    MS_U16 *u16Quality;
    MS_BOOL ret;
} DVBT_GetSignalQuality_PARAM, *PDVBT_GetSignalQuality_PARAM;

typedef struct DLL_PACKED _DVBT_GetSignalQualityWithRFPower_PARAM
{
    MS_U16 *u16Quality;
    float fRFPowerDbm;
    MS_BOOL ret;
} DVBT_GetSignalQualityWithRFPower_PARAM, *PDVBT_GetSignalQualityWithRFPower_PARAM;

/*bryan temp mark*/
#if(0)
typedef struct _DVBT_GetSNR_PARAM
{
    float *fSNR;
    MS_BOOL ret;
} DVBT_GetSNR_PARAM, *PDVBT_GetSNR_PARAM;
#endif

/*bryan temp mark*/
#if(0)
typedef struct _DVBT_GetPostViterbiBer_PARAM
{
    float *ber;
    MS_BOOL ret;
} DVBT_GetPostViterbiBer_PARAM, *PDVBT_GetPostViterbiBer_PARAM;
#endif

#if(0)
typedef struct _DVBT_GetPreViterbiBer_PARAM
{
    float *ber;
    MS_BOOL ret;
} DVBT_GetPreViterbiBer_PARAM, *PDVBT_GetPreViterbiBer_PARAM;
#endif

typedef struct DLL_PACKED _DVBT_GetPacketErr_PARAM
{
    MS_U16 *pktErr;
    MS_BOOL ret;
} DVBT_GetPacketErr_PARAM, *PDVBT_GetPacketErr_PARAM;

typedef struct DLL_PACKED _DVBT_GetTPSInfo_PARAM
{
    MS_U16 *u16Info;
    MS_BOOL ret;
} DVBT_GetTPSInfo_PARAM, *PDVBT_GetTPSInfo_PARAM;

typedef struct DLL_PACKED _DVBT_GetCellID_PARAM
{
    MS_U16 *u16CellID;
    MS_BOOL ret;
} DVBT_GetCellID_PARAM, *PDVBT_GetCellID_PARAM;

/*bryan temp mark*/
#if(0)
typedef struct _DVBT_GetFreqOffset_PARAM
{
    float *pFreqOff;
    MS_BOOL ret;
} DVBT_GetFreqOffset_PARAM, *PDVBT_GetFreqOffset_PARAM;
#endif

/*bryan temp mark*/
#if(0)
typedef struct _DVBT_NORDIGSSITableWrite_PARAM
{
    DMD_CONSTEL constel;
    DMD_CODERATE code_rate;
    float write_value;
    MS_BOOL ret;
} DVBT_NORDIGSSITableWrite_PARAM, *PDVBT_NORDIGSSITableWrite_PARAM;

typedef struct DLL_PACKED _DVBT_NORDIGSSITableRead_PARAM
{
    DMD_CONSTEL constel;
    DMD_CODERATE code_rate;
    float *read_value;
    MS_BOOL ret;
} DVBT_NORDIGSSITableRead_PARAM, *PDVBT_NORDIGSSITableRead_PARAM;
#endif

typedef struct DLL_PACKED _DVBT_SetPowerState_PARAM
{
    EN_POWER_MODE u16PowerState;
    MS_BOOL ret;
} DVBT_SetPowerState_PARAM, *PDVBT_SetPowerState_PARAM;


/*******kernel mode transform structure***************/





/*************************************************/


MS_U32 DVBTOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DVBTClose(void* pInstantTmp);
MS_U32 DVBTIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
//MS_U32 DVBTStr(MS_U32 u32PowerState, void* pModule);


#ifdef __cplusplus
}
#endif

#endif // _DRV_DVBT_H_

