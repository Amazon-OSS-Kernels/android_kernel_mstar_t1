/*
 *  drivers/switch/switch_gpio.c
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/switch.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>

#define CONFIG_MSTAR_SWITCH_GPIO
#define GPIO_POLL

#ifdef CONFIG_MSTAR_SWITCH_GPIO
#include "mdrv_gpio.h"
#include "mhal_gpio_reg.h"

#ifdef GPIO_POLL
#include <linux/kthread.h>

static struct task_struct *gpio_poll_tsk;
#endif

#if defined(CONFIG_BOARD_AMAZON_COMPAL_ALISON)
static struct gpio_switch_platform_data headset_switch_data = {
		.name = "h2w",
		.gpio = PAD_PCM_D0,
};
#define HEADPHONE_DEBUG_ENABLE_GPIO PAD_PCM_D5

#elif defined(CONFIG_BOARD_AMAZON_TF_abc123)
static struct gpio_switch_platform_data headset_switch_data = {
		.name = "h2w",
		.gpio = PAD_SAR3,
};
#else
/* This is an example to reference how to define a headset detect pin
 To make a new one, please refer to source code above */
static struct gpio_switch_platform_data headset_switch_data = {
		.name = "h2w",
		.gpio = PAD_SAR3,
};
#endif
static struct platform_device headset_switch_device = {
       .name             = "switch-gpio",
       .dev = {
               .platform_data    = &headset_switch_data,
       }
};

#endif

enum SWITCH_DETECTION_CTRL {
	UI_OFF,
	UI_DISABLED,
	UI_ON
};

struct gpio_switch_data {
	struct switch_dev sdev;
	unsigned gpio;
	const char *name_on;
	const char *name_off;
	const char *state_on;
	const char *state_off;
	int irq;
	struct work_struct work;
	int ui_state;
};

static ssize_t ui_state_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct switch_dev *sdev = (struct switch_dev *)
		dev_get_drvdata(dev);
	struct gpio_switch_data	*switch_data =
		container_of(sdev, struct gpio_switch_data, sdev);

	sprintf(buf, "%d\n", switch_data->ui_state);

	return strlen(buf);
}

static ssize_t ui_state_store(struct device *dev, struct device_attribute *attr,
		char *buf, size_t count)
{
	int ctrl;
	struct switch_dev *sdev = (struct switch_dev *)
		dev_get_drvdata(dev);
	struct gpio_switch_data	*switch_data =
		container_of(sdev, struct gpio_switch_data, sdev);

	kstrtoint(buf, 10, &ctrl);
	printk(KERN_ERR "switch: set UI state value (%d) %s\n", ctrl, buf);
	if (ctrl < UI_OFF || ctrl > UI_ON) {
		printk(KERN_ERR "switch: invalid UI state value (%d) %s\n", ctrl, buf);
		return -1;
	}

	switch_data->ui_state = ctrl;
	schedule_work(&switch_data->work);

	return count;
}

static DEVICE_ATTR(ui_state, S_IRUGO|S_IWUSR|S_IWGRP, ui_state_show, ui_state_store);

static void gpio_switch_work(struct work_struct *work)
{
	int state;
#if defined(CONFIG_BOARD_AMAZON_COMPAL_ALISON)
	int debug_port_state;
#endif
	struct gpio_switch_data	*data =
		container_of(work, struct gpio_switch_data, work);
#ifdef CONFIG_MSTAR_SWITCH_GPIO
	state = MDrv_GPIO_Pad_Read(headset_switch_data.gpio);
#if defined(CONFIG_BOARD_AMAZON_COMPAL_ALISON)
	debug_port_state = MDrv_GPIO_Pad_Read(HEADPHONE_DEBUG_ENABLE_GPIO);
	/* Force the state to 0(unplug) for speaker ouptut when debug port is 1 */
	if (debug_port_state == 1)
		state = 0;
#endif

#else
	state = gpio_get_value(data->gpio);
#endif
    // Send Headphone event for abc123
	if (state)
        state=2;

	/* Do not report hp connection if UI hp state is OFF */
	if (data->ui_state == UI_OFF) {
		state = 0;
	} else if (data->ui_state == UI_ON) {
		state = 2;
	}

    switch_set_state(&data->sdev, state);
}

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
	struct gpio_switch_data *switch_data =
	    (struct gpio_switch_data *)dev_id;

	schedule_work(&switch_data->work);
	return IRQ_HANDLED;
}

static ssize_t switch_gpio_print_state(struct switch_dev *sdev, char *buf)
{
	struct gpio_switch_data	*switch_data =
		container_of(sdev, struct gpio_switch_data, sdev);
	const char *state;
	if (switch_get_state(sdev))
		state = switch_data->state_on;
	else
		state = switch_data->state_off;

	if (state)
		return sprintf(buf, "%s\n", state);
	return -1;
}

