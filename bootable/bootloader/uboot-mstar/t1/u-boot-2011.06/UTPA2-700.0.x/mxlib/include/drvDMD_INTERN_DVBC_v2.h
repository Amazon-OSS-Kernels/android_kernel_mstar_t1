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
/// @file   drvDMD_DVBC_INTERN.h
/// @brief  DVBC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBC_V2_H_
#define _DRV_DVBC_V2_H_

//#include "MsCommon.h"
//#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBC.h"
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DLL_PUBLIC
#define DLL_PUBLIC
#endif


#define DMD_DVBC_MAX_DEMOD_NUM          1



//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
typedef struct DLL_PACKED
{
	MS_BOOL b_DCOFF_IsDVBC;
	MS_BOOL b_IsInit_DVBCCreat;
	EN_POWER_MODE eLastState;
} DMD_DVBC_SHARE_MEMORY_InitData;

typedef struct DLL_PACKED
{
    MS_U16 u16Version;
    MS_U16 u16SymbolRate;
    DMD_DVBC_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq;
    MS_BOOL bSpecInv;
    MS_BOOL bSerialTS;
    MS_U8 u8SarValue;
    MS_U32 u32ChkScanTimeStart;
    DMD_DVBC_LOCK_STATUS eLockStatus;
    MS_U16 u16Strength;
    MS_U16 u16Quality;
    MS_U32 u32Intp;
    MS_U32 u32FcFs;
    MS_U8 u8Qam;
    MS_U16 u16SymbolRateHal;
} DMD_DVBC_SHARE_MEMORY_PreviousData;

typedef struct DLL_PACKED
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
    MS_U8 *u8DMD_DVBC_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBC_DSPRegInitSize;
    MS_U8 *u8DMD_DVBC_InitExt; // TODO use system variable type
} DMD_DVBC_InitData_Transform;

typedef struct DLL_PACKED
{
    DMD_DVBC_SHARE_MEMORY_InitData  sDMD_DVBC_SHARE_MEMORY_InitData;
	DMD_DVBC_SHARE_MEMORY_PreviousData sDMD_DVBC_SHARE_MEMORY_PreviousData;
	DMD_DVBC_InitData_Transform sDMD_DVBC_InitData_Transform;
} DMD_DVBC_ResData;






typedef enum {
    DMD_DVBC_DRV_CMD_Dual_Public_Init,
    DMD_DVBC_DRV_CMD_Dual_Individual_Init,
    DMD_DVBC_DRV_CMD_Init,
    DMD_DVBC_DRV_CMD_Exit,
    DMD_DVBC_DRV_CMD_SetDbgLevel,
    DMD_DVBC_DRV_CMD_GetInfo,
    DMD_DVBC_DRV_CMD_GetLibVer,
    DMD_DVBC_DRV_CMD_GetFWVer,
    DMD_DVBC_DRV_CMD_GetDSPReg,
    DMD_DVBC_DRV_CMD_SetDSPReg,
    DMD_DVBC_DRV_CMD_GetReg,
    DMD_DVBC_DRV_CMD_SetReg,
    DMD_DVBC_DRV_CMD_SetSerialControl,
    DMD_DVBC_DRV_CMD_SetConfig,
    DMD_DVBC_DRV_CMD_SetSetConfig_symbol_rate_list,
    DMD_DVBC_DRV_CMD_SetActive,
    DMD_DVBC_DRV_CMD_GetLockWithRFPower,
    DMD_DVBC_DRV_CMD_GetSignalStrengthWithRFPower,
    DMD_DVBC_DRV_CMD_GetSignalQualityWithRFPower,
    DMD_DVBC_DRV_CMD_ActiveDmdSwitch,
    DMD_DVBC_DRV_CMD_GetSNR,
    DMD_DVBC_DRV_CMD_GetPostViterbiBer,
    DMD_DVBC_DRV_CMD_GetPacketErr,
    DMD_DVBC_DRV_CMD_GetCellID,
    DMD_DVBC_DRV_CMD_GetStatus,
    DMD_DVBC_DRV_CMD_SetPowerState,

    //waiting add
    DMD_DVBC_DRV_CMD_GetIFAGC
} DMD_DVBC_DRV_COMMAND;





