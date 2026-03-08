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
//Copyright (C) 2008 MStar  All Rights Reserved.

/*! \file  drvUSBHwCtl.h
	\brief  Functions for accessing the USB hardware.


 \n\b History:
	\n\b Date: 2008/11/20
	\n\b Author: Colin Tsai
	\n\b Reason: Create
	\n
*/
//=============================================================================

#ifndef DRV_USB_HWCTL_H
#define DRV_USB_HWCTL_H
#include "drvUsbHostConfig.h"

#define usb_readb(addr)    (*(volatile unsigned char *) (addr))
#define usb_readw(addr)    (*(volatile unsigned short *) (addr))
#define usb_readl(addr)    (*(volatile unsigned int *) (addr))
#define usb_writeb(b,addr) ((*(volatile unsigned char *) (addr)) = (b))
#define usb_writew(b,addr) ((*(volatile unsigned short *) (addr)) = (b))
#define usb_writel(b,addr) ((*(volatile unsigned int *) (addr)) = (b))

#define HOST20_USBCMD_FrameListSize_1024                  0x00
#define HOST20_USBCMD_FrameListSize_512                   0x01
#define HOST20_USBCMD_FrameListSize_256                   0x02

#define HOST20_Enable                  0x01
#define HOST20_Disable                 0x00

#define HOST20_USBEVENT_USB_CDC_CTRL                      0x100
#define HOST20_USBEVENT_USB_CDC_BUILK_IN                  0x80
#define HOST20_USBEVENT_USB_CDC_BUILK_OUT                 0x40
#define HOST20_USBINTR_IntOnAsyncAdvance                  0x20
#define HOST20_USBINTR_SystemError                        0x10
#define HOST20_USBINTR_FrameRollover                      0x08
#define HOST20_USBINTR_PortChangeDetect                   0x04
#define HOST20_USBINTR_USBError                           0x02
#define HOST20_USBINTR_CompletionOfTransaction            0x01
#define USBWAITEVENTS   (HOST20_USBINTR_CompletionOfTransaction|HOST20_USBINTR_USBError|HOST20_USBINTR_PortChangeDetect|HOST20_USBINTR_SystemError)

#define USBCINTR_IDChange                                 0x08
#define USBCINTR_BValidChange                             0x04
#define USBCINTR_AValidChange                             0x02
#define USBCINTR_VBusValidChange                          0x01

extern void UTMI_ORXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base);
extern void UTMI_ANDXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base);
extern void UTMI_SETXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base);
extern MS_U8 UTMI_READXBYTE_EX(MS_U8 offset, MS_U32 base);

//#define Enable_Issue_TestPacket
//#define Issue_TestPacket
//#define Enable_Burning_Test
//#define Enable_SOF_Only

#ifdef MS_NOSAPI
#define USB_MSEC_LOOP              ( CPU_CLOCK_FREQ/1000/4 )
#define USB_USEC_LOOP              ( CPU_CLOCK_FREQ / 1000 / 1000 / 4 )   //micro seconds
#define USB_DELAY(_loop)     { volatile int i; for (i=0; i<(_loop)*USB_MSEC_LOOP; i++); }
#define USB_DELAY_USEC(_loop)     { volatile int i; for (i=0; i<(_loop)*USB_USEC_LOOP; i++); }
#define USB_TIME(_stamp, _loop)    { _stamp = (_loop); }
#define USB_EXPIRE(_stamp)         ( !(--_stamp) )
#else
#define USB_DELAY(_msec)           MsOS_DelayTask(_msec)
#define USB_DELAY_USEC(_usec)           MsOS_DelayTaskUs(_usec)
#define USB_TIME(_stamp, _loop)    { _stamp = (_loop); }
#define USB_EXPIRE(_stamp)         ( !(--_stamp) )

#endif

#endif  //#define DRV_USB_HWCTL_H

