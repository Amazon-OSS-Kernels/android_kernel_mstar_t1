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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "halMPool.h"
#include "halCHIP.h"
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <sys/ioctl.h>  // ioctl
#endif

MS_PHY HAL_MsOS_MPool_VA2PA(MS_VIRT VirtAddr, MS_SIZE tVirt2PhysOff)
{
    return (MS_PHY)(VirtAddr - tVirt2PhysOff);
}

MS_VIRT  HAL_MsOS_MPool_PA2VA(MS_PHY u64Phys, MS_PHY u64MappedPhys, MS_U64 u64MappedSize, MS_SIZE tVirt2PhysOff, MS_BOOL bPartialMapping)
{
    if(bPartialMapping)
    {
        if((u64MappedPhys <= u64Phys) && (u64Phys < (u64MappedPhys + u64MappedSize)))
            return (MS_VIRT)(u64Phys + tVirt2PhysOff);
    }
    else
    {
        return (MS_VIRT)(u64Phys + tVirt2PhysOff);
    }

    return 0;
}
#if !defined(MSOS_TYPE_LINUX_KERNEL)
MS_BOOL HAL_MsOS_MPool_Dcache_Flush(int fd, MS_VIRT addrVirt, MS_SIZE u64Size, MS_PHY u64Phys)
{
    DevMalloc_MPool_Flush_Info_t stFlushInfo;
   
    stFlushInfo.virtAddrVirt = addrVirt;
    stFlushInfo.u64AddrPhys = u64Phys;
    stFlushInfo.u64Size = u64Size;

    if (ioctl(fd, MALLOC_IOC_FLUSHDCACHE_PAVA, &stFlushInfo))
    {
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_MsOS_MPool_Dcache_Flush_All(int fd)
{
    DevMalloc_MPool_Flush_Info_t stFlushInfo;

    if (ioctl(fd, MALLOC_IOC_FLUSHDCACHE_ALL, &stFlushInfo))
    {
        return FALSE;
    }
    return TRUE;
}
#endif
MS_PHY HAL_MsOS_MPool_PA2BA(MS_PHY u64PhyAddr)
{
    MS_PHY u64BusAddr = 0x0;
        
    // ba = pa + offset
	if( (u64PhyAddr >= HAL_MIU0_BASE) && (u64PhyAddr < HAL_MIU1_BASE) )	// MIU0
		u64BusAddr = u64PhyAddr - HAL_MIU0_BASE + HAL_MIU0_BUS_BASE;
	else if((u64PhyAddr >= HAL_MIU1_BASE) && (u64PhyAddr < HAL_MIU2_BASE)) //MIU1
		u64BusAddr = u64PhyAddr - HAL_MIU1_BASE + HAL_MIU1_BUS_BASE;
    else                                                                 //MIU2
        u64BusAddr = u64PhyAddr - HAL_MIU2_BASE + HAL_MIU2_BUS_BASE;
	return u64BusAddr;
}

MS_PHY HAL_MsOS_MPool_BA2PA(MS_PHY u64BusAddr)
{
    MS_PHY u64PhyAddr = 0x0;
 
    // pa = ba - offset
	if( (u64BusAddr >= HAL_MIU0_BUS_BASE) && (u64BusAddr < HAL_MIU1_BUS_BASE) )	// MIU0
		u64PhyAddr = u64BusAddr - HAL_MIU0_BUS_BASE + HAL_MIU0_BASE;
	else if( (u64BusAddr >= HAL_MIU0_BUS_BASE) && (u64BusAddr < HAL_MIU2_BUS_BASE) )// MIU1
		u64PhyAddr = u64BusAddr - HAL_MIU1_BUS_BASE + HAL_MIU1_BASE;
    else                                                                        //MIU2
        u64PhyAddr = u64BusAddr - HAL_MIU2_BUS_BASE + HAL_MIU2_BASE;
	return u64PhyAddr;
}
