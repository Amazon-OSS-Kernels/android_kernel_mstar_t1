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
#include  "include/drvTypes.h"
#include  "include/drvIO.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include  "include/drvPCIMEM.h"
#include  "include/drvTimer.h"
#include  "include/drvList.h"
#include  "include/drvKernel.h"
#include  "include/drvBitops.h"

// USB related header files
#include "include/drvUSB.h"
#include "drvHCD.h"
#include "drvHUB.h"

extern void USBCriticalSectionIn(MS_U8 Port);
extern void USBCriticalSectionOut(MS_U8 Port);
extern int hub_port_status(struct usb_device *dev, int port,
             U16 *status, U16 *change);
extern int hub_reset(struct usb_hub *hub);
extern void hub_start_disconnect(struct usb_device *dev);
extern void hub_port_connect_change(struct usb_hub *hubstate, int port,
          U16 portstatus, U16 portchange);
extern int clear_port_feature(struct usb_device *dev, int port, int feature);
extern void hub_power_on(struct usb_hub *hub);
extern int hub_hub_status(struct usb_hub *hub,
            U16 *status, U16 *change);
extern int clear_hub_feature(struct usb_device *dev, int feature);

/*static*/
LIST_HEAD(hub_event_list_Port2);  /* List of hubs needing servicing */

BOOL has_hub_events_Port2(U16 *pPortStatus)
{
    //U16 portstatus;
    BOOL status;
    U16 portchange;
    struct list_head *tmp;
    struct usb_device *dev;
    struct usb_hub *hub;
    //int i, ret;

    USBCriticalSectionIn(2);
    if (list_empty(&hub_event_list_Port2))
    {
        status = FALSE;
    }
    else
    {
        U32 u32OldIntr;
        u32OldIntr = MsOS_DisableAllInterrupts();

        tmp = hub_event_list_Port2.next;
        hub = list_entry(tmp, struct usb_hub, event_list);
        dev = interface_to_usbdev(hub->intf);

        //MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
        MsOS_RestoreAllInterrupts(u32OldIntr);


        hub_port_status(dev, 0, pPortStatus, &portchange);
        //diag_printf("portstatus: %X\n", *pPortStatus);

        if (portchange & USB_PORT_STAT_C_CONNECTION)
            status = TRUE;
        else
            status = FALSE;
    }

    USBCriticalSectionOut(2);
    return status;
}

static void hub_events_Port2(void)
{
  U32 flags;
  struct list_head *tmp;
  struct usb_device *dev;
  struct usb_hub *hub;
  U16 hubstatus;
  U16 hubchange;
  U16 portstatus;
  U16 portchange;
  int i, ret;

  while (1) {
    spin_lock_irqsave(&hub_event_lock_Port2, flags);

    if (list_empty(&hub_event_list_Port2))
      break;

    /* Grab the next entry from the beginning of the list */
 // MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
     U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

    tmp = hub_event_list_Port2.next;

	hub = list_entry(tmp, struct usb_hub, event_list);
    //list_entry(tmp, struct usb_hub, event_list,struct list_head, hub);
    dev = interface_to_usbdev(hub->intf);

    list_del_init(tmp);
 // MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
      MsOS_RestoreAllInterrupts(u32OldIntr);

    //if (unlikely(down_trylock(&hub->khubd_sem)))
    //  BUG();  /* never blocks, we were on list */

    spin_unlock_irqrestore(&hub_event_lock_Port2, flags);

    if (hub->error) {
      dev_dbg (&hub->intf->dev, "resetting for error %d\n",
        hub->error);

      if (hub_reset(hub)) {
        dev_dbg (&hub->intf->dev,
          "can't reset; disconnecting%s","\n");
        up(&hub->khubd_sem);
        hub_start_disconnect(dev);
        continue;
      }

      hub->nerrors = 0;
      hub->error = 0;
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

    diag_printf("get hub event 2\n");
    for (i = 0; i < hub->descriptor.bNbrPorts; i++) {
      ret = hub_port_status(dev, i, &portstatus, &portchange);
      if (ret < 0) {
        continue;
      }
      if (portchange & USB_PORT_STAT_C_CONNECTION) {
        hub_port_connect_change(hub, i, portstatus, portchange);
      } else if (portchange & USB_PORT_STAT_C_ENABLE) {
        dev_dbg (hubdev (dev),
          "port %d enable change, status %x\n",
          i + 1, portstatus);
        clear_port_feature(dev,
          i + 1, USB_PORT_FEAT_C_ENABLE);

         
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
        if (!(portstatus & USB_PORT_STAT_ENABLE)
            && (portstatus & USB_PORT_STAT_CONNECTION)
            && (dev->children[i])) {
          dev_err (&hub->intf->dev,
              "port %i "
              "disabled by hub (EMI?), "
              "re-enabling...",
            i + 1);
          hub_port_connect_change(hub,
            i, portstatus, portchange);
        }
      }

      if (portchange & USB_PORT_STAT_C_SUSPEND) {
        dev_dbg (&hub->intf->dev,
          "suspend change on port %d\n",
          i + 1);
        clear_port_feature(dev,
          i + 1,  USB_PORT_FEAT_C_SUSPEND);
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

      if (portchange & USB_PORT_STAT_C_OVERCURRENT) {
        dev_err (&hub->intf->dev,
          "over-current change on port %d\n",
          i + 1);
        clear_port_feature(dev,
          i + 1, USB_PORT_FEAT_C_OVER_CURRENT);
        hub_power_on(hub);
      }

      if (portchange & USB_PORT_STAT_C_RESET) {
        dev_dbg (&hub->intf->dev,
          "reset change on port %d\n",
          i + 1);
        clear_port_feature(dev,
          i + 1, USB_PORT_FEAT_C_RESET);
      }
    } /* end for i */

    /* deal with hub status changes */
	
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
    if (hub_hub_status(hub, &hubstatus, &hubchange) < 0) {
      dev_err (&hub->intf->dev, "get_hub_status failed%s","\n");
    }
    else {
      if (hubchange & HUB_CHANGE_LOCAL_POWER) {
        dev_dbg (&hub->intf->dev, "power change%s","\n");
        clear_hub_feature(dev, C_HUB_LOCAL_POWER);
      }
      if (hubchange & HUB_CHANGE_OVERCURRENT) {
        dev_dbg (&hub->intf->dev, "overcurrent change%s","\n");
        wait_ms(500);  /* Cool down */
        clear_hub_feature(dev, C_HUB_OVER_CURRENT);
                          hub_power_on(hub);
      }
    }
    up(&hub->khubd_sem);
        } /* end while (1) */

  spin_unlock_irqrestore(&hub_event_lock_Port2, flags);
}

void hub_poll_Port2(void)
{
  // Disable Interrupts
  //cli();
  //vdbg("%s","polling");
  //check_usb_lock();
  USBCriticalSectionIn(2);
  hub_events_Port2();
  USBCriticalSectionOut(2);

  //vdbg("%s","polling ok!!");
  //unlock_usb_core();
  // Re-enable Interrupts
  //sti();
}

void hub_add_event_Port2(struct list_head *newent)
{
	list_add(newent, &hub_event_list_Port2);
}
