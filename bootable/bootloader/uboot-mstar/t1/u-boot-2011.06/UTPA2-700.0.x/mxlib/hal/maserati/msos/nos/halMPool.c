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

#include "MsCommon.h"
#include "halMPool.h"
#include "halCHIP.h"

#if (defined(MCU_AEON))
#define RIU_MAP 0xA0000000
#define R2_MAU_BASE     (RIU_MAP+(0x122B00<<1))


typedef enum
{
    SECR2_MAU_2Gx2G = 0, //will not be applied in real projects
    SECR2_MAU_1Gx1G,
    SECR2_MAU_512Mx512M,
    SECR2_MAU_256Mx256M,
} SECR2_MAU_TYPE;
#endif


MS_PHY HAL_MsOS_MPool_PA2BA(MS_PHY u64PhyAddr)
{
     MS_PHY u64BusAddr = 0x0;

     // ba = pa + offset
     #if !defined(CONFIG_FRC)
     if( (u64PhyAddr >= HAL_MIU0_BASE) && (u64PhyAddr < HAL_MIU1_BASE) )     // MIU0
                u64BusAddr = u64PhyAddr - HAL_MIU0_BASE + HAL_MIU0_BUS_BASE;
     else if( (u64PhyAddr >= HAL_MIU1_BASE) && (u64PhyAddr < HAL_MIU2_BASE) )        // MIU1
                u64BusAddr = u64PhyAddr - HAL_MIU1_BASE + HAL_MIU1_BUS_BASE;
     else
                u64BusAddr = u64PhyAddr - HAL_MIU2_BASE + HAL_MIU2_BUS_BASE;    // MIU2
     #endif
     return u64BusAddr;
}

MS_PHY HAL_MsOS_MPool_VA2PA(MS_VIRT u64Virt)
{
//for aeon
#if (defined(MCU_AEON))
    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_PHY *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
    switch(u8MauVal)
    {
        case 0x87 : eSecR2MauType=SECR2_MAU_2Gx2G; break;
        case 0x85 : eSecR2MauType=SECR2_MAU_1Gx1G; break;
        case 0x83 : eSecR2MauType=SECR2_MAU_512Mx512M; break;
        case 0x81 : eSecR2MauType=SECR2_MAU_256Mx256M; break;
        default: eSecR2MauType=SECR2_MAU_1Gx1G; break;
    }

    if((eSecR2MauType==SECR2_MAU_1Gx1G)||(eSecR2MauType==SECR2_MAU_2Gx2G))
    {
        return ((MS_PHY)(u64Virt) & ~(0x80000000UL));
    }
    else
    {
        return ((MS_PHY)(u64Virt) & ~(0xC0000000UL));
    }
//for arm
#elif (defined(__arm__))

//for mboot
#if (defined(CONFIG_MBOOT))
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

//for chakra2
#else
    //===========================================
    // MIU0 (low 512MB) (VA) 0x0000:0000 ~ 0x1fff:ffff -> (PA) 0x0000:0000 ~ 0x1fff:ffff (cached)
    if ((0x00000000UL <= u64Virt) && (0x1fffffffUL >= u64Virt))
    {
        return (MS_PHY)(u64Virt & 0x1fffffffUL);
    }

    //===========================================
    // MIU0 (low 512MB) (VA) 0x2000:0000 ~ 0x3fff:fffff -> (PA) 0x0000:0000 ~ 0x1fff:ffff (uncached)
    if ((0x20000000UL <= u64Virt) && (0x3fffffffUL > u64Virt))
    {
        return (MS_PHY)(u64Virt & 0x1fffffffUL);
    }

    //===========================================
    // MIU1 (low 512MB) (VA) 0x4000:0000 ~ 0x5fff:ffff -> (PA) 0x8000:0000 ~ 0x9fff:ffff (cached)
    if ((0x40000000UL <= u64Virt) && (0x5fffffffUL >= u64Virt))
    {
        return (MS_PHY)((u64Virt & 0x1fffffffUL) + HAL_MIU1_BASE);
    }

    //===========================================
    // MIU1 (low 512MB) (VA) 0x6000:0000 ~ 0x7fff:ffff -> (PA) 0x8000:0000 ~ 0x9fff:ffff (uncached)
    if ((0x60000000UL <= u64Virt) && (0x7fffffffUL > u64Virt))
    {
        return (MS_PHY)((u64Virt & 0x1fffffffUL) + HAL_MIU1_BASE);
    }
#endif

// for mips
#else
#endif

    return 0xffffffffUL;
}

