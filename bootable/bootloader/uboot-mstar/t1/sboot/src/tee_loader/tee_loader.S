#ifndef _BOOT_INC_
#include "boot.inc"
#endif

@//-------------------------------------------------------------------------------------------------
@// TEELOADER_Entry:
@// @param  None
@// @return None
@// @note   Entry point of Boot code
@//-------------------------------------------------------------------------------------------------
    .globl      TEELOADER_Entry
TEELOADER_Entry:
    PUTK       'T'

    bl          BOOTRAM_InitEntry
    @// Load bootram header
    //ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =0x200
    ldr         r2, =CONFIG_EMMC_BOOTRAM_ADDRESS
    mov         r2, r2, lsr #9
    bl          eMMC_LoadImages

    @// Read chunk header
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    ldr         r2, =0x1000
    bl          eMMC_ReadData_MIU

    @//Authenticate CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x2F0
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x300
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain
    cmp         r0, #1
    bne         TeeLoader_AuthenticationFail

    @// Load Keybank
    bl          ns_uboot_init

    ldr         r2, =CONFIG_UBOOT_LOADADDR
    ldr         r0, [r2, #0x4C]    @// CUSTOMER_KEY_BANK_ROM_OFFSET
    bl          ns_uboot_read_keybank
    cmp         r0, #0
    beq         TeeLoader_AuthenticationFail

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =0x100
    ldr         r2, =CONFIG_BOOTRAM_LOADADDR + 0x100
    ldr         r3, =0x1
    bl          ns_uboot_SecureMain
    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         TeeLoader_AuthenticationFail

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =0x8000
    bl          drvNAND_Load2ndBL

#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
    #ifdef CONFIG_DOUBLE_MBOOT
        ldr         r0, =_ld_bootram1_start
        ldr         r1, [r0, #8]    @// _ld_bootram_size
    #else
        ldr         r0, =CONFIG_BOOTRAM_LOADADDR
        ldr         r1, [r0, #8]    @// _ld_bootram_size
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
    PUTK       '-'

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    #ifdef CONFIG_DOUBLE_MBOOT
        ldr         r0, =_ld_bootram1_start                                          @//Set the starting address of HASH for Authentication
        //ldr         r1, =(_ld_bootram1_size - 0x400)                                 @//Set the HASH size for Authentication
        ldr         r1, [r0, #8]    @//_ld_bootram_size
        sub         r1, r1, #0x400
        //ldr         r2, =(_ld_bootram1_end - 0x100)                               @//Set the starting address of SIG for Authentication
        ldr         r2, [r0, #4]    @//_ld_bootram_end
        sub         r2, r2, #0x100
    #else
        ldr         r0, =CONFIG_BOOTRAM_LOADADDR                                 @//Set the starting address of HASH for Authentication
        //ldr         r1, =(_ld_bootram_size-0x400)                                                  @//Set the HASH size for Authentication
        ldr         r1, [r0, #8]    @// _ld_bootram_size
        sub         r1, r1, #0x400
        #ifdef CONFIG_BOOTRAM_STORE_IN_MBOOT
            //ldr         r2, =(CONFIG_BOOTRAM_LOADADDR + _ld_bootram_size - 0x400)                       @//Set the starting address of SIG for Authentication
            ldr         r2, = (CONFIG_BOOTRAM_LOADADDR - 0x400)
            ldr         r4, [r0, #8]
            add         r2, r2, r4
        #else
            //ldr         r2, =(CONFIG_BOOTRAM_LOADADDR + _ld_bootram_size - 0x100)                       @//Set the starting address of SIG for Authentication
            ldr         r2, = (CONFIG_BOOTRAM_LOADADDR - 0x100)
            ldr         r4, [r0, #8]
            add         r2, r2, r4
        #endif
    #endif
    ldr         r3, =0x1
    bl          ns_uboot_SecureMain

    @//If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         TeeLoader_AuthenticationFail
#endif
#endif

#if defined (CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    @// load bl31.bin
    bl          ns_uboot_load_armfw
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH
#endif

    @// Load optee.bin
    bl          ns_uboot_load_optee
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH

    bl          ns_uboot_anti_rollback

LOAD_OPTEE_FINISH:
    @// Copy ree keybank
    ldr         r0, =_ld_TEE_LDR_Reekeybank_start
    bl          ns_uboot_copy_ReeKeybank


BOOT_POINT:

    bl          MDrv_AESDMA_DisableHwKey
#if defined (CONFIG_ARMv8_64BIT_KERNEL)
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r0, [r0]
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
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r0, [r0]
    bl          ns_uboot_boot
#else
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r0, [r0]
    mov         pc, r0
#endif
#endif

TeeLoader_AuthenticationFail:
    PUTK        '\n'
    PUTK        '\r'
    PUTK       'A'
    PUTK       'U'
    PUTK       'T'
    PUTK       'H'
    PUTK       '_'
TeeLoader_LoadOpteeFail:
    PUTK       'F'
    PUTK       'A'
    PUTK       'I'
    PUTK       'L'
    PUTK       '\n'
    PUTK       'o'
    PUTK       'p'
    PUTK       't'
    PUTK       'e'
    PUTK       'e'

TeeLoader_AuthenticationFailLoop:
    b           TeeLoader_AuthenticationFailLoop

TeeLoader_LoadBootRam:

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

