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

#ifndef _DLC_V2_H_
#define _DLC_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
//******** ENUM DEFINITIONS ********//
enum
{
    E_DLC_POOL_ID_INTERNAL = 0, // just for test, please refine.
} E_DLC_POOL_ID;


typedef enum
{
    E_DLC_CMD_INIT_EX,
    E_DLC_CMD_SET_SETTING_EX,
    E_DLC_CMD_SET_CURVE,
    E_DLC_CMD_SET_BLE_SLOP_POINT,
    E_DLC_CMD_SET_HDR_INFO,
    E_DLC_CMD_EXIT,
    E_DLC_CMD_SET_ON_OFF,
    E_DLC_CMD_SET_HANDLER_ON_OFF,
    E_DLC_CMD_SET_BLE_ON_OFF,
    E_DLC_CMD_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION,
    E_DLC_CMD_HANDLER,
    E_DLC_CMD_GET_HISTOGRAM_HANDLER,
    E_DLC_CMD_GET_AVERAGE_VALUE,
    E_DLC_CMD_GET_AVERAGE_VALUE_EX,
    E_DLC_CMD_INIT_CURVE,
    E_DLC_CMD_SPEEDUP_TRIGGER,
    E_DLC_CMD_GET_LUMA_CURVE_STATUS,
    E_DLC_CMD_CGC_RESET_C_GAIN,
    E_DLC_CMD_CGC_CHECK_C_GAIN_IN_PQ_COM,
    E_DLC_CMD_CGC_RESET_Y_GAIN,
    E_DLC_CMD_CGC_CHECK_Y_GAIN_IN_PQ_COM,
    E_DLC_CMD_CGC_RESET,
    E_DLC_CMD_CGC_INIT,
    E_DLC_CMD_CGC_REINIT,
    E_DLC_CMD_CGC_HANDLER,
    E_DLC_CMD_GET_LIB_VER,
    E_DLC_CMD_GET_INFO,
    E_DLC_CMD_GET_STATUS_EX,
    E_DLC_CMD_SET_DBG_LEVEL,
    E_DLC_CMD_WRITE_CURVE,
    E_DLC_CMD_WRITE_CURVE_SUB,
    E_DLC_CMD_GET_HISTOGRAM,
    E_DLC_CMD_DECODE_EXT_CMD,
    E_DLC_CMD_SET_CAPTURE_RANGE,
    E_DLC_CMD_SET_SETTING,
    E_DLC_CMD_INIT,
    E_DLC_CMD_GET_STATUS,
    E_DLC_CMD_SET_POWERSTATE,
    E_DBC_CMD_DLC_INIT,
    E_DBC_CMD_INIT,
    E_DBC_CMD_SET_STATUS,
    E_DBC_CMD_GET_STATUS,
    E_DBC_CMD_SET_READY,
    E_DBC_CMD_SET_DEBUG_MODE,
    E_DBC_CMD_GET_DEBUG_MODE,
    E_DBC_CMD_UPDATE_PWM,
    E_DBC_CMD_HANDLER,
    E_DBC_CMD_ADJUST_YC_GAIN,
    E_DBC_CMD_YC_GAIN_INIT,
    E_DBC_CMD_RESET,
    E_DBC_CMD_DECODE_EXT_CMD,
    E_DBC_CMD_NUM,
    E_DLC_CMD_MAX = E_DBC_CMD_NUM,
} E_DLC_IOCTL_CMDS;


typedef struct _stDLC_SetHDRInitData
{
    XC_DLC_HDRinit *pstDLC_HDRInitData;
    MS_BOOL bReturn;
} stDLC_SetHDRInitData, *pstDLC_HDRInitData;

typedef struct _stDLC_Init_Ex
{
    XC_DLC_init *pstXC_DLC_InitData;
    MS_U32 u32InitDataLen;
    MS_BOOL bReturn;
} stDLC_Init_Ex, *pstDLC_Init_Ex;


typedef struct _stDLC_SetSetting_Ex
{
    XC_DLC_MFinit_Ex *DLC_MFinit_Ex;
} stDLC_SetSetting_Ex, *pstDLC_SetSetting_Ex;


typedef struct _stDLC_SetCurve
{
    MS_U8 *pNormal;
    MS_U8 *pLight;
    MS_U8 *pDark;
} stDLC_SetCurve, *pstDLC_SetCurve;


