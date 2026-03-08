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


#ifndef _DRV_TIMER_H_
#define _DRV_TIMER_H_

#include <MsTypes.h>
#include "drvPorts.h"
#include "drvOSAPI.h"
#include "drvList.h"

struct timer_list 
{
    struct list_head entry;
    int expires;
    //int base_jiffies; // NUSED

    spinlock_t lock;    

    void (*function)(MS_U32);
    MS_U32 data;
    int tmout_value;
    int tmout_step;
    MS_U32 data_ex;
};

static __inline__ void ms_init_timer(struct timer_list * st_timer)
{    
    st_timer->tmout_step = 0;
    st_timer->tmout_value = 0;
    ms_list_init(&st_timer->entry);
    osapi_spin_lock_init(&st_timer->lock);
}

#define ms_timer_pending(x)  ((x)->expires>0)
#define ms_time_after(a,b)    ( ((int)(b) - (int)(a)) < 0 )
#define ms_time_before(a,b)  ms_time_after(b,a)

#define ms_time_after_eq(a,b)  ( ((int)(a) - (int)(b)) >= 0 )
#define ms_time_before_eq(a,b)  ms_time_after_eq(b,a)


extern MS_U32 volatile jiffies;

extern void ms_add_timer(struct timer_list * timer);
extern int ms_del_timer(struct timer_list * timer);
extern int ms_del_timer_sync(struct timer_list * timer);
extern int ms_update_timer(struct timer_list *timer, MS_U32 expires, MS_U32 steps);
extern void ms_init_timers(void);
extern void ms_remove_timers(void);

#define mod_timer(t,e,s) ms_update_timer(t,e,s)
#endif

