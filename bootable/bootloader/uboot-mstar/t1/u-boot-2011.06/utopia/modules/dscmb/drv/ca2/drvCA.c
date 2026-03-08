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
#include "utopia.h"

#ifdef CA_UTOPIA_20
#define  CA_UTOPIA20
#else
#undef   CA_UTOPIA20
#endif

#ifdef CA_UTOPIA20
#include "drvCA_v2.h"
#endif

#include "drvCA_private.h"

static MS_BOOL _bCADrv_Inited = FALSE;
static MS_S32 _s32CADrv_Mutex = -1 ;
static MS_BOOL _bBGCDrv_Inited = FALSE;

#ifdef CA_NO_PRINTF
#define DRV_CA_DEBUG(fmt, args...)
#else
#define DRV_CA_DEBUG(fmt, args...) do{ \
                printf("[%s][%d]"fmt"\n", __FUNCTION__, __LINE__,  ## args);}\
                while(0)
#endif

#define DRV_CA_ASSERT( _bool_, _msg_, ret) \
    do { if(!(_bool_)) { DRV_CA_DEBUG(_msg_); return (ret); } } while(0)

#define DRV_CA_INIT_CHECK(ret) DRV_CA_ASSERT(_bCADrv_Inited == TRUE, "The drvCA is not initialized\n", ret)
#define DRV_BGC_INIT_CHECK(ret) DRV_CA_ASSERT(_bBGCDrv_Inited == TRUE, "The drvBGC is not initialized\n", ret)

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)
//BGC Isr proc
#define BGC_FAIL_INT        (0x80000000UL)
#define BGC_START_INT       (0x40000000UL)
#define BGC_TASK_STACK_SIZE (4096UL)
static MS_U8    _u8BGC_StackBuffer[BGC_TASK_STACK_SIZE];
static MS_S32   _s32BGCTaskId;
static MS_S32   _s32BGCEventId;

#define BGCIntEnable()                       MsOS_EnableInterrupt (BGC_IRQ)
#define BGCIntDisable()                      MsOS_DisableInterrupt(BGC_IRQ)
#define BGCIntAttach(isr)                    MsOS_AttachInterrupt (BGC_IRQ, (InterruptCb )isr)
#define BGCIntDetach()                       MsOS_DetachInterrupt (BGC_IRQ)
#endif

typedef struct
{
    MS_BOOL bUsed;
    MS_U32  u32BGCMode;
}ST_BGC_SEC_CFG;

ST_BGC_SEC_CFG stBGCSecCfg[BGC_SEC_MAX];

#ifdef CA_UTOPIA20
static void *pu32CAInst = NULL;
#endif

//--------------------------------------------------------------------------------------------------
/// Initialize CA driver
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_CA_Init(void)
{
    MS_VIRT ptrRegBank;
    MS_PHY u32BankSize;

    if(TRUE == _bCADrv_Inited)
    {
        return TRUE;
    }

    if (FALSE == MDrv_MMIO_GetBASE(&ptrRegBank, &u32BankSize, MS_MODULE_HW))
    {
        MS_DEBUG_MSG(printf("[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    HAL_CA_SetBank(ptrRegBank);

    if(HAL_CA_Init() == FALSE)
    {
        MS_DEBUG_MSG(printf("[%s][%d] HAL CA init fail\n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    _bCADrv_Inited = TRUE;
    _bBGCDrv_Inited = FALSE;

    _s32CADrv_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*) "CA2_Mutex", MSOS_PROCESS_SHARED);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// @brief Enable Secure Boot
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_CA_OTP_EnableSecureBoot(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_EnableSecureBoot();
}

//-------------------------------------------------------------------------------------------------
/// @brief Check if Secure Boot Enabled
/// @return TRUE: Enabled
/// @return FALSE: Disabled
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_CA_OTP_IsSecureBootEnabled(void)
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
MS_BOOL _MDrv_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetBlockLock(u32Start, u32End, eLockType);
}

MS_BOOL _MDrv_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *peLockType)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_GetBlockLock(pu32Start, pu32End, peLockType);
}

//-------------------------------------------------------------------------------------------------
/// @brief Check if the value is zero in OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @return OTP value
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_CA_OTP_IsBlank(MS_U32 addr)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_IsBlank(addr);
}

//-------------------------------------------------------------------------------------------------
/// @brief Read 32bits value from OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @return OTP value
//-------------------------------------------------------------------------------------------------
MS_U32 _MDrv_CA_OTP_Read(MS_U32 addr)
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
MS_BOOL _MDrv_CA_OTP_Write(MS_U32 addr, MS_U32 value)
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
MS_BOOL _MDrv_CA_Locked(CA_OTP_LOCK eLock)
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
MS_BOOL _MDrv_CA_OTP_Lock(CA_OTP_LOCK eLock)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_Lock(eLock);
}

