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
 ///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvMPool.c
/// @brief  Memory pool manager
///
///////////////////////////////////////////////////////////////////////////////////////////////////

// Common Definition
#include "MsCommon.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define MPOOL_ERROR(fmt, args...)       while (0)
#define MPOOL_WARN(fmt, args...)        while (0)
#define MPOOL_PRINT(fmt, args...)       while (0)
#define MPOOL_ASSERT(_bool, _f)         if (!(_bool)) { (_f); }

#define MPOOL_IS_CACHE_ADDR(addr)       ( ( ((MS_U32)_pAddrVirtStart[0] <= (MS_U32)(addr)) && ((MS_U32)_pAddrVirtEnd[0] > (MS_U32)addr) ) ? TRUE : FALSE )
#define MPOOL_IS_UNCACHE_ADDR(addr)     ( ( ((MS_U32)_pAddrVirtStart[1] <= (MS_U32)(addr)) && ((MS_U32)_pAddrVirtEnd[1] > (MS_U32)addr) ) ? TRUE : FALSE )

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

// static MS_S32              _s32MPoolFd = -1;
// static void*               _pAddrVirtStart[2] = { NULL, NULL};
// static void*               _pAddrVirtEnd[2] = { NULL, NULL};
// static MS_U32              _u32AddrPhys[2] = { 0, 0};
// static MS_U32              _u32Size[2] = { 0, 0};
// static MS_U32              _u32Va2PaOff[2] = { 0, 0};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MsOS_MPool_Init(void)
{
    return TRUE;
}


MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
    return TRUE;
}


inline MS_U32 MsOS_MPool_VA2PA(MS_U32 pAddrVirt)
{
    return (pAddrVirt & 0x1FFFFFFF);
}

inline MS_U32 MsOS_MPool_PA2KSEG1(MS_U32 pAddrPhys) // un-cache
{
    return (pAddrPhys | 0xA0000000);
}

inline MS_U32 MsOS_MPool_PA2KSEG0(MS_U32 pAddrPhys) // cache
{
    return (pAddrPhys | 0x80000000);
}

inline MS_BOOL MsOS_MPool_Dcache_Flush(MS_U32 pAddrVirt, MS_U32 u32Size)
{
    return TRUE;
}

MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache)
{
    MPOOL_WARN("No Implement \n");
    return TRUE;
}
