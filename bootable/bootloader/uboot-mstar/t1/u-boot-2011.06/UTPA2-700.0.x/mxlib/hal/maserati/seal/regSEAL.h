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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regSEAL.h
/// @brief  SEAL Control Register Definition
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SEAL_H_
#define _REG_SEAL_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define BITS_RANGE(range)                       (BIT(((1)?range)+1) - BIT((0)?range))
#define BITS_RANGE_VAL(x, range)                ((x & BITS_RANGE(range)) >> ((0)?range))

#define SEAL_SECURE0_RANGE0                     (0x00023700UL)
#define SEAL_SECURE1_RANGE0                     (0x00023800UL)
#define SEAL_SECURE2_RANGE0                     (0x00021900UL)
#define SEAL_TZPC_NONPM                         (0x00023900UL)
#define SEAL_TZPC_PM                            (0x00003900UL)

//Secure range0
#define REG_SECURE0_RANGE0_START_ADDR           (SEAL_SECURE0_RANGE0+0x00UL)
#define REG_SECURE0_RANGE0_END_ADDR             (SEAL_SECURE0_RANGE0+0x08UL)
#define REG_SECURE0_RANGE0_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x0EUL)

#define REG_SECURE0_RANGE1_START_ADDR           (SEAL_SECURE0_RANGE0+0x10UL)
#define REG_SECURE0_RANGE1_END_ADDR             (SEAL_SECURE0_RANGE0+0x18UL)
#define REG_SECURE0_RANGE1_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x1EUL)

#define REG_SECURE0_RANGE2_START_ADDR           (SEAL_SECURE0_RANGE0+0x20UL)
#define REG_SECURE0_RANGE2_END_ADDR             (SEAL_SECURE0_RANGE0+0x28UL)
#define REG_SECURE0_RANGE2_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x2EUL)

#define REG_SECURE0_RANGE3_START_ADDR           (SEAL_SECURE0_RANGE0+0x30UL)
#define REG_SECURE0_RANGE3_END_ADDR             (SEAL_SECURE0_RANGE0+0x38UL)
#define REG_SECURE0_RANGE3_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x3EUL)

#define REG_SECURE0_RANGE4_START_ADDR           (SEAL_SECURE0_RANGE0+0x40UL)
#define REG_SECURE0_RANGE4_END_ADDR             (SEAL_SECURE0_RANGE0+0x48UL)
#define REG_SECURE0_RANGE4_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x4EUL)

#define REG_SECURE0_RANGE5_START_ADDR           (SEAL_SECURE0_RANGE0+0x50UL)
#define REG_SECURE0_RANGE5_END_ADDR             (SEAL_SECURE0_RANGE0+0x58UL)
#define REG_SECURE0_RANGE5_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x5EUL)

#define REG_SECURE0_RANGE6_START_ADDR           (SEAL_SECURE0_RANGE0+0x60UL)
#define REG_SECURE0_RANGE6_END_ADDR             (SEAL_SECURE0_RANGE0+0x68UL)
#define REG_SECURE0_RANGE6_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x6EUL)

#define REG_SECURE0_RANGE7_START_ADDR           (SEAL_SECURE0_RANGE0+0x70UL)
#define REG_SECURE0_RANGE7_END_ADDR             (SEAL_SECURE0_RANGE0+0x78UL)
#define REG_SECURE0_RANGE7_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x7EUL)

#define REG_SECURE0_DETECT_ENABLE               (SEAL_SECURE0_RANGE0+0xECUL)
//Secure range 1
#define REG_SECURE1_RANGE0_START_ADDR           (SEAL_SECURE1_RANGE0+0x00UL)
#define REG_SECURE1_RANGE0_END_ADDR             (SEAL_SECURE1_RANGE0+0x08UL)
#define REG_SECURE1_RANGE0_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x0EUL)

#define REG_SECURE1_RANGE1_START_ADDR           (SEAL_SECURE1_RANGE0+0x10UL)
#define REG_SECURE1_RANGE1_END_ADDR             (SEAL_SECURE1_RANGE0+0x18UL)
#define REG_SECURE1_RANGE1_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x1EUL)

#define REG_SECURE1_RANGE2_START_ADDR           (SEAL_SECURE1_RANGE0+0x20UL)
#define REG_SECURE1_RANGE2_END_ADDR             (SEAL_SECURE1_RANGE0+0x28UL)
#define REG_SECURE1_RANGE2_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x2EUL)

