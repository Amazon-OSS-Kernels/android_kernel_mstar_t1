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
/// @file    drvPQ_Datatypes.h
/// @brief  PQ interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVPQ_DATATYPES_H_
#define _DRVPQ_DATATYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    MS_422To444_ON,
    MS_422To444_OFF,
    MS_422To444_SIMPLE,
    MS_422To444_BICUBIC,
    MS_422To444_BYPASS,
    MS_422To444_NUMS
}MS_422To444_TYPE;

typedef enum
{
    MS_420_CUP_ON,
    MS_420_CUP_OFF,
    MS_420_CUP_NUM,
}MS_420_CUP_TYPE;

typedef enum
{
    MS_HNONLINEAR_OFF,
    MS_HNONLINEAR_1920_0,
    MS_HNONLINEAR_1920_1,
    MS_HNONLINEAR_1920_2,
    MS_HNONLINEAR_1680,
    MS_HNONLINEAR_1440,
    MS_HNONLINEAR_1366_0,
    MS_HNONLINEAR_1366_1,
    MS_HNONLINEAR_1366_2,
    MS_HNONLINEAR_3840_0,
    MS_HNONLINEAR_3840_1,
    MS_HNONLINEAR_3840_2,
    MS_HNONLINEAR_NUM,
}MS_HNONLINEAR_TYPE;

typedef enum
{
    MS_MADI_24_4R,
    MS_MADI_24_2R,
    MS_MADI_25_4R_MC,
    MS_MADI_25_4R,
    MS_MADI_25_2R,
    MS_MADI_26_4R,
    MS_MADI_26_2R,
    MS_MADI_27_4R,
    MS_MADI_27_2R,
    MS_MADI_P_MODE8,
    MS_MADI_P_MODE10,
    MS_MADI_P_MODE_MOT10,
    MS_MADI_P_MODE_MOT8,
    MS_MADI_24_RFBL_NFILM,
    MS_MADI_24_RFBL_FILM,
    MS_MADI_FBL_DNR,
    MS_MADI_FBL_MIU,
    MS_MADI_P_MODE8_NO_MIU,
    MS_MADI_24_4R_880,
    MS_MADI_24_2R_880,
    MS_MADI_25_4R_MC_NW,
    MS_MADI_25_6R_MC,
    MS_MADI_25_14F_6R_MC,    //Add New MADi mode
    MS_MADI_25_4R_884,
    MS_MADI_25_4R_880,
    MS_MADI_25_2R_884,
    MS_MADI_25_2R_880,
    MS_MADI_25_6R_MC_NW,
    MS_MADI_25_12F_8R_MC,
    MS_MADI_25_14F_8R_MC,
    MS_MADI_25_16F_8R_MC,
    MS_MADI_25_6R_FilmPreDet,
    MS_MADI_24_6R_FilmPreDet,
    MS_MADI_25_6R_FilmPreDet_PAL,
    MS_MADI_24_6R_FilmPreDet_PAL,
    MS_MADI_RFBL_2D,
    MS_MADI_RFBL_25D,
    MS_MADI_RFBL_3D,
    MS_MADI_RFBL_3D_YC,
    MS_MADI_RFBL_3D_FILM,
    MS_MADI_P_MODE8_444,
    MS_MADI_P_MODE10_444,
    MS_MADI_RFBL_P_MODE10_MOT,
    MS_MADI_RFBL_P_MODE8_MOT,
    MS_MADI_P_MODE_MOT10_8Frame,
    MS_MADI_P_MODE_MOT10_4Frame,
    MS_MADI_P_MODE_MC_3Frame,
    MS_MADI_P_MODE_MC_4Frame,
    MS_MADI_P_MODE_MC_6Frame_6R,
    MS_MADI_P_MODE_MC_12Frame_8R,
    MS_MADI_P_MODE_MC_8Frame_6R,
    MS_MADI_P_MODE_MC_16Frame_8R,
    MS_MADI_25_8F_4R_MC,
    MS_MADI_25_16F_6R_MC,
    MS_MADI_P_MODE8_2BYTE_3D,
    MS_MADi_25_8F_8R_MC,
    MS_MADI_RFBL_MC,
    MS_MADI_RFBL_P_MODE_MC,
    MS_MADI_P_MODE_P_MC_12Frame_8R,
    MS_MADI_P_MODE_N_MC_12Frame_8R,
    MS_MADI_25_6F_4R_MC_I,
    MS_MADI_25_6F_4R_MC_P,
    MS_MADi_25_4R_I,
    MS_MADi_25_4R_P,
    MS_MADi_25_4R_M_MC,
    MS_MADi_25_16F_8R_M_MC,
    MS_MADi_25_16F_6R_M_MC,
    MS_MADi_P_MODE8_M_444,
    MS_MADi_P_MODE_MC_M_4F,
    MS_MADi_P_MODE10_M_444,
    MS_MADi_P_MODE8_M,
    MS_MADi_P_MODE10_M,
    MS_MADi_25_4R_M,
    MS_MADi_P_MODE_MOT10_M,
    MS_MADi_P_MODE_MOT8_M,
    MS_MADi_P_MODE_MOT10_M_4K,
    MS_MADi_P_MODE10_M_444_4K,
    MS_MADi_P_MODE_MC_12Frame_8R_M,
    MS_MADi_P_MODE_MC_8Frame_8R_M,
    MS_MADI_P_MODE_MC_6Frame,
    MS_MADi_24_8F_4R_M_MC,
    MS_MADi_P_MODE8_M_4K,
    MS_MADi_P_MODE8_M_4K_60,
    MS_MADi_P_MODE8_M_4F,
    MS_MADi_24_4R_M,
    MS_MADi_P_MODE_MOT10_NR_Yonly_M,
    MS_MADi_P_MODE_MOT10_NO_DNR,
    MS_MADi_25_4R_720i_Main,
    MS_MADi_25_6F_4R_MC,
    MS_MADi_P_MODE_MC_4Frame_Yonly_Main,
    MS_MADI_NUM,
}MS_MADI_TYPE;

