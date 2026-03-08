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

#include <MsCommon.h>
#include "include/drvCompiler.h"
#include "include/drvPorts.h"
#include "include/drvPCIMEM.h"
#include "include/drvKernel.h"
#include "include/drvUSB.h"
#include "include/drvBitops.h"

/* ================================================ PCI Memory Management Porting ====================================================== */

static __inline__ int
is_page_busy (int blocks, U32 *bitmap)
{
  while (blocks > 0) {
    if (*bitmap++ != ~0UL)
      return 1;
    blocks -= BITS_PER_LONG;
  }
  return 0;
}
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

struct pci_pool *pci_pool_create (const char *name, struct pci_dev *pdev,
  size_t size, size_t align, size_t allocation)
{
  struct pci_pool    *retval;

  if (align == 0)
    align = 1;
  if (size == 0)
    return 0;
  else if (size < align)
    size = align;
  else if ((size % align) != 0) {
    size += align + 1;
    size &= ~(align - 1);
  }
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  if (allocation == 0) {
    if (PAGE_SIZE < size)
      allocation = size;
    else
      allocation = PAGE_SIZE;
  } else if (allocation < size)
    return 0;

  if (!(retval = (struct pci_pool*) kmalloc (sizeof (*retval), SLAB_KERNEL)))
    return retval;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  strncpy (retval->name, name, sizeof (retval->name) );
  INIT_LIST_HEAD (&retval->page_list);
  retval->dev = pdev;
  retval->size = size;
  retval->allocation = allocation;
  retval->blocks_per_page = allocation / size;
  kdbg("Created Mem info: size=%d,allocation=%d bytes, %d blocks per page", retval->size, retval->allocation, retval->blocks_per_page);
  return retval;
}
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

static struct pci_page *
pool_alloc_page (struct pci_pool *pool, int mem_flags)
{
  struct pci_page  *page;
  int    mapsize;

  mapsize = pool->blocks_per_page;
  mapsize = (mapsize + BITS_PER_LONG - 1) / BITS_PER_LONG;
  mapsize *= sizeof (U32);

  page = (struct pci_page *) kmalloc (mapsize + sizeof(struct pci_page), mem_flags);
  INIT_LIST_HEAD(&page->page_list);
  if (!page)
    return 0;

  page->vaddr = ncmem_alloc_page(&page->dma);

  if (page->vaddr) {
    memset (page->bitmap, ((U32)-1), mapsize);  // bit set == free
    list_add (&page->page_list, &pool->page_list);
  } else {
    kfree (page);
    page = 0;
  }
  return page;
}
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

void *pci_pool_alloc (struct pci_pool *pool, int mem_flags, dma_addr_t *handle)
{
  U32    flags;
  struct list_head  *entry;
  struct pci_page    *page;
  int      map, block;
  size_t      offset;
  void      *retval;

//restart:  //restrain warning message
  spin_lock_irqsave (&pool->lock, flags);
  list_for_each (entry, &pool->page_list) {
    U32    i;
    //page = list_entry (entry, struct pci_page, page_list);
    const struct list_head *__mptr = entry;
    page = (struct pci_page *)( (char *)__mptr - (char *)offsetof(struct pci_page,page_list));

    /* only cachable accesses here ... */
    for (map = 0, i = 0;
        i < pool->blocks_per_page;
        i += BITS_PER_LONG, map++) {
      if (page->bitmap [map] == 0)
        continue;
      block = ffz (~ page->bitmap [map]);
      if ((i + block) < pool->blocks_per_page) {
        clear_bit ( block, &page->bitmap [map],U32);
        offset = (BITS_PER_LONG * map) + block;
        offset *= pool->size;
        goto ready;
      }
    }
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!(page = pool_alloc_page (pool, mem_flags))) {
    /*
    if (mem_flags == SLAB_KERNEL) {
      DECLARE_WAITQUEUE (wait, current);

      current->state = TASK_INTERRUPTIBLE;
      add_wait_queue (&pool->waitq, &wait);
      spin_unlock_irqrestore (&pool->lock, flags);

      schedule_timeout (POOL_TIMEOUT_JIFFIES);

      current->state = TASK_RUNNING;
      remove_wait_queue (&pool->waitq, &wait);
      goto restart;
    }
    */
    retval = 0;
    goto done;
  }

  clear_bit (0, &page->bitmap [0],U32);
  offset = 0;
ready:

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  retval = (void*) (offset + (size_t) ( page->vaddr ));
  *handle = offset + page->dma;
done:
  spin_unlock_irqrestore (&pool->lock, flags);
  kdbg("The allocated addr is %p, block size is %d ,bit_map[%d] is 0x%08X",retval, pool->size ,map,(U32)page->bitmap [map]);
  return retval;
}

