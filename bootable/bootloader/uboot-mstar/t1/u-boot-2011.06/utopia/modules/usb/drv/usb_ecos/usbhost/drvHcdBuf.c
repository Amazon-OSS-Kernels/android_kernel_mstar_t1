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

#include <MsCommon.h>
#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include  "include/drvPCIMEM.h"
#include "include/drvTimer.h"
#include "include/drvKernel.h"

#if 1


#ifdef CONFIG_USB_DEBUG
  #define DEBUG
#else
  #undef DEBUG
#endif

// USB related header files
#include "include/drvUSB.h"
#include "drvHCD.h"



void hcd_buffer_destroy (struct usb_hcd *hcd);


static const size_t  pool_max [HCD_BUFFER_POOLS] = {
  32,
  64,
  128,
  512,
  PAGE_SIZE / 2
};

int hcd_buffer_create (struct usb_hcd *hcd)
{
  char    name [16];
  int     i, size;

  for (i = 0; i < HCD_BUFFER_POOLS; i++) {
    if (!(size = pool_max [i]))
      continue;
    USB_sprintf(name, /*sizeof(name),*/ "buffer-%d", size);
    hcd->pool [i] = pci_pool_create (name, hcd->pdev,
        size, size, 0);
    if (!hcd->pool [i]) {
      hcd_buffer_destroy (hcd);
      return -ENOMEM;
    }
  }
  return 0;
}

void hcd_buffer_destroy (struct usb_hcd *hcd)
{
  int    i;

  for (i = 0; i < HCD_BUFFER_POOLS; i++) {
    struct pci_pool    *pool = hcd->pool [i];
    if (pool) {
      pci_pool_destroy (pool);
      hcd->pool [i] = 0;
    }
  }
}

void *hcd_buffer_alloc (
  struct usb_bus     *bus,
  size_t      size,
  int      mem_flags,
  dma_addr_t    *dma
)
{
  struct usb_hcd    *hcd = (struct usb_hcd*) bus->hcpriv;
  int       i;

  for (i = 0; i < HCD_BUFFER_POOLS; i++) {
    if (size <= pool_max [i])
      return pci_pool_alloc (hcd->pool [i], mem_flags, dma);
  }
  return ncmem_alloc_page(dma);
}

void hcd_buffer_free (
  struct usb_bus     *bus,
  size_t      size,
  void       *addr,
  dma_addr_t    dma
)
{
  struct usb_hcd    *hcd = (struct usb_hcd*) bus->hcpriv;
  int       i;

  for (i = 0; i < HCD_BUFFER_POOLS; i++) {
    if (size <= pool_max [i]) {
      pci_pool_free (hcd->pool [i], addr, dma);
      return;
    }
  }
  ncmem_free_page((U32)addr);
}
#endif	//#ifdef

