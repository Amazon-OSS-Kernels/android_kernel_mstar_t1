/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_gpio.h
/// @brief  GPIO Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_

#include <asm/types.h>
#include "mdrv_types.h"
#include "mdrv_gpio_io.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_Init(void);
void MDrv_GPIO_WriteRegBit(U32 u32Reg, U8 u8Enable, U8 u8BitMsk);
void MDrv_GPIO_Pad_Set(U8 u8IndexGPIO);
void MDrv_GPIO_Pad_Oen(U8 u8IndexGPIO);
void MDrv_GPIO_Pad_Odn(U8 u8IndexGPIO);
U8 MDrv_GPIO_Pad_Read(U8 u8IndexGPIO);
U8 MDrv_GPIO_Pad_InOut(U8 u8IndexGPIO);
void MDrv_GPIO_Pull_High(U8 u8IndexGPIO);
void MDrv_GPIO_Pull_Low(U8 u8IndexGPIO);
void MDrv_GPIO_Set_High(U8 u8IndexGPIO);
void MDrv_GPIO_Set_Low(U8 u8IndexGPIO);
void MDrv_GPIO_Set_Input(U8 u8IndexGPIO);

#endif // _DRV_GPIO_H_

