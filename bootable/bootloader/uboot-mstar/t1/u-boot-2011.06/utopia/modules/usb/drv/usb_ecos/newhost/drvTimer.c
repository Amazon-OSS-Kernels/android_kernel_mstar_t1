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
//#include <MsCommon.h> // NUSED

//#include "include/drvConfig.h" // NUSED
//#include "include/drvCompiler.h" // NUSED
//#include "include/drvErrno.h" // NUSED
//#include "include/drvPorts.h" // NUSED
//#include "include/drvTimer.h" // NUSED
#include "include/drvKernel.h"
//#include "include/drvList.h" // NUSED

U32 volatile jiffies;

void ms_SW_Timer_Tick(U32 u32StTimer, U32 u32TimerID);
static struct list_head sw_timer_vec_head;

S32 g_Timer_ID;
S32 _s32MutexUTimer;
void ms_init_timers(void)
{
    ms_list_init(&sw_timer_vec_head);
    // Register software timer dispatch program
    // Base 1 ms, interruptted every 1 ms

    BOOL err;

    g_Timer_ID = MsOS_CreateTimer((TimerCb) ms_SW_Timer_Tick,100, 100, FALSE, "USB_TIMER");
    //err = OS_DefineCyclicHandler(&g_Timer_ID, ms_SW_Timer_Tick, (void*) 0, 100);
    //USB_ASSERT((err == TRUE), "Define USB timer fail");

    //err = OS_ActivateCyclicHandler(g_Timer_ID);
    //
    err=MsOS_StartTimer(g_Timer_ID);
    USB_ASSERT((err == TRUE), "Activate USB timer fail");    
    diag_printf("Init Software Timer ok \n");
        _s32MutexUTimer = MsOS_CreateMutex(E_MSOS_FIFO, "UTimer_MUTEX", MSOS_PROCESS_SHARED);
}

void ms_remove_timers(void)
{
    MsOS_DeleteTimer(g_Timer_ID);
    diag_printf("Remove Software Timer ok \n");
    MsOS_DeleteMutex(_s32MutexUTimer);
}

/*
 * Timer ISR: It's responsible for dispatching tasks registered to software-timer
 * task queue. It need to be implemented as Bottom-Half mechanism of Linux
 * Timer and USB IRQ have the same priority and cannot be interrupted by each other
 */
void ms_SW_Timer_Tick(U32 u32StTimer, U32 u32TimerID)
{
    struct timer_list *timer;
    struct list_head  *tmp;

    jiffies+=100;

    if ( Timer_task_lock == 1 )
        goto sw_done;

    // Dispath task from task queue of software-timer here
    if (!ms_is_empty_list(&sw_timer_vec_head))
    {
        tmp = sw_timer_vec_head.next;
        while (tmp != &sw_timer_vec_head)
        {
            timer = entry_to_container (tmp, struct timer_list, entry);
            tmp = tmp->next;
            //list_entry (tmp, struct timer_list, entry, struct list_head, timer);
            // Check if time expired ?
            timer->expires -= 100;
            if ( timer->expires <= 0 )
            {
                void (*pFunc)(U32);
                U32 data;
                // If lock ==> don't dispatch task
                /*
                                        if ( Timer_task_lock == 1 )
                                        {
                                        timer->expires++;
                                        continue;
                                        }
                                        */
                pFunc = timer->function;
                data = timer->data;
                ms_del_timer(timer);
                // Call registered function
                if ( (U32)pFunc )
                {
                    lock_usb_core();
                    pFunc(data);
                    unlock_usb_core();
                }
                else
                    USB_ASSERT( 0, "ERROR: fun is NULL\n");
                
                //Current version only support dispatch one task each time
                //break;
            }
        }
    }
    
sw_done:
    return;
    // End of dispatching
}

/***
 * ms_del_timer - deactive a timer.
 * @timer: the timer to be deactivated
 *
 * ms_del_timer() deactivates a timer - this works on both active and inactive
 * timers.
 *
 * The function returns whether it has deactivated a pending timer or not.
 * (ie. ms_del_timer() of an inactive timer returns 0, del_timer() of an
 * active timer returns 1.)
 */
int ms_del_timer(struct timer_list *timer)
{
    U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

    if( timer->entry.next != NULL && timer->entry.prev != NULL )
    {
        USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
        USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
        USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
        USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
        ms_list_remove_and_init(&timer->entry);
    }

    MsOS_RestoreAllInterrupts(u32OldIntr);
    return 1;
}

/***
 * ms_del_timer_sync - deactive a timer.
 * @timer: the timer to be deactivated
 *
 * ms_del_timer_sync() deactivates a timer - this works on both active and inactive
 * timers.
 *
 * The function will protect synchronization problem before deactivating timer
 */
int ms_del_timer_sync(struct timer_list *timer)
{
    U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

    if( timer->entry.next != NULL && timer->entry.prev != NULL )
    {
        USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
        USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
        USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
        USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
        ms_list_remove_and_init(&timer->entry);
    }

    MsOS_RestoreAllInterrupts(u32OldIntr);
    return 1;
}

/***
 * ms_add_timer - start a timer
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
void ms_add_timer(struct timer_list *timer)
{
    U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

    if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
        timer->expires = timer->tmout_step;
    else
        timer->expires = timer->tmout_value;

    ms_insert_list_before(&timer->entry, &sw_timer_vec_head);

    MsOS_RestoreAllInterrupts(u32OldIntr);
}

int ms_update_timer(struct timer_list *timer, U32 expires, U32 steps)
{
    int ret = 0;
    U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

    timer->tmout_value = expires;
    timer->tmout_step = steps;

    if( timer->entry.next != NULL && timer->entry.prev != NULL )
    {
        USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
        USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
        USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
        USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
        ms_list_remove_and_init(&timer->entry);
    }

    if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
        timer->expires = timer->tmout_step;
    else
        timer->expires = timer->tmout_value;
    ms_insert_list_before(&timer->entry, &sw_timer_vec_head);

    MsOS_RestoreAllInterrupts(u32OldIntr);
    return ret;
}
