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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiDMS.h
/// @brief  DMS Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_DMS_TYPE_H_
#define _API_DMS_TYPE_H_

#include "MsCommon.h"
#include "MsDevice.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// library information
#define MSIF_DMS_LIB_CODE               {'M','W','S','_'}
#define MSIF_DMS_LIBVER                 {'0','0'}
#define MSIF_DMS_BUILDNUM               {'0','0'}
#define MSIF_DMS_CHANGELIST             {'0','0','0','0','0','0','0','1'}

#define DMS_API_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'    */  \
    MSIF_CLASS,                         /* '00'      */  \
    MSIF_CUS,                           /* 0x0000    */  \
    MSIF_MOD,                           /* 0x0000    */  \
    MSIF_CHIP,                                           \
    MSIF_CPU,                                            \
    MSIF_DMS_LIB_CODE  ,                /* IP__      */  \
    MSIF_DMS_LIBVER    ,                /* 0.0 ~ Z.Z */  \
    MSIF_DMS_BUILDNUM  ,                /* 00 ~ 99   */  \
    MSIF_DMS_CHANGELIST,                /* CL#       */  \
    MSIF_OS

#define VERSION_ST_DMS_DISPFRAMEFORMAT      1
#define VERSION_ST_DMS_COLORDESCRIPTION     1
#define VERSION_ST_DMS_DOLBYHDRINFO         1
#define VERSION_ST_DMS_HDR_FRAME_INFO       1
#define VERSION_ST_DMS_DISP_FRM_INFO_EXT    1
#define VERSION_ST_DMS_COLORHWFORMAT        1
#define VERSION_ST_DMS_COLORSWFORMAT        1
#define VERSION_ST_DMS_FRAMEFORMAT          1

#define VERSION_ST_DMS_INITDATA             1
#define VERSION_ST_DMS_SETMEMORY_TYPE       1
#define VERSION_ST_DMS_SETWIN_INFO          1
#define VERSION_ST_DMS_WINDOW_INFO          1
#define VERSION_ST_DMS_CREATE_WIN_INFO      1
#define VERSION_ST_DMS_ZORDER_INFO          1
#define VERSION_ST_DMS_3D_INFO              1
#define VERSION_ST_DMS_HDRINFO              1
#define VERSION_ST_DMS_SETCAPTURE_INFO      1
#define VERSION_ST_DMS_CAPTURE_INFO         1
#define VERSION_ST_DMS_OUTPUT_WIN_INFO      1
#define VERSION_ST_DMS_MUTE_WIN_INFO        1
#define VERSION_ST_DMS_FREEZE_WIN_INFO      1
#define VERSION_ST_DMS_MUTE_COLOR_INFO      1

#define DMS_INVALID_CMA_HEAP_ID             0xFFFFFFFF
#define DMS_SWDRHISTOGRAM_INDEX             32  // SWDR histogram index

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// Define return value of MApi_DMS
typedef enum
{
    E_DMS_OK                = 0,
    E_DMS_FAIL              = -1,
    E_DMS_INPUT_INVALID     = -2,
    E_DMS_WINID_ILLEGAL     = -3,
    E_DMS_EXIST_3D          = -4,
    E_DMS_EXIST_HDR         = -5,
    E_DMS_EXIST_VE_OUT      = -6,
    E_DMS_MEMORY_OUT        = -7,
    E_DMS_WINSIZE_ERROR     = -8,
    E_DMS_ZORDER_DUPLICATED = -9,
} EN_DMS_RESULT;

typedef enum
{
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT,                // in MVC case it is L view 2 bit
    E_DMS_DISP_FRM_INFO_EXT_TYPE_INTERLACE = 1,        // interlace bottom 8bit will share the same enum value
    E_DMS_DISP_FRM_INFO_EXT_TYPE_DOLBY_EL = 1,         // with dolby enhance layer 8bit
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT_INTERLACE = 2,  // interlace bottom 2bit will share the same enum
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT_DOLBY_EL = 2,   // value with dolby enhance layer 2bit
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT_MVC,            // R view 2 bit
    E_DMS_DISP_FRM_INFO_EXT_TYPE_INTERLACE_MVC,
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT_INTERLACE_MVC = 5, // MVC interlace R-View 2bit will share the
    E_DMS_DISP_FRM_INFO_EXT_TYPE_DOLBY_META = 5,          // same enum with dolby meta data
    E_DMS_DISP_FRM_INFO_EXT_TYPE_MFCBITLEN,
    E_DMS_DISP_FRM_INFO_EXT_TYPE_MFCBITLEN_MVC,
    E_DMS_DISP_FRM_INFO_EXT_TYPE_MAX,
} EN_DMS_DISP_FRM_INFO_EXT_TYPE;

