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
/// @file   Mapi_mhl.h
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
    1. Check whether MHL cable is plugged or not in Rx side
    2. Disable R-term and output Vbus
    3. Check whether Cbus is connected(discovery pulse completed)
    4. Sink -> Source: "SET_HPD"
    5. Sink -> Source: Write_Stat - "DCAP_RDY" and Write_INT - "DCAP_CHG"
    6. Sink -> Source: Write_Stat - "PATH_EN"
*/


#ifndef _API_MHL_PRIVATE_H_
#define _API_MHL_PRIVATE_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#include "mdrv_mhl_st.h"
#include "apiMHL.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _MHL_RESOURCE_PRIVATE
{
    MS_U8 ucInitialIndex;
    MS_U8 ucMHLSupportPath;
    MS_U8 ucCurrentHDMIPort;
    MS_U8 ucCurrentQueueIndex;
    MS_U8 ucChipCapability;
    MS_U8 ucDeviceVenderID;
    MS_U8 ucTMDSClockMode;
    MS_U8 ucAskData;
    MS_U8 ucMSGData[2];
    MS_U8 ucReceiveCounter;
    MS_U8 ucEMSCReceiveCounter;
    MS_U8 ucSourceVenderID;
    MS_U8 ucProcDoneIndex;
    MS_U8 ucWriteBurstState;
    MS_U8 ucFeatureRequestState;
    MS_U8 ucECbusState;
    MS_U8 ucECbusTrainState;
    MS_U8 ucBISTModeState;
    MS_U8 ucPreRCPKeyCode;
    MS_U8 ucWriteBurstSequence;
    MS_U8 uc3DTotalEntryDTD;
    MS_U8 uc3DTotalEntryVIC;
    MS_U8 ucDeviceCapabilityTable[MHL_DEVICE_CAPABILITY_SIZE +MHL_XDEVICE_CAPABILITY_SIZE];
    MS_U8 ucSourceDevcapTable[MHL_DEVICE_CAPABILITY_SIZE +MHL_XDEVICE_CAPABILITY_SIZE];
    MS_U8 uc3DInformationDTD[MHL_3D_DTD_INFORMATION_LENGTH];
    MS_U8 uc3DInformationVIC[MHL_3D_VIC_INFORMATION_LENGTH];
    MS_U8 ucWriteBurstAddress;
    MS_U8 ucWriteBurstLength;
    MS_U8 ucWriteBurstInformation[MHL_MSC_SCRATCHPAD_SIZE +1];
    MS_U8 ucEMSCSendStatus;
    MS_U8 ucCallBackFunctionIndex;
    MS_U8 ucMHLPollingStack[MHL_POLLING_STACK_SIZE];
    MS_U8 ucPowerOnPatchState;
    MS_U8 ucHighEndVICTotalEntry;
    MS_U16 usDisplayIndex;
    MS_U16 usDeviceIndex;
    MS_U16 usEMSCRxBufferSize;
    MS_U16 usEMSCTxBufferSize;
    MS_U16 usReadDevcapMask;
    MS_U16 usReadExtendDevcapMask;
    MS_U16 usMSGKeyInfo;
    MS_U16 usHighEndVICTable[MHL_HEV_VIC_INFORMATION_LENGTH];
    MS_U16 usEMSCSupportTable[MHL_EMSC_SUPPORT_INFORMATION_LENGTH];
    MS_U32 ulCurrentTimerTick;
    MS_U32 ulSystemIndex;
    MS_U32 ulTimerIndex;
    MS_U32 ulCbusReceiveIndex;
    MS_U32 ulCbusSendIndex;
    MS_U32 ulEnableIndex;
    MS_U32 ulAudioTunnelTable[MHL_AUDIO_TUNNEL_INFORMATION_LENGTH];
    MS_S32 slMHLPollingTaskID;
    MS_VIRT ulRiuBaseAddress;
#if !defined (__aarch64__)
    MS_U32 ulRiuBaseAddressDummy;
#endif
    MS_VIRT ulPMRiuBaseAddress;
#if !defined (__aarch64__)
    MS_U32 ulPMRiuBaseAddressDummy;
#endif
    EN_POWER_MODE usPrePowerState;
    stMHL_TIMER_EVENT_INFO stTimerEventInfo[MHL_TIMER_EVENT_MASK];
    stMHL_QUEUE_INFO stCbusQueueInfo[MHL_CBUS_TOTAL_QUEUE_NUM];
    stMHL_VIRTUAL_CHANNEL_INFO stVirtualChannelInfo[MHL_VIRTUAL_CHANNEL_LENGTH];
    stMHL_BIST_MODE_INFO stBISTParameterInfo;
    stMHL_SIGNAL_STATUS_INFO stSignalStatusInfo;
}MHL_RESOURCE_PRIVATE;

