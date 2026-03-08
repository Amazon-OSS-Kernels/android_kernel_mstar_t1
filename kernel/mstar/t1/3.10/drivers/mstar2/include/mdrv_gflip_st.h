/**
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
*/
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mdrv_gflip_st.h
// @brief  GFlip KMD Driver Interface
// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MDRV_GFLIP_ST_H
#define _MDRV_GFLIP_ST_H

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Type and Structure Declaration
//=============================================================================
#define MULTIINFO_NUM 8

//IO Ctrl struct defines:
typedef enum
{
    /// Color format RGB555 and Blink.
    E_GFLIP_COLOR_RGB555_BLINK    =0,
    /// Color format RGB565.
    E_GFLIP_COLOR_RGB565          =1,
    /// Color format ARGB4444.
    E_GFLIP_COLOR_ARGB4444        =2,
    /// Color format alpha blink.
    E_GFLIP_COLOR_2266      =3,
    /// Color format I8 (256-entry palette).
    E_GFLIP_COLOR_I8              =4,
    /// Color format ARGB8888.
    E_GFLIP_COLOR_ARGB8888        =5,
    /// Color format ARGB1555.
    E_GFLIP_COLOR_ARGB1555        =6,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_ABGR8888        =7,
    /// Color format RGB555/YUV422.
    E_GFLIP_COLOR_RGB555YUV422    =8,
    /// Color format YUV422.
    E_GFLIP_COLOR_YUV422          =9,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_RGBA5551        =10,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_RGBA4444        =11,

    /// Invalid color format.
    E_GFLIP_COLOR_INVALID
} GFLIP_COLOR_TYPE;

typedef union
{
    MS_U8 u8DWinIntInfo;
    struct
    {
        MS_U8 bDWinIntWADR:1;
        MS_U8 bDWinIntPROG:1;
        MS_U8 bDWinIntTF:1;
        MS_U8 bDWinIntBF:1;
        MS_U8 bDWinIntVS:1;
        MS_U8 reserved:3;
    }sDwinIntInfo;
}GFLIP_DWININT_INFO, *PGFLIP_DWININT_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_PHY64 u32Addr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
}MS_GFLIP_INFO, *PMS_GFLIP_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_PHY64 u32Addr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_BOOL bTLBEnable;
    MS_PHY64 u32TLBAddr;
}MS_TLB_GFLIP_INFO, *PMS_TLB_GFLIP_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_PHY64 u32MainAddr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_PHY64 u32SubAddr; //in
}MS_GFLIP_3DINFO, *PMS_GFLIP_3DINFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_PHY64 u32MainAddr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_PHY64 u32SubAddr; //in
    MS_BOOL bTLBEnable;
    MS_PHY64 u32TLBAddr;
}MS_TLB_GFLIP_3DINFO, *PMS_TLB_GFLIP_3DINFO;

typedef struct
{
    MS_U8 u8GopInfoCnt;
    MS_GFLIP_3DINFO astGopInfo[MULTIINFO_NUM];
}MS_GFLIP_MULTIINFO, *PMS_GFLIP_MULTIINFO;

typedef struct
{
    MS_U8 u8GopInfoCnt;
    MS_TLB_GFLIP_3DINFO astTLBGopInfo[MULTIINFO_NUM];
}MS_TLB_GFLIP_MULTIINFO, *PMS_TLB_GFLIP_MULTIINFO;

typedef struct
{
    GFLIP_DWININT_INFO gflipDWinIntInfo;
    MS_BOOL bResetDWinIntInfo;

}MS_GFLIP_DWININT_INFO, *PMS_GFLIP_DWININT_INFO;

typedef struct
{
    GFLIP_DWININT_INFO gflipDWinIntInfo;
    MS_U32  u32Timeout;
    MS_BOOL bResetDWinIntInfo;
}MS_GFLIP_DWININT_INFO2, *PMS_GFLIP_DWININT_INFO2;

typedef struct
{
    MS_BOOL bEnable; //InOut
    MS_U8   u8FrameCount; //Out, value range: 0~3
    MS_U8   u8Result; //Out
}MS_GFLIP_VECAPTURESTATE, *PMS_GFLIP_VECAPTURESTATE;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_U32 u32Result; //Out
}MS_GFLIP_GOPGWINIDX,*PMS_GFLIP_GOPGWINIDX;

