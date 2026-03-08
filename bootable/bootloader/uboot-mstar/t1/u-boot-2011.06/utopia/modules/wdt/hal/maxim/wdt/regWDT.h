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

#ifndef _REG_WDT_H_
#define _REG_WDT_H_

////////////////////////////////////////////////////////////////////////////////
//  Macro and Define
////////////////////////////////////////////////////////////////////////////////

#define REG_WDT_BASE        ( 0x3000UL )
#define REG_TIMER0_BASE		( 0x3020UL )
#define REG_TIMER1_BASE		( 0x3040UL )

#define REG_WDT_SET(x)      ( REG_WDT_BASE + 2*x )
#define REG_TIMER0_SET(x)   ( REG_TIMER0_BASE + 2*x )
#define REG_TIMER1_SET(x)   ( REG_TIMER1_BASE + 2*x )


// WDT
#define REG_WDT_CLR         REG_WDT_SET(0x00UL)
#define REG_WDT_RST         REG_WDT_SET(0x02UL)
#define REG_WDT_INT         REG_WDT_SET(0x03UL)
#define REG_WDT_MAX         REG_WDT_SET(0x04UL)
#define REG_WDT_MAX2        REG_WDT_SET(0x05UL)

#define WDT_CLR_RESET_FLAG  BIT(0)
#define WDT_RST             BIT(0)

// Timer0
#define TIMER_0_CTRL_REG    REG_TIMER0_SET(0x00UL)
#define TIMER_0_MATCH_REG   REG_TIMER0_SET(0x01UL) //BIT0, RO
#define TIMER_0_MAX_REG		REG_TIMER0_SET(0x02UL) //BIT0-BIT31
#define TIMER_0_COUNT_REG   REG_TIMER0_SET(0x04UL) //BIT0-BIT31, RO
// Timer1
#define TIMER_1_CTRL_REG    REG_TIMER1_SET(0x00UL)
#define TIMER_1_MATCH_REG   REG_TIMER1_SET(0x01UL) //BIT0, RO
#define TIMER_1_MAX_REG		REG_TIMER1_SET(0x02UL) //BIT0-BIT31
#define TIMER_1_COUNT_REG   REG_TIMER1_SET(0x04UL) //BIT0-BIT31, RO

#define TIMER_ENABLE		BIT(0) //(from 0 to max, then rolled)
#define TIMER_TRIGGER		BIT(1) //(from 0 to max, then stop)
#define TIMER_DISABLE		BITS(1:0,0)
#define TIMER_INTEN         	BIT(0) //BIT8
#define TIMER_MATCH         	BIT(0)
////////////////////////////////////////////////////////////////////////////////
//  Type and Structure
////////////////////////////////////////////////////////////////////////////////

#endif // _REG_WDT_H_
