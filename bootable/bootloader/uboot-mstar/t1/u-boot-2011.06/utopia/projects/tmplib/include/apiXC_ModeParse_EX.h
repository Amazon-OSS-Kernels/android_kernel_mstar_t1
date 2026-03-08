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
/// @file  apiXC_ModeParse_EX.h
/// @brief MIU header file
///
////////////////////////////////////////////////////////////////////////////////
#ifndef _API_XC_MODEPARSE_EX_H_
#define _API_XC_MODEPARSE_EX_H_

#include "MsCommon.h"

// Driver
#include "drvXC_IOPort_EX.h"

// Api
#include "apiXC_EX.h"

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

//------------------------------
// Mode Detect & Search
//------------------------------
#define XC_MODEPARSE_EX_MD_FLAG_NULL            0x00

#define XC_MODEPARSE_EX_MD_FLAG_POR_HPVP        0x01
#define XC_MODEPARSE_EX_MD_FLAG_POR_HPVN        0x02
#define XC_MODEPARSE_EX_MD_FLAG_POR_HNVP        0x04
#define XC_MODEPARSE_EX_MD_FLAG_POR_HNVN        0x08
#define XC_MODEPARSE_EX_MD_FLAG_INTERLACE       0x10

#define XC_MODEPARSE_EX_MD_FLAG_EURO_HDTV_BIT   0x20

#define XC_MODEPARSE_EX_MD_FLAG_YPBPR_BIT       0x40
#define XC_MODEPARSE_EX_MD_FLAG_HDTV_BIT        0x80


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// The type of result after mode searching.
typedef enum
{
    E_XC_MODEPARSE_EX_NOT_PARSED = 0,        ///< timing not stable or stable no sync, not parse yet
    E_XC_MODEPARSE_EX_OUT_OF_RANGE,          ///< out of spec.
    E_XC_MODEPARSE_EX_UNSUPPORT_MODE,        ///< can't find in mode tables
    E_XC_MODEPARSE_EX_SUPPORT_MODE,          ///< support mode
} XC_MODEPARSE_EX_RESULT;

typedef struct
{
    // mode parse result
    MS_U8  u8ModeIndex;                    ///< OUT: mode parse will return the mode index by this variable
    MS_U16 u16ResWidth;                    ///< OUT: indicate the resolution -> width
    MS_U16 u16ResHeight;                   ///< OUT: indicate the resolution -> height

    // src type
    XC_EX_INPUT_SOURCE_TYPE eSrcType;       ///< IN: input source type

    // Analog
    MS_U16 u16HFreqx10;                    ///< IN: HFreq x 10, unit KHz after / 10
    MS_U16 u16VFreqx10;                    ///< IN: VFreq x 10, unit Hz after / 10
    MS_U16 u16Vtotal;                      ///< IN: Vertical total
    MS_U8  u8SyncStatus;                   ///< IN: from XC driver, including H/V sync polarity, interlaced or progressive, mode parse will or XC_EX_MD_USER_MODE_BIT if new mode found

    // Digital
    XC_EX_WINDOW_TYPE sDVI_HDMI_DE_Info;   ///< IN: DVI/HDMI input DE information

    // EuroHDTV detected or not
    XC_EX_DETECT_EURO_HDTV_STATUS_TYPE eEuroHDTVStatus;    ///< IN: timing monitor detect Euro/Aus HDTV status
} XC_MODEPARSE_EX_INPUT_INFO;  ///< the neccesary input timing information for mode search

