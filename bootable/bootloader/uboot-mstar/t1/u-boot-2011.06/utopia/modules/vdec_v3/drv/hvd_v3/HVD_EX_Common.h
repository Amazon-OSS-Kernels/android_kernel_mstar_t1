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
/// @file   drvHVD.h
/// @brief  HVD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HVD_H_
#define _DRV_HVD_H_

#ifdef __cplusplus
extern "C"
{
#endif
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief \b HVD_DRV_VERSION : HVD Version
//------------------------------------------------------------------------------
MS_BOOL  _MDrv_HVD_AUTH_IPCheck(MS_U32 u32CodecType);

HVD_EX_Result _HVD_EX_Init_(MS_U32 u32Id,MS_U32 u32ModeFlag);


#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_HVD_H_

