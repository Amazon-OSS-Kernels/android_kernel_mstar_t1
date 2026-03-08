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

#include <MsCommon.h>
#include <cyg/hal/hal_if.h>
#include "include/drvKernel.h"
#include "include/drvTimer.h"
#include "include/drvPorts.h"
#include "include/drvCPE_AMBA.h"

extern int usb_core_init(void);
extern struct bus_type usb_bus_type;

//int CPU_Int_State = INT_DISABLED;

extern void init_cache_memory(void);

S32      g_USBWaitFlg;
S32      g_USBWaitP2Flg;

void init_OS_Resource(void)
{

	g_USBWaitFlg = MsOS_CreateEventGroup("USB_Event");

    //   g_SCSISem = MsOS_CreateSemaphore(1, E_MSOS_FIFO, "USB_Sem");
//      _s32MutexUSB = MsOS_CreateMutex(E_MSOS_FIFO, "USB Mutex");


}

void free_OS_Resource(void)
{
	BOOL err;

	err = MsOS_DeleteEventGroup(g_USBWaitFlg);
	USB_ASSERT((err == TRUE ),"Delete Bulk Flag Fail...\n");

//	err = MsOS_DeleteSemaphore(g_SCSISem);
//        err=MsOS_DeleteMutex(_s32MutexUSB);
//	USB_ASSERT((err == TRUE ),"Delete USB mutex Fail...\n");
}

void init_OS_Port2_Resource(void)
{

	g_USBWaitP2Flg = MsOS_CreateEventGroup("USB_Event");

    //   g_SCSISem = MsOS_CreateSemaphore(1, E_MSOS_FIFO, "USB_Sem");
//      _s32MutexUSB = MsOS_CreateMutex(E_MSOS_FIFO, "USB Mutex");


}

void free_OS_Port2_Resource(void)
{
	BOOL err;

	err = MsOS_DeleteEventGroup(g_USBWaitP2Flg);
	USB_ASSERT((err == TRUE ),"Delete Port2 Bulk Flag Fail...\n");

//	err = MsOS_DeleteSemaphore(g_SCSISem);
//        err=MsOS_DeleteMutex(_s32MutexUSB);
//	USB_ASSERT((err == TRUE ),"Delete USB mutex Fail...\n");
}

void exit_sys(void)
{
	free_OS_Resource();
        free_OS_Port2_Resource();
}

void busyloop_delay(U32 count)
{
   U32 i,j;

   for (i=0; i<255; i++)
      for (j=0; j<count; j++) {
    }
}

#if 0
//The following code has serious bug.  Jonas_20110519
int find_next_zero_bit_le (void *p, int size, int offset)
{
  int i,j;
  int iOffset,jOffset;
  char *tmp = (char *)p;

  iOffset = offset%8;
  jOffset = offset/8;

  // Skip offset bits
  for(i=iOffset;i<8;i++)
    tmp[i] >>= 1;

  for(j=jOffset;j<size/8;j++)
  {
    for(i=iOffset;i<8;i++)
    {
      if ( ( tmp[j] & 0x01 ) == 0 )
      {
        return j*8+i;
      }
      tmp[j] >>= 1;
    }
  }
  // not found return (maximum size) back
  return size;
}

#else

#define BITOP_WORD(nr)		((nr) / BITS_PER_LONG)

U32 find_next_zero_bit(const U32 *addr, U32 size, U32 offset)
{
	const U32 *p = addr + BITOP_WORD(offset);
	U32 result = offset & ~(BITS_PER_LONG-1);
	U32 tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset %= BITS_PER_LONG;
	if (offset) {
		tmp = *(p++);
		tmp |= ~0UL >> (BITS_PER_LONG - offset);
		if (size < BITS_PER_LONG)
			goto found_first;
		if (~tmp)
			goto found_middle;
		size -= BITS_PER_LONG;
		result += BITS_PER_LONG;
	}
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	while (size & ~(BITS_PER_LONG-1)) {
		if (~(tmp = *(p++)))
			goto found_middle;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp |= ~0UL << size;
	if (tmp == ~0UL)	
		return result + size;	
found_middle:
	return result + ffz(tmp);
}


U32 find_next_zero_bit_le(U32 *addr, U32 size, U32 offset)
{
	return find_next_zero_bit(addr, size, offset);
}

#endif

void udelay(U32 us)
{
   HAL_DELAY_US(us);
}

void init_sys(void)
{
    #if 1
	init_cache_memory();
	INIT_LIST_HEAD(&usb_bus_type.devices_list);
	INIT_LIST_HEAD(&usb_bus_type.drivers_list);
	INIT_LIST_HEAD(&CPE_AMBA_bus_type.devices_list);
	INIT_LIST_HEAD(&CPE_AMBA_bus_type.drivers_list);
    #endif

	sti();

    #if  1
	// Init timer and init software timer link list
	init_timers();
    #endif

    #if 1
	// Init USB core (including hub)
	usb_core_init();
    #endif
}

void wait_ms(int x)
{
    if (x==0) return;
    MsOS_DelayTask(x);
}

void init_OS_Resource_EX(S32 *USBWaitFlg)
{
    *USBWaitFlg = MsOS_CreateEventGroup("USB_Event");
}

