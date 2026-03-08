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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSP.h
/// @brief  Legacy Transport Stream Processer (TSP) Driver Interface
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_TSP_LEGACY_H__
#define __DRV_TSP_LEGACY_H__


#include "drvTSP2.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


#define DRVTSP_PID_NULL                 TSP_PID_NULL

#define DRVTSP_FILTER_DEPTH             TSP_FILTER_DEPTH                                                              // TSP_FILTER_DEPTH

#define DRVTSP_ALIGN_UNIT               TSP_ALIGN_UNIT

#define DRVTSP_ALIGN(_x)                TSP_ALIGN(_x)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/*typedef enum
{
    DRVTSP_FAIL                     =   E_TSP_FAIL,
    DRVTSP_OK                       =   E_TSP_OK,
    DRVTSP_INVALID_PARAM            =   E_TSP_FAIL_PARAMETER,
    DRVTSP_FUNC_ERROR               =   E_TSP_FAIL_FUNCTION,
    DRVTSP_NOT_SUPPORTED            =   E_TSP_FAIL_NOT_SUPPORTED,
    DRVTSP_INVALID_SECFLT,

} DrvTSP_Result;*/

#define DRVTSP_FAIL                     E_TSP_FAIL
#define DRVTSP_OK                       E_TSP_OK
#define DRVTSP_INVALID_PARAM            E_TSP_FAIL_PARAMETER
#define DRVTSP_FUNC_ERROR               E_TSP_FAIL_FUNCTION
#define DRVTSP_NOT_SUPPORTED            E_TSP_FAIL_NOT_SUPPORTED
#define DRVTSP_INVALID_SECFLT           E_TSP_FAIL_INVALID


#define E_DRVTSP_TSIF_EN                E_TSP_TSIF_EN
#define E_DRVTSP_TSIF_DIS               E_TSP_TSIF_DIS
#define E_DRVTSP_TSIF_SERL              E_TSP_TSIF_SERL
#define E_DRVTSP_TSIF_PARL              E_TSP_TSIF_PARL
#define E_DRVTSP_TSIF_EXTSYNC           E_TSP_TSIF_EXTSYNC
#define E_DRVTSP_TSIF_BITSWAP           E_TSP_TSIF_BITSWAP



#define DrvTSP_FltType                  TSP_FltType
// get TS from Live stream
#define E_DRVTSP_FLT_TYPE_VIDEO         E_TSP_FLT_FIFO_VIDEO
#define E_DRVTSP_FLT_TYPE_AUDIO         E_TSP_FLT_FIFO_AUDIO
#define E_DRVTSP_FLT_TYPE_AUDIO2        E_TSP_FLT_FIFO_AUDIO2
#define E_DRVTSP_FLT_TYPE_AUDIO3        E_TSP_FLT_FIFO_AUDIO3
#define E_DRVTSP_FLT_TYPE_AUDIO4        E_TSP_FLT_FIFO_AUDIO4
#define E_DRVTSP_FLT_TYPE_PVR           E_TSP_FLT_FIFO_PVR
#define E_DRVTSP_FLT_TYPE_PVR0          E_TSP_FLT_FIFO_PVR
#define E_DRVTSP_FLT_TYPE_PVR1          E_TSP_FLT_FIFO_PVR1
#define E_DRVTSP_FLT_TYPE_VIDEO3D       E_TSP_FLT_FIFO_VIDEO3D
#define E_DRVTSP_FLT_TYPE_REC           E_TSP_FLT_FIFO_REC
#define E_DRVTSP_FLT_TYPE_PVR2          E_TSP_FLT_FIFO_PVR2
#define E_DRVTSP_FLT_TYPE_PVR3          E_TSP_FLT_FIFO_PVR3
#define E_DRVTSP_FLT_TYPE_PVR4          E_TSP_FLT_FIFO_PVR4
#define E_DRVTSP_FLT_TYPE_PVR5          E_TSP_FLT_FIFO_PVR5



// Section Filter Group                 on Filter Group
#define E_DRVTSP_FLT_TYPE_SECTION_MASK  E_TSP_FLT_USER_MASK
#define E_DRVTSP_FLT_TYPE_SECTION       E_TSP_FLT_USER_SEC
#define E_DRVTSP_FLT_TYPE_PCR           E_TSP_FLT_USER_PCR
#define E_DRVTSP_FLT_TYPE_PES           E_TSP_FLT_USER_PES
#define E_DRVTSP_FLT_TYPE_PACKET        E_TSP_FLT_USER_PKT
#define E_DRVTSP_FLT_TYPE_TELETEXT      E_TSP_FLT_USER_TTX
#define E_DRVTSP_FLT_TYPE_EMM           E_TSP_FLT_USER_EMM
#define E_DRVTSP_FLT_TYPE_ECM           E_TSP_FLT_USER_ECM

