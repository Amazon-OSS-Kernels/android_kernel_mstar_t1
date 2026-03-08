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
/// @file   halMBXINT.h
/// @brief  MStar Mailbox Interrupt HAL DDI
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MBX_INTERRUPT_H
#define _MHAL_MBX_INTERRUPT_H

#ifdef _MHAL_MBX_INTERRUPT_C
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

//=============================================================================
// Type and Structure Declaration
//=============================================================================
typedef void (*MBX_MSGRECV_CB_FUNC)(InterruptNum eIntNum);

//=============================================================================
// Function
//=============================================================================
INTERFACE MBX_Result MHAL_MBXINT_Init (MBX_CPU_ID eHostCPU, MBX_MSGRECV_CB_FUNC pMBXRecvMsgCBFunc, MS_VIRT virtRIUBaseAddrMBXINT);
INTERFACE void MHAL_MBXINT_DeInit (MBX_CPU_ID eHostCPU);
INTERFACE MBX_Result MHAL_MBXINT_ResetHostCPU (MBX_CPU_ID ePrevCPU, MBX_CPU_ID eConfigCpu);
INTERFACE MBX_Result MHAL_MBXINT_Fire (MBX_CPU_ID eDstCPUID, MBX_CPU_ID eSrcCPUID);
INTERFACE MBX_Result MHAL_MBXINT_CpuInit(MBX_CPU_ID eHKCPU);

#undef INTERFACE

#endif //_MHAL_MBX_INTERRUPT_H


