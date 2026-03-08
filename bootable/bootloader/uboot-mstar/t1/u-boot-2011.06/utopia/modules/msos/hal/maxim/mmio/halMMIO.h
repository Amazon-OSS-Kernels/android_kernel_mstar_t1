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
/// file    halMMIO.h
/// @brief  memory map io (MMIO) HAL
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_MMIO_H__
#define __HAL_MMIO_H__


//--------------------------------------------------------------------------------------------------
//  Hardware Capability
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

MS_U16  HAL_MMIO_GetType(MS_U32 u32Module);
MS_BOOL HAL_MMIO_GetBase(MS_PHY* virtBaseAddr, MS_PHY* pu32BaseSize, MS_U32 u32BankType);
MS_BOOL HAL_MMIO_GetIPBase(MS_VIRT* virtBaseAddr, MS_U16 u16BankType);

//we put the flush & read memory function here due to library dependency consideration
void    HAL_MMIO_FlushMemory(void);
void    HAL_MMIO_ReadMemory(void);

#endif // __HAL_MMIO_H__


