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
#ifndef DRV_USB_HOST_CONFIG_H
#define DRV_USB_HOST_CONFIG_H

//#define ATV_SERISE_USE
//#define NEPTUNE_SERIAL_USE
//#define CERAMAL_SERISE_USE
//#define PLUTO_SERIAL_USE
//#define TITANIA2_SERIAL_USE
#define TITANIA3_SERIAL_USE
//#define EUCLID_SERIAL_USE

#if defined (MCU_AEON)
#define CPU_TYPE_AEON
#elif defined (MCU_MIPS_4KE) || defined(MCU_MIPS_34K) || defined(MCU_MIPS_74K) || defined(MCU_MIPS_1004K)
#define CPU_TYPE_MIPS
#elif defined (__arm__) || defined (__aarch64__)
#define CPU_TYPE_ARM
#else
  #if defined (MIPS_CHAKRA)
  #define CPU_TYPE_MIPS
  #else
  #define CPU_TYPE_AEON
  #endif
#endif


#if defined(CPU_TYPE_AEON)
#define OS_BASE_ADDR        0xa0000000
#elif defined(CPU_TYPE_MIPS)
  #if defined(TITANIA3_SERIAL_USE) || defined(EUCLID_SERIAL_USE)
    #define OS_BASE_ADDR        0xbf000000
  #else
    #define OS_BASE_ADDR        0xbf800000
  #endif
#elif defined(CPU_TYPE_ARM)
    #define OS_BASE_ADDR        0xfd000000
#else
#No_CPU_type_for_USB
#endif

#ifdef PLUTO_SERIAL_USE
#define UHC_BASE    (OS_BASE_ADDR+0x4800)
#define UHC2_BASE   (OS_BASE_ADDR+0x1600)
#define UTMIBaseAddr      (OS_BASE_ADDR+0x7500)
#define UTMIBaseAddr2   (OS_BASE_ADDR+0x7580)
#define USBCBase            (OS_BASE_ADDR+0xe00)
#define USBCBase2           (OS_BASE_ADDR+0x1800)
#endif

#ifdef TITANIA2_SERIAL_USE
#define UHC_BASE    (OS_BASE_ADDR+0x4800)
#define UHC2_BASE   (OS_BASE_ADDR+0x1a00)
#define UTMIBaseAddr       (OS_BASE_ADDR+0x7500)
#define UTMIBaseAddr2   (OS_BASE_ADDR+0x7400)
#define USBCBase            (OS_BASE_ADDR+0xe00)
#define USBCBase2           (OS_BASE_ADDR+0xf00)
#endif

#ifdef EUCLID_SERIAL_USE
#define UHC_BASE            (OS_BASE_ADDR+0x204800)
#define UHC2_BASE           (OS_BASE_ADDR+0x201a00)
#define UTMIBaseAddr        (OS_BASE_ADDR+0x207500)
#define UTMIBaseAddr2       (OS_BASE_ADDR+0x207400)
#define USBCBase            (OS_BASE_ADDR+0x200e00)
#define USBCBase2           (OS_BASE_ADDR+0x200f00)
#endif

#ifdef TITANIA3_SERIAL_USE
#define UHC_BASE    (OS_BASE_ADDR+0x204800)
#define UHC2_BASE   (OS_BASE_ADDR+0x201a00)
#define UTMIBaseAddr       (OS_BASE_ADDR+0x207500)
#define UTMIBaseAddr2   (OS_BASE_ADDR+0x207400)
#define USBCBase            (OS_BASE_ADDR+0x200e00)
#define USBCBase2           (OS_BASE_ADDR+0x200f00)

#define UHC3_BASE   (OS_BASE_ADDR+0x227200)
#define UTMIBaseAddr3   (OS_BASE_ADDR+0x207200)
#define USBCBase3           (OS_BASE_ADDR+0x227000)

#define UHC4_BASE   (OS_BASE_ADDR+0x244c00)
#define UTMIBaseAddr4   (OS_BASE_ADDR+0x244100)
#define USBCBase4           (OS_BASE_ADDR+0x227100)
#endif

#ifdef TRITON_SERIAL_USE
#define UHC_BASE    0x2e00
#define UTMIBaseAddr     0x3100
#endif

#ifdef NEPTUNE_SERIAL_USE
#define UHC_BASE    (OS_BASE_ADDR+0x4800)
#define UTMIBaseAddr     (OS_BASE_ADDR+0x7500)
#endif

#if 0
#ifdef  CERAMAL_SERISE_USE
#define UHC_BASE    0x2400
#define UTMIBaseAddr     0x1f00
#define OnePort_OTG_EHCI
#define Process_018_USE
#endif
#endif

#ifdef  ATV_SERISE_USE
#define UHC_BASE     (OS_BASE_ADDR+0x4800)
#define UTMIBaseAddr     (OS_BASE_ADDR+0x7500)
#endif


//#define USB_PTP_ENABLE
//#define USB2_PTP_ENABLE
//#define USB3_PTP_ENABLE
//#define USB4_PTP_ENABLE

#define USB_HID_ENABLE
#define USB2_HID_ENABLE
//#define USB3_HID_ENABLE
//#define USB4_HID_ENABLE


#endif //#ifndef DRV_USB_HOST_CONFIG_H

