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
////////////////////////////////////////////////////
///    Header File
////////////////////////////////////////////////////
#include <command.h>
#include <MsBoot.h>

U_BOOT_CMD(
    bootcheck, CONFIG_SYS_MAXARGS, 1,    do_bootcheck,
    "bootcheck   - Do boot check\n",
    NULL
);

#if ENABLE_STB_ECOS_BOOT
U_BOOT_CMD(
    read_boot_info, CONFIG_SYS_MAXARGS, 1,    do_read_boot_info,
    "read_boot_info - read eCos boot info\n",
    NULL
);
#endif

#if defined(CONFIG_SEC_SYSTEM)
U_BOOT_CMD(
    Boot2secKernel, CONFIG_SYS_MAXARGS, 1,    do_Boot2secKernel,
    "Boot2secKernel   - jump to non-TEE second kernel\n",
    NULL
);
#endif

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
U_BOOT_CMD(
    smc, CONFIG_SYS_MAXARGS, 1,    do_smc,
    "smc   - jump to arm_trusted_firmware\n",
    NULL
);
#endif

#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    readArmFw, CONFIG_SYS_MAXARGS, 1,    do_read_arm_fw,
    "readArmFw   - read Nuttx to malloc buffer \n",
    NULL
);

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    readOptee, CONFIG_SYS_MAXARGS, 1,    do_read_optee,
    "readOptee   - core1 run at Nuttx \n",
    NULL
);
#endif


#if defined (CONFIG_DUAL_SYSTEM)
U_BOOT_CMD(
    bootflag_read, CONFIG_SYS_MAXARGS, 1,    do_bootflag_read,
    "bootflag_read   - Read bootflag.\n",
    NULL
);

U_BOOT_CMD(
    bootflag_set, CONFIG_SYS_MAXARGS, 1,    do_bootflag_set,
    "bootflag_set   - set bootflag to choose system0 or system1.\n",
    NULL
);

U_BOOT_CMD(
    sboot_verify_check, CONFIG_SYS_MAXARGS, 1,    do_sboot_verify_check,
    "sboot_verify_check   - Do sboot_verify_check\n",
    NULL
);

U_BOOT_CMD(
    copy_mpool, CONFIG_SYS_MAXARGS, 1,    do_copy_mpool,
    "copy_mpool   - Do bootflag check\n",
    NULL
);

U_BOOT_CMD(
    mboot_verify_check, CONFIG_SYS_MAXARGS, 1,    do_mboot_verify_check,
    "mboot_verify_check   - Do mboot_verify_check\n",
    NULL
);
#endif

U_BOOT_CMD(
    fcie_tsp_boot_sel, 2, 1,    do_fcie_tsp_boot_sel,
    "fcie_tsp_boot_sel   - Do fcie_tsp_boot_sel\n",
    NULL
);
