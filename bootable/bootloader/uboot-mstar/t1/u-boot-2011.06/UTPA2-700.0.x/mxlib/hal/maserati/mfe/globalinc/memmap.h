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
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////


//                       width  height  MBs
//h.264 level4.1, 2Kx1K	 2048	1024	8192
#ifndef _MEMMAP_H_
#define _MEMMAP_H_

#if defined(_MIPS_PLATFORM_) && defined(_MFE_BIG2_)
#include <sys/bsdtypes.h>
#elif defined(_MIPS_PLATFORM_) && defined(_MFE_T8_)
#endif
#include "mfe_type.h"

#if defined(_AEON_PLATFORM_) && defined(_MFE_T8_)
	#define DRAM_BASE       0xae3000//0xae3000 //0xafe000//we use DIP buffer temporally //0
	#define MEM_LOGMAPPHY   0x80000000
#elif defined(_MIPS_PLATFORM_) && defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
	#define DRAM_BASE       0x08100000//0xae3000 //0xafe000//we use DIP buffer temporally //0
	#define MEM_LOGMAPPHY   0xA0000000 //0xB0000000 A:Mstar board MIU0, B:MIU1//use Memory after 16MB  A:put in cache, 8: no in cache
#elif defined(_MIPS_PLATFORM_) && defined(_MFE_T8_)
	#define DRAM_BASE       0x08100000//0xae3000 //0xafe000//we use DIP buffer temporally //0
	#define MEM_LOGMAPPHY   0xA0000000//0xB0000000 A:Mstar board MIU0, B:MIU1//use Memory after 16MB  A:put in cache, 8: no in cache
#elif defined(_AEON_PLATFORM_)
	#define DRAM_BASE       0xae3000//0xae3000 //0xafe000//we use DIP buffer temporally //0
	#define MEM_LOGMAPPHY   0x80000000
#elif defined(_TRACE32_CMM_)
	#define DRAM_BASE       0x80000//0xA0080000//0
	#define MEM_LOGMAPPHY   0xA0000000
#elif defined(_MFE_BIG2_) && defined(_HIF_)
	#define DRAM_BASE       0x130000 //0x10080000//0xB0080000//0
    //this should be start from 0, but i only tried start from 0x20000
	#define MEM_LOGMAPPHY   0x10080000
#elif defined(_MFE_BIG2_) && defined(_MIPS_PLATFORM_)
	#ifdef _USE_PSRAM_
		#define DRAM_BASE       0x130000
		#define MEM_LOGMAPPHY   0xA4080000
		#define BANK_SEL        1
	#else
		#define DRAM_BASE       0x130000
		#define MEM_LOGMAPPHY   0xB0080000
		#define BANK_SEL        4
	#endif
#else
	#define DRAM_BASE       0x20000//0x10080000//0xB0080000//0
	#define MEM_LOGMAPPHY   0//0x10080000
#endif

#ifdef _WIN32 //!defined(_AEON_PLATFORM_) && !defined(_MIPS_PLATFORM_)
	#define MAX_MFE_MEMORY_SIZE	(8*1024*1024)
#endif

#endif
