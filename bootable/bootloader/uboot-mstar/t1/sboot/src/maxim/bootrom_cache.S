/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef _BOOT_INC_
#include "boot.inc"
#endif

    .text

    .global     BootRom_InitCache
BootRom_InitCache:
    mrc         p15, 0, r0, c1, c0, 0
    orr         r0, r0, #(0x1 << 12) | (1 << 11)    @//Enable L1 I-Cache and Branch Prediction
    isb
    mcr         p15, 0, r0, c1, c0, 0
    isb
    dsb

    bx          lr

    .size       BootRom_InitCache,.-BootRom_InitCache

    .end

