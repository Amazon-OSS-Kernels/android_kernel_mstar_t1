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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   MsOS_private.h
/// @brief  MStar OS Private Wrapper
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MS_OS_PRIVATE_H_
#define _MS_OS_PRIVATE_H_

#ifdef __cplusplus
extern "C"
{
#endif

//
// Atomic operation
//
void MsOS_AtomicSet(MsOS_Atomic *pAtomic, MS_S32 s32Value);
MS_S32 MsOS_AtomicRead(const MsOS_Atomic *pAtomic);

void MsOS_AtomicAdd(MsOS_Atomic *pAtomic, MS_S32 s32Value);
void MsOS_AtomicSub(MsOS_Atomic *pAtomic, MS_S32 s32Value);

MS_S32 MsOS_AtomicAddReturn(MsOS_Atomic *pAtomic, MS_S32 s32Value);
MS_S32 MsOS_AtomicSubReturn(MsOS_Atomic *pAtomic, MS_S32 s32Value);

//
// Per-thread data
//
MS_BOOL MsOS_CreateThreadDataIndex(MS_U32 *pu32Index);
MS_BOOL MsOS_DeleteThreadDataIndex(MS_U32 u32Index);
MS_BOOL MsOS_SetThreadData(MS_U32 u32Index, MS_U32 u32Data);
MS_BOOL MsOS_GetThreadData(MS_U32 u32Index, MS_U32 *pu32Data);

//
// Condition Variables
//
MS_S32 MsOS_CreateCondition(MS_S32 s32MutexId, const char *pCondName);
MS_BOOL MsOS_DeleteCondition(MS_S32 s32CondId);
MS_BOOL MsOS_WaitCondition(MS_S32 s32CondId, MS_U32 u32WaitMs);
MS_BOOL MsOS_SignalCondition(MS_S32 s32CondId);
MS_BOOL MsOS_BroadcastCondition(MS_S32 s32CondId);

//
// Spinlock
//
MS_S32 MsOS_CreateSpin(MsOSAttribute eAttribute, char *pSpinName);
MS_BOOL MsOS_DeleteSpin(MS_S32 s32SpinId);
MS_BOOL MsOS_ObtainSpin(MS_S32 s32SpinId);
MS_BOOL MsOS_ReleaseSpin(MS_S32 s32SpinId);
MS_BOOL MsOS_ObtainSpinIrqSave(MS_S32 s32SpinId, MS_U32 *pu32Flag);
MS_BOOL MsOS_ReleaseSpinIrqSave(MS_S32 s32SpinId, MS_U32 u32Flag);

#ifdef __cplusplus
}
#endif

#endif // _MS_OS_PRIVATE_H_
