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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   halMBX.h
/// @brief  MStar Mailbox HAL Driver DDI
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_MBX_H
#define _HAL_MBX_H

#ifdef _HAL_MBX_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Defines & Macros

//=============================================================================
//busy bit Set/Clear/Get
#define   _BUSY_S(arg)  {\
                            MS_U8 val; \
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val | MBX_STATE1_BUSY;\
                         }

#define   _BUSY_C(arg)  {\
                            MS_U8 val; \
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~MBX_STATE1_BUSY;\
                         }

#define   _BUSY(arg)    (REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) & MBX_STATE1_BUSY);

//////////////////////////////////////////////////////////////
//error bit Set/Clear/Get
#define   _ERR_S(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val | MBX_STATE1_ERROR;\
                         }

#define   _ERR_C(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~MBX_STATE1_ERROR;\
                         }

#define   _ERR(arg)    (REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) & MBX_STATE1_ERROR)

//////////////////////////////////////////////////////////////
//disabled bit Set/Clear/Get
#define   _DISABLED_S(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val | MBX_STATE1_DISABLED;\
                         }

#define   _DISABLED_C(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~MBX_STATE1_DISABLED;\
                         }

#define   _DISABLED(arg)    (REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) & MBX_STATE1_DISABLED)

////////////////////////////////////////////////////////////////////////
//overflow bit Set/Clear/Get
#define   _OVERFLOW_S(arg)  {\
                                MS_U8 val;\
                                val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                                REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val | MBX_STATE1_OVERFLOW;\
                              }

#define   _OVERFLOW_C(arg)  {\
                                MS_U8 val;\
                                val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                                REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~MBX_STATE1_OVERFLOW;\
                              }

#define   _OVERFLOW(arg)   (REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) & MBX_STATE1_OVERFLOW)

////////////////////////////////////////////////////////////////////////
//status bit clear
#define   _S1_C(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~(MBX_STATE1_DISABLED | MBX_STATE1_OVERFLOW | MBX_STATE1_ERROR | MBX_STATE1_BUSY);\
                        }

////////////////////////////////////////////////////////////////////////
//fire bit Set/Clear/Get
#define   _FIRE_S(arg)  {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                            REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val | MBX_CTRL_FIRE;\
                         }

#define   _FIRE_C(arg)  {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                            REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val & ~MBX_CTRL_FIRE;\
                         }

#define   _FIRE(arg)   (REG8_MBX_GROUP(arg, REG8_MBX_CTRL) & MBX_CTRL_FIRE)

////////////////////////////////////////////////////////////////////////
//readback bit Set/Clear/Get
#define   _READBACK_S(arg)   {\
                                  MS_U8 val;\
                                  val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                                  REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val | MBX_CTRL_READBACK;\
                               }

#define   _READBACK_C(arg)   {\
                                  MS_U8 val;\
                                  val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                                  REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val & ~MBX_CTRL_READBACK;\
                               }

#define   _READBACK(arg)   (REG8_MBX_GROUP(arg, REG8_MBX_CTRL) & MBX_CTRL_READBACK)

////////////////////////////////////////////////////////////////////////
//instant bit Set/Clear/Get
#define   _INSTANT_S(arg)   {\
                                  MS_U8 val;\
                                  val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                                  REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val | MBX_CTRL_INSTANT;\
                              }

#define   _INSTANT_C(arg)   {\
                                  MS_U8 val;\
                                  val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                                  REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val & ~MBX_CTRL_INSTANT;\
                              }

#define   _INSTANT(arg)   (REG8_MBX_GROUP(arg, REG8_MBX_CTRL) & MBX_CTRL_INSTANT)

//=============================================================================
// Type and Structure Declaration
//=============================================================================

//=============================================================================
// Enums
/// MBX HAL Recv Status Define
typedef enum
{
    /// Recv Success
    E_MBXHAL_RECV_SUCCESS = 0,
    /// Recv Error: OverFlow
    E_MBXHAL_RECV_OVERFLOW = 1,
    /// Recv Error: Not Enabled
    E_MBXHAL_RECV_DISABLED = 2,
} MBXHAL_Recv_Status;

/// MBX HAL Fire Status Define
typedef enum
{
    /// Fire Success
    E_MBXHAL_FIRE_SUCCESS = 0,
    /// Still Firing
    E_MBXHAL_FIRE_ONGOING = 1,
    /// Fire Error: Overflow:
    E_MBXHAL_FIRE_OVERFLOW = 2,
    /// Fire Error: Not Enabled
    E_MBXHAL_FIRE_DISABLED = 3,
} MBXHAL_Fire_Status;

//=============================================================================
// Mailbox HAL Driver Function
//=============================================================================

INTERFACE MBX_Result MHAL_MBX_ClearAll (MBX_Msg* pMbxMsg, MBX_ROLE_ID eSrcRole);

INTERFACE MBX_Result MHAL_MBX_Init(MBX_ROLE_ID eHostRole, MS_VIRT virtRIUBaseAddrMBX);
INTERFACE MBX_Result MHAL_MBX_SetConfig(MBX_ROLE_ID eHostRole);

INTERFACE MBX_Result MHAL_MBX_SetInformation(MBX_ROLE_ID eTargetRole, MS_VIRT virtRIUBaseAddrMBX, MS_U8 *pU8Info, MS_U8 u8Size);
INTERFACE MBX_Result MHAL_MBX_GetInformation(MBX_ROLE_ID eTargetRole, MS_VIRT virtRIUBaseAddrMBX, MS_U8 *pU8Info, MS_U8 u8Size);

INTERFACE MBX_Result MHAL_MBX_Fire(MBX_Msg* pMbxMsg, MBX_ROLE_ID eSrcRole);
INTERFACE MBX_Result MHAL_MBX_GetFireStatus(MBX_ROLE_ID eSrcRole, MBX_ROLE_ID eDstRole, MBXHAL_Fire_Status *pFireStatus);

INTERFACE MBX_Result MHAL_MBX_Recv(MBX_Msg* pMbxMsg, MBX_ROLE_ID eDstRole);
INTERFACE MBX_Result MHAL_MBX_RecvEnd(MBX_ROLE_ID eSrcRole, MBX_ROLE_ID eDstRole, MBXHAL_Recv_Status eRecvSatus);

#undef INTERFACE
#endif //_HAL_MBX_H

