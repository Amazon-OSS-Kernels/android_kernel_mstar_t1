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

#ifndef _XC_ADC_H_
#define _XC_ADC_H_

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
    MS_BOOL bEnable;
    E_DEST_TYPE enOutputType;
    INPUT_SOURCE_TYPE_t enInputPortType;
    MS_BOOL isDigitalSource;
} stXC_ADC_SETCVBSOUT, *pstXC_ADC_SETCVBSOUT;


typedef struct
{
    E_DEST_TYPE enOutputType;
    MS_BOOL bReturnValue;
} stXC_ADC_ISCVBSOUTENABLED, *pstXC_ADC_ISCVBSOUTENABLED;

typedef struct
{
    MS_U16 u16Value;
} stXC_ADC_SETPCCLOCK, *pstXC_ADC_SETPCCLOCK;

typedef struct
{
    MS_U8 u8Value;
} stXC_ADC_SETPHASE, *pstXC_ADC_SETPHASE;

typedef struct
{
    MS_U16 u16Value;
} stXC_ADC_SETPHASEEX, *pstXC_ADC_SETPHASEEX;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETPHASERANGE, *pstXC_ADC_GETPHASERANGE;

typedef struct
{
    MS_U8 u8ReturnValue;
} stXC_ADC_GETPHASE, *pstXC_ADC_GETPHASE;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETPHASEEX, *pstXC_ADC_GETPHASEEX;

typedef struct
{
    MS_BOOL bReturnValue;
} stXC_ADC_ISSCARTRGB, *pstXC_ADC_ISSCARTRGB;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETPCCLOCK, *pstXC_ADC_GETPCCLOCK;

typedef struct DLL_PACKED
{
    MS_U32 *u32min;
    MS_U32 *u32max;
    MS_U32 *u32Recommend_value;
} stXC_ADC_GETSOGLEVELRANGE, *pstXC_ADC_GETSOGLEVELRANGE;

typedef struct
{
    MS_U32 u32Value;
} stXC_ADC_SETSOGLEVEL, *pstXC_ADC_SETSOGLEVEL;


typedef struct
{
    INPUT_SOURCE_TYPE_t enSource;
    APIXC_AdcGainOffsetSetting* pstADCSetting;
} stXC_ADC_GETDEFAULTGAINOFFSET, *pstXC_ADC_GETDEFAULTGAINOFFSET;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETMAXIMALOFFSETVALUE, *pstXC_ADC_GETMAXIMALOFFSETVALUE;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETMAXIMALGAINVALUE, *pstXC_ADC_GETMAXIMALGAINVALUE;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETCENTERGAIN, *pstXC_ADC_GETCENTERGAIN;

typedef struct
{
    MS_U16 u16ReturnValue;
} stXC_ADC_GETCENTEROFFSET, *pstXC_ADC_GETCENTEROFFSET;

typedef struct
{
    MS_U8 u8Color;
    MS_U16 u16Value;
} stXC_ADC_SETGAIN, *pstXC_ADC_SETGAIN;

typedef struct
{
    MS_U8 u8Color;
    MS_U16 u16Value;
} stXC_ADC_SETOFFSET, *pstXC_ADC_SETOFFSET;

typedef struct DLL_PACKED
{
    APIXC_AdcGainOffsetSetting* pstADCSetting;
} stXC_ADC_ADJUSTGAINOFFSET, *pstXC_ADC_ADJUSTGAINOFFSET;

typedef struct
{
    INPUT_SOURCE_TYPE_t enInputSourceType;
} stXC_ADC_SOURCE_CALIBRATE, *pstXC_ADC_SOURCE_CALIBRATE;

typedef struct
{
    MS_U8 u8Value;
} stXC_ADC_SETRGB_PIPE_DELAY, *pstXC_ADC_SETRGB_PIPE_DELAY;

typedef struct
{
    MS_U16 u16Clpdly;
    MS_U16 u16Caldur;
} stXC_ADC_SCARTRGB_SOG_CLAMPDELAY, *pstXC_ADC_SCARTRGB_SOG_CLAMPDELAY;

typedef struct
{
    MS_BOOL bEnable;
} stXC_ADC_SET_YPBPRLOOSELPF, *pstXC_ADC_SET_YPBPRLOOSELPF;

typedef struct
{
    MS_U16 u16Value;
} stXC_ADC_SET_SOGBW, *pstXC_ADC_SET_SOGBW;

typedef struct
{
    MS_U16 u16Value;
} stXC_ADC_SETCLAMPDURATION, *pstXC_ADC_SETCLAMPDURATION;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stXC_ADC_ENABLEHWCALIBRATION, *pstXC_ADC_ENABLEHWCALIBRATION;

typedef struct
{
    ADC_DacCurrentMode_t eMode;
    MS_BOOL bReturnValue;
} stXC_ADC_SETIDACCURRENTMODE, *pstXC_ADC_SETIDACCURRENTMODE;

typedef struct
{
    ADC_DacCurrentMode_t eReturnMode;
} stXC_ADC_GETIDACCURRENTMODE, *pstXC_ADC_GETIDACCURRENTMODE;


#ifdef __cplusplus
}
#endif

#endif //_XC_ADC_H_
