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

#define RIU_MAP 0xFA000000
#define R2_MAU_BASE     (RIU_MAP+(0x122B00<<1))

typedef enum
{
    SECR2_MAU_2Gx2G = 0, //will not be applied in real projects
    SECR2_MAU_1Gx1G,
    SECR2_MAU_512Mx512M,
    SECR2_MAU_256Mx256M,
} SECR2_MAU_TYPE;

MS_U32 HAL_MsOS_MPool_VA2PA(MS_U32 u32Virt)
{
//for aeon
    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_U32 *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
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
        return ((MS_U32)(u32Virt) & ~(0x80000000));
    }
    else
    {
        return ((MS_U32)(u32Virt) & ~(0xC0000000));
    }

    
    return 0xffffffff;
}

MS_U32 HAL_MsOS_MPool_PA2KSEG0(MS_U32 u32Phys)
{
//for aeon
    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_U32 *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
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
        return (u32Phys + 0x00000000);
    }
    else if(eSecR2MauType==SECR2_MAU_512Mx512M)
    {
        // MIU0 (512MB) - PA 0x0000:0000~0x2000:0000 -> (VA) 0x4000:0000~0x6000:0000 cached
        if((u32Phys>=0x00000000)&&(u32Phys<0x20000000))
        {
        	return (u32Phys + 0x40000000);
        }

        // MIU1 (512MB) - PA 0x2000:0000~0x4000:0000 -> (VA) 0x6000:0000~0x8000:0000 cached
        if((u32Phys>=0x20000000)&&(u32Phys<0x40000000))
        {
        	return (u32Phys + 0x40000000);
        }
    }
    else if(eSecR2MauType==SECR2_MAU_256Mx256M)
    {
        // MIU0 (256MB) - PA 0x0000:0000~0x1000:0000 -> (VA) 0x4000:0000~0x5000:0000 cached
        if((u32Phys>=0x00000000)&&(u32Phys<0x10000000))
        {
        	return (u32Phys + 0x40000000);
        }

        // MIU1 (256MB) - PA 0x1000:0000~0x2000:0000 -> (VA) 0x5000:0000~0x6000:0000 cached
        if((u32Phys>=0x10000000)&&(u32Phys<0x20000000))
        {
        	return (u32Phys + 0x40000000);
        }
    }
    else //SECR2_MAU_2Gx2G
    {
    	return 0xffffffff;
    }

    return 0xFFFFFFFF;
}

MS_U32 HAL_MsOS_MPool_PA2KSEG1(MS_U32 u32Phys)
{

    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_U32 *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
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
        return (u32Phys + 0x80000000);
    }
    else if(eSecR2MauType==SECR2_MAU_512Mx512M)
    {
        //512Mx512M
        // MIU0 (512MB) - PA 0x0000:0000~0x2000:0000 -> (VA) 0xC000:0000~0xE000:0000 non-cached
        if((u32Phys>=0x00000000)&&(u32Phys<0x20000000))
        {
        	return (u32Phys + 0xC0000000);
        }

        // MIU1 (512MB) - PA 0x2000:0000~0x4000:0000 -> (VA) 0xE000:0000~0x10000:0000 non-cached
        if((u32Phys>=0x20000000)&&(u32Phys<0x40000000))
        {
        	return (u32Phys + 0xC0000000);
        }
    }
    else if(eSecR2MauType==SECR2_MAU_256Mx256M)
    {
        // MIU0 (256MB) - PA 0x0000:0000~0x1000:0000 -> (VA) 0xC000:0000~0xD000:0000 non-cached
        if((u32Phys>=0x00000000)&&(u32Phys<0x10000000))
        {
        	return (u32Phys + 0xC0000000);
        }

        // MIU1 (256MB) - PA 0x1000:0000~0x2000:0000 -> (VA) 0xD000:0000~0xE000:0000 non-cached
        if((u32Phys>=0x10000000)&&(u32Phys<0x20000000))
        {
        	return (u32Phys + 0xC0000000);
        }
    }
    else //SECR2_MAU_2Gx2G
    {
    	return 0xffffffff;
    }

    return 0xFFFFFFFF;
}
