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


#ifndef _PORTS_H_
#define _PORTS_H_

//#include <cyg/hal/hal_arch.h> // NUSED
#include <MsCommon.h>

//#undef  CONFIG_FARADAY_USB // NUSED
//#define CONFIG_FARADAY_USB // NUSED


#define USB_ASSERT( _bool_, _msg_ ) \
        do { \
            if ( ! ( _bool_ ) ) \
             {     \
                diag_printf("[%s]USB ASSERT %d: "_msg_, __FILE__, __LINE__); \
                while (1){} } \
        } while (0);



/* Cache instruction opcodes */
#define HAL_CACHE_OP(which, op)             (which | (op << 2))
#define HAL_DCACHE_HIT_INVALIDATE           0x5             // write back and invalidate
#define HAL_HIT_INVALIDATE                  0x4             // address invalidate

#define HAL_WHICH_ICACHE                    0x0
#define HAL_WHICH_DCACHE                    0x1

///#define HAL_DCACHE_LINE_SIZE            16      // Size of a data cache line

#define HAL_MIPS32_DCACHE_START_ADDRESS(_addr_) \
(((MS_U32)(_addr_)) & ~(HAL_DCACHE_LINE_SIZE-1))

#define HAL_MIPS32_DCACHE_END_ADDRESS(_addr_, _asize_) \
(((MS_U32)((_addr_) + (_asize_) + (HAL_DCACHE_LINE_SIZE-1) )) & ~(HAL_DCACHE_LINE_SIZE-1))


#if 0 // NUSED

#define USBHAL_DCACHE_FLUSH( _base_ , _asize_ )                           \
  do {                                                   \
    register U32 _addr_  = HAL_MIPS32_DCACHE_START_ADDRESS((U32)(_base_));        \
    register U32 _eaddr_ = HAL_MIPS32_DCACHE_END_ADDRESS((U32)(_base_), _asize_); \
    for( ; _addr_ < _eaddr_; _addr_ += HAL_DCACHE_LINE_SIZE )               \
      asm volatile (" cache %0, 0(%1)"                                                  \
                    :                                                                   \
                    : "I" (HAL_CACHE_OP(HAL_WHICH_DCACHE,  HAL_DCACHE_HIT_INVALIDATE)),         \
                      "r"(_addr_));                                                       \
     } while (0)

#define USBHAL_DCACHE_INVALIDATE( _base_ , _asize_ )                           \
    do {                                                        \
    register U32 _addr_  = HAL_MIPS32_DCACHE_START_ADDRESS( (U32)(_base_) );        \
    register U32 _eaddr_ = HAL_MIPS32_DCACHE_END_ADDRESS( (U32)(_base_) , (_asize_) ); \
    for( ; _addr_ < _eaddr_; _addr_ += HAL_DCACHE_LINE_SIZE )               \
      asm volatile (" cache %0, 0(%1)"                                                  \
                    :                                                                   \
                    : "I" (HAL_CACHE_OP(HAL_WHICH_DCACHE, HAL_HIT_INVALIDATE)),         \
                      "r"(_addr_));                                                       \
     } while (0)
#endif
#define wmb() HAL_REORDER_BARRIER()
#define rmb() HAL_REORDER_BARRIER()
#define barrier() HAL_REORDER_BARRIER()

#define	USB_sprintf	   sprintf //UTL_sprintf

// Delay routines
#define mdelay(x)  MsOS_DelayTask(x)
// Linux Device Driver related ports
extern void wait_ms(int x);

#define __init
#define __exit

// Prefetch for List (ARM v4 doesn't supports this feature
static __inline__ void prefetch(void *x) {;}  // Do nothing

/* ========================================  Function Declaration  ======================================== */
extern void udelay(MS_U32 us);

/* ========================================  Data structures  ======================================== */

// Used for ISR to stored the processor's context befor the processor entered interrupt code
struct stPtRegs {
  MS_U32 uregs[18];
};
#define ARM_cpsr  uregs[16]
#define ARM_pc    uregs[15]
#define ARM_lr    uregs[14]
#define ARM_sp    uregs[13]
#define ARM_ip    uregs[12]
#define ARM_fp    uregs[11]
#define ARM_r10   uregs[10]
#define ARM_r9    uregs[9]
#define ARM_r8    uregs[8]
#define ARM_r7    uregs[7]
#define ARM_r6    uregs[6]
#define ARM_r5    uregs[5]
#define ARM_r4    uregs[4]
#define ARM_r3    uregs[3]
#define ARM_r2    uregs[2]
#define ARM_r1    uregs[1]
#define ARM_r0    uregs[0]
#define ARM_ORIG_r0  uregs[17]

#define SLAB_KERNEL   0     //unmeaning
#define SLAB_ATOMIC   1     //unmeaning
#define GFP_KERNEL    2     //unmeaning
#define GFP_ATOMIC    3     //unmeaning
#define GFP_NOIO      4     //unmeaning

#define min_t(type,x,y) \
  ( (type)x < (type)y ? (type) x : (type) y)
#define max_t(type,x,y) \
  ( (type)x > (type)y ? (type) x : (type) y)

#ifdef min
#undef min
#define min(x,y) \
  ( x < y ?  x :  y)
#endif
#ifdef max
#undef max
#define max(x,y) \
  ( x > y ? x : y)
#endif

extern void ms_init_sys(void);
extern void ms_exit_sys(void);
#endif