//-------------------------------------------------------------------------------------------------
/// @brief Set the value of the RSAextID in OTP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_CA_OTP_SetRSAextID(MS_U32 u32Value)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetRSAextID(u32Value);
}

//-------------------------------------------------------------------------------------------------
/// @brief Get the value of the RSAextID in OTP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_U32 _MDrv_CA_OTP_GetRSAextID(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_GetRSAextID();
}

MS_BOOL _MDrv_CA_OTP_SetHASH1_REF_VER(MS_U32 u32HASH1_REF_VER)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetHASH1_REF_VER(u32HASH1_REF_VER);
}

MS_BOOL _MDrv_CA_OTP_GetHASH1_REF_VER(MS_U32 *pu32HASH1_REF_VER)
{
    DRV_CA_INIT_CHECK(FALSE);
    if(pu32HASH1_REF_VER == NULL)
        return FALSE;
    return HAL_CA_OTP_GetHASH1_REF_VER(pu32HASH1_REF_VER);
}


MS_BOOL _MDrv_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_SetHASH_REF_VER(u32HASH0_REF_VER, u32HASH2_REF_VER);
}

MS_BOOL _MDrv_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER)
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
MS_BOOL _MDrv_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode)
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
MS_BOOL _MDrv_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode)
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
MS_BOOL _MDrv_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size)
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
MS_BOOL _MDrv_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size)
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
MS_U32 _MDrv_CA_MaxDeviceIdSize(void)
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
MS_BOOL _MDrv_CA_OTP_EnableSecureCWMode(void)
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
MS_BOOL _MDrv_CA_OTP_IsSecureCWMode(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_IsSecureCWMode();
}

//-------------------------------------------------------------------------------------------------
/// 16bits Hardware random number generator
/// @return 16-bits random number
//-------------------------------------------------------------------------------------------------
MS_U32 _MDrv_CA_Random(void)
{
    DRV_CA_INIT_CHECK(FALSE);

    MsOS_ObtainMutex(_s32CADrv_Mutex, MSOS_WAIT_FOREVER);
    MS_U32 u32Rnd = HAL_CA_Random();
    MsOS_ReleaseMutex(_s32CADrv_Mutex);

    return u32Rnd;
}

MS_U32 _MDrv_CA_OTP_ReadCAVendor(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_ReadCAVendor();
}

