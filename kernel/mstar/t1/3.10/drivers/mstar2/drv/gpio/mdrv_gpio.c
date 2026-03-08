/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_gpio.c
/// @brief  GPIO Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
//#include "MsCommon.h"
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/spinlock.h>
#include <asm/io.h>

#include "mst_devid.h"

#include "mdrv_gpio.h"
#include "mhal_gpio_reg.h"
#include "mhal_gpio.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 0//reserved
static	struct semaphore	pm_gpio_mutex ;
#define	_MUTEX_INIT()   	init_MUTEX( &pm_gpio_mutex )
#define	_MUTEX_LOCK()   	down( &pm_gpio_mutex )
#define	_MUTEX_UNLOCK()		up( &pm_gpio_mutex )
#endif

spinlock_t gpio_lock;
/* Is it required to disable interrupt? */
#define LOCK_GPIO() \
	unsigned long flags;\
	spin_lock_irqsave(&gpio_lock, flags); 

#define UNLOCK_GPIO()\
	spin_unlock_irqrestore(&gpio_lock, flags);

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// GPIO chiptop initialization
/// @return None
/// @note   Called only once at system initialization
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Init(void)
{
	MHal_GPIO_Init();
	spin_lock_init(&gpio_lock);
}
EXPORT_SYMBOL(MDrv_GPIO_Init);


//-------------------------------------------------------------------------------------------------
/// select one pad to set
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_WriteRegBit(U32 u32Reg, U8 u8Enable, U8 u8BitMsk)
{
    MHal_GPIO_WriteRegBit(u32Reg,u8Enable,u8BitMsk);
}
EXPORT_SYMBOL(MDrv_GPIO_WriteRegBit);

//-------------------------------------------------------------------------------------------------
/// select one pad to set
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Pad_Set(U8 u8IndexGPIO)
{
    MHal_GPIO_Pad_Set(u8IndexGPIO);
}
EXPORT_SYMBOL(MDrv_GPIO_Pad_Set);

//-------------------------------------------------------------------------------------------------
/// enable output for selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Pad_Oen(U8 u8IndexGPIO)
{
	LOCK_GPIO();
	MHal_GPIO_Pad_Oen(u8IndexGPIO);
	UNLOCK_GPIO();
}
EXPORT_SYMBOL(MDrv_GPIO_Pad_Oen);

//-------------------------------------------------------------------------------------------------
/// enable input for selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Pad_Odn(U8 u8IndexGPIO)
{
	LOCK_GPIO();
	MHal_GPIO_Pad_Odn(u8IndexGPIO);
	UNLOCK_GPIO();
}
EXPORT_SYMBOL(MDrv_GPIO_Pad_Odn);

//-------------------------------------------------------------------------------------------------
/// read data from selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
U8 MDrv_GPIO_Pad_Read(U8 u8IndexGPIO)
{
	U8 ret;
	LOCK_GPIO();
	ret = MHal_GPIO_Pad_Level(u8IndexGPIO);
	UNLOCK_GPIO();
	return ret;
}
EXPORT_SYMBOL(MDrv_GPIO_Pad_Read);

//-------------------------------------------------------------------------------------------------
/// read pad direction for selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
U8 MDrv_GPIO_Pad_InOut(U8 u8IndexGPIO)
{
	U8 ret;
	LOCK_GPIO();
	ret = MHal_GPIO_Pad_InOut(u8IndexGPIO);
	UNLOCK_GPIO();
	return ret;
}
EXPORT_SYMBOL(MDrv_GPIO_Pad_InOut);

//-------------------------------------------------------------------------------------------------
/// output pull high for selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Pull_High(U8 u8IndexGPIO)
{
	LOCK_GPIO();
	MHal_GPIO_Pull_High(u8IndexGPIO);
	UNLOCK_GPIO();
}
EXPORT_SYMBOL(MDrv_GPIO_Pull_High);

//-------------------------------------------------------------------------------------------------
/// output pull low for selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Pull_Low(U8 u8IndexGPIO)
{
	LOCK_GPIO();
	MHal_GPIO_Pull_Low(u8IndexGPIO);
	UNLOCK_GPIO();
}
EXPORT_SYMBOL(MDrv_GPIO_Pull_Low);

//-------------------------------------------------------------------------------------------------
/// output set high for selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Set_High(U8 u8IndexGPIO)
{
	LOCK_GPIO();
	MHal_GPIO_Set_High(u8IndexGPIO);
	UNLOCK_GPIO();
}
EXPORT_SYMBOL(MDrv_GPIO_Set_High);

//-------------------------------------------------------------------------------------------------
/// output set low for selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Set_Low(U8 u8IndexGPIO)
{
	LOCK_GPIO();
	MHal_GPIO_Set_Low(u8IndexGPIO);
	UNLOCK_GPIO();
}
EXPORT_SYMBOL(MDrv_GPIO_Set_Low);

//-------------------------------------------------------------------------------------------------
/// output set low for selected one pad
/// @param  u8IndexGPIO              \b IN:  pad index
/// @return None
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Set_Input(U8 u8IndexGPIO)
{
	LOCK_GPIO();
	MHal_GPIO_Set_Input(u8IndexGPIO);
	UNLOCK_GPIO();
}
EXPORT_SYMBOL(MDrv_GPIO_Set_Input);

