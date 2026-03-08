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

#ifndef _BOARD_H_
#include "Board.h"
#endif

#ifndef _DRV_MIU_INIT_H_
#include "drv_miu_init.h"
#endif

#ifndef _MIUSEL_MAXIM_2MIU_H_
#include "MIUSEL_MAXIM_2MIU.h"
#endif

#ifdef CONFIG_ONEBIN_ENABLE
#include "drvLoadTbl.h"
#endif

    .text

MIU_MESSAGE_BIST0:      .short   'B', 'I', 'S', 'T', '0', '-', 0x00
MIU_MESSAGE_BIST1:      .short   'B', 'I', 'S', 'T', '1', '-', 0x00
MIU_MESSAGE_BIST2:      .short   'B', 'I', 'S', 'T', '2', '-', 0x00

MIU_MESSAGE_OK:         .short   'O', 'K', '\n', '\r', 0x00
MIU_MESSAGE_FAIL:       .short   'F', 'A', 'I', 'L', '\n', '\r', 0x00

    .align      4
    .global     BootRom_InitMiu
BootRom_InitMiu:

    mov         r13, lr

#ifdef CONFIG_MSTAR_TITANIA_BD_FPGA

    WREG        (MS_BUS_RIU_ADDR + (0x002bc6 << 1)), 0x3c11
    WREG        (MS_BUS_RIU_ADDR + (0x002bca << 1)), 0x403c
    WREG        (MS_BUS_RIU_ADDR + (0x002bc4 << 1)), 0x0004

    WREG        (MS_BUS_RIU_ADDR + (0x101202 << 1)), 0x0854
    WREG        (MS_BUS_RIU_ADDR + (0x101204 << 1)), 0x01C5
    WREG        (MS_BUS_RIU_ADDR + (0x101206 << 1)), 0x0120
    WREG        (MS_BUS_RIU_ADDR + (0x101208 << 1)), 0x0833
    WREG        (MS_BUS_RIU_ADDR + (0x10120a << 1)), 0x0c33
    WREG        (MS_BUS_RIU_ADDR + (0x10120c << 1)), 0x7111
    WREG        (MS_BUS_RIU_ADDR + (0x10120e << 1)), 0x100e
    WREG        (MS_BUS_RIU_ADDR + (0x101210 << 1)), 0x0031

    WREG        (MS_BUS_RIU_ADDR + (0x10122a << 1)), 0x000f
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x0C00
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x0C01
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x0C00

    WREG        (MS_BUS_RIU_ADDR + (0x100602 << 1)), 0x0854
    WREG        (MS_BUS_RIU_ADDR + (0x100604 << 1)), 0x01C5
    WREG        (MS_BUS_RIU_ADDR + (0x100606 << 1)), 0x0120
    WREG        (MS_BUS_RIU_ADDR + (0x100608 << 1)), 0x0833
    WREG        (MS_BUS_RIU_ADDR + (0x10060a << 1)), 0x0c33
    WREG        (MS_BUS_RIU_ADDR + (0x10060c << 1)), 0x7111
    WREG        (MS_BUS_RIU_ADDR + (0x10060e << 1)), 0x100e
    WREG        (MS_BUS_RIU_ADDR + (0x100610 << 1)), 0x0031

    WREG        (MS_BUS_RIU_ADDR + (0x10062a << 1)), 0x000f
    WREG        (MS_BUS_RIU_ADDR + (0x10061e << 1)), 0x0C00
    WREG        (MS_BUS_RIU_ADDR + (0x10061e << 1)), 0x0C01
    WREG        (MS_BUS_RIU_ADDR + (0x10061e << 1)), 0x0C00

    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x0008
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x000c
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x101200 << 1)), 0x001f

    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x0000
    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x0008
    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x000c
    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x000e
    WREG        (MS_BUS_RIU_ADDR + (0x100600 << 1)), 0x001f

    WREG        (MS_BUS_RIU_ADDR + (0x10061e << 1)), 0x0C08
    WREG        (MS_BUS_RIU_ADDR + (0x10121e << 1)), 0x0C08

