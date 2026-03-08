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

#ifndef _NSK2OTPFIELD_H_
#define _NSK2OTPFIELD_H_

////////////////////////////////////////////////////////////////////////////////
/// @file nsk2otpfield.h
/// @brief
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    ContentProtectionEnable = 0x40,
    NDS_OTP_TagBegin = ContentProtectionEnable,
    U_OTP_SBoot_0 = 0x41,
    U_OTP_ena_NSK2 = 0x42,
    U_OTP_ena_ACPUUseNSK2 = 0x43,
    U_OTP_ena_DBUSUseNSK2 = 0x44,
    U_OTP_ena_ForceTenSec = 0x45,
    U_OTP_ena_SCCheck = 0x46,
    U_OTP_ena_SWRN = 0x47,
    U_OTP_ena_TestRCFreq = 0x48,
    U_OTP_VenderConstSel = 0x49,

    U_OTP_RANDOM_0 = 0x4A,
    U_OTP_NOISE_0 = 0x4B,
    U_OTP_NDS_Parity_ChkEn_0 = 0x4C,
    U_OTP_OTPWritePWDProtect_0 = 0x4D,

    U_OTP_concurrency_configuration = 0x4E,
    U_OTP_nds_fc_disable = 0x4F,

    U_OTP_ena_EMMFilter = 0x52,
    U_OTP_allow_NSKCW2CryptoDMA = 0x53,
    U_OTP_ena_ACPU2DMA = 0x54,
    U_OTP_ena_DMA_DESBasedCipher = 0x55,
    U_OTP_ena_DMA_GetKeyFromKL = 0x56,
    U_OTP_allow_ACPU2KT = 0x57,


    U_OTP_allow_ACPUWrNSKKey2KT = 0x58,
    U_OTP_allow_NSK2KT = 0x59,
    U_OTP_ESAAlgo_invalidate = 0x5A,
    U_OTP_NDS_ContentProtect_sel = 0x5B,
    U_OTP_allow_CSA2Var = 0x5C,
    U_OTP_allow_CSA2Var_nonNSK = 0x5D,
    U_OTP_allow_CSA3NonNskVariant = 0x5E,
    U_OTP_allow_DVBCSA2ComfCipher = 0x5F,
    U_OTP_allow_DVBCSA3Var = 0x60,
    U_OTP_allow_Multi2BasedCipher = 0x61,
    U_OTP_Multi2SysKey = 0x62,
    U_OTP_SWMulti2SysKey = 0x63,
    U_OTP_ModifyXRC_Override = 0x64,
    U_OTP_Permutation_Override = 0x65,
    U_OTP_ena_TS2TSO = 0x66,
    U_OTP_ena_TS2TSO_0 = 0x67,
    U_OTP_ena_TS2TSO_1 = 0x68,
    U_OTP_ContentProEn_all1 = 0x69,
    U_OTP_ena_LSACPCM = 0x6A,
    U_OTP_EJTAG_MODE = 0x6B,
    U_OTP_DIG_JTAGMode = 0x6C,
    U_OTP_SBJTAGMode = 0x6D,
    U_OTP_SBI2CMode = 0x6E,
    U_OTP_I2CMode = 0x6F,	//previous: 7f. changed 2014.June.04
    U_OTP_SCANMode = 0x70,


    U_OTP_ena_USBSlaveMode = 0x71,
    U_OTP_Disable_TestInOut = 0x72,
    U_OTP_ena_NDS_JTAG_PWD = 0x73,//???
    U_OTP_SBoot_1 = 0x74,
    NDS_OTP_TagEnd,
}NSK2_OTPField;

#endif//_NSK2OTP_H_

