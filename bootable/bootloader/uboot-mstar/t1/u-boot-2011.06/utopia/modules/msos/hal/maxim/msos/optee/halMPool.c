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

MS_PHY HAL_MsOS_MPool_PA2BA(MS_PHY u64PhyAddr)
{
     MS_PHY u64BusAddr = 0x0;
     // ba = pa + offset
     if( (u64PhyAddr >= HAL_MIU0_BASE) && (u64PhyAddr < HAL_MIU1_BASE) )     // MIU0
                u64BusAddr = u64PhyAddr - HAL_MIU0_BASE + HAL_MIU0_BUS_BASE;
     else if( (u64PhyAddr >= HAL_MIU1_BASE) && (u64PhyAddr < HAL_MIU2_BASE) )        // MIU1
                u64BusAddr = u64PhyAddr - HAL_MIU1_BASE + HAL_MIU1_BUS_BASE;
     else
                u64BusAddr = u64PhyAddr - HAL_MIU2_BASE + HAL_MIU2_BUS_BASE;    // MIU2

     return u64BusAddr;
}

MS_PHY HAL_MsOS_MPool_VA2PA(MS_VIRT u64Virt)
{
    //===========================================
    // MIU0 (0 ~ 1024MB) (VA) 0x2000:0000 ~ 0x5fff:ffff -> (PA) 0x0000:0000 ~ 0x3fff:ffff (cached)
    if ((HAL_MIU0_BUS_BASE <= u64Virt) && ((HAL_MIU0_BUS_BASE + 0x3fffffff) >= u64Virt))
    {
        return (MS_PHY)(u64Virt - HAL_MIU0_BUS_BASE);
    }

    //===========================================
    // MIU0 (0 ~ 1024MB) (VA) 0x6000:0000 ~ 0x9fff:ffff -> (PA) 0x0000:0000 ~ 0x3fff:ffff (uncached)
    if (((HAL_MIU0_BUS_BASE + 0x40000000UL) <= u64Virt) && ((HAL_MIU0_BUS_BASE + 0x7fffffffUL) >= u64Virt))
    {
        return (MS_PHY)(u64Virt - (HAL_MIU0_BUS_BASE+0x40000000UL));
    }

    //===========================================
    // MIU1 (low 512MB) (VA) 0xa000:0000 ~ 0xbfff:ffff -> (PA) 0x8000:0000 ~ 0x9fff:ffff (cached)
    if ((HAL_MIU1_BUS_BASE <= u64Virt) && ((HAL_MIU1_BUS_BASE + 0x1fffffffUL) >= u64Virt))
    {
        return (MS_PHY)((u64Virt & 0x1fffffffUL) + HAL_MIU1_BASE);
    }

    //===========================================
    // MIU1 (0 ~ 1024MB) (VA) 0xc000:0000 ~ 0xffff:ffff -> (PA) 0x8000:0000 ~ 0xbfff:ffff (uncached)
    if (((HAL_MIU1_BUS_BASE + 0x20000000UL) <= u64Virt) && ((HAL_MIU1_BUS_BASE + 0x5fffffff) >= u64Virt))
    {
        return (MS_PHY)((u64Virt & 0x3fffffffUL) + HAL_MIU1_BASE);
    }
}

MS_VIRT HAL_MsOS_MPool_PA2KSEG0(MS_PHY u64Phys)
{
    //===========================================
    // MIU0 (0~1024MB) - (PA) 0x0000:0000 ~ 0x3fff:ffff -> (VA) 0x2000:0000 ~ 0x5fff:ffff (cached)
    if ((0x00000000UL <= u64Phys) && (0x3fffffffUL >= u64Phys))
    {
        return (MS_VIRT)((u64Phys & 0x3fffffffUL) + HAL_MIU0_BUS_BASE);
    }

    //===========================================
    // MIU1 (low 512MB) - (PA) 0x8000:0000 ~ 0x9fff:ffff -> (VA) 0xA000:0000 ~ 0xbfff:ffff (cached)
    if ((HAL_MIU1_BASE <= u64Phys) && ((HAL_MIU1_BASE + 0x1fffffffUL) >= u64Phys))
    {
        return (MS_VIRT)((u64Phys - HAL_MIU1_BASE) + HAL_MIU1_BUS_BASE);
    }
}

MS_VIRT HAL_MsOS_MPool_PA2KSEG1(MS_PHY u64Phys)
{
    //===========================================
    // MIU0 (0~1024MB) - (PA) 0x0000:0000 ~ 0x3fff:ffff -> (VA) 0x6000:0000 ~ 0x9fff:ffff (uncached)
    if ((0x00000000UL <= u64Phys) && (0x3fffffffUL >= u64Phys))
    {
        return (MS_VIRT)((u64Phys & 0x3fffffffUL) + HAL_MIU0_BUS_BASE);
    }

    //===========================================
    // MIU1 (0~1024MB) - (PA) 0x8000:0000 ~ 0xBfff:ffff -> (VA) 0xc000:0000 ~ 0xFfff:ffff (uncached)
    if ((HAL_MIU1_BASE <= u64Phys) && ((HAL_MIU1_BASE + 0x3fffffffUL) >= u64Phys))
    {
        return (MS_VIRT)((u64Phys - HAL_MIU1_BASE) + HAL_MIU1_BUS_BASE);
    }
}
