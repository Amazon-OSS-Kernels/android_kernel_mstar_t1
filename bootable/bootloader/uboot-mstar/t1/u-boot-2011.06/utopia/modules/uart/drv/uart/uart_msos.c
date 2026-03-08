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
/// file    drvSC_msos.c
/// @brief  SmartCard Driver MsOS Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#if (defined(OS_ECOS) || defined(OS_LINUX))


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvUART.h"

// Internal Definition
#include "uart_msos.h"
#include "regUART.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static P_UART_ISR_Proc                _os_UART_ISR_Proc[UART_PIU_NUM]; // in case there should be multiple ISR


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

static void _OS_UART_ISR0(InterruptNum eIntNum)
{
    _os_UART_ISR_Proc[0](0);
    MsOS_EnableInterrupt(UART0_PIU_IRQ);
}

#ifdef UART1_PIU_ADDR
static void _OS_UART_ISR1(InterruptNum eIntNum)
{
    _os_UART_ISR_Proc[1](1);
    MsOS_EnableInterrupt(UART1_PIU_IRQ);
}
#endif

#ifdef UART2_PIU_ADDR
static void _OS_UART_ISR2(InterruptNum eIntNum)
{
    _os_UART_ISR_Proc[2](2);
    MsOS_EnableInterrupt(UART2_PIU_IRQ);
}
#endif

void OS_UART_ISR_Enable(MS_U8 u8UARTID, P_UART_ISR_Proc pfISRProc)
{
    if (u8UARTID == 0)
    {
        _os_UART_ISR_Proc[0] = pfISRProc;
        MsOS_AttachInterrupt(UART0_PIU_IRQ, _OS_UART_ISR0);
        MsOS_EnableInterrupt(UART0_PIU_IRQ);
    }

#ifdef UART1_PIU_ADDR
    if (u8UARTID == 1)
    {
        _os_UART_ISR_Proc[1] = pfISRProc;
        MsOS_AttachInterrupt(UART1_PIU_IRQ, (InterruptCb)_OS_UART_ISR1);
        MsOS_EnableInterrupt(UART1_PIU_IRQ);
    }
#endif

#ifdef UART2_PIU_ADDR
    if (u8UARTID == 2)
    {
        _os_UART_ISR_Proc[2] = pfISRProc;
        MsOS_AttachInterrupt(UART2_PIU_IRQ, (InterruptCb)_OS_UART_ISR2);
        MsOS_EnableInterrupt(UART2_PIU_IRQ);
    }
#endif
}

void OS_UART_ISR_Disable(MS_U8 u8UARTID)
{
    if (u8UARTID == 0)
    {
        MsOS_DisableInterrupt(UART0_PIU_IRQ);
        MsOS_DetachInterrupt (UART0_PIU_IRQ);
        _os_UART_ISR_Proc[0] = NULL;
    }

#ifdef UART1_PIU_ADDR
    if (u8UARTID == 1)
    {
        MsOS_DisableInterrupt(UART1_PIU_IRQ);
        MsOS_DetachInterrupt (UART1_PIU_IRQ);
        _os_UART_ISR_Proc[1] = NULL;
    }
#endif

#ifdef UART2_PIU_ADDR
    if (u8UARTID == 2)
    {
        MsOS_DisableInterrupt(UART2_PIU_IRQ);
        MsOS_DetachInterrupt (UART2_PIU_IRQ);
        _os_UART_ISR_Proc[2] = NULL;
    }
#endif
}

MS_BOOL OS_UART_Init(MS_U8 u8UARTID)
{

    if (u8UARTID == 0)
    {
        MsOS_DisableInterrupt(UART0_PIU_IRQ);
        MsOS_DetachInterrupt (UART0_PIU_IRQ);
    }

#ifdef UART1_PIU_ADDR
    if (u8UARTID == 1)
    {
        MsOS_DisableInterrupt(UART1_PIU_IRQ);
        MsOS_DetachInterrupt (UART1_PIU_IRQ);
    }
#endif

#ifdef UART2_PIU_ADDR
    if (u8UARTID == 2)
    {
        MsOS_DisableInterrupt(UART2_PIU_IRQ);
        MsOS_DetachInterrupt (UART2_PIU_IRQ);
    }
#endif

    return TRUE;
}


MS_BOOL OS_UART_Exit(void)
{
    return TRUE;
}

#endif //OS_ECOS
