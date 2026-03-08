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
/// @file   drvDMD_DVBS_INTERN.h
/// @brief  DVBS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBS_V2_H_
#define _DRV_DVBS_V2_H_

//#include "MsCommon.h"
//#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBS.h"
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DLL_PUBLIC
#define DLL_PUBLIC
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
typedef enum {
    DMD_DVBS_DRV_CMD_Init,
    DMD_DVBS_DRV_CMD_Exit,
    DMD_DVBS_DRV_CMD_SetDbgLevel,
    DMD_DVBS_DRV_CMD_GetInfo,
    DMD_DVBS_DRV_CMD_GetLibVer,
    DMD_DVBS_DRV_CMD_GetFWVer,
    DMD_DVBS_DRV_CMD_GetDSPReg,
    DMD_DVBS_DRV_CMD_SetDSPReg,
    DMD_DVBS_DRV_CMD_GetReg,
    DMD_DVBS_DRV_CMD_SetReg,
    DMD_DVBS_DRV_CMD_SetSerialControl,
    DMD_DVBS_DRV_CMD_BlindScan_Config,
    DMD_DVBS_DRV_CMD_BlindScan_Config_symbol_rate_list,
    DMD_DVBS_DRV_CMD_SetConfig,
    DMD_DVBS_DRV_CMD_SetConfig_symbol_rate_list,
    DMD_DVBS_DRV_CMD_SetActive,
    DMD_DVBS_DRV_CMD_GetLockWithRFPower,
    DMD_DVBS_DRV_CMD_GetSignalStrengthWithRFPower,
    //DMD_DVBS_DRV_CMD_GetSignalQualityWithRFPower,
    DMD_DVBS_DRV_CMD_ActiveDmdSwitch,
    DMD_DVBS_DRV_CMD_GetSNR,
    DMD_DVBS_DRV_CMD_GetPostViterbiBer,
    DMD_DVBS_DRV_CMD_GetPacketErr,
    DMD_DVBS_DRV_CMD_GetCellID,
    DMD_DVBS_DRV_CMD_GetStatus,
    DMD_DVBS_DRV_CMD_SetPowerState,
    DMD_DVBS_DRV_CMD_Demod_Restart,
    DMD_DVBS_DRV_CMD_Get_FreqOffset,
    DMD_DVBS_DRV_CMD_BlindScan_Start,
    DMD_DVBS_DRV_CMD_BlindScan_NextFreq,
    DMD_DVBS_DRV_CMD_BlindScan_Cancel,
    DMD_DVBS_DRV_CMD_BlindScan_End,
    DMD_DVBS_DRV_CMD_BlindScan_GetChannel,
    DMD_DVBS_DRV_CMD_BlindScan_GetCurrentFreq,
    DMD_DVBS_DRV_CMD_BlindScan_WaitCurFreqFinished,
    DMD_DVBS_DRV_CMD_BlindScan_GetTunerFreq,
    DMD_DVBS_DRV_CMD_DiSEqC_Init,
    DMD_DVBS_DRV_CMD_DiSEqC_SetLNBOut,
    DMD_DVBS_DRV_CMD_DiSEqC_GetLNBOut,
    DMD_DVBS_DRV_CMD_DiSEqC_Set22kOnOff,
    DMD_DVBS_DRV_CMD_DiSEqC_Get22kOnOff,
    DMD_DVBS_DRV_CMD_DiSEqC_SendCmd,
    DMD_DVBS_DRV_CMD_DiSEqC_SetTxToneMode,
    DMD_DVBS_DRV_CMD_DiSEqC_SetTone,
    DMD_DVBS_DRV_CMD_UnicableAGCCheckPower,
    DMD_DVBS_DRV_CMD_GetIFAGC,

    //Leo add
    DMD_DVBS_DRV_CMD_GetTsDivNum,
    DMD_DVBS_DRV_CMD_GetCurrentDemodCodeRate,
    DMD_DVBS_DRV_CMD_GetTunrSignalLevel_PWR
} DMD_DVBS_DRV_COMMAND;


typedef struct DLL_PACKED _DMD_DVBS_InitData_Transform
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
    MS_U8 *u8DMD_DVBS_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBS_DSPRegInitSize;
    MS_U8 *u8DMD_DVBS_InitExt; // TODO use system variable type
} DMD_DVBS_InitData_Transform;


