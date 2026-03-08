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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiACP.h
/// @brief  ACP Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_ACP_H_
#define _API_ACP_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// library information
#define MSIF_ACP_LIB_CODE               {'A','C','P','_'}
#define MSIF_ACP_LIBVER                 {'0','0'}
#define MSIF_ACP_BUILDNUM               {'0','2'}
#define MSIF_ACP_CHANGELIST             {'0','0','6','1','4','0','9','2'}

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

// Result of VE function
typedef enum _ACP_Result
{
    E_ACP_NOT_SUPPORT = -1,
    E_ACP_FAIL = 0,
    E_ACP_OK = 1,
} ACP_Result;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//----------------------------------------------------------------
// MApi_ACP_SetProtection
// enable/disable protection for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @bIsYPbPr: TRUE: component, FALSE: CVBS
// @u8Type: protection type
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_ACP_SetProtection(MS_BOOL bEnable, MS_BOOL bIsYPbPr, MS_U8 u8Type);

//----------------------------------------------------------------
// MApi_DCS_SetProtection
// enable/disable DCS protection for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @u8Type: protection type
// @return ACP_Result
//----------------------------------------------------------------
// ACP_Result MApi_DCS_SetProtection(MS_BOOL bEnable, MS_U8 u8Type);

//----------------------------------------------------------------
// MApi_DCS_SetActivationKey
// DCS protection Activation Key Setting By External
// @pu8ActivationKeyTbl: External Activation Key Table
// @u8ActivationKeyTblSize: External Activation Key Table Size
// @return ACP_Result
//----------------------------------------------------------------
// ACP_Result MApi_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);

#ifdef __cplusplus
}
#endif

#endif

