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
/// file    mdrv_gpio_io.c
/// @brief  GPIO Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/irqdomain.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/gpio.h>
#include <linux/vmalloc.h>

#include "mst_devid.h"
#include "mdrv_gpio_io.h"
#include "mhal_gpio_reg.h"
#include "mhal_gpio.h"
//#include "mdrv_gpio.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define GPIO_DBG_ENABLE		1
#if GPIO_DBG_ENABLE
#define GPIO_PRINT(fmt, args...)        printk("[GPIO][%05d] " fmt, __LINE__, ## args)
#else
#define GPIO_PRINT(fmt, args...)
#endif
#define GPIO_MAX_PIN_NUMBER 200
#define GPIO_MAX_RESERVED_PIN_NUMBER 20 /* reserved pins that should not be exposed to sysfs. E.g. swi2c pads */
struct mstar_gpio_dev
{
	struct platform_device* pdev;
	struct irq_domain *irq_domain;
	u32 eint_num;
	spinlock_t lock;
	u32 *pin_status;
	u32 ngpio;
};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

static int mstar_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	int value = MHal_GPIO_Pad_Level(offset);
	//GPIO_PRINT(KERN_EMERG "[%s] gpio_%d: value=%d\n", __FUNCTION__, offset, value);
	return value;
}

static void mstar_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	//GPIO_PRINT(KERN_EMERG "[%s] gpio_%d: value=%d\n", __FUNCTION__, offset, value);
	if (value) //high
	{
		MHal_GPIO_Set_High(offset);
	}
	else
	{
		MHal_GPIO_Set_Low(offset);
	}
}

static int mstar_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	//MHal_GPIO_Pad_Odn(offset);
	MHal_GPIO_Set_Input(offset);
	return 0;
}

static int mstar_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
	//mstar_gpio_set(chip, offset, value);
	MHal_GPIO_Pad_Oen(offset);
	return 0;
}

static int mstar_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	return MHal_GPIO_Get_Interrupt_Num((u8)offset);
}

static struct gpio_chip mstar_gpio_chip = {
	.label			= "gpio",
	.to_irq 			= mstar_gpio_to_irq,
	.direction_input	= mstar_gpio_direction_input,
	.direction_output	= mstar_gpio_direction_output,
	.set				= mstar_gpio_set,
	.get				= mstar_gpio_get,
	.base			= 0,
};

#if 0
static irqreturn_t test_irq_handler(int irq, void *data)
{
	int a; 
	a++;
	//printk("==[%s]==\n",__FUNCTION__);
	return a;//dead code
}
static void mstar_gpio_test_irq(void)
{
	int irq_num;
	int i;
	int ret;

	GPIO_PRINT("test irq start\n");
	for (i = 0; i <8; i++)
	{
		irq_num = gpio_to_irq(gpio_IntPad[i]);
		ret = request_irq(irq_num, test_irq_handler, 0, "gpio_test", NULL);
		if(ret < 0)
			GPIO_PRINT("request irq failed :%d\n", ret);
		else
			GPIO_PRINT("requested irq: %d\n", irq_num);

		ret = irq_set_irq_type(irq_num, IRQ_TYPE_EDGE_RISING);
		if(ret < 0)
		{
			GPIO_PRINT("failed to set irq%d type to rising: %d \n", irq_num, ret);
			return;
		}
		disable_irq(irq_num);
		enable_irq(irq_num);
		/* TODO: create an use case to verify gpio interrupt*/
	}
}
#endif

static int mstar_gpio_dt_parse_reserved_pads(int *reserved_pads)
{
	u32 sda_pad;
	u32 scl_pad;
	int rsv = 0;
	struct device_node *dn;

	if (!reserved_pads)
		return -EINVAL;

	/* obtain pads that are used by swi2c */
	for_each_compatible_node(dn, NULL, "mstar,swi2c")
	{
		if (rsv >= GPIO_MAX_RESERVED_PIN_NUMBER)
			break;

		if (0 != of_property_read_u32(dn, "sda-gpio", &sda_pad) ||
			0 != of_property_read_u32(dn, "scl-gpio", &scl_pad) )
		{
			GPIO_PRINT("failed to obtain pads reserved for swi2c\n");
			continue;
		}

		reserved_pads[rsv] = sda_pad;
		reserved_pads[rsv+1] = scl_pad;
		rsv += 2;
	}
	return 0;

}

#if defined (CONFIG_ARM64) && defined(CONFIG_OF)
static struct of_device_id mstar_gpio_of_device_ids[] = {
		{.compatible = "mstar,gpio"},
		{},
};
#endif