typedef struct DLL_PACKED _DVBS_Init_PARAM
{
	DMD_DVBS_InitData_Transform *pDMD_DVBS_InitData;
	MS_U32 u32InitDataLen;
	MS_BOOL ret;
}DVBS_Init_PARAM,*PDVBS_Init_PARAM;

typedef struct DLL_PACKED _DVBS_SetDbgLevel_PARAM
{
	DMD_DVBS_DbgLv u8DbgLevel;
	MS_BOOL ret;
}DVBS_SetDbgLevel_PARAM,*PDVBS_SetDbgLevel_PARAM;

typedef struct DLL_PACKED _DVBS_EXIT_PARAM_PARAM
{
	MS_BOOL ret;
}DVBS_EXIT_PARAM_PARAM,*PDVBS_EXIT_PARAM_PARAM;

typedef struct DLL_PACKED _DVBS_GetInfo_PARAM
{
	const DMD_DVBS_Info* ret_info;
}DVBS_GetInfo_PARAM,*PDVBS_GetInfo_PARAM;

typedef struct _DVBS_GetLibVer_PARAM
{
	const MSIF_Version **ppVersion;
	MS_BOOL ret;
}DVBS_GetLibVer_PARAM,*PDVBS_GetLibVer_PARAM;

typedef struct DLL_PACKED _DVBS_GetFWVer_PARAM
{
	MS_U16 *ver;
	MS_BOOL ret;
}DVBS_GetFWVer_PARAM,*PDVBS_GetFWVer_PARAM;


typedef struct DLL_PACKED _DVBS_GetDSPReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 *pu8Data;
	MS_BOOL ret;
}DVBS_GetDSPReg_PARAM,*PDVBS_GetDSPReg_PARAM;

typedef struct DLL_PACKED _DVBS_SetDSPReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 pu8Data;
	MS_BOOL ret;
}DVBS_SetDSPReg_PARAM,*PDVBS_SetDSPReg_PARAM;


typedef struct DLL_PACKED _DVBS_GetReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 *pu8Data;
	MS_BOOL ret;
}DVBS_GetReg_PARAM,*PDVBS_GetReg_PARAM;

typedef struct DLL_PACKED _DVBS_SetReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 u8Data;
	MS_BOOL ret;
}DVBS_SetReg_PARAM,*PDVBS_SetReg_PARAM;


typedef struct DLL_PACKED _DVBS_SetSerialControl_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBS_SetSerialControl_PARAM,*PDVBS_SetSerialControl_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_Config_Symbol_rate_list_PARAM
{
	MS_U32 u32SymbolRate;
	DMD_DVBS_MODULATION_TYPE eQamMode;
	MS_U32 u32IFFreq;
	MS_BOOL bSpecInv;
	MS_BOOL bSerialTS;
	MS_U16 *pu16_symbol_rate_list;
	MS_U8 u8_symbol_rate_list_num;
	MS_BOOL ret;
}DVBS_BlindScan_Config_Symbol_rate_list_PARAM,*PDVBS_BlindScan_Config_Symbol_rate_list_PARAM;

typedef struct DLL_PACKED _DVBS_SetConfig_Symbol_rate_list_PARAM
{
	MS_U16 u16SymbolRate;
	DMD_DVBS_MODULATION_TYPE eQamMode;
	MS_U32 u32IFFreq;
	MS_BOOL bSpecInv;
	MS_BOOL bSerialTS;
	MS_U16 *pu16_symbol_rate_list;
	MS_U8 u8_symbol_rate_list_num;
	MS_BOOL ret;
}DVBS_SetConfig_Symbol_rate_list_PARAM,*PDVBS_SetConfig_Symbol_rate_list_PARAM;

typedef struct DLL_PACKED _DVBS_SetActive_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBS_SetActive_PARAM,*PDVBS_SetActive_PARAM;

typedef struct DLL_PACKED _DVBS_GetLockWithRFPower
{
	DMD_DVBS_GETLOCK_TYPE eType;
	DMD_DVBS_LOCK_STATUS *eLockStatus;
	MS_U32 u32CurrRFPowerDbm;
	MS_U32 u32NoChannelRFPowerDbm;
	MS_BOOL ret;
}DVBS_GetLockWithRFPower,*PDVBS_GetLockWithRFPower;

typedef struct DLL_PACKED _DVBS_ActiveDmdSwitch_PARAM
{
	MS_U8 demod_no;
	MS_BOOL ret;
}DVBS_ActiveDmdSwitch_PARAM,*PDVBS_ActiveDmdSwitch_PARAM;