typedef enum
{
    E_DMS_CODEC_TYPE_NONE,
    E_DMS_CODEC_TYPE_MPEG2,
    E_DMS_CODEC_TYPE_H263,
    E_DMS_CODEC_TYPE_MPEG4,
    E_DMS_CODEC_TYPE_DIVX311,
    E_DMS_CODEC_TYPE_DIVX412,
    E_DMS_CODEC_TYPE_FLV,
    E_DMS_CODEC_TYPE_VC1_ADV,
    E_DMS_CODEC_TYPE_VC1_MAIN,
    E_DMS_CODEC_TYPE_RV8,
    E_DMS_CODEC_TYPE_RV9,
    E_DMS_CODEC_TYPE_H264,
    E_DMS_CODEC_TYPE_AVS,
    E_DMS_CODEC_TYPE_MJPEG,
    E_DMS_CODEC_TYPE_MVC,
    E_DMS_CODEC_TYPE_VP8,
    E_DMS_CODEC_TYPE_HEVC,
    E_DMS_CODEC_TYPE_VP9,
    E_DMS_CODEC_TYPE_HEVC_DV,
    E_DMS_CODEC_TYPE_MAX,
} EN_DMS_CODECTYPE;

typedef enum
{
    E_DMS_FRAME_TYPE_I,
    E_DMS_FRAME_TYPE_P,
    E_DMS_FRAME_TYPE_B,
    E_DMS_FRAME_TYPE_OTHER,
    E_DMS_FRAME_TYPE_MAX,
} EN_DMS_FRAMETYPE;

typedef enum
{
    E_DMS_FIELD_TYPE_NONE,
    E_DMS_FIELD_TYPE_TOP,
    E_DMS_FIELD_TYPE_BOTTOM,
    E_DMS_FIELD_TYPE_BOTH,
    E_DMS_FIELD_TYPE_MAX,
} EN_DMS_FIELDTYPE;

typedef enum
{
    E_DMS_VIEW_TYPE_CENTER,
    E_DMS_VIEW_TYPE_LEFT,
    E_DMS_VIEW_TYPE_RIGHT,
    E_DMS_VIEW_TYPE_TOP,
    E_DMS_VIEW_TYPE_BOTTOM,
    E_DMS_VIEW_TYPE_MAX,
} EN_DMS_VIEWTYPE;

typedef enum
{
    E_DMS_3DMODE_DEFAULT,
    E_DMS_3DMODE_SIDEBYSIDE,
} EN_DMS_3DMode;

typedef enum
{
    E_DMS_COLOR_FORMAT_HW_HVD,                 //YUV420 HVD tiled format
    E_DMS_COLOR_FORMAT_HW_MVD,                 //YUV420 MVD tiled format
    E_DMS_COLOR_FORMAT_SW_YUV420_PLANAR,       //YUV420 Planar
    E_DMS_COLOR_FORMAT_SW_RGB565,              //RGB565
    E_DMS_COLOR_FORMAT_SW_ARGB8888,            //ARGB8888
    E_DMS_COLOR_FORMAT_YUYV,                   //YUV422 YUYV
    E_DMS_COLOR_FORMAT_SW_RGB888,              //RGB888
    E_DMS_COLOR_FORMAT_10BIT_TILE,             //YUV420 tiled 10 bits mode
    E_DMS_COLOR_FORMAT_SW_YUV420_SEMIPLANAR,   //YUV420 SemiPlanar
    E_DMS_COLOR_FORMAT_YUYV_CSC_BIT601,        //YUV422 YUYV from RGB2YUV bit601 mode
    E_DMS_COLOR_FORMAT_YUYV_CSC_255,           //YUV422 YUYV from RGB2YUV 0~255 mode
    E_DMS_COLOR_FORMAT_HW_EVD,                 //YUV420 EVD tiled format
    E_DMS_COLOR_FORMAT_MAX,
} EN_DMS_COLORFORMAT;

