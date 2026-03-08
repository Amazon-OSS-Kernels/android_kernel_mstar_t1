/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
OUTPUT_ARCH(arm)
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")

ENTRY(_vector)

MEMORY
{
#if (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH == 1)
    boot :  ORIGIN = 0x1fc00000,                LENGTH = 14K
    boot2 : ORIGIN = 0x1fc00000 + 7K,           LENGTH = 7K
    boot3 : ORIGIN = 0x1fc00000 + 7K,           LENGTH = 7K
    ram :   ORIGIN = CONFIG_BOOTRAM_LOADADDR,   LENGTH = 9K
#elif (CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH == 1)
    boot :  ORIGIN = 0x1fc00000,                LENGTH = 14K
    boot2 : ORIGIN = 0x1fc00000 + 7K,           LENGTH = 7K
    boot3 : ORIGIN = 0x1fc00000 + 7K,           LENGTH = 7K
    boot4 : ORIGIN = 0x1fc00000 + 7K,           LENGTH = 7K
#ifdef CONFIG_DOUBLE_MBOOT
    ram1 :  ORIGIN = CONFIG_BOOTRAM_LOADADDR +17k,   LENGTH = 5K
    ram2 :  ORIGIN = CONFIG_BOOTRAM_LOADADDR,       LENGTH = 17K
#else
    ram :  ORIGIN = CONFIG_BOOTRAM_LOADADDR,        LENGTH = 26K
#endif
#else
    boot :  ORIGIN = 0x00000000,                LENGTH = 8K
    rom :   ORIGIN = 0x00000000+0x2000,         LENGTH = 18K
    ram :   ORIGIN = CONFIG_BOOTRAM_LOADADDR,   LENGTH = 128K
#endif
}

SECTIONS
{
#if (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH == 1)
    .text1 :
    {
        _ld_boot_start = ABSOLUTE(.);
        *boot.o             (.text*)

        . = ALIGN(16);
        _ld_boot_end = ABSOLUTE(.);
        _ld_boot_size = _ld_boot_end - _ld_boot_start;

        _ld_bootrom_start = ABSOLUTE(.);
        *bootrom.o          (.text*)
        *bootrom_cache.o    (.text*)
        *drvRIU.o           (.text*)
        *drvAESDMA.o        (.text*)
        *drvAESDMA.o        (.rodata*)
        *halAESDMA.o        (.text*)
        *drvNAND_prg.o      (.text*)
        *drvNAND_hal.o      (.text*)
        *drvNAND_hal_v5.o   (.text*)
        *drvNAND_platform.o (.text*)
        *drvNAND_prg.o      (.data*)
        *drvNAND_prg.o      (.bss*)
        *drvNAND_hal.o      (.bss*)
        *drvNAND_hal_v5.o   (.bss*)
        *drvNAND_platform.o (.bss*)
        *bootrom_data_miu.o (.rodata*)

        . = LENGTH(boot) - LENGTH(boot2);

        _ld_program_reload_addr = ABSOLUTE(.);
        *bootrom_pll.o      (.text*)
        *bootrom_clock.o    (.text*)
        *bootrom_peripheral.o   (.text*)
        *bootrom.o          (.rodata*)
/*      *drvPadConf.o       (.rodata*) */

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl_PreInit)))

        . = LENGTH(boot) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(boot);
        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;

    } > boot

    .text2 : AT (LOADADDR(.text1) + LENGTH(boot))
    {
        _ld_miu_flow_flash_start = LENGTH(boot) - _ld_program_reload_addr;
        _ld_miu_flow_flash_size = LENGTH(boot2) - _ld_program_reload_addr;
        _ld_miu_flow_start = ABSOLUTE(.);

        *bootrom_miu.o         (.text*)
    #if (CONFIG_MIU_DDR4 == 1)
        *bootrom_dqs_ddr4.o      (.text*)
        *bootrom_dqs_ddr4.o     (.rodata*)
    #elif (CONFIG_MIU_DDR3 == 1)
        *bootrom_dqs_ddr3.o      (.text*)
        *bootrom_dqs_ddr3.o     (.rodata*)
    #else
    #endif
        . = LENGTH(boot2) - 512;
        /* Reserved for Secure Booting */
        . = LENGTH(boot2);
        _ld_miu_flow_end = ABSOLUTE(.);
        _ld_miu_flow_size = _ld_miu_flow_end - _ld_miu_flow_start;

    } > boot2

    .text3 : AT (LOADADDR(.text2) + LENGTH(boot2))
    {
        _ld_miu_str_flow_flash_start = LENGTH(boot2) - _ld_program_reload_addr + _ld_miu_flow_flash_start;
        _ld_miu_str_flow_flash_size = LENGTH(boot3) - _ld_program_reload_addr;
        _ld_miu_str_flow_start = ABSOLUTE(.);

        *bootrom_miu_str.o      (.text*)
        *bootrom_data_miu_str.o (.rodata*)

        . = LENGTH(boot3) - 512;
        /* Reserved for Secure Booting */
        . = LENGTH(boot3);
        _ld_miu_str_flow_end = ABSOLUTE(.);
        _ld_miu_str_flow_size = _ld_miu_str_flow_end - _ld_miu_str_flow_start;

    } > boot3

    .text4 : AT (LOADADDR(.text1) + 0x8000)
    {
        _ld_bootram_start = ABSOLUTE(.);
        *bootram.o          (.text*)

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl)))

        *nandflash.o        (.text*)
        *nandflash.o        (.rodata*)
        *uart16550.o        (.text*)
        *setSPI.o           (.text*)
        *setSPI.o           (.rodata*)
        *apiBDMA.o          (.text*)
        *apiBDMA.o          (.rodata*)
        *apiBDMA.o          (.data*)
        *apiBDMA.o          (.bss*)

        *MSDecompress.o     (.text*)
        *MSDecompress.o     (.rodata*)
        *MSDecompress.o     (.data*)
        *MSDecompress.o     (.bss*)

        *(NANDCODE)

        *(.ARM.exidx*)

        . = ALIGN(16);
        _ld_bootdata_start = .;
        *(.data*)
        . = ALIGN(16);
        _ld_bootdata_end = .;

        _ld_bootbss_start = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        _ld_bootbss_end = ABSOLUTE(.);

        . = LENGTH(ram) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(ram);
        _ld_bootram_end = ABSOLUTE(.);
        _ld_bootram_size = _ld_bootram_end - _ld_bootram_start;

    } > ram

