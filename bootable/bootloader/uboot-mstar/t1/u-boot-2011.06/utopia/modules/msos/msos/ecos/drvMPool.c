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

#include "cyg/kernel/kapi.h"
#include "cyg/hal/hal_arch.h"
#include "cyg/hal/hal_diag.h"
#include "cyg/hal/hal_cache.h"


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "regCHIP.h"
#include "halCHIP.h"
#include "halMMIO.h"
#include "MsVersion.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define MPOOL_DEBUG
#endif

#ifdef MPOOL_DEBUG
    #define MPOOL_WARN(fmt, args...)         (printf("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ##args))
#else
    #define MPOOL_WARN(fmt, args...)         while (0)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if (defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_T7) || defined(CHIP_T9) || defined(CHIP_U4) || defined(CHIP_T13) || defined(CHIP_A1) || defined(CHIP_A6) || defined(CHIP_A7) || defined(CHIP_AMETHYST) || defined(CHIP_EMERALD) || defined(CHIP_NUGGET) || defined(CHIP_NIKON))
#define MIU1_CACHEABLE_START        0xC0000000
#define MIU1_CACHEABLE_END          0xD0000000
#define MIU1_UNCACHEABLE_START      0xD0000000
#define MIU1_UNCACHEABLE_END        0xE0000000
#elif defined(CHIP_K2)
#define MIU1_CACHEABLE_START        0x00000000
#define MIU1_CACHEABLE_END          0x10000000
#define MIU1_UNCACHEABLE_START      0x10000000
#define MIU1_UNCACHEABLE_END        0x20000000
#elif defined(CHIP_KAISER)
#define MIU1_CACHEABLE_START        0x60000000
#define MIU1_CACHEABLE_END          0x50000000
#define MIU1_UNCACHEABLE_START      0x60000000
#define MIU1_UNCACHEABLE_END        0x80000000
#endif

#if (defined(CHIP_KAISER))
//cached/unchched segment
#define KSEG0_BASE              ((void *)0x00000000)
#define KSEG1_BASE              ((void *)0x20000000)
#else
//cached/unchched segment
#define KSEG0_BASE              ((void *)0x80000000)
#define KSEG1_BASE              ((void *)0xa0000000)

#endif

#define MPOOL_ERROR(fmt, args...)       while (0)
#define MPOOL_PRINT(fmt, args...)       while (0)
#define MPOOL_ASSERT(_bool, _f)         if (!(_bool)) { (_f); }
#define MPOOL_IS_CACHE_ADDR(addr)       ( ( ((MS_U32)_pAddrVirtStart[0] <= (MS_U32)(addr)) && ((MS_U32)_pAddrVirtEnd[0] > (MS_U32)addr) ) ? TRUE : FALSE )
#define MPOOL_IS_UNCACHE_ADDR(addr)     ( ( ((MS_U32)_pAddrVirtStart[1] <= (MS_U32)(addr)) && ((MS_U32)_pAddrVirtEnd[1] > (MS_U32)addr) ) ? TRUE : FALSE )
#define DCACHE_LINE_SIZE 16

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

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
    MPOOL_WARN("No Implement \n");
    return TRUE;
}

MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
    MPOOL_WARN("No Implement \n");
    return TRUE;
}