typedef enum
{
    E_DMS_CAPTURE_COLOR_FORMAT_16X32TILE,      //YUV420 16(w)x32(h) tiled format
    E_DMS_CAPTURE_COLOR_FORMAT_32X16TILE,      //YUV420 32(w)x16(h) tiled format
    E_DMS_CAPTURE_COLOR_FORMAT_32X32TILE,      //YUV420 32(w)x32(h) tiled format
    E_DMS_CAPTURE_COLOR_FORMAT_YUYV,           //YUV422 YUYV
    E_DMS_CAPTURE_COLOR_FORMAT_YVYU,           //YUV422 YVYU
    E_DMS_CAPTURE_COLOR_FORMAT_UYVY,           //YUV422 UYVY
    E_DMS_CAPTURE_COLOR_FORMAT_VYUY,           //YUV422 VYUY
    E_DMS_CAPTURE_COLOR_FORMAT_NV12,           //YUV420 SemiPlanar
    E_DMS_CAPTURE_COLOR_FORMAT_NV21,           //YUV420 SemiPlanar
    E_DMS_CAPTURE_COLOR_FORMAT_MAX,
} EN_DMS_CAPTURE_COLORFORMAT;

typedef enum
{
    E_DMS_STATUS_INITIALIZED            = (0x1 << 0),
    E_DMS_STATUS_SUPPORT_MULTI_WINDOW = (0x1<<1),
    E_DMS_STATUS_SUPPORT_SECURE_VIDEO   = (0x1 << 2),
    E_DMS_STATUS_SUPPORT_3D             = (0x1 << 3),
    E_DMS_STATUS_SUPPORT_HDR            = (0x1 << 4),
    E_DMS_STATUS_SUPPORT_MEMC           = (0x1 << 5),
    E_DMS_STATUS_SUPPORT_VE_OUT         = (0x1 << 6),
} EN_DMS_STATUS;

typedef enum
{
    E_DMS_CAPABILITY_MULTI_WINDOW   = (0x1 << 1),
    E_DMS_CAPABILITY_VSYNC_BRIDGE   = (0x1 << 2),
    E_DMS_CAPABILITY_AVSYNC         = (0x1 << 3),
    E_DMS_CAPABILITY_SECURE_VIDEO   = (0x1 << 4),
    E_DMS_CAPABILITY_3D             = (0x1 << 5),
    E_DMS_CAPABILITY_HDR            = (0x1 << 6),
    E_DMS_CAPABILITY_MEMC           = (0x1 << 7),
    E_DMS_CAPABILITY_VE_OUT         = (0x1 << 8),
    E_DMS_CAPABILITY_CAPTURE_WITH_BUFFER_MODE      = (0x1 << 9),
    E_DMS_CAPABILITY_CAPTURE_WITH_BUFFERLESS_MODE  = (0x1 << 10),
} EN_DMS_CAPABILITY;

typedef enum
{
    E_DMS_AR_DEFAULT = 0,
    E_DMS_AR_16x9,
    E_DMS_AR_4x3,
    E_DMS_AR_AUTO,
    E_DMS_AR_Panorama,
    E_DMS_AR_JustScan,
    E_DMS_AR_Zoom1,
    E_DMS_AR_Zoom2,
    E_DMS_AR_14x9,
    E_DMS_AR_DotByDot,
    E_DMS_AR_Subtitle,
    E_DMS_AR_Movie,
    E_DMS_AR_Personal,
    E_DMS_AR_4x3_PanScan,
    E_DMS_AR_4x3_LetterBox,
    E_DMS_AR_16x9_PillarBox,
    E_DMS_AR_16x9_PanScan,
    E_DMS_AR_4x3_Combind,
    E_DMS_AR_16x9_Combind,
    E_DMS_AR_Zoom_2x,
    E_DMS_AR_Zoom_3x,
    E_DMS_AR_Zoom_4x,
    E_DMS_AR_CUS = 0x20,
    E_DMS_AR_MAX = 0x40,
} EN_DMS_ASPECT_RATIO;

