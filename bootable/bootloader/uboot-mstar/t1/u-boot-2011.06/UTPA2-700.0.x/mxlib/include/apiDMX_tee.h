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

///////////////////////////////////////////////////////////////////////////////
///
/// @file   apiDMX_tee.h
/// @brief  MStar Demux Driver Interface for Tee
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
/*! \defgroup DMX TEE modules
  */
//-------------------------------------------------

#ifndef __API_DMX_TEE_H__
#define __API_DMX_TEE_H__

#include "MsCommon.h"


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


typedef struct _DMX_TEE_BUF_Param
{
    MS_U8   u8EngId;             // PVR engine ID or Filein engine ID
    MS_U8   u8MiuSel;            // MIU selection, default is 0
    MS_U32  u32BufSize;          // Phsical Buffer Size
    MS_U32  u32BufAddr;          // Phsical Buffer Address
} DMX_TEE_BUF_Param;

typedef enum
{
    E_DMX_TSPFW_BUF,                ///< For TSP FW buffer control
    E_DMX_TSPVQ_BUF,                ///< For TSP VQ buffer control
    E_DMX_PVR_BUF,                  ///< For TSP PVR buffer control
    E_DMX_FILEIN_BUF,               ///< For TSP Finein buffer control
} DMX_TEE_USER_CMD;

//-----------------------------------------------------------------------------
//  Functions
//-----------------------------------------------------------------------------


//Used for TEE
//-------------------------------------------------------------------------------------------------
/// Create DMX Tee service task
/// @ingroup DMX_Tee
/// @return TRUE or FALSE for Pass or Fail
/// @note Please call this API when Tee system init, and must create this task before DMX init is called
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_Create_Tee_System(void);

//------------------------------------------------------------------------------
/// SetControl for TEE
/// @ingroup DMX_Tee
/// @param cmd \b IN : TEE user cmd
/// @param param \b IN : TEE control parameters
/// @return MS_BOOL
/// @note Tee set DMX setting inform to DMX
//------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_SetControl(DMX_TEE_USER_CMD cmd, void* param);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_DMX_TEE_H__
