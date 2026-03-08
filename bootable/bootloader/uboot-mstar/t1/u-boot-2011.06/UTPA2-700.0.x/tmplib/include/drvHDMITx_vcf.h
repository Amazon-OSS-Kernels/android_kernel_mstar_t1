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
/// @file   drvHDMITx_VCF.h
/// @brief  HDMI Tx Driver Interface for Vancleef
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HDMITX_VCF_H_
#define _DRV_HDMITX_VCF_H_


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


typedef enum
{
    E_VANCLEEF_EVENT_RUN = 0x00000001,
    E_VANCLEEF_EVENT_IRQ = 0x00000002,
} E_VANCLEEF_EVENT;

typedef enum
{
    E_READM0_EVENT_RUN = 0x00000001,
} E_READM0_EVENT;


//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_HDMITx_Vancleef_Init(void);
MS_BOOL MDrv_HDMITx_Vancleef_RxLoadHDCPKey(void);
MS_BOOL MDrv_HDMITx_Vancleef_RegisterISR(void);
void MDrv_HDMITx_Vancleef_RxLoadBCaps(MS_U8 u8BCapsData);
void MDrv_HDMITx_Vancleef_RxLoadBStatus(MS_U8 *u8BStatusData);
void MDrv_HDMITx_Vancleef_RxWriteKSVFIFO(MS_U8 *u8KsvFifo, MS_U8 u8Size);
void MDrv_HDMITx_Vancleef_RxWriteRepeaterReady(MS_BOOL bFlag);
void MDrv_HDMITx_Vancleef_RxWriteV(MS_U8 *u8VData);
void MDrv_HDMITx_Vancleef_RxClearINT(void);
void MDrv_HDMITx_Vancleef_GetHDMIRxM0(MS_U8 *u8Ptr);
void MDrv_HDMITx_Vancleef_DebugEnable(MS_BOOL bEnable);


#ifdef __cplusplus
}
#endif

#endif // _DRV_HDMITX_VCF_H_