static void
pool_free_page (struct pci_pool *pool, struct pci_page *page)
{
  //dma_addr_t  dma = page->dma;
  ncmem_free_page((U32)page->vaddr);
  list_del (&page->page_list);
  kfree (page);
}


void
pci_pool_destroy (struct pci_pool *pool)
{
  U32    flags;
  spin_lock_irqsave (&pool->lock, flags);
  while (!list_empty (&pool->page_list)) {
    struct pci_page    *page;

    //page = list_entry (pool->page_list.next,struct pci_page, page_list);
    const struct list_head *__mptr = pool->page_list.next;
    page = (struct pci_page *)( (char *)__mptr - (char *)offsetof(struct pci_page,page_list) );

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

    if (is_page_busy (pool->blocks_per_page, page->bitmap)) {
      printk (KERN_ERR "pci_pool_destroy %s, %p busy\n",
         pool->name, page->vaddr);
      /* leak the still-in-use consistent memory */
      list_del (&page->page_list);
      kfree (page);
    } else
      pool_free_page (pool, page);
  }
  spin_unlock_irqrestore (&pool->lock, flags);
  kfree (pool);
}
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/


static struct pci_page *
pool_find_page (struct pci_pool *pool, dma_addr_t dma)
{
  U32    flags;
  struct list_head  *entry;
  struct pci_page    *page;

  spin_lock_irqsave (&pool->lock, flags);
  list_for_each (entry, &pool->page_list) {

    //page = list_entry (entry, struct pci_page, page_list);
    const struct list_head *__mptr = entry;
    page = (struct pci_page *)( (char *)__mptr - (char *)offsetof(struct pci_page,page_list) );
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

    if (dma < page->dma)
      continue;
    if (dma < (page->dma + pool->allocation))
      goto done;
  }
  page = 0;
done:
  spin_unlock_irqrestore (&pool->lock, flags);
  return page;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

BOOL
pci_pool_free (struct pci_pool *pool, void *vaddr, dma_addr_t dma)
{
  struct pci_page    *page;
  U32    flags;
  int      map, block;

  if ((page = pool_find_page (pool, dma)) == 0) {
    printk (KERN_ERR "pci_pool_free %s, %p/%lx (bad dma)\n",
    pool->name, vaddr,
    (U32) dma);
    return FALSE;
  }
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  block = dma - page->dma;
  block /= pool->size;
  map = block / BITS_PER_LONG;
  block %= BITS_PER_LONG;

  spin_lock_irqsave (&pool->lock, flags);
  set_bit (block, &page->bitmap [map], U32);
  kdbg("Free Block: addr=0x%08X bitmap is 0x%08X",dma,(U32)page->bitmap [map]);
  /*
  if (waitqueue_active (&pool->waitq))
    wake_up (&pool->waitq);
  */
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  /*
   * Resist a temptation to do
   *    if (!is_page_busy(bpp, page->bitmap)) pool_free_page(pool, page);
   * it is not interrupt safe. Better have empty pages hang around.
   */
  spin_unlock_irqrestore (&pool->lock, flags);
  return TRUE;
}
/* ============================================== End of PCI Memory Management Porting ============================================== */

