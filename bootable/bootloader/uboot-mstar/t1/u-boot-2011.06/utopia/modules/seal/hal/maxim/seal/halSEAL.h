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

#ifndef _HAL_SEAL_H_
#define _HAL_SEAL_H_

#include <drvSEAL.h>

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _FUNC_NOT_USED()        do {} while ( 0 )

#define SEAL_SECURE_RANGE_ALIGN     (0x1000UL) //4k bytes for secure range alignement
#define SEAL_IMI_RANGE_ALIGN        (0x400UL) //1k bytes for IMI range alignment
#define SEAL_NONPM_GROUP_NUM        (24UL)
#define SEAL_NONPM_GROUP_IP_NUM     (16UL)
#define SEAL_NONPM_TBL_IP_NUM       (SEAL_NONPM_GROUP_NUM*SEAL_NONPM_GROUP_IP_NUM)
#define SEAL_PM_GROUP_NUM           (4UL)
#define SEAL_PM_GROUP_IP_NUM        (16UL)
#define SEAL_PM_TBL_IP_NUM          (SEAL_PM_GROUP_NUM*SEAL_PM_GROUP_IP_NUM)
#define SEAL_PROCESSOR_GROUP_NUM    (1UL)
#define SEAL_PROCESSOR_NUM_PERGROUP (9UL)
#define SEAL_TBL_PROCESSOR_NUM      (SEAL_PROCESSOR_GROUP_NUM*SEAL_PROCESSOR_NUM_PERGROUP)
#define SEAL_LOCK_GROUP_NUM         (26UL)
#define SEAL_LOCK_NUM_PERGROUP      (16UL)
#define SEAL_TBL_LOCK_NUM           (SEAL_LOCK_GROUP_NUM*SEAL_LOCK_NUM_PERGROUP)
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_HAL_MIU_DEV0,
    E_HAL_MIU_DEV1,
    E_HAL_MIU_NUM,
}eHalMiu_Dev;

typedef enum
{
    E_HAL_SEAL_SECURE_RANGE_ID0,
    E_HAL_SEAL_SECURE_RANGE_ID1,
    E_HAL_SEAL_SECURE_RANGE_ID2,
    E_HAL_SEAL_SECURE_RANGE_ID3,
    E_HAL_SEAL_SECURE_RANGE_ID4,
    E_HAL_SEAL_SECURE_RANGE_ID5,
    E_HAL_SEAL_SECURE_RANGE_ID6,
    E_HAL_SEAL_SECURE_RANGE_ID7,
    E_HAL_SEAL_SECURE_RANGE_ID8,
    E_HAL_SEAL_SECURE_RANGE_ID9,
    E_HAL_SEAL_SECURE_RANGE_ID10,
    E_HAL_SEAL_SECURE_RANGE_ID11,
    E_HAL_SEAL_SECURE_RANGE_ID12,
    E_HAL_SEAL_SECURE_RANGE_ID13,
    E_HAL_SEAL_SECURE_RANGE_ID14,
    E_HAL_SEAL_SECURE_RANGE_ID15,
    E_HAL_SEAL_SECURE_RANGE_NUM,
}eHalSeal_SecureRangeId;

typedef enum
{
    E_HAL_SEAL_DBBUS,
    E_HAL_SEAL_MCU51,
    E_HAL_SEAL_CPU2,
    E_HAL_SEAL_VD_R2,
    E_HAL_SEAL_SECURE_R2,
    E_HAL_SEAL_SC,
    E_HAL_SEAL_CMDQ,
    E_HAL_SEAL_PROCESSOR_NUM,
}eHalSeal_ProcessorId;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_SEAL_SetIOMapBase(MS_VIRT u32NonPmBaseAddr, MS_VIRT u32PmBaseAddr);
MS_BOOL HAL_Seal_SecureRangeSet(MS_U8 u8SecureRangeId, MS_U64 u64StartAddr, MS_U64 u64EndAddr, MS_U32 u32Attribute);
MS_BOOL HAL_Seal_SecureRangeQuery(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId, MS_U32 *pAttribute, MS_U64 *pStartAddr, MS_U64 *pEndAddr);
MS_BOOL HAL_Seal_IMI_RangeSet(MS_U32 u32StartAddr, MS_U32 u32EndAddr, MS_BOOL bEnable);
MS_BOOL HAL_Seal_GetMiuHitFlag(MS_U8 u8MiuDev);
MS_U8 HAL_Seal_GetMiuHitRangeID(MS_U8 u8MiuDev);
MS_U8 HAL_Seal_GetMiuHitClientID(MS_U8 u8MiuDev);
MS_BOOL HAL_Seal_GetMiuSecureFlag(MS_U8 u8MiuDev);
MS_BOOL HAL_Seal_GetMiuWriteFlag(MS_U8 u8MiuDev);
MS_U64 HAL_Seal_GetMiuHitAddr(MS_U8 u8MiuDev);
MS_BOOL HAL_Seal_ClearMiuHitLog(MS_U8 u8MiuDev);
MS_BOOL HAL_Seal_SecureRangeLock(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId);
MS_BOOL HAL_Seal_NonSecureProcessorSet(MS_U8 u8ProcessorId, MS_BOOL bNonSecure);
MS_BOOL HAL_Seal_NonSecureProcessorQuery(MS_U8 u8ProcessorId, MS_BOOL *pNonSecure);
MS_BOOL HAL_Seal_SecureSlaveSet(MS_U32 u32SlaveId, MS_BOOL bSecure);
MS_BOOL HAL_Seal_SecureSlaveQuery(MS_U32 u32SlaveId, MS_BOOL *pSecure);
MS_BOOL HAL_Seal_SecureMasterSet(MS_U32 u32MasterId, MS_BOOL bSecure);
MS_BOOL HAL_Seal_SecureMasterQuery(MS_U32 u32MasterId, MS_BOOL *pSecure);
MS_BOOL HAL_Seal_BufferLock(MS_U8 u8SecureBufferLockId, MS_BOOL bLock);
MS_BOOL HAL_Seal_QueryBufferLocked(eSeal_Lock eLockId, MS_BOOL* bLocked);
MS_BOOL HAL_Seal_MaskInterruptFlag(MS_U8 u8MiuDev, MS_BOOL bMask);
MS_BOOL HAL_SEAL_POWER_SUSPEND(void);
MS_BOOL HAL_SEAL_POWER_RESUME(void);
MS_BOOL HAL_Seal_CheckSecureRange(MS_PHY phyStartAddr, MS_U32 u32Length, eSeal_CheckSecureRangeType eCheckSecureRangeType);

#ifdef MSOS_TYPE_OPTEE
MS_BOOL HAL_Seal_ChangeIPSecureDMAAbillity(MS_U32 u32ModuleID, MS_U32 u32ModuleParameter, MS_U32 u32IsSecure);
MS_BOOL HAL_Seal_SetSecureRange(MS_PHY u64phy, MS_U32 u32Length, MS_U32 u32IsSecure);
#endif
#endif // _HAL_SEAL_H_

