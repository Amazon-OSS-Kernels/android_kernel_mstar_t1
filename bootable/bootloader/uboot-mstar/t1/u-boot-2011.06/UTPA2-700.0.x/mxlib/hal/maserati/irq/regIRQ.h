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
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_IRQ_H_
#define _REG_IRQ_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define RIUBASE_IRQ         0x1900   //(0x101900-0x100000)
#define RIUBASE_IRQ_EXP     0x1900   //(0x101900-0x100000)
#define RIUBASE_IRQ_HYP     0x1000   //(0x101000-0x100000)

#define RIUBASE_IRQ_FRC     0x300500 //(0x400500-0x100000)

#if 1//(frcr2_integration###)
//################
//
// For FRC banks
// Host 0 : NON-PM
// Host 1 : FRC-R2
//
#define REG_FRC_C_FIQ_MASK                     (RIUBASE_IRQ_FRC + (0x24*2))
#define REG_FRC_C_FIQ_CLR                      (RIUBASE_IRQ_FRC + (0x2C*2))
#define REG_FRC_FIQ_FINAL_STATUS               (RIUBASE_IRQ_FRC + (0x2C*2))

#define REG_FRC_C_IRQ_MASK                     (RIUBASE_IRQ_FRC + (0x34*2))
#define REG_FRC_IRQ_FINAL_STATUS               (RIUBASE_IRQ_FRC + (0x3C*2))

#define REG_FRC_C_FIQ_EXP_MASK                 (RIUBASE_IRQ_FRC + (0x24*2))
#define REG_FRC_C_FIQ_EXP_CLR                  (RIUBASE_IRQ_FRC + (0x2C*2))
#define REG_FRC_C_FIQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_FRC + (0x2C*2))

#define REG_FRC_C_FIQ_HYP_MASK                 (RIUBASE_IRQ_FRC + (0x24*2))
#define REG_FRC_C_FIQ_HYP_CLR                  (RIUBASE_IRQ_FRC + (0x2C*2))
#define REG_FRC_C_FIQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_FRC + (0x2C*2))

#define REG_FRC_C_IRQ_EXP_MASK                 (RIUBASE_IRQ_FRC + (0x34*2))
#define REG_FRC_C_IRQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_FRC + (0x3C*2))

#define REG_FRC_C_IRQ_HYP_MASK                 (RIUBASE_IRQ_FRC + (0x34*2))
#define REG_FRC_C_IRQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_FRC + (0x3C*2))
#endif

//################
//
// For ARM Core0+SEC-R2 in NonPM
// Host 1 : ARM CA9 Core 0
// Host 3 : SEC-R2
//
#define REG_AEON_C_FIQ_MASK                     (RIUBASE_IRQ + (0x64*2))
#define REG_AEON_C_FIQ_CLR                      (RIUBASE_IRQ + (0x6C*2))
#define REG_AEON_FIQ_FINAL_STATUS               (RIUBASE_IRQ + (0x6C*2))

#define REG_AEON_C_IRQ_MASK                     (RIUBASE_IRQ + (0x74*2))
#define REG_AEON_IRQ_FINAL_STATUS               (RIUBASE_IRQ + (0x7C*2))

#define REG_AEON_C_FIQ_EXP_MASK                 (RIUBASE_IRQ_EXP + (0x66*2)) //NOT EXIST NOW
#define REG_AEON_C_FIQ_EXP_CLR                  (RIUBASE_IRQ_EXP + (0x6E*2)) //NOT EXIST NOW
#define REG_AEON_C_FIQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_EXP + (0x6E*2)) //NOT EXIST NOW

#define REG_AEON_C_FIQ_HYP_MASK                 (RIUBASE_IRQ_HYP + (0x64*2)) //NOT EXIST NOW
#define REG_AEON_C_FIQ_HYP_CLR                  (RIUBASE_IRQ_HYP + (0x6C*2)) //NOT EXIST NOW
#define REG_AEON_C_FIQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_HYP + (0x6C*2)) //NOT EXIST NOW

