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
/// @file    drvPQ.h
/// @brief  PQ interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVPQ_CUS_H_
#define _DRVPQ_CUS_H_

#ifdef _MDRV_PQ_CUS_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif
#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/**
*   PQ GRule Support List
*/
typedef enum
{
    /// PQ GRule Support Film
    E_PQ_SUPPORTED_FILM,
    /// PQ GRule Support NR
    E_PQ_SUPPORTED_NR,
    /// PQ GRule Support MPEG NR
    E_PQ_SUPPORTED_MPEG_NR,
    /// PQ GRule Support Black Level
    E_PQ_SUPPORTED_BLACK_LEVEL,
    /// PQ GRule Support Ultra Clear
    E_PQ_SUPPORTED_ULTRA_CLEAR,
    /// PQ GRule Support Max number
    E_PQ_SUPPORTED_MAX,
} PQ_SUPPORTED_TYPE;

/**
 * PQ MPEG_NR_FUNCTION_TYPE
 */
typedef enum
{
    /// off
    PQ_MPEG_NR_MIN,
    /// off
    PQ_MPEG_NR_OFF = PQ_MPEG_NR_MIN,
    /// Low
    PQ_MPEG_NR_LOW,
    /// Middle
    PQ_MPEG_NR_MID,
    /// High
    PQ_MPEG_NR_HIGH,
    /// Auto
    PQ_MPEG_NR_AUTO,
    /// Default
    PQ_MPEG_NR_DEFAULT,
    /// The max support number of PQ MPEG Noise reduction
    PQ_MPEG_NR_NUM,
}PQ_MPEG_NR_FUNCTION_TYPE;

/**
 * PQ PQ_XVYCC_TYPE
 */
typedef enum
{
    PQ_XVYCC_NORMAL,
    PQ_XVYCC_ON_XVYCC,
    PQ_XVYCC_ON_SRGB,
    PQ_XVYCC_NUM,
}PQ_XVYCC_TYPE;

/**
 * EN_PQ_CFD_CTRL_TYPE
 */
typedef enum
{
    /// Initialize
    E_PQ_CFD_CTRL_SET_INIT = 0,
    /// VGA
    E_PQ_CFD_CTRL_SET_VGA = 1,
    /// TV (ATV)
    E_PQ_CFD_CTRL_SET_TV = 2,
    /// CVBS (AV)
    E_PQ_CFD_CTRL_SET_CVBS = 3,
    /// S-Video
    E_PQ_CFD_CTRL_SET_SVIDEO = 4,
    /// YPbPr
    E_PQ_CFD_CTRL_SET_YPBPR = 5,
    /// Scart
    E_PQ_CFD_CTRL_SET_SCART = 6,
    /// HDMI
    E_PQ_CFD_CTRL_SET_HDMI = 7,
    /// DTV
    E_PQ_CFD_CTRL_SET_DTV = 8,
    /// DVI
    E_PQ_CFD_CTRL_SET_DVI = 9,
    /// MM
    E_PQ_CFD_CTRL_SET_MM = 10,
    /// Panel
    E_PQ_CFD_CTRL_SET_PANEL = 11,
    /// HDR
    E_PQ_CFD_CTRL_SET_HDR = 12,
    /// EDID
    E_PQ_CFD_CTRL_SET_EDID = 13,
    /// OSD
    E_PQ_CFD_CTRL_SET_OSD = 14,
    /// Fire
    E_PQ_CFD_CTRL_SET_FIRE = 15,
    /// DLC
    E_PQ_CFD_CTRL_SET_DLC = 16,
    /// Linear RGB
    E_PQ_CFD_CTRL_SET_LINEAR_RGB = 17,

    E_PQ_CFD_CTRL_MAX,
} EN_PQ_CFD_CTRL_TYPE;

typedef struct
{
    /// Structure version
    MS_U32 u32Version;
    /// Structure length
    MS_U16 u16Length;

    /// Window (main or sub window)
    MS_U8 u8Win;

    /// Color format, reference EN_KDRV_XC_CFD_COLOR_FORMAT.
    MS_U8 u8ColorFormat;
    /// Color data format, reference EN_KDRV_XC_CFD_COLOR_DATA_FORMAT.
    MS_U8 u8ColorDataFormat;
    /// Full range
    MS_BOOL bIsFullRange;
    /// Color primaries
    MS_U8 u8ColorPrimaries;
    /// Transfer characteristics
    MS_U8 u8TransferCharacteristics;
    /// Matrix coefficients
    MS_U8 u8MatrixCoefficients;

} ST_PQ_CFD_ANALOG;

typedef struct
{
    /// Structure version
    MS_U32 u32Version;
    /// Structure length
    MS_U16 u16Length;

    /// Window (main or sub window)
    MS_U8 u8Win;
    /// Linear RGB enable
    MS_BOOL bEnable;
} ST_PQ_CFD_LINEAR_RGB;