#elif (CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH == 1)
    .text1 :
    {
        *boot.o             (.text*)

        . = ALIGN(16);
        _ld_bootrom_start = ABSOLUTE(.);

        *bootrom.o          (.text*)
        *bootrom_cache.o    (.text*)
        *drvRIU.o           (.text*)
        *drvAESDMA.o        (.text*)
        *drvAESDMA.o        (.rodata*)
        *halAESDMA.o        (.text*)
        *eMMC_prg.o         (.text*)
        *eMMC_hal.o         (.text*)
        *eMMC_platform.o    (.text*)
        *eMMC_RPMB.o        (.text*)
        *eMMC_prg.o         (.data*)
        *eMMC_hal.o         (.data*)
        *eMMC_platform.o    (.data*)
        *eMMC_RPMB.o        (.data*)
        *eMMC_prg.o         (.bss*)
        *eMMC_hal.o         (.bss*)
        *eMMC_platform.o    (.bss*)
        *eMMC_RPMB.o        (.bss*)
/*        *bootrom_data_miu.o (.rodata*) */
#ifdef CONFIG_ONEBIN_ENABLE
        *drvLoadTable.o    (.text*)
#endif
        . = LENGTH(boot) - LENGTH(boot2);

        _ld_program_reload_addr = ABSOLUTE(.);
        *bootrom_pll.o      (.text*)
        *bootrom_clock.o    (.text*)
        *bootrom_peripheral.o   (.text*)
        *bootrom.o          (.rodata*)
/*      *drvPadConf.o       (.rodata*) */

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl_PreInit)))

        . = LENGTH(boot) - 1024;
        . = LENGTH(boot);
        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;

    } > boot

    .text2 : AT (LOADADDR(.text1) + LENGTH(boot))
    {
        _ld_miu_flow_flash_start = LENGTH(boot) - _ld_program_reload_addr;
        _ld_miu_flow_flash_size = LENGTH(boot2) - _ld_program_reload_addr;
        _ld_miu_flow_start = ABSOLUTE(.);
#ifdef CONFIG_ONEBIN_ENABLE
        KEEP (*(SORT_BY_NAME(.bss.MIUBuffer)))
#endif
        *bootrom_miu.o         (.text*)
        *bootrom_data_miu.o    (.rodata*)

    #if (CONFIG_MIU_DDR4 == 1)
        *bootrom_dqs_ddr4.o      (.text*)
        *bootrom_dqs_ddr4.o     (.rodata*)
    #elif (CONFIG_MIU_DDR3 == 1)
        *bootrom_dqs_ddr3.o      (.text*)
        *bootrom_dqs_ddr3.o     (.rodata*)
    #else
    #endif

        . = LENGTH(boot2) - 512;
        . = LENGTH(boot2);
        _ld_miu_flow_end = ABSOLUTE(.);
        _ld_miu_flow_size = _ld_miu_flow_end - _ld_miu_flow_start;

    } > boot2

    .text3 : AT (LOADADDR(.text2) + LENGTH(boot2))
    {
        _ld_miu_str_flow_flash_start = LENGTH(boot2) - _ld_program_reload_addr + _ld_miu_flow_flash_start;
        _ld_miu_str_flow_flash_size = LENGTH(boot3) - _ld_program_reload_addr;
        _ld_miu_str_flow_start = ABSOLUTE(.);
#ifdef CONFIG_ONEBIN_ENABLE
        KEEP (*(SORT_BY_NAME(.bss.MIUSTRBuffer)))
#endif
        *bootrom_miu_str.o      (.text*)
        *bootrom_data_miu_str.o (.rodata*)

        . = LENGTH(boot3) - 512;
        . = LENGTH(boot3);
        _ld_miu_str_flow_end = ABSOLUTE(.);
        _ld_miu_str_flow_size = _ld_miu_str_flow_end - _ld_miu_str_flow_start;

    } > boot3

    .text4 : AT (LOADADDR(.text3) + LENGTH(boot3))
    {
        _ld_miu_dqs_flow_start = ABSOLUTE(.);

#if 0
    #if (CONFIG_MIU_DDR4 == 1)
        *bootrom_dqs_ddr4.o      (.text*)
        *bootrom_dqs_ddr4.o     (.rodata*)
    #elif (CONFIG_MIU_DDR3 == 1)
        *bootrom_dqs_ddr3.o      (.text*)
        *bootrom_dqs_ddr3.o     (.rodata*)
    #else
    #endif
#endif

        . = LENGTH(boot4) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(boot4);
        _ld_miu_dqs_flow_end = ABSOLUTE(.);
        _ld_miu_dqs_flow_size = _ld_miu_dqs_flow_end - _ld_miu_dqs_flow_start;

    } > boot4

