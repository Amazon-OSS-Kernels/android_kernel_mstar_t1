/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Inlucde Fils
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <drvMIU.h>
#include <MsTypes.h>

#include <common.h>
#include <MsDebug.h>
#include <miu/MsDrvMiu.h>
#include <mhal_miu.h>
#include <MsStr.h>
#define MIU_ALIGN(_val_,_shift_) (((_val_) >> _shift_) << _shift_)
static MS_U8 miuInit=0;
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Internal Functions
///
///////////////////////////////////////////////////////////////////////////////////////////////////

U8 _MIU_ReadByte(U32 u32RegOffset)
{
    return ((volatile U8*)(MIUREG_BASE))[(u32RegOffset << 1) - (u32RegOffset & 1)];
}

U16 _MIU_Read2Bytes(U32 u32RegOffset)
{
    return ((volatile U16*)(MIUREG_BASE))[u32RegOffset];
}

void _MIU_WriteByte(U32 u32RegOffset, U8 u8Val)
{
    ((volatile U8*)(MIUREG_BASE))[(u32RegOffset << 1) - (u32RegOffset & 1)] = u8Val;
}

void _MIU_Write2Bytes(U32 u32RegOffset, U16 u16Val)
{
    ((volatile U16*)(MIUREG_BASE))[u32RegOffset] = u16Val;
}

void _MIU_WriteBytesBit(U32 u32RegOffset, U8 bEnable, U16 u16Mask)
{
    U16 val = _MIU_Read2Bytes(u32RegOffset);
    val = (bEnable) ? (val | u16Mask) : (val & ~u16Mask);
    _MIU_Write2Bytes(u32RegOffset, val);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Global Functions
///
///////////////////////////////////////////////////////////////////////////////////////////////////
MS_U64 BA2PA(MS_U64 u64BusAddr)
{
	  MS_U64 u64PhyAddr = 0;
    u64PhyAddr = HAL_MIU_BA2PA(u64BusAddr);
    if(u64PhyAddr == 0)
    {
        UBOOT_ERROR("BusAddr = 0x%llx \n",u64BusAddr);
    }
    return u64PhyAddr;
}

MS_U64 PA2BA(MS_U64 u64PhyAddr)
{
    return HAL_MIU_PA2BA(u64PhyAddr);
}

EN_MIUID MsApi_Miu_GetID(MS_U64 uPhyAddrs)
{
    unsigned char enMiuID;

    if(uPhyAddrs < CONFIG_SYS_MIU_INTERVAL)
    {
        enMiuID = E_MIU_ID_0;
    }
    else if(uPhyAddrs < CONFIG_SYS_MIU2_INTERVAL)
    {
        enMiuID = E_MIU_ID_1;
    }
    else
    {
        enMiuID = E_MIU_ID_2;
    }
    return enMiuID;
}

MIU_DDR_SIZE dram_size_to_enum(MS_U32 u32DramSize)
{
    switch(u32DramSize)
    {
        case 0x02000000:
            return E_MIU_DDR_32MB;
        case 0x04000000:
            return E_MIU_DDR_64MB;
        case 0x08000000:
            return E_MIU_DDR_128MB;
        case 0x10000000:
            return E_MIU_DDR_256MB;
        case 0x20000000:
            return E_MIU_DDR_512MB;
        case 0x40000000:
            return E_MIU_DDR_1024MB;
        case 0x80000000:
            return E_MIU_DDR_2048MB;
        default:
            printf("[PROTECT ERROR], not expected dram size: 0x%X\n", (unsigned int)u32DramSize);
            return -1;
    }
}

char* dram_enum_to_StrSize(MIU_DDR_SIZE u32DramEnum)
{
    switch(u32DramEnum)
    {
        case E_MIU_DDR_32MB:
            return "32";
        case E_MIU_DDR_64MB:
            return "64";
        case E_MIU_DDR_128MB:
            return "128";
        case E_MIU_DDR_256MB:
            return "256";
        case E_MIU_DDR_512MB:
            return "512";
        case E_MIU_DDR_1024MB:
            return "1024";
        case E_MIU_DDR_2048MB:
            return "2048";
        default:
            printf("[PROTECT ERROR], not expected dram enum: 0x%X\n", (unsigned int)u32DramEnum);
            return NULL;
    }
}

char* MsDrv_GetMIUSize(MS_U8 MiuID)
{
    char *pDramSize = NULL;
    UBOOT_TRACE("IN\n");

//    MIU_DDR_SIZE enDramEnum=0;
//
//    if(MDrv_MIU_Dram_ReadSize(MiuID,&enDramEnum)==TRUE)
//    {
//        UBOOT_DEBUG("enDramEnum = %d \n",enDramEnum);
//        pDramSize = dram_enum_to_StrSize(enDramEnum);
//    }
//    UBOOT_DEBUG("MIU%d SIZE : %s MB\n",MiuID,pDramSize);

    pDramSize = NULL;

    UBOOT_TRACE("OK\n");
    return pDramSize;
}


U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum)
{
    u8 u8MIUGpPriority = 0, idx=0;
	U32 u32RegAddr = 0;
	UBOOT_TRACE("IN\n");

    if(bMIU1==0)
        {
            u32RegAddr =MIU0_REG_BASE+REG_MIU_GP_PRIORITY;
        }
        else if(bMIU1==1)
        {
            u32RegAddr =MIU1_REG_BASE+REG_MIU_GP_PRIORITY;
        }
        else if(bMIU1==2)
        {
            u32RegAddr =MIU2_REG_BASE+REG_MIU_GP_PRIORITY;
        }

    /*u8MIUGpPriority=u8MIUGpPriority;*/
    /*idx=idx;*/
	if (u8GpNum > HAL_MIU_GP_PRIORITY_NUM)
	{
		UBOOT_ERROR("%s: exced max group number!\n", __FUNCTION__);
		return FALSE;
	}

    for (idx = 0; idx < HAL_MIU_GP_PRIORITY_NUM; idx++)
    {
        if (au8GpPriority[idx] > (HAL_MIU_GP_PRIORITY_NUM-1))
        {
            UBOOT_ERROR("%s: wrong group number!\n", __FUNCTION__);
            return FALSE;
        }
        u8MIUGpPriority |= (au8GpPriority[idx] << (idx*2));
    }
    //printf("Set MIU%u group priority:%x.....\n", bMIU1, u8MIUGpPriority);
    //A3 Revision 1 can not modify miu priority
    if(((volatile U16*)(PM_TOP_BASE))[CHIP_ID]== 0x47 && ((volatile U8*)(PM_TOP_BASE))[((CHIP_VER<<1)-(CHIP_VER&1))] <= 0x01)
        return TRUE;

	_MIU_WriteBytesBit(u32RegAddr,DISABLE, BIT8);
	_MIU_WriteByte(u32RegAddr, u8MIUGpPriority);
	_MIU_WriteBytesBit(u32RegAddr,ENABLE, BIT8);
	udelay(2);
	_MIU_WriteBytesBit(u32RegAddr,DISABLE, BIT8);
	UBOOT_TRACE("OK\n");

	return TRUE;
}