//DMS Memory
typedef enum
{
    E_DMS_MEMORY_TYPE_XC,
    E_DMS_MEMORY_TYPE_GOP_DISPLAY,
    E_DMS_MEMORY_TYPE_MVOP_DISPLAY,
    E_DMS_MEMORY_TYPE_DIPDI,
    E_DMS_MEMORY_TYPE_FREEZE,
    E_DMS_MEMORY_TYPE_PQ_MISC,
    E_DMS_MEMORY_TYPE_MAX,
} EN_DMS_MEMORY_TYPE;

//DMS HDR Type
typedef enum
{
    E_DMS_HDR_NONE,
    E_DMS_HDR_SWDR,
    E_DMS_HDR_HDR10,
    E_DMS_HDR_HLG,
    E_DMS_HDR_TCH,
    E_DMS_HDR_DOLBY,
    E_DMS_HDR_MAX,
} EN_DMS_HDR_TYPE;

// DMS 3D input info
typedef enum
{
    //range [0000 ~ 1111] reserved for hdmi 3D spec
    E_DMS_3D_INPUT_FRAME_PACKING                     = 0x00, //0000
    E_DMS_3D_INPUT_FIELD_ALTERNATIVE                 = 0x01, //0001
    E_DMS_3D_INPUT_LINE_ALTERNATIVE                  = 0x02, //0010
    E_DMS_3D_INPUT_SIDE_BY_SIDE_FULL                 = 0x03, //0011
    E_DMS_3D_INPUT_L_DEPTH                           = 0x04, //0100
    E_DMS_3D_INPUT_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH   = 0x05, //0101
    E_DMS_3D_INPUT_TOP_BOTTOM                        = 0x06, //0110
    E_DMS_3D_INPUT_SIDE_BY_SIDE_HALF                 = 0x08, //1000
    E_DMS_3D_INPUT_CHECK_BORAD                       = 0x09, //1001

    //user defined
    E_DMS_3D_INPUT_MODE_USER                         = 0x10,
    E_DMS_3D_INPUT_MODE_NONE                         = E_DMS_3D_INPUT_MODE_USER,
    E_DMS_3D_INPUT_FRAME_ALTERNATIVE,
    E_DMS_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE,
    // optimize for format: in:E_DMS_3D_INPUT_FRAME_PACKING, out:E_DMS_3D_OUTPUT_TOP_BOTTOM,E_DMS_3D_OUTPUT_SIDE_BY_SIDE_HALF
    E_DMS_3D_INPUT_FRAME_PACKING_OPT,
    // optimize for format: in:E_DMS_3D_INPUT_TOP_BOTTOM, out:E_DMS_3D_OUTPUT_SIDE_BY_SIDE_HALF
    E_DMS_3D_INPUT_TOP_BOTTOM_OPT,
    E_DMS_3D_INPUT_NORMAL_2D,
    E_DMS_3D_INPUT_NORMAL_2D_INTERLACE,
    E_DMS_3D_INPUT_NORMAL_2D_INTERLACE_PTP,
    E_DMS_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT,
    E_DMS_3D_INPUT_NORMAL_2D_HW,                //for hw 2D to 3D use
    E_DMS_3D_INPUT_PIXEL_ALTERNATIVE,
} EN_DMS_3D_INPUT_MODE;

