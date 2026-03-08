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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvSYS_priv.h
/// @brief  System Control Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DRVCMAPOOL_H_
#define _DRVCMAPOOL_H_

//#if defined (MSOS_TYPE_LINUX)
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

#if 1
#define CMA_FLAG_MAP_VMA    (1UL<<0)
#define CMA_FLAG_CACHED      (1UL<<1)
#define CMA_FLAG_VIRT_ADDR   (1UL<<2)
#define CMA_FLAG_MTLB_POOL  (1UL<<3)
#endif

/**
 * These are the only ids that should be used for Ion heap ids.
 * The ids listed are the order in which allocation will be attempted
 * if specified. Don't swap the order of heap ids unless you know what
 * you are doing!
 * Id's are spaced by purpose to allow new Id's to be inserted in-between (for
 * possible fallbacks)
 */
enum ion_heap_ids {
    INVALID_HEAP_ID = -1,

    //system heap
    ION_SYSTEM_HEAP_ID = 0,

    //system contig heap
    ION_SYSTEM_CONTIG_HEAP_ID = 1,

    //carveout heap
    ION_CARVEOUT_HEAP_ID = 2,

    //CHUNK heap
    ION_CHUNK_HEAP_ID = 3,

    //dma heap
    ION_DMA_HEAP_ID = 4,

    //CMA heap
    //mstar id start with 15, and it is created with bootarg in module init dynamically
    //don't add manually!!!!!!!
    ION_CMA_HEAP_ID_START = 15,

	//mstar cma heap: mali type
	ION_MALI_MIUO_HEAP_ID = 16,
	ION_MALI_MIU1_HEAP_ID = 17,
	ION_MALI_MIU2_HEAP_ID = 18,

	//mstar cma heap: vdec type
	ION_VDEC_HEAP_ID = 19,

	//mstar cma heap: xc type
#if defined(CHIP_MONET)
        ION_XC_HEAP_ID = 20,
#else
        ION_XC_HEAP_ID = 22,
#endif

    ION_HEAP_ID_RESERVED = 31 /** Bit reserved for ION_SECURE flag */
};

//////////////////////////////////////
// function parameter structure //////
//////////////////////////////////////
struct CMA_Pool_Alloc_Param 
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel 
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_SIZE length;               //in 
    MS_U32 flags;                 //in: if need virtual address, set VIRT_ADDR
    MS_VIRT virt_addr;          //out: if flags: VIRT_ADDR is setted
};

struct CMA_Pool_Free_Param
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_SIZE length;               //in
};

struct CMA_Pool_Init_Param
{
    MS_U32 heap_id;     //in: maybe shared with more than one pools which based on this heap
	
    MS_U64 mtlb_addrspace_offset;    //in: pool location in mtlb address space
    MS_U64 mtlb_addrspace_len;       //in: pool length in mtlb address space
    //if app need virtual address, set MAP_VMA flag
    //if need mapping cached, set MAP_CACHE(when MAP_VMA is set), else noncache
    //if access MTLB address space, set MTLB_POOL 	
    MS_VIRT flags; 
    	

    MS_U32 pool_handle_id; //out: generate pool id based on heap specified by heap id
    MS_U32 miu;  //out: heap base on
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu
    MS_SIZE heap_length; //out: heap leagth
    MS_VIRT virt_addr;//out:if need,shoud give this out put
};


//////////////////////////////////////
// function for utopia2.0 ////////////
//////////////////////////////////////
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Init(struct CMA_Pool_Init_Param * Init_Param);
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Release(MS_U32 pool_handle_id);
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_GetMem(struct CMA_Pool_Alloc_Param * alloc_param);
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_PutMem(struct CMA_Pool_Free_Param * free_param);

#ifdef __cplusplus
}
#endif
//#endif
#endif
