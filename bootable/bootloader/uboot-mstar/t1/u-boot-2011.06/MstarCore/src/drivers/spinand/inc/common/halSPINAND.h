/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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

#ifndef _HAL_SPINAND_H_
#define _HAL_SPINAND_H_

//-------------------------------------------------------------------------------------------------
//  Macro definition
//-------------------------------------------------------------------------------------------------
#define DEBUG_SPINAND(debug_level, x)     do { if (_u8SPINANDDbgLevel >= (debug_level)) (x); } while(0)

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

MS_U32 HAL_SPINAND_Read (MS_U32 u32Addr, MS_U32 u32DataSize, MS_U8 *pu8Data);
MS_U32 HAL_SPINAND_Write( MS_U32 u32_PageIdx, MS_U8 *u8Data, MS_U8 *pu8_SpareBuf);
MS_U8  HAL_SPINAND_ReadID(MS_U32 u32DataSize, MS_U8 *pu8Data);
MS_U32 HAL_SPINAND_RFC(MS_U32 u32Addr, MS_U8 *pu8Data);
void HAL_SPINAND_Config(MS_U32 u32PMRegBaseAddr, MS_U32 u32NonPMRegBaseAddr);
MS_U32 HAL_SPINAND_BLOCKERASE(MS_U32 u32_PageIdx);
U32 HAL_SPINAND_Init(void);
MS_U32 HAL_SPINAND_WriteProtect(MS_BOOL bEnable);
MS_BOOL HAL_SPINAND_PLANE_HANDLER(MS_U32 u32Addr);
MS_U32 HAL_SPINAND_SetMode(SPINAND_MODE eMode);
void HAL_SPINAND_CSCONFIG(void);

extern MS_U8 _u8SPINANDDbgLevel;


#endif // _HAL_SPINAND_H_
