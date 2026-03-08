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
/// @file   drvDMD_ATSC_v2.h
/// @brief  DMD ATSC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_ATSC_V2_H_
#define _DRV_ATSC_V2_H_

#include "drvDMD_ATSC.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifndef DLL_PUBLIC
#define DLL_PUBLIC
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum {
    DMD_ATSC_DRV_CMD_SetDbgLevel,
    DMD_ATSC_DRV_CMD_GetInfo,
    DMD_ATSC_DRV_CMD_GetLibVer,
    DMD_ATSC_DRV_CMD_Init,
    DMD_ATSC_DRV_CMD_Exit,
    DMD_ATSC_DRV_CMD_SetConfig,
    DMD_ATSC_DRV_CMD_SetReset,
    DMD_ATSC_DRV_CMD_Set_QAM_SR,
    DMD_ATSC_DRV_CMD_SetActive,
    DMD_ATSC_DRV_CMD_SetPowerState,
    DMD_ATSC_DRV_CMD_GetLock,
    DMD_ATSC_DRV_CMD_GetModulationMode,
    DMD_ATSC_DRV_CMD_GetSignalStrength,
    DMD_ATSC_DRV_CMD_GetSignalQuality,
    DMD_ATSC_DRV_CMD_GetSNRPercentage,
    DMD_ATSC_DRV_CMD_GET_QAM_SNR,
    DMD_ATSC_DRV_CMD_Read_uCPKT_ERR,
    DMD_ATSC_DRV_CMD_GetPreViterbiBer,
    DMD_ATSC_DRV_CMD_GetPostViterbiBer,
    DMD_ATSC_DRV_CMD_ReadFrequencyOffset,
    DMD_ATSC_DRV_CMD_SetSerialControl,
    DMD_ATSC_DRV_CMD_IIC_BYPASS_MODE,
    DMD_ATSC_DRV_CMD_SWITCH_SSPI_GPIO,
    DMD_ATSC_DRV_CMD_GPIO_GET_LEVEL,
    DMD_ATSC_DRV_CMD_GPIO_SET_LEVEL,
    DMD_ATSC_DRV_CMD_GPIO_OUT_ENABLE,
    DMD_ATSC_DRV_CMD_DoIQSwap,
    DMD_ATSC_DRV_CMD_GetReg,
    DMD_ATSC_DRV_CMD_SetReg,
    DMD_ATSC_DRV_CMD_MD_Init,
    DMD_ATSC_DRV_CMD_MD_Exit,
    DMD_ATSC_DRV_CMD_MD_SetConfig,
    DMD_ATSC_DRV_CMD_MD_SetReset,
    DMD_ATSC_DRV_CMD_MD_Set_QAM_SR,
    DMD_ATSC_DRV_CMD_MD_SetActive,
    DMD_ATSC_DRV_CMD_MD_SetPowerState,
    DMD_ATSC_DRV_CMD_MD_GetLock,
    DMD_ATSC_DRV_CMD_MD_GetModulationMode,
    DMD_ATSC_DRV_CMD_MD_GetSignalStrength,
    DMD_ATSC_DRV_CMD_MD_GetSignalQuality,
    DMD_ATSC_DRV_CMD_MD_GetSNRPercentage,
    DMD_ATSC_DRV_CMD_MD_GET_QAM_SNR,
    DMD_ATSC_DRV_CMD_MD_Read_uCPKT_ERR,
    DMD_ATSC_DRV_CMD_MD_GetPreViterbiBer,
    DMD_ATSC_DRV_CMD_MD_GetPostViterbiBer,
    DMD_ATSC_DRV_CMD_MD_ReadFrequencyOffset,
    DMD_ATSC_DRV_CMD_MD_SetSerialControl,
    DMD_ATSC_DRV_CMD_MD_IIC_BYPASS_MODE,
    DMD_ATSC_DRV_CMD_MD_SWITCH_SSPI_GPIO,
    DMD_ATSC_DRV_CMD_MD_GPIO_GET_LEVEL,
    DMD_ATSC_DRV_CMD_MD_GPIO_SET_LEVEL,
    DMD_ATSC_DRV_CMD_MD_GPIO_OUT_ENABLE,
    DMD_ATSC_DRV_CMD_MD_DoIQSwap,
    DMD_ATSC_DRV_CMD_MD_GetReg,
    DMD_ATSC_DRV_CMD_MD_SetReg
} DMD_ATSC_DRV_COMMAND;

typedef struct DLL_PACKED _ATSC_DBG_LEVEL_PARAM
{
    DMD_ATSC_DbgLv u8DbgLevel;
} ATSC_DBG_LEVEL_PARAM, *PATSC_DBG_LEVEL_PARAM;

typedef struct DLL_PACKED _ATSC_GET_INFO_PARAM
{
    DMD_ATSC_Info* pInfo;
} ATSC_GET_INFO_PARAM, *PATSC_GET_INFO_PARAM;

typedef struct DLL_PACKED _ATSC_GET_LIB_VER_PARAM
{
    const MSIF_Version **ppVersion;
} ATSC_GET_LIB_VER_PARAM, *PATSC_GET_LIB_VER_PARAM;

typedef struct DLL_PACKED _ATSC_INIT_PARAM
{
    MS_U8 id;
    DMD_ATSC_InitData *pDMD_ATSC_InitData;
    MS_U32             u32InitDataLen;
} ATSC_INIT_PARAM, *PATSC_INIT_PARAM;

typedef struct DLL_PACKED _ATSC_SET_CONFIG_PARAM
{
    MS_U8 id;
    DMD_ATSC_DEMOD_TYPE eType;
    MS_BOOL             bEnable;
} ATSC_SET_CONFIG_PARAM, *PATSC_SET_CONFIG_PARAM;

