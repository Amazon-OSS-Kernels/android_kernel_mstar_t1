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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mdrv_mhl.h
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MHL_H_
#define _DRV_MHL_H_

#include "MsCommon.h"
#include "mdrv_mhl_st.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

enum MHL_SYSTEM_CONTROL_TYPE
{
    MHL_CABLE_PLUGGED = 0,
    MHL_CABLE_UNPLUGGED,
    MHL_INPUT_SOURCE_CHANGE,
    MHL_CBUS_UNCONNECTION,
    MHL_CBUS_CONNECTION,
    MHL_CBUS_NORMAL_24BIT_MODE,
    MHL_CBUS_PACKET_PIXEL_MODE,
    MHL_CBUS_AUTO_EQ_SETTING,
    MHL_CBUS_WAKE_UP_DETECT,
    MHL_CBUS_TEST_RASING,
    MHL_CBUS_TEST_FALLING,
};

enum MHL_CABLE_DETECT_TYPE
{
    MHL_CABLE_DETECT_PORTA_FLAG = 0x01,
    MHL_CABLE_DETECT_PORTB_FLAG = 0x02,
    MHL_CABLE_DETECT_PORTC_FLAG = 0x04,
    MHL_CABLE_DETECT_PORTD_FLAG = 0x08,
};

enum MHL_ECBUS_STATUS_TYPE
{
    MHL_CBUS_TX_QUEUE_FREEZE_FLAG = BIT(0),
};

#define MHL_UTOPIA_VERSION_10       1
#define MHL_UTOPIA_VERSION_20       2

