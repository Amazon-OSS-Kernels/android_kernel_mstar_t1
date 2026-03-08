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
////////////////////////////////////////////////////////////////////////////////
//
//
//
/// @file drvMFE.h
/// @brief MFE driver
///
/// MFE is the Mpeg4 ASP encoder, used for PVR function.
///
/// Features:
/// - Mpeg 4 ASP ( part 2 ) encoder.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MFE_H_
#define _MHAL_MFE_H_

#ifndef _KERNEL_MODE_
#include <stdio.h>
#endif

#include "MFE_chip.h"

#ifdef _AEON_PLATFORM_
#if defined(_MFE_T8_)
#include "DataType.h"
#else
#include "MsTypes.h"
#endif
#endif

#if defined(_MIPS_PLATFORM_)
#if defined(_MFE_BIG2_)
#include <sys/bsdtypes.h>
#include "shellcfg.h"   //for diag_printf
#elif defined(_KERNEL_MODE_)&&defined(_MFE_T8_)
#include "mdrv_types.h"
#else
//#include "mdrv_types.h"
#include "MsTypes.h"
#endif
#endif

#include "mfe_type.h"
#include "mfe_common.h"

#ifdef _MFE_BIG2_
#define MIU_SHIFT     2//3
#define MIU_SIZE     4//8
#else
#define MIU_SHIFT     3
#define MIU_SIZE     8
#endif


typedef struct {
    unsigned long   start_addr;
    unsigned long   end_addr;
    long            used_size;	// 0 means not used
} OutBitSBUF;

#if !(defined(_KERNEL_MODE_)&&defined(_MIPS_PLATFORM_)&&defined(_MFE_T8_))
#ifdef _NO_FILESYSTEM_ //defined(_AEON_PLATFORM_)
extern int fp_script;
#else
extern FILE *fp_script;
#endif
#endif
#ifdef _WIN32//_BCB_PLATFORM_
#define MFE_SCRIPT_OUT 1
//#define _IPB_FRAMEQP_
#elif defined(_TRACE32_CMM_)
#define MFE_SCRIPT_OUT 0
#else
#define MFE_SCRIPT_OUT 0
#endif
#if (MFE_SCRIPT_OUT == 1)
    #define MFE_SCRIPT(x)	(x)
#else
    #define MFE_SCRIPT(x)  {}
#endif

#if defined(_AEON_PLATFORM_) && defined(_MFE_T8_)
	extern U32 RIU_BASE;// = 0xA0000000;
	#define REG_BANK_MFE    0x111000
	#define __MFE_REG(reg)		(*(volatile U16 *) ( RIU_BASE + (REG_BANK_MFE + reg*2)*2) )
#elif defined(_AEON_PLATFORM_)
	extern U32 RIU_BASE;// = 0xA0000000;
	#define REG_BANK_MFE    0x1200
	#define __MFE_REG(reg)		(*(volatile U16 *) ( RIU_BASE + (REG_BANK_MFE + reg)*4) )
//extern U8 FSwrite_ready;
#elif defined(_MFE_BIG2_) && defined(_MIPS_PLATFORM_)
	extern U32 RIU_BASE;// = 0xBF834000;
	#define REG_BANK_MFE    0
	#define __MFE_REG(reg)        (*(volatile U16 *) ( RIU_BASE + REG_BANK_MFE + (reg)*4) )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
	extern MFE_U32 RIU_BASE;// = 0xBF200000; //CH4
	#define REG_BANK_MFE    0x8800
	#define __MFE_REG(reg)		(*(volatile MFE_U16 *) ( RIU_BASE + (REG_BANK_MFE + reg) * 4 ) )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
	#define T8_RIU_BASE u32MFERegOSBase // = 0xBF200000; //CH4
	#define REG_BANK_MFE    0x8800
	#define __MFE_REG(reg)		(*(volatile MFE_U16 *) ( T8_RIU_BASE + (REG_BANK_MFE + reg) * 4 ) )
#elif defined(_HIF_) && defined(_MFE_BIG2_)
	extern U32 RIU_BASE;// = 0xA0000000;
	#define REG_BANK_MFE    0xd000
	#define __MFE_REG(reg)        FPGA_RIURead16(REG_BANK_MFE+reg, &val_64)
#elif defined(_FPGA_)
	extern U32 RIU_BASE;// = 0xA0000000;
	#define REG_BANK_MFE    0xa80
	#define __MFE_REG(reg)        FPGA_RIURead16(REG_BANK_MFE+reg, &val_64)
#else //if defined(_WIN32)//defined(_BCB_PLATFORM_)
	extern unsigned short REG_BANK_MFE[0x100];
	#define __MFE_REG(reg)		REG_BANK_MFE[reg]
#endif

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
void MHAL_MFE_InitRegBase(MFE_U32 u32RegBase);
#endif
void MHal_MFE_PowerOff(MFE_U32 is_off,MFE_U32 clock_level);
MFE_U32 MHal_MFE_GetBitstreamEncodedLen(void);
void MHal_MFE_set_outbitsbuf(OutBitSBUF *bitsbuf);
void MHal_MFE_SetIrqMask(MFE_U16 mask);
#ifdef _MFE_T8_
void MHal_MFE_Enable_MIU_Protection(int MIU_TEST_MODE,MFE_CONFIG* pConfig);
void MHal_MFE_Enable_MIU_Protection_Check(int MIU_TEST_MODE,int TYPE);
#endif

void MHal_MFE_ClearIRQ(MFE_U16 irq_bits);
void MHal_MFE_GetIRQ(MFE_U16 *irq_bits);
void MHal_MFE_SWReset(void);
void MHal_MFE_start(void);
void MHal_MFE_SetCLKCTL(void);
void MHal_MFE_ResetReg(void);
void MHal_MFE_GetCRC(MFE_U8 checksum_HW[8]);
#endif//_MHAL_MFE_H_
