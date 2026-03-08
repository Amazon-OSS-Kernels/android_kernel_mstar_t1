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
/// @file   drvSMBX.h
/// @brief  Simple Mailbox Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_SMBX_H__
#define __DRV_SMBX_H__



#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define SMBX_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'S','M','B','X'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','1'},                          /* 00 ~ 99                                          */  \
    {'0','0','3','9','7','8','9','4'},  /* CL#                                              */  \
    MSIF_OS

enum
{
	CMD_PM_INIT               = 0x01,
	CMD_PM_POWERDOWN,
	CMD_SECUREBOOT_GETKEY0,           //For Kernel Auth.
    CMD_SECUREBOOT_AES_ENCODE,
    CMD_SECUREBOOT_AES_DECODE,
    CMD_SECUREBOOT_GETKEY,
    CMD_SECUREBOOT_GETKEYDONE,
};

enum
{
	TRIGGER_MIPSto51          = 0x01,
	TRIGGER_51toMIPS,
};



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_SMBX_SUCCESS,
    E_SMBX_FAILED,
} SMBX_Result;

typedef struct
{
	MS_BOOL		bInitialized;
	MS_S32		s32SMBXLock;
} SMBX_Ctx;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

SMBX_Result MDrv_SMBX_Init(void);
SMBX_Result MDrv_SMBX_SendCmd(MS_U16 u16Cmd, MS_U16 *pU16Data, MS_U8 u8Num);
SMBX_Result MDrv_SMBX_Exit(void);
SMBX_Result MDrv_SMBX_GetLibVer(const MSIF_Version **ppVersion);


#ifdef __cplusplus
}
#endif


#endif // __DRV_TEMP_H__

