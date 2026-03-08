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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvPVR_IframeLUT_v2.h
/// @brief  PVR IframeLUT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_PVR_IFRAMELUT_V2_H_
#define _DRV_PVR_IFRAMELUT_V2_H_

#include "drvPVR_IframeLUT.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum
{
    E_MDrv_CMD_PVR_IframeLUT_Init,
    E_MDrv_CMD_PVR_IframeLUT_Open,
    E_MDrv_CMD_PVR_IframeLUT_SetVdecType,
    E_MDrv_CMD_PVR_IframeLUT_GetWritePtr,
    E_MDrv_CMD_PVR_IframeLUT_Close,
    E_MDrv_CMD_PVR_IframeLUT_Exit,
} ePVRIframeLUTIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

// param of MDrv_PVR_IframeLUT_Init is void

// param of MDrv_PVR_IframeLUT_Open
typedef PVR_IFRAMELUT_Cfg PVR_IFRAMELUT_OPEN_PARAM;

// param of MDrv_PVR_IframeLUT_SetVdecType
typedef struct DLL_PACKED
{
    MS_U32 u32PvrEng;
    EN_PVR_IFRAMELUT_VDEC_TYPE eVdecType;
} PVR_IFRAMELUT_SETVDECTYPE_PARAM;

// param of MDrv_PVR_IframeLUT_GetWritePtr
typedef struct DLL_PACKED
{
    MS_U32 u32PvrEng;
    MS_U32 *pu32WritePtr;
} PVR_IFRAMELUT_GETWRITEPTR_PARAM;

// param of MDrv_PVR_IframeLUT_Close
typedef MS_U32 PVR_IFRAMELUT_CLOSE_PARAM;

// param of MDrv_PVR_IframeLUT_Exit is void

#ifdef __cplusplus
}
#endif

#endif // _DRV_PVR_IFRAMELUT_V2_H_
