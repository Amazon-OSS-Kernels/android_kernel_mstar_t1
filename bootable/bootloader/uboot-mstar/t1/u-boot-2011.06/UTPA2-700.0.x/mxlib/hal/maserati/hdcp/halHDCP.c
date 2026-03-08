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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halHDCP.c
// @brief  HDCP HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************/
/*                                                                                                                     */
/*                                                   Includes                                                      */
/*                                                                                                                     */
/*********************************************************************/
#include <stdio.h>
#include <string.h>
#include "MsCommon.h"
#include "MsTypes.h"
#include "regHDCP.h"
#include "halHDCP.h"
#include "drvCPU.h"

#ifndef HAL_HDCP_C
#define HAL_HDCP_C

/*********************************************************************/
/*                                                                                                                     */
/*                                                      Defines                                                    */
/*                                                                                                                     */
/*********************************************************************/
#define DEF_HDCP_TX_FUNC_EN     0

#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define HalHDCPLogInfo(format, args...)       ULOGI("HDCP", format, ##args)
#define HalHDCPLogWarning(format, args...)    ULOGW("HDCP", format, ##args)
#define HalHDCPLogDebug(format, args...)      ULOGD("HDCP", format, ##args)
#define HalHDCPLogError(format, args...)      ULOGE("HDCP", format, ##args)
#define HalHDCPLogFatal(format, args...)      ULOGF("HDCP", format, ##args)

#else

#define HalHDCPLogInfo(format, args...)       printf(format, ##args)
#define HalHDCPLogWarning(format, args...)    printf(format, ##args)
#define HalHDCPLogDebug(format, args...)      printf(format, ##args)
#define HalHDCPLogError(format, args...)      printf(format, ##args)
#define HalHDCPLogFatal(format, args...)      printf(format, ##args)

#endif


#define DEF_SIZE_OF_KSXORLC128  16
#define DEF_SIZE_OF_RIV         8

MS_VIRT _gHDCPRegBase = 0x00U;
MS_VIRT _gHDCPPMRegBase = 0x00U;

#define HDCPREG(bank, addr)     (*((volatile MS_U16 *)((_gHDCPRegBase + (bank << 1U)) + (addr << 2U))))
#define HDCPPMREG(bank, addr)   (*((volatile MS_U16 *)((_gHDCPPMRegBase + (bank << 1U)) + (addr << 2U))))

#if (DEF_HDCP_TX_FUNC_EN == 1) //if chip don't support TX functoin, don't care
#define DEF_HDCP14_TX_KEY_REG_BANK  0x000000U
#define DEF_HDCP22_TX_KEY_REG_BANK  0x072700U //0x172700U
#define DEF_HDCP14_TX_REG_BANK      0x072B00U //0x172B00U
#define DEF_HDCP22_TX_REG_BANK      0x072F00U //0x172F00U
#endif

#define DEF_HDCP14_RX_KEY_REG_BANK  0x000000U
#define DEF_HDCP22_RX_KEY_REG_BANK  0x073A00U //0x173A00U
#define DEF_HDCP14_RX_REG_BANK      0x071200U //0x171200U
#define DEF_HDCP22_RX_REG_BANK      0x071200U //0x171200U

#define DEF_HDCP14_M0_SIZE          64U //bytes

/*********************************************************************/
/*                                                                                                                     */
/*                                                    Functions                                                    */
/*                                                                                                                     */
/*********************************************************************/
/*********************************************************************/
/*                                                                                                                     */
/*                                                    Internal                                                      */
/*                                                                                                                     */
/*********************************************************************/

MS_U16 MHalHdcpRegRead(MS_U32 bank, MS_U16 address)
{
    return HDCPREG(bank, address);
}

void MHalHdcpRegWrite(MS_U32 bank, MS_U16 address, MS_U16 reg_data)
{
    HalHDCPLogInfo("reg write:: bank = 0x%X : addr = 0x%X : regData = 0x%X\r\n", (unsigned int)bank, address, reg_data);
    HDCPREG(bank, address) = reg_data;
}

void MHalHdcpRegMaskWrite(MS_U32 bank, MS_U16 address, MS_U16 reg_mask, MS_U16 reg_data)
{
    MS_U16 reg_value;
    HalHDCPLogInfo("mask reg write:: bank = 0x%X, addr = 0x%X, regData = 0x%X", (unsigned int)bank, address, reg_data);

    reg_value = (HDCPREG(bank, address) & (~reg_mask)) | (reg_data & reg_mask);
    HDCPREG(bank, address) = reg_value;
}

MS_U16 MHalHdcpPMRegRead(MS_U32 bank, MS_U16 address)
{
    return HDCPPMREG(bank, address);
}

void MHalHdcpPMRegWrite(MS_U32 bank, MS_U16 address, MS_U16 reg_data)
{
    HDCPPMREG(bank, address) = reg_data;
}

void MHalHdcpPMRegMaskWrite(MS_U32 bank, MS_U16 address, MS_U16 reg_mask, MS_U16 reg_data)
{
    MS_U16 reg_value;

    reg_value = (HDCPPMREG(bank, address) & (~reg_mask)) | (reg_data & reg_mask);
    HDCPPMREG(bank, address) = reg_value;
}

