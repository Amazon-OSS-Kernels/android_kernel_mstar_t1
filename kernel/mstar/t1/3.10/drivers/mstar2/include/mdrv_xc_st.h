/*
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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



 @file   mdrv_xc_st.h
 @brief  XC KMD Driver Interface
 @author MStar Semiconductor Inc.

*/

#ifndef _MDRV_XC_ST_H
#define _MDRV_XC_ST_H

/* ============================================================================= */
/*  Includs */
/* ============================================================================= */

/* ============================================================================= */
/*  Type and Structure Declaration
/* ============================================================================= */
#define PQ_GAMMA_LUT_ENTRY (256)
#define PQ_GAMMA_CHANNEL  (3)

#define KDRV_XC_HDMI_HDR_METADATA_VERSION 0
#define KDRV_XC_PANEL_FORMAT_VERSION 0
/* CFD OSD structure version */
/* Version 0: Init structure */
/* Version 1: Add backlight and hue/contrast/saturation valid flag */
/* Version 2: Add color range and main/sub window. */
/* Version 3: Add ultra black / white */
/* Version 4: Add skip picture setting (hue/ contrast/ saturation) */
/* Version 5: Add color correction matrix */
/* Version 6: Add RGB Matrix */
/* Version 7: Add view Mode (switch view mode for dolby) */
#define KDRV_XC_OSD_CONTROL_VERSION 7
#define KDRV_XC_MAIN_CONTROL_VERSION 0
#define KDRV_XC_HDMI_INFOFRAME_PARSER_VERSION 0
#define KDRV_XC_HDMI_EDID_PARSER_VERSION 0
#define KDRV_XC_MM_PARSER_VERSION 0
#define KDRV_XC_CFD_HDMI_VERSION 0
/* CFD Panel structure version */
/* Version 0: Init structure */
/* Version 1: Add Linear RGB and customer color primaries setting */
/* Version 2: Add PWM port & dolby global dimming on/off & dolby global delay frame count. */
#define KDRV_XC_CFD_PANEL_VERSION 2
#define KDRV_XC_CFD_HDR_VERSION 0
#define KDRV_XC_CFD_EDID_VERSION 0
#define KDRV_XC_CFD_FIRE_VERSION 0
#define KDRV_XC_CFD_XC_STATUS_VERSION (1)
#define KDRV_XC_CFD_TMO_CUS_MAPPING_VERSION (1)
#define ST_KDRV_XC_PQ_GAMMA_IN_VERSION (3)
#define ST_KDRV_XC_PQ_RGB_PATTERN_IN_VERSION (1)
#define ST_KDRV_XC_PQ_BLUE_STRETCH_IN_VERSION (1)
#define ST_KDRV_XC_CFD_NR_VERSION (1)
/* AUTODOWNLOAD_FORMAT_INFO version of current XC lib */
/* version 2 : add bDataParsed */
/* version 3 : add enPQGammaFormat */
#define ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO_VERSION          (3)
#define ST_KDRV_XC_PQ_MANUAL_LUMA_CURVE_VERSION (1)
#define ST_KDRV_XC_PQ_LUMA_INFO_VERSION (1)
#define ST_KDRV_XC_PQ_STRETCH_SETTINGS_VERSION (1)
#define ST_KDRV_XC_PQ_CHROMA_INFO_VERSION (1)
#define HISTOGRAM_SATOFHUE_WIN_NUM (6)
#define ST_KDRV_XC_PQ_NONLINEARSCALING_API_PARAM_IN_VERSION (1)
/*  Kernel XC Event Structure */
/*  Version 1 : Initial structure */
#define ST_KDRV_XC_EVENT_VERSION (1)
/* Version 0: Init structure */
/* Version 1: Add content is full range and ultra black & white active */
/* Version 2: Add color type of the source */
/* Version 3: Add HDR metadata of the source(ott or hdmi) */
/* Version 4: Add HDR Mdebug Information */
/* Version 5: Add TMO Color Type */
/* Version 6: Add HDR Seamless Information */
/* Version 7: Add BOX Output Type */
/* Version 8: Add XC HW Point Color info */
#define ST_KDRV_XC_CFD_STATUS_VERSION (8)
/* Version 1: Init structure
#define ST_KDRV_XC_HDR_CFD_POINT_INFOR_VERSION (1)

typedef struct {
	MS_BOOL bCurveChange;  /*  Curve Change */
	MS_U8  u8LumaCurve[16];  /*  Default luma curve from Supernova DLC.ini */
	MS_U8  u8LumaCurve2_a[16];  /*  Default luma curve 2a  from Supernova DLC.ini */
	MS_U8  u8LumaCurve2_b[16];  /*  Default luma curve 2b  from Supernova DLC.ini */
	MS_U8  u8Result; /* Out
} ST_KDRV_XC_DLC_INFO;

typedef struct {
	MS_U64 u64VRAddress;  /*  Default VR Address from Supernova MMAP */
	MS_U32 u32VRSize;  /*  Default VR Size from Supernova MMAP */
	MS_U8  u8Result; /* Out */
} ST_KDRV_XC_DLC_SET_VR_ADDRESS;

typedef struct {
	MS_BOOL bBLEPointChange;  /*  BLE Point Change */
	MS_U16  u16BLESlopPoint[6];  /*  Default BLE Slop Point from Supernova DLC.ini */
	MS_U8  u8Result; /* Out
} ST_KDRV_XC_DLC_BLE_INFO;

typedef struct {
	MS_BOOL bInitInfoChange;  /*  Init Info Change */

	/*  Default luma curve */
	MS_U8 au8LumaCurve[16];
	MS_U8 au8LumaCurve2_a[16];
	MS_U8 au8LumaCurve2_b[16];
	MS_U8 au8DlcHistogramLimitCurve[17];

	MS_U8 u8DlcPureImageMode; /*  Compare difference of max and min bright */
	MS_U8 u8DlcLevelLimit; /*  n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7 */
	MS_U8 u8DlcAvgDelta; /*  n = 0 ~ 50, default value: 12 */
	MS_U8 u8DlcAvgDeltaStill; /*  n = 0 ~ 15 => 0: disable still curve, 1 ~ 15 => 0.1 ~ 1.5 enable still curve */
	MS_U8 u8DlcFastAlphaBlending; /*  min 17 ~ max 32 */
	MS_U8 u8DlcYAvgThresholdL; /*  default value: 0 */
	MS_U8 u8DlcYAvgThresholdH; /*  default value: 128 */

	MS_U8 u8DlcBLEPoint; /*  n = 24 ~ 64, default value: 48 */
	MS_U8 u8DlcWLEPoint; /*  n = 24 ~ 64, default value: 48 */
	MS_U8 bEnableBLE:1; /*  1: enable; 0: disable */
	MS_U8 bEnableWLE:1; /*  1: enable; 0: disable */
	MS_U8 u8DlcYAvgThresholdM;
	MS_U8 u8DlcCurveMode;
	MS_U8 u8DlcCurveModeMixAlpha;
	MS_U8 u8DlcAlgorithmMode;
	MS_U8 u8DlcSepPointH;
	MS_U8 u8DlcSepPointL;
	MS_U16 u16DlcBleStartPointTH;
	MS_U16 u16DlcBleEndPointTH;
	MS_U8 u8DlcCurveDiff_L_TH;
	MS_U8 u8DlcCurveDiff_H_TH;
	MS_U16 u16DlcBLESlopPoint_1;
	MS_U16 u16DlcBLESlopPoint_2;
	MS_U16 u16DlcBLESlopPoint_3;
	MS_U16 u16DlcBLESlopPoint_4;
	MS_U16 u16DlcBLESlopPoint_5;
	MS_U16 u16DlcDark_BLE_Slop_Min;
	MS_U8 u8DlcCurveDiffCoringTH;
	MS_U8 u8DlcAlphaBlendingMin;
	MS_U8 u8DlcAlphaBlendingMax;
	MS_U8 u8DlcFlicker_alpha;
	MS_U8 u8DlcYAVG_L_TH;
	MS_U8 u8DlcYAVG_H_TH;
	MS_U8 u8DlcDiffBase_L;
	MS_U8 u8DlcDiffBase_M;
	MS_U8 u8DlcDiffBase_H;
	/*  NEW DLC */
	MS_U8 u8LMaxThreshold;
	MS_U8 u8LMinThreshold;
	MS_U8 u8LMaxCorrection;
	MS_U8 u8LMinCorrection;
	MS_U8 u8RMaxThreshold;
	MS_U8 u8RMinThreshold;
	MS_U8 u8RMaxCorrection;
	MS_U8 u8RMinCorrection;
	MS_U8 u8AllowLoseContrast;
} ST_KDRV_XC_DLC_INIT_INFO;

typedef struct {
	MS_U32 u32OutputNits[512];
	MS_U16 u16InputPoint[512];
	MS_U16 u16ControlPoints;
	MS_S16 s16LastLess1IndexOutputnits;
	MS_S16 s16LastIndexM10000Outputs;
	MS_S16 s16LastIndexM100Outputs;
} ST_KDRV_XC_DLC_TMO_1DLUT;

/* CFD TMO_CUS_MAPPING, ReGen the Source && Target curve for Mapping Video and Graphic. */
typedef struct {
	/* Structure version */
	MS_U32 u32Version;
	/* Structure length */
	MS_U32 u32Length;

	/* Window (main or sub window) */
	MS_U8 u8Win;
	/* Enable/Disable Mapping */
	MS_BOOL bEnable;
} ST_KDRV_XC_CFD_TMO_CUS_MAPPING;

/*  HDR new tone mapping parameters. */
typedef struct {
	/*  TMO
	MS_U16 u16SrcMinRatio;          /* default 10 */
	MS_U16 u16SrcMedRatio;          /* default 512 */
	MS_U16 u16SrcMaxRatio;          /* default 990 */

	MS_U16 u16TgtMin;               /* default 500 */
	MS_U16 u16TgtMed;
	MS_U16 u16TgtMax;               /* default 300 */

	MS_U16 u16FrontSlopeMin;        /* default 256 */
	MS_U16 u16FrontSlopeMax;        /* default 512 */
	MS_U16 u16BackSlopeMin;         /* default 128 */
	MS_U16 u16BackSlopeMax;         /* default 256 */

	MS_U16 u16SceneChangeThrd;      /* default 1024 */
	MS_U16 u16SceneChangeRatioMax;  /* default 1024 */

	MS_U8 u8IIRRatio;            /* default 31 */
	MS_U8 u8TMOTargetMode;      /*  default 0. 0 : keeps the value in initial function  1 : from output source */
	MS_U16 u16SDRPanelGain;

	MS_U16 u16Smin;
	MS_U16 u16Smed;
	MS_U16 u16Smax;
	MS_U16 u16Tmin;
	MS_U16 u16Tmed;
	MS_U16 u16Tmax;

	MS_BOOL bRefMode;

	MS_U8 u8TMOUseIniControls;
	ST_KDRV_XC_DLC_TMO_1DLUT stTMO1DLUT;

} ST_KDRV_XC_TMO_INFO;

typedef enum {
	EN_KDRV_XC_HDR_DMA_BYPASS,
	EN_KDRV_XC_HDR_DMA_ENABLE,
	EN_KDRV_XC_HDR_DMA_MAX,
} EN_KDRV_XC_HDR_DMA_PATH;

/* IO Ctrl struct defines: refer to EN_XC_HDR_TYPE in apixc.h */
typedef enum {
	E_KDRV_XC_HDR_NONE,
	E_KDRV_XC_HDR_DOLBY,
	E_KDRV_XC_HDR_OPEN,
	E_KDRV_XC_HDR_TECHNICOLOR,
	E_KDRV_XC_HDR_DOLBY_LL,
	E_KDRV_XC_HDR_HDR10_PLUS,
	E_KDRV_XC_HDR_MAX,
} EN_KDRV_XC_HDR_TYPE;

typedef enum {
	E_KDRV_XC_INPUT_SOURCE_NONE,
	E_KDRV_XC_INPUT_SOURCE_HDMI,
	E_KDRV_XC_INPUT_SOURCE_OTT,
	E_KDRV_XC_INPUT_SOURCE_DTV,
	E_KDRV_XC_INPUT_SOURCE_MAX,
} EN_KDRV_XC_INPUT_SOURCE_TYPE;

typedef enum {
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_OP2GAMMA,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_FRCOP2GAMMA,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_XVYCC,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_ODTABLE1,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_ODTABLE2,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_ODTABLE3,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_DEMURA,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_OP2LUT,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_T3D_0,        /* t3d of sc0 */
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_T3D_1,        /* t3d of sc1 */
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_FRCSPTPOPM,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_FOOPM,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_HVSP,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_HVSP_DIP,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_SCRAMBLE,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_OSD_HDR,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_OSD_THDR,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_ADL2RIU,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_HSY,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_PQ_GAMMA,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_PANEL_GAMMA,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_DLC_256,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_ICC_IHC,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_3DLUT,         /*Dolby, TMO */
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_SC2VIP,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_RGBWGEN2,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_OD,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_PCID,
	E_KDRV_XC_AUTODOWNLOAD_CLIENT_MAX,
} EN_KDRV_XC_AUTODOWNLOAD_CLIENT;

