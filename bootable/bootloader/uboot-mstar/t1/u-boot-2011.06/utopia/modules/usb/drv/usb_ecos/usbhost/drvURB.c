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

#include <MsCommon.h>

#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include "include/drvTimer.h"
#include  "include/drvList.h"

#ifdef CONFIG_USB_DEBUG
  #define DEBUG
#else
  #undef DEBUG
#endif


// USB related header files
#include "include/drvUSB.h"
#include "drvHCD.h"


/*-------------------------------------------------------------------*/
void usb_init_urb(struct urb *m_urb)
{
  if (m_urb) {
    memset(m_urb, 0, sizeof(*m_urb));
    m_urb->count = (atomic_t)ATOMIC_INIT(1);
    spin_lock_init(&m_urb->lock);
  }
}

/*-------------------------------------------------------------------*/
struct urb *usb_alloc_urb(int iso_packets, int mem_flags)
{
  struct urb *urb;

  urb = (struct urb *)kmalloc(sizeof(struct urb) +
    iso_packets * sizeof(struct usb_iso_packet_descriptor),
    mem_flags);
  if (!urb) {
    usb_err("alloc_urb: kmalloc failed%s","");
    return NULL;
  }
  usb_init_urb(urb);
  return urb;
}

/*-------------------------------------------------------------------*/

void usb_free_urb(struct urb *m_urb)
{
  if (m_urb)
    if (atomic_dec_and_test(&m_urb->count))
      kfree(m_urb);
}

/*-------------------------------------------------------------------*/

struct urb * usb_get_urb(struct urb *urb)
{
  if (urb) {
    atomic_inc(&urb->count);
    return urb;
  } else
    return NULL;
}


/*-------------------------------------------------------------------*/

int usb_submit_urb(struct urb *m_urb, int mem_flags)
{
  int      pipe, temp_p, max_p;
  struct usb_device  *dev;
  struct usb_operations  *op;
  int      is_out;

  if (!m_urb || m_urb->hcpriv || !m_urb->complete)
    return -EINVAL;
  if (!(dev = m_urb->dev) ||
      (dev->state < USB_STATE_DEFAULT) ||
      (!dev->bus) || (dev->devnum <= 0))
    return -ENODEV;
  if (!(op = dev->bus->op) || !op->submit_urb)
    return -ENODEV;
  
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  
  

  if (mem_flags == ASYNC_SUBMIT_QTD)
     return op->submit_urb (m_urb, mem_flags);

  m_urb->status = -EINPROGRESS;
  m_urb->actual_length = 0;
  m_urb->bandwidth = 0;

  /* Lots of sanity checks, so HCDs can rely on clean data
   * and don't need to duplicate tests
   */
  pipe = m_urb->pipe;
  temp_p = usb_pipetype (pipe);
  is_out = usb_pipeout (pipe);
  
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  if (!usb_pipecontrol (pipe) && dev->state < USB_STATE_CONFIGURED)
    return -ENODEV;

  if (usb_endpoint_halted (dev, usb_pipeendpoint (pipe), is_out))
    return -EPIPE;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  max_p = usb_maxpacket (dev, pipe, is_out);
  if (max_p <= 0) {
    dbg ("%s: bogus endpoint %d-%s on usb-%s-%s (bad maxpacket %d)",
      __FUNCTION__,
      usb_pipeendpoint (pipe), is_out ? "OUT" : "IN",
      dev->bus->bus_name, dev->devpath,
      max_p);
    return -EMSGSIZE;
  }
  
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  if (temp_p == PIPE_ISOCHRONOUS) {
    int  n, len;

    /* "high bandwidth" mode, 1-3 packets/uframe? */
    if (dev->speed == USB_SPEED_HIGH) {
      int  mult = 1 + ((max_p >> 11) & 0x03);
      max_p &= 0x03ff;
      max_p *= mult;
    }
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

    if (m_urb->number_of_packets <= 0)
      return -EINVAL;
    for (n = 0; n < m_urb->number_of_packets; n++) {
      len = m_urb->iso_frame_desc [n].length;
      if (len < 0 || len > max_p)
        return -EMSGSIZE;
      m_urb->iso_frame_desc [n].status = (U32) -EXDEV;
      m_urb->iso_frame_desc [n].actual_length = 0;
    }
  }

  /* the I/O buffer must be mapped/unmapped, except when length=0 */
  if (m_urb->transfer_buffer_length < 0)
    return -EMSGSIZE;

#ifdef DEBUG
  {
  U32  orig_flags = m_urb->transfer_flags;
  U32  allowed;

  /* enforce simple/standard policy */
  allowed = URB_ASYNC_UNLINK;  // affects later unlinks
  allowed |= (URB_NO_TRANSFER_DMA_MAP | URB_NO_SETUP_DMA_MAP);
  allowed |= URB_NO_INTERRUPT;
  
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  
  switch (temp_p) {
	  case PIPE_BULK:
	    if (is_out)
	      allowed |= URB_ZERO_PACKET;
	    /* FALLTHROUGH */
	  case PIPE_CONTROL:
	    allowed |= URB_NO_FSBR;  /* only affects UHCI */
	    /* FALLTHROUGH */
	  default:      /* all non-iso endpoints */
	    if (!is_out)
	      allowed |= URB_SHORT_NOT_OK;
	    break;
	  case PIPE_ISOCHRONOUS:
	    allowed |= URB_ISO_ASAP;
	    break;
  }
  m_urb->transfer_flags &= allowed;

  /* fail if submitter gave bogus flags */
  if (m_urb->transfer_flags != orig_flags) {
    err ("BOGUS urb flags, %x --> %x",
      orig_flags, m_urb->transfer_flags);
    return -EINVAL;
  }
  }
#endif
  switch (temp_p) {
  case PIPE_ISOCHRONOUS:
  case PIPE_INTERRUPT:
    if (m_urb->interval <= 0)
      return -EINVAL;
    switch (dev->speed) {
    case USB_SPEED_HIGH:  /* units are microframes */
      // NOTE usb handles 2^15
      if (m_urb->interval > (1024 * 8))
        m_urb->interval = 1024 * 8;
      temp_p = 1024 * 8;
      break;
    case USB_SPEED_FULL:  /* units are frames/msec */
    case USB_SPEED_LOW:
      if (temp_p == PIPE_INTERRUPT) {
        // PIPE_INTERRUPT
        if (m_urb->interval > 255)
          return -EINVAL;
        // NOTE ohci only handles up to 32
        temp_p = 128;
      } else {
        // PIPE_ISOCHRONOUS
        if (m_urb->interval > 1024)
          m_urb->interval = 1024;
        // NOTE usb and ohci handle up to 2^15
        temp_p = 1024;
      }
      break;
    default:
      return -EINVAL;
    }
    while (temp_p > m_urb->interval)
      temp_p >>= 1;
    m_urb->interval = temp_p;
  }

  return op->submit_urb (m_urb, mem_flags);
}

/*-------------------------------------------------------------------*/

int usb_unlink_urb(struct urb *m_urb)
{
  if (m_urb && m_urb->dev && m_urb->dev->bus && m_urb->dev->bus->op)
    return m_urb->dev->bus->op->unlink_urb(m_urb);
  else
    return -ENODEV;
}

