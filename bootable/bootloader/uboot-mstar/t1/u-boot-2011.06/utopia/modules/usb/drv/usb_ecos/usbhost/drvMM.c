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

//=============================================================================
#include <MsCommon.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_arch.h>
#include "include/drvConfig.h"
#include "include/drvPorts.h"
#include "include/drvKernel.h"
#include "include/drvBitops.h"

#if 1
#define NOCACHE_MEMORY_SIZE   SZ_128K //SZ_64K, Kaiserin
//#define NOCACHE_MEMORY_REGION MEM_REGION_8M
//#define NOCACHE_REGION_NUM    3
//#define NOCACHE_MIN_ALIGN     32
//#define NCMEM_BUFFER_POOLS    4

extern int BaseOfFreeMemory;
extern int TopOfFreeMemory;
extern int BaseOfHeap;
extern int TopOfHeap;

#define NCM_DEBUG

#ifdef NCM_DEBUG
#define NCM_Dbg(x,...)		diag_printf(x,__VA_ARGS__)
#else
#define NCM_Dbg(x,...)
#endif

struct ncmem_pool_tag {  /* the pool */
  U32      base_addr;
  size_t      pages_per_pool;
  U32    *bitmap;
};
static struct ncmem_pool_tag  ncmem_pool;

U32 ncmem_addr;
U32 ncmem_base;

/*******************************************************************************
 * Routine name : ncmem_free_page
 *    returns    : none
 * Created by    : Peter Liao
 * Date created : 2004/01/25
 * Description  : Create a non-cacheable and non-bufferable page memory pool via
 *                CPU memoryt region declaration API. Page momery pool is splited
 *                by the size of page memory. Each page memory is aligned to
 *                page_size bytes.
 * Notes        :
 *******************************************************************************/
void init_cache_memory(void)
{
	int mapsize;


	 ncmem_addr=(U32)KSEG02KSEG1(Usb_AllocateNonCachedMemory(NOCACHE_MEMORY_SIZE + PAGE_SIZE));

	USB_ASSERT(ncmem_addr, "Allocate ncmem buffer fail\n");
	ncmem_base = (ncmem_addr + PAGE_SIZE)&(~(PAGE_SIZE-1));
	NCM_Dbg("The base address is 0x%8lX\n", ncmem_base);

	// Initialize non-cached memory pool information
	ncmem_pool.pages_per_pool = (NOCACHE_MEMORY_SIZE/PAGE_SIZE);
	mapsize = ncmem_pool.pages_per_pool;
	mapsize = (mapsize + BITS_PER_LONG - 1) / BITS_PER_LONG;
	mapsize *= sizeof (U32);
	ncmem_pool.base_addr =(U32) KSEG02KSEG1(ncmem_base);	// convert cached address to uncached
//	ncmem_pool.bitmap = (U32 *)LIB_MemoryAllocate(mapsize, MALLOC_CACHED_ADDR);
       ncmem_pool.bitmap = (U32*)Usb_AllocateNonCachedMemory(mapsize);
	memset(ncmem_pool.bitmap, ((U32)-1), mapsize);//Bitmap: 0-used 1-unused

	NCM_Dbg("Base addr is 0x%08lX\n",ncmem_pool.base_addr);
	NCM_Dbg("Top addr is 0x%08lX\n",ncmem_pool.base_addr+NOCACHE_MEMORY_SIZE-1);
	NCM_Dbg("Pages per pool is %d\n",ncmem_pool.pages_per_pool);
	NCM_Dbg("Bit map base addr is %p\n",ncmem_pool.bitmap);
}

/*******************************************************************************
 * Routine name : ncmem_alloc_page
 *    returns    : 0 - pointer to allocated virtual address
 *    dma_addr  : pointer to allocated physical address
 * Created by    : Peter Liao
 * Date created : 2004/01/25
 * Description  : It will allocate a new non-cacheable and non-bufferable memory
 *                (size = page_size) from non-cacheable page memory pool for callee
 *                PAGE_SIZE is defined in pci_mem.h file
 * Notes        :
 *******************************************************************************/
void *ncmem_alloc_page(dma_addr_t *dma_addr)
{
	U32    i;
	int     map, page;
	size_t  offset;
	void    *retval;
	U32 flags;

	spin_lock_irqsave (&pool->lock, flags);
	/* only cachable accesses here ... */
	for (map = 0, i = 0;
		i < ncmem_pool.pages_per_pool;
		i += BITS_PER_LONG, map++)
	{
		if (ncmem_pool.bitmap[map] == 0)
			continue;
		page = ffz (~ ncmem_pool.bitmap [map]);
		if ((i + page) < ncmem_pool.pages_per_pool) {
			clear_bit ( page, &ncmem_pool.bitmap [map],U32);
			offset = (BITS_PER_LONG * map) + page;
			offset *= PAGE_SIZE;
			goto ready;
		}
	}

	NCM_Dbg("ERROR: No enough non-cached memory space !!%s\n","");
	retval = 0;
	goto done;

ready:
	retval = (void*) (offset + (size_t) (ncmem_pool.base_addr));
	*dma_addr = (dma_addr_t)USB_VA2PA((U32)retval);	// Convert to physical address for DMA
done:
	spin_unlock_irqrestore (&pool->lock, flags);
	NCM_Dbg("The allocated addr is %p, bit_map[%d] is 0x%08lX\n",retval,map,(U32)(ncmem_pool.bitmap[map]));
	return retval;
}

