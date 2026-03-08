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
#ifndef DRV_USBHOST_CONFIG_H
#define DRV_USBHOST_CONFIG_H

#include <MsIRQ.h>

//#define URB_TIMEOUT_BY_WAIT_EVENT
//#define TIMER_NO_ALL_INT_DISABLE

#define ROOTHUB_INTERRUPT_MODE
#define ROOTHUB_TIMER_INTERVAL  500

#define NUM_OF_ROOT_HUB 4

#define URANUS4_SERIAL_USE  //U4, K1
////#define USE_PORT2_AS_PORT1  //U4, K1
//#define ENABLE_PORT1_DPDM_SWAP //U4 only
//#define ENABLE_PIPE_FLUSH
#define ENABLE_LEGACY_CACHE_SETTING

//#define CPU_TYPE_AEON
#define CPU_TYPE_MIPS

#if defined(CPU_TYPE_AEON)
#define OS_BASE_ADDR        0xa0000000
#elif defined(CPU_TYPE_MIPS) // U4, K1, K2
    #if defined(TITANIA2_SERIAL_USE) 
    #define OS_BASE_ADDR    0xbf800000
    #else
    #define OS_BASE_ADDR    0xbf200000
    #endif
    #define MIPS_L1_CACHE_SIZE 32
#else
#No CPU type for USB
#endif

#ifdef ENABLE_LEGACY_CACHE_SETTING
#define CPU_L1_CACHE_BOUND (15)
#else
#define CPU_L1_CACHE_BOUND (MIPS_L1_CACHE_SIZE-1)
#endif

#define KAISERIN_CHIP_TOP_BASE (OS_BASE_ADDR+(0x1E00*2))

#ifdef URANUS4_SERIAL_USE
#define BASE_UTMI0          (OS_BASE_ADDR+(0x3A80*2))
#define BASE_UHC0           (OS_BASE_ADDR+(0x2400*2))
#define BASE_USBC0          (OS_BASE_ADDR+(0x0700*2))
#define E_IRQ_UHC          (E_INT_IRQ_UHC)
#define E_IRQ_USBC          (E_INT_IRQ_USB)

#define BASE_UTMI1          (OS_BASE_ADDR+(0x3A00*2))
#define BASE_UHC1           (OS_BASE_ADDR+(0x0D00*2))
#define BASE_USBC1          (OS_BASE_ADDR+(0x0780*2))
#define E_IRQ_UHC1          (E_INT_IRQ_UHC1)
#define E_IRQ_USBC1          (E_INT_IRQ_USB1)

#define BASE_UTMI2          (OS_BASE_ADDR+(0x2A00*2))
#define BASE_UHC2           (OS_BASE_ADDR+(0x10300*2))
#define BASE_USBC2          (OS_BASE_ADDR+(0x10200*2))
#define E_IRQ_UHC2          (E_INT_IRQ_UHC2)
#define E_IRQ_USBC2          (E_INT_IRQ_USB2)
#endif

#define BASE_UTMI3      (OS_BASE_ADDR+(0x20A00*2))
#define BASE_UHC3          (OS_BASE_ADDR+(0x20900*2))
#define BASE_USBC3          (OS_BASE_ADDR+(0x20800*2))
#define E_IRQ_UHC3         (E_INT_IRQ_UHC3)
#define E_IRQ_USBC3          (E_INT_IRQ_USB3)


//#define USB_PTP_ENABLE
//#define USB2_PTP_ENABLE

//#define USB_HID_ENABLE

extern MS_U32 gBaseUTMI, gBaseUSBC, gBaseUHC;
extern MS_U32 gBaseUTMI2, gBaseUSBC2, gBaseUHC2;
extern MS_U32 gIrqUHC, gIrqUHC2;

#endif

