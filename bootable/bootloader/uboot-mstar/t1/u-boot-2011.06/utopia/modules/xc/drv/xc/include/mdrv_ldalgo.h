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
/// file    Mdrv_ldalgo.h
/// @brief  local dimming algorithm
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_LDALGO_H_
#define _MDRV_LDALGO_H_

#include "MsCommon.h"
//#include "mhal_xc_chip_config.h"

#ifdef _MDRV_LDALGO_C_
    #define INTERFACE
#else
    #define INTERFACE extern
#endif

typedef enum
{
    E_LD_DUMP_NONE                    = 0x00, // none dummp
    E_LD_DUMP_PWM_Block               = 0x01, // dummp pwm block
    E_LD_DUMP_Gamma_Block             = 0x02, // dummp gamma block
    E_LD_DUMP_VDown_Block             = 0x03, // dummp vdown block
} EnuLDDumpStage;

typedef struct __attribute__((packed)) _TYPE_LD_PANEL_TYPE_t
{
    MS_U8 u8PWMHigh;
    MS_U8 u8PWMWidth;
    MS_U8 u8LDWidth;
    MS_U8 u8LDHeight;
    MS_U16 u16PWMSize;
    MS_U16 u16BLSize;
    MS_U8 u8LD_Mode2D3D;
    MS_U8 u8LD_Mode2D3D_tmax;
}ST_TYPE_LD_PANEL_TYPE;

#define REG_DEBUG_LD_DUMP_SEL        SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_7A_L,0xF000) >>12

#if  (LD_ENABLE==1)
//-------------------------------------------------------------------------------------------------
/// set LD enable flags
/// @param u8LDEnable            \b IN: 0 off ; 1 on
/// @return  void                \b IN: none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_EnableLD(void *pInstance, MS_BOOL bLDEnable);
//-------------------------------------------------------------------------------------------------
/// set LD gamma control
/// @param  u8GammaControl        \b IN: gamma control index
/// @return  void                \b IN: none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_SetGammaControl(void *pInstance, MS_U8 u8GammaControl);
//-------------------------------------------------------------------------------------------------
/// set LD ISR
/// @param  void                \b IN: none
/// @return  void                \b IN: none
//-------------------------------------------------------------------------------------------------
void MDrv_XC_LD_ISR(void* pInstance);

//-------------------------------------------------------------------------------------------------
/// Get LD value
/// @param pu8LDValue    @ref MS_U8      \b OUT: get the LD back light value (The amount is u8WSize x u8HSize)
/// @param u8WSize       @ref MS_U8      \b IN: set the LD back light width
/// @param u8HSize       @ref MS_U8      \b IN: set the LD back light height
/// @Return TRUE: Successful FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL  MDrv_LD_Get_Value(void *pInstance, MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize );

#endif

#undef INTERFACE


#endif

