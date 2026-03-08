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
#ifndef __ARCH_CONFIG_H__
#define __ARCH_CONFIG_H__

#define ARCH_UMP_BACKEND_DEFAULT            1           /* OS_MEMORY */
#define ARCH_UMP_MEMORY_ADDRESS_DEFAULT     0x00000000  /* only required for the dedicated memory allocator */
#define ARCH_UMP_MEMORY_SIZE_DEFAULT        (512 << 20) /* 512MB */

#endif /* __ARCH_CONFIG_H__ */
