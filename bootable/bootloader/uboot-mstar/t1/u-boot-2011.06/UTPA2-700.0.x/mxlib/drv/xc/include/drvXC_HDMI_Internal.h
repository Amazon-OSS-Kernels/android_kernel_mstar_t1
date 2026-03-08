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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvXC_HDMI_Internal.h
/// @brief  Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVXC_HDMI_INTERNAL_H_
#define _DRVXC_HDMI_INTERNAL_H_

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

#include "drvXC_HDMI_if.h"

#ifdef _DRV_HDMI_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//HDMI
INTERFACE MS_U8 MDrv_HDMI_avi_infoframe_info(MS_U8 u8byte);
INTERFACE MS_U16 MDrv_HDMI_pll_ctrl1(HDMI_PLL_CTRL_t pllctrl, MS_BOOL bread, MS_U16 u16value);
INTERFACE MS_U16 MDrv_HDMI_GetHDE(void);
INTERFACE void MDrv_DVI_Accumulator_Monitor(void);

//HDCP

//DVI
INTERFACE MS_BOOL   MDrv_HDMI_dvi_clock_detect(E_MUX_INPUTPORT enInputPortType);

//**Not use**
INTERFACE void MDrv_HDMI_hpd_en(MS_BOOL benable);
INTERFACE MS_BOOL MDrv_HDCP_isexist(void);

INTERFACE MS_U8 MDrv_HDMI_audio_cp_data_info(MS_U8 u8byte);
INTERFACE MS_U16 MDrv_HDMI_pll_ctrl2(HDMI_PLL_CTRL2_t pllctrl, MS_BOOL bread, MS_U16 u16value);
INTERFACE void  MDrv_DVI_IMMESWITCH_PS_SW_Path(void *pInstance);
INTERFACE void MDrv_HDMI_pkt_reset_internal(void *pInstance, HDMI_REST_t breset );
INTERFACE void MDrv_HDMI_PROG_DDCRAM_internal(void *pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo);
INTERFACE stHDMI_INITIAL_TABLE MDrv_HDMI_init_internal(void *pInstance);
INTERFACE void MDrv_HDCP_initproductionkey_internal(void *pInstance, MS_U8 * pu8HdcpKeyData );
INTERFACE void MDrv_HDMI_pullhpd_internal(void *pInstance, MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse);
INTERFACE void MDrv_DVI_HF_adjust_internal(void);
INTERFACE void MDrv_DVI_Accumulator_Monitor(void);

#undef INTERFACE
#endif // _DRV_ADC_INTERNAL_H_