// DMS 3D output info
typedef enum
{
    E_DMS_3D_OUTPUT_MODE_NONE,
    E_DMS_3D_OUTPUT_LINE_ALTERNATIVE ,
    E_DMS_3D_OUTPUT_TOP_BOTTOM,
    E_DMS_3D_OUTPUT_SIDE_BY_SIDE_HALF,
    E_DMS_3D_OUTPUT_FRAME_ALTERNATIVE,              //25-->50,30-->60,24-->48,50-->100,60-->120----FRC 1:2
    E_DMS_3D_OUTPUT_FRAME_L,
    E_DMS_3D_OUTPUT_FRAME_R,
    E_DMS_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC,        //50->50, 60->60-------------------------------FRC 1:1
    E_DMS_3D_OUTPUT_CHECKBOARD_HW,                  //for hw 2d to 3d use
    E_DMS_3D_OUTPUT_LINE_ALTERNATIVE_HW,            //for hw 2d to 3d use
    E_DMS_3D_OUTPUT_PIXEL_ALTERNATIVE_HW,           //for hw 2d to 3d use
    E_DMS_3D_OUTPUT_FRAME_L_HW,                     //for hw 2d to 3d use
    E_DMS_3D_OUTPUT_FRAME_R_HW,                     //for hw 2d to 3d use
    E_DMS_3D_OUTPUT_FRAME_ALTERNATIVE_HW,           //for hw 2d to 3d use
    E_DMS_3D_OUTPUT_TOP_BOTTOM_HW,                  //for hw 2d to 3d use, it based on E_DMS_3D_OUTPUT_TOP_BOTTOM implement
    E_DMS_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW,           //for hw 2d to 3d use, it based on E_DMS_3D_OUTPUT_SIDE_BY_SIDE_HALF implement
    E_DMS_3D_OUTPUT_FRAME_PACKING,
    E_DMS_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR,         //for 4k0.5k@240 3D
    E_DMS_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR_HW       //for HW 2Dto3D 4k0.5k@240 3D
} EN_DMS_3D_OUTPUT_MODE;

// DMS 3D panel type
typedef enum
{
    E_DMS_3D_PANEL_NONE,
    E_DMS_3D_PANEL_SHUTTER,         //240hz panel, which can process updown, leftright,vertical or horizontal line weave
    E_DMS_3D_PANEL_PELLICLE,        //120hz panel, which can only process horizontal line weave
    E_DMS_3D_PANEL_4K1K_SHUTTER,    //120hz 4K1K panel, which can process updown, leftright,vertical or horizontal line weave
    E_DMS_3D_PANEL_MAX,
} EN_DMS_3D_PANEL_TYPE;

// DMS HDR meta type
typedef enum
{
    E_DMS_HDR_METATYPE_MPEG_VUI                           = (0x1 << 0),
    E_DMS_HDR_METADATA_MPEG_SEI_MASTERING_COLOR_VOLUME    = (0x1 << 1),
    E_DMS_HDR_METADATA_DOLBY_HDR                          = (0x1 << 2),
    E_DMS_HDR_METADATA_TCH                                = (0x1 << 3),
    E_DMS_HDR_METADATA_HDR10_PER_FRAME                    = (0x1 << 4),
    E_DMS_HDR_METATYPE_CONTENT_LIGHT                      = (0x1 << 5),
    E_DMS_HDR_METATYPE_DYNAMIC                            = (0x1 << 6),
} EN_DMS_HDRMetaType;

//DMS window define
typedef struct DLL_PACKED
{
    MS_U32 u32x;
    MS_U32 u32y;
    MS_U32 u32width;
    MS_U32 u32height;
} ST_DMS_WINDOW;

//DMS Color define
typedef struct
{
    MS_U32 u32R;
    MS_U32 u32G;
    MS_U32 u32B;
    MS_U32 u32A;
} ST_DMS_COLOR;

//DMS Init struct
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_INITDATA version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_INITDATA)
} ST_DMS_INITDATA;

//DMS set memory type struct
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_SETMEMORY_TYPE version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_SET_MEMORY_TYPE)

    EN_DMS_MEMORY_TYPE eMemoryType;
    MS_PHY phyAddr;
    MS_PHY phySize;
    MS_U32 u32CMAHeapID;
} ST_DMS_SET_MEMORY_TYPE;

//DMS Zorder info define
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    MS_U32 u32Zorder;
} ST_DMS_ZORDER_INFO;

//DMS Set Capture info define
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    MS_PHY phyAddr;
    MS_PHY phySize;

    MS_U32 u32Enable;
    MS_U32 u32Visible;
    MS_U32 u32FrameRate;       ///< If desired frameRate is 30fps, u32FrameRate value should be set 30000
    MS_U32 u32Width;
    MS_U32 u32Height;
    EN_DMS_CAPTURE_COLORFORMAT enColorFormat;
    MS_U32 u32BufferLessMode;  ///< Type 0: with ring buffer, Others: Enable IMI capture mode to save dram cost
} ST_DMS_SET_CAPTURE_INFO;

