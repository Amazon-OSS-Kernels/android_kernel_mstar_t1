/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************
#ifndef MS_DPRINTF_H
#define MS_DPRINTF_H

#include "MFE_chip.h"
#include "ULog.h"

// Switch for different debug messages
#define HAL_L0  (1UL /*0*/ << 0)    // error messages
#define HAL_L1  (1UL /*0*/ << 1)    // info messages
#define HAL_L2  (1UL /*0*/ << 2)    // debug messages
#define HAL_L3  (1UL /*0*/ << 3)    // more debug messages
#define HAL_L4  (1UL /*0*/ << 4)    // warning messages
#define DRV_L0  (1UL /*0*/ << 8)    // error messages
#define DRV_L1  (1UL /*0*/ << 9)    // info messages
#define DRV_L2  (1UL /*0*/ << 10)   // debug messages
#define DRV_L3  (1UL /*0*/ << 11)   // more debug messages
#define DRV_L4  (1UL /*0*/ << 12)   // warning messages
#define DRV_L5  (1UL /*0*/ << 13)   // debug messages (dump_reg)
#define DRV_L6  (1UL /*0*/ << 14)   // debug messages (timer)
#define DRV_L7  (1UL /*0*/ << 15)   // debug messages (VT rate control)
#define ADP_L0  (1UL /*0*/ << 16)   // error messages
#define ADP_L1  (1UL /*0*/ << 17)   // info messages
#define ADP_L2  (1UL /*0*/ << 18)   // debug messages
#define ADP_L3  (1UL /*0*/ << 19)   // more debug messages
#define ADP_L4  (1UL /*0*/ << 20)   // warning messages
#define APP_L0  (1UL /*0*/ << 24)   // fatal error messages

#define MFE_TRACE    (HAL_L3|DRV_L3|ADP_L3)
#define MFE_DEBUG    (HAL_L2|DRV_L2|ADP_L2)
#define MFE_INFO     (HAL_L1|DRV_L1|ADP_L1)
#define MFE_WARNING  (HAL_L4|DRV_L4|ADP_L4)
#define MFE_ERROR    (HAL_L0|DRV_L0|ADP_L0|APP_L0)

// Marked out below line to disable all debug messages!
#if defined(_WIN32)||!defined(_KERNEL_MODE_)
#define DEBUG_LEVEL (MFE_ERROR|MFE_WARNING|MFE_INFO) // info
//#define DEBUG_LEVEL (MFE_ERROR|MFE_WARNING|MFE_INFO|MFE_DEBUG) // debug
//#define DEBUG_LEVEL (MFE_ERROR|MFE_WARNING|MFE_INFO|MFE_DEBUG|DRV_L5) // debug & dump reg
//#define DEBUG_LEVEL (MFE_ERROR|MFE_WARNING|MFE_INFO|MFE_DEBUG|MFE_TRACE) // trace
//#define DEBUG_LEVEL (MFE_ERROR|MFE_WARNING|MFE_INFO|MFE_DEBUG|MFE_TRACE|DRV_L5) // trace & dump reg
#else
//#define DEBUG_LEVEL (MFE_ERROR|MFE_WARNING|MFE_INFO) // default
#endif

#if !defined(DEBUG_LEVEL)
#define DEBUG_LEVEL 0UL
#endif

#if !defined(_KERNEL_MODE_)&&(DEBUG_LEVEL!=0)
#define ms_dprintk ms_dprintf
#elif defined(_MFE_T8_)
//#include <linux/kernel.h>
#endif

#ifdef _MSC_VER

#if DEBUG_LEVEL == 0
#define ms_dprintk ms_dprintf
static __inline void ms_dprintf(MS_S32 level, const MS_S8 *fmt, ...) {}

#else /* DEBUG_LEVEL */

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static __inline void ms_dprintf(MS_S32 level, const MS_S8 *fmt, ...)
{
    va_list ap;
    MS_S8 msg[2048];

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

#define ms_dprintf(level, fmt, arg...)    \
    do {                                  \
        if (!(level & DEBUG_LEVEL))       \
            break;                        \
        if (level & MFE_TRACE) {          \
            ULOGD("MFE_EX", fmt, ##arg);  \
        }                                 \
        else if (level & MFE_DEBUG) {     \
            ULOGD("MFE_EX", fmt, ##arg);  \
        }                                 \
        else if (level & MFE_INFO) {      \
            ULOGI("MFE_EX", fmt, ##arg);  \
        }                                 \
        else if (level & MFE_WARNING) {   \
            ULOGW("MFE_EX", fmt, ##arg);  \
        }                                 \
        else if (level & MFE_ERROR) {     \
            ULOGE("MFE_EX", fmt, ##arg);  \
        }                                 \
    } while (0)

#endif /* DEBUG_LEVEL */

#endif /* _MSC_VER */

#endif /* MS_DPRINTF_H */

