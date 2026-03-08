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


#if defined(CONFIG_HAS_LED)


#include <linux/types.h>
#include <linux/fs.h>
#include <linux/io.h>

#include "mdrv_mstypes.h"

#include "mdrv_mbx.h"

#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/delay.h>

#include <mstar/mstar_chip.h>

#include <linux/module.h>
#include <linux/platform_device.h>

#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/leds.h>
#include <linux/sign_of_life.h>

#if defined(CONFIG_AMAZON_METRICS_LOG)
#include <linux/metricslog.h>
#endif

static struct mutex lock;

typedef enum{
	LED_DRV_INIT = 1,
	LED_ON_RED,
	LED_OFF_RED,
	LED_BREATH_RED,
	LED_BLINK_ONCE_RED,
	LED_KEEP_BLINK_RED,
	LED_ON_BLUE,
	LED_OFF_BLUE,
	LED_BREATH_BLUE,
	LED_BLINK_ONCE_BLUE,
	LED_KEEP_BLINK_BLUE
} LED_CONTROL;


#define INVALID_MBXCLASS(mbxClass)  (mbxClass >= E_MBX_CLASS_MAX)
#define PM_MBX_TIMEOUT      5000
#define PM_MBX_QUEUESIZE    8
#define PM_CMDIDX_LED       0x42

/* Message from SN to decide LED status */
#define LED_PWMGPIO_LIGHT_R   0xBB            /* Turn on Red LED		*/
#define LED_PWMGPIO_DARK_R    0xBC            /* Turn off Red LED		*/
#define LED_PWMGPIO_BREATH_R  0xBD            /* Red LED breath			*/
#define LED_PWMGPIO_FLICKER_R 0xBE            /* Red LED blink once		*/
#define LED_PWMGPIO_FLICKER_POWERON_R 0xBF    /* Red LED keep blinking	*/


#define LED_PWMGPIO_LIGHT_B 0xCB			/* Turn on Blue LED			*/
#define LED_PWMGPIO_DARK_B 0xCC				/* Turn off Blue LED		*/
#define LED_PWMGPIO_BREATH_B 0xCD			/* Blue LED breath			*/
#define LED_PWMGPIO_FLICKER_B 0xCE			/* Blue LED blink once		*/
#define LED_PWMGPIO_FLICKER_POWERON_B 0xCF	/* Blue LED keep blinking	*/
#define E_PM_CMDIDX_ACK_51ToARM 0x31  /*Need to double confirm this once get the RT_PM source code */

#define LED_RED_CTRL_FLAG 0x5500
#define LED_BLUE_CTRL_FLAG 0xAA00

static int debug_enable_led;
static int led_init_status;
static int led_red_status;
static int led_blue_status;
#define LEDS_DRV_DEBUG(format, args...) do { \
if (debug_enable_led) {\
	pr_err(format, ##args);\
	} \
} while (0)


/*=============================================================================
 * Local Variables
 *============================================================================= */
static DEFINE_MUTEX(Semutex_LD);
static DEFINE_SPINLOCK(spinlock_ld);
static struct file *mbx_fp;
static MS_BOOL bReceived;

#if defined(CONFIG_AMAZON_METRICS_LOG)

struct metrics_info {
	int flags;

	/* Time when system enters full suspend */
	struct timespec suspend_time;
};
static struct metrics_info info;


static void bq_log_metrics(char *msg,
	char *metricsmsg)
{
	char buf[512];
	struct timespec curr = current_kernel_time();
	/* Compute elapsed time and determine screen off or on drainage */
	struct timespec diff = timespec_sub(curr,
			info.suspend_time);

	snprintf(buf, sizeof(buf),
		"%s:def:value=0;CT;1,elapsed=%ld;TI;1:NR",
		metricsmsg,
		diff.tv_sec * 1000 + diff.tv_nsec / NSEC_PER_MSEC);
	log_to_metrics(ANDROID_LOG_INFO, "drain_metrics", buf);
	/* Mark the suspend or resume time */
	info.suspend_time = curr;
}

#endif

/*=============================================================================
 * Local Functions
 *============================================================================= */


