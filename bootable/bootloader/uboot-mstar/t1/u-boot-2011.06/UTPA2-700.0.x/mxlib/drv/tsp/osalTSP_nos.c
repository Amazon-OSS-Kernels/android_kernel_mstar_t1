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

#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)

#include "MsCommon.h"
#include "osalTSP.h"

#define TSP_NOS_EVT_NUM         0x10

extern MS_U32 _u32IntNum;

MS_S32 u32EventId[TSP_NOS_EVT_NUM];
MS_U32 u32EventFree = 0xFFFFFFFF;

// Mutex
inline MS_S32 OSAL_TSP_MutexCreate(MS_U8* pu8Name)
{
    return 0;
}

inline MS_BOOL OSAL_TSP_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    if(s32MutexId != -1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

inline MS_BOOL OSAL_TSP_MutexRelease(MS_S32 s32MutexId)
{
    if(s32MutexId != -1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
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
}

inline void OSAL_TSP_IntDetach(void)
{
    MsOS_DetachInterrupt((InterruptNum)_u32IntNum);
}

// Event
inline MS_S32 OSAL_TSP_EventCreate(MS_U8* pu8Name)
{
    MS_U32 i;

    for (i= 0; i< TSP_NOS_EVT_NUM; i++)
    {
        if (u32EventFree & (1 << i))
        {
            u32EventFree &= ~(1<< i);
            u32EventId[i] = 0x0;
            return (MS_S32) (i | 0x87650000);
        }
    }
    return -1;
}

inline MS_BOOL OSAL_TSP_EventDelete(MS_S32 s32TspEventId)
{
    if (0x87650000 != (0xFFFF0000 & s32TspEventId))
    {
        return FALSE;
    }
    else
    {
        s32TspEventId &= 0x0000FFFF;
    }
    u32EventFree |= (1<< s32TspEventId);
    u32EventId[s32TspEventId] = 0x0;
    return TRUE;
}

inline MS_BOOL OSAL_TSP_EventSet(MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
    MS_U32 u32Id;

    if (0x87650000 != (0xFFFF0000 & s32EventGroupId))
    {
        return FALSE;
    }

    u32Id = s32EventGroupId & 0x0000FFFF;
    u32EventId[u32Id] |= u32EventFlag;
    return TRUE;
}

inline MS_BOOL OSAL_TSP_EventGet(MS_S32 s32TspEventId, MS_U32 u32EventGroup, MS_U32* pu32RetrievedEventFlag, MS_U32 u32Opt, MS_U32 u32Timeout)
{
    MS_U32 u32Id;

    if (0x87650000 != (0xFFFF0000 & s32TspEventId))
    {
        return FALSE;
    }
    // @FIXME: nos verson don't support timeout at this stage

    u32Id = s32TspEventId & 0x0000FFFF;
    *pu32RetrievedEventFlag = u32EventId[u32Id] & u32EventGroup;
    if (OSAL_EVENT_OR_CLEAR == u32Opt)
    {
    u32EventId[u32Id] &= ~(u32EventGroup);
    }
    return (*pu32RetrievedEventFlag)? TRUE: FALSE;
}

/*
inline MS_BOOL OSAL_TSP_FltEventGet(MS_S32 s32TspEventId, MS_U32 u32FilterID)
{
    MS_U32  u32Id;
    MS_BOOL bResult;

    if (0x87650000 != (0xFFFF0000 & s32TspEventId))
    {
        return FALSE;
    }

    u32Id = s32TspEventId & 0x0000FFFF;

    //printf("u32EventId[u32Id] = %lx\n", u32EventId[u32Id]);

    bResult = u32EventId[u32Id] & BIT(u32FilterID);
    u32EventId[u32Id] &= ~BIT(u32FilterID);

    return bResult;
}
*/

inline MS_S32 OSAL_TSP_TaskCreate(OSAL_TSP_ProcType Proc, MS_U8* pu8Name)
{
    return 0;
}
#endif // #ifdef MSOS_TYPE_NOS
