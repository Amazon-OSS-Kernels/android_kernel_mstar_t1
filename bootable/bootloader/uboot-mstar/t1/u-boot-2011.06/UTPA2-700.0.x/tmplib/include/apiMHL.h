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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mapi_mhl.h
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
*/

/*! \defgroup MISC MISC modules
 * \defgroup MHL MHL interface (apiMHL.h)
 *  \ingroup MISC
    <b>Operation Flow</b>
    -# Check whether MHL cable is plugged or not in Rx side
    -# Disable R-term and output Vbus
    -# Check whether Cbus is connected(discovery pulse completed)
    -# Sink -> Source: "SET_HPD"
    -# Sink -> Source: Write_Stat - "DCAP_RDY" and Write_INT - "DCAP_CHG"
    -# Sink -> Source: Write_Stat - "PATH_EN"


 * \defgroup MHL_BASIC MHL Module basic
 *  \ingroup MHL
 * \defgroup MHL_CAPABILITY MHL capability setting
 *  \ingroup MHL
 * \defgroup MHL_CONFIG MHL pad config
 *  \ingroup MHL
 * \defgroup MHL_STATUS MHL status flag
 *  \ingroup MHL
 * \defgroup MHL_SEND MHL Tx send function
 *  \ingroup MHL
 * \defgroup MHL_RECEIVE MHL Rx recevice data
 *  \ingroup MHL
 * \defgroup MHL_CALLBACK MHL call back function
 *  \ingroup MHL
 * \defgroup MHL_REMOVE MHL want to remove function
 *  \ingroup MHL
*/

#ifndef _API_MHL_H_
#define _API_MHL_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

//////////////////////////////////////////////////
//                MHL SW Version                //
//////////////////////////////////////////////////
#define P4_CHANGELIST       "00522289"
#define FW_VER_MAJOR        "04"
#define FW_VER_MINOR        "18"

typedef MS_U8 (*link_cbus_event_handle_func)(MS_U8);

typedef enum
{
    E_MHL_POWER_ON,
    E_MHL_POWER_STANDBY,
    E_MHL_POWER_DOWN
} E_MHL_POWERCTRL_Mode;

enum
{
    RCP_KEYID_SELECT = 0x00,
    RCP_KEYID_UP = 0x01,
    RCP_KEYID_DOWN = 0x02,
    RCP_KEYID_LEFT = 0x03,
    RCP_KEYID_RIGHT = 0x04,
    RCP_KEYID_RIGHT_UP = 0x05,
    RCP_KEYID_RIGHT_DOWN = 0x06,
    RCP_KEYID_LEFT_UP = 0x07,
    RCP_KEYID_LEFT_DOWN = 0x08,
    RCP_KEYID_ROOT_MENU = 0x09,
    RCP_KEYID_SETUP_MENU = 0x0A,
    RCP_KEYID_CONTENTS_MENU = 0x0B,
    RCP_KEYID_FAVORITE_MENU = 0x0C,
    RCP_KEYID_EXIT = 0x0D,
    //0x0E - 0x1F are reserved
    RCP_KEYID_NUM_0 = 0x20,
    RCP_KEYID_NUM_1 = 0x21,
    RCP_KEYID_NUM_2 = 0x22,
    RCP_KEYID_NUM_3 = 0x23,
    RCP_KEYID_NUM_4 = 0x24,
    RCP_KEYID_NUM_5 = 0x25,
    RCP_KEYID_NUM_6 = 0x26,
    RCP_KEYID_NUM_7 = 0x27,
    RCP_KEYID_NUM_8 = 0x28,
    RCP_KEYID_NUM_9 = 0x29,
    RCP_KEYID_DOT = 0x2A,
    RCP_KEYID_ENTER = 0x2B,
    RCP_KEYID_CLEAR = 0x2C,
    //0x2D - 0x2F are reserved
    RCP_KEYID_CH_UP = 0x30,
    RCP_KEYID_CH_DOWN = 0x31,
    RCP_KEYID_PRE_CH = 0x32,
    RCP_KEYID_SOUND_SELECT = 0x33,
    RCP_KEYID_INPUT_SELECT = 0x34,
    RCP_KEYID_SHOW_INFO = 0x35,
    RCP_KEYID_HELP = 0x36,
    RCP_KEYID_PAGE_UP = 0x37,
    RCP_KEYID_PAGE_DOWN = 0x38,
    //0x39 - 0x40 are reserved
    RCP_KEYID_VOL_UP = 0x41,
    RCP_KEYID_VOL_DOWN = 0x42,
    RCP_KEYID_MUTE = 0x43,
    RCP_KEYID_PLAY = 0x44,
    RCP_KEYID_STOP = 0x45,
    RCP_KEYID_PAUSE = 0x46,
    RCP_KEYID_RECORD = 0x47,
    RCP_KEYID_REWIND = 0x48,
    RCP_KEYID_FAST_FWD = 0x49,
    RCP_KEYID_EJECT  = 0x4A,
    RCP_KEYID_FORWARD = 0x4B,
    RCP_KEYID_BACKWARD = 0x4C,
    //0x4D - 0x4F are reserved
    RCP_KEYID_ANGLE = 0x50,
    RCP_KEYID_SUBPICTURE = 0x51,
    //0x52 - 0x5F are reserved
    RCP_KEYID_PLAY_FUNC = 0x60,
    RCP_KEYID_PAUSE_PLAY_FUNC = 0x61,
    RCP_KEYID_RECORD_FUNC = 0x62,
    RCP_KEYID_PAUSE_REC_FUNC = 0x63,
    RCP_KEYID_STOP_FUNC = 0x64,
    RCP_KEYID_MUTE_FUNC = 0x65,
    RCP_KEYID_UN_MUTE_FUNC = 0x66,
    RCP_KEYID_TUNE_FUNC = 0x67,
    RCP_KEYID_MEDIA_FUNC = 0x68,
    //0x69 - 0x70 are reserved
    RCP_KEYID_F1 = 0x71,
    RCP_KEYID_F2 = 0x72,
    RCP_KEYID_F3 = 0x73,
    RCP_KEYID_F4 = 0x74,
    RCP_KEYID_F5 = 0x75,
    //0x76 - 0x7D are reserved
    RCP_KEYID_VENDOR_SPECIFIC = 0x7E,
    RCP_KEYID_RESERVED = 0x7F,
    RCP_KEYID_SELECT_RELEASE = 0x80,
}; // MscRcpKcode_T;

