/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************

#undef DEBUG

#include <MsCommon.h>
#include "include/drvCompiler.h"
#include "include/drvConfig.h"
#include "include/drvDevice.h"
#include "include/drvKernel.h"
#include "include/drvList.h"
#include "drvBase.h"

void device_initialize(struct device_s *dev)
{
  INIT_LIST_HEAD(&dev->node);
  INIT_LIST_HEAD(&dev->children);
  INIT_LIST_HEAD(&dev->driver_list);
  INIT_LIST_HEAD(&dev->bus_list);
  atomic_set(&dev->refcount,1);
}

int device_add(struct device_s *dev)
{
  struct device_s * parent;

  if (!dev || !strlen(dev->bus_id))
    return -EINVAL;

  parent = dev->parent;

  pr_debug("DEV: registering device: ID = '%s', name = %s\n",
     dev->bus_id, dev->name);

  if (parent)
    list_add_tail(&dev->node,&parent->children);


  bus_add_device(dev);

  return ENOERR;
}


void device_del(struct device_s * dev)
{
  struct device_s * parent = dev->parent;

  if (parent)
    list_del_init(&dev->node);

  bus_remove_device(dev);

}

void device_unregister(struct device_s * dev)
{
  pr_debug("DEV: Unregistering device. ID = '%s', name = '%s'\n",
     dev->bus_id,dev->name);
  device_del(dev);
  //calling device registed release function to releaes all device's allocated resouces
  if(dev->release)
    dev->release(dev);
}

