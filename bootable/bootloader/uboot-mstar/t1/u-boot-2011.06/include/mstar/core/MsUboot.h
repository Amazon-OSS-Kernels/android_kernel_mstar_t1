/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   MsUboot.h
/// @brief  MStar Common Interface Header File
/// @author MStar Semiconductor Inc.
/// @note   MsUboot.h includes most command header files including basic data type, macro(MsTypes.h),\n
/// board configuration(MsBoard.h), and OS related API(MsOS.h).\n
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MS_UBOOT_H_
#define _MS_UBOOT_H_

#define BIT0			                 0x01
#define BIT1			                 0x02
#define BIT2			                 0x04
#define BIT3			                 0x08
#define BIT4			                 0x10
#define BIT5			                 0x20
#define BIT6			                 0x40
#define BIT7			                 0x80

#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)

extern unsigned long MS_RIU_MAP;
#define RIU     ((unsigned short volatile *) MS_RIU_MAP)
#define RIU8    ((unsigned char  volatile *) MS_RIU_MAP)

#define MDrv_WriteByte(u32Reg, u8Value)     RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Value
#define MDrv_ReadByte(u32Reg)                   RIU8[(u32Reg << 1) - (u32Reg & 1)]

extern unsigned long RIU_MAP;
extern void MDrv_Sys_WholeChipReset(void);

#endif /*_MS_UBOOT_H_*/

