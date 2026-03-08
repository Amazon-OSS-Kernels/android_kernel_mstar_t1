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

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/uaccess.h>

#ifndef __MDRV_DVFS_H__
#include "mdrv_dvfs.h"
#endif

DEFINE_MUTEX(MDrvDvfsMutex);
#define MAX_WRITE_BUFFER	64
static atomic_t dvfs_voltage_proc_is_open = ATOMIC_INIT(0);
static atomic_t dvfs_frequency_proc_is_open = ATOMIC_INIT(0);
static atomic_t dvfs_boost_duration_proc_is_open = ATOMIC_INIT(0);
static unsigned int bootarg_auto_measurement=0;
static struct proc_dir_entry *proc_mstar_dvfs_dir;
extern unsigned int query_frequency(unsigned int cpu_id);

//=================================================================================================
U32 MDrvDvfsProc(U32 dwInputCpuClock, U8 dwCpu)
{
    U32 dwOutputCpuClock = 0;

    if (bootarg_auto_measurement)
        return dwInputCpuClock;

    mutex_lock(&MDrvDvfsMutex);
    dwOutputCpuClock = MHalDvfsProc(dwInputCpuClock / 1000, dwCpu);
    mutex_unlock(&MDrvDvfsMutex);

    return (dwOutputCpuClock * 1000);
}

//=================================================================================================
void MDrvDvfsInit(void)
{
    MDrvHalDvfsInit();
}

//=================================================================================================
void MDrvDvfsCpuDisplay(U8 dwCpu)
{
    int dwCluster = getCpuCluster(dwCpu);
    MHalDvfsCpuDisplay(dwCluster);
}

//=================================================================================================
void MDrvDvfsCpuPowerAdjustment(U32 dwCpuPowerVoltage, U8 dwCpu)
{
#ifdef CONFIG_MSTAR_CPU_calibrating
    MHalDvfsCpuPowerAdjustment(dwCpuPowerVoltage, dwCpu);
#elif defined(CONFIG_MSTAR_CPU_CLUSTER_CALIBRATING)
    MHalDvfsCpuPowerAdjustment(dwCpuPowerVoltage, dwCpu);
#endif
}

//=================================================================================================
void MDrvDvfsCorePowerAdjustment(U32 dwCorePowerVoltage, U8 dwCpu)
{
#ifdef CONFIG_MSTAR_CPU_calibrating
    MHalDvfsCorePowerAdjustment(dwCorePowerVoltage, dwCpu);
#elif defined(CONFIG_MSTAR_CPU_CLUSTER_CALIBRATING)
    MHalDvfsCorePowerAdjustment(dwCorePowerVoltage, dwCpu);
#endif
}

//=================================================================================================
U32 MDrvDvfsQueryCpuClock(U32 dwCpuClockType, U8 dwCpu)
{
    return (MHalDvfsQueryCpuClock(dwCpuClockType, dwCpu) * 1000);
}

//=================================================================================================
U32 MDrvDvfsQueryCpuClockByTemperature(U8 dwCpu)
{
    U32 dwOutputCpuClock = 0;

    //mutex_lock(&MDrvDvfsMutex);
    dwOutputCpuClock = MHalDvfsQueryCpuClockByTemperature(dwCpu);
    //mutex_unlock(&MDrvDvfsMutex);

    return (dwOutputCpuClock * 1000);
}

//=================================================================================================
U32 MDrvDvfsGetCpuTemperature(U8 dwCpu)
{
    return MHalDvfsGetCpuTemperature(dwCpu);
}
//=================================================================================================
U32 MDrvDvfsGetVoltage(U8 dwCpu)
{
    return MHalDvfsGetVoltage(dwCpu);
}
//=================================================================================================
U32 MDrvDvfsGetSidd(void)
{
    return MHalDvfsGetSidd();
}

//=================================================================================================
U32 MDrvDvfsGetOsc(U8 dwCpu)
{
    return MHalDvfsGetOsc(dwCpu);
}

//=================================================================================================
static int MDrvDvfsProbe(struct platform_device *pdev)
{
    int wReturnValue = 0;

    if(!(pdev->name) || \
       strcmp(pdev->name, "Mstar_DVFS") || \
       pdev->id != 0)
    {
        wReturnValue = -ENXIO;
    }

    MDrvDvfsInit();

    return wReturnValue;
}