typedef enum
{
    MSC_RAP_POLL = 0x00,
    MSC_RAP_CONTENT_ON = 0x10,
    MSC_RAP_CONTENT_OFF = 0x11,
} MscRapActionCode_T;

enum
{
    MSC_RCP_NO_ERROR = 0x00,     // RCP No Error
    MSC_RCP_INEFFECTIVE_KEY_CODE = 0x01,     // The key code in the RCP sub-command is not recognized
    MSC_RCP_RESPONDER_BUSY = 0x02,     // RCP Response busy
}; //MscRcpStatusCode_T;

enum
{
    MSC_RAP_NO_ERROR = 0x00,     // RAP No Error
    MSC_RAP_UNRECOGNIZED_ACTION_CODE = 0x01,
    MSC_RAP_UNSUPPORTED_ACTION_CODE = 0x02,
    MSC_RAP_RESPONDER_BUSY = 0x03,
}; //MscRapStatusCode_T;

enum MHL_CBUS_CONTROL_TYPE
{
    MHL_FORCE_HDMI_BY_PASS = 0,
    MHL_FORCE_24BIT_MODE,
    MHL_FORCE_PACKET_PIXEL_MODE,
    MHL_CBUS_ISOLATION_ON,
    MHL_CBUS_ISOLATION_OFF,
    MHL_CBUS_FLOATING_ON,
    MHL_CBUS_FLOATING_OFF,
    MHL_CBUS_RTERM_OFF,
    MHL_CBUS_FORCE_CLEAR_HPD,
    MHL_CBUS_FORCE_RESET_HPD,
    MHL_VBUS_HW_DETCET,
    MHL_VBUS_CHARGE_ON,
    MHL_VBUS_CHARGE_OFF,
    MHL_CBUS_FORCE_READY_DEVCAP,
    MHL_CBUS_FORCE_RECEIVE_3D_REQ,
    MHL_CBUS_FORCE_SEND_COMMAND_ENABLE,
    MHL_CBUS_FORCE_SEND_COMMAND_DISABLE,
    MHL_CBUS_CHECK_CONTEND_ON_ENABLE,
    MHL_CBUS_CHECK_CONTEND_ON_DISABLE,
};

enum MHL_CBUS_STATUS_FLAG_TYPE
{
    MHL_CBUS_STATUS_FLAG_CABLE_DETECT = 0,
    MHL_CBUS_STATUS_FLAG_CBUS_CONNECT,
    MHL_CBUS_STATUS_FLAG_PATH_EN,
    MHL_CBUS_STATUS_FLAG_RECEIVE_PATH_EN,
    MHL_CBUS_STATUS_FLAG_RECEIVE_MUTED,
};