inline MS_U32 MsOS_MPool_VA2PA(MS_U32 addr)
{
#if (defined(MCU_AEON))
    #if (defined(CHIP_T3) || defined(CHIP_U3) || defined(CHIP_JANUS))
            return ((MS_U32)(addr) & ~(0xC0000000));
    #else
            return ((MS_U32)(addr) & ~(0x80000000));
    #endif
#else
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_Edison) || defined(CHIP_EIFFEL) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS))
                // miu0 (low 256MB) (VA)0x8000:0000~0x9000:0000 --> (PA)0x0000:0000~0x1000:0000
                // miu0 (low 256MB) (VA)0xA000:0000~0xB000:0000 --> (PA)0x0000:0000~0x1000:0000
                if ((0x80000000 <= addr) && (0x90000000 > addr))
                {
                    return (addr & 0x1fffffff);
                }
                if ((0xA0000000 <= addr) && (0xB0000000 > addr))
                {
                    return (addr & 0x1fffffff);
                }
                // miu0 (high 256MB) (VA)0xC000:0000~0xD000:0000 -> (PA)0x1000:0000~0x2000:0000
                // miu0 (high 256MB) (VA)0xD000:0000~0xE000:0000 -> (PA)0x1000:0000~0x2000:0000
                if ((0xC0000000 <= addr) && (0xD0000000 > addr))
                {
                    return ((addr - 0xC0000000) + 0x10000000);
                }
                if ((0xD0000000 <= addr) && (0xE0000000 > addr))
                {
                    return ((addr - 0xD0000000) + 0x10000000);
                }
                // miu1 (low 256MB) (VA)0x0000:0000~0x1000:0000 -> (PA)0x2000:0000~0x3000:0000
                // miu1 (low 256MB) (VA)0x1000:0000~0x2000:0000 -> (PA)0x2000:0000~0x3000:0000
                if ((0x00000000 <= addr) && (0x10000000 > addr))
                {
                    return (addr + 0x20000000);
                }
                if ((0x10000000 <= addr) && (0x20000000 > addr))
                {
                    return (addr + 0x10000000);
                }
                // miu1 (high 256MB) (VA)0x2000:0000~0x3000:0000 -> (PA)0x3000:0000~0x4000:0000
                // miu1 (high 256MB) (VA)0x3000:0000~0x4000:0000 -> (PA)0x3000:0000~0x4000:0000
                if ((0x20000000 <= addr) && (0x30000000 > addr))
                {
                    return (addr + 0x10000000);
                }
                if ((0x30000000 <= addr) && (0x40000000 > addr))
                {
                    return addr;
                }
                return 0;
        #elif (defined(CHIP_KENYA))// kenya has only 1 MIU
                // miu0 (low 256MB) (VA)0x8000:0000~0x9000:0000 --> (PA)0x0000:0000~0x1000:0000
                // miu0 (low 256MB) (VA)0xA000:0000~0xB000:0000 --> (PA)0x0000:0000~0x1000:0000
                if ((0x80000000 <= addr) && (0x90000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                if ((0xA0000000 <= addr) && (0xB0000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                // miu0 (high 256MB) (VA)0xC000:0000~0xD000:0000 -> (PA)0x1000:0000~0x2000:0000
                // miu0 (high 256MB) (VA)0xD000:0000~0xE000:0000 -> (PA)0x1000:0000~0x2000:0000
                if ((0xC0000000 <= addr) && (0xD0000000 > addr))
                {
                    return ((addr - 0xC0000000) + 0x10000000);
                }
                if ((0xD0000000 <= addr) && (0xE0000000 > addr))
                {
                    return ((addr - 0xD0000000) + 0x10000000);
                }
                return 0;
        #elif (defined(CHIP_K1) || defined(CHIP_KELTIC) || defined(CHIP_KAPPA))
                // miu0
                // K1 has only 1 MIU
                if ((0x80000000 <= addr) && (0x90000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                if ((0xA0000000 <= addr) && (0xB0000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                return 0;
        #elif (defined(CHIP_KAISER))
                // miu0 (low 256MB) (VA)0x0000:0000~0x1000:0000 --> (PA)0x0000:0000~0x1000:0000
                // miu0 (low 256MB) (VA)0x2000:0000~0x3000:0000 --> (PA)0x0000:0000~0x1000:0000
                if ((0x00000000 <= addr) && (0x10000000 > addr))
                {
                    return (addr);
                }
                if ((0x20000000 <= addr) && (0x30000000 > addr))
                {
                    return (addr & 0x0FFFFFFF);
                }
                // miu0 (high 256MB) (VA)0x1000:0000~0x2000:0000 -> (PA)0x1000:0000~0x2000:0000
                // miu0 (high 256MB) (VA)0x3000:0000~0x4000:0000 -> (PA)0x1000:0000~0x2000:0000
                if ((0x10000000 <= addr) && (0x20000000 > addr))
                {
                    return (addr);
                }
                if ((0x30000000 <= addr) && (0x40000000 > addr))
                {
                    return (addr - 0x20000000);
                }
                // miu1 (low 256MB) (VA)0x4000:0000~0x5000:0000 -> (PA)0x8000:0000~09000:0000
                // miu1 (low 256MB) (VA)0x6000:0000~0x7000:0000 -> (PA)0x8000:0000~0x9000:0000
                if ((0x40000000 <= addr) && (0x50000000 > addr))
                {
                    return (addr + 0x40000000);
                }
                if ((0x60000000 <= addr) && (0x70000000 > addr))
                {
                    return (addr + 0x20000000);
                }
                // miu1 (high 256MB) (VA)0x5000:0000~0x6000:0000 -> (PA)0x9000:0000~0xA000:0000
                // miu1 (high 256MB) (VA)0x7000:0000~0x8000:0000 -> (PA)0x9000:0000~0xA000:0000
                if ((0x50000000 <= addr) && (0x60000000 > addr))
                {
                    return (addr + 0x40000000);
                }
                if ((0x70000000 <= addr) && (0x80000000 > addr))
                {
                    return  (addr + 0x20000000);
                }
                return 0;
        #else
                // miu0
                if ((0x80000000 <= addr) && (0x90000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                if ((0xA0000000 <= addr) && (0xB0000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                // miu1
                if ((MIU1_CACHEABLE_START <= addr) && (MIU1_CACHEABLE_END > addr))
                {
                    return ((addr & ~MIU1_CACHEABLE_START) | (HAL_MIU1_BASE));
                }
                if ((MIU1_UNCACHEABLE_START <= addr) && (MIU1_UNCACHEABLE_END > addr))
                {
                    return ((MS_U32)(((MS_U32)(addr & ~MIU1_UNCACHEABLE_START)) | (HAL_MIU1_BASE)));
                }
                return 0;
        #endif
    #else
            return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
    #endif
#endif

}

inline MS_U32 MsOS_MPool_PA2KSEG1(MS_U32 addr) // un-cache
{
#if (defined(MCU_AEON))
    #if (defined(CHIP_T3) || defined(CHIP_U3))
            return ((MS_U32)(addr) | (0xC0000000));
    #else
            return ((MS_U32)(addr) | (0x80000000));
    #endif
#else // for mips and refine it later
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_Edison) || defined(CHIP_EIFFEL) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS))
                // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> 0xA000:0000~0xB000:0000 uncached
                if ((0x00000000 <= addr) && (0x10000000 > addr))
                {
                    return (addr + 0xA0000000);
                }
                // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> 0xD000:0000~0xE000:0000 uncached
                if ((0x10000000 <= addr) && (0x20000000 > addr))
                {
                    return ((addr - 0x10000000) + 0xD0000000);
                }
                // miu1 (low 256MB) - (PA)0x2000:0000~0x3000:0000 -> 0x1000:0000~0x2000:0000 uncached
                if ((0x20000000 <= addr) && (0x30000000 > addr))
                {
                    return (addr - 0x10000000);
                }
                // miu1 (high 256MB) - (PA)0x3000:0000~0x4000:0000 -> 0x3000:0000~0x4000:0000 uncached
                if ((0x30000000 <= addr) && (0x40000000 > addr))
                {
                    return addr;
                }
                return 0;
        #elif (defined(CHIP_KENYA))
                // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> 0xA000:0000~0xB000:0000 uncached
                if ((0x00000000 <= addr) && (0x10000000 > addr))
                {
                    return (addr + 0xA0000000);
                }
                // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> 0xD000:0000~0xE000:0000 uncached
                if ((0x10000000 <= addr) && (0x20000000 > addr))
                {
                    return ((addr - 0x10000000) + 0xD0000000);
                }
                return 0;
        #elif (defined(CHIP_K1) || defined(CHIP_KELTIC) || defined(CHIP_KAPPA))
                // miu0
                if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) | 0xa0000000));
                }
                return 0;
        #elif (defined(CHIP_KAISER))
               // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> 0x2000:0000~0x3000:0000 uncached
                if ((0x00000000 <= addr) && (0x10000000 > addr))
                {
                    return (addr + 0x20000000);
                }
                // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> 0x3000:0000~0x4000:0000 uncached
                if ((0x10000000 <= addr) && (0x20000000 > addr))
                {
                    return (addr + 0x20000000);
                }
                // miu1 (low 256MB) - (PA)0x8000:0000~0x9000:0000 -> 0x6000:0000~0x7000:0000 uncached
                if ((0x80000000 <= addr) && (0x90000000 > addr))
                {
                    return (addr - 0x20000000);
                }
                // miu1 (high 256MB) - (PA)0x9000:0000~0xA000:0000 -> 0x7000:0000~0x8000:0000 uncached
                if ((0x90000000 <= addr) && (0xA0000000 > addr))
                {
                    return (addr - 0x20000000);
                }
                return 0;
        #else
                // miu0
                if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) | 0xa0000000));
                }
                // miu1
                if ((HAL_MIU1_BASE <= addr) && ((HAL_MIU1_BASE * 2) > addr))
                {
                    return ((addr & ~(HAL_MIU1_BASE)) | 0xD0000000);
                }
                return 0;
        #endif
    #else
            return ((MS_U32)(((MS_U32)addr) | 0xa0000000));
    #endif
#endif

}

