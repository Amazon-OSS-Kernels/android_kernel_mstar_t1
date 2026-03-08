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
// file   osalTSP.h
// @brief  Transport Stream Processer (TSP) OSAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __OSAL_TSP_NOS_H__
#define __OSAL_TSP_NOS_H__

#include "MsOS.h"
#include "ULog.h"

#define _MDrv_TSP_GetEvent(events, flag, opt, timeout)  OSAL_TSP_EventGet(_s32TspEventId, (flag), &(events), (opt), (timeout))

/// interrupt service routine prototype
typedef struct
{
    MS_U32 u32FltId;
    void* pRet;
    MS_BOOL bInfiniteWait;
    MS_BOOL bFromTask;
} OSAL_TSP_ProcTypeParam;

typedef void (*OSAL_TSP_ProcType)(OSAL_TSP_ProcTypeParam* pData, MS_U32* pu32Evt);
typedef void (*OSAL_TSP_IsrType)(void);

#define OSAL_TSP_MUTEX_TIMEOUT   MSOS_WAIT_FOREVER

// printf
#define OSAL_TSP_Print(fmt,arg...)   ULOGD("TSP", fmt, ##arg)

MS_S32  OSAL_TSP_MutexCreate(MS_U8* pu8Name);
MS_BOOL OSAL_TSP_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout);
MS_BOOL OSAL_TSP_MutexRelease(MS_S32 s32MutexId);
void OSAL_TSP_Proc(OSAL_TSP_ProcType Proc, OSAL_TSP_ProcTypeParam* pData, MS_U32* pu32Evt);
void    OSAL_TSP_IntDisable(void);
void    OSAL_TSP_IntEnable(void);
void    OSAL_TSP_IntAttach(OSAL_TSP_IsrType Proc);
void    OSAL_TSP_IntDetach(void);
MS_S32  OSAL_TSP_EventCreate(MS_U8* pu8Name);
MS_BOOL OSAL_TSP_EventDelete(MS_S32 s32EventGroupId);
MS_BOOL OSAL_TSP_EventSet(MS_S32 s32EventGroupId, MS_U32 u32EventFlag);
MS_BOOL OSAL_TSP_EventGet(MS_S32 s32TspEventId, MS_U32 u32EventGroup, MS_U32* pu32RetrievedEventFlag, MS_U32 u32Opt, MS_U32 u32Timeout);
#define OSAL_EVENT_OR           0x0001
#define OSAL_EVENT_OR_CLEAR     0x0003

// Task
MS_S32 OSAL_TSP_TaskCreate(OSAL_TSP_ProcType Proc, MS_U8* pu8Name);

#endif // #ifndef __OSAL_TSP_NOS_H__
