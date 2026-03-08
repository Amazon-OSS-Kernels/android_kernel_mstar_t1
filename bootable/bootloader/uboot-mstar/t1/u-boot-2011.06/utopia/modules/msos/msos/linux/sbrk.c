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
#include <sys/types.h>  /* For size_t */
#include <stdio.h>

#define MFAIL       ((void *) -1)

// please define these symbols in linker script
unsigned long heap_end = 0xFFFFFFFF;
unsigned long sbrk_top = 0xFFFFFFFF;

void dlinit(unsigned long _heap_start, unsigned long _heap_end)
{
    sbrk_top = _heap_start;
    heap_end = _heap_end;
}

void *sbrk(unsigned int size)
{
    void *ptr;

    // NOTE: we assume all allocated address are 4 byte-aligned!!!
    size = (size + 3) & (~3);

    if (size > 0)
    {
        if ( (sbrk_top + size) <= heap_end)
        {
            ptr = (void *)sbrk_top;
            sbrk_top += size;
            return ptr;
        }
    }
    else if (size == 0)
    {
        return (void *)sbrk_top;
    }
    else
    {
        printf("sbrk: error\n");
        // we don't currently support shrink behavior
    }
    return MFAIL;
}

void *_sbrk(unsigned int size) __attribute__ ((weak, alias("sbrk")));
