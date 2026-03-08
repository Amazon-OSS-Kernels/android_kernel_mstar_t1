/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//******************************************************************************
#include <stdio.h>
#include <assert.h>
#include "datatype.h"
#include "mhal_arch.h"

#define ALIGN_16(_x)            ((_x + 15) & ~15)           // No data type specified, optimized by complier
#define KSEG0_BASE	            ((void *)0x80000000)
#define KSEG1_BASE	            ((void *)0xa0000000)

// Data cache

#define NOS_HAL_DCACHE_SIZE                 4096                        // Size of data cache in bytes
#define NOS_HAL_DCACHE_LINE_SIZE            16                          // Size of a data cache line
#define NOS_HAL_DCACHE_WAYS                 2                           // Associativity of the cache

#define NOS_HAL_MIPS_CACHE_HIT_WRITEBACK_INV_D   0x15 /* 5 1 */

#define NOS_HAL_DCACHE_START_ADDRESS(_addr_) \
(((U32)(_addr_)) & ~(NOS_HAL_DCACHE_LINE_SIZE-1))
#define NOS_HAL_DCACHE_END_ADDRESS(_addr_, _asize_) \
((U32)((_addr_) + (_asize_)))
#define NOS_HAL_DCACHE_IS_ENABLED(_state_) (_state_) = 1;
#define _nos_hal_asm_mips_cpp_stringize( _x_ ) #_x_
#define _NOS_HAL_ASM_SET_MIPS_ISA( _isal_ ) asm volatile ( \
      ".set mips" _nos_hal_asm_mips_cpp_stringize(_isal_) )
#define _NOS_DWAY(_n_) ((_n_)*NOS_HAL_DCACHE_SIZE/NOS_HAL_DCACHE_WAYS)
#define _NOS_HAL_ASM_DCACHE_ALL_WAYS( _cmd_ , _addr_ )      \
    asm volatile ("cache %0,0(%1);"                     \
                  "cache %0,%2(%1);"                    \
                    : : "I" ((_cmd_) | 1), "r"(_addr_), \
                        "I" (_NOS_DWAY(1)))

extern void   MAsm_CPU_Sync(void);

void nos_hal_dcache_flush( U32 base , U32 asize )
{
    register U32 addr  = NOS_HAL_DCACHE_START_ADDRESS(base);
    register U32 eaddr = NOS_HAL_DCACHE_END_ADDRESS(base, asize);
    register U32 state;
    NOS_HAL_DCACHE_IS_ENABLED( state );
    if( state ) {
        _NOS_HAL_ASM_SET_MIPS_ISA(3);
        for( ; addr < eaddr; addr += NOS_HAL_DCACHE_LINE_SIZE )
        { _NOS_HAL_ASM_DCACHE_ALL_WAYS(NOS_HAL_MIPS_CACHE_HIT_WRITEBACK_INV_D, addr); }
        _NOS_HAL_ASM_SET_MIPS_ISA(0);
    }
}
void mhal_dcache_flush( U32 u32Base, U32 u32Size )
{
    U32 u32Addr;

    u32Addr = u32Base & 0xFFFFFFF0;
    u32Size+= (u32Base - u32Addr);
    u32Size = ALIGN_16(u32Size);

    //U32 u32OldIntr;
    if ( (u32Addr % NOS_HAL_DCACHE_LINE_SIZE) || ( (u32Addr & (U32)KSEG1_BASE) != (U32)KSEG0_BASE) )
    {
        return;
    }
    //u32OldIntr = MsOS_DisableAllInterrupts();
    nos_hal_dcache_flush( u32Addr, u32Size);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    //MsOS_RestoreAllInterrupts(u32OldIntr);
    //return TRUE;
}