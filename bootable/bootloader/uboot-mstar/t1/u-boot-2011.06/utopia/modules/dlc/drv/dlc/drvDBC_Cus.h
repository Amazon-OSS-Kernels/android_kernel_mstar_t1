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
#ifndef DRV_DBC_CUSH
#define DRV_DBC_CUSH

#ifdef __cplusplus
extern "C"
{
#endif
//-----------------------------------------------------------------------
// due to patent issues, temporary DISABLE it
//-----------------------------------------------------------------------
#define ENABLE_DBC  FALSE
#define DBC_CHANGE_STRONG  FALSE

//------------------------------------------------------------------------
// DCR library start
//------------------------------------------------------------------------

typedef struct
{
    MS_U8 ucMaxPWM;
    MS_U8 ucMinPWM;
    MS_U8 ucMax_Video;
    MS_U8 ucMid_Video;
    MS_U8 ucMin_Video;
    MS_U8 ucCurrentPWM;
    MS_U8 ucAlpha;
    MS_U8 ucBackLight_Thres;
    MS_U8 ucAvgDelta;
    MS_U8 ucFlickAlpha;
    MS_U8 ucFastAlphaBlending; // min 17 ~ max 32
    MS_U8 ucLoop_Dly;
    MS_U8 ucLoop_Dly_H_Init;
    MS_U8 ucLoop_Dly_MH_Init;
    MS_U8 ucLoop_Dly_ML_Init;
    MS_U8 ucLoop_Dly_L_Init;
    MS_U8 ucY_Gain_H;
    MS_U8 ucC_Gain_H;
    MS_U8 ucY_Gain_M;
    MS_U8 ucC_Gain_M;
    MS_U8 ucY_Gain_L;
    MS_U8 ucC_Gain_L;
    MS_U8 bYGainCtrl : 1; // 1: enable; 0: disable
    MS_U8 bCGainCtrl : 1; // 1: enable; 0: disable
}StuDbc_FinetuneParamaters_Lib;

//------------------------------------------------------------------------
// For Debug Command
//------------------------------------------------------------------------
typedef void (*MDrv_DLC_DBC_OnOff_Callback)          (MS_BOOL/*On/Off*/);
typedef void (*MDrv_DLC_DBC_AdjustBackLight_Callback)(MS_U32 /*PWM_VALUE*/);
typedef void (*MDrv_DBC_Print_Callback)           (MS_U8 /*PWM_VALUE*/);

#if (ENABLE_DBC )
void    msDBCInit(void);
MS_U16  msBackLightHandler(void);
MS_U16  msDbcLibVersionCheck(void);
void    msAdjustYCGain(MS_U8 u8YGain,MS_U8 u8CGain);
void    msDBC_ParametersTool(MS_U8 *pCmdBuff, MDrv_DLC_DBC_OnOff_Callback fnDBC_AP_OnOff, MDrv_DLC_DBC_AdjustBackLight_Callback fnDBC_AdjustBacklight, MDrv_DBC_Print_Callback putchar );
#else
#define msDBCInit(x)
#define msBackLightHandler(x)  0
#define  msDbcLibVersionCheck(x) 0
#define  msAdjustYCGain(x, y)
#define  msDBC_ParametersTool(a, b, c, d)
#endif

//------------------------------------------------------------------------
// Utopia custom functions
//------------------------------------------------------------------------

MS_U8   MDrv_DLC_DBC_GetDebugMode(void);
void    MDrv_DLC_DBC_SetDebugMode(MS_U8 u8DBC_DebugMode);
void    MDrv_DLC_DBC_UpdatePWM(MS_U8 u8PWMvalue);
void    MDrv_DLC_DBC_Init(StuDbc_FinetuneParamaters_Lib DLC_DBC_MFinit);
MS_U16  MDrv_DLC_DBC_Handler(void);
MS_U16  MDrv_DLC_DBC_GetMFVer(void);
MS_BOOL MDrv_DLC_GetDBCInfo(StuDbc_FinetuneParamaters_Lib *pDLCDBCData, MS_U32 u32DBCDataLen);
void    MDrv_DLC_DBC_AdjustYCGain(void);
void    MDrv_DLC_DBC_YCGainInit(MS_U8 u8YGain_M, MS_U8 u8YGain_L, MS_U8 u8CGain_M, MS_U8 u8CGain_L, MS_U8 u8YGain_H, MS_U8 u8CGain_H);

#ifdef __cplusplus
}
#endif

#endif
