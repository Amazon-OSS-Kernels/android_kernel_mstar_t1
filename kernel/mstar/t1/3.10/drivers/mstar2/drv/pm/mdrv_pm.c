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
/// file    mdrv_mpool.c
/// @brief  Memory Pool Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
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
#include <linux/time.h>  //added
#include <linux/timer.h> //added
#include <linux/device.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <asm/types.h>
#include <asm/cacheflush.h>

#if defined(CONFIG_COMPAT)
#include <linux/compat.h>
#endif

#include "mdrv_mstypes.h"
#include "mdrv_pm.h"
#include "mhal_pm.h"
#include "mdrv_system.h"

//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Golbal variable
//-------------------------------------------------------------------------------------------------
unsigned long gPM_DramAddr = 0x10000;
unsigned long gPM_DramSize = DRAM_MMAP_SZ;
unsigned long gPM_DataAddr = 0;
unsigned long gPM_DataSize = 0;

//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local function
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Golbal function
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_SetSRAMOffsetForMCU(void)
{
    PM_Result result;
    result = MHal_PM_CopyBin2Sram(gPM_DramAddr);
    if (result != E_PM_OK)
    {
        return result;
    }

    MHal_PM_SetDram2Register(gPM_DataAddr);

    result = MHal_PM_SetSRAMOffsetForMCU();
    return result;
}

void MDrv_PM_RunTimePM_Disable_PassWord(void)
{
	MHal_PM_RunTimePM_Disable_PassWord();
}

static char *temp_buf;
static phys_addr_t *remap_addr;
PM_Result MDrv_PM_CopyBin2PhysicalDram(void)
{
    memset(remap_addr, 0, gPM_DramSize);
    memcpy(remap_addr, temp_buf, gPM_DramSize);
    return E_PM_OK;
}

PM_Result MDrv_PM_CopyBin2TempBuffer(void)
{

    struct file *fp = NULL;
    mm_segment_t fs;
    loff_t pos;
    pr_info("start MDrv_PM_CopyBin2TempBuffer \n");
	if (temp_buf == NULL) {
		temp_buf = (char *)vmalloc(gPM_DramSize);
		if (temp_buf == NULL) {
			pr_err("%s, vmalloc failed\n", __FUNCTION__);
			BUG();
		}
	}

    memset(temp_buf, 0, gPM_DramSize);

    fp = filp_open("/tvconfig/config/PM.bin", O_RDONLY, 0);
    if (IS_ERR(fp)){
	pr_err("%s, filp_open failed\n", __FUNCTION__);
	BUG();
    }
    fs = get_fs();
    set_fs(KERNEL_DS);
    pos = 0;
    vfs_read(fp, temp_buf, gPM_DramSize, &pos);

    filp_close(fp,NULL);
    set_fs(fs);
    if (remap_addr == NULL) {
	remap_addr = (phys_addr_t *)ioremap(gPM_DramAddr + ARM_MIU0_BUS_BASE, gPM_DramSize);
	if (remap_addr == NULL) {
		pr_err("%s, ioremap failed\n", __FUNCTION__);
		BUG();
	}
    }

    return E_PM_OK;
}

//-------------------------------------------------------------------------------------------------
void MDrv_SetDram(u32 u32Addr, u32 u32Size)
{
    gPM_DramAddr = u32Addr;
    gPM_DramSize = u32Size;
}
//-------------------------------------------------------------------------------------------------
void MDrv_SetData(u32 u32Addr, u32 u32Size)
{
    gPM_DataAddr = u32Addr;
    gPM_DataSize = u32Size;
}

// Luke add new function for Maxim 
PM_Result MDrv_PM_GetCfg(PM_Cfg_t* pstPMCfg)
{
    phys_addr_t *remap_addr = NULL;
    remap_addr = (phys_addr_t *)ioremap(gPM_DataAddr + ARM_MIU0_BUS_BASE, gPM_DataSize);
    if (remap_addr == NULL)
    {
        printk("%s, ioremap failed\n", __FUNCTION__);
        return E_PM_FAIL;
    }

    memcpy((void*)&(pstPMCfg->stPMWakeCfg), remap_addr, sizeof(PM_WakeCfg_t));
    memcpy((void*)&(pstPMCfg->stPMPowerDownCfg), remap_addr + PM_POWERDOWN_CFG_OFFSET, sizeof(PM_PowerDownCfg_t));

    iounmap(remap_addr);
    return E_PM_OK;
}

PM_Result MDrv_PM_SetCfg(PM_Cfg_t* pstPMCfg)
{
    phys_addr_t *remap_addr = NULL;
    remap_addr = (phys_addr_t *)ioremap(gPM_DataAddr + ARM_MIU0_BUS_BASE, gPM_DataSize);
    if (remap_addr == NULL)
    {
        printk("%s, ioremap failed\n", __FUNCTION__);
        return E_PM_FAIL;
    }

    memcpy(remap_addr, (void*)&(pstPMCfg->stPMWakeCfg), sizeof(PM_WakeCfg_t));
    memcpy(remap_addr + PM_POWERDOWN_CFG_OFFSET, (void*)&(pstPMCfg->stPMPowerDownCfg), sizeof(PM_PowerDownCfg_t));

    iounmap(remap_addr);
    return E_PM_OK;
}


