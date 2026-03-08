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

#ifndef _DRV_MUX_H_
#define _DRV_MUX_H_


//------------------------------
// Mux related
//------------------------------
void MDrv_XC_mux_dispatch(void *pInstance, E_MUX_INPUTPORT src , E_MUX_OUTPUTPORT dest);
void MDrv_XC_mux_set_sync_port_by_dataport(void *pInstance, E_MUX_INPUTPORT src_port );

void MDrv_XC_mux_turnoff_cmux(void *pInstance);
void MDrv_XC_mux_turnoff_ymux(void *pInstance);

void MDrv_SC_EnableCLK_for_DIP(void *pInstance, MS_BOOL bEnable);

E_MUX_INPUTPORT MDrv_XC_Mux_GetHDMIPort(void *pInstance, INPUT_SOURCE_TYPE_t src );
MS_U8 MDrv_XC_MUX_MapInputSourceToVDYMuxPORT(void *pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType );

void MDrv_XC_mux_SetScalerIndexInformationToADC(void *pInstance, MS_U8 u8ScalerIndex, MS_U16 u16InputSourceType);

#endif

