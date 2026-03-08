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


#ifndef _HAL_SC_H_
#define _HAL_SC_H_


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define SC_DEV_NUM                  1UL              // number of device Johnson
#define SC_IRQ                      E_INT_IRQ_SMART//E_IRQ_UART1
#define SC_IRQ2                     E_INT_IRQ_UART2//E_IRQ_UART2

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SC_READ(id,addr)               ((!id)?UART1_READ(addr):UART2_READ(addr))
#define SC_WRITE(id,addr, val)         if((!id)){UART1_WRITE(addr, val);} else {UART2_WRITE(addr, val);}
#define SC_OR(id,addr, val)            if((!id)){UART1_OR(addr, val);}  else {UART2_OR(addr, val);}
#define SC_AND(id,addr, val)           if((!id)){UART1_AND(addr, val);} else {UART2_AND(addr, val);}
#define SC_XOR(id,addr, val)           if((!id)){UART1_XOR(addr, val);} else {UART2_XOR(addr, val);}

#define SC_3M_UARTDIV(d)            (((d*9UL/5UL)>>1)+((d*9UL/5UL)&0x1UL))        // 3M/d=43.2M/(16*uart_div) => div=d*3/5
#define SC_4P5M_UARTDIV(d)          (((d*6UL/5UL)>>1)+((d*6UL/5UL)&0x1UL))        // 4.5M/d=43.2M/(16*uart_div) => div=d*3/5
#define SC_6M_UARTDIV(d)            (((d*9UL/10UL)>>1)+((d*9UL/10UL)&0x1UL))      // 6M/d=43.2M/(16*uart_div) => div=d*9/20


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
extern MS_VIRT   _regSCBase[2]; // for DRV register access

void    HAL_SC_SetClk(MS_U8 u8SCID,MS_U16 u16clk);
void    HAL_SC_Init(MS_U8 u8SCID);
void    HAL_SC_Exit(MS_U8 u8SCID);
void    HAL_SC_RegMap(MS_VIRT u32RegBase);
void    HAL_SC_HW_RegMap(MS_VIRT u32RegBase);
void    HAL_SC_PowerCtrl(MS_U8 u8SCID, MS_BOOL bEnable);
MS_BOOL HAL_SC_CardVoltage_Config(MS_U8 u8SCID, SC_VoltageCtrl eVoltage);
void    HAL_SC_CardInvert(MS_U8 u8SCID, SC_CardDetType eTpye);

#endif // _HAL_SC_H_
