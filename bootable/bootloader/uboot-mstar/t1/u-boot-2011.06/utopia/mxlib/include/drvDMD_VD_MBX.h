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
/// @file   drvSYS.h
/// @brief  System Control Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_SYS_DMD_VD_MBX_H_
#define _DRV_SYS_DMD_VD_MBX_H_

#ifdef __cplusplus
extern "C"
{
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_DMD_VD_MBX_TYPE_ATV,
    E_DMD_VD_MBX_TYPE_DVBT,
    E_DMD_VD_MBX_TYPE_DVBC,
    E_DMD_VD_MBX_TYPE_ATSC,
    E_DMD_VD_MBX_TYPE_DVBS,
    E_DMD_VD_MBX_TYPE_ISDBT,
    E_DMD_VD_MBX_TYPE_DTMB,
    E_DMD_VD_MBX_TYPE_DVBT2,
    E_DMD_VD_MBX_TYPE_MAX
}DMD_VD_MBX_Type;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_DMD_VD_MBX_Init(void);
void MDrv_SYS_DMD_VD_MBX_SetType(DMD_VD_MBX_Type eType);
MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadDSPReg(MS_U32 u32Reg, MS_U8 *u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteReg(MS_U32 u32Reg, MS_U8 u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteDSPReg(MS_U32 u32Reg, MS_U8 u8Value);
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_WriteReg(MS_U32 u32Reg, MS_U8 u8Value);
#ifdef __cplusplus
}
#endif
#endif // _DRV_SYS_DMD_VD_MBX_H_
