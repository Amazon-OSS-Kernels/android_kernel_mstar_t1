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
/// @file   drvDMD_ISDBT_v2.h
/// @brief  DMD ISDBT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_ISDBT_V2_H_
#define _DRV_ISDBT_V2_H_

#include "drvDMD_ISDBT.h"

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
    DMD_ISDBT_DRV_CMD_SetDbgLevel,
    DMD_ISDBT_DRV_CMD_GetInfo,
    DMD_ISDBT_DRV_CMD_GetLibVer,
    DMD_ISDBT_DRV_CMD_Init,
    DMD_ISDBT_DRV_CMD_Exit,
    DMD_ISDBT_DRV_CMD_AdvSetConfig,
    DMD_ISDBT_DRV_CMD_SetPowerState,
    DMD_ISDBT_DRV_CMD_GetLock,
    DMD_ISDBT_DRV_CMD_GetModulationMode,
    DMD_ISDBT_DRV_CMD_GetSignalStrength,
    DMD_ISDBT_DRV_CMD_GetFreqOffset,
    DMD_ISDBT_DRV_CMD_GetSignalQuality,
    DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerA,
    DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerB,
    DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerC,
    DMD_ISDBT_DRV_CMD_GetSignalQualityCombine,
    DMD_ISDBT_DRV_CMD_GetSNR,
    DMD_ISDBT_DRV_CMD_Read_PKT_ERR,
    DMD_ISDBT_DRV_CMD_GetPreViterbiBer,
    DMD_ISDBT_DRV_CMD_GetPostViterbiBer,
    DMD_ISDBT_DRV_CMD_GET_PKT_ERR,
    DMD_ISDBT_DRV_CMD_SetSerialControl,
    DMD_ISDBT_DRV_CMD_IIC_BYPASS_MODE,
    DMD_ISDBT_DRV_CMD_SWITCH_SSPI_GPIO,
    DMD_ISDBT_DRV_CMD_GPIO_GET_LEVEL,
    DMD_ISDBT_DRV_CMD_GPIO_SET_LEVEL,
    DMD_ISDBT_DRV_CMD_GPIO_OUT_ENABLE,
    DMD_ISDBT_DRV_CMD_DoIQSwap,
    DMD_ISDBT_DRV_CMD_GetReg,
    DMD_ISDBT_DRV_CMD_SetReg,
    DMD_ISDBT_DRV_CMD_MD_Init,
    DMD_ISDBT_DRV_CMD_MD_Exit,
    DMD_ISDBT_DRV_CMD_MD_AdvSetConfig,
    DMD_ISDBT_DRV_CMD_MD_SetActive,
    DMD_ISDBT_DRV_CMD_MD_SetPowerState,
    DMD_ISDBT_DRV_CMD_MD_GetLock,
    DMD_ISDBT_DRV_CMD_MD_GetModulationMode,
    DMD_ISDBT_DRV_CMD_MD_GetSignalStrength,
    DMD_ISDBT_DRV_CMD_MD_GetFreqOffset,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQuality,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerA,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerB,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerC,
    DMD_ISDBT_DRV_CMD_MD_GetSignalQualityCombine,
    DMD_ISDBT_DRV_CMD_MD_GetSNR,
    DMD_ISDBT_DRV_CMD_MD_Get_PKT_ERR,
    DMD_ISDBT_DRV_CMD_MD_GetPreViterbiBer,
    DMD_ISDBT_DRV_CMD_MD_GetPostViterbiBer,
    DMD_ISDBT_DRV_CMD_MD_Read_PKT_ERR,
    DMD_ISDBT_DRV_CMD_MD_SetSerialControl,
    DMD_ISDBT_DRV_CMD_MD_IIC_BYPASS_MODE,
    DMD_ISDBT_DRV_CMD_MD_SWITCH_SSPI_GPIO,
    DMD_ISDBT_DRV_CMD_MD_GPIO_GET_LEVEL,
    DMD_ISDBT_DRV_CMD_MD_GPIO_SET_LEVEL,
    DMD_ISDBT_DRV_CMD_MD_GPIO_OUT_ENABLE,
    DMD_ISDBT_DRV_CMD_MD_DoIQSwap,
    DMD_ISDBT_DRV_CMD_MD_GetReg,
    DMD_ISDBT_DRV_CMD_MD_SetReg
} DMD_ISDBT_DRV_COMMAND;

typedef struct DLL_PACKED _ISDBT_DBG_LEVEL_PARAM
{
    DMD_ISDBT_DbgLv u8DbgLevel;
} ISDBT_DBG_LEVEL_PARAM, *PISDBT_DBG_LEVEL_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_INFO_PARAM
{
    DMD_ISDBT_Info* pInfo;
} ISDBT_GET_INFO_PARAM, *PISDBT_GET_INFO_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_LIB_VER_PARAM
{
    const MSIF_Version **ppVersion;
} ISDBT_GET_LIB_VER_PARAM, *PISDBT_GET_LIB_VER_PARAM;