#ifdef CONFIG_DOUBLE_MBOOT
    .text5 : AT (LOADADDR(.text1) + 0x8000)
    {
        _ld_bootram1_start = ABSOLUTE(.);

#ifdef CONFIG_DUAL_SYSTEM
        *bootram_hash_dualsystem.o          (.text*)
        *bootram_hash_dualsystem.o          (.rodata*)
#else
        *bootram_hash.o          (.text*)
        *bootram_hash.o          (.rodata*)
#endif

        . = LENGTH(ram1) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(ram1);
        _ld_bootram1_end = ABSOLUTE(.);
        _ld_bootram1_size = _ld_bootram1_end - _ld_bootram1_start;

    } > ram1

    .text6 : AT (LOADADDR(.text1) + 0x9400)
    {
        _ld_bootram2_start = .;
        *bootram.o          (.text*)

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl)))

        *nandflash.o        (.text*)
        *nandflash.o        (.rodata*)
        *uart16550.o        (.text*)
        *setSPI.o           (.text*)
        *setSPI.o           (.rodata*)
        *apiBDMA.o          (.text*)
        *apiBDMA.o          (.rodata*)
        *apiBDMA.o          (.data*)
        *apiBDMA.o          (.bss*)

        *ns_uboot_loader.o          (.text*)
        *ns_uboot_loader.o          (.rodata*)
        *ns_uboot_loader_keybank.o  (.rodata*)
        *drvSEAL.o          (.text*)
        *halSEAL.o          (.text*)
        *halSEAL.o          (.rodata*)
        *halAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.rodata*)

        *MSDecompress.o     (.text*)
        *MSDecompress.o     (.rodata*)
        *MSDecompress.o     (.data*)
        *MSDecompress.o     (.bss*)
        *(EMMCCODE)
        *(.ARM.exidx*)

        . = ALIGN(16);
        _ld_bootdata_start = .;
        *(.data*)
        . = ALIGN(16);
        _ld_bootdata_end = .;

        _ld_bootbss_start = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        _ld_bootbss_end = ABSOLUTE(.);

        . = LENGTH(ram2) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(ram2);
        _ld_bootram2_end = ABSOLUTE(.);
        _ld_bootram2_size = _ld_bootram2_end - _ld_bootram2_start;

    } > ram2
