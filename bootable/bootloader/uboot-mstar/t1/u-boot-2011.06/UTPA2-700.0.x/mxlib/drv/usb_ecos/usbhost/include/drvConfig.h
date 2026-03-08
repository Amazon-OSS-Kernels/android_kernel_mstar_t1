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


#ifndef _CONFIG_H_
#define _CONFIG_H_

// Define Hardware configuration constant macro

#define __LINUX_ARM_ARCH__ 4    //  Architecture 4

/* Mass Storage Class Driver related CONFIG macro definition*/
//#define CONFIG_USB_STORAGE_DEBUG
//#define CONFIG_USB_SCSI_DEBUG
/* =======================================================*/
/* Test application related CONFIG macro definitions */
#define CONFIG_MASS_STORAGE_TEST
//#define CONFIG_USB_PRINTER_TEST
//#define CONFIG_USB_POLLING_ONLY_TEST

/* =======================================================*/
/* Macros for Speed limitation */
//#define FORCE_FULL_SPEED
//#ifdef FORCE_FULL_SPEED
//#undef FORCE_HIGH_SPEED
//#else
//#define FORCE_HIGH_SPEED
//#endif
/* =======================================================*/
/* OTG application related macro definitions */
//#define CONFIG_OTG
//#define CONFIG_OPT_TESTING
//#define CONFIG_HOST_AP_BURNIN  //Do specific procedures then leave
#define OTG_SRP_DATA_PULSING
//#define OTG_SRP_VBUS_PULSING

// for FPGA host test mode
//#define CONFIG_HC_TEST

#ifdef CONFIG_OPT_TESTING
#undef  CONFIG_OTG
//#define CONFIG_OTG
#endif
/* =======================================================*/
//#define CONFIG_FARADAY_USBH100
//#define CONFIG_FARADAY_FOTG100
//#define CONFIG_FARADAY_FOTG200

//#ifdef CONFIG_FARADAY_USBH100
//#undef CONFIG_OTG
//#undef CONFIG_OPT_TESTING
//#undef CONFIG_FARADAY_FOTG100
//#undef CONFIG_FARADAY_FOTG200
//#endif

//#ifdef CONFIG_FARADAY_FOTG200
//#undef CONFIG_FARADAY_FOTG100
//#undef CONFIG_FARADAY_USBH100
//#endif

//#ifdef CONFIG_FARADAY_FOTG100
//#undef CONFIG_FARADAY_FOTG200
//#undef  CONFIG_FARADAY_USBH100
//#define CONFIG_FARADAY_USBH100
//#endif
/* ======================================================*/
/* Transfer types support macro definitions for USB 2.0 */

#ifdef CONFIG_FARADAY_FOTG200
//#define have_split_iso
#define have_iso_itd
#define have_fstn
#endif
// Frequecy of kernel timer interrupt
#define HZ                 1000
#undef  __ARMEB__                // For Little Endian (CPU is little endian)
#define __ARMEL__                // For Little Endian (CPU is little endian)
/* ======================================================*/
/* DEBUG fucniton macro definitions */
//#define SHOW_MSG
//#define DEBUG
//#define CONFIG_DEBUG
//#define CONFIG_USB_DEBUG
//#define VERBOSE_DEBUG
//#define KERNEL_DEBUG
#ifndef CONFIG_FARADAY_FOTG200
#ifdef VERBOSE_DEBUG
#define OHCI_VERBOSE_DEBUG  //Enable Verbose debug, dump more details
#endif
#else
#ifdef VERBOSE_DEBUG
#define EHCI_VERBOSE_DEBUG  //Enable Verbose debug, dump more details
#endif
#endif

#define __LITTLE_ENDIAN 1234

#endif

