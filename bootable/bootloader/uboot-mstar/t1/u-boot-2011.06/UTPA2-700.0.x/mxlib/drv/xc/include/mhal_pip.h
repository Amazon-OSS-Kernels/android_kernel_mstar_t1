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
// [mhal_sc.h]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#ifndef MHAL_PIP_H
#define MHAL_PIP_H
#ifndef DISABLE_PIP_FUNCTION

void Hal_SC_Sub_SetDisplayWindow_burst(void *pInstance, MS_WINDOW_TYPE *pstDispWin);
void Hal_SC_Sub_SetDisplayWindow(void *pInstance, MS_WINDOW_TYPE *pstDispWin);
void Hal_SC_subwindow_disable_burst(void *pInstance);
void Hal_SC_subwindow_disable(void *pInstance);
MS_BOOL Hal_SC_Is_subwindow_enable(void *pInstance);
void Hal_SC_set_border_format(void *pInstance, MS_U8 u8Left, MS_U8 u8Right, MS_U8 u8Up, MS_U8 u8Down, MS_U8 u8color, SCALER_WIN eWindow );
void Hal_SC_border_enable(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void Hal_SC_set_Zorder_main_first(void *pInstance, MS_BOOL bMainFirst);
void Hal_SC_set_Zorder_main_first_burst(void *pInstance, MS_BOOL bMainFirst);
void Hal_SC_SetPIP_FRC(void *pInstance, MS_BOOL enable, MS_U8 u8IgonreLinesNum);
void Hal_SC_set_Zextra_y_half(void *pInstance, MS_BOOL bInterlace);
void Hal_SC_set_Zextra_y_half_burst(void *pInstance, MS_BOOL bInterlace);
void Hal_SC_enable_extra_request(void *pInstance, MS_BOOL bEnable);
void Hal_SC_EnableCLK_for_SUB(void * pInstance, MS_BOOL bEnable);

#endif
#endif /* MHAL_SC_H */
