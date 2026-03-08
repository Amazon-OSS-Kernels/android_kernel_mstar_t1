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
// file   osalGPD.h
// @brief  GPD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __OSAL_GPD_H__
#define __OSAL_GPD_H__

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if defined( MSOS_TYPE_LINUX) || defined( MSOS_TYPE_ECOS)
    #define GPD_ENABLE_MSOS_SYSTEM_CALL     1
#endif

#define OSAL_GPD_MUTEX_TIMEOUT  MSOS_WAIT_FOREVER

#if defined(GPD_ENABLE_MSOS_SYSTEM_CALL)
#define GPD_Timer_Delayms(x)    MsOS_DelayTask(x)
#else
#define GPD_Timer_Delayms(x) \
    do{ \
        volatile MS_U32 GPD_ticks = 0; \
        while( GPD_ticks < (((MS_U32)(x))<<10) ) \
        { \
            GPD_ticks++; \
        } \
    }while(0)

#endif //GPD_ENABLE_MSOS_SYSTEM_CALL

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// GPD Interrupt Register Function
typedef void (*OSAL_GPD_IsrFuncCb)(MS_U32);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
// Mutex
MS_S32 OSAL_GPD_MutexCreate(MS_U8* pu8Name);
MS_BOOL OSAL_GPD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout);
MS_BOOL OSAL_GPD_MutexRelease(MS_S32 s32MutexId);
MS_BOOL OSAL_GPD_MutexDelete(MS_S32 s32MutexId);

//ISR
MS_BOOL OSAL_GPD_ISR_Disable(void);
MS_BOOL OSAL_GPD_ISR_Enable(void);
MS_BOOL OSAL_GPD_ISR_Attach(OSAL_GPD_IsrFuncCb pfnISRCB);
MS_BOOL OSAL_GPD_ISR_Detach(void);

#endif // #ifndef __OSAL_GPD_H__
