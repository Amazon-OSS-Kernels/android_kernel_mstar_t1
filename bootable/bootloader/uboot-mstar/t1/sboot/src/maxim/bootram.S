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

#if defined(CONFIG_SBOOT_TO_KERNEL) && defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
#define LINUX_KERNEL_FLASH_ADDRESS  0x14010000
#define LINUX_KERNEL_DRAM_ADDRESS   0x20208000
#define LINUX_KERNEL_ENTRY          0x20208000
#define LINUX_KERNEL_SIZE           0x00800000
#elif defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
#define LINUX_KERNEL_FLASH_ADDRESS  0x14020000
#define LINUX_KERNEL_DRAM_ADDRESS   0x20400000
#define LINUX_KERNEL_ENTRY          0x20408000
#define LINUX_KERNEL_SIZE           0x00200000
#elif defined(CONFIG_SBOOT_TO_KERNEL)
#define LINUX_KERNEL_FLASH_ADDRESS  0x14010000
#define LINUX_KERNEL_DRAM_ADDRESS   0x20208000
#define LINUX_KERNEL_ENTRY          0x20208000
#define LINUX_KERNEL_SIZE           0x00800000
#endif

@//-------------------------------------------------------------------------------------------------
@// vector:
@//-------------------------------------------------------------------------------------------------
vector:
    @//Synchronous EL1t
    .align  7
    .word 0x14000000
    @//IRQ EL3t
    .align  7
    .word 0x14000000
    @//FIQ EL3t
    .align  7
    .word 0x14000000
    @//Error EL3t
    .align  7
    .word 0x14000000

    @//Synchronous EL3h
    .align  7
    .word 0x14000000
    @//IRQ EL3h
    .align  7
    .word 0x14000000
    @//FIQ EL3h
    .align  7
    .word 0x14000000
    @//Error EL3h
    .align  7
    .word 0x14000000

    @//Synchronous 64-bit EL2
    .align  7
    .word 0x14000000
    @//IRQ 64-bit EL2
    .align  7
    .word 0x14000000
    @//FIQ 64-bit EL2
    .align  7
    .word 0x14000000
    @//Error 64-bit EL2
    .align  7
    .word 0x14000000

    @//Synchronous 32-bit EL2
    .align  7
    @//switch to 64 bit mode
#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
#if 0
	.word 0xd51e4021 //msr	elr_el3, x1
	.word 0xd53e4001 //mrs	x1, spsr_el3
	.word 0x9278dc21 //and	x1, x1, #0xffffffffffffff00
	.word 0xb27d0021 //orr	x1, x1, #0x8
	.word 0xb27e0021 //orr	x1, x1, #0x4
	.word 0xb2400021 //orr	x1, x1, #0x1
	.word 0xd51e4001 //msr	spsr_el3, x1
	.word 0xd2808801 //mov	x1, #0x440                 	// #1088
	.word 0xd51e1101 //msr	scr_el3, x1
	.word 0xd69f03e0 //eret
#else
	.word 0xd29ff985 	//mov	x5, #0xffcc                	// #65484
	.word 0xeb05001f 	//cmp	x0, x5
	.word 0x54000121 	//b.ne	2c <FirmwarePath2>
	.word 0xd51e4021 	//msr	elr_el3, x1
	.word 0xd53e4001 	//mrs	x1, spsr_el3
	.word 0x9278dc21 	//and	x1, x1, #0xffffffffffffff00
	.word 0xb27c0021 	//orr	x1, x1, #0x10
	.word 0xb2400421 	//orr	x1, x1, #0x3
	.word 0xd2808821 	//mov	x1, #0x441                 	// #1089
	.word 0xd51e1101 	//msr	scr_el3, x1
	.word 0xd69f03e0 	//eret
	.word 0xd51e4021 	//msr	elr_el3, x1
	.word 0xd53e4001 	//mrs	x1, spsr_el3
	.word 0x9278dc21 	//and	x1, x1, #0xffffffffffffff00
	.word 0xb27d0021 	//orr	x1, x1, #0x8
	.word 0xb27e0021 	//orr	x1, x1, #0x4
	.word 0xb2400021 	//orr	x1, x1, #0x1
	.word 0xd51e4001 	//msr	spsr_el3, x1
	.word 0xd2808801 	//mov	x1, #0x440                 	// #1088
	.word 0xd51e1101 	//msr	scr_el3, x1
	.word 0xd69f03e0 	//eret
#endif
#else
    .word 0xd2808820 //mov x0, #0x441
    .word 0xd51e1100 //msr scr_el3, x0
    .word 0xd2a4051e //mov x30, #0x20280000
    .word 0xd51e403e //msr elr_el3, x30
    .word 0xd53e4001 //mrs x1, spsr_el3
    .word 0x927be821 //and x1, x1, #0xffffffffffffffe0
    .word 0xb2400021 //orr x1, x1, #0x1
    .word 0xb27d0021 //orr x1, x1, #0x8
    .word 0xd51e4001 //msr spsr_el3, x1
    .word 0xd69f03e0 //eret
