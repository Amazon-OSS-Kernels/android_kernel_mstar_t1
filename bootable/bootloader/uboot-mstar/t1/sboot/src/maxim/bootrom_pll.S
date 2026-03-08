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
    .global     BootRom_InitPll

BootRom_InitPll:

    //----------------------------------------------------------
    // switch FIQ / IRQ merge bit
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, [r1, #(0x64 << 1)]
    orr         r0, r0, #0x50
    strb        r0, [r1, #(0x64 << 1)]

    // Set 0x1239fc bit 3 to 1
    ldr         r0, [r1, #(0xfc << 1)]
    orr         r0, r0, #(0x1 << 3)
    strb        r0, [r1, #(0xfc << 1)]

    // Set 0x1239fc bit 2 to 1
    ldr         r0, [r1, #(0xfc << 1)]
    orr         r0, r0, #(0x1 << 2)
    strb        r0, [r1, #(0xfc << 1)]

    // Set 0x1239fc bit 1 to 1
    ldr         r0, [r1, #(0xfc << 1)]
    orr         r0, r0, #(0x1 << 1)
    strb        r0, [r1, #(0xfc << 1)]

    //----------------------------------------------------------
    // ADC
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x102500 << 1))
    ldr         r0, =0x3f                   @//Soft-reset for DAC/ATOP/PLLB/ADCB/PLLA/ADCA
    strb        r0, [r1, #(0x0e << 1)]
    ldr         r0, =0x00
    strb        r0, [r1, #(0x0e << 1)]

    ldr         r0, =0x0000                 @//Power-On LDO
    str         r0, [r1, #(0xfc << 1)]
    ldr         r0, =0x0000                 @//Power-On LDO
    str         r0, [r1, #(0xfe << 1)]

    ldr         r0, =0x0000                 @//Power-on for related adc_atop IPs
    str         r0, [r1, #(0x08 << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0x0a << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0x0c << 1)]
    ldr         r0, =0x0030                 @//Enable SOG MUX
    str         r0, [r1, #(0x7e << 1)]

    ldr         r0, =0x08                   @//Enable IDAC (Ch1)
    strb        r0, [r1, #(0x90 << 1)]
    ldr         r0, =0x08                   @//Enable IDAC (Ch1)
    strb        r0, [r1, #(0x92 << 1)]
    ldr         r0, =0x00                   @//Enable CVBSO1
    strb        r0, [r1, #(0xa0 << 1)]
    ldr         r0, =0x00                   @//Enable CVBSO2
    strb        r0, [r1, #(0xa4 << 1)]


    //----------------------------------------------------------
    // XTAL/MPLL/MIUPLL/EVDPLL
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    ldr         r0, =0x0000                 @//Power-on for PD_3P3_1, PD_3P3_0 and PD_3P3_USB1
    str         r0, [r1, #(0x38 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110c00 << 1))
    ldr         r0, =0x0000                 @//reg_mpll_pd = 0
                                            @//reg_mpll_clk_adc216m_pd = 0
                                            @//Module: ana_misc (0x110C)
                                            @//reg_mpll_clk_adc432m_pd = 0
    str         r0, [r1, #(0x02 << 1)]
    ldr         r0, =0x19                   //MIUPLL select 600MHz
    strb        r0, [r1, #(0x66 << 1)]
    ldr         r0, =0x11                   //Enable test clock out
    strb        r0, [r1, #(0x68 << 1)]
    ldr         r0, =0x00                   //MIU128BUSPLL power on
    strb        r0, [r1, #((0x62 << 1) + 1)]
//  ldr         r0, =0x19                   //MIUPLL select 600MHz
//  strb        r0, [r1, #(0x76 << 1)]
    ldr         r0, =0x11                   //Enable test clock out
    strb        r0, [r1, #(0x78 << 1)]
    ldr         r0, =0x00                   //MIU256BUSPLL power on
    strb        r0, [r1, #((0x72 << 1) + 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110b00 << 1))
    ldr         r0, =0x00                   //EVDPLL power on
    strb        r0, [r1, #((0x82 << 1) + 1)]
    ldr         r0, =0x16                   //EVDPLL select 532MHz
    strb        r0, [r1, #(0x86 << 1)]


    //----------------------------------------------------------
    // ARMPLL
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110c00 << 1))

#ifdef CONFIG_MSTAR_DVFS_ENABLE
    ldr         r0, =(CONFIG_DVFS_CPU_CLOCK_INIT & 0xFFFF)
    str         r0, [r1, #(0xc0 << 1)]
    ldr         r0, =((CONFIG_DVFS_CPU_CLOCK_INIT >> 16) & 0xFFFF)
    str         r0, [r1, #(0xc2 << 1)]
#else
    ldr         r0, =(CONFIG_CPU_CLOCK & 0xFFFF)
    str         r0, [r1, #(0xc0 << 1)]
    ldr         r0, =((CONFIG_CPU_CLOCK >> 16) & 0xFFFF)
    str         r0, [r1, #(0xc2 << 1)]
#endif

    ldr         r0, =0x0001
    str         r0, [r1, #(0xc4 << 1)]
    ldr         r0, =0x00                   //reg_mipspll_pd = 0
    strb        r0, [r1, #((0x22 << 1) + 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100500 << 1))
    ldr         r0, =CPU_CLOCK
    str         r0, [r1, #(0x1e << 1)]

    //----------------------------------------------------------
    // UPLL0
    //----------------------------------------------------------
    WREG        (MS_BUS_RIU_ADDR + (0x100800 << 1)), 0x00c0     //enable UPLL0 clock
    WREG_B      (MS_BUS_RIU_ADDR + (0x10080E << 1)), 0x01       //enable 384MHz clk from UPLL0
    WREG        (MS_BUS_RIU_ADDR + (0x100804 << 1)), 0x0110     //UPLL0

    //----------------------------------------------------------
    // UPLL1
    //----------------------------------------------------------
                                                                //initial UPLL1 & UTMISS0
    WREG        (MS_BUS_RIU_ADDR + (0x100820 << 1)), 0x00c0       //enable UPLL1 clock

    WREG        (MS_BUS_RIU_ADDR + (0x100824 << 1)), 0x0110     //UPLL1

    WREG        (MS_BUS_RIU_ADDR + (0x103A88 << 1)), 0x040F     // UTMI0
    WREG        (MS_BUS_RIU_ADDR + (0x103A80 << 1)), 0x0001
    WREG        (MS_BUS_RIU_ADDR + (0x103A08 << 1)), 0x040F	    // UTMI1
    WREG        (MS_BUS_RIU_ADDR + (0x103A00 << 1)), 0x0001
    WREG        (MS_BUS_RIU_ADDR + (0x103908 << 1)), 0x040F	    // UTMI2
    WREG        (MS_BUS_RIU_ADDR + (0x103900 << 1)), 0x0001
    WREG        (MS_BUS_RIU_ADDR + (0x122088 << 1)), 0x042F	    // UTMI3
    WREG        (MS_BUS_RIU_ADDR + (0x122080 << 1)), 0x0001
    WREG        (MS_BUS_RIU_ADDR + (0x122308 << 1)), 0x042F	    // UTMI-SS0
    WREG        (MS_BUS_RIU_ADDR + (0x122300 << 1)), 0x0001

    //----------------------------------------------------------
    // USB0 Module: usb0 (0x100700)
    //----------------------------------------------------------
    WREG_B      (MS_BUS_RIU_ADDR + (0x103A80 << 1)), 0xC2       //Turn dp/dm pull-down use override mode
    WREG_B      (MS_BUS_RIU_ADDR + (0x100700 << 1)), 0x0a       //Disable MAC initial suspend, Reset UHC
    WREG_B      (MS_BUS_RIU_ADDR + (0x100700 << 1)), 0x28       //Release UHC reset, enable UHC and OTG XIU function

                                                                //Module: utmi0 (0x103A80)
    WREG        (MS_BUS_RIU_ADDR + (0x103AA2 << 1)), 0x2088     //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    WREG        (MS_BUS_RIU_ADDR + (0x103AA0 << 1)), 0x8051     //PLL_TEST[15]: Bypass 480MHz clock divider
    WREG        (MS_BUS_RIU_ADDR + (0x103A82 << 1)), 0x2084     //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG        (MS_BUS_RIU_ADDR + (0x103A88 << 1)), 0x0426     //bit<7>: Power down UTMI port-0 bandgap current

    WREG        (MS_BUS_RIU_ADDR + (0x103A80 << 1)), 0x6bc3     //reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator

    DELAYUS     1000

    WREG        (MS_BUS_RIU_ADDR + (0x103A80 << 1)), 0x69c3     //Turn on UPLL, reg_pdn: bit<9>

    DELAYUS     2000

    RREG        r0, (MS_BUS_RIU_ADDR + (0x100702 << 1))         // UHC select enable (usb0: 0x100700)
    ldr         r1, =0x0003
    bic         r0, r0, r1
    orr         r0, r0, #0x0001
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x100702 << 1))

    RREG        r0, (MS_BUS_RIU_ADDR + (0x102440 << 1))         // 0: VBUS On. (uhc0: 0x102400)
    ldr         r1, =0x0010
    bic         r0, r0, r1
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x102440 << 1))

    WREG        (MS_BUS_RIU_ADDR + (0x103A80 << 1)), 0x0001     //Disable dp/dm pull-down override mode

    //----------------------------------------------------------
    // USB1 Module: usb1 (0x100780)
    //----------------------------------------------------------
    WREG_B      (MS_BUS_RIU_ADDR + (0x103A00 << 1)), 0xC2       //Turn dp/dm pull-down use override mode
    WREG_B      (MS_BUS_RIU_ADDR + (0x100780 << 1)), 0x0a       //Disable MAC initial suspend, Reset UHC
    WREG_B      (MS_BUS_RIU_ADDR + (0x100780 << 1)), 0x28       //Release UHC reset, enable UHC and OTG XIU function

                                                                //Module: utmi1 (0x103A00)
    WREG        (MS_BUS_RIU_ADDR + (0x103A22 << 1)), 0x2088     //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    WREG        (MS_BUS_RIU_ADDR + (0x103A20 << 1)), 0x8051     //PLL_TEST[15]: Bypass 480MHz clock divider
    WREG        (MS_BUS_RIU_ADDR + (0x103A02 << 1)), 0x2084     //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG        (MS_BUS_RIU_ADDR + (0x103A08 << 1)), 0x0426     //bit<7>: Power down UTMI port-0 bandgap current

    WREG        (MS_BUS_RIU_ADDR + (0x103A00 << 1)), 0x6bc3     //reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator

    DELAYUS     1000

    WREG        (MS_BUS_RIU_ADDR + (0x103A00 << 1)), 0x69c3     //Turn on UPLL, reg_pdn: bit<9>

    DELAYUS     2000

    RREG        r0, (MS_BUS_RIU_ADDR + (0x100782 << 1))         //UHC select enable (usb1: 0x100780)
    ldr         r1, =0x0003
    bic         r0, r0, r1
    orr         r0, r0, #0x0001
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x100782 << 1))

    RREG        r0, (MS_BUS_RIU_ADDR + (0x100D40 << 1))         //0: VBUS On. (uhc1: 0x100D00)
    ldr         r1, =0x0010
    bic         r0, r0, r1
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x100D40 << 1))

    WREG        (MS_BUS_RIU_ADDR + (0x103A00 << 1)), 0x0001     //Disable dp/dm pull-down override mode

    //----------------------------------------------------------
    // USB2 Module: usb2 (0x113800)
    //----------------------------------------------------------
    WREG_B      (MS_BUS_RIU_ADDR + (0x103900 << 1)), 0xC2       //Turn dp/dm pull-down use override mode
    WREG_B      (MS_BUS_RIU_ADDR + (0x113800 << 1)), 0x0a       //Disable MAC initial suspend, Reset UHC
    WREG_B      (MS_BUS_RIU_ADDR + (0x113800 << 1)), 0x28       //Release UHC reset, enable UHC and OTG XIU function

                                                                //Module: utmi2 (0x103900)
    WREG        (MS_BUS_RIU_ADDR + (0x103922 << 1)), 0x2088     //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    WREG        (MS_BUS_RIU_ADDR + (0x103920 << 1)), 0x8051     //PLL_TEST[15]: Bypass 480MHz clock divider
    WREG        (MS_BUS_RIU_ADDR + (0x103902 << 1)), 0x2084     //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG        (MS_BUS_RIU_ADDR + (0x103908 << 1)), 0x0426     //bit<7>: Power down UTMI port-0 bandgap current

    WREG        (MS_BUS_RIU_ADDR + (0x103900 << 1)), 0x6bc3     //reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator

    DELAYUS     1000

    WREG        (MS_BUS_RIU_ADDR + (0x103900 << 1)), 0x69c3     //Turn on UPLL, reg_pdn: bit<9>

    DELAYUS     2000

    RREG        r0, (MS_BUS_RIU_ADDR + (0x113802 << 1))         //UHC select enable (usb2: 0x113800)
    ldr         r1, =0x0003
    bic         r0, r0, r1
    orr         r0, r0, #0x0001
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x113802 << 1))

    RREG        r0, (MS_BUS_RIU_ADDR + (0x113940 << 1))         //0: VBUS On. (uhc2: 0x113900)
    ldr         r1, =0x0010
    bic         r0, r0, r1
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x113940 << 1))

    WREG      (MS_BUS_RIU_ADDR + (0x103900 << 1)), 0x0001       //Turn dp/dm pull-down use override mode

    //----------------------------------------------------------
    // USB3 Module: usb3 (0x113880)
    //----------------------------------------------------------
    WREG_B      (MS_BUS_RIU_ADDR + (0x122080 << 1)), 0xC2       //Turn dp/dm pull-down use override mode
    WREG_B      (MS_BUS_RIU_ADDR + (0x113880 << 1)), 0x0a       //Disable MAC initial suspend, Reset UHC
    WREG_B      (MS_BUS_RIU_ADDR + (0x113880 << 1)), 0x28       //Release UHC reset, enable UHC and OTG XIU function

                                                                //Module: utmi3 (0x122080)
    WREG        (MS_BUS_RIU_ADDR + (0x1220A2 << 1)), 0x2088     //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    WREG        (MS_BUS_RIU_ADDR + (0x1220A0 << 1)), 0x8051     //PLL_TEST[15]: Bypass 480MHz clock divider
    WREG        (MS_BUS_RIU_ADDR + (0x122082 << 1)), 0x2084     //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG        (MS_BUS_RIU_ADDR + (0x122088 << 1)), 0x0426     //bit<7>: Power down UTMI port-0 bandgap current

    WREG        (MS_BUS_RIU_ADDR + (0x122080 << 1)), 0x6bc3     //reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator

    DELAYUS     1000

    WREG        (MS_BUS_RIU_ADDR + (0x122080 << 1)), 0x69c3     //Turn on UPLL, reg_pdn: bit<9>

    DELAYUS     2000

    RREG        r0, (MS_BUS_RIU_ADDR + (0x113882 << 1))         //UHC select enable (usb2: 0x113880)
    ldr         r1, =0x0003
    bic         r0, r0, r1
    orr         r0, r0, #0x0001
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x113882 << 1))

    RREG        r0, (MS_BUS_RIU_ADDR + (0x122640 << 1))         //0: VBUS On. (uhc2: 0x122600)
    ldr         r1, =0x0010
    bic         r0, r0, r1
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x122640 << 1))

    WREG      (MS_BUS_RIU_ADDR + (0x122080 << 1)), 0x0001       //Turn dp/dm pull-down use override mode

    //----------------------------------------------------------
    // USB4 Module: usb4-ss0 (0x153200)
    //----------------------------------------------------------
    WREG_B      (MS_BUS_RIU_ADDR + (0x122300 << 1)), 0xC2       //Turn dp/dm pull-down use override mode
    WREG_B      (MS_BUS_RIU_ADDR + (0x153200 << 1)), 0x0a       //Disable MAC initial suspend, Reset UHC
    WREG_B      (MS_BUS_RIU_ADDR + (0x153200 << 1)), 0x28       //Release UHC reset, enable UHC and OTG XIU function

                                                                //Module: utmi4-ss0 (0x122300)
    WREG        (MS_BUS_RIU_ADDR + (0x122322 << 1)), 0x2088     //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    WREG        (MS_BUS_RIU_ADDR + (0x122320 << 1)), 0x8051     //PLL_TEST[15]: Bypass 480MHz clock divid
    WREG        (MS_BUS_RIU_ADDR + (0x122302 << 1)), 0x2084     //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG        (MS_BUS_RIU_ADDR + (0x122308 << 1)), 0x0426     //bit<7>: Power down UTMI port-0 bandgap current

    WREG        (MS_BUS_RIU_ADDR + (0x122300 << 1)), 0x6bc3     //reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator

    DELAYUS     1000

    WREG        (MS_BUS_RIU_ADDR + (0x122300 << 1)), 0x69c3     //Turn on UPLL, reg_pdn: bit<9>

    DELAYUS     2000

    RREG        r0, (MS_BUS_RIU_ADDR + (0x153202 << 1))         //UHC select enable (usb30hs_usbc: 0x153200)
    ldr         r1, =0x0003
    bic         r0, r0, r1
    orr         r0, r0, #0x0001
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x153202 << 1))

    RREG        r0, (MS_BUS_RIU_ADDR + (0x153040 << 1))         //0: VBUS On. (usb30hs_uhc: 0x153000)
    ldr         r1, =0x0010
    bic         r0, r0, r1
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x153040 << 1))

    WREG      (MS_BUS_RIU_ADDR + (0x122300 << 1)), 0x0001       //Turn dp/dm pull-down use override mode

    //----------------------------------------------------------
    // MPLL_LVDS
    //----------------------------------------------------------
    //ldr         r1, =(MS_BUS_RIU_ADDR + (0x103100 << 1))
    //ldr         r0, =0xb010
    //str         r0, [r1, #(0xd8 << 1)]


    bx          lr

    .size       BootRom_InitPll,.-BootRom_InitPll

    .end

