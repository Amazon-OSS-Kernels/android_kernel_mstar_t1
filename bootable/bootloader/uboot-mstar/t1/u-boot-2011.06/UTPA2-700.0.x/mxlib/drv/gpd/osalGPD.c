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
// file   osalGPD.c
// @brief  RVD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsOS.h"
#include "osalGPD.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//define GPD ISR vector
#define GPD_ISR_VECTOR  E_INT_IRQ_GPD

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static OSAL_GPD_IsrFuncCb _pfnGPDIsrCb = NULL;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _OSAL_GPD_ISRHandler(InterruptNum num)
{
    if(_pfnGPDIsrCb)
    {
        _pfnGPDIsrCb((MS_U32)num);
    }
    MsOS_EnableInterrupt(GPD_ISR_VECTOR);
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
// Mutex
inline MS_S32 OSAL_GPD_MutexCreate(MS_U8* pu8Name)
{
    return MsOS_CreateMutex(E_MSOS_FIFO, (char *)pu8Name, MSOS_PROCESS_SHARED);
}

inline MS_BOOL OSAL_GPD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    return MsOS_ObtainMutex(s32MutexId, u32Timeout);
}

inline MS_BOOL OSAL_GPD_MutexRelease(MS_S32 s32MutexId)
{
    return MsOS_ReleaseMutex(s32MutexId);
}

inline MS_BOOL OSAL_GPD_MutexDelete(MS_S32 s32MutexId)
{
    return MsOS_DeleteMutex(s32MutexId);
}

//ISR
inline MS_BOOL OSAL_GPD_ISR_Disable(void)
{
    return MsOS_DisableInterrupt(GPD_ISR_VECTOR);
}

inline MS_BOOL OSAL_GPD_ISR_Enable(void)
{
    return MsOS_EnableInterrupt(GPD_ISR_VECTOR);
}

inline MS_BOOL OSAL_GPD_ISR_Attach(OSAL_GPD_IsrFuncCb pfnISRCB)
{
    _pfnGPDIsrCb = pfnISRCB;
    return MsOS_AttachInterrupt(GPD_ISR_VECTOR, (InterruptCb)_OSAL_GPD_ISRHandler);
}

inline MS_BOOL OSAL_GPD_ISR_Detach(void)
{
    _pfnGPDIsrCb = NULL;
    return MsOS_DetachInterrupt(GPD_ISR_VECTOR);
}