#endif
    @//IRQ 32-bit EL2
    .align  7
    .word 0x14000000
    @//FIQ 32-bit EL2
    .align  7
    .word 0x14000000
    @//Error 32-bit EL2
    .align  7
    .word 0x14000000

@//-------------------------------------------------------------------------------------------------
@// BOOTRAM_Entry:
@// @param  None
@// @return None
@// @note   Entry point of Boot code
@//-------------------------------------------------------------------------------------------------
    .globl      BOOTRAM_Entry
BOOTRAM_Entry:

    ldr         r0, =vector                 @//Starting Address of Linux Kernel in SPI Flash
    ldr         r1, =0x20000000             @//Dest Address of Linux Kernel in DRAM
    ldr         r2, =0x80000                @//Image Size of Linux Kernel

_CopyImageLoop1:

    ldr         r3, [r0], #0x04
    str         r3, [r1], #0x04
    subs        r2, r2, #0x04
    ldr         r3, =0x0007FFFF
    and         r4, r2, r3
    cmp         r4, #0
    bgt         _CopyImageLoop1
    PUTK        '#'
    cmp         r2, #0
    bgt         _CopyImageLoop1


    DPUTK       '_'

@//Configure PAD top for MUX and GPIO (e.g. DDR power)
    ldr         r0, =padInitTbl
    add         r0, r0, #10
    bl          MDrv_WriteRegMaskTbl

#ifdef CONFIG_MSTAR_EMMC_LDO_POWER_OFF
    bl          Bootram_PowerSaving
#endif

#ifdef CONFIG_MSTAR_WINDERMERE
    //Init Windermere
    bl          BOOTRAM_InitWindermere
#endif

// hml patch
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x2f00 << 1))
    ldr         r0, [r1, #(0x40 << 1)]
    bic         r0, r0, #0x18
    str         r0, [r1, #(0x40 << 1)]


@//================================================================================================
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) || \
    defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)

    #if defined(CONFIG_ARMv8_32BIT_KERNEL)
        ldr         r0, =Bootram_SecondaryCore
    #elif defined(CONFIG_ARMv8_64BIT_KERNEL)
        ldr         r0, =Bootram_SecondaryCore_Prepare
    #endif

    ldr         r1, =0xFFFF
    mov         r2, r0, lsr #16
    and         r2, r2, r1
    REG_C2M     r2, BOOT_INFO_REGISTER_CORE1_START_ADDR_HIGH

    and         r2, r0, r1
    REG_C2M     r2, BOOT_INFO_REGISTER_CORE1_START_ADDR_LOW

#endif


    @//Initial Global Interrupt Interface

    #if defined(CONFIG_ARMv8_32BIT_KERNEL)
        WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0080), 0x00000000
    #elif defined(CONFIG_ARMv8_64BIT_KERNEL)
        WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0080), 0x3fffffff
    #endif

    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0004), 0x000000f0
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0180), 0xffff0000
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0100), 0x0000ffff

    @//Initial Internal Interrupt Controller
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0000), 0x00000003
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0000), 0x00000003

    //Poke Core1/2/3 using software interrupt
    ldr         r1, =REG_GIC_DIST_BASE_ADDRESS + 0x80       // GICD_IGROUPR
    #if defined(CONFIG_ARMv8_64BIT_KERNEL)
        ldr         r0, =~0                                 // Grp1 interrupts
        ldr         r2, =~(3 << 30)                         // mboot will be secure_group, so bit_30 need to be 0
    #elif defined(CONFIG_ARMv8_32BIT_KERNEL)
        ldr         r0, =0                                  // Grp1 interrupts
        ldr         r2, =0                                  // mboot will be secure_group, so bit_30 need to be 0
    #endif
        and         r0, r0, r2
        str         r0, [r1]
        str         r0, [r1],#4

    #ifdef CONFIG_ARM_GIC_SPI_MODE

    ldr         r2, =0xa
_GIC_LOOP:
    str         r0, [r1],#0x4
    sub         r2,r2,#0x1
    cmp         r2,#0
    bne         _GIC_LOOP

    #endif


#if 0
    ldr         r1, =REG_GIC_CPU_BASE_ADDRESS               // GICC_CTLR
    ldr         r0, =0x3                                    // EnableGrp0 | EnableGrp1
    str         r0, [r1]

    ldr         r0, =(1 << 7)                     // allow NS access to GICC_PMR
    str         r0, [r1, #4]                    // GICC_PMR

	sev
#endif

    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0F00), 0x000e0001

@// Clear data in BSS section

    ldr         r0, =_ld_bootbss_start
    ldr         r1, =_ld_bootbss_end
    mov         r2, #0

    cmp         r0, r1
    beq         _ClearBssExit

_ClearBssLoop:
    cmp         r0, r1
    str         r2, [r0], #0x04
    bne         _ClearBssLoop