/*******************************************************************************
 * Routine name : ncmem_free_page
 *    returns    : none
 *    addr      : allocated virtual base address
 * Created by    : Peter Liao
 * Date created : 2004/01/25
 * Description  : It will de-allocate/release the allocated non-cacheable memory
 *                page to non-cacheable page memory pool.
 * Notes        :
 *******************************************************************************/
void ncmem_free_page(U32 addr)
{
	U32 flags;
	int map, page;

	page = addr - ncmem_pool.base_addr;
	page /= PAGE_SIZE;
	map = page / BITS_PER_LONG;
	page %= BITS_PER_LONG;
	spin_lock_irqsave (NULL, flags);
	set_bit (page, &(ncmem_pool.bitmap[map]), U32);
	NCM_Dbg("Free Block: bitmap[%d] is 0x%08lX",map,(U32)ncmem_pool.bitmap[map]);
	spin_unlock_irqrestore (&pool->lock, flags);
}
/*******************************************************************************
 * Routine name : ncmem_alloc
 *    returns    : 0 - pointer to allocated virtual address
 *    dma_addr  : pointer to allocated physical address
 * Created by    : Peter Liao
 * Date created : 2004/02/10
 * Description  : It will allocate a new non-cacheable and non-bufferable memory
 *                (size = user specific) from non-cacheable page memory pool for callee
 *                Therefore, it may waste up memory space if the size is not mutiply
 *                of page size. Callee should call this function before calling
 *                ncmem_alloc_page because of continuous issue.
 * Notes        :
 *******************************************************************************/
void *ncmem_alloc(dma_addr_t *dma_addr, U32 size)
{
	U32 flags;
	U32 i;
	U32 map, page;
	U32 num_of_pages;
	size_t  offset;
	void    *retval;

	//If the size is not multiply of PAGE_SIZE, just allocate a PAGE for remaining space
	if ( size%PAGE_SIZE != 0)
		size = size/PAGE_SIZE +1;
	else
		size = size/PAGE_SIZE;
	num_of_pages = size/PAGE_SIZE;
	spin_lock_irqsave (&pool->lock, flags);
	/* only cachable accesses here ... */
	for (map = 0, i = 0;
		i < ncmem_pool.pages_per_pool;
		i += BITS_PER_LONG, map++)
	{
		if (ncmem_pool.bitmap[map] == 0)
			continue;
		page = ffz (~ ncmem_pool.bitmap [map]);
		if ((i + page) < ncmem_pool.pages_per_pool) {
			offset = (BITS_PER_LONG * map) + page;
			offset *= PAGE_SIZE;
			goto ready;
		}
	}
	NCM_Dbg("ERROR: No enough non-cached memory space !!%s\n","");
	retval = 0;
	goto done;

ready:
	//Allocate enough pages for required memory space
	for(i=0;(i < num_of_pages);i++)
	{
		if ( page >= BITS_PER_LONG )
		{
		page = 0;
		map++;
		}
		clear_bit ( page++, &ncmem_pool.bitmap [map],U32);
	}
	retval = (void*) (offset + (size_t) (ncmem_pool.base_addr));
	*dma_addr = (dma_addr_t)USB_VA2PA((U32)retval);
done:
	spin_unlock_irqrestore (&pool->lock, flags);
	NCM_Dbg("The allocated addr = %p, size = %ld pages = %ld, end of bitmap[%ld]=0x%08lX",retval ,size, num_of_pages, map,(U32)ncmem_pool.bitmap[map]);
	return retval;
}

/*******************************************************************************
 * Routine name : ncmem_free
 *    returns    : none
 *    addr      : allocated virtual base address
 *    size      : allocated size
 * Created by    : Peter Liao
 * Date created : 2004/02/10
 * Description  : It will de-allocate/release the allocated non-cacheable memory
 *                space groupged by pages.
 * Notes        :
 *******************************************************************************/
void ncmem_free(U32 addr,U32 size)
{
	U32 flags;
	int i;
	int map, page, num_of_pages;

	if ( size%PAGE_SIZE != 0)
		size = size/PAGE_SIZE +1;
	else
		size = size/PAGE_SIZE;
	num_of_pages = size/PAGE_SIZE;

	page = ncmem_pool.base_addr - addr;
	page /= PAGE_SIZE;
	map = page / BITS_PER_LONG;
	page %= BITS_PER_LONG;
	spin_lock_irqsave (&pool->lock, flags);
	for(i=0;(i < num_of_pages);i++)
	{
		if ( page >= BITS_PER_LONG )
		{
			page = 0;
			map++;
		}
		set_bit (page, &(ncmem_pool.bitmap[map]), U32);
	}
	NCM_Dbg("Free non-cacheable memory : size = %ld = %d pages, end of bitmap[%d]=0x%08lX",size, num_of_pages, map,(U32)ncmem_pool.bitmap[map]);
	spin_unlock_irqrestore (&pool->lock, flags);
}

void Destory_NC_mem(void)
{
#if 0
	LIB_MemoryFree((void*) ncmem_base);
#else
	//MsOS_FreeMemory((void*)CYGARC_CACHED_ADDRESS( ncmem_addr),gs32NonCachedPoolID_MIU0 );
	Usb_FreeNonCachedMemory((void*)CYGARC_CACHED_ADDRESS( ncmem_addr));
#endif
}

extern void MsOS_FlushMemory(void);
void Chip_Flush_Memory(void)
{
    MsOS_FlushMemory();
}

#endif	//#ifdef

