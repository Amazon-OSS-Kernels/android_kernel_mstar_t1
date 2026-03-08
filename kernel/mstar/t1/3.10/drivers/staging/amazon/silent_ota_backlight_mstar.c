/**
* Copyright (c) 2017 Amazon, Inc.
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

#include <linux/types.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/silent_ota_backlight.h>
#include <linux/sign_of_life.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

void MDrv_GPIO_Set_High(unsigned char u8IndexGPIO);
void MDrv_GPIO_Set_Low(unsigned char u8IndexGPIO);
unsigned char MDrv_GPIO_Get_Level(unsigned char u8IndexGPIO);
static DEFINE_MUTEX(backlight_lock);

#define BACKLIGHT_GPIO 9
extern ptrdiff_t mstar_pm_base;

#define REG_ADDR(addr)                           (*((volatile unsigned short int*)(mstar_pm_base + (addr << 1))))
#define PM_ADDR_OFFSET				 0x060a

/* read 2 byte */
#define REG_RR(_reg_)                            ({REG_ADDR(_reg_); })

/* write 2 byte */
#define REG_W2B(_reg_, _val_)    \
		do { REG_ADDR(_reg_) = (_val_); } while (0)


/*=============================================================================
 * Global Variables
 *============================================================================= */
static unsigned int backlight_status; /* 1: on, 0: off */
static unsigned int boot_complete;

static void set_backlight(unsigned char on)
{
	if (on) {
		pr_info("Turn on backlight from kernel \n");
		MDrv_GPIO_Set_Low(BACKLIGHT_GPIO);
		REG_W2B(PM_ADDR_OFFSET, REG_RR(PM_ADDR_OFFSET) | PM_SPARE_SCREEN_STATE);
		backlight_status = 1;
	} else {
		pr_info("Turn off backlight from kernel \n");
		MDrv_GPIO_Set_High(BACKLIGHT_GPIO);
		REG_W2B(PM_ADDR_OFFSET, REG_RR(PM_ADDR_OFFSET) & ~PM_SPARE_SCREEN_STATE);
		backlight_status = 0;
	}
	return;
}

/*
This fucntion will toggle the backlight when user pressing IR power key before boot completed,
and it will turn on the backlight when user press "Netflix", "Amazon Video" and "Amazon Music"
keys.

*/
unsigned int toggle_backlight(unsigned int value)
{
	if (unlikely(!boot_complete)) {
		mutex_lock(&backlight_lock);
		if (value == OTA_TOGGLE_BACKLIGHT) {
			set_backlight(!backlight_status);
		} else if (value == OTA_TURN_ON_BACKLIGHT) {
			if (!backlight_status)
				set_backlight(1);
		} else if (value == OTA_TURN_OFF_BACKLIGHT) {
			if (backlight_status)
				set_backlight(0);
		} else {
			pr_info("unsupported argument for toggle_backlight\n");
		}
		mutex_unlock(&backlight_lock);
	}
	return 0;

}

static int backlight_status_proc_show(struct seq_file *m, void *v)
{
	backlight_status = !MDrv_GPIO_Get_Level(BACKLIGHT_GPIO);
	seq_printf(m, "%d\n", backlight_status);
	return 0;
}
/* This fucntion will be called when user pressing the power key on TV keypad */
ssize_t backlight_status_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	return toggle_backlight(OTA_TOGGLE_BACKLIGHT);
}


static int backlight_status_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, backlight_status_proc_show, NULL);
}

static const struct file_operations backlight_status_proc_fops = {
	.open		= backlight_status_proc_open,
	.read		= seq_read,
	.write		= backlight_status_proc_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

/* This fucntion will be called when checking if system is in silent OTA mode */

static int silent_ota_status_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", mstar_get_silent_ota_flag());
	return 0;
}

static int silent_ota_status_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, silent_ota_status_proc_show, NULL);
}

static const struct file_operations silent_ota_status_proc_fops = {
	.open           = silent_ota_status_proc_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};

static int boot_complete_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", boot_complete);
	return 0;
}

static int boot_complete_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, boot_complete_proc_show, NULL);
}

/* This functioni will be called init.maxim.rc when boot complete */
ssize_t boot_complete_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char buffer[2];

	if (count != 1)
		return count;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[1] = '\0';
	pr_info("write boot_completed  %s %d \n", buffer, buffer[0]);
	sscanf(buffer, "%d", &boot_complete);
	if (boot_complete == 1)
		mstar_clear_silent_ota_flag();

	return count;
}

static const struct file_operations boot_complete_fops = {
	.open = boot_complete_proc_open,
	.read = seq_read,
	.write = boot_complete_proc_write,
	.llseek = seq_lseek,
	.release = single_release,
	.owner = THIS_MODULE,
};

static int __init ota_backlight_init(void)
{
	int ret;
/* Low means backlight is on, High means backlight is off */
	backlight_status = !MDrv_GPIO_Get_Level(BACKLIGHT_GPIO);
	pr_info("Inital backlight_status is %d \n", backlight_status);
	proc_create("boot_completed", 0664, NULL, &boot_complete_fops);
	proc_create("backlight_status", 0664, NULL, &backlight_status_proc_fops);
	proc_create("silent_ota_status", 0444, NULL, &silent_ota_status_proc_fops);
	return ret;
}

static void __exit ota_backlight_exit(void)
{
	return;
}

early_initcall(ota_backlight_init);
module_exit(ota_backlight_exit);

MODULE_AUTHOR("Amazon.");
MODULE_DESCRIPTION("OTA backlight driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ota-backlight");
