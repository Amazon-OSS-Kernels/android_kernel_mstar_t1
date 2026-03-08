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

#ifndef _SHARE_TYPE_H_
#define _SHARE_TYPE_H_

#include <config.h>
//-------------------------------------------------------------------------------------------------
//  Software Data Type
//-------------------------------------------------------------------------------------------------

struct bootloader_message {
    char command[32];
    char status[32];
    char recovery[768];
	// The 'recovery' field used to be 1024 bytes.  It has only ever
	// been used to store the recovery command line, so 768 bytes
	// should be plenty.  We carve off the last 256 bytes to store the
	// stage string (for multistage packages) and possible future
	// expansion.
	char stage[32];
	char slot_suffix[32];
	char reserved[192];
};

#define CMD_BUF 128
#define MAX_LINE_SIZE       CONFIG_SYS_CBSIZE
#define BOOT_MODE_RECOVERY_STR "boot-recovery\0"
#define BOOT_MODE_UPGRADEBOOTLOADER_STR "update-radio/hboot\0"
#if defined(CONFIG_AN_FASTBOOT_ENABLE)
#define BOOT_MODE_BOOTLOADER_STR "boot-bootloader\0"
#endif
#define HDMI_AUTO_WAKEUP_NAME    "hdmi_auto_wakeup"
#define POWER_KEY_NAME          "power_key"
#define IR_HEADER_CODE0_NAME    "ir_header_code0"
#define IR_HEADER_CODE1_NAME    "ir_header_code1"
#define TO_PHY_ADDR(x) ((U32)x&0x1FFFFFFF)

#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000

#define BIT16	    0x00010000
#define BIT17		0x00020000
#define BIT18		0x00040000
#define BIT19		0x00080000
#define BIT20		0x00100000
#define BIT21		0x00200000
#define BIT22		0x00400000
#define BIT23		0x00800000

#define BIT24		0x01000000
#define BIT25		0x02000000
#define BIT26		0x04000000
#define BIT27		0x08000000
#define BIT28		0x10000000
#define BIT29		0x20000000
#define BIT30		0x40000000
#define BIT31		0x80000000
/// data type unsigned char, data length 1 byte
typedef unsigned char   U8;                                 // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short  U16;                                // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned int    U32;                                // 4 bytes
/// data type unsigned long long, data length 8 byte
typedef unsigned long long    U64;                          // 8 bytes
/// data type signed char, data length 1 byte
typedef char    S8;                                 // 1 byte
/// data type signed short, data length 2 byte
typedef short   S16;                                // 2 bytes
/// data type signed int, data length 4 byte
typedef int S32;                                // 4 bytes

typedef U8 BOOLEAN;


#endif // _SHARE_TYPE_H_
