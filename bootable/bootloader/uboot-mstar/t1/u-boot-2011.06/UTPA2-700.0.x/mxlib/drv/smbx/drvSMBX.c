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
///////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvSMBX.c
/// @brief  Simple Mailbox Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvSMBX.h"

// Internal Definition
#include "regSMBX.h"
#include "halSMBX.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

// SMBX driver info
static MSIF_Version                 _drv_smbx_Version = {
                                        .DDI = { SMBX_DRV_VERSION, },
                                    };

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

SMBX_Result MDrv_SMBX_Init(void)
{
    return MHAL_SMBX_Init();
}

SMBX_Result MDrv_SMBX_SendCmd(MS_U16 u16Cmd, MS_U16 *pU16Data, MS_U8 u8Num)
{
	return MHAL_SMBX_SendCmd(u16Cmd, pU16Data, u8Num);
}

SMBX_Result MDrv_SMBX_Exit(void)
{
    return MHAL_SMBX_Exit();
}

SMBX_Result MDrv_SMBX_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return E_SMBX_FAILED;

    *ppVersion = &_drv_smbx_Version;
    return E_SMBX_SUCCESS;
}

