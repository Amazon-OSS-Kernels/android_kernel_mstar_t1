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
// file   osalJPD.h
// @brief  JPD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __OSAL_JPD_H__
#define __OSAL_JPD_H__

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if defined( MSOS_TYPE_LINUX) || defined( MSOS_TYPE_ECOS)
    #define JPD_ENABLE_MSOS_SYSTEM_CALL     1
#endif

#define OSAL_JPD_MUTEX_TIMEOUT  MSOS_WAIT_FOREVER

#if defined(JPD_ENABLE_MSOS_SYSTEM_CALL)
#define JPD_Timer_Delayms(x)    MsOS_DelayTask(x)
#else
#define JPD_Timer_Delayms(x) \
    do{ \
        volatile MS_U32 jpd_ticks = 0; \
        while( jpd_ticks < (((MS_U32)(x))<<10) ) \
        { \
            jpd_ticks++; \
        } \
    }while(0)

#endif //JPD_ENABLE_MSOS_SYSTEM_CALL

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// JPD Interrupt Register Function
typedef void (*OSAL_JPD_IsrFuncCb)(MS_U32);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
// Mutex
MS_S32 OSAL_JPD_MutexCreate(MS_U8* pu8Name);
MS_BOOL OSAL_JPD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout);
MS_BOOL OSAL_JPD_MutexRelease(MS_S32 s32MutexId);
MS_BOOL OSAL_JPD_MutexDelete(MS_S32 s32MutexId);

//ISR
MS_BOOL OSAL_JPD_ISR_Disable(void);
MS_BOOL OSAL_JPD_ISR_Enable(void);
MS_BOOL OSAL_JPD_ISR_Attach(OSAL_JPD_IsrFuncCb pfnISRCB);
MS_BOOL OSAL_JPD_ISR_Detach(void);

#endif // #ifndef __OSAL_JPD_H__