//--------------------------------------------------------------------------------------------------
/// Initialize CA driver
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_Init(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32CAInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_CA, &pu32CAInst, 0x0, NULL))
        {
            MS_DEBUG_MSG(printf("[%s][%d] UtopiaOpen CA fail\n", __FUNCTION__, __LINE__));
            return FALSE;
        }
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_Init, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_Init();
#endif
}

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)
static void _BGC_Isr_Proc(void)
{
    MS_U32  u32Events = 0;

    // coverity[no_escape]
    while (1)
    {
        MsOS_WaitEvent(_s32BGCEventId, BGC_FAIL_INT, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        if(u32Events & BGC_FAIL_INT)
        {
            HAL_CA_BGC_ISR();
        }

        HAL_CA_BGC_DisableINT();
        BGCIntEnable();
    } // Task while loop
}

static void _BGC_Isr(void)
{
    //set event to trigger proc deal with bgc fail
    MsOS_SetEvent(_s32BGCEventId, BGC_FAIL_INT);
}
#endif

//--------------------------------------------------------------------------------------------------
/// Load BGC FW
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_BGC_Init(CA_BGC_CFG stBGCCfg)
{
    DRV_CA_INIT_CHECK(FALSE);
    MS_BOOL ret = FALSE;
    if(stBGCCfg.u32FwAddr == 0)
    {
        return FALSE;
    }

    if(stBGCCfg.u32FwSize > 0x2000)
    {   //max size is 8K
        return FALSE;
    }

    if(_bBGCDrv_Inited == TRUE)
    {
        return TRUE;
    }

    ret = HAL_CA_BGC_LoadFW(stBGCCfg.u32FwAddr, stBGCCfg.u32FwSize);
    if(ret == FALSE)
    {
        return FALSE;
    }


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)
    //Isr event
    _s32BGCEventId = MsOS_CreateEventGroup("BGC_Event");
    if (_s32BGCEventId < 0)
    {
        return FALSE;
    }

    //Isr Task
    _s32BGCTaskId = MsOS_CreateTask((TaskEntry)_BGC_Isr_Proc,
                                        0,
                                        E_TASK_PRI_SYS,
                                        TRUE,
                                        (void*)_u8BGC_StackBuffer,
                                        BGC_TASK_STACK_SIZE,
                                        "BGC_Task");

    if (_s32BGCTaskId < 0)
    {
        MsOS_DeleteEventGroup(_s32BGCEventId);
        return FALSE;
    }


    BGCIntDisable();
    BGCIntDetach ();
    BGCIntAttach (_BGC_Isr);
    BGCIntEnable ();
#endif

    memset(stBGCSecCfg, 0, sizeof(ST_BGC_SEC_CFG));

    _bBGCDrv_Inited = TRUE;

    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Set BGC Protected section by u32SecId
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_BGC_SetSection(MS_U32 u32SecId, const CA_BGC_SEC_CFG* pstSecCfg)
{
    DRV_CA_INIT_CHECK(FALSE);
    DRV_BGC_INIT_CHECK(FALSE);
    MS_BOOL ret = FALSE;

    if(u32SecId >= BGC_SEC_MAX || (pstSecCfg == NULL))
    {
        return FALSE;
    }

    ret = HAL_CA_BGC_SetSec(u32SecId, pstSecCfg);
    if(ret == TRUE)
    {
        stBGCSecCfg[u32SecId].bUsed = TRUE;
        stBGCSecCfg[u32SecId].u32BGCMode = pstSecCfg->u32BGCMode;
    }

    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Reset BGC Protected section if BGC mode is 0:dynamic by u32SecId,
/// This function will check if the pu8RstPSWD is equal to CA_BGC_SEC_CFG.u8ResetPassword
/// If correct, reset the BGC section with u32SecId
/// @return TRUE - If pu8RstPSWD is match
/// @return FALSE -If pu8RstPSWD is not match
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_BGC_ResetSection(MS_U32 u32SecId, const MS_U8* const pu8RstPSWD)
{
    DRV_CA_INIT_CHECK(FALSE);
    DRV_BGC_INIT_CHECK(FALSE);
    MS_BOOL ret = FALSE;

    if((u32SecId >= BGC_SEC_MAX)  || (pu8RstPSWD == NULL))
    {
        return FALSE;
    }

    if(stBGCSecCfg[u32SecId].bUsed == TRUE)
    {
        if(stBGCSecCfg[u32SecId].u32BGCMode == 0)//dynamic
        {
            ret = HAL_CA_BGC_ResetSec(u32SecId, pu8RstPSWD);
            if(ret == TRUE)
            {
                stBGCSecCfg[u32SecId].bUsed = FALSE;
            }
        }
    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// @brief Init CA MBX driver
/// @return TRUE: OK
/// @return FALSE: Error
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_MBX_Init(void)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// @brief Write data to MBX
/// @param[in] u8Addr The address of the input buffer
/// @param[in] u32Length The length of the input buffer
/// @return The length that actually write to MBX
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_MBX_Write(MS_U8* pu8Addr, MS_U32 u32Length)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// @brief Read data from MBX
/// @param[Out] u8Addr The address of the output buffer
/// @param[in] u32Length The length of the output buffer
/// @return The length that actually read from MBX
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_MBX_Read(MS_U8* pu8Addr, MS_U32 u32Length)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// @brief Enable Secure Boot
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_EnableSecureBoot(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_EnableSecureBoot, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_EnableSecureBoot();
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Check if Secure Boot Enabled
/// @return TRUE: Enabled
/// @return FALSE: Disabled
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_IsSecureBootEnabled(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_IsSecureBootEnabled, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_IsSecureBootEnabled();
#endif
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
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_SETBLOCKLOCK pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32Start = u32Start;
    pArgs.u32End = u32End;
    pArgs.eLockType = eLockType;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_SetBlockLock, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_SetBlockLock(u32Start, u32End, eLockType);
#endif
}

MS_BOOL MDrv_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *peLockType)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_GETBLOCKLOCK pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.pu32Start = pu32Start;
    pArgs.pu32End = pu32End;
    pArgs.peLockType = peLockType;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_GetBlockLock, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_GetBlockLock(pu32Start, pu32End, peLockType);
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Check if the value is zero in OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @return OTP value
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_IsBlank(MS_U32 addr)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_ISBLANK pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32addr = addr;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_IsBlank, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_IsBlank(addr);
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Read 32bits value from OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @return OTP value
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_OTP_Read(MS_U32 addr)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_READ pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32Addr = addr;
    pArgs.u32OTPValue = 0;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_Read, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return pArgs.u32OTPValue;
#else
    return _MDrv_CA_OTP_Read(addr);
#endif
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
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_WRITE pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32Addr = addr;
    pArgs.u32value = value;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_Write, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_Write(addr, value);
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Read the OTP Lock config
/// @param[in] eLock
/// @return TRUE: Locked.
/// @return FALSE: Not locked
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_Locked(CA_OTP_LOCK eLock)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_LOCK20 pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.eLock = eLock;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_Locked, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_Locked(eLock);
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Lock OTP area
/// @param[in] eLock
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_Lock(CA_OTP_LOCK eLock)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_LOCK20 pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.eLock = eLock;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_Lock, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_Lock(eLock);
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Set the value of the RSAextID in OTP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_SetRSAextID(MS_U32 u32Value)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_SETRSAEXTID pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32value= u32Value;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_SetRSAextID, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_SetRSAextID(u32Value);
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Get the value of the RSAextID in OTP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_OTP_GetRSAextID(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_GETRSAEXTID pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_GetRSAextID, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return pArgs.u32value;
#else
    return _MDrv_CA_OTP_GetRSAextID();
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Set the value of the TEE RSAextID in OTP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_SetTEERSAextID(MS_U32 u32Value)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// @brief Get the value of the TEE RSAextID in OTP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_OTP_GetTEERSAextID(void)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

MS_BOOL MDrv_CA_OTP_SetHASH1_REF_VER(MS_U32 u32HASH1_REF_VER)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_SETHASH1REFVER pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32HASH1_REF_VER = u32HASH1_REF_VER;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_SetHASH1_REF_VER, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_SetHASH1_REF_VER(u32HASH1_REF_VER);
#endif
}

MS_BOOL MDrv_CA_OTP_GetHASH1_REF_VER(MS_U32 *pu32HASH1_REF_VER)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_GETHASH1REFVER pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.pu32HASH1_REF_VER = pu32HASH1_REF_VER;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_GetHASH1_REF_VER, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_GetHASH1_REF_VER(pu32HASH1_REF_VER);
#endif
}


MS_BOOL MDrv_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_SETHASHREFVER pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32HASH0_REF_VER = u32HASH0_REF_VER;
    pArgs.u32HASH2_REF_VER = u32HASH2_REF_VER;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_SetHASH_REF_VER, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_SetHASH_REF_VER(u32HASH0_REF_VER, u32HASH2_REF_VER);
#endif
}

MS_BOOL MDrv_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_GETHASHREFVER pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.pu32HASH0_REF_VER = pu32HASH0_REF_VER;
    pArgs.pu32HASH2_REF_VER = pu32HASH2_REF_VER;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_GetHASH_REF_VER, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_GetHASH_REF_VER(pu32HASH0_REF_VER, pu32HASH2_REF_VER);
#endif
}

MS_BOOL MDrv_CA_OTP_SetHASH_TEE_REF_VER(MS_U32 u32HASH_TEE0_REF_VER, MS_U32 u32HASH_TEE1_REF_VER, MS_U32 u32HASH_TEE2_REF_VER)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

MS_BOOL MDrv_CA_OTP_GetHASH_TEE_REF_VER(MS_U32 *pu32HASH_TEE0_REF_VER, MS_U32 *pu32HASH_TEE1_REF_VER, MS_U32 *pu32HASH_TEE2_REF_VER)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
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
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_SETDEBUGPORTMODE pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.eDebugPort = eDebugPort;
    pArgs.eMode = eMode;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_SetDebugPortMode, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_SetDebugPortMode(eDebugPort, eMode);
#endif
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
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_GETDEBUGPORTMODE pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.eDebugPort = eDebugPort;
    pArgs.eMode = eMode;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_GetDebugPortMode, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_GetDebugPortMode(eDebugPort, eMode);
#endif
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
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_SETDEVICEID pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.pu8Did = pu8Did;
    pArgs.u32Size = u32Size;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_SetDeviceId, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_SetDeviceId(pu8Did, u32Size);
#endif
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
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_GETDEVICEID pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    pArgs.pu8Did = pu8Did;
    pArgs.u32Size = u32Size;

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_GetDeviceId, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_GetDeviceId(pu8Did, u32Size);
#endif
}


