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

#ifdef CONFIG_ONEBIN_ENABLE
#include "drvLoadTbl.h"
#endif

    .text

BOOT_MESSAGE_AC_ON: .short  '\r', '\n', 'A', 'C', '_', 'O', 'N', '\r', '\n', 0x00
BOOT_MESSAGE_DC_ON: .short  '\r', '\n', 'D', 'C', '_', 'O', 'N', '\r', '\n', 0x00

    .align      4

@//-------------------------------------------------------------------------------------------------
@// BootRom_Entry:
@// @param  None
@// @return None
@// @note   Entry point of Boot code
@//-------------------------------------------------------------------------------------------------
    .globl      BootRom_Entry
BootRom_Entry:

    DBGREG      0x0001

#ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN
    bl          BootRom_PLLRecovery
#endif /*CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN*/

#ifndef CONFIG_MSTAR_TITANIA_BD_FPGA

    @//Configure PAD top for MUX and GPIO (e.g. DDR power)
    ldr         r0, =padInitTbl_PreInit
    add         r0, r0, #10
    bl          MDrv_WriteRegMaskTbl

#ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN

#ifdef CONFIG_MSTAR_VID_ENABLE
    @//Initial Voltage Identification
    bl          BootRom_InitVid
#endif  /*CONFIG_MSTAR_VID_ENABLE*/

    @//Initial releated settings of PLL
    bl          BootRom_InitPll

    @//Initial related settings of Clock
    bl          BootRom_InitClock

    @//DBGREG      0x0002

#endif /*CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN*/
#endif /*CONFIG_MSTAR_TITANIA_BD_FPGA*/
    @//Initial related settings of UART
    bl          BootRom_InitPeripheral

#ifdef CONFIG_MSTAR_DVFS_ENABLE
    @//Enable Dynamic Voltage / Frequency Selection (DVFS)
    bl          BootRom_InitDvfs
#endif

#if  !defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && \
     !defined(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51)

    @//Initial related settings of MIU
    RREG        r0, (MS_BUS_RIU_ADDR + (0x0E70 << 1))   @//Disable Divisor Latch Access
    bic         r0, r0, #0x00FF
    bic         r0, r0, #0x0F00
    ldr         r1, =0xF000
    cmp         r0, r1
    beq         BootRom_Str_AcFlow

#ifdef CONFIG_MSTAR_STR_ENABLE
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    ldr         r0, =_ld_program_reload_addr
    ldr         r1, =_ld_miu_str_flow_flash_start
    ldr         r2, =0x1C00
    bl          drvNAND_LoadBLOffset
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    ldr         r0, =_ld_program_reload_addr
    ldr         r1, =CONFIG_EMMC_HASH1_STR_SIZE
    ldr         r2, =(CONFIG_EMMC_HASH0_SIZE + CONFIG_EMMC_HASH1_SIZE + CONFIG_EMMC_HASH1_MIU_SIZE)
    mov         r2, r2, lsr #9
    bl          eMMC_LoadImages
#endif

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    ldr         r0, =_ld_program_reload_addr            @//Set the starting address of HASH for Authentication
    ldr         r1, =0x1A00                             @//Set the HASH size for Authentication
    ldr         r2, =(_ld_program_reload_addr + 0x1B00) @//Set the starting address of SIG for Authentication
    ldr         r3, =0x1
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRom_AuthenticationFail
#endif

    bl          BootRom_InitMiu_STR

#ifdef CONFIG_MSTAR_SELF_STR_TEST_ENABLE
    bl          Miu0_BistTest_Str
    bl          Miu1_BistTest_Str

Self_Str_Test_Level:
    b           Self_Str_Test_Level
#endif

#else
    DPUTK_STR   BOOT_MESSAGE_DC_ON
    b           BootRom_Str_InitMiu
#endif /* CONFIG_MSTAR_STR_ENABLE */

    b           BootRom_Str_Exit

BootRom_Str_AcFlow:
    DPUTK_STR   BOOT_MESSAGE_AC_ON

BootRom_Str_InitMiu:
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    ldr         r0, =_ld_program_reload_addr
    ldr         r1, =_ld_miu_flow_flash_start
    ldr         r2, =0x1C00
    bl          drvNAND_LoadBLOffset