typedef struct DLL_PACKED _DVBC_Dual_Public_Init_PARAM
{
	MS_U8 u8AGC_Tristate_Ctrl;
	MS_U8 u8Sar_Channel;
	MS_BOOL ret;
} DVBC_Dual_Public_Init_PARAM,*PDVBC_Dual_Public_Init_PARAM;

typedef struct DLL_PACKED _DVBC_Dual_Individual_Init_PARAM
{
	DMD_DVBC_InitData_Transform *pDMD_DVBC_InitData;
	MS_U32 u32InitDataLen;
	MS_BOOL ret;
}DVBC_Dual_Individual_Init_PARAM,*PDVBC_Dual_Individual_Init_PARAM;

typedef struct DLL_PACKED _DVBC_Init_PARAM
{
	DMD_DVBC_InitData_Transform *pDMD_DVBC_InitData;
	MS_U32 u32InitDataLen;
	MS_BOOL ret;
}DVBC_Init_PARAM,*PDVBC_Init_PARAM;

typedef struct DLL_PACKED _DVBC_SetDbgLevel_PARAM
{
	DMD_DVBC_DbgLv u8DbgLevel;
	MS_BOOL ret;
}DVBC_SetDbgLevel_PARAM,*PDVBC_SetDbgLevel_PARAM;

typedef struct DLL_PACKED _DVBC_EXIT_PARAM_PARAM
{
	MS_BOOL ret;
}DVBC_EXIT_PARAM_PARAM,*PDVBC_EXIT_PARAM_PARAM;

typedef struct DLL_PACKED _DVBC_GetInfo_PARAM
{
	const DMD_DVBC_Info* ret_info;
}DVBC_GetInfo_PARAM,*PDVBC_GetInfo_PARAM;

typedef struct _DVBC_GetLibVer_PARAM
{
	const MSIF_Version **ppVersion;
	MS_BOOL ret;
}DVBC_GetLibVer_PARAM,*PDVBC_GetLibVer_PARAM;

typedef struct DLL_PACKED _DVBC_GetFWVer_PARAM
{
	MS_U16 *ver;
	MS_BOOL ret;
}DVBC_GetFWVer_PARAM,*PDVBC_GetFWVer_PARAM;


typedef struct DLL_PACKED _DVBC_GetDSPReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 *pu8Data;
	MS_BOOL ret;
}DVBC_GetDSPReg_PARAM,*PDVBC_GetDSPReg_PARAM;

typedef struct DLL_PACKED _DVBC_SetDSPReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 pu8Data;
	MS_BOOL ret;
}DVBC_SetDSPReg_PARAM,*PDVBC_SetDSPReg_PARAM;


typedef struct DLL_PACKED _DVBC_GetReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 *pu8Data;
	MS_BOOL ret;
}DVBC_GetReg_PARAM,*PDVBC_GetReg_PARAM;

typedef struct DLL_PACKED _DVBC_SetReg_PARAM
{
	MS_U16 u16Addr;
	MS_U8 u8Data;
	MS_BOOL ret;
}DVBC_SetReg_PARAM,*PDVBC_SetReg_PARAM;


typedef struct DLL_PACKED _DVBC_SetSerialControl_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBC_SetSerialControl_PARAM,*PDVBC_SetSerialControl_PARAM;


typedef struct DLL_PACKED _DVBC_SetConfig_Symbol_rate_list_PARAM
{
	MS_U16 u16SymbolRate;
	DMD_DVBC_MODULATION_TYPE eQamMode;
	MS_U32 u32IFFreq;
	MS_BOOL bSpecInv;
	MS_BOOL bSerialTS;
	MS_U16 *pu16_symbol_rate_list;
	MS_U8 u8_symbol_rate_list_num;
	MS_BOOL ret;
}DVBC_SetConfig_Symbol_rate_list_PARAM,*PDVBC_SetConfig_Symbol_rate_list_PARAM;

typedef struct DLL_PACKED _DVBC_SetActive_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBC_SetActive_PARAM,*PDVBC_SetActive_PARAM;

