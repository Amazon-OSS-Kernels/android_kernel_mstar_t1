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
// file   osalRVD.h
// @brief  RVD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __OSAL_RVD_H__
#define __OSAL_RVD_H__

#include "MsOS.h"
#if defined( MSOS_TYPE_LINUX) || defined( MSOS_TYPE_ECOS)
    #define RVD_ENABLE_MSOS_SYSTEM_CALL     1
#endif

#define RVD_ISR_VECTOR          E_INT_IRQ_SVD_HVD
#define OSAL_RVD_MUTEX_TIMEOUT  MSOS_WAIT_FOREVER


MS_S32 OSAL_RVD_MutexCreate(MS_U8* pu8Name);
MS_BOOL OSAL_RVD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout);
MS_BOOL OSAL_RVD_MutexRelease(MS_S32 s32MutexId);
MS_BOOL OSAL_RVD_MutexDelete(MS_S32 s32MutexId);

MS_BOOL OSAL_RVD_ISR_Disable(void);
MS_BOOL OSAL_RVD_ISR_Enable(void);
MS_BOOL OSAL_RVD_ISR_Attach(void* pfnISRCB);
MS_BOOL OSAL_RVD_ISR_Detach(void);

#endif // #ifndef __OSAL_HVD_H__
