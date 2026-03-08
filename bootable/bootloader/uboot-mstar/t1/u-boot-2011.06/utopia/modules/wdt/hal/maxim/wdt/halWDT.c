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
/// @file halWDT.h
/// @brief Piu Watch Dog Timer hal
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Include Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsTypes.h"
#include "halWDT.h"
#include "regWDT.h"

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

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_WDT_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_WDT_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_WDT_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_WDT_Read2Byte(u32RegAddr) | (HAL_WDT_Read2Byte(u32RegAddr+2) << 16));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    HAL_WDT_Write2Byte(u32RegAddr, u32Val & 0x0000FFFFUL);
    HAL_WDT_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Start
/// @brief \b Function  \b Description: Stop WDT operations
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Start(void)
{
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Stop
/// @brief \b Function  \b Description: Stop WDT operations
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Stop(void)
{
    MS_BOOL bRet = TRUE;

    do {
        bRet = HAL_WDT_Write4Byte(REG_WDT_MAX, WDT_CLOSE_KEY);
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Clear
/// @brief \b Function  \b Description: Clear WDT.
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Clear(void)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_WriteByte(REG_WDT_CLR, WDT_RST);
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_ClearRstFlag
/// @brief \b Function  \b Description: Clear WDT reset flag.
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done ;FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_ClearRstFlag(void)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_WriteByte(REG_WDT_RST, WDT_CLR_RESET_FLAG);
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_IsReset
/// @brief \b Function  \b Description: Read the reset status
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: reset at least once, FALSE: no reset occurs.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_IsReset(void)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = ((HAL_WDT_ReadByte(REG_WDT_RST)) & (WDT_RST));
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_IsEnable
/// @brief \b Function  \b Description: Check wdt enable or  not.
/// @param <IN>         \b u8Ch:
/// @param <OUT>       \b None :
/// @param <RET>        \b TRUE: has started, FALSE: not yet.
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_IsEnable(void)
{
    MS_BOOL bRet = FALSE;

    if (HAL_WDT_Read4Byte(REG_WDT_MAX) != WDT_CLOSE_KEY) {
        bRet = TRUE;
    }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetTimer
/// @brief \b Function  \b Description: wdt interval 65536*(65536-wdt_sel) cycles
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done, FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_SetTimer(MS_U16 sec)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_Write4Byte(REG_WDT_MAX, HAL_WDTCycles(sec));
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetIntTimer
/// @brief \b Function  \b Description: when wdt_cnt[31:16]>reg_wdt_int_sel, wdt_int occurs
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done, FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_SetIntTimer(MS_U16 sec)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_Write2Byte(REG_WDT_INT, HAL_WDTCycles(sec)>>16);
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetTimer
/// @brief \b Function  \b Description: wdt interval 65536*(65536-wdt_sel) cycles
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done, FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_SetTimer_ms(MS_U16 msec)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_Write4Byte(REG_WDT_MAX, HAL_WDTCyclesM(msec));
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetTimer
/// @brief \b Function  \b Description: wdt interval 65536*(65536-wdt_sel) cycles
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done, FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_SetTimer_us(MS_U16 usec)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_Write4Byte(REG_WDT_MAX, HAL_WDTCyclesU(usec));
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetIOMapBase
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_WDT_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIO_MapBase = virtBase;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Get_IOMap_Base
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_WDT_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}
