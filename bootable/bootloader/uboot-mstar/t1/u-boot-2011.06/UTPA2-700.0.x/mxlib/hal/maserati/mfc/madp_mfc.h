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
#ifndef _MADP_MFC_H_
#define _MADP_MFC_H_

//#include "mdrv_types.h"
//#include "mdrv_mfc_io.h"
//#include "mdrv_mfc_st.h"
#include "mdrv_mfc.h"

//------------------------------------------------------------------------------
//  Global veriable
//------------------------------------------------------------------------------
// extern MST_MFC_SYS_INFO_t *mfc_sysinfo;

//------------------------------------------------------------------------------
//  Structure
//------------------------------------------------------------------------------
/*
typedef enum
{
    MFC_I2cUseBusN       = 0x34,
    MFC_I2cUseBus        = 0x35,
    MFC_CpuWaitN         = 0x36,
    MFC_CpuWait          = 0x37,
    MFC_SerialDebugEnter = 0x44,
    MFC_SerialDebugExit  = 0x45,
} MFC_SERIAL_DEBUG_CMD_e;
*/
//------------------------------------------------------------------------------
//  Functions prototype
//------------------------------------------------------------------------------
void MAdp_MFC_SpiLoadCode(void);
void MAdp_MFC_Init(PMST_MFC_SYS_INFO_t mfcSysInfo, U8 u8IsS7M);
void MAdp_MFC_OnOff(BOOL bOnOff);
void MAdp_MFC_Compensation(U8 u8Blur, U8 u8Judder, U8 u8MFC);
void MAdp_MFC_True_Motion_Demo(U8 u8TrueMotionDemo);
void MAdp_MFC_Video_Block(U8 u8Ctrl, BOOL bCtrl);
void MAdp_MFC_LVDSOnOff(BOOL bOnOff);
void MAdp_MFC_BypassWindow(BOOL bOnOff, U8 u8WinId, U16 u16Hstart, U16 u16Vstart, U16 u16Hsize, U16 u16Vsize);
U16 MAdp_MFC_GetFirmwareVersion(void);
U16 MAdp_MFC_GetFirmwareVersionFromBin(void);
void MAdp_MFC_SetSpreadSpectrum(U16 u16KHz, U16 u16Percent, BOOL bEnable, BOOL bMiuLVDS);
void MAdp_MFC_OnOffDebugBlock(BOOL bOnOff);
void MAdp_MFC_DemoBarControl(BOOL bOnOFF, BOOL bDirection, U8 u8Width, U8 u8Color);
U8 MAdp_MFC_GetModelType(void);
void MAdp_MFC_ReadHVTotal(U16* pHTotal, U16* pVTotal);
U8 MAdp_MFC_IsStable(void);
void MAdp_MFC_SetLVDSVesaJeida(BOOL bVesaJeida);
void MAdp_MFC_SetFrameRate(U8 u8frameRate);
void MAdp_MFC_SetSlowFrameLock(BOOL bEnable);
void MAdp_MFC_ControlFrameLockMode(U8 u8type);
void MAdp_MFC_RESET(void);
void MAdp_MFC_UpdateSW(void);
void MAdp_MFC_SetReverseControl(U8 u8Mode);
void MAdp_MFC_SetLVDSBitNum(U8 u8LVDSBitNum);
void MAdp_MFC_SetODCTable(void);
void MAdp_MFC_SetVFreq(U16 u16Inputfreq, BOOL enableFPLL);

#endif /* _MADP_MFC_H_ */