/// CFD control information, reference mdrv_xc_st.h in kernel.
typedef struct  __attribute__((packed))
{
    /// Control type, reference EN_MAPI_CFD_CTRL_TYPE
    EN_PQ_CFD_CTRL_TYPE enCtrlType;
    /// Control parameter
    void *pParam;
#if !defined (__aarch64__)
    /// Dummy parameter
    void *pDummy;
#endif
    /// Control parameter length
    MS_U32 u32ParamLen;
    /// Error code
    MS_U16 u16ErrCode;
} ST_PQ_CFD_CONTROL_INFO;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
/// map inputsource type into index for QMap
/// @param  eWindow                          \b IN: PQ Window Main or sub
/// @param  enInputSourceType                   \b IN: the query input source type
/// @return corresponding index
//-------------------------------------------------------------------------------------------------
//INTERFACE MS_U16 QM_InputSourceToIndex(void* pInstance, PQ_WIN eWindow, MS_U8 enInputSourceType);

INTERFACE MS_U16 QM_InputSourceToIndex_UFSC(PQ_WIN eWindow, MS_U8 enInputSourceType);
//-------------------------------------------------------------------------------------------------
/// Load FilmMode Table
/// @param  eWindow                          \b IN: TBD
/// @param  enFilmModeType               \b IN: TBD
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_LoadFilmModeTable(PQ_WIN eWindow, PQ_FILM_MODE_FUNCTION_TYPE enFilmModeType);

//-------------------------------------------------------------------------------------------------
/// Load DynamicContrast Table
/// @param  eWindow                          \b IN: TBD
/// @param  enDynamicContrastType   \b IN: TBD
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_LoadDynamicContrastTable(PQ_WIN eWindow, PQ_DYNAMIC_CONTRAST_FUNCTION_TYPE enDynamicContrastType);

//-------------------------------------------------------------------------------------------------
/// Load NR Table
/// @param  eWindow                          \b IN: TBD
/// @param  en3DNRType                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_LoadNRTable(PQ_WIN eWindow, PQ_3D_NR_FUNCTION_TYPE en3DNRType);

//-------------------------------------------------------------------------------------------------
/// Load MPEG NR Table
/// @param  eWindow                          \b IN: TBD
/// @param  enMPEGNRType                \b IN: TBD
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_LoadMPEGNRTable(PQ_WIN eWindow, PQ_MPEG_NR_FUNCTION_TYPE enMPEGNRType);

//-------------------------------------------------------------------------------------------------
/// Load UltraClear Table
/// @param  eWindow                          \b IN: TBD
/// @param  enCtrlType                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_Load_ULTRACLEAR_Table(PQ_WIN eWindow, PQ_FEATURE_SWITCH_TYPE enCtrlType);

//-------------------------------------------------------------------------------------------------
/// Load XvYCC Table
/// @param  eWindow                          \b IN: TBD
/// @param  enXvyccType                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_Load_XVYCC_Table(PQ_WIN eWindow, PQ_XVYCC_TYPE enXvyccType);

//-------------------------------------------------------------------------------------------------
/// Set Dynamic Scaling on/off
/// @param  enType                             \b IN: TBD
/// @retval Support or not
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_GRULE_Get_Support_Status(PQ_SUPPORTED_TYPE enType);

//-------------------------------------------------------------------------------------------------
/// Set Memory format
/// @param  eWindow                          \b IN: TBD
/// @param  bMemFmt422                       \b IN: TBD
/// @param  bFBL                             \b IN: TBD
/// @param  u8BitsPerPixel                   \b IN: TBD
/// @return TBD
//-------------------------------------------------------------------------------------------------
INTERFACE PQ_DEINTERLACE_MODE MDrv_PQ_SetMemFormat(PQ_WIN eWindow,
                                                MS_BOOL bMemFmt422,
                                                MS_BOOL bFBL,
                                                MS_U8 *u8BitsPerPixel);

//-------------------------------------------------------------------------------------------------
/// Reduce Bandwidth for OSD
/// @param  eWindow                          \b IN: TBD
/// @param  bOSD_On                          \b IN: TBD
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_ReduceBW_ForOSD(PQ_WIN eWindow, MS_BOOL bOSD_On);

//-------------------------------------------------------------------------------------------------
/// Set PQ array of the display type.
/// @param  u16DisplayWidth                \b IN: Panel width
/// @param  enDisplaType                   \b IN: One, PIP, POP
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_Init_DisplayType(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType);

//-------------------------------------------------------------------------------------------------
/// Set PQ array of the display type.
/// @param  u16DisplayWidth                \b IN: Panel width
/// @param  enDisplaType                   \b IN: One, PIP, POP
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_Set_DisplayType(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType);

