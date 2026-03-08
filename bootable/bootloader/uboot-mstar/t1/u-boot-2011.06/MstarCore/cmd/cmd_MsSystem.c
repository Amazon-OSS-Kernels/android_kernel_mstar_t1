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
#include <MsSystem.h>

U_BOOT_CMD(
    wdt_enable, CONFIG_SYS_MAXARGS, 2,    do_wdt_enable,
    "wdt_enable n - set Watchdog timer to n secs\n",
    NULL
);

U_BOOT_CMD(
	du,1,        1,	do_disable_uart,
	"du  - Disable UART\n",
	"command: du\n"
);

#if CONFIG_RESCUE_ENV
U_BOOT_CMD(
    BrickTerminator ,    CONFIG_SYS_MAXARGS,    0,     do_BrickTerminator_Main,
    "BrickTerminator",
    "    - BrickTerminator [recover]\n"
);
#endif