//--------------------------------------------------------------------------------------------------
/// @brief The MAX size of the Device ID
/// @return TRUE: OK
/// @return FALSE: Error

/// @note
//--------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_MaxDeviceIdSize(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_MAXDEVICEIDSIZE pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_MaxDeviceIdSize, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return pArgs.u32MaxDeviceIdSize;
#else
    return _MDrv_CA_MaxDeviceIdSize();
#endif
}

//-------------------------------------------------------------------------------------------------
/// @brief Write the OTP reserved area
/// @param[in] pu8RData a buffer to write the reserved area.
/// @param[in] u32Size The size of the buffer
/// @return TRUE: OK
/// @return FALSE: Error
/// @note:
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_WriteReserved(const MS_U8 *pu8RData, MS_U32 u32Size)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// @brief Read the OTP reserved area
/// @param[out] pu8RData a buffer to read the reserved area.
/// @param[in] u32Size The size of the buffer
/// @return TRUE: OK
/// @return FALSE: Error
/// @note:
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_ReadReserved(MS_U8 *pu8RData, MS_U32 u32Size)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// @brief The MAX size of the reserved area
/// @return the MAX size of the reserved area
/// @note
//--------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_MaxReservedSize(void)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// @brief Set ACPU cannot write clear CW to key table
/// @return TRUE: OK
/// @return FALSE: Error
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_EnableSecureCWMode(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_EnableSecureCWMode, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_EnableSecureCWMode();
#endif
}

