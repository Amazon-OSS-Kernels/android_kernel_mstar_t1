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

#ifndef _HAL_HVD_H_
#define _HAL_HVD_H_
#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// Feature
#define HAL_HVD_ENABLE_MUTEX_PROTECT    HVD_ENABLE_MUTEX_PROTECT
#define HAL_HVD_ENABLE_MIU_PROTECT      HVD_ENABLE_MIU_RST_PROTECT
#define HAL_HVD_ENABLE_VPU_CMD    1

// MBox type of FW cmd
#if (HAL_HVD_ENABLE_VPU_CMD)
    #define HAL_HVD_CMD_MBOX            E_HVD_VPU_HI_0
    #define HAL_HVD_CMD_ARG_MBOX        E_HVD_VPU_HI_1
#else
    #define HAL_HVD_CMD_MBOX            E_HVD_HI_0
    #define HAL_HVD_CMD_ARG_MBOX        E_HVD_HI_1
#endif
// MBox other usages
#define HAL_HVD_REG_ISR_VPU     E_HVD_VPU_RISC_1
#define HAL_HVD_REG_FW_STATE    E_HVD_RISC_0
#define HAL_HVD_REG_ISR_HVD     E_HVD_RISC_1
#define HAL_HVD_REG_DISP_INFO_COPYED    E_HVD_RISC_1
#define HAL_HVD_REG_DISP_INFO_CHANGE    E_HVD_RISC_1
#define HAL_HVD_REG_DISP_CTL    E_HVD_HI_0

#define PRESET_ONE_PENDING_BUFFER       (1 << 0)  /// For AVC, one pending buffer mode, reduce from two to one
#define PRESET_FRAMERATE_HANDLING       (1 << 1)  /// For AVC, Handle frame rate by input frame rate when sequence did not have frame rate info.
#define PRESET_TSP_IN_BBU_MODE          (1 << 2)  /// For AVS/AVS+ only. Raw data comes from TSP while HVD HW engine triggers decode according to BBU table

#if defined(CHIP_T3)
// patch for HW bug
#define HVD_MIU1_BASE_ADDRESS   0x08000000UL//0x10000000UL
#else // CHIP_U3
#define HVD_MIU1_BASE_ADDRESS   0x08000000UL
#endif

#define HVD_RV_BROKENBYUS_BIT    29

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_HVD_HI_0,
    E_HVD_HI_1,
    E_HVD_RISC_0,
    E_HVD_RISC_1,
    E_HVD_VPU_HI_0,
    E_HVD_VPU_HI_1,
    E_HVD_VPU_RISC_0,
    E_HVD_VPU_RISC_1,
} HVD_MBOX_TYPE;

typedef enum
{
    //Support TSP/TS/File mode
    E_HAL_HVD_MAIN_STREAM,

    //Only support file mode
    E_HAL_HVD_SUB_STREAM,

    //Only support MVC mode
    E_HAL_HVD_MVC_STREAM,
} HAL_HVD_StreamType;

typedef struct
{
    HAL_HVD_StreamId eStreamId;
    MS_BOOL bUsed;
    MS_BOOL bDispOutSide;
    MS_U32 u32PTSPreWptr;
    MS_U32 u32PTSByteCnt;
    MS_U32 u32BBUWptr;
    MS_U32 u32BBUEntryNum;
    MS_U32 u32BBUEntryNumTH;
    MS_U32 u32DispQIndex;
    MS_U32 u32PrivateData;
    MS_U32 u32FreeData;
    MS_U32 u32RegBase;
} HVD_EX_Stream;

