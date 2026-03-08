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
    .global     BootRom_InitClock

BootRom_InitClock:

    mov         r13, lr

    //----------------------------------------------------------
    // MIPSPLL
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101d00 << 1))
    ldr         r0, =0x01
    str         r0, [r1, #(0xf8 << 1)]                  //Enable CA53 select ARMPLL_0 PLL

    //----------------------------------------------------------
    // IP: clk_mcu, uart, spi
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x103300 << 1))
    ldr         r0, =0x00                               //Set CLK_R2_SECURE to 288MHz (Enabled)
    strb        r0, [r1, #(0x7a << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000e00 << 1))

    ldr         r0, =0x04                               //Set clock of clk_spi to 86MHz
    strb        r0, [r1, #((0x40 << 1) + 1)]
    ldr         r0, =0x44
    strb        r0, [r1, #((0x40 << 1) + 1)]
    ldr         r0, =0x8438                             //Set CLK_MCU_PM to 216MHz (Enabled)
    str         r0, [r1, #(0x40 << 1)]                  //Set CLK_SPI to 54MHz (Enabled)

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100b00 << 1))

    ldr         r0, =0x0001                             //Set CLK_MCU to 216MHz (Enabled)
    str         r0, [r1, #(0x20 << 1)]                  //Set MCU clock to CLK_MCU

    ldr         r0, =0x0C                               //Set CLK_UART0 to 123MHz (Enabled)
    strb        r0, [r1, #((0x26 << 1) + 1)]            //Set CLK_UART1 to 123MHz (Enabled)
    ldr         r0, =0x0C0C                             //Set CLK_UART2 to 123MHz (Enabled)
    str         r0, [r1, #(0x28 << 1)]                  //Set CLK_UART3 to 123MHz (Enabled)
    ldr         r0, =0x0C0C                             //Set CLK_UART4 to 123MHz (Enabled)
    str         r0, [r1, #(0x32 << 1)]

    ldr         r0, =0x04                               //Set CLK_SPI to 54MHz
    strb        r0, [r1, #(0x2c << 1)]
    ldr         r0, =0x44                               //Set CLK_SPI to 54MHz (Enabled)
    strb        r0, [r1, #(0x2c << 1)]
    ldr         r0, =0x04                               //Set CLK_SPI_M to 48MHz (Enabled)
    strb        r0, [r1, #(0x2e << 1)]
    ldr         r0, =0x24                               //Set CLK_SPI_M to 48MHz (Enabled)
    strb        r0, [r1, #(0x2e << 1)]

    //----------------------------------------------------------
    // DIG_MUX
    //----------------------------------------------------------
    ldr         r0, =0x00                               //Set CLK_TCK (Enabled)
    strb        r0, [r1, #(0x34 << 1)]

    //----------------------------------------------------------
    // IP: clk_miu
    //----------------------------------------------------------
    ldr         r0, =0x01                               //Set CLK_DDR_SYN to 432MHz (Enabled)
    strb        r0, [r1, #(0xfc << 1)]
    ldr         r0, =0x00                               //Set CLK_MIU_REC to XTAL div 8 (Enabled)
    strb        r0, [r1, #(0x3c << 1)]
    ldr         r0, =0x0808                             //switch clk_miu_256bus to MIU256PLL and clk_miu to MIU128PLL
    str         r0, [r1, #(0x3a << 1)]

    //----------------------------------------------------------
    // IP: clk_gop
    //----------------------------------------------------------
    ldr         r0, =0x0000                             //Set CLK_GOPG 0/1 to clk_odclk_p (Enabled)
    str         r0, [r1, #(0x80 << 1)]
    ldr         r0, =0x00                               //Set CLK_GOPG2 to clk_odclk_p (Enabled)
    strb        r0, [r1, #(0x82 << 1)]
    ldr         r0, =0x00                               //clk_gopg0_mg[3:0] clk_ggopg2_mg[7:4], selec their own clock
    strb        r0, [r1, #(0xfe << 1)]

    ldr         r0, =0x0000                             //Set CLK_PSRAM 0/1 (Enabled)
    str         r0, [r1, #(0x86 << 1)]
    ldr         r0, =0x4000                             // gop scaling lb clk: [15:12][11:8][7:4][3:0]
    str         r0, [r1, #(0x88 << 1)]
    ldr         r0, =0x0000                             // gop2 lb[7:4], gop1 lb[3:0], selec their own clock
    str         r0, [r1, #(0x8a << 1)]

    //----------------------------------------------------------
    // IP: clk_ge
    //----------------------------------------------------------
    ldr         r0, =0x00                               //Set CLK_GE to 128bit MIU clock (Enabled)
    strb        r0, [r1, #(0x90 << 1)]

    //----------------------------------------------------------
    // IP: clk_emac
    //----------------------------------------------------------
    ldr         r0, =0x0000                             //Set CLK_EMAC_AHB to 123MHz (Enabled), Set CLK_EMAC_RX to CLK_EMAC_RX_in (25MHz) (Enabled)
    str         r0, [r1, #(0xc0 << 1)]
    ldr         r0, =0x0000                             //Set CLK_EMAC_TX to CLK_EMAC_TX_IN (25MHz) (Enabled), Set CLK_EMAC_TX_REF to CLK_EMAC_TX_IN (50MHz) (Enabled)
    str         r0, [r1, #(0xc2 << 1)]
    ldr         r0, =0x00                               //Set CLK_EMAC_RX_REF to CLK_EMAC_RX_IN (50MHz) (Enabled)
    strb        r0, [r1, #(0xc4 << 1)]

    //----------------------------------------------------------
    // clk_sc
    //----------------------------------------------------------
    ldr         r0, =0x0800
    str         r0, [r1, #(0x9c << 1)]            //turn on clk_edclk
    ldr         r0, =0x0008
    str         r0, [r1, #(0xa6 << 1)]            //turn on clk_odclk
    ldr         r0, =0x0800
    str         r0, [r1, #(0xba << 1)]            //turn on clk_fmclk
    ldr         r0, =0x0000
    str         r0, [r1, #(0xa2 << 1)]
    ldr         r0, =0x3c00
    str         r0, [r1, #(0xa4 << 1)]            //turn on clk_fclk
    ldr         r0, =0x20
    strb        r0, [r1, #((0xa8 << 1) + 1)]            // idclk_f1 selection
    ldr         r0, =0x0020
    str         r0, [r1, #(0xaa << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0xae << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0xb0 << 1)]
    ldr         r0, =0x0020
    str         r0, [r1, #(0xb2 << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0xbe << 1)]

    //----------------------------------------------------------
    // CLK RTC
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x0e00 << 1))
    ldr         r0, =0x08
    strb        r0, [r1, #(0x44 << 1)]

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA) && \
    !defined(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    bl          BootRom_InitSpiFlashClock
#endif

#if defined(CONFIG_MSTAR_GPU_MANUAL)
    bl          BootRom_SetGPUClock
#endif

    @//RTC
    WREG_B      (MS_BUS_RIU_ADDR + (0x0E44 << 1)), 0x04

    mov         lr, r13
    bx          lr

#if defined(CONFIG_MSTAR_GPU_MANUAL)
@//-------------------------------------------------------------------------------------------------
@// BootRom_SetGPUClock
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
BootRom_SetGPUClock:
    @//Set GPU Clock
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110800 << 1))
    ldr         r0, =CONFIG_GPU_CLOCK
    ldr         r0, [r1, #(0x8c << 1)]
    ldr         r0, [r1, #(0xd2 << 1)]
    orr         r0, r0, #0x01
    str         r0, [r1, #(0xd2 << 1)]

    bx          lr
#endif

#if !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
/*
 *#############################################################################
 * BootRom_InitSpiFlashClock SPI Clock init
 * @param  None
 * @return None
 * @note   do SPI Clock init
 *#############################################################################
 */
BootRom_InitSpiFlashClock:
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000a00 << 1))
    ldr         r0, =0x0fff                 @//Set CSZ deselect time/CSZ setup time/CSZ hold time to 16 SPI clock cycles
    str         r0, [r1, #(0xe2 << 1)]
    ldr         r0, =0x01                   @//Enable fast read mode
    strb        r0, [r1, #(0xe4 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x000e00 << 1))
    ldr         r0, =0x04                   @//Set clock of clk_spi to 86MHz
    strb        r0, [r1, #((0x40 << 1) + 1)]
    ldr         r0, =0x44                   @//Switch clk_spi between clk_mcu_p and clk_ext_xtali_buf
    strb        r0, [r1, #((0x40 << 1) + 1)]
    ldr         r0, =0x80                   @//Set clock of MCU to 170MHz
    strb        r0, [r1, #(0x40 << 1)]

    bx          lr
 #endif

    .end

