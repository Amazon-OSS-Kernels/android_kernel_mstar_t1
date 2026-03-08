/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
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
#define HAL_CHIP_SUPPORT_EVD

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

#if defined(CHIP_T3)
// patch for HW bug
#define HVD_MIU1_BASE_ADDRESS   0x08000000UL//0x10000000UL
#else // CHIP_U3
#define HVD_MIU1_BASE_ADDRESS   0x08000000UL
#endif

#define HVD_RV_BROKENBYUS_BIT    29

#ifdef VDEC3
#define HAL_HVD_INVALID_BBU_ID 0xFFFFFFFF
#endif

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

#ifdef VDEC3
    E_HAL_HVD_N_STREAM,
#endif
} HAL_HVD_StreamType;

#ifdef VDEC3
typedef enum
{
    E_HAL_HVD_AVC,
    E_HAL_HVD_AVS,
    E_HAL_HVD_RM,
    E_HAL_HVD_MVC,
    E_HAL_HVD_VP8,
    E_HAL_HVD_MJPEG,
    E_HAL_HVD_VP6,
    E_HAL_HVD_HEVC,
    E_HAL_HVD_VP9,
    E_HAL_HVD_HEVC_DV,
    E_HAL_HVD_NONE,
} HAL_HVD_CodecType;

typedef enum
{
    E_HAL_HVD_STATE_STOP,
    E_HAL_HVD_STATE_RUNNING,
    E_HAL_HVD_STATE_PAUSING,
    E_HAL_HVD_STATE_PAUSE_DONE
} HAL_HVD_Task_State;

#endif

