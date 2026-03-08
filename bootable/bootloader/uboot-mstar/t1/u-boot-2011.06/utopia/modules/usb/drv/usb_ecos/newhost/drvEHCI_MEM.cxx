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

#include "include/drvPorts.h"
#include "drvEHCI.h"

static __inline__ void ms_ehci_qtd_init (struct ehci_qtd *pQtd, dma_addr_t tDmaAddr);

/*-------------------------------------------------------------------------*/
static struct ehci_qtd *ms_ehci_qtd_alloc (struct ehci_hcd *pEhci, int iFlags)
{
	struct ehci_qtd    *pQtd;
	dma_addr_t    tDmaAddr;

	pQtd = (struct ehci_qtd*) ms_mem_pool_alloc (pEhci->pQtdPool, iFlags, &tDmaAddr);  
	if (pQtd != 0) 
	{
		ms_ehci_qtd_init (pQtd, tDmaAddr);
	}
  
	return pQtd;
}

static __inline__ void ms_ehci_qtd_init (struct ehci_qtd *pQtd, dma_addr_t tDmaAddr)
{  
  memset (pQtd, 0, sizeof *pQtd);
  pQtd->qtd_dma_addr = tDmaAddr;
  pQtd->hw_token = QTD_STS_HALT;
  pQtd->hw_next_qtd = EHCI_LIST_END;
  pQtd->hw_alt_next_qtd = EHCI_LIST_END;
  ms_list_init (&pQtd->qtd_list);
}

static __inline__ void ms_ehci_qtd_free (struct ehci_hcd *pEhci, struct ehci_qtd *pQtd)
{
	ms_mem_pool_free (pEhci->pQtdPool, pQtd, pQtd->qtd_dma_addr);
}


static struct ehci_qh *ms_ehci_qh_alloc (struct ehci_hcd *pEhci, int iFlags)
{ 
	struct ehci_qh    *pQh;
	dma_addr_t    tDmaAddr;

	pQh = (struct ehci_qh *)ms_mem_pool_alloc (pEhci->pQhPool, iFlags, &tDmaAddr);
	if (!pQh)
		return pQh;
  
	memset (pQh, 0, sizeof *pQh);
	osapi_atomic_set (&pQh->tRefCnt, 1);
	pQh->qh_dma_addr = tDmaAddr;
	// INIT_LIST_HEAD (&pQh->qh_list);
	diag_printf("pQh: %p, pQh->qh_dma_addr: %p\n", (void *)pQh, (void *)pQh->qh_dma_addr);
	ms_list_init (&pQh->qtd_list);
  
	pQh->pDummyQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
	if (pQh->pDummyQtd == 0) 
	{
		diag_printf ("no dummy td\n");
		ms_mem_pool_free (pEhci->pQhPool, pQh, pQh->qh_dma_addr);
        // TODO: free qH?
		pQh = 0;
	}
  
	return pQh;
}

static __inline__ struct ehci_qh *ms_qh_get (struct ehci_qh *pQh)
{  
  osapi_atomic_inc (&pQh->tRefCnt);  
  return pQh;
}

static void ms_qh_put (struct ehci_hcd *pEhci, struct ehci_qh *pQh)
{
	if (!osapi_atomic_dec_and_test (&pQh->tRefCnt))
		return;
  
	if (!ms_is_empty_list (&pQh->qtd_list) || pQh->qh_next.ptr) 
	{
		USB_ASSERT (0, "unused qh not empty!\n");
	}
  
	if (pQh->pDummyQtd)
		ms_ehci_qtd_free (pEhci, pQh->pDummyQtd);  
	ms_mem_pool_free (pEhci->pQhPool, pQh, pQh->qh_dma_addr);
}

/*-------------------------------------------------------------------------*/

static void ms_ehci_mem_cleanup (struct ehci_hcd *pEhci)
{
  if (pEhci->stAsync)
    ms_qh_put (pEhci, pEhci->stAsync);
  pEhci->stAsync = 0;

  /* PCI consistent memory and pools */
  if (pEhci->pQtdPool)
    ms_mem_pool_destroy (pEhci->pQtdPool);
  pEhci->pQtdPool = 0;

  if (pEhci->pQhPool) {
    ms_mem_pool_destroy (pEhci->pQhPool);
    pEhci->pQhPool = 0;
  }
  
  if (pEhci->pPeriodic)
  {    
    pEhci->u32PeriodicSize = DEFAULT_I_TDPS; // to match with allocated size
    ms_debug_msg("Free periodic frame list array %p, size %d\n",pEhci->pPeriodic, pEhci->u32PeriodicSize * sizeof (void *));
    ms_hcd_buffer_free(&pEhci->hcd.self, pEhci->u32PeriodicSize * sizeof (void *), pEhci->pPeriodic, pEhci->tPeriodicDma);
  }
  pEhci->pPeriodic = 0;
  pEhci->tPeriodicDma = 0;

  /* shadow periodic table */
  if (pEhci->pshadow)
    kfree (pEhci->pshadow);
  pEhci->pshadow = 0;
}

static int ms_ehci_mem_init (struct ehci_hcd *pEhci, int iFlags)
{
  
  U32 i;
  
  ms_debug_func("Creat Page memory pool for qtd\n");
  pEhci->pQtdPool = ms_mem_pool_create ("ehci_qtd", sizeof (struct ehci_qtd),
      32 /* byte alignment (for hw parts) */,
      4096 /* can't cross 4K */);
  if (!pEhci->pQtdPool) 
  {
    goto fail;
  }
  
  ms_debug_func("Creat Page memory pool for qh\n");
  pEhci->pQhPool = ms_mem_pool_create ("ehci_qh", sizeof (struct ehci_qh),
      32 /* byte alignment (for hw parts) */,
      4096 /* can't cross 4K */);
  if (!pEhci->pQhPool) 
  {
    goto fail;
  }
  pEhci->stAsync = ms_ehci_qh_alloc (pEhci, iFlags);
  if (!pEhci->stAsync) 
  {
    goto fail;
  }  

  /* Hardware periodic table */  
  pEhci->pPeriodic = (U32*) ms_hcd_buffer_alloc(&pEhci->hcd.self,pEhci->u32PeriodicSize * sizeof (void *),0,&pEhci->tPeriodicDma);
  //ehci->periodic_dma = (dma_addr_t) ehci->periodic;	//Note_HC_Test
  ms_debug_msg("Allocate a non-cacheable memory pool for periodic frame list array %p, size %d\n",pEhci->pPeriodic, pEhci->u32PeriodicSize * sizeof (void *));
  if (pEhci->pPeriodic == 0) 
  {
    goto fail;
  }
  for (i = 0; i < pEhci->u32PeriodicSize; i++)
    pEhci->pPeriodic [i] = EHCI_LIST_END;
  
  pEhci->pshadow = (union ehci_qh_shadow  *) kmalloc (pEhci->u32PeriodicSize * sizeof (void *), iFlags);
  if (pEhci->pshadow == 0) 
  {
    goto fail;
  }
  memset (pEhci->pshadow, 0, pEhci->u32PeriodicSize * sizeof (void *));
  return 0;

fail:
  ms_debug_err("couldn't init memory\n");
  ms_ehci_mem_cleanup (pEhci);
  return -ENOMEM;
}

