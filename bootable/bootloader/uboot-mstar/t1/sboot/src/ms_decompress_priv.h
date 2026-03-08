/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __MS_DECOMPRESS_PRIV_H__
#define __MS_DECOMPRESS_PRIV_H__

#if 0

#define OFFSET_BITS     12
#define LENGTH_BITS     4

#define WINDOW_SIZE     (1 << OFFSET_BITS)

#define MAX_UNCODED     2
#define MAX_CODED       ((1 << LENGTH_BITS) + MAX_UNCODED)

#endif

#define UNCOMPRESSED_LENGTH     (14336UL)

#ifdef __mips__

#define BOOT_LOADER_LENGTH      (0x30000)
#define COMPRESSED_LENGTH_POS   (0xFCAUL)

#if( ENABLE_MSTAR_EMERALD   \
   ||ENABLE_MSTAR_MILAN    \
   ||ENABLE_MSTAR_MULAN     \
   )
    #define DECOMPRESS_BUF              (0xA2000000UL) // 32M
    #define DECOMPRESS_BUF_CACHEABLE    (0x82000000UL) // 32M
    #define MEMORY_POOL_CACHEABLE       (0x83000000UL) // 40M
#else
    #define DECOMPRESS_BUF              (0xA1100000UL) // 17M
    #define DECOMPRESS_BUF_CACHEABLE    (0x81100000UL) // 17M
    #define MEMORY_POOL_CACHEABLE       (0x82100000UL) // 31M
#endif

#define NON_CACHEABLE_TO_CACHEABLE_MASK (0xDFFFFFFF)

#elif defined(__arm__)
    #define MS_BUS_MIU0_0000M     CONFIG_MIU0_BUSADDR
    #define BOOT_LOADER_LENGTH    (0x30000)
    #define COMPRESSED_LENGTH_POS    (0xFCAUL)

    //jc_db: need to fix cache problem
    #define DECOMPRESS_BUF                 (MS_BUS_MIU0_0000M+(CONFIG_DRAM_SIZE_MB_RELOCATE-19)*0x100000)
    #define DECOMPRESS_BUF_CACHEABLE       (DECOMPRESS_BUF)
    #define MEMORY_POOL_CACHEABLE          (MS_BUS_MIU0_0000M+(CONFIG_DRAM_SIZE_MB_RELOCATE-13)*0x100000)
    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xFFFFFFFF)
#else

    #define COMPRESSED_LENGTH_POS   (0x114AUL)

    #define DECOMPRESS_BUF              (0x01100000UL)
    #define DECOMPRESS_BUF_CACHEABLE    (0x01100000UL)
    #define MEMORY_POOL_CACHEABLE       (0x01800000UL)

    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xFFFFFFFF)
#endif
#define CRC32_POS	(0x14)

#define MEMORY_POOL_SIZE			(0x0A00000UL)


#if 0

#define ENCODED         0       // encoded string
#define UNCODED         1       // unencoded character

typedef struct encoded_string_t
{
    unsigned int offset;    // offset to start of longest match
    unsigned int length;    // length of longest match
} encoded_string_t;

#define Wrap(value, limit)      (value&0xFFF) //(((value) < (limit)) ? (value) : ((value) - (limit)))

#endif

#endif /* __MS_DECOMPRESS_PRIV_H__ */