#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    ldr         r0, =_ld_program_reload_addr
    ldr         r1, =CONFIG_EMMC_HASH1_MIU_SIZE
    ldr         r2, =(CONFIG_EMMC_HASH0_SIZE + CONFIG_EMMC_HASH1_SIZE)
    mov         r2, r2, lsr #9
    bl          eMMC_LoadImages
#endif

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    ldr         r0, =_ld_program_reload_addr            @//Set the starting address of HASH for Authentication
    ldr         r1, =0x1A00                             @//Set the HASH size for Authentication
    ldr         r2, =_ld_program_reload_addr + 0x1B00   @//Set the starting address of SIG for Authentication
    ldr         r3, =0x1
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRom_AuthenticationFail
#endif

    bl          BootRom_InitMiu

#if defined(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51)
    WREG_B      (MS_BUS_RIU_ADDR + (0xe68 << 1) + 1), 0x55
#endif

BootRom_Str_Exit:
#endif /*CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51*/

    bl          BootRom_SetL3Clock

@//================================================================================================
    @//Initial related settings of cache
    bl          BootRom_InitCache

@//================================================================================================
#ifdef ADAPTIVE
#ifdef CONFIG_MSTAR_STR_ENABLE
    @// Check dram size if in AC on flow
    RREG        r0, (MS_BUS_RIU_ADDR + (0x0E70 << 1))   @//Disable Divisor Latch Access
    bic         r0, r0, #0x00FF
    bic         r0, r0, #0x0F00
    ldr         r1, =0xF000
    cmp         r0, r1
    bne         BootRom_MIU_DramSize_DC_On
#endif

