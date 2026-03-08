/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <command.h>
#include <CusLED.h>
#include <config.h>

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    led ,    CONFIG_SYS_MAXARGS,    1,     do_led,
    "See led commands",
	"mbx   : [pattern] [index](only for 51 booting) \n"
    "time  : [delay time]  \n"
	"stop  : stop led  \n"
);

U_BOOT_CMD(
    led_red ,    CONFIG_SYS_MAXARGS,    1,     do_led_red,
    "light on led red\n",
    NULL
);

U_BOOT_CMD(
    led_green ,  CONFIG_SYS_MAXARGS,    1,     do_led_green,
    "light on led green\n",
    NULL
);
#endif
