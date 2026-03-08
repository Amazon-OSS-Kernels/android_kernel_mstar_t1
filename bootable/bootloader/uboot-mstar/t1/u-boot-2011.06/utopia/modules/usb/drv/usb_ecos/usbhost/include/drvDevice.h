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

#include "MsCommon.h"
#include "drvOSAPI.h"
#include "drvTypes.h"
#include "drvList.h"
#include "drvPorts.h"

#define DEVICE_NAME_SIZE  24
#define BUS_ID_SIZE       24

#ifndef U16
#define U16 MS_U16
#endif


struct device_s;

struct device_driver {
  char      name[DEVICE_NAME_SIZE];
  struct bus_type    * bus;

  struct list_head  bus_list; // driver's bus's drivers list, added by Pliauo
  struct list_head  devices;  // driver's devices

  int  (*probe)  (struct device_s * dev);
  int  (*remove)  (struct device_s * dev);
  void (*shutdown)  (struct device_s * dev);
  int  (*suspend)  (struct device_s * dev, U32 state, U32 level);
  int  (*resume)  (struct device_s * dev, U32 level);
}; 

// FIXME:
struct device_s {

  struct list_head node;        // node in device's parent's children list
  struct list_head bus_list;    // node in device's bus's list
  struct list_head driver_list; // Node in device's driver's list
  struct list_head children;    // List of child devices

  
  struct device_s   * parent;

  //only reserve refcount in kobj
  int   refcount;
  char  name[DEVICE_NAME_SIZE];  /* descriptive ascii string */
  char  bus_id[BUS_ID_SIZE];  /* position on parent bus */

  struct bus_type  * bus;    /* type of bus device is on */
  struct device_driver *driver;  /* which driver has allocated this device */
  void    *driver_data;  /* data private to the driver */

  unsigned char *saved_state;  /* saved device state */
  cyg_uint64    *dma_mask;  /* dma mask (if dma'able device) */

  void  (*release)(struct device_s * dev);
}; 

static __inline__ void
dev_set_drvdata (struct device_s *dev, void *data)
{
  dev->driver_data = data;
}

static __inline__ void *
dev_get_drvdata (struct device_s *dev)
{
  return dev->driver_data;
}

extern void device_initialize(struct device_s *dev);

/* debugging and troubleshooting/diagnostic helpers. */
#define dev_printk(level, dev, format, ...)  \
  printk(format, __VA_ARGS__)

#ifdef DEV_DBG
#define dev_dbg(dev, format, ...)    \
  dev_printk("KERN_DBG", dev , format , __VA_ARGS__)
#else
#define dev_dbg(dev, format, ...) do {} while (0)
#endif

#ifdef DEV_ERR
#define dev_err(dev, format, ...)    \
  dev_printk("KERN_ERR" , dev , format , __VA_ARGS__)
#else
#define	dev_err(dev, format, ...) do {} while (0)
#endif  

#ifdef DEV_INFO
#define dev_info(dev, format, ...)    \
  dev_printk("KERN_INFO" , dev , format , __VA_ARGS__)
#else
#define dev_info(dev, format, ...) do {} while (0)
#endif

#ifdef DEV_WARM
#define dev_warn(dev, format, ...)    \
  dev_printk("KERN_WARNING" , dev , format , __VA_ARGS__)
#else
#define dev_warn(dev, format, ...) do {} while (0)
#endif

//#define get_device(dev_ptr)         dev_ptr->refcount++;dev_ptr   // Just return the passed pointer back
static __inline__ struct device_s *get_device(struct device_s *dev_ptr)
{
  atomic_inc(&dev_ptr->refcount);
  return dev_ptr;
}
#define put_device(dev_ptr)


struct usb_device_id {
  /* which fields to match against? */
  U16    match_flags;

  /* Used for product specific matches; range is inclusive */
  U16    idVendor;
  U16    idProduct;
  U16    bcdDevice_lo;
  U16    bcdDevice_hi;

  /* Used for device class matches */
  unsigned char    bDeviceClass;
  unsigned char    bDeviceSubClass;
  unsigned char    bDeviceProtocol;

  /* Used for interface class matches */
  unsigned char    bInterfaceClass;
  unsigned char    bInterfaceSubClass;
  unsigned char    bInterfaceProtocol;

  /* not matched against */
  U32  driver_info;
};

struct bus_type {
  char  name[24];
  
  struct list_head  drivers_list;
  struct list_head  devices_list;

   int    (*match)(struct device_s * dev, struct device_driver * drv);
 
   int    (*hotplug) (struct device_s *dev, char **envp,
            int num_envp, char *buffer, int buffer_size);
};


/* Some useful macros to use to create struct usb_device_id */
#define USB_DEVICE_ID_MATCH_VENDOR        0x0001
#define USB_DEVICE_ID_MATCH_PRODUCT       0x0002
#define USB_DEVICE_ID_MATCH_DEV_LO        0x0004
#define USB_DEVICE_ID_MATCH_DEV_HI        0x0008
#define USB_DEVICE_ID_MATCH_DEV_CLASS     0x0010
#define USB_DEVICE_ID_MATCH_DEV_SUBCLASS  0x0020
#define USB_DEVICE_ID_MATCH_DEV_PROTOCOL  0x0040
#define USB_DEVICE_ID_MATCH_INT_CLASS     0x0080
#define USB_DEVICE_ID_MATCH_INT_SUBCLASS  0x0100
#define USB_DEVICE_ID_MATCH_INT_PROTOCOL  0x0200

extern int  driver_register(struct device_driver * drv);
extern void driver_unregister(struct device_driver * drv);
extern void device_unregister(struct device_s * dev);
extern int  device_add(struct device_s *dev);
extern int  bus_register(struct bus_type * bus);
extern int  bus_add_device(struct device_s * dev);
extern int  bus_add_driver(struct device_driver * drv);
extern void bus_remove_driver(struct device_driver * drv);
extern void bus_unregister(struct bus_type * bus);
extern void bus_remove_device(struct device_s * dev);
#endif