//DMS Capture info define
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    MS_PHY phyBufferYAddr;
    MS_PHY phyBufferCAddr;  ///< If capture Format is YUV420, need use this phyBufferCAddr address for C address
    MS_U64 u64Pts;
    MS_U32 u32frameCount;   ///< To distinguish whether the frame is a new frame
} ST_DMS_CAPTURE_INFO;

/// define the information for DMS to set XC window
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    //-------------
    // Input
    //-------------
    MS_U32 u32InputSourceType;      ///<Input source

    //-------------
    // Window
    //-------------
    ST_DMS_WINDOW stCapWin;        ///<Capture window
    ST_DMS_WINDOW stDispWin;       ///<Display window
    ST_DMS_WINDOW stCropWin;       ///<Crop window

    //-------------
    // Timing
    //-------------
    MS_U32  u32Interlace;             ///<Interlaced or Progressive
    MS_U32  u32HDuplicate;            ///<flag for vop horizontal duplicate, for MVD, YPbPr, indicate input double sampled or not
    MS_U32  u32InputVFreq;          ///<Input V Frequency, VFreqx10, for calculate output panel timing
    MS_U32  u32InputVTotal;         ///<Input Vertical total, for calculate output panel timing
    MS_U32  u32DefaultHtotal;       ///<Default Htotal for VGA/YPbPr input
    MS_U32  u32DefaultPhase;            ///<Default Phase for VGA/YPbPr input

    //-------------------------
    // customized post scaling
    //-------------------------
    MS_U32  u32HCusScaling;               ///<assign post H customized scaling instead of using XC scaling
    MS_U32  u32HCusScalingSrc;          ///<post H customized scaling src width
    MS_U32  u32HCusScalingDst;          ///<post H customized scaling dst width
    MS_U32  u32VCusScaling;               ///<assign post V manuel scaling instead of using XC scaling
    MS_U32  u32VCusScalingSrc;          ///<post V customized scaling src height
    MS_U32  u32VCusScalingDst;          ///<post V customized scaling dst height

    //--------------
    // 9 lattice
    //--------------
    MS_U32  u32DisplayNineLattice;        ///<used to indicate where to display in panel and where to put in frame buffer

    //-------------------------
    // customized pre scaling
    //-------------------------
    MS_U32  u32PreHCusScaling;            ///<assign pre H customized scaling instead of using XC scaling
    MS_U32  u32PreHCusScalingSrc;       ///<pre H customized scaling src width
    MS_U32  u32PreHCusScalingDst;       ///<pre H customized scaling dst width
    MS_U32  u32PreVCusScaling;            ///<assign pre V manuel scaling instead of using XC scaling
    MS_U32  u32PreVCusScalingSrc;       ///<pre V customized scaling src height
    MS_U32  u32PreVCusScalingDst;       ///<pre V customized scaling dst height
} ST_DMS_XC_SETWIN_INFO;

//DMS 3D info define
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    EN_DMS_3D_INPUT_MODE en3DInputMode;
    EN_DMS_3D_OUTPUT_MODE en3DOutputMode;
    EN_DMS_3D_PANEL_TYPE en3DPanelType;
} ST_DMS_3D_INFO;

// DMS HDR info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    EN_DMS_HDR_TYPE enHDRType;
} ST_DMS_HDR_INFO;

//DMS create window info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    MS_U32 u32InputSourceType;
} ST_DMS_CREATE_WIN_INFO;

// DMS output window info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    ST_DMS_WINDOW stCropWin;    // if no use, set to 0; if crop valid, it means output window is moved outside of the outputlayer
    ST_DMS_WINDOW stOutputWin;
} ST_DMS_OUTPUT_WIN_INFO;

//DMS set window info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;          // ST_DMS_SETWIN_INFO version
    MS_U32 u32Length;           // sizeof(ST_DMS_SETWIN_INFO)

    MS_U32 u32XCWinInfoValid;
    ST_DMS_XC_SETWIN_INFO stXCWinInfo;

    ST_DMS_OUTPUT_WIN_INFO stOutputWinInfo;

    EN_DMS_ASPECT_RATIO enARC;
    MS_U32 u32OnOutputLayer;
} ST_DMS_SETWIN_INFO;

