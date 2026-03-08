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
#include <MsRawIO.h>

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    init_raw_io, CONFIG_SYS_MAXARGS, 0, do_raw_io_init,
    "init raw_io module",
    " \n"
);

#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    config_raw_io, CONFIG_SYS_MAXARGS, 0, do_raw_io_config,
    "Config the target device for raw I/O",
    "SPI \n"
	"config_raw_io NAND [partition] [volume]\n"
	"config_raw_io MMC [volume]\n"
	"If you don't input any parameters, uboot would use the default settings\n"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    raw_io_status, CONFIG_SYS_MAXARGS, 0, do_raw_io_status,
    "get raw_io status",
    " \n"
);

U_BOOT_CMD(
    push_raw_io_config, CONFIG_SYS_MAXARGS, 0, do_raw_io_config_push,
    "push raw_io current config",
    " \n"
);

U_BOOT_CMD(
    pop_raw_io_config, CONFIG_SYS_MAXARGS, 0, do_raw_io_config_pop,
    "pop raw_io last config",
    " \n"
);

U_BOOT_CMD(
    raw_read, CONFIG_SYS_MAXARGS, 0, do_raw_read,
    "Read the raw datas that store in the target device\n"
    "Yo have to execute 'config_raw_io' before using this cmd\n",
    "[dram addr] [offset] [len] \n"
	"unit:bytes\n"
);

U_BOOT_CMD(
    raw_write, CONFIG_SYS_MAXARGS, 0, do_raw_write,
    "Write the raw datas that store in the target device"
    "Yo have to execute 'config_raw_io' before using this cmd\n",
    "[dram addr] [offset] [len] \n"
	"unit:bytes\n"
);

#endif

