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

#ifndef __MS_USB_CH9_H
#define __MS_USB_CH9_H

#include "MsTypes.h"

/*-------------------------------------------------------------------------*/

#define USB_DIR_OUT             0       /* to device */
#define USB_DIR_IN              0x80    /* to host */

#define USB_TYPE_MASK        (0x03 << 5) // pipe type
#define USB_TYPE_STANDARD    (0x00 << 5) // standard message
#define USB_TYPE_CLASS      (0x01 << 5)  // class specific message
#define USB_TYPE_VENDOR      (0x02 << 5) // vendor specific message
#define USB_TYPE_RESERVED    (0x03 << 5) // reserved

#define USB_RECIP_MASK              0x1f
#define USB_RECIP_DEVICE            0x00 // target device
#define USB_RECIP_INTERFACE         0x01 // target interface
#define USB_RECIP_ENDPOINT          0x02 // target endpoint
#define USB_RECIP_OTHER             0x03

#define USB_REQ_GET_STATUS          0x00 // control pipe standard command, get status
#define USB_REQ_CLEAR_FEATURE       0x01 // control pipe standard command, clear feature
#define USB_REQ_SET_FEATURE         0x03 // control pipe standard command, set feature
#define USB_REQ_SET_ADDRESS         0x05 // control pipe standard command, set address
#define USB_REQ_GET_DESCRIPTOR      0x06 // control pipe standard command, get descriptor
#define USB_REQ_SET_DESCRIPTOR      0x07 // control pipe standard command, set descriptor
#define USB_REQ_GET_CONFIGURATION   0x08 // control pipe standard command, get configuration
#define USB_REQ_SET_CONFIGURATION   0x09 // control pipe standard command, set configuration
#define USB_REQ_GET_INTERFACE       0x0A // control pipe standard command, get interface
#define USB_REQ_SET_INTERFACE       0x0B // control pipe standard command, set interface
#define USB_REQ_SYNCH_FRAME         0x0C // control pipe standard command, synchronize frame

struct usb_ctrlrequest {
  MS_U8 bRequestType;
  MS_U8 bRequest;
  MS_U16 wValue;
  MS_U16 wIndex;
  MS_U16 wLength;
} __attribute__((packed));

/*-------------------------------------------------------------------------*/

#define USB_DT_DEVICE               0x01
#define USB_DT_CONFIG               0x02
#define USB_DT_STRING               0x03
#define USB_DT_INTERFACE            0x04
#define USB_DT_ENDPOINT             0x05
#define USB_DT_DEVICE_QUALIFIER     0x06
#define USB_DT_OTHER_SPEED_CONFIG   0x07
#define USB_DT_INTERFACE_POWER      0x08
#define USB_DT_OTG                  0x09

struct usb_descriptor_header {
  MS_U8  bLength;
  MS_U8  bDescriptorType;
} __attribute__((packed));


/*-------------------------------------------------------------------------*/

/* USB_DT_DEVICE: Device descriptor */
struct usb_device_descriptor {
  MS_U8  bLength;
  MS_U8  bDescriptorType;

  MS_U16 bcdUSB;
  MS_U8  bDeviceClass;
  MS_U8  bDeviceSubClass;
  MS_U8  bDeviceProtocol;
  MS_U8  bMaxPacketSize0;
  MS_U16 idVendor;
  MS_U16 idProduct;
  MS_U16 bcdDevice;
  MS_U8  iManufacturer;
  MS_U8  iProduct;
  MS_U8  iSerialNumber;
  MS_U8  bNumConfigurations;
} __attribute__((packed));

#define USB_DT_DEVICE_SIZE    18

