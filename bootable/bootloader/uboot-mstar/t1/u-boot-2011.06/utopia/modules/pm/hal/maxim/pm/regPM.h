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

#ifndef _REGPM_H_
#define _REGPM_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
//v: value n: shift n bits
#define __BIT(x)    ((MS_U8)(1 << (x)))
#define __BIT0       __BIT(0)
#define __BIT1       __BIT(1)
#define __BIT2       __BIT(2)
#define __BIT3       __BIT(3)
#define __BIT4       __BIT(4)
#define __BIT5       __BIT(5)
#define __BIT6       __BIT(6)
#define __BIT7       __BIT(7)


//-----------------------------------------------------------------------------
// PM_SLEEP register
//-----------------------------------------------------------------------------
#define PM_REG_BASE                            (0x0700UL*2)
#define PM_MSIC_REG_BASE                       (0x1700UL*2)
#define PMSLEEP_REG_GPIO                       (0x0780UL*2)

//                                              bank, regiter
#define REG_PM_DUMMY_ACTIVE_STANDBY       ((PM_REG_BASE + 0x38*2+0))
#define REG_PM_DUMMY_POWERON_MODE         ((PM_REG_BASE + 0x38UL*2+1))
#define REG_PM_DUMMY_WAKEUP_SOURCE		  ((PM_REG_BASE + 0x39UL*2+0))
#define REG_PM_CKG_RTC                    ((PM_REG_BASE + 0x22UL*2+0))
#define REG_PM_CHIP_CFG_OVERWRITE         ((PM_REG_BASE + 0x1FUL*2+0))
	#define CHIP_CFG_MIPS_EN			  __BIT2
	#define CHIP_CFG_MIPS_VAL			  __BIT6
#define REG_PM_CPU_SW_RST                 ((PM_MSIC_REG_BASE + 0x29UL*2+1))
	#define PM_51_SW_RST				  __BIT4

#define PM_REG1_ScratchPad                 ((PM_REG_BASE + 0x52UL*2))
#define PM_REG2_ScratchPad                 ((PM_REG_BASE + 0x53UL*2))
#define PM_REG3_ScratchPad                 ((PM_REG_BASE + 0x54UL*2))
#define PM_REG4_ScratchPad                 ((PM_REG_BASE + 0x55UL*2))

#define PMGPIO_OEN                          __BIT0
#define PMGPIO_OUT                          __BIT1
#define PMGPIO_IN                           __BIT2
#define REG_PM_GPIO4_OEN                    ((PMSLEEP_REG_GPIO + 0x04UL*2))
#define REG_PM_GPIO4_OUT                    ((PMSLEEP_REG_GPIO + 0x04UL*2))
#define REG_PM_GPIO_PM_LOCK                 ((PM_REG_BASE + 0x12UL*2))
//-----------------------------------------------------------------------------
// PM_SLEEP RTC register
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
// RTC Reg
//------------------------------------------------------------------------------
#define PMRTC_REG_BASE                            (0x0900UL*2)
//                                              bank, regiter


#define REG_PMRTC_CTRL            ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x00UL*2))//RTC_REG_BASE + 0x00

#define PMRTC_CTRL_NOT_RSTZ       (1 << 0)
#define PMRTC_CTRL_CNT_EN         (1 << 1)
#define PMRTC_CTRL_WRAP_EN        (1 << 2)
#define PMRTC_CTRL_LOAD_EN        (1 << 3)
#define PMRTC_CTRL_READ_EN        (1 << 4)
#define PMRTC_CTRL_INT_MASK       (1 << 5)
#define PMRTC_CTRL_INT_FORCE      (1 << 6)
#define PMRTC_CTRL_INT_CLEAR      (1 << 7)


#define REG_PMRTC_FREQ_CW         ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x01UL*2))//(RTC_REG_BASE + 0x01 * 2)
#define REG_PMRTC_LOAD_VAL        ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x03UL*2))//(RTC_REG_BASE + 0x03 * 2)
#define REG_PMRTC_MATCH_VAL       ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x07UL*2))//(RTC_REG_BASE + 0x05 * 2)
#define REG_PMRTC_INTERRUPT       ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x00UL*2 + 1))//(RTC_REG_BASE + 0x07 * 2)

#define PMRTC_INT_RAW_STATUS      (1 << 0)
#define PMRTC_INT_STATUS          (1 << 1)

#define REG_PMRTC_CNT             ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x0bUL*2))//(PMRTC_REG_BASE + 0x08 * 2)


//------------------------------------------------------------------------------
// RTC1 Reg
//------------------------------------------------------------------------------
#define PMRTC1_REG_BASE                            (0x0980UL*2)
//                                              bank, regiter


#define REG_PMRTC1_CTRL            ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x00UL*2))//RTC_REG_BASE + 0x00

#define PMRTC1_CTRL_NOT_RSTZ       (1 << 0)
#define PMRTC1_CTRL_CNT_EN         (1 << 1)
#define PMRTC1_CTRL_WRAP_EN        (1 << 2)
#define PMRTC1_CTRL_LOAD_EN        (1 << 3)
#define PMRTC1_CTRL_READ_EN        (1 << 4)
#define PMRTC1_CTRL_INT_MASK       (1 << 5)
#define PMRTC1_CTRL_INT_FORCE      (1 << 6)
#define PMRTC1_CTRL_INT_CLEAR      (1 << 7)


#define REG_PMRTC1_FREQ_CW         ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x01UL*2))//(RTC_REG_BASE + 0x01 * 2)
#define REG_PMRTC1_LOAD_VAL        ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x03UL*2))//(RTC_REG_BASE + 0x03 * 2)
#define REG_PMRTC1_MATCH_VAL       ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x07UL*2))//(RTC_REG_BASE + 0x05 * 2)
#define REG_PMRTC1_INTERRUPT       ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x00UL*2 + 1))//(RTC_REG_BASE + 0x07 * 2)

#define PMRTC1_INT_RAW_STATUS      (1 << 0)
#define PMRTC1_INT_STATUS          (1 << 1)

#define REG_PMRTC1_CNT             ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x0bUL*2))//(PMRTC_REG_BASE + 0x08 * 2)




#endif  //_REGPM_H_
