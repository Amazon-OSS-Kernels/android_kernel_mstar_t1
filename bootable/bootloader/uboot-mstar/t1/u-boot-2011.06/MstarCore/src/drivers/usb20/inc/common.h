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

#ifndef __COMMON_H__
#define __COMMON_H__

#include <linux/string.h>
#include "blockdev.h"
#include <config.h>
#include <usb.h>

#define __DEBUG_ON__

#if defined(__DEBUG_ON__)
	#include <stdio.h>
	#define	MS_MSG	printf
#else
	#define	MS_MSG(fmt, ...)
#endif

/* Debuging Message Switch */
#define USB_LIB_DEBUG	1
//#define USB_LIB_BULK_DEBUG	1
#define USB_GLUE_DEBUG	1

#endif /* __COMMON_H__ */
