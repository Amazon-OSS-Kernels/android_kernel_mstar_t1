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

 
#ifndef _OS_API_H_
#define _OS_API_H_

#include <MsCommon.h>
//#include <cyg/hal/hal_if.h> // NUSED
// Note, include for kernel service functions of eCos
//#include <cyg/kernel/kapi.h> // NUSED
//#include <cyg/hal/hal_intr.h> // NUSED
#include <cyg/hal/hal_misc.h> // for BITn
#include "drvPorts.h"

#define IRQ_NONE              0
#define IRQ_HANDLED           1
#define TASK_UNINTERRUPTIBLE  0
/* ========================================  Data structures  ======================================== */

typedef int spinlock_t;
typedef int atomic_t;

#define SPIN_LOCK_UNLOCKED  (spinlock_t)0
#define SPIN_LOCK_LOCKED    (spinlock_t)1

/* ========================================  Function Declaration  ======================================== */
#define DECLARE_WAITQUEUE(x,y)
#define init_waitqueue_head(x)
#define add_wait_queue(x,y)
#define remove_wait_queue(x,y)
#define wake_up(x)
#define signal_pending(x)     0

// Spin lock functions
#define osapi_spin_lock_init(lock)
#define osapi_spin_lock(lock)
#define osapi_spin_unlock(lock)
#define osapi_spin_lock_irqsave(lock,flag)      {flag = MsOS_DisableAllInterrupts();}
#define osapi_spin_unlock_irqrestore(lock,flag) MsOS_RestoreAllInterrupts(flag)
#define osapi_spin_lock_irq(lock)
#define osapi_spin_unlock_irq(lock)
// Semaphore functions
#define osapi_up(x)
#define osapi_down(x)
// Mutex functions
#define osapi_init_MUTEX(x)
#define osapi_init_MUTEX_LOCKED(x)
#define osapi_atomic_read(v)        *(v)
#define osapi_ATOMIC_INIT(value)    value
// Kernel function porting (Unsupport now, so just skip)
#define set_current_state(x)
#define osapi_local_irq_restore(x)
#define osapi_local_irq_save(x)
#define osapi_local_irq_disable()
#define osapi_local_irq_enable()
#define osapi_lock_kernel()
#define osapi_unlock_kernel()
#define osapi_schedule()

static __inline__ void osapi_atomic_inc(int *cnt_p)
{
  (*cnt_p)++;
}

static __inline__ void osapi_atomic_dec(int *cnt_p)
{
  (*cnt_p)--;
}

static __inline__ int osapi_atomic_dec_and_test(int *count)
{
  (*count)--;
  return (*count == 0);
}

static __inline__ void osapi_atomic_set(int *count, int value)
{
  (*count) = value;
}

static __inline__ void osapi_schedule_timeout(int ms)
{
	mdelay(ms);
}
#endif

