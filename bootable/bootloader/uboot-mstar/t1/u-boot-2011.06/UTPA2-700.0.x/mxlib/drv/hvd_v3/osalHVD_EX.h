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
// file   osalHVD.h
// @brief  HVD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __OSAL_HVD_H__
#define __OSAL_HVD_H__

#include "MsOS.h"

//#if defined( MSOS_TYPE_LINUX) || defined( MSOS_TYPE_ECOS) || defined( MSOS_TYPE_NOS)
//    #define HVD_ENABLE_MSOS_SYSTEM_CALL     1
//#endif


#define HVD_ISR_VECTOR          E_INT_IRQ_SVD_HVD
#define OSAL_HVD_MUTEX_TIMEOUT  MSOS_WAIT_FOREVER

#define EVD_ISR_VECTOR         E_INT_IRQ_EVD

MS_S32 OSAL_HVD_MutexCreate(MS_U8* pu8Name);
MS_BOOL OSAL_HVD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout);
MS_BOOL OSAL_HVD_MutexRelease(MS_S32 s32MutexId);
MS_BOOL OSAL_HVD_MutexDelete(MS_S32 s32MutexId);

MS_BOOL OSAL_HVD_ISR_Attach(MS_U32 u32ISRType, void* pfnISRCB);
MS_BOOL OSAL_HVD_ISR_Detach(MS_U32 u32ISRType);
MS_BOOL OSAL_HVD_ISR_Enable(MS_U32 u32ISRType);
MS_BOOL OSAL_HVD_ISR_Disable(MS_U32 u32ISRType);

#endif // #ifndef __OSAL_HVD_H__
