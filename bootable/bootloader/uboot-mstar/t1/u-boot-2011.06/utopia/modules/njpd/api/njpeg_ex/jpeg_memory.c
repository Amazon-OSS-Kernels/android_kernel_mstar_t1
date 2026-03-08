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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    jpeg_memory.c
/// @brief  JPEG memory management
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef CMODEL
#include "MsCommon.h"
#include "njpeg_def.h"
//#include "Utl.h"
#else
#include <string.h>
#include "jpeg_cmodel_def.h"
#endif
#include "jpeg_memory.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HLEN    (sizeof(__memt__))
#define MIN_BLOCK   (HLEN * 4)

#define AVAIL   (__mem_avail__[0])

#define MIN_POOL_SIZE   (HLEN * 10)


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: __memt__
/// @brief \b Struct \b Description: mpool structure for jpd
//-----------------------------------------------------------------------------
typedef struct __mem__
{
    struct __mem__  *next;  ///< single-linked list
    MS_U32  len;            ///< length of following block
} __memt__, *__memp__;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
__memt__  __mem_avail__ [2] =
{
    { NULL, 0 },    /* HEAD for the available block list */
    { NULL, 0 },    /* UNUSED but necessary so free doesn't join HEAD or ROVER with the pool */
};


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL JPEG_MEMORY_init_mempool (void *pool, MS_U32 size)
{
    if (size < MIN_POOL_SIZE)
        return FALSE;   /* FAILURE */

    __mem_avail__[0].next = NULL;
    __mem_avail__[0].len = 0;
    __mem_avail__[1].next = NULL;
    __mem_avail__[1].len = 0;

    if (pool == NULL)
    {
        pool = (void *)1;
        size--;
    }

/* Don't memset JPEG memory pool, it is cobuffer with display buffer. */
//    JPEG_memset(pool, 0, size);

    AVAIL.next = (struct __mem__  *)pool;
    AVAIL.len  = size;

    (AVAIL.next)->next = NULL;
    (AVAIL.next)->len  = size - HLEN;

    return TRUE;    /* SUCCESS */
}

void *JPEG_MEMORY_malloc (MS_U32 size)
{
    __memp__ q;     /* ptr to free block */
    __memp__ p;     /* q->next */
    MS_U32 k;       /* space remaining in the allocated block */
    q = &AVAIL;

    while (1)
    {
        p = q->next;
        if (p == NULL)
        {
            return (NULL);  /* FAILURE */
        }

        if (p->len >= size)
            break;

        q = p;
    }

    k = p->len - size;  /* calc. remaining bytes in block */

    if (k < MIN_BLOCK)  /* rem. bytes too small for new block */
    {
        q->next = p->next;
        return (&p[1]); /* SUCCESS */
    }

    k -= HLEN;
    p->len = k;

    q = (__memp__ ) ((MS_VIRT)((MS_U8 *) (&p [1])) + k);
    q->len = size;
    q->next = NULL;

    return (&q[1]); /* SUCCESS */
}

void JPEG_MEMORY_free (void *memp)
{
    __memp__ q;     /* ptr to free block */
    __memp__ p;     /* q->next */
    __memp__ p0;    /* block to free */

    if ((memp == NULL) || (AVAIL.len == 0))
        return;

    p0 = (__memp__) memp;
    p0 = &p0 [-1];  /* get address of header */

    q = &AVAIL;

    while (1)
    {
        p = q->next;

        if ((p == NULL) || (p > (__memp__) memp))
            break;

        q = p;
    }

    if ((p != NULL) && ((((MS_U8 *)memp) + p0->len) == (MS_U8 *)p))
    {
        p0->len += p->len + HLEN;
        p0->next = p->next;
    }
    else
    {
        p0->next = p;
    }

    if ((((MS_U8 *)q) + q->len + HLEN) == (MS_U8 *)p0)
    {
        q->len += p0->len + HLEN;
        q->next = p0->next;
    }
    else
    {
        q->next = p0;
    }
}

