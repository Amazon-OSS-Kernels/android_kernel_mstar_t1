/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
/**
 *
 *
 */

#define _DRVAUDSP_C

///////////////////////////////////////////////////////////////////////////////
//
/// @file drvByteDMA.c
/// @brief DRAM byte DMA control driver
///////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "drvAUDSP.h"
#include "halAUDSP.h"
#include "./internal/drvAUDIO_internal.h"

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

static MSIF_Version _drv_audsp_version = {
    .DDI = { AUDSP_DRV_VERSION, },
};

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDSP_GetLibVer()
/// @brief \b Function \b Description:  This function is used to get version information for audsp module
/// @return   MS_BOOL  \b : TRUE or FALSE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDSP_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_audsp_version;
    return TRUE;
}