_ClearBssExit:
@//================================================================================================
@// Initialize system stack pointer for unzip subroutine
#ifdef CONFIG_MSTAR_STR_MINISIZE
    ldr         r13, =CONFIG_KERNEL_START_ADDRESS
#else
    ldr         r13, =ADDR_STACK_POINTER
#endif

#if defined(CONFIG_MSTAR_TITANIA_BD_FPGA) || defined(CONFIG_SBOOT_TO_KERNEL)
@//================================================================================================
@//Copy Image to DRAM

    ldr         r0, =LINUX_KERNEL_FLASH_ADDRESS     @//Starting Address of Linux Kernel in SPI Flash
    ldr         r1, =LINUX_KERNEL_DRAM_ADDRESS      @//Dest Address of Linux Kernel in DRAM
    ldr         r2, =LINUX_KERNEL_SIZE              @//Image Size of Linux Kernel

_CopyImageLoop:

    ldr         r3, [r0], #0x04
    str         r3, [r1], #0x04

    subs        r2, r2, #0x04

    ldr         r3, =0x0007FFFF
    and         r4, r2, r3
    cmp         r4, #0
    bgt         _CopyImageLoop

    PUTK        '#'

    cmp         r2, #0
    bgt         _CopyImageLoop

@//================================================================================================
@//Jump to Kernel Entry of Linux

    PUTK        '\n'
    PUTK        '\r'

    ldr         pc, =LINUX_KERNEL_ENTRY

#endif


@//================================================================================================
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    @//LOAD CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    bl          drvNAND_LoaduBoot

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE) && defined(CONFIG_SECURITY_BOOT)

    @//Authenticate CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x2F0
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x300
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRam_AuthenticationFail

