/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
// File Name: mhal_DDC2BI.c
// Description: For Slave I2C functions.
//
// Registers read and write command list:
// DATA WRITE : START + 0x6E + 0x10 + addrh + addrl + data + STOP
// DATA READ  : START + 0x6E + 0x10 + addrh + addrl + START + 0x6F + data +STOP
//
// Functions read and write command list:
// write contrast   : START + 0x6E + 0x20 +  01  +   xxh +   data  + STOP
// read  contrast   : START + 0x6E + 0x20 +  01  +   xxh +   START + 0x6F + (data) + STOP
// write Brightness : START + 0x6E + 0x20 +  02  +   xxh +   data  + STOP
// read  Brightness : START + 0x6E + 0x20 +  02  +   xxh +   START + 0x6F + (data) + STOP
// ON/OFF Panel set : START + 0x6E + 0x20 +  10  +   xxh +   0:OFF 1:ON  + STOP
// ON/OFF Panel Rd  : START + 0x6E + 0x20 +  10  +   xxh +   START + 0x6F + (data) + STOP
//
////////////////////////////////////////////////////////////////////////////////


#define _MHAL_DDC2BI_C_

#include <string.h>
#include "MsCommon.h"

extern MS_U32 DDC2BI_RIU_BASE;

void mhal_DDC2BI_init_riu_base(MS_U32 u32riu_base, MS_U32 u32PMriu_base)
{
    DDC2BI_RIU_BASE = u32PMriu_base;
}

