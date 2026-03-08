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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HDCP_H_
#define _DRV_HDCP_H_

#include "MsCommon.h"
/*********************************************************************/
/*                                                                                                                     */
/*                                                      Defines                                                    */
/*                                                                                                                     */
/*********************************************************************/

/*********************************************************************/
/*                                                                                                                     */
/*                                                      Structure                                                  */
/*                                                                                                                     */
/*********************************************************************/

/*********************************************************************/
/*                                                                                                                     */
/*                                                 Proto-type                                                    */
/*                                                                                                                     */
/*********************************************************************/
//HDCP14
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxInitHdcp()
/// initiallze hardware setting of hdcp1.x Tx module
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxInitHdcp(MS_U8 u8PortIdx);

//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxLoadKey()
/// loading Hdcp1.x key
/// @param pu8KeyData	\b IN: hdcp key pointer
/// @param bUseKmNewMode \b IN: indicate decrypt mode is new or old mode
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxLoadKey(MS_U8* pu8KeyData, MS_BOOL bUseKmNewMode);

//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxSetAuthPass()
/// setting register when authentication flow is done
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxSetAuthPass(MS_U8 u8PortIdx);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxEnableENC_EN()
/// enable or disable ENC_EN signal
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param bEnable	\b IN: indicate enable or disable ENC_EN signal
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxEnableENC_EN(MS_U8 u8PortIdx, MS_BOOL bEnable);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxProcessAn()
/// filling or fetching AN value depends on using external or internal AN
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param bUseInternalAn	\b IN: indicate internal or external AN
/// @param pu8An \b IN: if using external AN, treat this as input AN; \b OUT: if using internal AN, treat this as return AN
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxProcessAn(MS_U8 u8PortIdx, MS_BOOL bUseInternalAn, MS_U8* pu8An);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxGetAKSV()
/// fetch AKSV from hdcp1.x key
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8Aksv	\b OUT: MS_U8 pointer to store AKSV for return
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxGetAKSV(MS_U8 u8PortIdx, MS_U8* pu8Aksv);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxCompareRi()
/// compare Ri value of sink and source side
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8SinkRi	\b IN: MS_U8 pointer to store Ri value of sink device
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxCompareRi(MS_U8 u8PortIdx, MS_U8* pu8SinkRi);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxConfigMode()
/// config hdcp1.x encrypt mode
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param u8Mode	\b IN: encrypt mode of hdcp1.x
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxConfigMode(MS_U8 u8PortIdx, MS_U8 u8Mode);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxGenerateCipher()
/// decrypt hdcp1.x key and init cipher engine
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8Bksv	\b IN: MS_U8 pointer to store BKSV value
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxGenerateCipher(MS_U8 u8PortIdx, MS_U8* pu8Bksv);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxProcessR0()
/// trigger HW module to do the R0 calculation and return if process is done
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxProcessR0(MS_U8 u8PortIdx);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxGetM0()
/// fetch M0 value from HW module
/// @param u8PortIdx    \b IN: HDMI Port Index
/// @param pu8M0        \b IN: pointer to store M0 value
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxGetM0(MS_U8 u8PortIdx, MS_U8* pu8M0);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14GetM0()
/// fetch HDCP 1.4  M0 value from Sink device
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8Data	\b IN: MS_U8 pointer to store returned M0 value
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14GetM0(MS_U8 u8PortIdx, MS_U8 *pu8Data);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14FillKey()
/// Load HDCP 1.4  key
/// @param pu8KeyData	\b IN: MS_U8 pointer to store HDCP 1.4 key value
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14FillKey(MS_U8 *pu8KeyData);

