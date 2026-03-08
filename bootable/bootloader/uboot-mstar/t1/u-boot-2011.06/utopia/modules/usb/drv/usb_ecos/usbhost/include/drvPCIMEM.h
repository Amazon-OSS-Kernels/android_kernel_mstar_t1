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

#include "drvPorts.h"
#include "drvTypes.h"
#include "drvOSAPI.h"
#include "drvList.h"
#include "../drvUsbcommon.h"

#define PAGE_SIZE     4096

#define BITS_PER_LONG 32

/*
 * ffz = Find First Zero in word. Undefined if no zero exists,
 * so code should check against ~0UL first..
 */
static __inline__ U32 ffz(U32 word)
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

// Data structure for PCI momery management
struct pci_pool {  /* the pool */
  struct list_head     page_list;
  spinlock_t       	lock;
  size_t         		blocks_per_page;
  size_t         		size;
  struct pci_dev       *dev;
  size_t         		allocation;
  char         		name [32];
  //wait_queue_head_t  waitq;
};

struct pci_page {  /* cacheable header for 'allocation' bytes */
  struct list_head     page_list;
  void         	*vaddr;
  void         	*vaddr_rptr;  //Real pointer to the base of the allocated memory
  dma_addr_t       	dma;
  U32       	bitmap[0];
};

//Does not support pci_free_consistent now, just ignore it by macro
//#define pci_free_consistent(w,x,y,z)
extern struct pci_pool *pci_pool_create (const char *name, struct pci_dev *pdev,
  size_t size, size_t align, size_t allocation);
extern void *pci_pool_alloc (struct pci_pool *pool, int mem_flags, dma_addr_t *handle);
extern void pci_pool_destroy (struct pci_pool *pool);
extern BOOL pci_pool_free (struct pci_pool *pool, void *vaddr, dma_addr_t dma);

#endif


