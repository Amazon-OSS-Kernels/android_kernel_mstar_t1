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
/// @file  apiXC_PCMonitor.h
/// @brief header file
///
////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Video Video modules

 *  \defgroup XC_FE XC_FE modules
 *  \ingroup Video 
 
    PC monitor is used to detect signal information.\n
    For more information,see \link PC_Monitor PC_Monitor interface (apiXC_PCMonitor.h) \endlink

 * \defgroup PC_Monitor PC_Monitor interface (apiXC_PCMonitor.h)
 *  \ingroup XC_FE
 
    PC monitor is used to detect signal information such as
    - input source type : Component / HDMI / DVI / VGA
    - input horizontal frequency
    - input vertical frequency
    - input vertical total
    - input sync detection : H/V polarity, scan type
    - HDMI/DVI only : Data Enable section detection result
    - Euro HDTV detection result
    and the sync stable result will be passed back and make sure these signal characteristics are steady.

    TO BE NOTICED :
    signal characteristics being steady does not mean that this signal are valid signal.

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html apiXC_PCMonitor_base_flow.png

 * \defgroup PC_Monitor_INIT PC_Monitor init control
 *  \ingroup PC_Monitor

 *! \defgroup PC_Monitor_FEATURE PC_Monitor feature operation
 *  \ingroup PC_Monitor

 *! \defgroup PC_Monitor_INFO PC_Monitor Infomation pool
 *  \ingroup PC_Monitor

 *! \defgroup PC_Monitor_ToBeModified PC_Monitor api to be modified
 *  \ingroup PC_Monitor

 *! \defgroup PC_Monitor_ToBeRemove PC_Monitor api to be removed
 *  \ingroup PC_Monitor
 */


#ifndef _API_XC_PC_MONITOR_H_
#define _API_XC_PC_MONITOR_H_
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

#define ENABLE_IP_AUTO_COAST    (0)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Indicate the status of pc monitor.
typedef enum
{
    E_XC_PCMONITOR_STABLE_NOSYNC = 0,       ///< Input timing stable, no input sync detected
    E_XC_PCMONITOR_STABLE_SYNC ,            ///< Input timing stable, no input sync detected
    E_XC_PCMONITOR_UNSTABLE,                ///< Timing change, has to wait InfoFrame if HDMI input
} XC_PCMONITOR_STATUS;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// PC monitor Init
/// @ingroup PC_Monitor_INIT
/// @param  u8MaxWindowNum                 \b IN: max window number
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_PCMonitor_Init(MS_U8 u8MaxWindowNum);
//-------------------------------------------------------------------------------------------------
/// PC monitor restart
/// @ingroup PC_Monitor_FEATURE
/// @param  eWindow                 \b IN: window ID
//-------------------------------------------------------------------------------------------------
void MApi_XC_PCMonitor_Restart(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// PC monitor restart
/// @ingroup PC_Monitor_ToBeModified
/// @param  u16TimingStableCounter                 \b IN: stable counter
/// @param  u16TimingNosyncCounter                 \b IN: no signal counter
//-------------------------------------------------------------------------------------------------
void MApi_XC_PCMonitor_SetTimingCount(MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);

//-------------------------------------------------------------------------------------------------
/// PC monitor
/// @ingroup PC_Monitor_FEATURE
/// @param  src                         \b IN: stable counter
/// @param  eWindow                 \b IN: window ID
/// @return XC_PCMONITOR_STATUS
//-------------------------------------------------------------------------------------------------
XC_PCMONITOR_STATUS MApi_XC_PCMonitor(INPUT_SOURCE_TYPE_t src, SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get current status of PC monitor
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: window ID
/// @return XC_PCMONITOR_STATUS
//-------------------------------------------------------------------------------------------------
XC_PCMONITOR_STATUS MApi_XC_PCMonitor_GetCurrentState(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get the sync status of PC monitor
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: window ID
/// @return MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_PCMonitor_GetSyncStatus(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get horizontal frequency * 10 
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 horizontal frequency * 10
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_PCMonitor_Get_HFreqx10(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get horizontal frequency * 1000
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U32 horizontal frequency * 1000
//-------------------------------------------------------------------------------------------------
// MS_U32 MApi_XC_PCMonitor_Get_HFreqx1K(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical frequency * 10
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 vertical frequency * 10
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_PCMonitor_Get_VFreqx10(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical frequency * 1000
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U32 vertical frequency * 1000
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_XC_PCMonitor_Get_VFreqx1K(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical total line
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 vertical total line
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_PCMonitor_Get_Vtotal(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get DVI/HDMI DE information
/// @ingroup PC_Monitor_INFO
/// @param  eWindow                 \b IN: which window we are going to get
/// @param  msWin                    \b IN: DE window size
//-------------------------------------------------------------------------------------------------
void MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(SCALER_WIN eWindow,MS_WINDOW_TYPE* msWin);

//-------------------------------------------------------------------------------------------------
/// Get sync loss status
/// @ingroup PC_Monitor_INFO
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_PCMonitor_SyncLoss(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Check the invalid timing
/// @ingroup PC_Monitor_INFO
/// @param  bPollingOnly            \b IN: enable polling only
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_PCMonitor_InvalidTimingDetect(MS_BOOL bPollingOnly, SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// PC monitor restart
/// @ingroup PC_Monitor_ToBeModified
/// @param  u16TimingStableCounter                 \b IN: stable counter
/// @param  u16TimingNosyncCounter                 \b IN: no signal counter
/// @return E_APIXC_ReturnValue
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_PCMonitor_SetTimingCountEx( INPUT_SOURCE_TYPE_t eCurrentSrc, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);

#ifdef __cplusplus
}
#endif

#endif /* _API_XC_PC_MONITOR_H_ */
