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

#ifndef _BOARD_H_
#include "Board.h"
#endif

#ifndef _DRV_MIU_INIT_H_
#include "drv_miu_init_str.h"
#endif

#ifndef _MIUSEL_MAXIM_2MIU_H_
#include "MIUSEL_MAXIM_2MIU.h"
#endif

    .text

#ifdef CONFIG_MSTAR_SELF_STR_TEST_ENABLE
    MIU_MESSAGE_BIST0_Str:      .short   'B', 'I', 'S', 'T', '0', '-', 0x00
    MIU_MESSAGE_BIST1_Str:      .short   'B', 'I', 'S', 'T', '1', '-', 0x00
    MIU_MESSAGE_BIST2_Str:      .short   'B', 'I', 'S', 'T', '2', '-', 0x00

    MIU_MESSAGE_OK_Str:         .short   'O', 'K', '\n', '\r', 0x00
    MIU_MESSAGE_FAIL_Str:       .short   'F', 'A', 'I', 'L', '\n', '\r', 0x00
#endif

    MESSAGE_DC_ON_STR:          .short  '\r', '\n', 'S', 'T', 'R', '_', 'O', 'N', '\r', '\n', 0x00

    .align      4
    .global     BootRom_InitMiu_STR
BootRom_InitMiu_STR:

    mov         r13, lr


    DPUTK_STR   MESSAGE_DC_ON_STR

    DBGREG      0x2001

#ifdef CONFIG_MSTAR_TITANIA_BD_FPGA

    WREG        (MS_BUS_RIU_ADDR + (0x003008 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x101ea0 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x002bc6 << 1)), 0x3c11
    WREG        (MS_BUS_RIU_ADDR + (0x002bca << 1)), 0x403c
    WREG        (MS_BUS_RIU_ADDR + (0x002bc4 << 1)), 0x0004
    WREG        (MS_BUS_RIU_ADDR + (0x101202 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x101202 << 1)), 0x0818
    WREG        (MS_BUS_RIU_ADDR + (0x101204 << 1)), 0x0185
    WREG        (MS_BUS_RIU_ADDR + (0x101206 << 1)), 0x0120
    WREG        (MS_BUS_RIU_ADDR + (0x101208 << 1)), 0x0833
    WREG        (MS_BUS_RIU_ADDR + (0x10120a << 1)), 0x0c33
    WREG        (MS_BUS_RIU_ADDR + (0x10120c << 1)), 0x7111
    WREG        (MS_BUS_RIU_ADDR + (0x10120e << 1)), 0x100e
    WREG        (MS_BUS_RIU_ADDR + (0x101210 << 1)), 0x0031
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x0c01
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x0c00
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x0008
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x000c
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x001f
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x0c08
    WREG        (MS_BUS_RIU_ADDR + (0x101228 << 1)), 0x4000

    WREG        (MS_BUS_RIU_ADDR + (0x10123a << 1)), 0x0001
    WREG        (MS_BUS_RIU_ADDR + (0x10123c << 1)), 0x03ff

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))

