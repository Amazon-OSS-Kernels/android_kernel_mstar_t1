/**
* Copyright (c) 2006 â€“ 2018 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   MsOSmm.h
/// @brief  MsOS Memory management
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MSOSMM_H_
#define _MSOSMM_H_

#if defined (MSOS_TYPE_LINUX)
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

typedef struct _MSOS_MMPool_Chunk
{
    MS_U32 pool_handle_id;  //in:   pool handle id, when pool init, returned by kernel 
    MS_U64 offset_in_pool;  //in:   offset in pool
    MS_U64 length;          //in:   chunk length
}MSOS_MMPool_Chunk;

struct MSOS_MMPool_Unmap_Param
{
    MS_U8* virt_addr;       //in: the VA need to unmap
    MS_U64 length;          //in: unmap length
};

struct MSOS_MMPool_Map_Param
{
    MSOS_MMPool_Chunk chunk;
    
    MS_U32 cache_type;      //in:   cache type of mapping, //0:noncache,1:cache,2:bufferable
    MS_U8* virt_addr;       //out:  virtual address for user mode access
    MS_S32 error_code;      //out:  reason about why map failed
};

struct MSOS_MMPool_PutMem_Param
{
    MSOS_MMPool_Chunk chunk;
};

struct MSOS_MMPool_GetMem_Param 
{
    MSOS_MMPool_Chunk chunk;
    
    MS_S32 error_code;      //out:  reason about why MApi_MSOS_MMPool_GetMem failed
};

struct MSOS_MMPool_Init_Param
{
    char *pool_name;                //in:   global identify name for pool to shared between multiple process
	
    MS_U64 PoolStartPa;             //in:   pool location in pa
    MS_U64 len;                     //in: 

    MS_U32 pool_handle_id;          //out:  generate pool id based on space specified by space_id
    MS_U32 miu;                     //out:  miu id this space belongs, index from 0.
    MS_U32 space_type;              //out:  return space type to application, //0: normal memory, 1:cma
    MS_S32 error_code;              //out:  error code when pool init failed
};


enum MSOS_MMPool_event_Param
{
  MSOS_MMPool_EVENT_CONFLICT_Param = 0,
  MSOS_MMPool_EVENT_NO_WAIT_Param,
  MSOS_MMPool_EVNET_NUM_Param
};

struct MSOS_MMPool_Event_Param
{
    enum MSOS_MMPool_event_Param event;
    MS_U32 pool_handle_id;
};

struct MSOS_MMPool_Notify_Param
{
    MS_U32 pool_handle_id;//pool handle
    void (*notify_callback)(MS_U32 pool_handle_id,MS_U64 start,MS_U64 length);
};

struct MSOS_MMPool_Locate_Chunk_Param
{
    MSOS_MMPool_Chunk chunk;

    MS_U64 PA;//out:   PA value.
    MS_S32 error_code; // out£º reason why failed.

    MS_S32 miu;//out:  in which miu.
    MS_U32 space_type;     //out: heap id the pa in .After get this,with the help of MApi_MSOS_MMPool_HEAP_ATTR,can get more more info.
};

MS_S32 __attribute__((weak)) MSOS_MMPool_Module_Init(void);
MS_S32 __attribute__((weak)) MSOS_MMPool_Module_DeInit(void);

MS_BOOL __attribute__((weak)) MSOS_MMPool_Init(struct MSOS_MMPool_Init_Param * Init_Param);
MS_BOOL __attribute__((weak)) MSOS_MMPool_Release(MS_U32 pool_handle_id);

MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem(struct MSOS_MMPool_GetMem_Param * get_param);
MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem_Timeout(struct MSOS_MMPool_GetMem_Param* get_param,MS_U32 timeout);
MS_BOOL __attribute__((weak)) MSOS_MMPool_PutMem(struct MSOS_MMPool_PutMem_Param * put_param);

MS_BOOL __attribute__((weak)) MSOS_MMPool_MapVA(struct MSOS_MMPool_Map_Param * map_param);//map va(same space of msos). if want to have user mode va in kernel mode utopia, need map by it's .so.
void    __attribute__((weak)) MSOS_MMPool_UnmapVA(struct MSOS_MMPool_Unmap_Param * unmap_param);

MS_BOOL __attribute__((weak)) MSOS_MMPool_Notify(struct MSOS_MMPool_Notify_Param *notify_param);

MS_BOOL __attribute__((weak)) MSOS_MMPool_Locate_Chunk(struct MSOS_MMPool_Locate_Chunk_Param * chunk_param);

    #ifdef __cplusplus
}
#endif
#endif
#endif
