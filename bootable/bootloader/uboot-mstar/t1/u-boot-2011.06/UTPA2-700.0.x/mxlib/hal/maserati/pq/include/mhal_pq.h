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
#ifndef MHAL_PQ_H
#define MHAL_PQ_H

#define MEMFMT24BIT_YC10BIT_MOT     0xBB
#define MEMFMT24BIT_YC8BIT_MOT      0xAA
#define MEMFMT24BIT_YC10BIT         0x99
#define MEMFMT24BIT_YC8BIT          0x88
#define MEMFMT24BIT_CLEAR           0x00

#define SC_FILTER_Y_SRAM1               0x00
#define SC_FILTER_Y_SRAM2               0x01
#define SC_FILTER_Y_SRAM3               0x02
#define SC_FILTER_Y_SRAM4               0x03
#define SC_FILTER_C_SRAM1               0x04
#define SC_FILTER_C_SRAM2               0x05
#define SC_FILTER_C_SRAM3               0x06
#define SC_FILTER_C_SRAM4               0x07
#define SC_FILTER_SRAM_COLOR_INDEX      0x08
#define SC_FILTER_SRAM_COLOR_GAIN_SNR   0x09
#define SC_FILTER_SRAM_COLOR_GAIN_DNR   0x0A
#define SC_FILTER_SRAM_ICC_CRD          0x0B
#define SC_FILTER_SRAM_IHC_CRD          0x0C
#define SC_FILTER_SRAM_XVYCC_DE_GAMMA   0x0D
#define SC_FILTER_SRAM_XVYCC_GAMMA      0x0E

#define SUPPORT_SCMI_V2     TRUE
#define SUPPORT_DYNAMIC_MEM_FMT TRUE

#define PQ_SUPPORT_DEVICE_NUM 1

#define Y2Y_709_601_VERSION 1

#define REG_CHIP_REVISION           0x1ECF

#define PQ_CFD_INFO

void Hal_PQ_init_riu_base(void *pInstance,MS_U32 u32riu_base);
MS_U8 Hal_PQ_get_sync_flag(void *pInstance,MS_BOOL bMainWin);
MS_U8 Hal_PQ_get_input_vsync_value(void *pInstance,MS_BOOL bMainWin);
MS_U8 Hal_PQ_get_output_vsync_value(void *pInstance,MS_BOOL bMainWin);
MS_U8 Hal_PQ_get_input_vsync_polarity(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_memfmt_doublebuffer(void *pInstance,MS_BOOL bEn);
void Hal_PQ_set_mem_fmt(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val, MS_U16 u16Mask);
void Hal_PQ_set_mem_fmt_en(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val, MS_U16 u16Mask);
void Hal_PQ_set_420upSample(void *pInstance,MS_U16 u16value);
void Hal_PQ_set_force_y_motion(void *pInstance,MS_BOOL bMainWin, MS_U16 u16value);
MS_U8 Hal_PQ_get_force_y_motion(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_force_c_motion(void *pInstance,MS_BOOL bMainWin, MS_U16 u16value);
MS_U8 Hal_PQ_get_force_c_motion(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_dipf_temporal(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val);
MS_U16 Hal_PQ_get_dipf_temporal(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_dipf_spatial(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val);
MS_U8 Hal_PQ_get_dipf_spatial(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_vsp_sram_filter(void *pInstance,MS_U8 u8vale);
MS_U8 Hal_PQ_get_vsp_sram_filter(void *pInstance);
void Hal_PQ_set_dnr(void *pInstance,MS_BOOL bMainWin, MS_U8 u8val);
MS_U8 Hal_PQ_get_dnr(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_presnr(void *pInstance,MS_BOOL bMainWin, MS_U8 u8val);
MS_U8 Hal_PQ_get_presnr(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_film(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val);
MS_U8 Hal_PQ_get_film(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_c_sram_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_y_sram_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_c_sram_table_Main2(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_y_sram_table_Main2(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_sram_color_index_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_sram_color_gain_snr_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_sram_color_gain_dnr_table(void *pInstance,MS_U8 u8sramtype, void *pData);
MS_U16 Hal_PQ_get_hnonlinear_idx(void *pInstance,MS_HNONLINEAR_TYPE etype);
MS_U8 Hal_PQ_get_madi_fbl_mode(void *pInstance,MS_BOOL bMemFmt422,MS_BOOL bInterlace);
MS_U16 Hal_PQ_get_sram_size(void *pInstance,MS_U16 u16sramtype);
void Hal_PQ_OPWriteOff_Enable(void *pInstance,MS_BOOL bEna);
void Hal_PQ_set_sram_ihc_crd_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_sram_icc_crd_table(void *pInstance,MS_U8 u8sramtype, void *pData);
MS_U16 Hal_PQ_get_rw_method(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_rw_method(void *pInstance,MS_BOOL bMainWin, MS_U16 u16method);
void Hal_PQ_set_xvycc_matrix_coefficient(void *pInstance,MS_S32 pData[][3], MS_BOOL bMainWin);
void Hal_PQ_set_xvycc_matrix_enable(void *pInstance,MS_BOOL bEnable, MS_BOOL bMainWin);
MS_BOOL HAL_PQ_user_mode_control(void *pInstance);
void HAL_PQ_set_SelectCSC(void *pInstance,MS_U16 u16selection, MS_BOOL  bMainWin);
void Hal_PQ_set_sram_xvycc_gamma_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_UFSC_c_sram_table(void *pInstance, MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_UFSC_y_sram_table(void *pInstance, MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_UFSC_sram_ihc_crd_table(void *pInstance, MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_UFSC_sram_icc_crd_table(void *pInstance, MS_U8 u8sramtype, void *pData);
MS_BOOL Hal_PQ_get_cfd_info(MS_U16 u16PqIndex, MS_U8 *pu8ColorFormat, MS_U8 *pu8ColorDataFormat, MS_BOOL *pbIsFullRange, MS_U8 *pu8ColorPrimaries, MS_U8 *pu8TransferCharacteristics, MS_U8 *pu8MatrixCoefficients);
#endif


