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

#ifndef _HAL_MPOOL_H_
#define _HAL_MPOOL_H_

#include "MsCommon.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/* Use 'M' as magic number */
#define MALLOC_IOC_MAGIC                'M'

// MALLOC_IOC_MPOOL_INFO
#define MALLOC_IOC_MPOOL_INFO           _IOWR(MALLOC_IOC_MAGIC, 0x00, DevMalloc_MPool_Info_t)
#define MALLOC_IOC_FLUSHDCACHE          _IOR(MALLOC_IOC_MAGIC, 0x01, DevMalloc_MPool_Info_t)
#define MALLOC_IOC_MPOOL_CACHE          _IOR(MALLOC_IOC_MAGIC, 0x03, unsigned int)
#define MALLOC_IOC_MPOOL_SET            _IOR(MALLOC_IOC_MAGIC, 0x04, DevMalloc_MPool_Info_t)
#define MALLOC_IOC_MPOOL_KERNELDETECT   _IOWR(MALLOC_IOC_MAGIC, 0x05, DevMalloc_MPool_Kernel_Info_t)
#define MALLOC_IOC_FLUSHDCACHE_PAVA     _IOR(MALLOC_IOC_MAGIC, 0x07, DevMalloc_MPool_Flush_Info_t)
#define MALLOC_IOC_FLUSHDCACHE_ALL      _IOR(MALLOC_IOC_MAGIC, 0x09, DevMalloc_MPool_Flush_Info_t)
#define MALLOC_IOC_SETWATCHPT           _IOW(MALLOC_IOC_MAGIC, 0x0A, DevMalloc_MPool_Watchpt_Info_t)
#define MALLOC_IOC_GETWATCHPT           _IOR(MALLOC_IOC_MAGIC, 0x0B, char*)


#define _FUNC_NOT_USED()        do {} while ( 0 )

#define MIU_INTERVAL 0x80000000UL
#define ENABLE_PARTIAL_MAPPING 1
#define DISABLE_PARTIAL_MAPPING 0

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//MS_U32 u32MpoolRegBase;

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct
{
    MS_U64             u64Addr;
    MS_U64             u64Size;
    MS_U64             u64Interval;
    MS_U8              u8MiuSel;
} DevMalloc_MPool_Info_t;

typedef struct
{
    MS_U64                u64lxAddr;
    MS_U64                u64lxSize;
    MS_U64                u64lx2Addr;
    MS_U64                u64lx2Size;
}DevMalloc_MPool_Kernel_Info_t;


typedef struct
{
    MS_VIRT                virtAddrVirt;
    MS_U64                 u64AddrPhys;
    MS_U64                 u64Size;
} DevMalloc_MPool_Flush_Info_t;




typedef struct{
     unsigned int                ASID;
     unsigned int                global;
     MS_U64                      u64AddrVirt;
     unsigned int                rwx;
     unsigned int                mask;
} DevMalloc_MPool_Watchpt_Info_t;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_PHY HAL_MsOS_MPool_VA2PA(MS_VIRT addrVirt, MS_SIZE tVirt2PhysOff);
MS_VIRT  HAL_MsOS_MPool_PA2VA(MS_PHY u64Phys, MS_PHY u64MappedPhys, MS_U64 u64MappedSize, MS_SIZE tVirt2PhysOff, MS_BOOL bPartialMapping);
MS_BOOL HAL_MsOS_MPool_Dcache_Flush(int fd, MS_VIRT addrVirt, MS_SIZE tSize, MS_PHY u64Phys);
MS_BOOL HAL_MsOS_MPool_Dcache_Flush_All(int fd);
MS_PHY HAL_MsOS_MPool_PA2BA(MS_PHY u64PhyAddr);
MS_PHY HAL_MsOS_MPool_BA2PA(MS_PHY u64Addr);
#endif // _HAL_MPOOL_H_