// DMS window mute info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    MS_U32 u32Enable;
} ST_DMS_MUTE_WINDOW_INFO;


// DMS freeze window info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    MS_U32 u32Enable;
} ST_DMS_FREEZE_WINDOW_INFO;


// DMS mute color info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    ST_DMS_COLOR stMuteColor;
} ST_DMS_MUTE_COLOR_INFO;

////Frame Info set
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_COLORDESCRIPTION version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_COLORDESCRIPTION)
    //color_description: indicates the chromaticity/opto-electronic coordinates of the source primaries
    MS_U8 u8ColorPrimaries;
    MS_U8 u8TransferCharacteristics;
    // matrix coefficients in deriving YUV signal from RGB
    MS_U8 u8MatrixCoefficients;
} ST_DMS_COLORDESCRIPTION;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_MASTER_COLOR_DISPLAY version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_MASTER_COLOR_DISPLAY)
    //mastering color display: color volumne of a display
    MS_U32 u32MaxLuminance;
    MS_U32 u32MinLuminance;
    MS_U16 u16DisplayPrimaries[3][2];
    MS_U16 u16WhitePoint[2];
} ST_DMS_MASTERCOLORDISPLAY;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_DOLBYHDRINFO version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_DOLBYHDRINFO)
    // bit[0:1] 0: Disable 1:Single layer 2: Dual layer, bit[2] 0:Base Layer 1:Enhance Layer
    MS_U8  u8DVMode;
    MS_PHY phyHDRMetadataAddr;
    MS_U32 u32HDRMetadataSize;
    MS_PHY phyHDRRegAddr;
    MS_U32 u32HDRRegSize;
    MS_PHY phyHDRLutAddr;
    MS_U32 u32HDRLutSize;
    MS_U8  u8DMEnable;
    MS_U8  u8CompEnable;
    MS_U8  u8CurrentIndex;
} ST_DMS_DOLBYHDRINFO;

// DMS HDR frame info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_HDR_FRAME_INFO version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_HDR_FRAME_INFO)

    // bit[0]: MS_ColorDescription present or valid, bit[1]: MS_MasterColorDisplay present or valid
    MS_U32 u32FrmInfoExtAvail;
    // //color_description: indicates the chromaticity/opto-electronic coordinates of the source primaries
    ST_DMS_COLORDESCRIPTION   stColorDescription;
    // mastering color display: color volumne of a display
    ST_DMS_MASTERCOLORDISPLAY stMasterColorDisplay;
    //DMS_MasterColorDisplay stMasterColorDisplay;
    ST_DMS_DOLBYHDRINFO       stDolbyHDRInfo;
} ST_DMS_HDR_FRAME_INFO;

//DMS window info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;      // ST_DMS_WINDOW_INFO version
    MS_U32 u32Length;       // sizeof(ST_DMS_WINDOW_INFO)

    MS_U32 u32DeviceID;
    MS_U32 u32WinID;
    MS_U32 u32Layer; 	    // WIN_LAYER eLayer
    MS_U32 u32WinIsused;     // Is used

    ST_DMS_ZORDER_INFO stZorderInfo;
    ST_DMS_3D_INFO st3DInfo;
    ST_DMS_HDR_INFO stHDRInfo;
    ST_DMS_SET_CAPTURE_INFO stSetCapInfo;

    ST_DMS_SETWIN_INFO stSetWinInfo;
} ST_DMS_WINDOW_INFO;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_COLORHWFORMAT version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_COLORHWFORMAT)
    MS_PHY phyLumaAddr;
    MS_PHY phyChromaAddr;
    MS_PHY phyLumaAddr2Bit;
    MS_PHY phyChromaAddr2Bit;
    MS_U32 u32LumaPitch;
    MS_U32 u32ChromaPitch;
    MS_U32 u32LumaPitch2Bit;
    MS_U32 u32ChromaPitch2Bit;
    MS_U32 u32MFCodecInfo;
    MS_PHY phyMFCBITLEN;
    MS_U8  u8V7DataValid;
    MS_U16 u16Width_subsample;
    MS_U16 u16Height_subsample;
    MS_PHY phyLumaAddr_subsample;
    MS_PHY phyChromaAddr_subsample;
    MS_U16 u16Pitch_subsample;
    MS_U8  u8TileMode_subsample;
    MS_PHY u32HTLBTableAddr;
    MS_U8  u8HTLBPageSizes;
    MS_U8  u8HTLBChromaEntriesSize;
    MS_PHY u32HTLBChromaEntriesAddr;
    MS_U16 u16MaxContentLightLevel;
    MS_U16 u16MaxPicAverageLightLevel;
    MS_U64 u64NumUnitsInTick;
    MS_U64 u64TimeScale;
    MS_U16 u16Histogram[DMS_SWDRHISTOGRAM_INDEX];
} ST_DMS_COLORHWFORMAT;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_FRAMEFORMAT version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_FRAMEFORMAT)
    EN_DMS_FRAMETYPE enFrameType;
    EN_DMS_FIELDTYPE enFieldType;
    EN_DMS_VIEWTYPE enViewType;
    MS_U32 u32Width;
    MS_U32 u32Height;
    MS_U32 u32CropLeft;
    MS_U32 u32CropRight;
    MS_U32 u32CropTop;
    MS_U32 u32CropBottom;
    ST_DMS_COLORHWFORMAT stHWFormat;
    MS_U32 u32Idx;
    MS_U32 u32PriData;
    MS_U8 u8LumaBitdepth;
    MS_U8 u8ChromaBitdepth;
} ST_DMS_FRAMEFORMAT;

