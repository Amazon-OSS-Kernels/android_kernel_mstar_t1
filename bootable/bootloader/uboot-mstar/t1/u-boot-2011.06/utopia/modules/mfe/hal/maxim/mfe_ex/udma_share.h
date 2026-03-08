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
#ifndef _UDMA_SHARE_H_
#define _UDMA_SHARE_H_

#include "MFE_chip.h"
#include "drv_mfe_st.h"
#include "mfe_type.h"

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) &&defined(_KERNEL_MODE_)
	//#define addr_log2phy(log_addr) MS_VA2PA(log_addr) //no log to phy in drv level
	#define addr_phy2log(phy_addr) MDrv_SYS_PA2NonCacheSeg( (void*) phy_addr )
#elif defined(_MFE_UTOPIA_)||defined(__MOBILE_CASE__)
#include "MsCommon.h"
#include "MsOS.h"
	#define addr_log2phy(log_addr) MS_VA2PA(log_addr)
	#define addr_phy2log(phy_addr) MS_PA2KSEG1(phy_addr)
#elif defined(WIN32)
#include "pmem_mm.h"
	#define addr_log2phy(log_addr) pmem_mm_va2pa((MS_U32)log_addr)
	#define addr_phy2log(phy_addr) pmem_mm_pa2va((MS_U32)phy_addr)
#endif

MS_BOOL MMAPInit(MS_PHY buf_base, MS_U32 buf_size, MS_PHY* ppBufStart);
MS_BOOL MMAPMalloc(MS_PHY* ppBufStart, MS_PHY pBufEnd, MS_U32 size, MEMMAP_t * memmap, MS_PHY nAlignBytes, const MS_S8* msg);

#endif
