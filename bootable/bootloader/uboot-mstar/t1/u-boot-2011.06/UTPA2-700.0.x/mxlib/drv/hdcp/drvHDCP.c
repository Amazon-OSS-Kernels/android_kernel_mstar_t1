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
/// file   drvHDCP.c
/// @brief  HDCP relative functions
///////////////////////////////////////////////////////////////////////////////////////////////////

/*********************************************************************/
/*                                                                                                                     */
/*                                                   Includes                                                      */
/*                                                                                                                     */
/*********************************************************************/
#include "MsTypes.h"
#include "MsCommon.h"
#include <stdio.h>
#include <string.h>
#include "drvHDCP.h"
#include "halHDCP.h"
#include "MsOS.h"
#include "MsDevice.h"
#include "drvMMIO.h"

#ifndef DRV_HDCP_C
#define DRV_HDCP_C

/*********************************************************************/
/*                                                                                                                     */
/*                                                      Defines                                                    */
/*                                                                                                                     */
/*********************************************************************/
/*********************************************************************/
/*                                                                                                                     */
/*                                                      Global                                                      */
/*                                                                                                                     */
/*********************************************************************/
static MS_BOOL g_bHdcpMMIOBaseInit = FALSE;

/*********************************************************************/
/*                                                                                                                     */
/*                                                    Functions                                                    */
/*                                                                                                                     */
/*********************************************************************/
/*********************************************************************/
/*                                                                                                                     */
/*                                  HDCP14 Tx Function Start                                              */
/*                                                                                                                     */
/*********************************************************************/
void __FUNC_HDCP14TX_Relative__(void){} // dummy function for pivot
//TBD

/*********************************************************************/
/*                                                                                                                     */
/*                                  HDCP14 Rx Function Start                                              */
/*                                                                                                                     */
/*********************************************************************/
void __FUNC_HDCP14RX_Relative__(void){} // dummy function for pivot
//TBD
MS_BOOL MDrv_HDCP_HDCP14GetM0(MS_U8 u8PortIdx, MS_U8 *pu8Data)
{
    if (pu8Data == NULL)
    {
        return FALSE; //DRVAESDMA_FAIL;
    }
    else
    {
        MHal_HDCP_HDCP14GetM0(u8PortIdx, pu8Data);
        return TRUE; //DRVAESDMA_OK;
    }
}

/*********************************************************************/
/*                                                                                                                     */
/*                                  HDCP22 Tx Function Start                                              */
/*                                                                                                                     */
/*********************************************************************/
void __FUNC_HDCP22TX_Relative__(void){} // dummy function for pivot

MS_BOOL MDrv_HDCP_HDCP2Init(void)
{
    MS_VIRT u32Bank, u32IRBank;
    MS_PHY u32BankSize;  // Non-PM bank
    MS_PHY u32IrBankSize;  // PM bank

    if (g_bHdcpMMIOBaseInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_BDMA))
        {
            printf("MDrv_MMIO_GetBASE (NonPM base)fail\n");
            MS_ASSERT(0);
        }

        if (FALSE == MDrv_MMIO_GetBASE(&u32IRBank, &u32IrBankSize, MS_MODULE_IR))
        {
            printf("MDrv_MMIO_GetBASE (PM base)fail\n");
            MS_ASSERT(0);
        }

        MHal_HDCP_HDCP2SetBank(u32Bank, u32IRBank);
        g_bHdcpMMIOBaseInit = TRUE;
    }

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_HDCP2TxEnableEncrypt()
//  [Description]:
//                  enable/disable Hdcp22 Tx encrytion process
//  [Arguments]:
//                  [MS_U8] u8PortIdx
//                  [MS_BOOL] bEnable
//  [Return]:
//                  void
//
//**************************************************************************
MS_BOOL MDrv_HDCP_HDCP2TxEnableEncrypt(MS_U8 u8PortIdx, MS_BOOL bEnable)
{
    MHal_HDCP_HDCP2TxEnableEncrypt(u8PortIdx, bEnable);

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_HDCP2TxFillCipherKey()
//  [Description]:
//                  Fill Cipher key (Ks^LC128, riv) for HDCP 2.2 Tx
//  [Arguments]:
//                  [MS_U8] u8PortIdx
//                  [MS_U8*] pu8Riv,
//                  [MS_U8*] pu8KsXORLC128
//  [Return]:
//                  void
//
//**************************************************************************
MS_BOOL MDrv_HDCP_HDCP2TxFillCipherKey(MS_U8 u8PortIdx, MS_U8 *pu8Riv, MS_U8 *pu8ContentKey)
{
    MHal_HDCP_HDCP2TxFillCipherKey( u8PortIdx, pu8Riv, pu8ContentKey);

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_HDCP2TxGetCipherState()
//  [Description]:
//                  return Status about HDCP22 Tx Cipher engine
//  [Arguments]:
//                  [MS_U8] u8PortIdx
//                  [MS_U8*] pu8State
//  [Return]:
//                  [MS_U32]
//
//**************************************************************************
MS_BOOL MDrv_HDCP_HDCP2TxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State)
{
    MHal_HDCP_HDCP2TxGetCipherState(u8PortIdx, pu8State);

    return TRUE;
}

/*********************************************************************/
/*                                                                                                                     */
/*                                  HDCP22 Rx Function Start                                              */
/*                                                                                                                     */
/*********************************************************************/
void __FUNC_HDCP22RX_Relative__(void){} // dummy function for pivot

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_HDCP2RxProcessCipher()
//  [Description]:
//                  Fill Cipher key (Ks^LC128, riv) for HDCP 2.2 Rx
//  [Arguments]:
//                  [MS_U8] u8PortIdx
//                  [MS_U8*] pu8Riv,
//                  [MS_U8*] pu8ContentKey
//  [Return]:
//                  [MS_U32]
//
//**************************************************************************
MS_BOOL MDrv_HDCP_HDCP2RxProcessCipher(MS_U8 u8PortIdx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey)
{
    MHal_HDCP_HDCP2RxProcessCipher(u8PortIdx, pu8Riv, pu8ContentKey);

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_HDCP2RxGetCipherState()
//  [Description]:
//                  return Status about HDCP22 Rx Cipher engine
//  [Arguments]:
//                  [MS_U8] u8PortIdx
//                  [MS_U8*] pu8State
//  [Return]:
//                  [MS_U32]
//
//**************************************************************************
MS_BOOL MDrv_HDCP_HDCP2RxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State)
{
    MHal_HDCP_HDCP2RxGetCipherState(u8PortIdx, pu8State);

    return TRUE;
}


#endif //#ifndef DRV_HDCP_C
