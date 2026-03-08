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
/// @file   drvSAR.h
/// @brief  SAR Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_SAR SAR interface
   *  \ingroup  G_PERIPHERAL
     *! \defgroup G_SAR_INIT Initialization Task relative
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_COMMON Common Task relative
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_CONTROL Control relative
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_OTHER  other relative
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_ToBeModified SAR api to be modified
     *  \ingroup  G_SAR
     *! \defgroup G_SAR_ToBeRemove SAR api to be removed
     *  \ingroup  G_SAR
*/

#ifndef _DRV_CUS_SAR_H_
#define _DRV_CUS_SAR_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// define SAR library version
#define MSIF_CUS_SAR_LIB_CODE               {'C','S','A','R'}
#define MSIF_CUS_SAR_LIBVER                 {'0','3'}
#define MSIF_CUS_SAR_BUILDNUM               {'0','9'}
#define MSIF_CUS_SAR_CHANGELIST             {'0','0','4','3','3','2','5','1'}

#define CUS_SAR_DRV_VERSION             /* Character String for DRV/API version  */  \
    MSIF_TAG,                           /* 'MSIF' */  \
    MSIF_CLASS,                         /* '00' */  \
    MSIF_CUS,                           /* 0x0000 */  \
    MSIF_MOD,                           /* 0x0000 */  \
    MSIF_CHIP,                                       \
    MSIF_CPU,                                        \
    MSIF_CUS_SAR_LIB_CODE,              /* IP__ */  \
    MSIF_CUS_SAR_LIBVER,                /* 0.0 ~ Z.Z */  \
    MSIF_CUS_SAR_BUILDNUM,              /* 00 ~ 99 */  \
    MSIF_CUS_SAR_CHANGELIST,            /* CL# */  \
    MSIF_OS

/// SAR driver
#define CUS_SAR_VERSION_STRING_MAX      33
#define CUS_SAR_DUMMY_KEY               0xFF
#define CUS_SAR_DUMMY_STATUS            0x00
#define CUS_SAR_VERSION            0x00000001

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//##################################
//
//  Access SAR settings by using Mailbox
//  sent commands to PM
//
//##################################


//-------------------------------------------------------------------------------------------------
/// Get SAR Keypad key code function.
/// @ingroup G_SAR_COMMON
/// @param pu8Key: key code pointer
/// @param pu8Repeat: key repeat pointer
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_GetKeyCode_Custom(MS_U8 *pu8Key, MS_U8 *pu8Repeat);

//-------------------------------------------------------------------------------------------------
/// Get SAR Multi-Keypad key code function with every SAR channel
/// @ingroup G_SAR_COMMON
/// @param SAR_KeyValue: key code struct pointer
/// @param u32Size: Struct Size
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_GetMultiKeyCode_Custom(SAR_KeyValue *sSarKey, MS_U32 u32Size, MS_U8 u8ReplyKey);


#ifdef __cplusplus
}
#endif


#endif // _DRV_SAR_H_

