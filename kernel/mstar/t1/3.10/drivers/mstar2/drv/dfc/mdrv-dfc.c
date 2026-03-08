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
/// @file   Mdrvl_dfc.c
/// @brief  DFC Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/cpu.h>
#include <linux/suspend.h>
#include <linux/delay.h>

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------
#define DFC_INFO(fmt, args...) printk(KERN_INFO "[DFC INFO] %s:%d " fmt,__FUNCTION__,__LINE__, ##args)
#define DFC_WARN(fmt, args...) printk(KERN_WARNING "[DFC WARNING] %s:%d " fmt,__FUNCTION__,__LINE__, ##args)
#define DFC_ERR(fmt, args...) printk(KERN_ERR "[DFC ERROR] %s:%d " fmt,__FUNCTION__,__LINE__, ##args)

#if defined(CONFIG_ARM64)
extern ptrdiff_t mstar_pm_base;
#define RIU(addr)                          (*((volatile unsigned short *)(mstar_pm_base + (addr << 1))))
#else
#define RIU(addr)                          (*((volatile unsigned short *)(0xFD000000 + (addr << 1))))
#endif

#define Hand_shrink 0x1033A0
#define STRING_LEN 100

#if (defined CONFIG_MSTAR_CPU_calibrating) || (defined CONFIG_MSTAR_CPU_CLUSTER_CALIBRATING)
extern atomic_t disable_dvfs;
#endif

static int dfc_proc_show(struct seq_file *m, void *v) {
	seq_printf(m, "DFC proc!\n");
	return 0;
}

static int dfc_proc_open(struct inode *inode, struct  file *file) {
	return single_open(file, dfc_proc_show, NULL);
}
static ssize_t dfc_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	size_t len = STRING_LEN;
	char mbuf[STRING_LEN + 1];
	int call_count = 0;

	if (len > count)
		len = count;

	if (copy_from_user (mbuf, buf, len)){
		DFC_WARN("copy_from_user Error,please check");
		return -EFAULT;
	}

	DFC_INFO("Change frequency to %s\n",mbuf);

#if (defined CONFIG_MSTAR_CPU_calibrating) || (defined CONFIG_MSTAR_CPU_CLUSTER_CALIBRATING)
	/* Disable DVFS before suspend */
	Mdrv_CpuFreq_All_Lock((char *)__func__);
	atomic_set(&disable_dvfs, 1);
	Mdrv_CpuFreq_All_UnLock((char *)__func__);
#endif

	//Add Bring up PM51 code here or in APP ??
	//		??
	//		??

	extern void _chip_flush_miu_pipe(void);
	if(disable_nonboot_cpus()){
		enable_nonboot_cpus();
		DFC_WARN("FAILED to STOP CPU\n");
		return 0;
	}
	preempt_disable();
	arch_suspend_disable_irqs();
	_chip_flush_miu_pipe();
	BUG_ON(!irqs_disabled());
	while(RIU(Hand_shrink) != 0xfac0){
		if(call_count == 1){
			//Handshrink with PM51
			//	??
			//	??
		}
		udelay(500);
		call_count++;
	}
	arch_suspend_enable_irqs();
	BUG_ON(irqs_disabled());
	preempt_enable();
	enable_nonboot_cpus();
	RIU(Hand_shrink) = 0;

#if (defined CONFIG_MSTAR_CPU_calibrating) || (defined CONFIG_MSTAR_CPU_CLUSTER_CALIBRATING)
	/* Enable DVFS after resume, this is error case */
	Mdrv_CpuFreq_All_Lock((char *)__func__);
	atomic_set(&disable_dvfs, 0);
	Mdrv_CpuFreq_All_UnLock((char *)__func__);
#endif

	DFC_INFO("Return\n");

	return count;
}

static const struct file_operations dfc_proc_fops = {
	.owner = THIS_MODULE,
	.open = dfc_proc_open,
	.write = dfc_proc_write,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init dfc_proc_init(void) {
	proc_create("dfc", 0, NULL, &dfc_proc_fops);
	return 0;
}

static void __exit dfc_proc_exit(void) {
	remove_proc_entry("dfc", NULL);
}

MODULE_LICENSE("GPL");
module_init(dfc_proc_init);
module_exit(dfc_proc_exit);