#define USB_CLASS_PER_INTERFACE     0 /* for DeviceClass */
#define USB_CLASS_AUDIO             1 // audio class
#define USB_CLASS_COMM              2 // comminication class
#define USB_CLASS_HID               3 // HID class
#define USB_CLASS_PHYSICAL          5
#define USB_CLASS_STILL_IMAGE       6 // still image class for DSC
#define USB_CLASS_PRINTER           7 // printer class
#define USB_CLASS_STORAGE           8 // supported class, mass storage
#define USB_CLASS_HUB               9 // supported class, hub class
#define USB_CLASS_CDC_DATA          0x0a
#define USB_CLASS_CSCID             0x0b  /* chip+ smart card */
#define USB_CLASS_CONTENT_SEC       0x0d  /* content security */
#define USB_CLASS_APP_SPEC          0xfe
#define USB_CLASS_VENDOR_SPEC       0xff // vendor used

/*-------------------------------------------------------------------------*/

struct usb_config_descriptor {
  MS_U8  bLength;
  MS_U8  bDescriptorType;

  MS_U16 wTotalLength;
  MS_U8  bNumInterfaces;
  MS_U8  bConfigurationValue;
  MS_U8  iConfiguration;
  MS_U8  bmAttributes;
  MS_U8  bMaxPower;
} __attribute__((packed));

#define USB_DT_CONFIG_SIZE    9

#define USB_CONFIG_ATT_ONE          (1 << 7)  /* must be set */
#define USB_CONFIG_ATT_SELFPOWER    (1 << 6)  /* self powered */
#define USB_CONFIG_ATT_WAKEUP       (1 << 5)  /* can wakeup */

/*-------------------------------------------------------------------------*/

/* USB_DT_STRING: String descriptor */
struct usb_string_descriptor {
  MS_U8  bLength;
  MS_U8  bDescriptorType;

  MS_U16 wData[1];    /* UTF-16LE encoded */
} __attribute__((packed));

/*-------------------------------------------------------------------------*/

/* USB_DT_INTERFACE: Interface descriptor */
struct usb_interface_descriptor {
  MS_U8  bLength;
  MS_U8  bDescriptorType;

  MS_U8  bInterfaceNumber;
  MS_U8  bAlternateSetting;
  MS_U8  bNumEndpoints;
  MS_U8  bInterfaceClass;
  MS_U8  bInterfaceSubClass;
  MS_U8  bInterfaceProtocol;
  MS_U8  iInterface;
} __attribute__((packed));

#define USB_DT_INTERFACE_SIZE    9

/*-------------------------------------------------------------------------*/

/* USB_DT_ENDPOINT: Endpoint descriptor */
struct usb_endpoint_descriptor {
  MS_U8  bLength;
  MS_U8  bDescriptorType;

  MS_U8  bEndpointAddress;
  MS_U8  bmAttributes;
  MS_U16 wMaxPacketSize;
  MS_U8  bInterval;

  MS_U8  bRefresh;
  MS_U8  bSynchAddress;
} __attribute__((packed));

#define USB_DT_ENDPOINT_SIZE    7
#define USB_DT_ENDPOINT_AUDIO_SIZE  9  /* Audio extension */

#define USB_ENDPOINT_NUMBER_MASK  0x0f  /* in bEndpointAddress */
#define USB_ENDPOINT_DIR_MASK    0x80

#define USB_ENDPOINT_XFERTYPE_MASK  0x03  /* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL  0
#define USB_ENDPOINT_XFER_ISOC    1
#define USB_ENDPOINT_XFER_BULK    2
#define USB_ENDPOINT_XFER_INT    3

/*-------------------------------------------------------------------------*/

enum usb_device_speed {
  USB_UNKNOWN_SPEED = 0,            /* enumerating */
  USB_LOW_SPEED, USB_FULL_SPEED,    /* usb 1.1 */
  USB_HIGH_SPEED                    /* usb 2.0 */
};

enum usb_device_state {
  USB_STATE_NOTATTACHED = 0,

  /* the chapter 9 device states */
  USB_STATE_ATTACHED,
  USB_STATE_POWERED,
  USB_STATE_DEFAULT,      /* limited function */
  USB_STATE_ADDRESS,
  USB_STATE_CONFIGURED,      /* most functions */

  USB_STATE_SUSPENDED

};

/*-------------------------------------------------------------------------*/
#endif