typedef enum {
	E_KDRV_XC_AUTODOWNLOAD_TRIGGER_MODE,
	E_KDRV_XC_AUTODOWNLOAD_ENABLE_MODE,
} EN_KDRV_XC_AUTODOWNLOAD_MODE;

typedef enum {
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_TMO,
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_3DLUT,
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_GAMMA,
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_DEGAMMA,
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_OOTF,            /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_RGB3DLUT,        /*  for HDR IP 3D LUT */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_B0105,           /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_B0202_TMLUTI,    /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_B0202_TMLUTS,    /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_B0202_SMLUTI,    /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_B0202_SMLUTS,    /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_B0103,           /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_OSD_B0103,           /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_OSD_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_OSD_B0202_TMLUTI,    /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_OSD_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_OSD_B0202_TMLUTS,    /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_OSD_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_OSD_B0202_SMLUTI,    /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_OSD_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_OSD_B0202_SMLUTS,    /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_OSD_HDR */
	E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_TOSD_DEGAMMA,        /*  for E_KDRV_XC_AUTODOWNLOAD_CLIENT_OSD_THDR */
	E_KDRV_XC_AUTODOWNLOAD_ICC_IHC_SUB_ICC,
	E_KDRV_XC_AUTODOWNLOAD_ICC_IHC_SUB_IHC,
} EN_KDRV_XC_AUTODOWNLOAD_SUB_CLIENT;

typedef enum {
	E_KDRV_XC_AUTODOWNLOAD_XVYCC_SUB_GAMMA,
	E_KDRV_XC_AUTODOWNLOAD_XVYCC_SUB_DEGAMMA
} EN_KDRV_XC_AUTODOWNLOAD_XVYCC_SUB_CLIENT;

typedef enum {
    /* each data is 12bit, the table size is 384*8bit */
	E_KDRV_XC_ADL_PQ_GAMMA_12BIT_UNFORMATTED = 0,
    /* each data is 12bit, the table size is 256*16bit */
	E_KDRV_XC_ADL_PQ_GAMMA_12BIT_FORMATTED,
    /* each data is 14bit, the table size is 256*16bit */
	E_KDRV_XC_ADL_PQ_GAMMA_14BIT_FORMATTED,
    /*  Max enum value */
	E_KDRV_XC_ADL_PQ_GAMMA_FORMAT_MAX,
} EN_KDRV_XC_ADL_PQ_GAMMA_FORMAT;

typedef enum {
	E_KDRV_XC_HDR_COLOR_NONE,
	E_KDRV_XC_HDR_COLOR_YUV420,
	E_KDRV_XC_HDR_COLOR_YUV422,
	E_KDRV_XC_HDR_COLOR_YUV444,
	E_KDRV_XC_HDR_COLOR_RGB,
	E_KDRV_XC_HDR_COLOR_MAX,
} EN_KDRV_XC_HDR_COLOR_FORMAT;

typedef enum {
	E_KDRV_MLOAD_UNSUPPORTED = 0,
	E_KDRV_MLOAD_DISABLED    = 1,
	E_KDRV_MLOAD_ENABLED     = 2,
} EN_KDRV_MLOAD_TYPE;

typedef enum {
	E_CLIENT_MAIN_XC = 0,
	E_CLIENT_MAIN_HDR = 1,
	E_CLIENT_SUB_XC = 2,
	E_CLIENT_SUB_HDR = 3,
	E_CLIENT_MAIN_DEFLICKER = 4,
    /* For MultiWindow Usage */
	E_CLIENT_MULTIWIN_SETTING = 5,
	E_CLIENT_MULTIWIN_SETTING_NEXTFRAME = 6,
	E_CLIENT_MULTIWIN_STR = 7,
	E_CLIENT_MULTIWIN_LDR = 8,
	E_CLIENT_MAIN_DOLBY = 9, /*  for hdmi dolby */

	E_CLIENT_2D_MENULOAD_00 = 0x10,/* the first num:triger point, the sencend num:client. */
	E_CLIENT_2D_MENULOAD_01,
	E_CLIENT_2D_MENULOAD_02 ,
	E_CLIENT_2D_MENULOAD_03,
	E_CLIENT_2D_MENULOAD_04,
	E_CLIENT_2D_MENULOAD_05,
	E_CLIENT_2D_MENULOAD_10,
	E_CLIENT_2D_MENULOAD_11,
	E_CLIENT_2D_MENULOAD_12,
	E_CLIENT_2D_MENULOAD_13,
	E_CLIENT_2D_MENULOAD_14,
	E_CLIENT_2D_MENULOAD_15,
	E_CLIENT_2D_MENULOAD_20,
	E_CLIENT_2D_MENULOAD_21,
	E_CLIENT_2D_MENULOAD_22,
	E_CLIENT_2D_MENULOAD_23,
	E_CLIENT_2D_MENULOAD_24,
	E_CLIENT_2D_MENULOAD_25,
	E_CLIENT_2D_MENULOAD_30,
	E_CLIENT_2D_MENULOAD_31,
	E_CLIENT_2D_MENULOAD_32,
	E_CLIENT_2D_MENULOAD_33,
	E_CLIENT_2D_MENULOAD_34,
	E_CLIENT_2D_MENULOAD_35,
	E_CLIENT_2D_MENULOAD_40,
	E_CLIENT_2D_MENULOAD_41,
	E_CLIENT_2D_MENULOAD_42,
	E_CLIENT_2D_MENULOAD_43,
	E_CLIENT_2D_MENULOAD_44,
	E_CLIENT_2D_MENULOAD_45,

	E_CLIENT_MAX,
} EN_MLOAD_CLIENT_TYPE;

typedef enum {
	E_KDRV_MAIN_WINDOW = 0,
	E_KDRV_SUB_WINDOW,

	E_KDRV_MULTI_WINDOW0,          /* multiWindow0 */
	E_KDRV_MULTI_WINDOW1,          /* multiWindow1 */
	E_KDRV_MULTI_WINDOW2,          /* multiWindow2 */
	E_KDRV_MULTI_WINDOW3,          /* multiWindow3 */
	E_KDRV_MULTI_WINDOW4,          /* multiWindow4 */
	E_KDRV_MULTI_WINDOW5,          /* multiWindow5 */
	E_KDRV_MULTI_WINDOW6,          /* multiWindow6 */
	E_KDRV_MULTI_WINDOW7,          /* multiWindow7 */

	E_KDRV_MULTI_WINDOW8,          /* multiWindow8 */
	E_KDRV_MULTI_WINDOW9,          /* multiWindow9 */
	E_KDRV_MULTI_WINDOW10,         /* multiWindow10 */
	E_KDRV_MULTI_WINDOW11,         /* multiWindow11 */
	E_KDRV_MULTI_WINDOW12,         /* multiWindow12 */
	E_KDRV_MULTI_WINDOW13,         /* multiWindow13 */
	E_KDRV_MULTI_WINDOW14,         /* multiWindow14 */ */
	E_KDRV_MULTI_WINDOW15,         /* multiWindow15 */

	E_KDRV_MAX_WINDOW,
} EN_KDRV_WINDOW;

typedef enum {
	E_KDRV_XC = 0,
	E_KDRV_HDR,
} EN_KDRV_DS_CLIENT_TYPE;

typedef enum {
	E_KDRV_XC_SC0 = 0,
	E_KDRV_XC_SC1,
	E_KDRV_XC_MAX,
} EN_KDRV_SC_DEVICE;

typedef enum {
	E_K_APIXC_RET_FAIL = 0,                   /* Fail */
	E_K_APIXC_RET_OK,                         /* OK */
	E_K_APIXC_RET_FAIL_FUN_NOT_SUPPORT,       /* Fail */
	E_K_APIXC_RET_FAIL_ENUM_NOT_SUPPORT,      /* Fail */
	E_K_APIXC_RET_FAIL_INVALID_PARAMETER,     /* Fail */
} E_K_APIXC_ReturnValue;

typedef enum {
	E_K_XC_DS_INDEX_MVOP,
	E_K_XC_DS_INDEX_SCALER,
	E_K_XC_DS_INDEX_MAX,
}E_K_XC_DS_INDEX_SOURCE;

typedef struct {
	MS_U16 u16CMDCNT_IPM;
	MS_U16 u16CMDCNT_IPS;
	MS_U16 u16CMDCNT_OPM;
	MS_U16 u16CMDCNT_OPS;
} K_XC_DS_CMDCNT;

typedef enum {
	E_MLOAD_TRIGGER_BY_OP_SYNC = 0,
	E_MLOAD_TRIGGER_BY_IP_MAIN_SYNC = 1,
	E_MLOAD_TRIGGER_BY_IP_SUB_SYNC = 2,
	E_MLOAD_TRIGGER_BY_SW = 3,
	E_MLOAD_TRIGGER_BY_OP2_SYNC = 4,
	E_MLOAD_TRIGGER_MAX,
} EN_MLOAD_TRIG_SYNC;

typedef enum {
	E_MLOAD_2D_TRIGER0 = 0,
	E_MLOAD_2D_TRIGER1 = 1,
	E_MLOAD_2D_TRIGER2 = 2,
	E_MLOAD_2D_TRIGER3 = 3,
	E_MLOAD_2D_TRIGER4 = 4,
	E_MLOAD_2D_TRIGERMAX,
} EN_MLOAD_TRIGER;

typedef struct __attribute__((packed)) {
    /* MVOP
	MS_U32 u32MFCodecInfo;

    /* XC
	MS_BOOL bUpdate_DS_CMD;
	MS_BOOL bEnableDNR;
	MS_U32  u32DSBufferSize;
	MS_BOOL bEnable_ForceP;
	MS_BOOL bDynamicScalingEnable;
	MS_BOOL bSwitchMVOPToSCPath;
	MS_BOOL bFBL;
	MS_BOOL bR_FBL;
} K_XC_SETDS_Info;

typedef struct __attribute__((packed)) {
	MS_BOOL bDynamicScalingEnable;
} K_XC_GETDS_Info;

typedef struct __attribute__((packed)) {
	MS_U64 u64DS_Info_BaseAddr;         /*< The memory base address to update IP/OP dynamic scaling registers. Absolute without any alignment. */
	MS_U8  u8MIU_Select;                /*< 0: MIU0, 1: MIU1, etc. */
	MS_U8  u8DS_Index_Depth;            /*< The index depth (how many entries to fire per MVOP Vsync falling) of DS */
	MS_BOOL bOP_DS_On;                  /*< OP dynamic scaling on/off */
	MS_BOOL bIPS_DS_On;                 /*< IPS dynamic scaling on/off */
	MS_BOOL bIPM_DS_On;                 /*< IPM dynamic scaling on/off */
	MS_U32  u32DSBufferSize;            /*< Ds buffer size */
} K_XC_DynamicScaling_Info;

typedef enum {
	K_DS_IP,
	K_DS_OP,
} k_ds_reg_ip_op_sel;

typedef enum {
	K_DS_XC,
	K_DS_MVOP,
	K_DS_GOP,
} k_ds_reg_source_sel;

/*****************DS NEW*****************/
typedef enum {
	E_DS_CLIENT_XC,
	E_DS_CLIENT_HDR,
	E_DS_CLIENT_PQ,
} E_DS_CLIENT;

typedef struct {
	E_DS_CLIENT client;
	MS_U32  max_num;
} K_DS_register_info;
/****************************************/

typedef struct __attribute__((packed)) {
	MS_U8 *pu8Data;                     /* pointer to data */
#if !defined (__aarch64__)
    void    *pDummy;
#endif
	MS_U32 u32Size;                     /* data size */
} ST_KDRV_XC_OPEN_METADATA_INFO;

typedef struct __attribute__((packed)) {
	MS_U8 *pu8Data;                     /* pointer to data */
#if !defined (__aarch64__)
    void    *pDummy;
#endif
	MS_U32 u32Size;                     /* data size */
} ST_KDRV_XC_3DLUT_INFO;

typedef struct {
	MS_U8 u8Version;
	MS_U32 u32Size;
	MS_U32 *pu32RGBHistogram;
#if !defined (__aarch64__)
    void *pDummy;
#endif
 } ST_KDRV_XC_RGBHistogram;

typedef struct {
	MS_U64    phyBaseAddr;                      /* baseaddr */
	MS_U32    u32Size;
	MS_U32    u32MiuNo;
} ST_KDRV_XC_SHARE_MEMORY_INFO;

typedef struct {
	MS_U16 u16X;           /*<start x of the window */
	MS_U16 u16Y;           /*<start y of the window */
	MS_U16 u16Width;       /*<width of the window */
	MS_U16 u16Height;      /*<height of the window */
} ST_KDRV_XC_WINDOW_INFO;

typedef struct {
	EN_KDRV_XC_AUTODOWNLOAD_CLIENT enClient; /* current client */
	MS_U64 phyBaseAddr;                      /* baseaddr */
	MS_U32 u32Size;                          /* size */
	MS_U32 u32MiuNo;                         /* miu no */
	MS_BOOL bEnable;                         /* enable/disable the client */
	EN_KDRV_XC_AUTODOWNLOAD_MODE enMode;     /* work mode */
} ST_KDRV_XC_AUTODOWNLOAD_CONFIG_INFO;

