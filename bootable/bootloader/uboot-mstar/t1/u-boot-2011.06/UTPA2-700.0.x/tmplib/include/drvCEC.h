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

// INTERFACE void MDrv_CEC_PortSelect(MsCEC_INPUT_PORT InputPort);
// INTERFACE void MDrv_CEC_Init(MS_U32 u32XTAL_CLK_Hz);
// INTERFACE void MDrv_CEC_Exit(void);
// INTERFACE void MDrv_CEC_CheckExistDevices(void);
INTERFACE MS_BOOL MDrv_CEC_RxChkBuf(void);
INTERFACE CEC_ERROR_CODE MDrv_CEC_TxApi(MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8* operand_ptr, MS_U8 len);
// INTERFACE CEC_ERROR_CODE MDrv_CEC_TxApi2(MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8* operand_ptr, MS_U8 len);
// INTERFACE MS_BOOL MDrv_CEC_CheckFrame(MsCEC_MSG_TRANS_TYPE msg_type, MS_U8 ucLen);
// INTERFACE void MDrv_CEC_ConfigWakeUp(void);
// INTERFACE void MDrv_CEC_Enabled(MS_BOOL bEnableFlag);
// INTERFACE void MDrv_CEC_SetMyLogicalAddress(MsCEC_DEVICELA MyLogicalAddress);
// INTERFACE void MDrv_CEC_InitChip(MS_U32 u32XTAL_CLK_Hz);
// INTERFACE MS_U8 MDrv_CEC_TxStatus(void);
// INTERFACE MS_BOOL MDrv_CEC_Device_Is_Tx(void);
// INTERFACE MS_U32 MDrv_CEC_SetPowerState(EN_POWER_MODE u16PowerState);

#if ENABLE_CEC_MULTIPLE
INTERFACE void MDrv_CEC_SetMyLogicalAddress2(MsCEC_DEVICELA MyLogicalAddress);
#endif



// INTERFACE void MDrv_CEC_SetRetryCount(MS_U8 u8RetryCount);
// INTERFACE void MDrv_CEC_ConfigWakeupInfoVendorID(MS_U8* u8CecVendorID);




////////////////////////////////////////////////////////////////////////////////
#undef INTERFACE

////////////////////////////////////////////////////////////////////////////////
#endif //_DRV_CEC_H_

