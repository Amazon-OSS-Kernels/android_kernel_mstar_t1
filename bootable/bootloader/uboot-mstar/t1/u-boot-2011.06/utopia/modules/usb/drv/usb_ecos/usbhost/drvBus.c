/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************

//#undef DEBUG

#include <MsCommon.h>
#include  "include/drvPorts.h"
#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"

#include "drvBase.h"


void device_bind_driver(struct device_s * dev)
{
	pr_debug("bound device '%s' to driver '%s'\n",
				dev->bus_id,dev->driver->name);
	list_add_tail(&dev->driver_list,&dev->driver->devices);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int bus_match(struct device_s * dev, struct device_driver * drv)
{
	int error = -ENODEV;

	// Call BUS provided match function to match device ID witch specific comparing method
	if (dev->bus->match(dev,drv)) {
		dev->driver = drv;
		// Match device ==> call driver's probe function
		if (drv->probe) {
			if ((error = drv->probe(dev))) {
				dev->driver = NULL;
				return error;
			}
		}
		device_bind_driver(dev);
		error = 0;
	}

	return error;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int device_attach(struct device_s * dev)
{
	struct bus_type * bus = dev->bus;
	struct list_head * entry;
	struct device_driver  * drv;
	struct list_head *_mtr;

	if (dev->driver) {
		device_bind_driver(dev);
		return 1;
	}

	if (bus->match) {
		list_for_each(entry,&bus->drivers_list) {
			_mtr = entry ;
			drv = (struct device_driver *)((char *)_mtr - (char *)offsetof(struct device_driver, bus_list) );
			if (!bus_match(dev,drv))
				return 1;
		}
	}

  return 0;
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void driver_attach(struct device_driver * drv)
{
	struct bus_type * bus = drv->bus;
	struct list_head * entry;
	struct device_s * dev;
	const struct list_head *_mtr;

	if (!bus->match)
		return;

	list_for_each(entry,&bus->devices_list) {
		_mtr = entry ;
		dev = (struct device_s *)((char *)_mtr - (char *)offsetof(struct device_s, bus_list) );
		if (!dev->driver) {
			bus_match(dev,drv);
		}
	}
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void device_release_driver(struct device_s * dev)
{
	struct device_driver * drv = dev->driver;

	if (drv) {
		list_del_init(&dev->driver_list);
		if (drv->remove)
			drv->remove(dev);
		dev->driver = NULL;
	}
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void driver_detach(struct device_driver * drv)
{
	struct list_head * entry, * next;
	struct device_s *dev;
	struct list_head *_mptr;

	list_for_each_safe(entry,next,&drv->devices) {
		// struct device * dev = container_of(entry,struct device,driver_list);
		_mptr = entry;
		dev = (struct device_s *)( (char *)_mptr - (char *)offsetof(struct device_s, driver_list) );
		device_release_driver(dev);
	}

}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int bus_add_device(struct device_s * dev)
{
	int error = 0;

	if (dev->bus) {
		pr_debug("bus %s: add device %s\n",dev->bus->name,dev->bus_id);
		list_add_tail(&dev->bus_list,&dev->bus->devices_list);
		device_attach(dev);
	}
	return error;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void bus_remove_device(struct device_s * dev)
{
	if (dev->bus) {
		pr_debug("bus %s: remove device %s\n",dev->bus->name,dev->bus_id);
		device_release_driver(dev);
		list_del_init(&dev->bus_list);
	}
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int bus_add_driver(struct device_driver * drv)
{
	struct bus_type * bus = drv->bus;
	int error = 0;

	if (bus) {
		pr_debug("bus %s: add driver %s\n",bus->name,drv->name);

		// Add driver to bus driver lsit
		list_add_tail(&drv->bus_list,&bus->drivers_list);
		driver_attach(drv);
	}
	return error;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void bus_remove_driver(struct device_driver * drv)
{
	if (drv->bus) {
		pr_debug("bus %s: remove driver %s\n",drv->bus->name,drv->name);
		driver_detach(drv);
		list_del_init(&drv->bus_list);
	}
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int bus_register(struct bus_type * bus)
{
	pr_debug("bus type '%s' registered\n",bus->name);
	return 0;
}



void bus_unregister(struct bus_type * bus)
{
	pr_debug("bus %s: unregistering\n",bus->name);
}