typedef struct _stDLC_SetBleSlopPoint
{
    MS_U16 *pBLESlopPoint;
} stDLC_SetBleSlopPoint, *pstDLC_SetBleSlopPoint;


typedef struct _stDLC_Exit
{
    MS_BOOL bReturn;
} stDLC_Exit, *pstDLC_Exit;


typedef struct _stDLC_SetOnOff
{
    MS_BOOL bSwitch;
    MS_BOOL bWindow;
} stDLC_SetOnOff, *pstDLC_SetOnOff;


typedef struct _stDLC_SetDlcHandlerOnOff
{
    MS_BOOL bDlcEnable;
    MS_BOOL bReturn;
} stDLC_SetDlcHandlerOnOff, *pstDLC_SetDlcHandlerOnOff;


typedef struct _stDLC_SetBleOnOff
{
    MS_BOOL bSwitch;
    MS_BOOL bReturn;
} stDLC_SetBleOnOff, *pstDLC_SetBleOnOff;


typedef struct _stDLC_EnableMainSubCurveSynchronization
{
    MS_BOOL bEnable;
} stDLC_EnableMainSubCurveSynchronization, *pstDLC_EnableMainSubCurveSynchronization;


typedef struct _stDLC_Handler
{
    MS_BOOL bWindow;
} stDLC_Handler, *pstDLC_Handler;


typedef struct _stDLC_GetHistogramHandler
{
    MS_BOOL bWindow;
    MS_BOOL bReturn;
} stDLC_GetHistogramHandler, *pstDLC_GetHistogramHandler;


typedef struct _stDLC_GetAverageValue
{
    MS_U8 u8Return;
} stDLC_GetAverageValue, *pstDLC_GetAverageValue;


typedef struct _stDLC_GetAverageValue_Ex
{
    MS_U16 u16Return;
} stDLC_GetAverageValue_Ex, *pstDLC_GetAverageValue_Ex;


typedef struct _stDLC_InitCurve
{
    MS_U16 u16HStart;
    MS_U16 u16HEnd;
    MS_U16 u16VStart;
    MS_U16 u16VEnd;
} stDLC_InitCurve, *pstDLC_InitCurve;


typedef struct _stDLC_SpeedupTrigger
{
    MS_U8 u8Loop;
} stDLC_SpeedupTrigger, *pstDLC_SpeedupTrigger;


typedef struct _stDLC_GetLumaCurveStatus
{
    MS_BOOL bReturn;
} stDLC_GetLumaCurveStatus, *pstDLC_GetLumaCurveStatus;


typedef struct _stDLC_CGC_ResetCGain
{

} stDLC_CGC_ResetCGain, *pstDLC_CGC_ResetCGain;


typedef struct _stDLC_CGC_CheckCGainInPQCom
{

} stDLC_CGC_CheckCGainInPQCom, *pstDLC_CGC_CheckCGainInPQCom;


typedef struct _stDLC_CGC_ResetYGain
{

} stDLC_CGC_ResetYGain, *pstDLC_CGC_ResetYGain;


typedef struct _stDLC_CGC_CheckYGainInPQCom
{

} stDLC_CGC_CheckYGainInPQCom, *pstDLC_CGC_CheckYGainInPQCom;


typedef struct _stDLC_CGC_Reset
{

} stDLC_CGC_Reset, *pstDLC_CGC_Reset;


typedef struct _stDLC_CGC_Init
{

} stDLC_CGC_Init, *pstDLC_CGC_Init;


typedef struct _stDLC_CGC_ReInit
{

} stDLC_CGC_ReInit, *pstDLC_CGC_ReInit;


typedef struct _stDLC_CGC_Handler
{

} stDLC_CGC_Handler, *pstDLC_CGC_Handler;


typedef struct _stDLC_GetLibVer
{
    const MSIF_Version **ppVersion;
    E_XC_DLC_RESULT eResult;
} stDLC_GetLibVer, *pstDLC_GetLibVer;


typedef struct _stDLC_GetInfo
{
    const XC_DLC_ApiInfo * eResult;
} stDLC_GetInfo, *pstDLC_GetInfo;


typedef struct _stDLC_GetStatus_Ex
{
    XC_DLC_ApiStatus *pDrvStatus;
    MS_BOOL bWindow;
    MS_BOOL bReturn;
} stDLC_GetStatus_Ex, *pstDLC_GetStatus_Ex;


