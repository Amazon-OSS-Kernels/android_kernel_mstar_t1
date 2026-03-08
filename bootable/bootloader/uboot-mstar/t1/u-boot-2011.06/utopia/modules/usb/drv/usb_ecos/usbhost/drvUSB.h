/**
* Copyright (c) 2006 â€“ 2018 MStar Semiconductor, Inc.
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
#ifndef _DRVUSB_H
#define _DRVUSB_H

#include "drvUsbcommon.h"

//
// Function support
//
#define USBC_IP_SUPPORT 1

#define USB_IF_EHSET_SUPPORT 1 // embeded host hisg speed electrical test procedure

#ifdef MS_NOSAPI
#define USB_CDC_SUPPORT 0 // don't support cdc in non-os case in this stage
#else
#define USB_CDC_SUPPORT 0 // 1 to turn on cdc support
#endif

#define USB_MASS_STORAGE_SUPPORT 1

#define USB_HID_SUPPORT                     0

//
// Parameter definition
//
#define USBC_OVER_CURRENT 1
#define USBC_NON_OVER_CURRENT 2

#define  USB_PLUG_IN	1
#define  USB_PLUG_OUT	2

#define USB_EVENT_DEV_TYPE_UNKNOW    0
#define USB_EVENT_DEV_TYPE_STOR         1
#define USB_EVENT_DEV_TYPE_HID            2
#define USB_EVENT_DEV_TYPE_CDC          3

#define USB_NOT_RESPONSE		 0xff
#define USB_INTERFACE_CLASS_NONE 0
#define USB_INTERFACE_CLASS_MSD  0x08
#define USB_INTERFACE_CLASS_HUB  0x09
#define USB_INTERFACE_CLASS_HID  0x03

//-----------------------------------------
// U4_series_usb_init flag:
// Use low word as flag
#define EHCFLAG_NONE             0x0
#define EHCFLAG_DPDM_SWAP        0x1
#define EHCFLAG_TESTPKG          0x2
#define EHCFLAG_DOUBLE_DATARATE  0x4
// Use high word as data
#define EHCFLAG_DDR_MASK     0xF0000000
#define EHCFLAG_DDR_x15      0x10000000 //480MHz x1.5
#define EHCFLAG_DDR_x18      0x20000000 //480MHz x1.8
//-----------------------------------------

#define HUB_STACK_SIZE  0x2000

typedef void ( *USBCallback ) (MS_U8 u8Event, MS_U32 u32Para, char *s);
// for MDrv_USB_RegisterCallBack()
//u8Event: 1=plug, 2=unplug¡K can extend later);
//u32Para: bit 0 ~ 3, device type (0: unknow, 1: Mass Storage, 2: HID)
//             bit 4 ~ 31, reserved
// for MDrv_OverCurrentDetect_RegisterCallBack()
//u8Event: 1=over current,¡K can extend later);
//u32Para: port number

extern USBCallback _DrvUSB_CBFun;

void MDrv_OverCurrentDetect_RegisterCallBack (USBCallback pCallbackFn, MS_U8 port_mask);
void MDrv_USB_RegisterCallBack (USBCallback pCallbackFn);
MS_BOOL MDrv_UsbBlockReadToMIU(MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockWriteFromMIU(MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockReadToMIUEx(MS_U8 uPort,MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockWriteFromMIUEx(MS_U8 uPort,MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);

//MS_BOOL MDrv_UsbDeviceConnect(struct usb_hcd *hcd);
//void MDrv_Usb_Init(void);
void MDrv_Usb_Init(
    mem_Alloc     pfn_Cachedmem_Alloc,
    mem_Free      pfn_Cachedmem_Free,
    mem_Alloc     pfn_NonCachedmem_Alloc,
    mem_Free      pfn_NonCachedmem_Free,
    mem_VA2PA     pfn_mem_VA2PA,
    mem_PA2VA     pfn_mem_PA2VA,
    mem_Cached2Noncached pfn_mem_Cached2Noncached,
    mem_NonCached2Cached pfn_mem_NonCached2Cached
);
MS_BOOL MDrv_USB_Port_Init(MS_U8 u8PortNum);
void MDrv_UsbClose(void);
MS_U32 MDrv_GetUsbBlockSize(MS_U8 lun);
MS_U32 MDrv_GetUsbBlockSizeEx(MS_U8 uPort, MS_U8 lun);
MS_U32 MDrv_GetUsbBlockNumEx(MS_U8 uPort, MS_U8 lun);

MS_U32 MDrv_GetUsbBlockNum(MS_U8 lun);
MS_U8 MDrv_UsbGetMaxLUNCount(void);
MS_U8 MDrv_UsbGetMaxLUNCountEx(MS_U8 uPort);
MS_U8 MDrv_GET_MASS_VALID_LUN(void);
MS_BOOL ChkUsbReady(void);
MS_BOOL ChkUsbReadyEx(MS_U8 uPort);
MS_BOOL MDrv_UsbIsLunConnected(MS_U8 uPort, MS_U8 lun);
MS_U8 MDrv_USB_MscLookupHostID(char *str);
char *MDrv_USB_MscLookupProductString(char *str);
#endif //#ifndef _DRVUART_H

