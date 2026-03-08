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


#include <MsCommon.h>

#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include  "include/drvTimer.h"
#include  "include/drvList.h"


#include "include/drvUSB.h"
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void usb_show_endpoint(struct usb_host_endpoint *endpoint)
{
  usb_show_endpoint_descriptor(&endpoint->desc);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void usb_show_interface(struct usb_host_interface *altsetting)
{
  int i;

  usb_show_interface_descriptor(&altsetting->desc);

  for (i = 0; i < altsetting->desc.bNumEndpoints; i++)
    usb_show_endpoint(altsetting->endpoint + i);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void usb_show_config(struct usb_host_config *config)
{
  int ii, jj;
  struct usb_interface *ifp;

  usb_show_config_descriptor(&config->desc);
  for (ii = 0; ii < config->desc.bNumInterfaces; ii++) {
    ifp = config->interface[ii];

    if (!ifp)
      break;

    printk("\nInterface: %d\n", ii);
    for (jj = 0; jj < ifp->num_altsetting; jj++)
      usb_show_interface(ifp->altsetting + jj);
  }
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_device(struct usb_device *dev)
{
  int i;

  usb_show_device_descriptor(&dev->descriptor);
  if (dev->descriptor.bNumConfigurations > 0) {
    for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
      usb_show_config(dev->config + i);
  }
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_device_descriptor(struct usb_device_descriptor *desc)
{
  if (!desc)
  {
    printk("desc is NULL\n");
    return;
  }
  printk("  Len = %d\n", desc->bLength);
  printk("  DescType = %x\n", desc->bDescriptorType);

  printk("  Ver = %x.%02x\n", desc->bcdUSB >> 8, desc->bcdUSB & 0xff);
  printk("  VID:PID      = %04x:%04x\n", desc->idVendor, desc->idProduct);
  printk("  MaxPktSize0      = %d\n", desc->bMaxPacketSize0);
  printk("  NumConfig   = %d\n", desc->bNumConfigurations);
  printk("  Dev Ver      = %x.%02x\n", desc->bcdDevice >> 8, desc->bcdDevice & 0xff);

  printk("  Dev Class:SubClass:Protocol = %02x:%02x:%02x\n",
    desc->bDeviceClass, desc->bDeviceSubClass, desc->bDeviceProtocol);
  switch (desc->bDeviceClass) {
  case 0:
    printk("    Per-interface classes\n");
    break;
  case USB_CLASS_AUDIO:
    printk("    Audio class\n");
    break;
  case USB_CLASS_COMM:
    printk("    Communications class\n");
    break;
  case USB_CLASS_HID:
    printk("    HID class\n");
    break;
  case USB_CLASS_PRINTER:
    printk("    Printer class\n");
    break;
  case USB_CLASS_STORAGE:
    printk("    MSD class\n");
    break;
  case USB_CLASS_HUB:
    printk("    Hub class\n");
    break;
  case USB_CLASS_VENDOR_SPEC:
    printk("    Vendor device Class\n");
    break;
  default:
    printk("    Unknown device Class\n");
  }
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_config_descriptor(struct usb_config_descriptor *desc)
{
  printk("Config Desc:\n");
  printk("Len = %d\n",desc->bLength);
  printk("DescType = %x\n", desc->bDescriptorType);
  printk("TotalLen = %x\n", desc->wTotalLength);
  printk("Interfaces = %x\n", desc->bNumInterfaces);
  printk("ConfigValue = %x\n", desc->bConfigurationValue);
  printk("Config = %x\n", desc->iConfiguration);
  printk("Attr = %x\n", desc->bmAttributes);
  printk("MaxPower = %d\n", desc->bMaxPower * 2);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_interface_descriptor(struct usb_interface_descriptor *desc)
{
  printk("Alt Setting: %d\n", desc->bAlternateSetting);
  printk("Len = %d\n", desc->bLength);
  printk("DescType = %x\n", desc->bDescriptorType);
  printk("Interface = %x\n", desc->bInterfaceNumber);
  printk("AltSetting = %x\n", desc->bAlternateSetting);
  printk("NumEpts = %x\n", desc->bNumEndpoints);
  printk("Interface Class:SubClass:Protocol =   %x:%x:%x\n",
    desc->bInterfaceClass, desc->bInterfaceSubClass, desc->bInterfaceProtocol);
  printk("Interface = %x\n", desc->iInterface);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_endpoint_descriptor(struct usb_endpoint_descriptor *desc)
{
  printk("Endpoint:\n");
  printk("Len = %d\n", desc->bLength);
  printk("DescType = %x\n", desc->bDescriptorType);
  printk("EptAddress = %x\n", desc->bEndpointAddress);
  printk("Attributes = %x\n", desc->bmAttributes);
  printk("MaxPktSize = %x\n", desc->wMaxPacketSize);
  printk("Interval = %x\n", desc->bInterval);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_string(struct usb_device *dev, char *id, int index)
{
#if 0
  char *buf;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!index)
    return;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!(buf = (char*) kmalloc(256, GFP_KERNEL)))
    return;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (usb_string(dev, index, buf, 256) > 0)
    {
       dev_printk(KERN_INFO, &dev->dev, "%s: %s\n", id, buf);
    }
  kfree(buf);
#endif
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_dump_urb (struct urb *urb)
{
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("urb                   :%p\n", urb);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("dev                   :%p\n", urb->dev);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("pipe                  :%08X\n", urb->pipe);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("status                :%d\n", urb->status);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("transfer_flags        :%08X\n", urb->transfer_flags);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("transfer_buffer       :%p\n", urb->transfer_buffer);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("transfer_buffer_length:%d\n", urb->transfer_buffer_length);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("actual_length         :%d\n", urb->actual_length);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("setup_packet          :%p\n", urb->setup_packet);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("start_frame           :%d\n", urb->start_frame);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("number_of_packets     :%d\n", urb->number_of_packets);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("interval              :%d\n", urb->interval);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("error_count           :%d\n", urb->error_count);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("context               :%p\n", urb->context);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("complete              :%p\n", urb->complete);
}

