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

#ifndef _DRV_CEC_H_
#define _DRV_CEC_H_




//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MDRV_IRQ_BEGIN      0x40
#define MDRV_IRQ_END        0x7F
#define IRQ_ADCDVI2RIU     (MDRV_IRQ_BEGIN + 31)//31 (level)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef _DRV_CEC_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

MS_U32 mdrv_CEC_STREventProc(void* pModule, EN_POWER_MODE usPowerState);

INTERFACE MS_BOOL MDrv_CEC_DDC2BI_GetInfo(void* pInstance, MS_CEC_INFO_LIST *pInfo);
// INTERFACE void MDrv_CEC_PortSelect(void* pInstance, MsCEC_INPUT_PORT InputPort);
// INTERFACE void MDrv_CEC_Init(void* pInstance, MS_U32 u32XTAL_CLK_Hz);
// INTERFACE void MDrv_CEC_Exit(void* pInstance);
// INTERFACE void MDrv_CEC_CheckExistDevices(void* pInstance);
INTERFACE MsCEC_DEVICELA MDrv_CEC_NextDevice(void* pInstance);
INTERFACE void MDrv_CEC_ChkRxBuf(void* pInstance);
// command call _MDrv_CEC_TxApi
INTERFACE CEC_ERROR_CODE MDrv_CEC_TxSendMsg(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8 *cmd, MS_U8 len);
INTERFACE CEC_ERROR_CODE MDrv_CEC_TxSendMsg2(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8* cmd, MS_U8 len);
INTERFACE CEC_ERROR_CODE MDrv_CecTxSendPollingMsg(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ActiveSource(void* pInstance);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_RoutingChange(void* pInstance, MS_U8* orig_address, MS_U8* new_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqActiveSource(void* pInstance);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_SetStreamPath(void* pInstance, MS_U8* new_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_Standby(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_RecordOff(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_RecordOn(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_RECORD_SOURCE_TYPE cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReportCECVersion(void* pInstance, MsCEC_DEVICELA dst_address, MS_U8 version);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqCECVersion(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReportPhycalAddress(void* pInstance);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqPhycalAddress(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_DeckControl(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_DECK_CTRL_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_DecStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_DECK_STATUS_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_MSg_GiveDeckStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_STATUS_REQUEST_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_MSg_DCPlay(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_PLAY_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqMenuStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_MENU_REQ_TYPE cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_UserCtrlPressed(void* pInstance, MS_BOOL bUserCtrlEn, MsCEC_DEVICELA dst_address, MsCEC_MSG_USER_CTRL_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_UserCtrlReleased(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_GiveAudioStatus(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReportPowerStatus(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqPowerStatus(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_FeatureAbort(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MsCEC_MSG_ABORT_REASON cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_Abort(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_SendMenuLanguage(void* pInstance, MS_U8* pu8MenuLanguageCode);
INTERFACE CEC_ERROR_CODE MDrv_CecMsg_ReqARCInitiation(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CecMsg_ReqARCTermination(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CecMsg_AudioModeReq(void* pInstance, MsCEC_DEVICELA dst_address, MS_BOOL amp_switch, MS_U8* my_address);
// command end
// INTERFACE MS_BOOL MDrv_CEC_CheckFrame(void* pInstance, MsCEC_MSG_TRANS_TYPE msg_type, MS_U8 ucLen);
// INTERFACE void MDrv_CEC_ConfigWakeUp(void* pInstance);
// INTERFACE void MDrv_CEC_Enabled(void* pInstance, MS_BOOL bEnableFlag);
// INTERFACE void MDrv_CEC_SetMyLogicalAddress(void* pInstance, MsCEC_DEVICELA MyLogicalAddress);
// INTERFACE void MDrv_CEC_InitChip(void* pInstance, MS_U32 u32XTAL_CLK_Hz);
// INTERFACE MS_U8 MDrv_CEC_TxStatus(void* pInstance);
INTERFACE MS_BOOL MDrv_CEC_CheckDeviceIsTx(void* pInstance);
// INTERFACE MS_U32 MDrv_CEC_SetPowerState(void* pInstance, EN_POWER_MODE u16PowerState);

#if ENABLE_CEC_MULTIPLE
INTERFACE void MDrv_CEC_SetMyLogicalAddress2(void* pInstance, MsCEC_DEVICELA MyLogicalAddress);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReportPhycalAddress2(void* pInstance);
INTERFACE void MDrv_CEC_SetMyPhysicalAddress2(void* pInstance, MS_U8 *pdata);
INTERFACE void MDrv_CEC_SetInitiator(void* pInstance, MsCEC_DEVICELA IniLA);
#endif



// INTERFACE void MDrv_CEC_SetRetryCount(void* pInstance, MS_U8 u8RetryCount);
// INTERFACE void MDrv_CEC_ConfigWakeupInfoVendorID(void* pInstance, MS_U8 *u8CecVendorID);

MS_U8 MDrv_CEC_Get_Header(void* pInstance);
MS_U8 MDrv_CEC_Get_OpCode(void* pInstance);
MS_U8 MDrv_CEC_Get_Para(void* pInstance, MS_U8 u8Idx);
MS_U8 MDrv_CEC_GetCmdLen(void* pInstance);
MS_BOOL MDrv_CEC_IsRxBufEmpty(void* pInstance);
void MDrv_CEC_SetActiveLogicalAddress(void* pInstance, MsCEC_DEVICELA Addr);
MsCEC_DEVICELA MDrv_CEC_GetActiveLogicalAddress(void* pInstance);
MsCEC_MSG_POWER_STATUS_PARM MDrv_CEC_GetPowerStatus(void* pInstance);
MS_U8 MDrv_CEC_GetFifoIdx(void* pInstance);
void MDrv_CEC_SetFifoIdx(void* pInstance, MS_U8 u8Idx);
void MDrv_CEC_SetActivePowerStatus(void* pInstance, MsCEC_MSG_POWER_STATUS_PARM Status);
MsCEC_MSG_POWER_STATUS_PARM MDrv_CEC_GetActivePowerStatus(void* pInstance);
void MDrv_CEC_SetActivePhysicalAddress(void* pInstance, MS_U8 u8Para1, MS_U8 u8Para2);
void MDrv_CEC_SetActiveDeviceCECVersion(void* pInstance, MS_U8 u8Ver);
void MDrv_CEC_SetActiveDeviceType(void* pInstance, MsCEC_DEVICE_TYPE Type);
MS_U8 MDrv_CEC_GetMsgCnt(void* pInstance);
void MDrv_CEC_SetMsgCnt(void* pInstance, MS_U8 u8Cnt);
MS_U8 MDrv_CEC_GetRxData(void* pInstance, MS_U8 u8Fifoidx, MS_U8 u8Idx);
void MDrv_CEC_SetMyPhysicalAddress(void* pInstance, MS_U8 *pdata);


////////////////////////////////////////////////////////////////////////////////
#undef INTERFACE

////////////////////////////////////////////////////////////////////////////////
#endif //_DRV_CEC_H_

