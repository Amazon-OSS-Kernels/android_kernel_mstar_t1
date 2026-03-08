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
#ifndef DRV_USBHOST_CONFIG_H
#define DRV_USBHOST_CONFIG_H

#include <MsIRQ.h>
#include <drvUSB_eCos.h> // for root hub support only, applying the configuration

#define ECOS_USB_HOST_LOCAL_VER "20140910"

/* eCos USB Host driver exclusive default setting */
//#define URB_TIMEOUT_BY_WAIT_EVENT

//#define ROOTHUB_TIMER_INTERVAL  500

#define NUM_OF_ROOT_HUB 4

//#define ENABLE_LEGACY_CACHE_SETTING

#define ENABLE_DISCONNECT_FAST_RESPONSE

/* 20140630 quick response for file read/write root hub disconnected */
#define ENABLE_RW_DISCONNECTING

#define ENABLE_ROOTHUB_DISCONN_REINIT

/* 20140715 maximum hub top level to restric hub_probe */
#ifdef USB_NOT_SUPPORT_EX_HUB
#define MAX_HUB_TOPO_LEVEL 1
#else
#define MAX_HUB_TOPO_LEVEL 6
#endif

//#define CPU_TYPE_AEON
#if defined(CHIP_KAISER)
#define CPU_TYPE_ARM
#else
#define CPU_TYPE_MIPS
#endif

#if defined(CPU_TYPE_AEON)
#define OS_BASE_ADDR        0xa0000000
#elif defined(CPU_TYPE_MIPS) // U4, K1, K2
    #if defined(TITANIA2_SERIAL_USE)
    #define OS_BASE_ADDR    0xbf800000
    #else
    #define OS_BASE_ADDR    0xbf200000
    #endif
    #define MIPS_L1_CACHE_SIZE 32
#elif defined(CPU_TYPE_ARM) // K3
    #define OS_BASE_ADDR        0xfd200000
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

#define BASE_USBBC_NULL     (0)

/****** USB port RIU base address ******/
#define BASE_UTMI0          (OS_BASE_ADDR+(0x3A80*2))
#define BASE_UHC0           (OS_BASE_ADDR+(0x2400*2))
#define BASE_USBC0          (OS_BASE_ADDR+(0x0700*2))
#define BASE_USBBC0_KAPPA   (OS_BASE_ADDR+(0x20500*2))
#define BASE_USBBC0_KELTIC  (OS_BASE_ADDR+(0x11700*2))
#define BASE_USBBC0_KENYA   (OS_BASE_ADDR+(0x205E0*2))
#define BASE_USBBC0_KAISER  (OS_BASE_ADDR+(0x22F00*2))
#define BASE_USBBC0_KERES   (OS_BASE_ADDR+(0x13700*2))
#define E_IRQ_UHC          (E_INT_IRQ_UHC)
#define E_IRQ_USBC          (E_INT_IRQ_USB)

#define BASE_UTMI1          (OS_BASE_ADDR+(0x3A00*2))
#define BASE_UHC1           (OS_BASE_ADDR+(0x0D00*2))
#define BASE_USBC1          (OS_BASE_ADDR+(0x0780*2))
#define BASE_USBBC1_KENYA   (OS_BASE_ADDR+(0x205C0*2))
#define BASE_USBBC1_KAISER  (OS_BASE_ADDR+(0x22F40*2))
#define BASE_USBBC1_KERES   (OS_BASE_ADDR+(0x13740*2))
#define E_IRQ_UHC1          (E_INT_IRQ_UHC1)
#define E_IRQ_USBC1          (E_INT_IRQ_USB1)

#define BASE_UTMI2          (OS_BASE_ADDR+(0x2A00*2))
#define BASE_UHC2           (OS_BASE_ADDR+(0x10300*2))
#define BASE_USBC2          (OS_BASE_ADDR+(0x10200*2))
#define BASE_USBBC2_KAISER  (OS_BASE_ADDR+(0x22F80*2))
#define E_IRQ_UHC2          (E_INT_IRQ_UHC2)
#define E_IRQ_USBC2          (E_INT_IRQ_USB2)

#define BASE_UTMI3      (OS_BASE_ADDR+(0x20A00*2))
#define BASE_UHC3          (OS_BASE_ADDR+(0x20900*2))
#define BASE_USBC3          (OS_BASE_ADDR+(0x20800*2))
#define E_IRQ_UHC3         (E_INT_IRQ_UHC3)
#define E_IRQ_USBC3          (E_INT_IRQ_USB3)