#define REG_SECURE1_RANGE3_START_ADDR           (SEAL_SECURE1_RANGE0+0x30UL)
#define REG_SECURE1_RANGE3_END_ADDR             (SEAL_SECURE1_RANGE0+0x38UL)
#define REG_SECURE1_RANGE3_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x3EUL)

#define REG_SECURE1_RANGE4_START_ADDR           (SEAL_SECURE1_RANGE0+0x40UL)
#define REG_SECURE1_RANGE4_END_ADDR             (SEAL_SECURE1_RANGE0+0x48UL)
#define REG_SECURE1_RANGE4_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x4EUL)

#define REG_SECURE1_RANGE5_START_ADDR           (SEAL_SECURE1_RANGE0+0x50UL)
#define REG_SECURE1_RANGE5_END_ADDR             (SEAL_SECURE1_RANGE0+0x58UL)
#define REG_SECURE1_RANGE5_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x5EUL)

#define REG_SECURE1_RANGE6_START_ADDR           (SEAL_SECURE1_RANGE0+0x60UL)
#define REG_SECURE1_RANGE6_END_ADDR             (SEAL_SECURE1_RANGE0+0x68UL)
#define REG_SECURE1_RANGE6_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x6EUL)

#define REG_SECURE1_RANGE7_START_ADDR           (SEAL_SECURE1_RANGE0+0x70UL)
#define REG_SECURE1_RANGE7_END_ADDR             (SEAL_SECURE1_RANGE0+0x78UL)
#define REG_SECURE1_RANGE7_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x7EUL)

#define REG_SECURE1_DETECT_ENABLE               (SEAL_SECURE1_RANGE0+0xECUL)
//Secure range 2
#define REG_SECURE2_RANGE0_START_ADDR           (SEAL_SECURE2_RANGE0+0x00UL)
#define REG_SECURE2_RANGE0_END_ADDR             (SEAL_SECURE2_RANGE0+0x08UL)
#define REG_SECURE2_RANGE0_ATTRIBUTE            (SEAL_SECURE2_RANGE0+0x0EUL)

#define REG_SECURE2_RANGE1_START_ADDR           (SEAL_SECURE2_RANGE0+0x10UL)
#define REG_SECURE2_RANGE1_END_ADDR             (SEAL_SECURE2_RANGE0+0x18UL)
#define REG_SECURE2_RANGE1_ATTRIBUTE            (SEAL_SECURE2_RANGE0+0x1EUL)

#define REG_SECURE2_RANGE2_START_ADDR           (SEAL_SECURE2_RANGE0+0x20UL)
#define REG_SECURE2_RANGE2_END_ADDR             (SEAL_SECURE2_RANGE0+0x28UL)
#define REG_SECURE2_RANGE2_ATTRIBUTE            (SEAL_SECURE2_RANGE0+0x2EUL)

#define REG_SECURE2_RANGE3_START_ADDR           (SEAL_SECURE2_RANGE0+0x30UL)
#define REG_SECURE2_RANGE3_END_ADDR             (SEAL_SECURE2_RANGE0+0x38UL)
#define REG_SECURE2_RANGE3_ATTRIBUTE            (SEAL_SECURE2_RANGE0+0x3EUL)

#define REG_SECURE2_RANGE4_START_ADDR           (SEAL_SECURE2_RANGE0+0x40UL)
#define REG_SECURE2_RANGE4_END_ADDR             (SEAL_SECURE2_RANGE0+0x48UL)
#define REG_SECURE2_RANGE4_ATTRIBUTE            (SEAL_SECURE2_RANGE0+0x4EUL)

#define REG_SECURE2_RANGE5_START_ADDR           (SEAL_SECURE2_RANGE0+0x50UL)
#define REG_SECURE2_RANGE5_END_ADDR             (SEAL_SECURE2_RANGE0+0x58UL)
#define REG_SECURE2_RANGE5_ATTRIBUTE            (SEAL_SECURE2_RANGE0+0x5EUL)

#define REG_SECURE2_RANGE6_START_ADDR           (SEAL_SECURE2_RANGE0+0x60UL)
#define REG_SECURE2_RANGE6_END_ADDR             (SEAL_SECURE2_RANGE0+0x68UL)
#define REG_SECURE2_RANGE6_ATTRIBUTE            (SEAL_SECURE2_RANGE0+0x6EUL)

