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


#ifndef _PCI_MEM_MM_H_
#define _PCI_MEM_MM_H_

#include "drvTypes.h"
#include "drvOSAPI.h"
#include "drvList.h"

#define PAGE_SIZE     4096

#define BITS_PER_LONG 32

static __inline__ MS_U32 ms_find_1st_zero(MS_U32 word)
{
  int k;

  word = ~word;
  k = 31;
  if (word & 0x0000ffff) { k -= 16; word <<= 16; }
  if (word & 0x00ff0000) { k -= 8;  word <<= 8;  }
  if (word & 0x0f000000) { k -= 4;  word <<= 4;  }
  if (word & 0x30000000) { k -= 2;  word <<= 2;  }
  if (word & 0x40000000) { k -= 1; }
        return k;
}

struct ms_mem_pool { 
  struct list_head  page_list;
  spinlock_t        lock;
  MS_U32            blocks_per_page;
  MS_U32            size;  
  MS_U32            allocation;
  char              name [32];
};

struct ms_mem_page {  /* cacheable header for 'allocation' bytes */
  struct list_head    page_list;
  void          *vaddr;
  void          *vaddr_rptr;  //Real pointer to the base of the allocated memory
  dma_addr_t    dma;
  MS_U32        bitmap[0];
};

extern struct ms_mem_pool *ms_mem_pool_create (const char *pName, MS_U32 u32Size, MS_U32 u32AlignLen, MS_U32 U32CrossLimit);
extern void *ms_mem_pool_alloc (struct ms_mem_pool *pMem_pool, int iFlags, dma_addr_t *pDma_addr);
extern void ms_mem_pool_destroy (struct ms_mem_pool *pMem_pool);
extern MS_BOOL ms_mem_pool_free (struct ms_mem_pool *pMem_pool, void *pVirt_addr, dma_addr_t dma_addr);

#endif


