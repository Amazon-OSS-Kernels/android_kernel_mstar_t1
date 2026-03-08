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
/// file    Mdrv_sc_3d.h
/// @brief  Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_SC_3D_H_
#define _MDRV_SC_3D_H_
#undef INTERFACE
#ifdef DRV_SC_3D_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifndef DISABLE_3D_FUNCTION

#if HW_2DTO3D_SUPPORT
    #define NBLK_H_MAX 120
    #define NBLK_V_MAX 68
    #define YMAX            0
    #define YMIN            1
    #define YCTRS           2
    #define BLUE            3
    #define YAVG            4
    #define HEDGE           5
    #define VEDGE           6
    #define GREEN           7
    #define BUILDING        8
    #define YAVGEDGE        9
    #define CBAVG           10
    #define CRVG            11
    #define OBJIBNDRY       12
    #define OBJIDENT        13
    #define YMAXBUFINDEX    9
    #define YMINBUFINDEX    12
    #define YAVGBUFINDEX    15
    #define YHEDGEBUFINDEX  28
    #define YVEDGEBUFINDEX  29
    #define HBLOCKNUM       60
    #define VBLOCKNUM       34
    #define MAXINDEX        256
    #define MAXARTGAIN      15
    #define ELEMENTNUM      14
    #define DDBANKOFFSET    0x00010000; //64K
#endif


typedef struct _PIXEL_DATA
{
    MS_U16 R;
    MS_U16 G;
    MS_U16 B;
}PIXEL;

#define IS_INPUT_FRAME_PACKING(eWin)                ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_FRAME_PACKING))
#define IS_INPUT_FIELD_ALTERNATIVE(eWin)            ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_FIELD_ALTERNATIVE))
#define IS_INPUT_LINE_ALTERNATIVE(eWin)             ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_LINE_ALTERNATIVE))
#define IS_INPUT_SIDE_BY_SIDE_FULL(eWin)            ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_SIDE_BY_SIDE_FULL))
#define IS_INPUT_L_DEPTH(eWin)                      ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_L_DEPTH))
#define IS_INPUT_L_DEPTH_GFX_GFX_DEPTH(eWin)        ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH))
#define IS_INPUT_TOP_BOTTOM(eWin)                   ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_TOP_BOTTOM))
#define IS_INPUT_SIDE_BY_SIDE_HALF(eWin)            ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF))
#define IS_INPUT_MODE_NONE(eWin)                    ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_MODE_NONE))
#define IS_INPUT_FRAME_ALTERNATIVE(eWin)            ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_FRAME_ALTERNATIVE))
#define IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWin)  ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE))
#define IS_INPUT_FRAME_PACKING_OPT(eWin)            ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_FRAME_PACKING_OPT ))
#define IS_INPUT_TOP_BOTTOM_OPT(eWin)               ((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_TOP_BOTTOM_OPT ))
#define IS_INPUT_NORMAL_2D(eWin)                    (pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_NORMAL_2D )
#define IS_INPUT_NORMAL_2D_INTERLACE(eWin)          (pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_NORMAL_2D_INTERLACE )
#define IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWin)      (pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_NORMAL_2D_INTERLACE_PTP )
#define IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWin) (pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT )
#define IS_INPUT_NORMAL_2D_HW(eWin)                 (pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_NORMAL_2D_HW )
#define IS_INPUT_CHECK_BOARD(eWin)                  (pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_CHECK_BORAD)
#define IS_INPUT_PIXEL_ALTERNATIVE(eWin)            (pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWin] == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)

#define IS_OUTPUT_MODE_NONE()                       ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_MODE_NONE))
#define IS_OUTPUT_LINE_ALTERNATIVE()                ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))
#define IS_OUTPUT_TOP_BOTTOM()                      ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
#define IS_OUTPUT_SIDE_BY_SIDE_HALF()               ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF))
#define IS_OUTPUT_FRAME_ALTERNATIVE()               ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
#define IS_OUTPUT_FRAME_L()                         ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_L))
#define IS_OUTPUT_FRAME_R()                         ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_R))
#define IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()         ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC ))
#define IS_OUTPUT_LINE_ALTERNATIVE_HW()             ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW))
#define IS_OUTPUT_PIXEL_ALTERNATIVE_HW()            ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_PIXEL_ALTERNATIVE_HW))
#define IS_OUTPUT_CHECKBOARD_HW()                   ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_CHECKBOARD_HW))
#define IS_OUTPUT_FRAME_L_HW()                      ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_L_HW))
#define IS_OUTPUT_FRAME_R_HW()                      ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_R_HW))
#define IS_OUTPUT_FRAME_ALTERNATIVE_HW()            ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW))
#define IS_OUTPUT_TOP_BOTTOM_HW()                   ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM_HW))
#define IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()            ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW))
#define IS_OUTPUT_FRAME_PACKING()                   ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_PACKING))


