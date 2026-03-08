/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************


#ifndef __ARM_TYPES_H
#define __ARM_TYPES_H

#include <MsTypes.h>

/* Dma addresses are 32-bits wide.  */
typedef MS_U32 dma_addr_t;
typedef MS_U32 dma64_addr_t;

typedef void irqreturn_t;

#if 0
typedef U32	size_t;

#ifndef ssize_t
typedef cyg_uint64 ssize_t;
#endif
#endif

#endif

