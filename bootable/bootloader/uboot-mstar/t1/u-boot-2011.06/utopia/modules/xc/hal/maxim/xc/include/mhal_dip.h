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
// [mhal_dip.h]
// Date: 20120208
// Descriptions: Add a new layer for HW setting
//==============================================================================
#ifndef MHAL_DIP_H
#define MHAL_DIP_H

#include "hwreg_sc.h"

//==============================================================================
// Scaling Ratio Macro

// H_PreScalingDownRatio() was refined to reduce the calculation error.
// Use round up (x+y/2)/y might reduce the scaling ratio and induce right vertical garbage line.
// So use un-conditional add by 1 (x+y)/y.
#define H_PreScalingDownRatioDIP(Input, Output)                ((MS_U32)( (((MS_U64)(Output)) * 1048576ul)/ (Input) + 1 ))
#define V_PreScalingDownRatioDIP(Input, Output)                ((MS_U32)( (((MS_U64)(Output)) * 1048576ul)/ (Input) + 1 ))  // CB mode

#define DIP_CHIP_CAP(eWindow,_ret)     do{    \
                                            switch(eWindow){                \
                                                case DIP_WINDOW:            \
                                                    _ret = DIP_CAP_EXIST|   \
                                                           DIP_CAP_420TILE| \
                                                           DIP_CAP_MIRROR|  \
                                                           DIP_CAP_DIPR|    \
                                                           DIP_CAP_R2Y|     \
                                                           DIP_CAP_OP1_CAPTURE_V2|\
                                                           DIP_CAP_SCALING_DOWN|\
                                                           DIP_CAP_OSD_BLENDING; \
                                                    break;                  \
                                                case DWIN0_WINDOW:            \
                                                    _ret = DIP_CAP_EXIST|   \
                                                           DIP_CAP_MIRROR|  \
                                                           DIP_CAP_DIPR|    \
                                                           DIP_CAP_R2Y|     \
                                                           DIP_CAP_OP1_CAPTURE_V2|\
                                                           DIP_CAP_SCALING_DOWN| \
                                                           DIP_CAP_OC_CAPTURE;\
                                                    break;                  \
                                                case DWIN1_WINDOW:            \
                                                    _ret = DIP_CAP_EXIST|   \
                                                           DIP_CAP_MIRROR|  \
                                                           DIP_CAP_DIPR|    \
                                                           DIP_CAP_OP1_CAPTURE_V2|\
                                                           DIP_CAP_R2Y|     \
                                                           DIP_CAP_SCALING_DOWN; \
                                                    break;                  \
                                                default:                    \
                                                    _ret = 0;               \
                                                    break;                  \
                                            }                               \
                                        }while(0)

typedef enum
{
	E_XC_DIP_SOURCE_TYPE_SUB2,
	E_XC_DIP_SOURCE_TYPE_SUB,
	E_XC_DIP_SOURCE_TYPE_MAIN,
	E_XC_DIP_SOURCE_TYPE_OP,
} MS_XC_DIP_SOURCE_TYPE;

#define REG_CLK_BASE                       0x100B00UL
#define REG_CLK_SC_BASE                    0x113200UL

//DIP mux
#define DIP_DIP_MUX_ADC                    (0)
#define DIP_DIP_MUX_HDMI                   (1)
#define DIP_DIP_MUX_VD                     (2)
#define DIP_DIP_MUX_MVOP                   (3)
#define DIP_DIP_MUX_IP_MAIN                (4)
#define DIP_DIP_MUX_OP                     (7)
#define DIP_DIP_MUX_IP_SUB                 (4)
#define DIP_DIP_MUX_SC2_OP                 (7)
#define DIP_DIP_MUX_OSD                    (6)

//DIP SC clock
#define REG_CKG_DIP_GATE                   (REG_CLK_SC_BASE + (0x10<<1) )
    #define CKG_DWIN0_GATE_EN              BIT(3)
    #define CKG_DWIN1_GATE_EN              BIT(4)