/*
typedef struct DLL_PACKED _DVBC_GetLockWithRFPower_Transform
{
	DMD_DVBC_GETLOCK_TYPE eType;
	DMD_DVBC_LOCK_STATUS *eLockStatus;
	MS_BOOL ret;
}DVBC_GetLockWithRFPower_Transform,*PDVBC_GetLockWithRFPower_Transform;
*/

typedef struct DLL_PACKED _DVBC_GetLockWithRFPower
{
	DMD_DVBC_GETLOCK_TYPE eType;
	MS_U32 u32CurrRFPowerDbm;
	MS_U32 u32NoChannelRFPowerDbm;
	MS_U32 u32TimeInterval;
	DMD_DVBC_LOCK_STATUS *eLockStatus;
	MS_BOOL ret;
}DVBC_GetLockWithRFPower,*PDVBC_GetLockWithRFPower;

/*
typedef struct _DVBC_GetSignalStrengthWithRFPower_PARAM
{
	MS_U16 *u16Strength;
	MS_BOOL ret;
}DVBC_GetSignalStrengthWithRFPower_PARAM,*PDVBC_GetSignalStrengthWithRFPower_PARAM;


typedef struct _DVBC_GetSignalQualityWithRFPower_PARAM
{
	MS_U16 *u16Quality;
	MS_BOOL ret;
}DVBC_GetSignalQualityWithRFPower_PARAM,*PDVBC_GetSignalQualityWithRFPower_PARAM;
*/

typedef struct DLL_PACKED _DVBC_ActiveDmdSwitch_PARAM
{
	MS_U8 demod_no;
	MS_BOOL ret;
}DVBC_ActiveDmdSwitch_PARAM,*PDVBC_ActiveDmdSwitch_PARAM;

//waiting add
typedef struct _DVBC_GetIFAGC_PARAM
{
	MS_U8 *ifagc_reg;
	MS_U8 *ifagc_reg_lsb;
	MS_U16 *ifagc_err_reg;
	MS_BOOL ret;
}DVBC_GetIFAGC_PARAM,*PDVBC_GetIFAGC_PARAM;


//typedef struct DLL_PACKED _DVBC_GetPacketErr_PARAM

typedef struct _DVBC_GetSNR_PARAM
{
	MS_U16 *snr_reg;
	MS_BOOL ret;
}DVBC_GetSNR_PARAM,*PDVBC_GetSNR_PARAM;


typedef struct _DVBC_GetPostViterbiBer_PARAM
{
	MS_U32 *BitErr_reg;
	MS_U16 *BitErrPeriod_reg;
	MS_BOOL ret;
}DVBC_GetPostViterbiBer_PARAM,*PDVBC_GetPostViterbiBer_PARAM;


typedef struct DLL_PACKED _DVBC_GetPacketErr_PARAM
{
	MS_U16 *pktErr;
	MS_BOOL ret;
}DVBC_GetPacketErr_PARAM,*PDVBC_GetPacketErr_PARAM;

typedef struct DLL_PACKED _DVBC_GetCellID_PARAM
{
	MS_U16 *u16CellID;
	MS_BOOL ret;
}DVBC_GetCellID_PARAM,*PDVBC_GetCellID_PARAM;


typedef struct _DVBC_GetStatus_PARAM
{
	DMD_DVBC_MODULATION_TYPE *pQAMMode;
	MS_U16 *u16SymbolRate;
	MS_U32 *config_Fc_reg;
	MS_U32 *Fc_over_Fs_reg;
	MS_U16 *Cfo_offset_reg;
	MS_BOOL ret;
}DVBC_GetStatus_PARAM,*PDVBC_GetStatus_PARAM;


typedef struct DLL_PACKED _DVBC_SetPowerState_PARAM
{
	EN_POWER_MODE u16PowerState;
	MS_U32 ret_U32;
}DVBC_SetPowerState_PARAM,*PDVBC_SetPowerState_PARAM;

void DVBCRegisterToUtopia(void);




/*******kernel mode transform structure***************/





/*************************************************/


MS_U32 DVBCOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DVBCClose(void* pInstantTmp);
MS_U32 DVBCIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
//MS_U32 DVBCStr(MS_U32 u32PowerState, void* pModule);


#ifdef __cplusplus
}
#endif

#endif // _DRV_DVBC_H_

