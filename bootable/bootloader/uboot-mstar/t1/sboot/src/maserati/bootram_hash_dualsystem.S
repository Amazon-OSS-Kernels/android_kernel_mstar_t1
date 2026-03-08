/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef _BOOT_INC_
#include "boot.inc"
#endif

@//-------------------------------------------------------------------------------------------------
@// BOOTRAM_Entry_Hash:
@// @param  None
@// @return None
@// @note   Entry point of Boot code
@//-------------------------------------------------------------------------------------------------
    .globl      BOOTRAM_Entry_Hash

BOOTRAM_Entry_Hash:

    DBGREG      0x0000

    @//Check bootflag
    bl          eMMC_CheckBootFlag

    cmp         r0, #1
    beq         BootRam_AuthenticationHash3_bk     @//bootflag != 0, verify hash3_bk


@//-------------------------------------------------------------------------------------------------
@// BootRam_AuthenticationHash3
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRam_AuthenticationHash3
BootRam_AuthenticationHash3:
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =_ld_bootram2_size
    ldr         r2, =0x1002
    bl          eMMC_ReadData_MIU

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE)
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR                                 @//Set the starting address of HASH for Authentication
    ldr         r1, =0x2000                                                  @//Set the HASH size for Authentication
    ldr         r2, =CONFIG_BOOTRAM_LOADADDR + 0x2000                        @//Set the starting address of SIG for Authentication
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain
#endif

    @//If authentication fail, jump to reset whole system

    cmp         r0, #1
    beq        Jump2Bootram  @//jump to hash3
    WREG_B      MS_BUS_RIU_ADDR + (0x103380 << 1), 0x5A      @//verify hash3 failed
    RREG        r0, (MS_BUS_RIU_ADDR + (0x103380 << 1))
    ldr         r1, =0x5A00
    and         r0, r0, #0xff00
    cmp         r0, r1
    beq         BootRam_AuthenticationFail     @//hash3 & hash3_bk verify failed


#if defined(CONFIG_MSTAR_AUTHEN_ENABLE)
@//-------------------------------------------------------------------------------------------------
@// BootRam_AuthenticationHash3_bk
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRam_AuthenticationHash3_bk
BootRam_AuthenticationHash3_bk:
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =_ld_bootram2_size
    ldr         r2, =0x8002
    bl          eMMC_ReadData_MIU

    ldr         r0, =CONFIG_BOOTRAM_LOADADDR                                 @//Set the starting address of HASH for Authentication
    ldr         r1, =0x2000                                                  @//Set the HASH size for Authentication
    ldr         r2, =CONFIG_BOOTRAM_LOADADDR + 0x2000                        @//Set the starting address of SIG for Authentication
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    cmp         r0, #1
    beq         Jump2Bootram  @//jump to hash3

    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x5A    @//verify hash3_bk failed

    RREG        r0, (MS_BUS_RIU_ADDR + (0x103380 << 1))        @//check if hash3 verify OK?
    ldr         r1, =0x5A
    and         r0, r0, #0xff
    cmp         r0, r1
    beq         BootRam_AuthenticationFail   @//hash3 & hash3_bk verify failed
    b           BootRam_AuthenticationHash3
#endif

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE)
Jump2Bootram:
    @//Jump to the entry of BOOTRAM_Entry
    ldr         pc, =BOOTRAM_Entry

BootRam_AuthenticationFail:
    b           BootRam_AuthenticationFail
#endif