_CheckDramData:

    RREG        r0, (MS_BUS_RIU_ADDR + (0x101200 << 1))
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         _CheckDramData

#else

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x3900 << 1))
    ldr         r0, [r1, #(0x64 << 1)]
    orr         r0, #0x01
    str         r0, [r1, #(0x64 << 1)]
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123900 << 1))
    ldr         r0, [r1, #(0x60 << 1)]
    orr         r0, #(0x01 << 2)
    str         r0, [r1, #(0x60 << 1)]

    ldr         r0, =MIU_PreInit
    bl          MDrv_WriteRegTbl32

#if !defined(CONFIG_MIU0_DRAM_NONE)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0001
    str         r0, [r1, #(0x78 << 1)]
    ldr         r0, =0x1000
    str         r0, [r1, #(0x24 << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0x24 << 1)]

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
    ldr         r0, =0x0124
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
    ldr         r0, =MIU0_DDR_Init
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
    ldr         r0, =MIU1_DDR_Init
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

#endif /* CONFIG_MIU0_DRAM_NONE */

#if !defined(CONFIG_ENABLE_MIU0_AUTO_DQS)
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
#endif //CONFIG_ENABLE_MIU0_AUTO_DQS

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

@//================================================================================================
#ifdef CONFIG_MSTAR_EFUSE_DDR_ATOP
#if defined(CONFIG_MIU0_DDR4_2400)

    @//Initial DDR ATOP Trimming Control Setting
#if !defined(CONFIG_MIU0_DRAM_NONE)
    ldr         r0, =0x214c
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110D00 << 1))
    ldr         r2, =0x0
    bl          BootRom_InitDdrAtopTrimmingControl
#endif
#if !defined(CONFIG_MIU1_DRAM_NONE)
    ldr         r0, =0x214c
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r2, =0x2
    ldr         r3, =0x8
    bl          BootRom_InitDdrAtopTrimmingControl
#endif

#else

    @//Initial DDR ATOP Trimming Control Setting
#if !defined(CONFIG_MIU0_DRAM_NONE)
    ldr         r0, =0x0148
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110D00 << 1))
    ldr         r2, =0x0
    bl          BootRom_InitDdrAtopTrimmingControl
#endif
#if !defined(CONFIG_MIU1_DRAM_NONE)
    ldr         r0, =0x0148
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r2, =0x1
    ldr         r3, =0x8
    ldr         r4, =0x6 //(22-16)
    bl          BootRom_InitDdrAtopTrimmingControl
#endif

#endif //CONFIG_MIU0_DDR4_2400
#endif //CONFIG_MSTAR_EFUSE_DDR_ATOP
#endif /* CONFIG_MIU1_DRAM_NONE */

#if !defined(CONFIG_ENABLE_MIU1_AUTO_DQS)
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
#endif //CONFIG_ENABLE_MIU1_AUTO_DQS

#if defined(CONFIG_ENABLE_AUTO_DQS)
#if !defined(CONFIG_MIU0_DRAM_NONE)
    @//Scan MIU0 DQS
    ldr         r0, =0
#if (CONFIG_MIU_DDR4 == 1)
    bl          BootRom_MiuDdr4Dqs
#elif (CONFIG_MIU_DDR3 == 1)
    bl          BootRom_MiuDdr3Dqs
#endif

#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    @//Scan MIU1 DQS
    ldr         r0, =1

#if (CONFIG_MIU_DDR4 == 1)
    bl          BootRom_MiuDdr4Dqs
#elif (CONFIG_MIU_DDR3 == 1)
    bl          BootRom_MiuDdr3Dqs
#endif

#endif

#endif

    DELAYUS     800                                         //delay 800us

#if !defined(CONFIG_MIU0_DRAM_NONE)
    @//MIU0 BIST test
    bl          Miu0_BistTest