//DIP clock
#define REG_CKG_DIP                        (REG_CLK_BASE + (0x59<<1) )
    #define CKG_DIP_GATED                  BIT(0)
    #define CKG_DIP_INVERT                 BIT(1)
    #define CKG_DIP_CLK_MASK               BMASK(5:2)
    #define CKG_DIP_CLK_ADC                (0 << 2)
    #define CKG_DIP_CLK_HDMI               (1 << 2)
    #define CKG_DIP_CLK_VD                 (2 << 2)
    #define CKG_DIP_CLK_MVOP               (3 << 2)
    #define CKG_DIP_CLK_IP_MAIN            (4 << 2)
    #define CKG_DIP_CLK_OP                 (7 << 2)
    #define CKG_DIP_CLK_IP_SUB             (5 << 2)
    #define CKG_DIP_CLK_SC2_OP             (10 << 2)
    #define CKG_DIP_CLK_OSD                (7 << 2)

//DWIN0 mux
#define DIP_DWIN0_MUX_ADC                  (0)
#define DIP_DWIN0_MUX_HDMI                 (1)
#define DIP_DWIN0_MUX_VD                   (2)
#define DIP_DWIN0_MUX_MVOP                 (3)
#define DIP_DWIN0_MUX_IP_MAIN              (4)
#define DIP_DWIN0_MUX_OP                   (7)
#define DIP_DWIN0_MUX_IP_SUB               (4)
#define DIP_DWIN0_MUX_SC2_OP               (7)
#define DIP_DWIN0_MUX_OSD                  (6)

//DWIN0 clock
#define REG_CKG_DWIN0                      (REG_CLK_BASE + (0x5F<<1) )
    #define CKG_DWIN0_GATED                BIT(0)
    #define CKG_DWIN0_INVERT               BIT(1)
    #define CKG_DWIN0_CLK_MASK             BMASK(4:2)
    #define CKG_DWIN0_CLK_ADC              (0 << 2)
    #define CKG_DWIN0_CLK_HDMI             (1 << 2)
    #define CKG_DWIN0_CLK_VD               (2 << 2)
    #define CKG_DWIN0_CLK_MVOP             (3 << 2)
    #define CKG_DWIN0_CLK_IP_MAIN          (4 << 2)
    #define CKG_DWIN0_CLK_OP               (7 << 2)
    #define CKG_DWIN0_CLK_IP_SUB           (4 << 2)
    #define CKG_DWIN0_CLK_SC2_OP           (1 << 2)
    #define CKG_DWIN0_CLK_OSD              (7 << 2)

//DWIN1 mux
#define DIP_DWIN1_MUX_ADC                  (0)
#define DIP_DWIN1_MUX_HDMI                 (1)
#define DIP_DWIN1_MUX_VD                   (2)
#define DIP_DWIN1_MUX_MVOP                 (3)
#define DIP_DWIN1_MUX_IP_MAIN              (4)
#define DIP_DWIN1_MUX_OP                   (7)
#define DIP_DWIN1_MUX_IP_SUB               (4)
#define DIP_DWIN1_MUX_SC2_OP               (7)
#define DIP_DWIN1_MUX_OSD                  (6)

//DWIN1 clock
#define REG_CKG_DWIN1                      (REG_CLK_BASE + (0x5F<<1) + 1 )
    #define CKG_DWIN1_GATED                BIT(0)
    #define CKG_DWIN1_INVERT               BIT(1)
    #define CKG_DWIN1_CLK_MASK             BMASK(4:2)
    #define CKG_DWIN1_CLK_ADC              (0 << 2)
    #define CKG_DWIN1_CLK_HDMI             (1 << 2)
    #define CKG_DWIN1_CLK_VD               (2 << 2)
    #define CKG_DWIN1_CLK_MVOP             (3 << 2)
    #define CKG_DWIN1_CLK_IP_MAIN          (4 << 2)
    #define CKG_DWIN1_CLK_OP               (7 << 2)
    #define CKG_DWIN1_CLK_IP_SUB           (4 << 2)
    #define CKG_DWIN1_CLK_SC2_OP           (1 << 2)
    #define CKG_DWIN1_CLK_OSD              (7 << 2)

//==============================================================================

