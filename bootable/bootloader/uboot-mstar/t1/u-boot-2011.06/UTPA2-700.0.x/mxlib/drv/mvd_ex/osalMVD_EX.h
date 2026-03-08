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
// file   osalMVD.h
// @brief  MPEG Video Decoder (MVD) OSAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __OSAL_MVD_H__
#define __OSAL_MVD_H__

#include "MsOS.h"

#define OSAL_MVD_MUTEX_TIMEOUT   MSOS_WAIT_FOREVER

MS_S32 OSAL_MVD_MutexCreate(MS_U8* pu8Name);
MS_BOOL OSAL_MVD_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout);
MS_BOOL OSAL_MVD_MutexRelease(MS_S32 s32MutexId);
MS_BOOL OSAL_MVD_MutexDelete(MS_S32 s32MutexId);

MS_BOOL OSAL_MVD_MutexInit(void);
MS_BOOL OSAL_MVD_LockHwMutex(void);
MS_BOOL OSAL_MVD_UnlockHwMutex(void);
MS_BOOL OSAL_MVD_MutexExit(void);

#if 1//defined(CHIP_T2) || defined(CHIP_U3)) || defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_U4) || defined(CHIP_JANUS)
MS_BOOL OSAL_MVD_IntDisable(void);
MS_BOOL OSAL_MVD_IntEnable(void);
MS_BOOL OSAL_MVD_IsrAttach(void* pfnIsr);
MS_BOOL OSAL_MVD_IsrDetach(void);
#else
#define OSAL_MVD_IntDisable()
#define OSAL_MVD_IntEnable()
#define OSAL_MVD_IsrAttach(x)
#define OSAL_MVD_IsrDetach()
#endif

#endif // #ifndef __OSAL_MVD_H__


