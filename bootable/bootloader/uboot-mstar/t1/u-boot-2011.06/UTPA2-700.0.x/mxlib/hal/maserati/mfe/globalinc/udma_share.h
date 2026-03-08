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
#ifndef _UDMA_SHARE_H_
#define _UDMA_SHARE_H_

//#define _FPGA_
//#define _MSVC6_ //open when using VC6.0 platform
#include "MFE_chip.h"
#ifdef _AEON_PLATFORM_
#ifdef _MFE_T8_
#else
#include "../../../../MsTypes.h"
#endif
#endif

#ifdef _MIPS_PLATFORM_
#if defined(_MFE_T8_)&&(!defined(_KERNEL_MODE_))
#include "../../../../MsTypes.h"
#include "../../../../drv/mfe/mdrv_mfe_st.h"
#elif defined(_MFE_T8_)&&(defined(_KERNEL_MODE_))
#include "mdrv_mfe_st.h"
#include "mdrv_types.h"
#else
#include "mdrv_types.h"
#endif
#endif //_MIPS_PLATFORM_

#include "mfe_type.h"

#ifdef _WIN32
#include "../../mdrv_mfe_st.h"
#endif

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) &&defined(_KERNEL_MODE_)
	//#define addr_log2phy(log_addr) MS_VA2PA(log_addr) //no log to phy in drv level
	#define addr_phy2log(phy_addr) MDrv_SYS_PA2NonCacheSeg( (void*) phy_addr )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) &&(!defined(_KERNEL_MODE_))
	#define addr_log2phy(log_addr) MS_VA2PA(log_addr)
	#define addr_phy2log(phy_addr) MS_PA2KSEG1(phy_addr)
#elif defined(_M4VE_BIG2_)&&defined(_MIPS_PLATFORM_)
	#include "utility.h"
	#define addr_log2phy(log_addr) MApi_UTL_HwMemAccessOffset((void *)log_addr)
	//#define addr_phy2log(phy_addr) ((phy_addr)+MEM_LOGMAPPHY) //MApi_UTL_PhyMemRestore((CYG_ADDRESS)phy_addr, INTF_M4VE_MC_W)
	#define addr_phy2log(phy_addr) (MApi_UTL_PhyMemRestore((CYG_ADDRESS)phy_addr, INTF_M4VE_MC_W) + CYGARC_KSEG_UNCACHED_BASE )
	//#define addr_phy2log(phy_addr) MApi_UTL_PhyMemRestore((CYG_ADDRESS)phy_addr, INTF_M4VE_MC_W)
#elif defined(WIN32)
	extern MFE_U32 MAdp_MPool_VA2PA_Win32(MFE_U32 a);
	extern MFE_U32 MAdp_MPool_PA2VA_Win32(MFE_U32 a);
	#define addr_log2phy(log_addr) MAdp_MPool_VA2PA_Win32((MFE_U32)log_addr)
	#define addr_phy2log(phy_addr) MAdp_MPool_PA2VA_Win32((MFE_U32)phy_addr)
#else
	#define addr_log2phy(log_addr) ((log_addr)-MEM_LOGMAPPHY)
	#define addr_phy2log(phy_addr) ((phy_addr)+MEM_LOGMAPPHY)
#endif

typedef void (*proto_write)(MFE_U32 u32Address, MFE_U16 val);
typedef void (*proto_read) (MFE_U32 u32Address, MFE_U16 *val);

void MMAPInit(MFE_U32 buf_base, MFE_U32 buf_size, MFE_U32* ppBufStart);
long MMAPMalloc(MFE_U32* ppBufStart, MFE_U32 pBufEnd, MFE_U32 size, MEMMAP_t * memmap, int nAlignBytes,const char* msg);
int reg_scan(unsigned short *reg_mask, MFE_U32 num_reg, proto_write write_func, proto_read read_func);

#endif
