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

#ifndef _HAL_PCMCIA_H_
#define _HAL_PCMCIA_H_

#include "regPCMCIA.h"

#ifdef __cplusplus
extern "C"
{
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
extern MS_VIRT u32PCMCIA_RIU_BaseAdd;

#define RIU_READ_BYTE(Addr)         ( READ_BYTE( u32PCMCIA_RIU_BaseAdd + (Addr) ) )
#define RIU_READ_2BYTE(Addr)        ( READ_WORD( u32PCMCIA_RIU_BaseAdd + (Addr) ) )
#define RIU_WRITE_BYTE(Addr, Val)   { WRITE_BYTE( u32PCMCIA_RIU_BaseAdd + (Addr), Val) }
#define RIU_WRITE_2BYTE(Addr, Val)  { WRITE_WORD( u32PCMCIA_RIU_BaseAdd + (Addr), Val) }

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct {
   MS_BOOL bCardAInsert;
   MS_BOOL bCardARemove;
   MS_BOOL bCardAData;   
   MS_BOOL bCardBInsert;
   MS_BOOL bCardBRemove;
   MS_BOOL bCardBData;   
} ISR_STS;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_PCMCIA_Set_RIU_base(MS_VIRT ptrRIU_base);
MS_U8 HAL_PCMCIA_Read_Byte(MS_VIRT ptrAddr);
void HAL_PCMCIA_Write_Byte(MS_VIRT ptrAddr, MS_U8 u8Val);

MS_BOOL HAL_PCMCIA_GetIntStatus(ISR_STS*);
void   HAL_PCMCIA_ClrInt(MS_U32);
void   HAL_PCMCIA_MaskInt(MS_U32, MS_BOOL);
void   HAL_PCMCIA_ClkCtrl(MS_BOOL);
#ifdef __cplusplus
extern "C"
}
#endif

#endif // _HAL_PCMCIA_H_

