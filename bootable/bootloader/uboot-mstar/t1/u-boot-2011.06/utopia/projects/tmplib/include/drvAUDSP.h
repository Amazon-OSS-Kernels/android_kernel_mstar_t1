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

#ifndef _DRVAUDSP_H_
#define _DRVAUDSP_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
/// Version string.
#define MSIF_AUDSP_LIB_CODE                     {'A','D','S','P'}    //Lib code
#define MSIF_AUDSP_LIBVER                       {'0','0'}            //LIB version
#define MSIF_AUDSP_BUILDNUM                     {'3','F'}            //Build Number
#define MSIF_AUDSP_CHANGELIST                   {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define AUDSP_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_AUDSP_LIB_CODE,                        /* IP__                                             */  \
    MSIF_AUDSP_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_AUDSP_BUILDNUM,                        /* 00 ~ 99                                          */  \
    MSIF_AUDSP_CHANGELIST,                      /* CL#                                              */  \
    MSIF_OS

//================================================================
//  AUDSP version informtion functions
//================================================================
extern MS_BOOL MDrv_AUD_GetLibVer(const MSIF_Version **ppVersion);

#ifdef __cplusplus
}
#endif

#endif