#define E_DRVTSP_FLT_TYPE_LAST_ENUM     E_TSP_FLT_USER_NULL

#define E_DRVTSP_FLT_MASK               E_TSP_FLT_SRC_MASK
#define E_DRVTSP_FLT_SOURCE_TYPE_LIVE   E_TSP_FLT_SRC_LIVE0
#define E_DRVTSP_FLT_SOURCE_TYPE_FILE   E_TSP_FLT_SRC_TSIF01_FILE

#define E_DRVTSP_FLT_SCMB_MASK          E_TSP_FLT_FLAG_MASK
#define E_DRVTSP_FLT_TYPE_SCMB          E_TSP_FLT_FLAG_CA


#define DrvTSP_FltMode                  TSP_FltMode
#define E_DRVTSP_FLT_MODE_CONTI         E_TSP_FLT_MODE_CONTI
#define E_DRVTSP_FLT_MODE_ONESHOT       E_TSP_FLT_MODE_ONESHOT

// optional
#define E_DRVTSP_FLT_MODE_CRCCHK        E_TSP_FLT_MODE_CRCCHK

#define DrvTSP_FltState                 TSP_FltState
#define E_DRVTSP_FLT_STATE_FREE         E_TSP_FLT_STATE_FREE
#define E_DRVTSP_FLT_STATE_ALLOC        E_TSP_FLT_STATE_ALLOC
#define E_DRVTSP_FLT_STATE_ENABLE       E_TSP_FLT_STATE_ENABLE
#define E_DRVTSP_FLT_STATE_SCRAMBLED    E_TSP_FLT_STATE_SCRAMBLED
#define E_DRVTSP_FLT_STATE_STREAM_AVAIL E_TSP_FLT_STATE_STREAM_AVAIL
#define E_DRVTSP_FLT_STATE_SCRAMBLED_ORG E_TSP_FLT_STATE_SCRAMBLED_ORG
#define E_DRVTSP_FLT_STATE_OVERFLOW     E_TSP_FLT_STATE_OVERFLOW
#define E_DRVTSP_FLT_STATE_NA           E_TSP_FLT_STATE_NA


typedef enum //_DrvTSP_RecMode
{
    //[TODO] rename REC_MODE to PVR_MODE

    E_DRVTSP_REC_MODE_ENG0_FLTTYPE  =   E_TSP_REC_MODE_ENG0_FLTTYPE,
    E_DRVTSP_REC_MODE_ENG1_FLTTYPE  =   E_TSP_REC_MODE_ENG1_FLTTYPE,
    E_DRVTSP_REC_MODE_ENG0_BYPASS   =   E_TSP_REC_MODE_ENG0_BYPASS,
    E_DRVTSP_REC_MODE_ENG1_BYPASS   =   E_TSP_REC_MODE_ENG1_BYPASS,

} DrvTSP_RecMode;


typedef enum //_DrvTSP_CtrlMode
{
    E_DRVTSP_CTRL_MODE_TS0          =   E_TSP_CTRL_MODE_TS0,
    E_DRVTSP_CTRL_MODE_TS1          =   E_TSP_CTRL_MODE_TS1,
    E_DRVTSP_CTRL_MODE_MEM          =   E_TSP_CTRL_MODE_MEM,
} DrvTSP_CtrlMode;


typedef enum // @FIXME merge this enum with drvtsp2.h-> TSP_TSPad
{
    E_DRVTSP_PAD_DEMOD              =   0x0,
    E_DRVTSP_PAD_EXT_INPUT0         =   0x1,
    E_DRVTSP_PAD_EXT_INPUT1         =   0x2,
    E_DRVTSP_PAD_EXT_INPUT2           = 0x4,
} DrvTSP_PadIn;

typedef struct
{
    DrvTSP_PadIn                        ePad;
    MS_BOOL                             bClkInv;
    MS_BOOL                             bExtSync;
    MS_BOOL                             bParallel;
} DrvTSP_If_Set;

