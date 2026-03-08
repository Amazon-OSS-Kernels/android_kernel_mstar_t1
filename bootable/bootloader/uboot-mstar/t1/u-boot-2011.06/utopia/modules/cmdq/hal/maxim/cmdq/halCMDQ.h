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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halCMDQ.h
// @brief  CMDQ HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_CMDQ_H__
#define __HAL_CMDQ_H__

#include "regCMDQ.h"

// Trigger Bus Defination
#define HAL_GE_GE2CMDQ_TRI                  0x0000
#define HAL_BDMA_INT_BDMA_0                 0x0001
#define HAL_BDMA_INT_BDMA_1                 0x0002
#define HAL_GPU_GPU2CMDQ_TRIG               0x0003
#define HAL_GOPG0_GOP_CMDQ_INT_0            0x0004
#define HAL_GOPG1_GOP_CMDQ_INT_1            0x0005
#define HAL_GOPG2_GOP_CMDQ_INT_2            0x0006
#define HAL_GOPG3_GOP_CMDQ_INT_3            0x0007
#define HAL_SCALAR_SC2CMDQ_TRIG_0           0x0008
#define HAL_SCALAR_SC2CMDQ_TRIG_1           0x0009
#define HAL_SCALAR_SC2CMDQ_TRIG_2           0x000A
#define HAL_SCALAR_SC2CMDQ_TRIG_3           0x000B
#define HAL_AESDMA_CMDQDMA_FINISHED         0x000C
#define HAL_VOP_DCM2GOP_VDE                 0x000D
#define HAL_VOP_DCS2GOP_VDE                 0x000E


#define BUS_SHIFT 4

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))
////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_CMDQ_SetBank(MS_U32 hnd, MS_VIRT u32BankAddr);
void HAL_CMDQ_Enable(MS_U32 hnd);
void HAL_CMDQ_Stop(MS_U32 hnd);
void HAL_CMDQ_Reset(MS_U32 hnd);
void HAL_CMDQ_Set_Miu_Length(MS_U32 hnd, MS_U32 value);
void HAL_CMDQ_Set_Miu_Request(MS_U32 hnd, MS_U32 value);
MS_BOOL HAL_CMDQ_Set_Mode(MS_U32 hnd, MS_U32 ModeSel);
void HAL_CMDQ_Set_Start_Pointer(MS_U32 hnd, MS_PHY StartAddr);
void HAL_CMDQ_Set_End_Pointer(MS_U32 hnd, MS_PHY EndAddr);
void HAL_CMDQ_Set_Offset_Pointer(MS_U32 hnd, MS_U32 OffsetAddr);
void HAL_CMDQ_Set_Timer(MS_U32 hnd, MS_U32 time);
void HAL_CMDQ_Set_Ratio(MS_U32 hnd, MS_U32 ratio);
void HAL_CMDQ_Reset_Soft_Interrupt(MS_U32 hnd);
void HAL_CMDQ_Reset_Start_Pointer_bit(MS_U32 hnd);
void HAL_CMDQ_Read_Soft_Interrupt(MS_U32 hnd);
void HAL_CMDQ_Read_Dec_Done(MS_U32 hnd);
void HAL_CMDQ_Start(MS_U32 hnd);
MS_U32 HAL_CMDQ_Read_Dummy_Register(MS_U32 hnd);
void HAL_CMDQ_Write_Dummy_Register(MS_U32 hnd, MS_U32 DummyValue);
void HAL_CMDQ_Set_MIU_SELECT(MS_U32 hnd, MS_U32 miu_select);
MS_U32 HAL_CMDQ_Read_Start_Pointer(MS_U32 hnd);
MS_U32 HAL_CMDQ_Read_End_Pointer(MS_U32 hnd);
void HAL_CMDQ_Write_Dummy_Register_release_polling(MS_U32 hnd, MS_U32 Write_bit);
MS_U32 HAL_CMDQ_Error_Command(MS_U32 hnd, MS_U32 select_bit);
void HAL_CMDQ_Write_Pointer(MS_U32 hnd, MS_PHY Write_value);
MS_U32 HAL_CMDQ_Read_Pointer(MS_U32 hnd);
MS_U32 HAL_CMDQ_Get_Write_Pointer(MS_U32 hnd);


void   HAL_CMDQ_Trigger_Wait(MS_U32 hnd, MS_U32 sigbits);
void   HAL_CMDQ_Skip_Commands(MS_U32 hnd, MS_U32 skipbits);
MS_U32 HAL_CMDQ_Read_Misc_Status(MS_U32 hnd);
void   HAL_CMDQ_Set_Debug_Step_Mode(MS_U32 hnd, MS_BOOL on);
MS_U32 HAL_CMDQ_Get_Debug_Step_Mode(MS_U32 hnd);
void   HAL_CMDQ_Debug_One_Step(MS_U32 hnd);
void   HAL_CMDQ_Reset2(MS_U32 hnd, MS_BOOL high);



#endif // #ifndef _CMDQ_REG_MCU_H_
