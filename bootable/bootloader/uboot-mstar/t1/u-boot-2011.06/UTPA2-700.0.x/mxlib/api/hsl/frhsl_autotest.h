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
/// @file MSrv_ChannelManager.h
/// @brief\b ChannelManager interface enumeration and struct definitaiotn.
///
/// ChannelManager is provid API for UI layer use.
///
/// Features:
/// - Provide the enum and struct definiation for ChannelManager.
///////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (MSOS_TYPE_LINUX)
#ifndef _FRHAUTOTEST_H_
#define _FRHAUTOTEST_H_

#include "frhsl_cfg.h"

// SH@ just for test

/*******************************************************************************
** Global Function Prototypes
*******************************************************************************/

MS_BOOL FrHsl_AutoTest_Init(stHslController* const _hsl);
void FrHsl_AutoTest_End(stHslController* const _hsl);
#if HSL_TCPIP
void CmdSend(stHslController* const _hsl, const void *message, int length);
#endif

#endif
#endif
