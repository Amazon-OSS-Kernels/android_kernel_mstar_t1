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

#ifndef _XC_PCMONITOR_H_
#define _XC_PCMONITOR_H_

#ifdef __cplusplus
extern "C"
{
#endif



typedef struct
{
    MS_U8 u8MaxWindowNum;
    MS_BOOL bReturnValue;
} stXC_PCMONITOR_INIT, *pstXC_PCMONITOR_INIT;

typedef struct
{
    SCALER_WIN eWindow;
} stXC_PCMONITOR_RESTART, *pstXC_PCMONITOR_RESTART;

typedef struct
{
    MS_U16 u16TimingStableCounter;
    MS_U16 u16TimingNosyncCounter;
} stXC_PCMONITOR_SETTIMINGCOUNT, *pstXC_PCMONITOR_SETTIMINGCOUNT;

typedef struct
{
    INPUT_SOURCE_TYPE_t eCurrentSrc;
    SCALER_WIN eWindow;
    XC_PCMONITOR_STATUS eReturnValue;
} stXC_PCMONITOR, *pstXC_PCMONITOR;

typedef struct
{
    SCALER_WIN eWindow;
    XC_PCMONITOR_STATUS eReturnValue;
} stXC_PCMONITOR_GETCURRENTSTATE, *pstXC_PCMONITOR_GETCURRENTSTATE;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U8 u8ReturnValue;
} stXC_PCMONITOR_GETSYNCSTATUS, *pstXC_PCMONITOR_GETSYNCSTATUS;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U16 u16ReturnValue;
} stXC_PCMONITOR_GET_HFREQX10, *pstXC_PCMONITOR_GET_HFREQX10;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U32 u32ReturnValue;
} stXC_PCMONITOR_GET_HFREQX1K, *pstXC_PCMONITOR_GET_HFREQX1K;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U16 u16ReturnValue;
} stXC_PCMONITOR_GET_VFREQX10, *pstXC_PCMONITOR_GET_VFREQX10;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U32 u32ReturnValue;
} stXC_PCMONITOR_GET_VFREQX1K, *pstXC_PCMONITOR_GET_VFREQX1K;

typedef struct
{
    SCALER_WIN eWindow;
    MS_U16 u16ReturnValue;
} stXC_PCMONITOR_GET_VTOTAL, *pstXC_PCMONITOR_GET_VTOTAL;

typedef struct
{
    SCALER_WIN eWindow;
    MS_WINDOW_TYPE* msWin;
} stXC_PCMONITOR_GET_DVI_HDMI_DE_INFO, *pstXC_PCMONITOR_GET_DVI_HDMI_DE_INFO;

typedef struct
{
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_PCMONITOR_SYNCLOSS, *pstXC_PCMONITOR_SYNCLOSS;

typedef struct
{
    MS_BOOL bPollingOnly;
    SCALER_WIN eWindow;
    MS_BOOL bReturnValue;
} stXC_PCMONITOR_INVALIDTIMINGDETECT, *pstXC_PCMONITOR_INVALIDTIMINGDETECT;

typedef struct
{
    INPUT_SOURCE_TYPE_t eCurrentSrc;
    MS_U16 u16TimingStableCounter;
    MS_U16 u16TimingNosyncCounter;
    E_APIXC_ReturnValue eReturnValue;
} stXC_PCMONITOR_SETTIMINGCOUNTEX, *pstXC_PCMONITOR_SETTIMINGCOUNTEX;

#ifdef __cplusplus
}
#endif

#endif //_XC_PCMONITOR_H_

