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

#include <cyg/hal/hal_arch.h>
#include "MsCommon.h"
#include "../drvUsbcommon.h"

#undef  CONFIG_FARADAY_USB
#define CONFIG_FARADAY_USB


#define USB_ASSERT( _bool_, _msg_ ) \
        do { \
            if ( ! ( _bool_ ) ) \
             {   while (1){} } \
        } while (0);



/* Cache instruction opcodes */
#define HAL_CACHE_OP(which, op)             (which | (op << 2))
#define HAL_DCACHE_HIT_INVALIDATE           0x5             // write back and invalidate
#define HAL_HIT_INVALIDATE                  0x4             // address invalidate

#define HAL_WHICH_ICACHE                    0x0
#define HAL_WHICH_DCACHE                    0x1

///#define HAL_DCACHE_LINE_SIZE            16      // Size of a data cache line

#define HAL_MIPS32_DCACHE_START_ADDRESS(_addr_) \
(((U32)(_addr_)) & ~(HAL_DCACHE_LINE_SIZE-1))

#define HAL_MIPS32_DCACHE_END_ADDRESS(_addr_, _asize_) \
(((U32)((_addr_) + (_asize_) + (HAL_DCACHE_LINE_SIZE-1) )) & ~(HAL_DCACHE_LINE_SIZE-1))


#if 0

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

// Debug Function
#define KERN_DEBUG          "KERN_DEBUG "
#define KERN_ERR            "KERN_ERR "
#define KERN_INFO           "KERN_INFO "
#define KERN_WARNING        "KERN_WARNING "

#define printk(x,...)

#define	USB_sprintf	   sprintf //UTL_sprintf

#define CPUToLE32(_x_) (_x_)
#define	LE16ToCPU(x)	(x)
#define	CPUToLE16(x)	(x)
#define	LE32ToCPU(x)	(x)
// Delay routines
#define DelayTime(x)	MsOS_DelayTask(x)

//#define wait_ms(x) DelayTime(x)
#define mdelay(x)  DelayTime(x)
// Linux Device Driver related ports
extern void wait_ms(int x);

#define __init
#define __exit

// Prefetch for List (ARM v4 doesn't supports this feature
static __inline__ void prefetch(void *x) {;}  // Do nothing

/* ========================================  Function Declaration  ======================================== */
extern void udelay(MS_U32 us);
extern void busyloop_delay(MS_U32 count);

/* ========================================  Data structures  ======================================== */

// Used for ISR to stored the processor's context befor the processor entered interrupt code
struct pt_regs {
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

struct pci_dev{
  void *dev;
};

#define CPE_FAKE_PCIDEV ((struct pci_dev *) 1111)
#define pcidev_is_CPE(dev) (dev == CPE_FAKE_PCIDEV)

// Timer data structure for Root hub

#define SLAB_KERNEL   0     //unmeaning
#define SLAB_ATOMIC   1     //unmeaning
#define GFP_KERNEL    2     //unmeaning
#define GFP_ATOMIC    3     //unmeaning
#define GFP_NOIO      4     //unmeaning
#define ASYNC_MAKE_QTD		5  //Asynchronous IO mode for making qtd list
#define ASYNC_SUBMIT_QTD	6  //Asynchronous IO mode for sumbitting qtd list

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

#define BUG()
extern void init_sys(void);
extern void exit_sys(void);
#endif


