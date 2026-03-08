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
#ifndef MHAL_PQ_CUS_H
#define MHAL_PQ_CUS_H

#define PQ_HAL_EWINDOW_PARAM 1

MS_U16 Hal_PQ_get_420_cup_idx(void *pInstance,MS_420_CUP_TYPE etype, PQ_WIN bMainWin);
MS_U16 Hal_PQ_get_422To444_idx(void *pInstance,MS_422To444_TYPE etype, PQ_WIN bMainWin);
MS_U8 Hal_PQ_get_422To444_mode(void *pInstance,MS_BOOL bMemFmt422);
//MS_U16 Hal_PQ_get_madi_idx(void *pInstance,MS_MADI_TYPE etype, PQ_WIN bMainWin);
MS_U16 Hal_PQ_get_madi_idx(void *pInstance,PQ_WIN bMainWin, MS_MADI_TYPE etype);
MS_U16 Hal_PQ_get_csc_ip_idx(void *pInstance,MS_CSC_IP_TYPE enCSC, PQ_WIN bMainWin);
MS_U16 Hal_PQ_get_ip_idx(void *pInstance,MS_PQ_IP_TYPE eType, PQ_WIN bMainWin);
MS_U16 Hal_PQ_get_hsd_sampling_idx(void *pInstance,MS_HSD_SAMPLING_TYPE eType, PQ_WIN bMainWin);
MS_U16 Hal_PQ_get_adc_sampling_idx(void *pInstance,MS_ADC_SAMPLING_TYPE eType, PQ_WIN bMainWin);

#endif