typedef struct {
	MS_U32 u32FormatInfo_Version;               /*<Version of current structure */
	MS_U16 u16FormatInfo_Length;                /*<Length of this structure */

	EN_KDRV_XC_AUTODOWNLOAD_SUB_CLIENT enSubClient; /* sub client */
	MS_BOOL bEnableRange;                           /* whether enable range, 0: write data to (0-511), 1: write data to (u32StartAddr-u16EndAddr) */
	MS_U16 u16StartAddr;                            /* start addr, range: 0-511, startaddr <= endaddr */
	MS_U16 u16EndAddr;                              /* end addr: range: 0-511, startaddr <= endaddr */
	MS_BOOL bDataParsed;                        /*Is data dram format */
	EN_KDRV_XC_ADL_PQ_GAMMA_FORMAT enPQGammaFormat;   /* PQ Gamma Data Format */
} ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO;

typedef struct {
	EN_KDRV_XC_AUTODOWNLOAD_XVYCC_SUB_CLIENT enSubClient; /* sub client */
} ST_KDRV_XC_AUTODOWNLOAD_XVYCC_FORMAT_INFO;

typedef struct __attribute__((packed)) {
	MS_U32 u32DataInfo_Version;         /*<Version of current structure. */
	MS_U16 u16DataInfo_Length;          /*<Length of this structure */

	EN_KDRV_XC_AUTODOWNLOAD_CLIENT enClient;  /* current client */
	MS_U8 *pu8Data;                           /* pointer to data */
#if !defined (__aarch64__)
    void    *pDummy;
#endif
	MS_U32 u32Size;                         /* data size */
    void *pParam;                           /* pointer to ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO */
#if !defined (__aarch64__)
    void    *pDummy2;
#endif
} ST_KDRV_XC_AUTODOWNLOAD_DATA_INFO;

typedef struct {
	EN_KDRV_XC_AUTODOWNLOAD_CLIENT enClient;  /* current client */
	MS_BOOL bSupported;                       /* whether it is supported */
} ST_KDRV_XC_AUTODOWNLOAD_CLIENT_SUPPORTED_CAPS;

typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	MS_U64 PhyAddr;
	MS_U32 u32BufByteLen;
} ST_KDRV_MLOAD_INIT;

typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	MS_BOOL bEnable;
} ST_KDRV_MLOAD_ENABLE;

typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	EN_KDRV_MLOAD_TYPE eReturn;
} ST_KDRV_MLOAD_GETSTATUS;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
    /* Reg Bank (IN) */
	MS_U32 u32Bank;
    /* Reg Address (IN) */
	MS_U32 u32Addr;
    /* Reg Data (IN) */
	MS_U16 u16Data;
    /*Reg Mask (IN) */
	MS_U16 u16Mask;
} ST_KDRV_XC_REG;

typedef enum {
    /* Fire MultiWIndow, Get Fire Control REG */
	E_KDRV_XC_MULTIWIN_FIRE,
    /* Get Wait Done REG for MultiWindow */
	E_KDRV_XC_MULTIWIN_WAITDONE,
} EN_KDRV_XC_MULTIWIN_FIRETYPE;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
    /* Control MultiWindow Window Number */
	EN_KDRV_WINDOW enWindow;
    /* Control MultiWindow Node */
	MS_U16 u16NodeIndex;
    /* Control MultiWindow Fire Type */
	EN_KDRV_XC_MULTIWIN_FIRETYPE enFiretype;

    /* Menuload Client Node Start Hbye Address (OUT) */
	ST_KDRV_XC_REG stHbyteAddr;
    /* Menuload Client Node Start Lbyte Address (OUT) */
	ST_KDRV_XC_REG stLbyteAddr;
    /* Menuload Command Depth (OUT) */
	ST_KDRV_XC_REG stDepth;
    /* Menuload Enable(OUT) */
	ST_KDRV_XC_REG stEnable;
    /* Menuload Triger(OUT) */
	ST_KDRV_XC_REG stTriger;
    /* Menuload Finish Polling Reg(OUT) */
	ST_KDRV_XC_REG stWaitDone;
} ST_KDRV_XC_MLOAD_MULTIWIN_FIREINFO;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
    /* Control MultiWindow Window Number */
	EN_KDRV_WINDOW enWindow;
    /* Control MultiWindow Node */
	MS_U16 u16NodeIndex;
} ST_KDRV_XC_MLOAD_CONTROLINFO;

typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	MS_BOOL bImmeidate;
	MS_BOOL bReturn;
} ST_KDRV_MLOAD_FIRE;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
	EN_MLOAD_CLIENT_TYPE enClientType;
	EN_MLOAD_TRIG_SYNC enTriggerSync;
	ST_KDRV_XC_MLOAD_CONTROLINFO *pstInfo;
#if !defined (__aarch64__)
	MS_U32    u32AlignDummy;
#endif
	MS_BOOL bReturn;
} ST_KDRV_XC_MLOAD_FIRE_EXT;

typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	MS_U32 u32Addr;
	MS_U16 u16Data;
	MS_U16 u16Mask;
	MS_BOOL bReturn;
} ST_KDRV_MLOAD_WRT_CMD;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
	EN_MLOAD_CLIENT_TYPE enClientType;
	EN_MLOAD_TRIG_SYNC enTriggerSync;
	ST_KDRV_XC_REG stKReg;
	ST_KDRV_XC_MLOAD_CONTROLINFO *pstInfo;
#if !defined (__aarch64__)
	MS_U32    u32AlignDummy;
#endif
	MS_BOOL bReturn;
} ST_KDRV_XC_MLOAD_WRT_CMD_EXT;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
	EN_MLOAD_CLIENT_TYPE enClientType;
	ST_KDRV_XC_MLOAD_MULTIWIN_FIREINFO *pstInfo;
#if !defined (__aarch64__)
    void *pDummy;
#endif
	MS_BOOL bReturn;
} ST_KDRV_XC_MLOAD_MULTIWINFIRE;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
	EN_MLOAD_CLIENT_TYPE enClientType;
	EN_KDRV_WINDOW enWindow;
	MS_BOOL bReturn;
} ST_KDRV_XC_MLOAD_MULTIWINRESET;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
	ST_KDRV_XC_REG *stReg;
#if !defined (__aarch64__)
	MS_U32    u32AlignDummy;
#endif
	EN_KDRV_WINDOW enWindow;
	MS_BOOL bReturn;
} ST_KDRV_XC_MLOAD_READ_CMD;


typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	MS_U32 u32Bank;
	MS_U32 u32Addr;
	MS_U16 u16Data;
	MS_U16 u16Mask;
	MS_BOOL bReturn;
} ST_KDRV_MLOAD_WRT_CMD_NONXC;

typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	MS_U16 u16train;
	MS_U16 u16disp;
	MS_BOOL bReturn;
} ST_KDRV_MLOAD_SET_IP_TRIG_P;

typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	MS_U16 *pu16Train;
	MS_U16 *pu16Disp;
	MS_BOOL bReturn;
} ST_KDRV_MLOAD_GET_IP_TRIG_P;

typedef struct {
	EN_MLOAD_CLIENT_TYPE _client_type;
	EN_MLOAD_TRIG_SYNC eTriggerSync;
} ST_KDRV_MLOAD_SET_TRIG_SYNC;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	EN_KDRV_WINDOW  eWindow;
	E_K_APIXC_ReturnValue eReturn;
} ST_KDRV_DS_GET_DSForceIndexSupported;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	E_K_XC_DS_INDEX_SOURCE eDSIdxSrc;
	EN_KDRV_WINDOW  eWindow;
	E_K_APIXC_ReturnValue eReturn;
} ST_KDRV_DS_SET_DSIndexSourceSelect;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	MS_BOOL bEnable;
	MS_U8 u8Index;
	EN_KDRV_WINDOW  eWindow;
	E_K_APIXC_ReturnValue eReturn;
} ST_KDRV_DS_SET_DSForceIndex;

typedef struct __attribute__((packed)) {
	EN_KDRV_SC_DEVICE u32DeviceID;
	K_XC_DynamicScaling_Info *pstDSInfo;
#if !defined (__aarch64__)
	MS_U32    pDummy;
#endif
	MS_U32 u32DSInfoLen;
	EN_KDRV_WINDOW  eWindow;
	MS_BOOL bReturn;
} ST_KDRV_DS_SetDynamicScaling;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	MS_BOOL bEnable;
} ST_KDRV_DS_SET_DynamicScalingFlag;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	MS_BOOL bReturn;
} ST_KDRV_DS_GetDynamicScalingStatus;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	MS_BOOL bEnable;
	MS_BOOL bReturn;
} ST_KDRV_DS_EnableIPMTuneAfterDS;

typedef struct __attribute__((packed)) {
	EN_KDRV_SC_DEVICE u32DeviceID;
	EN_KDRV_WINDOW eWindow;
	E_DS_CLIENT client;
	MS_U32 u32CmdRegAddr;
	MS_U16 u16CmdRegValue;
	K_ds_reg_ip_op_sel IPOP_Sel;
	K_ds_reg_source_sel Source_Select;
	K_XC_DS_CMDCNT *pstXC_DS_CmdCnt;
#if !defined (__aarch64__)
	MS_U32    pDummy;
#endif
} ST_KDRV_DS_WriteSWDSCommand;

typedef struct __attribute__((packed)) {
	EN_KDRV_SC_DEVICE u32DeviceID;
	EN_KDRV_WINDOW eWindow;
	E_DS_CLIENT client;
	MS_U32 u32Bank;
	MS_U32 u32Addr;
	MS_U16 u16Data;
	MS_U16 u16Mask;
	K_ds_reg_ip_op_sel IPOP_Sel;
	K_XC_DS_CMDCNT *pstXC_DS_CmdCnt;
#if !defined (__aarch64__)
	MS_U32    pDummy;
#endif
} ST_KDRV_DS_WriteSWDSCommandNonXC;

typedef struct __attribute__((packed)) {
	EN_KDRV_SC_DEVICE u32DeviceID;
	EN_KDRV_WINDOW eWindow;
	E_DS_CLIENT client;
	MS_U32 u32CmdRegAddr;
	MS_U16 u16CmdRegValue;
	MS_U16 u16Mask;
	K_ds_reg_ip_op_sel IPOP_Sel;
	K_ds_reg_source_sel Source_Select;
	K_XC_DS_CMDCNT *pstXC_DS_CmdCnt;
#if !defined (__aarch64__)
	MS_U32    pDummy;
#endif
} ST_KDRV_DS_WriteSWDSCommand_Mask;

typedef struct __attribute__((packed)) {
	EN_KDRV_SC_DEVICE u32DeviceID;
	EN_KDRV_WINDOW eWindow;
	E_DS_CLIENT client;
	K_ds_reg_ip_op_sel IPOP_Sel;
	K_XC_DS_CMDCNT *pstXC_DS_CmdCnt;
#if !defined (__aarch64__)
	MS_U32    pDummy;
#endif
} ST_KDRV_DS_Add_NullCommand;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	E_DS_CLIENT client;
	MS_U32 max_num;
	MS_BOOL bReturn;
} ST_KDRV_DS_SET_CLIENT;

typedef struct {
	MS_U8 u8SupportIndexNum;
} ST_KDRV_DS_GET_SUPPORT_INDEX_NUM;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	EN_KDRV_WINDOW eWindow;
	MS_U8 u8DSIndex;
} ST_KDRV_DS_GET_INDEX_INFO;

typedef struct {
	EN_KDRV_SC_DEVICE u32DeviceID;
	EN_KDRV_WINDOW eWindow;
} ST_KDRV_DS_FIRE_INDEX_INFO;

/* Define Window position and size attribute */
/* Note!!!! For CFD plus, DO not use this struct, Plz use Vdec Sharememory Struct */
typedef struct {
	MS_U16 u16x;           /*<start x of the window */
	MS_U16 u16y;           /*<start y of the window */
	MS_U16 u16width;       /*<width of the window */
	MS_U16 u16height;      /*<height of the window */
} ST_KDRV_WINDOW_TYPE;

typedef struct {
	MS_U8   u8CurrentIndex;
	MS_U64  phyRegAddr;      /*  HDR register */
	MS_U32  u32RegSize;
	MS_U64  phyLutAddr;      /*  look-up table */
	MS_U32  u32LutSize;
	MS_BOOL bDMEnable;
	MS_BOOL bCompEnable;
	ST_KDRV_WINDOW_TYPE stCapWin;/* real source resolution */
} ST_KDRV_XC_DS_HDRInfo;

typedef struct __attribute__((packed)) {
	EN_KDRV_WINDOW eWindow;
	EN_KDRV_SC_DEVICE u32DeviceID;
	K_XC_SETDS_Info *stDSInfo;
#if !defined (__aarch64__)
	MS_U32    pDummy;
#endif
} ST_KDRV_SET_XC_DS_Info;

typedef struct __attribute__((packed)) {
	EN_KDRV_WINDOW eWindow;
	EN_KDRV_SC_DEVICE u32DeviceID;
	K_XC_GETDS_Info *stGetDSInfo;
#if !defined (__aarch64__)
	MS_U32    pDummy;
#endif
} ST_KDRV_GET_XC_DS_Info;