const U16 client_gp[9] = {REG_MIUSEL_GP0, REG_MIUSEL_GP1, REG_MIUSEL_GP2, REG_MIUSEL_GP3, REG_MIUSEL_GP4, REG_MIUSEL_GP5, REG_MIUSEL_GP6, REG_MIUSEL_GP7, 0};

U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum)
{
	if (u8GpNum > HAL_MIU_GP_NUM)
	{
		UBOOT_ERROR("%s: exced max group number!\n", __FUNCTION__);
		return FALSE;
	}

    return HAL_MIU_SelectMIU_UBoot(bMIU1, au16SelMiu, u8GpNum);
}

void MsDrv_MiuInit(void)
{
		UBOOT_TRACE("IN\n");
		if(miuInit==0)
		{
				UBOOT_DEBUG("do MDrv_MIU_Init\n");
				MDrv_MIU_Init();
				miuInit=1;
		}
		UBOOT_TRACE("OK\n");
}

void MsDrv_kernelProtect(MS_U8  u8Blockx,MS_U64 u64Lx1Start, MS_U64 u64Lx1End)
{
	UBOOT_TRACE("IN\n");
#ifdef SUPPORT_MIU_NEW_PROTECT_API
    MsDrv_MIU_ProtectEx(u8Blockx, HAL_MIU_GetKernelProtectClientsEx(), u64Lx1Start, u64Lx1End, ENABLE);
#else
    MsDrv_MIU_Protect(u8Blockx, HAL_MIU_GetKernelProtectClients(), u64Lx1Start, u64Lx1End, ENABLE);
#endif
	UBOOT_TRACE("OK\n");
}

