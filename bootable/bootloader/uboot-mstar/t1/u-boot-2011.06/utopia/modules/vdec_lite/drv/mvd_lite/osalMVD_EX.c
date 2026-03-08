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
// file   osalMVD.c
// @brief  MPEG Video Decoder (MVD) OSAL
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "osalMVD_EX.h"

static MS_S32 _s32MvdMutexId = -1;
static MS_U8  _u8MvdMutex[] = {"MVD_Mutex"};

/// Mutex
inline MS_S32 OSAL_MVD_MutexCreate(MS_U8* pu8Name)
{
    return MsOS_CreateMutex(E_MSOS_FIFO, (char*)pu8Name, MSOS_PROCESS_SHARED);
}

inline MS_BOOL OSAL_MVD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    return MsOS_ObtainMutex(s32MutexId, u32Timeout);
}

inline MS_BOOL OSAL_MVD_MutexRelease(MS_S32 s32MutexId)
{
    return MsOS_ReleaseMutex(s32MutexId);
}

inline MS_BOOL OSAL_MVD_MutexDelete(MS_S32 s32MutexId)
{
    return MsOS_DeleteMutex(s32MutexId);
}

#if 1

// Mutex function for MVD HW
MS_BOOL OSAL_MVD_MutexInit(void)
{
    if (_s32MvdMutexId < 0)
    {
        _s32MvdMutexId = OSAL_MVD_MutexCreate(_u8MvdMutex);
    }
    return (_s32MvdMutexId != -1);
}

MS_BOOL OSAL_MVD_LockHwMutex(void)
{
    //printf(">>>> OSAL_MVD_LockHwMutex!\n");
    if (_s32MvdMutexId != -1)
        return OSAL_MVD_MutexObtain(_s32MvdMutexId, OSAL_MVD_MUTEX_TIMEOUT);
    else
        return FALSE;
}

MS_BOOL OSAL_MVD_UnlockHwMutex(void)
{
    //printf("<<<< OSAL_MVD_UnlockHwMutex!\n");
    if (_s32MvdMutexId != -1)
        return OSAL_MVD_MutexRelease(_s32MvdMutexId);
    else
        return FALSE;
}

MS_BOOL OSAL_MVD_MutexExit(void)
{
    MS_BOOL bRet = TRUE;
    if(_s32MvdMutexId != -1)
    {
        OSAL_MVD_MutexDelete(_s32MvdMutexId);
        _s32MvdMutexId = -1;
    }
    return bRet;
}

#endif

#if 1//(defined(CHIP_T2) || defined(CHIP_U3))
// Interrupt
// fixme: define MVD_ISR_VECTOR in HAL
#define MVD_ISR_VECTOR E_INT_IRQ_MVD
inline MS_BOOL OSAL_MVD_IntDisable(void)
{
    return MsOS_DisableInterrupt(MVD_ISR_VECTOR);
}

inline MS_BOOL OSAL_MVD_IntEnable(void)
{
    MS_BOOL ret = FALSE;

    ret = MsOS_EnableInterrupt(MVD_ISR_VECTOR);

    #ifndef MSOS_TYPE_LINUX_KERNEL // TODO: check if this function can be support in kernel mode in later version
    MsOS_CompleteInterrupt(MVD_ISR_VECTOR);
    #endif

    return ret;
}

inline MS_BOOL OSAL_MVD_IsrAttach(void* pfnIsr)
{
    return MsOS_AttachInterrupt(MVD_ISR_VECTOR, (InterruptCb)pfnIsr);
}

inline MS_BOOL OSAL_MVD_IsrDetach(void)
{
    return MsOS_DetachInterrupt(MVD_ISR_VECTOR);
}
#endif

