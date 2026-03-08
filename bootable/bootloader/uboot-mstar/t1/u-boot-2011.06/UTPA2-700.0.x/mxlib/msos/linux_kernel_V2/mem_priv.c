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

#include <linux/slab.h>
#include "mem_priv.h"
#include "mem.h"

// #define free kfree
// #define malloc(size) kmalloc((size), GFP_KERNEL)

#ifdef CYGDBG_USE_ASSERTS
cyg_bool
Cyg_Mempool__check_this(Cyg_Mempool* pThis, cyg_assert_class_zeal zeal) const
{
    CYG_REPORT_FUNCTION();
    if( this == NULL ) return false;
#ifdef CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_THREADAWARE
    return pThis->mypool.check_this( &pThis->mypool, zeal );
#else
    return true;
#endif
}
#endif

#ifdef CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_THREADAWARE
cyg_uint8 *
Cyg_Mempool__alloc(Cyg_Mempool* pThis, cyg_int32 size)
{
    return pThis->mypool.alloc( &pThis->mypool, size );
}

# ifdef CYGFUN_KERNEL_THREADS_TIMER
cyg_uint8 *
Cyg_Mempool__alloc(Cyg_Mempool* pThis, cyg_int32 size, cyg_tick_count delay_timeout)
{
    return pThis->mypool.alloc( &pThis->mypool, size , delay_timeout );
}
# endif
#endif

cyg_uint8 *
Cyg_Mempool__try_alloc(Cyg_Mempool* pThis, cyg_int32 size)
{
    return pThis->mypool.try_alloc( &pThis->mypool, size );
}

cyg_uint8 *
Cyg_Mempool__resize_alloc( Cyg_Mempool* pThis, cyg_uint8 *alloc_ptr, cyg_int32 newsize,
                                    cyg_int32 *oldsize )
{
    return pThis->mypool.resize_alloc( &pThis->mypool, alloc_ptr, newsize, oldsize );
}


cyg_bool
Cyg_Mempool__free( Cyg_Mempool* pThis, cyg_uint8 *p, cyg_int32 size )
{
    return pThis->mypool.free( &pThis->mypool, p, size );
}

cyg_int32
Cyg_Mempool__get_allocation_size( Cyg_Mempool* pThis, cyg_uint8* p )
{
    return pThis->mypool.get_allocation_size(&pThis->mypool, p);
}


void
Cyg_Mempool__get_status( Cyg_Mempool* pThis, cyg_mempool_status_flag_t flags,
                                  Cyg_Mempool_Status* pstatus )
{


    pstatus->waiting = 0;

    pThis->mypool.get_status( &pThis->mypool, flags, pstatus );
}




void cyg_mempool_var_create(
    void            *base,
    cyg_int32       size,
    cyg_handle_t    *handle,
    cyg_mempool_var *var
    )
{


    var->pThis = kmalloc(sizeof(Cyg_Mempool), GFP_KERNEL);
    if(var->pThis != NULL)
        Cyg_Mempool__Create((Cyg_Mempool*)var->pThis, (cyg_uint8*)base, size, 16);
    *handle = (cyg_handle_t)var;
}


void cyg_mempool_var_delete(cyg_handle_t varpool)
{

    cyg_mempool_var *var = (cyg_mempool_var*)varpool;
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    Cyg_Mempool__Delete(pThis);
    kfree(pThis);
    var->pThis = NULL;
}

#ifdef CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_THREADAWARE
void *cyg_mempool_var_alloc(cyg_handle_t varpool, cyg_int32 size)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    return pThis->alloc(size);
}

# ifdef CYGFUN_KERNEL_THREADS_TIMER

void *cyg_mempool_var_timed_alloc(
    cyg_handle_t     varpool,
    cyg_int32        size,
    cyg_tick_count_t abstime)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    return pThis->(size, abstime);
}

# endif
#endif

void *cyg_mempool_var_try_alloc(
    cyg_handle_t varpool,
    cyg_int32    size)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    return pThis->try_alloc(pThis, size);
}


void cyg_mempool_var_free(cyg_handle_t varpool, void *p)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    cyg_bool b;

    b = pThis->free(pThis, (cyg_uint8 *)p, 0);
    CYG_ASSERT( b, "Bad free");
}



cyg_bool_t cyg_mempool_var_waiting(cyg_handle_t varpool)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    Cyg_Mempool_Status stat;

    pThis->get_status(pThis, CYG_MEMPOOL_STAT_WAITING, &stat);
    return (stat.waiting != 0);
}


void cyg_mempool_var_get_info(
    cyg_handle_t varpool,
    cyg_mempool_info *info)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    Cyg_Mempool_Status stat;

    pThis->get_status(pThis,
                   CYG_MEMPOOL_STAT_AREASIZE|
                   CYG_MEMPOOL_STAT_TOTALFREE|
                   CYG_MEMPOOL_STAT_ARENABASE|
                   CYG_MEMPOOL_STAT_ORIGSIZE|
                   CYG_MEMPOOL_STAT_MAXFREE, &stat );

    info->totalmem = stat.areasize;
    info->freemem  = stat.totalfree;
    info->size = stat.origsize;

    info->base = (void*)stat.arenabase;
    info->blocksize  = -1;
    info->maxfree = stat.maxfree;
}





void
Cyg_Mempool__Create(
    Cyg_Mempool* pThis,
    cyg_uint8 *base,
    cyg_int32 size,
    cyg_int32 alignment)
{
    memset(pThis, 0, sizeof(Cyg_Mempool));

    pThis->try_alloc= Cyg_Mempool__try_alloc;
    pThis->resize_alloc= Cyg_Mempool__resize_alloc;
    pThis->free= Cyg_Mempool__free;
    pThis->get_allocation_size= Cyg_Mempool__get_allocation_size;
    pThis->get_status= Cyg_Mempool__get_status;
    Cyg_Mempool_Implementation__Create(&pThis->mypool,
        base,
        size,
        (CYG_ADDRWORD) alignment );
}


void
Cyg_Mempool__Delete( Cyg_Mempool* pThis)
{
    memset(pThis, 0, sizeof(Cyg_Mempool));
}