typedef struct _stDLC_SetDbgLevel
{
    MS_U16 u16DbgSwitch;
    MS_BOOL bReturn;
} stDLC_SetDbgLevel, *pstDLC_SetDbgLevel;


typedef struct _stDLC_WriteCurve
{
    MS_U8 *pu8Table;
    MS_BOOL bReturn;
} stDLC_WriteCurve, *pstDLC_WriteCurve;


typedef struct _stDLC_WriteCurve_Sub
{
    MS_U8 *pu8Table;
    MS_BOOL bReturn;
} stDLC_WriteCurve_Sub, *pstDLC_WriteCurve_Sub;


typedef struct _stDLC_GetHistogram
{
    MS_U16 *pu16Histogram;
    E_XC_DLC_HISTOGRAM_TYPE enHistogramType;
    MS_BOOL bReturn;
} stDLC_GetHistogram, *pstDLC_GetHistogram;


typedef struct _stDLC_DecodeExtCmd
{
    tDLC_CONTROL_PARAMS *params;
} stDLC_DecodeExtCmd, *pstDLC_DecodeExtCmd;


typedef struct _stDLC_SetCaptureRange
{
    XC_DLC_CAPTURE_Range *pu16_Range;
} stDLC_SetCaptureRange, *pstDLC_SetCaptureRange;


typedef struct _stDLC_SetSetting
{
    XC_DLC_MFinit DLC_MFinit;
} stDLC_SetSetting, *pstDLC_SetSetting;


typedef struct _stDLC_Init
{
    XC_DLC_init *pstXC_DLC_InitData;
    MS_U32 u32InitDataLen;
    MS_BOOL bReturn;
} stDLC_Init, *pstDLC_Init;


typedef struct _stDLC_GetStatus
{
    XC_DLC_ApiStatus *pDrvStatus;
    MS_BOOL bWindow;
    MS_BOOL bReturn;
} stDLC_GetStatus, *pstDLC_GetStatus;

typedef struct _stDLC_SetPowerState
{
    EN_POWER_MODE enPowerState;
    MS_U32 u32Return;
} stDLC_SetPowerState, *pstDLC_SetPowerState;


typedef struct _stDBC_DLC_Init
{
    XC_DLC_DBC_MFinit DLC_DBC_MFinit;
} stDBC_DLC_Init, *pstDBC_DLC_Init;


typedef struct _stDBC_Init
{

} stDBC_Init, *pstDBC_Init;


typedef struct _stDBC_Setstatus
{
    MS_BOOL bDBCEnable;
} stDBC_Setstatus, *pstDBC_Setstatus;


typedef struct _stDBC_Getstatus
{
    MS_BOOL bReturn;
} stDBC_Getstatus, *pstDBC_Getstatus;


typedef struct _stDBC_SetReady
{
    MS_BOOL bDBCReady;
} stDBC_SetReady, *pstDBC_SetReady;


typedef struct _stDBC_SetDebugMode
{
    MS_U8 ucDBC_DebugMode;
} stDBC_SetDebugMode, *pstDBC_SetDebugMode;


typedef struct _stDBC_GetDebugMode
{
    MS_U8 u8Return;
} stDBC_GetDebugMode, *pstDBC_GetDebugMode;


typedef struct _stDBC_UpdatePWM
{
    MS_U8 u8PWMvalue;
} stDBC_UpdatePWM, *pstDBC_UpdatePWM;


typedef struct _stDBC_Handler
{
    MS_U16 u16Return;
} stDBC_Handler, *pstDBC_Handler;


typedef struct _stDBC_AdjustYCGain
{

} stDBC_AdjustYCGain, *pstDBC_AdjustYCGain;


typedef struct _stDBC_YCGainInit
{
    MS_U8 u8YGain_M;
    MS_U8 u8YGain_L;
    MS_U8 u8CGain_M;
    MS_U8 u8CGain_L;
} stDBC_YCGainInit, *pstDBC_YCGainInit;


typedef struct _stDBC_Reset
{

} stDBC_Reset, *pstDBC_Reset;


typedef struct _stDBC_DecodeExtCmd
{
    tDBC_CONTROL_PARAMS *params;
} stDBC_DecodeExtCmd, *pstDBC_DecodeExtCmd;

#ifdef __cplusplus
}
#endif

#undef _DLC_V2_H_
#endif //_DLC_V2_H_

