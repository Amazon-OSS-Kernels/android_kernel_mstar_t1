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

#ifndef __IO_H_
#define __IO_H_

#include "MsCommon.h"
#include "drvTypes.h"
//#include "byteorder.h"
#include "../drvUSBHwCtl.h"

#if 0
#define __raw_readb(a)    ((U8)UHC_READXBYTE((U32)a-UHC_BASE))

#define __raw_readw(a)    ((U16)UHC_READXBYTE((U32)a-UHC_BASE) | \
                                         ((U16)UHC_READXBYTE((U32)a+1-UHC_BASE) << 8) )

#define __raw_readl(a)    ((U32)UHC_READXBYTE((U32)a-UHC_BASE) | \
                                         ((U32)UHC_READXBYTE((U32)a+1-UHC_BASE) << 8) | \
                                         ((U32)UHC_READXBYTE((U32)a+2-UHC_BASE) << 16) | \
                                         ((U32)UHC_READXBYTE((U32)a+3-UHC_BASE) << 24) )


#define __raw_readb_Port2(a)    ((U8)UHC2_READXBYTE((U32)a-UHC2_BASE))

#define __raw_readw_Port2(a)    ((U16)UHC2_READXBYTE((U32)a-UHC2_BASE) | \
                                         ((U16)UHC2_READXBYTE((U32)a+1-UHC2_BASE) << 8) )

#define __raw_readl_Port2(a)    ((U32)UHC2_READXBYTE((U32)a-UHC2_BASE) | \
                                         ((U32)UHC2_READXBYTE((U32)a+1-UHC2_BASE) << 8) | \
                                         ((U32)UHC2_READXBYTE((U32)a+2-UHC2_BASE) << 16) | \
                                         ((U32)UHC2_READXBYTE((U32)a+3-UHC2_BASE) << 24) )

//#define writel(value, addr)    mwHost20Portw((U32)addr-gUHC_BASE, value)
static inline void writel(U32 value, U32 addr)
{
    if ( (addr>=UHC_BASE) && (addr<=UHC_BASE+0x80) )
        mwHost20Portw((U32)addr-UHC_BASE, value);
    else if ( (addr>=UHC2_BASE) && (addr<=UHC2_BASE+0x80) )
        mwHost20Portw_Port2((U32)addr-UHC2_BASE, value);
}

//#define readl(addr)           (__raw_readl((U32)addr))
static inline U32 readl(U32 c)
{
    if ( (c>=UHC_BASE) && (c<=UHC_BASE+0x80) )
        return __raw_readl((U32)c);
    else if ( (c>=UHC2_BASE) && (c<=UHC2_BASE+0x80) )
        return __raw_readl_Port2((U32)c);
}

//#define readb(c)              __raw_readb((U32)c)
static inline U8 readb(U32 c)
{
    if ( (c>=UHC_BASE) && (c<=UHC_BASE+0x80) )
        return __raw_readb((U32)c);
    else if ( (c>=UHC2_BASE) && (c<=UHC2_BASE+0x80) )
        return __raw_readb_Port2((U32)c);
}

//#define readw(c)              (__raw_readw((U32)c))
static inline U16 readw(U32 c)
{
    if ( (c>=UHC_BASE) && (c<=UHC_BASE+0x80) )
        return __raw_readw((U32)c);
    else if ( (c>=UHC2_BASE) && (c<=UHC2_BASE+0x80) )
        return __raw_readw_Port2((U32)c);
}

#endif


#endif

