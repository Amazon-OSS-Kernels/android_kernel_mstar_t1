/**
 * Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

/*

  @file   mdrv_utopia2k_str.c
  @brief  Utopia2K STR interface
  @author MStar Semiconductor Inc.

*/

#include <linux/timer.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/async.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/input.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <power.h>
#include <linux/seq_file.h>
#include "mdrv_utopia2k_str_io.h"
#include <linux/debugfs.h>

#define UTOPIA2K_STR_SELF_TEST 0

#define UTOPIA2K_STR_NAME "Mstar-utopia2k-str"
#define POWERON_REASON_NAME "Mstar-poweron-reason"

#if WAKUP_ANDROID_FROM_KERNEL

static unsigned short pm_wakeup_source;
static unsigned short pm_wakeup_key;
static char power_on_src_name[32] = {};
static char wakeup_src_name[32] = {};
static unsigned int wakeup_key_code;
static unsigned char pm_report_wakeup;
static unsigned int rtc_str_wake;
static DEFINE_MUTEX(event_lock);

extern int mstar_get_wake_key_name(u32 ir_code, u32 *key_name, u32 *key_code, u32 *toggle);
static void get_wakeup_reason()
{

	pm_wakeup_source = WAKEUP_SOURCE;
	pm_wakeup_key = POWERON_KEY;
	strcpy(wakeup_src_name, "empty");
	strcpy(power_on_src_name, "unknonw");
	pm_report_wakeup = 0;
	wakeup_key_code = 0;
	char *tmpPtr = NULL;

	switch (pm_wakeup_source) {
	case WKUP_SRC_IR:
		if (mstar_get_wake_key_name(pm_wakeup_key, wakeup_src_name, &wakeup_key_code, NULL) < 0) {
			pr_err("UNKNOWN key 0x%x, err!", pm_wakeup_key);
			pr_err("system ignores unknown key and will not wake up \n");
			sprintf(power_on_src_name, "%s", "unknown_key");
			sprintf(wakeup_src_name, "%s", "unknown_key");
			pm_report_wakeup = 0;
			break;
		}

		tmpPtr = strstr(wakeup_src_name, "KEY_CUSTOM");
		if (tmpPtr) {
			sprintf(power_on_src_name, "%s_%c", "app", *(tmpPtr+11));
		}

		if (strstr(wakeup_src_name, "KEY_BUTTON1"))
			sprintf(power_on_src_name, "%s", "customized button 1");

		if (strstr(wakeup_src_name, "KEY_BUTTON2"))
			sprintf(power_on_src_name, "%s", "customized button 2");

		if (strstr(wakeup_src_name, "KEY_POWER"))
			sprintf(power_on_src_name, "%s", "power");



		pm_report_wakeup = 1;
		break;
	case WKUP_SRC_SAR:
		pm_report_wakeup = 1;
		switch (pm_wakeup_key) {
		case KEYPAD_POWER_CODE:
			sprintf(power_on_src_name, "%s", "power");
			sprintf(wakeup_src_name, "%s", "KEY_POWER");
			wakeup_key_code = KEY_POWER;
			break;
		default:
			pr_err("UNKNOWN key 0x%x, err!", pm_wakeup_key);
			pr_err("system ignores unknown key and will not wake up \n");
			sprintf(power_on_src_name, "%s", "unknown_key");
			sprintf(wakeup_src_name, "%s", "unknown_key");

			pm_report_wakeup = 0;
			break;
		}
		break;
	case WKUP_SRC_CEC:
		sprintf(power_on_src_name, "%s", "CEC");
		sprintf(wakeup_src_name, "%s", "CEC");
		pm_report_wakeup = 1;
		wakeup_key_code = KEY_POWER_CEC;
		break;
	case WKUP_SRC_WOL:
		sprintf(power_on_src_name, "%s", "WOL");
		sprintf(wakeup_src_name, "%s", "WOL");
		pm_report_wakeup = 1;
		break;
	case WKUP_SRC_GPIO:
		sprintf(power_on_src_name, "%s", "GPIO");
		sprintf(wakeup_src_name, "%s", "GPIO");
		pm_report_wakeup = 1;
		break;
	case WKUP_SRC_BT:
		sprintf(power_on_src_name, "%s", "BT");
		sprintf(wakeup_src_name, "%s", "BT");
		pm_report_wakeup = 1;
		break;
	case WKUP_SRC_WIFI:
		sprintf(power_on_src_name, "%s", "WIFI");
		sprintf(wakeup_src_name, "%s", "WIFI");
		pm_report_wakeup = 1;
		break;
	case WKUP_SRC_RTC:
	case WKUP_SRC_RTC2:
		sprintf(power_on_src_name, "%s", "rtc");
		sprintf(wakeup_src_name, "%s", "rtc");
		pm_report_wakeup = rtc_str_wake ? 1 : 0;

		break;
	default:
		sprintf(power_on_src_name, "%s", "unknown_source");
		sprintf(wakeup_src_name, "%s", "unknown_key");
		pm_report_wakeup = 0;

		break;
	}
	return;
}