#define REG_AEON_C_IRQ_EXP_MASK                 (RIUBASE_IRQ_EXP + (0x76*2)) //NOT EXIST NOW
#define REG_AEON_C_IRQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_EXP + (0x7E*2)) //NOT EXIST NOW

#define REG_AEON_C_IRQ_HYP_MASK                 (RIUBASE_IRQ_HYP + (0x74*2)) //NOT EXIST NOW
#define REG_AEON_C_IRQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_HYP + (0x7C*2)) //NOT EXIST NOW

//################
//
// For PM+MIPS in NonPM
// Host 0 : PM
// Host 1 : MIPS (V)
//
#define REG_MIPS_C_FIQ_MASK                     (RIUBASE_IRQ + 0x24*2)
#define REG_MIPS_C_FIQ_CLR                      (RIUBASE_IRQ + 0x2C*2)
#define REG_MIPS_FIQ_FINAL_STATUS               (RIUBASE_IRQ + 0x2C*2)

#define REG_MIPS_C_IRQ_MASK                     (RIUBASE_IRQ + 0x34*2)
#define REG_MIPS_IRQ_FINAL_STATUS               (RIUBASE_IRQ + 0x3C*2)

#define REG_MIPS_C_FIQ_EXP_MASK                 (RIUBASE_IRQ_EXP + 0x26*2)
#define REG_MIPS_C_FIQ_EXP_CLR                  (RIUBASE_IRQ_EXP + 0x2E*2)
#define REG_MIPS_C_FIQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_EXP + 0x2E*2)

#define REG_MIPS_C_FIQ_HYP_MASK                 (RIUBASE_IRQ_HYP + 0x24*2)
#define REG_MIPS_C_FIQ_HYP_CLR                  (RIUBASE_IRQ_HYP + 0x2C*2)
#define REG_MIPS_C_FIQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_HYP + 0x2C*2)

#define REG_MIPS_C_IRQ_EXP_MASK                 (RIUBASE_IRQ_EXP + 0x36*2)
#define REG_MIPS_C_IRQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_EXP + 0x3E*2)

#define REG_MIPS_C_IRQ_HYP_MASK                 (RIUBASE_IRQ_HYP + 0x34*2)
#define REG_MIPS_C_IRQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_HYP + 0x3C*2)

#ifdef MCU_AEON
#if defined(CONFIG_FRC)//frcr2_integration###
#define REG_C_FIQ_MASK                          REG_FRC_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_FRC_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_FRC_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_FRC_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_FRC_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_FRC_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_FRC_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_FRC_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_FRC_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_FRC_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_FRC_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_FRC_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_FRC_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_FRC_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_FRC_C_IRQ_HYP_FINAL_STATUS
#else
#define REG_C_FIQ_MASK                          REG_AEON_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_AEON_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_AEON_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_AEON_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_AEON_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_AEON_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_AEON_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_AEON_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_AEON_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_AEON_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_AEON_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_AEON_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_AEON_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_AEON_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_AEON_C_IRQ_HYP_FINAL_STATUS
#endif
#endif


#ifdef MCU_MIPS_34K
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS

#endif


#ifdef MCU_MIPS_4KE

#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS

#endif

#ifdef MCU_MIPS_74K
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS
#endif

#ifdef MCU_ARM_CA12
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS
#endif


#ifdef MCU_ARM_CA7
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS
#endif

/*
#define REG_SEND_IRQ_FROM_CPU0                  (RIUBASE_IRQ + 0x7C)
#define REG_SEND_IRQ_FROM_CPU1                  (RIUBASE_IRQ + 0x7E)
#define REG_SEND_IRQ_FROM_CPU2                  (RIUBASE_IRQ_AEON1 + 0x7E)
*/

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_IRQ_H_