#ifdef CONFIG_MSTAR_SWITCH_GPIO
#ifdef GPIO_POLL
static void gpio_poll(void* arg)
{
    struct gpio_switch_data *switch_data =
	    (struct gpio_switch_data *)arg;

    int state;
#if defined(CONFIG_BOARD_AMAZON_COMPAL_ALISON)
	int debug_port_state;
#endif

    while(1)
    {
        schedule_timeout_interruptible(msecs_to_jiffies(100));
        state = MDrv_GPIO_Pad_Read(headset_switch_data.gpio);
#if defined(CONFIG_BOARD_AMAZON_COMPAL_ALISON)
		debug_port_state = MDrv_GPIO_Pad_Read(HEADPHONE_DEBUG_ENABLE_GPIO);
		/* Force the state to 0(unplug) for speaker ouptut when debug port is 1 */
		if (debug_port_state == 1)
			state = 0;
#endif

		/*  Change the state to 2 if plug detected for headphone event */
		if (state)
			state = 2;

        if (switch_data->sdev.state != state)
        {
            schedule_work(&switch_data->work);
        }
    }
}
#endif
#endif

static int gpio_switch_probe(struct platform_device *pdev)
{
	struct gpio_switch_platform_data *pdata = pdev->dev.platform_data;
	struct gpio_switch_data *switch_data;
	int ret = 0;

	if (!pdata)
		return -EBUSY;

	switch_data = kzalloc(sizeof(struct gpio_switch_data), GFP_KERNEL);
	if (!switch_data)
		return -ENOMEM;

	switch_data->sdev.name = pdata->name;
	switch_data->gpio = pdata->gpio;
	switch_data->name_on = pdata->name_on;
	switch_data->name_off = pdata->name_off;
	switch_data->state_on = pdata->state_on;
	switch_data->state_off = pdata->state_off;
	switch_data->sdev.print_state = switch_gpio_print_state;
	switch_data->ui_state = UI_DISABLED;

	ret = switch_dev_register(&switch_data->sdev);
	if (ret < 0)
		goto err_switch_dev_register;

	ret = device_create_file(switch_data->sdev.dev, &dev_attr_ui_state);
	if (ret < 0)
		goto err_create_file_ui_state;

#ifdef CONFIG_MSTAR_SWITCH_GPIO
    MDrv_GPIO_Init();
    switch_data->gpio = headset_switch_data.gpio;
#else
	ret = gpio_request(switch_data->gpio, pdev->name);
	if (ret < 0)
		goto err_request_gpio;

	ret = gpio_direction_input(switch_data->gpio);
	if (ret < 0)
		goto err_set_gpio_input;
#endif

	INIT_WORK(&switch_data->work, gpio_switch_work);

#ifdef CONFIG_MSTAR_SWITCH_GPIO

#ifdef GPIO_POLL
    gpio_poll_tsk = kthread_create(gpio_poll, switch_data, "GPIO poll Task");
	if (IS_ERR(gpio_poll_tsk)) {
		printk("create kthread for GPIO poll Task fail\n");
	}else
		wake_up_process(gpio_poll_tsk);
#else

#endif

#else
	switch_data->irq = gpio_to_irq(switch_data->gpio);
	if (switch_data->irq < 0) {
		ret = switch_data->irq;
		goto err_detect_irq_num_failed;
	}

	ret = request_irq(switch_data->irq, gpio_irq_handler,
			  IRQF_TRIGGER_LOW, pdev->name, switch_data);
	if (ret < 0)
		goto err_request_irq;
#endif
	/* Perform initial detection */
	gpio_switch_work(&switch_data->work);

	return 0;

err_request_irq:
err_detect_irq_num_failed:
err_set_gpio_input:
	gpio_free(switch_data->gpio);
err_request_gpio:
	device_remove_file(&switch_data->sdev.dev, &dev_attr_ui_state);
err_create_file_ui_state:
	switch_dev_unregister(&switch_data->sdev);
err_switch_dev_register:
	kfree(switch_data);

	return ret;
}

static int gpio_switch_remove(struct platform_device *pdev)
{
	struct gpio_switch_data *switch_data = platform_get_drvdata(pdev);

	cancel_work_sync(&switch_data->work);
	gpio_free(switch_data->gpio);
	device_remove_file(switch_data->sdev.dev, &dev_attr_ui_state);
	switch_dev_unregister(&switch_data->sdev);
	kfree(switch_data);

	return 0;
}

static struct platform_driver gpio_switch_driver = {
	.probe		= gpio_switch_probe,
	.remove		= gpio_switch_remove,
	.driver		= {
		.name	= "switch-gpio",
		.owner	= THIS_MODULE,
	},
};

static int __init gpio_switch_init(void)
{
#ifdef CONFIG_MSTAR_SWITCH_GPIO
    platform_device_register(&headset_switch_device);
#endif
	return platform_driver_register(&gpio_switch_driver);
}

static void __exit gpio_switch_exit(void)
{
	platform_driver_unregister(&gpio_switch_driver);
}

module_init(gpio_switch_init);
module_exit(gpio_switch_exit);

MODULE_AUTHOR("Mike Lockwood <lockwood@android.com>");
MODULE_DESCRIPTION("GPIO Switch driver");
MODULE_LICENSE("GPL");
