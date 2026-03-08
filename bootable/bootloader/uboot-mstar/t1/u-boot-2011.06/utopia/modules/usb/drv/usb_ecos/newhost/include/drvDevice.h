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

#ifndef _Device_H_
#define _Device_H_

#include <MsCommon.h>
#include "drvOSAPI.h"
#include "drvTypes.h"
#include "drvList.h"
#include "drvPorts.h"

#define DEVICE_NAME_SIZE  24
#define BUS_ID_SIZE       24

struct device_s;

struct device_driver {
  char   name[DEVICE_NAME_SIZE];
  struct ms_bus_type    * bus;

  struct list_head  bus_list; // driver's bus's drivers list, added by Pliauo
  struct list_head  devices;  // driver's devices

  int  (*probe)  (struct device_s * dev);
  int  (*remove)  (struct device_s * dev);   
}; 

// FIXME:
struct device_s {

  struct list_head node;        // node in device's parent's children list
  struct list_head bus_list;    // node in device's bus's list
  struct list_head driver_list; // node in device's driver's list
  struct list_head children;    // list of child devices
  
  struct device_s   * parent;
  
  int   dev_refcnt;
  char  dev_name[DEVICE_NAME_SIZE]; 
  char  bus_id[BUS_ID_SIZE];        // position on parent bus 

  struct ms_bus_type  * bus_type;   // type of device bus
  struct device_driver *driver;     // this device allocated driver
  void   *driver_data;         

  cyg_uint64    *dma_mask;          // for support dma device

  void  (*release_func)(struct device_s * dev);
}; 

static __inline__ void
ms_dev_set_drvdata (struct device_s *dev, void *data)
{
  dev->driver_data = data;
}

static __inline__ void *
ms_dev_get_drvdata (struct device_s *dev)
{
  return dev->driver_data;
}

extern void ms_device_initialize(struct device_s *dev);

static __inline__ struct device_s *ms_get_device(struct device_s *dev_ptr)
{
  osapi_atomic_inc(&dev_ptr->dev_refcnt);
  return dev_ptr;
}

struct usb_device_id {  
  MS_U16    match_id_flags;
  
  MS_U16    idVendor;
  MS_U16    idProduct;
  MS_U16    bcdDevice_lo;
  MS_U16    bcdDevice_hi;
  
  MS_U8    bDeviceClass;
  MS_U8    bDeviceSubClass;
  MS_U8    bDeviceProtocol;
 
  MS_U8    bInterfaceClass;
  MS_U8    bInterfaceSubClass;
  MS_U8    bInterfaceProtocol;
  
  MS_U32  match_id_info;
};

struct ms_bus_type { 
  struct list_head  drivers_list;
  struct list_head  devices_list;

  int    (*match_func)(struct device_s * dev, struct device_driver * drv);    
};

#define USBDEV_MATCH_ID_VENDOR        0x0001
#define USBDEV_MATCH_ID_PRODUCT       0x0002
#define USBDEV_MATCH_ID_DEV_LO        0x0004
#define USBDEV_MATCH_ID_DEV_HI        0x0008
#define USBDEV_MATCH_ID_DEV_CLASS     0x0010
#define USBDEV_MATCH_ID_DEV_SUBCLASS  0x0020
#define USBDEV_MATCH_ID_DEV_PROTOCOL  0x0040
#define USBDEV_MATCH_ID_INT_CLASS     0x0080
#define USBDEV_MATCH_ID_INT_SUBCLASS  0x0100
#define USBDEV_MATCH_ID_INT_PROTOCOL  0x0200

extern int  ms_driver_register(struct device_driver * drv);
extern void ms_driver_unregister(struct device_driver * drv);
extern void ms_device_unregister(struct device_s * dev);
extern int  ms_device_add(struct device_s *dev);
extern int  ms_bus_add_device(struct device_s * dev);
extern int  ms_bus_add_driver(struct device_driver * drv);
extern void ms_bus_remove_driver(struct device_driver * drv);
extern void ms_bus_remove_device(struct device_s * dev);
#endif