#endif

#define utopia2k_str_dbg(fmt, ...)              \
do {                                    \
	if (utopia2k_str_debug_enabled) \
		pr_info(fmt, ##__VA_ARGS__);                \
} while (0)

static ASYNC_DOMAIN_EXCLUSIVE(condition_domain);
static spinlock_t lock;

struct utopia2k_str_module {
	struct list_head list;
	struct task_struct *p;
	struct timer_list timer;
	f_utopia_str fp_str;
	int selftest;
	int mode;
	void *data;
};

struct dts_post_condition {
	struct list_head list;
	char name[];
};

struct list_head utopia2k_str_head;

struct list_head dts_post_condition_head;

#if WAKUP_ANDROID_FROM_KERNEL
static struct input_dev *input;
static struct wakeup_source *ws;
#endif

#ifdef CONFIG_OF
static const char *const power_mode[] = {
	"",
	"suspend",
	"resume"
};
#endif

bool utopia2k_str_debug_enabled;
bool utopia2k_str_selftest;

static int __init utopia2k_str_debug_enable(char *str)
{
	utopia2k_str_debug_enabled = 1;
	return 1;
}
__setup("utopia2k_str_debug", utopia2k_str_debug_enable);

static void utopia2k_str_module_timeout(unsigned long data)
{
	struct utopia2k_str_module *um = (struct utopia2k_str_module *)data;
	show_stack(um->p, NULL);
}

static ktime_t utopia2k_str_time_debug_start(struct utopia2k_str_module *module)
{
	ktime_t calltime = ktime_set(0, 0);

	utopia2k_str_dbg("[module %i] calling [%pf].\n",
		task_pid_nr(current), module->fp_str);
	calltime = ktime_get();

	return calltime;
}

#if WAKUP_ANDROID_FROM_KERNEL
static void utopia2k_str_report_wakeup_event(void)
{
	if (wakeup_key_code == 0) {
		if (pm_wakeup_source == WKUP_SRC_CEC) {
			sprintf(power_on_src_name, "%s", "CEC");
			sprintf(wakeup_src_name, "%s", "CEC");
			pm_report_wakeup = 1;
			wakeup_key_code = KEY_POWER_CEC;
		} else {
			wakeup_key_code = KEY_POWER;
			sprintf(wakeup_src_name, "KEY_POWER");
		}
	}
	if (input != NULL) {
		mutex_lock(&event_lock);
		input_event(input, EV_KEY, wakeup_key_code, 1);
		input_sync(input);
		input_event(input, EV_KEY, wakeup_key_code, 0);
		input_sync(input);
		mutex_unlock(&event_lock);
	} else {
		pr_err("utopia2k_str input device is NULL\n");
		return;
	}
	pr_info("Sending %s key event to FWK\n", wakeup_src_name);
	wakeup_key_code = 0;

}

static int utopia2k_str_should_report_wakeup_event(void)
{

	pr_info("Wakeup source %s %s report wakeup event.\n",
		wakeup_src_name, pm_report_wakeup ? "will" : "won't");
	return pm_report_wakeup;
}
#endif

static void utopia2k_str_time_debug_report(ktime_t calltime,
	struct utopia2k_str_module *module)
{
	ktime_t delta, rettime;

	rettime = ktime_get();
	delta = ktime_sub(rettime, calltime);
	utopia2k_str_dbg("[module %i] call [%pf] return after [%lld] usecs.\n",
			task_pid_nr(current), module->fp_str,
			(unsigned long long)ktime_to_ns(delta) >> 10);
}

int mdrv_utopia2k_str_setup_function_ptr(void *p_module,
	f_utopia_str fp_str)
{
	struct utopia2k_str_module *um;
	um = kmalloc(sizeof(struct utopia2k_str_module), GFP_KERNEL);
	if (!um)
		return -ENOMEM;
	INIT_LIST_HEAD(&um->list);
	list_add(&um->list, &utopia2k_str_head);
	um->fp_str = fp_str;
	um->data = p_module;
	um->selftest = 0;
	um->p = NULL;
	return 0;
}
EXPORT_SYMBOL(mdrv_utopia2k_str_setup_function_ptr);

int mdrv_utopia2k_str_wait_condition(const char *name,
	MS_U32 mode, MS_U32 stage)
{
	int ret = 0;
#ifdef CONFIG_OF
	char node_path[128] = {0};
	struct device_node *stage_node;
	struct property *prop;

	int i = 0;
	size_t l = 0, total = 0;
	const char *p;

	struct dts_post_condition *pc;
	int cnt = 0;

	int prop_cnt;

	if (utopia2k_str_selftest) {
		pr_notice("self test mode, don't wait!");
		return 0;
	}

	if (mode <= 0 || mode >= UTOPIA2K_STR_POWER_MAX) {
		pr_err("error!! no such mode [%d].\n", mode);
		return -1;
	}

	sprintf(node_path, "/%s/%s/%s/stage%d",
		UTOPIA2K_STR_NAME, name, power_mode[mode], stage);

	utopia2k_str_dbg("module %i in wait %s start...\n",
		task_pid_nr(current), node_path);

	stage_node = of_find_node_by_path(node_path);
	if (!stage_node)
		return -1;

	prop = of_find_property(stage_node, "pre-condition", NULL);

	prop_cnt = of_property_count_strings(stage_node, "pre-condition");

	if (prop_cnt <= 0) {
		ret = -1;
		goto wait_out_np;
	}

	p = prop->value;

	while (true) {
		for (i = 0, p = prop->value, total = 0;
				total < prop->length; total += l, p += l, i++) {
			l = strlen(p) + 1;
			/* for the following case:
			     pre-condition =
			     "xc_suspend_stage0", "", "oo_suspend_stage0";
			*/
			if (1 == l) {
				cnt++;
				continue;
			}
			spin_lock(&lock);
			list_for_each_entry(pc,
					&dts_post_condition_head, list) {
				if (pc && pc->name) {
					if (!strncmp(pc->name, p, l)) {
						cnt++;
						break;
					}
				}
			}
			spin_unlock(&lock);
		}
		if (cnt == prop_cnt)
			break;
		cnt = 0;
		schedule_timeout_interruptible(HZ / 1000);
	}

wait_out_np:

	of_node_put(stage_node);

	utopia2k_str_dbg("module %i in wait %s end.\n",
		task_pid_nr(current), node_path);
#endif
	return ret;
}
EXPORT_SYMBOL(mdrv_utopia2k_str_wait_condition);

int mdrv_utopia2k_str_send_condition(const char *name,
	MS_U32 mode, MS_U32 stage)
{
	int ret = 0;
#ifdef CONFIG_OF
	char node_path[128] = {0};
	struct device_node *stage_node;
	struct property *prop;

	int i = 0;
	size_t l = 0, total = 0;
	const char *p;

	struct dts_post_condition *pc;

	if (mode <= 0 || mode >= UTOPIA2K_STR_POWER_MAX) {
		pr_err("error!! no such mode [%d].\n", mode);
		return -1;
	}

	sprintf(node_path, "/%s/%s/%s/stage%d",
		UTOPIA2K_STR_NAME, name, power_mode[mode], stage);

	utopia2k_str_dbg("module %i in send %s start...\n",
		task_pid_nr(current), node_path);

	stage_node = of_find_node_by_path(node_path);
	if (!stage_node)
		return -1;

	prop = of_find_property(stage_node, "post-condition", NULL);

	if (!prop)
		goto send_out_np;
	if (!prop->value)
		goto send_out_np;
	if (strnlen(prop->value, prop->length) >= prop->length)
		goto send_out_np;

	p = prop->value;

	for (i = 0; total < prop->length; total += l, p += l, i++) {
		l = strlen(p) + 1;
		/* for the following case:
		    post-condition =
		    "xc_suspend_stage0", "", "oo_suspend_stage0";
		*/
		if (1 == l)
			continue;
		pc = kmalloc(sizeof(struct dts_post_condition) + l, GFP_KERNEL);
		if (!pc) {
			ret = -ENOMEM;
			break;
		}
		strncpy(pc->name, p, l);
		INIT_LIST_HEAD(&pc->list);
		spin_lock(&lock);
		list_add(&pc->list, &dts_post_condition_head);
		spin_unlock(&lock);
	}

send_out_np:

	of_node_put(stage_node);

	utopia2k_str_dbg("module %i in send %s end.\n",
		task_pid_nr(current), node_path);
#endif
	return ret;
}
EXPORT_SYMBOL(mdrv_utopia2k_str_send_condition);


#if UTOPIA2K_STR_SELF_TEST
int vdec_str(int u32_power_state, void *p_module)
{
	mdrv_utopia2k_str_wait_condition("vdec", u32_power_state, 0);
	schedule_timeout_interruptible(HZ);
	mdrv_utopia2k_str_send_condition("vdec", u32_power_state, 0);
	return 0;
}

int xc_str(int u32_power_state, void *p_module)
{
	mdrv_utopia2k_str_wait_condition("xc", u32_power_state, 0);
	schedule_timeout_interruptible(HZ/2);
	mdrv_utopia2k_str_send_condition("xc", u32_power_state, 0);
	return 0;
}

int oo_str(int u32_power_state, void *p_module)
{
	mdrv_utopia2k_str_wait_condition("oo", u32_power_state, 0);
	schedule_timeout_interruptible(HZ);
	mdrv_utopia2k_str_send_condition("oo", u32_power_state, 0);
	return 0;
}
#endif

static void mdrv_utopia2k_str_async_scheduler(void *data, async_cookie_t cookie)
{
	struct utopia2k_str_module *module = (struct utopia2k_str_module *)data;
	ktime_t starttime;

	if (utopia2k_str_selftest && module) {
		/* only suspend/resume selected module in selftest mode */
		if (!module->selftest)
			return;
	}

	if (module && module->fp_str != NULL) {
		/* 2s timeout */
		module->p = current;
		init_timer(&module->timer);
		module->timer.function = utopia2k_str_module_timeout;
		module->timer.data = (unsigned long)module;
		mod_timer(&module->timer, jiffies + 2 * HZ);
		/* debug module time start */
		starttime = utopia2k_str_time_debug_start(module);
		/* module entry */
		module->fp_str(module->mode, module->data);
		/* debug module time end */
		utopia2k_str_time_debug_report(starttime, module);
		del_timer_sync(&module->timer);
	}
}

int mdrv_utopia2k_str_module_selftest(void *p_module, char *module_name)
{
	struct utopia2k_str_module *module;

	utopia2k_str_selftest = 1;
	list_for_each_entry(module, &utopia2k_str_head, list) {
		if (module && module->fp_str != NULL
				&& module->data == p_module) {
			/* find the module & set its selftest */
			module->selftest = 1;
		}
	}
	list_for_each_entry(module, &utopia2k_str_head, list) {
		if (module && module->fp_str != NULL
				&& module->data == p_module) {
			module->mode = UTOPIA2K_STR_POWER_SUSPEND;
			async_schedule_domain(mdrv_utopia2k_str_async_scheduler,
				module, &condition_domain);
		}
	}
	async_synchronize_full_domain(&condition_domain);
	pr_debug("%s suspend finished\n", module_name);

	list_for_each_entry(module, &utopia2k_str_head, list) {
		if (module && module->fp_str != NULL
				&& module->data == p_module) {
			module->mode = UTOPIA2K_STR_POWER_RESUME;
			async_schedule_domain(mdrv_utopia2k_str_async_scheduler,
				module, &condition_domain);
		}
	}
	async_synchronize_full_domain(&condition_domain);
	pr_debug("%s resume finished\n", module_name);

	return 0;
}
EXPORT_SYMBOL(mdrv_utopia2k_str_module_selftest);

static int mstar_utopia2k_str_drv_suspend(struct platform_device *dev,
	pm_message_t state)
{
	struct utopia2k_str_module *module;
	struct dts_post_condition *pc, *n;

	list_for_each_entry(module, &utopia2k_str_head, list) {
		if (module && module->fp_str != NULL) {
			module->mode = UTOPIA2K_STR_POWER_SUSPEND;
			async_schedule_domain(mdrv_utopia2k_str_async_scheduler,
				module, &condition_domain);
		}
	}
	async_synchronize_full_domain(&condition_domain);

	list_for_each_entry_safe(pc, n, &dts_post_condition_head, list) {
		list_del(&pc->list);
		kfree(pc);
	}

	pr_info("-----------utopia2k str suspend-----------\n");

	return 0;
}
static int mstar_utopia2k_str_drv_resume(struct platform_device *dev)
{
	struct utopia2k_str_module *module;
	struct dts_post_condition *pc, *n;

	list_for_each_entry(module, &utopia2k_str_head, list) {
		if (module && module->fp_str != NULL) {
			module->mode = UTOPIA2K_STR_POWER_RESUME;
			async_schedule_domain(mdrv_utopia2k_str_async_scheduler,
				module, &condition_domain);
		}
	}
	async_synchronize_full_domain(&condition_domain);

	list_for_each_entry_safe(pc, n, &dts_post_condition_head, list) {
		list_del(&pc->list);
		kfree(pc);
	}

#if WAKUP_ANDROID_FROM_KERNEL
	get_wakeup_reason();
	/* Make sure resuming from power off, rather than break from suspend*/
	if ((STENT_RESUME_FROM_SUSPEND == get_state_value())
			&& utopia2k_str_should_report_wakeup_event()) {
		/* Hold wake lock for a while until Android really waking up*/
		__pm_wakeup_event(ws, 5 * MSEC_PER_SEC);
		utopia2k_str_report_wakeup_event();
	}
#endif
	pr_info("-----------utopia2k str resume-----------\n");
	return 0;
}

static int mstar_utopia2k_str_drv_probe(struct platform_device *pdev)
{
	int error = 0;
#if WAKUP_ANDROID_FROM_KERNEL
	struct device *dev = &pdev->dev;

	ws = wakeup_source_register(UTOPIA2K_STR_NAME);

	input = input_allocate_device();
	if (!input) {
		error = -ENOMEM;
		goto error_out;
	}
	input->name = "MStar Utopia2k STR Virtual Wakeup Event";
	input->phys = NULL;
	input->dev.parent = dev;

	input->id.bustype = BUS_VIRTUAL;
	input->id.vendor = 0x3697UL;
	input->id.product = 0x0001UL;
	input->id.version = 0x0001UL;

	input_set_capability(input, EV_KEY, KEY_POWER);
	input_set_capability(input, EV_KEY, KEY_CUSTOM_1);
	input_set_capability(input, EV_KEY, KEY_CUSTOM_2);
	input_set_capability(input, EV_KEY, KEY_CUSTOM_3);
	input_set_capability(input, EV_KEY, KEY_BUTTON_1);
	input_set_capability(input, EV_KEY, KEY_BUTTON_2);
	input_set_capability(input, EV_KEY, KEY_POWER_CEC);
	error = input_register_device(input);
	if (error)
		goto free_device;
	get_wakeup_reason();
	return 0;

free_device:
	input_free_device(input);
error_out:
	dev_err(dev, "probe failed, error: %d\n", error);
#endif
	return error;
}

static int mstar_utopia2k_str_drv_remove(struct platform_device *pdev)
{
#if WAKUP_ANDROID_FROM_KERNEL
	wakeup_source_unregister(ws);
	input_free_device(input);
#endif
	return 0;
}

static ssize_t mstar_utopia2k_str_read(struct file *filp,
	char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t mstar_utopia2k_str_write(struct file *filp,
	const char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}

static long mstar_utopia2k_str_ioctl(struct file *filp, u_int cmd, u_long arg)
{
	int ret = 0;

	switch (cmd) {
#if WAKUP_ANDROID_FROM_KERNEL
	case UTOPIA2K_STR_IOC_GET_WKUP_SRC:
		ret = put_user(pm_wakeup_source, (unsigned int *)arg);

		break;
	case UTOPIA2K_STR_IOC_SET_WKUP_SRC:
		ret = get_user(pm_wakeup_source, (unsigned int *)arg);
		pr_info(" pw_wakeup_source is set to 0x%x\n", pm_wakeup_source);
		break;
	case UTOPIA2K_STR_IOC_REPORT_EVENT:
		utopia2k_str_report_wakeup_event();
		break;
#endif
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static int mstar_utopia2k_str_release(struct inode *inode, struct file *filp)
{
	return 0;
}


static int mstar_utopia2k_str_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int show_power_on_reason(struct seq_file *m, void *v)
{
	pr_info("system is powered on by %s \n", power_on_src_name);
	seq_printf(m, "%s", power_on_src_name);
}

static int mstar_poweron_reason_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, show_power_on_reason, NULL);
}

static const struct file_operations mstar_utopia2k_str_fops = {
	.owner		= THIS_MODULE,
	.read		= mstar_utopia2k_str_read,
	.write		= mstar_utopia2k_str_write,
	.unlocked_ioctl = mstar_utopia2k_str_ioctl,
#if defined(CONFIG_COMPAT)
	.compat_ioctl = mstar_utopia2k_str_ioctl,
#endif
	.open		= mstar_utopia2k_str_open,
	.release	= mstar_utopia2k_str_release,
};

static const struct file_operations mstar_poweron_reason_fops = {
	.read = seq_read,
	.open = mstar_poweron_reason_open,
	.release = single_release,
};

#if defined(CONFIG_ARM64)
static struct of_device_id mstar_utopia2k_str_of_device_ids[] = {
		 {.compatible = UTOPIA2K_STR_NAME},
		 {},
};
#endif
static struct platform_driver Mstar_utopia2k_str_driver = {
	.probe		= mstar_utopia2k_str_drv_probe,
	.remove	= mstar_utopia2k_str_drv_remove,
	.suspend	= mstar_utopia2k_str_drv_suspend,
	.resume	= mstar_utopia2k_str_drv_resume,

	.driver = {
#if defined(CONFIG_ARM64)
		.of_match_table = mstar_utopia2k_str_of_device_ids,
#endif
		.name	= UTOPIA2K_STR_NAME,
		.owner	= THIS_MODULE,
	}
};

#ifndef CONFIG_OF
static struct platform_device Mstar_utopia2k_str_device = {
	.name	= UTOPIA2K_STR_NAME,
	.id	= 0,
};
#endif

static int __init mstar_utopia2k_str_init(void)
{
#ifndef CONFIG_OF
	platform_device_register(&Mstar_utopia2k_str_device);
#endif
	platform_driver_register(&Mstar_utopia2k_str_driver);

	INIT_LIST_HEAD(&dts_post_condition_head);
	INIT_LIST_HEAD(&utopia2k_str_head);
	spin_lock_init(&lock);

#ifdef CONFIG_PROC_FS
	proc_create(UTOPIA2K_STR_NAME,
		S_IRUGO | S_IWUGO, NULL, &mstar_utopia2k_str_fops);
	proc_create(POWERON_REASON_NAME,
		S_IRUSR, NULL, &mstar_poweron_reason_fops);
#endif

#if UTOPIA2K_STR_SELF_TEST
	mdrv_utopia2k_str_setup_function_ptr(NULL, oo_str);
	mdrv_utopia2k_str_setup_function_ptr(NULL, xc_str);
	mdrv_utopia2k_str_setup_function_ptr(NULL, vdec_str);
#endif

	return 0;
}

static void __exit mstar_utopia2k_str_exit(void)
{
	struct utopia2k_str_module *module;
	list_for_each_entry(module, &utopia2k_str_head, list) {
		list_del(&module->list);
		kfree(module);
	}
	platform_driver_unregister(&Mstar_utopia2k_str_driver);
#ifndef CONFIG_OF
	platform_device_unregister(&Mstar_utopia2k_str_device);
#endif
}

static int rtc_str_wake_debug_show(struct seq_file *s, void *data)
{
	seq_printf(s, "%d\n", rtc_str_wake);
}

static int rtc_str_wake_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtc_str_wake_debug_show, NULL);
}

