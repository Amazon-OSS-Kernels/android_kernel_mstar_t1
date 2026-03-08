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

//#include <MsCommon.h> // NUSED
//#include "include/drvConfig.h" // NUSED
//#include "include/drvCompiler.h" // NUSED
//#include "include/drvErrno.h" // NUSED
//#include "include/drvPorts.h" // NUSED
//#include "include/drvPCIMEM.h" // NUSED
//#include "include/drvTimer.h" // NUSED
#include "include/drvKernel.h"

// USB related header files
//#include "include/drvUSBHost.h" // NUSED
#include "drvUsbd.h"

void ms_hcd_buffer_destroy (struct usb_hcd *hcd);

static const size_t  pool_max [HCD_MEM_POOLS] = {
    32,
    64,
    128,
    512,
    PAGE_SIZE / 2
};

/**
     * @brief               create hcd buffer
     *
     * @param           struct usb_hcd *hcd
     *
     * @return          buffer pointer
     */
int ms_hcd_buffer_create (struct usb_hcd *hcd)
{
    char    name [16];
    int     i, pool_size;    

    for (i = 0; i < HCD_MEM_POOLS; i++) 
    {
        if (!(pool_size = pool_max [i]))
            continue;
        USB_sprintf(name, /*sizeof(name),*/ "buffer-%d", pool_size);
        hcd->pool [i] = ms_mem_pool_create (name, pool_size, pool_size, 0);
        if (!hcd->pool [i]) 
        {
            ms_hcd_buffer_destroy (hcd);
            return -ENOMEM;
        }
    }
    return 0;
}

/**
     * @brief               hcd buffer destroy
     *
     * @param           struct usb_hcd *hcd
     *
     * @return          none
     */
void ms_hcd_buffer_destroy (struct usb_hcd *hcd)
{
    int    i;

    for (i = 0; i < HCD_MEM_POOLS; i++) 
    {
        struct ms_mem_pool    *pool = hcd->pool [i];
        
        if (pool) 
        {
            ms_mem_pool_destroy (pool);
            hcd->pool [i] = 0;
        }
    }
}

/**
     * @brief               hcd buffer memory allocate
     *
     * @param           struct usb_bus     *bus
     * @param           size_t      size
     * @param           void       *addr
     * @param           dma_addr_t    dma     
     *
     * @return          buffer pointer
     */
void *ms_hcd_buffer_alloc (
    struct usb_bus     *bus,
    size_t      size,
    int      mem_flags,
    dma_addr_t    *dma
)
{
    struct usb_hcd    *hcd = (struct usb_hcd*) bus->hcpriv;
    int       i;

    for (i = 0; i < HCD_MEM_POOLS; i++) 
    {
        if (size <= pool_max [i])
            return ms_mem_pool_alloc (hcd->pool [i], mem_flags, dma);
    }
    return ncmem_alloc_page(dma);
}

/**
     * @brief               hcd buffer memory free
     *
     * @param           struct usb_bus     *bus
     * @param           size_t      size
     * @param           void       *addr
     * @param           dma_addr_t    dma     
     *
     * @return          none
     */
void ms_hcd_buffer_free (
    struct usb_bus     *bus,
    size_t      size,
    void       *addr,
    dma_addr_t    dma
)
{
    struct usb_hcd    *hcd = (struct usb_hcd*) bus->hcpriv;
    int       i;

    for (i = 0; i < HCD_MEM_POOLS; i++) 
    {
        if (size <= pool_max [i]) 
        {
            ms_mem_pool_free (hcd->pool [i], addr, dma);
            return;
        }
    }
    ncmem_free_page((U32)addr);
}

