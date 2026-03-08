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

    .global     BootRom_InitPeripheral
BootRom_InitPeripheral:

    mov         r13, lr

@//================================================================================================
    @//Initial related settings of UART
    bl          BootRom_InitUart

@//================================================================================================
    @//Enable 8051 to access MIU for MStar TV Tool
    bl          BootRom_EnableMcuAccess

@//================================================================================================
    @//Initial related settings of RIU
    bl          BootRom_InitRiu

#ifdef CONFIG_MSTAR_EFUSE_ENABLE
    @//Initial Trimming Setting in eFuse
    bl          BootRom_InitEfuse
#endif

#if !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)

    @//checkpoint for autotest boottime, plz dont remove it
    DPUTK       '['
    DPUTK       'A'
    DPUTK       'T'
    DPUTK       ']'
    DPUTK       '['
    DPUTK       'M'
    DPUTK       'B'
    DPUTK       ']'
    DPUTK       '['
    DPUTK       's'
    DPUTK       't'
    DPUTK       'a'
    DPUTK       'r'
    DPUTK       't'
    DPUTK       ' '
    DPUTK       's'
    DPUTK       'b'
    DPUTK       ']'
    DPUTK       '['
    DPUTK       '0'
    DPUTK       ']'
#endif



#ifdef REG_USB30_GPIO_CONTROL
    bl          BootRom_InitUsb30Gpio
#endif

    mov         lr, r13
    bx          lr

@//-------------------------------------------------------------------------------------------------
@// BootRom_PLLRecovery
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_PLLRecovery
BootRom_PLLRecovery:
    WREG        (MS_BUS_RIU_ADDR + (0x101E26 << 1)), 0x0200     @//set ejtag padmux
    WREG        (MS_BUS_RIU_ADDR + (0x101882 << 1)), 0x0404     @//Set L3 switch to xtali
    WREG        (MS_BUS_RIU_ADDR + (0x110C72 << 1)), 0x0101     @//MIU_256BUS_PLL power down
    WREG        (MS_BUS_RIU_ADDR + (0x110C62 << 1)), 0x0101     @//MIU_128BUS_PLL power down

    bx          lr
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitRiu
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitRiu
BootRom_InitRiu:

    @//Enable 32-bit RIU Write Data
    RREG        r3, (MS_BUS_RIU_ADDR + (0x101882 << 1))
    ldr         r4, =0x00008000
    orr         r3, r3, r4
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x101882 << 1))

#ifdef CONFIG_MSTAR_32BIT_RIU_MASK_ENABLE
    RREG        r3, (MS_BUS_RIU_ADDR + (0x10012E << 1))
    ldr         r4, =0x00000001
    orr         r3, r3, r4
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x10012E << 1))

    RREG        r3, (MS_BUS_RIU_ADDR + (0x101882 << 1))
    ldr         r4, =0x00008000
    orr         r3, r3, r4
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x101882 << 1))

    RREG        r3, (MS_BUS_RIU_ADDR + (0x00012E << 1))
    ldr         r4, =0x00000001
    orr         r3, r3, r4
    REG_C2M     r3, (MS_BUS_RIU_ADDR + (0x00012E << 1))
#endif

    bx          lr

@//-------------------------------------------------------------------------------------------------
@// BootRom_InitUart
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitUart
BootRom_InitUart:

#ifdef CONFIG_MSTAR_TITANIA_BD_FPGA
    WREG        (MS_BUS_RIU_ADDR + (0x000E12 << 1)), 0x0800
    WREG        (MS_BUS_RIU_ADDR + (0x101EA6 << 1)), 0x2104
    WREG        (MS_BUS_RIU_ADDR + (0x10098C << 1)), 0x0083
    WREG        (MS_BUS_RIU_ADDR + (0x100980 << 1)), 0x0013
    WREG        (MS_BUS_RIU_ADDR + (0x100984 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x100988 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x10098C << 1)), 0x0003
    WREG        (MS_BUS_RIU_ADDR + (0x100988 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x100988 << 1)), 0x0007
    WREG        (MS_BUS_RIU_ADDR + (0x003008 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x00300A << 1)), 0x0000

    DPUTK       'U'
    DPUTK       'A'
    DPUTK       'R'
    DPUTK       'T'
    DPUTK       '_'
    DPUTK       '3'
    DPUTK       '8'
    DPUTK       '4'
    DPUTK       '0'
    DPUTK       '0'

    DPUTK       '\n'
    DPUTK       '\r'

