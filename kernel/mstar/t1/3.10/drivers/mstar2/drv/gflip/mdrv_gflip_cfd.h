/**
* Copyright (c) 2006-2019 MediaTek Inc.
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

#ifndef _MDRV_GFLIP_CFD_H
#define _MDRV_GFLIP_CFD_H

typedef struct _STU_CFDAPI_UI_CONTROL {
    MS_U32 u32Version;   /* <Version of current structure. Please always set to "CFD_HDMI_OSD_ST_VERSION" as input */
    MS_U16 u16Length;    /* <Length of this structure, u16Length=sizeof(STU_CFDAPI_UI_CONTROL) */

    /* 1x = 50 */
    /* range = [0-100] */
    MS_U16 u16Hue;

    /* 1x = 128 */
    /* range = [0-255] */
    MS_U16 u16Saturation;

    /* 1x = 1024 */
    /* range = [0-2047] */
    MS_U16 u16Contrast;

    MS_U8  u8ColorCorrection_En;

    /* 1x = 1024 */
    /* rnage = [-2048-2047] */
    MS_S32 s32ColorCorrectionMatrix[3][3];

    /* 1x = 1024 */
    /* range = [0-2047] */
    MS_U16 u16Brightness;

    /* 1x = 1024 */
    /* range = [0-2048] */
    /* u16RGBGGain[0] = Rgain */
    /* u16RGBGGain[1] = Ggain */
    /* u16RGBGGain[2] = Bgain */
    MS_U16 u16RGBGGain[3];

    /* 0:off */
    /* 1:on */
    /* default on , not in the document */
    MS_U8  u8OSD_UI_En;

    /* Mode 0: update matrix by OSD and color format driver */
    /* Mode 1: only update matrix by OSD controls */
    /* for mode1 : the configures of matrix keep the same as the values by calling CFD last time */
    MS_U8  u8OSD_UI_Mode;

    /* 0:auto depends on STB rule */
    /* 1:always do HDR2SDR for HDR input */
    /* 2:always not do HDR2SDR for HDR input */
    MS_U8  u8HDR_UI_H2SMode;
} STU_CFDAPI_UI_CONTROL;

typedef struct _STU_CFDAPI_DEBUG {
    MS_U32 u32Version;
    MS_U16 u16Length;

    MS_U8  ShowALLInputInCFDEn;

} STU_CFDAPI_DEBUG;

typedef struct _STU_CFDAPI_GOP_FORMAT {
    MS_U32 u32Version;
    MS_U16 u16Length;

    /* bit[0]: */
    /* 1: GOP use premultiplied Alpha */

    /* bit[1]: IsAlphaForGOPFlag */
    /* 1: alpha value is for GOP */
    MS_U8  u8GOP_AlphaFormat;

} STU_CFDAPI_GOP_FORMAT;

typedef struct _STU_CFDAPI_GOP_PRESDRIP {
    MS_U32 u32Version;   /* <Version of current structure. Please always set to "CFD_MAIN_CONTROL_ST_VERSION" as input */
    MS_U16 u16Length;    /* <Length of this structure, u16Length=sizeof(STU_CFDAPI_Kano_SDRIP) */

    /* IP2 CSC */
    MS_U8 u8CSC_Mode;
    MS_U8 u8CSC_Ratio1;
    MS_U8 u8CSC_Manual_Vars_en;
    MS_U8 u8CSC_MC;

} STU_CFDAPI_GOP_PRESDRIP;

typedef struct _STU_CFD_MS_ALG_INTERFACE_GOP_PRESDRIP {

    STU_CFDAPI_GOP_PRESDRIP  stu_GOP_PRESDRIP_Param;

} STU_CFD_MS_ALG_INTERFACE_GOP_PRESDRIP;

typedef struct _STU_CFDAPI_HW_IPS_GOP {

    /* u8HWGroup is for GOP group ID, starts from 0 */
    MS_U8 u8HWGroup;

    STU_CFD_MS_ALG_INTERFACE_GOP_PRESDRIP *pstu_PRESDRIP_Input;

} STU_CFDAPI_HW_IPS_GOP;

typedef struct _STU_CFD_COLORIMETRY {
    /* order R->G->B */
    MS_U16 u16Display_Primaries_x[3];      /* data *0.00002 0xC350 = 1 */
    MS_U16 u16Display_Primaries_y[3];      /* data *0.00002 0xC350 = 1 */
    MS_U16 u16White_point_x;               /* data *0.00002 0xC350 = 1 */
    MS_U16 u16White_point_y;               /* data *0.00002 0xC350 = 1 */

} STU_CFD_COLORIMETRY;