MS_VIRT HAL_MsOS_MPool_PA2KSEG0(MS_PHY u64Phys)
{
//for aeon
#if (defined(MCU_AEON))
    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_PHY *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
    switch(u8MauVal)
    {
        case 0x87 : eSecR2MauType=SECR2_MAU_2Gx2G; break;
        case 0x85 : eSecR2MauType=SECR2_MAU_1Gx1G; break;
        case 0x83 : eSecR2MauType=SECR2_MAU_512Mx512M; break;
        case 0x81 : eSecR2MauType=SECR2_MAU_256Mx256M; break;
        default: eSecR2MauType=SECR2_MAU_1Gx1G; break;
    }

    if(eSecR2MauType==SECR2_MAU_1Gx1G)
    {
        // MIU0 (1GB) - PA 0x0000:0000~0x4000:0000 -> (VA) 0x0000:0000~0x4000:0000 cached
        // MIU1 (1GB) - PA 0x4000:0000~0x8000:0000 -> (VA) 0x4000:0000~0x8000:0000 cached
        return (MS_VIRT)(u64Phys + 0x00000000UL);
    }
    else if(eSecR2MauType==SECR2_MAU_512Mx512M)
    {
        // MIU0 (512MB) - PA 0x0000:0000~0x2000:0000 -> (VA) 0x4000:0000~0x6000:0000 cached
        if((u64Phys>=0x00000000UL)&&(u64Phys<0x20000000UL))
        {
        	return (MS_VIRT)(u64Phys + 0x40000000UL);
        }

        // MIU1 (512MB) - PA 0x2000:0000~0x4000:0000 -> (VA) 0x6000:0000~0x8000:0000 cached
        if((u64Phys>=0x20000000UL)&&(u64Phys<0x40000000UL))
        {
        	return (MS_VIRT)(u64Phys + 0x40000000UL);
        }
    }
    else if(eSecR2MauType==SECR2_MAU_256Mx256M)
    {
        // MIU0 (256MB) - PA 0x0000:0000~0x1000:0000 -> (VA) 0x4000:0000~0x5000:0000 cached
        if((u64Phys>=0x00000000UL)&&(u64Phys<0x10000000UL))
        {
        	return (MS_VIRT)(u64Phys + 0x40000000UL);
        }

        // MIU1 (256MB) - PA 0x1000:0000~0x2000:0000 -> (VA) 0x5000:0000~0x6000:0000 cached
        if((u64Phys>=0x10000000UL)&&(u64Phys<0x20000000UL))
        {
        	return (MS_VIRT)(u64Phys + 0x40000000UL);
        }
    }
    else //SECR2_MAU_2Gx2G
    {
    	return 0xffffffffUL;
    }
//for arm
#elif (defined(__arm__))

#if (defined(CONFIG_MBOOT))
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

//for chakra2
#else
    //===========================================
    // MIU0 (low 512MB) - (PA) 0x0000:0000 ~ 0x1fff:ffff -> (VA) 0x0000:0000 ~ 0x1fff:ffff (cached)
    if ((0x00000000UL <= u64Phys) && (0x1fffffffUL >= u64Phys))
    {
        return (MS_VIRT)u64Phys;
    }

    //===========================================
    // MIU1 (low 512MB) - (PA) 0x8000:0000 ~ 0x9fff:ffff -> (VA) 0x4000:0000 ~ 0x5fff:ffff (cached)
    if ((HAL_MIU1_BASE <= u64Phys) && ((HAL_MIU1_BASE + 0x1fffffffUL) >= u64Phys))
    {
        return (MS_VIRT)((u64Phys - HAL_MIU1_BASE) + 0x40000000UL);
    }
#endif

//for mips
#else
#endif
    return 0xffffffffUL;
}

