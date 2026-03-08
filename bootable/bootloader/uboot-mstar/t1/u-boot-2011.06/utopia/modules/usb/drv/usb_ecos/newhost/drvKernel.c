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

//#include <MsCommon.h> // NUSED
//#include <cyg/hal/hal_if.h> // NUSED
#include "include/drvKernel.h"
//#include "include/drvTimer.h" // NUSED
//#include "include/drvPorts.h" // NUSED
//#include "include/drvCPE_AMBA.h" // NUSED

extern int ms_usb_core_init(void);
extern int ms_usb_core_uninit(void);
extern struct ms_bus_type usb_bus_type;
extern void init_cache_memory(void);

// The previous code has serious bug.  Removed. Jonas_20110519
// The following code has been rewitten by  Jonas_20110519
#define BITOP_WORD(nr)  ((nr) / BITS_PER_LONG)
/**
     * @brief               find out the position of the next zero
     *
     * @param           U32 *addr
     * @param           U32 size
     * @param           U32 offset
     *
     * @return          zero bit position
     */
U32 find_next_zero_bit(const U32 *addr, U32 size, U32 offset)
{
    const U32 *p = addr + BITOP_WORD(offset);
    U32 ms_result = offset & ~(BITS_PER_LONG-1);
    U32 ms_tmp;

    if (offset >= size)
        return size;
    size -= ms_result;
    offset %= BITS_PER_LONG;
    if (offset) 
    {
        ms_tmp = *(p++);
        ms_tmp |= ~0UL >> (BITS_PER_LONG - offset);
        if (size < BITS_PER_LONG)
            goto ms_found_first;
        if (~ms_tmp)
            goto ms_found_middle;
        size -= BITS_PER_LONG;
        ms_result += BITS_PER_LONG;
    }
    while (size & ~(BITS_PER_LONG-1)) 
    {
        if (~(ms_tmp = *(p++)))
            goto ms_found_middle;
        ms_result += BITS_PER_LONG;
        size -= BITS_PER_LONG;
    }
    if (!size)
        return ms_result;
    ms_tmp = *p;

ms_found_first:
    ms_tmp |= ~0UL << size;
    if (ms_tmp == ~0UL)	
        return ms_result + size;	
ms_found_middle:
    return ms_result + ms_find_1st_zero(ms_tmp);
}

/**
     * @brief               find out the position of the next zero
     *
     * @param           U32 *addr
     * @param           U32 size
     * @param           U32 offset
     *
     * @return          zero bit position
     */
U32 find_next_zero_bit_le(U32 *addr, U32 size, U32 offset)
{
    return find_next_zero_bit(addr, size, offset);
}

/**
     * @brief               wait for N us
     *
     * @param           U32 us
     *
     * @return          none
     */
inline void udelay(U32 us)
{
   HAL_DELAY_US(us);
}

/**
     * @brief               initial USB system
     *
     * @param           none
     *
     * @return          none
     */
void ms_init_sys(void)
{
    init_cache_memory();
    ms_list_init(&usb_bus_type.devices_list);
    ms_list_init(&usb_bus_type.drivers_list);

    sti();

    // Init timer and init software timer link list
    ms_init_timers();

    // Init USB core (including hub)
    ms_usb_core_init();
}

void ms_exit_sys(void)
{
    ms_usb_core_uninit();
    ms_remove_timers();
    
    cli();
    destroy_NC_mem();
}

/**
     * @brief               wait for N ms
     *
     * @param           int x
     *
     * @return          none
     */
void wait_ms(int x)
{
    if (x==0) 
        return;
    MsOS_DelayTask(x);
}

/**
     * @brief               create USB event flag
     *
     * @param           S32 *USBWaitFlg
     *
     * @return          none
     */
void ms_init_OS_Resource_EX(S32 *USBWaitFlg)
{
    *USBWaitFlg = MsOS_CreateEventGroup("USB_Event");
}

void ms_release_OS_Resource_EX(S32 *USBWaitFlg)
{
    MsOS_DeleteEventGroup(*USBWaitFlg);    
}

