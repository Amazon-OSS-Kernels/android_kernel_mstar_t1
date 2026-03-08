/**
* Copyright (c) 2006 ¡V 2017 MStar Semiconductor, Inc.
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

#ifndef _MHAL_SC_H_
#define _MHAL_SC_H_

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define SC_DEV_NUM                  1               // number of device
#define SC_IRQ                      E_IRQ_SMART//E_INT_IRQ_UART1 //E_IRQ_UART1
#define SC_IRQ2                     E_IRQ_UART2

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SC_READ(id, addr)           (!(id) ? UART1_READ(addr) : UART2_READ(addr))
#define SC_WRITE(id, addr, val)     if (!(id)) { UART1_WRITE(addr, val);} else { UART2_WRITE(addr, val);}
#define SC_OR(id,addr, val)         if (!(id)) { UART1_OR(addr, val);   } else { UART2_OR(addr, val);   }
#define SC_AND(id,addr, val)        if (!(id)) { UART1_AND(addr, val);  } else { UART2_AND(addr, val);  }
#define SC_XOR(id,addr, val)        if (!(id)) { UART1_XOR(addr, val);  } else { UART2_XOR(addr, val);  }

#define SC_3M_UARTDIV(d)            (((d*9/5)>>1)+((d*9/5)&0x1UL))        // 3M/d=43.2M/(16*uart_div) => div=d*3/5
#define SC_4P5M_UARTDIV(d)          (((d*6/5)>>1)+((d*6/5)&0x1UL))        // 4.5M/d=43.2M/(16*uart_div) => div=d*3/5
#define SC_6M_UARTDIV(d)            (((d*9/10)>>1)+((d*9/10)&0x1UL))      // 6M/d=43.2M/(16*uart_div) => div=d*9/20


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#endif // _MHAL_TEMP_H_