//HDCP22
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2Init()
/// Initial Process of HDCP2.2
/// @param \b IN: none
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2Init(void);
//HDCP22 TX
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2Init()
/// Initial of HDCP2.2 tx hw module
/// @param u8PortIdx \b IN: HDMI Port Index
/// @param bEnable \b IN: init / de-init flag
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxModuleInit(MS_U8 u8PortIdx, MS_BOOL bEnable);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2TxEnableEncrypt()
/// Enable HDCP2.2 Encryption
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param bEnable: 	\b IN: TRUE: enable encryption; FALSE: disable encryption
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxEnableEncrypt(MS_U8 u8PortIdx, MS_BOOL bEnable);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2TxFillCipherKey()
/// Fill HDCP2.2 TX Cipher Key
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @pu8Riv		\b IN: MS_U8 pointer which stored RIV data
/// @pu8ContentKey	\b IN: MS_U8 pointer which stored KS^LV128
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxFillCipherKey(MS_U8 u8PortIdx, MS_U8 *pu8Riv, MS_U8 *pu8ContentKey);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2RxGetCipherState()
/// Get current HDCP2.2 TX status
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8State		\b IN: MS_U8 pointer which return HDCP22 TX cipher state
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State);

//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2TxSetAuthPass()
/// Set or clear HDCP2.2 TX authentication pass
/// @u8PortIdx		\b IN: HDMI Port Index
/// @bEnable		       \b IN: disable or enable authentication pass bit
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxSetAuthPass(MS_U8 u8PortIdx, MS_BOOL bEnable);

//HDCP22 Rx
//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxInit()
/// hdcp22 rx init setting;
/// @u8PortIdx		\b IN: HDMI Port Index
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxInit(MS_U8 u8PortIdx);

//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxProcessCipher()
/// Fill HDCP2.2 RX Cipher Key and set SKE pass;
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8Riv		\b IN: MS_U8 pointer which stored RIV data
/// @pu8ContentKey	\b IN: MS_U8 pointer which stored KS^LV128
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxProcessCipher(MS_U8 u8PortIdx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey);

//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxEnableENCEN()
/// set or clear SKE pass bit;
/// @u8PortIdx		\b IN: HDMI Port Index
/// @bEnable		\b IN: flag to indicate set or clear
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxSetSKEPass(MS_U8 u8PortIdx, MS_BOOL bEnable);

//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxFillCipherKey()
/// Fill HDCP2.2 RX Cipher Key only;
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8Riv		\b IN: MS_U8 pointer which stored RIV data
/// @pu8ContentKey	\b IN: MS_U8 pointer which stored KS^LV128
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxFillCipherKey(MS_U8 u8PortIdx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey);

//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2RxGetCipherState()
/// Get current HDCP2.2 RX status
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8State		\b IN: MS_U8 pointer which return HDCP22 RX cipher state
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State);

//----------------------------------------------------------------
/// MDrv_HDCP_GetHDCP2Status()
/// Get HDCP22 encrytion status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @pu32HDCPStatus	    \b IN: MS_U32 pointer which return encrytion status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_GetHDCP2Status(MS_U32 u32HdmiPort, MS_U32 *pu32HDCPStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_SetHDCP2Status()
/// Set HDCP22 encrytion status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @u32HDCPStatus	    \b IN: Encrytion status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_SetHDCP2Status(MS_U32 u32HdmiPort, MS_U32 u32HDCPStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_GetHDCP1Status()
/// Get HDCP14 encrytion status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @pu32HDCPStatus	    \b IN: MS_U32 pointer which return encrytion status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_GetHDCP1Status(MS_U32 u32HdmiPort, MS_U32 *pu32HDCPStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_SetHDCP1Status()
/// Set HDCP14 encrytion status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @u32HDCPStatus	    \b IN: Encrytion status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_SetHDCP1Status(MS_U32 u32HdmiPort, MS_U32 u32HDCPStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_GetHDMIStatus()
/// Get HDMI enable status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @pu32HDMIStatus	    \b IN: MS_U32 pointer which return HDMI enable status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_GetHDMIStatus(MS_U32 u32HdmiPort, MS_U32 *pu32HDMIStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_SetHDMIStatus()
/// Set HDMI enable status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @u32HDMIStatus	    \b IN: HDMI enable status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_SetHDMIStatus(MS_U32 u32HdmiPort, MS_U32 u32HDMIStatus);

#endif //#ifndef _DRV_HDCP_H_