typedef struct DLL_PACKED _ISDBT_INIT_PARAM
{
    MS_U8 id;
    DMD_ISDBT_InitData *pDMD_ISDBT_InitData;
    MS_U32             u32InitDataLen;
} ISDBT_INIT_PARAM, *PISDBT_INIT_PARAM;

typedef struct DLL_PACKED _ISDBT_SET_CONFIG_PARAM
{
    MS_U8 id;
    DMD_ISDBT_DEMOD_TYPE eType;
    MS_BOOL             bEnable;
} ISDBT_SET_CONFIG_PARAM, *PISDBT_SET_CONFIG_PARAM;

typedef struct DLL_PACKED _ISDBT_SET_POWER_STATE_PARAM
{
    MS_U8 id;
    EN_POWER_MODE u16PowerState;
} ISDBT_SET_POWER_STATE_PARAM, *PISDBT_SET_POWER_STATE_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_LOCK_PARAM
{
    MS_U8 id;
    DMD_ISDBT_GETLOCK_TYPE eType;
    DMD_ISDBT_LOCK_STATUS  status;
} ISDBT_GET_LOCK_PARAM, *PISDBT_GET_LOCK_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_MODULATION_MODE_PARAM
{
    MS_U8 id;
    EN_ISDBT_Layer eLayerIndex;
    sISDBT_MODULATION_MODE IsdbtModulationMode;
} ISDBT_GET_MODULATION_MODE_PARAM, *PISDBT_GET_MODULATION_MODE_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_SIGNAL_STRENGTH_PARAM
{
    MS_U8 id;
    MS_U16 u16Strength;
} ISDBT_GET_SIGNAL_STRENGTH_PARAM, *PISDBT_GET_SIGNAL_STRENGTH_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_FREQ_OFFSET_PARAM
{
    MS_U8 id;
    DMD_ISDBT_CFO_DATA cfo;
} ISDBT_GET_FREQ_OFFSET_PARAM, *PISDBT_GET_FREQ_OFFSET_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_SIGNAL_QUALITY_PARAM
{
    MS_U8 id;
    MS_U16 u16SignalQuality;
} ISDBT_GET_SIGNAL_QUALITY_PARAM, *PISDBT_GET_SIGNAL_QUALITY_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_SNR_PARAM
{
    MS_U8 id;
    DMD_ISDBT_SNR_DATA snr;
} ISDBT_GET_SNR_PARAM, *PISDBT_GET_SNR_PARAM;

typedef struct DLL_PACKED _ISDBT_READ_PKT_ERR_PARAM
{
    MS_U8 id;
    EN_ISDBT_Layer eIsdbtLayer;
    MS_U16 u16PacketErr;
} ISDBT_READ_PKT_ERR_PARAM, *PISDBT_READ_PKT_ERR_PARAM;

typedef struct DLL_PACKED _ISDBT_GET_BER_PARAM
{
    MS_U8 id;
    DMD_ISDBT_GET_BER_VALUE ber;
} ISDBT_GET_BER_PARAM, *PISDBT_GET_BER_PARAM;

typedef struct DLL_PACKED _ISDBT_SET_SERIAL_CONTROL_PARAM
{
    MS_U8 id;
    MS_U8 u8TsConfigData;
} ISDBT_SET_SERIAL_CONTROL_PARAM, *PISDBT_SET_SERIAL_CONTROL_PARAM;

typedef struct DLL_PACKED _ISDBT_IIC_BYPASS_MODE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ISDBT_IIC_BYPASS_MODE_PARAM, *PISDBT_IIC_BYPASS_MODE_PARAM;

typedef struct DLL_PACKED _ISDBT_SWITCH_SSPI_GPIO_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} ISDBT_SWITCH_SSPI_GPIO_PARAM, *PISDBT_SWITCH_SSPI_GPIO_PARAM;

typedef struct DLL_PACKED _ISDBT_GPIO_LEVEL_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bLevel;
} ISDBT_GPIO_LEVEL_PARAM, *PISDBT_GPIO_LEVEL_PARAM;

typedef struct DLL_PACKED _ISDBT_GPIO_OUT_ENABLE_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bEnableOut;
} ISDBT_GPIO_OUT_ENABLE_PARAM, *PISDBT_GPIO_OUT_ENABLE_PARAM;

typedef struct DLL_PACKED _ISDBT_DO_IQ_SWAP_PARAM
{
    MS_U8 id;
    MS_BOOL bIsQPad;
} ISDBT_DO_IQ_SWAP_PARAM, *PISDBT_DO_IQ_SWAP_PARAM;

typedef struct DLL_PACKED _ISDBT_REG_PARAM
{
    MS_U8 id;
    MS_U16 u16Addr;
    MS_U8 u8Data;
} ISDBT_REG_PARAM, *PISDBT_REG_PARAM;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC extern void ISDBTRegisterToUtopia(FUtopiaOpen ModuleType);
DLL_PUBLIC extern MS_U32 ISDBTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
DLL_PUBLIC extern MS_U32 ISDBTClose(void* pInstance);
DLL_PUBLIC extern MS_U32 ISDBTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ISDBT_V2_H_