#define REG_SECURE2_RANGE7_START_ADDR           (SEAL_SECURE2_RANGE0+0x70UL)
#define REG_SECURE2_RANGE7_END_ADDR             (SEAL_SECURE2_RANGE0+0x78UL)
#define REG_SECURE2_RANGE7_ATTRIBUTE            (SEAL_SECURE2_RANGE0+0x7EUL)

#define REG_SECURE2_DETECT_ENABLE               (SEAL_SECURE2_RANGE0+0xECUL)

//Secure range hitted log
#define REG_SECURE0_HITTED_STATUS              (SEAL_SECURE0_RANGE0+0xE0UL)
#define REG_SECURE1_HITTED_STATUS              (SEAL_SECURE1_RANGE0+0xE0UL)
#define REG_SECURE2_HITTED_STATUS              (SEAL_SECURE2_RANGE0+0xE0UL)
#define REG_SECURE0_HITTED_ADDR                (SEAL_SECURE0_RANGE0+0xE2UL)
#define REG_SECURE1_HITTED_ADDR                (SEAL_SECURE1_RANGE0+0xE2UL)
#define REG_SECURE2_HITTED_ADDR                (SEAL_SECURE2_RANGE0+0xE2UL)
#define REG_SECURE0_HITTED_AUXI_STATUS         (SEAL_SECURE0_RANGE0+0xECUL)
#define REG_SECURE1_HITTED_AUXI_STATUS         (SEAL_SECURE1_RANGE0+0xECUL)
#define REG_SECURE2_HITTED_AUXI_STATUS         (SEAL_SECURE2_RANGE0+0xECUL)
#define REG_SECURE_HITTED_LOG_CLR              (BIT0)
#define REG_SECURE_HITTED_IRQ_MASK             (BIT1)
#define REG_SECURE_HITTED_FALG                 (BIT2)
#define REG_SECURE_HITTED_IS_WRITE             (BIT15)
#define REG_SECURE_HITTED_CLIENT_ID            14:8
#define REG_SECURE_HITTED_RANGE_ID             6:3
#define GET_HIT_RANGE_ID(regval)               BITS_RANGE_VAL(regval, REG_SECURE_HITTED_RANGE_ID)
#define GET_HIT_CLIENT_ID(regval)              BITS_RANGE_VAL(regval, REG_SECURE_HITTED_CLIENT_ID)

//Secure range lock
#define REG_SECURE0_LOCK                       (SEAL_SECURE0_RANGE0+0xFEUL)
#define REG_SECURE1_LOCK                       (SEAL_SECURE1_RANGE0+0xFEUL)
#define REG_SECURE2_LOCK                       (SEAL_SECURE2_RANGE0+0xFEUL)
#define REG_SECURE3_LOCK                       (SEAL_SECURE3_RANGE0+0xFEUL)
#define REG_SECURE4_LOCK                       (SEAL_SECURE3_RANGE0+0xFEUL)
#define REG_SECURE5_LOCK                       (SEAL_SECURE3_RANGE0+0xFEUL)
#define REG_SECURE6_LOCK                       (SEAL_SECURE3_RANGE0+0xFEUL)
#define REG_SECURE7_LOCK                       (SEAL_SECURE3_RANGE0+0xFEUL)

//Non secure processor
#define REG_TZPC_NONSECURE_PROCESSOR           (SEAL_TZPC_NONPM+0x02UL)
#define REG_TZPC_NONPM_SECURE_SLAVE            (SEAL_TZPC_NONPM+0x20UL)
#define REG_TZPC_PM_SECURE_SLAVE               (SEAL_TZPC_PM+0x20UL)
#define REG_TZPC_NONPM_SECURE_MASTER           (SEAL_TZPC_NONPM+0x80UL)
#define REG_TZPC_NONSECURE_HEMCU               (SEAL_TZPC_NONPM+0x61UL)
#define REG_TZPC_PROTECT_CTL                   (SEAL_TZPC_NONPM+0x60UL)

//IMI secure range
#define REG_IMI_RANGE_START_ADDR               (SEAL_TZPC_NONPM+0xE0UL)
#define REG_IMI_RANGE_END_ADDR                 (SEAL_TZPC_NONPM+0xE4UL)

//Buffer lock
#define REG_TZPC_BUFFER_LOCK                   (SEAL_TZPC_NONPM+0xE8UL)
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_SEAL_H_

