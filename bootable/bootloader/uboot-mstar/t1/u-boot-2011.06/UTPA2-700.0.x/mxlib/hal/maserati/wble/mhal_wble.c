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
#define MHAL_WBLE_C

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "MsTypes.h"

// Internal Definition
#include "hwreg_wble.h"
#include "wble_hwreg_utility2.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U32 WBLE_RIU_BASE;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
// Put this function here because hwreg_utility2 only for hal.
void Hal_WBLE_init_riu_base(MS_VIRT u32riu_base)
{
    WBLE_RIU_BASE = u32riu_base;
}


MS_U8 Hal_WBLE_get_black_start(void)
{
    MS_U8 u8val = SC_R2BYTE(REG_SC_BK1A_10_L) & 0x00FF;
    return u8val;
}

MS_U8 Hal_WBLE_get_black_slop(void)
{
    MS_U8 u8val = (SC_R2BYTE(REG_SC_BK1A_10_L) & 0xFF00)>>8;
    return u8val;
}

void Hal_WBLE_set_ble(MS_BOOL bEn)
{
    if(bEn)
        SC_W2BYTEMSK(REG_SC_BK1A_08_L, 0x40, 0x40);
    else
        SC_W2BYTEMSK(REG_SC_BK1A_08_L, 0x00, 0x40);
}

void Hal_WBLE_set_wle(MS_BOOL bEn)
{
    if(bEn)
        SC_W2BYTEMSK(REG_SC_BK1A_08_L, 0x20, 0x20);
    else
        SC_W2BYTEMSK(REG_SC_BK1A_08_L, 0x00, 0x20);
}


void Hal_WBLE_set_ble_start(MS_U8 u8val)
{
    MS_U16 u16val;
    u16val = u8val;
    SC_W2BYTEMSK(REG_SC_BK1A_10_L, u16val, 0x00FF);
}


void Hal_WBLE_set_ble_slop(MS_U8 u8val)
{
    MS_U16 u16val;
    u16val = (MS_U16)u8val<<8;
    SC_W2BYTEMSK(REG_SC_BK1A_10_L, u16val, 0xFF00);
}

void Hal_WBLE_set_wle_start(MS_U8 u8val)
{
    MS_U16 u16val;
    u16val = u8val;
    SC_W2BYTEMSK(REG_SC_BK1A_11_L, u16val, 0x00FF);
}


void Hal_WBLE_set_wle_slop(MS_U8 u8val)
{
    MS_U16 u16val;
    u16val = (MS_U16)u8val<<8;
    SC_W2BYTEMSK(REG_SC_BK1A_11_L, u16val, 0xFF00);
}

MS_U8 Hal_WBLE_get_sw_ble_method(void)
{
    MS_U8 u8val = SC_R2BYTE(REG_SC_BK1A_20_L) & 0x00FF;
    return u8val;
}

MS_U8 Hal_WBLE_get_ble_stregth(void)
{
    MS_U8 u8val = (SC_R2BYTE(REG_SC_BK1A_20_L) & 0xFF00)>>8;
    return u8val;
}

MS_U8 Hal_WBLE_get_ble_step(void)
{
    MS_U8 u8val = (SC_R2BYTE(REG_SC_BK1A_23_L) & 0xFF00)>>8;
    return u8val;
}

MS_U8 Hal_WBLE_get_ble_flickalpha(void)
{
    MS_U8 u8val = SC_R2BYTE(REG_SC_BK1A_23_L) & 0x00FF;
    return u8val;
}


MS_U16 Hal_WBLE_get_ble_slop_boundary(void)
{
    return SC_R2BYTE(REG_SC_BK1A_21_L);
}

MS_U16 Hal_WBLE_get_ble_start_boundary(void)
{
    return SC_R2BYTE(REG_SC_BK1A_22_L);
}

MS_U16 Hal_WBLE_get_ble_delta(void)
{
    return (SC_R2BYTE(REG_SC_BK1A_24_L) & 0xFF00);
}



