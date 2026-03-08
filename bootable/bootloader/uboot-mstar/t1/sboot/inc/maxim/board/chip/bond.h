/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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
#ifndef _CHIP_BOND_H_
#define _CHIP_BOND_H_

#include "Board.h"

//----------------------------------------------------------------------------
// UART_SRC_SEL
//----------------------------------------------------------------------------
#ifndef UART0_SRC_SEL
    #define UART0_SRC_SEL   (UART_SEL_PIU_UART0)
#endif

#ifndef UART1_SRC_SEL
    #define UART1_SRC_SEL   (UART_SEL_VD_MHEG5)
#endif

#ifndef UART2_SRC_SEL
    #define UART2_SRC_SEL   (UART_SEL_TSP)
#endif

#ifndef UART3_SRC_SEL
    #define UART3_SRC_SEL   (UART_SEL_MHEG5)
#endif

#ifndef UART4_SRC_SEL
    #define UART4_SRC_SEL   (UART_SEL_PIU_UART1)
#endif

#ifndef UART5_SRC_SEL
    #define UART5_SRC_SEL   (UART_SEL_PIU_FAST_UART)
#endif

#endif /* _CHIP_BOND_H_ */