/* Reference in E_CFD_CFIO  color_format_input.h */
typedef enum {
    /* start of RGB group */
	E_KDRV_XC_CFD_CFIO_RGB_NOTSPECIFIED      = 0x0, /* means RGB, but no specific colorspace */
	E_KDRV_XC_CFD_CFIO_RGB_BT601_625         = 0x1,
	E_KDRV_XC_CFD_CFIO_RGB_BT601_525         = 0x2,
	E_KDRV_XC_CFD_CFIO_RGB_BT709             = 0x3,
	E_KDRV_XC_CFD_CFIO_RGB_BT2020            = 0x4,
	E_KDRV_XC_CFD_CFIO_SRGB                  = 0x5,
	E_KDRV_XC_CFD_CFIO_ADOBE_RGB             = 0x6,
	E_KDRV_XC_CFD_CFIO_RGB_DCIP3_D65         = 0x7,
	E_KDRV_XC_CFD_CFIO_RGB_DCIP3_THEATER     = 0x8,
	E_KDRV_XC_CFD_CFIO_RGB_END               = 0x9,

    /* start of YUV group */
	E_KDRV_XC_CFD_CFIO_YUV_NOTSPECIFIED      = 0x80, /* means YUV, but no specific colorspace */
	E_KDRV_XC_CFD_CFIO_YUV_BT601_625         = 0x81,
	E_KDRV_XC_CFD_CFIO_YUV_BT601_525         = 0x82,
	E_KDRV_XC_CFD_CFIO_YUV_BT709             = 0x83,
	E_KDRV_XC_CFD_CFIO_YUV_BT2020_NCL        = 0x84, /* 132 */
	E_KDRV_XC_CFD_CFIO_YUV_BT2020_CL         = 0x85,
	E_KDRV_XC_CFD_CFIO_XVYCC_601             = 0x86,
	E_KDRV_XC_CFD_CFIO_XVYCC_709             = 0x87,
	E_KDRV_XC_CFD_CFIO_SYCC601               = 0x88,
	E_KDRV_XC_CFD_CFIO_ADOBE_YCC601          = 0x89,
	E_KDRV_XC_CFD_CFIO_YUV_END               = 0x8A,

    /* start of RGB/YUV group */
	E_KDRV_XC_CFD_CFIO_DOLBY_HDR_TEMP        = 0x8B,
	E_KDRV_XC_CFD_CFIO_RESERVED_START
} EN_KDRV_XC_CFD_CFIO;

typedef struct {
	MS_U32 u32Version;
	MS_U32 u32Length;
    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* data size */
	MS_U32 u32GammutSize;
    /* pointer to gamma B */
	MS_U16 *pu16SDRGammut;
#if !defined (__aarch64__)
    /* Dummy parameter */
    void *pDummy;
#endif
 } ST_KDRV_XC_CFD_SDRGAMMUT;

typedef struct {
	MS_U32 u32Version;
	MS_U32 u32Length;
	MS_S16 s16YOffset;
	MS_U16 u16YGain;
	MS_S16 s16COffset;
	MS_U16 u16CGain;
 } ST_KDRV_XC_CFD_YC_OffsetGain;

typedef struct {
	MS_U32 u32Version;
	MS_U32 u32Length;
	MS_S16 s16YOffsetIn;
	MS_U16 u16YGainIn;
	MS_S16 s16COffsetIn;
	MS_U16 u16CGainIn;
	MS_S16 s16YOffsetOut;
	MS_U16 u16YGainOut;
	MS_S16 s16COffsetOut;
	MS_U16 u16CGainOut;
 } ST_KDRV_XC_CFD_XC_YC_OffsetGainBoth;

typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;
    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* Gamut mmaping 3*3 data */
	MS_S32 s32GamutLutData[9];
 } ST_KDRV_XC_CFD_GAMUTLUT_PREVIOUS;

typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;
    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* Gamma enable/disable */
	MS_BOOL bSDRGammaEnable;
    /* Degamma enable/disable */
	MS_BOOL bSDRDegammaEnable;
    /* SDR 3D LUT enable */
	MS_BOOL bSDR3DLUTEnable;
    /* Gamut mmaping 3*3 data */
	MS_S32 s32GamutLutData[9];
 } ST_KDRV_XC_CFD_GAMUTLUT_POST;

typedef struct {
	MS_U32 u32Version;
	MS_U32 u32Length;
    /* Window (main or sub window)
	MS_U8 u8Win;

    /* pointer to gamma R
	MS_U8 *pu8GammaR;
#if !defined (__aarch64__)
    /* Dummy parameter
    void *pDummy1;
#endif
    /* data size
	MS_U32 u32GammaRSize;

    /* pointer to gamma G
	MS_U8 *pu8GammaG;
#if !defined (__aarch64__)
    /* Dummy parameter
    void *pDummy2;
#endif
    /* data size
	MS_U32 u32GammaGSize;

    /* pointer to gamma B
	MS_U8 *pu8GammaB;
#if !defined (__aarch64__)
    /* Dummy parameter
    void *pDummy3;
#endif
    /* data size
	MS_U32 u32GammaBSize;
 } ST_KDRV_XC_CFD_SDRGAMMA;

typedef struct {
	MS_U32 u32Version;
	MS_U32 u32Length;
    /* Window (main or sub window)
	MS_U8 u8Win;

    /* pointer to gamma R
	MS_U8 *pu8DegammaR;
#if !defined (__aarch64__)
    /* Dummy parameter
    void *pDummy1;
#endif
    /* data size
	MS_U32 u32DegammaRSize;

    /* pointer to gamma G
	MS_U8 *pu8DegammaG;
#if !defined (__aarch64__)
    /* Dummy parameter
    void *pDummy2;
#endif
    /* data size
	MS_U32 u32DegammaGSize;

    /* pointer to gamma B
	MS_U8 *pu8DegammaB;
#if !defined (__aarch64__)
    /* Dummy parameter
    void *pDummy3;
#endif
    /* data size
	MS_U32 u32DegammaBSize;
} ST_KDRV_XC_CFD_SDRDEGAMMA;

/* SWDR information
typedef struct __attribute__((packed)) {
	MS_BOOL bDRE_En;                            /* 1: enable; 0: disable */
	MS_BOOL bDRE_SWDR_En;                       /* 1: enable; 0: disable */
	MS_U16  *pu16Data;
#if !defined (__aarch64__)
    void *pDummy;
#endif
	MS_U16  u16NumofData;
	MS_U16  u16DataIndex;
	EN_KDRV_WINDOW eWindow;
	MS_U8   u8DRE_DR_Set_BasicStrength;         /* default = 4, max = 32 */
	MS_U8   u8DRE_SWDR_Set_BasicStrength;       /* default = 4, max = 32 */
	MS_U16  u16DRE_SWDR_Set_StrengthDk;         /* dentity = 0x100; max = 0x200 */
	MS_U16  u16DRE_SWDR_Set_StrengthBr;         /* dentity = 0x100; max = 0x200 */ */
	MS_U8   u8DRE_SWDR_Set_SceneProtectLevel;   /* default = 0x08, max = 0x20 */
	MS_U8   u8DRE_Set_TemporalFilterLevel;      /* default = 0x06, max = 0x20 */ */
	MS_U8   u8DRE_Set_ColorCorrectLevel;        /* default = 0x10, max = 0x20 */
 } ST_KDRV_XC_SWDR_INFO;

typedef enum {
	E_KDRV_XC_SWDR_HIST_TYPE_PMode                 = 0x0,
	E_KDRV_XC_SWDR_HIST_TYPE_IFrameMode            = 0x1,
	E_KDRV_XC_SWDR_HIST_TYPE_I1stFieldMode         = 0x2,
	E_KDRV_XC_SWDR_HIST_TYPE_I2ndFieldMode         = 0x3,
	E_KDRV_XC_SWDR_HIST_TYPE_NULL                  = 0xFF,
} EN_KDRV_XC_SWDR_HIST_TYPE;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version; /* <Version of current structure. Please always set to "SWDR_INPUT_STRUCTURE_VERSION" as input */
	MS_U32 u32Length; /* <Length of this structure, u16Length=sizeof(XC_SWDR_Input) */
	MS_U32 *pHist_Curr; /*  ( Top + Bottom ) 24bits * 32bins * 2 */
#if !defined (__aarch64__)
    void *pDummy0;    /* Dummy parameter */
#endif
	MS_U32 *pHist_Prev; /*  ( Top + Bottom ) 24bits * 32bins * 2 */
#if !defined (__aarch64__)
    void *pDummy1;    /* Dummy parameter */
#endif
	MS_U16 u16Hist_size; /*  32bins * 2 */
	MS_U16 *pOOTFLut; /*  1.15bits * 512bins */
#if !defined (__aarch64__)
    void *pDummy2;    /* Dummy parameter */
#endif
	MS_U16 u16OOTF_size; /*  512 bins */
	EN_KDRV_XC_SWDR_HIST_TYPE  eHist_type; /*  _DRV_HIST_TYPE; */
	MS_U8  u8Src_type; /*  _E_CFD_CFIO_TR */
	MS_U8  u8Out_type; /*  _E_CFD_CFIO_TR */
	EN_KDRV_WINDOW  eWindow;
	MS_U8 *pu8IpHistAulData;
#if !defined (__aarch64__)
    void *pDummy3;    /* Dummy parameter
#endif
	MS_U16 u16IpHistAulData_size;
 } ST_KDRV_XC_SWDR_INPUT;

typedef enum {
	E_KDRV_VIDEO_CODEC_UNKNOWN = -1,     /* Video codec type is unknown. */
	E_KDRV_VIDEO_CODEC_MPEG4,            /* Video codec type is MPEG 4. */
	E_KDRV_VIDEO_CODEC_MJPEG,            /* Video codec type is motion JPG. */
	E_KDRV_VIDEO_CODEC_H264,             /* Video codec type is H264. */
	E_KDRV_VIDEO_CODEC_RM,               /* Video codec type is RealVideo. */
	E_KDRV_VIDEO_CODEC_TS,               /* Video codec type is TS File. */
	E_KDRV_VIDEO_CODEC_MPEG,             /* Video codec type is MPEG 1/2. */
	E_KDRV_VIDEO_CODEC_VC1,              /* Video codec type is VC1. */
	E_KDRV_VIDEO_CODEC_AVS,              /* Video codec type is Audio Video Standard. */
	E_KDRV_VIDEO_CODEC_FLV,              /* Video codec type is FLV. */
	E_KDRV_VIDEO_CODEC_MVC,              /* Video codec type is MVC. */
	E_KDRV_VIDEO_CODEC_VP6,              /* Video codec type is VP6. */
	E_KDRV_VIDEO_CODEC_VP8,              /* Video codec type is VP8. */
	E_KDRV_VIDEO_CODEC_HEVC,             /* Video codec type is HEVC. */
	E_KDRV_VIDEO_CODEC_VP9,              /* Video codec type is VP9. */
} EN_KDRV_XC_VIDEO_CODEC;

typedef enum {
	E_KDRV_FRAME_TYPE_I,
	E_KDRV_FRAME_TYPE_P,
	E_KDRV_FRAME_TYPE_B,
	E_KDRV_FRAME_TYPE_OTHER,
	E_KDRV_FRAME_TYPE_MAX,
} EN_KDRV_XC_FRAMETYPE;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version; /*  <Version of current structure. Please always set to "FRAME_INFO_STRUCTURE_VERSION" as input */
	MS_U32 u32Length;  /*  <Length of this structure, u16Length=sizeof(ST_KDRV_XC_VDEC_INFO) */
	EN_KDRV_WINDOW  eWindow;
	EN_KDRV_XC_SWDR_HIST_TYPE enScanType; /*  scan type (P/I frame/I field/NULL) */
	MS_U32 u32QPMin; /*  Min of quantization parameter */
	MS_U32 u32QPAvg; /*  Avg of quantization parameter */
	MS_U32 u32QPMax; /*  Max of quantization parameter */
	MS_U8 u8BotFlag; /*  Bottom field = 1, top field = 0 */
	EN_KDRV_XC_VIDEO_CODEC enCodecType; /*  code type (H264/MPEG...) */
	EN_KDRV_XC_FRAMETYPE enFrameType; /*  Frame type (I/B/P) */
	EN_KDRV_XC_HDR_COLOR_FORMAT enStreamColorFormat; /* YUV420/YUV422/YUV444/RGB */
 } ST_KDRV_XC_FRAME_INFO;



typedef struct __attribute__((packed)) {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U32 u32Length;

    /* Enable user's colorformat (User's or CFD control) */
	MS_BOOL bEnableUserColorimetry;
	EN_KDRV_XC_CFD_CFIO enOutputColorimetry;
} ST_KDRV_XC_CFD_SET_OUTPUT_COLORIMETRY;

typedef struct __attribute__((packed)) {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U32 u32Length;

    /* Enable user's HDR type (User's or CFD control) */
	MS_BOOL bEnableUserHDRType;
	EN_KDRV_XC_HDR_TYPE enOutputHDRType;
} ST_KDRV_XC_CFD_SET_OUTPUT_HDR_TYPE;

typedef struct {
	MS_U8 u8Vid;
	MS_U8 u8DeflickerEn;
	MS_U8 u8SnrMotionEn;
	MS_U8 u8DefaultSnrGain;
	MS_U8 u8DeflickerStr;
	MS_U8 u8HiCplxStr;
 } ST_KDRV_PQ_DEFLICKER;

