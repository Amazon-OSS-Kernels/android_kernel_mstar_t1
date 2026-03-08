#ifndef __LINUX_CMA_H
#define __LINUX_CMA_H

/*
 * Contiguous Memory Allocator for DMA mapping framework
 * Copyright (c) 2010-2011 by Samsung Electronics.
 * Written by:
 *	Marek Szyprowski <m.szyprowski@samsung.com>
 *	Michal Nazarewicz <mina86@mina86.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License or (at your optional) any later version of the license.
 */

/*
 * Contiguous Memory Allocator
 *
 *   The Contiguous Memory Allocator (CMA) makes it possible to
 *   allocate big contiguous chunks of memory after the system has
 *   booted.
 *
 * Why is it needed?
 *
 *   Various devices on embedded systems have no scatter-getter and/or
 *   IO map support and require contiguous blocks of memory to
 *   operate.  They include devices such as cameras, hardware video
 *   coders, etc.
 *
 *   Such devices often require big memory buffers (a full HD frame
 *   is, for instance, more then 2 mega pixels large, i.e. more than 6
 *   MB of memory), which makes mechanisms such as kmalloc() or
 *   alloc_page() ineffective.
 *
 *   At the same time, a solution where a big memory region is
 *   reserved for a device is suboptimal since often more memory is
 *   reserved then strictly required and, moreover, the memory is
 *   inaccessible to page system even if device drivers don't use it.
 *
 *   CMA tries to solve this issue by operating on memory regions
 *   where only movable pages can be allocated from.  This way, kernel
 *   can use the memory for pagecache and when device driver requests
 *   it, allocated pages can be migrated.
 *
 * Driver usage
 *
 *   CMA should not be used by the device drivers directly. It is
 *   only a helper framework for dma-mapping subsystem.
 *
 *   For more information, see kernel-docs in drivers/base/dma-contiguous.c
 */

#ifdef __KERNEL__

struct cma;
struct page;
struct device;

#ifdef CONFIG_CMA
#if defined(CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER) || defined(CONFIG_MSTAR_IPAPOOL)
#define CMA_HEAP_MIUOFFSET_NOCARE (-1UL)
#endif

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
#ifdef CONFIG_MP_CMA_PATCH_COUNT_TIMECOST
#define CMA_HEAP_MEASUREMENT_LENG 96
#endif
#ifdef CONFIG_MP_CMA_PATCH_MBOOT_STR_USE_CMA
#define CMA_HEAP_NAME_LENG  32
#else
#define CMA_HEAP_NAME_LENG  32
#endif
struct CMA_BootArgs_Config {
     int miu;
     int heap_type;
     int pool_id;
     unsigned long start;  //for boot args this is miu offset
     unsigned long size;
     char name[CMA_HEAP_NAME_LENG];
};
#endif

#ifdef CONFIG_MP_ION_PATCH_MSTAR
struct cma {
    unsigned long   base_pfn;
    unsigned long   count;
    unsigned long   *bitmap;
    struct mutex    lock;
#ifdef CONFIG_MP_CMA_PATCH_COUNT_TIMECOST
    struct cma_measurement *cma_measurement_ptr;
#endif
};
#endif

#ifdef CONFIG_MP_CMA_PATCH_COUNT_TIMECOST
struct cma_measurement {
    const char *cma_heap_name;
    unsigned int cma_heap_id;
    struct mutex cma_measurement_lock;

    /* Measure Node Start */
    unsigned long total_alloc_size_kb;
    unsigned long total_alloc_time_cost_ms;

    unsigned long total_migration_size_kb;
    unsigned long total_migration_time_cost_ms;
    /* Measure Node End */

    /* Reset Node Start */
    unsigned long cma_measurement_reset;
    /* Reset Node End */
};
#endif

struct dma_contiguous_stats {
    phys_addr_t base;
    size_t size;
};

/*
 * There is always at least global CMA area and a few optional device
 * private areas configured in kernel .config.
 */
#define MAX_CMA_AREAS	(1 + CONFIG_CMA_AREAS)

extern struct cma *dma_contiguous_default_area;

static inline struct cma *dev_get_cma_area(struct device *dev)
{
	if (dev && dev->cma_area)
		return dev->cma_area;
	return dma_contiguous_default_area;
}

static inline void dev_set_cma_area(struct device *dev, struct cma *cma)
{
	if (dev)
		dev->cma_area = cma;
}

static inline void dma_contiguous_set_default(struct cma *cma)
{
	dma_contiguous_default_area = cma;
}

void dma_contiguous_reserve(phys_addr_t addr_limit);

int __init dma_contiguous_reserve_area(phys_addr_t size, phys_addr_t base,
				       phys_addr_t limit, struct cma **res_cma);

/**
 * dma_declare_contiguous() - reserve area for contiguous memory handling
 *			      for particular device
 * @dev:   Pointer to device structure.
 * @size:  Size of the reserved memory.
 * @base:  Start address of the reserved memory (optional, 0 for any).
 * @limit: End address of the reserved memory (optional, 0 for any).
 *
 * This function reserves memory for specified device. It should be
 * called by board specific code when early allocator (memblock or bootmem)
 * is still activate.
 */

static inline int dma_declare_contiguous(struct device *dev, phys_addr_t size,
					 phys_addr_t base, phys_addr_t limit)
{
	struct cma *cma;
	int ret;
	ret = dma_contiguous_reserve_area(size, base, limit, &cma);
	if (ret == 0)
		dev_set_cma_area(dev, cma);

	return ret;
}

struct page *dma_alloc_from_contiguous(struct device *dev, int count,
				       unsigned int order);
struct page *dma_alloc_at_from_contiguous(struct device *dev, int count,
				       unsigned int align, phys_addr_t at_addr);
bool dma_release_from_contiguous(struct device *dev, struct page *pages,
				 int count);

void *dma_alloc_from_contiguous_addr(struct device *dev, unsigned long start,
                                                                int count, unsigned int align);

struct page *dma_alloc_from_contiguous_direct(struct device *dev, int count,
                       unsigned int align, long *retlen);

void __dma_flush_buffer(struct page *page, size_t size);
void __dma_clear_buffer2(struct page *page, size_t size);

#else

#define MAX_CMA_AREAS	(0)

static inline struct cma *dev_get_cma_area(struct device *dev)
{
	return NULL;
}

static inline void dev_set_cma_area(struct device *dev, struct cma *cma) { }

static inline void dma_contiguous_set_default(struct cma *cma) { }

static inline void dma_contiguous_reserve(phys_addr_t limit) { }

static inline int dma_contiguous_reserve_area(phys_addr_t size, phys_addr_t base,
				       phys_addr_t limit, struct cma **res_cma) {
	return -ENOSYS;
}

static inline
int dma_declare_contiguous(struct device *dev, phys_addr_t size,
			   phys_addr_t base, phys_addr_t limit)
{
	return -ENOSYS;
}

static inline
struct page *dma_alloc_from_contiguous(struct device *dev, int count,
				       unsigned int order)
{
	return NULL;
}

struct page *dma_alloc_at_from_contiguous(struct device *dev, int count,
				       unsigned int align, phys_addr_t at_addr);




static inline
bool dma_release_from_contiguous(struct device *dev, struct page *pages,
				 int count)
{
	return false;
}

void __dma_flush_buffer(struct page *page, size_t size);
void __dma_clear_buffer2(struct page *page, size_t size);

#endif

#endif

#endif