//=================================================================================================
static int MDrvDvfsRemove(struct platform_device *pdev)
{
    return 0;
}

//=================================================================================================
static int MDrvDvfsSuspend(struct platform_device *dev, pm_message_t state)
{
    return 0;
}

//=================================================================================================
static int MDrvDvfsResume(struct platform_device *dev)
{
    return 0;
}

//=================================================================================================
static struct platform_driver MstarDvfsDriver =
{
    .probe      = MDrvDvfsProbe,
    .remove     = MDrvDvfsRemove,
    .suspend    = MDrvDvfsSuspend,
    .resume     = MDrvDvfsResume,

    .driver =
    {
        .name   = "Mstar_DVFS",
        .owner  = THIS_MODULE,
    }
};
//=================================================================================================
ssize_t dvfs_voltage_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char buffer[MAX_WRITE_BUFFER];
	unsigned int set_cpu = 0;
	unsigned int set_voltage = 0;
	unsigned int garbage = 0;
	int set;

	if (!count)
		return count;

	if (count >= MAX_WRITE_BUFFER)
		count = MAX_WRITE_BUFFER - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';

    if(sscanf(buffer,"%d %d %d", &set_cpu, &set_voltage, &garbage) == 3)
    {
       return -EINVAL;
    }
    else if (sscanf(buffer,"%d %d", &set_cpu, &set_voltage) == 2)
    {
        if (set_cpu > CONFIG_NR_CPUS)
            return -EINVAL;

        printk("Function: %d, Line: %d, Set CPU:%d voltage:%d\n", __func__, __LINE__, set_cpu, set_voltage);
        MHalDvfsCpuPowerAdjustment(set_voltage, set_cpu);
	    return count;

    }
    return -EINVAL;

}

static int dvfs_voltage_seq_show(struct seq_file *s, void *v)
{
	int i;
	unsigned int volt = 0;

	for_each_online_cpu(i)
	{
		volt = MDrvDvfsGetVoltage(i);
		//printk("CPU:%d volt:%d\n", i, volt);
		seq_printf(s, "CPU: %d volt: %d\n", i, volt);
	}
	return 0;
}

static int dvfs_voltage_proc_open(struct inode *inode, struct file *file)
{
	if(atomic_read(&dvfs_voltage_proc_is_open))
		return -EACCES;

	atomic_set(&dvfs_voltage_proc_is_open, 1);

	return single_open(file, &dvfs_voltage_seq_show, NULL);
}

static int dvfs_voltage_proc_release(struct inode *inode, struct file * file)
{
	WARN_ON(!atomic_read(&dvfs_voltage_proc_is_open));
	atomic_set(&dvfs_voltage_proc_is_open, 0);
	return single_release(inode, file);
}

static const struct file_operations proc_dvfs_voltage_fileops = {
	.owner		= THIS_MODULE,
	.open       = dvfs_voltage_proc_open,
	.write      = dvfs_voltage_proc_write,
	.read		= seq_read,
	.llseek     = seq_lseek,
	.release    = dvfs_voltage_proc_release,
};

//=================================================================================================
ssize_t dvfs_frequency_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char buffer[MAX_WRITE_BUFFER];
	unsigned int set_cpu = 0;
	unsigned int set_frequency = 0;
	unsigned int garbage = 0;
	int set;

	if (!count)
		return count;

	if (count >= MAX_WRITE_BUFFER)
		count = MAX_WRITE_BUFFER - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';

    if(sscanf(buffer,"%d %d %d", &set_cpu, &set_frequency, &garbage) == 3)
    {
       return -EINVAL;
    }
    else if (sscanf(buffer,"%d %d", &set_cpu, &set_frequency) == 2)
    {
        if (set_cpu > CONFIG_NR_CPUS)
            return -EINVAL;

        printk("Function: %d, Line: %d, Set CPU:%d freq:%d\n", __func__, __LINE__, set_cpu, set_frequency);
        MHalDvfsCpuClockAdjustment(set_frequency, set_cpu);
	    return count;

    }
    return -EINVAL;
}

static int dvfs_frequency_seq_show(struct seq_file *s, void *v)
{
	int i;
	unsigned int freq = 0;

	for_each_online_cpu(i)
	{
		freq = query_frequency(i);
		//printk("CPU: %d freq: %u\n", i, freq);
		seq_printf(s, "CPU: %d freq: %u\n", i, freq);
	}
	return 0;
}