typedef enum
{
    E_DRVTSP_FILEIN_MODE_ENG0_TS      = E_TSP_FILE_2_TSP,
    E_DRVTSP_FILEIN_MODE_ENG0_VPES    = E_TSP_FILE_2_VIDEO,
    E_DRVTSP_FILEIN_MODE_ENG0_APES    = E_TSP_FILE_2_AUDIO,
    E_DRVTSP_FILEIN_MODE_ENG0_A2PES   = E_TSP_FILE_2_AUDIO2,
    E_DRVTSP_FILEIN_MODE_ENG0_V3DPES  = E_TSP_FILE_2_VIDEO3D,
    E_DRVTSP_FILEIN_MODE_ENG0_A3PES   = E_TSP_FILE_2_AUDIO3,
} DrvTSP_FileinMode;


#define E_DRVTSP_FILEIN_STATE_IDLE      E_TSP_FILE_STATE_IDLE
#define E_DRVTSP_FILEIN_STATE_BUSY      E_TSP_FILE_STATE_BUSY
#define E_DRVTSP_FILEIN_STATE_PAUSE     E_TSP_FILE_STATE_PAUSE

typedef TSP_FileState                 DrvTSP_FileinState;

#define DrvTSP_Event                    TSP_Event
#define E_DRVTSP_EVENT_DATA_INIT        E_TSP_EVENT_DATA_IDLE
#define E_DRVTSP_EVENT_DATA_READY       E_TSP_EVENT_DATA_READY
#define E_DRVTSP_EVENT_SEC_CRCERROR     E_TSP_EVENT_SEC_CRCERROR
#define E_DRVTSP_EVENT_BUF_OVERFLOW     E_TSP_EVENT_BUF_OVERFLOW
#define E_DRVTSP_EVENT_PVRBUF_FULL      E_TSP_EVENT_PVRBUF_FULL
#define E_DRVTSP_EVENT_PVRBUF_OVERFLOW  E_TSP_EVENT_PVRBUF_OVERFLOW

#define E_DRVTSP_EVENT_CB_MASK          E_TSP_EVENT_CB_MASK
#define E_DRVTSP_EVENT_CB_POLL          E_TSP_EVENT_CB_POLL
#define E_DRVTSP_EVENT_CB_SELF          E_TSP_EVENT_CB_AUTO

#define E_DRVTSP_PKTMODE_188            E_TSP_PKTMODE_188
#define E_DRVTSP_PKTMODE_192            E_TSP_PKTMODE_192
#define E_DRVTSP_PKTMODE_204            E_TSP_PKTMODE_204
#define E_DRVTSP_PKTMODE_130            E_TSP_PKTMODE_130
#define E_DRVTSP_PKTMODE_134            E_TSP_PKTMODE_134

typedef TSP_PktMode                     DrvTSP_PacketMode;

typedef TSP_EventMsg                    DrvTSP_Msg;
/// @name TSP_EventMsg
/// Macro definitions for manipulating TSP_EventMsg
/// @{
#define MSG_FLTINFO_SEC_ID_MASK         TSP_MSG_FLTID_MASK
#define MSG_FLTINFO_SEC_ID_SHFT         TSP_MSG_FLTID_SHFT
#define MSG_FLTINFO_ENG_ID_MASK         TSP_MSG_ENGID_MASK
#define MSG_FLTINFO_ENG_ID_SHFT         TSP_MSG_ENGID_SHFT
#define MSG_PVRBUF_ID_MASK              TSP_MSG_PVRID_MASK
#define MSG_PVRBUF_ID_SHFT              TSP_MSG_PVRID_SHFT
#define MSG_PVRBUF_ID_NULL              TSP_MSG_PVRID_NULL
/// @}


typedef enum //_DrvTSP_DbgLevel
{
    E_DRVTSP_DBG_Release            =   E_TSP_DBG_L0,
    E_DRVTSP_DBG_L1                 =   E_TSP_DBG_L3,
    E_DRVTSP_DBG_L2                 =   E_TSP_DBG_L7,
} DrvTSP_DbgLevel;

#define E_DRVTSP_CAP_FILTER_NUM     E_TSP_CAP_RECFLT_IDX

typedef enum
{
    E_DRVTSP_HW_STATUS              =   E_TSP_HW_STATUS,
    E_DRVTSP_HW_INIT                =   E_TSP_HW_INIT,
    E_DRVTSP_HW_ALIVE               =   E_TSP_HW_ALIVE,
    E_DRVTSP_HW_DEALIVE             =   E_TSP_HW_DEALIVE,
} DrvTSP_HW_Status;