// Debug Log
#include "ULog.h"

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define DIP_H_INFO(x, args...) ULOGI("DIP HAL", x, ##args)
// Warning, illegal paramter but can be self fixed in functions
#define DIP_H_WARN(x, args...) ULOGW("DIP HAL", x, ##args)
//  Need debug, illegal paramter.
#define DIP_H_DBUG(x, args...) ULOGD("DIP HAL", x, ##args)
// Error, function will be terminated but system not crash
#define DIP_H_ERR(x, args...) ULOGE("DIP HAL", x, ##args)
// Critical, system crash. (ex. assert)
#define DIP_H_FATAL(x, args...) ULOGF("DIP HAL", x, ##args)

//==============================================================================
#ifdef MHAL_DIP_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if (defined ANDROID) && (defined TV_OS)
#define DIP_CMDQ_ENABLE 1
#else
#define DIP_CMDQ_ENABLE 0
#endif

#define Hal_SC_DWIN_set_ficlk(args...)
#define Hal_SC_DWIN_set_wr_bank_mapping(args...)
#define Hal_SC_DWIN_set_csc(args...)
#define Hal_SC_DWIN_set_delayline(args...)
#define Hal_SC_DWIN_sw_db_burst(args...)
#define Hal_SC_DWIN_Set_FB_Num(args...)
#define Hal_SC_DWIN_get_memory_bit_fmt(args...) 0
#define Hal_SC_DWIN_set_capture_v_start(args...)
#define Hal_SC_DWIN_set_capture_h_start(args...)
#define Hal_SC_DWIN_set_capture_v_size(args...)
#define Hal_SC_DWIN_set_capture_h_size(args...)
#define Hal_SC_DWIN_get_capture_window(args...)
#define Hal_SC_DWIN_set_miusel(args...)
#define Hal_SC_DWIN_set_memoryaddress(args...)
#define Hal_SC_DWIN_DisableInputSource(args...)
#define Hal_SC_DWIN_SetInputSource(args...)
#define Hal_SC_DWIN_software_reset(args...)
#define Hal_SC_DWIN_setfield(args...)
#define Hal_SC_DWIN_set_image_wrap(args...)
#define Hal_SC_DWIN_set_input_sync_reference_edge(args...)
#define Hal_SC_DWIN_set_input_vsync_delay(args...)
#define Hal_SC_DWIN_set_de_only_mode(args...)
#define Hal_SC_DWIN_set_input_10bit(args...)
#define Hal_SC_DWIN_set_fir_down_sample_divider(args...)
#define Hal_SC_DWIN_set_coast_input(args...)
#define Hal_SC_DWIN_set_coast_window(args...)
#define Hal_SC_DWIN_set_input_sync_sample_mode(args...)
#define Hal_SC_DWIN_set_post_glitch_removal(args...)
#define Hal_SC_DWIN_set_DE_Mode_Glitch(args...)
#define Hal_SC_DWIN_set_de_bypass_mode(args...)
#define Hal_SC_DWIN_set_ms_filter(args...)
#define Hal_SC_DWIN_Disable_IPM_ReadWriteRequest(args...)
#define Hal_SC_DWIN_get_de_window(args...)
#define Hal_SC_DWIN_EnableIPAutoCoast(args...)
INTERFACE void Hal_SC_DWIN_set_422_cbcr_swap(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_set_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_DIP_WIN eWindow);
INTERFACE XC_FRAME_STORE_NUMBER Hal_SC_DWIN_GetFrameStoreMode(void *pInstance, SCALER_DIP_WIN eWindow,MS_BOOL bInterlace);
INTERFACE void Hal_SC_DWIN_EnableR2YCSC(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_Set_vsd_output_line_count(void *pInstance, MS_BOOL bEnable,MS_U32 u32LineCount,SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_Set_vsd_input_line_count(void *pInstance, MS_BOOL bEnable,MS_BOOL bUserMode,MS_U32 u32UserLineCount,SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_sw_db(void *pInstance, P_SC_DIP_SWDB_INFO pDBreg, SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_set_input_vsync_inverse(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
//==============Set===================
INTERFACE MS_U16 HAL_XC_DIP_GetBusSize(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SWReset(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_Init(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetBase0(void *pInstance, MS_PHY u64BufStart,MS_PHY u64BufEnd,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetBase1(void *pInstance, MS_PHY u64BufStart,MS_PHY u64BufEnd,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetWinProperty(void *pInstance, MS_U8 u8BufCnt,MS_U16 u16Width,MS_U16 u16LineOft,MS_U16 u16Height,MS_PHY u64OffSet, SCALER_DIP_SOURCE_TYPE eSource,MS_BOOL bPIP, MS_BOOL b2P_Enable, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetWinProperty1(void *pInstance, MS_PHY u64OffSet,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetMiuSel(void *pInstance, MS_U8 u8MIUSel,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetDIPRMiuSel(void *pInstance, MS_U8 u8YBufMIUSel,MS_U8 u8CBufMIUSel , SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_CpatureOneFrame(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_CpatureOneFrame2(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_ClearIntr(void *pInstance, MS_U16 u16mask,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_EnableCaptureStream(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_EnableIntr(void *pInstance, MS_U16 u8mask, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetDataFmt(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fmt,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SelectSourceScanType(void *pInstance, EN_XC_DWIN_SCAN_TYPE enScan,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetAlphaValue(void *pInstance, MS_U8 u8AlphaVal,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetUVSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetInterlaceWrite(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetY2R(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetYCSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetRGBSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetOutputCapture(void *pInstance, MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_Set444to422(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fmt,MS_BOOL bSrcYUVFmt,MS_BOOL bSrcFmt422,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetFRC(void *pInstance, MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetMirror(void *pInstance, MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetDIPRProperty(void *pInstance, ST_XC_DIPR_PROPERTY *pstDIPRProperty, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetDIPRProperty_MFDEC(void *pInstance, ST_DIP_MFDEC_INFO stDIPR_MFDecInfo, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetDIPRProperty_DI(void *pInstance, ST_XC_DIPR_PROPERTY *pstDIPRProperty, ST_DIP_DIPR_3DDI_SETTING *stDIPR_DIInfo, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_InterruptAttach(void *pInstance, InterruptCb pIntCb,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_InterruptDetach(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_MuxDispatch(void *pInstance, SCALER_DIP_SOURCE_TYPE eSource,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_Rotation(void *pInstance, MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,MS_PHY u64StartAddr,SCALER_DIP_WIN eTmpWindow);
INTERFACE void HAL_XC_DIP_SetPinpon(void *pInstance, MS_BOOL bPinpon,MS_PHY u64PinponAddr,MS_PHY u64OffSet,SCALER_DIP_WIN eWindow);
INTERFACE SCALER_DIP_WIN HAL_XC_DIP_GetHVSP(void* pInstance);
INTERFACE MS_BOOL HAL_XC_DIP_SetHVSP(void* pInstance,MS_BOOL bSelect, SCALER_DIP_WIN eWindow);
INTERFACE MS_BOOL HAL_XC_DIP_Set420TileBlock(void* pInstance,EN_XC_DIP_TILE_BLOCK eTileBlock,SCALER_DIP_WIN eWindow);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
INTERFACE void HAL_XC_DIP_Check_Clock(MS_U64* u64ReqHdl,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_Check_Scale(MS_U64* u64ReqHdl,MS_U16 *u16H_Scaling_Enable, MS_U32 *u32H_Scaling_Ratio, MS_U16 *u16V_Scaling_Enable, MS_U32 *u32V_Scaling_Ratio,SCALER_DIP_WIN eWindow);
#endif
INTERFACE void HAL_XC_DIP_Enable(void *pInstance, MS_BOOL bEnable);
//==============Get===================
INTERFACE MS_U8 HAL_XC_DIP_GetBufCnt(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE MS_U16 HAL_XC_DIP_GetBPP(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fbFmt,SCALER_DIP_WIN eWindow);
INTERFACE MS_U16 HAL_XC_DIP_WidthAlignCheck(void *pInstance, MS_U16 u16Width,MS_U16 u16Bpp,SCALER_DIP_WIN eWindow);
INTERFACE MS_U16 HAL_XC_DIP_GetIntrStatus(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE MS_BOOL HAL_XC_DIP_GetInterlaceWrite(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE EN_DRV_XC_DWIN_DATA_FMT HAL_XC_DIP_GetDataFmt(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE EN_XC_DWIN_SCAN_TYPE HAL_XC_DIP_GetSourceScanType(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_2P_Width_Check(void *pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_DIP_WIN eWindow);
INTERFACE MS_BOOL HAL_XC_DIP_CMDQBegin(void *pInstance,SCALER_DIP_WIN eWindow);
INTERFACE MS_BOOL HAL_XC_DIP_CMDQEnd(void *pInstance,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_IMI_ENABLE(void* pInstance,MS_BOOL bEnalbe, SCALER_DIP_WIN eWindow);

#undef INTERFACE
#endif /* MHAL_DIP_H */
