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
#define _HAL_RTC_C

////////////////////////////////////////////////////////////////////////////////
/// @file halWDT.h
/// @brief Piu Watch Dog Timer hal
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Include Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsTypes.h"
#include "halRTC.h"
#include "regRTC.h"

////////////////////////////////////////////////////////////////////////////////
//  Driver Compiler Options
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Global Variables
////////////////////////////////////////////////////////////////////////////////
static  MS_VIRT  _gMIO_MapBase = 0;

////////////////////////////////////////////////////////////////////////////////
//  Global Functions
////////////////////////////////////////////////////////////////////////////////

#if 0
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 HAL_RTC_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U16 HAL_RTC_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

static MS_U32 HAL_RTC_Read4Byte(MS_U32 u32RegAddr)
{
     MS_U32 u32Val =((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr+2];
     u32Val<<=16;
     u32Val|=((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
     return u32Val;
}

#if 0
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_RTC_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_RTC_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_RTC_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

static void HAL_RTC_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    HAL_RTC_Write2Byte(u32RegAddr,(MS_U16)(u32Val&0xFFFF));
    HAL_RTC_Write2Byte(u32RegAddr+2,(MS_U16)(u32Val>>16));
}

static MS_BOOL HAL_RTC_WriteBit(MS_U32 u32RegAddr, MS_U16 u16Val, MS_BOOL bEnable)
{
    MS_U16 u16Reg;

    if (!u32RegAddr)
    {
        return FALSE;
    }

    u16Reg = ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
    if(bEnable)
        u16Reg|=u16Val;
    else
        u16Reg&=(~u16Val);
    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Reg;
    return TRUE;
}

static MS_U32 HAL_RTC_GET_BASE(E_MS_RTC eRtc)
{
    MS_U32 u32RegAddr=0;
    switch(eRtc)
    {
        case E_RTC_0:
            u32RegAddr=REG_RTC_BASE_0;
            break;
        case E_RTC_2:
            u32RegAddr=REG_RTC_BASE_2;
            break;
    }
    return u32RegAddr;
}

void HAL_RTC_RESET(E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_SOFT_RSTZ_BIT,bEnable);
}

void HAL_RTC_Counter(E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_CNT_EN_BIT,bEnable);
}

void HAL_RTC_Wrap_Count (E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_WRAP_EN_BIT,bEnable);
}

void HAL_RTC_Loading (E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_LOAD_EN_BIT,bEnable);
}

void HAL_RTC_Reading(E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_READ_EN_BIT,bEnable);
}

void HAL_RTC_IntMask(E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_INT_MASK_BIT,bEnable);
}

void HAL_RTC_IntForce(E_MS_RTC eRtc)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_INT_FORCE_BIT,ENABLE);
}

void HAL_RTC_IntClear(E_MS_RTC eRtc)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_INT_CLEAR_BIT,ENABLE);
}

void HAL_RTC_Set_Frequency(E_MS_RTC eRtc,MS_U32 u32Freq, MS_U32 u32Xtal)
{
    if(u32Freq)
    {
        MS_U32 u32Reg = u32Xtal/u32Freq;
        HAL_RTC_Write4Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_FREQ_CW,  u32Reg);
    }
}

void HAL_RTC_Set_Counter(E_MS_RTC eRtc,MS_U32 u32Val)
{
    HAL_RTC_Write4Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_LOAD_VAL, u32Val);
}

void HAL_RTC_Match_Counter(E_MS_RTC eRtc,MS_U32 u32Val)
{
    HAL_RTC_Write4Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_MATCH_VAL,  u32Val);
}

MS_U16 HAL_RTC_INT_Status(E_MS_RTC eRtc)
{
    return HAL_RTC_Read2Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_INT);
}

MS_U32 HAL_RTC_Read_Counter(E_MS_RTC eRtc)
{
    MS_U32 u32Reg;
    MS_U16 u16Dummy=100;
    HAL_RTC_Reading(eRtc, ENABLE);
    while(u16Dummy--); //wait for HW latch bits okay, otherwise sometimes it read wrong value
    u32Reg = HAL_RTC_Read4Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CNT);
    HAL_RTC_Reading(eRtc, DISABLE);
    return u32Reg;
}

void HAL_RTC_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIO_MapBase = virtBase;
}

MS_VIRT HAL_RTC_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}
