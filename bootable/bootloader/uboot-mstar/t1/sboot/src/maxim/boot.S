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

    .text
    .globl      _vector

_vector:

    .word 0xd503201f  // nop
    .word 0xd503201f  // nop
    .word 0x5800089a  // ldr  x26, 0x110
    .word 0xd51ec01a  // msr  vbar_el3, x26

    @//init cpu register
    .word 0xd53e1007  // mrs  x7, sctlr_el3
    .word 0xd2820008  // mov  x8, #0x1000
    .word 0xb27e0108  // orr  x8, x8, #0x4
    .word 0xb2400108  // orr  x8, x8, #0x1
    .word 0xb2750108  // orr  x8, x8, #0x800
    .word 0x8a2800e7  // bic  x7, x7, x8
    .word 0xb27b00e7  // orr  x7, x7, #0x20
    .word 0xb27d00e7  // orr  x7, x7, #0x8
    .word 0xd51e1007  // msr  sctlr_el3, x7
    .word 0xd539f227  // mrs  x7, s3_1_c15_c2_1
    .word 0xb27a00e7  // orr  x7, x7, #0x40
    .word 0xd519f227  // msr  s3_1_c15_c2_1, x7

    @//set 0x123964=0x1051 to switch FIQ/IRQ merge bit
    .word 0x5800065a  // ldr  x26, 0xc8
    .word 0x18000679  // ldr  w25, 0xcc
    .word 0x39000359  // strb w25, [x26]
    .word 0xd503201f  // nop

    @//set 0x101e27=0x02 to enable ejtag
    .word 0x5800051a  // ldr  x26, 0xa0
    .word 0x18000539  // ldr  w25, 0xa4
    .word 0x39000359  // strb w25, [x26]

    @//switch from aarch64 to aarch32
    .word 0x5800053e  // ldr  x30, 0xa4
    .word 0xd503201f  // nop
    .word 0xd51cc01f  // msr  vbar_el2, xzr
    .word 0xd51c101f  // msr  sctlr_el2, xzr
    .word 0xd518101f  // msr  sctlr_el1, xzr
    .word 0xd51e115f  // msr  cptr_el3, xzr
    .word 0xd51c115f  // msr  cptr_el2, xzr
    .word 0xd518105f  // msr  cpacr_el1, xzr
    .word 0xd51c11ff  // msr  hacr_el2, xzr
    .word 0xd51c117f  // msr  hstr_el2, xzr
    .word 0xd51ce07f  // msr  cntvoff_el2, xzr
    .word 0xd53800a0  // mrs  x0, mpidr_el1
    .word 0xd51c00a0  // msr  vmpidr_el2, x0
    .word 0xd5380000  // mrs  x0, midr_el1
    .word 0xd51c0000  // msr  vpidr_el2, x0
    .word 0xd53e1021  // mrs  x1, actlr_el3
    .word 0xb2400021  // orr  x1, x1, #0x1
    .word 0xb27f0021  // orr  x1, x1, #0x2
    .word 0xb27c0021  // orr  x1, x1, #0x10
    .word 0xb27b0021  // orr  x1, x1, #0x20
    .word 0xb27a0021  // orr  x1, x1, #0x40
    .word 0xd51e1021  // msr  actlr_el3, x1
    .word 0xd51c111f  // msr  hcr_el2, xzr
    .word 0xd51e403e  // msr  elr_el3, x30
    .word 0xd51c101f  // msr  sctlr_el2, xzr
    .word 0xd2807920  // msr  elr_el3, x30
    .word 0xd51e4000  // msr  sctlr_el2, xzr
    .word 0xd51e403e  // msr  elr_el3, x30
    .word 0xd53e4001  // mrs  x1, spsr_el3
    .word 0x928043e2  // mov  x2, #0xfffffffffffffde0
    .word 0x8a020021  // and  x1, x1, x2
    .word 0xb2400421  // orr  x1, x1, #0x3
    .word 0xb27c0021  // orr  x1, x1, #0x10
    .word 0x9276f821  // and  x1, x1, #0xfffffffffffffdff
    .word 0xd51e4001  // msr  spsr_el3, x1
    .word 0xd69f03e0  // eret

    @//data section
    .word 0x00000000  // dada=0x00000000
    .word 0x1f203c4d  // dada=0x1f203c4d
    .word 0x00000000  // data=0x00000000
    .word 0x00000002  // dada=0x00000002
    .word 0x00000000  // dada=0x00000000
    .word Boot_Entry
    .word 0x00000000  // data=0x00000000
    .word 0x1f2472c8  // data=0x00000000
    .word 0x00000000  // data=0x00000000
    .word 0x00001051  // data=0x00000000
    .word 0x00000000  // data=0x00000000
    .word CONFIG_BOOTRAM_LOADADDR // data=CONFIG_BOOTRAM_LOADADDR
    .word 0x00000000  // data=0x00000000

