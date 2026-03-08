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

#ifndef _CORE_HCD_H_
#define _CORE_HCD_H_

#include <MsCommon.h>
#include "include/drvUSBHost.h"
#include "drvHub.h"

#define  HCD_FS   0x0010
#define  HCD_HS   0x0020
#define HCD_MEM_POOLS  5

struct usb_hcd {

  struct usb_bus    self;
  int  hcd_flags;
  char    product_desc[24];  /* product/vendor string */

  struct timer_list  roothub_timer;
  struct urb		*hcd_status_urb;
  struct list_head  hcd_dev_list;

  int  (*ms_urb_enqueue) (struct usb_hcd *hcd, struct urb *urb, int mem_flags);
  int  (*ms_urb_dequeue) (struct usb_hcd *hcd, struct urb *urb);

#if 1
  // Check root hub registered
  unsigned        roothub_registered:1;
  // for the new root-hub polling mechanism.
  unsigned        hcd_poll_pending:1;
#endif

  int      ehci_irq;
  U32      *uhc_regs;
  struct device_s    *controller;
  struct ms_mem_pool    *pool [HCD_MEM_POOLS];

  int      state;
#if 1 // new to collect hcd related variables
    U32 host_id;
    //U32 pre_sts; // preserve the previous state (ehci_hub_status_data)
    //U32 pre_temp; // preserve the previous state (ehci_hub_control_EX)
    S32 USBWaitFlg; // OS wait flag
    U32 badDeviceCnt; // for Philips mp3 device
    BOOL isBadDevice; // backward compatible
    BOOL isRootHubPortReset; // backward compatible, may be omitted
    struct list_head *phub_event;
#endif
};

// define usb_hcd.state
#define  __ACTIVE    0x01
#define  __SLEEPY    0x02
#define  __SUSPEND    0x04
#define  __TRANSIENT    0x80

#define  HCD_STATE_HALT    0
#define  HCD_STATE_RUNNING  (__ACTIVE)
#define  HCD_STATE_READY    (__ACTIVE|__SLEEPY)
#define  HCD_STATE_QUIESCING  (__SUSPEND|__TRANSIENT|__ACTIVE)
#define  HCD_STATE_RESUMING  (__SUSPEND|__TRANSIENT)
#define  HCD_STATE_SUSPENDED  (__SUSPEND)

#define  HCD_IS_RUNNING(state) ((state) & __ACTIVE)
#define  HCD_IS_SUSPENDED(state) ((state) & __SUSPEND)

struct stPtRegs;

struct s_hcd_dev {
  struct list_head  dev_list;
  struct list_head  urb_list;

  void      *ep[32];
};

struct ms_usb_bus_ops {
  int (*ms_alloc_hcd_dev)(struct usb_device *);
  int (*ms_deallo_hcd_dev)(struct usb_device *);
  int (*ms_hcd_submit_urb) (struct urb *urb, int mem_flags);
  int (*ms_hcd_unlink_urb) (struct urb *urb);
  void *(*ms_alloc_hcd_buffer)(struct usb_bus *bus, size_t size,
      int mem_flags,
      dma_addr_t *dma);
  void (*ms_free_hcd_buffer)(struct usb_bus *bus, size_t size,
      void *addr, dma_addr_t dma);
  void (*ms_disable_ep)(struct usb_device *udev, int bEndpointAddress);
};

extern void ms_usb_hcd_giveback_urb (struct urb *urb, struct stPtRegs *regs);
extern void ms_usb_bus_init (struct usb_bus *bus);
extern void ms_rh_status_dequeue (struct usb_hcd *pHcd, struct urb *pUrb);

int ms_hcd_buffer_create (struct usb_hcd *hcd);
void ms_hcd_buffer_destroy (struct usb_hcd *hcd);

void *ms_hcd_buffer_alloc (struct usb_bus *bus, size_t size,
  int mem_flags, dma_addr_t *dma);
void ms_hcd_buffer_free (struct usb_bus *bus, size_t size,
  void *addr, dma_addr_t dma);

extern struct ms_usb_bus_ops ms_usb_hcd_operations;

extern int ms_usb_new_device(struct usb_device *dev, struct device_s *parent);
extern void ms_usb_choose_address(struct usb_device *dev);
extern void ms_usb_disconnect(struct usb_device **);

extern int ms_usb_get_config(struct usb_device *dev);
extern void ms_usb_destroy_config(struct usb_device *dev);
extern int ms_usb_set_address(struct usb_device *dev);

#define usb_snddefctrl(dev)    (((U32)EP_CONTROL << 30))
#define usb_rcvdefctrl(dev)    (((U32)EP_CONTROL << 30) | USB_DIR_IN)

/*-------------------------------------------------------------------------*/

#define ms_CalcBitTime(bytes)  (7 * 8 * bytes / 6)  /* with ¾ã¼Æ truncation */

#define NS_2_US(ns)  ((ns + 500L) / 1000L)

#define USB2_EHCI_DELAY  5       // (ns)
#define EHCI_BW_DELAY    1000L   // (ns)
#define HUB_BW_LS_SETUP  333L    // (ns)
#define HIGHSPEED_US(bytes) NS_2_US ( ((55 * 8 * 2083)/1000) \
  + ((2083UL * (3167 + ms_CalcBitTime (bytes)))/1000) \
  + USB2_EHCI_DELAY)

#define HIGHSPEED_US_ISO(bytes) NS_2_US ( ((long)(38 * 8 * 2.083)) \
  + ((2083UL * (3167 + ms_CalcBitTime (bytes)))/1000) \
  + USB2_EHCI_DELAY)

extern int ms_usb_calc_bus_time (int speed, int is_input, int bytes);

/*-------------------------------------------------------------------------*/

#define Req_SetHubFeature     (0x2000 | USB_REQ_SET_FEATURE)
#define Req_SetPortFeature    (0x2300 | USB_REQ_SET_FEATURE)
#define Req_GetHubDescriptor  (0xa000 | USB_REQ_GET_DESCRIPTOR)
#define Req_GetHubStatus      (0xa000 | USB_REQ_GET_STATUS)
#define Req_GetPortStatus     (0xa300 | USB_REQ_GET_STATUS)
#define Req_ClearHubFeature   (0x2000 | USB_REQ_CLEAR_FEATURE)
#define Req_ClearPortFeature  (0x2300 | USB_REQ_CLEAR_FEATURE)

#define ReqType_Device \
  ((USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_DEVICE)<<8)
#define ReqType_DeviceOut \
  ((USB_DIR_OUT|USB_TYPE_STANDARD|USB_RECIP_DEVICE)<<8)

#define ReqType_Interface \
  ((USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_INTERFACE)<<8)

#define ReqType_Endpoint \
  ((USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_INTERFACE)<<8)
#define ReqType_EndpointOut \
  ((USB_DIR_OUT|USB_TYPE_STANDARD|USB_RECIP_INTERFACE)<<8)

/*-------------------------------------------------------------------------*/
extern int ms_usb_register_bus (struct usb_bus *);
extern void ms_usb_deregister_bus (struct usb_bus *);
extern int ms_register_root_hub (struct usb_hcd *hcd);
static __inline__ int hcd_register_root (struct usb_hcd *hcd)
{
    return ms_register_root_hub(hcd);
}

extern struct list_head usb_bus_list;

#define usb_endpoint_halt(dev, ep, out) ((dev)->halted[out] |= (1 << (ep)))

#define usb_endpoint_dir(ep_dir)  (!((ep_dir) & USB_DIR_IN))

//#define bitmap   DeviceRemovable

#endif