BootRam_AuthChunkHeader_Exit:
#endif

    @// LOAD UBOOT
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r3, [r0, #0]    @// UBOOT_ROM_START in MBOOT PARTITION
    ldr         r1, [r0, #4]    @// UBOOT_RAM_START
    ldr         r2, [r0, #8]    @// UBOOT_RAM_END
    ldr         r5, [r0, #4]    @// UBOOT_RAM_START
    ldr         r6, [r0, #8]    @// UBOOT_RAM_END
    sub         r1, r2, r1      @// UBOOT_SIZE = UBOOT_RAM_END - UBOOT_RAM_START
    add         r1, r1, r3      @// LOAD SIZE
    add         r1, r1 , #0x110  @// SECURITY INFO Size 0x110
    sub         r0, r0, r3
    bl          drvNAND_LoaduBoot

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE) && defined(CONFIG_SECURITY_BOOT)

    @//Authenticate U-BOOT
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    sub         r1, r6, r5
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x10
    add         r2, r2, r1
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRam_AuthenticationFail

BootRam_AuthUBoot_Exit:
#endif

#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    @//LOAD CHUNK HEADER
    bl          UartHandShake
    cmp         r0, #1
    beq         CUSTOMER_UBOOT_EXIT

    PUTK        '\r'
    PUTK        'K'
    PUTK        'O'
    PUTK        '\r'

    bl          UartWaitRXSTop
    PUTK        '\r'
    PUTK        'S'
    PUTK        '\r'

    bl          UartLoadContent
    cmp         r0, #1
    beq         BootRam_AuthenticationFail
    PUTK        '\r'
    DPUTK       'A'
    DPUTK       'U'
    DPUTK       'T'
    DPUTK       'H'
    DPUTK       '_'
    DPUTK       'O'
    DPUTK       'K'
    PUTK        '\r'

    ldr         r0, =0xFFCC
    ldr         r1, =CONFIG_UBOOT_LOADADDR
    ldr         r3, =0x16001080
    ldr         r2, [r3]
    ldr         r4, =(0x3 << 30)
    orr         r2, r2, r4
    str         r2, [r3]
    ldr         r3, =0x16001000
    mov         r2, #3
    str         r2, [r3]
    ldr         r3, =0x16002000
    mov         r2, #3
    str         r2, [r3]
    ldr         r3, =0x16002004
    mov         r2, #0xf8
    str         r2, [r3]
    .arch_extension sec
    smc         #0

CUSTOMER_UBOOT_EXIT:
#ifdef CONFIG_DOUBLE_MBOOT
    RREG        r0, (MS_BUS_RIU_ADDR + (0x103380 << 1))        @//check mboot.bin in MBOOT or MBOOTBAK partition
    ldr         r1, =0x0100
    and         r0, r0, #0xff00
    cmp         r0, r1
    beq         emmc_ReadChunkHead_MBOT2                       @//reg[0x103381]#0=1b, MBOOT partition authen fail, try MBOOTBAK partition
#endif

    DPUTK       'm'
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    ldr         r2, =0x1000
    bl          eMMC_ReadData_MIU

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE)
    @//Authenticate CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x2F0
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x300
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
#if defined(CONFIG_DOUBLE_MBOOT)
    beq         BootRam_LoadUBoot

    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x01    @//reg[0x103381]#0=1b, Set ChunkHead of MBOOT Authen fail flag
    ldr         pc, =BOOTRAM_Entry_Hash                          @//MBOOT authen fail, jump to BOOTRAM_Entry_Hash to load MBOOTBAK

emmc_ReadChunkHead_MBOT2:
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    ldr         r2, =0x8000
    bl          eMMC_ReadData_MIU

    @//Authenticate CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x2F0
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x300
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRam_AuthenticationFail
#elif defined(CONFIG_SECURITY_BOOT)
    bne         BootRam_AuthenticationFail
#endif
BootRam_AuthChunkHeader_Exit:
#else
    bl          BootRam_LoadUBoot
#endif

    @// LOAD UBOOT
BootRam_LoadUBoot:

#ifdef CONFIG_MSTAR_NS_UBOOT
    bl          ns_uboot_init

    ldr         r2, =CONFIG_UBOOT_LOADADDR
    ldr         r0, [r2, #0x4C]    @// CUSTOMER_KEY_BANK_ROM_OFFSET
    bl          ns_uboot_read_keybank
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH

#if defined (CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    @// load bl31.bin
    bl          ns_uboot_load_armfw
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH
#endif


    @// load optee.bin
    bl          ns_uboot_load_optee
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH

    bl          ns_uboot_support_faststr
    cmp         r0, #0
    bne         BOOT_POINT
LOAD_OPTEE_FINISH:
#endif


#ifdef CONFIG_DOUBLE_MBOOT
    RREG        r0, (MS_BUS_RIU_ADDR + (0x103380 << 1))        @//check mboot.bin in MBOOT or MBOOTBAK partition
    ldr         r1, =0x0100
    and         r0, r0, #0xff00
    cmp         r0, r1
    beq         emmc_ReadUBOOT_MBOT2
#endif

    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r5, [r0, #4]    @// UBOOT_RAM_START
    ldr         r6, [r0, #8]    @// UBOOT_RAM_END
    sub         r1, r6 , r5
    bl          ns_uboot_set_uboot_version

#ifdef CONFIG_MSTAR_NS_UBOOT
    @// read chunk header again because optee/armfw also use uboot loadaddr
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    ldr         r2, =0x1000
    bl          eMMC_ReadData_MIU
#endif

    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r5, [r0, #4]    @// UBOOT_RAM_START
    ldr         r6, [r0, #8]    @// UBOOT_RAM_END
    sub         r1, r6 , r5
    add         r1, r1 , #0x110  @// SECURITY INFO Size 0x110
    #ifdef CONFIG_DOUBLE_MBOOT
    ldr         r2, =0x1023
    #else
    ldr         r2, =0x1002
    #endif
    bl          eMMC_ReadData_MIU

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE)
    @//Authenticate U-BOOT
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    sub         r1, r6, r5
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x10
    add         r2, r2, r1
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
#if defined(CONFIG_DOUBLE_MBOOT)
    beq         BOOT_POINT
    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x01    @//Set ChunkHead of MBOOT Authenticate fail flag
    ldr         pc, =BOOTRAM_Entry_Hash                          @//MBOOT authen fail, jump to BOOTRAM_Entry_Hash to load MBOOTBAK

emmc_ReadUBOOT_MBOT2:

#ifdef CONFIG_MSTAR_NS_UBOOT
    @// read chunk header again because optee/armfw also use uboot loadaddr
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    ldr         r2, =0x8000
    bl          eMMC_ReadData_MIU
#endif

    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r5, [r0, #4]    @// UBOOT_RAM_START
    ldr         r6, [r0, #8]    @// UBOOT_RAM_END
    sub         r1, r6 , r5
    add         r1, r1 , #0x110  @// SECURITY INFO Size 0x110
    ldr         r2, =0x8023
    bl          eMMC_ReadData_MIU

    @//Authenticate U-BOOT
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    sub         r1, r6, r5
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x10
    add         r2, r2, r1
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRam_AuthenticationFail
#elif defined(CONFIG_SECURITY_BOOT) || defined(CONFIG_MSTAR_NS_UBOOT)
    bne         BootRam_AuthenticationFail
#endif
BootRam_AuthUBoot_Exit:
#else
    bl          BOOT_POINT
#endif


#else
    #ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN		//Programming NAND for ROM Boot
        bl          DoMSDecompression
    #else
		ldr			r0, =CONFIG_UBOOT_LOADADDR
		ldr			r11, = (0x20000000 + 0x20000 + 0x400)				@// UBOOT_START ADDRESS in DRAM
		ldr			r1, =0x200000

_CopyUbootImgLoop:
		ldr 		r3, [r11], #4
		str			r3, [r0],  #4
		subs		r1, r1, #4
		bne			_CopyUbootImgLoop
		ldr         pc, =CONFIG_UBOOT_LOADADDR

    #endif
#endif

@//================================================================================================
BOOT_POINT:

    bl          ns_uboot_anti_rollback

#ifdef CONFIG_MSTAR_NS_UBOOT
#if defined (CONFIG_ARMv8_64BIT_KERNEL)
    //WREG        0x1f203C08, 0x3000
    //WREG        0x1f2473F8, 0x0
    //WREG        0x1f203C4C, 0x300
    //WREG        0x1f003DC0, 0x1
    //WREG        0x1f005D88, 0x1
    ldr         r0, =NS_UBOOT_EXIT
    bl          ns_uboot_boot
#else
#if defined(CONFIG_ARMv7_OPTEE) // ARMv7 optee flow with 32bit kernel
    bl          ns_uboot_get_optee_addr
    and         r4, r0, r0
    ldr         r0, #0
    ldr         r1, #0
    ldr         r2, #0
    ldr         r3, #0
    bl          r4
#elif defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) //SMP ARMv8 optee flow with 32bit kernel
    ldr         r0, =NS_UBOOT_EXIT
    bl          ns_uboot_boot
#endif
#endif
NS_UBOOT_EXIT:
    WREG        (MS_BUS_RIU_ADDR + (0x100514 << 1)), 0x3BA8          @// OP-TEE start-up will overwrite this register, so write it back here; this is SW work-around and chip-dependent
#endif

    DPUTK       'U'

    WREG        0x1f006040, 0x0000          @//Disable Hardware Timer 1

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) || \
    defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    ldr         pc, =CONFIG_UBOOT_LOADADDR
#else

#ifndef CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51
#ifdef CONFIG_MSTAR_PM_SWIR
    ldr         r0, =0x14030000
#else
    ldr         r0, =0x14020000
#endif
#else
    ldr         r0, =0x14030000
#endif

    ldr         pc, [r0, #16]
#endif

#if defined(CONFIG_ARMv8_32BIT_KERNEL)
    .align 4
    .globl      Bootram_SecondaryCore
Bootram_SecondaryCore:
#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
	.word 0xd53e1007 	//mrs	x7, sctlr_el3
	.word 0xd2820008 	//mov	x8, #0x1000                	// #4096
	.word 0xb27e0108 	//orr	x8, x8, #0x4
	.word 0xb2400108 	//orr	x8, x8, #0x1
	.word 0xb2750108 	//orr	x8, x8, #0x800
	.word 0x8a2800e7 	//bic	x7, x7, x8
	.word 0xb27b00e7 	//orr	x7, x7, #0x20
	.word 0xb27d00e7 	//orr	x7, x7, #0x8
	.word 0xd51e1007 	//msr	sctlr_el3, x7
	.word 0xd539f227 	//mrs	x7, s3_1_c15_c2_1
	.word 0xb27a00e7 	//orr	x7, x7, #0x40
	.word 0xd519f227 	//msr	s3_1_c15_c2_1, x7

FirmwarePath:
	.word 0xd503205f 	//wfe
	.word 0x5800053a 	//ldr	x26, d8 <NormalPath+0x50>
	.word 0xb9400341 	//ldr	w1, [x26]
	.word 0x58000522 	//ldr	x2, e0 <NormalPath+0x58>
	.word 0xd53800a0 	//mrs	x0, mpidr_el1
	.word 0x12001c07 	//and	w7, w0, #0xff
	.word 0x12181c00 	//and	w0, w0, #0xff00
	.word 0x0b401ce0 	//add	w0, w7, w0, lsr #7
	.word 0x2a000042 	//orr	w2, w2, w0
	.word 0x6b01005f 	//cmp	w2, w1
	.word 0x54000181 	//b.ne	88 <NormalPath>
	.word 0x52800000 	//mov	w0, #0x0                   	// #0
	.word 0xb9000340 	//str	w0, [x26]
	.word 0x5800043a 	//ldr	x26, e8 <NormalPath+0x60>
	.word 0xb9400347 	//ldr	w7, [x26]
	.word 0x5800043a 	//ldr	x26, f0 <NormalPath+0x68>
	.word 0xb9400348 	//ldr	w8, [x26]
	.word 0x0b074109 	//add	w9, w8, w7, lsl #16
	.word 0xd65f0120 	//ret	x9
	.word 0xd503201f 	//nop
	.word 0xd503201f 	//nop
	.word 0x17ffffeb 	//b	30 <FirmwarePath>

NormalPath:
	.word 0x5800029a 	//ldr	x26, d8 <NormalPath+0x50>
	.word 0xb9400341 	//ldr	w1, [x26]
	.word 0x18000222 	//ldr	w2, d4 <NormalPath+0x4c>
	.word 0x6b02003f 	//cmp	w1, w2
	.word 0x54fffcc1 	//b.ne	30 <FirmwarePath>
	.word 0xd2800009 	//mov	x9, #0x0                   	// #0
	.word 0x5800025a 	//ldr	x26, e8 <NormalPath+0x60>
	.word 0xb9400347 	//ldr	w7, [x26]
	.word 0x5800025a 	//ldr	x26, f0 <NormalPath+0x68>
	.word 0xb9400348 	//ldr	w8, [x26]
	.word 0x0b074109 	//add	w9, w8, w7, lsl #16
	.word 0xd5384240 	//mrs	x0, currentel
	.word 0xd51e4029 	//msr	elr_el3, x9
	.word 0xd51c101f 	//msr	sctlr_el2, xzr
	.word 0xd2807920 	//mov	x0, #0x3c9                 	// #969
	.word 0xd51e4000 	//msr	spsr_el3, x0
	.word 0xd2808020 	//mov	x0, #0x401                 	// #1025
	.word 0xd51e1100 	//msr	scr_el3, x0
	.word 0xd69f03e0 	//eret
	.word	0x0000babe
	.word	0x1f200a28
	.word	0x00000000
	.word	0x0000fac0
	.word	0x00000000
	.word	0x1f200a24
	.word	0x00000000
	.word	0x1f200a20
	.word	0x00000000
#else
    .word 0xd503205f  //wfe
    .word 0xd53e1007  //mrs x7, sctlr_el3
    .word 0xd2820008  //mov x8, #0x1000
    .word 0xb27e0108  //orr x8, x8, #0x4
    .word 0xb2400108  //orr x8, x8, #0x1
    .word 0xb2750108  //orr x8, x8, #0x800
    .word 0x8a2800e7  //bic x7, x7, x8
    .word 0xb27b00e7  //orr x7, x7, #0x20
    .word 0xb27d00e7  //orr x7, x7, #0x8
    .word 0xd51e1007  //msr sctlr_el3, x7
    .word 0xd539f227  //mrs x7, s3_1_c15_c2_1
    .word 0xb27a00e7  //orr x7, x7, #0x40
    .word 0xd519f227  //msr s3_1_c15_c2_1, x7
    .word 0x580005ba  //ldr x26, +0xb4
    .word 0xb9400341  //ldr w1, [x26]
    .word 0x180005a2  //ldr w2, +0xb4
    .word 0x6b02003f  //cmp w1, w2
    .word 0x54fffde1  //b.n eBootram_SecondaryCore

    @//aarch32_setup_ex
    .word 0xd2800007  //mov x7, #0x0
    .word 0x5800057a  //ldr x26, +0xac
    .word 0xb9400347  //ldr w7, [x26]
    .word 0xd51ec01f  //msr vbar_el3, xzr
    .word 0xd51cc01f  //msr vbar_el2, xzr
    .word 0xd51c101f  //msr sctlr_el2, xzr
    .word 0xd518101f  //msr sctlr_el1, xzr
    .word 0xd51e115f  //msr cptr_el3, xzr
    .word 0xd51c115f  //msr cptr_el2, xzr
    .word 0xd518105f  //msr cpacr_el1, xzr
    .word 0xd51c11ff  //msr hacr_el2, xzr
    .word 0xd51c117f  //msr hstr_el2, xzr
    .word 0xd51ce07f  //msr cntvoff_el2, xzr
    .word 0xd53800a0  //mrs x0, mpidr_el1
    .word 0xd51c00a0  //msr vmpidr_el2, x0
    .word 0xd5380000  //mrs x0, midr_el1
    .word 0xd51c0000  //msr vpidr_el2, x0
    .word 0xd53e1021  //mrs x1, actlr_el3
    .word 0xb2400021  //orr x1, x1, #0x1
    .word 0xb27f0021  //orr x1, x1, #0x2
    .word 0xb27c0021  //orr x1, x1, #0x10
    .word 0xb27b0021  //orr x1, x1, #0x20
    .word 0xb27a0021  //orr x1, x1, #0x40
    .word 0xd51e1021  //msr actlr_el3, x1
    .word 0xd51c111f  //msr hcr_el2, xzr
    .word 0xd51e403e  //msr elr_el3, x30
    .word 0xd51c101f  //msr sctlr_el2, xzr
    .word 0xd2807920  //mov x0, #0x3c9
    .word 0xd51e4000  //msr spsr_el3, x0
    .word 0xd2800000  //mov x0, #0x0
    .word 0xd51e1100  //msr scr_el3, x0
    .word 0xd51e4027  //msr elr_el3, x7
    .word 0xd53e4001  //mrs x1, spsr_el3
    .word 0x928043e2  //mov x2, #0xfffffffffffffde0
    .word 0x8a020021  //and x1, x1, x2
    .word 0xb2400421  //orr x1, x1, #0x3
    .word 0xb27c0021  //orr x1, x1, #0x10
    .word 0x9276f821  //and x1, x1, #0xfffffffffffffdff
    .word 0xd51e4001  //msr spsr_el3, x1
    .word 0xd69f03e0  //eret

    .align 3
    @//data
    .word 0x20208000  //data=0x20208000
    .word 0x00000000  //data=0x00000000
    .word 0x0000babe  //data=0x0000babe
    .word 0x00000000  //data=0x00000000
    .word 0x20208004  //data=0x20208004
    .word 0x00000000  //data=0x00000000
#endif

#elif defined(CONFIG_ARMv8_64BIT_KERNEL)
    .align 4
    .globl      Bootram_SecondaryCore_Prepare
Bootram_SecondaryCore_Prepare:

	.word 0xd282c001 	//mov	x1, #0x1600                	// #5632
	.word 0xd370bc21 	//lsl	x1, x1, #16
	.word 0x52801e00 	//mov	w0, #0xf0                  	// #240
	.word 0xb9200420 	//str	w0, [x1,#8196]
	.word 0x12800000 	//mov	w0, #0xffffffff            	// #-1
 	.word 0xb9108020 	//str	w0, [x1,#4224]
	.word 0x52800060 	//mov	w0, #0x3                   	// #3
	.word 0xb9100020 	//str	w0, [x1,#4096]
 	.word 0x52800060 	//mov	w0, #0x3                   	// #3
	.word 0xb9200020 	//str	w0, [x1,#8192]
    #.word 0xd503201f
    #.word 0xd503201f

    #.align 4
    .globl      Bootram_SecondaryCore
Bootram_SecondaryCore:
#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
	.word 0xd53e1007 	//mrs	x7, sctlr_el3
	.word 0xd2820008 	//mov	x8, #0x1000                	// #4096
	.word 0xb27e0108 	//orr	x8, x8, #0x4
	.word 0xb2400108 	//orr	x8, x8, #0x1
	.word 0xb2750108 	//orr	x8, x8, #0x800
	.word 0x8a2800e7 	//bic	x7, x7, x8
	.word 0xb27b00e7 	//orr	x7, x7, #0x20
	.word 0xb27d00e7 	//orr	x7, x7, #0x8
	.word 0xd51e1007 	//msr	sctlr_el3, x7
	.word 0xd539f227 	//mrs	x7, s3_1_c15_c2_1
	.word 0xb27a00e7 	//orr	x7, x7, #0x40
	.word 0xd519f227 	//msr	s3_1_c15_c2_1, x7

FirmwarePath:
	.word 0xd503205f 	//wfe
	.word 0x5800053a 	//ldr	x26, d8 <NormalPath+0x50>
	.word 0xb9400341 	//ldr	w1, [x26]
	.word 0x58000522 	//ldr	x2, e0 <NormalPath+0x58>
	.word 0xd53800a0 	//mrs	x0, mpidr_el1
	.word 0x12001c07 	//and	w7, w0, #0xff
	.word 0x12181c00 	//and	w0, w0, #0xff00
	.word 0x0b401ce0 	//add	w0, w7, w0, lsr #7
	.word 0x2a000042 	//orr	w2, w2, w0
	.word 0x6b01005f 	//cmp	w2, w1
	.word 0x54000181 	//b.ne	88 <NormalPath>
	.word 0x52800000 	//mov	w0, #0x0                   	// #0
	.word 0xb9000340 	//str	w0, [x26]
	.word 0x5800043a 	//ldr	x26, e8 <NormalPath+0x60>
	.word 0xb9400347 	//ldr	w7, [x26]
	.word 0x5800043a 	//ldr	x26, f0 <NormalPath+0x68>
	.word 0xb9400348 	//ldr	w8, [x26]
	.word 0x0b074109 	//add	w9, w8, w7, lsl #16
	.word 0xd65f0120 	//ret	x9
	.word 0xd503201f 	//nop
	.word 0xd503201f 	//nop
	.word 0x17ffffeb 	//b	30 <FirmwarePath>

NormalPath:
	.word 0x5800029a 	//ldr	x26, d8 <NormalPath+0x50>
	.word 0xb9400341 	//ldr	w1, [x26]
	.word 0x18000222 	//ldr	w2, d4 <NormalPath+0x4c>
	.word 0x6b02003f 	//cmp	w1, w2
	.word 0x54fffcc1 	//b.ne	30 <FirmwarePath>
	.word 0xd2800009 	//mov	x9, #0x0                   	// #0
	.word 0x5800025a 	//ldr	x26, e8 <NormalPath+0x60>
	.word 0xb9400347 	//ldr	w7, [x26]
	.word 0x5800025a 	//ldr	x26, f0 <NormalPath+0x68>
	.word 0xb9400348 	//ldr	w8, [x26]
	.word 0x0b074109 	//add	w9, w8, w7, lsl #16
	.word 0xd5384240 	//mrs	x0, currentel
	.word 0xd51e4029 	//msr	elr_el3, x9
	.word 0xd51c101f 	//msr	sctlr_el2, xzr
	.word 0xd28079a0 	//mov	x0, #0x3cd                 	// #973
	.word 0xd51e4000 	//msr	spsr_el3, x0
	.word 0xd2808020 	//mov	x0, #0x401                 	// #1025
	.word 0xd51e1100 	//msr	scr_el3, x0
	.word 0xd69f03e0 	//eret
	.word	0x0000babe
	.word	0x1f200a28
	.word	0x00000000
	.word	0x0000fac0
	.word	0x00000000
	.word	0x1f200a24
	.word	0x00000000
	.word	0x1f200a20
	.word	0x00000000
#else
    .word 0xd503205f  //wfe
    .word 0xd5380000  //mrs     x0, midr_el1
    .word 0x58000541  //ldr     x1, c8 <Bootram_SecondaryCore+0xd0>
    .word 0x8a010000  //and     x0, x0, x1
    .word 0xd344fc00  //lsr     x0, x0, #4
    .word 0xf134201f  //cmp     x0, #0xd08
    .word 0x540000a1  //b.ne    28  <Bootram_SecondaryCore+0x2c>
    .word 0xd539b040  //mrs     x0, s3_1_c11_c0_2
    .word 0xd2804441  //mov     x1, #0x222                      // #546
    .word 0xaa010000  //orr     x0, x0, x1
    .word 0xd519b040  //msr     s3_1_c11_c0_2, x0
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
    .word 0x580002fa  //ldr x26, b8 <Bootram_SecondaryCore+0xC0>
    .word 0xb9400341  //ldr w1, [x26]
    .word 0x18000222  //ldr w2, ac <Bootram_SecondaryCore+0xb0>
    .word 0x6b02003f  //cmp w1, w2
    .word 0x54fffde1  //b.ne Bootram_SecondaryCore
    .word 0xd2800009  //mov x9, #0x0
    .word 0x5800027a  //ldr x26, c0 <Bootram_SecondaryCore+0xc8>
    .word 0xb9400347  //ldr w7, [x26]
    .word 0x5800027a  //ldr x26, c0 <Bootram_SecondaryCore+0xd0>
    .word 0xb9400348  //ldr w8, [x26]
    .word 0x0b074109  //add w9, w8, w7, lsl #16
    .word 0xd5384240  //mrs x0, currentel
    .word 0xd51e4029  //msr elr_el3, x9
    .word 0xd51c101f  //msr sctlr_el2, xzr
    .word 0xd2807920  //mov x0, #0x3c9
    .word 0xd51e4000  //msr spsr_el3, x0
    .word 0xd2808020  //mov x0, #0x401
    .word 0xd51e1100  //msr scr_el3, x0
    @//.word 0x14000000  //eret
    .word 0xd69f03e0  //eret
    @//.word 0x0000fff0

    .align 3
    @//data
    .word   0x0000babe
    .word   0x00000000
    .word   0x0000fff0
    .word   0x00000000
    .word   0x1f200a28
    .word   0x00000000
    .word   0x1f200a24
    .word   0x00000000
    .word   0x1f200a20
    .word   0x00000000
#endif
#endif //CONFIG_ARMv8_64BIT_KERNEL

#if 1
BootRam_AuthenticationFail:
    PUTK        '\n'
    PUTK        '\r'
    DPUTK       'A'
    DPUTK       'U'
    DPUTK       'T'
    DPUTK       'H'
    DPUTK       '_'
    DPUTK       'F'
    DPUTK       'A'
    DPUTK       'I'
    DPUTK       'L'
#ifdef CONFIG_DOUBLE_MBOOT
    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x03    @//Set MBOOT & MBOOTBAK partition both authen fail flag
#endif

BootRam_AuthenticationFailLoop:
    b           BootRam_AuthenticationFailLoop
#endif



#ifdef CONFIG_MSTAR_WINDERMERE
    .global     BOOTRAM_InitWindermere
BOOTRAM_InitWindermere:
    ldr         r12, =(MS_BUS_RIU_ADDR + (0x101e00 << 1))
    bic         r0, r0, #0x0000
	str         r0, [r12, #(0x38 << 1)]

    ldr         r12, =(MS_BUS_RIU_ADDR + (0x112000 << 1))
    ldr         r0, [r12, #(0x02 << 1)]
    bic         r0, r0, #0x2000
	str         r0, [r12, #(0x02 << 1)]

    ldr         r12, =(MS_BUS_RIU_ADDR + (0x112800 << 1))
    ldr         r0, [r12, #(0x40 << 1)]
    bic         r0, r0, #0x10
    str         r0, [r12, #(0x40 << 1)]

    ldr         r0, [r12, #(0x78 << 1)]
    bic         r0, r0, #0x1000
    str         r0, [r12, #(0x78 << 1)]

    ldr         r0, [r12, #(0x6a << 1)]
    bic         r0, r0, #0x4000
    str         r0, [r12, #(0x6a << 1)]

    ldr         r0, [r12, #(0x7a << 1)]
    orr         r0, r0, #0x01
    str         r0, [r12, #(0x7a << 1)]

    ldr         r0, [r12, #(0x0e << 1)]
    bic         r0, r0, #0x10
    str         r0, [r12, #(0x0e << 1)]


    bx          lr
#endif

@//================================================================================================
Bootram_PowerSaving:

    //eMMC Internal LDO tune off
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123f00 << 1))
    ldr         r0, [r1, #(0x6c << 1)]
    orr         r0, #(0x01 << 5)
    str         r0, [r1, #(0x6c << 1)]

    bx          lr

    .globl  amzn_identifier
amzn_identifier:
    .ascii "YAMA"
    // sboot version <LO, HI>
    .byte 0x12, 0x00
