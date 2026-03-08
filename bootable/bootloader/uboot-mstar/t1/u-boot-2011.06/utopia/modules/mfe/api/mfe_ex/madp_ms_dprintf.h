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
#ifndef MADP_MS_DPRINTF_H
#define MADP_MS_DPRINTF_H
#if defined(_WIN32)||!defined(_KERNEL_MODE_)
#include "ms_dprintf.h"

#else
// Switch for different debug messages
#define ADP_L1  (1UL /*0*/ << 16)
#define ADP_L2  (1UL /*0*/ << 17)
#define ADP_L3  (1UL /*0*/ << 18)
#define ADP_L4  (1UL /*0*/ << 19)
#define APP_L0  (1UL /*0*/ << 28)


// Marked out below line to disable all debug messages!
//#define ADP_DEBUG_LEVEL (ADP_L1|ADP_L2|APP_L0|ADP_L3)
//#define ADP_DEBUG_LEVEL (ADP_L4)
#if !defined(ADP_DEBUG_LEVEL)
#define ADP_DEBUG_LEVEL 0UL
#endif


#ifdef _MSC_VER

#if ADP_DEBUG_LEVEL == 0

static __inline void ms_dprintf(MS_S32 level, const MS_S8 *fmt, ...) {}

#else /* ADP_DEBUG_LEVEL */

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static __inline void ms_dprintf(MS_S32 level, const MS_S8 *fmt, ...)
{
    va_list ap;
    MS_S8 msg[2048];

    if (!(level & ADP_DEBUG_LEVEL))
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

#endif /* ADP_DEBUG_LEVEL */

#else /* _MSC_VER */

#if ADP_DEBUG_LEVEL == 0
#define ms_dprintf(arg...) ((void) 0)

#else /* ADP_DEBUG_LEVEL */

#ifdef MIPSDEV

#include <cyg/infra/diag.h>

#define ms_dprintf(level, fmt, arg...) \
    do { \
        if (level & ADP_DEBUG_LEVEL) diag_printf(fmt, ##arg); \
    } while (0)

#else /* MIPSDEV */

#define ms_dprintf(level, fmt, arg...) \
    do { \
        if (level & ADP_DEBUG_LEVEL) printf(fmt, ##arg); \
    } while (0)

#endif /* MIPSDEV */

#endif /* ADP_DEBUG_LEVEL */

#endif /* _MSC_VER */
#endif /*_WIN32*/ //#if defined(_WIN32)||!defined(_KERNEL_MODE_)


#endif /* MADP_MS_DPRINTF_H */