#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    @//MIU1 BIST test
    bl          Miu1_BistTest
#endif

    ldr         r0, =MIU_PostInit
    bl          MDrv_WriteRegTbl32

#endif

#ifdef CONFIG_MSTAR_MOBF_ENABLE
    bl          BootRom_InitMobf_Miu
#endif

    mov         lr, r13
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
@// MiuBistTest
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
#if !defined(CONFIG_MIU1_DRAM_NONE)
    .global     Miu1_BistTest
Miu1_BistTest:

    DPUTK_STR   MIU_MESSAGE_BIST1

    WREG        (MS_BUS_RIU_ADDR + (0x162206 << 1)), 0x7FFF @//unmask MIU1_group_4 bit15
    ldr         r12, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r11, =0x0000
    str         r11, [r12, #(0xe0 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0000   @//1012e0 //reset bist test
    ldr         r11, =0x8000
    str         r11, [r12, #(0xf8 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012f8 << 1)), 0x8000   @//1012f8 bit_15=1 choice to MIU_1
    WREG        (MS_BUS_RIU_ADDR + (0x1620f8 << 1)), 0x0000                                              @//1620f8 bit_15=0 choice to MIU_1
    b           Miu_BistTest
#endif

    .global     Miu0_BistTest
Miu0_BistTest:

    DPUTK_STR   MIU_MESSAGE_BIST0

    WREG        (MS_BUS_RIU_ADDR + (0x161506 << 1)), 0x7FFF @//unmask MIU0_group_4 bit15
    ldr         r12, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r11, =0x0000
    str         r11, [r12, #(0xe0 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0000   @//1012e0 //reset bist test
    str         r11, [r12, #(0xf8 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012f8 << 1)), 0x0000   @//1012f8 bit_15=0 choice to MIU_0
    WREG        (MS_BUS_RIU_ADDR + (0x1620f8 << 1)), 0x0000                                              @//1620f8 bit_15=0 choice to MIU_0

    .global     Miu_BistTest
Miu_BistTest:
    ldr         r12, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r11, =0x0000
    str         r11, [r12, #(0xe2 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e2 << 1)), 0x0000   @//1012e2 //start address base unit:4 k bytes

    ldr         r0, =0xFFFF
    ldr         r11, =BIST_LEN
    and         r11, r11, r0
    str         r11, [r12, #(0xe4 << 1)]
    ldr         r11, =BIST_LEN
    lsr         r11, #16
    and         r11, r11, r0
    str         r11, [r12, #(0xe6 << 1)]    @//WREG_D      (MS_BUS_RIU_ADDR + (0x1012e4 << 1)), BIST_LEN

    ldr         r11, =0x5aa5
    str         r11, [r12, #(0xe8 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e8 << 1)), 0x5aa5   @//1012e8 //bist test pattern

    WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0002   @//
    WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0003   @//bist test mode:3

Miu_CheckBistLoop:
    RREG        r1, (MS_BUS_RIU_ADDR + (0x1012e0 << 1))
    and         r0, r1, #0x8000
    cmp         r0, #0
    beq         Miu_CheckBistLoop

    and         r0, r1, #0x6000
    cmp         r0, #0
    beq         Miu_Bist_OK

Miu_Bist_Fail:
    DPUTK_STR   MIU_MESSAGE_FAIL

Miu_Bist_FailLoop:
    b           Miu_Bist_FailLoop

Miu_Bist_OK:
    DPUTK_STR   MIU_MESSAGE_OK

Miu_Bist_Exit:

    bx          lr

#ifdef CONFIG_MSTAR_EFUSE_DDR_ATOP
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitDdrAtopTrimmingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitDdrAtopTrimmingControl
BootRom_InitDdrAtopTrimmingControl:

    mov         r11, lr

    @//Get Trim Value of DDR_ATOP in eFuse
    ldr         r5, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    str         r0, [r5, #(0x50 << 1)]
    orr         r0, r0, #0x2000
    str         r0, [r5, #(0x50 << 1)]

BootRom_ReadEfuse_DdrAtopLoop_0:

    ldr         r0, [r5, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_DdrAtopLoop_0

    ldr         r6, [r5, #(0x58 << 1)]
    ldr         r7, [r5, #(0x5a << 1)]
    cmp         r2, #1
    beq         BootRom_ReadEfuse_DdrAtopLoop_1
    cmp         r2, #2
    beq         BootRom_ReadEfuse_DdrAtopLoop_2

    mov         r7, r7, lsl #16
    orr         r0, r6, r7
    b           BootRom_ReadEfuse_DdrAtopLoop_Exit

BootRom_ReadEfuse_DdrAtopLoop_1:
    mov         r6, r6, lsr r3
    and         r6, r6, #0x7
    mov         r7, r7, lsr r4
    mov         r7, r7, lsl #3
    orr         r0, r6, r7

    b           BootRom_ReadEfuse_DdrAtopLoop_Exit

BootRom_ReadEfuse_DdrAtopLoop_2:
    mov         r7, r7, lsl #16
    orr         r0, r6, r7
    mov         r0, r0, lsr r3

BootRom_ReadEfuse_DdrAtopLoop_Exit:

    bl          BootRom_DdrDrivingControl

    mov         lr, r11
    bx          lr


@//-------------------------------------------------------------------------------------------------
@// BootRom_DdrDrivingControl
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_DdrDrivingControl
BootRom_DdrDrivingControl:
BootRom_DdrDrivingControl_DRVP:
    mov         r10, lr
    @//MIU_drvp
    and         r4, r0, #(0x01 << 3)
    cmp         r4, #0
    beq         BootRom_DdrDrivingControl_DRVN

    @//eFuse IO[2:0] = Driving Strength
    and         r4, r0, #0x07

    @//{0x110D_DE[8], 0x110D_5e[3:0]} = dqs
    ldr         r2, [r1, #(0x5e << 1)]
    and         r2, r2, #(0x0f << 0)
    lsr         r2, r2, #0
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 8)
    lsr         r3, r3, #8
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5e << 1)]
    bic         r5, r5, #(0x0f << 0)
    and         r6, r2, #0x0f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0x5e << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 8)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//dqs

    @//{0x110D_DE[9], 0x110D_5a[11:8]} = dqs_h
    ldr         r2, [r1, #(0x5a << 1)]
    and         r2, r2, #(0x0f << 8)
    lsr         r2, r2, #8
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 9)
    lsr         r3, r3, #9
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5a << 1)]
    bic         r5, r5, #(0x0f << 8)
    and         r6, r2, #0x0f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0x5a << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 9)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #9
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//dqs_h

    @//{0x110D_DE[10], 0x110D_5e[7:4]} = dq
    ldr         r2, [r1, #(0x5e << 1)]
    and         r2, r2, #(0x0f << 4)
    lsr         r2, r2, #4
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 10)
    lsr         r3, r3, #10
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5e << 1)]
    bic         r5, r5, #(0x0f << 4)
    and         r6, r2, #0x0f
    lsl         r6, r6, #4
    orr         r5, r5, r6
    str         r5, [r1, #(0x5e << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 10)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #10
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//dq

    @//{0x110D_DE[11], 0x110D_5a[15:12]} = dq_h
    ldr         r2, [r1, #(0x5a << 1)]
    and         r2, r2, #(0x0f << 12)
    lsr         r2, r2, #12
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 11)
    lsr         r3, r3, #11
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5a << 1)]
    bic         r5, r5, #(0x0f << 12)
    and         r6, r2, #0x0f
    lsl         r6, r6, #12
    orr         r5, r5, r6
    str         r5, [r1, #(0x5a << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 11)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #11
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//dq_h

    @//{0x110D_DE[12], 0x110D_5E[11:8]} = CMD
    ldr         r2, [r1, #(0x5E << 1)]
    and         r2, r2, #(0x0f << 8)
    lsr         r2, r2, #8
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 12)
    lsr         r3, r3, #12
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5E << 1)]
    bic         r5, r5, #(0x0f << 8)
    and         r6, r2, #0x0f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0x5E << 1)]

    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 12)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #12
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//CMD

    @//{0x110D_DE[13], 0x110D_5e[15:12]} = Clk
    ldr         r2, [r1, #(0x5e << 1)]
    and         r2, r2, #(0x0f << 12)
    lsr         r2, r2, #12
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 13)
    lsr         r3, r3, #13
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5e << 1)]
    bic         r5, r5, #(0x0f << 12)
    and         r6, r2, #0x0f
    lsl         r6, r6, #12
    orr         r5, r5, r6
    str         r5, [r1, #(0x5e << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 13)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #13
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//clk

    @//{0x110D_DE[14], 0x110D_58[11:8]} = CSB
    ldr         r2, [r1, #(0x58 << 1)]
    and         r2, r2, #(0x0f << 8)
    lsr         r2, r2, #8
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 14)
    lsr         r3, r3, #14
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x58 << 1)]
    bic         r5, r5, #(0x0f << 8)
    and         r6, r2, #0x0f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0x58 << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 14)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #14
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//CSB

BootRom_DdrDrivingControl_DRVN:

    @//MIU_drvn
    and         r4, r0, #(0x01 << 7)
    cmp         r4, #0
    beq         BootRom_DdrDrivingControl_Exit

    @//eFuse IO[2:0] = Driving Strength
    lsr         r4, r0, #0x4
    and         r4, r4, #0x7

    @//{0x110D_DE[0], 0x110D_5c[3:0]} = dqs
    ldr         r2, [r1, #(0x5c << 1)]
    and         r2, r2, #(0x0f << 0)
    lsr         r2, r2, #0
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 0)
    lsr         r3, r3, #0
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5c << 1)]
    bic         r5, r5, #(0x0f << 0)
    and         r6, r2, #0x0f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0x5c << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 0)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//dqs

    @//{0x110D_DE[1], 0x110D_5a[3:0]} = dqs_h
    ldr         r2, [r1, #(0x5a << 1)]
    and         r2, r2, #(0x0f << 0)
    lsr         r2, r2, #0
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 1)
    lsr         r3, r3, #1
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5a << 1)]
    bic         r5, r5, #(0x0f << 0)
    and         r6, r2, #0x0f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0x5a << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 1)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #1
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//dqs_h

    @//{0x110D_DE[2], 0x110D_5c[7:4]} = dq
    ldr         r2, [r1, #(0x5c << 1)]
    and         r2, r2, #(0x0f << 4)
    lsr         r2, r2, #4
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 2)
    lsr         r3, r3, #2
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5c << 1)]
    bic         r5, r5, #(0x0f << 4)
    and         r6, r2, #0x0f
    lsl         r6, r6, #4
    orr         r5, r5, r6
    str         r5, [r1, #(0x5c << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 2)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #2
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//dq

    @//{0x110D_DE[3], 0x110D_5a[7:4]} = dq_h
    ldr         r2, [r1, #(0x5a << 1)]
    and         r2, r2, #(0x0f << 4)
    lsr         r2, r2, #4
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 3)
    lsr         r3, r3, #3
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5a << 1)]
    bic         r5, r5, #(0x0f << 4)
    and         r6, r2, #0x0f
    lsl         r6, r6, #4
    orr         r5, r5, r6
    str         r5, [r1, #(0x5a << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 3)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #3
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//dq_h

    @//{0x110D_DE[4], 0x110D_5c[11:8]} = CMD
    ldr         r2, [r1, #(0x5c << 1)]
    and         r2, r2, #(0x0f << 8)
    lsr         r2, r2, #8
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 4)
    lsr         r3, r3, #4
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5c << 1)]
    bic         r5, r5, #(0x0f << 8)
    and         r6, r2, #0x0f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0x5c << 1)]

    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 4)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #4
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//CMD

    @//{0x110D_DE[5], 0x110D_5c[15:12]} = Clk
    ldr         r2, [r1, #(0x5c << 1)]
    and         r2, r2, #(0x0f << 12)
    lsr         r2, r2, #12
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 5)
    lsr         r3, r3, #5
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x5c << 1)]
    bic         r5, r5, #(0x0f << 12)
    and         r6, r2, #0x0f
    lsl         r6, r6, #12
    orr         r5, r5, r6
    str         r5, [r1, #(0x5c << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 5)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #5
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//clk

    @//{0x110D_DE[6], 0x110D_58[3:0]} = CSB
    ldr         r2, [r1, #(0x58 << 1)]
    and         r2, r2, #(0x0f << 0)
    lsr         r2, r2, #0
    ldr         r3, [r1, #(0xDE << 1)]
    and         r3, r3, #(0x01 << 6)
    lsr         r3, r3, #6
    lsl         r3, r3, #4
    orr         r2, r2, r3

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0x58 << 1)]
    bic         r5, r5, #(0x0f << 0)
    and         r6, r2, #0x0f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0x58 << 1)]
    ldr         r5, [r1, #(0xDE << 1)]
    bic         r5, r5, #(0x01 << 6)
    and         r6, r2, #0x10
    lsr         r6, r6, #4
    lsl         r6, r6, #6
    orr         r5, r5, r6
    str         r5, [r1, #(0xDE << 1)]  @//CSB

BootRom_DdrDrivingControl_Exit:
    mov         lr, r10
    bx          lr

@//-------------------------------------------------------------------------------------------------
@// BootRom_DdrAtopTrimmingDataProcess
@// @param  r2: DDR ATOP value, r4: efuse value
@// @return None
@// @note r3,r4,r5 used
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_DdrAtopTrimmingDataProcess
BootRom_DdrAtopTrimmingDataProcess:

    @//Check Driving Strength Group ([+1, +2], [0, -1, -2]
    and         r5, r4, #(0x01 << 2)
    cmp         r5, #0
    beq         BootRom_DdrAtopTrimmingDataProcess_1_1

BootRom_DdrAtopTrimmingDataProcess_Round_1_0:

    @//Driving Strength Group [+1, +2]
    and         r5, r4, #0x03

    @//If Driving Strength was 3, That is a Fail Case
    #cmp         r5, #0x3
    #beq         BootRom_InitDdrAtopTrimmingDataProcessExit

    @//Add Driving Strength in eFuse
    add         r2, r2, r5, lsl #1

    cmp         r2, #0x1f
    ble         BootRom_InitDdrAtopTrimmingDataProcessExit

    ldr         r2, =0x1f
    b           BootRom_InitDdrAtopTrimmingDataProcessExit

BootRom_DdrAtopTrimmingDataProcess_1_1:

    @//Driving Strength Group [0, -1, -2]
    and         r5, r4, #0x03

    @//If Driving Strength was 3, That is a Fail Case
    #cmp         r5, #3
    #beq         BootRom_InitDdrAtopTrimmingDataProcessExit

	//lsl  		r5, #1
    cmp         r2, r5
    blt         BootRom_DdrAtopTrimmingDataProcess_1_2

    @//Subtract Driving Strength in eFuse
    sub         r2, r2, r5, lsl #1
    b           BootRom_InitDdrAtopTrimmingDataProcessExit

BootRom_DdrAtopTrimmingDataProcess_1_2:

    @//If The Default Value is less than The Value of Driving Strength, Force to 0
    ldr         r2, =0x00

BootRom_InitDdrAtopTrimmingDataProcessExit:

    bx          lr

#endif

#ifdef CONFIG_MSTAR_MOBF_ENABLE
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitMobf_Miu
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitMobf_Miu
BootRom_InitMobf_Miu:

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

    .size       BootRom_InitMiu,.-BootRom_InitMiu
