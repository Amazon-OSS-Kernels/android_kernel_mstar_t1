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


#ifndef _LINUX_TIMER_H_
#define _LINUX_TIMER_H_

#include <MsTypes.h>
#include "drvPorts.h"
#include "drvOSAPI.h"
#include "drvList.h"

struct timer_list {
  struct list_head entry;
  int expires;
  int base_jiffies;

  spinlock_t lock;
  U32 magic;

  void (*function)(U32);
  U32 data;
  int tmout_value;
  int tmout_step;
  U32 data_ex;
};

#define TIMER_MAGIC  0x4b87ad6e

/*
#define TIMER_INITIALIZER(_function, _expires, _data) {    \
    .function = (_function),      \
    .expires = (_expires),        \
    .data = (_data),        \
    .base = NULL,          \
    .magic = TIMER_MAGIC,        \
    .lock = SPIN_LOCK_UNLOCKED,      \
  }
*/

#define TIMER_INITIALIZER(var, _function, _expires, _data) {    \
    var.function = (_function),      \
    var.expires = (_expires),        \
    var.data = (_data),        \
    var.base = NULL,          \
    var.magic = TIMER_MAGIC,        \
    var.lock = SPIN_LOCK_UNLOCKED,      \
  }

/***
 * init_timer - initialize a timer.
 * @timer: the timer to be initialized
 *
 * init_timer() must be done to a timer prior calling *any* of the
 * other timer functions.
 */

static __inline__ void init_timer(struct timer_list * timer)
{
  timer->magic = TIMER_MAGIC;
  timer->tmout_step = 0;
  timer->tmout_value = 0;
  INIT_LIST_HEAD(&timer->entry);
  spin_lock_init(&timer->lock);
}

/***
 * timer_pending - is a timer pending?
 * @timer: the timer in question
 *
 * timer_pending will tell whether a given timer is currently pending,
 * or not. Callers must ensure serialization wrt. other operations done
 * to this timer, eg. interrupt contexts, or other CPUs on SMP.
 *
 * return value: 1 if the timer is pending, 0 if not.
 */
 
#define timer_pending(x)  ((x)->expires>0)
/*
 *  These inlines deal with timer wrapping correctly. You are 
 *  strongly encouraged to use them
 *  1. Because people otherwise forget
 *  2. Because if the timer wrap changes in future you won't have to
 *     alter your driver code.
 *
 * time_after(a,b) returns true if the time a is after time b.
 *
 * Do this with "<0" and ">=0" to only test the sign of the result. A
 * good compiler would generate better code (and a really good compiler
 * wouldn't care). Gcc is currently neither.
 */
#define time_after(a,b)    ( ((int)(b) - (int)(a)) < 0 )
#define time_before(a,b)  time_after(b,a)

#define time_after_eq(a,b)  ( ((int)(a) - (int)(b)) >= 0 )
#define time_before_eq(a,b)  time_after_eq(b,a)


extern U32 volatile jiffies;

extern void add_timer(struct timer_list * timer);
extern int del_timer(struct timer_list * timer);
extern int del_timer_sync(struct timer_list * timer);
extern int mod_timer(struct timer_list *timer, U32 expires, U32 steps);
extern void init_timers(void);
extern void remove_timers(void);

#endif

