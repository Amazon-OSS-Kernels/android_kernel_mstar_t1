/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    cmd_secure.c
/// @brief  SCS Main Function
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <common.h>
#include <command.h>
#include <secure/MsSecureBoot.h>
#include <secure/MsSecureUpgrade.h>
#include <secure/MsSignature.h>
#include <MsSecureTee.h>


U_BOOT_CMD(
    store_secure_info, 3, 0, do_save_secure_info,
    "store secure info\n",
    "store_secure_info targetName(hex) imageDramAddr(hex)\n"
);

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    SecureInit, 7, 0, do_Secure_Init,
    "Init security booting.\n",
    "SecureInit\n"
);

U_BOOT_CMD(
    verify_sha256, 5, 0,do_verify_sha256,
    "This command is only for verification \n",
    "verify_sha256 intAddr(hex) outAddr(hex) intLen(hex) count(hex)\n"
);

U_BOOT_CMD(
    sha256, 4, 0,do_sha256,
    "The sha256 cacluate \n",
    "sha256 intAddr(hex) outAddr(hex) intLen(hex)\n"
);

U_BOOT_CMD(
    rsa, 4, 0,do_rsa,
    "rsa cacluate \n",
    "rsa signAddr(hex) keyAddr(hex) outAddr(hex)\n"
);

U_BOOT_CMD(
    stest, 3, 1,    do_performance_test,
    "test IO performance on continue mode or interleave mode]\n",
    "-c [partition]\n"
    "-i [partition]\n"
);


U_BOOT_CMD(authenticateAN, 2, 0, do_AN_Authenticate,
   "authenticate android's boot and recover image",
    "[dram addr]\n"
);

U_BOOT_CMD(authenticate, 3, 0, do_Authenticate,
   "authenticate the kernel image",
    "[image name]\n"
    #if !(ENABLE_MODULE_ANDROID_BOOT == 1)
    #if defined (CONFIG_SECURITY_BOOT) && (ENABLE_STB_BOOT == 1)
    "image name]:KL RFS,MSLIB,APP,MWLIB,LIB,CON,RTPM\n"
    #else
    "image name]:KL RFS,MSLIB,APP,MWLIB,LIB,CON\n"
    #endif
    #else
    "image name:boot recovery\n"
    #endif
);

U_BOOT_CMD(dumpSignature, 2, 0, do_DumpSignature,
    "dump the signature",
    "[target file]\n"
    #if !(ENABLE_MODULE_ANDROID_BOOT == 1)
    "target file:KL,RFS,MSLIB,APP,MWLIB,LIB,CON,RTPM\n"
    #else
    "target file:boot recovery\n"
    #endif
);

#if defined(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51)
U_BOOT_CMD(se_ut1, 1, 0, do_DoSecureEngUnitTestCase1,
    "The test case 1 for secure engine \n",
    "se_ut1"
);
U_BOOT_CMD(se_ut2, 1, 0, do_DoSecureEngUnitTestCase2,
    "The test case 2 for secure engine \n",
    "se_ut2"
);
U_BOOT_CMD(se_ut3, 1, 0, do_DoSecureEngUnitTestCase3,
    "The test case 3 for secure engine \n",
    "se_ut3"
);

U_BOOT_CMD(se_ut4, 1, 0, do_DoSecureEngUnitTestCase4,
    "The test case 4 for secure engine \n",
    "se_ut4"
);

U_BOOT_CMD(se_ut5, 1, 0, do_DoSecureEngUnitTestCase5,
    "The test case 5 for secure engine \n",
    "se_ut5"
);
#endif

#if (ENABLE_MODULE_TEE==1) || (ENABLE_SECURITY_BOOT == 1)
U_BOOT_CMD(readKL, 1, 0, do_ReadKL,
    "readKL \n",
    "fixed dram address for reading Kernel"
);

U_BOOT_CMD(bootKL, 1, 0, do_BootKL,
    "bootKL \n",
    "fixed dram address for booting Kernel"
);

U_BOOT_CMD(SecureBootCmd, 1, 0, do_SecureBootCmd,
    "SecureBootCmd \n",
    "fixed dram address for Kernel"
);
#endif

#if defined(CONFIG_SECURE_USB_UPGRADE)
U_BOOT_CMD(filepartloadSegAES, 7, 0, do_file_part_load_with_segment_aes_decrypted,
    "load a part of file to RAM and do descrypted",
    "filepartloadSegAES interface(hex) device(hex) addr(hex) filename(hex) offset(hex) bytes(hex)\n"
);

U_BOOT_CMD(fileSegRSA, 4, 0, do_file_segment_rsa_authendication,
    "do file segment rsa authendication",
    "fileSegRSA interface(hex) device(hex) filename(hex) \n"
);
#endif

#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    readNuttx, CONFIG_SYS_MAXARGS, 1,    do_readNuttx,
    "readNuttx   - read Nuttx to malloc buffer \n",
    NULL
);

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    bootNuttx, CONFIG_SYS_MAXARGS, 1,    do_bootNuttx,
    "bootNuttx   - core1 run at Nuttx \n",
    NULL
);

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    wait_tee_ready, CONFIG_SYS_MAXARGS, 1,    do_wait_tee_ready,
    "wait_tee_ready   - wait_tee_ready \n",
    NULL
);
#ifdef CONFIG_MSTAR_R2_STR
U_BOOT_CMD(
    r2resume, CONFIG_SYS_MAXARGS, 1,    do_r2_resume,
    "   - R2 Tee str resume \n",
    NULL
);
#endif
U_BOOT_CMD(
    store_nuttx_config, CONFIG_SYS_MAXARGS, 1,    do_save_nconfig,
    "   - save nuttx_config.bin to storage\n",
    NULL
);
U_BOOT_CMD(
    load_nuttx_config, 1, 1,    do_load_nconfig,
    "  - load nuttx_config.bin from storage\n",
    NULL
);
U_BOOT_CMD(
    dbgTee, 2, 1,    do_set_tee_debug_level,
    "  - set tee's debug level\n",
    "[Level] \n"
    "Level:'DISABLE', 'ERROR', 'INFO', 'TRACE', 'DEBUG', 'DEBUG_L2' \n"
);
#endif

#ifdef CONFIG_TFTP_SECURE_CHECK
U_BOOT_CMD(secure_image_check, 4, 0, do_secure_image_check,
    "Secure image check \n",
    "secure_image_check [addr] [size] [type]"
);
#endif