static int mstar_gpio_drv_probe(struct platform_device *pdev)
{
	int retval = 0;
	int pad;
	int ngpio;
	int reserved_pads[GPIO_MAX_RESERVED_PIN_NUMBER];
	struct mstar_gpio_dev *gpio_dev;
	const struct of_device_id *match;

	GPIO_PRINT("gpio probe is called\n");
	match = of_match_device(mstar_gpio_of_device_ids, &pdev->dev);
	if (!match)
	{
		dev_err(&pdev->dev, "Failed to find gpio controller\n");
		return -ENODEV;
	}

	if ( !(pdev->name) || strcmp(pdev->name,"Mstar-gpio")
		|| pdev->id!=0)
	{
		retval = -ENXIO;
	}

	gpio_dev = devm_kzalloc(&pdev->dev, sizeof(struct mstar_gpio_dev), GFP_KERNEL);
	if (!gpio_dev)
	{
		dev_err(&pdev->dev, "failed to alloc memory for gpio!\n");
		return -ENOMEM;
	}

	/* get gpio allocated irq count, INT_COUNT is statically allocated by hal layer */
	gpio_dev->eint_num = INT_COUNT;
	if (gpio_dev->eint_num == 0)
	{
		dev_err(&pdev->dev, "Couldn't determine # GPIO Pins uses IRQ\n");
		return -ENOENT;
	}

	ngpio = MHal_GPIO_Get_Pins_Count();
	BUG_ON(ngpio <= 0);

	gpio_dev->pdev = pdev;
	gpio_dev->ngpio = ngpio;
	gpio_dev->pin_status = vmalloc(sizeof(u32) * ngpio * 2);//oen and out
	if (!gpio_dev->pin_status)
	{
		dev_err(&pdev->dev, "failed to alloc memory for gpio!\n");
		return -ENOMEM;
	}

	retval = mstar_gpio_dt_parse_reserved_pads(reserved_pads);
	if(retval < 0)
	{
		dev_err(&pdev->dev, "failed to obtain GPIO reserved pads\n");
	}

	platform_set_drvdata(pdev, gpio_dev);
	mstar_gpio_chip.of_node = pdev->dev.of_node;
	mstar_gpio_chip.ngpio = ngpio;
	gpio_dev->irq_domain = irq_domain_add_tree(pdev->dev.of_node,
							&irq_domain_simple_ops,
							gpio_dev);
	if (!gpio_dev->irq_domain)
	{
		dev_err(&pdev->dev, "Couldn't allocate IRQ domain\n");
		retval = -ENXIO;
		goto err_irq_domain;
	}
	/* create irq mapping */
	for (pad= 0; pad< gpio_dev->eint_num; pad++)
	{
		int irq_pad = (int)gpio_IntPad[pad];
		irq_create_mapping(gpio_dev->irq_domain, irq_pad);
		/*let arm gic handle the polarity and mask*/
	}
	gpiochip_add(&mstar_gpio_chip);

	spin_lock_init(&gpio_dev->lock);

	/* export all pins except reserved to sysfs */
	for (pad = 0; pad < ngpio; pad++)
	{
		int rsv;
		/* linear search takes long time if GPIO_MAX_RESERVED_PIN_NUMBER is very large,
		consider refine the implementation if the number is larger than 50 */
		for (rsv = 0; rsv < GPIO_MAX_RESERVED_PIN_NUMBER; rsv++)
		{
			if (reserved_pads[rsv] == pad)
				break;
		}
		if(rsv < GPIO_MAX_RESERVED_PIN_NUMBER)
			continue;

		retval = gpio_request(pad, "sysfs");
		if (retval < 0)
			continue;
		retval = gpio_export(pad, 1);
		//WARN_ON(retval < 0);
		//GPIO_PRINT("GPIO export to sysfs:%d ret %d\n", i, retval);
	}
#if 0
	mstar_gpio_test_irq();
#endif
	return 0;

err_irq_domain:
	irq_domain_remove(gpio_dev->irq_domain);

	return retval;
}

static int mstar_gpio_drv_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct mstar_gpio_dev *gpio_dev;
	u8 pin_cnt = 0;
	int ret = 0;

	GPIO_PRINT("%s is invoked\n", __FUNCTION__);

	gpio_dev = (struct mstar_gpio_dev *)platform_get_drvdata(pdev);
	/* store gpio pins status */
	BUG_ON(gpio_dev->pin_status == NULL);

	ret = MHal_GPIO_Get_Pin_Status_Array(gpio_dev->pin_status, gpio_dev->ngpio, &pin_cnt );
	BUG_ON(gpio_dev->pin_status == 0);

	return ret;
}

static int mstar_gpio_drv_resume(struct platform_device *pdev)
{
	struct mstar_gpio_dev *gpio_dev;
	u8 pin_cnt = 0;
	int ret;

	GPIO_PRINT("%s is invoked\n", __FUNCTION__);

	gpio_dev = (struct mstar_gpio_dev *)platform_get_drvdata(pdev);

	BUG_ON(gpio_dev->pin_status == NULL);

	/* restore gpio pins status */
	ret = MHal_GPIO_Set_Pin_Status_Array(gpio_dev->pin_status, gpio_dev->ngpio, &pin_cnt );
	BUG_ON(gpio_dev->pin_status == 0);

	/* TODO: re-expose pins to sysfs? */
	return 0;
}

static int mstar_gpio_drv_remove(struct platform_device *pdev)
{
	if( !(pdev->name) || strcmp(pdev->name,"Mstar-gpio")
		|| pdev->id!=0)
	{
		return -1;
	}
	pdev->dev.platform_data = NULL;
	return 0;
}

static struct platform_driver Mstar_gpio_driver = {
	.probe			= mstar_gpio_drv_probe,
	.remove 		= mstar_gpio_drv_remove,
	.suspend		= mstar_gpio_drv_suspend,
	.resume 		= mstar_gpio_drv_resume,

	.driver = {
#if defined(CONFIG_ARM64)
		.of_match_table = of_match_ptr(mstar_gpio_of_device_ids),
#endif
		.name 	= "Mstar-gpio",
		.owner 	= THIS_MODULE,
	}
};

static int __init mstar_gpio_drv_init_module(void)
{
	return platform_driver_register(&Mstar_gpio_driver);
}

static void __exit mstar_gpio_drv_exit_module(void)
{
	platform_driver_unregister(&Mstar_gpio_driver);
}

postcore_initcall(mstar_gpio_drv_init_module);
module_exit(mstar_gpio_drv_exit_module);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("GPIO driver");
MODULE_LICENSE("GPL");

