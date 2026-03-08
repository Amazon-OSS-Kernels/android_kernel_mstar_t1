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

typedef struct
{
    MS_U32 u32WritePtr;
    MS_U32 u32OffsetCnt;
} HVD_PTS_Table_Ctrl;

typedef struct
{
    MS_U32 u32PTS;
    MS_U32 u32OffsetCnt;
    MS_U32 u32ID_L;
    MS_U32 u32ID_H;
} HVD_PTS_Table_Entry;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void        HAL_HVD_FlushMemory(void);
void        HAL_HVD_ReadMemory(void);
void        HAL_HVD_CheckMIUSel(MS_BOOL bChange);
MS_U32      HAL_HVD_Get_HWVersionID(void);
void        HAL_HVD_PowerCtrl(MS_BOOL bEnable);
void        HAL_HVD_InitRegBase(MS_U32 u32RegBase);
void        HAL_HVD_SetPreCtrlVariables(MS_U32 drvprectrl);
HVD_Return  HAL_HVD_InitVariables(MS_U32 drvctrl);
HVD_Return  HAL_HVD_InitShareMem(void);
HVD_Return  HAL_HVD_InitRegCPU(void);
HVD_Return  HAL_HVD_SetData(HVD_SetData u32type, MS_U32 u32Data);
MS_U32      HAL_HVD_GetData(HVD_GetData eType);

MS_S64      HAL_HVD_GetData_EX(HVD_GetData eType);
HVD_Return  HAL_HVD_SetCmd(HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg);
HVD_Return  HAL_HVD_DeInit(void);
HVD_Return  HAL_HVD_PushPacket(HVD_BBU_Info *pInfo);
void        HAL_HVD_EnableISR(MS_BOOL bEnable);
void        HAL_HVD_SetForceISR(MS_BOOL bEnable);
void        HAL_HVD_SetClearISR(void);
MS_BOOL     HAL_HVD_IsISROccured(void);
MS_BOOL     HAL_HVD_IsEnableISR(void);
MS_BOOL     HAL_HVD_IsAlive(void);
MS_BOOL     HAL_HVD_RstPTSCtrlVariable(void);
MS_BOOL     HAL_HVD_FlushRstShareMem(void);
void        HAL_HVD_UartSwitch2FW(MS_BOOL bEnable);
MS_U32      HAL_HVD_GetData_Dbg(MS_U32 u32Addr);
void        HAL_HVD_SetData_Dbg(MS_U32 u32Addr, MS_U32 u32Data);
MS_U16      HAL_HVD_GetCorretClock(MS_U16 u16Clock);
void        HAL_HVD_UpdateESWptr_Fire(void);
void        HAL_HVD_MVD_PowerCtrl(MS_BOOL bEnable);
void        HAL_HVD_Dump_FW_Status(void);
void        HAL_HVD_Dump_HW_Status(MS_U32 numb);
void        HAL_HVD_SetMiuBurstLevel(HVD_MIU_Burst_Cnt_Ctrl eMiuBurstCntCtrl);
MS_BOOL     HAL_HVD_GetFrmRateIsSupported(MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate);

#endif // _HAL_HVD_H_

