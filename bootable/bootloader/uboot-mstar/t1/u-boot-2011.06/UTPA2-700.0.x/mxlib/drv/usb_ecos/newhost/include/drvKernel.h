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

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <MsCommon.h>

#include "drvPorts.h"
#include "drvUSBHost.h"
//#include <cyg/hal/plf_intr.h> // NUSED
//#include <cyg/hal/drv_api.h> // NUSED
#include <cyg/hal/hal_diag.h>

/* Debug print definition */
//#define DBG_MSG
//#define DBG_WARN
//#define DBG_FUNC
#define DBG_ERR

#undef  ms_debug_msg
#undef  ms_debug_warn
#undef  ms_debug_err
#undef  ms_debug_func

#ifdef DBG_MSG
#define ms_debug_msg(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_msg(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_WARN
#define ms_debug_warn(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_warn(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_ERR
#define ms_debug_err(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_err(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_FUNC
#define ms_debug_func(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_func(fmt, arg...) do {} while (0)
#endif

// Funtion for Protecting shared data (solve synchronisation by the easiest way)
//#define     MSOS_WAIT_FOREVER   (20000)

//#define INT_DISABLED  0 // NUSED
//#define INT_ENABLED   1 // NUSED

//extern int CPU_Int_State; // NUSED

/* to support class driver */
#define MS_IOBUF_SIZE   64
#define IDLE_STATE          1

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

#if 0 // NUSED
//Disable IRQ and FIQ and store cpsr
static __inline__ void save_and_disable_firqs(MS_U32 *flag)
{
    //HAL_DISABLE_INTERRUPTS(flag);
}

//Restore stored cpsr
static __inline__ void restore_firqs(MS_U32 *flag)
{
    //HAL_RESTORE_INTERRUPTS(*flag);
}
#endif

static __inline__ void sti(void)
{
    unlock_TmrTaskDispatch();
}

static __inline__ void cli(void)
{
    lock_TmrTaskDispatch();
}

extern void *ncmem_alloc_page(dma_addr_t *dma_addr);
extern void ncmem_free_page(MS_U32 addr);
extern void destroy_NC_mem(void);

extern void MsOS_FlushMemory(void);
extern void MsOS_ReadMemory(void);

extern void ms_USBCriticalSectionIn(MS_U8 Port);
extern void ms_USBCriticalSectionOut(MS_U8 Port);
#define USBCriticalSectionIn(p) ms_USBCriticalSectionIn(p)
#define USBCriticalSectionOut(p) ms_USBCriticalSectionOut(p)

#endif
