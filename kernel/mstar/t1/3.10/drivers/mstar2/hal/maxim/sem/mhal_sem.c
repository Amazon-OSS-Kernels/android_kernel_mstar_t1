/**
* Copyright (c) 2006 – 2017 MStar Semiconductor, Inc.
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

//#include "MsCommon.h"
#include <linux/autoconf.h>
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
#include <asm/io.h>

#include "mhal_reg_sem.h"
#include "mhal_sem.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define REG16(addr)     *((volatile u16*)((CONFIG_REGISTER_BASE_ADDRESS + ((addr)<< 2))))

#define SEM_ID_GP0  \
    E_SEM_DUMMY, \
    E_SEM_GE0, \
    E_SEM_GE1, \
    E_SEM_BDMA,\
    E_SEM_PM,\
    E_SEM_TEE, \
    E_SEM_MBX_RECV,\
    E_SEM_MBX_SEND, \
    E_SEM_USER0, \
    E_SEM_USER1, \
    E_SEM_USER2, \
    E_SEM_USER3, \
    E_SEM_CA, \
    E_SEM_IIC, \
    E_SEM_DUMMY, \
    E_SEM_DUMMY

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
const eSemId SemIdTbl[SEM_MAX_GP_CLIENT] =
{
    SEM_ID_GP0
};

s16 HAL_SEM_GetSemId(eSemId eSemId)
{
    u8 idx;

    for (idx = 0; idx < SEM_MAX_GP_CLIENT; idx++)
        if (eSemId == SemIdTbl[idx])
            return idx;
    return (-1);
}

u32 HAL_SEM_Get_Num(void)
{
    return REG_SEM_MAX_NUM;
}

bool HAL_SEM_Get_Resource(u8 u8SemID, u16 u16ResId)
{
    if(u8SemID> REG_SEM_MAX_NUM)
        return false;
    REG16(REG_SEM_ID0+ u8SemID)= u16ResId;
    return (u16ResId == REG16(REG_SEM_ID0+ u8SemID))? true: false;
}

bool HAL_SEM_Free_Resource(u8 u8SemID, u16 u16ResId)
{
    if(u8SemID> REG_SEM_MAX_NUM)
        return false;

    if (u16ResId != REG16(REG_SEM_ID0+ u8SemID))
    {
        return false;
    }
    REG16(REG_SEM_ID0+ u8SemID)= 0x00;
    return true;
}


bool HAL_SEM_Reset_Resource(u8 u8SemID)
{
    if(u8SemID> REG_SEM_MAX_NUM)
        return false;
    REG16(REG_SEM_ID0+ u8SemID)= 0x00;
    return true;
}


bool HAL_SEM_Get_ResourceID(u8 u8SemID, u16* pu16ResId)
{
    if(u8SemID> REG_SEM_MAX_NUM)
        return false;
    *pu16ResId = REG16(REG_SEM_ID0+ u8SemID);
    return true;
}
