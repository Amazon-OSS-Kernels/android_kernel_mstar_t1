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

////////////////////////////////////////////////////////////////////////////////
///
/// @file  apiXC_Auto.h
/// @brief XC api for Auto
///
////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Video Video modules

 *  \defgroup XC_FE XC_FE modules 
 *  \ingroup Video
 
    Auto is used to implement auto adjust and auto calibration.\n
    For more information,see \link Auto_Geometry Auto_Geometry interface (apiXC_Auto.h)   \endlink

 *  \defgroup Auto_Geometry Auto_Geometry interface (apiXC_Auto.h) 
 *  \ingroup XC_FE  
 
    Auto is used to implement auto adjust and auto calibration, including
    - VGA auto adjust
    - ADC Calibration

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html auto_flow.png

 *  \defgroup AUTO_INIT AUTO init control 
 *  \ingroup Auto_Geometry      

 *! \defgroup AUTO_FEATURE AUTO feature operation
 *  \ingroup Auto_Geometry     

 *! \defgroup AUTO_INFO AUTO Infomation pool
 *  \ingroup Auto_Geometry     

 *! \defgroup AUTO_ToBeModified AUTO api to be modified 
 *  \ingroup Auto_Geometry     

 *! \defgroup AUTO_ToBeRemove AUTO api to be removed 
 *  \ingroup Auto_Geometry 
 */


#ifndef _APIXC_AUTO_H_
#define _APIXC_AUTO_H_
#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define XC_AUTO_SIGNAL_INFO_VERSION 1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_XC_AUTO_TUNE_NULL = 0x00,

    // Basic auto-tune
    E_XC_AUTO_TUNE_VALID_DATA    =   0x01,
    E_XC_AUTO_TUNE_POSITION      =   0x02,
    E_XC_AUTO_TUNE_FREQ          =   0x04,
    E_XC_AUTO_TUNE_PHASE         =   0x08,
    E_XC_AUTO_TUNE_BASIC         =   E_XC_AUTO_TUNE_VALID_DATA | E_XC_AUTO_TUNE_POSITION | E_XC_AUTO_TUNE_FREQ | E_XC_AUTO_TUNE_PHASE,

    // Auto-tune RGB Color
    E_XC_AUTO_TUNE_RGB_OFFSET    =   0x10,
    E_XC_AUTO_TUNE_RGB_GAIN      =   0X20,
    E_XC_AUTO_TUNE_RGB_COLOR     =   E_XC_AUTO_TUNE_RGB_OFFSET | E_XC_AUTO_TUNE_RGB_GAIN,

    // Auto-tune YUV Color
    E_XC_AUTO_TUNE_YUV_COLOR     =   0X40,

    E_XC_AUTO_TUNE_SCART_RGB_GAIN =  0x80,

    // Advance auto-tune
    E_XC_AUTO_TUNE_ADVANCE       =   E_XC_AUTO_TUNE_BASIC | E_XC_AUTO_TUNE_RGB_COLOR,
} XC_Auto_TuneType;

typedef enum
{
    E_XC_INTERNAL_CALIBRATION,
    E_XC_EXTERNAL_CALIBRATION,
} XC_Auto_CalibrationType;

typedef enum
{
    E_XC_HW_CALIBRATION,
    E_XC_SW_CALIBRATION,
} XC_Auto_CalibrationMode;

typedef enum
{
    E_XC_AUTO_POR_HPVP,
    E_XC_AUTO_POR_HPVN,
    E_XC_AUTO_POR_HNVP,
    E_XC_AUTO_POR_HNVN,
}XC_Auto_SYNC_POR;

typedef struct
{

    MS_U16 u16HorizontalStart;      ///< horizontal start
    MS_U16 u16VerticalStart;        ///< vertical start
    MS_U16 u16HorizontalTotal;      ///< ADC horizontal total
    MS_U16 u16HResolution;          ///< H size
    MS_U8  u8Phase;                 ///< obsolete

} XC_Auto_Signal_Info;

typedef struct
{
    MS_U16 u16Version;              ///< version
    MS_U16 u16HorizontalStart;      ///< horizontal start
    MS_U16 u16VerticalStart;        ///< vertical start
    MS_U16 u16HorizontalTotal;      ///< ADC horizontal total
    MS_U16 u16HResolution;          ///< H size
    MS_U16 u16Phase;                ///< ADC phase
} XC_Auto_Signal_Info_Ex;

typedef struct
{
    INPUT_SOURCE_TYPE_t eCurrentSrc;
    SCALER_WIN eWindow;
    XC_Auto_SYNC_POR eSyncPolarity;
    MS_U32 u32HSyncStart;
    MS_U32 u32HSyncEnd;
}XC_AUTO_SYNC_INFO;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//*************************************************************************
//Function name: MApi_XC_Auto_Geometry
//Description: Auto-tune for PC mode
/// @ingroup AUTO_FEATURE
/// @param  enAutoTuneType                 \b IN: Auto function select
/// @param  ActiveInfo                          \b OUT: Current PC mode setting
/// @param  StandardInfo                      \b IN:Standard mode. (This is singal information of stardard)
/// @param  eWindow                            \b IN: Window
/// @return @ref MS_BOOL
//*************************************************************************
MS_BOOL MApi_XC_Auto_Geometry(XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info *ActiveInfo, XC_Auto_Signal_Info *StandardInfo, SCALER_WIN eWindow);