static int MDrv_LED_RecHandler(void)
{
	MBX_Result enMbxResult = E_MBX_UNKNOW_ERROR;
	MBX_Msg stMbxCommand;
	MS_U8 u8Ack = 0;

	if (TRUE != bReceived)
		return E_MBX_SUCCESS;

	/* Waiting for message done */
	memset((void *)&stMbxCommand, 0x00, sizeof(MBX_Msg));
	spin_lock_irq(&spinlock_ld);
	enMbxResult = MDrv_MBX_RecvMsg((MS_U64)mbx_fp, E_MBX_CLASS_PM_NOWAIT, &stMbxCommand, 1, \
		MBX_CHECK_NORMAL_MSG, 0);
	spin_unlock_irq(&spinlock_ld);
	LEDS_DRV_DEBUG("   rec enMbxResult:%d \n", enMbxResult);

   /*check result */
	if (enMbxResult == E_MBX_SUCCESS) {
		if ((stMbxCommand.u8Ctrl != 0) && (stMbxCommand.u8Ctrl != 1)) {
			enMbxResult = E_MBX_ERR_NOT_IMPLEMENTED;
			pr_err("Not Implemented!\n");
		}

		switch (stMbxCommand.u8Index) {
		case E_PM_CMDIDX_ACK_51ToARM:
			u8Ack = (MS_U8)stMbxCommand.u8Parameters[0];
			break;
		default:
			break;
		}
	} else if (E_MBX_ERR_TIME_OUT == enMbxResult) {
		pr_err("Handler receive timeout! nMbxResult:%d \n", enMbxResult);
	} else {
		pr_err("Handler receive error nMbxResult:%d \n", enMbxResult);
	}

	return enMbxResult;
}


static MS_S8 MDrv_LD_SetupMbx(void)
{
	MBX_Result enMbxResult = E_MBX_UNKNOW_ERROR;
	mbx_fp = filp_open("/dev/msmailbox", O_RDWR, 0);

	if (IS_ERR(mbx_fp)) {
		mbx_fp = NULL;
		pr_err("!! OPEN fail  \n");
		return -1;
	}

	enMbxResult = MDrv_MBX_Init((MS_U64)mbx_fp, E_MBX_CPU_MIPS, E_MBX_ROLE_HK, PM_MBX_TIMEOUT);
	if (E_MBX_SUCCESS != enMbxResult) {
		pr_err("!! MDrv_MBX_Init fail nMbxResult:%d \n", enMbxResult);
		return -1;
	} else {
		extern int MDrv_MBX_NotifyMsgRecCbFunc(void *);
		MDrv_MBX_Enable((MS_U64)mbx_fp, TRUE);
		MDrv_MBX_NotifyMsgRecCbFunc(MDrv_LED_RecHandler);

		enMbxResult = MDrv_MBX_RegisterMSG((MS_U64)mbx_fp, E_MBX_CLASS_PM_NOWAIT, PM_MBX_QUEUESIZE);
		if (E_MBX_SUCCESS != enMbxResult) {
			pr_err("!!MDrv_MBX_RegisterMSG fail nMbxResult:%d \n", enMbxResult);
		}

		return enMbxResult;
	}
}




