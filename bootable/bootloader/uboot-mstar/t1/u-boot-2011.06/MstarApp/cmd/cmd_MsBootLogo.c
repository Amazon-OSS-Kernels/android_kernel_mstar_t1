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
#include <bootlogo/MsBootLogo.h>

#if (!(CONFIG_LOGO_STORE_IN_MBOOT) && !(CONFIG_LOGO_STORE_NOT_SET) && !(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN))
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    dbtable_init, CONFIG_SYS_MAXARGS, 1,    do_dbtable_init,
    "dbtable_init   - let data  is ready (logo, music, panel, ursa).\n",
    NULL
);
#endif

#if (ENABLE_MODULE_DISPLAY_LOGO==1)
U_BOOT_CMD(
    bootlogo, CONFIG_SYS_MAXARGS, 1,    do_bootlogo,
    "bootlogo   - Display Logo_Music\n",
    NULL
);
#endif

#if (ENABLE_MODULE_POWER_MUSIC==1)
U_BOOT_CMD(
    audio_preinit, CONFIG_SYS_MAXARGS, 1,    do_audiopreinit,
    "audio_preinit   - Init audio driver\n",
    NULL
);

U_BOOT_CMD(
    bootmusic, CONFIG_SYS_MAXARGS, 1,    do_bootmusic,
    "bootmusic   - play music\n",
    NULL
);
#endif




