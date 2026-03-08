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
#include <MsUtility.h>

#if(ENABLE_MODULE_COMPRESS==1)
U_BOOT_CMD(
	mscompress7,	 6, 0, do_mscompress7,
	"Compress or decompress lzma files",
	"\n"
	"Usage:  lzma <e|d> <index> Src_Address Src_Length Dst_Address\n"
	"      e: encode file\n"
	"      d: decode file\n"
	"      <index> start to (de)compress position\n"
);
#endif

U_BOOT_CMD(
	delay,	 6, 0, do_mdelay,
	"delay time, time unit is ms\n",
	"Usage: delay [time]."
);

#if ((ENABLE_MODULE_USB == 1)&&(ENABLE_MODULE_FAT==1))
#if (ENABLE_MODULE_FAT_WRITE==1)
U_BOOT_CMD(
    spi2usb, 4, 0, do_spi2usb,
    "Read data from spi to usb",
    "[spi offset] [length] [output file name]\n"
);
#endif

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    usb2spi, 4, 0, do_usb2spi,
    "write data from usb to spi",
    "[spi offset] [length] [intput file name]\n"
);
#endif

#endif

#if ((ENABLE_MODULE_USB == 1)&&(ENABLE_MODULE_EEPROM==1))
U_BOOT_CMD(eeprom2usb, 4, 0, do_eeprom2usb,
    "dump data from eeprom to usb",
    "<eeprom offset> <len> <output file name>\n"
    );


U_BOOT_CMD(usb2eeprom, 4, 0, do_usb2eeprom,
    "write data from usb to eeprom",
    "<eeprom offset> <len> <intput file name>\n"
    );

#endif

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_PROGRAM)
U_BOOT_CMD(nandprogramforrom, 1, 0, do_nandprogramforrom,
    "program nand for rom boot",
    "\n"
    );
#endif

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_CMDLINE)
U_BOOT_CMD(skipnandprogramforrom, 1, 0, do_skipnandprogramforrom,
    "enter cmd line when use tv tool for nand program",
    "\n"
    );
#endif

U_BOOT_CMD(showversion, 1, 0, do_showVersion,
    "Show version",
    "\n"
    );