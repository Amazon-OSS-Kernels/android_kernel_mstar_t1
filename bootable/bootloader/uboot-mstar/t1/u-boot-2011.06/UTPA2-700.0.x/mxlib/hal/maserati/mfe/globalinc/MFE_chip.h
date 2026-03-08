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
#ifndef _MFE_CHIP_H_
#define _MFE_CHIP_H_
/*
This file includes all definitions which we use in MHal, MDrv, and MAdp layer.
We also have some definitions only belonged to MHal(in drvMFE.h), MDrv(in mdrv_m4ve.c), and MAdp(in madp_m4ve.c)
*/

// Chip definition.        << Choose one and only one >>
//#define _MFE_BIG2_
#define _MFE_T8_

// CPU-arch platform.        << Choose one and only one >>
#define _MIPS_PLATFORM_
//#define _AEON_PLATFORM_
//#define _FPGA_    // For PC UDMA/HIF FPGA testing. Refer to fpga_def.h

// File system
//#define _NO_FILESYSTEM_

// Bitstream getting, ISR
#define _ENABLE_ISR_
#define _NO_WAIT_FRAMEDONE_    // NOTE: Must defined if _ENABLE_ISR_ is defined.

//#define MEM_CHECK //check mem write after my_malloc
//#define CRC_CHECK //check crc

#ifdef CRC_CHECK
#define _CRC_PATTERN_GEN_    //check CRC pattern which from C MODEL
#endif
// Linux/TV-specific
#if defined(_MFE_T8_)
    //#define _MFE_LG_ //must also define _KERNEL_MODE and _MIPS_PLATFORM_
    #define _MFE_UTOPIA_
    #define _MFE_A3_
    //#define _KERNEL_MODE_        // Acts as kernel-mode driver.
    //#define CHECK_OUTPUT_FROM_DRAM
    //#define _MEM_READ_WRITE_CONTROL_
    //_INTERNAL_DIP_ and CUSTOM_INTERFACE1 are used for VOIP demo
    //#define _INTERNAL_DIP_
    //#define CUSTOM_INTERFACE1
    //#define DRV_MFE_TIME_MEASURE
#ifndef _CRC_PATTERN_GEN_
    #define SPS_PPS_IN_EACH_I
#endif
#endif

 #define OBUF_SIZE   409600    // Bytes
 #define OBUF_NUM    1
 //#define OBUF_SIZE   306000    // Bytes
//#define OBUF_SIZE   128000    // Bytes
//#define OBUF_NUM    4


// Mobile-specific
#if defined(_MFE_BIG2_)
    #define _USE_PSRAM_
    //#define _TRACE32_CMM_
#endif

#if defined(_FPGA_)
    #include "fpga_def.h"
#endif

#endif    // _MFE_CHIP_H_
