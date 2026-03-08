/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of
* the GNU General Public License as published by the Free Software Foundation;
* either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/
#ifndef _Mstar_Boot_Header_
#define _Mstar_Boot_Header_
#include <CusBoot.h>


#if defined (CONFIG_SECURITY_BOOT)
#define BOOT_COMMAND "authenticate recovery 0x25000000; bootm 0x25000000"
#else
  #if (ENABLE_MODULE_MMC == 1)
  #define BOOT_COMMAND "mmc read.p 0x45000000 recovery 0x00800000; bootm   0x45000000"
  #else
  #define BOOT_COMMAND "nand read.e 0x45000000 recovery 0x00800000; bootm 0x45000000"
  #endif
#endif

typedef enum
{
    EN_BOOT_MODE_UNKNOWN,
    EN_BOOT_MODE_NORMAL,
    EN_BOOT_MODE_RECOVERY,
    EN_BOOT_MODE_FORCEUGRADE,
    EN_BOOT_MODE_USB_UPGRADE,
    EN_BOOT_MODE_NET_UPGRADE,
    EN_BOOT_MODE_OAD_UPGRADE,
#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
    EN_BOOT_MODE_ENV_UPGRADE,
#endif
    EN_BOOT_MODE_USB_RECOVRY_UPGRADE,
    EN_BOOT_MODE_OTA_UPGRADE,
    EN_BOOT_MODE_RECOVRY_WIPE_DATA,
    EN_BOOT_MODE_RECOVRY_WIPE_CACHE,
    EN_BOOT_MODE_UART_DEBUG,
#if CONFIG_RESCUE_ENV
    EN_BOOT_MODE_BRICK_TERMINATOR,
#endif
#if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
    EN_BOOT_MODE_SYSTEM_RESTORE,
#endif
#if defined (CONFIG_SEC_SYSTEM)
    EN_BOOT_MODE_SECOND_SYSTEM,
#endif
#if defined (CONFIG_AN_FASTBOOT_ENABLE)
   EN_BOOT_MODE_FASTBOOT,
#endif

}EN_BOOT_MODE;

#if defined (CONFIG_DUAL_SYSTEM)
#define DUAL_SYSTEM_PARTITION_SUFFIX "1"
#define DUAL_MBOOT_PARTITION_SUFFIX "BAK"

unsigned char read_bootflag(void);
int do_bootflag_read (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootflag_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_sboot_verify_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_copy_mpool(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_mboot_verify_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
int do_bootcheck (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int isBootToRecovery(void);

#if ENABLE_STB_ECOS_BOOT
int do_read_boot_info (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_SEC_SYSTEM)
int do_Boot2secKernel (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
int do_smc (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
int do_read_arm_fw (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_read_optee (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

int do_fcie_tsp_boot_sel(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
