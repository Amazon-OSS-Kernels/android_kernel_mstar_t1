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
#include <MsUpgrade.h>
#include <MsUpgradeUtility.h>
#include <config.h>
#include <command.h>

#if (ENABLE_MODULE_NET==1)
U_BOOT_CMD(
    mstar ,    CONFIG_SYS_MAXARGS,    1,     do_mstar,
    "update kernal & root file system automatically by script file",
    "mstar [script_file_name]\n"
    "set CmdAbort = 0 in env to ignore the abort when cmd error\n"
);
#endif

#if (ENABLE_MODULE_USB == 1)

U_BOOT_CMD(
    ustar ,    CONFIG_SYS_MAXARGS,    1,     do_ustar,
    "ustar   - update kernal & root file system automatically by script file\n",
    "[script_file_name] \n"
    "    - when use this command \n"
    "    - at first use usb start \n"
    "    - then use ustar \n"
    "    -  \n"
);

#if !((CONFIG_MINIUBOOT) || (CONFIG_SECURE_USB_UPGRADE))
U_BOOT_CMD(
    udstar ,    CONFIG_SYS_MAXARGS,    1,     do_udstar,
    "Execute the script file that is stored in usb disk\n",
    "[script_file_name] \n"
    "If you don't input a file name, udstar will use the defatult file name \n"
);
#endif

#endif

#if (ENABLE_MODULE_NETUPDATE == 1)
U_BOOT_CMD(
    nstar ,    CONFIG_SYS_MAXARGS,    1,     do_nstar,
    "nstar   - update softwave system automatically via net\n",
   NULL
);
#endif

#if CONFIG_RESCUE_ENV
U_BOOT_CMD(
    bstar ,    CONFIG_SYS_MAXARGS,    1,     do_bstar,
    "bstar   - update softwave system automatically via backup\n",
    NULL
);
#endif

#if (ENABLE_MODULE_OAD == 1)
U_BOOT_CMD(
    ostar ,    CONFIG_SYS_MAXARGS,    1,     do_ostar,
    "ostar   - update softwave system automatically via oad\n",
    NULL
);
#endif

#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
U_BOOT_CMD(
    rstar ,    CONFIG_SYS_MAXARGS,    1,     do_rstar,
    "do env update from the lastest spi bank.\n",
    "    -  \n"
);
#endif

#if CONFIG_MINIUBOOT
#else
#if ( (ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_NETUPDATE == 1) || (ENABLE_MODULE_USB == 1))
U_BOOT_CMD(
    mbup ,    CONFIG_SYS_MAXARGS,    1,     do_mboot_upgrade,
    "mboot upgrade\n",
    "[mboot dram buffer addr] [mboot size] \n"
);
#endif
#endif

