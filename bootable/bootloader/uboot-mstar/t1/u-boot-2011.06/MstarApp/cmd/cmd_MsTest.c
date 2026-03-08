/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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
#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    msg, CONFIG_SYS_MAXARGS, 1,    do_msg,
    "print string - msg [string]\n",
    NULL
);
#endif
#ifdef CONFIG_DISPLAY_VERSIONINFO
U_BOOT_CMD(
    mversion, CONFIG_SYS_MAXARGS, 1, mversion,
    "show changelist - mversion \n",
    NULL
);
#endif

U_BOOT_CMD(
    gettime, CONFIG_SYS_MAXARGS, 1,    do_gettime,
    "gettime   - Get the system executing time\n",
    NULL
);

#if defined(CONFIG_SECURITY_BOOT) && defined(CONFIG_SECURE_HW_IP)
U_BOOT_CMD(
    secure_driver_check, CONFIG_SYS_MAXARGS, 1,    do_secure_driver_check,
    "gettime   - Get the system executing time\n",
    NULL
);
#endif

U_BOOT_CMD(
    CmdPerformanceTest, CONFIG_SYS_MAXARGS, 1,    do_cmdperformancetest,
    "gettime   - Get the system executing time\n",
    NULL
);
