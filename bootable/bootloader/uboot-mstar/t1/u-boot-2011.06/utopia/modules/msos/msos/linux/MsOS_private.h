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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   MsOS_private.h
/// @brief  MStar OS Wrapper
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MS_OS_PRIVATE_H_
#define _MS_OS_PRIVATE_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
/// MsOS_SHM_Expand (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SHM_Expand(MS_U32 u32ShmSize, MS_U32 u32ExpandSize);

//-------------------------------------------------------------------------------------------------
/// MsOS_CrossThreadUnlockMutex (Detail need to be done.)
/// @ingroup MsOS_Task
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CrossThreadUnlockMutex(MS_S32 u32Index, MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif // _MS_OS_PRIVATE_H_