typedef struct _STU_CFDAPI_PANEL_FORMAT {
    MS_U32 u32Version;   /* <Version of current structure. Please always set to "CFD_HDMI_PANEL_ST_VERSION" as input */
    MS_U16 u16Length;    /* <Length of this structure, u16Length=sizeof(STU_CFDAPI_PANEL_FORMAT) */

    MS_U16 u16Panel_Med_Luminance;          /* data * 1 nits */
    MS_U16 u16Panel_Max_Luminance;          /* data * 1 nits */
    MS_U16 u16Panel_Min_Luminance;          /* data * 0.0001 nits */

    /* order R->G->B */
    STU_CFD_COLORIMETRY stu_Cfd_Panel_ColorMetry;

} STU_CFDAPI_PANEL_FORMAT;

typedef struct _STU_CFDAPI_UI_CONTROL_TESTING {

    MS_U32 u32TestCases;

} STU_CFDAPI_UI_CONTROL_TESTING;

typedef struct _STU_CFDAPI_MAIN_CONTROL_GOP_TESTING {

    MS_U32 u32TestCases;

} STU_CFDAPI_MAIN_CONTROL_GOP_TESTING;

typedef struct _STU_CFDAPI_MAIN_CONTROL_GOP {

    MS_U32 u32Version;   /* <Version of current structure. Please always set to "CFD_MAIN_CONTROL_ST_VERSION" as input */
    MS_U16 u16Length;    /* <Length of this structure, u16Length=sizeof(STU_CFD_MAIN_CONTROL) */

    /* E_CFD_MC_SOURCE */
    /* specify which input source */
    MS_U8 u8Input_Source;

    /* E_CFD_CFIO */
    MS_U8 u8Input_Format;

    /* E_CFD_MC_FORMAT */
    /* specify RGB/YUV format of the input of the first HDR/SDR IP */
    /* E_CFD_MC_FORMAT_RGB       = 0x00, */
    /* E_CFD_MC_FORMAT_YUV422    = 0x01, */
    /* E_CFD_MC_FORMAT_YUV444    = 0x02, */
    /* E_CFD_MC_FORMAT_YUV420    = 0x03, */
    MS_U8 u8Input_DataFormat;

    /* limit/full */
    /* assign with E_CFD_CFIO_RANGE */
    /* 0:limit 1:full */
    MS_U8 u8Input_IsFullRange;

    /* SDR/HDR */
    /* E_CFIO_HDR_STATUS */
    /* 0:SDR */
    /* 1:HDR1 */
    /* 2:HDR2 */
    MS_U8 u8Input_HDRMode;

    /* assign by E_CFD_CFIO_CP */
    MS_U8 u8Input_ext_Colour_primaries;

    /* assign by E_CFD_CFIO_TR */
    MS_U8 u8Input_ext_Transfer_Characteristics;

    /* assign by E_CFD_CFIO_MC */
    MS_U8 u8Input_ext_Matrix_Coeffs;

    /* used this gamut when u8Input_ext_Colour_primaries is E_CFD_CFIO_CP_SOURCE */
    STU_CFD_COLORIMETRY stu_Cfd_source_ColorMetry;

    /* specify RGB/YUV format of the output of the last HDR/SDR IP */
    /* E_CFD_MC_SOURCE */
    MS_U8 u8Output_Source;

    /* E_CFD_CFIO */
    MS_U8 u8Output_Format;

    /* E_CFD_MC_FORMAT */
    MS_U8 u8Output_DataFormat;

    /* 0:limit 1:full */
    MS_U8 u8Output_IsFullRange;

    /* E_CFIO_HDR_STATUS */
    /* 0:SDR */
    /* 1:HDR1 */
    /* 2:HDR2 */
    MS_U8 u8Output_HDRMode;

    /* assign by E_CFD_CFIO_CP */
    MS_U8 u8Output_ext_Colour_primaries;

    /* assign by E_CFD_CFIO_TR */
    MS_U8 u8Output_ext_Transfer_Characteristics;

    /* assign by E_CFD_CFIO_MC */
    MS_U8 u8Output_ext_Matrix_Coeffs;

    /* used this gamut when u8Output_ext_Colour_primaries is E_CFD_CFIO_CP_SOURCE */
    STU_CFD_COLORIMETRY stu_Cfd_target_ColorMetry;

    /* STU_CFDAPI_MAIN_CONTROL_GOP_TESTING stu_Cfd_main_control_testing; */


} STU_CFDAPI_MAIN_CONTROL_GOP;

