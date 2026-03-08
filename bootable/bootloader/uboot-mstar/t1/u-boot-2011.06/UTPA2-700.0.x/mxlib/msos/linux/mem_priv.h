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
#ifndef __MEM_PRIV_ECOS_H__
#define __MEM_PRIV_ECOS_H__

#include "MsCommon.h"
#include <string.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define cyg_uint8               MS_U8
#define cyg_uint16              MS_U16
#define cyg_uint32              MS_U32

#define cyg_int8                MS_S8
#define cyg_int16               MS_S16
#define cyg_int32               MS_S32

#define CYG_ADDRWORD            MS_VIRT//cyg_int32
#define CYG_ADDRESS             MS_VIRT//cyg_uint32

#define cyg_bool                MS_BOOL
#define cyg_bool_t              MS_BOOL
#define cyg_mempool_status_flag_t       cyg_uint16

#define CYG_ASSERT(b, str)      if (!(b)) { printf("%s", (str)); MS_ASSERT(0); }

typedef struct{
    const cyg_uint8 *arenabase;
    cyg_int32   areasize;
    cyg_int32   freeblocks;
    cyg_int32   totalallocated;
    cyg_int32   totalfree;
    cyg_int32   blocksize;
    cyg_int32   maxfree;
    cyg_int8    waiting;
    const cyg_uint8 *origbase;
    cyg_int32   origsize;
    cyg_int8    maxoverhead;

} Cyg_Mempool_Status;

inline void
Cyg_Mempool_Status_init(Cyg_Mempool_Status* pStMPoolStatus)
{
    pStMPoolStatus->arenabase       = (const cyg_uint8 *)-1;
    pStMPoolStatus->areasize       = -1;
    pStMPoolStatus->freeblocks      = -1;
    pStMPoolStatus->totalallocated  = -1;
    pStMPoolStatus->totalfree       = -1;
    pStMPoolStatus->blocksize       = -1;
    pStMPoolStatus->maxfree         = -1;
    pStMPoolStatus->waiting         = -1;
    pStMPoolStatus->origbase        = (const cyg_uint8 *)-1;
    pStMPoolStatus->origsize        = -1;
    pStMPoolStatus->maxoverhead     = -1;
}

inline void
Cyg_Mempool_Status__Cyg_Mempool_Status(Cyg_Mempool_Status* pStMPoolStatus)
{
    Cyg_Mempool_Status_init(pStMPoolStatus);
}

struct memdq {
    struct memdq *prev, *next;
    cyg_int32 size;
};

typedef struct Cyg_Mempool_Implementation_s Cyg_Mempool_Implementation;

inline cyg_uint8 *
Cyg_Mempool_Implementation__try_alloc(Cyg_Mempool_Implementation* pThis, cyg_int32 size );

inline cyg_uint8 *
Cyg_Mempool_Implementation__resize_alloc( Cyg_Mempool_Implementation* pThis,
                                                   cyg_uint8 *alloc_ptr,
                                                   cyg_int32 newsize,
                                                   cyg_int32 *oldsize );

inline cyg_bool
Cyg_Mempool_Implementation__free( Cyg_Mempool_Implementation* pThis, cyg_uint8 *p, cyg_int32 size );

inline cyg_int32
Cyg_Mempool_Implementation__get_allocation_size( Cyg_Mempool_Implementation* pThis, cyg_uint8 *p );

inline void
Cyg_Mempool_Implementation__get_status(
    Cyg_Mempool_Implementation* pThis,
    cyg_mempool_status_flag_t flags,
    Cyg_Mempool_Status* pstatus );


struct Cyg_Mempool_Implementation_s{
#if 0

    Cyg_Mempool_Implementation() {};


    Cyg_Mempool_Implementation &
    operator=( Cyg_Mempool_Implementation &ref )
    { return ref; };
#endif