#else
    .text5 : AT (LOADADDR(.text1) + 0x8000)
    {
        _ld_bootram_start = .;
        *bootram.o          (.text*)
        *bootram_cus_uboot2.o        (.text*)
        *bootram_cus_uboot2.o        (.rodata*)
        *bootram_cus_uboot2.o        (.data*)
        *bootram_cus_uboot2.o        (.bss*)

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl)))

        *nandflash.o        (.text*)
        *nandflash.o        (.rodata*)
        *uart16550.o        (.text*)
        *setSPI.o           (.text*)
        *setSPI.o           (.rodata*)
        *apiBDMA.o          (.text*)
        *apiBDMA.o          (.rodata*)
        *apiBDMA.o          (.data*)
        *apiBDMA.o          (.bss*)

        *ns_uboot_loader.o          (.text*)
        *ns_uboot_loader.o          (.rodata*)
        *ns_uboot_loader_keybank.o  (.rodata*)
        *drvSEAL.o          (.text*)
        *halSEAL.o          (.text*)
        *halSEAL.o          (.rodata*)
        *halAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.rodata*)

        *MSDecompress.o     (.text*)
        *MSDecompress.o     (.rodata*)
        *MSDecompress.o     (.data*)
        *MSDecompress.o     (.bss*)
        *(EMMCCODE)
        *(.ARM.exidx*)

        . = ALIGN(16);
        _ld_bootdata_start = .;
        *(.data*)
        . = ALIGN(16);
        _ld_bootdata_end = .;

        _ld_bootbss_start = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        _ld_bootbss_end = ABSOLUTE(.);

        . = LENGTH(ram) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(ram);
        _ld_bootram_end = ABSOLUTE(.);
        _ld_bootram_size = _ld_bootram_end - _ld_bootram_start;

    } > ram
#endif

#else
    .text1 :
    {
        _ld_boot_start = ABSOLUTE(.);
        *boot.o             (.text*)
        . = ALIGN(16);
        _ld_boot_end = ABSOLUTE(.);
        _ld_boot_size = _ld_boot_end - _ld_boot_start;

    } > boot

    .text2 : AT ( LOADADDR(.text1) + 0x2000 )
    {
        _ld_bootrom_start = ABSOLUTE(.);
        *drvRIU.o           (.text*)
        *bootrom.o          (.text*)
        *bootrom_pll.o      (.text*)
        *bootrom_clock.o    (.text*)
        *bootrom_peripheral.o   (.text*)
        *bootrom_cache.o    (.text*)
        *bootrom_miu.o      (.text*)
        *bootrom_miu_str.o  (.text*)

    #if (CONFIG_MIU_DDR4 == 1)
        *bootrom_dqs_ddr4.o      (.text*)
    #elif (CONFIG_MIU_DDR3 == 1)
        *bootrom_dqs_ddr3.o      (.text*)
    #else
    #endif

        *bootrom.o          (.rodata*)
        *bootrom_data_miu.o (.rodata*)
/*      *drvPadConf.o       (.rodata*) */

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl_PreInit)))

        . = ALIGN(16);
        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;
    } > rom

    .text3 : AT (LOADADDR(.text2) + SIZEOF(.text2))
    {
        _ld_bootram_start = ABSOLUTE(.);
        *bootram.o          (.text*)

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl)))

        *nandflash.o        (.text*)
        *nandflash.o        (.rodata*)
        *uart16550.o        (.text*)
        *setSPI.o           (.text*)
        *setSPI.o           (.rodata*)
        *apiBDMA.o          (.text*)
        *apiBDMA.o          (.rodata*)

        *MSDecompress.o     (.text*)
        *MSDecompress.o     (.rodata*)

        *(.ARM.exidx*)

    } > ram

    .data :
    {
        . = ALIGN(16);
        _ld_bootdata_start = ABSOLUTE(.);
        *(.data*)
        . = ALIGN(16);
        _ld_bootdata_end = ABSOLUTE(.);
        _ld_bootram_end = ABSOLUTE(.);
        _ld_bootram_size = _ld_bootram_end - _ld_bootram_start;
    } > ram

    .bss :
    {
        . = ALIGN(16);
        _ld_bootbss_start = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        _ld_bootbss_end = ABSOLUTE(.);
    } > ram

#endif

    /DISCARD/ :
    {
        /* ABI crap starts here */

        *(.reginfo)
        *(.note*d)
    }

#if (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH == 1)
    .padding : AT(LOADADDR(.text1) + 0x8000 - 4)
    {
        /* LONG(0x0) */
    } > ram
#elif (CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH == 1)
#ifdef CONFIG_DOUBLE_MBOOT
    .padding : AT(LOADADDR(.text1) + 0x8000 - 4)
    {
        /* LONG(0x0) */
    } > ram1
    .padding : AT(LOADADDR(.text1) + 0x9400 - 4)
    {
        /* LONG(0x0) */
    } > ram2
#else
    .padding : AT(LOADADDR(.text1) + 0x8000 - 4)
    {
        /* LONG(0x0) */
    } > ram
#endif
#else
    .padding : AT(LOADADDR(.text1) + 0x10000 - 4)
    {
        LONG(0x0)
    } > ram
#endif
}
