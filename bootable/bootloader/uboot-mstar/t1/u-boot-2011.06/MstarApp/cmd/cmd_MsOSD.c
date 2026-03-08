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
#include <common.h>
#include <command.h>
#include <MsOSD.h>
#if (ENABLE_MODULE_DISPLAY_OSD==1)
U_BOOT_CMD(
    osd_create,    3,   1,  do_create_osdlayer,
    "osd_create - create osd layer \n",
    "create osd layer \n"
    "    - w: in Dec\n"
    "    - h: in Dec\n"
);

U_BOOT_CMD(
    osd_destroy,    1,   1,  do_destroy_osdlayer,
    "osd_destroy - destroy osd layer \n",
    "destroy osd layer \n"
);

U_BOOT_CMD(
    osd_flush,    1,   1,  do_flush2screen_osdlayer,
    "osd_flush - flush canvas to screen \n",
    "flush canvas to screen \n"
);

U_BOOT_CMD(
    draw_rect,    6,   1,  do_draw_rect,
    "draw_rect - draw rect with color \n",
    "draw rect \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - w: in Dec\n"
    "    - h: in Dec\n"
    "    - color: in Hex\n"
);

U_BOOT_CMD(
    draw_pixel,    4,   1,  do_draw_pixel,
    "draw_pixel - draw a pixel with color \n",
    "draw a pixel \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
);

U_BOOT_CMD(
    draw_string,    100,   1,  do_draw_string,
    "draw_string - draw string with color \n",
    "draw string \n"
    "    - string: in char\n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
    "    - attrib: align attrib, 0: left, 1: middle, 2: right\n"
);

U_BOOT_CMD(
    draw_progress,    5,   1,  do_draw_Progress,
    "draw_string - draw string with color \n",
    "draw string \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
    "    - per: percent in Dec\n"
);

U_BOOT_CMD(
    draw_jpg, CONFIG_SYS_MAXARGS, 1,    do_draw_jpg,
    "blt_jpg   - Blt JPG\n",
    NULL
);
#endif
