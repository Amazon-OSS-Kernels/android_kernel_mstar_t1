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
#include <MsEeprom.h>


U_BOOT_CMD(
	eeprom,	5,	1,	do_eeprom,
	"EEPROM sub-system",
	"wb [eeprom offset] [value(byte)] : write 1 byte to eeprom\n"
	"eeprom rb [eeprom offset] : read 1 byte to eeprom\n"
	"eeprom ws [eeprom offset] [string] : write string to eeprom\n"
    "eeprom dump [eeprom offset] [cnt] :dump eeprom information\n"
	"eeprom read [addr] [eeprom offset] [Read length]\n"
	"eeprom write [addr] [eeprom offset] [Write length]\n"
	"eeprom erase [eeprom_size]\n"
	"eeprom switch [DeviceID]\n"
	"eeprom list : show the register device information"
);
