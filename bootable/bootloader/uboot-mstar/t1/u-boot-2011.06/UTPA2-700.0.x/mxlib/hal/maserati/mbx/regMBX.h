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
/// @file   regMBX.h
/// @brief  MStar Mailbox Driver Reg Def.
/// @attention
/// <b><em>For also support PM/big endian, the regs will only support byte operations \n
/// The short reg access will be removed! </em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MHAL_MBX_REG_H
#define _MHAL_MBX_REG_H

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Defines & Macros
//=============================================================================
#define RIU_MAP _virtRIUBaseAddrMBX

#define RIU8    (( MS_U8 volatile *) RIU_MAP)

#define REG_MBX_BASE                (0x19C0UL) //(0x103380-0x100000)/2
#if 1//frcr2_integration###
//###############################
//
//  FRC MBX Dummy Register Allocation
//  Bank : 0x400300
//  Reg16_offset : 0x40 ~ 47 (HKCPU to FRCR2)
//  Reg16_offset : 0x48 ~ 4F (FRCR2 to HKCPU)
//
//###############################
#define REG_MBX_FRC_BASE            (0x1801C0UL) //(0x400380-0x100000)/2
#define REG_MBX_FRC_OFST            (REG_MBX_FRC_BASE - REG_MBX_BASE)
#endif
#define MBX_REG8(gid, addr)         RIU8[((gid+REG_MBX_BASE)<<2) + ((addr) * 2) - ((addr) & 1)]

//Reg8 defines:
#define REG8_MBX_CTRL               0x0000
    #define MBX_CTRL_FIRE             BIT(0)
    #define MBX_CTRL_READBACK         BIT(1)
    #define MBX_CTRL_INSTANT          BIT(2)
#define REG8_MBX_MAIL_CLASS          0x0001
#define REG8_MBX_MAIL_IDX            0x0002
#define REG8_MBX_PARAMETER_CNT       0x0003
#define REG8_MBX_PARAMETER_S         0x0004
#define REG8_MBX_PARAMETER_E         0x000D
#define REG8_MBX_STATE_0              0x000E
#define REG8_MBX_STATE_1              0x000F
    #define MBX_STATE1_DISABLED          BIT(4)
    #define MBX_STATE1_OVERFLOW           BIT(5)
    #define MBX_STATE1_ERROR              BIT(6)
    #define MBX_STATE1_BUSY               BIT(7)

#define REG_MBX_GROUP0      0x00UL
#define REG_MBX_GROUP1      0x08UL
#define REG_MBX_GROUP2      0x10UL
#define REG_MBX_GROUP3      0x18UL
#define REG_MBX_GROUP4      0x20UL
#define REG_MBX_GROUP5      0x28UL
#if 1//frcr2_integration###
#define REG_MBX_GROUP6      (0x00UL + REG_MBX_FRC_OFST)//(HKCPU to FRCR2)
#define REG_MBX_GROUP7      (0x08UL + REG_MBX_FRC_OFST)//(FRCR2 to HKCPU)
#endif

#define REG8_MBX_GROUP(gid, addr) MBX_REG8(gid, addr)

#define REG8_MBX_MIPS(addr)        MBX_REG8(REG_MBX_GROUP1, addr) //group0 allocated for mips
#define REG8_MBX_AEON(addr)        MBX_REG8(REG_MBX_GROUP0, addr) //group1 allocated for aeon
#define REG8_MBX_PM(addr)          MBX_REG8(REG_MBX_GROUP3, addr) //group3 allocated for pm

#endif //__MHAL_MBX_REG_H