MS_VIRT HAL_MsOS_MPool_PA2KSEG1(MS_PHY u64Phys)
{
//for aeon
#if (defined(MCU_AEON))
    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_PHY *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
    switch(u8MauVal)
    {
        case 0x87 : eSecR2MauType=SECR2_MAU_2Gx2G; break;
        case 0x85 : eSecR2MauType=SECR2_MAU_1Gx1G; break;
        case 0x83 : eSecR2MauType=SECR2_MAU_512Mx512M; break;
        case 0x81 : eSecR2MauType=SECR2_MAU_256Mx256M; break;
        default: eSecR2MauType=SECR2_MAU_1Gx1G; break;
    }

    if(eSecR2MauType==SECR2_MAU_1Gx1G)
    {
        // MIU0 (1GB) - PA 0x0000:0000~0x4000:0000 -> (VA) 0x8000:0000~0xC000:0000 non-cached
        // MIU1 (1GB) - PA 0x4000:0000~0x8000:0000 -> (VA) 0xC000:0000~0x10000:0000 non-cached
        return (MS_VIRT)(u64Phys + 0x80000000UL);
    }
    else if(eSecR2MauType==SECR2_MAU_512Mx512M)
    {
        //512Mx512M
        // MIU0 (512MB) - PA 0x0000:0000~0x2000:0000 -> (VA) 0xC000:0000~0xE000:0000 non-cached
        if((u64Phys>=0x00000000UL)&&(u64Phys<0x20000000UL))
        {
        	return (MS_VIRT)(u64Phys + 0xC0000000UL);
        }

        // MIU1 (512MB) - PA 0x2000:0000~0x4000:0000 -> (VA) 0xE000:0000~0x10000:0000 non-cached
        if((u64Phys>=0x20000000UL)&&(u64Phys<0x40000000UL))
        {
        	return (MS_VIRT)(u64Phys + 0xC0000000UL);
        }
    }
    else if(eSecR2MauType==SECR2_MAU_256Mx256M)
    {
        // MIU0 (256MB) - PA 0x0000:0000~0x1000:0000 -> (VA) 0xC000:0000~0xD000:0000 non-cached
        if((u64Phys>=0x00000000UL)&&(u64Phys<0x10000000UL))
        {
        	return (MS_VIRT)(u64Phys + 0xC0000000UL);
        }

        // MIU1 (256MB) - PA 0x1000:0000~0x2000:0000 -> (VA) 0xD000:0000~0xE000:0000 non-cached
        if((u64Phys>=0x10000000UL)&&(u64Phys<0x20000000))
        {
        	return (MS_VIRT)(u64Phys + 0xC0000000UL);
        }
    }
    else //SECR2_MAU_2Gx2G
    {
    	return 0xffffffffUL;
    }
//for arm
#elif (defined(__arm__))

#if (defined(CONFIG_MBOOT))
    //===========================================
    // MIU0 (0~1024MB) - (PA) 0x0000:0000 ~ 0x3fff:ffff -> (VA) 0x6000:0000 ~ 0x9fff:ffff (uncached)
    if ((0x00000000UL <= u64Phys) && (0x3fffffffUL >= u64Phys))
    {
        return (MS_VIRT)((u64Phys & 0x3fffffffUL) + HAL_MIU0_BUS_BASE + 0x40000000UL);
    }

    //===========================================
    // MIU1 (0~1024MB) - (PA) 0x8000:0000 ~ 0xBfff:ffff -> (VA) 0xc000:0000 ~ 0xFfff:ffff (uncached)
    if ((HAL_MIU1_BASE <= u64Phys) && ((HAL_MIU1_BASE + 0x3fffffffUL) >= u64Phys))
    {
        return (MS_VIRT)((u64Phys - HAL_MIU1_BASE) + HAL_MIU1_BUS_BASE + 0x20000000UL);
    }

//for chakra2
#else
    //===========================================
    // MIU0 (low 512MB) - (PA) 0x0000:0000 ~ 0x1fff:ffff -> (VA) 0x2000:0000 ~ 0x3fff:ffff (uncached)
    if ((0x00000000UL <= u64Phys) && (0x1fffffffUL >= u64Phys))
    {
        return (MS_VIRT)(u64Phys + 0x20000000UL);
    }

    //===========================================
    // MIU1 (low 512MB) - (PA) 0x8000:0000 ~ 0x9fff:ffff -> (VA) 0x6000:0000 ~ 0x7fff:ffff (uncached)
    if ((HAL_MIU1_BASE <= u64Phys) && ((HAL_MIU1_BASE + 0x1fffffffUL) >= u64Phys))
    {
        return (MS_VIRT)((u64Phys - HAL_MIU1_BASE) + 0x60000000UL);
    }
#endif

//for mips
#else
#endif
    return 0xffffffffUL;
}