typedef struct _STU_CFDAPI_TOP_CONTROL_GOP {
    /* share with different HW */
    MS_U32 u32Version;
    MS_U16 u16Length;
    STU_CFDAPI_MAIN_CONTROL_GOP      *pstu_Main_Control;
    /* STU_CFDAPI_MM_PARSER           *pstu_MM_Param; */
    /* STU_CFDAPI_HDMI_EDID_PARSER    *pstu_HDMI_EDID_Param; */
    STU_CFDAPI_UI_CONTROL            *pstu_UI_Param;
    STU_CFDAPI_PANEL_FORMAT          *pstu_Panel_Param;
    STU_CFDAPI_HW_IPS_GOP            *pstu_HW_IP_Param;
    STU_CFDAPI_GOP_FORMAT            *pstu_GOP_Param;

    STU_CFDAPI_DEBUG                 *pstu_Debug_Param;

} STU_CFDAPI_TOP_CONTROL_GOP;

typedef struct _STU_CFD_MS_ALG_COLOR_FORMAT_LITE {
    /* E_CFD_CFIO */
    MS_U8 u8Input_Format;

    /* E_CFD_MC_FORMAT */
    MS_U8 u8Input_DataFormat;

    /* limit/full */
    /* assign with E_CFD_CFIO_RANGE */
    /* 0:limit 1:full */
    MS_U8 u8Input_IsFullRange;

    /* SDR/HDR */
    /* 0:SDR */
    /* 1:HDR1 */
    /* 2:HDR2 */
    MS_U8 u8Input_HDRMode;

    /* follow E_CFD_CFIO_GAMUTORDER_IDX */
    /* use for non-panel output */
    /* MS_U8 u8Input_GamutOrderIdx; */

    /* E_CFD_CFIO_RGB */
    /* E_CFD_CFIO_YUV */
    /* E_CFD_CFIO */
    MS_U8 u8Output_Format;

    MS_U8 u8Output_DataFormat;

    /* MS_U8 u8Output_GamutOrderIdx; */
    /* follow E_CFD_CFIO_GAMUTORDER_IDX */
    /* use for non-panel output */

    MS_U8 u8Output_IsFullRange;

    /* SDR/HDR */
    /* 0:SDR */
    /* 1:HDR1 */
    /* 2:HDR2 */
    MS_U8 u8Output_HDRMode;

    /* Temp_Format[0] : output of IP2 CSC, input of HDR IP */
    /* Temp_Format[1] : output of HDR IP, input of SDR IP */
    MS_U8 u8Temp_Format[2];

    /* E_CFD_MC_FORMAT */
    MS_U8 u8Temp_DataFormat[2];

    /* E_CFD_CFIO_RANGE */
    MS_U8 u8Temp_IsFullRange[2];

    /* E_CFIO_HDR_STATUS */
    MS_U8 u8Temp_HDRMode[2];

    /* MS_U8 u8Temp_GamutOrderIdx[2]; */

    /* redefinition */
    /* item 0-10 are the same as table E.3 in HEVC spec */
    MS_U8 u8InputColorPriamries;
    MS_U8 u8OutputColorPriamries;
    MS_U8 u8TempColorPriamries[2];
    /* 0:Reserverd */
    /* 1:BT. 709/sRGB/sYCC */
    /* 2:unspecified */
    /* 3:Reserverd */
    /* 4:BT. 470-6 */
    /* 5:BT. 601_625/PAL/SECAM */
    /* 6:BT. 601_525/NTSC/SMPTE_170M */
    /* 7:SMPTE_240M */
    /* 8:Generic film */
    /* 9:BT. 2020 */
    /* 10:CIEXYZ */
    /* 11:AdobeRGB */
    /* 255:undefined */

    /* redefinition */
    /* item 0-17 are the same as table E.4 in HEVC spec */
    MS_U8 u8InputTransferCharacterstics;
    MS_U8 u8OutputTransferCharacterstics;
    MS_U8 u8TempTransferCharacterstics[2];
    /* 0 Reserverd */
    /* 1 BT. 709 */
    /* 2 unspecified */
    /* 3 Reserverd */
    /* 4 Assume display gamma 2.2 */
    /* 5 Assume display gamma 2.8 */
    /* 6 BT. 601_525/BT. 601_525 */
    /* 7 SMPTE_240M */
    /* 8 linear */
    /* 9 Logarithmic (100:1 range) */
    /* 10    Logarithmic (100*sqrt(10):1 range) */
    /* 11    xvYCC */
    /* 12    BT. 1361 extend color gamut system */
    /* 13    sRGB/sYCC */
    /* 14    BT. 2020 */
    /* 15    BT. 2020 */
    /* 16    SMPTE ST2084 for 10.12.14.16-bit systetm */
    /* 17    SMPTE ST428-1 */
    /* 18   AdobeRGB */
    /* 255  undefined */

    /* the same the same as table E.5 in HEVC spec */
    MS_U8 u8InputMatrixCoefficients;
    MS_U8 u8OutputMatrixCoefficients;
    MS_U8 u8TempMatrixCoefficients[2];
    /* 0 Identity */
    /* 1 BT. 709/xvYCC709 */
    /* 2 unspecified */
    /* 3 Reserverd */
    /* 4 USFCCT 47 */
    /* 5 BT. 601_625/PAL/SECAM/xvYCC601/sYCC */
    /* 6 BT. 601_525/NTSC/SMPTE_170M */
    /* 7 SMPTE_240M */
    /* 8 YCgCo */
    /* 9 BT. 2020NCL(non-constant luminance) */
    /* 10    BT. 2020CL(constant luminance) */
    /* 255:Reserverd */
    STU_CFD_COLORIMETRY stu_Cfd_ColorMetry[4];


    /* temp variable for debug */
    MS_U16 u16_check_status;

    MS_U8 u8DoTMO_Flag;
    MS_U8 u8DoGamutMapping_Flag;
    MS_U8 u8DoDLC_Flag;
    MS_U8 u8DoBT2020CLP_Flag;

    /* 0 or 1 */
    /* 0: u8HWGroup is for mainsub */
    /* 1: u8HWGroup is for GOP groups */
    MS_U8 u8HWGroupMode;
    MS_U8 u8HWGroup;

} STU_CFD_MS_ALG_COLOR_FORMAT_LITE;

