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
///
/// file   drvDSCMB.c
/// @brief  Descrambler (DSCMB) Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "MsVersion.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "drvMMIO.h"
#include "drvCA.h"
#include "regCA.h"
#include "halCA.h"

static MS_BOOL _bCADrv_Inited = FALSE;

#define DRV_CA_ASSERT( _bool_, _msg_, ret) \
    do { if(!(_bool_)) { printf(_msg_); return (ret); } } while(0)

#define DRV_CA_INIT_CHECK(ret) DRV_CA_ASSERT(_bCADrv_Inited == TRUE, "The drvCA is not initialized\n", ret)

//--------------------------------------------------------------------------------------------------
/// Initialize CA driver
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_Init(void)
{
    MS_U32 u32RegBank, u32BankSize;

    if (FALSE == MDrv_MMIO_GetBASE(&u32RegBank, &u32BankSize, MS_MODULE_HW))
    {
        MS_DEBUG_MSG(printf("[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    HAL_CA_SetBank(u32RegBank);

    if(HAL_CA_Init() == FALSE)
    {
        MS_DEBUG_MSG(printf("[%s][%d] HAL CA init fail\n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    _bCADrv_Inited = TRUE;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// @brief Enable Secure Boot
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_EnableSecureBoot(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_EnableSecureBoot();
}

//-------------------------------------------------------------------------------------------------
/// @brief Check if Secure Boot Enabled
/// @return TRUE: Enabled
/// @return FALSE: Disabled
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_IsSecureBootEnabled(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_IsSecureBootEnabled();
}

//-------------------------------------------------------------------------------------------------
/// @brief Forbid OTP read/write by the range
/// @param[in] u32Start: start address. unit in 4 bytes
/// @param[in] u32End: end address. unit in 4 bytes
/// @param[in] eLockType: E_CA_LOCK_WRITE forbid writing, E_CA_LOCK_READ  forbid reading
/// @return
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetBlockLock(u32Start, u32End, eLockType);
}

MS_BOOL MDrv_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *peLockType)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_GetBlockLock(pu32Start, pu32End, peLockType);
}

//-------------------------------------------------------------------------------------------------
/// @brief Check if the value is zero in OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @return OTP value
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_IsBlank(MS_U32 addr)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_IsBlank(addr);
}

//-------------------------------------------------------------------------------------------------
/// @brief Read 32bits value from OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @return OTP value
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_OTP_Read(MS_U32 addr)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_Read(addr);
}

//-------------------------------------------------------------------------------------------------
/// @brief Write 32bits value to OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @param[in] value
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_Write(MS_U32 addr, MS_U32 value)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_Write(addr, value);
}

//-------------------------------------------------------------------------------------------------
/// @brief Read the OTP Lock config
/// @param[in] eLock
/// @return TRUE: Locked.
/// @return FALSE: Not locked
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_Locked(CA_OTP_LOCK eLock)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_Locked(eLock);
}

//-------------------------------------------------------------------------------------------------
/// @brief Lock OTP area
/// @param[in] eLock
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_Lock(CA_OTP_LOCK eLock)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_Lock(eLock);
}

//-------------------------------------------------------------------------------------------------
/// @brief Set the value of the RSAextID in OTP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_SetRSAextID(MS_U32 u32Value)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetRSAextID(u32Value);
}

//-------------------------------------------------------------------------------------------------
/// @brief Get the value of the RSAextID in OTP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_OTP_GetRSAextID(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_GetRSAextID();
}

MS_BOOL MDrv_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetHASH_REF_VER(u32HASH0_REF_VER, u32HASH2_REF_VER);
}

MS_BOOL MDrv_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER)
{
    DRV_CA_INIT_CHECK(FALSE);
    if(pu32HASH0_REF_VER == NULL || pu32HASH2_REF_VER == NULL)
        return FALSE;
    return HAL_CA_OTP_GetHASH_REF_VER(pu32HASH0_REF_VER, pu32HASH2_REF_VER);
}

//-------------------------------------------------------------------------------------------------
/// @brief Set debug port mode
/// @param[in] eDebugPort E_CA_DEBUG_PORT_I2C or E_CA_DEBUG_PORT_JTAG
/// @param[in] eMode OPEN, PASSWORD_PROTECTED, or CLOSE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetDebugPortMode(eDebugPort, eMode);
}


//--------------------------------------------------------------------------------------------------
/// Get debug port state from the shadow register
/// @param[in] eDebugPort choose debug port
/// @param[out] eMode the state of the eDebugPort
/// @return TRUE: OK
/// @return FALSE: Error
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_GetDebugPortMode(eDebugPort, eMode);
}

//-------------------------------------------------------------------------------------------------
/// @brief Write the OTP Device ID
/// @param[in] pu8Did a buffer to store the Device ID. In LSB first format
/// @param[in] u32Size The size of the buffer
/// @return TRUE: OK
/// @return FALSE: Error
/// @note: The device ID in the OTP is LSB first and this is a byte-to-byte copy API.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetDeviceId(pu8Did, u32Size);
}

//-------------------------------------------------------------------------------------------------
/// @brief Read the OTP Device ID
/// @param[out] pu8Did a buffer to store the Device ID. In LSB first format
/// @param[in] u32Size The size of the buffer
/// @return TRUE: OK
/// @return FALSE: Error
/// @note: The device ID in the OTP is LSB first and this is a byte-to-byte copy API.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_GetDeviceId(pu8Did, u32Size);
}


//--------------------------------------------------------------------------------------------------
/// @brief The MAX size of the Device ID
/// @return TRUE: OK
/// @return FALSE: Error

/// @note
//--------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_MaxDeviceIdSize(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_MaxDeviceIdSize();
}

//--------------------------------------------------------------------------------------------------
/// @brief Set ACPU cannot write clear CW to key table
/// @return TRUE: OK
/// @return FALSE: Error
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_EnableSecureCWMode(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_EnableSecureCWMode();
}

//--------------------------------------------------------------------------------------------------
/// @brief Check if ACPU can write clear CW to key table
/// @return TRUE: OK
/// @return FALSE: Error
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_IsSecureCWMode(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_IsSecureCWMode();
}

//-------------------------------------------------------------------------------------------------
/// 16bits Hardware random number generator
/// @return 16-bits random number
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_Random(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_Random();
}


