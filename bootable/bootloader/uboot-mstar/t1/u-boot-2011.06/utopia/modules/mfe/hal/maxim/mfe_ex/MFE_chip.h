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
#ifndef _MFE_CHIP_H_
#define _MFE_CHIP_H_
/*
This file includes all definitions which we use in MHal, MDrv, and MAdp layer.
We also have some definitions only belonged to MHal(in drvMFE.h), MDrv(in mdrv_m4ve.c), and MAdp(in madp_m4ve.c)
*/

// Chip definition.		<< Choose one and only one >>
//#define _MFE_BIG2_
#define _MFE_T8_

//#define _MFE_M1_

//#define NO_JPEG
//#define _MFE_ENABLE_CRC64_
#if defined(_MFE_M1_)
//#define __UBOOT__
//#define __MFE_G2__
#define _NON_THREAD_
//#define _SW_BUF_MODE_
//#define _GenSkipHeader_
#endif

// CPU-arch platform.		<< Choose one and only one >>
#define _MIPS_PLATFORM_
//#define _AEON_PLATFORM_
//#define _FPGA_	// For PC UDMA/HIF FPGA testing. Refer to fpga_def.h

// File system
//#define _NO_FILESYSTEM_

// Bitstream getting, ISR
// #define _ENABLE_ISR_
//#define MFE_MIU_PROTECT
//#define MEM_CHECK //check mem write after my_malloc
//##define CRC_CHECK //check crc

#ifdef CRC_CHECK
#define _CRC_PATTERN_GEN_	//check CRC pattern which from C MODEL
#endif

#define MFE_ER_MODE  0x11UL  //reg_mfe_g_er_mode 0/1/2/3: mby/bs/mby+bs/off
#define MFE_ER_MBY   0UL     //reg_mfe_g_er_mby 0/1/2/3: every 1/2/4/8 mb row(s) (error resilence)
#define MFE_ER_BS_TH 0UL     //reg_mfe_g_er_bs_th er_bs mode threshold

// Linux/TV-specific
#if defined(_MFE_T8_)
    //#define _MFE_LG_ //must also define _KERNEL_MODE and _MIPS_PLATFORM_
#ifndef WIN32
    #define _MFE_UTOPIA_
#endif
    //#define _SUPPORT_JPE_
    #define _MFE_A3_
    #define _MFE_AGATE_ //must define _MFE_A3_
    #define _MFE_EDISON_ //must define _MFE_A3_ and _MFE_AGATE_
    #define _MFE_MAXIM_ //must define _MFE_A3_ and _MFE_AGATE_ and _MFE_EDISON_
    #define _NON_THREAD_
    //#define _KERNEL_MODE_		// Acts as kernel-mode driver.
    //#define CHECK_OUTPUT_FROM_DRAM
    //#define _MEM_READ_WRITE_CONTROL_
    //_INTERNAL_DIP_ and CUSTOM_INTERFACE1 are used for VOIP demo
    //#define _INTERNAL_DIP_
    //#define CUSTOM_INTERFACE1
    //#define DRV_MFE_TIME_MEASURE

#endif


// Features
#define MFE_SUPPORT_1080P
#define MFE_DBF_PACKED_MODE


// OBUF

#if defined(MFE_SUPPORT_1080P)
    #define OBUF_SIZE   (2UL * 1024UL * 1024UL)	// 2 MBytes
#elif defined(MFE_SUPPORT_720P)
    #define OBUF_SIZE   (1UL * 1024UL * 1024UL)	// 1 MBytes
#endif

#define OBUF_NUM    1UL


#endif	// _MFE_CHIP_H_