//------ Hardware ECO enable switch ----------------------------------
//---- 1. fix pv2mi bridge mis-behavior
#if defined(CHIP_KAPPA) || \
	defined(CHIP_KELTIC) || \
	defined(CHIP_KENYA) || \
	defined(CHIP_KRITI) || \
	defined(CHIP_KERES)
        #define ENABLE_PV2MI_BRIDGE_ECO
#endif

//---- 2. Reduce EOF1 to 16us for performance imporvement
#if !defined(CHIP_U4) && !defined(CHIP_K1) && !defined(CHIP_K2)
/* Enlarge EOF1 from 12us to 16us for babble prvention under hub case.
 * However, we keep the "old config name". 20130121
 */
        #define ENABLE_16US_EOF1
#endif

//---- 3. Enable UTMI 240 as 120 phase
#if defined(CHIP_KAISER)
        #define ENABLE_UTMI_240_AS_120_PHASE_ECO
#endif

//---- 4. Change to 55 interface
#if defined(CHIP_KAISER)
        #define ENABLE_UTMI_55_INTERFACE
#endif

//---- 5. tx/rx reset clock gating cause XIU timeout
#if defined(CHIP_KAISER)
        #define ENABLE_TX_RX_RESET_CLK_GATING_ECO
#endif

//---- 6. Setting PV2MI bridge read/write burst size to minimum
#if 0 // every chip must apply it, so far
        #define _USB_MINI_PV2MI_BURST_SIZE 0
#else
        #define _USB_MINI_PV2MI_BURST_SIZE 1
#endif

//---- 7. HS connection fail problem (Gate into VFALL state)
#if defined(CHIP_KELTIC) || \
	defined(CHIP_KERES)
	#define ENABLE_HS_CONNECTION_FAIL_INTO_VFALL_ECO
#endif

#if defined(CHIP_KRITI)
#define ENABLE_HS_DISCONNECTION_DEBOUNCE_ECO
#endif

//------ Software patch enable switch ----------------------------------
//---- 1. Monkey Test software Patch
#if defined(CHIP_U4) || defined(CHIP_K1) || defined(CHIP_K2)
        #define _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH    1
#else
        #define _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH    0
#endif

//---- 2. Clock phase adjustment software Patch
#if defined(CHIP_KERES)
        #define _USB_CLOCK_PHASE_ADJ_PATCH    1
#else
        #define _USB_CLOCK_PHASE_ADJ_PATCH    0
#endif

//---- 3. enabe PVCI i_miwcplt wait for mi2uh_last_done_z
#if defined(CHIP_KERES) || defined(CHIP_KIRIN)
        #define _USB_MIU_WRITE_WAIT_LAST_DONE_Z_PATCH    1
#else
        #define _USB_MIU_WRITE_WAIT_LAST_DONE_Z_PATCH    0
#endif

//-----------------------------------------
// U4_series_usb_init flag:
// Use low word as flag
#define EHCFLAG_NONE             0x0
#define EHCFLAG_DPDM_SWAP        0x1
#define EHCFLAG_TESTPKG          0x2
#define EHCFLAG_DOUBLE_DATARATE  0x4
#define EHCFLAG_USBBC_OFF        0x8
// Use high word as data
#define EHCFLAG_DDR_MASK     0xF0000000
#define EHCFLAG_DDR_x15      0x10000000 //480MHz x1.5
#define EHCFLAG_DDR_x18      0x20000000 //480MHz x1.8
//-----------------------------------------

#define HUB_STACK_SIZE  0x2000
#define ms_usbhost_err(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)


//------ UTMI eye diagram parameters ---------------------------------
#if 0
        // for 40nm
        #define UTMI_EYE_SETTING_2C     (0x98)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x10)
        #define UTMI_EYE_SETTING_2F     (0x01)
#elif 0
        // for 40nm after Agate, use 55nm setting7
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting6
        #define UTMI_EYE_SETTING_2C     (0x10)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting5
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting4
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting3
        #define UTMI_EYE_SETTING_2C     (0x10)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif defined(CHIP_KENYA) || \
    defined(CHIP_KAISER)
        // for 40nm after Agate, use 55nm setting2
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#else
        // for 40nm after Agate, use 55nm setting1, the default
        #define UTMI_EYE_SETTING_2C     (0x10)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#endif

#endif