    struct memdq head;
    cyg_uint8  *obase;
    cyg_int32  osize;
    cyg_uint8  *bottom;
    cyg_uint8  *top;
    cyg_int32  alignment;
    cyg_int32  freemem;




    cyg_int32
    (*roundup)(Cyg_Mempool_Implementation* pThis, cyg_int32 size);

    struct memdq *
    (*addr2memdq)( Cyg_Mempool_Implementation* pThis, cyg_uint8 *addr );

    struct memdq *
    (*alloc2memdq)( Cyg_Mempool_Implementation* pThis, cyg_uint8 *addr );

    cyg_uint8 *
    (*memdq2alloc)( Cyg_Mempool_Implementation* pThis, struct memdq *dq );

    void
    (*insert_free_block)( Cyg_Mempool_Implementation* pThis, struct memdq *freedq );







    cyg_uint8 *
    (*try_alloc)( Cyg_Mempool_Implementation* pThis, cyg_int32 /* size */ );











    cyg_uint8 *
    (*resize_alloc)( Cyg_Mempool_Implementation* pThis, cyg_uint8 *alloc_ptr, cyg_int32 newsize,
                  cyg_int32 *oldsize );



    cyg_bool
    (*free)( Cyg_Mempool_Implementation* pThis,
          cyg_uint8 * /* ptr */,
          cyg_int32   /* size */ );



    cyg_int32 (*get_allocation_size)( Cyg_Mempool_Implementation* pThis, cyg_uint8 * /* ptr */ );




    void
    (*get_status)( Cyg_Mempool_Implementation* pThis,
                cyg_mempool_status_flag_t /* flags */,
                Cyg_Mempool_Status* /* status */ );
};


#define CYG_MEMPOOL_STAT_ARENABASE       (1<<0)
#define CYG_MEMPOOL_STAT_AREASIZE       (1<<1)
#define CYG_MEMPOOL_STAT_FREEBLOCKS      (1<<2)
#define CYG_MEMPOOL_STAT_TOTALALLOCATED  (1<<3)
#define CYG_MEMPOOL_STAT_TOTALFREE       (1<<4)
#define CYG_MEMPOOL_STAT_BLOCKSIZE       (1<<5)
#define CYG_MEMPOOL_STAT_MAXFREE         (1<<6)
#define CYG_MEMPOOL_STAT_WAITING         (1<<7)
#define CYG_MEMPOOL_STAT_ORIGBASE        (1<<9)
#define CYG_MEMPOOL_STAT_ORIGSIZE        (1<<10)
#define CYG_MEMPOOL_STAT_MAXOVERHEAD     (1<<11)

inline cyg_int32
Cyg_Mempool_Implementation__roundup( Cyg_Mempool_Implementation* pThis, cyg_int32 size )
{

    size += sizeof(struct memdq);
    size = (size + pThis->alignment - 1) & -pThis->alignment;
    return size;
}

inline struct memdq *
Cyg_Mempool_Implementation__addr2memdq( Cyg_Mempool_Implementation* pThis, cyg_uint8 *addr )
{
    struct memdq *dq;
    dq = (struct memdq *)(pThis->roundup(pThis, (MS_VIRT)addr) - sizeof(struct memdq));
    return dq;
}

inline struct memdq *
Cyg_Mempool_Implementation__alloc2memdq( Cyg_Mempool_Implementation* pThis, cyg_uint8 *addr )
{
    return (struct memdq *)(addr - sizeof(struct memdq));
}

inline cyg_uint8 *
Cyg_Mempool_Implementation__memdq2alloc( Cyg_Mempool_Implementation* pThis, struct memdq *dq )
{
    return ((cyg_uint8 *)dq + sizeof(struct memdq));
}


