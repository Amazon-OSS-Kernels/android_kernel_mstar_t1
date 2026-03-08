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
#define SEAL_TZPC_NONPM                         (0x00023900UL)
#define SEAL_TZPC_PM                            (0x00003900UL)

#define RANGE_ADDR_OFFSET   (0x08UL)
#define RANGE_ADDR_HBYTE_OFFSET    (0x01UL)

// MIU0 secure range
#define REG_SECURE0_RANGE0_START_ADDR           (SEAL_SECURE0_RANGE0+0x00UL)
#define REG_SECURE0_RANGE0_END_ADDR             (SEAL_SECURE0_RANGE0+0x04UL)
#define REG_SECURE0_RANGE0_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x06UL)

#define REG_SECURE0_DETECT_ENABLE               (SEAL_SECURE0_RANGE0+0xECUL)

// MIU1 secure range
#define REG_SECURE1_RANGE0_START_ADDR           (SEAL_SECURE1_RANGE0+0x00UL)
#define REG_SECURE1_RANGE0_END_ADDR             (SEAL_SECURE1_RANGE0+0x04UL)
#define REG_SECURE1_RANGE0_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x06UL)

#define REG_SECURE1_DETECT_ENABLE               (SEAL_SECURE1_RANGE0+0xECUL)

#define SECURE_RANGE_ADDR_DYN_CH             (BIT15)

//Secure range hitted log
#define REG_SECURE0_HITTED_STATUS              (SEAL_SECURE0_RANGE0+0xE0UL)
#define REG_SECURE1_HITTED_STATUS              (SEAL_SECURE1_RANGE0+0xE0UL)
#define REG_SECURE0_HITTED_ADDR                (SEAL_SECURE0_RANGE0+0xE2UL)
#define REG_SECURE1_HITTED_ADDR                (SEAL_SECURE1_RANGE0+0xE2UL)
#define REG_SECURE0_HITTED_RANGE_ID            (SEAL_SECURE0_RANGE0+0xEEUL)
#define REG_SECURE1_HITTED_RANGE_ID            (SEAL_SECURE1_RANGE0+0xEEUL)
#define REG_SECURE_HITTED_LOG_CLR              (BIT0)
#define REG_SECURE_HITTED_IRQ_MASK             (BIT1)
#define REG_SECURE_HITTED_FALG                 (BIT2)
#define REG_SECURE_HITTED_IS_NONSECURE         (BIT3)
#define REG_SECURE_HITTED_IS_WRITE             (BIT15)
#define REG_SECURE_HITTED_CLIENT_ID            14:8
#define REG_SECURE_HITTED_RANGE_ID             4:0
#define GET_HIT_RANGE_ID(regval)               BITS_RANGE_VAL(regval, REG_SECURE_HITTED_RANGE_ID)
#define GET_HIT_CLIENT_ID(regval)              BITS_RANGE_VAL(regval, REG_SECURE_HITTED_CLIENT_ID)

//Secure range lock
#define REG_SECURE0_LOCK                       (SEAL_SECURE0_RANGE0+0xFEUL)
#define REG_SECURE1_LOCK                       (SEAL_SECURE1_RANGE0+0xFEUL)

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
#define REG_TZPC_BUFFER_LOCK                   (SEAL_TZPC_NONPM+0xC8UL)

#endif // _REG_SEAL_H_