MBX_Result LED_DRV_Ctrl(MS_U16 var)
{
	MBX_Msg stMbxCommand;
	MS_U16 command;
	MBX_Result enMbxResult = E_MBX_UNKNOW_ERROR;
	LEDS_DRV_DEBUG("LED_DRV_Ctrl command(%d)\n", command);
	if (led_init_status == 0) {
		if (MDrv_LD_SetupMbx() != E_MBX_SUCCESS) {
			pr_err("MDrv_LD_SetupMbx failed\n");
		}
		led_init_status = 1;
	}

	switch (var) {
	case LED_DRV_INIT:
		if (led_init_status == 0) {
			if (MDrv_LD_SetupMbx() != E_MBX_SUCCESS) {
				pr_err("MDrv_LD_SetupMbx failed\n");
				return -1;
			}
			led_init_status = 1;
		}
		return 1;
	case LED_ON_RED:
		command = LED_PWMGPIO_LIGHT_R;
		led_red_status = 1;
		LEDS_DRV_DEBUG("Turn on Red LED\n");
		break;
	case LED_OFF_RED:
		command = LED_PWMGPIO_DARK_R;
		led_red_status = 0;
		LEDS_DRV_DEBUG("Turn off Red LED\n");
		break;
	case LED_BREATH_RED:
		command = LED_PWMGPIO_BREATH_R;
		if (led_red_status == 0) {
			command |= LED_RED_CTRL_FLAG;
		}
		LEDS_DRV_DEBUG("Set LED to breath mode \n");
		break;
#if 0
	case LED_BLINK_ONCE_RED:
		command = LED_PWMGPIO_FLICKER_R;
		if (led_red_status == 0) {
			command |= LED_RED_CTRL_FLAG;
		}
		LEDS_DRV_DEBUG("LED blink once 0x%x \n" , command);
		break;
#endif
	case LED_KEEP_BLINK_RED:
		command = LED_PWMGPIO_FLICKER_POWERON_R;
		if (led_red_status == 0) {
			command |= LED_RED_CTRL_FLAG;
		}
		LEDS_DRV_DEBUG("LED keep blinking\n");
		break;
	case LED_ON_BLUE:
		command = LED_PWMGPIO_LIGHT_B;
		led_blue_status = 1;
		LEDS_DRV_DEBUG("Turn on Blue LED\n");
		break;
	case LED_OFF_BLUE:
		command = LED_PWMGPIO_DARK_B;
		led_blue_status = 0;
		LEDS_DRV_DEBUG("Turn off Blue LED \n");
		break;
	case LED_BREATH_BLUE:
		command = LED_PWMGPIO_BREATH_B;
		if (led_blue_status == 0) {
			command |= LED_BLUE_CTRL_FLAG;
		}
		LEDS_DRV_DEBUG("Blue LED breath \n");
		break;
	case LED_BLINK_ONCE_BLUE:
		command = LED_PWMGPIO_FLICKER_B;
		if (led_blue_status == 0) {
			command |= LED_BLUE_CTRL_FLAG;
		}
		LEDS_DRV_DEBUG("Blue LED blink once \n");
		break;
	case LED_KEEP_BLINK_BLUE:
		command = LED_PWMGPIO_FLICKER_POWERON_B;
		if (led_blue_status == 0) {
			command |= LED_BLUE_CTRL_FLAG;
		}
		LEDS_DRV_DEBUG("Blue LED keep blinkig \n");
		break;
	default:
		pr_err("Wrong LED control command \n");
		return 0;
	}
	/*send msg to PM */
	memset((void *)&stMbxCommand, 0, sizeof(MBX_Msg));
	stMbxCommand.eRoleID = E_MBX_ROLE_PM;               /* Do Not Change */
	stMbxCommand.eMsgType = E_MBX_MSG_TYPE_INSTANT;     /* Do Not Change */
	stMbxCommand.u8Ctrl = 0;                            /* Do Not Change */
	stMbxCommand.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;    /* Do Not Change */
	stMbxCommand.u8ParameterCount = 2;                  /* Implement by yourself Between ARM & PM */
	stMbxCommand.u8Parameters[0] = (MS_U8)(0xAA);
	stMbxCommand.u8Parameters[1] = (MS_U8)(0xBB);       /* reserve for support freq in Breath and Flickr mode */

	if (command & LED_RED_CTRL_FLAG) {
		stMbxCommand.u8Index = LED_PWMGPIO_LIGHT_R;
		enMbxResult = MDrv_MBX_SendMsg((MS_U64)mbx_fp, &stMbxCommand, 0);
		LEDS_DRV_DEBUG("LED_DRV_Ctrl, Turn on RED led firstly, result is 0x%x\n", enMbxResult);
		led_red_status = 1;
		udelay(5);
	}
	if (command & LED_BLUE_CTRL_FLAG) {
		stMbxCommand.u8Index = LED_PWMGPIO_LIGHT_B;
		enMbxResult = MDrv_MBX_SendMsg((MS_U64)mbx_fp, &stMbxCommand, 0);
		LEDS_DRV_DEBUG("LED_DRV_Ctrl, Turn on Blue led firstly, result is 0x%x\n", enMbxResult);
		led_blue_status = 1;
		udelay(5);
	}
	stMbxCommand.u8Index = (U8)(command&0x00FF);                     /* Implement by yourself Between ARM & PM */
	enMbxResult = MDrv_MBX_SendMsg((MS_U64)mbx_fp, &stMbxCommand, 0);
	return enMbxResult;
}

/****************************************************************************
 * driver functions
 ***************************************************************************/

struct margo_led {
	struct led_classdev cdev;
	const char *name;

};
static struct margo_led leds[] = {
	{
		.name = "tv_led",
	},
#if defined(CONFIG_AMAZON_METRICS_LOG)
	{
		.name = "dummy_light",
	}
#endif
};