//--------------------------------------------------------------------------------------------------
/// @brief Check if ACPU can write clear CW to key table
/// @return TRUE: OK
/// @return FALSE: Error
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_IsSecureCWMode(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_IsSecureCWMode, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CA_OTP_IsSecureCWMode();
#endif
}

//-------------------------------------------------------------------------------------------------
/// 16bits Hardware random number generator
/// @return 16-bits random number
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_Random(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_RANDOM pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_Random, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return pArgs.u32Random;
#else
    return _MDrv_CA_Random();
#endif
}

MS_U32 MDrv_CA_OTP_ReadCAVendor(void)
{
#ifdef CA_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CA_OTP_READCAVENDOR pArgs;
    if(pu32CAInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32CAInst, E_MDRV_CMD_CA_OTP_ReadCAVendor, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        MS_DEBUG_MSG(printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret));
        return FALSE;
    }

    return pArgs.u32CAVendor;
#else
    return _MDrv_CA_OTP_ReadCAVendor();
#endif
}

MS_BOOL MDrv_CA_STR_Init(void)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

MS_BOOL MDrv_CA_STR_SetParamAddr(MS_U8 *pu8VAddr)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

MS_BOOL MDrv_CA_STR_SetMAC(MS_U8 *pu8MAC, MS_U32 u32MACSize)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}

MS_BOOL MDrv_CA_OTP_DumpKeyProperty(MS_U32 u32KeyNumber)
{
    // Not Implement
    DRV_CA_DEBUG("Not Implement\n");
    return FALSE;
}
