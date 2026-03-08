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

//=============================================================================
#include <MsCommon.h>

#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include "include/drvTimer.h"
#include "include/drvKernel.h"
#include "include/drvList.h"

U32 volatile jiffies;

//void SW_Timer_Tick(U32 u32Value);
void SW_Timer_Tick(U32 u32StTimer, U32 u32TimerID);

static struct list_head sw_timer_vec_head;

S32 g_Timer_ID;

#ifdef TIMER_NO_ALL_INT_DISABLE
#include "drvHUB.h"
extern struct s_ChipUsbHostDef *pCurrentChip;
void disable_USB_interrupts(void)
{
    U8 i;

    for (i=0; i<pCurrentChip->nRootHub; i++)
        MsOS_DisableInterrupt(pCurrentChip->reg[i].uhcIRQ);
}

void enable_USB_interrupts(void)
{
    U8 i;
    
    for (i=0; i<pCurrentChip->nRootHub; i++)
        MsOS_EnableInterrupt(pCurrentChip->reg[i].uhcIRQ);
}
#endif

S32 _s32MutexUTimer;
void init_timers(void)
{
	INIT_LIST_HEAD(&sw_timer_vec_head);
	// Register software timer dispatch program
	// Base 1 ms, interruptted every 1 ms

	BOOL err;

	g_Timer_ID = MsOS_CreateTimer((TimerCb) SW_Timer_Tick,100, 100, FALSE, "USB_TIMER");
	//err = OS_DefineCyclicHandler(&g_Timer_ID, SW_Timer_Tick, (void*) 0, 100);
	//USB_ASSERT((err == TRUE), "Define USB timer fail");

	//err = OS_ActivateCyclicHandler(g_Timer_ID);
	//
	err=MsOS_StartTimer(g_Timer_ID);
	USB_ASSERT((err == TRUE), "Activate USB timer fail");
	//kdbg("Init Software Timer ok %s","\n");
	printk("Init Software Timer ok \n");
        _s32MutexUTimer = MsOS_CreateMutex(E_MSOS_FIFO, "UTimer_MUTEX", MSOS_PROCESS_SHARED);
}

void remove_timers(void)
{
	MsOS_DeleteTimer(g_Timer_ID);
	printk("Remove Software Timer ok \n");
        MsOS_DeleteMutex(_s32MutexUTimer);
}

/*
 * Timer ISR: It's responsible for dispatching tasks registered to software-timer
 * task queue. It need to be implemented as Bottom-Half mechanism of Linux
 * Timer and USB IRQ have the same priority and cannot be interrupted by each other
 */
//void SW_Timer_Tick(U32 pData)
void SW_Timer_Tick(U32 u32StTimer, U32 u32TimerID)
{
	struct timer_list *timer;
	struct list_head  *tmp;

	jiffies+=100;

	if ( Timer_task_lock == 1 )
		goto sw_done;

	// Dispath task from task queue of software-timer here
	if (!list_empty(&sw_timer_vec_head))
	{
		tmp = sw_timer_vec_head.next;
		while (tmp != &sw_timer_vec_head)
		{
			timer = list_entry (tmp, struct timer_list, entry);
			tmp = tmp->next;
			//list_entry (tmp, struct timer_list, entry, struct list_head, timer);
			// Check if time expired ?
			timer->expires -= 100;
			if ( timer->expires <= 0 )
			{
				void (*fn)(U32);
				U32 data;
				// If lock ==> don't dispatch task
				/*
				if ( Timer_task_lock == 1 )
				{
				  timer->expires++;
				  continue;
				}
				*/
				fn = timer->function;
				data = timer->data;
				del_timer(timer);
				// Call registered function
				if ( (U32)fn )
				{
					lock_usb_core();
					fn(data);
					unlock_usb_core();
				}
				else
				{
					printk("ERROR: fun is NULL\n");
					while(1);
				}
				//Current version only support dispatch one task each time
				//break;
			}
			/*
			if ( (U32)tmp->next == (U32)0x1 )
			while(1);
			if ( tmp->next == NULL )
			while(1);*/
		}
	}
sw_done:
	return;
	// End of dispatching
}

