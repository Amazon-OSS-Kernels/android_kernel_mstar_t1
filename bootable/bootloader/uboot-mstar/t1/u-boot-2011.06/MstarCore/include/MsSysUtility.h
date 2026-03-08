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

#ifndef __CMD_MS_SYS_UTILITY__
#define __CMD_MS_SYS_UTILITY__
#include <MsTypes.h>

MS_U8 ReadByte(MS_U32 u32RegAddr);
MS_U16 Read2Byte(MS_U32 u32RegAddr);
MS_BOOL WriteByte(MS_U32 u32RegAddr, MS_U8 u16Val);
MS_BOOL Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL WriteRegBit(MS_U32 u32RegAddr, MS_U16 u8Bit, MS_BOOL bEnable);
MS_BOOL WriteRegBitPos(MS_U32 u32RegAddr, MS_U8 u8Bit, MS_BOOL bEnable);

int do_riu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_gpio( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
int do_sar( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);

#endif

