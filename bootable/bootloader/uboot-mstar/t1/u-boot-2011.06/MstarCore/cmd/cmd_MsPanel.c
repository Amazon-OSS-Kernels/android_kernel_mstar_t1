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
#include <common.h>
#include <command.h>

#if (CONFIG_PANEL_INIT)
#include <MsApiPanel.h>


#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    panel_pre_init, 2, 1, do_panel_pre_init,
    "panel_pre_init  - init panel by panel.ini\n",
    "command: panel_pre_init [option]\n"
    " -s : static init : panel init para from uboot\n"
    " -d : dynamic init : panel init para from SN"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    panel_init, 2, 1, do_panel_init,
    "panel_init  - init panel by panel.ini\n",
    "command: panel_init [option]\n"
    " -s : static init : panel init para from uboot\n"
    " -d : dynamic init : panel init para from SN"
);
#endif

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    panel_post_init, CONFIG_SYS_MAXARGS, 0, do_backLigth_on,
    "backligth on  - backlight on\n",
    "command: backlight_on\n"
);

#endif

#if (CONFIG_LOCAL_DIMMING)
U_BOOT_CMD(
    local_dimming,    CONFIG_SYS_MAXARGS,    2,     do_local_dimming,
    "do local dimming setting.",
    "    -  \n"
);

#endif