typedef struct
{
    MS_U32 u32Addr;
    MS_U16 u16X;
    MS_U16 u16Y;
    MS_U16 u16W;
    MS_U16 u16H;
    MS_U8  u8GopIdx;
    MS_U8  u8GwinIdx;
    GFLIP_COLOR_TYPE clrType;
}MS_GWIN_INFO;

typedef struct
{
    MS_BOOL bCurveChange;  // Curve Change
    MS_U8  u8LumaCurve[16];  // Default luma curve from Supernova DLC.ini
    MS_U8  u8LumaCurve2_a[16];  // Default luma curve 2a  from Supernova DLC.ini
    MS_U8  u8LumaCurve2_b[16];  // Default luma curve 2b  from Supernova DLC.ini
    MS_U8  u8Result; //Out
}MS_DLC_INFO;

typedef struct
{
    MS_BOOL bBLEPointChange;  // BLE Point Change
    MS_U16  u16BLESlopPoint[6];  // Default BLE Slop Point from Supernova DLC.ini
    MS_U8  u8Result; //Out
}MS_BLE_INFO;

typedef struct
{
    MS_BOOL bInitInfoChange;  // Init Info Change

    // Default luma curve
    MS_U8 ucLumaCurve[16];
    MS_U8 ucLumaCurve2_a[16];
    MS_U8 ucLumaCurve2_b[16];
    MS_U8 ucDlcHistogramLimitCurve[17];

    MS_U8 ucDlcPureImageMode; // Compare difference of max and min bright
    MS_U8 ucDlcLevelLimit; // n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    MS_U8 ucDlcAvgDelta; // n = 0 ~ 50, default value: 12
    MS_U8 ucDlcAvgDeltaStill; // n = 0 ~ 15 => 0: disable still curve, 1 ~ 15 => 0.1 ~ 1.5 enable still curve
    MS_U8 ucDlcFastAlphaBlending; // min 17 ~ max 32
    MS_U8 ucDlcYAvgThresholdL; // default value: 0
    MS_U8 ucDlcYAvgThresholdH; // default value: 128

    MS_U8 ucDlcBLEPoint; // n = 24 ~ 64, default value: 48
    MS_U8 ucDlcWLEPoint; // n = 24 ~ 64, default value: 48
    MS_U8 bEnableBLE : 1; // 1: enable; 0: disable
    MS_U8 bEnableWLE : 1; // 1: enable; 0: disable
    MS_U8 ucDlcYAvgThresholdM;
    MS_U8 ucDlcCurveMode;
    MS_U8 ucDlcCurveModeMixAlpha;
    MS_U8 ucDlcAlgorithmMode;
    MS_U8 ucDlcSepPointH;
    MS_U8 ucDlcSepPointL;
    MS_U16 uwDlcBleStartPointTH;
    MS_U16 uwDlcBleEndPointTH;
    MS_U8 ucDlcCurveDiff_L_TH;
    MS_U8 ucDlcCurveDiff_H_TH;
    MS_U16 uwDlcBLESlopPoint_1;
    MS_U16 uwDlcBLESlopPoint_2;
    MS_U16 uwDlcBLESlopPoint_3;
    MS_U16 uwDlcBLESlopPoint_4;
    MS_U16 uwDlcBLESlopPoint_5;
    MS_U16 uwDlcDark_BLE_Slop_Min;
    MS_U8 ucDlcCurveDiffCoringTH;
    MS_U8 ucDlcAlphaBlendingMin;
    MS_U8 ucDlcAlphaBlendingMax;
    MS_U8 ucDlcFlicker_alpha;
    MS_U8 ucDlcYAVG_L_TH;
    MS_U8 ucDlcYAVG_H_TH;
    MS_U8 ucDlcDiffBase_L;
    MS_U8 ucDlcDiffBase_M;
    MS_U8 ucDlcDiffBase_H;
    // NEW DLC
    MS_U8 ucLMaxThreshold;
    MS_U8 ucLMinThreshold;
    MS_U8 ucLMaxCorrection;
    MS_U8 ucLMinCorrection;
    MS_U8 ucRMaxThreshold;
    MS_U8 ucRMinThreshold;
    MS_U8 ucRMaxCorrection;
    MS_U8 ucRMinCorrection;
    MS_U8 ucAllowLoseContrast;
}MS_DLC_INIT_INFO;

