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

#include <command.h>
#include <MsApiFat.h>

#if (ENABLE_MODULE_FAT==1)

U_BOOT_CMD(
    fatfilesize, 5, 0, do_fat_fsfilesize,
    "fatfilesize - load binary file from a dos filesystem\n",
    "<interface> <dev[:part]> <filename> <filesize>\n"
    "    - load binary file 'filename' from 'dev' on 'interface'\n"
    "      to address 'addr' from dos filesystem\n"
);

U_BOOT_CMD(
	fatpartload,	7,	0,	do_fat_fspartload,
	"fatpartload - load binary file from a dos filesystem\n",
	"<interface> <dev[:part]>  <addr> <filename> <offset> [bytes]\n"
	"    - load binary file 'filename' from 'dev' on 'interface'\n"
	"      to address 'addr' from dos filesystem\n"
);
#if(ENABLE_MODULE_FAT_WRITE==1)
U_BOOT_CMD(
    fatwrite,    6,    0,    do_fat_fswrite,
    "fatwrite - write binary file to a dos filesystem\n",
    "<interface> <dev[:part]>  <addr> <filename> [bytes]\n"
    "    - load binary file 'filename' from 'dev' on 'interface'\n"
    "      to address 'addr' from dos filesystem\n"
);
#endif

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    filelisttest,    1,    0,    do_fat_fileList_test,
    "This command is only for file list test\n",
    "filelisttest\n"
);

U_BOOT_CMD(
    filelist,    1,    0,    do_dump_fileList,
    "Dump the file list.\n",
    "filelist\n"
);
#endif

#endif
