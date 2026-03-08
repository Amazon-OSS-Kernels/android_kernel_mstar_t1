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
#include <MsMmap.h>

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
	get_mmap, 2, 0,	do_get_mmap,
	"get memory info from supernova's mmap",
	"[ID]\n"
	"get_mmap -l:list all IDs that are list in mmap.ini"
	"example:\n"
	"#define E_MMAP_ID_COPROCESSOR_AVAILABLE  0x0000000000\n"
	"get_mmap E_MMAP_ID_COMPROCESSOR"
);
#endif