typedef struct {
	MS_U8  u8ActionMode;
	MS_U32 u32NumofReg;
	MS_U16 u16ClientID;
} ST_KDRV_PQ_SAVE_BIN_INFO;

typedef enum {
    /* Customer */
	E_KDRV_XC_PQ_BIN_PATH_CUSTOMER,
    /* Default */
	E_KDRV_XC_PQ_BIN_PATH_DEFAULT,
    /* INI */
	E_KDRV_XC_PQ_BIN_PATH_INI,
    /* Bandwidth */
	E_KDRV_XC_PQ_BIN_PATH_BANDWIDTH,
    /* The max support number of paths */
	E_KDRV_XC_PQ_BIN_PATH_MAX
} EN_KDRV_XC_PQ_BIN_PATH;

typedef struct __attribute__((packed)) {
	EN_KDRV_XC_PQ_BIN_PATH enXCPqBinPath;
	MS_U8 u8size;
	char *b_PQBinFilePath;
#if !defined (__aarch64__)
    void *pDummy0;    /* Dummy parameter */
#endif
} ST_KDRV_XC_SET_PQBINPATH;


/*CFD pq gamma channel */
typedef enum {
	E_KDRV_XC_CFD_PQ_GAMMA_CHANNEL_R,
	E_KDRV_XC_CFD_PQ_GAMMA_CHANNEL_G,
	E_KDRV_XC_CFD_PQ_GAMMA_CHANNEL_B,
	E_KDRV_XC_CFD_PQ_GAMMA_CHANNEL_MAX
} EN_KDRV_XC_CFD_PQ_GAMMA_CHANNEL_TYPE;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
	MS_U16 u16X;
	MS_U16 u16Y;
	MS_U16 u16Width;
	MS_U16 u16Height;
} ST_KDRV_XC_WIN_INFO;

/* HSY setting */
typedef enum {
	E_KDRV_XC_HSY_HUE_SETTING,
	E_KDRV_XC_HSY_SAT_SETTING,
	E_KDRV_XC_HSY_LUMA_SETTING,
	E_KDRV_XC_HSY_NUMS,
} EN_KDRV_XC_HSY_FUNC;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;               /*  Struct version */
	MS_U16 u16Length;                /*  Sturct length */

	EN_KDRV_XC_HSY_FUNC  enXCHSYFunc; /*  HSY setting */
	MS_S32 *ps32Data;                /* degrees or gain value */
#if !defined (__aarch64__)
    void    *pDummy0;
#endif
	MS_U8 u8DataLength;             /* The data length of Setting */
	MS_U8 u8Win;                        /*  0:main window 1:sub window */
 } ST_KDRV_XC_HSY_SETTING_INFO;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;               /*  Struct version */
	MS_U16 u16Length;                /*  Sturct length */

	EN_KDRV_XC_HSY_FUNC  enXCHSYFunc; /*  HSY setting */
	MS_S32 *ps32MinValue;
#if !defined (__aarch64__)
    void    *pDummy0;
#endif
	MS_S32 *ps32MaxValue;
#if !defined (__aarch64__)
    void    *pDummy1;
#endif
	MS_U8 *pu8NumofRegion;            /*  The number of hue region */
#if !defined (__aarch64__)
    void    *pDummy2;
#endif
	MS_U8 u8Win;                        /*  0:main window 1:sub window */
 } ST_KDRV_XC_HSY_ADJ_RANGE_INFO;

typedef struct {
	MS_U32 u32Version;               /*  Struct version */
	MS_U16 u16Length;                /*  Sturct length */

	MS_U8 u8Win;
	MS_U8 u8Bff;                     /* 0: top first  1: bottom first */
	MS_S32 s32Width;
	MS_S32 s32Height;
	MS_BOOL bFieldOrderChange;               /* the result of field order detection */
} ST_KDRV_XC_FOD_INFO;

/* DMA info
typedef struct {
	MS_U8 u8FrameNumber;
	MS_U8 u8MiuSel;
	MS_U8 u8IPMCurFrameNum;
	MS_U32 u32Addr;
	MS_U8 u8BytePerWord;
} ST_KDRV_XC_DMA_INFO;

typedef enum {
	E_KDRV_XC_MEMORY_TYPE_AUTOUPLOAD,
	E_KDRV_XC_MEMORY_TYPE_CMDQ,
	E_KDRV_XC_MEMORY_TYPE_MAX,
} EN_KDRV_XC_MEMORY_TYPE;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;               /*  Struct version */
	MS_U32 u32Length;                /*  Struct size */
	EN_KDRV_XC_MEMORY_TYPE enType;   /*  memory type */
	MS_U64 u64BaseAddr;              /*  Memory address */
	MS_U32 u32Size;                  /*  size */
} ST_KDRV_XC_MEMORY_INFO;

/* cfd ctrl for ST_KDRV_XC_CFD_CONTROL_INFO */
typedef enum {
    /* Initialize, refer to ST_KDRV_XC_CFD_INIT */
	E_KDRV_XC_CFD_CTRL_SET_INIT = 0,
    /* VGA, refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_VGA = 1,
    /* TV (ATV), refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_TV = 2,
    /* CVBS (AV), refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_CVBS = 3,
    /* S-Video, refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_SVIDEO = 4,
    /* YPbPr, refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_YPBPR = 5,
    /* Scart, refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_SCART = 6,
    /* HDMI, refer to ST_KDRV_XC_CFD_HDMI */
	E_KDRV_XC_CFD_CTRL_SET_HDMI = 7,
    /* DTV, refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_DTV = 8,
    /* DVI, refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_DVI = 9,
    /* MM, refer to ST_KDRV_XC_CFD_ANALOG */
	E_KDRV_XC_CFD_CTRL_SET_MM = 10,
    /* Panel, refer to ST_KDRV_XC_CFD_PANEL */
	E_KDRV_XC_CFD_CTRL_SET_PANEL = 11,
    /* HDR, refer to ST_KDRV_XC_CFD_HDR */
	E_KDRV_XC_CFD_CTRL_SET_HDR = 12,
    /* EDID, refer to ST_KDRV_XC_CFD_EDID */
	E_KDRV_XC_CFD_CTRL_SET_EDID = 13,
    /* OSD, refer to ST_KDRV_XC_CFD_OSD */
	E_KDRV_XC_CFD_CTRL_SET_OSD = 14,
    /* Fire, refer to ST_KDRV_XC_CFD_FIRE */
	E_KDRV_XC_CFD_CTRL_SET_FIRE = 15,
    /* DLC, refer to ST_KDRV_XC_CFD_DLC */
	E_KDRV_XC_CFD_CTRL_SET_DLC = 16,
    /* Linear RGB, refer to ST_KDRV_XC_CFD_LINEAR_RGB */
	E_KDRV_XC_CFD_CTRL_SET_LINEAR_RGB = 17,
    /* HDMI out status, refer to ST_KDRV_XC_CFD_HDMI */
	E_KDRV_XC_CFD_CTRL_GET_HDMI_STATUS = 18,
    /* Set force HDR Onoff or auto select setting */
	E_KDRV_XC_CFD_CTRL_SET_HDR_ONOFF_SETTING = 19,
    /* Status */
	E_KDRV_XC_CFD_CTRL_GET_STATUS = 20,
    /* TMO */
	E_KDRV_XC_CFD_CTRL_SET_TMO = 21,
    /* Update Status */
	E_KDRV_XC_CFD_CTRL_UPDATE_STATUS = 22,
    /* Get Picture Attributes */
	E_KDRV_XC_CFD_CTRL_PICTURE_ATTRIBUTE = 23,
    /* Update Info into CFD */
	E_KDRV_XC_CFD_CTRL_UPDATE_INFO = 24,
    /* Set HDMI Seamless */
	E_KDRV_XC_CFD_CTRL_SET_HDMI_SEAMLESS = 26,
    /* Get Status of Paring HDMI Packet */
	E_KDRV_XC_CFD_CTRL_GET_PARSING_HDMI_PACKET = 27,
    /* Set QMAP Data */
	E_KDRV_XC_CFD_CTRL_SET_QMAPDATA = 28,
    /* Get Histogram */
	E_KDRV_XC_CFD_CTRL_GET_IP_HISTOGRAM = 29,
    /*3DLUT */
	E_KDRV_XC_CFD_CTRL_3DLUT = 30,
    /*EOTF */
	E_KDRV_XC_CFD_CTRL_EOTF = 31,
    /*OETF */
	E_KDRV_XC_CFD_CTRL_OETF = 32,
    /*HLG GAIN */
	E_KDRV_XC_CFD_CTRL_HLGGAIN = 33,
    /*GAMMA enable/disable */
	E_KDRV_XC_CFD_CTRL_GAMMA_ENABLE = 34,
    /*Gamut Pre Control */
	E_KDRV_XC_CFD_CTRL_GAMUT_PREVIOUS = 35,
    /*Gamut Post Control */
	E_KDRV_XC_CFD_CTRL_GAMUT_POST = 36,
    /*SDR Gamma Control */
	E_KDRV_XC_CFD_CTRL_SDR_GAMMA = 37,
    /*SDR Degamma Control */
	E_KDRV_XC_CFD_CTRL_SDR_DEGAMMA = 38,
    /*Set Yc offset and gain in */
	E_KDRV_XC_CFD_CTRL_YCOFFSETGAININ = 39,
    /*Set Yc offset and gain out */
	E_KDRV_XC_CFD_CTRL_YCOFFSETGAINOUT = 40,
    /*Set Yc offset and gain in Sub */
	E_KDRV_XC_CFD_CTRL_YCOFFSETGAININSUB = 41,
    /*SDR 3DLUT Control */
	E_KDRV_XC_CFD_CTRL_SDR_3DLUT = 42,
    /* CFD_CTRL_PICTURE_MODE */
	E_KDRV_XC_CFD_CTRL_PICTURE_MODE = 43,
    /* Set Customer IP parameter */
	E_KDRV_XC_CFD_CTRL_UPDATE_CUSTOMER_SETTING = 44,
    /* Get CFD version */
	E_KDRV_XC_CFD_CTRL_GET_CFD_VERSION = 45,
    /* Update XC status into CFD, refer to ST_KDRV_XC_STATUS_INFO */
	E_KDRV_XC_CFD_CTRL_UPDATE_XC_STATUS = 46,
    /* CFD_CTRL_TMO_CUS_MAPPING */
	E_KDRV_XC_CFD_CTRL_TMO_CUS_MAPPING = 47,
    /* NR SETTINGS */
	E_KDRV_XC_CFD_CTRL_SET_NR_SETTINGS = 48,
    /* Set Metadata Path */
	E_KDRV_XC_CFD_SET_METADATA_PATH = 49,
    /* Set Metadata to CFD */
	E_KDRV_XC_CFD_SET_METADATA = 50,
    /* set manual luma curve */
	E_KDRV_XC_CFD_CTRL_SET_MANUAL_LUMA_CURVE = 51,
    /* Get Luma Info */
	E_KDRV_XC_CFD_CTRL_GET_LUMA_INFO = 52,
    /* Set Stretch Settings */
	E_KDRV_XC_CFD_CTRL_SET_STRETCH_SETTINGS = 53,
    /* Get Chroma Info */
	E_KDRV_XC_CFD_CTRL_GET_CHROMA_INFO = 54,
    /* CFD CTRL_TYPE_EX start */
    /* (Set this base is 0x40000000 for int type) */
	E_KDRV_XC_CFD_CTRL_TYPE_EX_BASE = 0x40000000,
    /* OSD_STATUS */
	E_KDRV_XC_CFD_CTRL_GET_OSD_PROCESS_CONFIGS = 0x40000001,
    /* Set Seamless */
	E_KDRV_XC_CFD_CTRL_SET_OUTPUT_SEAMLESS = 0x40000002,
    /* HDR Output Colorimetry */
	E_KDRV_XC_CFD_CTRL_SET_OUTPUT_COLORIMETRY = 0x40000003,
    /* HDR Output HDR Type */
	E_KDRV_XC_CFD_CTRL_SET_OUTPUT_HDR_TYPE = 0x40000004,
	E_KDRV_XC_CFD_CTRL_MAX,
} EN_KDRV_XC_CFD_CTRL_TYPE;

typedef enum {
	E_KDRV_XC_HDR_METADATA_AUTO = 0,     /*Set Metadata to CFD by vdec, default path */
	E_KDRV_XC_HDR_METADATA_STATIC = 1,   /* Set Metadata to CFD by uper layer, such as AN P */
	E_KDRV_XC_HDR_METADATA_MAX,
} EN_KDRV_XC_HDR_METADATA_PATH;


/* struct for MDRV_XC_IOC_CFDCONCTRL */
typedef struct  __attribute__((packed)) {
	EN_KDRV_XC_CFD_CTRL_TYPE enCtrlType;
    void *pParam;
#if !defined (__aarch64__)
    void *pDummy;
#endif
	MS_U32 u32ParamLen; /* param length */
	MS_U16 u16ErrCode;
} ST_KDRV_XC_CFD_CONTROL_INFO;

/* cfd ctrl: set init */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* Input source */
	MS_U8 u8InputSource;
} ST_KDRV_XC_CFD_INIT;

