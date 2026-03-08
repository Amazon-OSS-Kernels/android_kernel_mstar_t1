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
#include <config.h>

#if (CONFIG_BOOTVIDEO == 1)
#include <command.h>
#include <bootvideo/MsBootVideo.h>

U_BOOT_CMD(
    bootvideo, CONFIG_SYS_MAXARGS, 1,    do_bootvideo,
    "bootvideo   - Display Logo_Video\n",
    NULL
);
#endif  // #if (CONFIG_BOOTVIDEO == 1)