typedef struct _STU_CFDAPI_PREPROCESS_STATUS_LITE {
    MS_U32 u32Version;
    MS_U16 u16Length;

    STU_CFD_MS_ALG_COLOR_FORMAT_LITE stu_Cfd_color_format;

} STU_CFDAPI_PREPROCESS_STATUS_LITE;

typedef struct _STU_CFDAPI_GOP_PREPROCESS_OUT {
    MS_U32 u32Version;
    MS_U16 u16Length;

    STU_CFDAPI_PREPROCESS_STATUS_LITE    stu_Cfd_preprocess_status;

} STU_CFDAPI_GOP_PREPROCESS_OUT;

typedef struct _STU_CFD_GENERAL_CONTROL_GOP {
    MS_U32 u32Version;   /* <Version of current structure. Please always set to "CFD_MAIN_CONTROL_ST_VERSION" as input */
    MS_U16 u16Length;    /* <Length of this structure, u16Length=sizeof(STU_CFD_MAIN_CONTROL) */

    /* MS_U8 u8InputSource; */
    /* MS_U8 u8MainSubMode; */
    /* MS_U8 u8IsRGBBypass; */

    MS_U8 u8HWGroupMode;
    MS_U8 u8HWGroup;

    /* E_CFD_CFIO_RANGE_LIMIT */
    /* E_CFD_CFIO_RANGE_FULL */
    /* MS_U8 u8DoPathFullRange; */
    STU_CFDAPI_DEBUG                 stu_Debug_Param;

} STU_CFD_GENERAL_CONTROL_GOP;

