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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvPWM_v2.h
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_PWM_V2_H_
#define _DRV_PWM_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MDrv_CMD_PWM_Oen,
    MDrv_CMD_PWM_Period,
    MDrv_CMD_PWM_DutyCycle,
    MDrv_CMD_PWM_UnitDiv,
    MDrv_CMD_PWM_Div,
    MDrv_CMD_PWM_Polarity,
    MDrv_CMD_PWM_Vdben,
    MDrv_CMD_PWM_ResetEn,
    MDrv_CMD_PWM_Dben,
    MDrv_CMD_PWM_Shift,
    MDrv_CMD_PWM_Nvsync,
} ePwmIoctlOpt;


//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------


typedef struct _PWM_OEN_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bOenPWM;
}PWM_OEN_PARAM, *PPWM_OEN_PARAM;

typedef struct _PWM_PERIOD_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32PeriodPWM;
}PWM_PERIOD_PARAM, *PPWM_PERIOD_PARAM;

typedef struct _PWM_DUTYCYCLE_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32DutyPWM;
}PWM_DUTYCYCLE_PARAM, *PPWM_DUTYCYCLE_PARAM;

typedef struct _PWM_UNITDIV_PARAM
{
    MS_U16 u16UnitDivPWM;
}PWM_UNITDIV_PARAM, *PPWM_UNITDIV_PARAM;

typedef struct _PWM_DIV_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_U16 u16DivPWM;
}PWM_DIV_PARAM, *PPWM_DIV_PARAM;

typedef struct _PWM_POLARITY_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bPolPWM;
}PWM_POLARITY_PARAM, *PPWM_POLARITY_PARAM;

typedef struct _PWM_VDBEN_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bVdbenPWM;
}PWM_VDBEN_PARAM, *PPWM_VDBEN_PARAM;

typedef struct _PWM_RESETEN_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bRstPWM;
}PWM_RESETEN_PARAM, *PPWM_RESETEN_PARAM;

typedef struct _PWM_DBEN_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bDbenPWM;
}PWM_DBEN_PARAM, *PPWM_DBEN_PARAM;

typedef struct _PWM_SHIFT_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32DutyPWM;
}PWM_SHITF_PARAM, *PPWM_SHIFT_PARAM;

typedef struct _PWM_NVSYNC_PARAM
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bNvsPWM;
}PWM_NVSYNC_PARAM, *PPWM_NVSYNC_PARAM;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_BDMA_V2_H_

