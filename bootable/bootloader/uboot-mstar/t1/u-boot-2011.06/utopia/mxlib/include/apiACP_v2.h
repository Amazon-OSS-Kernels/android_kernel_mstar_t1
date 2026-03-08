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
/// @file   apiACP_v2.h
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ACP_V2_H_
#define _ACP_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

/// ACP CMD
typedef enum
{
    //ACP property
    E_ACP_SET_PROTECTION,
    E_ACP_DCS_PROTECTION,
    E_ACP_DCS_SETACTIVATIONKEY,
    E_ACP_SET_MV_BITCONTROL,
}E_ACP_IOCTL_CMDS;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bIsYPbPr;
    MS_U8 u8Type;
    ACP_Result eReturnValue;
} stACP_SET_PROTECTION,*pstACP_SET_PROTECTION;

typedef struct
{
    MS_BOOL bEnable;
    MS_ACP_MV_BitControl MV_BitControl_Data;
    ACP_Result eReturnValue;
} stACP_SET_MV_BITCONTROL,*pstACP_SET_MV_BITCONTROL;

typedef struct
{
    MS_BOOL bEnable;
    MS_U8 u8Type;
    ACP_Result eReturnValue;
} stACP_DCS_PROTECTION,*pstACP_DCS_PROTECTION;

typedef struct
{
    MS_U8 *pu8ActivationKeyTbl;
    MS_U8 u8ActivationKeyTblSize;
    ACP_Result eReturnValue;
} stACP_DCS_SETACTIVATIONKEY,*pstACP_DCS_SETACTIVATIONKEY;

#ifdef __cplusplus
}
#endif

#endif // _API_ACP_H_

