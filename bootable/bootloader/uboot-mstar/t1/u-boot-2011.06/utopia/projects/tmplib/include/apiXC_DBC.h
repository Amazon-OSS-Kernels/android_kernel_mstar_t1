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
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiXC_Dlc.h
/// @brief  DLC API layer Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Video Video modules

 *  \defgroup XC_BE XC_BE modules 
 *  \ingroup Video
 
    XC_BE is used for\n
    1. Dynamic Backlight Curve : Calculate the DBC base on average value.\n
    For more information,see \link DBC DBC interface (apiXC_DBC.h) \endlink

 *  \defgroup DBC DBC interface (apiXC_DBC.h) 
 *  \ingroup XC_BE     
 
    - Dynamic Backlight Curve : Calculate the DBC base on average value.

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html apiXC_DLC_DBC_init_flow.png

 *  \defgroup DBC_INIT DBC init control 
 *  \ingroup DBC      

 *! \defgroup DBC_FEATURE DBC feature operation
 *  \ingroup DBC     

 *! \defgroup DBC_INFO DBC Infomation pool
 *  \ingroup DBC     

 *! \defgroup DBC_ToBeModified DBC api to be modified 
 *  \ingroup DBC     

 *! \defgroup DBC_ToBeRemove DBC api to be removed 
 *  \ingroup DBC 
 */


#ifndef _API_XC_DBC_H_
#define _API_XC_DBC_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
/// DLC DBC initilization
/// @ingroup DBC_INIT
// @param DLC_DBC_MFinit               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_Init(XC_DLC_DBC_MFinit DLC_DBC_MFinit);

//-------------------------------------------------------------------------------------------------
/// DBC initilization
/// @ingroup DBC_INIT
//-------------------------------------------------------------------------------------------------
void MApi_XC_DBC_Init(void);

//-------------------------------------------------------------------------------------------------
/// DLC DBC set status
/// @ingroup DBC_FEATURE
// @param bDBCEnable               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_Setstatus(MS_BOOL bDBCEnable);

//-------------------------------------------------------------------------------------------------
/// DLC DBC get status
/// @ingroup DBC_INFO
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_DBC_Getstatus(void);

//-------------------------------------------------------------------------------------------------
/// Set DBC Ready Mode 
/// @ingroup DBC_FEATURE
// @param bDBCReady               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_DBC_SetReady(MS_BOOL bDBCReady);

//-------------------------------------------------------------------------------------------------
/// Set DLC DBC Debug Mode 
/// @ingroup DBC_FEATURE
// @param ucDBC_DebugMode               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_DBC_SetDebugMode(MS_U8 ucDBC_DebugMode);

//-------------------------------------------------------------------------------------------------
/// DLC DBC Debug Mode Status
/// @ingroup DBC_INFO
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_DLC_DBC_GetDebugMode(void);

//-------------------------------------------------------------------------------------------------
/// DLC DBC update PWM
/// @ingroup DBC_FEATURE
/// @param   u8PWMvalue           \b IN: PWM value to set the lumming
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_UpdatePWM(MS_U8 u8PWMvalue);

//-------------------------------------------------------------------------------------------------
/// DLC DBC handler
/// @ingroup DBC_FEATURE
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16  MApi_XC_DLC_DBC_Handler(void);

//-------------------------------------------------------------------------------------------------
/// DLC DBC adjsut YC gain
/// @ingroup DBC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_AdjustYCGain(void);

//-------------------------------------------------------------------------------------------------
/// DLC DBC YC gain initilization
/// @ingroup DBC_INIT
// @param u8YGain_M               \b IN: TBD
// @param u8YGain_L               \b IN: TBD
// @param u8CGain_M               \b IN: TBD
// @param u8CGain_L               \b IN: TBD
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_YCGainInit(MS_U8 u8YGain_M, MS_U8 u8YGain_L, MS_U8 u8CGain_M, MS_U8 u8CGain_L);

//-------------------------------------------------------------------------------------------------
/// DLC DBC reset
/// @ingroup DBC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_DBC_Reset(void);

//-------------------------------------------------------------------------------------------------
/// @brief callback functions & struct used to support debug command.
//-------------------------------------------------------------------------------------------------
typedef void   (*MApi_XC_DBC_OnOff_Callback)(MS_BOOL/*On/Off*/);

//-------------------------------------------------------------------------------------------------
/// @brief callback functions & struct used to support debug command.
//-------------------------------------------------------------------------------------------------
typedef void   (*MApi_XC_DBC_AdjustBackLight_Callback)(MS_U32 /*PWM_VALUE*/);

//-------------------------------------------------------------------------------------------------
/// @brief structure used to support debug command.
//-------------------------------------------------------------------------------------------------
typedef struct {
    MS_U8 *pCmdBuff;                                            ///<1. command buffer from UART
    MApi_XC_DBC_OnOff_Callback           fnDBC_AP_OnOff;        ///<2. Function to control DBC on/off
    MApi_XC_DBC_AdjustBackLight_Callback fnDBC_AdjustBacklight; ///<3. Functoin to control backlight
    MApi_XC_DLC_Print_Callback           fnDBC_Putchar;         ///<4. User putchar function
}tDBC_CONTROL_PARAMS;

//-------------------------------------------------------------------------------------------------
/// @brief uart debug command decoder for Dynamic Backlight Control.
/// @ingroup DBC_FEATURE
/// @param params   passin params.  
//-------------------------------------------------------------------------------------------------
void MApi_XC_DBC_DecodeExtCmd(tDBC_CONTROL_PARAMS *params );

#ifdef __cplusplus
}
#endif

#endif // _API_XC_DBC_H_
