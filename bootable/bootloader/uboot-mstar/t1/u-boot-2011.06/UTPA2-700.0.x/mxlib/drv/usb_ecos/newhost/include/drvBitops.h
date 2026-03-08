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
#ifndef __DRV_BITOPS_H
#define __DRV_BITOPS_H

#include <MsTypes.h>

// find first bit set
static __inline__ int generic_ffs(int x)
{
    int r = 1;

    if (!x)
        return 0;
    if (!(x & 0xffff))
    {
        x >>= 16;
        r += 16;
    }
    if (!(x & 0xff))
    {
        x >>= 8;
        r += 8;
    }
    if (!(x & 0xf))
    {
        x >>= 4;
        r += 4;
    }
    if (!(x & 3))
    {
        x >>= 2;
        r += 2;
    }
    if (!(x & 1))
    {
        x >>= 1;
        r += 1;
    }
    return r;
}

static __inline__ int __test_bit(int nr, const MS_U32 * p)
{
    return (p[nr >> 5] >> (nr & 31)) & 1UL;
}

#define ms_find_1st_set(x)      generic_ffs(x)
#define ms_test_bit(nr,p)       __test_bit(nr,p)
#define ms_clear_bit(nr,addr,type) *(addr) = (volatile type )( (~( (type)1 << (type)nr)) & (type)(*((volatile type *)(addr))))
#define ms_set_bit(nr,addr,type)   *(addr) = (volatile type )( ( ( (type)1 << (type)nr)) | (type)(*((volatile type *)(addr))))

static __inline__ void ms_devmap_clear_bit(MS_U32 nr, MS_U32 *p)
{
    int bit = nr & (8 * sizeof(MS_U32) - 1);
    int idx = nr >> 5;

    ms_clear_bit(bit, &p[idx], MS_U32);
}

static __inline__ void ms_devmap_set_bit(MS_U32 nr, MS_U32 *p)
{
    int bit = nr & (8 * sizeof(MS_U32) - 1);
    int idx = nr >> 5;

    ms_set_bit(bit, &p[idx], MS_U32);
}

static __inline__ int __ms_test_and_clear_bit_le(MS_U32 nr, MS_U32 *p)
{
    int oldVal = (*p >> (nr & 31)) & 1UL;

    ms_clear_bit(nr, p, MS_U32);
    return oldVal;
}

static __inline__ int __ms_test_and_set_bit_le(MS_U32 nr, MS_U32 *p)
{
    int oldVal = (*p >> (nr & 31)) & 1UL;

    ms_set_bit(nr, p, MS_U32);
    return oldVal;
}

 // APIs for little endian
#define ms_test_and_set_bit(nr,p)    __ms_test_and_set_bit_le(nr,p)
#define ms_test_and_clear_bit(nr,p)  __ms_test_and_clear_bit_le(nr,p)
extern int find_next_zero_bit_le (void *p, int size, int offset);
#define find_next_zero_bit(p,size,offset)  find_next_zero_bit_le(p,size,offset)

#endif
