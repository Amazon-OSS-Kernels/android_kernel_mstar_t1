/**
* Copyright (c) 2006 ??2016 MStar Semiconductor, Inc.
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
@//-------------------------------------------------------------------------------------------------
@// Boot_Entry:
@// @param  None
@// @return None
@// @note   Entry point of Boot code
@//-------------------------------------------------------------------------------------------------
    .globl      Boot_Entry

Boot_Entry:
#if 0
    @//AC power don't do it
    RREG        r0, (MS_BUS_RIU_ADDR + (0x0E70 << 1))   @//Disable Divisor Latch Access
    bic         r0, r0, #0x00FF
    bic         r0, r0, #0x0F00
    ldr         r1, =0xF000
    cmp         r0, r1
    beq         Boot_Str_AcFlow

    // Set 0x39_32 bit 0 to 1
    RREG        r3, (MS_BUS_RIU_ADDR + (0x3964 << 1))
    ldr         r4, =0x00000001
    orr         r3, r3, r4
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x3964 << 1))
    // Set 0x1239_30 bit 2 to 1
    RREG        r3, (MS_BUS_RIU_ADDR + (0x123960 << 1))
    ldr         r4, =0x00000004
    orr         r3, r3, r4
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x123960 << 1))
    // Set dummy
    WREG_B        (MS_BUS_RIU_ADDR + (0x0600 << 1)), 0x55
    // Polling dummy

wait_pm_loop:
    RREG        r0, (MS_BUS_RIU_ADDR + (0x0600 << 1))       @//
    ldr         r1, =0xAA
    and         r0, r0, #0xff
    cmp         r0, r1
    bne         wait_pm_loop     @//

    //Disable backdoor
    // Set 0x39_32 bit 0 to 0
    RREG        r3, (MS_BUS_RIU_ADDR + (0x3964 << 1))
    bic        r3, r3, #0x00000001
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x3964 << 1))
    // Set 0x1239_30 bit 2 to 0
    RREG        r3, (MS_BUS_RIU_ADDR + (0x123960 << 1))
    bic        r3, r3, #0x00000004
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x123960 << 1))
Boot_Str_AcFlow:
#endif
aarch32_entry:
#if !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)

    @//Set bonding overwrite to ARM bootup without EJTAG
@// WREG        (MS_BUS_RIU_ADDR + (0x0e3e << 1)), 0x1f
#endif


Boot_Start:

    @//Initalize the PIU timer to measure boot time
    bl          BootRom_InitTimer

    RREG        r3, (MS_BUS_RIU_ADDR + (0x000e52 << 1))
#ifndef CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51
    bic         r3, r3, #0x00001000         @//Reset PM51
#endif
    orr         r3, r3, #0x0000003f         @//Set deglitch period for vdd2low_rst and chip_top_powergood
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x000e52 << 1))

    DBGREG      0x3697

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

    wfe

    @//Core 1 must jump to DRAM space to avoid SPI flash hang issue
    ldr         pc, =Bootram_SecondaryCore

Boot_SecondaryCoreLoop:
    b           Boot_SecondaryCoreLoop
