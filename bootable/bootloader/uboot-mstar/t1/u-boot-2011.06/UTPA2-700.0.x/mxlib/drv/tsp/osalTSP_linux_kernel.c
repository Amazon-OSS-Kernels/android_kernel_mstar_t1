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
// file    osalTSP.c
// @brief  Transport Stream Processer (TSP) OSAL
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MSOS_TYPE_LINUX_KERNEL

#include "MsCommon.h"
#include "osalTSP.h"

#define TSP_TASK_STACK_SIZE     4096

extern MS_U32 _u32IntNum;

static MS_U8 _u8OsalTspStack[TSP_TASK_STACK_SIZE];

// Mutex
inline MS_S32 OSAL_TSP_MutexCreate(MS_U8* pu8Name)
{
    return MsOS_CreateMutex(E_MSOS_FIFO, (char*)pu8Name, MSOS_PROCESS_SHARED);
}

inline MS_BOOL OSAL_TSP_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    return MsOS_ObtainMutex(s32MutexId, u32Timeout);
}

inline MS_BOOL OSAL_TSP_MutexRelease(MS_S32 s32MutexId)
{
    return MsOS_ReleaseMutex(s32MutexId);
}

// Process
inline void OSAL_TSP_Proc(OSAL_TSP_ProcType Proc, OSAL_TSP_ProcTypeParam* pData, MS_U32* pu32Evt)
{
    Proc(pData, pu32Evt);
}

// @FIXME: E_IRQ_TSP should not used here
// Interrupt
inline void OSAL_TSP_IntDisable(void)
{
    MsOS_DisableInterrupt((InterruptNum)_u32IntNum);
}

inline void OSAL_TSP_IntEnable(void)
{
    MsOS_EnableInterrupt((InterruptNum)_u32IntNum);
}

inline void OSAL_TSP_IntAttach(OSAL_TSP_IsrType Proc)
{
    MsOS_AttachInterrupt((InterruptNum)_u32IntNum, (InterruptCb)Proc);
    #ifdef TSP_UTOPIA_20
    //MsOS_CompleteInterrupt((InterruptNum)_u32IntNum);
    #endif
}

inline void OSAL_TSP_IntDetach(void)
{
    MsOS_DetachInterrupt((InterruptNum)_u32IntNum);
}

// Event
inline MS_S32 OSAL_TSP_EventCreate(MS_U8* pu8Name)
{
    return MsOS_CreateEventGroup((char *)pu8Name);
}

inline MS_BOOL OSAL_TSP_EventDelete(MS_S32 s32EventGroupId)
{
    return MsOS_DeleteEventGroup(s32EventGroupId);
}

inline MS_BOOL OSAL_TSP_EventSet(MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
    return MsOS_SetEvent(s32EventGroupId, u32EventFlag);
}

inline MS_BOOL OSAL_TSP_EventGet(MS_S32 s32TspEventId, MS_U32 u32EventGroup, MS_U32* pu32RetrievedEventFlag, MS_U32 u32Opt, MS_U32 u32Timeout)
{
    EventWaitMode eWaitMode=E_AND;

    if (OSAL_EVENT_OR_CLEAR == u32Opt)
    {
        eWaitMode = E_OR_CLEAR;
}
    else if (OSAL_EVENT_OR == u32Opt)
    {
        eWaitMode = E_OR;
    }
    else if(u32Opt > E_OR_CLEAR)
    {
        MS_ASSERT(0);
    }
    return MsOS_WaitEvent(s32TspEventId, u32EventGroup, pu32RetrievedEventFlag, eWaitMode , u32Timeout);
}

inline MS_BOOL OSAL_TSP_FltEventGet(MS_S32 s32TspEventId, MS_U32 u32FilterID)
{
        return FALSE;
}

inline MS_S32 OSAL_TSP_TaskCreate(OSAL_TSP_ProcType Proc, MS_U8* pu8Name)
{
    return MsOS_CreateTask((TaskEntry)Proc,
                           (MS_U32)NULL,
#ifdef __arm__
                           E_TASK_PRI_MEDIUM,
#else
                           E_TASK_PRI_SYS,
#endif
                           TRUE,
                           _u8OsalTspStack,
                           TSP_TASK_STACK_SIZE,
                           (char *)pu8Name);
}
#endif // #ifdef MSOS_TYPE_NOS