typedef struct _DVBS_GetSignalStrengthWithRFPower_PARAM
{
	MS_U16 fRFPowerDbm;
	DMD_DVBS_DEMOD_TYPE *pDemodType;
	MS_U8  *u8_DVBS2_CurrentCodeRateLocal;
	MS_U8   *u8_DVBS2_CurrentConstellationLocal;
	MS_BOOL ret;
}DVBS_GetSignalStrengthWithRFPower_PARAM,*PDVBS_GetSignalStrengthWithRFPower_PARAM;

//Leo add
typedef struct _DVBS_GetIFAGC_PARAM
{
	MS_U8 *ifagc_reg;
	MS_U8 *ifagc_reg_lsb;
	MS_U16 *ifagc_err_reg;
	MS_BOOL ret;
}DVBS_GetIFAGC_PARAM,*PDVBS_GetIFAGC_PARAM;


//typedef struct DLL_PACKED _DVBS_GetPacketErr_PARAM

typedef struct _DVBS_GetSNR_PARAM
{
	MS_U32 *u32NDA_SNR_A;
	MS_U32 *u32NDA_SNR_AB;
	MS_BOOL ret;
}DVBS_GetSNR_PARAM,*PDVBS_GetSNR_PARAM;


typedef struct _DVBS_GetPostViterbiBer_PARAM
{
	MS_U32 *BitErr_reg;
	MS_U16 *BitErrPeriod_reg;
	MS_BOOL ret;
}DVBS_GetPostViterbiBer_PARAM,*PDVBS_GetPostViterbiBer_PARAM;


typedef struct DLL_PACKED _DVBS_GetPacketErr_PARAM
{
	MS_U16 *pktErr;
	MS_BOOL ret;
}DVBS_GetPacketErr_PARAM,*PDVBS_GetPacketErr_PARAM;

typedef struct DLL_PACKED _DVBS_GetCellID_PARAM
{
	MS_U16 *u16CellID;
	MS_BOOL ret;
}DVBS_GetCellID_PARAM,*PDVBS_GetCellID_PARAM;


typedef struct _DVBS_GetStatus_PARAM
{
	DMD_DVBS_MODULATION_TYPE *pQAMMode;
	MS_U32 *u32SymbolRate;
	MS_S16  *s16CFO;
	MS_BOOL ret;
}DVBS_GetStatus_PARAM,*PDVBS_GetStatus_PARAM;


typedef struct DLL_PACKED _DVBS_SetPowerState_PARAM
{
	EN_POWER_MODE u16PowerState;
	MS_U32 ret_U32;
}DVBS_SetPowerState_PARAM,*PDVBS_SetPowerState_PARAM;

typedef struct DLL_PACKED _DVBS_Get_FreqOffset_PARAM
{
	MS_S16  *s16CFO;
	MS_BOOL bRet;
}DVBS_Get_FreqOffset_PARAM,*PDVBS_Get_FreqOffset_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_Start_PARAM
{
	MS_U16 u16StartFreq;
	MS_U16 u16EndFreq;
	MS_BOOL bRet;
}DVBS_BlindScan_Start_PARAM,*PDVBS_BlindScan_Start_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_NextFreq_PARAM
{
	MS_BOOL *bBlindScanEnd;
	MS_BOOL bRet;
}DVBS_BlindScan_NextFreq_PARAM,*PDVBS_BlindScan_NextFreq_PARAM;


typedef struct DLL_PACKED _DVBS_BlindScan_Cancel_PARAM
{
	MS_BOOL bRet;
}DVBS_BlindScan_Cancel_PARAM,*PDVBS_BlindScan_Cancel_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_End_PARAM
{
	MS_BOOL bRet;
}DVBS_BlindScan_End_PARAM,*PDVBS_BlindScan_End_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_GetChannel_PARAM
{
	MS_U16 u16ReadStart;
	MS_U16 *u16TPNum;
	HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable;
	MS_BOOL bRet;
}DVBS_BlindScan_GetChannel_PARAM,*PDVBS_BlindScan_GetChannel_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_GetCurrentFreq_PARAM
{
	MS_U32 *u32CurrentFreq;
	MS_BOOL bRet;
}DVBS_BlindScan_GetCurrentFreq_PARAM,*PDVBS_BlindScan_GetCurrentFreq_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_WaitCurFreqFinished_PARAM
{
	MS_U8 *u8Progress;
	MS_U8 *u8FindNum;
	MS_U8 *substate_reg;
	MS_U32 *u32Data;
	MS_U16 *symbolrate_reg;
	MS_U16 *CFO_reg;
	MS_BOOL bRet;
}DVBS_BlindScan_WaitCurFreqFinished_PARAM,*PDVBS_BlindScan_WaitCurFreqFinished_PARAM;