#define CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_COALESCE
inline void
Cyg_Mempool_Implementation__insert_free_block( Cyg_Mempool_Implementation* pThis, struct memdq *dq )
{
    struct memdq *hdq=&pThis->head;

    pThis->freemem += dq->size;
#ifdef CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_COALESCE

    struct memdq *idq;

    for (idq = hdq->next; idq != hdq; idq = idq->next) {
        if (idq > dq)
            break;
    }

    dq->next = idq;
    dq->prev = idq->prev;
    idq->prev = dq;
    dq->prev->next = dq;


    if ((dq->next != hdq) && ((char *)dq + dq->size == (char *)dq->next)) {
        dq->size += dq->next->size;
        dq->next = dq->next->next;
        dq->next->prev = dq;
    }
    if ((dq->prev != hdq) && ((char *)dq->prev + dq->prev->size == (char *)dq)) {
        dq->prev->size += dq->size;
        dq->prev->next = dq->next;
        dq->next->prev = dq->prev;
        dq = dq->prev;
    }
#else
    dq->prev = hdq;
    dq->next = hdq->next;
    hdq->next = dq;
    dq->next->prev=dq;
#endif
}


inline
void
Cyg_Mempool_Implementation__Create(
        Cyg_Mempool_Implementation* pThis,
        cyg_uint8 *base,
        cyg_int32 size,
        CYG_ADDRWORD align )
{


    memset(pThis, 0, sizeof(Cyg_Mempool_Implementation));


    pThis->roundup= Cyg_Mempool_Implementation__roundup;
    pThis->addr2memdq= Cyg_Mempool_Implementation__addr2memdq;
    pThis->alloc2memdq= Cyg_Mempool_Implementation__alloc2memdq;
    pThis->memdq2alloc= Cyg_Mempool_Implementation__memdq2alloc;
    pThis->insert_free_block= Cyg_Mempool_Implementation__insert_free_block;
    pThis->try_alloc= Cyg_Mempool_Implementation__try_alloc;
    pThis->resize_alloc= Cyg_Mempool_Implementation__resize_alloc;
    pThis->free= Cyg_Mempool_Implementation__free;
    pThis->get_allocation_size= Cyg_Mempool_Implementation__get_allocation_size;
    pThis->get_status= Cyg_Mempool_Implementation__get_status;

    CYG_ASSERT( align > 0, "Bad alignment" );
    CYG_ASSERT(0!=align ,"align is zero");
    CYG_ASSERT(0==(align & (align-1)),"align not a power of 2");

    if ((unsigned)size < sizeof(struct memdq)) {
        pThis->bottom = NULL;
        return;
    }

    pThis->obase=base;
    pThis->osize=size;

    pThis->alignment = align;
    while (pThis->alignment < (cyg_int32)sizeof(struct memdq))
        pThis->alignment += pThis->alignment;
    CYG_ASSERT(0==(pThis->alignment & (pThis->alignment-1)),"alignment not a power of 2");






    pThis->bottom = (cyg_uint8 *)pThis->addr2memdq(pThis, base);




    pThis->top = (cyg_uint8 *)((MS_VIRT)(base+size+sizeof(struct memdq)) & -pThis->alignment) -
        sizeof(struct memdq);

    CYG_ASSERT( pThis->top > pThis->bottom , "heap too small" );
    CYG_ASSERT( pThis->top <= (base+size), "top too large" );
    CYG_ASSERT( ((MS_VIRT)(pThis->top+sizeof(struct memdq)) & (pThis->alignment-1))==0,
                "top badly aligned" );

    struct memdq *hdq = &pThis->head, *dq = (struct memdq *)pThis->bottom;

    CYG_ASSERT( ((MS_VIRT)pThis->memdq2alloc(pThis, dq) & (pThis->alignment-1))==0,
                 "bottom badly aligned" );

    hdq->prev = hdq->next = dq;
    hdq->size = 0;
    dq->prev = dq->next = hdq;

    pThis->freemem = dq->size = pThis->top - pThis->bottom;

    return;
}



