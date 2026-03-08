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
/// @file   drvCIPHER_v2.h
/// @brief  CIPHER Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_CIPHER_V2_H_
#define _DRV_CIPHER_V2_H_

#include "drvCIPHER.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    E_MDRV_CMD_CIPHER_Init,
    E_MDRV_CMD_CIPHER_Reset,
    E_MDRV_CMD_CIPHER_ResetKey,
    E_MDRV_CMD_CIPHER_DbgLevel,
    E_MDRV_CMD_CIPHER_DMACalc,
    E_MDRV_CMD_CIPHER_HASH,
    E_MDRV_CMD_CIPHER_IsDMADone,
    E_MDRV_CMD_CIPHER_IsHASHDone,

} eCipherIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef struct DLL_PACKED _CIPHER_RESETKEY
{
    MS_U32          u32CAVid;
    MS_U32          u32KeyIdx;
    DRV_CIPHER_RET  ret;
} CIPHER_RESETKEY;

typedef struct DLL_PACKED _CIPHER_DBGLEVEL
{
    CIPHER_DBGMSG_LEVEL eDBGMsgLevel;
} CIPHER_DBGLEVEL;

typedef struct DLL_PACKED _CIPHER_DMACALC
{
    DRV_CIPHER_DMACFG   stCfg;
    MS_U32              *pu32CmdId;
    DRV_CIPHER_RET      ret;
} CIPHER_DMACALC, *PCIPHER_DMACALC;

typedef struct DLL_PACKED _CIPHER_HASHCALC
{
    DRV_CIPHER_HASHCFG  stCfg;
    MS_U32              *pu32CmdId;
    DRV_CIPHER_RET      ret;
} CIPHER_HASHCALC;

typedef struct DLL_PACKED _CIPHER_ISDMADONE
{
    MS_U32 u32CmdId;
    MS_U32 *pu32Exception;
} CIPHER_ISDMADONE, *PCIPHER_ISDMADONE;

typedef struct DLL_PACKED _CIPHER_ISHASHDONE
{
    MS_U32 u32CmdId;
    MS_U32 *pu32Exception;
} CIPHER_ISHASHDONE;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
MS_U32 CIPHEROpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 CIPHERClose(void* pInstantTmp);
MS_U32 CIPHERIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pu32Args);

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_CIPHER_V2_H_