enum MHL_CABLE_DETECT_USE_TYPE
{
    MHL_CABLE_DETECT_USE_PORTA = 0x01,
    MHL_CABLE_DETECT_USE_PORTB = 0x02,
    MHL_CABLE_DETECT_USE_PORTC = 0x04,
    MHL_CABLE_DETECT_USE_PORTD = 0x08,
};

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Assign which HDMI port support MHL
/// @ingroup MHL_BASIC
/// @param  ucSelect          \b IN: MHL support port
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_MHLSupportPath(MS_U8 ucSelect);

//-------------------------------------------------------------------------------------------------
/// Initialize MHL
/// @ingroup MHL_BASIC
/// @param  edid                \b IN: point to MHL EDID
/// @param  DevCap          \b IN: point to MHL capability
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_init(MS_U8 *edid, MS_U8 *DevCap);
void mapi_mhl_LoadEDID(MS_U8 *edid);

//-------------------------------------------------------------------------------------------------
/// MHL read EDID
/// @ingroup MHL_CAPABILITY
/// @param  usSize             \b IN: Size of EDID want to read back
/// @param  edid                \b IN: point to MHL EDID
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_ReadEDID(MS_U16 usSize, MS_U8 *edid);

//-------------------------------------------------------------------------------------------------
/// MHL load device capability
/// @ingroup MHL_CAPABILITY
/// @param  DevCap          \b IN: point to MHL capability
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_LoadDeviceCapability(MS_U8 *devcap);

//-------------------------------------------------------------------------------------------------
/// MHL load vender ID
/// @ingroup MHL_CAPABILITY
/// @param  ucVenderID     \b IN: point to MHL capability
/// @return None
//-------------------------------------------------------------------------------------------------
// void mapi_mhl_SetVenderID(MS_U8 ucVenderID);

//-------------------------------------------------------------------------------------------------
/// MHL set cable detect invert
/// @ingroup MHL_CONFIG
/// @param  bCableDetectInvert     \b IN: cable detect invert flag
/// @return None
//-------------------------------------------------------------------------------------------------
// void mapi_mhl_InvertCableDetect(MS_BOOL bCableDetectInvert);

//-------------------------------------------------------------------------------------------------
/// MHL set Vbus config mode
/// @ingroup MHL_CONFIG
/// @param  ucState         \b IN: Vbus config mode
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_VbusConfigSetting(MS_U8 ucState);
// void mapi_mhl_CableDetectPadSetting(MS_U8 ucSelect);
void mapi_mhl_handler(void);
void mapi_mhl_PowerCtrl(E_MHL_POWERCTRL_Mode pctrl);
// MHL flag check function
MS_BOOL mapi_mhl_CableDetect(void);

//-------------------------------------------------------------------------------------------------
/// MHL get Cbus connection flag
/// @ingroup MHL_STATUS
/// @return TRUE : MHL Cbus connection
/// @return FALSE : MHL Cbus disconnection
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_CbusStatus(void);

//-------------------------------------------------------------------------------------------------
/// MHL get wake up pulse interrupt  flag
/// @ingroup MHL_STATUS
/// @return TRUE : MHL receive wake up pulse
/// @return FALSE : MHL not receive wake up pulse
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_CbusWakeupIntFlag(void);
// MS_BOOL mapi_mhl_ReadDevcapFlag(void);
// MS_BOOL mapi_mhl_SrcRCPSupportFlag(void);

//-------------------------------------------------------------------------------------------------
/// MHL source support RAP flag
/// @ingroup MHL_STATUS
/// @return TRUE : MHL source support RAP
/// @return FALSE : MHL source nots support RAP or unknow
//-------------------------------------------------------------------------------------------------
// MS_BOOL mapi_mhl_SrcRAPSupportFlag(void);
// MS_BOOL mapi_mhl_SrcMSGResponseFlag(void);
// MHL Tx function
// MS_BOOL mapi_mhl_SendMSGCmdFlag(void);
MS_BOOL mapi_mhl_SendRAPCmd(MS_U8 databuf);
MS_BOOL mapi_mhl_SendRCPCmd(MS_U8 databuf);
MS_BOOL mapi_mhl_SendRCP_PressRelease_Cmd(MS_U8 u8RCPKey, MS_BOOL bIsRelease);
MS_BOOL mapi_mhl_SendRCPAutoReleaseCmd(MS_U8 ucKeyCode);

//-------------------------------------------------------------------------------------------------
/// MHL send UCP command
/// @ingroup MHL_SEND
/// @param  databuf         \b IN: key code
/// @return TRUE : MHL send UCP success
/// @return FALSE : MHL send UCP fail or not ready
//-------------------------------------------------------------------------------------------------
// MS_BOOL mapi_mhl_SendUCPCmd(MS_U8 databuf);

