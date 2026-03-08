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
#ifndef _MDRV_MFE_H_
#define _MDRV_MFE_H_

//#define _CRC_PATTERN_GEN_	//check CRC pattern which from C MODEL

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)

#elif defined(_MFE_BIG2_)&&defined(_MIPS_PLATFORM_)
#include <sys/bsdtypes.h>
#include "pthread_map.h"
#else //defined(_WIN32)

#endif

typedef enum {
    WAIT_FRAME_DONE = 0,
    WAIT_INPUT_FRAME = 1,
    WRITE_MFE_REG = 2,
    WAIT_AVAIL_BITSBUF = 3,
} MFE_STAT;


#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
struct MFE_Dev {
	struct cdev cdev;	  // Char device structure
};
#endif

#endif