typedef struct DLL_PACKED _ATSC_SET_QAM_SR_PARAM
{
    MS_U8 id;
    DMD_ATSC_DEMOD_TYPE eType;
    MS_U16              symbol_rate;
} ATSC_SET_QAM_SR_PARAM, *PATSC_SET_QAM_SR_PARAM;

typedef struct DLL_PACKED _ATSC_SET_ACTIVE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ATSC_SET_ACTIVE_PARAM, *PATSC_SET_ACTIVE_PARAM;

typedef struct DLL_PACKED _ATSC_SET_POWER_STATE_PARAM
{
    MS_U8 id;
    EN_POWER_MODE u16PowerState;
} ATSC_SET_POWER_STATE_PARAM, *PATSC_SET_POWER_STATE_PARAM;

typedef struct DLL_PACKED _ATSC_GET_LOCK_PARAM
{
    MS_U8 id;
    DMD_ATSC_GETLOCK_TYPE eType;
    DMD_ATSC_LOCK_STATUS  status;
} ATSC_GET_LOCK_PARAM, *PATSC_GET_LOCK_PARAM;

typedef struct DLL_PACKED _ATSC_GET_MODULATION_MODE_PARAM
{
    MS_U8 id;
    DMD_ATSC_DEMOD_TYPE eType;
} ATSC_GET_MODULATION_MODE_PARAM, *PATSC_GET_MODULATION_MODE_PARAM;

typedef struct DLL_PACKED _ATSC_GET_SIGNAL_STRENGTH_PARAM
{
    MS_U8 id;
    MS_U16 u16Strength;
} ATSC_GET_SIGNAL_STRENGTH_PARAM, *PATSC_GET_SIGNAL_STRENGTH_PARAM;

typedef struct DLL_PACKED _ATSC_GET_SIGNAL_QUALITY_PARAM
{
    MS_U8 id;
    DMD_ATSC_SIGNAL_CONDITION eQuality;
} ATSC_GET_SIGNAL_QUALITY_PARAM, *PATSC_GET_SIGNAL_QUALITY_PARAM;

typedef struct DLL_PACKED _ATSC_GET_SNR_PERCENTAGE_PARAM
{
    MS_U8 id;
    MS_U8 u8Percentage;
} ATSC_GET_SNR_PERCENTAGE_PARAM, *PATSC_GET_SNR_PERCENTAGE_PARAM;

typedef struct DLL_PACKED _ATSC_GET_SNR_PARAM
{
    MS_U8 id;
    DMD_ATSC_SNR_DATA snr;
} ATSC_GET_SNR_PARAM, *PATSC_GET_SNR_PARAM;

typedef struct DLL_PACKED _ATSC_GET_UCPKT_ERR_PARAM
{
    MS_U8 id;
    MS_U16 u16PacketErr;
} ATSC_GET_UCPKT_ERR_PARAM, *PATSC_GET_UCPKT_ERR_PARAM;

typedef struct DLL_PACKED _ATSC_GET_BER_PARAM
{
    MS_U8 id;
    DMD_ATSC_BER_DATA ber;
} ATSC_GET_BER_PARAM, *PATSC_GET_BER_PARAM;

typedef struct DLL_PACKED _ATSC_READ_FREQ_OFFSET_PARAM
{
    MS_U8 id;
    DMD_ATSC_CFO_DATA cfo;
} ATSC_READ_FREQ_OFFSET_PARAM, *PATSC_READ_FREQ_OFFSET_PARAM;

typedef struct DLL_PACKED _ATSC_SET_SERIAL_CONTROL_PARAM
{
    MS_U8 id;
    MS_U8 u8TsConfigData;
} ATSC_SET_SERIAL_CONTROL_PARAM, *PATSC_SET_SERIAL_CONTROL_PARAM;

typedef struct DLL_PACKED _ATSC_IIC_BYPASS_MODE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ATSC_IIC_BYPASS_MODE_PARAM, *PATSC_IIC_BYPASS_MODE_PARAM;

typedef struct DLL_PACKED _ATSC_SWITCH_SSPI_GPIO_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ATSC_SWITCH_SSPI_GPIO_PARAM, *PATSC_SWITCH_SSPI_GPIO_PARAM;

typedef struct DLL_PACKED _ATSC_GPIO_LEVEL_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bLevel;
} ATSC_GPIO_LEVEL_PARAM, *PATSC_GPIO_LEVEL_PARAM;

typedef struct DLL_PACKED _ATSC_GPIO_OUT_ENABLE_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bEnableOut;
} ATSC_GPIO_OUT_ENABLE_PARAM, *PATSC_GPIO_OUT_ENABLE_PARAM;

typedef struct DLL_PACKED _ATSC_DO_IQ_SWAP_PARAM
{
    MS_U8 id;
    MS_BOOL bIsQPad;
} ATSC_DO_IQ_SWAP_PARAM, *PATSC_DO_IQ_SWAP_PARAM;

typedef struct DLL_PACKED _ATSC_REG_PARAM
{
    MS_U8 id;
    MS_U16 u16Addr;
    MS_U8 u8Data;
} ATSC_REG_PARAM, *PATSC_REG_PARAM;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC extern void ATSCRegisterToUtopia(FUtopiaOpen ModuleType);
DLL_PUBLIC extern MS_U32 ATSCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
DLL_PUBLIC extern MS_U32 ATSCClose(void* pInstance);
DLL_PUBLIC extern MS_U32 ATSCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ATSC_V2_H_