#if !defined(CONFIG_MIU0_DRAM_NONE)
    ldr         r0, =(MS_BUS_RIU_ADDR + (0x161500 << 1))
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    bl          Miu_DramSizeCheck

    ldr         r0, =(MS_BUS_RIU_ADDR + (0x000E00 << 1))
    ldr         r2, [r0, #(0xa8 << 1)]
    bic         r2, #0x0f
    orr         r2, r3
    str         r2, [r0, #(0xa8 << 1)]
#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    ldr         r0, =(MS_BUS_RIU_ADDR + (0x162200 << 1))
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    bl          Miu_DramSizeCheck

    ldr         r0, =(MS_BUS_RIU_ADDR + (0x000E00 << 1))
    ldr         r2, [r0, #(0xa8 << 1)]
    bic         r2, #0xf0
    orr         r2, r3, lsl #4
    str         r2, [r0, #(0xa8 << 1)]
#endif

    b           BootRom_MIU_DramSize_Exit

#ifdef CONFIG_MSTAR_STR_ENABLE
BootRom_MIU_DramSize_DC_On:

#if !defined(CONFIG_MIU0_DRAM_NONE)
    ldr         r0, =(MS_BUS_RIU_ADDR + (0x000E00 << 1))
    ldr         r3, [r0, #(0xa8 << 1)]
    add         r3, #0x0f
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101200 << 1))
    ldr         r2, [r1, #(0xd2 << 1)]
    bic         r2, #0xff00
    orr         r2, r3, lsl #12
    str         r2, [r1, #(0xd2 << 1)]
#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    ldr         r0, =(MS_BUS_RIU_ADDR + (0x000E00 << 1))
    ldr         r3, [r0, #(0xa8 << 1)]
    add         r3, #0xf0
    lsr         r3, #0x04
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100600 << 1))
    ldr         r2, [r1, #(0xd2 << 1)]
    bic         r2, #0xff00
    orr         r2, r3, lsl #12
    str         r2, [r1, #(0xd2 << 1)]
#endif
#endif

BootRom_MIU_DramSize_Exit:
#endif
@//================================================================================================
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =0x8000
    bl          drvNAND_Load2ndBL

#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    #ifdef CONFIG_DOUBLE_MBOOT
    ldr         r0, =_ld_bootram1_start
    ldr         r1, =_ld_bootram1_size
    #else
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =_ld_bootram_size
    #endif

#ifdef CONFIG_BOOTRAM_STORE_IN_MBOOT
    ldr         r2, =0x1002     @//bootram store in mboot.bin's 0x400 offset(0x200400)
    bl          eMMC_ReadData_MIU
#else
    ldr         r2, =CONFIG_EMMC_BOOTRAM_ADDRESS
    mov         r2, r2, lsr #9
    bl          eMMC_LoadImages
#endif

#else

    @//Copy image of bootram to DRAM
    bl          BootRom_LoadBootRam

#endif

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    #ifdef CONFIG_DOUBLE_MBOOT
    ldr         r0, =_ld_bootram1_start                        @//Set the starting address of HASH for Authentication
    ldr         r1, =(_ld_bootram1_size - 0x400)                         @//Set the HASH size for Authentication
    ldr         r2, =(_ld_bootram1_end - 0x100)                @//Set the starting address of SIG for Authentication
    #else
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR                                 @//Set the starting address of HASH for Authentication
    ldr         r1, =(_ld_bootram_size - 0x400)                                                 @//Set the HASH size for Authentication
    ldr         r2, =(_ld_bootram_end - 0x100)                        @//Set the starting address of SIG for Authentication
    #endif
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRom_AuthenticationFail
#endif

@//================================================================================================
#ifdef CONFIG_MSTAR_STR_ENABLE
    bl          BootRom_StrEntry
#endif

@//================================================================================================
    DBGREG      0x0003
    @//Jump to the entry of BOOTRAM
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH) && defined(CONFIG_DOUBLE_MBOOT)
    ldr         pc, =BOOTRAM_Entry_Hash
#else
    ldr         pc, =BOOTRAM_Entry
#endif

BootRom_DummyLoop:
    b           BootRom_DummyLoop

@//-------------------------------------------------------------------------------------------------
@// BootRom_PLLRecovery
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
BootRom_PLLRecovery:
    WREG        (MS_BUS_RIU_ADDR + (0x101E26 << 1)), 0x0200     @//set ejtag padmux
    WREG        (MS_BUS_RIU_ADDR + (0x101882 << 1)), 0x0404     @//Set L3 switch to xtali
    WREG        (MS_BUS_RIU_ADDR + (0x110C72 << 1)), 0x0101     @//MIU_256BUS_PLL power down
    WREG        (MS_BUS_RIU_ADDR + (0x110C62 << 1)), 0x0101     @//MIU_128BUS_PLL power down

    bx          lr

@//================================================================================================
#if !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
@//-------------------------------------------------------------------------------------------------
@// BootRom_LoadBootRam
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
BootRom_LoadBootRam:

    ldr         r10, =_ld_bootrom_end
    ldr         r11, =_ld_bootram_start
    ldr         r12, =_ld_bootram_end
    ldr         r9, =_ld_bootram_start
    sub         r12, r12, r9

_CopyDataLoop:
    ldr         r9, [r10], #4
    str         r9, [r11], #4
    subs        r12, r12, #4
    bne         _CopyDataLoop

    bx          lr
#endif

@//-------------------------------------------------------------------------------------------------
@// BootRom_WriteUartCharactor
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .globl      BootRom_WriteUartCharactor
BootRom_WriteUartCharactor:
    ldr         r12, =(MS_BUS_RIU_ADDR + (0x100900 << 1))
    strb        r0, [r12, #(0x80 << 1)]     @//REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x100980 << 1))
BootRom_WriteUartCharactor_Loop:
    ldr         r0, [r12, #(0x94 << 1)]     @//RREG        r0, (MS_BUS_RIU_ADDR + (0x100994 << 1))
    and         r0, r0, #0x0060
    cmp         r0, #0x0060
    bne         BootRom_WriteUartCharactor_Loop

    bx          lr

@//-------------------------------------------------------------------------------------------------
@// BootRom_WriteUartString
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
    .globl      BootRom_WriteUartString
BootRom_WriteUartString:

    ldrb        r11, [r0], #2
    and         r11, r11, #0xff
    cmp         r11, #0x00
    beq         BootRom_WriteUartString_Exit

    ldr         r12, =(MS_BUS_RIU_ADDR + (0x100900 << 1))
    str         r11, [r12, #(0x80 << 1)]     @//REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x100980 << 1))
BootRom_WriteUartString_Loop:
    ldr         r11, [r12, #(0x94 << 1)]     @//RREG        r0, (MS_BUS_RIU_ADDR + (0x100994 << 1))
    and         r11, r11, #0x60
    cmp         r11, #0x60
    bne         BootRom_WriteUartString_Loop
    b           BootRom_WriteUartString

BootRom_WriteUartString_Exit:

    bx          lr

@//-------------------------------------------------------------------------------------------------
@// BootRom_SetL3Clock
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
BootRom_SetL3Clock:

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101800 << 1))
    ldr         r0, =0x84
    strb        r0, [r1, #(0x82 << 1)]   //Select MIU2x Clock

    bx          lr

@//-------------------------------------------------------------------------------------------------
@// BootRom_StrEntry
@// @param  None
@// @return None
@// @note   None
@//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_MSTAR_STR_ENABLE
    .global     BootRom_StrEntry
BootRom_StrEntry:

    mov         r13, lr

.if (MSTAR_STR_SBOOT_RESUME == 1)
    bl          read_wkup_pmu
    bl          put_reg_val

    is_slept
    bne         1f

    set_wakeup
    b           2f

1:
    eor         r0, r0, r0
    bl          write_wkup_pmu

2:
    is_wakeup
    bne         3f
    PUTK        'W'
    PUTK        'K'
    PUTK        'P'
    bl          read_wkup_pmu

    bic         r0, r0, #WAKEUP_FLAG_MASK
    mov         r0, r0, lsl #16             @// wake up info addr
    ldmia       r0, {r1-r2}
    ldr         r0, =MSTAR_SLEEP_MAGIC
    cmp         r1, r0
    bne         3f

    PUTK        'J'
    cmp         r2, #0
    beq         3f

    PUTK        'M'
    mov         pc, r2                      @// jump to wakup addr
3:
.endif

    mov         lr, r13
    bx          lr

#endif

@//================================================================================================
#ifdef CONFIG_MSTAR_STR_ENABLE
#if 0   //calculate CRC for STR debug
MEM_REGION1_ST      = 0x00200000
MEM_REGION1_SZ      = 0x09F00000
@//MEM_REGION2_ST   = 0x10E00000    @//MIU0
@//MEM_REGION2_SZ   = 0x0C200000    @//MIU0

MEM_REGION2_ST      = 0x01800000    @//MIU1
MEM_REGION2_SZ      = 0x0E800000    @//MIU1

@//Input: r0-region address   r1-resion size
@//Output: r0-CRC value
BDMABank = 0x1F201200
BDMACRC_Calc_MIU:
    ldr         r2, =BDMABank

    mov         r3, #0
    str         r3, [r2, #(0x0c<<2)]
    str         r3, [r2, #(0x0d<<2)]
@// mov         r3, #0x0340
@// str         r3, [r2, #(0x02<<2)]

    str         r0, [r2, #(0x04<<2)]
    str         r0, [r2, #(0x06<<2)]
    lsr         r0, r0, #16
    str         r0, [r2, #(0x05<<2)]
    str         r0, [r2, #(0x07<<2)]

    str         r1, [r2, #(0x08<<2)]
    lsr         r1, r1, #16
    str         r1, [r2, #(0x09<<2)]

    ldr         r3, =0x1DB7
    str         r3, [r2, #(0x0a<<2)]
    ldr         r3, =0x04c1
    str         r3, [r2, #(0x0b<<2)]

    ldr         r3, [r2]
    orr         r3, r3, #1
    str         r3, [r2]

BDMACRC_Calc_agn:
    ldr         r3, [r2, #(0x01<<2)]
    tst         r3, #0x08
    beq         BDMACRC_Calc_agn

    ldr         r3, [r2, #(0x0c<<2)]
    ldr         r0, [r2, #(0x0d<<2)]
    orr         r0, r3, r0, LSL #16

    mov         pc, lr

 SBootMemCRCCheck:
    mov         r8, lr
    PUTK        'S'
    PUTK        'B'
    PUTK        'O'
    PUTK        'O'
    PUTK        'T'
    PUTK        ' '
    PUTK        'C'
    PUTK        'R'
    PUTK        'C'
    PUTK        ':'

    PUTK        '\n'
    PUTK        '\r'

    PUTK        '1'
    PUTK        '.'
    ldr         r0, =MEM_REGION1_ST
    ldr         r1, =MEM_REGION1_SZ
    WREG        (MS_BUS_RIU_ADDR + (0x100904 << 1)), 0x0340
    bl          BDMACRC_Calc_MIU
    bl          put_reg_val
    PUTK        '\n'
    PUTK        '\r'

    PUTK        '2'
    PUTK        '.'
    ldr         r0, =MEM_REGION2_ST
    ldr         r1, =MEM_REGION2_SZ
    WREG        (MS_BUS_RIU_ADDR + (0x100904 << 1)), 0x0341
    bl          BDMACRC_Calc_MIU
    bl          put_reg_val
    PUTK        '\n'
    PUTK        '\r'
    mov         pc, r8
    .ltorg
#endif

.if (MSTAR_STR_SBOOT_RESUME == 1)
read_wkup_pmu:
    ldr         r2, =PMU_WAKEUP_ADDR_REGL
    ldr         r0, [r2]

    ldr         r2, =PMU_WAKEUP_ADDR_REGH
    ldr         r1, [r2]

    and         r0, r0, #PMU_WAKEUP_ADDR_LMASK
    and         r1, r1, #PMU_WAKEUP_ADDR_HMASK
    orr         r0, r0, r1
    mov         pc, lr
    .ltorg

write_wkup_pmu:
    ldr         r3, =PMU_WAKEUP_ADDR_REGL
    ldr         r1, [r3]
    bic         r1, #PMU_WAKEUP_ADDR_LMASK
    and         r2, r0, #PMU_WAKEUP_ADDR_LMASK
    orr         r1, r1, r2
    str         r1, [r3]

    ldr         r3, =PMU_WAKEUP_ADDR_REGH
    ldr         r1, [r3]
    bic         r1, #PMU_WAKEUP_ADDR_HMASK
    and         r2, r0, #PMU_WAKEUP_ADDR_HMASK
    orr         r1, r1, r2
    str         r1, [r3]
    mov         pc, lr
    .ltorg
.endif

text_tbl: .short '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'

@// r9-r12 destroyed
put_reg_val:
    mov         r9, #32
    mov         r11, #0x0F
40:
    sub         r9, #4
    and         r10, r11, r0, LSR r9
    ldr         r12, =text_tbl
    add         r12, r12, r10, lsl #1
    ldr         r10,  [r12]
    mov         r12, #0x1F000000
    orr         r12, r12, #0x00200000
    orr         r12, r12, #0x1300
    strb        r10,[r12]
    cmp         r9, #0
    bgt         40b

    mov         r10, #'-'
    strb        r10,[r12]

    mov         pc, lr
    .ltorg
#endif

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
BootRom_AuthenticationFail:
    DPUTK       '\n'
    DPUTK       '\r'
    DPUTK       'A'
    DPUTK       'U'
    DPUTK       'T'
    DPUTK       'H'
    DPUTK       '_'
    DPUTK       'F'
    DPUTK       'A'
    DPUTK       'I'
    DPUTK       'L'

BootRom_AuthenticationFailLoop:
    b           BootRom_AuthenticationFailLoop
#endif

@//-------------------------------------------------------------------------------------------------
@// Miu_DramSizeCheck
@// @param
@// @return None
@// @note
@//-------------------------------------------------------------------------------------------------
#ifdef ADAPTIVE
Miu_DramSizeCheck:

    @//Disable Boundry Check
    ldr         r2, =0x0
    str         r2, [r0, #(0xDE << 1)]

    ldr         r2, =0xA0000000
    ldr         r3, =0x0
    str         r3, [r2]

Miu_DramSize128M:
    ldr         r4, =0xA8000000
    ldr         r3, =0x7
    str         r3, [r4]
    ldr         r5, [r2]
    cmp         r3, r5
    beq         Miu_DramSizeSet

Miu_DramSize256M:
    ldr         r4, =0xB0000000
    ldr         r3, =0x8
    str         r3, [r4]
    ldr         r5, [r2]
    cmp         r3, r5
    beq         Miu_DramSizeSet

Miu_DramSize512M:
    ldr         r4, =0xC0000000
    ldr         r3, =0x9
    str         r3, [r4]
    ldr         r5, [r2]
    cmp         r3, r5
    beq         Miu_DramSizeSet

Miu_DramSize1024M:
    ldr         r3, =0xA

Miu_DramSizeSet:

    ldr         r2, [r1, #(0xd2 << 1)]
    bic         r2, #0xf000
    orr         r2, r3, lsl #12
    str         r2, [r1, #(0xd2 << 1)]

    @//Enable Boundry Check
    ldr         r2, =0x3
    str         r2, [r0, #(0xDE << 1)]

    bx          lr
#endif