typedef struct _STU_CFD_CONTROL_POINT {
    MS_U32 u32Version;   /* <Version of current structure. Please always set to "CFD_CONTROL_POINT_ST_VERSION" as input */
    MS_U16 u16Length;    /* <Length of this structure, u16Length=sizeof(STU_CFD_CONTROL_POINT) */
    MS_U8 u8MainSubMode;
    MS_U8 u8Format;
    MS_U8 u8DataFormat;
    MS_U8 u8IsFullRange;
    MS_U8 u8HDRMode;
    MS_U8 u8SDRIPMode;
    MS_U8 u8HDRIPMode;
    MS_U8 u8GamutOrderIdx;
    MS_U8 u8ColorPriamries;
    MS_U8 u8TransferCharacterstics;
    MS_U8 u8MatrixCoefficients;

    MS_U16 u16BlackLevelY;
    MS_U16 u16BlackLevelC;
    MS_U16 u16WhiteLevelY;
    MS_U16 u16WhiteLevelC;

    /* HDR10plus */
    MS_U32 u16PanelMaxLuminance;
    MS_U32 u32NormGain;

    /* new part */
    /* for CFD control */
    /* 0: call Mapi_Cfd_Preprocessing and Mapi_Cfd_Decision */
    /* 1: call Mapi_Cfd_Preprocessing only */
    MS_U8 u8Cfd_process_mode;

    /* Process Mode */
    /* 0: color format driver on - auto */
    /* CFD process depends on the input of CFD */
    /* 1: shows SDR UI, UI 709, no video */
    MS_U8 u8PredefinedProcess;

    /* report Process */
    /* bit0 DoDLCflag */
    /* bit1 DoGamutMappingflag */
    /* bit2 DoTMOflag */
    MS_U8 u8Process_Status;
    MS_U8 u8Process_Status2;
    MS_U8 u8Process_Status3;

    MS_U16 u16_check_status;
    STU_CFD_COLORIMETRY stu_Source_Param_Colorimetry;
    STU_CFD_COLORIMETRY stu_Panel_Param_Colorimetry;
} STU_CFD_CONTROL_POINT;

typedef struct {
    STU_CFD_GENERAL_CONTROL_GOP          *pstu_Cfd_General_Control;
    STU_CFD_CONTROL_POINT                 *pstu_Cfd_Control_Point_Front;
    STU_CFD_CONTROL_POINT                 *pstu_Cfd_Control_Point_End;
    STU_CFD_MS_ALG_INTERFACE_GOP_PRESDRIP *pstu_SDRIP_Param;
    STU_CFDAPI_UI_CONTROL                 *pstu_UI_control;

} ST_CFD_GOP_PRESDR_INPUT;

typedef struct _STU_Register_Table {
    MS_U32 u32Depth;
    MS_U32 *pu32Address;
    MS_U16 *pu16Value;
    MS_U16 *pu16Mask;
    MS_U16 *pu16Client;
} STU_Register_Table;

typedef struct _STU_Autodownload_Table {
    MS_U16 u16client;
    MS_U8 *pu8Data ;
    MS_U32 u32Size;
} STU_Autodownload_Table;

typedef struct _ST_STATUS {
    MS_U32 u32Version;
    MS_U16 u16Length;
    MS_U16 u16UpdateStatus; /* Check whitch IPs need to update */
} ST_STATUS;

typedef struct {
    MS_U32 u32Version;
    MS_U16 u16Length;
    STU_Register_Table stRegTable;
    STU_Autodownload_Table stAdlTable;
    ST_STATUS stStatus;
} ST_CFD_CSC_HW_OUTPUT;

/* Coloect all output */
typedef struct {
    MS_U32 u32Version;
    MS_U16 u16Length;
    STU_Register_Table stRegTable;
    STU_Autodownload_Table stAdlTable;
    ST_STATUS stStatus;
} ST_CFD_GENERAL_HW_OUTPUT;

typedef struct {
    MS_U32 u32Version;
    MS_U16 u16Length;
    ST_CFD_CSC_HW_OUTPUT     stu_cfd_GOP_PreSDR_CSC_hw_output;
    ST_CFD_GENERAL_HW_OUTPUT stu_cfd_GOP_PreSDR_RGBOffset_hw_output;

} ST_CFD_GOP_PreSDR_HW_OUTPUT;

typedef struct _STU_CFDAPI_TOP_CONTROL_GOP_TESTING {
    /* share with different HW */
    MS_U32 u32Version;
    MS_U16 u16Length;

    MS_U8  u8TestEn;

    STU_CFDAPI_MAIN_CONTROL_GOP_TESTING   stu_Main_Control;

    STU_CFDAPI_UI_CONTROL_TESTING         stu_UI_Param;
    /* STU_CFDAPI_PANEL_FORMAT_TESTING       stu_Panel_Param; */
    /* STU_CFDAPI_HW_IPS_GOP_TESTING         stu_HW_IP_Param; */
    /* STU_CFDAPI_GOP_FORMAT_TESTING         stu_GOP_Param; */

} STU_CFDAPI_TOP_CONTROL_GOP_TESTING;


#endif  /* _MDRV_GFLIP_CFD_H */