typedef struct DLL_PACKED
{
    MS_U32 u32LumaAddrExt[E_DMS_DISP_FRM_INFO_EXT_TYPE_MAX];
    MS_U32 u32ChromaAddrExt[E_DMS_DISP_FRM_INFO_EXT_TYPE_MAX];
    MS_U16 u16Width;      // the width of second frame
    MS_U16 u16Height;     // the height of second frame
    MS_U16 u16Pitch[2];   // the pitch of second frame
} ST_DMS_DISP_FRM_INFO_EXT;

//
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                           /// ST_DMS_DispFrameFormat version
    MS_U32 u32Length;                           /// sizeof(ST_DMS_DispFrameFormat)
    MS_U32 u32OverlayID;
    ST_DMS_FRAMEFORMAT stFrames[2];
    EN_DMS_COLORFORMAT enColorFormat;
    MS_U32 u32FrameNum;
    MS_U64 u64Pts;
    MS_U32 u32CodecType;
    MS_U32 u32FrameRate;
    MS_U32 u32AspectWidth;
    MS_U32 u32AspectHeight;
    MS_U32 u32VdecStreamVersion;
    MS_U32 u32VdecStreamId;
    MS_U32 u32UniqueId;
    MS_U8 u8AspectRate;
    MS_U8 u8Interlace;
    MS_U8 u8FrcMode;
    MS_U8 u83DMode;
    MS_U8 u8BottomFieldFirst;
    MS_U8 u8FreezeThisFrame;
    MS_U8 u8ToggleTime;
    MS_U8 u8MCUMode;
    MS_U8 u8FieldCtrl;          // control one field mode, always top or bot when FF or FR
    MS_U8 u8ApplicationType;
    MS_U8 u83DLayout;           // 3D layout from SEI, the possible value is OMX_3D_LAYOUT enum in OMX_Video.h
    MS_U8 u8ColorInXVYCC;
    MS_U8 u8LowLatencyMode;     // for CTS or other application, drop new frame when render too fast
    MS_U8 u8VdecComplexity;
    MS_U8 u8HTLBTableId;
    MS_U8 u8HTLBEntriesSize;
    MS_U8 u8AFD;               //active frame code
    ST_DMS_HDR_FRAME_INFO stHDRInfo;
    ST_DMS_DISP_FRM_INFO_EXT stDispFrmInfoExt;
    MS_U16 u16MIUBandwidth;
    MS_U16 u16Bitrate;
    MS_U32 u32TileMode;
    MS_PHY phyHTLBEntriesAddr;
    MS_PHY phyVsyncBridgeAddr;
    MS_PHY phyVsyncBridgeExtAddr;
} ST_DMS_DISPFRAMEFORMAT;

#ifdef __cplusplus
}
#endif

#endif

