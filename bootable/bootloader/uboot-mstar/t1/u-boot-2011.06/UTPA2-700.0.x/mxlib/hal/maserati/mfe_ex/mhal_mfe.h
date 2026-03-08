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
////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MFE_H_
#define _MHAL_MFE_H_

#if defined(_MFE_UTOPIA_)
#include <stdio.h>
#endif

#include "MFE_chip.h"
#include "drv_mfe_st.h"
#if defined(_MIPS_PLATFORM_)
#if defined(_MFE_BIG2_)
#include <sys/bsdtypes.h>
#include "shellcfg.h"   //for diag_printf
#elif defined(_KERNEL_MODE_)&&defined(_MFE_T8_)
#include "mdrv_types.h"
#elif defined(__MOBILE_CASE__)
#include "msutil/MsTypes.h"
#else
#include "MsTypes.h"
#endif
#endif

#include "mfe_type.h"
#include "mfe_common.h"

#ifdef _MFE_BIG2_
#define MIU_SHIFT    2UL
#define MIU_SIZE     4UL //8
#else
#define MIU_SHIFT    3UL
#define MIU_SIZE     8UL
#endif

#if defined(_AEON_PLATFORM_) && defined(_MFE_T8_)
	extern MS_U32 RIU_BASE;// = 0xA0000000UL;
	#define REG_BANK_MFE    0x111000UL
	#define __MFE_REG(reg)		(*(volatile U16 *) ( RIU_BASE + (REG_BANK_MFE + reg*2)*2) )
#elif defined(__MFE_G2__)
	#define T8_RIU_BASE ((MS_VIRT)u32MFERegOSBase) // = 0x25000000UL; G2 RIU base.
	#define REG_BANK_MFE  0x400UL
	#define __MFE_REG(reg) (*(volatile MS_U32 *) (void*)( T8_RIU_BASE + (reg)*4 ) )
#elif defined(_AEON_PLATFORM_)
	extern MS_U32 RIU_BASE;// = 0xA0000000UL;
	#define REG_BANK_MFE    0x1200UL
	#define __MFE_REG(reg)		(*(volatile U16 *) ( RIU_BASE + (REG_BANK_MFE + reg)*4) )
//extern U8 FSwrite_ready;
#elif defined(_MFE_BIG2_) && defined(_MIPS_PLATFORM_)
	extern MS_U32 RIU_BASE;// = 0xBF834000UL;
	#define REG_BANK_MFE    0UL
	#define __MFE_REG(reg)        (*(volatile U16 *) ( RIU_BASE + REG_BANK_MFE + (reg)*4) )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
	extern MS_U32 RIU_BASE;// = 0xBF200000UL; //CH4
	#define REG_BANK_MFE    0x8800UL
	#define __MFE_REG(reg)		(*(volatile MS_U16 *) ( RIU_BASE + (REG_BANK_MFE + reg) * 4 ) )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& defined(_MFE_UTOPIA_)
	#define T8_RIU_BASE ((MS_VIRT)u32MFERegOSBase) // = 0xBF200000UL; //CH4
#ifdef _MFE_KAISER_
	#define REG_BANK_MFE    0x11380UL
#else
    #define REG_BANK_MFE    0x8800UL
#endif
	#define __MFE_REG(reg)		(*(volatile MS_U16 *) ( T8_RIU_BASE + (REG_BANK_MFE + reg) * 4 ) )
    #define REG_BANK_MFE1    0x8880UL
    #define __MFE_REG1(reg)		(*(volatile MS_U16 *) ( T8_RIU_BASE + (REG_BANK_MFE1 + reg) * 4 ) )
#else //if defined(_WIN32)//defined(_BCB_PLATFORM_)
    #define T8_RIU_BASE 0x0a80UL // = 0xBF200000UL; //CH4
	#define REG_BANK_MFE 0x0a80UL //Local FPGA
	#define REG_BANK_MFE1 0x0b80UL //Local FPGA
    //#define T8_RIU_BASE 0x0b80UL // = 0xBF200000UL; //CH4
	//#define REG_BANK_MFE 0x0b80UL //Local FPGA
	//#define REG_BANK_MFE1 0x01080UL //Local FPGA
#endif

MS_BOOL MHal_MFE_GetHWCap(MS_U16 *width, MS_U16 *height);

#if (defined(_MFE_T8_)||defined(_MFE_M1_)) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
void MHAL_MFE_InitRegBase(MS_U32 u32RegBase);
void MHAL_MFE_CreateRegMap(MFE_REG* mfe_reg, MFE_REG1* mfe_reg1);
void MHAL_MFE_DelRegMap(MFE_REG* mfe_reg, MFE_REG1* mfe_reg1);
#endif
void MHal_MFE_PowerOff(MS_U32 is_off,MFE_CLK_LEVEL clock_level);
MS_U32 MHal_MFE_GetBitstreamEncodedLen(void);
void MHal_MFE_set_outbitsbuf(MFE_REG* mfe_reg, OutBitSBUF *bitsbuf, MS_S32 outbufsize);
void MHal_MFE_SetIrqMask(MS_U16 mask);

#ifdef MFE_MIU_PROTECT
void MHal_MFE_Enable_MIU_Protection(MS_S32 MIU_TEST_MODE,MFE_CONFIG* pConfig);
void MHal_MFE_Enable_MIU_Protection_Check(MS_S32 MIU_TEST_MODE, MS_S32 TYPE);
#endif
MS_U32 MHal_MFE_CycleReport(void);
void MHal_MFE_ClearIRQ(MS_U16 irq_bits);
void MHal_MFE_GetIRQ(MS_U16 *irq_bits);
void MHal_MFE_SWReset(MFE_REG* mfe_reg);
void MHal_MFE_start(void);
void MHal_MFE_SetCLKCTL(MFE_REG* mfe_reg);
void MHal_MFE_ResetReg(MFE_REG* mfe_reg);
void MHal_MFE_GetCRC(MS_U8 checksum_HW[8]);
#endif//_MHAL_MFE_H_

