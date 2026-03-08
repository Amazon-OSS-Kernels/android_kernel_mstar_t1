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
// file   osalRVD.c
// @brief  RVD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "osalRVD.h"

// Mutex
inline MS_S32 OSAL_RVD_MutexCreate(MS_U8* pu8Name)
{
    return MsOS_CreateMutex(E_MSOS_FIFO, (char *)pu8Name, MSOS_PROCESS_SHARED);
}

inline MS_BOOL OSAL_RVD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    return MsOS_ObtainMutex(s32MutexId, u32Timeout);
}

inline MS_BOOL OSAL_RVD_MutexRelease(MS_S32 s32MutexId)
{
    return MsOS_ReleaseMutex(s32MutexId);
}

inline MS_BOOL OSAL_RVD_MutexDelete(MS_S32 s32MutexId)
{
    return MsOS_DeleteMutex(s32MutexId);
}

inline MS_BOOL OSAL_RVD_ISR_Disable(void)
{
    return MsOS_DisableInterrupt(RVD_ISR_VECTOR);
}

inline MS_BOOL OSAL_RVD_ISR_Enable(void)
{
    return MsOS_EnableInterrupt(RVD_ISR_VECTOR);
}

inline MS_BOOL OSAL_RVD_ISR_Attach(void* pfnISRCB)
{
    return MsOS_AttachInterrupt(RVD_ISR_VECTOR, (InterruptCb)pfnISRCB);
}

inline MS_BOOL OSAL_RVD_ISR_Detach(void)
{
    return MsOS_DetachInterrupt(RVD_ISR_VECTOR);
}