inline void Cyg_Mempool_Implementation__Delete(Cyg_Mempool_Implementation* pThis)
{

    memset(pThis, 0, sizeof(Cyg_Mempool_Implementation));
}










inline cyg_uint8 *
Cyg_Mempool_Implementation__try_alloc(Cyg_Mempool_Implementation* pThis, cyg_int32 size )
{
    struct memdq *dq = &pThis->head;
    cyg_uint8 *alloced;






    if (NULL == pThis->bottom)
        return NULL;

    size = pThis->roundup(pThis, size);

    do {
        CYG_ASSERT( dq->next->prev==dq, "Bad link in dq");
        dq = dq->next;
        if(0 == dq->size) {
            CYG_ASSERT(dq == &pThis->head, "bad free block");
            return NULL;
        }
    } while(dq->size < size);

    if( size == dq->size ) {

        dq->prev->next = dq->next;
        dq->next->prev = dq->prev;
        alloced = (cyg_uint8 *)dq;
    } else {

        CYG_ASSERT( dq->size > size, "block found is too small");



        dq->size -=size;





        CYG_ASSERT( (cyg_int32)sizeof(struct memdq)<=dq->size ,
                "not enough space for list item" );

        alloced = (cyg_uint8 *)dq + dq->size;
    }

    CYG_ASSERT( (pThis->bottom<=alloced) && (alloced<=pThis->top), "alloced outside pool" );



    dq = (struct memdq *)alloced;
    dq->size = size;
    dq->next = dq->prev = (struct memdq *)0xd530d53;

    pThis->freemem -=size;

    cyg_uint8 *ptr = pThis->memdq2alloc(pThis, dq );
    CYG_ASSERT( ((CYG_ADDRESS)ptr & (pThis->alignment-1)) == 0,
                "returned memory not aligned" );

    CYG_ASSERT(NULL!= ptr, "allocate memory fail");
    return ptr;
}













inline cyg_uint8 *
Cyg_Mempool_Implementation__resize_alloc( Cyg_Mempool_Implementation* pThis,
                                                   cyg_uint8 *alloc_ptr,
                                                   cyg_int32 newsize,
                                                   cyg_int32 *oldsize )
{
    cyg_uint8 *ret = NULL;







    if ( NULL != oldsize )
    {


    }

    CYG_ASSERT( (pThis->bottom <= alloc_ptr) && (alloc_ptr <= pThis->top),
                "alloc_ptr outside pool" );

    struct memdq *dq=pThis->alloc2memdq( pThis, alloc_ptr );


    CYG_ASSERT( (dq->next == dq->prev) &&
                (dq->next == (struct memdq *)0xd530d53), "bad alloc_ptr" );

    newsize = pThis->roundup(pThis, newsize);

    if ( NULL != oldsize )
        *oldsize = dq->size;

    if ( newsize > dq->size ) {

        if ( (cyg_uint8 *)dq + newsize <= pThis->top ) {
            struct memdq *nextdq = (struct memdq *)((cyg_uint8 *)dq + dq->size);

            if ( (nextdq->next != nextdq->prev) &&
                 (nextdq->size >= (newsize - dq->size)) ) {



                nextdq->next->prev = nextdq->prev;
                nextdq->prev->next = nextdq->next;
                dq->size += nextdq->size;
                pThis->freemem -= nextdq->size;
            }
        }
    }



    if ( newsize < dq->size ) {


        struct memdq *newdq = (struct memdq *)((cyg_uint8 *)dq + newsize);

        newdq->size = dq->size - newsize;
        dq->size = newsize;

        CYG_ASSERT( (cyg_int32)sizeof(struct memdq)<=newdq->size ,
                    "not enough space for list item" );


        pThis->insert_free_block( pThis, newdq );

        ret = alloc_ptr;

    }
    else if ( newsize == dq->size ) {
        ret = alloc_ptr;
    }





    return ret;

}