_CheckDramData:
    ldr         r0, [r1, #(0x00 << 1)]
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         _CheckDramData

#else

    bl          MIU_Secure_flow_str

    ldr         r0, =MIU_PreInit_Str
    bl          MDrv_WriteRegTbl32

    @//  AVD reset
    //bl          Reset_AVD_str

#if !defined(CONFIG_MIU0_DRAM_NONE)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0001
    str         r0, [r1, #(0x78 << 1)]
    ldr         r0, =0x1000
    str         r0, [r1, #(0x24 << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0x24 << 1)]

    DELAYUS     100

#if defined(CONFIG_MIU0_DDR3_1666)
    #error "Not support DDR3-1600"

#elif defined(CONFIG_MIU0_DDR3_1866)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x34e6
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x001d
    str         r0, [r1, #(0x32 << 1)]
    ldr         r0, =0x0001
    str         r0, [r1, #(0x8a << 1)]

#elif defined(CONFIG_MIU0_DDR3_2133)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0xefab
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x0019
    str         r0, [r1, #(0x32 << 1)]
    ldr         r0, =0x0124
    str         r0, [r1, #(0x8a << 1)]

#elif defined(CONFIG_MIU0_DDR4_2400)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x5a37
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x0017
    str         r0, [r1, #(0x32 << 1)]
    ldr         r0, =0x0003
    str         r0, [r1, #(0x8a << 1)]

#else
    #error "Invalid DRAM Setting"
#endif

    DELAYUS     100

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0004
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0x0114
    str         r0, [r1, #(0x2c << 1)]

#ifdef CONFIG_ONEBIN_ENABLE
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =MIUBuffer
    ldr         r2, =MIU0
    bl          drv_LoadTbl
    cmp         r0, #1
    beq         SET_BUFFER_ADDRESS_MIU0
    @DPUTK_STR    MIU0_LoadTbl_FAIL

DEAD_END_MIU0:
    b           DEAD_END_MIU0

SET_BUFFER_ADDRESS_MIU0:
    ldr         r0, =MIUBuffer

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    ldr         r1, =ONEBIN_SIG_OFF_1024                  @//Set the HASH size for Authentication
    ldr         r2, =(MIUBuffer + ONEBIN_SIG_OFF_1024) @//Set the starting address of SIG for Authentication
    ldr         r3, =0x1
    bl          MDrv_AESDMA_SecureMain

@//If authentication fail, jump to reset whole system
    cmp         r0, #1
    @bne         BootRom_miu_AuthenticationFail
    ldr         r0, =MIUBuffer
#endif
#else
    ldr         r0, =MIU0_DDR_Init_Str
#endif
    bl          MDrv_WriteRegTbl32

#endif /* CONFIG_MIU0_DRAM_NONE */

#if !defined(CONFIG_MIU1_DRAM_NONE)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0001
    str         r0, [r1, #(0x78 << 1)]
    ldr         r0, =0x1000
    str         r0, [r1, #(0x24 << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0x24 << 1)]

    DELAYUS     100

#if defined(CONFIG_MIU1_DDR3_1666)
    #error "Not support DDR3-1600"

#elif defined(CONFIG_MIU1_DDR3_1866)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0xa229
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x001d
    str         r0, [r1, #(0x32 << 1)]
    ldr         r0, =0x0001
    str         r0, [r1, #(0x8a << 1)]

#elif defined(CONFIG_MIU1_DDR3_2133)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0xefab
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x0019
    str         r0, [r1, #(0x32 << 1)]
    ldr         r0, =0x0124
    str         r0, [r1, #(0x8a << 1)]

#elif defined(CONFIG_MIU1_DDR4_2400)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x5a37
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x0017
    str         r0, [r1, #(0x32 << 1)]
    ldr         r0, =0x0003
    str         r0, [r1, #(0x8a << 1)]

#else
    #error "Invalid DRAM Setting"

#endif

    DELAYUS     100

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0004
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0x0114
    str         r0, [r1, #(0x2c << 1)]

#ifdef CONFIG_ONEBIN_ENABLE
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =MIUBuffer
    ldr         r2, =MIU1
    bl          drv_LoadTbl
    cmp         r0, #1
    beq         SET_BUFFER_ADDRESS_MIU1
    @//DPUTK_STR    MIU1_LoadTbl_FAIL

DEAD_END_MIU1:
    b           DEAD_END_MIU1

SET_BUFFER_ADDRESS_MIU1:
    ldr         r0, =MIUBuffer

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    ldr         r1, =ONEBIN_SIG_OFF_1024                  @//Set the HASH size for Authentication
    ldr         r2, =(MIUBuffer + ONEBIN_SIG_OFF_1024) @//Set the starting address of SIG for Authentication
    ldr         r3, =0x1
    bl          MDrv_AESDMA_SecureMain

@//If authentication fail, jump to reset whole system
    cmp         r0, #1
    @bne         BootRom_miu_AuthenticationFail
    ldr         r0, =MIUBuffer
#endif
#else
    ldr         r0, =MIU1_DDR_Init_Str
#endif
    bl          MDrv_WriteRegTbl32

#endif /* CONFIG_MIU1_DRAM_NONE */

    bl          Release_Sw_Reset

    DELAYUS     500                        @//delay 500us

#if !defined(CONFIG_MIU0_DRAM_NONE)
    @//MIU0 INIT Cycle
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0x0000                 @//hold miu_0
    str         r0, [r1, #(0x00 << 1)]
    ldr         r0, =0x0008                 @//DRAM reset
    str         r0, [r1, #(0x00 << 1)]
    ldr         r0, =0x000c                 @//DRAM reset+CS
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     1000                        @//delay 1ms

    ldr         r0, =0x000e                 @//DRAM reset+CS+CKE
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     500                         @//delay 500us

    ldr         r0, =0x005f                 @//DRAM reset+CS+CKE+auto cycle+ODE
    str         r0, [r1, #(0x00 << 1)]

CheckMiu0Init:
    ldr         r0, [r1, #(0x00 << 1)]
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         CheckMiu0Init

#if defined(CONFIG_MIU0_DDR4_2400)
    // This script is for MIU0 DDR4 VrefDQ change (begin)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldrb        r0, [r1, #(0x00 << 1)]
    orr         r0, #0x20
    strb        r0, [r1, #(0x00 << 1)]          //reg_auto_ref_off = 1
    ldrb        r0, [r1, #((0x06 << 1) + 1)]
    orr         r0, #0x10
    strb        r0, [r1, #((0x06 << 1) + 1)]    //reg_cke_always_on = 1

    // Set MRx[15:8] = access MR6
    ldr         r0, =0xc8
    strb        r0, [r1, #((0x18 << 1) + 1)]
    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    strb        r0, [r1, #((0x00 << 1) + 1)]    //reg_single_cmd    = 0 (MRW)

    // Set MRx[6:0] = Choose VrefDQ value & Range
    ldrb        r0, [r1, #(0x18 << 1)]
    bic         r0, #0x7f
    //RREG        r5, (MS_BUS_RIU_ADDR + (0x103382 << 1))
    ldr         r5, =0x20
    orr         r0, r5
    strb        r0, [r1, #(0x18 << 1)]          //0(Range)_11111(value)

    //Enable DQ Vref
    ldrb        r0, [r1, #(0x18 << 1)]
    orr         r0, #0x80
    strb        r0, [r1, #(0x18 << 1)]          // = 1 (Enable VrefDQ training)

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 1

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    DELAYUS     2

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 1

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    DELAYUS     2

    //  Disable set DQ Vref
    ldrb        r0, [r1, #(0x18 << 1)]
    bic         r0, #0x80
    strb        r0, [r1, #(0x18 << 1)]          // = 1 (Enable VrefDQ training)

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 1

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    DELAYUS     1

    ldrb        r0, [r1, #(0x00 << 1)]
    bic         r0, #0x20
    strb        r0, [r1, #(0x00 << 1)]          // reg_auto_ref_off = 0
#endif /* CONFIG_MIU0_DDR4_2400 */
#endif /* CONFIG_MIU0_DRAM_NONE */

#if !defined(CONFIG_MIU1_DRAM_NONE)
    @//MIU1 INIT Cycle
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0x0000                 @//hold miu_1
    str         r0, [r1, #(0x00 << 1)]
    ldr         r0, =0x0008                 @//DRAM reset
    str         r0, [r1, #(0x00 << 1)]
    ldr         r0, =0x000c                 @//DRAM reset+CS
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     1000                        @//delay 1ms

    ldr         r0, =0x000e                 @//DRAM reset+CS+CKE
    str         r0, [r1, #(0x00 << 1)]

    DELAYUS     500                         @//delay 500us

    ldr         r0, =0x005f                 @//DRAM reset+CS+CKE+auto cycle+ODE
    str         r0, [r1, #(0x00 << 1)]

CheckMiu1Init:
    ldr         r0, [r1, #(0x00 << 1)]
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         CheckMiu1Init

#if defined(CONFIG_MIU1_DDR4_2400)
    // This script is for MIU0 DDR4 VrefDQ change (begin)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldrb        r0, [r1, #(0x00 << 1)]
    orr         r0, #0x20
    strb        r0, [r1, #(0x00 << 1)]          //reg_auto_ref_off = 1
    ldrb        r0, [r1, #((0x06 << 1) + 1)]
    orr         r0, #0x10
    strb        r0, [r1, #((0x06 << 1) + 1)]    //reg_cke_always_on = 1

    // Set MRx[15:8] = access MR6
    ldr         r0, =0xc8
    strb        r0, [r1, #((0x18 << 1) + 1)]
    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    strb        r0, [r1, #((0x00 << 1) + 1)]    //reg_single_cmd    = 0 (MRW)

    // Set MRx[6:0] = Choose VrefDQ value & Range
    ldrb        r0, [r1, #(0x18 << 1)]
    bic         r0, #0x7f
    RREG        r5, (MS_BUS_RIU_ADDR + (0x103384 << 1))
    orr         r0, r5
    strb        r0, [r1, #(0x18 << 1)]          //0(Range)_11111(value)

    //Enable DQ Vref
    ldrb        r0, [r1, #(0x18 << 1)]
    orr         r0, #0x80
    strb        r0, [r1, #(0x18 << 1)]          // = 1 (Enable VrefDQ training)

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 1

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    DELAYUS     2

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 1

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    DELAYUS     2

    //  Disable set DQ Vref
    ldrb        r0, [r1, #(0x18 << 1)]
    bic         r0, #0x80
    strb        r0, [r1, #(0x18 << 1)]          // = 1 (Enable VrefDQ training)

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 1

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]    // reg_single_cmd_en = 0

    DELAYUS     1

    ldrb        r0, [r1, #(0x00 << 1)]
    bic         r0, #0x20
    strb        r0, [r1, #(0x00 << 1)]          // reg_auto_ref_off = 0
#endif /* CONFIG_MIU1_DDR4_2400 */
#endif /* CONFIG_MIU1_DRAM_NONE */

    WREG        (MS_BUS_RIU_ADDR + (0x1012fe << 1)), 0xa0e1

@//-----------------------------------STR--------------------------------------

    @// enter refreseh
    bl          Enter_DDR_RefreshMode
    DELAYUS     1000

    @// disable oenz
    bl          Miu_str_oenz_disable
    DELAYUS     1

    @// ATOP_PD on
#ifndef CONFIG_MIU0_DRAM_NONE
    WREG        (MS_BUS_RIU_ADDR + (0x110d54 <<1 )), 0xC000 @//ATOP_PD off
#endif
#ifndef CONFIG_MIU1_DRAM_NONE
    WREG        (MS_BUS_RIU_ADDR + (0x161654 <<1 )), 0xC000 @//ATOP_PD off
#endif


    @// rx enable
#ifndef CONFIG_MIU0_DRAM_NONE
#if defined(CONFIG_MIU_DDR4)
    WREG        (MS_BUS_RIU_ADDR + (0x110d08 << 1)), 0x007f
#else
    WREG        (MS_BUS_RIU_ADDR + (0x110d08 << 1)), 0x003f
#endif
#endif
#ifndef CONFIG_MIU1_DRAM_NONE
    WREG        (MS_BUS_RIU_ADDR + (0x161608 << 1)), 0x003f
#endif


    @// exit refresh
    bl          Exit_DDR_RefreshMode

    DELAYUS     300

#ifndef CONFIG_MIU0_DRAM_NONE
    @// push dram
    @// miu0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110D00 << 1))
    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x05   @//set bit1 & bit3 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x0A   @//set bit0 & bit2 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    bic         r2, r2, #0x0A   @// clear bit1 & bit3
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x0A   @//set bit0 & bit2 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    bic         r2, r2, #0x0A   @// clear bit1 & bit3
    str         r2, [r1, #(0x1E << 1)]
#endif

#ifndef CONFIG_MIU1_DRAM_NONE
    @// miu1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x05   @//set bit1 & bit3 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x0A   @//set bit0 & bit2 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    bic         r2, r2, #0x0A   @// clear bit1 & bit3
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    orr         r2, r2, #0x0A   @//set bit0 & bit2 = 1
    str         r2, [r1, #(0x1E << 1)]

    ldr         r2, [r1, #(0x1E << 1)]
    bic         r2, r2, #0x0A   @// clear bit1 & bit3
    str         r2, [r1, #(0x1E << 1)]
#endif


@//-----------------------------------STR--------------------------------------

    ldr         r0, =MIU_PostInit_Str
    bl          MDrv_WriteRegTbl32

#endif

#ifdef CONFIG_MSTAR_MOBF_ENABLE
    bl          BootRom_InitMobf_MiuStr
#endif

    mov         lr, r13
    bx          lr

@//-------------------------------------------------------------------------------------------------
@// Miu_str_oenz_disable
@// @param  None
@// @return None
@// @note   Miu_str_oenz_disable
@//-------------------------------------------------------------------------------------------------
Miu_str_oenz_disable:

#ifndef CONFIG_MIU0_DRAM_NONE

    ldr         r9, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r8, [r9, #(0x02 << 1)]
    bic         r8, r8, #0xf000
    str         r8, [r9, #(0x02 << 1)]

#endif /* CONFIG_MIU0_DRAM_NONE */

#ifndef CONFIG_MIU1_DRAM_NONE
    ldr         r9, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r8, [r9, #(0x02 << 1)]
    bic         r8, r8, #0xf000
    str         r8, [r9, #(0x02 << 1)]

#endif /* CONFIG_MIU1_DRAM_NONE */

    bx          lr
.ltorg
@//-------------------------------------------------------------------------------------------------
@// Enter_DDR_RefreshMode
@// @param  None
@// @return None
@// @note   Enter_DDR_RefreshMode
@//-------------------------------------------------------------------------------------------------
Enter_DDR_RefreshMode:
    WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0000

#ifndef CONFIG_MIU0_DRAM_NONE
    @// [DDR3][enter] <MIU0>
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0xFFFE
    str         r0, [r1, #(0x46 << 1)]
    ldr         r0, =0xFFFF
    str         r0, [r1, #(0x66 << 1)]
    str         r0, [r1, #(0x86 << 1)]
    str         r0, [r1, #(0xA6 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161500 << 1))
    str         r0, [r1, #(0x06 << 1)]
    str         r0, [r1, #(0x26 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x152000 << 1))
    str         r0, [r1, #(0x06 << 1)]
    str         r0, [r1, #(0x26 << 1)]

    DELAYUS     200

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x18 << 1)]

    ldrb        r0, [r1]
    bic         r0, #0x30
    orr         r0, #0x20
    strb        r0, [r1]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    orr         r0, #0x04
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    orr         r0, #0x02
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldr         r0, =0xffff
    str         r0, [r1, #(0x46 << 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x20
    orr         r0, #0x20
    strb        r0, [r1, #((0x00 << 1) + 1)]
#endif /* CONFIG_MIU0_DRAM_NONE */

#ifndef CONFIG_MIU1_DRAM_NONE
    @// [DDR3][enter] <MIU1>
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0xFFFE
    str         r0, [r1, #(0x46 << 1)]
    ldr         r0, =0xFFFF
    str         r0, [r1, #(0x66 << 1)]
    str         r0, [r1, #(0x86 << 1)]
    str         r0, [r1, #(0xA6 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162200 << 1))
    str         r0, [r1, #(0x06 << 1)]
    str         r0, [r1, #(0x26 << 1)]

    ldr         r2, =(MS_BUS_RIU_ADDR + (0x152100 << 1))
    str         r0, [r1, #(0x06 << 1)]
    str         r0, [r1, #(0x26 << 1)]

    DELAYUS     200

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x18 << 1)]

    ldrb        r0, [r1]
    bic         r0, #0x30
    orr         r0, #0x20
    strb        r0, [r1]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    orr         r0, #0x04
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    orr         r0, #0x02
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldr         r0, =0xffff
    str         r0, [r1, #(0x46 << 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x20
    orr         r0, #0x20
    strb        r0, [r1, #((0x00 << 1) + 1)]
#endif /* CONFIG_MIU1_DRAM_NONE */



    bx          lr

@//-------------------------------------------------------------------------------------------------
@// Exit_DDR_RefreshMode
@// @param  None
@// @return None
@// @note   Exit_DDR_RefreshMode
@//-------------------------------------------------------------------------------------------------
Exit_DDR_RefreshMode:

#ifndef CONFIG_MIU0_DRAM_NONE

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x20
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldr         r0, =0xfffe
    str         r0, [r1, #(0x46 << 1)]

    // Insert refresh cmd & trun on auto-refresh
    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    orr         r0, #0x02
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1]
    bic         r0, #0x30
    strb        r0, [r1]

    DELAYUS     5

    // enable ODT
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldrb        r0, [r1]
    bic         r0, #0x10
    orr         r0, #0x10
    strb        r0, [r1]

    DELAYUS     5

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0xFFFE
    str         r0, [r1, #(0x46 << 1)]
    ldr         r0, =0xFFFF
    str         r0, [r1, #(0x66 << 1)]
    str         r0, [r1, #(0x86 << 1)]
    str         r0, [r1, #(0xA6 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161500 << 1))
    str         r0, [r1, #(0x06 << 1)]
    str         r0, [r1, #(0x26 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x152000 << 1))
    str         r0, [r1, #(0x06 << 1)]
    str         r0, [r1, #(0x26 << 1)]

#endif /* CONFIG_MIU0_DRAM_NONE */

#ifndef CONFIG_MIU1_DRAM_NONE

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x20
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldr         r0, =0xfffe
    str         r0, [r1, #(0x46 << 1)]

    // Insert refresh cmd & trun on auto-refresh
    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    orr         r0, #0x02
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    orr         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x01
    strb        r0, [r1, #((0x00 << 1) + 1)]

    ldrb        r0, [r1]
    bic         r0, #0x30
    strb        r0, [r1]

    DELAYUS     5

    // Enable ODT
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldrb        r0, [r1]
    bic         r0, #0x10
    orr         r0, #0x10
    strb        r0, [r1]

    DELAYUS     5

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0xFFFE
    str         r0, [r1, #(0x46 << 1)]
    ldr         r0, =0xFFFF
    str         r0, [r1, #(0x66 << 1)]
    str         r0, [r1, #(0x86 << 1)]
    str         r0, [r1, #(0xA6 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162200 << 1))
    str         r0, [r1, #(0x06 << 1)]
    str         r0, [r1, #(0x26 << 1)]

    ldr         r2, =(MS_BUS_RIU_ADDR + (0x152100 << 1))
    str         r0, [r1, #(0x06 << 1)]
    str         r0, [r1, #(0x26 << 1)]

#endif /* CONFIG_MIU1_DRAM_NONE */


    bx          lr

@//-------------------------------------------------------------------------------------------------
@// Release_Sw_Reset
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
Release_Sw_Reset:
    //MIU1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0x8c00
    str         r0, [r1, #(0x1e << 1)]
    //MIU0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0x8c00
    str         r0, [r1, #(0x1e << 1)]
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161500 << 1))
    ldr         r0, =0x0000
    str         r0, [r1, #(0xe6 << 1)]
    //MIU1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162200 << 1))
    ldr         r0, =0x0000
    str         r0, [r1, #(0xe6 << 1)]
    bx          lr

@//-------------------------------------------------------------------------------------------------
@// Reset_AVD_str
@// @param  None
@// @return None
@// @note  Reset_AVD_str
@//-------------------------------------------------------------------------------------------------
Reset_AVD_str:

    ldr         r12, =(MS_BUS_RIU_ADDR + (0x103600 << 1))
    ldr         r8, [r12, #(0x12 << 1)]
    bic         r8, r8, #0x4000
    str         r8, [r12, #(0x12 << 1)]

    ldr         r8, [r12, #(0x12 << 1)]
    orr         r8, r8, #0x4000
    str         r8, [r12, #(0x12 << 1)]

    ldr         r8, [r12, #(0x12 << 1)]
    bic         r8, r8, #0x4000
    str         r8, [r12, #(0x12 << 1)]

    bx          lr

@//-------------------------------------------------------------------------------------------------
@// Bist for str
@// @param  None
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_MSTAR_SELF_STR_TEST_ENABLE
    .global     Miu1_BistTest_Str
Miu1_BistTest_Str:
    DPUTK_STR   MIU_MESSAGE_BIST1_Str

    ldr         r12, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r11, =0x0000
    str         r11, [r12, #(0xe0 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0000   @//1012e0 //reset bist test
    ldr         r11, =0x8000
    str         r11, [r12, #(0xf8 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012f8 << 1)), 0x8000   @//1012f0 bit_15=1 choice to MIU_1
    WREG        (MS_BUS_RIU_ADDR + (0x1620f8 << 1)), 0x0000
    b           Miu_BistTest_Str

    .global     Miu0_BistTest_Str
Miu0_BistTest_Str:
    DPUTK_STR   MIU_MESSAGE_BIST0_Str

    ldr         r12, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r11, =0x0000
    str         r11, [r12, #(0xe0 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0000   @//1012e0 //reset bist test
    str         r11, [r12, #(0xf8 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012f0 << 1)), 0x0000   @//1012f0 bit_15=0 choice to MIU_0
    WREG        (MS_BUS_RIU_ADDR + (0x1620f8 << 1)), 0x0000

    .global     Miu_BistTest_Str
Miu_BistTest_Str:
    ldr         r12, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r11, =0x0000
    str         r11, [r12, #(0xe2 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e2 << 1)), 0x0000   @//1012e2 //start address base unit:4 k bytes

    ldr         r0, =0xFFFF
    ldr         r11, =0xFFFF
    and         r11, r11, r0
    str         r11, [r12, #(0xe4 << 1)]
    ldr         r11, =0x00F
    and         r11, r11, r0
    str         r11, [r12, #(0xe6 << 1)]    @//WREG_D      (MS_BUS_RIU_ADDR + (0x1012e4 << 1)), BIST_LEN

    ldr         r11, =0x5aa5
    str         r11, [r12, #(0xe8 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e8 << 1)), 0x5aa5   @//1012e8 //bist test pattern

    WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0101   @//bist test mode:3 + read mode

Miu_CheckBistLoop_Str:
    RREG        r1, (MS_BUS_RIU_ADDR + (0x1012e0 << 1))
    and         r0, r1, #0x8000
    cmp         r0, #0
    beq         Miu_CheckBistLoop_Str

    and         r0, r1, #0x6000
    cmp         r0, #0
    beq         Miu_Bist_OK_Str

Miu_Bist_Fail_Str:
    DPUTK_STR   MIU_MESSAGE_FAIL_Str

Miu_Bist_FailLoop_Str:
    b           Miu_Bist_FailLoop_Str

Miu_Bist_OK_Str:
    DPUTK_STR   MIU_MESSAGE_OK_Str

    bx          lr
#endif

#ifdef CONFIG_MSTAR_MOBF_ENABLE
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitMobf_MiuStr
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitMobf_MiuStr
BootRom_InitMobf_MiuStr:

    WREG        (MS_BUS_RIU_ADDR + (0x100A32 << 1)), 0x0010
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0xC001

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x0001
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x0302
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0002

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x0504
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x0706
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0003

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x0908
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x0B0A
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0004

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x0D0C
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x0F0E
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0005

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x1001
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x1302
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0006

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x1504
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x1706
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0007

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x1908
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x1B0A
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0008

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x1D0C
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x1F0E
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0009

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x00F1
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x03F2
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x000A

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x05F4
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x07F6
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x000B

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x09F8
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x0BFA
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x000C

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x0DFC
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x0FFE
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x000D

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x7001
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x7302
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x000E

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x7504
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x7706
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x000F

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x7908
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x7B0A
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0010

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x7D0C
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x7F0E
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0011

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x0081
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x0382
    WREG        (MS_BUS_RIU_ADDR + (0x113BEC << 1)), 0x0012

    WREG        (MS_BUS_RIU_ADDR + (0x113BE8 << 1)), 0x0084
    WREG        (MS_BUS_RIU_ADDR + (0x113BEA << 1)), 0x0786

    @//Set HW key mode
    RREG        r0, (MS_BUS_RIU_ADDR + (0x113BEC << 1))
    orr         r0, r0, #0x0060
    REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x113BEC << 1))

    bx          lr
#endif

    .size       BootRom_InitMiu_STR,.-BootRom_InitMiu_STR
//-------------------------------------------------------------------------------------------------
// MIU_Secure_flow_str
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
MIU_Secure_flow_str:

    //1. Set all DRAM in secure range
    //MIU0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123700 << 1))
    ldr         r0, =0x0
    str         r0, [r1]                    // start address low
    str         r0, [r1, #(0x02 << 1)]      // start address high
    str         r0, [r1, #(0x04 << 1)]      // end address low
    ldr         r0, =0x08
    orr         r0, #0x300
    str         r0, [r1, #(0x06 << 1)]      // end address high
    ldr         r0, =0x8308
    str         r0, [r1, #(0x06 << 1)]      // end address high
    ldr         r0, =0x1
    str         r0, [r1, #(0xec << 1)]      // secure range 0 det en

    //MIU1
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123800 << 1))
    ldr         r0, =0x0
    str         r0, [r1]                    // start address low
    str         r0, [r1, #(0x02 << 1)]      // start address high
    str         r0, [r1, #(0x04 << 1)]      // end address low
    ldr         r0, =0x08
    orr         r0, #0x300
    str         r0, [r1, #(0x06 << 1)]      // end address high
    ldr         r0, =0x8308
    str         r0, [r1, #(0x06 << 1)]      // end address high
    ldr         r0, =0x1
    str         r0, [r1, #(0xec << 1)]      // secure range 0 det en

    //2. Set all RIU to secure, except MIU bank
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, [r1, #(0x02 << 1)]
    orr         r0, #(0x01 << 2)
    str         r0, [r1, #(0x02 << 1)]
    ldr         r0, =(0x01 << 5)        //0x1006(5)
    str         r0, [r1, #(0x20 << 1)]
    ldr         r0, =(0x01 << 6)        //0x1012(22)
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0x0
    str         r0, [r1, #(0x24 << 1)]
    str         r0, [r1, #(0x26 << 1)]
    str         r0, [r1, #(0x28 << 1)]
    ldr         r0, =(0x01 << 7)        //0x110d(87)
    str         r0, [r1, #(0x2a << 1)]
    ldr         r0, =0x0
    str         r0, [r1, #(0x2c << 1)]
    str         r0, [r1, #(0x2e << 1)]
    str         r0, [r1, #(0x30 << 1)]
    str         r0, [r1, #(0x32 << 1)]
    str         r0, [r1, #(0x34 << 1)]
    str         r0, [r1, #(0x36 << 1)]
    str         r0, [r1, #(0x38 << 1)]
    str         r0, [r1, #(0x3a << 1)]
    str         r0, [r1, #(0x3c << 1)]
    ldr         r0, =(0x01 << 13)        //0x1616(253)
    str         r0, [r1, #(0x3e << 1)]
    ldr         r0, =0x0
    str         r0, [r1, #(0x40 << 1)]
    str         r0, [r1, #(0x42 << 1)]
    str         r0, [r1, #(0x44 << 1)]
    str         r0, [r1, #(0x46 << 1)]
    str         r0, [r1, #(0x48 << 1)]
    str         r0, [r1, #(0x4a << 1)]
    str         r0, [r1, #(0x4c << 1)]
    str         r0, [r1, #(0x4e << 1)]
    str         r0, [r1, #(0x50 << 1)]
    str         r0, [r1, #(0x52 << 1)]
    str         r0, [r1, #(0x54 << 1)]

    //3. enable block 51 patch
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x3900 << 1))
    ldr         r0, =0x01
    str         r0, [r1, #(0x64 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, [r1, #(0x60 << 1)]
    orr         r0, #(0x01 << 2)
    str         r0, [r1, #(0x60 << 1)]

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x0e00 << 1))
    ldr         r0, =0xbabe
    str         r0, [r1, #(0xaa << 1)]

    //4. announce 51 to access die domain
MIU_SECURE_LOOP:
    ldr         r0, [r1, #(0xaa << 1)]
    ldr         r2, =0xdead
    cmp         r0, r2
    bne         MIU_SECURE_LOOP

#ifndef CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51
    //5. ARM kill PM51, pull PM51 reset
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x2e00 << 1))
    ldr         r3, [r1, #(0x52 << 1)]
    bic         r3, r3, #0x00001000         @//Reset PM51
    str         r3, [r1, #(0x52 << 1)]
#endif

    //6. ARM Release all DRAM in secure range
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123700 << 1))
    ldr         r0, =0x0
    str         r0, [r1, #(0x06 << 1)]      // end address high
    str         r0, [r1, #(0x06 << 1)]      // end address high
    str         r0, [r1, #(0xec << 1)]      // secure range 0 det en

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123800 << 1))
    ldr         r0, =0x0
    str         r0, [r1, #(0x06 << 1)]      // end address high
    str         r0, [r1, #(0x06 << 1)]      // end address high
    str         r0, [r1, #(0xec << 1)]      // secure range 0 det en

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, =0xffff
    str         r0, [r1, #(0x20 << 1)]
    str         r0, [r1, #(0x22 << 1)]
    str         r0, [r1, #(0x24 << 1)]
    str         r0, [r1, #(0x26 << 1)]
    str         r0, [r1, #(0x28 << 1)]
    str         r0, [r1, #(0x2a << 1)]
    str         r0, [r1, #(0x2c << 1)]
    str         r0, [r1, #(0x2e << 1)]
    str         r0, [r1, #(0x30 << 1)]
    str         r0, [r1, #(0x32 << 1)]
    str         r0, [r1, #(0x34 << 1)]
    str         r0, [r1, #(0x36 << 1)]
    str         r0, [r1, #(0x38 << 1)]
    str         r0, [r1, #(0x3a << 1)]
    str         r0, [r1, #(0x3c << 1)]
    str         r0, [r1, #(0x3e << 1)]
    str         r0, [r1, #(0x40 << 1)]
    str         r0, [r1, #(0x42 << 1)]
    str         r0, [r1, #(0x44 << 1)]
    str         r0, [r1, #(0x46 << 1)]
    str         r0, [r1, #(0x48 << 1)]
    str         r0, [r1, #(0x4a << 1)]
    str         r0, [r1, #(0x4c << 1)]
    str         r0, [r1, #(0x4e << 1)]
    str         r0, [r1, #(0x50 << 1)]
    str         r0, [r1, #(0x52 << 1)]
    str         r0, [r1, #(0x54 << 1)]

    bx          lr