typedef enum
{
    E_XC_MODEPARSE_EX_RES_640X350,     // 00
    E_XC_MODEPARSE_EX_RES_640X400,     // 01
    E_XC_MODEPARSE_EX_RES_720X400,     // 02
    E_XC_MODEPARSE_EX_RES_640X480,     // 03
    E_XC_MODEPARSE_EX_RES_800X600,     // 04
    E_XC_MODEPARSE_EX_RES_832X624,     // 05
    E_XC_MODEPARSE_EX_RES_1024X768,    // 06
    E_XC_MODEPARSE_EX_RES_1280X1024,   // 07
    E_XC_MODEPARSE_EX_RES_1600X1200,   // 08
    E_XC_MODEPARSE_EX_RES_1152X864,    // 09
    E_XC_MODEPARSE_EX_RES_1152X870,    // 10
    E_XC_MODEPARSE_EX_RES_1280X768,    // 11
    E_XC_MODEPARSE_EX_RES_1280X960,    // 12
    E_XC_MODEPARSE_EX_RES_720X480,     // 13
    E_XC_MODEPARSE_EX_RES_1920X1080,   // 14
    E_XC_MODEPARSE_EX_RES_1280X720,    // 15
    E_XC_MODEPARSE_EX_RES_720X576,     // 16
    E_XC_MODEPARSE_EX_RES_1920X1200,   // 17
    E_XC_MODEPARSE_EX_RES_1400X1050,   // 18
    E_XC_MODEPARSE_EX_RES_1440X900,    // 19
    E_XC_MODEPARSE_EX_RES_1680X1050,   // 20
    E_XC_MODEPARSE_EX_RES_1280X800,    // 21
    E_XC_MODEPARSE_EX_RES_1600X1024,   // 22
    E_XC_MODEPARSE_EX_RES_1600X900,    // 23
    E_XC_MODEPARSE_EX_RES_1360X768,    // 24
    E_XC_MODEPARSE_EX_RES_848X480,     // 25
    E_XC_MODEPARSE_EX_RES_1920X1080P,  // 26
    E_XC_MODEPARSE_EX_RES_1366X768,    // 27
    E_XC_MODEPARSE_EX_RES_864X648,     // 28
    E_XC_MODEPARSE_EX_RES_1280X1470,   // 29
    E_XC_MODEPARSE_EX_RES_1920X2205,   // 30
    E_XC_MODEPARSE_EX_RES_720X240,     // 31
    E_XC_MODEPARSE_EX_RES_720x288,     // 32
    E_XC_MODEPARSE_EX_RES_1152X900,    // 33
    E_XC_MODEPARSE_EX_RES_856x480,     // 34
    E_XC_MODEPARSE_EX_RES_MAXIMUM
} XC_MODEPARSE_EX_RESOLUTION_TYPE;

typedef struct
{
    XC_MODEPARSE_EX_RESOLUTION_TYPE enResolutionIndex;   ///< resolution

    MS_U16 u16HorizontalFrequency;             ///< Horizontal frequency
    MS_U16 u16VerticalFrequency;               ///< Vertical frequency

    MS_U16 u16HorizontalStart;                 ///< Horizontal start
    MS_U16 u16VerticalStart;                   ///< Vertical start

    MS_U16 u16HorizontalTotal;                 ///< Horizontal Total
    MS_U16 u16VerticalTotal;                   ///< Vertical Total
    MS_U8 u8VTotalTolerance;                   ///< VTotal tolerance

    MS_U8 u8AdcPhase;                          ///< ADC phase

    MS_U8 u8StatusFlag; ///< flags
    // b0: VSync polarity(1/0 = positive/negative)
    // b1: HSync polarity(1/0 = positive/negative)
    // b2: Sync polarity care bit
    // b4: interlace mode
} XC_MODEPARSE_EX_PCADC_MODETABLE_TYPE;

typedef struct
{
    XC_MODEPARSE_EX_RESOLUTION_TYPE enResolutionIndex;   ///< resolution

    MS_U16 u16HorizontalFrequency;             ///< Horizontal frequency
    MS_U16 u16VerticalFrequency;               ///< Vertical frequency

    MS_U16 u16HorizontalStart;                 ///< Horizontal start
    MS_U16 u16VerticalStart;                   ///< Vertical start

    MS_U16 u16HorizontalTotal;                 ///< Horizontal Total
    MS_U16 u16VerticalTotal;                   ///< Vertical Total
    MS_U16 u16VTotalTolerance;                   ///< VTotal tolerance

    MS_U16 u16AdcPhase;                          ///< adc phase

    MS_U8 u8StatusFlag; ///< flags
    // b0: VSync polarity(1/0 = positive/negative)
    // b1: HSync polarity(1/0 = positive/negative)
    // b2: Sync polarity care bit
    // b4: interlace mode
} XC_MODEPARSE_EX_PCADC_MODETABLE_TYPE_EX;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC MS_BOOL MApi_XC_ModeParse_EX_Init(XC_DEVICE_ID *pDeviceId);
// mode search

DLL_PUBLIC XC_MODEPARSE_EX_RESULT MApi_XC_ModeParse_EX_MatchMode(XC_DEVICE_ID *pDeviceId, XC_MODEPARSE_EX_PCADC_MODETABLE_TYPE* ModeDB, MS_U8 u16NumberOfItems , XC_MODEPARSE_EX_INPUT_INFO *psInputInfo);
DLL_PUBLIC XC_MODEPARSE_EX_RESULT MApi_XC_ModeParse_EX_MatchModeEx(XC_DEVICE_ID *pDeviceId, XC_MODEPARSE_EX_PCADC_MODETABLE_TYPE_EX* ModeDB, MS_U8 u16NumberOfItems , XC_MODEPARSE_EX_INPUT_INFO *psInputInfo);
#ifdef __cplusplus
}
#endif

#endif /* _API_XC_MODEPARSE_EX_H_ */