void MsDrv_NuttxProtect(MS_U8  u8Blockx,MS_U64 u64NuttxStart, MS_U64 u64NuttxEnd)
{
	  UBOOT_TRACE("IN\n");

    MsDrv_MiuInit();

    MS_U8 u8MIUProtect_ID[16]={0};

    // ==== Linux kernel ID Protect
    UBOOT_DEBUG("Nuttx Protect Index[%d] start:[0x%llx],end:[0x%llx] \n",u8Blockx,u64NuttxStart,u64NuttxEnd);
    memset(u8MIUProtect_ID,0,sizeof(u8MIUProtect_ID));

    u8MIUProtect_ID[0] = MIU_CLIENT_SECURE_R2_RW; //MIPS W

    MsDrv_MIU_Protect(u8Blockx, &u8MIUProtect_ID[0], u64NuttxStart, u64NuttxEnd, ENABLE);

	  UBOOT_TRACE("OK\n");
}

void MsDrv_PMProtect(MS_U8  u8Blockx,MS_U64 u64Start, MS_U64 u64End)
{
	 UBOOT_TRACE("IN\n");

    MsDrv_MiuInit();

    MS_U8 u8MIUProtect_ID[16]={0};

    // ==== Linux kernel ID Protect
    UBOOT_DEBUG("PM Protect Index[%d] LX[0x%llx][0x%llx] \n",u8Blockx,u64Start,u64End);

    u8MIUProtect_ID[0] = MIU_CLIENT_PM51_RW; //PM RW

    MsDrv_MIU_Protect(u8Blockx, &u8MIUProtect_ID[0], u64Start, u64End, DISABLE);

    MsDrv_MIU_Protect(u8Blockx, &u8MIUProtect_ID[0], u64Start, u64End, ENABLE);

	UBOOT_TRACE("OK\n");
}

MS_BOOL MsDrv_MIU_Protect(MS_U8  u8Blockx, MS_U8  *pu8ProtectId, MS_U64 u64Start, MS_U64 u64End, MS_BOOL bSetFlag)
{
    MS_BOOL isProtect=FALSE;
    MS_U32 u32PageShift;
	UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("MsDrv_MIU_Protect Index[%d] LX[0x%llx][0x%llx] \n",u8Blockx,u64Start,u64End);

    if(!MDrv_MIU_ProtectAlign(&u32PageShift))
    {
        UBOOT_ERROR("ERROR MDrv_MIU_ProtectAlign FAIL \n");
    }
    if(u64Start>=0x100000000)
    {
        UBOOT_ERROR("PA >100000000 need fix \n");
    }
    else
    {
        isProtect=MDrv_MIU_Protect(u8Blockx, pu8ProtectId, MIU_ALIGN(u64Start, u32PageShift), MIU_ALIGN(u64End, u32PageShift), bSetFlag);
    }
    if(!isProtect)
    {
        UBOOT_ERROR("ERROR MDrv_MIU_Protect FAIL \n");
    }
    UBOOT_TRACE("OK\n");

    return isProtect;
}

MS_BOOL MsDrv_MIU_ProtectEx(MS_U8  u8Blockx, MS_U32  *pu32ProtectId, MS_U64 u64Start, MS_U64 u64End, MS_BOOL bSetFlag)
{
#ifdef SUPPORT_MIU_NEW_PROTECT_API
    MS_BOOL isProtect=FALSE;
    MS_U32 u32PageShift;
	UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("MsDrv_MIU_Protect Index[%d] LX[0x%llx][0x%llx] \n",u8Blockx,u64Start,u64End);

    if(!MDrv_MIU_ProtectAlign(&u32PageShift))
    {
        UBOOT_ERROR("ERROR MDrv_MIU_ProtectAlign FAIL \n");
    }

    isProtect=MDrv_MIU_ProtectEx(u8Blockx, pu32ProtectId, MIU_ALIGN(u64Start, u32PageShift), MIU_ALIGN(u64End, u32PageShift), bSetFlag);

    if(!isProtect)
    {
        UBOOT_ERROR("ERROR MDrv_MIU_Protect FAIL \n");
    }

	UBOOT_TRACE("OK\n");

    return isProtect;
#else
    return FALSE;
#endif
}

#ifdef CONFIG_MSTAR_FAST_STR