/* cfd ctrl: set vga/tv/cvbs/svideo/ypbpr/scart/mm/dvi */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Window (main or sub window) */
	MS_U8 u8Win;

    /* Color format, reference EN_KDRV_XC_CFD_COLOR_FORMAT. */
	MS_U8 u8ColorFormat;
    /* Color data format, reference EN_KDRV_XC_CFD_COLOR_DATA_FORMAT. */
	MS_U8 u8ColorDataFormat;
    /* Full range */
	MS_BOOL bIsFullRange;
    /* Color primaries */
	MS_U8 u8ColorPrimaries;
    /* Transfer characteristics */
	MS_U8 u8TransferCharacteristics;
    /* Matrix coefficients */
	MS_U8 u8MatrixCoefficients;
} ST_KDRV_XC_CFD_ANALOG;

/* hdr10 plus vsif for ST_KDRV_XC_CFD_HDMI, refer to  STU_CFD_HDR10PLUS_VSIF in mdrv_alg_interface.h */
typedef struct {
	MS_U16 u16Version;      /* Version. */
	MS_U16 u16Size;         /* Structure size. */
	MS_BOOL bValid;         /* if vsif info frame is valid */
	MS_U8 u8VSIFTypeCode;
	MS_U8 u8VSIFVersion;
	MS_U8 u8Length;
	MS_U32 u32IEEECode;
	MS_U8 u8ApplicationVersion;
	MS_U8 u8TargetSystemDisplayMaxLuminance;
	MS_U8 u8AverageMaxRGB;
	MS_U8 au8DistributionValues[9];
	MS_U16 u16KneePointX;
	MS_U16 u16KneePointY;
	MS_U8 u8NumBezierCurveAnchors;
	MS_U8 au8BezierCurveAnchors[9];
	MS_U8 u8GraphicsOverlayFlag;
	MS_U8 u8NoDelayFlag;
} ST_KDRV_HDMI_VSIF_PACKET_HDR10_PLUS;

/* cfd ctrl: set hdmi and get hdmi status */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Window (main or sub window) */
	MS_U8 u8Win;

    /* Full range */
	MS_BOOL bIsFullRange;

    /* AVI infoframe */
    /* Pixel Format */
	MS_U8 u8PixelFormat;
    /* Color imetry */
	MS_U8 u8Colorimetry;
    /* Extended Color imetry */
	MS_U8 u8ExtendedColorimetry;
    /* Rgb Quantization Range */
	MS_U8 u8RgbQuantizationRange;
    /* Ycc Quantization Range */
	MS_U8 u8YccQuantizationRange;

    /* HDR infoframe */
    /* HDR infoframe valid */
	MS_BOOL bHDRInfoFrameValid;
    /* EOTF (/* 0:SDR gamma, 1:HDR gamma, 2:SMPTE ST2084, 3:Future EOTF, 4-7:Reserved) */
	MS_U8 u8EOTF;
    /* Static metadata ID (0: Static Metadata Type 1, 1-7:Reserved for future use) */
	MS_U8 u8SMDID;
    /* Display primaries x */
	MS_U16 u16Display_Primaries_x[3];
    /* Display primaries y */
	MS_U16 u16Display_Primaries_y[3];
    /* White point x */
	MS_U16 u16White_point_x;
    /* White point y */
	MS_U16 u16White_point_y;
    /* Panel max luminance */
	MS_U16 u16MasterPanelMaxLuminance;
    /* Panel min luminance */
	MS_U16 u16MasterPanelMinLuminance;
    /* Max content light level */
	MS_U16 u16MaxContentLightLevel;
    /* Max frame average light level */
	MS_U16 u16MaxFrameAvgLightLevel;
} ST_KDRV_XC_CFD_HDMI;

/* cfd ctrl: set panel
typedef struct {
    /* Structure version, refer to CFD_PANEL_VERSION */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Color format, reference EN_KDRV_XC_CFD_COLOR_FORMAT. */
	MS_U8 u8ColorFormat;
    /* Color data format, reference EN_KDRV_XC_CFD_COLOR_DATA_FORMAT. */
	MS_U8 u8ColorDataFormat;
    /* Full range */
	MS_BOOL bIsFullRange;

    /* Display primaries x, data *0.00002 0xC350 = 1 */
	MS_U16 u16Display_Primaries_x[3];
    /* Display primaries y, data *0.00002 0xC350 = 1 */
	MS_U16 u16Display_Primaries_y[3];
    /* White point x, data *0.00002 0xC350 = 1 */
	MS_U16 u16White_point_x;
    /* White point y, data *0.00002 0xC350 = 1 */
	MS_U16 u16White_point_y;
    /* Max luminance, data * 1 nits */
	MS_U16 u16MaxLuminance;
    /* Med luminance, data * 1 nits */
	MS_U16 u16MedLuminance;
    /* Min luminance, data * 0.0001 nits */
	MS_U16 u16MinLuminance;
    /* Linear RGB */
	MS_BOOL bLinearRgb;
    /* Customer color primaries */
	MS_BOOL bCustomerColorPrimaries;
    /* Source white x */
	MS_U16 u16SourceWx;
    /* Source white y */
	MS_U16 u16SourceWy;
    /* PWM port */
	MS_U8 u8PWMPort;
    /* dolby global dimming on/off */
	MS_BOOL bGlobalDimming;
    /* OTT dolby global delay frame count */
	MS_S8 s8DelayFrame;
    /* HDMI dolby global delay frame count */
	MS_S8 s8HDMIDelayFrame;
} ST_KDRV_XC_CFD_PANEL;

/* cfd ctrl: set hdr */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* HDR type, reference EN_KDRV_XC_CFD_HDR_TYPE */
	MS_U8 u8HdrType;
    /*  HDR10/HLG TMO level. 0: low, 1: middle, 2: high */
	MS_U8 u8TmoLevel;
    /* reference mode parameter invalid */
	MS_BOOL bRefModeValid;
    /* reference mode enable */
	MS_BOOL bRefMode;
} ST_KDRV_XC_CFD_HDR;

/* hdr type for ST_KDRV_XC_CFD_HDR */
typedef enum {
    /* None HDR */
	E_KDRV_XC_CFD_HDR_TYPE_NONE = 0x0000,
    /* Dolby HDR */
	E_KDRV_XC_CFD_HDR_TYPE_DOLBY = 0x0001,
    /* Open HDR (HDR10) */
	E_KDRV_XC_CFD_HDR_TYPE_OPEN = 0x0002,
    /* TCH */
	E_KDRV_XC_CFD_HDR_TYPE_TCH = 0x0004,
    /* HLG */
	E_KDRV_XC_CFD_HDR_TYPE_HLG = 0x0008,
    /* HDR10 plus */
	E_KDRV_XC_CFD_HDR_TYPE_HDR10_PLUS = 0x0010,
    /* Open HDR in DV (Internal use) */
	E_KDRV_XC_CFD_HDR_TYPE_OPEN_IN_DOLBY = 0x0100,
    /* HDMI Low Latency in DV (Internal use) */
	E_KDRV_XC_CFD_HDR_TYPE_LOWLATENCY_HDMI_IN_DOLBY = 0x0101,
    /* HDMI Low Latency RGB in DV (Internal use) */
	E_KDRV_XC_CFD_HDR_TYPE_LOWLATENCY_RGB_HDMI_IN_DOLBY = 0x0110,
    /* Max
	E_KDRV_XC_CFD_HDR_TYPE_MAX = 0xffff
} EN_KDRV_XC_CFD_HDR_TYPE;

typedef enum {
	E_KDRV_XC_HDR_OUTPUT_MODE_AUTO  = 0x0,
	E_KDRV_XC_HDR_OUTPUT_MODE_ON    = 0x1,
	E_KDRV_XC_HDR_OUTPUT_MODE_OFF   = 0x2,
	E_KDRV_XC_HDR_OUTPUT_MODE_MAX,
} EN_KDRV_XC_HDR_OUTPUT_MODE;

/* cfd ctrl: set edid */
typedef struct {
	MS_U32 u32Version;   /*<Version of current structure. Please always set to "CFD_HDMI_EDID_ST_VERSION" as input */
	MS_U16 u16Length;    /*<Length of this structure, u16Length=sizeof(STU_CFDAPI_HDMI_EDID_PARSER) */

	MS_U8 u8HDMISinkHDRDataBlockValid;
    /* assign by E_CFD_VALIDORNOT */
    /* 0 :Not valid */
    /* 1 :valid */

	MS_U8 u8HDMISinkEOTF;
    /* byte 3 in HDR static Metadata Data block */

	MS_U8 u8HDMISinkSM;
    /* byte 4 in HDR static Metadata Data block */

	MS_U8 u8HDMISinkDesiredContentMaxLuminance;           /* need a LUT to transfer */
	MS_U8 u8HDMISinkDesiredContentMaxFrameAvgLuminance; /* need a LUT to transfer */
	MS_U8 u8HDMISinkDesiredContentMinLuminance;           /* need a LUT to transfer */
    /* byte 5 ~ 7 in HDR static Metadata Data block */

	MS_U8 u8HDMISinkHDRDataBlockLength;
    /* byte 1[4:0] in HDR static Metadata Data block */

    /* order R->G->B */
    /* MS_U16 u16display_primaries_x[3];                       /* data *1/1024 0x03FF = 0.999 */
    /* MS_U16 u16display_primaries_y[3];                       /* data *1/1024 0x03FF = 0.999 */
    /* MS_U16 u16white_point_x;                                /* data *1/1024 0x03FF = 0.999 */
    /* MS_U16 u16white_point_y;                                /* data *1/1024 0x03FF = 0.999 */
    /* Display primaries x, data *0.00002 0xC350 = 1 */
	MS_U16 u16Display_Primaries_x[3];
    /* Display primaries y, data *0.00002 0xC350 = 1 */
	MS_U16 u16Display_Primaries_y[3];
    /* White point x, data *0.00002 0xC350 = 1 */
	MS_U16 u16White_point_x;
    /* White point y, data *0.00002 0xC350 = 1 */
	MS_U16 u16White_point_y;
    /* address 0x19h to 22h in base EDID */

	MS_U8 u8HDMISinkEDIDBaseBlockVersion;                /* for debug
    /* address 0x12h in EDID base block */

	MS_U8 u8HDMISinkEDIDBaseBlockReversion;              /* for debug */
    /* address 0x13h in EDID base block */

	MS_U8 u8HDMISinkEDIDCEABlockReversion;               /* for debug */
    /* address 0x01h in EDID CEA block */

    /* table 59 Video Capability Data Block (VCDB) */
    /* 0:VCDB is not avaliable */
    /* 1:VCDB is avaliable */
	MS_U8 u8HDMISinkVCDBValid;

	MS_U8 u8HDMISinkSupportYUVFormat;
    /* bit 0:Support_YUV444 */
    /* bit 1:Support_YUV422 */
    /* bit 2:Support_YUV420 */

    /* QY in Byte#3 in table 59 Video Capability Data Block (VCDB) */
    /* bit 3:RGB_quantization_range */

    /* QS in Byte#3 in table 59 Video Capability Data Block (VCDB) */
    /* bit 4:Y_quantization_range 0:no data(due to CE or IT video) ; 1:selectable */


	MS_U8 u8HDMISinkExtendedColorspace;
    /* byte 3 of Colorimetry Data Block */
    /* bit 0:xvYCC601 */
    /* bit 1:xvYCC709 */
    /* bit 2:sYCC601 */
    /* bit 3:Adobeycc601 */
    /* bit 4:Adobergb */
    /* bit 5:BT2020 cl */
    /* bit 6:BT2020 ncl */
    /* bit 7:BT2020 RGB */

	MS_U8 u8HDMISinkEDIDValid;
} ST_KDRV_XC_CFD_EDID;

/* cfd ctrl: set osd */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Hue */
	MS_U16 u16Hue;
    /* Saturation */
	MS_U16 u16Saturation;
    /* Contrast */
	MS_U16 u16Contrast;

    /* 0:off */
    /* 1:on */
    /* default on , not in the document */
	MS_U8  u8OSDUIEn;

    /* Mode 0: update matrix by OSD and color format driver */
    /* Mode 1: only update matrix by OSD controls */
    /* for mode1 : the configures of matrix keep the same as the values by calling CFD last time */
	MS_U8  u8OSDUIMode;

    /*
	MS_BOOL bHueValid;
	MS_BOOL bSaturationValid;
	MS_BOOL bContrastValid;

	MS_BOOL bBacklightValid;
	MS_U32 u32MinBacklight;
	MS_U32 u32MaxBacklight;
	MS_U32 u32Backlight;

	MS_U8 u8Win;
	MS_BOOL bColorRangeValid;
	MS_U8 u8ColorRange; /*  0: Auto, 1: Full Range (0~255), 2: Limit Range (16~235) */

	MS_BOOL bUltraBlackLevelValid;
	MS_U8 u8UltraBlackLevel;
	MS_BOOL bUltraWhiteLevelValid;
	MS_U8 u8UltraWhiteLevel;

	MS_BOOL bSkipPictureSettingValid;
	MS_BOOL bSkipPictureSetting;

	MS_BOOL bColorCorrectionValid;
	MS_S16 s16ColorCorrectionMatrix[32];

	MS_BOOL bYVUtoRGBMatrixValid;
	MS_S16 s16YVUtoRGBMatrix[32];

	MS_U16 u16R;
	MS_U16 u16G;
	MS_U16 u16B;

	MS_BOOL bRValid;
	MS_BOOL bGValid;
	MS_BOOL bBValid;

    /* 0:auto depends on STB rule */
    /* 1:always do HDR2SDR for HDR input */
    /* 2:always not do HDR2SDR for HDR input */
	MS_U8  u8HDR_UI_H2SMode;

	MS_BOOL bViewModeValid;
	MS_U8 u8ViewMode;

	MS_BOOL bSharpnessValid;
	MS_U8 u8Sharpness;
} ST_KDRV_XC_CFD_OSD;

