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
// file   osalHVD.c
// @brief  HVD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "osalHVD_EX.h"
#include "drvHVD_def.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

// Mutex
inline MS_S32 OSAL_HVD_MutexCreate(MS_U8* pu8Name)
{
    return MsOS_CreateMutex(E_MSOS_FIFO, (char *)pu8Name, MSOS_PROCESS_SHARED);
}

inline MS_BOOL OSAL_HVD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    return MsOS_ObtainMutex(s32MutexId, u32Timeout);
}

inline MS_BOOL OSAL_HVD_MutexRelease(MS_S32 s32MutexId)
{
    return MsOS_ReleaseMutex(s32MutexId);
}

inline MS_BOOL OSAL_HVD_MutexDelete(MS_S32 s32MutexId)
{
    return MsOS_DeleteMutex(s32MutexId);
}

#if 0
// Process
inline void OSAL_HVD_Proc(OSAL_HVD_ProcType Proc, MS_U32 u32FltId, void* pDummy)
{
    // Proc(u32FltId, u32Timeout);
}
#endif
// Interrupt
inline MS_BOOL OSAL_HVD_ISR_Disable(MS_U32 u32ISRType)
{
    MS_U32 u32IsrVer = 0;
    switch(u32ISRType)
    {
        case E_HWDEC_ISR_HVD:
            u32IsrVer = HVD_ISR_VECTOR;
            break;
        #if SUPPORT_EVD
        case E_HWDEC_ISR_EVD:
            u32IsrVer = EVD_ISR_VECTOR;
            break;
        #endif
        #if SUPPORT_G2VP9
        case E_HWDEC_ISR_G2VP9:
            break;
        #endif
        default:
            break;
    }
    if(u32IsrVer == 0)
        return FALSE;
    return MsOS_DisableInterrupt(u32IsrVer);
}


inline MS_BOOL OSAL_HVD_ISR_Enable(MS_U32 u32ISRType)
{
    MS_U32 u32IsrVer = 0;
    switch(u32ISRType)
    {
        case E_HWDEC_ISR_HVD:
            u32IsrVer = HVD_ISR_VECTOR;
            break;
        #if SUPPORT_EVD
        case E_HWDEC_ISR_EVD:
            u32IsrVer = EVD_ISR_VECTOR;
            break;
        #endif
        #if SUPPORT_G2VP9
        case E_HWDEC_ISR_G2VP9:
            break;
        #endif
        default:
            break;
    }
    if(u32IsrVer == 0)
        return FALSE;

    return MsOS_EnableInterrupt(u32IsrVer);
}


inline MS_BOOL OSAL_HVD_ISR_Attach(MS_U32 u32ISRType, void* pfnISRCB)
{
    MS_U32 u32IsrVer = 0;
    switch(u32ISRType)
    {
        case E_HWDEC_ISR_HVD:
            u32IsrVer = HVD_ISR_VECTOR;
            break;
        #if SUPPORT_EVD
        case E_HWDEC_ISR_EVD:
            u32IsrVer = EVD_ISR_VECTOR;
            break;
        #endif
        #if SUPPORT_G2VP9
        case E_HWDEC_ISR_G2VP9:
            break;
        #endif
        default:
            break;
    }
    if(u32IsrVer == 0)
        return FALSE;

    return MsOS_AttachInterrupt(u32IsrVer, (InterruptCb)pfnISRCB);
}


inline MS_BOOL OSAL_HVD_ISR_Detach(MS_U32 u32ISRType)
{
    MS_U32 u32IsrVer = 0;
    switch(u32ISRType)
    {
        case E_HWDEC_ISR_HVD:
            u32IsrVer = HVD_ISR_VECTOR;
            break;
        #if SUPPORT_EVD
        case E_HWDEC_ISR_EVD:
            u32IsrVer = EVD_ISR_VECTOR;
            break;
        #endif
        #if SUPPORT_G2VP9
        case E_HWDEC_ISR_G2VP9:
            break;
        #endif
        default:
            break;
    }
    if(u32IsrVer == 0)
        return FALSE;
    return MsOS_DetachInterrupt(u32IsrVer);
}

