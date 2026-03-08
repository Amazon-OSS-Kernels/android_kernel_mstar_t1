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
#ifndef __HAL_SEM_H__
#define __HAL_SEM_H__

#include "MsCommon.h"
#include "drvSEM.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SEM_MAX_GP_CLIENT             (16)

#ifdef __cplusplus
extern "C"
{
#endif

MS_BOOL HAL_SEM_SetBank(MS_VIRT u32BaseAddr);
MS_BOOL HAL_SEM_Get_Resource(MS_U8 u8SemID, MS_U16 u16ResId);
MS_BOOL HAL_SEM_Free_Resource(MS_U8 u8SemID, MS_U16 u16ResId);
MS_BOOL HAL_SEM_Reset_Resource(MS_U8 u8SemID);
MS_BOOL HAL_SEM_Get_ResourceID(MS_U8 u8SemID, MS_U16* pu16ResId);
MS_U32 HAL_SEM_Get_Num(void);
MS_S16 HAL_SEM_GetSemId(eSemId SemId);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif // #ifndef __HAL_SEM_H__
