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
//==============================================================================
#ifndef MHAL_WBLE_H
#define MHAL_WBLE_H




void Hal_WBLE_init_riu_base(MS_VIRT u32riu_base);
void Hal_WBLE_set_ble(MS_BOOL bEn);
void Hal_WBLE_set_wle(MS_BOOL bEn);
void Hal_WBLE_set_ble_start(MS_U8 u8val);
void Hal_WBLE_set_ble_slop(MS_U8 u8val);
void Hal_WBLE_set_wle_start(MS_U8 u8val);
void Hal_WBLE_set_wle_slop(MS_U8 u8val);

MS_U8 Hal_WBLE_get_sw_ble_method(void);
MS_U8 Hal_WBLE_get_black_start(void);
MS_U8 Hal_WBLE_get_black_slop(void);
MS_U8 Hal_WBLE_get_ble_stregth(void);
MS_U8 Hal_WBLE_get_ble_step(void);
MS_U8 Hal_WBLE_get_ble_flickalpha(void);

MS_U16 Hal_WBLE_get_ble_delta(void);
MS_U16 Hal_WBLE_get_ble_slop_boundary(void);
MS_U16 Hal_WBLE_get_ble_start_boundary(void);


#endif
