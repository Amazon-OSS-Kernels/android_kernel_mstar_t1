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
#include <command.h>
#include <common.h>
#include <MsTest.h>
#include <MsEnvironment.h>
#include <MsApiPM.h>

#if CONFIG_MINIUBOOT
#else

#if defined (CONFIG_POINTS_HANDLER_ENABLE)
U_BOOT_CMD(
    add_bootcp, CONFIG_SYS_MAXARGS, 1,    do_add_bootcheckpoints,
    "add_bootcp   - Perform to enable the booting check-points(reboot 15 times, then auto disable)\n",
    NULL
);
#endif

U_BOOT_CMD(
    testmode , CONFIG_SYS_MAXARGS, 1,    do_testmode_set ,
    "set testmode into bootargs.",
    "testmode <mode>\n"
    "mode:autotest,moduletest,hsl,mdebugtest"
);

#endif

U_BOOT_CMD(
    bootargs_set,    CONFIG_SYS_MAXARGS,    1,     do_set_bootargs,
    "Set info exchange and set to boot args. ",
    "    -  \n"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    config2env,    CONFIG_SYS_MAXARGS,    1,     do_config2env,
    "Set config to environment.",
    "    -  \n"
);
#endif

#if (CONFIG_ENABLE_RTPM)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    runtime_pm, CONFIG_SYS_MAXARGS, 1,    do_run_time_pm,
    "if boot to PM\n",
    NULL
);
#endif

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    dc, 2, 1, do_del_boogargs_cfg,
    "delete the specific cofig that is in the bootargs\n",
    "[config]\n"
    "For example: dl autotest\n"
);

U_BOOT_CMD(
    ac, 3, 1, do_set_bootargs_cfg,
    "set a new config to the bootargs\n",
    "[config][content]\n"
    "For example: add autotest true\n"
);

#ifdef CONFIG_ENABLE_SYNC_MMAP
U_BOOT_CMD(
    sync_mmap, 1, 1, do_sync_mmap_to_env,
    "Sync the some id's address and len to env and bootargs from supernova's mmap\n",
    "\n"
);
#endif

U_BOOT_CMD(
    m2e, 4, 1, do_mmap_to_env,
    "Restore the address and len to env from supernova's mmap\n",
    "[ID] [n1] [n2]"
    "For example:"
    "ID:E_MMAP_ID_PM51_USAG"
    "n1: This is an name of env, this env store the addr for ID"
    "n2: This is an name of env, this env store the len for ID"
    "\n"
);
#endif

