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

#ifndef _XC_AUTO_H_
#define _XC_AUTO_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    XC_Auto_TuneType enAutoTuneType;
    XC_Auto_Signal_Info *ActiveInfo;
    XC_Auto_Signal_Info *StandardInfo;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_AUTO_GEOMETRY, *pstXC_AUTO_GEOMETRY;

typedef struct
{
    XC_Auto_TuneType enAutoTuneType;
    XC_Auto_Signal_Info_Ex *ActiveInfo;
    XC_Auto_Signal_Info_Ex *StandardInfo;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_AUTO_GEOMETRY_EX, *pstXC_AUTO_GEOMETRY_EX;

typedef struct
{
    MS_BOOL bReturnValue;
} stXC_AUTO_STOPAUTOGEOMETRY, *pstXC_AUTO_STOPAUTOGEOMETRY;

typedef struct
{
    XC_Auto_CalibrationType type;
    XC_Auto_TuneType enAutoTuneType;
    APIXC_AdcGainOffsetSetting *pstADCSetting;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_AUTO_GAINOFFSET, *pstXC_AUTO_GAINOFFSET;

typedef struct
{
    XC_Auto_TuneType enAutoTuneType;
    APIXC_AdcGainOffsetSetting *pstADCSetting;
    MS_BOOL bReturnValue;
} stXC_AUTO_GETHWFIXEDGAINOFFSET, *pstXC_AUTO_GETHWFIXEDGAINOFFSET;

typedef struct
{
    MS_U8 Value;
    MS_BOOL bReturnValue;
} stXC_AUTO_SETVALIDDATA, *pstXC_AUTO_SETVALIDDATA;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPrFlag;
} stXC_AUTO_AUTOOFFSET, *pstXC_AUTO_AUTOOFFSET;

typedef struct
{
    XC_Auto_Signal_Info_Ex *pSignalInfo;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_AUTO_DETECTWIDTH, *pstXC_AUTO_DETECTWIDTH;

typedef struct
{
    XC_Auto_CalibrationMode eMode;
    MS_BOOL bReturnValue;
} stXC_AUTO_SETCALIBRATIONMODE, *pstXC_AUTO_SETCALIBRATIONMODE;

typedef struct
{
    XC_Auto_CalibrationMode* eMode;
} stXC_AUTO_GETCALIBRATIONMODE, *pstXC_AUTO_GETCALIBRATIONMODE;

typedef struct
{
    XC_AUTO_SYNC_INFO* pSyncInfo;
} stXC_AUTO_GETSYNCINFO, *pstXC_AUTO_GETSYNCINFO;

#ifdef __cplusplus
}
#endif

#endif //_XC_AUTO_H_

