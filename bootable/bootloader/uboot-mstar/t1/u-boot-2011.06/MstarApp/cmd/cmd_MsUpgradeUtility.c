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
#include <MsUpgradeUtility.h>
#include <config.h>
#include <command.h>

#if(ENABLE_MODULE_DONT_OVERWRITE==1)
U_BOOT_CMD(dont_overwrite_init, 1, 0, do_dont_overwrite_init,
            "This serial apis are for protecting the specific partitions are overwrited. ex: customer\n"
            "This cmd is used to init this function\n",
            "\n");

U_BOOT_CMD(dont_overwrite, 2, 0, do_dont_overwrite,
            "This serial apis are for protecting the specific partitions are overwrited. ex: customer\n"
            "This cmd is used to register which partition should not be overwrite\n",
            "[partition name] \n");

U_BOOT_CMD(dont_overwrite_free, 1, 0, do_dont_overwrite_free,
            "This serial apis are for protecting the specific partitions are overwrited. ex: customer\n"
            "This cmd is used to free all registered partitions.\n",
            "\n");

U_BOOT_CMD(force_overwrite_init, 1, 0, do_force_overwirte_init,
            "This serial apis are for force write and re-create partitions.\n"
            "This cmd is used to register which partition should force overwrite\n",
            "\n");

U_BOOT_CMD(force_overwrite, 2, 0, do_force_overwrite,
            "This serial apis are for force write and re-create partitions.\n"
            "This cmd is used to register which partition should force overwrite\n",
            "[partition name] \n");

#endif

U_BOOT_CMD
(
    update_mode, 3, 0, do_rw_en_update_mode,
    "get/set update_mode \n",
    "<g/s> [EN_UPDATE_MODE]\n"
    "EN_UPDATE_MODE:\n'EN_UPDATE_MODE_NONE' 'EN_UPDATE_MODE_TFTP' 'EN_UPDATE_MODE_USB' 'EN_UPDATE_MODE_OAD'\n'EN_UPDATE_MODE_NET' 'EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED' 'EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED'\n"
);

U_BOOT_CMD(filepartload, 5, 0, do_file_part_load,
    "load part of a file to RAM",
    "<addr> <filename> [bytes]\n"
    "    - load file 'filename' to address 'addr'\n");

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD
(
    checkfile, CONFIG_SYS_MAXARGS, 0, do_check_file_partition,
    "check file exist in u disk,and set the partition.",
    "<Dir>\n"
    "    - check a file in U disk Dir exist or not\n"
);
#endif

#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)
U_BOOT_CMD
(
    UpdateNuttx, CONFIG_SYS_MAXARGS, 0, do_update_nuttx,
    "upgrade nuttx.",
    "<Dir>\n"
    "    - upgrade nuttx\n"
);
#endif
#ifdef HB_CUS
U_BOOT_CMD(
    cusid ,  6,    0,     do_checkOUIValid,
    "cusid check the image is release by the valid guys\n",
    "command cusid <oui> <swModel> <swVer> <ChipVersion> <BoardSel>\n"
);
#endif

