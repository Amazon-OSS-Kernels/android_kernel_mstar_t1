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

#ifndef SC_DIP_H
#define SC_DIP_H
////===========================>>SW DB Buffer
#ifdef DRV_SC_DIPSCALING_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define ALIGN_CHECK(value,factor) ((value + factor-1) & (~(factor-1)))
#define DIP_BYTE_PER_WORD       16UL
#define DIP_HVD_TILE_BLOCK_H    16UL
#define DIP_HVD_TILE_BLOCK_V    32UL
#define DWIN_W_LIMITE_OFT       31UL
#define DWIN_BYTE_PER_WORD      32UL

typedef struct
{
    MS_U16 u16H_CapStart;
    MS_U16 u16H_CapSize;
    MS_U16 u16V_CapStart;
    MS_U16 u16V_CapSize;

    MS_U32 u32H_PreScalingRatio;
    MS_U32 u32V_PreScalingRatio;

    MS_U16 u16DNROffset;
    MS_U16 u16DNRFetch;
}SC_DIP_SWDB_INFO, *P_SC_DIP_SWDB_INFO;

/// DWIN scan mode
typedef enum
{
    GOPDWIN_SCAN_MODE_AUTO = 0,
    GOPDWIN_SCAN_MODE_PROGRESSIVE = GOPDWIN_SCAN_MODE_AUTO,  ///< Progressive scan
    GOPDWIN_SCAN_MODE_INTERLACE,  ///< Interlace scan
    GOPDWIN_SCAN_MODE_MAX
} EN_XC_DWIN_SCAN_TYPE;

/// DWIN source data format
typedef enum
{
    /// YCrYCb.
    XC_DWIN_DATA_FMT_YUV422 = 0,
    /// RGB domain
    XC_DWIN_DATA_FMT_RGB565,
    /// RGB domain
    XC_DWIN_DATA_FMT_ARGB8888,
    /// YUV420 HVD tile fmt
    XC_DWIN_DATA_FMT_YUV420,
    /// YC separate 422
    XC_DWIN_DATA_FMT_YC422,
    /// YUV420 H265 tile fmt
    XC_DWIN_DATA_FMT_YUV420_H265,
    /// YUV420 H265 10 bits tile fmt
    XC_DWIN_DATA_FMT_YUV420_H265_10BITS,
    /// YUV420 planer
    XC_DWIN_DATA_FMT_YUV420_PLANER,
    /// YUV420 semi planer
    XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER,
    XC_DWIN_DATA_FMT_MAX
} EN_DRV_XC_DWIN_DATA_FMT;

INTERFACE SC_DIP_SWDB_INFO stDIP_DBreg;         ///< SW double buffer

void MDrv_XC_DIP_SWReset(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_Init(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_CMDQ_SetAction(void* pInstance,EN_XC_DIP_CMDQ_ACTION eAction, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_init_swdb(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_sw_db(void* pInstance,P_SC_DIP_SWDB_INFO pDBreg, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_SetInputSource(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_GetInputSource(void* pInstance,INPUT_SOURCE_TYPE_t *enInputSourceType,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_DisableInputSource(void* pInstance,MS_BOOL bDisable, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_SetFrameBufferAddress(void* pInstance,MS_PHY u64FBAddress, MS_PHY u64FBSize, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_v_start(void* pInstance,MS_U16 u16Vstart , SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_h_start(void* pInstance,MS_U16 u16Vstart , SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_v_size(void* pInstance,MS_U16 u16Vsize , SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_h_size(void* pInstance,MS_U16 u16Hsize , SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_window(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_get_capture_window(void* pInstance,MS_WINDOW_TYPE* capture_win,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_get_DE_Window(void* pInstance,MS_WINDOW_TYPE* psWin,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_EnableR2YCSC(void* pInstance,MS_BOOL benable, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_delayline(void* pInstance,MS_U8 u8DelayLines, SCALER_DIP_WIN eWindow );
void MDrv_XC_DWIN_set_prescaling_ratio(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_storeFrameNUM(void* pInstance,SCALER_DIP_WIN eWindow,MS_U8 u8FrameNum);
void MDrv_XC_DWIN_set_fetch_number_limit(void* pInstance,XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_EnableIPAutoCoast(void* pInstance,MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_Scaler_Field_Detect(void* pInstance,INPUT_SOURCE_TYPE_t enInputSrcType, EN_XC_DWIN_SCAN_TYPE enScanType, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_Disable_IPM_ReadWriteRequest(void* pInstance,MS_BOOL bDisable, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_SetMode(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow);
//==============Set===================
void MDrv_XC_DIP_SetWindow(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetFRC(void* pInstance,MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetWinProperty(void* pInstance,ST_XC_DIP_WINPROPERTY *pstDIPWinProperty, SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_CaptureOneFrame(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_CaptureOneFrame2(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_EnableIntr(void* pInstance,MS_U16 u8mask, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_ClearIntr(void* pInstance,MS_U16 u16mask,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_EnableCaptureStream(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SelectSourceScanType(void* pInstance,EN_XC_DWIN_SCAN_TYPE enScan,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetInterlaceWrite(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetDataFmt(void* pInstance,EN_DRV_XC_DWIN_DATA_FMT fmt,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetAlphaValue(void* pInstance,MS_U8 u8AlphaVal,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetY2R(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetUVSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetYCSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetRGBSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetOutputCapture(void* pInstance,MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetMirror(void* pInstance,MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetDIPRProperty(void* pInstance,ST_XC_DIPR_PROPERTY *pstDIPRProperty, SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_InterruptAttach(void* pInstance,InterruptCb pIntCb,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_InterruptDetach(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetRotation(void* pInstance,MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,SCALER_DIP_WIN eTmpWindow);
void MDrv_XC_DIP_SetPinpon(void* pInstance,MS_BOOL bPinpon,MS_PHY u64PinponAddr,SCALER_DIP_WIN eWindow);
MS_BOOL MDrv_XC_DIP_SetHVSP(void* pInstance,MS_BOOL bSelect, SCALER_DIP_WIN eWindow);
MS_BOOL MDrv_XC_DIP_Set420TileBlock(void* pInstance,EN_XC_DIP_TILE_BLOCK eTileBlock,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetBdgLevel(EN_XC_DIP_DEBUG_LEVEL level);
//==============Get===================
MS_U16 MDrv_XC_DIP_GetIntrStatus(void* pInstance,SCALER_DIP_WIN eWindow);
BUFFER_INFO MDrv_XC_DIP_GetBufferInfo(void* pInstance,SCALER_DIP_WIN eWindow);
#undef INTERFACE
#endif /* DRV_SC_SCALING_H */

