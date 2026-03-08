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

#ifndef __CMD_MS_EEPROM__
#define __CMD_MS_EEPROM__

#include <MsTypes.h>

#define EEPROM_PAGESIZE           0x10


void eeprom_switch(int i);   // base 0

int do_eeprom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
//-------------------------------------------------------------------------------------------------
/// Read bytes from the Eeprom device
/// @param  offset                                   \b IN: the offset in the Eeprom device
/// @param  pBuf                                      \b IN: the read data dram buffer
/// @param  u16BufLen                                  \b IN: the read data length
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int eeprom_PageRead(MS_U16 offset, MS_U8 *pBuf, MS_U16 u16BufLen);

//-------------------------------------------------------------------------------------------------
/// write bytes to the Eeprom device
/// @param  offset                                   \b IN: the offset in the Eeprom device
/// @param  pBuf                                      \b IN: the write data dram buffer
/// @param  u16BufLen                                  \b IN: the write data length
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int eeprom_PageWrite(MS_U16 offset, MS_U8 *pBuf, MS_U16 u16BufLen);

//-------------------------------------------------------------------------------------------------
/// Read 1 byte from the Eeprom device
/// @param  offset                                   \b IN: the offset in the Eeprom device
/// @return  MS_U8                                    \b OUT: the read byte value
//-------------------------------------------------------------------------------------------------
MS_U8 eeprom_ReadByte(MS_U16 offset);
//-------------------------------------------------------------------------------------------------
/// Write 1 byte to the Eeprom device
/// @param  offset                                   \b IN: the offset in the Eeprom device
/// @return  u8Value                                  \b OUT: the write byte value
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int eeprom_WriteByte(MS_U16 offset,MS_U8 u8Value);

#endif

