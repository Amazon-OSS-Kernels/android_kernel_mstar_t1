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

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_Init(void)
{
    return TRUE;
}

MS_BOOL MsOS_MPool_Close(void)
{
    return TRUE;
}

// @FIXME: support one pool at this stage
MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
    return TRUE;
}

inline MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt)
{
#ifdef MCU_MIPS_4KE
    return (MS_U32)(((MS_U32)pAddrVirt)&0x1fffffff);
#elif (defined(MCU_AEON))
    return ((MS_U32)(pAddrVirt) & ~(0xC0000000));
#else //ARM
    // miu0 (low 256MB) (VA)0x6000:0000~0x7000:0000 --> (PA)0x0000:0000~0x1000:0000 (cached)
    // miu0 (low 256MB) (VA)0x8000:0000~0x9000:0000 --> (PA)0x0000:0000~0x1000:0000 (uncached)
    if ((0x60000000 <= pAddrVirt) && (0x70000000 > pAddrVirt))
    {
        return (pAddrVirt & 0x0fffffff);
    }
    if ((0x80000000 <= pAddrVirt) && (0x90000000 > pAddrVirt))
    {
        return (pAddrVirt & 0x0fffffff);
    }
    // miu0 (high 256MB) (VA)0x7000:0000~0x8000:0000 -> (PA)0x1000:0000~0x2000:0000 (cached)
    // miu0 (high 256MB) (VA)0x9000:0000~0xA000:0000 -> (PA)0x1000:0000~0x2000:0000 (uncached)
    if ((0x70000000 <= pAddrVirt) && (0x80000000 > pAddrVirt))
    {
        return (pAddrVirt & 0x1fffffff);
    }
    if ((0x90000000 <= pAddrVirt) && (0xA0000000 > pAddrVirt))
    {
        return (pAddrVirt & 0x1fffffff);
    }
    // miu1 (low 256MB) (VA)0xA000:0000~0xB000:0000 -> (PA)0x6000:0000~0x7000:0000 (cached)
    // miu1 (low 256MB) (VA)0xC000:0000~0xD000:0000 -> (PA)0x6000:0000~0x7000:0000 (uncached)
    if ((0xA0000000 <= pAddrVirt) && (0xB0000000 > pAddrVirt))
    {
        return ((pAddrVirt & 0x0fffffff) + HAL_MIU1_BASE);
    }
    if ((0xC0000000 <= pAddrVirt) && (0xD0000000 > pAddrVirt))
    {
        return ((pAddrVirt & 0x0fffffff) + HAL_MIU1_BASE);
    }
    // miu1 (high 256MB) (VA)0xB000:0000~0xC000:0000 -> (PA)0x7000:0000~0x8000:0000 (cached)
    // miu1 (high 256MB) (VA)0xD000:0000~0xE000:0000 -> (PA)0x7000:0000~0x8000:0000 (uncached)
    if ((0xB0000000 <= pAddrVirt) && (0xC0000000 > pAddrVirt))
    {
        return ((pAddrVirt & 0x1fffffff) + HAL_MIU1_BASE);
    }
    if ((0xD0000000 <= pAddrVirt) && (0xE0000000 > pAddrVirt))
    {
        return ((pAddrVirt & 0x1fffffff) + HAL_MIU1_BASE);
    }
#endif
	return -1;
}

inline MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys)
{
#ifdef MCU_MIPS_4KE
    return (MS_U32)(((MS_U32)pAddrPhys)|0xa0000000);
#elif (defined(MCU_AEON))
    // miu0 (512MB) - (PA) 0x0000:0000~0x2000:0000 -> 0xC000:0000~0xE000:0000

    if((0x00000000 <= pAddrPhys) && (0x20000000 > pAddrPhys))
    {
        return (pAddrPhys + 0xC0000000);
    }

    // miu1 (512MB) - (PA) 0x2000:0000~0x4000:0000 -> 0xE000:0000~0xFFFF:FFFF
    if((0x20000000 <= pAddrPhys) && (0x40000000 > pAddrPhys))
    {
        return (pAddrPhys + 0xC0000000);
    }
#else //ARM
    // miu0 (low 256MB) - (PA)000000:0000~0x1000:0000 -> (VA)0x8000:0000~0x9000:0000 uncached
    if ((0x00000000 <= pAddrPhys) && (0x10000000 > pAddrPhys))
    {
        return (pAddrPhys + 0x80000000);
    }
    // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> (VA)0x9000:0000~0xA000:0000 uncached
    if ((0x10000000 <= pAddrPhys) && (0x20000000 > pAddrPhys))
    {
        return (pAddrPhys + 0x80000000);
    }
    // miu1 (low 256MB) - (PA)0x6000:0000~0x7000:0000 -> (VA)0xC000:0000~0xD000:0000 uncached
    if ((0x60000000 <= pAddrPhys) && (0x70000000 > pAddrPhys))
    {
        return (pAddrPhys + 0x60000000);
    }
    // miu1 (high 256MB) - (PA)0x7000:0000~0x8000:0000 -> (VA)0xD000:0000~0xE000:0000 uncached
    if ((0x70000000 <= pAddrPhys) && (0x80000000 > pAddrPhys))
    {
        return (pAddrPhys + 0x60000000);
    }
#endif

	return -1;
}
MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE tOffset, MS_SIZE tMapSize, MS_BOOL bNonCache)
{
    return TRUE;
}

void  MsOS_MPool_SetDbgLevel(MsOSMPool_DbgLevel DbgLevel)
{
}