//-------------------------------------------------------------------------------------------------
/// get OSD BW Grule MADI setting
/// @param  eWindow                        \b IN: PQ Window Main or sub
/// @param  *pu16TabIdx_MADi               \b IN: MADI table Index
/// @param   *pu16TabIdx_MADi_Motion       \b IN: MADI motion table Index
/// @return TRUE: Success, FALSE: failed
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_GetMADiFromOSDBWGrule(PQ_WIN eWindow, MS_U16 *pu16TabIdx_MADi, MS_U16 *pu16TabIdx_MADi_Motion);

//-------------------------------------------------------------------------------------------------
/// get InGeneral MADI setting
/// @param  eWindow                        \b IN: PQ Window Main or sub
/// @param  *pu16TabIdx_MADi               \b IN: MADI table Index
/// @param   *pu16TabIdx_MADi_Motion       \b IN: MADI motion table Index
/// @return TRUE: Success, FALSE: failed
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_GetMADiInGeneral(PQ_WIN eWindow,MS_U16 *pu16TabIdx_MADi,MS_U16 *pu16TabIdx_MADi_Motion);

//-------------------------------------------------------------------------------------------------
/// Load PTP Table
/// @param  eWindow                          \b IN: PQ Window Main or sub
/// @param  enPTP_Type                       \b IN: QMap PTP type
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_LoadPTPTable(PQ_WIN eWindow, PQ_PTP_FUNCTION_TYPE enPTP_Type);

//-------------------------------------------------------------------------------------------------
/// Check current input timing is suitable for PTP or not
/// @param  pstXC_SetWin_Info                \b IN: Set Win Info
/// @param  eWindow                          \b IN: PQ Window Main or sub
/// @return TRUE: suitable for PTP, FALSE: not suitable
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_Check_PointToPoint_Condition(XC_SETWIN_INFO *pstXC_SetWin_Info, PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable PostCCS
/// @param  eWindow                          \b IN: PQ Window Main or sub
/// @param  bEnable                        \b IN: Enable/Disable
/// @return TRUE: Success, FALSE: Fail
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetPostCCSOnOff(PQ_WIN eWindow, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Adaptive tuning
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_AdaptiveTuning(void);

//-------------------------------------------------------------------------------------------------
/// Film Mode Any Candence Enable
/// @param  bEnable                           \b IN: enable or disable
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_PQ_FilmMode_AnyCandence_Enable(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Load table by context
/// @param  eWindow                             \b IN: TBD
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_BW_LoadTableByContext(PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Bandwidth load initial table
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_BW_LoadInitTable(void);

//-------------------------------------------------------------------------------------------------
/// Set H264 information
/// @param  bEn                                \b IN: it's H264? TRUE or FALSE
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetH264_OnOff(MS_BOOL bEn);

//-------------------------------------------------------------------------------------------------
/// Set G3D information
/// @param  bEn                                \b IN: it's G3D? TRUE or FALSE
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetG3D_OnOff(MS_BOOL bEn);

//-------------------------------------------------------------------------------------------------
/// Set 4kx2k MVC information
/// @param  bEn                                \b IN: it's H264? TRUE or FALSE
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetMVC4kx1k_OnOff(MS_BOOL bEn);

//-------------------------------------------------------------------------------------------------
/// Set Network mm information
/// @param  bEn                                \b IN: it's network mm case? TRUE or FALSE
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetNetworkMM_OnOff(MS_BOOL bEn);


//-------------------------------------------------------------------------------------------------
/// Set MM information
/// @param  bEn                                \b IN: it's MM? TRUE or FALSE
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetMM_OnOff(MS_BOOL bEn);

//-------------------------------------------------------------------------------------------------
/// Set Rmvb information
/// @param  bEn                                \b IN: it's Rmvb? TRUE or FALSE
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetRmvb_OnOff(MS_BOOL bEn, PQ_WIN ePQWindow);

//-------------------------------------------------------------------------------------------------
/// Set VIP all function bypass
/// @param  bEnable                                \b IN: VIP all function bypass TRUE or FALSE
/// @param  eWindow                                \b IN: PQ Window
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetVIPBypass_OnOff(MS_BOOL bEnable,PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Enable or diable peaking
/// @param  bEnable                          \b IN: Enable or diable peaking
/// @param  eWindow                          \b IN: PQ Window
/// @return  MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetPeaking_OnOff(MS_BOOL bEnable, PQ_WIN eWindow);

//------------------------------------------------------------------------------
/// Film mode fake out customization setting
/// @return                             \b Out: OK or Fail
//------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_PQ_SetFakeOutEnable(MS_BOOL bEnable);
//------------------------------------------------------------------------------------------------
/// Reset BW table to deafult
/// @param NONE
/// @return NONE
//------------------------------------------------------------------------------------------------
void MDrv_BW_resetToDefault(void);

//-------------------------------------------------------------------------------------------------
/// Bandwidth load BW table depends on special customer mode
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_BW_CusMode_LoadTable(MS_U8 u8TabIdx);

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif /* _DRVPQ_CUS_H_ */
