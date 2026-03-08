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
#ifndef MS_DPRINTF_H
#define MS_DPRINTF_H

#include "MFE_chip.h"
// Switch for different debug messages
#define HAL_L1  (1/*0*/ << 0)
#define HAL_L2  (1/*0*/ << 1)
#define DRV_L1    (1/*0*/ << 4)    // ioctl
#define DRV_L2    (1/*0*/ << 5)    // CRC
#define DRV_L3  (1/*0*/ << 6)    // sem_wait/post, poll_wait/wake_up
#define DRV_L4  (1/*0*/ << 7)    // other messages
#define DRV_L5  (1/*0*/ << 8)    // dump_reg
#define DRV_L6  (1/*0*/ << 9)    // timer
#if defined(_WIN32)||!defined(_KERNEL_MODE_)
#define ADP_L1  (1/*0*/ << 16)
#define ADP_L2  (1/*0*/ << 17)
#define ADP_L3  (1/*0*/ << 18)
#define ADP_L4  (1/*0*/ << 19)
#define ADP_L0  (1/*0*/ << 20)
#define APP_L1 (1/*0*/ << 28)
// Marked out below line to disable all debug messages!
//#define DEBUG_LEVEL (ADP_L1|ADP_L2|ADP_L3|HAL_L1|HAL_L2|DRV_L1|DRV_L2|DRV_L3|DRV_L4|DRV_L5)
//#define DEBUG_LEVEL (ADP_L1|ADP_L2|ADP_L3)
#else
//#define DEBUG_LEVEL (HAL_L1|HAL_L2|DRV_L1|DRV_L2|DRV_L3|DRV_L4|DRV_L5)
#endif

#if !defined(DEBUG_LEVEL)
#define DEBUG_LEVEL 0
#endif

#if !defined(_KERNEL_MODE_)&&(DEBUG_LEVEL!=0)
#define ms_dprintk ms_dprintf
#elif defined(_MFE_T8_)
#include <linux/kernel.h>
#endif

#ifdef _MSC_VER

#if DEBUG_LEVEL == 0

static __inline void ms_dprintf(int level, const char *fmt, ...) {}

#else /* DEBUG_LEVEL */

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static __inline void ms_dprintf(int level, const char *fmt, ...)
{
    va_list ap;
    char msg[2048];

    if (!(level & DEBUG_LEVEL))
        return;

    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
#ifdef WINCE
    fprintf(stderr, "%s", msg);
#else
    OutputDebugStringA(msg);
#endif
    va_end(ap);
}

#endif /* DEBUG_LEVEL */

#else /* _MSC_VER */

#if DEBUG_LEVEL == 0

#define ms_dprintf(arg...) ((void) 0)
#define ms_dprintk(arg...) ((void) 0)

#else /* DEBUG_LEVEL */

#ifdef MIPSDEV

#include <cyg/infra/diag.h>

#define ms_dprintf(level, fmt, arg...) \
    do { \
        if (level & DEBUG_LEVEL) diag_printf(fmt, ##arg); \
    } while (0)

#define ms_dprintk(level, fmt, arg...) \
    do { \
        if (level & DEBUG_LEVEL) printk(fmt, ##arg); \
    } while (0)

#else /* MIPSDEV */

#define ms_dprintf(level, fmt, arg...) \
    do { \
        if (level & DEBUG_LEVEL) printf(fmt, ##arg); \
    } while (0)

#ifdef _KERNEL_MODE_
#define ms_dprintk(level, fmt, arg...) \
    do { \
        if (level & DEBUG_LEVEL) printk(fmt, ##arg); \
    } while (0)
#endif

#endif /* MIPSDEV */

#endif /* DEBUG_LEVEL */

#endif /* _MSC_VER */

#endif /* MS_DPRINTF_H */