void MSDrv_FSTR_MiuBackup(ST_MIU_INFO * stMiuInfo)
{
    stMiuInfo->u32MiuPriority[0]=_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIU_GP_PRIORITY);
    stMiuInfo->u32MiuSel[0][0]=_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP0);
    stMiuInfo->u32MiuSel[0][1]=_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP1);
    stMiuInfo->u32MiuSel[0][2]=_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP2);
    stMiuInfo->u32MiuSel[0][3]=_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP3);
    stMiuInfo->u32MiuSel[0][4]=_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP4);
    stMiuInfo->u32MiuSel[0][5]=_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP5);
    stMiuInfo->u32MiuSel[0][6]=Read2Byte(0x137F20);
    UBOOT_DEBUG("\n*****u32MiuPriority=[0x%x] [0x%x]\n",stMiuInfo->u32MiuPriority[0],_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIU_GP_PRIORITY));
    UBOOT_DEBUG("*****u32MiuSel[0]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[0][0],_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP0));
    UBOOT_DEBUG("*****u32MiuSel[1]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[0][1],_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP1));
    UBOOT_DEBUG("*****u32MiuSel[2]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[0][2],_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP2));
    UBOOT_DEBUG("*****u32MiuSel[3]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[0][3],_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP3));
    UBOOT_DEBUG("*****u32MiuSel[4]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[0][4],_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP4));
    UBOOT_DEBUG("*****u32MiuSel[5]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[0][5],_MIU_Read2Bytes(MIU0_REG_BASE+REG_MIUSEL_GP5));
    UBOOT_DEBUG("*****137F20 u32MiuSel[8]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[0][6],Read2Byte(0x137F20));
#ifndef CONFIG_MIU1_DRAM_NONE
    stMiuInfo->u32MiuPriority[1]=_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIU_GP_PRIORITY);
    stMiuInfo->u32MiuSel[1][0]=_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP0);
    stMiuInfo->u32MiuSel[1][1]=_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP1);
    stMiuInfo->u32MiuSel[1][2]=_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP2);
    stMiuInfo->u32MiuSel[1][3]=_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP3);
    stMiuInfo->u32MiuSel[1][4]=_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP4);
    stMiuInfo->u32MiuSel[1][5]=_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP5);
    stMiuInfo->u32MiuSel[1][6]=Read2Byte(0x137F22);
    UBOOT_DEBUG("\n*****u32MiuPriority=[0x%x] [0x%x]\n",stMiuInfo->u32MiuPriority[1],_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIU_GP_PRIORITY));
    UBOOT_DEBUG("*****u32MiuSel[0]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[1][0],_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP0));
    UBOOT_DEBUG("*****u32MiuSel[1]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[1][1],_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP1));
    UBOOT_DEBUG("*****u32MiuSel[2]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[1][2],_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP2));
    UBOOT_DEBUG("*****u32MiuSel[3]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[1][3],_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP3));
    UBOOT_DEBUG("*****u32MiuSel[4]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[1][4],_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP4));
    UBOOT_DEBUG("*****u32MiuSel[5]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[1][5],_MIU_Read2Bytes(MIU1_REG_BASE+REG_MIUSEL_GP5));
    UBOOT_DEBUG("*****137F22 u32MiuSel[8]=[0x%x]  [0x%x] \n",stMiuInfo->u32MiuSel[1][6],Read2Byte(0x137F22));
#endif

#ifndef CONFIG_MIU2_DRAM_NONE
    stMiuInfo->u32MiuPriority[2]=_MIU_Read2Bytes(MIU2_REG_BASE+REG_MIU_GP_PRIORITY);
    stMiuInfo->u32MiuSel[2][0]=_MIU_Read2Bytes(MIU2_REG_BASE+REG_MIUSEL_GP0);
    stMiuInfo->u32MiuSel[2][1]=_MIU_Read2Bytes(MIU2_REG_BASE+REG_MIUSEL_GP1);
    stMiuInfo->u32MiuSel[2][2]=_MIU_Read2Bytes(MIU2_REG_BASE+REG_MIUSEL_GP2);
    stMiuInfo->u32MiuSel[2][3]=_MIU_Read2Bytes(MIU2_REG_BASE+REG_MIUSEL_GP3);
    stMiuInfo->u32MiuSel[2][4]=_MIU_Read2Bytes(MIU2_REG_BASE+REG_MIUSEL_GP4);
    stMiuInfo->u32MiuSel[2][5]=_MIU_Read2Bytes(MIU2_REG_BASE+REG_MIUSEL_GP5);
#endif
}
#endif