typedef enum
{
    MS_VD_SAMPLING_STANDARD,
    MS_VD_SAMPLING_1135,
    MS_VD_SAMPLING_1135_MUL_15,
    MS_VD_SAMPLING_NUM,
}MS_VD_SAMPLING_TYPE;

typedef enum
{
    MS_PQ_IP_VD_SAMPLING,
    MS_PQ_IP_HSD_SAMPLING,
    MS_PQ_IP_ADC_SAMPLING,
    MS_PQ_IP_NUM,
}MS_PQ_IP_TYPE;

typedef enum
{
    MS_ADC_SAMPLING_X_1,
    MS_ADC_SAMPLING_X_2,
    MS_ADC_SAMPLING_X_4,
    MS_ADC_SAMPLING_NUM,
}MS_ADC_SAMPLING_TYPE;

typedef enum
{
    MS_CSC_IP_CSC,
    MS_CSC_IP_VIP_CSC,
    MS_CSC_IP_NUMs,
}MS_CSC_IP_TYPE;

typedef enum
{
    MS_HSD_SAMPLING_Div_1_000,
    MS_HSD_SAMPLING_Div_1_125,
    MS_HSD_SAMPLING_Div_1_250,
    MS_HSD_SAMPLING_Div_1_375,
    MS_HSD_SAMPLING_Div_1_500,
    MS_HSD_SAMPLING_Div_1_625,
    MS_HSD_SAMPLING_Div_1_750,
    MS_HSD_SAMPLING_Div_1_875,
    MS_HSD_SAMPLING_Div_2_000,
    MS_HSD_SAMPLING_NUM,
}MS_HSD_SAMPLING_TYPE;

#ifdef __cplusplus
}
#endif

#endif /* _DRVPQ_DATATYPES_H_ */