typedef struct
{
    HAL_HVD_StreamId eStreamId;
    MS_BOOL bUsed;
    MS_BOOL bDispOutSide;
    MS_PHY u32PTSPreWptr;
    MS_U32 u32PTSByteCnt;
    MS_U32 u32BBUWptr;
    MS_U32 u32BBUEntryNum;
    MS_U32 u32BBUEntryNumTH;
    MS_U32 u32DispQIndex;
    MS_U32 u32PrivateData;
    MS_U32 u32FreeData;
    MS_U32 u32RegBase;
    MS_BOOL bfirstGetFrmInfoDone;
#ifdef VDEC3
    HAL_HVD_CodecType u32CodecType;
    HAL_HVD_Task_State ePpTaskState;
    MS_S32 s32HvdPpTaskId;
#endif
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
void        HAL_HVD_EX_InitRegBase(MS_VIRT u32RegBase);
HVD_Return  HAL_HVD_EX_InitVariables(MS_U32 u32Id);
#ifdef VDEC3
MS_U32      HAL_HVD_EX_GetBBUId(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_FreeBBUId(MS_U32 u32Id, MS_U32 u32BBUId);
HVD_Return  HAL_HVD_EX_InitShareMem(MS_U32 u32Id, MS_BOOL bFWdecideFB, MS_BOOL bCMAUsed);
HVD_Return  HAL_HVD_EX_InitRegCPU(MS_U32 u32Id, MS_BOOL bFWdecideFB);
#else
HVD_Return  HAL_HVD_EX_InitShareMem(MS_U32 u32Id);
HVD_Return  HAL_HVD_EX_InitRegCPU(MS_U32 u32Id);
#endif
HVD_Return  HAL_HVD_EX_SetData(MS_U32 u32Id, HVD_SetData u32type, MS_VIRT u32Data);
MS_VIRT     HAL_HVD_EX_GetData(MS_U32 u32Id, HVD_GetData eType);
MS_S64      HAL_HVD_EX_GetData_EX(MS_U32 u32Id, HVD_GetData eType);
MS_VIRT     HAL_HVD_EX_GetShmAddr(MS_U32 u32Id);
MS_VIRT     HAL_HVD_EX_GetDispQExtShmAddr(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_GetFrmRateIsSupported(MS_U32 u32Id, MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate);
HVD_Return  HAL_HVD_EX_SetCmd(MS_U32 u32Id, HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg);
HVD_Return  HAL_HVD_EX_DeInit(MS_U32 u32Id);
HVD_Return  HAL_HVD_EX_PushPacket(MS_U32 u32Id, HVD_BBU_Info *pInfo);
void        HAL_HVD_EX_EnableISR(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetForceISR(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetClearISR(HWDEC_ISR_TYPE eISRType);
MS_BOOL     HAL_HVD_EX_IsISROccured(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_IsEnableISR(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_IsAlive(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_RstPTSCtrlVariable(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_FlushRstShareMem(MS_U32 u32Id);
void        HAL_HVD_EX_UartSwitch2FW(MS_BOOL bEnable);
MS_U32      HAL_HVD_EX_GetData_Dbg(MS_U32 u32Addr);
void        HAL_HVD_EX_SetData_Dbg(MS_U32 u32Addr, MS_U32 u32Data);
MS_U16      HAL_HVD_EX_GetCorretClock(MS_U16 u16Clock);
void        HAL_HVD_EX_UpdateESWptr_Fire(MS_U32 u32Id);
void        HAL_HVD_EX_MVD_PowerCtrl(MS_BOOL bEnable);
void        HAL_HVD_EX_Dump_FW_Status(MS_U32 u32Id);
void        HAL_HVD_EX_Dump_HW_Status(MS_U32 u32Num);
void        HAL_HVD_EX_SetMiuBurstLevel(HVD_EX_Drv_Ctrl *pDrvCtrl, HVD_MIU_Burst_Cnt_Ctrl eMiuBurstCntCtrl);
void	    HVD_EX_SetRstFlag(MS_BOOL bRst);
MS_BOOL     HAL_HVD_EX_DispFrameAllViewed(MS_U32 u32Id);
MS_U8        _HVD_EX_GetStreamIdx(MS_U32 u32Id);

#if HVD_ENABLE_MVC
MS_BOOL     HAL_HVD_EX_CheckMVCID(MS_U32 u32Id);
VDEC_EX_View  HAL_HVD_EX_GetView(MS_U32 u32Id);
#endif ///HVD_ENABLE_MVC
MS_BOOL HAL_HVD_EX_Init_Share_Mem(void);
void        HAL_HVD_EX_SpareBandwidth(MS_U32 u32Id);
void        HAL_HVD_EX_PowerSaving(MS_U32 u32Id);
MS_U32      HAL_HVD_EX_GetDispFrmNum(MS_U32 u32Id);
HVD_Return HAL_HVD_EX_SetHVDColBBUMode(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetHwRegBase(MS_U32 u32Id, MS_U32 u32ModeFlag);
#if SUPPORT_EVD
void        HAL_EVD_EX_ClearTSPInput(MS_U32 u32Id);
#endif
MS_BOOL HAL_HVD_EX_GetSupport2ndMVOPInterface(void);
void        HAL_HVD_EX_SetBufferAddr(MS_U32 u32Id);
void        HAL_HVD_EX_SetPreCtrlVariables(MS_U32 u32Id,MS_VIRT drvprectrl);
MS_BOOL     HAL_HVD_EX_Is_RM_Supported(MS_U32 u32Id);
void HAL_HVD_MVDMiuClientSel(MS_U8 u8MiuSel);
typedef enum
{
	E_BBU_FSM_START  = 0,
    E_BBU_FSM_0,
    E_BBU_FSM_00,
    E_BBU_FSM_001,
} VDEC_EX_BBU_FSM_STATE;


void HAL_HVD_EX_BBU_Proc(MS_U32 u32Id);
void HAL_HVD_EX_BBU_StopProc(MS_U32 u32Id);

MS_U32 HAL_HVD_EX_GetDVSupportProfiles(void);
MS_U32 HAL_HVD_EX_GetDVSupportHighestLevel(MS_U32 pDV_Stream_Profile);
#endif
#endif // _HAL_HVD_H_