inline MS_U32 MsOS_MPool_PA2KSEG0(MS_U32 addr) // cache
{
#if (defined(MCU_AEON))
        return (addr);
#else // for mips and refine it later
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_Edison) || defined(CHIP_EIFFEL) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS))
                // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> (VA)0x8000:0000~0x9000:0000 cached
                if ((0x00000000 <= addr) && (0x10000000 > addr))
                {
                    return (addr + 0x80000000);
                }
                // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> (VA)0xC000:0000~0xD000:0000 cached
                if ((0x10000000 <= addr) && (0x20000000 > addr))
                {
                    return ((addr - 0x10000000) + 0xC0000000);
                }
                // miu1 (low 256MB) - (PA)0x2000:0000~0x3000:0000 -> (VA)0x0000:0000~0x1000:0000 cached
                if ((0x20000000 <= addr) && (0x30000000 > addr))
                {
                    return (addr - 0x20000000);
                }
                // miu1 (high 256MB) - (PA)0x3000:0000~0x4000:0000 -> (VA)0x2000:0000~0x3000:0000 cached
                if ((0x30000000 <= addr) && (0x40000000 > addr))
                {
                    return (addr - 0x10000000);
                }
                return 0;
        #elif (defined(CHIP_KENYA))
                // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> (VA)0x8000:0000~0x9000:0000 cached
                if ((0x00000000 <= addr) && (0x10000000 > addr))
                {
                    return (addr + 0x80000000);
                }
                // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> (VA)0xC000:0000~0xD000:0000 cached
                if ((0x10000000 <= addr) && (0x20000000 > addr))
                {
                    return ((addr - 0x10000000) + 0xC0000000);
                }
                return 0;
        #elif (defined(CHIP_K1) || defined(CHIP_KELTIC)|| defined(CHIP_KAPPA))
                // miu0
                if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) | 0x80000000));
                }
                return 0;
        #elif (defined(CHIP_KAISER))
                // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> (VA)0x0000:0000~0x1000:0000 cached
                if ((0x00000000 <= addr) && (0x10000000 > addr))
                {
                    return (addr);
                }
                // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> (VA)0x1000:0000~0x2000:0000 cached
                if ((0x10000000 <= addr) && (0x20000000 > addr))
                {
                    return (addr);
                }
                // miu1 (low 256MB) - (PA)0x8000:0000~0x9000:0000 -> (VA)0x4000:0000~0x5000:0000 cached
                if ((0x80000000 <= addr) && (0x90000000 > addr))
                {
                    return (addr - 0x40000000);
                }
                // miu1 (high 256MB) - (PA)0x9000:0000~0xA000:0000 -> (VA)0x5000:0000~0x6000:0000 cached
                if ((0x90000000 <= addr) && (0xA0000000 > addr))
                {
                    return (addr - 0x40000000);
                }
                return 0;
        #else
                // miu0
                if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) | 0x80000000));
                }
                // miu1
                if ((HAL_MIU1_BASE <= addr) && ((HAL_MIU1_BASE * 2) > addr))
                {
                    return ((addr & ~(HAL_MIU1_BASE)) | 0xC0000000);
                }
                return 0;
        #endif
    #else
            return ((MS_U32)(((MS_U32)addr) | 0x80000000));
    #endif
#endif

}


inline MS_BOOL MsOS_MPool_Dcache_Flush(MS_U32 u32Start, MS_U32 u32Size)
{
    MS_U32 u32OldIntr;
    MS_U32 u32Addr;

    u32Addr = u32Start & 0xFFFFFFF0;
    u32Size+= (u32Start - u32Addr);
    u32Size = ALIGN_16(u32Size);

    if ( (u32Addr % DCACHE_LINE_SIZE) || ( (u32Addr & (MS_U32)KSEG1_BASE) != (MS_U32)KSEG0_BASE) )
    {
        return FALSE;
    }
    u32OldIntr = MsOS_DisableAllInterrupts();
    HAL_DCACHE_FLUSH( u32Addr, u32Size);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    MsOS_RestoreAllInterrupts(u32OldIntr);
    return TRUE;
}

MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache)
{
    MPOOL_WARN("No Implement \n");
    return TRUE;
}
