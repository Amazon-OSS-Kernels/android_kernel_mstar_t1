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
/// MDrv_HDCP_HDCP14GetM0()
/// fetch HDCP 1.4  M0 value from Sink device
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8Data	\b IN: MS_U8 pointer to store returned M0 value
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MDrv_HDCP_HDCP14GetM0(MS_U8 u8PortIdx, MS_U8 *pu8Data);
//HDCP22
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2Init()
/// Initial Process of HDCP2.2
/// @param	\b IN: none
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MDrv_HDCP_HDCP2Init(void);
//HDCP22 TX
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2TxEnableEncrypt()
/// Enable HDCP2.2 Encryption
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param bEnable: 	\b IN: TRUE: enable encryption; FALSE: disable encryption
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MDrv_HDCP_HDCP2TxEnableEncrypt(MS_U8 u8PortIdx, MS_BOOL bEnable);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2TxFillCipherKey()
/// Fill HDCP2.2 TX Cipher Key
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @pu8Riv		\b IN: MS_U8 pointer which stored RIV data
/// @pu8ContentKey	\b IN: MS_U8 pointer which stored KS^LV128
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MDrv_HDCP_HDCP2TxFillCipherKey(MS_U8 u8PortIdx, MS_U8 *pu8Riv, MS_U8 *pu8ContentKey);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2RxGetCipherState()
/// Get current HDCP2.2 TX status
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8State		\b IN: MS_U8 pointer which return HDCP22 TX cipher state
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MDrv_HDCP_HDCP2TxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State);
//HDCP22 Rx
//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxProcessCipher()
/// Fill HDCP2.2 RX Cipher Key
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8Riv		\b IN: MS_U8 pointer which stored RIV data
/// @pu8ContentKey	\b IN: MS_U8 pointer which stored KS^LV128
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MDrv_HDCP_HDCP2RxProcessCipher(MS_U8 u8PortIdx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2RxGetCipherState()
/// Get current HDCP2.2 RX status
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8State		\b IN: MS_U8 pointer which return HDCP22 RX cipher state
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MDrv_HDCP_HDCP2RxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State);

#endif //#ifndef _DRV_HDCP_H_