typedef struct __attribute__((packed))
{
    MS_U8      u8ColorPrimaries;
    MS_U8      u8TransferCharacteristics;
    MS_U8      u8MatrixCoefficients;
} MS_DLC_HDR_METADATA_MPEG_VUI;

// Data format follows HDR_Metadata_MpegSEI_MasteringColorVolume
typedef struct __attribute__((packed))
{
    MS_U16     display_primaries_x[3]; // x = data*0.00002    [709] {15000, 7500, 32000}
    MS_U16     display_primaries_y[3]; // y = data*0.00002    [709] {30000, 3000, 16500}
    MS_U16     white_point_x; // x = data*0.00002    [709] 15635
    MS_U16     white_point_y; // y = data*0.00002    [709] 16450
    MS_U32     max_display_mastering_luminance; // data*0.0001 nits    [600nits] 6000000
    MS_U32     min_display_mastering_luminance; // data*0.0001 nits    [0.3nits] 3000
} MS_DLC_HDR_METADATA_MPEG_SEI_MASTERING_COLOR_VOLUME;

typedef struct __attribute__((packed))
{
    MS_U16 u16Smin; // 0.10
    MS_U16 u16Smed; // 0.10
    MS_U16 u16Smax; // 0.10
    MS_U16 u16Tmin; // 0.10
    MS_U16 u16Tmed; // 0.10
    MS_U16 u16Tmax; // 0.10
    MS_U16 u16MidSourceOffset;
    MS_U16 u16MidTargetOffset;
    MS_U16 u16MidSourceRatio;
    MS_U16 u16MidTargetRatio;
} MS_DLC_HDRToneMappingData;

typedef struct __attribute__((packed))
{
    MS_U16 u16tRx; // target Rx
    MS_U16 u16tRy; // target Ry
    MS_U16 u16tGx; // target Gx
    MS_U16 u16tGy; // target Gy
    MS_U16 u16tBx; // target Bx
    MS_U16 u16tBy; // target By
    MS_U16 u16tWx; // target Wx
    MS_U16 u16tWy; // target Wy
} MS_DLC_HDRGamutMappingData;

// Document : CEA-861.3_V16BallotDraft
typedef struct __attribute__((packed))
{
    MS_U8      u8EOTF; // 0:SDR gamma, 1:HDR gamma, 2:SMPTE ST2084, 3:Future EOTF, 4-7:Reserved
    MS_U16     u16Rx; // display primaries Rx
    MS_U16     u16Ry; // display primaries Ry
    MS_U16     u16Gx; // display primaries Gx
    MS_U16     u16Gy; // display primaries Gy
    MS_U16     u16Bx; // display primaries Bx
    MS_U16     u16By; // display primaries By
    MS_U16     u16Wx; // display primaries Wx
    MS_U16     u16Wy; // display primaries Wy
    MS_U16     u16Lmax; // max display mastering luminance
    MS_U16     u16Lmin; // min display mastering luminance
    MS_U16     u16MaxCLL; // maximum content light level
    MS_U16     u16MaxFALL; // maximum frame-average light level
} MS_DLC_HDRMetadataHdmiTxInfoFrame;

typedef struct __attribute__((packed))
{
    MS_U8 PixelFormat;              // Pixel Format
    MS_U8 Colorimetry;              // Color imetry
    MS_U8 ExtendedColorimetry;      // Extended Color imetry
    MS_U8 RgbQuantizationRange;     // Rgb Quantization Range
    MS_U8 YccQuantizationRange;     // Ycc Quantization Range
    MS_U8 StaticMetadataDescriptorID; //Static Metadata Descriptor ID
} MS_DLC_HDRHdmiTxAviInfoFrame;

