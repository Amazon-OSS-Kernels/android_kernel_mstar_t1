/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <command.h>
#include <CusUpgrade.h>
#include <config.h>


#if (ENABLE_MODULE_USB == 1)
U_BOOT_CMD(
    custar ,    CONFIG_SYS_MAXARGS,    1,     do_custar,
    "do usb update from the specified file that is in usb.\n",
    "    -  \n"
);
U_BOOT_CMD(
    ota_zip_check ,    CONFIG_SYS_MAXARGS,    1,     do_ota_zip_check,
    "do OTA zip package check.\n",
    "    -  \n"
);

U_BOOT_CMD(
    usb_bin_check ,    CONFIG_SYS_MAXARGS,    1,     do_usb_bin_check,
    "do usb bin document check.\n",
    "    -  \n"
);
#endif

#if (ENABLE_MODULE_OAD == 1)
U_BOOT_CMD(
    costar ,    CONFIG_SYS_MAXARGS,    1,     do_costar,
    "do oad update from the specified file that is in oad partition\n",
    "    -  \n"
);
#endif

#if (ENABLE_MODULE_NETUPDATE== 1)
U_BOOT_CMD(
    cnstar ,    CONFIG_SYS_MAXARGS,    1,     do_cnstar,
    "do net update from the specified file that is in oad partition\n",
    "    -  \n"
);
#endif

U_BOOT_CMD(
    recovery_wipe_partition ,    CONFIG_SYS_MAXARGS,    1,     do_wipe_datacache_check,
    "do recovery wipe data or cache.\n",
    "    -  \n"
);
#if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
U_BOOT_CMD(
    SystemRestore ,    CONFIG_SYS_MAXARGS,    1,     do_systemrestore,
    "do restore system by OTA upgrade package in systembackup partition.\n",
    "    -  \n"
);
#endif


#if defined (CONFIG_URSA_6M40)
U_BOOT_CMD(
    ursa7_upgrade ,    CONFIG_SYS_MAXARGS,    1,     do_ursa7_upgrade,
    "ursa7 upgrade from USB\n",
    "    -  \n"
);

U_BOOT_CMD(
    ursa7_reset ,    CONFIG_SYS_MAXARGS,    1,     do_ursa7_reset,
    "ursa7 upgrade from USB\n",
    "    -  \n"
);
#endif