typedef enum
{
    E_DRVTSP_SW_INIT                =   E_TSP_SW_INIT,
    E_DRVTSP_SW_INIT_FAIL           =   E_TSP_SW_INIT_FAIL,
    E_DRVTSP_SW_SUSPEND             =   E_TSP_SW_SUSPEND,
    E_DRVTSP_SW_RESUME              =   E_TSP_SW_RESUME,
    E_DRVTSP_SW_POWEROFF            =   E_TSP_SW_POWEROFF,
    E_DRVTSP_SW_CLOSE               =   E_TSP_SW_CLOSE,
    E_DRVTSP_SW_EXIT                =   E_TSP_SW_EXIT,
} DrvTSP_SW_Status;

typedef P_TSP_Callback              P_DrvTSP_EvtCallback;

// TSP event define
#define TSP_TASK_EVENT_SECTION_SELF     TSP_EVENT_SECTION
#define TSP_TASK_EVENT_FLT_FREE         TSP_EVENT_FREEFLT

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
extern  TSP_Result                              MDRV_TSP_FAIL_NOT_SUPPORTED(void);

#define MDrv_TSP_PidFlt_Alloc                   MDrv_TSP_FLT_Alloc
#define MDrv_TSP_PidFlt_Free(e, p...)           MDrv_TSP_FLT_Free(p)
#define MDrv_TSP_PidFlt_SetPid(e, p...)         MDrv_TSP_FLT_SetPID(p)
#define MDrv_TSP_PidFlt_GetPid(e, p...)         MDrv_TSP_FLT_GetPID(p)
#define MDrv_TSP_PidFlt_SelSecFlt(e, p...)      MDrv_TSP_FLT_SelSEC(p)
#define MDrv_TSP_PidFlt_Enable(e, p...)         MDrv_TSP_FLT_Enable(p)
#define MDrv_TSP_PidFlt_GetState(e, p...)       MDrv_TSP_FLT_GetState(p)
#define MDrv_TSP_PidFlt_ChangeSource(e, p...)   MDrv_TSP_FLT_ChangeSource(p)
#define MDrv_TSP_PidFlt_SetFltRushPass(e, p...) MDrv_TSP_FLT_SetFltRushPass(p)
#define MDrv_TSP_PidFlt_GetScmbSts(p...)        MDrv_TSP_Flt_GetScmbSts(p)


#define MDrv_TSP_SecFlt_SetMode(e, p...)        MDrv_TSP_FLT_SetMode(p)
#define MDrv_TSP_SecFlt_SetPattern(e, p...)     MDrv_TSP_FLT_SetMatch(p)
#define MDrv_TSP_SecFlt_Notify(e, p...)         MDrv_TSP_FLT_SetCallback(p)
#define MDrv_TSP_SecFlt_GetState(e, p...)       MDrv_TSP_FLT_GetState(p)

#define MDrv_TSP_SecFlt_Alloc                   MDrv_TSP_SEC_Alloc
#define MDrv_TSP_SecFlt_Free(e, p...)           MDrv_TSP_SEC_Free(p)
#define MDrv_TSP_SecFlt_SetBuffer(e, p...)      MDrv_TSP_SEC_SetBuffer(p)
#define MDrv_TSP_SecFlt_GetBufStart(e, p...)    MDrv_TSP_SEC_GetStart(p)
#define MDrv_TSP_SecFlt_GetBufSize(e, p...)     MDrv_TSP_SEC_GetSize(p)
#define MDrv_TSP_SecFlt_GetReadAddr(e, p...)    MDrv_TSP_SEC_GetRead(p)
#define MDrv_TSP_SecFlt_GetWriteAddr(e, p...)    MDrv_TSP_SEC_GetWrite(p)
#define MDrv_TSP_SecFlt_ResetBuffer(e, p...)    MDrv_TSP_SEC_Reset(p)
#define MDrv_TSP_SecFlt_Update(e, p...)         MDrv_TSP_SEC_Update(p)

#define MDrv_TSP_SecFlt_SetReadAddr(e, p...)    MDrv_TSP_SEC_SetRead(p)

#define MDrv_TSP_File_SetAddr                   MDrv_TSP_FILE_CmdAddr
#define MDrv_TSP_File_SetSize                   MDrv_TSP_FILE_CmdSize
#define MDrv_TSP_File_Start(p1...)              MDrv_TSP_FILE_CmdStart((TSP_FileMode)p1)
#define MDrv_TSP_File_Pause                     MDrv_TSP_FILE_Pause
#define MDrv_TSP_File_Resume                    MDrv_TSP_FILE_Resume
#define MDrv_TSP_File_Stop                      MDrv_TSP_FILE_Stop
#define MDrv_TSP_File_GetState                  MDrv_TSP_FILE_GetState
#define MDrv_TSP_File_SetRate                   MDrv_TSP_FILE_SetRate
#define MDrv_TSP_File_SetPacketMode             MDrv_TSP_FILE_SetPktMode
#define MDrv_TSP_File_PS_Path_Enable(p1...)     MDrv_TSP_FILE_PS_Path_Enable((TSP_FileMode)p1)
#define MDrv_TSP_AVFifo_BlockEnable             MDrv_TSP_FIFO_BlockEnable

