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
//==============================================================================
// [mhal_adc.h]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#ifndef MHAL_ADC_H
#define MHAL_ADC_H


//#include "hwreg_utility.h"
#include "MsCommon.h"

#define ADC_SOG_FILTER_THRSHLD      40

#define ADC_BANDWIDTH_CLK_THRSHLD0    6 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD1    14 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD2    15 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD3    16 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD4    17 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD5    18 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD6    20 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD7    21 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD8    23 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD9    25 // 20/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD10    30 // 50/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD11    60 // 100/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD12    120 // 200/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD13    150 // 300/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD14    190 // 400/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD15    260 // 500/0.75

#define ADC_REFERENCE_VOLATAGE_LDO      0
#define ADC_REFERENCE_VOLATAGE_BANDGAP  1

#define ADC_IDAC_CURRENT_MODE_FULL 0
#define ADC_IDAC_CURRENT_MODE_HALF 1
#define ADC_IDAC_CURRENT_MODE_QUART 2

#define ADC_ADCX_ENABLE
/******************************************************************************/
/*                   Function Prototypes                     */
/******************************************************************************/

void Hal_ADC_set_cvbs_out(void *pInstance, E_ADC_CVBSOUT_TYPE e_cvbs_out_type);
MS_BOOL Hal_ADC_is_cvbs_out_enabled(void *pInstance, E_MUX_OUTPUTPORT outputPort);
void Hal_ADC_clk_gen_setting(void *pInstance, ADC_Gen_Clock_Type clocktype);
void Hal_ADC_set_mux(void *pInstance, E_MUX_INPUTPORT port);
void Hal_ADC_set_CVBSOMux(void *pInstance, ADC_CVBSOPortNumber CVBSOPortNumber, E_MUX_INPUTPORT port);

void Hal_ADC_gainoffset_reset(void *pInstance);
void Hal_ADC_offset_setting(void *pInstance, XC_AdcGainOffsetSetting *pstADCSetting);
void Hal_ADC_gain_setting(void *pInstance, XC_AdcGainOffsetSetting *pstADCSetting);
void Hal_ADC_ExitExternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE eADC_Source,XC_AdcGainOffsetSetting* InitialGainOffset);
MS_BOOL Hal_ADC_InitExternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE enAdcSource);
void Hal_ADC_InitInternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE enAdcSource, SCALER_WIN eWindow);
void Hal_ADC_ext_clk_en(void *pInstance, MS_BOOL benable);
void Hal_ADC_hdmi_vco_ctrl(void *pInstance, MS_U16 u16InputClock);
void Hal_ADC_vco_ctrl(void *pInstance, MS_U16 u16InputClock);
void Hal_XC_ADC_Set_VClamp_level(void *pInstance, ADC_VClamp_Level_Type type);
void Hal_ADC_set_vco_ctrl(void *pInstance, MS_BOOL bIsDVIPort, MS_U16 u16InputClock);


void Hal_ADC_sog_filter_en(void *pInstance, MS_BOOL bEnable);
void Hal_ADC_reset(void *pInstance, MS_U16 u16Reset);

void Hal_ADC_dtop_internaldc_setting(void *pInstance, ADC_Internal_Voltage InternalVoltage);

//  Set RGB Gain setting
void Hal_ADC_SetGainR(void *pInstance, MS_U16 u16value);
void Hal_ADC_SetGainG(void *pInstance, MS_U16 u16value);
void Hal_ADC_SetGainB(void *pInstance, MS_U16 u16value);
//  Get RGB Gain setting
MS_U16 Hal_ADC_GetGainR(void *pInstance);
MS_U16 Hal_ADC_GetGainG(void *pInstance);
MS_U16 Hal_ADC_GetGainB(void *pInstance);

//  Set RGB Offset setting
void Hal_ADC_SetOffsetR(void *pInstance, MS_U16 u16value);
void Hal_ADC_SetOffsetG(void *pInstance, MS_U16 u16value);
void Hal_ADC_SetOffsetB(void *pInstance, MS_U16 u16value);
//  Get RGB Offset setting
MS_U16 Hal_ADC_GetOffsetR(void *pInstance);
MS_U16 Hal_ADC_GetOffsetG(void *pInstance);
MS_U16 Hal_ADC_GetOffsetB(void *pInstance);

