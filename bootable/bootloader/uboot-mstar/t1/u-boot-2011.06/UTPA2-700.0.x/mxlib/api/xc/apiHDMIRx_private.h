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
/// @file   MapiHDMIRx_private.h
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _API_HDMI_RX_PRIVATE_H_
#define _API_HDMI_RX_PRIVATE_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "mhal_hdmi.h"
#include "drvXC_HDMI_if.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HDMI_HDCP_KEY_LENGTH                289U
#define HDMI_HDCP_BKSV_LENGTH               5U
#define HDMI_HDCP22_RIV_LENGTH              8U
#define HDMI_HDCP22_SESSION_KEY_LENGTH      16U
#define HDMI_HDCP22_MESSAGE_LENGTH          129U
#define HDMI_POLLING_STACK_SIZE             1U
#define HDMI_PACKET_RECEIVE_COUNT           5U   // 10ms
#define HDMI_HDCP_KSV_LIST_LENGTH           635U  // max: 127x5
#define HDMI_HDCP_VPRIME_LENGTH             20U

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _HDMI_RX_RESOURCE_PRIVATE
{
    MS_BOOL bSelfCreateTaskFlag;
    MS_BOOL bHDMITaskProcFlag;
    MS_BOOL bImmeswitchSupport;
    MS_BOOL bHDCP14KeyVaildFlag;
    MS_BOOL bHPDInvertFlag[4];
    MS_U8 ucInitialIndex;
    MS_U8 ucMHLSupportPath;
    MS_U8 ucHDCPKeyTable[HDMI_HDCP_KEY_LENGTH];
    MS_U8 ucCheckPacketState[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucReceiveInterval[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucReceiveIntervalCount[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucHDMIPollingStack[HDMI_POLLING_STACK_SIZE];
    MS_U8 ucHDCP22RecvIDListSend[4];
    MS_U32 ulPacketStatus[HDMI_INFO_SOURCE_MAX];
    MS_S32 slHDMIPollingTaskID;
    EN_POWER_MODE usPrePowerState;
    stHDMI_POLLING_INFO stHDMIPollingInfo[4];
}HDMI_RX_RESOURCE_PRIVATE;

typedef void        (*IOCTL_HDMIRX_INITIAL)                      (void*, stHDMI_INITIAL_TABLE);
typedef void        (*IOCTL_HDMIRX_STABLE_POLLING)              (void*);
typedef MS_U8       (*IOCTL_HDMIRX_GET_SOURCE_VERSION)          (void*, E_MUX_INPUTPORT);
typedef MS_U8       (*IOCTL_HDMIRX_CHECK_HDCP_STATE)            (void*, E_MUX_INPUTPORT);
typedef void        (*IOCTL_HDMIRX_CHECK_HDCP14_KEY_VAILD)      (void*, MS_U8*);
typedef MS_BOOL     (*IOCTL_HDMIRX_GET_HDCP14_KEY_VAILD_FLAG)  (void*);
typedef void        (*IOCTL_HDMIRX_SET_HPD_INVERT_FLAG)         (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef MS_U32      (*IOCTL_HDMIRX_GET_PACKET_RECEIVE)          (void*, MS_U8);
/*************************** HDCP Repeater ***************************/
typedef void        (*IOCTL_HDMIRX_WRITE_X74)                     (void*, E_MUX_INPUTPORT, MS_U8, MS_U8);
typedef MS_U8       (*IOCTL_HDMIRX_READ_X74)                      (void*, E_MUX_INPUTPORT, MS_U8);
typedef void        (*IOCTL_HDMIRX_SET_REPEATER)                  (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef void        (*IOCTL_HDMIRX_SET_BSTATUS)                   (void*, E_MUX_INPUTPORT, MS_U16);
typedef void        (*IOCTL_HDMIRX_SET_HDMI_MODE)                 (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef MS_U8       (*IOCTL_HDMIRX_GET_INTERRUPT_STATUS)        (void*, E_MUX_INPUTPORT);
typedef void        (*IOCTL_HDMIRX_WRITE_KSV_LIST)                (void*, E_MUX_INPUTPORT, MS_U8*, MS_U32);
typedef void        (*IOCTL_HDMIRX_SET_VPRIME)                     (void*, E_MUX_INPUTPORT, MS_U8*);
/*************************** HDCP Repeater ***************************/
/*************************** HDCP22 ***************************/
typedef void        (*IOCTL_HDMIRX_FILL_CIPHER_KEY)              (void*, MS_U8, MS_U8*, MS_U8*);
typedef void        (*IOCTL_HDMIRX_PORT_INIT)                     (void*, MS_U8);
typedef MS_BOOL     (*IOCTL_HDMIRX_POLLING_READ_DONE)            (void*, MS_U8);
typedef void        (*IOCTL_HDMIRX_ENABLE_CIPHER)                 (void*, MS_U8, MS_U8, MS_BOOL);
typedef void        (*IOCTL_HDMIRX_SEND_MESSAGE)                  (void*, MS_U8, MS_U8, MS_U8*, MS_U32, void*);
typedef MS_BOOL     (*IOCTL_HDMIRX_HDCP22_HANDER)                 (void*, MS_U8, MS_U8*, MS_U8*, MS_U32*);
/*************************** HDCP22 ***************************/

typedef struct _HDMI_RX_INSTANT_PRIVATE
{
    MS_U32 ulDeviceID;
    IOCTL_HDMIRX_INITIAL                    fpHDMIRxInitial;
    IOCTL_HDMIRX_STABLE_POLLING             fpHDMIRxStablePolling;
    IOCTL_HDMIRX_GET_SOURCE_VERSION         fpHDMIRxGetSourceVersion;
    IOCTL_HDMIRX_CHECK_HDCP_STATE           fpHDMIRxCheckHDCPState;
    IOCTL_HDMIRX_CHECK_HDCP14_KEY_VAILD     fpHDMIRxCheckHDCP14KeyVaild;
    IOCTL_HDMIRX_GET_HDCP14_KEY_VAILD_FLAG  fpHDMIRxGetHDCP14KeyVaildFlag;
    IOCTL_HDMIRX_SET_HPD_INVERT_FLAG        fpHDMIRxSetHPDInvertFlag;
    IOCTL_HDMIRX_GET_PACKET_RECEIVE         fpHDMIRxGetPacketReceive;
    /*************************** HDCP Repeater ***************************/
    IOCTL_HDMIRX_WRITE_X74                  fpHDMIRxWriteX74;
    IOCTL_HDMIRX_READ_X74                   fpHDMIRxReadX74;
    IOCTL_HDMIRX_SET_REPEATER               fpHDMIRxSetRepeater;
    IOCTL_HDMIRX_SET_BSTATUS                fpHDMIRxSetBstatus;
    IOCTL_HDMIRX_SET_HDMI_MODE              fpHDMIRxSetHDMIMode;
    IOCTL_HDMIRX_GET_INTERRUPT_STATUS       fpHDMIRxGetInterruptStatus;
    IOCTL_HDMIRX_WRITE_KSV_LIST             fpHDMIRxWriteKSVList;
    IOCTL_HDMIRX_SET_VPRIME                 fpHDMIRxSetVPrime;
    /*************************** HDCP Repeater ***************************/
    IOCTL_HDMIRX_FILL_CIPHER_KEY            fpHDMIRxFillCipherKey;
    IOCTL_HDMIRX_PORT_INIT                  fpHDMIRxPortInit;
    IOCTL_HDMIRX_POLLING_READ_DONE          fpHDMIRxPollingReadDone;
    IOCTL_HDMIRX_ENABLE_CIPHER              fpHDMIRxEnableCipher;
    IOCTL_HDMIRX_SEND_MESSAGE               fpHDMIRxSendMessage;
    IOCTL_HDMIRX_HDCP22_HANDER              fpHDMIRxHDCP22Handler;
}HDMI_RX_INSTANT_PRIVATE;

typedef enum
{
    HDMI_RX_POOL = 0,
    HDMI_RX_POOL_MAX,
}eHDMIRxPoolID;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
MS_U32 mdrv_HDMI_STREventProc(void* pModule, EN_POWER_MODE usPowerState);
void mdrv_HDMI_InitialSetting(void* pInstance, stHDMI_INITIAL_TABLE stInitialTable);
void mdrv_HDMI_StablePolling(void* pInstance);
MS_U8 mdrv_HDMI_GetSourceVersion(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_U8 mdrv_HDMI_CheckHDCPState(void* pInstance, E_MUX_INPUTPORT enInputPortType);
void mdrv_HDMI_CheckHDCP14KeyVaild(void* pInstance, MS_U8 *ucHDCPKeyData);
MS_BOOL mdrv_HDMI_GetHDCP14KeyVaildFlag(void* pInstance);
void mdrv_HDMI_SetHPDInvertFlag(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bHPDInvertFlag);
MS_U32 mdrv_HDMI_GetPacketReceiveStatus(void* pInstance, MS_U8 ucHDMIInfoSource);
/*************************** HDCP Repeater ***************************/
void mdrv_HDCP_WriteX74(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset, MS_U8 ucData);
MS_U8 mdrv_HDCP_ReadX74(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset);
void mdrv_HDCP_SetRepeater(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bIsRepeater);
void mdrv_HDCP_SetBstatus(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U16 usBstatus);
void mdrv_HDCP_SetHDMIMode(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMIMode);
MS_U8 mdrv_HDCP_GetInterruptStatus(void* pInstance, E_MUX_INPUTPORT enInputPortType);
void mdrv_HDCP_WriteKSVList(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8* pucKSV, MS_U32 ulDataLen);
void mdrv_HDCP_SetVPrime(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8* pucVPrime);
/*************************** HDCP Repeater ***************************/
MS_BOOL MDrv_HDMI_IOCTL(MS_U32 ulCommand, void *pBuffer, MS_U32 ulBufferSize);
void mdrv_HDCP22_FillCipherKey(void* pInstance, MS_U8 ucPortIdx, MS_U8* pucRiv, MS_U8* pucSessionKey);
void mdrv_HDCP22_PortInit(void* pInstance, MS_U8 ucPortIdx);
MS_BOOL mdrv_HDCP22_PollingReadDone(void* pInstance, MS_U8 ucPortIdx);
void mdrv_HDCP22_EnableCipher(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable);
void mdrv_HDCP22_SendMessage(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy);
MS_BOOL mdrv_HDCP22_Handler(void* pInstance, MS_U8 ucPortIdx, MS_U8* ucPortType, MS_U8* pucMsgData, MS_U32* wMsgLen);

#endif //_API_HDMI_RX_PRIVATE_H_
