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


#ifndef _HALGPIO_H_
#define _HALGPIO_H_

#ifdef __cplusplus
extern "C" {
#endif
//-------------------------------------------------------------------------------------------------
//  Header
//-------------------------------------------------------------------------------------------------
#include "drvGPIO.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


/// GPIO operations for GPIO extension or IIC implmentation
struct gpio_operations
{
    void (*set_high) (MS_GPIO_NUM gpio);    ///< output high
    void (*set_low)  (MS_GPIO_NUM gpio);    ///< output low
    void (*set_input)(MS_GPIO_NUM gpio);    ///< set to input
    void (*set_output)(MS_GPIO_NUM gpio);    ///< set to output
    int (*get_inout)(MS_GPIO_NUM gpio);    ///< get in or out
    int (*get_level)(MS_GPIO_NUM gpio);    ///< get level
};

#define END_GPIO_NUM    208

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void HAL_GPIO_SetIOMapBase(MS_VIRT u32Base);
void HAL_GPIO_SetIOMapBase_PM(MS_VIRT u32Base);

void HAL_GPIO_Interrupt_Init(void);
void HAL_GPIO_Interrupt_Action(void);
GPIO_Result HAL_GPIO_Attach_Interrupt(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback);
GPIO_Result HAL_GPIO_Datach_Interrupt(MS_GPIO_NUM gpio_num);
GPIO_Result HAL_GPIO_Enable_Interrupt(MS_GPIO_NUM gpio);
GPIO_Result HAL_GPIO_Disable_Interrupt(MS_GPIO_NUM gpio);
void HAL_GPIO_Enable_Interrupt_all(void);
void HAL_GPIO_Disable_Interrupt_all(void);

#ifdef __cplusplus
}
#endif


#endif      //_HALGPIO_H_