static int dvfs_frequency_proc_open(struct inode *inode, struct file *file)
{
	if(atomic_read(&dvfs_frequency_proc_is_open))
		return -EACCES;
	atomic_set(&dvfs_frequency_proc_is_open, 1);

	return single_open(file, &dvfs_frequency_seq_show, NULL);
}

static int dvfs_frequency_proc_release(struct inode *inode, struct file * file)
{
	WARN_ON(!atomic_read(&dvfs_frequency_proc_is_open));
	atomic_set(&dvfs_frequency_proc_is_open, 0);
	return single_release(inode, file);
}

static const struct file_operations proc_dvfs_frequency_fileops = {
	.owner		= THIS_MODULE,
	.open       = dvfs_frequency_proc_open,
	.write      = dvfs_frequency_proc_write,
	.read		= seq_read,
	.llseek     = seq_lseek,
	.release    = dvfs_frequency_proc_release,
};

//=================================================================================================
static int dvfs_temp_seq_show(struct seq_file *s, void *v)
{
	int i;
	unsigned int temperature = 0;

	for_each_online_cpu(i)
	{
		temperature = MDrvDvfsGetCpuTemperature(i);
		//printk("CPU:%d temp:%d\n", i, temperature);
		seq_printf(s, "CPU: %d temp: %d\n", i, temperature);
	}
	return 0;
}

static int dvfs_temp_proc_open(struct inode *inode, struct file *file)
{
	//return seq_open(file, &dvfs_temp_seq_ops);
	return single_open(file, &dvfs_temp_seq_show, NULL);
}

ssize_t dvfs_temp_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char buffer[MAX_WRITE_BUFFER];
	unsigned int set_cpu = 0;
	unsigned int set_temprature = 0;
	unsigned int garbage = 0;
	unsigned int high_low_bound = 0; /*0: low, 1: high, 2: max*/

	if (!count)
		return count;

	if (count >= MAX_WRITE_BUFFER)
		count = MAX_WRITE_BUFFER - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';

	if (sscanf(buffer, "%d %d %d %d", &set_cpu, &high_low_bound, &set_temprature, &garbage) == 4) {
		return -EINVAL;
	} else if (sscanf(buffer, "%d %d %d", &set_cpu, &high_low_bound, &set_temprature) == 3) {
		if (set_cpu > CONFIG_NR_CPUS)
			return -EINVAL;
		printk("Function: %s, Line: %d, Set CPU:%d, level:%d, temperature:%d\n", __func__, __LINE__, set_cpu, high_low_bound, set_temprature);
		if (high_low_bound == 0)
			MHalDvfs_set_low_bound_temperature(set_cpu, set_temprature);
		else if (high_low_bound == 1)
			MHalDvfs_set_high_bound_temperature(set_cpu, set_temprature);
		else if (high_low_bound == 2)
			MHalDvfs_set_max_bound_temperature(set_cpu, set_temprature);

		return count;
	}
	return -EINVAL;
}

static const struct file_operations proc_dvfs_temp_fileops = {
	.owner		= THIS_MODULE,
	.open       = dvfs_temp_proc_open,
	.read		= seq_read,
	.write		= dvfs_temp_proc_write,
	.llseek     = seq_lseek,
	.release    = single_release,
};

//=================================================================================================
static int dvfs_sidd_seq_show(struct seq_file *s, void *v)
{
	unsigned int sidd_val = 0;

	sidd_val = MDrvDvfsGetSidd();
    //printk("Sidd: %u\n", sidd_val);
	seq_printf(s, "Sidd: %u\n", sidd_val);

	return 0;
}

static int dvfs_sidd_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, &dvfs_sidd_seq_show, NULL);
}

static const struct file_operations proc_dvfs_sidd_fileops = {
	.owner		= THIS_MODULE,
	.open       = dvfs_sidd_proc_open,
	.read		= seq_read,
	.llseek     = seq_lseek,
	.release    = single_release,
};

//=================================================================================================
static int dvfs_osc_seq_show(struct seq_file *s, void *v)
{
	int i;
	unsigned int osc_val = 0;

	for_each_online_cpu(i)
	{
		osc_val = MDrvDvfsGetOsc(i);
		//printk("CPU:%d Osc:%d\n", i, osc_val);
		seq_printf(s, "CPU: %d osc: %d\n", i, osc_val);
	}

	return 0;
}

