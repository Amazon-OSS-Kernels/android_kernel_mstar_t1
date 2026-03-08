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
/// @file  apiXC_PCMonitor_EX.h
/// @brief header file
///
////////////////////////////////////////////////////////////////////////////////
#ifndef _API_XC_PC_MONITOR_EX_H_
#define _API_XC_PC_MONITOR_EX_H_

#include "MsCommon.h"

// Driver
//#include "drvXC_IOPort.h"

// Api
#include "apiXC_EX.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define XC_PCMONITOR_EX_ENABLE_IP_AUTO_COAST    (0)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Indecate the status of pc monitor.
typedef enum
{
    E_XC_PCMONITOR_EX_STABLE_NOSYNC = 0,       ///< Input timing stable, no input sync detected
    E_XC_PCMONITOR_EX_STABLE_SYNC ,            ///< Input timing stable, no input sync detected
    E_XC_PCMONITOR_EX_UNSTABLE,                ///< Timing change, has to wait InfoFrame if HDMI input
} XC_PCMONITOR_EX_STATUS;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC MS_BOOL MApi_XC_PCMonitor_EX_Init(XC_DEVICE_ID *pDeviceId, MS_U8 u8MaxWindowNum);
DLL_PUBLIC void MApi_XC_PCMonitor_EX_Restart(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC void MApi_XC_PCMonitor_EX_SetTimingCount(XC_DEVICE_ID *pDeviceId, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);

DLL_PUBLIC XC_PCMONITOR_EX_STATUS MApi_XC_PCMonitor_EX(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE src, XC_EX_SCALER_WIN eWindow);

// get input timing information
DLL_PUBLIC XC_PCMONITOR_EX_STATUS MApi_XC_PCMonitor_EX_GetCurrentState(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC MS_U8 MApi_XC_PCMonitor_EX_GetSyncStatus(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get horizontal frequency * 10
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 horizontal frequency * 10
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_XC_PCMonitor_EX_Get_HFreqx10(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get horizontal frequency * 1000
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U32 horizontal frequency * 1000
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_XC_PCMonitor_EX_Get_HFreqx1K(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical frequency * 10
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 vertical frequency * 10
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_XC_PCMonitor_EX_Get_VFreqx10(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical frequency * 1000
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U32 vertical frequency * 1000
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_XC_PCMonitor_EX_Get_VFreqx1K(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC MS_U16 MApi_XC_PCMonitor_EX_Get_Vtotal(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC void MApi_XC_PCMonitor_EX_Get_Dvi_Hdmi_De_Info(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow,XC_EX_WINDOW_TYPE* msWin);
DLL_PUBLIC MS_BOOL MApi_XC_PCMonitor_EX_SyncLoss(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC MS_BOOL MApi_XC_PCMonitor_EX_InvalidTimingDetect(XC_DEVICE_ID *pDeviceId, MS_BOOL bPollingOnly, XC_EX_SCALER_WIN eWindow);
DLL_PUBLIC XC_EX_RETURN_VALUE MApi_XC_PCMonitor_EX_SetTimingCountEx( XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE eCurrentSrc, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);

#ifdef __cplusplus
}
#endif

#endif /* _API_XC_PC_MONITOR_EX_H_ */
