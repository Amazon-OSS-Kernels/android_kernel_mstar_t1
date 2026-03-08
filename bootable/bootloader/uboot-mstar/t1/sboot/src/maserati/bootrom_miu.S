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
#include "drv_miu_init.h"
#endif

#ifndef _MIUSEL_MASERATI_2MIU_H_
#include "MIUSEL_MASERATI_2MIU.h"
#endif
#if (ENABLE_ONEBIN_ENABLE == 1)
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
    #error "Not support DDR3-1666"

#elif defined(CONFIG_MIU0_DDR3_1866)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0402
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0xa229
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x001d
    str         r0, [r1, #(0x32 << 1)]

#elif defined(CONFIG_MIU0_DDR3_2133)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x34e6
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x001a
    str         r0, [r1, #(0x32 << 1)]

#elif defined(CONFIG_MIU0_DDR4_2400)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x110d00 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x0a37
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x0017
    str         r0, [r1, #(0x32 << 1)]

#else
    #error "Invalid DRAM Setting"

#endif

    DELAYUS     100

    ldr         r0, =0x0004
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0x0114
    str         r0, [r1, #(0x2c << 1)]

#if (ENABLE_ONEBIN_ENABLE == 1)
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
    #error "Not support DDR3-1666"

#elif defined(CONFIG_MIU1_DDR3_1866)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0402
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0xa229
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x001d
    str         r0, [r1, #(0x32 << 1)]

#elif defined(CONFIG_MIU1_DDR3_2133)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x34e6
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x001a
    str         r0, [r1, #(0x32 << 1)]

#elif defined(CONFIG_MIU1_DDR4_2400)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x161600 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x0a37
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x0017
    str         r0, [r1, #(0x32 << 1)]

#else
    #error "Invalid DRAM Setting"

#endif

    DELAYUS     100

    ldr         r0, =0x0004
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0x0114
    str         r0, [r1, #(0x2c << 1)]


#if (ENABLE_ONEBIN_ENABLE == 1)
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

#if !defined(CONFIG_MIU2_DRAM_NONE)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162100 << 1))
    ldr         r0, =0x0001
    str         r0, [r1, #(0x78 << 1)]
    ldr         r0, =0x1000
    str         r0, [r1, #(0x24 << 1)]
    ldr         r0, =0x0000
    str         r0, [r1, #(0x24 << 1)]

#if defined(CONFIG_MIU2_DDR3_1666)
    #error "Not support DDR3-1666"

#elif defined(CONFIG_MIU2_DDR3_1866)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162100 << 1))
    ldr         r0, =0x0402
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0xa229
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x001d
    str         r0, [r1, #(0x32 << 1)]

#elif defined(CONFIG_MIU2_DDR3_2133)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162100 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x34e6
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x001a
    str         r0, [r1, #(0x32 << 1)]

#elif defined(CONFIG_MIU2_DDR4_2400)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162100 << 1))
    ldr         r0, =0x0400
    str         r0, [r1, #(0x36 << 1)]
    ldr         r0, =0x2004
    str         r0, [r1, #(0x34 << 1)]
    ldr         r0, =0x0a37
    str         r0, [r1, #(0x30 << 1)]
    ldr         r0, =0x0017
    str         r0, [r1, #(0x32 << 1)]

#else
    #error "Invalid DRAM Setting"

#endif

    DELAYUS     100

    ldr         r0, =0x0004
    str         r0, [r1, #(0x22 << 1)]
    ldr         r0, =0x0114
    str         r0, [r1, #(0x2c << 1)]
#if (ENABLE_ONEBIN_ENABLE == 1)
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =MIUBuffer
    ldr         r2, =MIU2
    bl          drv_LoadTbl
    cmp         r0, #1
    beq         SET_BUFFER_ADDRESS_MIU2
    @DPUTK_STR    MIU0_LoadTbl_FAIL
DEAD_END_MIU2:
    b           DEAD_END_MIU2
SET_BUFFER_ADDRESS_MIU2:
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
    ldr         r0, =MIU2_DDR_Init
#endif
    bl          MDrv_WriteRegTbl32

#endif /* CONFIG_MIU2_DRAM_NONE */


    bl          Release_Sw_Reset

    DELAYUS     200                                         @//delay 200us

#if !defined(CONFIG_MIU0_DRAM_NONE)
    @//MIU0 INIT Cycle
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0x0000                 @//hold miu_0
    str         r0, [r1]
    ldr         r0, =0x0008                 @//DRAM reset
    str         r0, [r1]
    ldr         r0, =0x000c                 @//DRAM reset+CS
    str         r0, [r1]

    DELAYUS     500                         @//delay 500us

    ldr         r0, =0x000e                 @//DRAM reset+CS+CKE
    str         r0, [r1]

    DELAYUS     500                         @//delay 500us

    ldr         r0, =0x005f                 @//DRAM reset+CS+CKE+auto cycle+ODE
    str         r0, [r1]

CheckMiu0Init:
    ldr         r0, [r1]
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         CheckMiu0Init

#if defined(CONFIG_MIU0_DDR4_2400)
#if 1 //!defined(CONFIG_ENABLE_AUTO_DQS)
    // This script is for MIU0 DDR4 VrefDQ change (begin)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldrb        r0, [r1]
    orr         r0, #0x20
    strb        r0, [r1]                        //reg_auto_ref_off = 1
    ldrb        r0, [r1, #((0x06 << 1) + 1)]
    orr         r0, #0x10
    strb        r0, [r1, #((0x06 << 1) + 1)]    //reg_cke_always_on = 1

    // Set MRx[15:8] = access MR6
    ldr         r0, =0xc8
    strb        r0, [r1, #((0x18 << 1) + 1)]
    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    strb        r0, [r1, #((0x00 << 1) + 1)]    //reg_single_cmd    = 0 (MRW)


    @//Set MRx[6:0] = Choose VrefDQ value & Range
    @//Note:
    @//orr  r0, #value(The value change on dummy register 0x103382(File:MIU_MST098D_10AVPC_MASERATI.h)
    ldrb        r0, [r1, #(0x18 << 1)]
    bic         r0, #0x7f
    RREG        r5, (MS_BUS_RIU_ADDR + (0x103382 << 1))
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

    ldrb        r0, [r1]
    bic         r0, #0x20
    strb        r0, [r1]                        // reg_auto_ref_off = 0
#endif /*CONFIG_ENABLE_AUTO_DQS*/
#endif /* CONFIG_MIU0_DDR4_2400 */
#endif /* CONFIG_MIU0_DRAM_NONE */

#if !defined(CONFIG_MIU1_DRAM_NONE)
    @//MIU1 INIT Cycle
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r0, =0x0000                 @//hold miu_1
    str         r0, [r1]
    ldr         r0, =0x0008                 @//DRAM reset
    str         r0, [r1]
    ldr         r0, =0x000c                 @//DRAM reset+CS
    str         r0, [r1]

    DELAYUS     500                         @//delay 500us

    ldr         r0, =0x000e                 @//DRAM reset+CS+CKE
    str         r0, [r1]

    DELAYUS     500                         @//delay 500us

    ldr         r0, =0x005f                 @//DRAM reset+CS+CKE+auto cycle+ODE
    str         r0, [r1]

CheckMiu1Init:
    ldr         r0, [r1]
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         CheckMiu1Init

#if defined(CONFIG_MIU1_DDR4_2400)
#if 1 //!defined(CONFIG_ENABLE_AUTO_DQS)
    // This script is for MIU0 DDR4 VrefDQ change (begin)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldrb        r0, [r1]
    orr         r0, #0x20
    strb        r0, [r1]                        //reg_auto_ref_off = 1
    ldrb        r0, [r1, #((0x06 << 1) + 1)]
    orr         r0, #0x10
    strb        r0, [r1, #((0x06 << 1) + 1)]    //reg_cke_always_on = 1

    // Set MRx[15:8] = access MR6
    ldr         r0, =0xc8
    strb        r0, [r1, #((0x18 << 1) + 1)]
    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    strb        r0, [r1, #((0x00 << 1) + 1)]    //reg_single_cmd    = 0 (MRW)

    @// Set MRx[6:0] = Choose VrefDQ value & Range
    @//Note:
    @//orr  r0, #value(The value change on dummy register 0x103384(File:MIU_MST098D_10AVPC_MASERATI.h)
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

    ldrb        r0, [r1]
    bic         r0, #0x20
    strb        r0, [r1]                        // reg_auto_ref_off = 0
#endif /*CONFIG_ENABLE_AUTO_DQS*/
#endif /* CONFIG_MIU1_DDR4_2400 */
#endif /* CONFIG_MIU1_DRAM_NONE */

#if !defined(CONFIG_MIU2_DRAM_NONE)
    @//MIU2 INIT Cycle
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162000 << 1))
    ldr         r0, =0x0000                 @//hold miu_2
    str         r0, [r1]
    ldr         r0, =0x0008                 @//DRAM reset
    str         r0, [r1]
    ldr         r0, =0x000c                 @//DRAM reset+CS
    str         r0, [r1]

    DELAYUS     500                         @//delay 500us

    ldr         r0, =0x000e                 @//DRAM reset+CS+CKE
    str         r0, [r1]

    DELAYUS     500                         @//delay 500us

    ldr         r0, =0x005f                 @//DRAM reset+CS+CKE+auto cycle+ODE
    str         r0, [r1]

CheckMiu2Init:
    ldr         r0, [r1]
    and         r0, r0, #0x8000
    cmp         r0, #0
    beq         CheckMiu2Init

#if defined(CONFIG_MIU2_DDR4_2400)
#if 1 //!defined(CONFIG_ENABLE_AUTO_DQS)
    // This script is for MIU0 DDR4 VrefDQ change (begin)
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162000 << 1))
    ldrb        r0, [r1]
    orr         r0, #0x20
    strb        r0, [r1]                        //reg_auto_ref_off = 1
    ldrb        r0, [r1, #((0x06 << 1) + 1)]
    orr         r0, #0x10
    strb        r0, [r1, #((0x06 << 1) + 1)]    //reg_cke_always_on = 1

    // Set MRx[15:8] = access MR6
    ldr         r0, =0xc8
    strb        r0, [r1, #((0x18 << 1) + 1)]
    ldrb        r0, [r1, #((0x00 << 1) + 1)]
    bic         r0, #0x1e
    strb        r0, [r1, #((0x00 << 1) + 1)]    //reg_single_cmd    = 0 (MRW)

    @// Set MRx[6:0] = Choose VrefDQ value & Range
    @//Note:
    @//orr  r0, #value(The value change on dummy register 0x103386(File:MIU_MST098D_10AVPC_MASERATI.h)
    ldrb        r0, [r1, #(0x18 << 1)]
    bic         r0, #0x7f
    RREG        r5, (MS_BUS_RIU_ADDR + (0x103386 << 1))
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

    ldrb        r0, [r1]
    bic         r0, #0x20
    strb        r0, [r1]                        // reg_auto_ref_off = 0
#endif /*CONFIG_ENABLE_AUTO_DQS*/
#endif /* CONFIG_MIU2_DDR4_2400 */
#endif /* CONFIG_MIU2_DRAM_NONE */

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

#if !defined(CONFIG_MIU2_DRAM_NONE)
    @//Scan MIU2 DQS
    ldr         r0, =2

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

#if !defined(CONFIG_MIU2_DRAM_NONE)
    @//MIU2 BIST test
    bl          Miu2_BistTest
#endif

    ldr         r0, =MIU_PostInit
    bl          MDrv_WriteRegTbl32

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
    //MIU2
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x162000 << 1))
    ldr         r0, =0x8c00
    str         r0, [r1, #(0x1e << 1)]
    //MIU0
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r0, =0x8c00
    str         r0, [r1, #(0x1e << 1)]
    bx          lr

@//-------------------------------------------------------------------------------------------------
@// MiuBistTest
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
#if !defined(CONFIG_MIU2_DRAM_NONE)
    @//MIU2 BIST test
    .global     Miu2_BistTest
Miu2_BistTest:

    DPUTK_STR   MIU_MESSAGE_BIST2

    WREG        (MS_BUS_RIU_ADDR + (0x162306 << 1)), 0x7FFF @//unmask MIU1_group_4 bit15
    ldr         r12, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r11, =0x0000
    str         r11, [r12, #(0xe0 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012e0 << 1)), 0x0000   @//1012e0 //reset bist test
    ldr         r11, =0x0000
    str         r11, [r12, #(0xf8 << 1)]    @//WREG        (MS_BUS_RIU_ADDR + (0x1012f8 << 1)), 0x8000   @//1012f8 bit_15=1 choice to MIU_1
    WREG        (MS_BUS_RIU_ADDR + (0x1620f8 << 1)), 0x8000                                              @//1620f8 bit_15=0 choice to MIU_1
    b           Miu_BistTest
#endif


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

    .size       BootRom_InitMiu,.-BootRom_InitMiu
