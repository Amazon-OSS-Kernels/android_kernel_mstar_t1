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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   cma_mpool_manager.h
/// @brief  CMA mpool Manager interface
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CMA_POOL_ST_H__
#define __DRV_CMA_POOL_ST_H__
#define KERN_CHUNK_NUM 3

struct cma_alloc_args {
    MS_U64 offset_in_heap;
    MS_VIRT cpu_addr;
    MS_SIZE length;        
    MS_U64 align;
    MS_U32 heap_id;
    MS_U32 flags;
};

struct cma_free_args {
    MS_U32 heap_id;
    MS_U64 offset_in_heap;
    MS_SIZE length;
};

struct cma_heap_info {
    MS_U32 heap_id;
    MS_VIRT flags;

    MS_U32 miu;
    MS_U64 bus_addr;
    MS_U64 heap_miu_start_offset;
    MS_SIZE heap_length;
    MS_VIRT virt_addr;//out:if need,shoud give this out put
};

#endif