void Hal_ADC_dtop_calibration_target_setting(void *pInstance, MS_BOOL bIsYPbPrFlag);
void Hal_ADC_dtop_sw_mode_setting(void *pInstance, MS_BOOL bEnable, MS_BOOL bIsAutoSWMode);

void Hal_ADC_reset(void *pInstance, MS_U16 u16Reset);
void Hal_ADC_Set_Source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType, E_MUX_INPUTPORT* enInputPortType, MS_U8 u8PortCount);
void Hal_ADC_dtop_clk_setting ( void *pInstance, MS_U16 u16Value );
void Hal_ADC_hpolarity_setting(void *pInstance, MS_BOOL bHightActive);
void Hal_ADC_set_phase(void *pInstance, MS_U8 u8Value);
void Hal_ADC_set_phaseEx(void *pInstance, MS_U16 u16Value);
MS_U16 Hal_ADC_get_phase_range(void *pInstance);
MS_U8 Hal_ADC_get_phase(void *pInstance);
MS_U16 Hal_ADC_get_phaseEx(void *pInstance);
void Hal_ADC_Set_Source_Calibration(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType);
void Hal_ADC_clamp_placement_setting(void *pInstance, MS_U16 u16InputClockMHz);
void Hal_ADC_dvi_setting(void *pInstance);
void Hal_ADC_poweroff(void *pInstance);
void Hal_XC_ADC_poweron_source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType);

void Hal_ADC_set_mode(void *pInstance, ADC_INPUTSOURCE_TYPE enADCInput, MS_U16 u16PixelClockPerSecond, MS_U16 u16HorizontalTotal, MS_U16 u16SamplingRatio);
void Hal_ADC_SCART_RGB_setMode(void *pInstance, MS_BOOL bEnable);
void Hal_ADC_SourceSwitch(void *pInstance, MS_BOOL bSwitch);

void Hal_ADC_auto_adc_backup(void *pInstance, SCALER_WIN eWindow);
void Hal_ADC_auto_adc_restore(void *pInstance, SCALER_WIN eWindow);
void Hal_ADC_disable_csc(void *pInstance, SCALER_WIN eWindow);

MS_BOOL Hal_ADC_is_scart_rgb(void *pInstance);
MS_U16 Hal_ADC_get_clk (void *pInstance);
void Hal_ADC_get_SoG_LevelRange(void *pInstance, MS_U32 *u32Min, MS_U32 *u32Max, MS_U32 *u32Recommend_value);
void Hal_ADC_set_SoG_Level(void *pInstance, MS_U32 u32Value);
MS_BOOL Hal_ADC_set_SoG_Calibration(void *pInstance);

void Hal_ADC_set_RGB_PIPE_Delay(void *pInstance, MS_U8 u8Value);
void Hal_ADC_set_ScartRGB_SOG_ClampDelay(void *pInstance, MS_U16 u16Clpdly, MS_U16 u16Caldur);
void Hal_ADC_set_YPbPrLooseLPF(void *pInstance, MS_BOOL benable);
void Hal_ADC_Set_SOGBW(void *pInstance, MS_U16 u16value);

void Hal_ADC_dtop_iClampDuration_setting(void *pInstance, MS_U16 u16value);
void Hal_ADC_dtop_vClampDuration_setting(void *pInstance, MS_U16 u16value);

void Hal_ADC_Enable_HWCalibration(void *pInstance, MS_BOOL bEnable);
MS_BOOL Hal_ADC_SetCalibrationRefSource(void *pInstance, MS_U16 type);
MS_U16 Hal_ADC_GetCalibrationRefSource(void *pInstance);

void Hal_ADC_ISOG_Enable(void *pInstance, MS_BOOL bEnable);
MS_BOOL Hal_ADC_ISOG_CheckEnabled(void *pInstance);
void Hal_ADC_ISOG_SetResetWidth(void *pInstance, MS_U8 u8value);

MS_BOOL Hal_ADC_SetIdacCurrentMode(void *pInstance, MS_U16 u16mode);
MS_U16 Hal_ADC_GetIdacCurrentMode(void *pInstance);
#endif // MHAL_ADC_H

