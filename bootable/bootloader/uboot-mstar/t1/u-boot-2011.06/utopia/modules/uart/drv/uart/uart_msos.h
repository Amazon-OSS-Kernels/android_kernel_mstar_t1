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
/// file    uart_msos.h
/// @brief  UART Driver MsOS Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#if (defined(OS_ECOS) || defined(OS_LINUX))
#ifndef _UART_MSOS_H_
#define _UART_MSOS_H_


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsOS.h"


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

extern MS_BOOL OS_UART_Init(MS_U8 u8UARTID) ;
extern void    OS_UART_ISR_Enable(MS_U8 u8UARTID, P_UART_ISR_Proc pfISRProc) ;
extern void    OS_UART_ISR_Disable(MS_U8 u8UARTID) ;

#endif // _SC_MSOS_H_
#endif // (defined(OS_ECOS) || defined(OS_LINUX))