#else

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100b00 << 1))
    ldr         r0, =0x0c01
    str         r0, [r1, #(0x26 << 1)]                      @//Set CLK_UART0 to 123Mhz

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, =0x0000
    str         r0, [r1, #(0xa6 << 1)]
    ldr         r0, =0x0100
    str         r0, [r1, #(0xa8 << 1)]

.if (BOOTINFO_UART0_ENABLE == 1)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000e00 << 1))
    ldr         r0, [r1, #(0x12 << 1)]
    bic         r0, r0, #0x0800                             @//Disable UART Rx
    bic         r0, r0, #0x1000                             @//switch UART to ARM
    str         r0, [r1, #(0x12 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, [r1, #(0xa6 << 1)]
    orr         r0, r0, #0x0004
    str         r0, [r1, #(0xa6 << 1)]                      @//Select UART0 source to PIU UART0

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100900 << 1))
    ldr         r0, [r1, #(0x8c << 1)]
    orr         r0, r0, #0x0080
    str         r0, [r1, #(0x8c << 1)]                      @//Enable Divisor Latch Access

    ldr         r0, =UART_DIVISOR_L
    str         r0, [r1, #(0x80 << 1)]                      @//Set Divisor Latch LSB

    ldr         r0, =UART_DIVISOR_H
    str         r0, [r1, #(0x84 << 1)]                      @//Set Divisor Latch MSB

    ldr         r0, [r1, #(0x8c << 1)]
    bic         r0, r0, #0x0080
    str         r0, [r1, #(0x8c << 1)]                      @//Disable Divisor Latch Access

    ldr         r0, =0x0000
    str         r0, [r1, #(0x88 << 1)]
    ldr         r0, =0x0007
    str         r0, [r1, #(0x88 << 1)]                      @//Enable FIFO and Clear TX/RX FIFO

    ldr         r0, =0x0000
    str         r0, [r1, #(0x8c << 1)]
    ldr         r0, =0x0003
    str         r0, [r1, #(0x8c << 1)]                      @//8-bit Data / 1-bit Stop / No Parity Bit

    ldr         r0, =0x0000
    str         r0, [r1, #(0x90 << 1)]                      @//Set DTS/RTS to 1

    ldr         r0, =0x0000
    str         r0, [r1, #(0x84 << 1)]                      @//Disable Interrupt
.endif

.if (BOOTINFO_UART1_ENABLE == 1)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100b00 << 1))
    ldr         r0, =0x000c
    str         r0, [r1, #(0x28 << 1)]                      @//Set CLK_UART1 to 123Mhz

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, [r1, #(0xa6 << 1)]
    orr         r0, r0, #0x0050
    str         r0, [r1, #(0xa6 << 1)]                      @//Select UART1 source to PIU UART1

    ldr         r0, [r1, #(0x04 << 1)]
    orr         r0, r0, #0x0300
    str         r0, [r1, #(0x04 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110600 << 1))
    ldr         r0, [r1, #(0x0c << 1)]
    orr         r0, r0, #0x0080
    str         r0, [r1, #(0x0c << 1)]                      @//Enable Divisor Latch Access

    ldr         r0, =UART_DIVISOR_L
    str         r0, [r1, #(0x00 << 1)]                      @//Set Divisor Latch LSB

    ldr         r0, =UART_DIVISOR_H
    str         r0, [r1, #(0x04 << 1)]                      @//Set Divisor Latch MSB

    ldr         r0, [r1, #(0x0c << 1)]
    bic         r0, r0, #0x0080
    str         r0, [r1, #(0x0c << 1)]                      @//Disable Divisor Latch Access

    ldr         r0, =0x0000
    str         r0, [r1, #(0x08 << 1)]
    ldr         r0, =0x0007
    str         r0, [r1, #(0x08 << 1)]                      @//Enable FIFO and Clear TX/RX FIFO

    ldr         r0, =0x0000
    str         r0, [r1, #(0x0c << 1)]
    ldr         r0, =0x0003
    str         r0, [r1, #(0x0c << 1)]                      @//8-bit Data / 1-bit Stop / No Parity Bit

    ldr         r0, =0x0000
    str         r0, [r1, #(0x10 << 1)]                      @//Set DTS/RTS to 1

    ldr         r0, =0x0000
    str         r0, [r1, #(0x04 << 1)]                      @//Disable Interrupt
.endif

.if (BOOTINFO_UART2_ENABLE == 1)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, [r1, #(0xa6 << 1)]
    orr         r0, r0, #0x0500
    str         r0, [r1, #(0xa6 << 1)]                      @//Select UART2 source to PIU UART1
.endif

    #DPUTK       'U'
    #DPUTK       'A'
    #DPUTK       'R'
    #DPUTK       'T'
    #DPUTK       '_'
    #DPUTK       '1'
    #DPUTK       '1'
    #DPUTK       '5'
    #DPUTK       '2'
    #DPUTK       '0'
    #DPUTK       '0'

    #DPUTK       '\n'
    #DPUTK       '\r'
#ifdef CONFIG_FAST_UART_RTSCTS
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100b00 << 1))    @//Set CLK_FUART to 123Mhz
    ldr         r0, =0x000c
    str         r0, [r1, #(0x2a << 1)]

    RREG        r0, (MS_BUS_RIU_ADDR + (0x11068C << 1))     @//Enable Divisor Latch Access
    orr         r0, r0, #0x0080
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x11068C << 1))

    WREG        (MS_BUS_RIU_ADDR + (0x110680 << 1)), UART_DIVISOR_L   @//Set Divisor Latch LSB
    WREG        (MS_BUS_RIU_ADDR + (0x110684 << 1)), UART_DIVISOR_H   @//Set Divisor Latch MSB

    RREG        r0, (MS_BUS_RIU_ADDR + (0x11068C << 1))     @//Disable Divisor Latch Access
    ldr         r1, =0xFF7F
    and         r0, r0, r1
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x11068C << 1))

    WREG        (MS_BUS_RIU_ADDR + (0x110688 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x110688 << 1)), 0x0007 @//Enable FIFO and Clear TX/RX FIFO

    WREG        (MS_BUS_RIU_ADDR + (0x11068C << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x11068C << 1)), 0x0003 @//8-bit Data / 1-bit Stop / No Parity Bit
    WREG        (MS_BUS_RIU_ADDR + (0x110690 << 1)), 0x0000 @//Set DTS/RTS to 1
    WREG        (MS_BUS_RIU_ADDR + (0x110684 << 1)), 0x0000 @//Disable Interrupt
#endif

#endif

    bx          lr

@//-------------------------------------------------------------------------------------------------
@// BootRom_EnableMcuAccess
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_EnableMcuAccess
BootRom_EnableMcuAccess:

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002b00 << 1))
    ldr         r0, =0x0004
    str         r0, [r1, #(0xc4 << 1)]
    ldr         r0, =0x3c11
    str         r0, [r1, #(0xc6 << 1)]
    ldr         r0, =0x0010
    str         r0, [r1, #(0xc8 << 1)]
    ldr         r0, =0x403c
    str         r0, [r1, #(0xca << 1)]
    ldr         r0, =0x0101
    str         r0, [r1, #(0xcc << 1)]

    bx          lr

#ifdef REG_USB30_GPIO_CONTROL
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitUsb30Gpio
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
BootRom_InitUsb30Gpio:

		 WREG		 (MS_BUS_RIU_ADDR + (REG_USB30_GPIO_CONTROL << 1)), REG_USB30_GPIO_BANK @//offset addr
		 RREG		 r0, (MS_BUS_RIU_ADDR + ((REG_USB30_GPIO_CONTROL + 2) << 1))
		 ldr		 r1, =0x1FFF							 @//[bit 13:15] other usage
		 bic		 r0, r0, r1
		 add		 r0, r0, #(REG_USB30_GPIO_BIT << 8) @//bit8~bit10 bit count
		 add		 r0, r0, #(REG_USB30_GPIO_OFFSET) @//bit0~bit7 offset addr
		 add		 r0, r0, #(REG_USB30_GPIO_LOW_ACTIVE<<11) @// Turn on 5V  ( 0:	GPIO High	1: GPIO Low)
		 REG_C2M	 r0, (MS_BUS_RIU_ADDR + ((REG_USB30_GPIO_CONTROL + 2) << 1))

#ifdef ENABLE_USB30_GPIO_PORT2
		 WREG		 (MS_BUS_RIU_ADDR + (REG_USB30_PORT2_GPIO_CONTROL << 1)), REG_USB30_PORT2_GPIO_BANK @//offset addr
		 RREG		 r0, (MS_BUS_RIU_ADDR + ((REG_USB30_PORT2_GPIO_CONTROL + 2) << 1))
		 ldr		 r1, =0x1FFF							 @//[bit 13:15] other usage
		 bic		 r0, r0, r1
		 add		 r0, r0, #(REG_USB30_PORT2_GPIO_BIT << 8) @//bit8~bit10 bit count
		 add		 r0, r0, #(REG_USB30_PORT2_GPIO_OFFSET) @//bit0~bit7 offset addr
		 add		 r0, r0, #(REG_USB30_PORT2_GPIO_LOW_ACTIVE<<11) @// Turn on 5V	( 0:  GPIO High   1: GPIO Low)
		 REG_C2M	 r0, (MS_BUS_RIU_ADDR + ((REG_USB30_PORT2_GPIO_CONTROL + 2) << 1))
#endif

		 bx 		 lr

#endif

#ifdef CONFIG_MSTAR_EDINBURGH
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitExtDemod_Edinburgh
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitExtDemod_Edinburgh
BootRom_InitExtDemod_Edinburgh:

    bx          lr

#endif

#ifdef CONFIG_MSTAR_CASHMERE
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitExtDemod_Cashmere
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitExtDemod_Cashmere
BootRom_InitExtDemod_Cashmere:

    bx          lr
#endif

#ifdef CONFIG_MSTAR_VID_ENABLE
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitVid
@// @param  None
@// @return None
@// @note None
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitVid
BootRom_InitVid:


@//============================================================================
    @//Set Specific Label of VID
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, =0x2330
    str         r0, [r1, #(0x00 << 1)]  //cluster 0
    str         r0, [r1, #(0x08 << 1)]  //cluster 1

    @//Set Initial Value of CPU Clock Register
    ldr         r0, =CONFIG_MSTAR_DVFS_INIT_CPU_CLOCK_LIT
    str         r0, [r1, #(0x02 << 1)]  //cluster 0
    ldr         r0, =CONFIG_MSTAR_DVFS_INIT_CPU_CLOCK_BIG
    str         r0, [r1, #(0x0a << 1)]  //cluster 1

	@//Set Initial Value of Running State
    ldr         r0, =0x0
    str         r0, [r1, #(0x04 << 1)]  //cluster 0
    str         r0, [r1, #(0x0c << 1)]  //cluster 1

    @//Set Initial Value of CPU Temperature Register
    ldr         r0, [r1, #(0x04 << 1)]
    orr         r0, #(0xFF << 0x08)
    str         r0, [r1, #(0x04 << 1)]  //cluster 0

    ldr         r0, [r1, #(0x0c << 1)]
    orr         r0, #(0xFF << 0x08)
    str         r0, [r1, #(0x0c << 1)]  //cluster 1

    @//Set Initial Value of Special Clock Register
    ldr         r0, =0x2454
    str         r0, [r1, #(0x06 << 1)]  //cluster 0
    str         r0, [r1, #(0x0e << 1)]  //cluster 1

@//============================================================================
    @//Get SIDD_CPU + SIDD_CORE Value -> r2: bank6d[21:12]
    @//Get OSC_BIG Value              -> r3: bank6d[11:2]
    @//Get SIDD Value                 -> r4: bank6b[9:0]
    @//Get OSC_LIG Value              -> r5: bank6b[19:10]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x01b4
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x21b4
    str         r0, [r1, #(0x50 << 1)]

BootRom_InitVid_ReadEfuse_0:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_InitVid_ReadEfuse_0

    ldr         r6, [r1, #(0x58 << 1)]
    ldr         r7, [r1, #(0x5a << 1)]
    mov         r7, r7, lsl #16
    orr         r0, r6, r7

    ldr         r9, =0x3ff

    // r2: SIDD_CPU + SIDD_CORE
    lsr         r2, r0, #12
    and         r2, r2, r9

    // r3: OSC_BIG
    lsr         r3, r0, #2
    and         r3, r3, r9

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x01ac
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x21ac
    str         r0, [r1, #(0x50 << 1)]

BootRom_InitVid_ReadEfuse_1:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_InitVid_ReadEfuse_1

    ldr         r6, [r1, #(0x58 << 1)]
    ldr         r7, [r1, #(0x5a << 1)]
    mov         r7, r7, lsl #16
    orr         r0, r6, r7

    // r4: SIDD
    and         r4, r0, r9

    // r5: OSC_LIG
    lsr         r5, r0, #10
    and         r5, r5, r9

    // r6: [HPC/HPC+, ENG6/ENG4]
    lsr         r6, r0, #30
    and         r6, r6, #3

    cmp         r4, #0
    bne         BootRom_InitVid_HCP_PLUS_ENG6_Package

BootRom_InitVid_Normal_Package:
    @//Set Chip Package
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x02 << 1)]
    bic         r0, r0, #(0x07 << 12)
    str         r0, [r1, #(0x02 << 1)]
    str         r0, [r1, #(0x0a << 1)]

    b           BootRom_InitVid_Package_Exit

BootRom_InitVid_HCP_PLUS_ENG6_Package:
    cmp         r6, #3
    bne         BootRom_InitVid_HCP_ENG6_Package

    @//Set Chip Package
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x02 << 1)]
    bic         r0, r0, #(0x07 << 12)
    orr         r0, r0, #(0x01 << 12)
    str         r0, [r1, #(0x02 << 1)]  //cluster 0
    str         r0, [r1, #(0x0a << 1)]  //cluster 0

    b           BootRom_InitVid_Package_Exit

BootRom_InitVid_HCP_ENG6_Package:

    cmp         r6, #1
    bne         BootRom_InitVid_ENG4_Package

    @//Set Chip Package
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x02 << 1)]
    bic         r0, r0, #(0x07 << 12)
    orr         r0, r0, #(0x02 << 12)
    str         r0, [r1, #(0x02 << 1)]  //cluster 0
    str         r0, [r1, #(0x0a << 1)]  //cluster 0

    b           BootRom_InitVid_Package_Exit

BootRom_InitVid_ENG4_Package:

    @//Set Chip Package
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x02 << 1)]
    bic         r0, r0, #(0x07 << 12)
    orr         r0, r0, #(0x03 << 12)
    str         r0, [r1, #(0x02 << 1)]  //cluster 0
    str         r0, [r1, #(0x0a << 1)]  //cluster 0

BootRom_InitVid_Package_Exit:

#if defined(CONFIG_MSTAR_DVFS_RAPTOR)
    @//Set Chip Package
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x02 << 1)]
    bic         r0, r0, #(0x03 << 12)
    orr         r0, r0, #(0x04 << 12)
    str         r0, [r1, #(0x02 << 1)]  //cluster 0
    str         r0, [r1, #(0x0a << 1)]  //cluster 0
#endif

#if defined(CONFIG_MSTAR_DVFS_AMAZON)
    //ldr         r1, =(MS_BUS_RIU_ADDR + (0x003800 << 1))
    //ldr         r0, [r1, #(0x06 << 1)]
    //ldr         r6, =0x41
    //cmp         r0, r6
    //bne         BootRom_InitVid_Package_Exit

    @//Set Chip Package
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x02 << 1)]
    bic         r0, r0, #(0x03 << 12)
    orr         r0, r0, #(0x05 << 12)
    str         r0, [r1, #(0x02 << 1)]  //cluster 0
    str         r0, [r1, #(0x0a << 1)]  //cluster 0
#endif



    cmp         r2, #0
    beq         BootRom_InitVid_Exit                    @//If no SIDD existed in eFuse, bypass VID flow

    cmp         r3, #0
    beq         BootRom_InitVid_Exit                    @//If no SIDD existed in eFuse, bypass VID flow

    cmp         r4, #0
    beq         BootRom_InitVid_Exit                    @//If no SIDD existed in eFuse, bypass VID flow

    cmp         r5, #0
    beq         BootRom_InitVid_Exit                    @//If no SIDD existed in eFuse, bypass VID flow


@//============================================================================
    ldr         r6, =CONFIG_OSC_THRESHOLD_CPU_BIG
    cmp         r3, r6
    bls         BootRom_InitVid_SetCpuVoltage_SS_BIG       @//If OSC <= The Value of Threshold 0, Set Corresponding Voltage 0

    ldr         r6, =CONFIG_SIDD_THRESHOLD_CPU
    cmp         r4, r6
    bls         BootRom_InitVid_SetCpuVoltage_SS_BIG       @//If SIDD <= The Value of Threshold 0, Set Corresponding Voltage 0

    @//Set CPU Power Type for FF Corner Chip
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x0c << 1)]
    bic         r0, r0, #(0x03 << 3)
    orr         r0, r0, #(0x2 << 3)
    str         r0, [r1, #(0x0c << 1)]

    b           BootRom_InitVid_SetCpuVoltage_Exit_BIG

BootRom_InitVid_SetCpuVoltage_SS_BIG:
    @//Set CPU Power Type for SS Corner Chip
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x0c << 1)]
    bic         r0, r0, #(0x03 << 3)
    str         r0, [r1, #(0x0c << 1)]

BootRom_InitVid_SetCpuVoltage_Exit_BIG:

@//============================================================================
    ldr         r6, =CONFIG_OSC_THRESHOLD_CORE_BIG
    cmp         r3, r6
    bls         BootRom_InitVid_SetCoreVoltage_SS       @//If OSC <= The Value of Threshold 0, Set Corresponding Voltage 0

    ldr         r6, =CONFIG_SIDD_THRESHOLD_CORE
    cmp         r2, r6
    bls         BootRom_InitVid_SetCoreVoltage_SS       @//If SIDD <= The Value of Threshold 0, Set Corresponding Voltage 0

    @//Set core Power Type for FF Corner Chip
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x0c << 1)]
    bic         r0, r0, #(0x03 << 5)
    orr         r0, r0, #(0x2 << 5)
    str         r0, [r1, #(0x0c << 1)]

    ldr         r0, [r1, #(0x04 << 1)]
    bic         r0, r0, #(0x03 << 5)
    orr         r0, r0, #(0x2 << 5)
    str         r0, [r1, #(0x04 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002e00 << 1))
    ldr         r0, =0x0001
    str         r0, [r1, #(0x84 << 1)]

    b           BootRom_InitVid_SetCoreVoltage_Exit

BootRom_InitVid_SetCoreVoltage_SS:
    @//Set core Power Type for SS Corner Chip
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x0c << 1)]
    bic         r0, r0, #(0x03 << 5)
    str         r0, [r1, #(0x0c << 1)]

    ldr         r0, [r1, #(0x04 << 1)]
    bic         r0, r0, #(0x03 << 5)
    str         r0, [r1, #(0x04 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002e00 << 1))
    ldr         r0, =0x0100
    str         r0, [r1, #(0x84 << 1)]

BootRom_InitVid_SetCoreVoltage_Exit:

@//============================================================================
    ldr         r6, =CONFIG_OSC_THRESHOLD_CPU_LIT
    cmp         r5, r6
    bls         BootRom_InitVid_SetCpuVoltage_SS_LIT       @//If OSC <= The Value of Threshold 0, Set Corresponding Voltage 0

    ldr         r6, =CONFIG_SIDD_THRESHOLD_CPU
    cmp         r4, r6
    bls         BootRom_InitVid_SetCpuVoltage_SS_LIT       @//If SIDD <= The Value of Threshold 0, Set Corresponding Voltage 0

    @//Set CPU Power Type for FF Corner Chip
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x04 << 1)]
    bic         r0, r0, #(0x03 << 3)
    orr         r0, r0, #(0x2 << 3)
    str         r0, [r1, #(0x04 << 1)]
    b           BootRom_InitVid_SetCpuVoltage_Exit_LIT

BootRom_InitVid_SetCpuVoltage_SS_LIT:
    @//Set CPU Power Type for SS Corner Chip
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, [r1, #(0x04 << 1)]
    bic         r0, r0, #(0x03 << 3)
    str         r0, [r1, #(0x04 << 1)]

BootRom_InitVid_SetCpuVoltage_Exit_LIT:


    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000e00 << 1))
    ldr         r0, [r1, #(0x38 << 1)]
    ldr         r2, =(0x01 << 10)
    orr         r0, r0, r2
    str         r0, [r1, #(0x38 << 1)]

BootRom_InitVid_Exit:

    bx          lr
#endif

#ifdef CONFIG_MSTAR_DVFS_ENABLE
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitDvfs
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitDvfs
BootRom_InitDvfs:

@//============================================================================
    @//Set Specific Label of DVFS
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, =0x3697
    str         r0, [r1, #(0x00 << 1)]  //cluster 0
    str         r0, [r1, #(0x08 << 1)]  //cluster 1


@//============================================================================
    @//Set Initial Value of CPU Clock
    // Cluster 0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110c00 << 1))
    ldr         r0, =(CONFIG_DVFS_CPU_CLOCK_MIN_LIT & 0xFFFF)
    str         r0, [r1, #(0xa0 << 1)]
    ldr         r0, =((CONFIG_DVFS_CPU_CLOCK_MIN_LIT >> 16) & 0xFFFF)
    str         r0, [r1, #(0xa2 << 1)]

    ldr         r0, =(CONFIG_DVFS_CPU_CLOCK_INIT_LIT & 0xFFFF)
    str         r0, [r1, #(0xa4 << 1)]
    ldr         r0, =((CONFIG_DVFS_CPU_CLOCK_INIT_LIT >> 16) & 0xFFFF)
    str         r0, [r1, #(0xa6 << 1)]

    // Cluster 1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x111600 << 1))
    ldr         r0, =(CONFIG_DVFS_CPU_CLOCK_MIN_BIG & 0xFFFF)
    str         r0, [r1, #(0xa0 << 1)]
    ldr         r0, =((CONFIG_DVFS_CPU_CLOCK_MIN_BIG >> 16) & 0xFFFF)
    str         r0, [r1, #(0xa2 << 1)]

    ldr         r0, =(CONFIG_DVFS_CPU_CLOCK_INIT_BIG & 0xFFFF)
    str         r0, [r1, #(0xa4 << 1)]
    ldr         r0, =((CONFIG_DVFS_CPU_CLOCK_INIT_BIG >> 16) & 0xFFFF)
    str         r0, [r1, #(0xa6 << 1)]

@//============================================================================
    @//Enable Temperature Sensor
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000e00 << 1))
    ldr         r0, [r1, #(0x5e << 1)]
    orr         r0, r0, #0x04
    str         r0, [r1, #(0x5e << 1)]

    @//Switch CPU Temperature Sesnsor
    ldr         r0, [r1, #(0xc8 << 1)]
    ldr         r2, =(0x01 << 10)
    orr         r0, r0, r2
    str         r0, [r1, #(0xc8 << 1)]

@//============================================================================

BootRom_InitDvfs_Exit:

    bx          lr
#endif

#ifdef CONFIG_MSTAR_MOBF_ENABLE
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitMobf
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitMobf
BootRom_InitMobf:

    bx          lr
#endif

@//-------------------------------------------------------------------------------------------------
@// BootRom_InitTimer
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitTimer
BootRom_InitTimer:

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

#ifdef CONFIG_MSTAR_SBOOT_WDT_ENABLE
    @//Set WDT period maximum value to 0 to disable watchdog timer
    WREG        (MS_BUS_RIU_ADDR + (0x300a << 1)), ((CONFIG_SBOOT_WDT_VALUE >> 16) & 0xFFFF)
    WREG        (MS_BUS_RIU_ADDR + (0x3008 << 1)), (CONFIG_SBOOT_WDT_VALUE & 0xFFFF)
    WREG        (MS_BUS_RIU_ADDR + (0x3000 << 1)), 0x0001   @//Re-start WDT
#else
    @//Set WDT period maximum value to 0 to disable watchdog timer
    WREG        (MS_BUS_RIU_ADDR + (0x3008 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x300a << 1)), 0x0000
#endif

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x003000 << 1))
    ldr         r0, =0xffff
    str         r0, [r1, #(0x24 << 1)]
    ldr         r0, =0xffff
    str         r0, [r1, #(0x26 << 1)]
    ldr         r0, =0xffff
    str         r0, [r1, #(0x44 << 1)]
    ldr         r0, =0xffff
    str         r0, [r1, #(0x46 << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0x20 << 1)]
    ldr         r0, =0x0001
    str         r0, [r1, #(0x20 << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0x40 << 1)]
    ldr         r0, =0x0001
    str         r0, [r1, #(0x40 << 1)]

    bx          lr

#ifdef CONFIG_MSTAR_EFUSE_ENABLE
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitEfuse
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitEfuse
BootRom_InitEfuse:

    mov         r12, lr

    @//Set eFuse Redundancy bit
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0002
    str         r0, [r1, #(0x00 << 1)]
    str         r0, [r1, #(0x26 << 1)]

#ifdef CONFIG_MSTAR_EFUSE_VIDEO_ATOP
    @//Initial Video ATOP Trimming Control Setting
    bl          BootRom_InitVideoAtopTrimmingControl
#endif

#ifdef CONFIG_MSTAR_EFUSE_MHL_COMBO_ATOP
    @//Initial MHL Combo ATOP Trimming Control Setting
    bl          BootRom_InitMhlComboAtopTrimmingControl
#endif

#ifdef CONFIG_MSTAR_EFUSE_MHL_ECBUS_ATOP
    @//Initial MH3L E-/C-Bus ATOP Trimming Control Setting
    bl          BootRom_InitMhlEcbusAtopTrimmingControl
#endif

#ifdef CONFIG_MSTAR_EFUSE_MOD_ATOP
    @//Initial MOD Trimming Control Setting
    bl          BootRom_InitModAtopTrimmingControl
#endif

#ifdef CONFIG_MSTAR_EFUSE_USB_XCVR
    @//Initial USB XCVR Trimming Control Setting
    bl          BootRom_InitUsbXcvrTrimmingControl
#endif

#ifdef CONFIG_MSTAR_EFUSE_USB3_XCVR
    @//Initial USB XCVR Trimming Control Setting
    bl          BootRom_InitUsb3XcvrTrimmingControl
#endif

#ifdef CONFIG_MSTAR_EFUSE_PM_SAR_ATOP
    @//Initial PM SAR ATOP Trimming Control Setting
    bl          BootRom_InitPmSarAtopTrimmingControl
#endif


#ifdef CONFIG_MSTAR_EFUSE_ETH_ATOP
    @//Initial ETHERNET ATOP Trimming Control Setting
    bl          BootRom_InitEthAtopTrimmingControl
#endif

#ifdef CONFIG_MSTAR_EFUSE_CBUS
    @//Initial CBUS Trimming Control Setting
    bl          BootRom_InitCbusTrimmingControl
#endif

    mov         lr, r12
    bx          lr
#endif

#ifdef CONFIG_MSTAR_EFUSE_VIDEO_ATOP
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitVideoAtopTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitVideoAtopTrimmingControl
BootRom_InitVideoAtopTrimmingControl:

    @//Get Trim Value of Video ATOP in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0130
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x2130
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_VideoAtopLoop:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_VideoAtopLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//Activateefuse
    and         r2, r0, #(0x1 << 7)
    cmp         r2, #0
    beq         BootRom_InitVideoAtopTrimmingControlExit

    @//BGAP_vol
    and         r4, r0, #(0x1F << 0)
    lsl         r4, r4, #8
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x102500 << 1))
    ldr         r2, [r1, #(0xB6 << 1)]
    ldr         r3, =(0x1F << 8)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xB6 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x102500 << 1))
    ldr         r2, [r1, #(0xB6 << 1)]
    ldr         r3, =(0x01 << 13)
    orr         r2, r2, r3
    str         r2, [r1, #(0xB6 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103D00 << 1))
    ldr         r2, [r1, #(0x20 << 1)]
    ldr         r3, =(0x01 << 10)
    orr         r2, r2, r3
    str         r2, [r1, #(0x20 << 1)]

#ifdef CONFIG_MSTAR_EFUSE_VIDEO_ATOP_FULL_CURRENT

    @//IDAC_cur_full
    @//Activateefuse
    and         r2, r0, #(0x1 << 15)
    cmp         r2, #0
    beq         BootRom_InitVideoAtopTrimmingControlExit

    and         r4, r0, #(0x7F << 8)
    lsr         r4, r4, #8
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103D00 << 1))
    ldr         r2, [r1, #(0x10 << 1)]
    ldr         r3, =(0x0F << 12)
    bic         r2, r2, r3

    ldr         r3, =(0x7F << 0)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x10 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103D00 << 1))
    ldr         r2, [r1, #(0x20 << 1)]
    ldr         r3, =(0x01 << 10)
    bic         r2, r2, r3
    str         r2, [r1, #(0x20 << 1)]
#endif

#ifdef CONFIG_MSTAR_EFUSE_VIDEO_ATOP_ONE_HALF_CURRENT

    @//IDAC_cur_half
    @//Activateefuse
    and         r2, r0, #(0x1 << 23)
    cmp         r2, #0
    beq         BootRom_InitVideoAtopTrimmingControlExit

    and         r4, r0, #(0x7F << 16)
    lsr         r4, r4, #16
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103D00 << 1))
    ldr         r2, [r1, #(0x10 << 1)]
    ldr         r3, =(0x0F << 12)
    bic         r2, r2, r3
    ldr         r3, =(0x02 << 12)
    orr         r2, r2, r3

    ldr         r3, =(0x7F << 0)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x10 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103D00 << 1))
    ldr         r2, [r1, #(0x20 << 1)]
    ldr         r3, =(0x01 << 10)
    bic         r2, r2, r3
    str         r2, [r1, #(0x20 << 1)]
#endif

#ifdef CONFIG_MSTAR_EFUSE_VIDEO_ATOP_ONE_QUARTER_CURRENT

    @//IDAC_cur_quar
    @//Activateefuse
    and         r2, r0, #(0x1 << 31)
    cmp         r2, #0
    beq         BootRom_InitVideoAtopTrimmingControlExit

    and         r4, r0, #(0x7F << 24)
    lsr         r4, r4, #24
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103D00 << 1))
    ldr         r2, [r1, #(0x10 << 1)]
    ldr         r3, =(0x0F << 12)
    bic         r2, r2, r3
    ldr         r3, =(0x02 << 12)
    orr         r2, r2, r3

    ldr         r3, =(0x7F << 0)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x10 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103D00 << 1))
    ldr         r2, [r1, #(0x20 << 1)]
    ldr         r3, =(0x01 << 10)
    bic         r2, r2, r3
    str         r2, [r1, #(0x20 << 1)]
#endif

BootRom_InitVideoAtopTrimmingControlExit:

    bx          lr
#endif

#ifdef CONFIG_MSTAR_EFUSE_MHL_COMBO_ATOP
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitMhlComboAtopTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitMhlComboAtopTrimmingControl
BootRom_InitMhlComboAtopTrimmingControl:

    @//Get Trim Value of MHL Combo ATOP in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0154
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x2154
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_MhlComboAtopLoop:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_MhlComboAtopLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//MHL_rterm_v
    @//Activateefuse
    and         r2, r0, #(0x1 << 12)
    cmp         r2, #0
    beq         BootRom_InitMhlComboAtopTrimmingControlExit

    and         r4, r0, #(0x0F << 8)
    lsr         r4, r4, #8
    lsl         r4, r4, #12

    @//Port 0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x170600 << 1))
    ldr         r2, [r1, #(0xB2 << 1)]
    ldr         r3, =(0x01 << 8)
    orr         r2, r2, r3
    str         r2, [r1, #(0xB2 << 1)]

    ldr         r2, [r1, #(0xD6 << 1)]
    ldr         r3, =(0x0F << 12)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xD6 << 1)]

    @//Port 1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x170800 << 1))
    ldr         r2, [r1, #(0xB2 << 1)]
    ldr         r3, =(0x01 << 8)
    orr         r2, r2, r3
    str         r2, [r1, #(0xB2 << 1)]

    ldr         r2, [r1, #(0xD6 << 1)]
    ldr         r3, =(0x0F << 12)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xD6 << 1)]


    @//MHL_rterm_h
    @//Activateefuse
    //and         r2, r0, #(0x1 << 17)
    //cmp         r2, #0
    //beq         BootRom_InitMhlComboAtopTrimmingControlExit

    //and         r4, r0, #(0x0F << 13)
    //lsr         r4, r4, #13
    //lsl         r4, r4, #12

    @//Port 0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x170200 << 1))
    ldr         r2, [r1, #(0xB2 << 1)]
    ldr         r3, =(0x01 << 8)
    orr         r2, r2, r3
    str         r2, [r1, #(0xB2 << 1)]

    ldr         r2, [r1, #(0xD6 << 1)]
    ldr         r3, =(0x0F << 12)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xD6 << 1)]

    @//Port 1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x170400 << 1))
    ldr         r2, [r1, #(0xB2 << 1)]
    ldr         r3, =(0x01 << 8)
    orr         r2, r2, r3
    str         r2, [r1, #(0xB2 << 1)]

    ldr         r2, [r1, #(0xD6 << 1)]
    ldr         r3, =(0x0F << 12)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xD6 << 1)]

BootRom_InitMhlComboAtopTrimmingControlExit:

    bx          lr
#endif

#ifdef CONFIG_MSTAR_EFUSE_MHL_ECBUS_ATOP
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitMhlEcbusAtopTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitMhlEcbusAtopTrimmingControl
BootRom_InitMhlEcbusAtopTrimmingControl:

    @//Get Trim Value of MHL E-/C-Bus ATOP in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0154
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x2154
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_MhlEcbusAtopLoop:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_MhlEcbusAtopLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//ECBUS_cur
    and         r4, r0, #(0xF << 0)
    lsl         r4, r4, #8
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x173C00 << 1))
    ldr         r2, [r1, #(0xB0 << 1)]
    ldr         r3, =(0xF << 8)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xB0 << 1)]

    @//ECBUS_rterm
    and         r4, r0, #(0xF << 4)
    lsr         r4, r4, #4
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x173C00 << 1))
    ldr         r2, [r1, #(0xAA << 1)]
    ldr         r3, =(0xF << 0)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xAA << 1)]

    bx          lr
#endif

#ifdef CONFIG_MSTAR_EFUSE_MOD_ATOP
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitModAtopTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitModAtopTrimmingControl
BootRom_InitModAtopTrimmingControl:

    @//Get Trim Value of MOD ATOP in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0138
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x2138
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_ModAtopLoop:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_ModAtopLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//MOD_swing
    @//Activateefuse
    and         r2, r0, #(0x1 << 6)
    cmp         r2, #0
    beq         BootRom_InitModAtopTrimmingControlExit

    and         r2, r0, #(0x3F << 0)
    lsl         r1, r2, #8
    orr         r4, r2, r1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x111E00 << 1))
    ldr         r2, [r1, #(0x80 << 1)]
    ldr         r3, =0x3F3F
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x80 << 1)]  @//CH0/CH1

    ldr         r2, [r1, #(0x82 << 1)]
    ldr         r3, =0x3F3F
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x82 << 1)]  @//CH2/CH3 ICON

    ldr         r2, [r1, #(0x84 << 1)]
    ldr         r3, =0x3F3F
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x84 << 1)]  @//CH4/CH5 ICON

    ldr         r2, [r1, #(0x86 << 1)]
    ldr         r3, =0x3F3F
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x86 << 1)]  @//CH6/CH7 ICON

    ldr         r2, [r1, #(0x88 << 1)]
    ldr         r3, =0x3F3F
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x88 << 1)]  @//CH8/CH9 ICON

    ldr         r2, [r1, #(0x8A << 1)]
    ldr         r3, =0x3F3F
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x8A << 1)]  @//CH10/CH11 ICON

    ldr         r2, [r1, #(0x8C << 1)]
    ldr         r3, =0x3F3F
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x8C << 1)]  @//CH12/CH13 ICON

    ldr         r2, [r1, #(0x8E << 1)]
    ldr         r3, =0x3F3F
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x8E << 1)]  @//CH12/CH13 ICON

BootRom_InitModAtopTrimmingControlExit:

    bx          lr
#endif

#ifdef CONFIG_MSTAR_EFUSE_USB_XCVR
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitUsbXcvrTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitUsbXcvrTrimmingControl
BootRom_InitUsbXcvrTrimmingControl:

    @//Get Trim Value of USB XCVR in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x013C
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x213C
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_UsbXcvrLoop:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_UsbXcvrLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//Activateefuse
    and         r2, r0, #(0x1 << 4)
    cmp         r2, #0
    beq         BootRom_InitUsbXcvrTrimmingControlExit

    @//Port0
    and         r4, r0, #(0x0F << 0)
    lsl         r4, r4, #5
    ldr         r3, =(0x0F << 5)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103A00 << 1))
    ldr         r2, [r1, #(0xA8 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xA8 << 1)]

    @//Port1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103A00 << 1))
    ldr         r2, [r1, #(0x28 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x28 << 1)]

    @//Port2
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103900 << 1))
    ldr         r2, [r1, #(0x28 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x28 << 1)]

    @//PortSS0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x122300 << 1))
    ldr         r2, [r1, #(0x28 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x28 << 1)]

    @//PortSS1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x122300 << 1))
    ldr         r2, [r1, #(0xA8 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xA8 << 1)]

BootRom_InitUsbXcvrTrimmingControlExit:

    bx          lr
#endif


#ifdef CONFIG_MSTAR_EFUSE_USB3_XCVR
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitUsb3XcvrTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitUsb3XcvrTrimmingControl
BootRom_InitUsb3XcvrTrimmingControl:

    @//Get Trim Value of USB XCVR in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0140
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x2140
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_Usb3XcvrLoop:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_Usb3XcvrLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//Port 0
    @//Activateefuse
    and         r2, r0, #(0x1 << 10)
    cmp         r2, #0
    beq         BootRom_InitUsb3XcvrTrimmingControlExit

    @//RX_RCTRL
    and         r4, r0, #(0x1F << 0)
    ldr         r3, =(0x1F << 0)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x122100 << 1))
    ldr         r2, [r1, #(0x08 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x08 << 1)]

    @//TX_RT
    and         r4, r0, #(0x1F << 5)
    lsr         r4, r4, #5
    ldr         r2, [r1, #(0x10 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x10 << 1)]


    @//Port 1
    @//Activateefuse
    and         r2, r0, #(0x1 << 21)
    cmp         r2, #0
    beq         BootRom_InitUsb3XcvrTrimmingControlExit

    @//RX_RCTRL
    and         r4, r0, #(0x1F << 11)
    lsr         r4, r4, #11
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x153500 << 1))
    ldr         r2, [r1, #(0x08 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x08 << 1)]

    @//TX_RT
    and         r4, r0, #(0x1F << 16)
    lsr         r4, r4, #16
    ldr         r2, [r1, #(0x10 << 1)]
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x10 << 1)]

BootRom_InitUsb3XcvrTrimmingControlExit:

    bx          lr
#endif


#ifdef CONFIG_MSTAR_EFUSE_PM_SAR_ATOP
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitPmSarAtopTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitPmSarAtopTrimmingControl
BootRom_InitPmSarAtopTrimmingControl:

    @//Get PM SAR Trim Value of T_Sensor in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0144
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x2144
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_PmSarAtopLoop:
    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_PmSarAtopLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//Activateefuse
    and         r4, r0, #(0x1 << 20)
    cmp         r4, #0
    beq         BootRom_InitPmSarAtopTrimmingControlExit

    @//BGAP_vol
    and         r4, r0, #(0x3F << 0)
    lsl         r4, r4, #1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000E00 << 1))
    ldr         r2, [r1, #(0xc8 << 1)]
    ldr         r3, =(0x003F << 1)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0xc8 << 1)]

BootRom_InitPmSarAtopTrimmingControlExit:

    bx          lr
#endif

#ifdef CONFIG_MSTAR_EFUSE_ETH_ATOP
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitEthAtopTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitEthAtopTrimmingControl
BootRom_InitEthAtopTrimmingControl:

    @//Get Trim Value of ETH_ATOP in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0134
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x2134
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_EthAtopLoop:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_EthAtopLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//Activateefuse
    and         r2, r0, #(0x1 << 18)
    cmp         r2, #0
    beq         BootRom_InitEthAtopTrimmingControlExit

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x003400 << 1))
    ldr         r2, [r1, #(0x60 << 1)]
    ldr         r3, =(0x01 << 2)
    orr         r2, r2, r3
    str         r2, [r1, #(0x60 << 1)]

    ldr         r2, [r1, #(0x68<< 1)]
    ldr         r3, =(0x01 << 15)
    orr         r2, r2, r3
    str         r2, [r1, #(0x68 << 1)]

    @//ETH_cur_100t
    and         r4, r0, #(0x1F << 0)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x003400 << 1))
    ldr         r2, [r1, #(0x68 << 1)]
    ldr         r3, =(0x1F << 0)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x68 << 1)]

    @//ETH_cur_10t
    and         r4, r0, #(0x1F << 5)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x003400 << 1))
    ldr         r2, [r1, #(0x68 << 1)]
    ldr         r3, =(0x1F << 5)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x68 << 1)]

    @//ETH_imp
    and         r4, r0, #(0xF << 10)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x003400 << 1))
    ldr         r2, [r1, #(0x68 << 1)]
    ldr         r3, =(0xF << 10)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x68 << 1)]

    @//ETH_lpf
    and         r4, r0, #(0xF << 14)
    lsr         r4, r4, #14
    lsl         r4, r4, #7
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x003400 << 1))
    ldr         r2, [r1, #(0x60 << 1)]
    ldr         r3, =(0xF << 7)
    bic         r2, r2, r3
    orr         r2, r2, r4
    str         r2, [r1, #(0x60 << 1)]

BootRom_InitEthAtopTrimmingControlExit:

    bx          lr
#endif

#ifdef CONFIG_MSTAR_EFUSE_CBUS
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitCbusTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitCbusTrimmingControl
BootRom_InitCbusTrimmingControl:

    @//Get Trim Value of CBUS in eFuse
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    ldr         r0, =0x0138
    str         r0, [r1, #(0x50 << 1)]
    ldr         r0, =0x2138
    str         r0, [r1, #(0x50 << 1)]

BootRom_ReadEfuse_CbusLoop:

    ldr         r0, [r1, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_CbusLoop

    ldr         r2, [r1, #(0x58 << 1)]
    ldr         r3, [r1, #(0x5a << 1)]
    mov         r3, r3, lsl #16
    orr         r0, r2, r3

    @//Activateefuse
    and         r2, r0, #(0x01 << 20)
    cmp         r2, #0
    beq         BootRom_InitCbusTrimmingControlExit

    @//CBUS_rterm
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x2f00 << 1))
    ldr         r2, [r1, #(0x42 << 1)]
    ldr         r3, =(0xF << 12)
    bic         r2, r2, r3

    //PD_1K_HIGH
    and         r4, r0, #(0x1 << 16)
    lsr         r4, r4, #16
    lsl         r4, r4, #13
    orr         r2, r2, r4

    //PD_1K_LOW
    and         r4, r0, #(0x1 << 17)
    lsr         r4, r4, #17
    lsl         r4, r4, #12
    orr         r2, r2, r4

    //PD_100K_HIGH
    and         r4, r0, #(0x1 << 18)
    lsr         r4, r4, #18
    lsl         r4, r4, #15
    orr         r2, r2, r4

    //PD_100K_LOW
    and         r4, r0, #(0x1 << 19)
    lsr         r4, r4, #19
    lsl         r4, r4, #14
    orr         r2, r2, r4

    str         r2, [r1, #(0x42 << 1)]

BootRom_InitCbusTrimmingControlExit:

    bx          lr
#endif