static int rtc_str_wake_debug_write(struct file *file, const char __user *userbuf, size_t count, loff_t *ppos)
{
	char buf[64];
	unsigned int set;
	count = min_t(size_t, count, (sizeof(buf)-1));
	if (copy_from_user(buf, userbuf, count))
		return -EFAULT;

	buf[count] = '\0';

	if (strict_strtol(buf, 0, &set) != 0)
		return -EINVAL;
	pr_info("RTC Alarm %s turn on the screen \n", set ? "will" : "won't");
	rtc_str_wake = set;
	return count;
}

static const struct file_operations rtc_str_wake_debug_fops = {
	.open           = rtc_str_wake_debug_open,
	.read           = seq_read,
	.write          = rtc_str_wake_debug_write,
	.llseek         = seq_lseek,
	.release        = single_release,
};

static int __init rtc_str_wake_debug_init(void)
{
	struct deentry *d;

	d = debugfs_create_file("rtc_str_wake", 0664, NULL, NULL,
		&rtc_str_wake_debug_fops);
	if (!d) {
		pr_err("Failed to create rtc_str_wake debug file \n");
		return -ENOMEM;
	}

	return 0;
}

late_initcall(rtc_str_wake_debug_init);
module_init(mstar_utopia2k_str_init);
module_exit(mstar_utopia2k_str_exit);

MODULE_DESCRIPTION("Mstar utopia2k STR Device Driver");
MODULE_LICENSE("GPL");