/* cfd ctrl: set_fire */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* Input source */
	MS_U8 u8InputSource;
    /* Update type, reference EN_KDRV_XC_CFD_UPDATE_TYPE */
	MS_U8 u8UpdateType;
    /* RGB bypass */
	MS_BOOL bIsRgbBypass;
    /* HD mode */
	MS_BOOL bIsHdMode;
} ST_KDRV_XC_CFD_FIRE;

/* cfd ctrl: set dlc */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* customer dlc curve */
	MS_BOOL bUseCustomerDlcCurve;
} ST_KDRV_XC_CFD_DLC;

/* cfd ctrl: set linear rgb */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* Linear RGB enable */
	MS_BOOL bEnable;
} ST_KDRV_XC_CFD_LINEAR_RGB;

/* mm hdr memory formate for ST_KDRV_XC_CFD_STATUS */
typedef struct {
	MS_U8 u8CurrentIndex;
	MS_U8 u8InputFormat;
	MS_U8 u8InputDataFormat;
	MS_BOOL u8Video_Full_Range_Flag;

	MS_BOOL bVUIValid;
	MS_U8 u8Colour_primaries;
	MS_U8 u8Transfer_Characteristics;
	MS_U8 u8Matrix_Coeffs;

	MS_BOOL bSEIValid;
	MS_U16 u16Display_Primaries_x[3];
	MS_U16 u16Display_Primaries_y[3];
	MS_U16 u16White_point_x;
	MS_U16 u16White_point_y;
	MS_U32 u32Master_Panel_Max_Luminance;
	MS_U32 u32Master_Panel_Min_Luminance;

	MS_BOOL bContentLightLevelEnabled;
	MS_U16  u16maxContentLightLevel;
	MS_U16  u16maxPicAverageLightLevel;
} ST_KDRV_XC_HDR_CFD_MEMORY_FORMAT;

typedef enum {
    /* IP1 input, equal to XC input */
	EN_KDRV_XC_CFD_IP1_INPUT = 1,
    /* VOP output */
	EN_KDRV_XC_CFD_VOP_OUTPUT = 2,
	EN_KDRV_XC_CFD_MAX,
} EN_KDRV_XC_CFD_POINT;

/* Color info get from CFD
typedef struct __attribute__((packed)) {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U32 u32Length;

    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* format information, define in HDMI spec */
	MS_U8 u8Format;
    /* data format, such as YUV420, YUV422, YUV444, RGB */
	MS_U8 u8DataFormat;
    /* color range, true: full range, false: limit range */
	MS_U8 u8IsFullRange;
    /* useful in MM source, format can get from ColorPriamrie, TransferCharacterstic and MatrixCoefficient when transfer MM to HDMI in Box */
	MS_U8 u8ColorPriamries;
    /* useful in MM source */
	MS_U8 u8TransferCharacterstics;
    /* useful in MM source */
	MS_U8 u8MatrixCoefficients;
} ST_KDRV_XC_HDR_CFD_POINT_INFO;

/* cfd ctrl: get status */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Window (main or sub window) */
	MS_U8 u8Win;
    /* HDR type (0: SDR, 1: Dolby HDR, 2: Open HDR) */
	MS_U8 u8VideoHdrType;
    /* HDR running */
	MS_BOOL bIsHdrRunning;
    /* Is full range */
	MS_BOOL bIsFullRange;
    /* Ultra black & white active */
	MS_BOOL bUltraBlackWhiteActive;
    /* Color Type */
	MS_U8 u8ColorType;
    /* TMO Color Type */
	MS_U8 u8TMOColorType;

    /* open HDR metadata */
    union {
			ST_KDRV_XC_CFD_HDMI stHdmiInfoCFD;
			ST_KDRV_XC_HDR_CFD_MEMORY_FORMAT  stHDRMemFormatCFD;
    } HDRMetadata;
    /* CRC Status */
	MS_BOOL bCRCStatus;
    /* set_HDRDS_info average time */
	MS_U16 u16SetDSAverageTime;
    /* set_HDRDS_info peak time */
	MS_U16 u16SetDSPeakTime;
    /* MM HDR vdec version number */
	MS_U8 u8VdecVersion;
    /* set_HDRDS_info peak frame */
	MS_U64 u64SetDSPeakFrame;
    /* HDMI packet change || MM Version change */
	MS_BOOL bHDRDataChanged;
    /* Seamless CFD Done */
	MS_BOOL bSeamlessCFDDone;
    /*  GOP after done */
	MS_BOOL bGOPDone;
    /*  Reset Freeze */
	MS_BOOL bResetSeamlessFlag;
    /*  Box HDR Mode status */
	EN_KDRV_XC_HDR_OUTPUT_MODE eHDROutputMode;
    /* point infor from CFD */
	ST_KDRV_XC_HDR_CFD_POINT_INFO *pstPointInfo;
#if !defined (__aarch64__)
    /* Dummy parameter */
    void *pDummy;
#endif
    /* date size, equal to sizeof(ST_KDRV_XC_HDR_CFD_POINT_INFO) * point number */
	MS_U16 u16DataSize;
} ST_KDRV_XC_CFD_STATUS;

/* tmo levle for ST_KDRV_XC_CFD_TMO */
typedef struct {
	MS_U32 u16HdrType;
	MS_U16 u16Level;

	MS_U16 u16ControlSize;
	MS_U8 *pu8data;
#if !defined (__aarch64__)
    /* Dummy parameter */
    void *pDummy;
#endif
} ST_KDRV_XC_CFD_TMO_LEVEL;

/* cfd status for ST_KDRV_XC_CFD_UPDATE_STATUS */
typedef enum {
	E_KDRV_XC_CFD_STATUS_SYNC_GEN_BLACK_SCREEN_DISABLE,
	E_KDRV_XC_CFD_STATUS_SYNC_GEN_BLACK_SCREEN_ENABLE,
	E_KDRV_XC_CFD_STATUS_SYNC_IP2VE_SETTING_FINISHED,
	E_KDRV_XC_CFD_STATUS_SYNC_PRE_GEN_BLACK_SCREEN_DISABLE,
	E_KDRV_XC_CFD_STATUS_SYNC_PRE_GEN_BLACK_SCREEN_ENABLE,
    /*MAX */
	E_KDRV_XC_CFD_STATUS_MAX
} EN_KDRV_XC_CFD_UPDATE_STATUS;

/* cfd ctrl: update status */
typedef struct {
	EN_KDRV_XC_CFD_UPDATE_STATUS enCFD_status;
} ST_KDRV_XC_CFD_UPDATE_STATUS;

/* cfd ctrl: picture attr */
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;

    /* Picture attribute control type, reference EN_XC_CFD_PICTURE_ATTR_CTRL_TYPE */
	MS_U8 u8ControlType;
    /* Picture attribute value */
	MS_U16 u16AttrValue;
} ST_KDRV_XC_CFD_PICTURE_ATTRIBUTE;

/* cfd ctrl: hdmi par
typedef struct {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;
    /* enable to parse HDMI packet */
	MS_BOOL bHDMIPacketParsing;
} ST_KDRV_XC_CFD_HDMI_PARSE;

/* Connect tool fucntion type */
typedef enum {
	E_KDRV_XC_TOOL_FILE_TO_SYSTEM,
	E_KDRV_XC_TOOL_WRITE_REGISTER,
	E_KDRV_XC_TOOL_READ_REGISTER,
	E_KDRV_XC_TOOL_WRITE_VR,
	E_KDRV_XC_TOOL_READ_VR,
	E_KDRV_XC_TOOL_WRITE_ADL,
	E_KDRV_XC_TOOL_READ_ADL,
	E_KDRV_XC_TOOL_MAX
} EN_KDRV_XC_TOOL_FUNCTION_TYPE;

typedef enum {
	E_KDRV_XC_SAVE_MAIN_BIN,
	E_KDRV_XC_SAVE_HSY_BIN,
	E_KDRV_XC_SAVE_MAX
} EN_KDRV_XC_SAVE_BIN_TYPE;

/* Qmap type for ST_KDRV_XC_PQ_QMAPDATA */
/* E_KDRV_XC_CFD_TMO_XXX: tmo qmap */
/* E_KDRV_XC_CFD_COLOR_XXX: small qmap */
/* E_KDRV_XC_CFD_QMAP_XXX: normal qmap */
typedef enum {
    /* TMO Qmap */
	E_KDRV_XC_CFD_TMO_QMAP = 0,
    /* HDR Grule */
	E_KDRV_XC_CFD_HDR_GRULE = 1,
    /* small PQ HDR Grule */
	E_KDRV_XC_CFD_COLOR_HDR_GRULE = 2,
    /* TMO DLC Grule */
	E_KDRV_XC_CFD_TMO_DLC_GRULE = 3,
    /* Max */
	E_KDRV_XC_CFD_QMAP_MAX
} EN_KDRV_XC_CFD_QMAP_TYPE;

/* cfd ctrl: set qmap data */
typedef struct __attribute__((packed)) {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U16 u16Length;
	MS_U32 u32Qmapid;
	MS_U32 u32Quality_InputType_Num;
	MS_U32 u32Quality_IP_Num;
	MS_U32 u32Table_Data_Size;
	MS_U32 u32Table_IP_Num;
	MS_U16 *pu16Quality_Map_Array;
#if !defined (__aarch64__)
    /* Dummy parameter */
	MS_U32 *pu32Alignment;
#endif
	MS_U8 *pu8Table_Data_Array;
#if !defined (__aarch64__)
    /* Dummy parameter */
	MS_U32 *pu32Alignment1;
#endif
	MS_U16 *pu16Table_Map_Array;
#if !defined (__aarch64__)
    /* Dummy parameter */
	MS_U32 *pu32Alignment2;
#endif
    /* Qmap type */
	EN_KDRV_XC_CFD_QMAP_TYPE enQmapType;
} ST_KDRV_XC_PQ_QMAPDATA;

/* Histogram type for ST_KDRV_XC_CFD_IP_HISTOGRAM */
typedef enum {
	E_KDRV_XC_CFD_HISTOGRAM_HUE_IP,
	E_KDRV_XC_CFD_HISTOGRAM_SATURATION_IP,
	E_KDRV_XC_CFD_HISTOGRAM_LUMA_IP,
	E_KDRV_XC_CFD_HISTOGRAM_MAX
} EN_KDRV_XC_CFD_HISTOGRAM_TYPE;

/* cfd ctrl: get ip histogram */
typedef struct  __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U16 u16Length;
	MS_U8  u8Win;
	EN_KDRV_XC_CFD_HISTOGRAM_TYPE enHistogramType;
	MS_U32 u32HistogramBin;                       /*  histogram bin of hw provide */
	MS_U32 *pu32HistogramValue;                   /*  histogram value, array size = u32HistogramBin */
#if !defined (__aarch64__)
    void    *pDummy0;
#endif
} ST_KDRV_XC_CFD_IP_HISTOGRAM;

/* cus ip for ST_KDRV_CUSTOMER_IP */
typedef enum {
	E_KDRV_XC_IP_RANGE_COVERT,             /* Pre SDR IP */
	E_KDRV_XC_IP_CSC,
	E_KDRV_XC_IP_HISTOGRAM_LOCATION,       /* HDR IP */
	E_KDRV_XC_IP_PRE_RANGE_COVERT,
	E_KDRV_XC_IP_PRE_CSC,
	E_KDRV_XC_IP_EOTF,
	E_KDRV_XC_IP_PRE_GAMUT,
	E_KDRV_XC_IP_OOTF,
	E_KDRV_XC_IP_RGB3DLUT,
	E_KDRV_XC_IP_OETF,
	E_KDRV_XC_IP_POST_CSC,
	E_KDRV_XC_IP_TMO,
	E_KDRV_XC_IP_POST_RANGE_COVERT,
	E_KDRV_XC_IP_POST_CSC_SDR,                 /* post HDR IP */
	E_KDRV_XC_IP_POST_GAMUT,
	E_KDRV_XC_IP_POST_DEGAMMA,
	E_KDRV_XC_IP_POST_RGB3D,
	E_KDRV_XC_IP_POST_GAMMA,
	E_KDRV_XC_IP_PRE_RANGE_COVERT_SDR,
	E_KDRV_XC_IP_POST_RANGE_COVERT_SDR,
	E_KDRV_XC_IP_DLC_RANGE,
	E_KDRV_XC_IP_VIP_OUT_RANGE,
	E_KDRV_XC_IP_MAX,
} EN_KDRV_XC_CFD_CUSTOMER_IP;

