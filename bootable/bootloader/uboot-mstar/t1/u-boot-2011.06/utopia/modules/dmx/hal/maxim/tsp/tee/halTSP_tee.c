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
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halTSP_tee.c
// @brief  Transport Stream Processer (TSP) HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "regTSP_tee.h"
#include "halTSP_tee.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT                      _virtRegBase = 0;

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define TSP_TSP0_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0x2A00 + ((addr)<<2))))
#define TSP_TSP1_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0x2C00 + ((addr)<<2))))
#define TSP_MMFI_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0x3800 + ((addr)<<2))))

//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSP_Tee_SetBank(MS_VIRT virtBankAddr)
{
    _virtRegBase = virtBankAddr;

    return TRUE;
}

MS_BOOL HAL_TSP_Tee_Set_FWBuf(MS_PHY phyAddr, MS_U32 u32Size)
{
    MS_U32 u32FwAddr = ((MS_U32)phyAddr >> MIU_BUS) >> TSP_DNLD_ADDR_ALI_SHIFT;
    MS_U32 u32FwSize = TSP_QMEM_SIZES;

    //printf("[%s][%d] u32FwAddr %x, u32FwSize %x \n", __FUNCTION__, __LINE__, (int)u32FwAddr, (int)u32FwSize);

    TSP_TSP0_REG(REG_TSP0_FW_DMA_ADDR_L) = ((MS_U16)u32FwAddr) & 0xFFFF;
    TSP_TSP1_REG(REG_TSP1_FW_DMA_ADDR_H) = (MS_U16)(u32FwAddr >> 16) & TSP_FW_DMA_ADDR_H_MASK;
    TSP_TSP0_REG(REG_TSP0_FW_DMA_NUM) = (MS_U16)u32FwSize;

    //lock down register
    TSP_TSP1_REG(REG_TSP1_ONEWAY) = TSP_TSP1_REG(REG_TSP1_ONEWAY) | TSP_FW_ONEWAY;

    return TRUE;
}

MS_BOOL HAL_TSP_Tee_Set_VQBuf(MS_PHY phyAddr, MS_U32 u32Size)
{
    MS_U32 u32VQSize = 0, u32UnitSize = 0;
    MS_PHY phyVQBufStart = phyAddr, phyMiuAddr = 0;
    MS_U8 u8ii = 0;

    u32VQSize = ((u32Size >> MIU_BUS) / TSP_VQ_NUM) << MIU_BUS; //miu alignment
    u32UnitSize = u32VQSize / TSP_VQ_PITCH;

    //printf("[%s][%d] phyAddr %x, u32Size %x \n", __FUNCTION__, __LINE__, (int)phyAddr, (int)u32Size);
    for(u8ii = 0; u8ii < TSP_VQ_NUM; u8ii++)
    {
        phyMiuAddr = phyVQBufStart >> MIU_BUS;
        if(u8ii == 0)
        {
            TSP_TSP1_REG(REG_TSP1_VQ0_BASE_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ0_BASE_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ0_SIZE) = (MS_U16)(u32UnitSize & 0xFFFF);
        }
        else if(u8ii == 1)
        {
            TSP_TSP1_REG(REG_TSP1_VQ1_BASE_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ1_BASE_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ1_SIZE) = (MS_U16)(u32UnitSize & 0xFFFF);
        }
        else if(u8ii == 2)
        {
            TSP_TSP1_REG(REG_TSP1_VQ2_BASE_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ2_BASE_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ2_SIZE) = (MS_U16)(u32UnitSize & 0xFFFF);
        }
        else if(u8ii == 3)
        {
            TSP_TSP1_REG(REG_TSP1_VQ3_BASE_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ3_BASE_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ3_SIZE) = (MS_U16)(u32UnitSize & 0xFFFF);
        }

        phyVQBufStart += u32VQSize;
    }

    return TRUE;

}