#define MHL_USE_UTOPIA_VERSION      MHL_UTOPIA_VERSION_20

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
MS_U32 mdrv_mhl_STREventProc(EN_POWER_MODE usPowerState);
void mdrv_mhl_MHLSupportPath(MS_U8 ucSelect);
void mdrv_mhl_Initial(MS_U8 *edid, MS_U8 *DevCap);
MS_U16 mdrv_mhl_Handler(void);
MS_BOOL mdrv_mhl_AutoSwitchHandler(MS_BOOL bReset, MS_U8 *ucCbusPath);
MS_U32 mdrv_mhl_SetPowerState(EN_POWER_MODE usPowerState);
void mdrv_mhl_CbusControl(MS_U8 ucState);
void mdrv_mhl_LoadEDID(MS_U8 *edid);
void mdrv_mhl_ReadEDID(MS_U16 usSize, MS_U8 *edid);
void mdrv_mhl_LoadDeviceCapability(MS_U8 *devcap);
void mdrv_mhl_SetVenderID(MS_U8 ucVenderID);
void mdrv_mhl_InvertCableDetect(MS_BOOL bCableDetectInvert);
void mdrv_mhl_VbusConfigSetting(MS_U8 ucState);
void mdrv_mhl_AdjustSettingIControl(MS_U8 ucIControl);
void mdrv_mhl_AdjustImpedanceSetting(MS_U8 ucImpedance);
MS_BOOL mdrv_mhl_GetCableDetectFlag(void);
MS_BOOL mdrv_mhl_GetCbusConnectFlag(void);
MS_BOOL mdrv_mhl_CbusWakeupIntFlag(void);
MS_BOOL mdrv_mhl_SrcRCPSupportFlag(void);
MS_BOOL mdrv_mhl_SrcRAPSupportFlag(void);
MS_BOOL mdrv_mhl_CbusGetStatusFlag(MS_U8 ucState);
MS_BOOL mdrv_mhl_SendRAPCommand(MS_U8 ucKeyCode);
MS_BOOL mdrv_mhl_SendRCPAutoReleaseCmd(MS_U8 ucKeyCode);
MS_BOOL mdrv_mhl_SendUCPCommand(MS_U8 ucKeyCode);
MS_BOOL mdrv_mhl_SendWriteBurst(MS_U8 ucAddr, MS_U8 ucLength, MS_U8 *ucData);
MS_BOOL mdrv_mhl_CbusSendUserWriteBurst(MS_U8 ucLength, MS_U8 *ucData);
MS_BOOL mdrv_mhl_GetDeviceCapability(MS_U16 usDevcapMask, MS_U8 *ucDevcap);
MS_BOOL mdrv_mhl_GetExtendDeviceCapability(MS_U16 usExtendDevcapMask, MS_U8 *ucExtendDevcap);
MS_BOOL mdrv_mhl_GetDeviceVenderID(MS_U8 *ucVenderID);
MS_BOOL mdrv_mhl_GetWriteBurstData(MS_U8 *ucData);
void mdrv_mhl_RegisterCallBackFunctions(MS_BOOL bRCPfunctionFlag, MS_BOOL bRAPfunctionFlag);
void mdrv_mhl_RegisterRcpCallBackFunction(MS_BOOL bRCPfunctionFlag);
void mdrv_mhl_RegisterRapCallBackFunction(MS_BOOL bRAPfunctionFlag);
void mdrv_mhl_RegisterUcpCallBackFunction(MS_BOOL bUCPfunctionFlag);
void mdrv_mhl_RegisterAttCallBackFunction(MS_BOOL bATTfunctionFlag);
void mdrv_mhl_RegisterRbpCallBackFunction(MS_BOOL bRBPfunctionFlag);
stMHL_INITIAL_CONFIG_INFO mdrv_mhl_GetConfiguration(void);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
MS_U32 mdrv_mhl_STREventProc(void* pModule, EN_POWER_MODE usPowerState);
void mdrv_mhl_MHLSupportPath(void* pInstance, MS_U8 ucSelect);
void mdrv_mhl_Initial(void* pInstance, MS_U8 *edid, MS_U8 *DevCap);
MS_U16 mdrv_mhl_Handler(void* pInstance);
MS_BOOL mdrv_mhl_AutoSwitchHandler(void* pInstance, MS_BOOL bReset, MS_U8 *ucCbusPath);
MS_U32 mdrv_mhl_SetPowerState(void* pInstance, EN_POWER_MODE usPowerState);
void mdrv_mhl_CbusControl(void* pInstance, MS_U8 ucState);
void mdrv_mhl_LoadEDID(void* pInstance, MS_U8 *edid);
void mdrv_mhl_ReadEDID(void* pInstance, MS_U16 usSize, MS_U8 *edid);
void mdrv_mhl_LoadDeviceCapability(void* pInstance, MS_U8 *devcap);
void mdrv_mhl_SetVenderID(void* pInstance, MS_U8 ucVenderID);
void mdrv_mhl_InvertCableDetect(void* pInstance, MS_BOOL bCableDetectInvert);
void mdrv_mhl_VbusConfigSetting(void* pInstance, MS_U8 ucState);
void mdrv_mhl_AdjustSettingIControl(void* pInstance, MS_U8 ucIControl);
void mdrv_mhl_AdjustImpedanceSetting(void* pInstance, MS_U8 ucImpedance);
MS_BOOL mdrv_mhl_GetCableDetectFlag(void* pInstance);
MS_BOOL mdrv_mhl_GetCbusConnectFlag(void* pInstance);
MS_BOOL mdrv_mhl_CbusWakeupIntFlag(void* pInstance);
MS_BOOL mdrv_mhl_SrcRCPSupportFlag(void* pInstance);
MS_BOOL mdrv_mhl_SrcRAPSupportFlag(void* pInstance);
MS_BOOL mdrv_mhl_CbusGetStatusFlag(void* pInstance, MS_U8 ucState);
MS_BOOL mdrv_mhl_SendRAPCommand(void* pInstance, MS_U8 ucKeyCode);
MS_BOOL mdrv_mhl_SendRCPAutoReleaseCmd(void* pInstance, MS_U8 ucKeyCode);
MS_BOOL mdrv_mhl_SendUCPCommand(void* pInstance, MS_U8 ucKeyCode);
MS_BOOL mdrv_mhl_SendWriteBurst(void* pInstance, MS_U8 ucAddr, MS_U8 ucLength, MS_U8 *ucData);
MS_BOOL mdrv_mhl_CbusSendUserWriteBurst(void* pInstance, MS_U8 ucLength, MS_U8 *ucData);
MS_BOOL mdrv_mhl_GetDeviceCapability(void* pInstance, MS_U16 usDevcapMask, MS_U8 *ucDevcap);
MS_BOOL mdrv_mhl_GetExtendDeviceCapability(void* pInstance, MS_U16 usExtendDevcapMask, MS_U8 *ucExtendDevcap);
MS_BOOL mdrv_mhl_GetDeviceVenderID(void* pInstance, MS_U8 *ucVenderID);
MS_BOOL mdrv_mhl_GetWriteBurstData(void* pInstance, MS_U8 *ucData);
void mdrv_mhl_RegisterCallBackFunctions(void* pInstance, MS_BOOL bRCPfunctionFlag, MS_BOOL bRAPfunctionFlag);
void mdrv_mhl_RegisterRcpCallBackFunction(void* pInstance, MS_BOOL bRCPfunctionFlag);
void mdrv_mhl_RegisterRapCallBackFunction(void* pInstance, MS_BOOL bRAPfunctionFlag);
void mdrv_mhl_RegisterUcpCallBackFunction(void* pInstance, MS_BOOL bUCPfunctionFlag);
void mdrv_mhl_RegisterAttCallBackFunction(void* pInstance, MS_BOOL bATTfunctionFlag);
void mdrv_mhl_RegisterRbpCallBackFunction(void* pInstance, MS_BOOL bRBPfunctionFlag);
stMHL_INITIAL_CONFIG_INFO mdrv_mhl_GetConfiguration(void* pInstance);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void mdrv_mhl_MDCMDEchoCommand(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine);
void mdrv_mhl_MDCMDGetInfo(void* pInstance, MS_U64* u64ReqHdl);
#endif

#endif

#endif //_MDRV_MHL_H_