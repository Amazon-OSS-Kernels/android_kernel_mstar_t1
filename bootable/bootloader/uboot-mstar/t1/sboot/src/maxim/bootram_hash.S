/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

    RREG        r0, (MS_BUS_RIU_ADDR + (0x103380 << 1))
    ldr         r1, =0x0100
    and         r0, r0, #0xff00
    cmp         r0, r1
    beq         BootRam_AuthenticationHash3_bk     @//reg[0x103381]#0=1b, MBOOT partition authen failed, try MBOOTBAK

BootRam_AuthenticationHash3:
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =_ld_bootram2_size
    ldr         r2, =0x1002                                      @//Read Hash3 at MBOOT Partition, addr=0x200400 (0x1002<<9)
    bl          eMMC_ReadData_MIU

    ldr         r0, =CONFIG_BOOTRAM_LOADADDR                     @//Set the starting address of HASH for Authentication
    ldr         r1, =(_ld_bootram2_size - 0x400)                                      @//Set the HASH size for Authentication
    ldr         r2, =CONFIG_BOOTRAM_LOADADDR + _ld_bootram2_size - 0x400            @//Set the starting address of SIG for Authentication
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    cmp         r0, #1
    beq         BootRam_Entry_Point
    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x01    @//Set Hash3 of MBOOT partition authen fail flag, try MBOOTBAK

BootRam_AuthenticationHash3_bk:
    PUTK 'B'
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =_ld_bootram2_size
    ldr         r2, =0x8002                                      @//Read Hash3_bak at MBOOTBAK Partition, addr=0x1000400 (0x8002<<9)
    bl          eMMC_ReadData_MIU

    ldr         r0, =CONFIG_BOOTRAM_LOADADDR                     @//Set the starting address of HASH for Authentication
    ldr         r1, =(_ld_bootram2_size - 0x400)                                      @//Set the HASH size for Authentication
    ldr         r2, =CONFIG_BOOTRAM_LOADADDR + _ld_bootram2_size - 0x400            @//Set the starting address of SIG for Authentication
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    cmp         r0, #1
    beq         BootRam_Entry_Point
    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x03    @//Set MBOOT & MBOOTBAK partition both authen fail flag
    bl          BootRam_AuthenticationFail                       @//HASH3 & HASH3_bak both authen failed

BootRam_Entry_Point:
    @//Jump to the entry of BOOTRAM_Entry
    ldr         pc, =BOOTRAM_Entry

BootRam_AuthenticationFail:
    PUTK        '\n'
    PUTK        '\r'
    DPUTK       'R'
    DPUTK       'A'
    DPUTK       'M'
    DPUTK       '1'
    DPUTK       '_'
    DPUTK       'A'
    DPUTK       'U'
    DPUTK       'T'
    DPUTK       'H'
    DPUTK       '_'
    DPUTK       'F'
    DPUTK       'A'
    DPUTK       'I'
    DPUTK       'L'
BootRam_AuthenticationFailLoop:
    b           BootRam_AuthenticationFailLoop
