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

#ifndef _HAL_HVD_SUB_H_
#define _HAL_HVD_SUB_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// Feature
/////#define HAL_HVD_SUB_ENABLE_VPU_LIB    1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_HVD_Sub_EnableISR(MS_BOOL bEnable);
void HAL_HVD_Sub_CheckMIUSel( MS_BOOL bChange);
MS_U32 HAL_HVD_Sub_Get_HWVersionID(void);
void HAL_HVD_Sub_PowerCtrl(MS_BOOL bEnable);
void HAL_HVD_Sub_InitRegBase(MS_U32 u32RegBase);
HVD_Return HAL_HVD_Sub_InitVariables( MS_U32 drvctrl  );
HVD_Return HAL_HVD_Sub_InitShareMem( void);
HVD_Return HAL_HVD_Sub_InitRegCPU( void);
HVD_Return HAL_HVD_Sub_SetData( HVD_SetData u32type , MS_U32 u32Data);
MS_U32 HAL_HVD_Sub_GetData( HVD_GetData eType );
HVD_Return HAL_HVD_Sub_SetCmd( HVD_User_Cmd u32Cmd , MS_U32 u32CmdArg);
HVD_Return HAL_HVD_Sub_DeInit(void);
HVD_Return HAL_HVD_Sub_PushPacket(HVD_BBU_Info* pInfo  );
void HAL_HVD_Sub_Enable_ISR(MS_BOOL bEnable);
void HAL_HVD_Sub_SetForceISR(MS_BOOL bEnable);
void HAL_HVD_Sub_SetClearISR(void);
MS_BOOL HAL_HVD_Sub_IsISROccured(void);
MS_BOOL HAL_HVD_Sub_IsEnableISR(void);
MS_BOOL HAL_HVD_Sub_RstPTSCtrlVariable(void);
MS_BOOL HAL_HVD_Sub_FlushRstShareMem(void);
MS_BOOL HAL_Sub_IsBBUEntryOdd(void);
void HAL_HVD_Sub_UartSwitch2FW( MS_BOOL bEnable );

MS_U32 HAL_HVD_Sub_GetData_Dbg( MS_U32 u32Addr );
void HAL_HVD_Sub_SetData_Dbg( MS_U32 u32Addr , MS_U32 u32Data);
MS_U16 HAL_HVD_Sub_GetCorretClock(MS_U16 u16Clock);
void HAL_HVD_Sub_UpdateESWptr_Fire(void);
void HAL_HVD_Sub_MVD_PowerCtrl(MS_BOOL bEnable);
void HAL_HVD_Sub_Dump_FW_Status(void);
void HAL_HVD_Sub_Dump_HW_Status(MS_U32 numb);

//???
void HAL_HVD_Sub_GetBBUEntry( MS_U32 u32Idx , MS_U32* u32NalOffset , MS_U32* u32NalSize  );
void HAL_HVD_Sub_Dump_BBUs(  MS_U32 u32StartIdx, MS_U32 u32EndIdx, MS_BOOL bShowEmptyEntry );


#endif // _HAL_HVD_SUB_H_

