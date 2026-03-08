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
#if 0
//#define ATV_SERISE_USE
//#define NEPTUNE_SERIAL_USE
//#define CERAMAL_SERISE_USE
//#define ERIS_SERIAL_USE
//#define PLUTO_SERIAL_USE
#define TITANIA2_SERIAL_USE

#ifdef PLUTO_SERIAL_USE
#define USBHOST2PORT            //for PLUTO 2 port solution
#define USB_POWER_SAVING_MODE
#define UHC1_BASE    0x2400
#define UHC_BASE   0xb00
#define UTMIBaseAddrPort1  0x3a80
#define UTMIBaseAddr     0x3ac0
#define USBCBasePort1                   0x700
#define USBCBase                            0xC00
#endif
#ifdef TITANIA2_SERIAL_USE
#define USBHOST2PORT            //for Titania2 2 port solution
#define USB_POWER_SAVING_MODE
#define UHC1_BASE    0x2400
#define UHC_BASE   0xd00
#define UTMIBaseAddrPort1     0x3a80
#define UTMIBaseAddr               0x3a00
#define USBCBasePort1               0x700
#define USBCBase                      0x780

#endif

#ifdef TRITON_SERIAL_USE
#define UHC_BASE    0x2e00
#define UTMIBaseAddr     0x3100
#endif

#ifdef NEPTUNE_SERIAL_USE
#define UHC_BASE    0x2400
#define UTMIBaseAddr     0x3a80
#endif

#ifdef  CERAMAL_SERISE_USE
#define UHC_BASE    0x2400
#define UTMIBaseAddr     0x1f00
#define OnePort_OTG_EHCI
#define Process_018_USE
#endif

#ifdef  ATV_SERISE_USE
#define UHC_BASE    0x2400
#define UTMIBaseAddr     0x3a80
#endif

#ifdef ERIS_SERIAL_USE
#define UHC_BASE         0xB00
#define UTMIBaseAddr     0x3AC0
#endif
#endif

//#define ENABLE_FORCE_FS
//#define USE_XDATA_ADDRESS_0XF000
#define DTV_STANDARD_LIB

#define Enable_Low_Temperature_Patch

