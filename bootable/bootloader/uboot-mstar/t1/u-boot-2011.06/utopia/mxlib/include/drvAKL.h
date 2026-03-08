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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvAKL.h
/// @brief  AKL Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_AKL_H__
#define __DRV_AKL_H__

#include "MsTypes.h"
#include "MsCommon.h"

#include "UFO.h"

#ifdef UFO_AKL_CERT

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define AKL_SET_KEY_TIMEOUT_VALUE    (20UL)

#define DRV_AKL_OK                   (0x00000000UL)
#define DRV_AKL_FAIL                 (0x00000001UL)
#define DRV_AKL_BAD_PARAM            (0x00000002UL)
#define DRV_AKL_TIMEOUT              (0x00000003UL)
//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------
typedef MS_U32                         DRV_AKL_RET;

typedef enum
{
    E_AKL_DBG_NONE,
    E_AKL_DBG_ERR,
    E_AKL_DBG_WARN,
    E_AKL_DBG_INFO,
}AKL_DBGMSG_LEVEL;

typedef enum
{
    E_AKL_ENG_NONE,
    E_AKL_ENG_LSAS,
    E_AKL_ENG_ESA,
    E_AKL_ENG_LSAD,
} AKL_Eng_Type;

typedef enum
{
    E_AKL_KEY_CLEAR,
    E_AKL_KEY_EVEN,
    E_AKL_KEY_ODD,
} AKL_Key_Type;

typedef enum
{
    E_AKL_CERT_CMD_SET_INPUT,
    E_AKL_CERT_CMD_SET_OPCODE,
    E_AKL_CERT_CMD_GET_OPCODE,
    E_AKL_CERT_CMD_GET_STATUS,
    E_AKL_CERT_CMD_GET_OUPUT,
} AKL_CERT_Cmd;

MS_BOOL MDrv_AKL_Init(void);
MS_BOOL MDrv_AKL_Reset(void);
MS_BOOL MDrv_AKL_SetDbgLevel(AKL_DBGMSG_LEVEL eDbgLevel);
MS_BOOL MDrv_AKL_SetManualACK(void);
DRV_AKL_RET MDrv_AKL_SetDMAKey(MS_U32 u32KeyIndex);
DRV_AKL_RET MDrv_AKL_SetDMAParserKey(MS_U32 u32OddKeyIndex, MS_U32 u32EvenKeyIndex);
DRV_AKL_RET MDrv_AKL_SetDSCMBKey(MS_U32 u32EngId, MS_U32 u32Index, AKL_Eng_Type eEngType, AKL_Key_Type eKeyType);
DRV_AKL_RET MDrv_AKL_SetTSIOKey(MS_U32 u32ServiceId, AKL_Key_Type eKeyType);
DRV_AKL_RET MDrv_AKL_CERTCmd(AKL_CERT_Cmd eCmd, MS_U8 *pu8Data, MS_U32 u32DataSize);

MS_BOOL MDrv_AKL_SetManualACKMode(MS_BOOL bEnable);  //K series

#ifdef __cplusplus
}
#endif

#endif  //UFO_AKL_CERT

#endif