MS_BOOL HAL_TSP_Tee_Set_PvrBuf(MS_U8 u8PvrId, MS_PHY phyAddr0, MS_U32 u32Size0, MS_PHY phyAddr1, MS_U32 u32Size1)
{
    MS_PHY phyEnd = phyAddr0 + u32Size0;
    MS_PHY phyMiuAddr = 0;

    if(u8PvrId == 0)
    {
        phyMiuAddr = phyAddr0 >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR_HEAD1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR_HEAD1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR_TAIL1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR_TAIL1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyEnd = phyAddr1 + u32Size1;
        phyMiuAddr = phyAddr1 >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR_HEAD2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR_HEAD2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR_TAIL2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR_TAIL2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
    }
    else if(u8PvrId == 1)
    {
        phyMiuAddr = phyAddr0 >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR1_HEAD1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR1_HEAD1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR1_TAIL1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR1_TAIL1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyEnd = phyAddr1 + u32Size1;
        phyMiuAddr = phyAddr1 >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR1_HEAD2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR1_HEAD2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR1_TAIL2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR1_TAIL2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSP_Tee_Get_PVRWriteAddr(MS_U8 u8PvrId, MS_PHY *pphyAddr)
{
    MS_U16 u16addr = 0, u16addr1 = 0;

    *pphyAddr = 0;

    if(u8PvrId >= TSP_PVR_ENG_NUM)
        return FALSE;

    TSP_TSP1_REG(REG_TSP1_PVR_CFG) = TSP_TSP1_REG(REG_TSP1_PVR_CFG) & ~REG_TSP1_CH_BW_WP_LD;
    if(u8PvrId == 0)
    {
        u16addr = TSP_TSP0_REG(REG_TSP0_PVR_MID1_L);
        u16addr1 = TSP_TSP0_REG(REG_TSP0_PVR_MID1_H);
        *pphyAddr = (((MS_U32)u16addr) & 0xFFFF) + ((((MS_U32)u16addr1) << 16) & 0xFFFF0000);
        *pphyAddr <<= MIU_BUS;
    }
    else if(u8PvrId == 1)
    {
        u16addr = TSP_TSP0_REG(REG_TSP0_PVR1_MID1_L);
        u16addr1 = TSP_TSP0_REG(REG_TSP0_PVR1_MID1_H);
        *pphyAddr = (((MS_U32)u16addr) & 0xFFFF) + ((((MS_U32)u16addr1) << 16) & 0xFFFF0000);
        *pphyAddr <<= MIU_BUS;
    }
    TSP_TSP1_REG(REG_TSP1_PVR_CFG) = TSP_TSP1_REG(REG_TSP1_PVR_CFG) | REG_TSP1_CH_BW_WP_LD;

    return TRUE;
}

MS_U32 HAL_TSP_Tee_FilePath2Tsif_Mapping(MS_U8 u8FileEng)
{
    return (((MS_U32)u8FileEng) & 0xFF);
}

MS_BOOL HAL_TSP_Tee_Set_FileinBuf(MS_U8 u8EngId, MS_PHY phyAddr, MS_U32 u32Size)
{
    switch(u8EngId)
    {
        case 0:
            TSP_TSP0_REG(REG_TSP0_FILE_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_TSP0_REG(REG_TSP0_FILE_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            TSP_TSP0_REG(REG_TSP0_FILE_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_TSP0_REG(REG_TSP0_FILE_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            break;
        case 1:
        case 2:
            return HAL_TSP_Tee_Set_MMFIBuf(u8EngId-1, phyAddr, u32Size);
        default:
            return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSP_Tee_Set_MMFIBuf(MS_U8 u8Id, MS_PHY phyAddr, MS_U32 u32Size)
{
    switch(u8Id)
    {
        case 0:
            TSP_MMFI_REG(REG_MMFI_FILE_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI_FILE_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI_FILE_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI_FILE_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            break;
        case 1:
            TSP_MMFI_REG(REG_MMFI1_FILE_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI1_FILE_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI1_FILE_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI1_FILE_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}