inline cyg_bool
Cyg_Mempool_Implementation__free( Cyg_Mempool_Implementation* pThis, cyg_uint8 *p, cyg_int32 size )
{





    if (!((pThis->bottom <= p) && (p <= pThis->top)))
        return false;

    struct memdq *dq=pThis->alloc2memdq( pThis, p );


    if ( (dq->next != dq->prev) ||
         (dq->next != (struct memdq *)0xd530d53) )
        return false;

    if ( 0==size ) {
        size = dq->size;
    } else {
        size = pThis->roundup(pThis, size);
    }

    if( dq->size != size )
        return false;

    CYG_ASSERT( (cyg_int32)sizeof(struct memdq)<=size ,
                "not enough space for list item" );

    pThis->insert_free_block( pThis, dq );

    return true;
}



inline cyg_int32
Cyg_Mempool_Implementation__get_allocation_size( Cyg_Mempool_Implementation* pThis, cyg_uint8 *p )
{






    if (!((pThis->bottom <= p) && (p <= pThis->top)))
        return -1;

    struct memdq *dq=pThis->alloc2memdq( pThis, p );


    if ( (dq->next != dq->prev) ||
         (dq->next != (struct memdq *)0xd530d53) )
        return -1;

    return dq->size;
}



inline void
Cyg_Mempool_Implementation__get_status(
    Cyg_Mempool_Implementation* pThis,
    cyg_mempool_status_flag_t flags,
    Cyg_Mempool_Status* pstatus )
{


    pstatus->arenabase = pThis->obase;
    if ( 0 != (flags & CYG_MEMPOOL_STAT_AREASIZE) )
        pstatus->areasize = pThis->top - pThis->bottom;
    if ( 0 != (flags & CYG_MEMPOOL_STAT_TOTALALLOCATED) )
        pstatus->totalallocated = (pThis->top-pThis->bottom) - pThis->freemem;

    pstatus->totalfree = pThis->freemem;
    if ( 0 != (flags & CYG_MEMPOOL_STAT_MAXFREE) ) {
        struct memdq *dq = &pThis->head;
        cyg_int32 mf = 0;

        do {
            CYG_ASSERT( dq->next->prev==dq, "Bad link in dq");
            dq = dq->next;
            if(0 == dq->size) {
                CYG_ASSERT(dq == &pThis->head, "bad free block");
                break;
            }
            if(dq->size > mf)
                mf = dq->size;
        } while(1);
        pstatus->maxfree = mf - sizeof(struct memdq);
    }

    pstatus->origbase = pThis->obase;

    pstatus->origsize = pThis->osize;




}

typedef struct Cyg_Mempool_s Cyg_Mempool;

struct Cyg_Mempool_s
{
#ifdef CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_THREADAWARE
    Cyg_Mempolt2<Cyg_Mempool_Implementation> mypool;
#else
    Cyg_Mempool_Implementation mypool;
#endif


    cyg_uint8 *
    (*try_alloc)( Cyg_Mempool* pThis, cyg_int32 /* size */ );











    cyg_uint8 *
    (*resize_alloc)( Cyg_Mempool* pThis, cyg_uint8 * /* alloc_ptr */, cyg_int32 /* newsize */,
                  cyg_int32 *);



    cyg_bool
    (*free)( Cyg_Mempool* pThis, cyg_uint8 * /* ptr */, cyg_int32);



    cyg_int32 (*get_allocation_size)( Cyg_Mempool* pThis, cyg_uint8 * /* ptr */ );




    void
    (*get_status)( Cyg_Mempool* pThis, cyg_mempool_status_flag_t /* flags */,
                Cyg_Mempool_Status * /* status */ );



};


void
Cyg_Mempool__Create( Cyg_Mempool* pThis, cyg_uint8 * /* base */, cyg_int32  /* size */, cyg_int32);


void
Cyg_Mempool__Delete(Cyg_Mempool* pThis);


#ifdef __cplusplus
}
#endif


#endif