typedef struct __attribute__((packed))
{
    /// HDR Version Info
    MS_U16 u16HDRVerInfo;
    /// HDR init Length
    MS_U16 u16HDRInitLength;
    /// HDR Enable
    MS_BOOL bHDREnable;
    /// HDR Function Select
    MS_U16 u16HDRFunctionSelect;
    /// HDR Metadata Mpeg VUI
    MS_DLC_HDR_METADATA_MPEG_VUI DLC_HDRMetadataMpegVUI;
    /// HDR Tone Mapping Data
    MS_DLC_HDRToneMappingData DLC_HDRToneMappingData;
    /// HDR Gamut Mapping Data
    MS_DLC_HDRGamutMappingData DLC_HDRGamutMappingData;
    //HDR Metadata Hdmi Tx Info Frame
    MS_DLC_HDRMetadataHdmiTxInfoFrame DLC_HDRMetadataHdmiTxInfoFrame;
    //HDR Hdmi Tx Avi Info Frame
    MS_DLC_HDRHdmiTxAviInfoFrame DLC_HDRHdmiTxAviInfoFrame;
} MS_HDR_INFO;

// HDR use customer DLC curve.
typedef struct __attribute__((packed))
{
    MS_BOOL bFixHdrCurve;
    MS_U16 u16DlcCurveSize;
    MS_U8 *pucDlcCurve;
#if !defined (__aarch64__)
    void    *pDummy;
#endif
} MS_DLC_HDRCustomerDlcCurve;

// HDR use customer color primaries.
typedef struct __attribute__((packed))
{
    MS_BOOL bCustomerEnable;
    MS_U16 u16sWx;
    MS_U16 u16sWy;
} MS_DLC_HDRCustomerColorPrimaries;

// HDR new tone mapping parameters.
typedef struct __attribute__((packed))
{
    // TMO
    MS_U16 u16SrcMinRatio;			//default 10
    MS_U16 u16SrcMedRatio;                   //default 512
    MS_U16 u16SrcMaxRatio;			//default 990

    MS_U8 u8TgtMinFlag;                         //default 1
    MS_U16 u16TgtMin;					//default 500
    MS_U8 u8TgtMaxFlag;                        //default 0
    MS_U16 u16TgtMax;				//default 300

    MS_U16 u16FrontSlopeMin;			//default 256
    MS_U16 u16FrontSlopeMax;			//default 512
    MS_U16 u16BackSlopeMin;			//default 128
    MS_U16 u16BackSlopeMax;			//default 256

    MS_U16 u16SceneChangeThrd;		//default 1024
    MS_U16 u16SceneChangeRatioMax;	//default 1024

    MS_U8 u8IIRRatio;					//default 31
    MS_U8 u8TMO_TargetMode;      // default 0. 0 : keeps the value in initial function  1 : from output source

} MS_DLC_HDRNewToneMapping;

typedef struct __attribute__((packed))
{
    /// HDR Version Info
    MS_U16 u16HDRVerInfo;
    /// HDR init Length
    MS_U16 u16HDRInitLength;
    /// HDR Enable
    MS_BOOL bHDREnable;
    /// HDR Function Select
    MS_U16 u16HDRFunctionSelect;
    /// HDR Metadata Mpeg VUI
    MS_DLC_HDR_METADATA_MPEG_VUI DLC_HDRMetadataMpegVUI;
    /// HDR Tone Mapping Data
    MS_DLC_HDRToneMappingData DLC_HDRToneMappingData;
    /// HDR Gamut Mapping Data
    MS_DLC_HDRGamutMappingData DLC_HDRGamutMappingData;
    //HDR Metadata Hdmi Tx Info Frame
    MS_DLC_HDRMetadataHdmiTxInfoFrame DLC_HDRMetadataHdmiTxInfoFrame;
    // Fix DLC Curve
    MS_DLC_HDRCustomerDlcCurve DLC_HDRCustomerDlcCurve;
    // Customer color primarie.
    MS_DLC_HDRCustomerColorPrimaries DLC_HDRCustomerColorPrimaries;
    //HDR Hdmi Tx Avi Info Frame
    MS_DLC_HDRHdmiTxAviInfoFrame DLC_HDRHdmiTxAviInfoFrame;
    /// HDR metadata MPEG SEI mastering color volume/
    MS_DLC_HDR_METADATA_MPEG_SEI_MASTERING_COLOR_VOLUME DLC_HDRMetadataMpegSEIMasteringColorVolume;
    // New tone mapping parameters.
    MS_DLC_HDRNewToneMapping DLC_HDRNewToneMapping;
} MS_HDR_INFO_EXTEND;


#endif //_MDRV_GFLIP_ST_H
