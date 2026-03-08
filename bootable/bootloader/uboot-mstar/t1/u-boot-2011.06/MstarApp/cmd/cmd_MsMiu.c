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

#include <common.h>
#include <command.h>

#include <MsApiMiu.h>

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    kernelProtect, 1, 1, do_MsApi_kernelProtect,
    "kernelProtect  - Protect kernel\n",
    "command: kernelProtect\n"
);

U_BOOT_CMD(
    pmProtect, 1, 1, do_MsApi_RunTimePmProtect,
    "runtime pm Protect  - Protect runtime PM\n",
    "command: pmProtect\n"
);

U_BOOT_CMD(
    kernelProtectBist, 1, 1, do_MsApi_kernelProtectBist,
    "kernelProtectBist  - Protect kernel bist\n",
    "command: kernelProtectBist\n"
);

U_BOOT_CMD(
    miuProtect, 1, 1, do_MsApi_MiuProtect,
    "miuProtect  - Protect miu\n",
    "command: miuProtect\n"
);


U_BOOT_CMD(
    nuttxProtect, 3, 1, do_MsApi_NuttxProtect,
    "nuttx Protect  - Protect nuttx\n",
    "command: nuttxProtect [start addr] [end addr]\n"
);
#endif

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    updatemiureg ,    CONFIG_SYS_MAXARGS,    1,     do_set_miu,
    "Call driver to update miu setting",
    "    - By Env MIU0(1)_GROUP_PRIORITY \n"
);