static int dvfs_osc_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, &dvfs_osc_seq_show, NULL);
}

static const struct file_operations proc_dvfs_osc_fileops = {
	.owner		= THIS_MODULE,
	.open       = dvfs_osc_proc_open,
	.read		= seq_read,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static int dvfs_boost_duration_proc_open(struct inode *inode, struct file *file)
{
	if (atomic_read(&dvfs_boost_duration_proc_is_open))
		return -EACCES;
	atomic_set(&dvfs_boost_duration_proc_is_open, 1);

	return single_open(file, &dvfs_boost_duration_seq_show, NULL);
}

static int dvfs_boost_duration_proc_release(struct inode *inode, struct file *file)
{
	WARN_ON(!atomic_read(&dvfs_boost_duration_proc_is_open));
	atomic_set(&dvfs_boost_duration_proc_is_open, 0);
	return single_release(inode, file);
}

static const struct file_operations proc_dvfs_boost_duration_fileops = {
	.owner		= THIS_MODULE,
	.open       = dvfs_boost_duration_proc_open,
	.write      = dvfs_boost_duration_proc_write,
	.read       = seq_read,
	.llseek     = seq_lseek,
	.release    = dvfs_boost_duration_proc_release,
};
/* ================================================================================================= */

static int __init MDrvDvfsModuleInit(void)
{
	struct proc_dir_entry *entry;

    proc_mstar_dvfs_dir = proc_mkdir("mstar_dvfs", NULL);

	if (!proc_mstar_dvfs_dir)
		return -ENOMEM;

    entry = proc_create("voltage", S_IRUSR | S_IWUSR, proc_mstar_dvfs_dir, &proc_dvfs_voltage_fileops);
	if (!entry)
		goto fail;

	entry = proc_create("frequency", S_IRUSR | S_IWUSR, proc_mstar_dvfs_dir, &proc_dvfs_frequency_fileops);
	if (!entry)
		goto fail;

	entry = proc_create("temperature", S_IRUSR | S_IRGRP | S_IROTH, proc_mstar_dvfs_dir, &proc_dvfs_temp_fileops);
	if (!entry)
		goto fail;

	entry = proc_create("sidd", S_IRUSR | S_IWUSR, proc_mstar_dvfs_dir, &proc_dvfs_sidd_fileops);
	if (!entry)
		goto fail;

	entry = proc_create("osc", S_IRUSR | S_IWUSR, proc_mstar_dvfs_dir, &proc_dvfs_osc_fileops);
	if (!entry)
		goto fail;

	entry = proc_create("boost_duration", S_IRUSR | S_IWUSR, proc_mstar_dvfs_dir,
		&proc_dvfs_boost_duration_fileops);
	if (!entry)
		goto fail;

	return (int) platform_driver_register(&MstarDvfsDriver);

fail:
	return -ENOMEM;
}

//=================================================================================================
static void __exit MDrvDvfsModuleExit(void)
{
	remove_proc_entry("voltage", proc_mstar_dvfs_dir);
	remove_proc_entry("temperature", proc_mstar_dvfs_dir);
	remove_proc_entry("sidd", proc_mstar_dvfs_dir);
	remove_proc_entry("osc", proc_mstar_dvfs_dir);
	remove_proc_entry("frequency", proc_mstar_dvfs_dir);
	remove_proc_entry("mstar_dvfs", NULL);

	platform_driver_unregister(&MstarDvfsDriver);
}


//=================================================================================================
static int __init DVFS_auto_measurement(char *str)
{
    if(strcmp(str, "enable") == 0)
    {
        printk("\033[32m auto_measurement enable \033[0m\n");
	    bootarg_auto_measurement= 1;
    }
    else
    {
        printk("\033[32m auto_measurement disable \033[0m\n");
	    bootarg_auto_measurement = 0;
    }
    MHalDvfsSetAutoMeasurement(bootarg_auto_measurement);
    return 0;
}
early_param("DVFS_MEASURE", DVFS_auto_measurement);

//=================================================================================================
module_init(MDrvDvfsModuleInit);
module_exit(MDrvDvfsModuleExit);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("Mstar DVFS Driver");
MODULE_LICENSE("GPL");

//=================================================================================================
