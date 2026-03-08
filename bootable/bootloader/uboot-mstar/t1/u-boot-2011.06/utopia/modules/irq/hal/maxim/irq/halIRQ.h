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

#ifndef _HAL_IRQ_H_
#define _HAL_IRQ_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_IRQ_Init(void);
void HAL_IRQ_Restore(void);
void HAL_IRQ_DetechAll(void);
void HAL_IRQ_Detech(MS_U32 u32Vector);
void HAL_IRQ_Attach(MS_U32 u32Vector, void *pIntCb, MS_U32 u32IntType );
void HAL_IRQ_Mask(MS_U32 u32Vector);
void HAL_IRQ_MaskAll(MS_BOOL bMask);
void HAL_IRQ_UnMask(MS_U32 u32Vector);
void HAL_IRQ_NotifyCpu(IRQ_CPU_TYPE type);
void HAL_IRQ_Set_IOMap(MS_U32 u32Base);
MS_BOOL HAL_IRQ_InISR(void);
#endif // _HAL_IRQ_H_