typedef struct DLL_PACKED _DVBS_BlindScan_GetTunerFreq_PARAM
{
	MS_U16 *u16TunerCenterFreq;
	MS_U16 *u16TunerCutOffFreq;
	MS_BOOL bRet;
}DVBS_BlindScan_GetTunerFreq_PARAM,*PDVBS_BlindScan_GetTunerFreq_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_Init_PARAM
{
	MS_BOOL bRet;
}DVBS_DiSEqC_Init_PARAM,*PDVBS_DiSEqC_Init_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_SetLNBOut_PARAM
{
       MS_BOOL bLow;
	MS_BOOL bRet;
}DVBS_DiSEqC_SetLNBOut_PARAM,*PDVBS_DiSEqC_SetLNBOut_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_GetLNBOut_PARAM
{
       MS_BOOL *bLNBOutLow;
	MS_BOOL bRet;
}DVBS_DiSEqC_GetLNBOut_PARAM,*PDVBS_DiSEqC_GetLNBOut_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_Set22kOnOff_PARAM
{
       MS_BOOL b22kOn;
	MS_BOOL bRet;
}DVBS_DiSEqC_Set22kOnOff_PARAM,*PDVBS_DiSEqC_Set22kOnOff_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_Get22kOnOff_PARAM
{
       MS_BOOL *b22kOn;
	MS_BOOL bRet;
}DVBS_DiSEqC_Get22kOnOff_PARAM,*PDVBS_DiSEqC_Get22kOnOff_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_SendCmd_PARAM
{
       MS_U8 *pCmd;
	MS_U8 u8CmdSize;
	MS_BOOL bRet;
}DVBS_DiSEqC_SendCmd_PARAM,*PDVBS_DiSEqC_SendCmd_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_SetTxToneMode_PARAM
{
       MS_BOOL bTxTone22kOff;
	MS_BOOL bRet;
}DVBS_DiSEqC_SetTxToneMode_PARAM,*PDVBS_DiSEqC_SetTxToneMode_PARAM;

typedef struct DLL_PACKED _DVBS_DiSEqC_SetTone_PARAM
{
       MS_BOOL bTone1;
	MS_BOOL bRet;
}DVBS_DiSEqC_SetTone_PARAM,*PDVBS_DiSEqC_SetTone_PARAM;

typedef struct DLL_PACKED _DVBS_UnicableAGCCheckPower_PARAM
{
       MS_BOOL pbAGCCheckPower;
	MS_BOOL bRet;
}DVBS_UnicableAGCCheckPower_PARAM,*PDVBS_UnicableAGCCheckPower_PARAM;

typedef struct DLL_PACKED _DVBS_GetTsDivNum_PARAM
{
       MS_U32 *u32SymbolRate;
	MS_U8* system_type_reg;
	MS_U8 *code_rate_idx;
	MS_U8 *fec_type_idx;
	MS_U8 *pilot_flag;
	MS_U32 *u32temp;
	MS_U8 *code_rate_reg;
	MS_BOOL bRet;
}DVBS_GetTsDivNum_PARAM,*PDVBS_GetTsDivNum_PARAM;

typedef struct DLL_PACKED _DVBS_GetCurrentDemodCodeRate_PARAM
{
       DMD_DVBS_CODE_RATE_TYPE *pCodeRate;
	MS_BOOL bRet;
}DVBS_GetCurrentDemodCodeRate_PARAM,*PDVBS_GetCurrentDemodCodeRate_PARAM;

typedef struct DLL_PACKED _DVBS_GetTunrSignalLevel_PWR_PARAM
{
       MS_U16 *u16Data;
	MS_BOOL bRet;
}DVBS_GetTunrSignalLevel_PWR_PARAM,*PDVBS_GetTunrSignalLevel_PWR_PARAM;

void DVBSRegisterToUtopia(void);




/*******kernel mode transform structure***************/





/*************************************************/


MS_U32 DVBSOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DVBSClose(void* pInstantTmp);
MS_U32 DVBSIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
//MS_U32 DVBSStr(MS_U32 u32PowerState, void* pModule);


#ifdef __cplusplus
}
#endif

#endif // _DRV_DVBS_H_

