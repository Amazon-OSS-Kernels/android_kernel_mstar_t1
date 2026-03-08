/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __MSDECOMPRESS_NONOS_PRIV_H__
#define __MSDECOMPRESS_NONOS_PRIV_H__


//===================================================
#define FLASH_SIZE_MAX  0x1000000

//===================================================
// Chunk header info:
#define CHUNK_HEADER_ADDR       0x20000 // sboot+pm
#define CHUNK_HEADER_LEN        0x80

#define CHUNK_HEADER_CRC32_POS  (0x14)
#define CRC32_POS	(0x14)

//========================================================================
// __section__ (".prog_img_info")
#ifdef __mips__

#else // Aeon
    #define AP_IMG_INFO_SECTION_OFFSET      0x1100
#endif

    #define AP_IMG_INFO_MAGIC_ID_ADDR       (AP_IMG_INFO_SECTION_OFFSET+0)
    #define AP_IMG_INFO_COMPRESS_INFO_ADDR  (AP_IMG_INFO_SECTION_OFFSET+0x47)
    #define AP_IMG_INFO_COMPRESS_LEN_ADDR   (AP_IMG_INFO_SECTION_OFFSET+0x4A)
    #define AP_IMG_INFO_MAGIC_ID_2_ADDR     (AP_IMG_INFO_SECTION_OFFSET+0x4E)
    #define AP_IMG_INFO_JOBA_DATA_PTR_ADDR  (AP_IMG_INFO_SECTION_OFFSET+0x52)

//---------------------------------------------------------------------------
// __section__ (".img_info")  at 0x2000
#define AP_BIN_TABLE_START_ADDR         (0x2000)

#define BIN_PACKAGE_HEADER_ID0          0x54454C09
#define BIN_PACKAGE_HEADER_ID1          0x58336900

#define BIN_TBL_OFFSET_ID_0             0
#define BIN_TBL_OFFSET_ID_1             4
#define BIN_TBL_OFFSET_BIN_COUNT        12
#define BIN_TBL_OFFSET_BIN_ITEM_START   14

#define BIN_TBL_BIN_ITEM_SIZE           11

//========================================================================

#define UNCOMPRESSED_LENGTH     (14336UL)


#if 0//defined(__arm__)
    #define MS_BUS_MIU0_0000M     CONFIG_MIU0_BUSADDR
    #define BOOT_LOADER_LENGTH    (0x30000)
    #define COMPRESSED_LENGTH_POS    (0xFCAUL)

    //jc_db: need to fix cache problem
    #define DECOMPRESS_BUF                 (MS_BUS_MIU0_0000M+(CONFIG_DRAM_SIZE_MB_RELOCATE-19)*0x100000)
    #define DECOMPRESS_BUF_CACHEABLE       (DECOMPRESS_BUF)
    #define MEMORY_POOL_CACHEABLE          (MS_BUS_MIU0_0000M+(CONFIG_DRAM_SIZE_MB_RELOCATE-13)*0x100000)

#elif defined(__mips__)

    #define BOOT_LOADER_LENGTH      (0x30000) // Need refine...

    #define COMPRESSED_LENGTH_POS   (0xFCAUL)

    #if( ENABLE_MSTAR_MILAN    \
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


#else // Aeon
    // Dram size : 32M ~ 128M

    //#define COMPRESSED_LENGTH_POS   (0x114AUL)

    #define MMAP_LD2_MEM_ADDR           0x01000100 // 16M

    //#define DECOMPRESS_BUF              (0x01100000UL)
    #define MMAP_DECOMP_SRC_BUF_ADDR    (0x01100000UL) // 17M

    //#define DECOMPRESS_BUF_CACHEABLE    (0x01100000UL)
    //#define MEMORY_POOL_CACHEABLE       (0x01800000UL)

#endif


#define MEMORY_POOL_SIZE        (0x0A00000UL)

//===================================================
#ifdef __mips__

    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xDFFFFFFF)

#elif defined(__arm__)

    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xFFFFFFFF)

#else

    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xFFFFFFFF)

#endif

//===============================================================

#define ALIGNED_VALUE(value, align)  ( ((value) + ((align)-1)) & ~((align)-1) )

//===============================================================

#endif /* __MSDECOMPRESS_NONOS_PRIV_H__ */