typedef void    (*IOCTL_MHL_SUPPORT_PATH)           (void*, MS_U8);
typedef void    (*IOCTL_MHL_INITIAL)                 (void*, MS_U8*, MS_U8*);
typedef MS_U16  (*IOCTL_MHL_HANDLER)                 (void*);
typedef MS_BOOL (*IOCTL_MHL_AUTO_SWITCH)            (void*, MS_BOOL, MS_U8*);
typedef MS_U32  (*IOCTL_MHL_SET_POWER_STATE)        (void*, EN_POWER_MODE);
typedef void    (*IOCTL_MHL_CBUS_CONTROL)            (void*, MS_U8);
typedef void    (*IOCTL_MHL_LOAD_EDID)               (void*, MS_U8*);
typedef void    (*IOCTL_MHL_READ_EDID)               (void*, MS_U8, MS_U8*);
typedef void    (*IOCTL_MHL_LOAD_DEVCAP)             (void*, MS_U8*);
typedef void    (*IOCTL_MHL_SET_VENDER_ID)           (void*, MS_U8);
typedef void    (*IOCTL_MHL_CABLE_DETECT_INVERT)    (void*, MS_BOOL);
typedef void    (*IOCTL_MHL_VBUS_CONFIG)             (void*, MS_U8);
typedef void    (*IOCTL_MHL_ADJUST_I_CONTROL)        (void*, MS_U8);
typedef void    (*IOCTL_MHL_ADJUST_IMPEDANCE)        (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_CABLE_DETECT_FLAG)       (void*);
typedef MS_BOOL (*IOCTL_MHL_CBUS_CONNECT_FLAG)       (void*);
typedef MS_BOOL (*IOCTL_MHL_CBUS_WAKEUP_FLAG)        (void*);
typedef MS_BOOL (*IOCTL_MHL_SOURCE_SUPPORT_RCP)      (void*);
typedef MS_BOOL (*IOCTL_MHL_SOURCE_SUPPORT_RAP)      (void*);
typedef MS_BOOL (*IOCTL_MHL_GET_STATUS_FLAG)         (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_SEND_RAP_COMMAND)        (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_SEND_RCP_COMMAND)        (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_SEND_UCP_COMMAND)        (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_SEND_WRITE_BURST)        (void*, MS_U8, MS_U8, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_SEND_USER_WRITE_BURST)  (void*, MS_U8, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_GET_DEVCAP)               (void*, MS_U16, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_GET_EXTEND_DEVCAP)       (void*, MS_U16, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_GET_VENDER_ID)           (void*, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_GET_WRITE_BURST_DATA)   (void*, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_CALL_BACK_FUNCTION)      (void*, MS_BOOL, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_RCP_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_RAP_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_UCP_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_ATT_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_RBP_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef stMHL_INITIAL_CONFIG_INFO  (*IOCTL_MHL_GET_CONFIGURATION)        (void*);

typedef struct _MHL_INSTANT_PRIVATE
{
    MS_U32                              ulDeviceID;
    IOCTL_MHL_SUPPORT_PATH              fpMHLSupportPath;
    IOCTL_MHL_INITIAL                   fpMHLInitial;
    IOCTL_MHL_HANDLER                   fpMHLHandler;
    IOCTL_MHL_AUTO_SWITCH               fpMHLAutoSwitchHandler;
    IOCTL_MHL_SET_POWER_STATE           fpMHLSetPowerState;
    IOCTL_MHL_CBUS_CONTROL              fpMHLCbusControl;
    IOCTL_MHL_LOAD_EDID                 fpMHLLoadEDID;
    IOCTL_MHL_READ_EDID                 fpMHLReadEDID;
    IOCTL_MHL_LOAD_DEVCAP               fpMHLLoadDeviceCapability;
    IOCTL_MHL_SET_VENDER_ID             fpMHLSetVenderID;
    IOCTL_MHL_CABLE_DETECT_INVERT       fpMHLCableDetectInvert;
    IOCTL_MHL_VBUS_CONFIG               fpMHLVbusConfigSetting;
    IOCTL_MHL_ADJUST_I_CONTROL          fpMHLAdjustIControl;
    IOCTL_MHL_ADJUST_IMPEDANCE          fpMHLAdjustImpedance;
    IOCTL_MHL_CABLE_DETECT_FLAG         fpMHLCableDetectFlag;
    IOCTL_MHL_CBUS_CONNECT_FLAG         fpMHLCbusConnectFlag;
    IOCTL_MHL_CBUS_WAKEUP_FLAG          fpMHLCbusWakeupIntFlag;
    IOCTL_MHL_SOURCE_SUPPORT_RCP        fpMHLSourceSupportRCP;
    IOCTL_MHL_SOURCE_SUPPORT_RAP        fpMHLSourceSupportRAP;
    IOCTL_MHL_GET_STATUS_FLAG           fpMHLGetStatusFlag;
    IOCTL_MHL_SEND_RAP_COMMAND          fpMHLSendRAPCommand;
    IOCTL_MHL_SEND_RCP_COMMAND          fpMHLSendRCPCommand;
    IOCTL_MHL_SEND_UCP_COMMAND          fpMHLSendUCPCommand;
    IOCTL_MHL_SEND_WRITE_BURST          fpMHLSendWriteBurst;
    IOCTL_MHL_SEND_USER_WRITE_BURST     fpMHLSendUserWriteBurst;
    IOCTL_MHL_GET_DEVCAP                fpMHLGetDeviceCapability;
    IOCTL_MHL_GET_EXTEND_DEVCAP         fpMHLGetExtendDeviceCapability;
    IOCTL_MHL_GET_VENDER_ID             fpMHLGetVenderID;
    IOCTL_MHL_GET_WRITE_BURST_DATA      fpMHLGetWriteBurstData;
    IOCTL_MHL_CALL_BACK_FUNCTION        fpMHLRegisterCallBackfunction;
    IOCTL_MHL_RCP_CALL_BACK_FUNCTION    fpMHLRegisterRCPCallBackfunction;
    IOCTL_MHL_RAP_CALL_BACK_FUNCTION    fpMHLRegisterRAPCallBackfunction;
    IOCTL_MHL_UCP_CALL_BACK_FUNCTION    fpMHLRegisterUCPCallBackfunction;
    IOCTL_MHL_ATT_CALL_BACK_FUNCTION    fpMHLRegisterATTCallBackfunction;
    IOCTL_MHL_RBP_CALL_BACK_FUNCTION    fpMHLRegisterRBPCallBackfunction;
    IOCTL_MHL_GET_CONFIGURATION         fpMHLGetConfiguration;
}MHL_INSTANT_PRIVATE;

typedef enum
{
    MHL_POOL = 0,
    MHL_POOL_MAX,
}eMHLPoolID;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

#endif //_API_MHL_PRIVATE_H_
