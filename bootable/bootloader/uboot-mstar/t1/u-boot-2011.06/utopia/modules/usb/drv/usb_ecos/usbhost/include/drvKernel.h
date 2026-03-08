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

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "MsCommon.h"

#include "drvPorts.h"
#include "drvUSB.h"
#include <cyg/hal/plf_intr.h>
#include <cyg/hal/drv_api.h>

#ifdef PR_DEBUG
#define pr_debug diag_printf
#else
#define pr_debug(fmt,...) \
  do { } while (0)
#endif

// Funtion for Protecting shared data (solve synchronisation by the easiest way)
//#define     MSOS_WAIT_FOREVER   (20000)

#define INT_DISABLED  0
#define INT_ENABLED   1

extern int CPU_Int_State;



#define USB_STOR_BLK_COMPLETE	1
#define USB_PTP_COMPLETE			2
#define USB_STOR_BLK_ASYNC    	4


static __inline__ void unlock_usb_core(void)
{
  USB_core_lock = 0;
}

static __inline__ void lock_usb_core(void)
{
  //Lokc ISR ==> don't process USB ISR when USB interrupt occurs
  USB_core_lock = 1;
}

static __inline__  void lock_TmrTaskDispatch(void)
{
  //Lock ==> don't dispatch registered software timer tasks
  Timer_task_lock = 1;
}

static __inline__  void unlock_TmrTaskDispatch(void)
{
  Timer_task_lock = 0;
}




#define TIMER_TASK_LOCK Timer_task_lock

// Note, need to be modified
static __inline__ void check_usb_lock(void)
{
	if ( USB_core_lock <= 0 )		// None use the USB_Core, we can lock it
		USB_core_lock = 1;
	else
	{
		// Locked, Someone is using usb core
		while( USB_core_lock > 0 )
			MsOS_DelayTask(100);

	    // Got the lock
       USB_core_lock = 1;
     }
}

//Disable IRQ and FIQ and store cpsr
static __inline__ void save_and_disable_firqs(U32 *flag)
{
	//HAL_DISABLE_INTERRUPTS(flag);
}

//Restore stored cpsr
static __inline__ void restore_firqs(U32 *flag)
{
	//HAL_RESTORE_INTERRUPTS(*flag);
}

//#define sti()
//#define cli()

static __inline__ void sti(void)
{
	//MsOS_EnableInterrupt(E_IRQ_USB);


	unlock_TmrTaskDispatch();
}

static __inline__ void cli(void)
{
	//MsOS_DisableInterrupt(E_IRQ_USB);

	lock_TmrTaskDispatch();
}

extern void *ncmem_alloc_page(dma_addr_t *dma_addr);
extern void ncmem_free_page(U32 addr);
extern void Destory_NC_mem(void);
#endif