// 3d internal internal interface
INTERFACE MS_BOOL MDrv_SC_3D_IsOutputVfreq48hz(void *pInstance, SCALER_WIN eWindow, MS_U16 u16InputVFreq);
INTERFACE MS_BOOL MDrv_SC_3D_AdjustPreVerSrcForFramepacking(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_IsIPMFetchNoLimitCase(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Variable_Init(void *pInstance);
INTERFACE void MDrv_SC_3D_Adjust_FillLBMode(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_PreVLength(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                            XC_InternalStatus *pSrcInfo,
                                            SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_PreHorDstSize(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                               XC_InternalStatus *pSrcInfo,
                                               SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_SubLineBufferOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                                     XC_InternalStatus *pSrcInfo,
                                                     SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_3D_Adjust_PreVerDstSize(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                                 XC_InternalStatus *pSrcInfo,
                                                 MS_U16 u16ScaleDst,
                                                 SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_3D_Adjust_PostHorDstSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                                  MS_U16 u16ScaleDst,
                                                  SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_3D_Adjust_PostHorSrcSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                                  MS_U16 u16ScaleSrc,
                                                  SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_3D_Adjust_PostVerSrcSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                                  MS_U16 u16ScaleSrc,
                                                  SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_PostVLength(void *pInstance, XC_InternalStatus *pSrcInfo,
                                             SCALER_WIN eWindow);
INTERFACE XC_FRAME_STORE_NUMBER MDrv_SC_3D_Adjust_FrameNum(void *pInstance, XC_FRAME_STORE_NUMBER u8FBNumType,
                                                 SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_DNRBase(void *pInstance, MS_PHY *pu32DNRBase0,
                                         MS_PHY *pu32DNRBase1,
                                         SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_OPMBase(void *pInstance, MS_PHY *pu32OPMBase0,
                                         MS_PHY *pu32OPMBase1,
                                         MS_PHY *pu32OPMBase2,
                                         MS_PHY u32Offset,
                                         SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_FetchOffset(void *pInstance, XC_InternalStatus *pSrcInfo,
                                             SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_FormatValidation(void *pInstance, XC_InternalStatus *pSrcInfo,
                                              SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_SetWriteMemory(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_MirrorDNROffset(void *pInstance, XC_InternalStatus *pSrcInfo,
                                                 MS_U16 *pu16V_SizeAfterPreScaling,
                                                 MS_U16 *pu16DNROffset,
                                                 MS_U16 *pu16Ratio,
                                                 SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_AdjustMirrorCrop(void *pInstance, XC_InternalStatus *pSrcInfo,SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_DNROffset(void *pInstance, MS_U16 *pu16DNROffsetTemp,MS_U16 *pu16DNROffset, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_CropWinWidth(void *pInstance, MS_U16 *pu16CropWinWidth,SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_DualOPMFetch(void *pInstance, MS_U16 *pu16Fetch,SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_PixelOffset(void *pInstance, XC_InternalStatus *pSrcInfo,MS_PHY *pu32FRCM_RPixelOffset,MS_U16 *pu16FRCM_ROffset,MS_U16 *pu16FRCM_ROffset3D, MS_BOOL bLeftSide, SCALER_WIN eWindow);

INTERFACE void MDrv_SC_3D_GetFP_Info(void *pInstance, XC_InternalStatus *pSrcInfo,
                                     MS_U16 *pu16VactVideo,
                                     MS_U16 *pu16VactSpace,
                                     MS_BOOL bPreVscalingEna);
INTERFACE void MDrv_SC_3D_Disable_Crop(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_3D_LoadReg(void *pInstance, E_XC_3D_INPUT_MODE eInputMode, E_XC_3D_OUTPUT_MODE eOutputMode, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_Is2Dto3DCase(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                                          E_XC_3D_OUTPUT_MODE e3dOutputMode);
INTERFACE MS_BOOL MDrv_SC_3D_IsUseSameBuffer(void *pInstance);
INTERFACE void MDrv_SC_3D_CloneMainSettings(void *pInstance, XC_InternalStatus *pSrcInfo,
                                            SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_Is_LR_Sbs2Line(void *pInstance);
INTERFACE void MDrv_SC_3D_AdjustHShift(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_BytesPer2Pixel(void *pInstance, MS_U8 *u8BytesPer2PixelTemp,SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_Is2TapModeSupportedFormat(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_Set2TapMode(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_PostPQSetting(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_IsDoubleAlign(void *pInstance);
INTERFACE MS_BOOL MDrv_SC_3D_IsSupportFBL3D(void); //NO_NEED    for get chip cap
INTERFACE MS_BOOL MDrv_XC_Enable_3D_LR_Sbs2Line(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_U16 MDrv_XC_Get_3D_HShift(void *pInstance);
INTERFACE MS_BOOL MDrv_XC_Is3DFormatSupported(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode, E_XC_3D_OUTPUT_MODE e3dOutputMode);
INTERFACE E_XC_3D_PANEL_TYPE MDrv_XC_Get_3D_Panel_Type(void *pInstance);
INTERFACE E_XC_3D_OUTPUT_MODE MDrv_XC_Get_3D_Output_Mode(void *pInstance);
INTERFACE E_XC_3D_INPUT_MODE MDrv_XC_Get_3D_Input_Mode(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_XC_Get_3D_HW_Version(void); //NO_NEED
INTERFACE MS_BOOL MDrv_XC_3D_IsHWDepthAdjSupported(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                                                   E_XC_3D_OUTPUT_MODE e3dOutputMode,
                                                   SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_Get_3D_IsSupportedHW2DTo3D(void); //NO_NEED
INTERFACE MS_BOOL MDrv_XC_EnableT3D(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_BOOL MDrv_SC_3D_IsFramePaking(void *pInstance, MS_U16 u16Width, MS_U16 u16Height, MS_BOOL bInterlace);
#undef INTERFACE
#endif
#endif

