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
// file   osalJPD.c
// @brief  RVD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsOS.h"
#include "osalJPD.h"
#include "jpeg_def.h"

#if SUPPORT_OJPD
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//define JPD ISR vector
#if defined(CHIP_T2)
#define JPD_ISR_VECTOR  E_INT_FIQ_JPD
#else
#define JPD_ISR_VECTOR  E_INT_IRQ_JPD
#endif


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
OSAL_JPD_IsrFuncCb _pfnJPDIsrCb = NULL;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _OSAL_JPD_ISRHandler(InterruptNum num)
{
    if(_pfnJPDIsrCb)
    {
        _pfnJPDIsrCb((MS_U32)num);
    }
    MsOS_EnableInterrupt(JPD_ISR_VECTOR);
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
// Mutex
inline MS_S32 OSAL_JPD_MutexCreate(MS_U8* pu8Name)
{
    return MsOS_CreateMutex(E_MSOS_FIFO, (char *)pu8Name, MSOS_PROCESS_SHARED);
}

inline MS_BOOL OSAL_JPD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    return MsOS_ObtainMutex(s32MutexId, u32Timeout);
}

inline MS_BOOL OSAL_JPD_MutexRelease(MS_S32 s32MutexId)
{
    return MsOS_ReleaseMutex(s32MutexId);
}

inline MS_BOOL OSAL_JPD_MutexDelete(MS_S32 s32MutexId)
{
    return MsOS_DeleteMutex(s32MutexId);
}

//ISR
inline MS_BOOL OSAL_JPD_ISR_Disable(void)
{
    return MsOS_DisableInterrupt(JPD_ISR_VECTOR);
}

inline MS_BOOL OSAL_JPD_ISR_Enable(void)
{
    return MsOS_EnableInterrupt(JPD_ISR_VECTOR);
}

inline MS_BOOL OSAL_JPD_ISR_Attach(OSAL_JPD_IsrFuncCb pfnISRCB)
{
    _pfnJPDIsrCb = pfnISRCB;
    return MsOS_AttachInterrupt(JPD_ISR_VECTOR, (InterruptCb)_OSAL_JPD_ISRHandler);
}

inline MS_BOOL OSAL_JPD_ISR_Detach(void)
{
    _pfnJPDIsrCb = NULL;
    return MsOS_DetachInterrupt(JPD_ISR_VECTOR);
}


#endif

