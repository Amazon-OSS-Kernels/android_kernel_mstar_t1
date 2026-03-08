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
// file    tsp2_msos.h
// @brief  Transport Stream Processer (TSP) OS Wrapper
////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(MSOS_TYPE_ECOS) || defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL) || defined(MSOS_TYPE_UCOS)

#ifndef __TSP_MSOS_H__
#define __TSP_MSOS_H__

typedef struct
{
    MS_U32                          u32FltId;
    void*                           pRet;
    MS_BOOL                         bInfiniteWait;
    MS_BOOL                         bFromTask;
} TSP_OS_ProcParam;

//typedef void                        (*TSP_OS_Proc)(TSP_OS_ProcParam* pData, MS_U32* pu32Evt);
typedef void                        (*TSP_OS_Proc)(void );
typedef void                        (*TSP_OS_Isr)(void);

#define TSP_OS_MUTEX_TIMEOUT        MSOS_WAIT_FOREVER

#define TSP_OS_Print(_f, _a...)     ULOGD("TSP", _f, ##_a)

#if defined(MSOS_TYPE_ECOS) || defined(MSOS_TYPE_UCOS)
#define TSP_ISR_Print(_f, _a...)    { }
#else
#define TSP_ISR_Print(_f, _a...)    ULOGD("TSP", _f, ##_a)
#endif

MS_S32  TSP_OS_MutexCreate(MS_U8* pu8Name);
MS_BOOL TSP_OS_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout);
MS_BOOL TSP_OS_MutexRelease(MS_S32 s32MutexId);
void    TSP_OS_MutexDelete(MS_S32 s32MutexId);
void    TSP_OS_IntDisable(void);
void    TSP_OS_IntEnable(void);
void    TSP_OS_IntAttach(TSP_OS_Isr Proc);
void    TSP_OS_IntDetach(void);
MS_S32  TSP_OS_EventCreate(MS_U8* pu8Name);
MS_BOOL TSP_OS_EventDelete(MS_S32 s32EventGroupId);
MS_BOOL TSP_OS_EventSet(MS_S32 s32EventGroupId, MS_U32 u32EventFlag);
MS_BOOL TSP_OS_EventGet(MS_S32 s32TspEventId, MS_U32 u32EventGroup, MS_U32* pu32RetrievedEventFlag, MS_U32 u32Opt, MS_U32 u32Timeout);
#define TSP_OS_EVENT_OR             0x0001
#define TSP_OS_EVENT_OR_CLEAR       0x0003
//MS_BOOL OSAL_TSP_FltEventGet(MS_S32 s32TspEventId, MS_U32 u32FilterID);

// Task
MS_S32  TSP_OS_TaskCreate(TSP_OS_Proc Proc, MS_U8* pu8Name);
MS_BOOL TSP_OS_TaskDelete(MS_S32 s32TaskId);


#endif // #ifndef __TSP_MSOS_H__

#endif // defined(MSOS_TYPE_ECOS) || defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_UCOS)

