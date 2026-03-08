/*
 *  drivers/switch/irblaster_detection.c
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
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/kdev_t.h>

#ifdef CONFIG_AMAZON_METRICS_LOG
#include <linux/metricslog.h>
#include <linux/vmalloc.h>
#ifndef BLASTER_METRICS_STR_LEN
#define BLASTER_METRICS_STR_LEN 128
#endif
#endif

#include "mdrv_gpio.h"
#include "mhal_gpio.h"
#include "mhal_gpio_reg.h"

#define DEVICE_NAME		"irblaster"

static struct task_struct *irblaster_poll_tsk;
static struct device *irblaster_dev;
struct class *irblaster_class;

enum IRBLASTER_STATUS {
	IRBLASTER_UNPLUGGED		= 0,
	IRBLASTER_PLUG_IN		= 1,
	IRBLASTER_FAULT_IN		= 2,
};


struct irblaster_detection_dev {
	struct device *dev;
#ifdef FAULTDETECTION_INTERRUPT_MODE
	struct work_struct work;

	int irfaultdetect_irq;
#endif
	int irdetect_gpio;
	int irfaultdetect_gpio;
	int irvregen_gpio;
	int irbucken_gpio;
	int	state;
	bool is_suspend;
};


static struct platform_device irblaster_detection_device = {
	.name	= "irblaster-detection",
};

static ssize_t show_plug(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct irblaster_detection_dev *irdev = dev_get_drvdata(dev);
	sprintf(buf, "%d\n", irdev->state);
	return strlen(buf);
}
static DEVICE_ATTR(plug, S_IRUGO, show_plug, NULL);

static void set_state(struct irblaster_detection_dev *dev, int state)
{
#ifdef CONFIG_AMAZON_METRICS_LOG
	char *blaster_metric_prefix = "blaster:def:monitor=1;CT;1";
	char mbuf[BLASTER_METRICS_STR_LEN + 1];
#endif
	char event_string[10];
	char *envp[] = { event_string, NULL };
	pr_debug("irblaster: state = %d\n", state);

	if (dev->state != state) {
		dev->state = state;
		snprintf(event_string, sizeof(event_string), "plug=%d", state);
		pr_debug("irblaster: generate IR detect uevent %s\n", envp[0]);
		kobject_uevent_env(&dev->dev->kobj, KOBJ_CHANGE, envp);
#ifdef CONFIG_AMAZON_METRICS_LOG
		snprintf(mbuf, BLASTER_METRICS_STR_LEN,
			"%s,irjack_dtected_%d;CT;",
			blaster_metric_prefix, state);
		log_to_metrics(ANDROID_LOG_INFO, "BlasterEvent", mbuf);
		log_counter_to_vitals(ANDROID_LOG_INFO, "Kernel", "Kernel",
			"BLASTER", "plug", (u32)state,
			"count", NULL, VITALS_NORMAL);

#endif
	}
}

#ifdef FAULTDETECTION_INTERRUPT_MODE
static void irblaster_faultflag_work(struct work_struct *work)
{
	int state;
	struct irblaster_detection_dev	*dev =
		container_of(work, struct irblaster_detection_dev, work);

	state = MDrv_GPIO_Pad_Read(dev->irfaultdetect_gpio);

	if (state) {
		state = IRBLASTER_FAULT_IN;
		pr_err("irblaster: FAULT - over current or temperature\n");
	}
	set_state(dev, state);
}

static irqreturn_t faultflag_irq_handler(int irq, void *data)
{
	struct irblaster_detection_dev *dev =
	    (struct irblaster_detection_dev *)data;

	schedule_work(&dev->work);
	return IRQ_HANDLED;
}
#endif

static int irblaster_poll(void *arg)
{
	struct irblaster_detection_dev *dev =
		(struct irblaster_detection_dev *)arg;
	int state = 0;

	while (1) {
		schedule_timeout_interruptible(msecs_to_jiffies(100));
		if (!dev->is_suspend) {
			if (MDrv_GPIO_Pad_Read(dev->irfaultdetect_gpio) == 0)
				state = IRBLASTER_FAULT_IN;
			else
				state = MDrv_GPIO_Pad_Read(dev->irdetect_gpio);
			set_state(dev, state);
		}
	}
	return 0;
}

static int irblaster_detection_probe(struct platform_device *pdev)
{
	struct irblaster_detection_dev *dev;
	int ret = 0;

	dev = kzalloc(sizeof(struct irblaster_detection_dev), GFP_KERNEL);
	if (!dev) {
		pr_err("irblaster: irblaster_detection_dev allocation fail");
		return -ENOMEM;
	}

	dev->irdetect_gpio = PAD_PCM_A5;
	dev->irfaultdetect_gpio = PAD_PCM2_IRQA_N;
#ifdef FAULTDETECTION_INTERRUPT_MODE
	dev->irfaultdetect_irq = MHal_GPIO_Get_Interrupt_Num(PAD_PCM2_IRQA_N);
#endif
	dev->irvregen_gpio = PAD_PCM_A7;
	dev->irbucken_gpio = PAD_PCM_A8;
	platform_set_drvdata(pdev, dev);
	MDrv_GPIO_Init();
	MDrv_GPIO_Set_Input(dev->irdetect_gpio);
	MDrv_GPIO_Set_Input(dev->irfaultdetect_gpio);
	MDrv_GPIO_Set_High(dev->irvregen_gpio);
	MDrv_GPIO_Set_High(dev->irbucken_gpio);

	irblaster_poll_tsk = kthread_create(irblaster_poll, dev, "Irblaster poll Task");
	if (IS_ERR(irblaster_poll_tsk)) {
		pr_err("irblaster: create kthread for GPIO poll Task fail\n");
		ret = PTR_ERR(irblaster_poll_tsk);
		goto err_kthread_create;
	} else
		wake_up_process(irblaster_poll_tsk);

	irblaster_class = class_create(THIS_MODULE, DEVICE_NAME);
	if (IS_ERR(irblaster_class)) {
		pr_err("irblaster: create class fail\n");
		ret = PTR_ERR(irblaster_class);
		goto err_class_create;
	}
	irblaster_dev = device_create(irblaster_class, NULL,
			MKDEV(0, 0), dev, "irblaster%d", 1);
	dev->dev = irblaster_dev;

	ret = device_create_file(irblaster_dev, &dev_attr_plug);
	if (ret < 0)
		goto err_create_file;

#ifdef FAULTDETECTION_INTERRUPT_MODE
	INIT_WORK(&dev->work, irblaster_faultflag_work);

	ret = request_irq(dev->irfaultdetect_irq, faultflag_irq_handler,
			  IRQF_TRIGGER_RISING, pdev->name, dev);
	if (ret < 0) {
		pr_err("irblaster: request_irq fail\n");
		goto err_request_irq;
	}
#endif
	set_state(dev, MDrv_GPIO_Pad_Read(dev->irdetect_gpio));
	return 0;

#ifdef FAULTDETECTION_INTERRUPT_MODE
err_request_irq:
	device_remove_file(irblaster_dev, &dev_attr_plug);
#endif
err_create_file:
	device_destroy(irblaster_class, MKDEV(0, 0));
	class_destroy(irblaster_class);
err_class_create:
err_kthread_create:
	kfree(dev);
	return ret;
}

static int irblaster_detection_remove(struct platform_device *pdev)
{
	struct irblaster_detection_dev *dev = platform_get_drvdata(pdev);

#ifdef FAULTDETECTION_INTERRUPT_MODE
	free_irq(dev->irfaultdetect_irq, dev);
#endif
	device_remove_file(irblaster_dev, &dev_attr_plug);
	device_destroy(irblaster_class, MKDEV(0, 0));
	class_destroy(irblaster_class);
	return 0;
}

static int irblaster_detection_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct irblaster_detection_dev *dev = platform_get_drvdata(pdev);

	MDrv_GPIO_Set_Low(dev->irvregen_gpio);
	MDrv_GPIO_Set_Low(dev->irbucken_gpio);
	dev->is_suspend = true;
	return 0;
}

static int irblaster_detection_resume(struct platform_device *pdev)
{
	struct irblaster_detection_dev *dev = platform_get_drvdata(pdev);

	MDrv_GPIO_Set_High(dev->irvregen_gpio);
	MDrv_GPIO_Set_High(dev->irbucken_gpio);
	dev->is_suspend = false;
	return 0;
}

static struct platform_driver irblaster_detection_driver = {
	.probe		= irblaster_detection_probe,
	.remove		= irblaster_detection_remove,
	.suspend	= irblaster_detection_suspend,
	.resume		= irblaster_detection_resume,
	.driver		= {
		.name	= "irblaster-detection",
		.owner	= THIS_MODULE,
	},
};

static int __init irblaster_detection_init(void)
{
	int ret;
	ret = platform_device_register(&irblaster_detection_device);
	if (ret < 0) {
		pr_err("failed to register irblaster device\n");
		return ret;
	}
	ret = platform_driver_register(&irblaster_detection_driver);
	if (ret < 0) {
		pr_err("failed to register irblaster driver\n");
		platform_device_unregister(&irblaster_detection_device);
	}
	return ret;
}

static void __exit irblaster_detection_exit(void)
{
	platform_driver_unregister(&irblaster_detection_driver);
	platform_device_unregister(&irblaster_detection_device);
}

module_init(irblaster_detection_init);
module_exit(irblaster_detection_exit);

MODULE_AUTHOR("Shenbo Huang <hsymbol@amazon.com>");
MODULE_DESCRIPTION("IRblaster detection");
MODULE_LICENSE("GPL");