#ifdef TIMER_NO_ALL_INT_DISABLE
void timerSectionIn(void)
{
    MsOS_StopTimer(g_Timer_ID);
    disable_USB_interrupts();
    MsOS_ObtainMutex(_s32MutexUTimer, MSOS_WAIT_FOREVER);
}
void timerSectionOut(void)
{
    enable_USB_interrupts();
    MsOS_StartTimer(g_Timer_ID);
    MsOS_ReleaseMutex(_s32MutexUTimer);
}
int del_timer_body(struct timer_list *timer)
{
     //MsOS_StopTimer(g_Timer_ID);
     //disable_USB_interrupts();

	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

     //enable_USB_interrupts();
     //MsOS_StartTimer(g_Timer_ID);

	//TIMER_TASK_LOCK = flags;
	return 1;
}
int del_timer(struct timer_list *timer)
{
    //timerSectionIn();
    del_timer_body(timer);
    //timerSectionOut();
    return 1;
}
int del_timer_sync_body(struct timer_list *timer)
{
    //MsOS_StopTimer(g_Timer_ID);
    //disable_USB_interrupts();

	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

     //enable_USB_interrupts();
     //MsOS_StartTimer(g_Timer_ID);

	//TIMER_TASK_LOCK = flags;
	return 1;
}
int del_timer_sync(struct timer_list *timer)
{
    timerSectionIn();
    del_timer_sync_body(timer);
    timerSectionOut();
    return 1;
}
void add_timer_body(struct timer_list *timer)
{
    //MsOS_StopTimer(g_Timer_ID);
    //disable_USB_interrupts();

	if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
		timer->expires = timer->tmout_step;
	else
		timer->expires = timer->tmout_value;

	list_add_tail(&timer->entry, &sw_timer_vec_head);

     //enable_USB_interrupts();
     //MsOS_StartTimer(g_Timer_ID);

	//TIMER_TASK_LOCK = flags;
}
void add_timer(struct timer_list *timer)
{
    timerSectionIn();
    add_timer_body(timer);
    timerSectionOut();
}
int mod_timer_body(struct timer_list *timer, U32 expires, U32 steps)
{
	//U32 flags;
	int ret = 0;

    //MsOS_StopTimer(g_Timer_ID);
    //disable_USB_interrupts();

	timer->tmout_value = expires;
	timer->tmout_step = steps;

	//ret = del_timer(timer);
	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

	//add_timer(timer);
	if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
		timer->expires = timer->tmout_step;
	else
		timer->expires = timer->tmout_value;
	list_add_tail(&timer->entry, &sw_timer_vec_head);

     //enable_USB_interrupts();
     //MsOS_StartTimer(g_Timer_ID);

	return ret;
}
int mod_timer(struct timer_list *timer, U32 expires, U32 steps)
{    
    int ret = 0;
    //timerSectionIn();
    ret = mod_timer_body(timer, expires, steps);
    //timerSectionOut();
    return ret;
}
#else // Disable All Interrupts
/***
 * del_timer - deactive a timer.
 * @timer: the timer to be deactivated
 *
 * del_timer() deactivates a timer - this works on both active and inactive
 * timers.
 *
 * The function returns whether it has deactivated a pending timer or not.
 * (ie. del_timer() of an inactive timer returns 0, del_timer() of an
 * active timer returns 1.)
 */
int del_timer(struct timer_list *timer)
{
	//U32 flags;

	//flags = TIMER_TASK_LOCK;
	//lock_TmrTaskDispatch();
	//save_and_disable_firqs(&flags);
	//HAL_DISABLE_INTERRUPTS(flags);
	//OS_DisableDispatch();yuwen
	//MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
	 U32 u32OldIntr;
     u32OldIntr = MsOS_DisableAllInterrupts();

	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

	//restore_firqs(&flags);
	//HAL_RESTORE_INTERRUPTS(flags);
	//OS_EnableDispatch();yuwen
	//MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
     MsOS_RestoreAllInterrupts(u32OldIntr);

	//TIMER_TASK_LOCK = flags;
	return 1;
}

/***
 * del_timer_sync - deactive a timer.
 * @timer: the timer to be deactivated
 *
 * del_timer_sync() deactivates a timer - this works on both active and inactive
 * timers.
 *
 * The function will protect synchronization problem before deactivating timer
 */
int del_timer_sync(struct timer_list *timer)
{
	//U32 flags;

	//flags = TIMER_TASK_LOCK;
	//lock_TmrTaskDispatch();
	//save_and_disable_firqs(&flags);
	//HAL_DISABLE_INTERRUPTS(flags);
	//OS_DisableDispatch();
	//MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
     U32 u32OldIntr;
     u32OldIntr = MsOS_DisableAllInterrupts();

	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

	//restore_firqs(&flags);
	//HAL_RESTORE_INTERRUPTS(flags);
	//OS_EnableDispatch();
	//MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
     MsOS_RestoreAllInterrupts(u32OldIntr);

	//TIMER_TASK_LOCK = flags;
	return 1;
}

/***
 * add_timer - start a timer
 * @timer: the timer to be added
 *
 * The kernel will do a ->function(->data) callback from the
 * timer interrupt at the ->expired point in the future. The
 * current time is 'jiffies'.
 *
 * The timer's ->expired, ->function (and if the handler uses it, ->data)
 * fields must be set prior calling this function.
 *
 * Timers with an ->expired field in the past will be executed in the next
 * timer tick. It's illegal to add an already pending timer.
 */
void add_timer(struct timer_list *timer)
{
	//U32 flags;

	//flags = TIMER_TASK_LOCK;
	//lock_TmrTaskDispatch();
	//save_and_disable_firqs(&flags);
	//HAL_DISABLE_INTERRUPTS(flags);
	//OS_DisableDispatch();
	//MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
    U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

	if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
		timer->expires = timer->tmout_step;
	else
		timer->expires = timer->tmout_value;

	list_add_tail(&timer->entry, &sw_timer_vec_head);

	//restore_firqs(&flags);
	//HAL_RESTORE_INTERRUPTS(flags);
	//OS_EnableDispatch();
	//MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
     MsOS_RestoreAllInterrupts(u32OldIntr);

	//TIMER_TASK_LOCK = flags;
}

int mod_timer(struct timer_list *timer, U32 expires, U32 steps)
{
	//U32 flags;
	int ret = 0;

	//save_and_disable_firqs(&flags);
	//OS_DisableDispatch();
	//MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
    U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

	timer->tmout_value = expires;
	timer->tmout_step = steps;

	//ret = del_timer(timer);
	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

	//add_timer(timer);
	if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
		timer->expires = timer->tmout_step;
	else
		timer->expires = timer->tmout_value;
	list_add_tail(&timer->entry, &sw_timer_vec_head);

	//restore_firqs(&flags);
	//OS_EnableDispatch();
	//MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
     MsOS_RestoreAllInterrupts(u32OldIntr);

	return ret;
}
#endif	//#ifdef