@//-------------------------------------------------------------------------------------------------
@// Boot_Entry:
@// @param  None
@// @return None
@// @note   Entry point of Boot code
@//-------------------------------------------------------------------------------------------------
    .globl      Boot_Entry

Boot_Entry:

#if !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)

    @//Set bonding overwrite to ARM bootup without EJTAG
@// WREG        (MS_BUS_RIU_ADDR + (0x0e3e << 1)), 0x1f
#endif

#ifdef CONFIG_MSTAR_SBOOT_WDT_ENABLE

    RREG        r0, (MS_BUS_RIU_ADDR + (0x0606 << 1))  @// check flag 0x0603 bit0
    ldr         r1, =0x0001
    and         r0, r0, r1
    cmp         r0, #0x0001
    beq         Boot_Start

    @//Set WDT period maximum value to 0 to disable watchdog timer
    WREG        (MS_BUS_RIU_ADDR + (0x300a << 1)), ((CONFIG_SBOOT_WDT_VALUE >> 16) & 0xFFFF)
    WREG        (MS_BUS_RIU_ADDR + (0x3008 << 1)), (CONFIG_SBOOT_WDT_VALUE & 0xFFFF)
    WREG        (MS_BUS_RIU_ADDR + (0x3000 << 1)), 0x0001   @//Re-start WDT

    WREG        (MS_BUS_RIU_ADDR + (0x0606 << 1)), 0x0001
    WREG        (MS_BUS_RIU_ADDR + (0x0e24 << 1)), 0xbabe

    @//Set GPIO to OUT
    ldr         r0, =(MS_BUS_RIU_ADDR + (0x0f08 << 1))
    ldr         r1, =0xfffc
    and         r2, r0, r1
    str         r2, [r0]

    @//Set GPIO to LOW
    ldr         r0, =(MS_BUS_RIU_ADDR + (0x0f08 << 1))
    ldr         r1, =0xfffd
    and         r2, r0, r1
    str         r2, [r0]

#else
    @//Set WDT period maximum value to 0 to disable watchdog timer
    WREG        (MS_BUS_RIU_ADDR + (0x3008 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x300a << 1)), 0x0000
#endif

Boot_Start:

    @//Initalize the PIU timer to measure boot time
    bl          BootRom_InitTimer

    RREG        r3, (MS_BUS_RIU_ADDR + (0x000e52 << 1))
    orr         r3, r3, #0x0000003f         @//Set deglitch period for vdd2low_rst and chip_top_powergood
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x000e52 << 1))

    DBGREG      0x3697

    bl          Boot_InitCpuRegister        @//Initialize CPU Registers

    DBGREG      0x0000

    mrc         p15, 0, r0, c0, c0, 5
    and         r0, r0, #0x03               @//Single Core
    cmp         r0, #0
    beq         Boot_PrimaryCore

    bl          Boot_SecondaryCore

Boot_PrimaryCore:

    ldr         pc, =BootRom_Entry          @//Jump to Entry Point of BootRom

Boot_SecondaryCore:
                                            @//Enable software interrupt
    @//Set Priority Mask Register in CPU Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x04), 0xf0

    @//Set Control Register in CPU Interface to Enable Software Interrupt
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x00), 0x01

                                            @//SECOND_START_ADDR_HI = 0x1f206740
                                            @//SECOND_START_ADDR_LO = 0x1f206744
                                            @//set LSB[1:0] of LO = 2'b11, then wait soft interrupt to wake up
    WREG        (MS_BUS_RIU_ADDR + (0x1033A2 << 1)), 0xff

    wfi

    @//Core 1 must jump to DRAM space to avoid SPI flash hang issue
    ldr         pc, =Bootram_SecondaryCore

Boot_SecondaryCoreLoop:
    b           Boot_SecondaryCoreLoop

@//-------------------------------------------------------------------------------------------------
@// Boot_InitCpuRegister
@// @param  None
@// @return None
@// @note   ARM internal register init setting
@//-------------------------------------------------------------------------------------------------
Boot_InitCpuRegister:

    mrc         p15, 0, r0, c1, c0, 0
    bic         r0, r0, #0x01               @//Disable MMU
    bic         r0, r0, #0x04               @//Disable D-Cache
    bic         r0, r0, #0x1000             @//Disable I-Cache
    mcr         p15, 0, r0, c1, c0, 0

    bx          lr