typedef struct
{
    MS_BOOL bColocateBBUMode;

}HVD_EX_PreSet;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL     HAL_HVD_EX_InitHW(MS_U32 u32Id,VPU_EX_DecoderType DecoderType);
MS_BOOL     HAL_HVD_EX_DeinitHW(void);
void        HAL_HVD_EX_FlushMemory(void);
void        HAL_HVD_EX_ReadMemory(void);
void        HAL_HVD_EX_SetDrvCtrlsBase(HVD_EX_Drv_Ctrl *pHVDCtrlsBase);
void        HAL_HVD_EX_CheckMIUSel(MS_BOOL bChange);
MS_U32      HAL_HVD_EX_GetHWVersionID(void);
HAL_HVD_StreamId HAL_HVD_EX_GetFreeStream(HAL_HVD_StreamType eStreamType);
void        HAL_HVD_EX_PowerCtrl(MS_BOOL bEnable);
void        HAL_HVD_EX_InitRegBase(MS_U32 u32RegBase);
HVD_Return  HAL_HVD_EX_InitVariables(MS_U32 u32Id);
HVD_Return  HAL_HVD_EX_InitShareMem(MS_U32 u32Id);
HVD_Return  HAL_HVD_EX_InitRegCPU(MS_U32 u32Id);
HVD_Return  HAL_HVD_EX_SetData(MS_U32 u32Id, HVD_SetData u32type, MS_U32 u32Data);
MS_U32      HAL_HVD_EX_GetData(MS_U32 u32Id, HVD_GetData eType);
MS_S64      HAL_HVD_EX_GetData_EX(MS_U32 u32Id, HVD_GetData eType);
MS_U32      HAL_HVD_EX_GetShmAddr(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_GetFrmRateIsSupported(MS_U32 u32Id, MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate);
HVD_Return  HAL_HVD_EX_SetCmd(MS_U32 u32Id, HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg);
HVD_Return  HAL_HVD_EX_DeInit(MS_U32 u32Id);
HVD_Return  HAL_HVD_EX_PushPacket(MS_U32 u32Id, HVD_BBU_Info *pInfo);
void        HAL_HVD_EX_EnableISR(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetForceISR(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetClearISR(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_IsISROccured(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_IsEnableISR(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_IsAlive(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_RstPTSCtrlVariable(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_FlushRstShareMem(MS_U32 u32Id);
void        HAL_HVD_EX_UartSwitch2FW(MS_BOOL bEnable);
MS_U32      HAL_HVD_EX_GetData_Dbg(MS_VIRT u32Addr);
void        HAL_HVD_EX_SetData_Dbg(MS_VIRT u32Addr, MS_U32 u32Data);
MS_U16      HAL_HVD_EX_GetCorretClock(MS_U16 u16Clock);
void        HAL_HVD_EX_UpdateESWptr_Fire(MS_U32 u32Id);
void        HAL_HVD_EX_MVD_PowerCtrl(MS_BOOL bEnable);
void        HAL_HVD_EX_Dump_FW_Status(MS_U32 u32Id);
void        HAL_HVD_EX_Dump_HW_Status(MS_U32 u32Num);
void        HAL_HVD_EX_SetMiuBurstLevel(HVD_EX_Drv_Ctrl *pDrvCtrl, HVD_MIU_Burst_Cnt_Ctrl eMiuBurstCntCtrl);
#ifdef SUPPORT_X_MODEL_FEATURE //We using the common compile option to handle X model
void	    HVD_EX_SetRstFlag(MS_BOOL bRst);
#endif
MS_BOOL     HAL_HVD_EX_DispFrameAllViewed(MS_U32 u32Id);
#if HVD_ENABLE_MVC
MS_BOOL     HAL_HVD_EX_CheckMVCID(MS_U32 u32Id);
VDEC_EX_View  HAL_HVD_EX_GetView(MS_U32 u32Id);
#endif ///HVD_ENABLE_MVC
MS_BOOL HAL_HVD_EX_Init_Share_Mem(void);
void        HAL_HVD_EX_SpareBandwidth(MS_U32 u32Id);
MS_U32      HAL_HVD_EX_GetDispFrmNum(MS_U32 u32Id);
HVD_Return HAL_HVD_EX_SetHVDColBBUMode(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetHwRegBase(MS_U32 u32Id, MS_U32 u32ModeFlag);
MS_BOOL HAL_HVD_EX_GetSupport2ndMVOPInterface(void);
void        HAL_HVD_EX_SetBufferAddr(MS_U32 u32Id);
void        HAL_HVD_EX_SetPreCtrlVariables(MS_U32 u32Id,MS_U32 drvprectrl);

typedef enum
{
	E_BBU_FSM_START  = 0,
    E_BBU_FSM_0,
    E_BBU_FSM_00,
    E_BBU_FSM_001,
} VDEC_EX_BBU_FSM_STATE;

MS_BOOL _HVD_EX_IS_BBU_TSP_MODE(MS_U32 u32Id);
MS_BOOL _HVD_EX_BBU_Get_TaskRunning(MS_U32 u32Id);
void _HVD_EX_BBU_Set_TaskRunning(MS_U32 u32Id,MS_BOOL val);
MS_U32 _HVD_EX_GetESOffsetIncrease(MS_U32 u32Id, MS_U32 inc, MS_U32 offset);
MS_U32 _HVD_EX_GetESOffsetMinus(MS_U32 u32Id, MS_U32 mis, MS_U32 offset);
void _HVD_EX_ES_DBG_PRINT(MS_U32 u32Id, MS_U32 offset, MS_U32 length);
MS_U32 _HVD_EX_ES_Read(MS_U32 u32Id, MS_U32 offset);

MS_BOOL _HVD_EX_CheckWaterLevel(MS_U32 u32Id, MS_U32 u32ESRptr, MS_U32 u32ESWptr);
MS_BOOL _HVD_EX_BBU_FindStartCode(MS_U32 u32Id,MS_U32* u32Offset, MS_U32 u32ESRptr, MS_U32 u32ESWptr);

void HAL_HVD_EX_BBU_Proc(MS_U32 u32Id);
void HAL_HVD_EX_BBU_StopProc(MS_U32 u32Id);

#endif

#endif // _HAL_HVD_H_

