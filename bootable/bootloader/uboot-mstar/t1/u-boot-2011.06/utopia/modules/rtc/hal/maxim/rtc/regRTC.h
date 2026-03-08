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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regWDT.h
/// @brief  Piu Watch Dog Timer Registers Definition
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_RTC_H_
#define _REG_RTC_H_

////////////////////////////////////////////////////////////////////////////////
//  Macro and Define
////////////////////////////////////////////////////////////////////////////////

#define REG_RTC_BASE_0          (0x1200UL)
#define REG_RTC_BASE_2          (0x1300UL)

#define REG_RTC_CTRL_REG        (0x0000UL)
    #define RTC_SOFT_RSTZ_BIT       (0x0001UL)//BIT0
    #define RTC_CNT_EN_BIT          (0x0002UL)//BIT1
    #define RTC_WRAP_EN_BIT         (0x0004UL)//BIT2
    #define RTC_LOAD_EN_BIT         (0x0008UL)//BIT3
    #define RTC_READ_EN_BIT         (0x0010UL)//BIT4
    #define RTC_INT_MASK_BIT        (0x0020UL)//BIT5
    #define RTC_INT_FORCE_BIT       (0x0040UL)//BIT6
    #define RTC_INT_CLEAR_BIT       (0x0080UL)//BIT7
#define REG_RTC_FREQ_CW         (0x0002UL)    //BIT0-BIT31
#define REG_RTC_LOAD_VAL        (0x0006UL)    //BIT0-BIT63
#define REG_RTC_MATCH_VAL       (0x000EUL)    //BIT0-BIT63
#define REG_RTC_INT             (0x0000UL)
    #define RTC_RAW_INT_BIT         (0x0100UL)//BIT8
    #define RTC_INT_BIT             (0x0200UL)//BIT9
#define REG_RTC_CNT             (0x0016UL)    //BIT0-BIT63

////////////////////////////////////////////////////////////////////////////////
//  Type and Structure
////////////////////////////////////////////////////////////////////////////////

#endif // _REG_WDT_H_