/* cfd ctrl: update cus setting */
typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
	MS_U8  u8Win;
	EN_KDRV_XC_CFD_CUSTOMER_IP enCustomerIp;
} ST_KDRV_CUSTOMER_IP;

/* cfd ctrl: update xc status */
typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;
	MS_U8 u8Win;
	MS_U8 u8InputSource;  /* input source */

	ST_KDRV_XC_WIN_INFO stCapWin;        /* cap window */
	ST_KDRV_XC_WIN_INFO stCropWin;       /* crop window */
	ST_KDRV_XC_WIN_INFO stDispWin;       /* display window */
	ST_KDRV_XC_WIN_INFO stScaledCropWin; /* Scaled crop window */

	ST_KDRV_XC_WIN_INFO stFSCCapWin;     /* cap window of FSC */
	ST_KDRV_XC_WIN_INFO stFSCCropWin;    /* crop window of FSC */
	ST_KDRV_XC_WIN_INFO stFSCScaledCropWin;  /* scaled crop window of FSC */

	MS_BOOL bInterlace;    /* Interlace or Progressive */
	MS_U16 u16InputVFreq;  /* IP Vsync, x10 */
	MS_U16 u16OP1VFreq;    /* OP1 Vsync, x10 */
	MS_U16 u16OutputVFreq; /* output Vsync, x10 */

	MS_U16 u16SizeAfterPreScalingH;    /* width of after prescaling */
	MS_U16 u16SizeAfterPreScalingV;    /* height of after prescaling */
	MS_U16 u16FSCSizeAfterPreScalingH; /* width of after FSC prescaling */
	MS_U16 u16FSCSizeAfterPreScalingV; /* height of after FSC prescaling */
} ST_KDRV_XC_STATUS_INFO;

typedef struct __attribute__((packed)) {
	MS_U32  u32Reg;
	MS_U8   u8Val;
} ST_KDRV_XC_VR_READBYTE;

typedef struct __attribute__((packed)) {
	MS_U32  u32Reg;
	MS_U16  u16Val;
} ST_KDRV_XC_VR_R2BYTE;

typedef struct __attribute__((packed)) {
	MS_U32  u32Reg;
	MS_U16  u16Mask;
	MS_U16  u16Val;
} ST_KDRV_XC_VR_R2BYTEMASK;

typedef struct __attribute__((packed)) {
	MS_U32  u32Reg;
	MS_U8   u8Val;
} ST_KDRV_XC_VR_WRITEBYTE;

typedef struct __attribute__((packed)) {
	MS_U32  u32Reg;
	MS_U16  u16Val;
} ST_KDRV_XC_VR_W2BYTE;

typedef struct __attribute__((packed)) {
	MS_U32  u32Reg;
	MS_U16  u16Mask;
	MS_U16  u16Val;
} ST_KDRV_XC_VR_W2BYTEMASK;

typedef enum {
	E_KDRV_XC_PQ_VIDEO_CTRL_MODE_MAIN,
	E_KDRV_XC_PQ_VIDEO_CTRL_MODE_SUB,
	E_KDRV_XC_PQ_VIDEO_CTRL_MODE_MAX,
} EN_KDRV_XC_PQ_VIDEO_HW_CTRL_SEL;

typedef struct __attribute__((packed)) {
	MS_U16 u16Gain;
	MS_U16 u16Offset;
} ST_KDRV_XC_PQ_GAIN_OFFSET;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;

	MS_BOOL bInit;

	EN_KDRV_XC_PQ_VIDEO_HW_CTRL_SEL enMainSubSel;

	ST_KDRV_XC_PQ_GAIN_OFFSET astGainOff1[PQ_GAMMA_CHANNEL];
	ST_KDRV_XC_PQ_GAIN_OFFSET astGainOff2[PQ_GAMMA_CHANNEL];

	MS_BOOL bOne_Drv3; /* One drives 3 channels, R=G, B=G  */
	MS_BOOL bEnable;
	MS_U32 au32_u20p12MaxBase[PQ_GAMMA_CHANNEL];
	MS_U32 au32_u20p12Lut[PQ_GAMMA_LUT_ENTRY * PQ_GAMMA_CHANNEL];
} ST_KDRV_XC_PQ_GAMMA_IN;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;

	MS_BOOL bEnable;
	MS_U16 u16_u0p16R;
	MS_U16 u16_u0p16G;
	MS_U16 u16_u0p16B;
} ST_KDRV_XC_PQ_RGB_PATTERN_IN;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;
	MS_U32 u32Length;

	MS_U8 u8Strength;
	MS_U8 u8SatLevel;
} ST_KDRV_XC_PQ_BLUE_STRETCH_IN;

typedef struct __attribute__((packed)) {
    /*  Structure version */
	MS_U32 u32Version;
    /*  Structure length */
	MS_U32 u32Length;

	MS_U8 u8Ctrl;
	MS_U8 u8Ctrl2;
	MS_BOOL bControlFlag;
	MS_U8 u8NRStrength;
	MS_U8 au8DNRLut[8];
} ST_KDRV_XC_CFD_NR;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;          /* <Version of current structure. */
	MS_U32 u32Length;           /* <Length of this structure, u16Length=sizeof(ST_KDRV_XC_GCE_IN) */

	MS_BOOL bEnable;
	MS_U16 u16PairNum;          /* For Manual Mode: 18to256 curve */
	MS_U32 *pu32IndexLut;       /* For Manual Mode: 18to256 curve */
#if !defined (__aarch64__)
    void    *pDummy0;
#endif
	MS_U32 *pu32OutputLut;      /* For Manual Mode: 18to256 curve */
#if !defined (__aarch64__)
    void    *pDummy1;
#endif
} ST_KDRV_XC_PQ_MANUAL_LUMA_CURVE;

typedef struct __attribute__((packed)) {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U32 u32Length;
    /* Content Light level valid info */
	MS_BOOL bContentLightLevelValid;
    /* Max Content Light Level in source, enhet is nit */
	MS_U16  u16MaxContentLightLevel;
    /* Max average Picture Content Light Level in source, enhet is nit */
	MS_U16  u16MaxPicAverageLightLevel;
 } ST_KDRV_XC_HDR_CLL_INFO;

typedef struct __attribute__((packed)) {
    /* Structure version */
	MS_U32 u32Version;
    /* Structure length */
	MS_U32 u32Length;
    /* SEI info valid info */
	MS_BOOL bSEIValid;
    /* SEI Info, DisplayPrimariesX in R/G/B, real range is 0-1, multiply 50000 */
	MS_U16 u16DisplayPrimariesX[3];
    /* SEI Info, DisplayPrimariesY in R/G/B, real range is 0-1, multiply 50000 */
	MS_U16 u16DisplayPrimariesY[3];
    /* SEI Info, White point X, real range is 0-1, multiply 50000 */
	MS_U16 u16WhitePointX;
    /* SEI Info, White point Y, real range is 0-1, multiply 50000 */
	MS_U16 u16WhitePointY;
    /* SEI Info, MaxPanelLuminance, real range is 0-1, multiply 1000 */
	MS_U32 u32MaxPanelLuminance;
    /* SEI Info, MinPanelLuminance, real range is 0-1, multiply 1000 */
	MS_U32 u32MinPanelLuminance;
} ST_KDRV_XC_HDR_SEI_INFO;


typedef struct __attribute__((packed)) {
    /*  Structure version */
	MS_U32 u32Version;
    /*  Structure length */
	MS_U32 u32Length;
    /*  Window (main or sub window) */
	MS_U8 u8Win;
    /* HDR SEI info */
	ST_KDRV_XC_HDR_SEI_INFO stHdrSEIInfo;
    /* HDR CLL Info */
	ST_KDRV_XC_HDR_CLL_INFO stHdrCLLInfo;
 } ST_KDRV_XC_HDR_METADATA;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;          /* <Version of current structure. */
	MS_U32 u32Length;           /* <Length of this structure, u16Length=sizeof(ST_KDRV_XC_PQ_LUMA_INFO) */

	MS_U8 u8Win;
	MS_U8 u8IpMaximumPixel;
	MS_U8 u8IpMinimumPixel;
	MS_S32 s32Yavg;
} ST_KDRV_XC_PQ_LUMA_INFO;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;          /* <Version of current structure.
	MS_U32 u32Length;           /* <Length of this structure, u16Length=sizeof(ST_KDRV_XC_PQ_STRETCH_SETTINGS) */

	MS_U8 u8Win;
	MS_U8 u8_u0p8BlackStretchRatio;
	MS_U8 u8_u0p8WhiteStretchRatio;
	MS_U16 u16_u4p12BlackStretchGain;
	MS_U16 u16_u4p12WhiteStretchGain;
} ST_KDRV_XC_PQ_STRETCH_SETTINGS;

typedef enum {
	E_KDRV_XC_CHROMA_INFO_CONTROL_SET_WINDOW_INFO = 0,
	E_KDRV_XC_CHROMA_INFO_CONTROL_GET_HIST,
	E_KDRV_XC_CHROMA_INFO_CONTROL_NUM,
} EN_KDRV_XC_CHROMA_INFO_CONTROL;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;          /*<Version of current structure. */
	MS_U32 u32Length;           /*<Length of this structure, u16Length=sizeof(ST_KDRV_XC_PQ_CHROMA_INFO) */

	MS_U8 u8Win;
	EN_KDRV_XC_CHROMA_INFO_CONTROL enChormaInfoCtl;
	MS_U8 u8SatbyHue_HueStart_Window0;
	MS_U8 u8SatbyHue_HueRange_Window0;
	MS_U8 u8SatbyHue_HueStart_Window1;
	MS_U8 u8SatbyHue_HueRange_Window1;
	MS_U8 u8SatbyHue_HueStart_Window2;
	MS_U8 u8SatbyHue_HueRange_Window2;
	MS_U8 u8SatbyHue_HueStart_Window3;
	MS_U8 u8SatbyHue_HueRange_Window3;
	MS_U8 u8SatbyHue_HueStart_Window4;
	MS_U8 u8SatbyHue_HueRange_Window4;
	MS_U8 u8SatbyHue_HueStart_Window5;
	MS_U8 u8SatbyHue_HueRange_Window5;
	MS_BOOL bHistValid;
	MS_U32 *apu32SatOfHueHist[HISTOGRAM_SATOFHUE_WIN_NUM];
	MS_U8  au8SatOfHueHistSize[HISTOGRAM_SATOFHUE_WIN_NUM];
} ST_KDRV_XC_PQ_CHROMA_INFO;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;          /* <Version of current structure. */
	MS_U32 u32Length;           /* <Length of this structure, u32Length=sizeof(ST_KDRV_XC_PQ_NONLINEARSCALING_API_PARAM_IN) */

	MS_U8  u8Win;   /*o:main 1:sub */
	MS_U8  u8HNLEn; /* by user */
	MS_U16 u16SourceWidth;
	MS_U16 u16TargetWidth;     /* target width of image for scaler , by user */
	MS_U32 u32IniFactor;
	MS_U8  u8HShiftMode;
	MS_U32 u32CurrentRatioH;
	MS_U32 u32TargetRatioH;       /* by user */
	MS_U32 u32TargetRatioHStart;  /* by user */
	MS_U16 u16Width0;             /* by user */
	MS_U16 u16Width1;             /* by user */
	MS_U8 u8ParamFromVREn;  /* 1: from VR  0: from settings */
} ST_KDRV_XC_PQ_NONLINEARSCALING_API_PARAM_IN;

/* Define Poll Evnet MISC */
/* please enum use BIT0 = 1, BIT1 = 2, BIT2 = 4 */
typedef enum {
	E_KDRV_XC_EVENT_MISC_A_NULL = 0,
	E_KDRV_XC_EVENT_MISC_A_DOLBY_LOGO = 0x1,
	E_KDRV_XC_EVENT_MISC_A_HDR_SEAMLESS_FREEZE = 0x2,
	E_KDRV_XC_EVENT_MISC_A_HDR_SEAMLESS_UNFREEZE = 0x4,
	E_KDRV_XC_EVENT_MISC_A_RELOAD_SCALER_SETTING = 0x8,
	E_KDRV_XC_EVENT_MISC_A_SOURCE_CHANGE = 0x10,
} EN_KDRV_XC_EVENT_MISC_A;

typedef enum {
	E_KDRV_XC_EVENT_MISC_B_NULL = 0,
} EN_KDRV_XC_EVENT_MISC_B;

typedef enum {
	E_KDRV_XC_EVENT_MISC_C_NULL = 0,
} EN_KDRV_XC_EVENT_MISC_C;

typedef enum {
	E_KDRV_XC_EVENT_MISC_D_NULL = 0,
} EN_KDRV_XC_EVENT_MISC_D;

typedef struct __attribute__((packed)) {
	MS_U32 u32Version;                   /*<Version of current structure.*/
	MS_U32 u32Length;                    /*<Length of current structure.*/
	EN_KDRV_XC_EVENT_MISC_A enEventMISC_A;
	EN_KDRV_XC_EVENT_MISC_B enEventMISC_B;
	EN_KDRV_XC_EVENT_MISC_C enEventMISC_C;
	EN_KDRV_XC_EVENT_MISC_D enEventMISC_D;
} ST_KDRV_XC_EVENT_MISC;

#endif /*_MDRV_XC_ST_H*/
