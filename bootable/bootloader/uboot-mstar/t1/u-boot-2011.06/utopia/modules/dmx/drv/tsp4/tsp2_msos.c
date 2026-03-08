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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    tsp2_msos.c
/// @brief  Transport Stream Processer (TSP) OS Wrapper
///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(MSOS_TYPE_ECOS) || defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL) || defined(MSOS_TYPE_UCOS)

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "tsp2_msos.h"

extern MS_U32                       _u32IntNum;


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define TSP_OS_STACK_SIZE           (0x1000)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

#if defined (MSOS_TYPE_ECOS) || defined(MSOS_TYPE_UCOS)
static MS_U32                       _TSP_OS_Stack[TSP_OS_STACK_SIZE/sizeof(MS_U32)];
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

MS_S32 TSP_OS_MutexCreate(MS_U8* pu8Name)
{
    return MsOS_CreateMutex(E_MSOS_FIFO, (char *)pu8Name, MSOS_PROCESS_SHARED);
}


MS_BOOL TSP_OS_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    return MsOS_ObtainMutex(s32MutexId, u32Timeout);
}


MS_BOOL TSP_OS_MutexRelease(MS_S32 s32MutexId)
{
    return MsOS_ReleaseMutex(s32MutexId);
}


void TSP_OS_MutexDelete(MS_S32 s32MutexId)
{
    MsOS_ReleaseMutex(s32MutexId);
    MsOS_DeleteMutex(s32MutexId);
}


void TSP_OS_IntDisable(void)
{
    MsOS_DisableInterrupt((InterruptNum)_u32IntNum);
}


void TSP_OS_IntEnable(void)
{
    MsOS_EnableInterrupt((InterruptNum)_u32IntNum);
    #if defined(TSP_UTOPIA_20) && defined(MSOS_TYPE_LINUX)
    MsOS_CompleteInterrupt((InterruptNum)_u32IntNum);
    #endif
}

void TSP_OS_IntAttach(TSP_OS_Isr Proc)
{
    MsOS_AttachInterrupt((InterruptNum)_u32IntNum, (InterruptCb)Proc);
}


void TSP_OS_IntDetach(void)
{
    MsOS_DetachInterrupt((InterruptNum)_u32IntNum);
}


MS_S32 TSP_OS_EventCreate(MS_U8* pu8Name)
{
    return MsOS_CreateEventGroup((char *)pu8Name);
}


MS_BOOL TSP_OS_EventDelete(MS_S32 s32EventId)
{
    return MsOS_DeleteEventGroup(s32EventId);
}


MS_BOOL TSP_OS_EventSet(MS_S32 s32EventId, MS_U32 u32EventFlag)
{
    return MsOS_SetEvent(s32EventId, u32EventFlag);
}


MS_BOOL TSP_OS_EventGet(MS_S32 s32EventId, MS_U32 u32EventGroup, MS_U32* pu32RetrievedEventFlag, MS_U32 u32Opt, MS_U32 u32Timeout)
{
    EventWaitMode eWaitMode = E_AND;

    if (TSP_OS_EVENT_OR_CLEAR == u32Opt)
    {
        eWaitMode = E_OR_CLEAR;
    }
    else if (TSP_OS_EVENT_OR == u32Opt)
    {
        eWaitMode = E_OR;
    }
    else
    {
        MS_ASSERT(0);
    }
    return MsOS_WaitEvent(s32EventId, u32EventGroup, pu32RetrievedEventFlag, eWaitMode, u32Timeout);
}

/*
inline MS_BOOL OSAL_TSP_FltEventGet(MS_S32 s32TspEventId, MS_U32 u32FilterID)
{
    return FALSE;
}
*/

MS_S32 TSP_OS_TaskCreate(TSP_OS_Proc Proc, MS_U8* pu8Name)
{
#if defined (MSOS_TYPE_ECOS) || defined(MSOS_TYPE_UCOS)

    return MsOS_CreateTask((TaskEntry) Proc,
                           (MS_U32) NULL,
                           E_TASK_PRI_SYS,
                           TRUE,
                           _TSP_OS_Stack,
                           TSP_OS_STACK_SIZE,
                           (char*) pu8Name);
#else

    return MsOS_CreateTask((TaskEntry) Proc,
                           (MS_U32) NULL,
                           E_TASK_PRI_SYS,
                           TRUE,
                           NULL,
                           0,
                           (char*) pu8Name);
#endif
}


MS_BOOL TSP_OS_TaskDelete(MS_S32 s32TaskId)
{
    return MsOS_DeleteTask(s32TaskId);
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


#endif // defined(MSOS_TYPE_ECOS) || defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_UCOS)