//-------------------------------------------------------------------------------------------------
/// MHL send write burst command directly
/// @ingroup MHL_SEND
/// @param  ucAddr          \b IN: Write burst address always 0x40
/// @param  ucLength       \b IN: Write burst data length
/// @param  buffer           \b IN: Write burst data content
/// @return TRUE : MHL send write burst success
/// @return FALSE : MHL send write burst fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_SendWriteBurst(MS_U8 ucAddr, MS_U8 ucLength, MS_U8 *buffer);

//-------------------------------------------------------------------------------------------------
/// MHL send write burst command use write request / write grant
/// @ingroup MHL_SEND
/// @param  ucLength       \b IN: Write burst data length
/// @param  pBuffer          \b IN: Write burst data content
/// @return TRUE : MHL send write burst success
/// @return FALSE : MHL send write burst fail or not ready
//-------------------------------------------------------------------------------------------------
// MS_BOOL SYMBOL_WEAK mapi_mhl_CbusSendUserWriteBurst(MS_U8 ucLength, MS_U8 *pBuffer);
// MS_BOOL mapi_mhl_ReadDeviceCapacibility(MS_U8 ucAddr, MS_U8 *ucData);
MS_BOOL mapi_mhl_GetDeviceCapacibility(MS_U16 usDevcapMask, MS_U8 *ucDevcap);

//-------------------------------------------------------------------------------------------------
/// MHL send write read extend device capability
/// @ingroup MHL_SEND
/// @param  usExtendDevcapMask     \b IN: Which data want to read
/// @param  ucExtendDevcap            \b IN: Read extend device capability buffer
/// @return TRUE : MHL read extend device capability success
/// @return FALSE : MHL read extend device capability fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL SYMBOL_WEAK mapi_mhl_GetExtendDeviceCapacibility(MS_U16 usExtendDevcapMask, MS_U8 *ucExtendDevcap);

//-------------------------------------------------------------------------------------------------
/// MHL send get vender ID
/// @ingroup MHL_SEND
/// @param  ucVenderID     \b IN: Vender buffer
/// @return TRUE : MHL get vender ID success
/// @return FALSE : MHL get vender ID fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_GetDeviceVenderID(MS_U8 *ucVenderID);

//-------------------------------------------------------------------------------------------------
/// MHL receive write burst data
/// @ingroup MHL_RECEIVE
/// @param  ucData      \b IN: Write burst data buffer
/// @return TRUE : MHL receive write burst data success
/// @return FALSE : MHL receive write burst data fail or not yeah
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_GetWriteBurstData(MS_U8 *ucData);
void mapi_mhl_Send3DInformation(void);
// void mapi_mhl_CbusSendCommand(void);
// MHL others function
// void mapi_mhl_CbusIsolate(void);
void mapi_mhl_CbusWakeupIntSetting(void);
MS_BOOL mapi_mhl_AutoSwitchHandler(MS_BOOL bReset, MS_U8 *ucCbusPath);
// void mapi_mhl_AdjustSettingIControl(MS_U8 ucIControl);
// void mapi_mhl_AdjustImpedanceSetting(MS_U8 ucImpedance);
// MS_BOOL mapi_mhl_WakeupDetect(void);
MS_U32 mapi_mhl_SetPowerState(EN_POWER_MODE u16PowerState);
void mapi_mhl_SetHPD(MS_BOOL bflag);
// void mapi_mhl_SetPsCtrl(MS_U8 u8Ctrl);
void mapi_mhl_CbusControl(MS_U8 ucState);
MS_BOOL mapi_mhl_CbusGetStatusFlag(MS_U8 ucState);
// void mapi_mhl_GetSWVersion(void);
// char* mapi_mhl_GetP4CL(void);
// char* mapi_mhl_GetMajorVersion(void);
// char* mapi_mhl_GetMinorVersion(void);
void mapi_mhl_RegisterCallBackFunctions(link_cbus_event_handle_func rcp, link_cbus_event_handle_func rap);
//void mapi_mhl_RegisterRcpCallBackFunction(link_cbus_event_handle_func rcp);
//void mapi_mhl_RegisterRapCallBackFunction(link_cbus_event_handle_func rap);
void mapi_mhl_RegisterUcpCallBackFunction(link_cbus_event_handle_func Ucp);
//void SYMBOL_WEAK mapi_mhl_RegisterAttCallBackFunction(link_cbus_event_handle_func Att);
//void SYMBOL_WEAK mapi_mhl_RegisterRbpCallBackFunction(link_cbus_event_handle_func Rbp);

#ifdef __cplusplus
}
#endif

#endif //_API_MHL_H_
