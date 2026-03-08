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
#ifndef __MEM_ECOS_H__
#define __MEM_ECOS_H__

#include "MsCommon.h"

#ifndef externC
# ifdef __cplusplus
#  define externC extern "C"
# else
#  define externC extern
# endif
#endif

#ifndef __THROW
#ifdef __cplusplus
# define __THROW throw()
#else
# define __THROW
#endif
#endif

#define cyg_handle_t            MS_U32

struct cyg_mempool_var_memdq {
    struct cyg_mempool_var_memdq *prev, *next;
    MS_S32 size;
};

typedef struct{
    struct cyg_mempool_var_memdq head;
    MS_U8*         obase;
    MS_U32         osize;
    MS_U8*         bottom;
    MS_U8*         top;
    MS_S32         alignment;
    MS_S32         freemem;
    void*          pThis;
}cyg_mempool_var;

typedef struct {
    MS_S32         totalmem;
    MS_S32         freemem;
    void*       base;
    MS_S32         size;
    MS_S32         blocksize;
    MS_S32         maxfree;                  // The largest free block
} cyg_mempool_info;


externC void cyg_mempool_var_create(
    void            *base,              /* base of memory to use for pool */
    MS_S32             size,               /* size of memory in bytes        */
    MS_U32             *handle,            /* returned handle of memory pool */
    cyg_mempool_var *var                /* space to put pool structure in */
    ) __THROW;

/* Delete variable size memory pool */
externC void cyg_mempool_var_delete(MS_U32 varpool) __THROW;

externC void *cyg_mempool_var_alloc(MS_U32 varpool, MS_S32 size) __THROW;

externC void *cyg_mempool_var_try_alloc(
    MS_U32 varpool,
    MS_S32 size) __THROW;

externC void cyg_mempool_var_free(MS_U32 varpool, void *p) __THROW;

#endif // #ifndef __MEM_ECOS_H__