//*************************************************************************
//Function name: MApi_XC_Auto_Geometry_EX
//Description: Auto-tune for PC mode
/// @ingroup AUTO_FEATURE
/// @param  enAutoTuneType                 \b IN: Auto function select
/// @param  ActiveInfo                          \b OUT: Current PC mode EX setting
/// @param  StandardInfo                      \b IN:Standard mode. (This is singal information of stardard)
/// @param  eWindow                            \b IN: Window
/// @return @ref MS_BOOL
//*************************************************************************
MS_BOOL MApi_XC_Auto_Geometry_Ex(XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info_Ex *ActiveInfo, XC_Auto_Signal_Info_Ex *StandardInfo, SCALER_WIN eWindow);

//*************************************************************************
//Function name: MApi_XC_Auto_StopAutoGeometry
//Description: Stop Auto-tune for PC mode
/// @ingroup AUTO_FEATURE
/// @return @ref MS_BOOL
//*************************************************************************
// MS_BOOL MApi_XC_Auto_StopAutoGeometry(void); // Call MApi_XC_Auto_Geometry to restart auto geometry again.

//***************************************************************************
///-This function is to set gain and offset
/// @ingroup AUTO_INFO
/// @param  type                         \b IN:calibration type
/// @param  enAutoTuneType       \b OUT:auto tune type
/// @param  pstADCSetting          \b IN:ADC setting
/// @param  eWindow                  \b IN: Window
/// @return @ref MS_BOOL
//***************************************************************************
MS_BOOL MApi_XC_Auto_GainOffset(XC_Auto_CalibrationType type, XC_Auto_TuneType enAutoTuneType,  APIXC_AdcGainOffsetSetting *pstADCSetting, SCALER_WIN eWindow);

//***************************************************************************
///-This function is to get fixed gain for HW calibration
/// @ingroup AUTO_INFO
/// @param  enAutoTuneType      \b IN:tune type
/// @param  pstADCSetting       \b OUT:gain value
/// @return @ref MS_BOOL
//***************************************************************************
MS_BOOL MApi_XC_Auto_GetHWFixedGainOffset(XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting);

//***************************************************************************
///-This function is to set ValidDataThreshold by customer
/// @ingroup AUTO_FEATURE
/// @param  Value        \b IN:ValidDataThreshold
/// @return @ref MS_BOOL
//***************************************************************************
// MS_BOOL MApi_XC_Auto_SetValidData(MS_U8 Value);

/******************************************************************************/
/// -This function will recalibration ADC offset, must be called when mode change.
/// @ingroup AUTO_FEATURE
/// @param bEnable \b IN:
/// - 0: Disable offset recalibration
/// - 1: Enable offset recalibration
/// @param bIsYPbPrFlag \b IN: to tell if YPbPr in
/// - 0: Vga mode
/// - 1: YPbPr mode
/******************************************************************************/
void MApi_XC_Auto_AutoOffset(MS_BOOL bEnable, MS_BOOL bIsYPbPrFlag);

/******************************************************************************/
/// -This function will try to get the actual mode width for VGA
/// @ingroup AUTO_INFO
/// @param  eWindow          \b IN: Window
/// @param  pSignalInfo      \b IN: Input signal info for distinguish
/// Note: The result of "MApi_XC_Auto_DetectWidth"  will highly depends on the HTT value of pSignalInfo
/******************************************************************************/
// MS_BOOL MApi_XC_Auto_DetectWidth(XC_Auto_Signal_Info_Ex *pSignalInfo , SCALER_WIN eWindow);

//***************************************************************************
///-This function is to set HW or Sw calibration
/// @ingroup AUTO_FEATURE
/// @param  eMode        \b IN: HW or SW mode for calibration
/// @return @ref MS_BOOL
//***************************************************************************
MS_BOOL MApi_XC_Auto_SetCalibrationMode(XC_Auto_CalibrationMode eMode);

//***************************************************************************
///-This function is to get HW or Sw calibration
/// @ingroup AUTO_INFO
/// @param  eMode        \b OUT: HW or SW mode for calibration
//***************************************************************************
void MApi_XC_Auto_GetCalibrationMode(XC_Auto_CalibrationMode *eMode);

//***************************************************************************
///-This function is to get sync info
/// @ingroup AUTO_INFO
/// @param  eMode        \b OUT: HW or SW mode for calibration
//***************************************************************************
void MApi_XC_AUTO_GetSyncInfo(XC_AUTO_SYNC_INFO *pSyncInfo);


#ifdef __cplusplus
}
#endif

#endif /* _APIXC_AUTO_H_ */
