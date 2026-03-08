/*
 *
 * (C) COPYRIGHT 2014-2016 MStar Semiconductor, Inc. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */

/**
 * @file mali_kbase_mem_block_export.c
 */

#include <mali_kbase_mem_block_export.h>

#include <linux/dma-buf.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/version.h>

#include "mstar/mstar_chip.h"

#ifndef ARM_MIU1_BASE_ADDR
#define ARM_MIU1_BASE_ADDR 0x80000000
#endif
#define MSTAR_MIU1_PHYS_BASE ARM_MIU1_BASE_ADDR
#if defined(MSTAR_MIU2_BUS_BASE) && defined(ARM_MIU2_BASE_ADDR)
#define MSTAR_MIU2_PHYS_BASE ARM_MIU2_BASE_ADDR
#endif

#define PHYS_TO_BUS_ADDRESS_ADJUST_MIU0 MSTAR_MIU0_BUS_BASE
#define PHYS_TO_BUS_ADDRESS_ADJUST_MIU1 (MSTAR_MIU1_BUS_BASE - MSTAR_MIU1_PHYS_BASE)
#if defined(MSTAR_MIU2_BUS_BASE) && defined(ARM_MIU2_BASE_ADDR)
#define PHYS_TO_BUS_ADDRESS_ADJUST_MIU2 (MSTAR_MIU2_BUS_BASE - MSTAR_MIU2_PHYS_BASE)
#endif

struct mem_block_data
{
    phys_addr_t base;
};

static struct sg_table* mem_block_map(struct dma_buf_attachment* attach,
                                      enum dma_data_direction direction)
{
    struct mem_block_data* data = attach->dmabuf->priv;
    unsigned long pfn = PFN_DOWN(data->base);
    struct page* page = pfn_to_page(pfn);
    struct sg_table* table;
    int ret;

    table = kzalloc(sizeof(*table), GFP_KERNEL);

    if (!table)
    {
        return ERR_PTR(-ENOMEM);
    }

    ret = sg_alloc_table(table, 1, GFP_KERNEL);

    if (ret < 0)
    {
        goto err;
    }

    sg_set_page(table->sgl, page, attach->dmabuf->size, 0);
    /* XXX: in sparse memory model, it's possible that pfn_to_page(page_to_pfn(page)) != page) */
    sg_dma_address(table->sgl) = data->base;
    /* sg_dma_address(table->sgl) = sg_phys(table->sgl); */

    return table;

err:
    kfree(table);
    return ERR_PTR(ret);
}

static void mem_block_unmap(struct dma_buf_attachment* attach,
                            struct sg_table* table,
                            enum dma_data_direction direction)
{
    sg_free_table(table);
    kfree(table);
}

static void mem_block_release(struct dma_buf* buf)
{
    struct mem_block_data* data = buf->priv;
    kfree(data);
}

static void* mem_block_do_kmap(struct dma_buf* buf, unsigned long pgoffset, bool atomic)
{
    struct mem_block_data* data = buf->priv;
    unsigned long pfn = PFN_DOWN(data->base) + pgoffset;
    struct page* page = pfn_to_page(pfn);

    if (atomic)
    {
        return kmap_atomic(page);
    }
    else
    {
        return kmap(page);
    }
}

static void* mem_block_kmap_atomic(struct dma_buf* buf, unsigned long pgoffset)
{
    return mem_block_do_kmap(buf, pgoffset, true);
}

static void mem_block_kunmap_atomic(struct dma_buf* buf, unsigned long pgoffset, void* vaddr)
{
    kunmap_atomic(vaddr);
}

static void* mem_block_kmap(struct dma_buf* buf, unsigned long pgoffset)
{
    return mem_block_do_kmap(buf, pgoffset, false);
}

static void mem_block_kunmap(struct dma_buf* buf, unsigned long pgoffset, void* vaddr)
{
    kunmap(vaddr);
}

static int mem_block_mmap(struct dma_buf* buf, struct vm_area_struct* vma)
{
    struct mem_block_data* data = buf->priv;

    return remap_pfn_range(vma, vma->vm_start,
                           PFN_DOWN(data->base),
                           vma->vm_end - vma->vm_start,
                           vma->vm_page_prot);
}

static struct dma_buf_ops mem_block_ops =
{
    .map_dma_buf    = mem_block_map,
    .unmap_dma_buf  = mem_block_unmap,
    .release        = mem_block_release,
    .kmap_atomic    = mem_block_kmap_atomic,
    .kunmap_atomic  = mem_block_kunmap_atomic,
    .kmap           = mem_block_kmap,
    .kunmap         = mem_block_kunmap,
    .mmap           = mem_block_mmap,
};

int mem_block_export_dma_buf(phys_addr_t base, u32 size, bool is_bus_address)
{
    struct mem_block_data* data;
    struct dma_buf* buf;
    int fd = 0;

    if (PAGE_ALIGN(base) != base || PAGE_ALIGN(size) != size)
    {
        return -EINVAL;
    }

    if (!is_bus_address)
    {
        if (base < MSTAR_MIU1_PHYS_BASE)
        {
            base += PHYS_TO_BUS_ADDRESS_ADJUST_MIU0;
        }
#if defined(MSTAR_MIU2_BUS_BASE) && defined(ARM_MIU2_BASE_ADDR)
        else if (base >= MSTAR_MIU2_PHYS_BASE)
        {
            base += PHYS_TO_BUS_ADDRESS_ADJUST_MIU2;
        }
#endif
        else
        {
            base += PHYS_TO_BUS_ADDRESS_ADJUST_MIU1;
        }
    }

    data = kzalloc(sizeof(*data), GFP_KERNEL);

    if (!data)
    {
        return -ENOMEM;
    }

    data->base = base;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0)
    buf = dma_buf_export(data, &mem_block_ops, size, O_RDWR, NULL);
#else
    buf = dma_buf_export(data, &mem_block_ops, size, O_RDWR);
#endif

    if (IS_ERR(buf))
    {
        kfree(data);
        return PTR_ERR(buf);
    }

    fd = dma_buf_fd(buf, O_CLOEXEC);

    if (fd < 0)
    {
        dma_buf_put(buf);
    }

    return fd;
}
