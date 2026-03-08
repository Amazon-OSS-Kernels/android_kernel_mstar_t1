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

#ifndef _CORE_USB_H_
#define _CORE_USB_H_

#include <MsTypes.h>

/* Functions local to drivers/usb/core/ */
//extern int usb_probe_interface (struct device_s *dev);
//extern int usb_unbind_interface (struct device_s *dev);
extern void ms_usb_disable_endpoint (struct usb_device *pDev, U32 epaddr);
extern void ms_usb_disable_interface (struct usb_device *pDev,
		struct usb_interface *pIntf);
extern void ms_usb_disable_device (struct usb_device *pDev, int skip_ep0);
extern void ms_usb_enable_endpoint (struct usb_device *pdev,
		struct usb_endpoint_descriptor *pEpt_desc);
extern void ms_usb_enable_interface (struct usb_device *pDev,
		struct usb_interface *pIntf);

#endif

