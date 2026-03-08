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
///
/// @file   drvCMDQ_priv.h
/// @brief  CMDQ Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef _DRVCMDQ_PRIV_H_
#define _DRVCMDQ_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

// driver version
#define CMDQ_VERSION            0x00000001

#include "MsTypes.h"
enum {
    MDrv_CMD_CMDQ_GetLibVer,
    MDrv_CMD_CMDQ_Printf_Crash_Command,
    MDrv_CMD_CMDQ_Receive,
    MDrv_CMD_CMDQ_Set_timer_ratio,
    MDrv_CMD_CMDQ_SetDbgLevel,
} eCMDQIoctlOpt;

//ioctl function pointer type
typedef DRVCMDQ_RESULT (*IOCTL_CMDQ_GETLIBVER)(const MSIF_Version **);
typedef DRVCMDQ_RESULT (*IOCTL_CMDQ_PRINTFCRASHCOMMAND)(void);
typedef DRVCMDQ_RESULT (*IOCTL_CMDQ_RECEIVE)(CH_Struct_Pointer);
typedef DRVCMDQ_RESULT (*IOCTL_CMDQ_SETTIMERRATIO)(MS_U32, MS_U32);
typedef DRVCMDQ_RESULT (*IOCTL_CMDQ_SETDBGLEVEL)(DrvCMDQ_DbgLevel);

// INSTANCE Private(ioctl function pointer)
typedef struct _CMDQ_INSTANT_PRIVATE
{
    IOCTL_CMDQ_GETLIBVER            fpCMDQGetLibVer;
    IOCTL_CMDQ_PRINTFCRASHCOMMAND   fpCMDQPrintf_Crash_Command;
    IOCTL_CMDQ_RECEIVE              fpCMDQReceive;
    IOCTL_CMDQ_SETTIMERRATIO        fpCMDQSet_Timer_Ratio;
    IOCTL_CMDQ_SETDBGLEVEL          fpCMDQSetDbgLevel;
}CMDQ_INSTANT_PRIVATE;

//////////////////////////////////////
// function parameter structure //////
//////////////////////////////////////
// for cmd MDrv_CMDQ_GetLibVer
typedef struct _CMDQ_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
}CMDQ_GETLIBVER_PARAM, *PCMDQ_GETLIBVER_PARAM;

// for cmd MDrv_CMDQ_Receive
typedef struct _CMDQ_RECEIVE_PARAM
{
    CH_Struct_Pointer New_IP_Head;
}CMDQ_RECEIVE_PARAM, *PCMDQ_RECEIVE_PARAM;

// for cmd MDrv_CMDQ_Set_timer_ratio
typedef struct _CMDQ_SETTIMERRATIO_PARAM
{
    MS_U32 time;
    MS_U32 ratio;
}CMDQ_SETTIMERRATIO_PARAM, *PCMDQ_SETTIMERRATIO_PARAM;

// for cmd MDrv_CMDQ_SetDbgLevel
typedef struct _CMDQ_SETDBGLEVEL_PARAM
{
    DrvCMDQ_DbgLevel DbgLevel;
}CMDQ_SETDBGLEVEL_PARAM, *PCMDQ_SETDBGLEVEL_PARAM;

//////////////////////////////////////
// function for utopia2.0 ////////////
//////////////////////////////////////
void CMDQRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 CMDQOpen(void** pInstance, void* pAttribute);
MS_U32 CMDQClose(void* pInstance);
MS_U32 CMDQIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);


// Resource_Private
typedef struct _CMDQ_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}CMDQ_RESOURCE_PRIVATE;

DRVCMDQ_RESULT MDrv_CMDQv2_Init(int hnd, MS_U32 miu);
DRVCMDQ_RESULT MDrv_CMDQv2_Start(int hnd, MS_BOOL bStart);
DRVCMDQ_RESULT MDrv_CMDQv2_Get_Memory_Size(int hnd, MS_PHY smalladdr, MS_PHY bigaddr, MS_U32 miu);
DRVCMDQ_RESULT MDrv_CMDQv2_Set_Buffer(int hnd, MS_PHY startaddr, MS_PHY endaddr);
DRVCMDQ_RESULT MDrv_CMDQv2_Reset(int hnd);
DRVCMDQ_RESULT MDrv_CMDQv2_Exit(int hnd);
DRVCMDQ_RESULT MDrv_CMDQv2_Stop(int hnd);
DRVCMDQ_RESULT MDrv_CMDQv2_Receive(int hnd, CH_Struct_Pointer iphead);
void           MDrv_CMDQv2_Insert_Redundant_Null(int hnd, MS_U32 n);
void           MDrv_CMDQv2_Transfer(int hnd, CAF_Struct_Pointer cmdarray, MS_SIZE size);
void           MDrv_CMDQv2_Set_dummy_Address(int hnd, MS_U32 value);
void           MDrv_CMDQv2_Set_MIU_SELECT(int hnd, MS_U32 miu);
DRVCMDQ_RESULT MDrv_CMDQv2_Set_timer_ratio(int hnd, MS_U32 time, MS_U32 ratio);
DRVCMDQ_RESULT MDrv_CMDQv2_Printf_Crash_Command(int hnd);
void           MDrv_CMDQv2_Gen_WaitTrigger_Bus_Command(int hnd, CAF_Struct_Pointer cmd, EN_CMDQTriggerBus_ID id, MS_BOOL falledge);
MS_U32         MDrv_CMDQv2_GetWritePoint(int hnd);
MS_U32         MDrv_CMDQv2_SetPowerState(int hnd, EN_POWER_MODE u16PowerState);
MS_U32         MDrv_CMDQv2_Get_FreeCmdLine_Size(int hnd);

void           MDrv_CMDQv2_Dbg_WriteDummy(int hnd, MS_U32 value);
MS_U32         MDrv_CMDQv2_Dbg_ReadDummy(int hnd);
void           MDrv_CMDQv2_Dbg_Pause(int hnd);
void           MDrv_CMDQv2_Dbg_Continue(int hnd);

void           MDrv_CMDQv2_DumpContext(void);


#ifdef __cplusplus
}
#endif

#endif // _DRVCMDQ_PRIV_H_
