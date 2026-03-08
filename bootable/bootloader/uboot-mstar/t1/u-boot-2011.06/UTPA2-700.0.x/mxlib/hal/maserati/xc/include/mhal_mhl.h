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
/// @file   Mdrv_mhl.h
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MMHL_MHL_H_
#define _MMHL_MHL_H_

#if(SUPPORT_MHL)

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MHL_CDR_STABLE_THRESHOLD        200
#define MHL_CDR_STABLE_OFFSET           10

#define CBUS_CDR_CLOCK_TARGET           20
#define CBUS_CLOCK_DETECT_LEVEL         (CBUS_CDR_CLOCK_TARGET *128 /12)

#define CBUS_CDR_OFFSET_TARGET          0.5
#define CBUS_CLOCK_DETECT_OFFSET        (CBUS_CDR_OFFSET_TARGET *128 /12)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 lens;
    MS_U16 databuf[DMHL_CBUS_TX_FIFO];
} mhalCbusFifo_S;

typedef enum
{
    RX_HDMI_RTERM = 0,    // HDMI mode
    RX_MHL_RTERM,    // MHL mode
    RX_RTERM_OFF,    // R-term off
} RXRtermControl_T;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

MS_BOOL mhal_mhl_CBusWrite(mhalCbusFifo_S *pdatabuf);
void mhal_mhl_CbusIsolate(MS_BOOL bFlag);
void mhal_mhl_init(MS_U8 *edid, MS_U8 *devcap);
#if(DMHL_WAKE_UP_PULSE_INT)
void mhal_mhl_MHLForceToStandby(void);
void mhal_mhl_ClearWakeupIntFlag(void);
#endif
MS_BOOL mhal_mhl_CableDetect(void);
void mhal_mhl_Cbus_SetPathEn(MS_BOOL bflag);
//void mhal_mhl_SetTMDSTxOnOff(Bool bflag);
MS_BOOL mhal_mhl_CbusIsMscMsgReceived(void);
void mhal_mhl_CbusIntCB(MS_U8 *rcstate, MS_U8 *rccmd, MS_U8 *rcdata, MS_U8 *rclen, MS_U8 *bIsCmdInData);
MS_U16 mhal_mhl_CbusStatus(void);
#if(DMHL_CDR_SW_MODE)
void mhal_mhl_CDRModeMonitor(MS_BOOL bPathEnable);
#endif
#if(DMHL_LG_PRADA_PATCH)
void mhal_mhl_AdjustCommonModeResistor(MS_BOOL bflag);
#endif
#if(DMHL_TEST_SIGNAL_SUPPORT)
void mhal_mhl_TestSignal(MS_BOOL bflag);
#endif
MS_BOOL mhal_mhl_IsCbusBusy(void);
//void mhal_mhl_MHLForceToFloating(MS_BOOL flag);
void mhal_mhl_RxRtermControl(RXRtermControl_T rctrl);
void mhal_mhl_LoadHdmiBypassTbl(void);
void mhal_mhl_LoadMhl24bitsModeTbl(void);
//void mhal_mhl_LoadMhlPackedPixelModeTbl(void);
void mhal_mhl_PowerCtrl(MS_U8 pctrl);
void mhal_mhl_SetHPD(MS_BOOL bflag);
MS_U8 mhal_mhl_GetDDCErrorCode(void);
MS_U16 mhal_mhl_GetTMDSClk(void);

#endif

#endif //_MHAL_MHL_H_

