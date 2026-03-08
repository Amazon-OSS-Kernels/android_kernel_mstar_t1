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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file    osalGE.c
// @brief  Graphic Engine (GE) OSAL
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "MsTypes.h"
#include "osalGE.h"
#include "drvMMIO.h"
#ifdef MSOS_TYPE_NOS

extern void mhal_dcache_flush(MS_U32 u32Base, MS_U32 u32Size);

#define GE_DCACHE_FLUSH mhal_dcache_flush
#else
#include "MsOS.h"

#define GE_DCACHE_FLUSH MsOS_Dcache_Flush
#endif

#if defined(MCU_MIPS_34K) || defined(MCU_MIPS_74K) || defined(MCU_MIPS_1004K)
#define REG_BASE_MAP    (0xBF200000)
#elif (defined(MCU_AEON))
#define REG_BASE_MAP    (0xA0000000)
#else   //mips 4ke
#define REG_BASE_MAP    (0xBF800000)
#endif

MS_BOOL OSAL_GE_GetMapBase(MS_VIRT *VAddr, MS_PHY *puSize)
{
    if( !MDrv_MMIO_GetBASE(VAddr, puSize, MS_MODULE_GE))
        return FALSE;

    return TRUE;
}

MS_BOOL OSAL_GE_GetMapBase2(MS_VIRT *VAddr, MS_PHY *puSize)
{
    //get chip bank for identify chip version
    if( !MDrv_MMIO_GetBASE(VAddr, puSize, MS_MODULE_PM))
        return FALSE;

    return TRUE;
}

// Mutex
MS_S32 OSAL_GE_MutexCreate(MS_U8* pu8Name)
{
    return 0;
}

MS_BOOL OSAL_GE_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    return TRUE;
}

MS_BOOL OSAL_GE_MutexRelease(MS_S32 s32MutexId)
{
    return TRUE;
}

MS_BOOL OSAL_GE_Dcache_Flush( MS_VIRT VABase, MS_U32 u32Size )
{
    GE_DCACHE_FLUSH(VABase, u32Size);
    return TRUE;
}
#if 0       // No IRQ supported in GE
void OSAL_GE_IntAttach(OSAL_GE_IsrProc Proc, MS_U8 u8Ch)
{
#ifdef MSOS_TYPE_NOS
    MsOS_AttachInterrupt(E_INT_IRQ_BDMA_CH0+u8Ch, (InterruptCb)Proc);
#else
 //   MsOS_AttachInterrupt(E_INT_IRQ_BDMA_CH0+u8Ch, Proc);
#endif
}

void OSAL_GE_IntDetach(MS_U8 u8Ch)
{
#ifdef MSOS_TYPE_NOS
    MsOS_DetachInterrupt(E_INT_IRQ_BDMA_CH0+u8Ch);
#else
    MsOS_DetachInterrupt((InterruptNum)(E_INT_IRQ_BDMA_CH0+u8Ch));
#endif
}
#endif