static ssize_t led_set(struct device *dev, struct device_attribute *attr,
				  const char *buf, size_t size)
{
	unsigned int action = 0;
	int ret = 0;
	mutex_lock(&lock);
	if (buf != NULL && size != 0) {
		LEDS_DRV_DEBUG("[led action] buf is %s and size is %zd\n", buf, size);
		ret = sscanf(buf, "%d", &action);
		if (ret != 1)
			return -EINVAL;
		if (action > 0xff)
			return -EINVAL;
		if (led_init_status == 0) {
			LED_DRV_Ctrl(LED_DRV_INIT);
			led_init_status = 1;
		}
		if (action == LED_BLINK_ONCE_RED) {
			if (led_red_status == 1) {
				LED_DRV_Ctrl(LED_OFF_RED);
				udelay(100);
			}
			LED_DRV_Ctrl(LED_ON_RED);
			udelay(10000);
			LED_DRV_Ctrl(LED_OFF_RED);
			mutex_unlock(&lock);
			return size;
		}

		if (action > LED_KEEP_BLINK_RED) {
			if (led_red_status == 1) {
				LED_DRV_Ctrl(LED_OFF_RED);
				led_red_status = 0;
			}
		} else {
			if (led_blue_status == 1) {
				LED_DRV_Ctrl(LED_OFF_BLUE);
				led_blue_status = 0;
			}
		}
		ret = LED_DRV_Ctrl(action);
	}
	mutex_unlock(&lock);
	return size;
}
static DEVICE_ATTR(tv_led_set, 0222, NULL, led_set);

#if defined(CONFIG_AMAZON_METRICS_LOG)

static ssize_t dummy_light_set(struct device *dev, struct device_attribute *attr,
				  const char *buf, size_t size)
{
	unsigned int action = 0;
	int ret = 0;
	if (buf != NULL && size != 0) {
		LEDS_DRV_DEBUG("[dummy light action] buf is %s and size is %zd\n", buf, size);
		ret = sscanf(buf, "%d", &action);
		if (ret != 1)
			return -EINVAL;
	}
	switch (action) {
	case 0:
		mstar_set_screen_flag();
		pr_info("backlight is off \n");
		bq_log_metrics("Screen on drainage", "screen_on_drain");
		break;
	case 1:
		mstar_clear_screen_flag();
		pr_info("backlight is on\n");
		bq_log_metrics("Screen off drainage", "screen_off_drain");
		break;
	default:
		break;
	}

	return size;
}
static DEVICE_ATTR(light_set, 0222, NULL, dummy_light_set);
#endif

static int mstar_leds_probe(struct platform_device *pdev)
{
	int i;
	int ret, rc;
	info.suspend_time = current_kernel_time();
	LEDS_DRV_DEBUG("[LED]%s\n", __func__);
	for (i = 0; i < ARRAY_SIZE(leds); i++) {

		leds[i].cdev.name = leds[i].name;
		ret = led_classdev_register(&pdev->dev, &leds[i].cdev);
		if (ret < 0)
			goto err;
		if (strcmp(leds[i].name, "tv_led") == 0) {
			rc = device_create_file(leds[i].cdev.dev, &dev_attr_tv_led_set);
			if (rc)
				pr_err("[LED]device_create_file led_pattern fail!\n");
		}
	#if defined(CONFIG_AMAZON_METRICS_LOG)
		if (strcmp(leds[i].name, "dummy_light") == 0) {
			rc = device_create_file(leds[i].cdev.dev, &dev_attr_light_set);
			if (rc)
				pr_err("[LED]device_create_file dummy_light fail!\n");
		}
	#endif
	}
	led_init_status = 0;
	mutex_init(&lock);
	return 0;

 err:
	if (i) {
		for (i = i - 1; i >= 0; i--) {
			led_classdev_unregister(&leds[i].cdev);
		}
	}
	return ret;
}

static int mstar_leds_remove(struct platform_device *pdev)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(leds); i++) {
		led_classdev_unregister(&leds[i].cdev);
	}

	return 0;
}

static struct platform_driver mstar_leds_driver = {
	.driver = {
		   .name = "leds-mstar",
		   .owner = THIS_MODULE,
		   },
	.probe = mstar_leds_probe,
	.remove = mstar_leds_remove,
};

static struct platform_device mstar_leds_device = {
	.name = "leds-mstar",
	.id = -1
};

static int __init mstar_leds_init(void)
{
	int ret;

	LEDS_DRV_DEBUG("[LED]%s\n", __func__);
	ret = platform_device_register(&mstar_leds_device);
	if (ret)
		pr_info("[LED]mstar_leds_init:dev:E%d\n", ret);

	ret = platform_driver_register(&mstar_leds_driver);

	if (ret) {
		pr_info("[LED]mstar_leds_init:drv:E%d\n", ret);
		platform_device_unregister(&mstar_leds_device);
		return ret;
	}

	return ret;
}

static void __exit mstar_leds_exit(void)
{
	platform_driver_unregister(&mstar_leds_driver);
	platform_device_unregister(&mstar_leds_device);
}



late_initcall(mstar_leds_init);
module_exit(mstar_leds_exit);

MODULE_AUTHOR("Mstar.");
MODULE_DESCRIPTION("LED driver for Mstar chip");
MODULE_LICENSE("GPL");
MODULE_ALIAS("leds-mstar");
#endif

