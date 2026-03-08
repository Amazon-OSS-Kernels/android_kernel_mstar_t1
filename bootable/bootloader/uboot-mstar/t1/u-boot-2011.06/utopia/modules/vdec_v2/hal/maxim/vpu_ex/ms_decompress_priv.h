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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef __MS_DECOMPRESS_PRIV_H__
#define __MS_DECOMPRESS_PRIV_H__

#define OFFSET_BITS     12
#define LENGTH_BITS     4

#define WINDOW_SIZE     (1 << OFFSET_BITS)

#define MAX_UNCODED     2
#define MAX_CODED       ((1 << LENGTH_BITS) + MAX_UNCODED)

#define ENCODED         0       // encoded string
#define UNCODED         1       // unencoded character

typedef struct encoded_string_t
{
    unsigned int offset;    // offset to start of longest match
    unsigned int length;    // length of longest match
} encoded_string_t;

#define Wrap(value, limit)      (value&0xFFF) //(((value) < (limit)) ? (value) : ((value) - (limit)))

#define MEMORY_POOL_SIZE                (0x1000000UL)
#define BUF_OFFSET_MALLOC_BUF           (0x400000UL)

#endif /* __MS_DECOMPRESS_PRIV_H__ */
