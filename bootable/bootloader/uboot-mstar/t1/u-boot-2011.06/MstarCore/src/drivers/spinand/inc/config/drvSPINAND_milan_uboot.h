/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regSPINAND.h
/// @brief  Serial Flash Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SPINAND_H_
#define _REG_SPINAND_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------

// !!! Uranus Serial Flash Notes: !!!
//  - The clock of DMA & Read via XIU operations must be < 3*CPU clock
//  - The clock of DMA & Read via XIU operations are determined by only REG_ISP_CLK_SRC; other operations by REG_ISP_CLK_SRC only
//  - DMA program can't run on DRAM, but in flash ONLY
//  - DMA from SPI to DRAM => size/DRAM start/DRAM end must be 8-B aligned


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define REG(Reg_Addr)               (*(volatile U16*)(Reg_Addr))
#define REG_OFFSET_SHIFT_BITS       2
#define GET_REG_ADDR(x, y)          (x+((y)<<REG_OFFSET_SHIFT_BITS))

// BASEADDR & BK
//------------------------------
#define RIU_PM_BASE                         0xBF000000
#define RIU_BASE                            0xBF200000

#define BK_ISP                              0x1000
#define BK_FSP                              0x1200
#define BK_QSPI                             0x1400
#define BK_PMSLP                            0x1E00
#define BK_CLK0                             0x1600
#define BK_BDMA                             0x1200
#define BK_CHIP                             0x3C00
#define FSP_REG_BASE_ADDR                   GET_REG_ADDR(RIU_BASE, BK_FSP)
#define QSPI_REG_BASE_ADDR                  GET_REG_ADDR(RIU_BASE, BK_QSPI)
#define PMSLP_REG_BASE_ADDR                 GET_REG_ADDR(RIU_BASE, BK_PMSLP)
#define CLK0_REG_BASE_ADDR                  GET_REG_ADDR(RIU_BASE, BK_CLK0)
#define BDMA_REG_BASE_ADDR                  GET_REG_ADDR(RIU_BASE, BK_BDMA)
#define CHIP_REG_BASE_ADDR                  GET_REG_ADDR(RIU_BASE, BK_CHIP)

#define REG_BANK_TIMER                      (0x1800)
#define TIMER_REG_BASE_ADDR                 GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER)
#define TIMER1_ENABLE                       GET_REG_ADDR(TIMER_REG_BASE_ADDR, 0x20)
#define TIMER1_HIT                          GET_REG_ADDR(TIMER_REG_BASE_ADDR, 0x21)
#define TIMER1_MAX_LOW                      GET_REG_ADDR(TIMER_REG_BASE_ADDR, 0x22)
#define TIMER1_MAX_HIGH                     GET_REG_ADDR(TIMER_REG_BASE_ADDR, 0x23)
#define TIMER1_CAP_LOW                      GET_REG_ADDR(TIMER_REG_BASE_ADDR, 0x24)
#define TIMER1_CAP_HIGH                     GET_REG_ADDR(TIMER_REG_BASE_ADDR, 0x25)

//----- Chip flash -------------------------
#include"drvSPINAND_reg.h"
#endif // _REG_SPINAND_H_
