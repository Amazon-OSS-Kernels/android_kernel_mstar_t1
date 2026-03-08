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

#ifndef _HAL_URDMA_H_
#define _HAL_URDMA_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/* FAST UART Base */
//#define HS_UART_DMA_REG             0x5E00
#define FAST_UART_DMA_REG           0x6980UL  //(0x1A60) * 4
#define TWO_BYTE_SPACE 				1

/* Init parameter */
#define CONF_RD_MIU32_SWAP          0 /* No Read MIU32 SWAP */
#define CONF_WD_MIU32_SWAP          0 /* No Write MIU32 SWAP */
#define CONF_RX_BYTE_ORDER          0 /* B7,B6,B5,B4,B3,B2,B1,B0 */
#define CONF_TX_BYTE_ORDER          0 /* B7,B6,B5,B4,B3,B2,B1,B0 */

#define FAST_UART_INT               E_INT_IRQ_UART2MCU
#define FAST_UART_DMA_INT           E_INT_IRQ_URDMA2MCU

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------


#endif // _HAL_URDMA_H_

