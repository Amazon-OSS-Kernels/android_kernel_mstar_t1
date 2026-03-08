/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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
/// @file   Mdrvl_miu.c
/// @brief  MIU Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/spinlock.h>
#include "mdrv_miu.h"
#include "mhal_miu.h"

//-------------------------------------------------------------------------------------------------
//  Function prototype with weak symbol 
//-------------------------------------------------------------------------------------------------

#if defined(CONFIG_MSTAR_MONET) || defined(CONFIG_MSTAR_MESSI) || defined(CONFIG_MSTAR_MASERATI) || defined(CONFIG_MSTAR_MANHATTAN)
    MS_BOOL HAL_MIU_Protect(MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_PHY phy64Start, MS_PHY phy64End, MS_BOOL bSetFlag); __attribute__((weak)); 
#else
    MS_BOOL HAL_MIU_Protect(MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_U32 u32BusStart, MS_U32 u32BusEnd, MS_BOOL bSetFlag); __attribute__((weak)); 
#endif

MS_BOOL HAL_MIU_ParseOccupiedResource(void) __attribute__((weak));
MS_U8* HAL_MIU_GetDefaultClientID_KernelProtect(void) __attribute__((weak));
MS_BOOL HAL_MIU_SlitInit(void) __attribute__((weak));
MS_BOOL HAL_MIU_SetSlitRange(MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_PHY u64BusStart, MS_PHY u64BusEnd, MS_BOOL  bSetFlag) __attribute__((weak)); 
MS_BOOL HAL_MIU_Slits(MS_U8 u8Blockx, MS_PHY u64SlitsStart, MS_PHY u64SlitsEnd, MS_BOOL bSetFlag) __attribute__((weak)); 

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------
static DEFINE_SPINLOCK(miu_lock);

//-------------------------------------------------------------------------------------------------
//  Local functions
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_Init
/// @brief \b Function  \b Description: parse occupied resource to software structure
/// @param None         \b IN :
/// @param None         \b OUT :
/// @param MS_BOOL      \b RET
/// @param None         \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////

MS_BOOL MDrv_MIU_Init(void)
{
    MS_BOOL ret;
    
    /* Parse the used client ID in hardware into software data structure */
    if(HAL_MIU_ParseOccupiedResource)
    {
        ret = HAL_MIU_ParseOccupiedResource();
    }
    else
    {
        ret = FALSE;
    }
    
    /* Initialize MIU slit setting */
    if(HAL_MIU_SlitInit)
    {
        ret = HAL_MIU_SlitInit();
    }
    
    return ret;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_GetDefaultClientID_KernelProtect()
/// @brief \b Function \b Description:  Get default client id array pointer for protect kernel
/// @param <RET>           \b     : The pointer of Array of client IDs
////////////////////////////////////////////////////////////////////////////////
MS_U8* MDrv_MIU_GetDefaultClientID_KernelProtect(void)
{
    if(HAL_MIU_GetDefaultClientID_KernelProtect)
    {
        return HAL_MIU_GetDefaultClientID_KernelProtect();
    }
    else
    {
        return NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_Protect()
/// @brief \b Function \b Description:  Enable/Disable MIU Protection mode
/// @param u8Blockx        \b IN     : MIU Block to protect (0 ~ 3)
/// @param *pu8ProtectId   \b IN     : Allow specified client IDs to write
/// @param u32Start        \b IN     : Starting address(bus address)
/// @param u32End          \b IN     : End address(bus address)
/// @param bSetFlag        \b IN     : Disable or Enable MIU protection
///                                      - -Disable(0)
///                                      - -Enable(1)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_Protect(
                          MS_U8    u8Blockx,
                          MS_U8    *pu8ProtectId,
                          MS_PHY   u64BusStart,
                          MS_PHY   u64BusEnd,
                          MS_BOOL  bSetFlag
                         )
{
    MS_BOOL Result = FALSE;

    /*Case of former MIU protect*/
    if((u8Blockx >= E_PROTECT_0) && (u8Blockx < E_SLIT_0))
    {
        if(HAL_MIU_Protect)
        {
            Result = HAL_MIU_Protect(u8Blockx, pu8ProtectId, u64BusStart, u64BusEnd, bSetFlag);
        }
    }
    /*Case of MIU slits*/
    else
    {
        if(HAL_MIU_SetSlitRange)
        {
            Result = HAL_MIU_SetSlitRange(u8Blockx, pu8ProtectId, u64BusStart, u64BusEnd, bSetFlag);
        }
    }
    
    return Result;
}

MS_BOOL MDrv_Protect_Save(void)
{
	MS_BOOL Result = FALSE;
	Result = HAL_Protect_Save();
	return Result;
}

MS_BOOL MDrv_Protect_Restore(void)
{
	MS_BOOL Result = FALSE;
	Result = HAL_Protect_Restore();
	return Result;
}

#ifdef CONFIG_MP_CMA_PATCH_DEBUG_STATIC_MIU_PROTECT
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_GetProtectInfo()
/// @brief \b Function  \b Description:  This function for querying client ID info
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param eClientID    \b IN   : Client ID
/// @param pInfo        \b OUT  : Client Info
/// @param None \b RET:   0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo)
{
    return HAL_MIU_GetProtectInfo(u8MiuDev, pInfo);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_Slits()
/// @brief \b Function \b Description:  Enable/Disable MIU Slit
/// @param u8Blockx        \b IN     : MIU Block to protect (E_MIU_SLIT_0)
/// @param u32Start        \b IN     : Starting address(bus address)
/// @param u32End          \b IN     : End address(bus address)
/// @param bSetFlag        \b IN     : Disable or Enable MIU protection
///                                      - -Disable(0)
///                                      - -Enable(1)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_Slits(MS_U8 u8Blockx, MS_PHY u64SlitsStart, MS_PHY u64SlitsEnd, MS_BOOL bSetFlag)
{
    unsigned long flags;
    MS_BOOL Result = FALSE;
    
    spin_lock_irqsave(&miu_lock, flags);
    
    if(HAL_MIU_Slits)
    {
        Result = HAL_MIU_Slits(u8Blockx, u64SlitsStart, u64SlitsEnd, bSetFlag);
    }
    
    spin_unlock_irqrestore(&miu_lock, flags);
    
    return Result;
}