/*********************************************************************/
/*                                                                                                                     */
/*                                                    External                                                     */
/*                                                                                                                     */
/*********************************************************************/
void MHal_HDCP_HDCP14GetM0(MS_U8 u8PortIdx, MS_U8 *pu8Data)
{
    MS_U8 cnt = 0x00;
    MS_U16 u16BKOffset = 0x00;

    u8PortIdx &= 0x0F;
    u16BKOffset = u8PortIdx * 0x300;

    //lib_lowprintf("%s:: u8PortIdx = 0x%x; u16BKoffset = 0x%x\r\n", __FUNCTION__, u8PortIdx,  u16BKOffset);
    for ( cnt = 0; cnt < (DEF_HDCP14_M0_SIZE >> 4); cnt++ )
    {
        MS_U16 u16tmpData = 0x00;

        u16tmpData = MHalHdcpRegRead(DEF_HDCP14_RX_REG_BANK + u16BKOffset, 0x0E + cnt);

	//lib_lowprintf("%s:: u16tmpData = 0x%x\r\n", __FUNCTION__, u16tmpData);
        *(pu8Data + cnt*2) = (MS_U8)(u16tmpData & 0x00FF);
        *(pu8Data + cnt*2 + 1) = (MS_U8)((u16tmpData & 0xFF00) >> 8);
    }
}

void MHal_HDCP_HDCP2SetBank(MS_U32 u32NonPmBankAddr, MS_U32 u32PmBankAddr)
{
    HalHDCPLogInfo("u32NonPmBankAddr = 0x%X, u32PmBankAddr = 0x%X\r\n", (unsigned int)u32NonPmBankAddr, (unsigned int)u32PmBankAddr);
    _gHDCPRegBase = u32NonPmBankAddr;
    _gHDCPPMRegBase = u32PmBankAddr;
}

void MHal_HDCP_HDCP2TxEnableEncrypt(MS_U8 u8PortIdx, MS_BOOL bEnable)
{
    return;
}

void MHal_HDCP_HDCP2TxFillCipherKey(MS_U8 u8PortIdx, MS_U8 *pu8Riv, MS_U8 *pu8KsXORLC128)
{
    return;
}

void MHal_HDCP_HDCP2TxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State)
{
    return;
}

void MHal_HDCP_HDCP2RxProcessCipher(MS_U8 u8PortIdx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey)
{
    MS_U8 cnt = 0;
    MS_U16 u16BKOffset = 0x00;
    MS_U16 u16RegOffset = 0x00;

    u16BKOffset = u8PortIdx * 0x300;
    u16RegOffset = u8PortIdx * 0x0C;

    HalHDCPLogDebug("%s:: PortIdx : BKOffset : RegOffset = 0x%X : 0x%X : 0x%X\r\n", __FUNCTION__, u8PortIdx, u16BKOffset, u16RegOffset);

    HalHDCPLogDebug("RIV::\r\n");
    for (cnt = 0; cnt < DEF_SIZE_OF_RIV; cnt++ )
    {
        HalHDCPLogDebug("0x%02X ", *(pu8Riv + cnt));
        if ((cnt+1)%16 == 0)
            HalHDCPLogDebug("\r\n");
    }
    HalHDCPLogDebug("\r\n");

    HalHDCPLogDebug("Ks^LC128::\r\n");
    for (cnt = 0; cnt < DEF_SIZE_OF_KSXORLC128; cnt++ )
    {
        HalHDCPLogDebug("0x%02X ", *(pu8ContentKey + cnt));
        if ((cnt+1)%16 == 0)
            HalHDCPLogDebug("\r\n");
    }
    HalHDCPLogDebug("\r\n");

    //Ks^LC128
    for ( cnt = 0; cnt < (DEF_SIZE_OF_KSXORLC128>>1); cnt++)
        MHalHdcpRegWrite(DEF_HDCP22_RX_KEY_REG_BANK + u16BKOffset, u16RegOffset + 0x30 + (DEF_SIZE_OF_KSXORLC128 >> 1) - 1 - cnt, *(pu8ContentKey + cnt*2 + 1)|(*(pu8ContentKey + cnt*2)<<8));

    //Riv
    for ( cnt = 0; cnt < (DEF_SIZE_OF_RIV>>1); cnt++)
        MHalHdcpRegWrite(DEF_HDCP22_RX_KEY_REG_BANK + u16BKOffset, u16RegOffset + 0x38 + (DEF_SIZE_OF_RIV >> 1) - 1 - cnt, *(pu8Riv + cnt*2 + 1)|(*(pu8Riv + cnt*2)<<8));

    //Set SKE successful
    MHalHdcpRegMaskWrite(DEF_HDCP22_RX_REG_BANK + u16BKOffset, 0x4E, 0x0001, 0x0001);
}

void MHal_HDCP_HDCP2RxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State)
{
    MS_U16 u16BKOffset = 0x00;
    //MS_U16 u16RegOffset = 0x00;

    u16BKOffset = u8PortIdx * 0x300;

    *pu8State = MHalHdcpRegRead(DEF_HDCP22_RX_REG_BANK + u16BKOffset, 0x4E) & 0x01;
}

#endif //#ifndef HAL_HDCP_C