#define MDrv_TSP_File_Eng_SetAddr                   MDrv_TSP_FILE_Eng_CmdAddr
#define MDrv_TSP_File_Eng_SetSize                   MDrv_TSP_FILE_Eng_CmdSize
#define MDrv_TSP_File_Eng_Start(p1, p2...)          MDrv_TSP_FILE_Eng_CmdStart(p1, (TSP_FileMode)p2)
#define MDrv_TSP_SetSTC                             MDrv_TSP_SetSTC64  //@F_TODO not decided the name yet why cmdstc?
#define MDrv_TSP_GetSTC                             MDrv_TSP_GetSTC64  //@F_TODO not decided the name yet why cmdstc?
#define MDrv_TSP_File_Eng_Pause                     MDrv_TSP_FILE_Eng_Pause
#define MDrv_TSP_File_Eng_Resume                    MDrv_TSP_FILE_Eng_Resume
#define MDrv_TSP_File_Eng_Stop                      MDrv_TSP_FILE_Eng_Stop
#define MDrv_TSP_File_Eng_GetState                  MDrv_TSP_FILE_Eng_GetState
#define MDrv_TSP_File_Eng_SetRate                   MDrv_TSP_FILE_Eng_SetRate
#define MDrv_TSP_File_Eng_SetPacketMode             MDrv_TSP_FILE_Eng_SetPktMode
#define MDrv_TSP_File_Eng_PS_Path_Enable(p1...)     MDrv_TSP_FILE_Eng_PS_Path_Enable((TSP_FileMode)p1)
#define MDrv_TSP_File_Eng_CMDQ_GetSlot              MDrv_TSP_FILE_Eng_GetCmdSlot
#define MDrv_TSP_File_Eng_CMDQ_Reset                MDrv_TSP_FILE_Eng_Reset

#define MDrv_TSP_AVFifo_Reset                   MDrv_TSP_FIFO_Reset
#define MDrv_TSP_AVFifo_Status                  MDrv_TSP_FIFO_GetStatus
#define MDrv_TSP_AVFifo_Overflow_Status         MDrv_TSP_FIFO_Overflow_Status
#define MDrv_TSP_AVFifo_SourceSelect            MDrv_TSP_FIFO_SourceSelect

#define MDrv_TSP_File_CMDQ_GetSlot              MDrv_TSP_FILE_GetCmdSlot
#define MDrv_TSP_File_CMDQ_Reset                MDrv_TSP_FILE_Reset

#define MDrv_TSP_FLT_ChkOwner(p...)             MDRV_TSP_FAIL_NOT_SUPPORTED()
#define MDrv_TSP_BurstLen                       MDrv_TSP_SECFLT_BurstLen
#define MDrv_TSP_IsAccess(p...)                 MDRV_TSP_FAIL_NOT_SUPPORTED()
#define MDrv_TSP_UnlockAccess(p...)             MDRV_TSP_FAIL_NOT_SUPPORTED()
#define MDrv_TSP_TTX_SecFlt_GetWriteAddr(p...)  MDRV_TSP_FAIL_NOT_SUPPORTED()
#define MDrv_TSP_TTX_SecFlt_GetWriteAddr(p...)  MDRV_TSP_FAIL_NOT_SUPPORTED()
#define MDRV_TSP_Proc_Pvr(p...)                 MDRV_TSP_FAIL_NOT_SUPPORTED()

#define MDrv_TSP_GetCap                         MDrv_TSP_GetCaps
#define MDrv_TSP_GetDbgPortInfo                 MDrv_TSP_DBG_GetDBGStatus

#define MDrv_TSP_Get_FW_VER                     MDrv_TSP_GetFWVER

#define MDrv_TSP_PVR_Start(p1, p2...)           MDrv_TSP_PVR_Start((TSP_RecMode)p1, p2)

#define MDrv_TSP_Parl_BitOrderSwap(e, p...)     MDrv_TSP_PARL_BitOrderSwap(p)

#define MDrv_TSP_PidFltFIFO_FullSel             MDrv_TSP_FLTFIFO_FullSel

#ifdef __cplusplus
} // closing brace for extern "C"
#endif


#endif // __DRV_TSP_LEGACY_H__
