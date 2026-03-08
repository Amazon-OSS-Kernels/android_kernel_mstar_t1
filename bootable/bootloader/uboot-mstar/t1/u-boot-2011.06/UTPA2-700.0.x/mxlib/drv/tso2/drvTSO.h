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
///////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSO.h
/// @brief  TS I/O Driver Interface
/// @attention
/// All TSO DDI are not allowed to use in any interrupt context other than TSO ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_TSO_H_
#define _DRV_TSO_H_



#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

/// TSP byte address alignment unit
#define DRVTSO_ALIGN_UNIT           16UL

/// TSP byte address alignment macro
#define DRVTSO_ALIGN(_x)            ALIGN_16((MS_U32)_x)

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// Transport stream null PID
#define DRVTSO_PID_NULL             0x1FFFUL

#define MSIF_TSO_LIB_CODE           {'T','S','O'}                                               // Lib code
#define MSIF_TSO_LIBVER             {'0','2'}                                                   // LIB version
#define MSIF_TSO_BUILDNUM           {'0','0'}                                                   // Build Number
#define MSIF_TSO_CHANGELIST         {'0','0','5','1','1','4','7','0'}                           // P4 ChangeList Number

#define TSO_DRV_VERSION             /* Character String for DRV/API version             */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    MSIF_CUS,                       /* 0x0000                                           */  \
    MSIF_MOD,                       /* 0x0000                                           */  \
    MSIF_CHIP,                                                                              \
    MSIF_CPU,                                                                               \
    MSIF_TSO_LIB_CODE,              /* IP__                                             */  \
    MSIF_TSO_LIBVER,                /* 0.0 ~ Z.Z                                        */  \
    MSIF_TSO_BUILDNUM,              /* 00 ~ 99                                          */  \
    MSIF_TSO_CHANGELIST,            /* CL#                                              */  \
    MSIF_OS


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// TSO DDI return value

/// @name TSO_Result
/// @ref TSO_Result
/// return value
/// @{
typedef enum
{
    DRVTSO_FAIL                      = 0,
    DRVTSO_OK                        = 1,
    DRVTSO_INVALID_PARAM             = 2,
    DRVTSO_FUNC_ERROR                = 3,
}TSO_Result;
/// @}

typedef enum //_DrvTSO_DbgLevel
{
    E_DRVTSO_DBG_Release = 0,
    E_DRVTSO_DBG_ERR, // display error msg
    E_DRVTSO_DBG_INFO,
} DrvTSO_DbgLevel;

typedef enum
{
    E_DRVTSO_CAP_FILTER_NUM             = 0,                     // Get filter number
    E_DRVTSO_CAP_RESOURCE_SIZE          = 1,                     // Get the data structure size of private resource (share resource)
    E_DRVTSO_CAP_ENGINE_NUM             = 2,                     // Get TSO engine number
    E_DRVTSO_CAP_ENGINE_TSIF_NUM        = 3,                     // Get TSO interface number of every engine

    E_DRVTSO_CAP_NULL,
} DrvTSO_Cap;

typedef enum
{
    /// Command Queue is Idle
    E_DRVTSO_STATE_IDLE        = 0x00,
    /// Command Queue is Busy
    E_DRVTSO_STATE_BUSY        = 0x01,

    E_DRVTSO_STATE_UNKNOWN     = 0xFF,
} DrvTSO_FileinState;

typedef enum
{
    E_DRVTSO_STATUS_UNKNOWN         = 0,
    E_DRVTSO_STATUS_FILEINDONE      = 0x01,    //bit 0
} DrvTSO_Status;

typedef enum
{
    E_DRVTSO_FLT_STATE_FREE           = 0x00000000UL,                                                 ///<\n
    E_DRVTSO_FLT_STATE_ALLOC          = 0x00000001UL,                                                 ///<\n
    E_DRVTSO_FLT_STATE_ENABLE         = 0x00000002UL,                                                 ///<\n
    E_DRVTSO_FLT_STATE_NA             = 0xFFFFFFFFUL,
} DrvTSO_FltState;

typedef enum
{
    E_DRVTSO_CTRL_MODE_TS0,             //compatible to tso 1.0, not support int tso 2.0
    E_DRVTSO_CTRL_MODE_TS1,
    E_DRVTSO_CTRL_MODE_TS2,
    E_DRVTSO_CTRL_MODE_TS3,
    E_DRVTSO_CTRL_MODE_TSCB,            //compatible to tso 1.0, not support int tso 2.0
    E_DRVTSO_CTRL_MODE_DEMOD0,          //compatible to tso 1.0, not support int tso 2.0
    E_DRVTSO_CTRL_MODE_DEMOD1,          //compatible to tso 1.0, not support int tso 2.0
    E_DRVTSO_CTRL_MODE_MEM,
    E_DRVTSO_CTRL_MODE_MEM1,

    E_DRVTSO_CTRL_MODE_TS5,
    E_DRVTSO_CTRL_MODE_TS6,

    E_DRVTSO_CTRL_MODE_MMT,

    E_DRVTSO_CTRL_MODE_INVALID
} DrvTSO_CtrlMode;

/// TSP file in Packet mode
typedef enum //_DrvTSO_PacketMode
{
    E_DRVTSO_PKTMODE_188              = 0x00000000UL,
    E_DRVTSO_PKTMODE_192              = 0x00000001UL,
    E_DRVTSO_PKTMODE_204              = 0x00000002UL,
    E_DRVTSO_PKTMODE_130              = 0x00000003UL,    // RVU
    E_DRVTSO_PKTMODE_134              = 0x00000004UL,    // RVU with timestamp
    E_DRVTSO_PKTMODE_200              = 0x00000005UL,    // packet size with 200 (Open cable Single stream)
} DrvTSO_PacketMode;

typedef enum
{
    E_DRVTSO_OUTCLK_DIV2N,
    E_DRVTSO_OUTCLK_DIVN,
    E_DRVTSO_OUTCLK_62M,
    E_DRVTSO_OUTCLK_54M,
    E_DRVTSO_OUTCLK_PTSOOUT,
    E_DRVTSO_OUTCLK_PTSOOUT_DIV8,
    E_DRVTSO_OUTCLK_Dmd,
    E_DRVTSO_OUTCLK_27M,
}DrvTSO_OutClk;

typedef enum
{
    E_DRVTSO_OUTCLKSRC_172M_2N,
    E_DRVTSO_OUTCLKSRC_288M_2N,
    E_DRVTSO_OUTCLKSRC_432M_2N,
    E_DRVTSO_OUTCLKSRC_216M_N,
    E_DRVTSO_OUTCLKSRC_P_TS0IN,
    E_DRVTSO_OUTCLKSRC_P_TS1IN,
    E_DRVTSO_OUTCLKSRC_P_TS2IN,
    E_DRVTSO_OUTCLKSRC_P_TS3IN,
    E_DRVTSO_OUTCLKSRC_P_TS4IN,
    E_DRVTSO_OUTCLKSRC_P_TS5IN,
}DrvTSO_OutClkSrc;

/// TSO interface
typedef enum
{
    E_DRVTSO_IF_LIVE0                      = 0x00,
    E_DRVTSO_IF_LIVE1                      = 0x01,
    E_DRVTSO_IF_LIVE2                      = 0x02,

    E_DRVTSO_IF_FILE0                      = 0x80,
    E_DRVTSO_IF_FILE1                      = 0x81,

    E_DRVTSO_IF_MMT0                       = 0x40,
} DrvTSO_If;

/// TSO input pad selection
typedef enum
{
    E_DRVTSO_PAD_EXT_INPUT0           = 0x00,
    E_DRVTSO_PAD_EXT_INPUT1           = 0x01,
    E_DRVTSO_PAD_EXT_INPUT2           = 0x02,
    E_DRVTSO_PAD_EXT_INPUT3           = 0x03,
    E_DRVTSO_PAD_EXT_INPUT4           = 0x04,
    E_DRVTSO_PAD_EXT_INPUT5           = 0x05,
    E_DRVTSO_PAD_DEMOD                = 0x07,

    E_DRVTSO_MEM                      = 0x80,
} DrvTSO_PadIn;

/// TSO output pad selection
typedef enum
{
    E_DRVTSO_PAD_NONE                  = 0x0,
    E_DRVTSO_PAD_EXT_OUTPUT1           = 0x1,
    E_DRVTSO_PAD_EXT_OUTPUT3           = 0x3,
} DrvTSO_PadOut;

typedef enum
{
    E_DRVTSO_CFG_NONE                  = 0x00000000UL,
    E_DRVTSO_CFG_OPIF                  = 0x00000001UL,
}DrvTSO_CfgCmd;

typedef struct
{
    DrvTSO_PadIn        ePad;
    MS_BOOL             bClkInv;
    MS_BOOL             bExtSync;
    MS_BOOL             bParallel;
} DrvTSO_If_Set;

typedef struct
{
    MS_U32              u32TsoEng;
    MS_BOOL             bSet;

    DrvTSO_CfgCmd       eCfgCmd;
    MS_U32              u32CmdData0;
}DrvTSO_Cfg;

#ifdef TSO_PVR_SUPPORT
typedef enum
{
    E_DRVTSO_PVR_SRC_NONE              = 0x00000000UL,
    E_DRVTSO_PVR_SRC_SVQ               = 0x00000001UL,
    E_DRVTSO_PVR_SRC_MMT               = 0x00000002UL,
}DrvTSO_PvrSrc;

/// TSO record mode
typedef enum
{
    E_DRVTSO_REC_MODE_FLTTYPE          = 0x00000000UL,
    E_DRVTSO_REC_MODE_BYPASS           = 0x00000002UL,
} DrvTSO_RecMode;
#endif //TSO_PVR_SUPPORT

TSO_Result MDrv_TSO_InitLibResource(void* pResMemAddr);
TSO_Result MDrv_TSO_Init(void);
TSO_Result MDrv_TSO_Exit(void);
TSO_Result MDrv_TSO_SetDbgLevel(DrvTSO_DbgLevel DbgLevel);
TSO_Result MDrv_TSO_Get_HWIntStatus(MS_U8 u8Eng, MS_U16* pu16status);
TSO_Result MDrv_TSO_SetOperateMode(MS_U8 u8Eng, DrvTSO_CtrlMode TSOCtrlMode, MS_BOOL bBypassAll, MS_BOOL bEnable);
TSO_Result MDrv_TSO_Ch_InputEnable(MS_U8 u8Eng, DrvTSO_If eIf, MS_BOOL bEnable);
TSO_Result MDrv_TSO_OutputEnable(MS_U8 u8Eng, MS_BOOL bEnable);
TSO_Result MDrv_TSO_Filein_Rate(MS_U8 u8FileEng, MS_U16 u16timer);
TSO_Result MDrv_TSO_SetOutClk(MS_U8 u8Eng, DrvTSO_OutClk eOutClk, DrvTSO_OutClkSrc eOutClkSrc, MS_U16 u16DivNum, MS_BOOL bClkInv);
TSO_Result MDrv_TSO_GetOutClk(MS_U8 u8Eng, DrvTSO_OutClk* peOutClk, DrvTSO_OutClkSrc* peOutClkSrc, MS_U16* pu16DivNum, MS_BOOL* pbClkInv);
TSO_Result MDrv_TSO_GetTSIFStatus(MS_U8 u8Eng, DrvTSO_If eIf, DrvTSO_If_Set* pIfSet, MS_BOOL* pbBypassAll, MS_BOOL* pbEnable);

// PIDFlt API
TSO_Result MDrv_TSO_PidFlt_Alloc(MS_U8 u8Eng, MS_U16 *pu16PidFltId);
TSO_Result MDrv_TSO_PidFlt_Free(MS_U8 u8Eng, MS_U16 u16PidFltId);
TSO_Result MDrv_TSO_PidFlt_SetPid(MS_U8 u8Eng, MS_U16 u16PidFltId, MS_U16 u16Pid);
TSO_Result MDrv_TSO_PidFlt_SetChSrc(MS_U8 u8Eng, MS_U16 u16PidFltId, DrvTSO_If eIf);
TSO_Result MDrv_TSO_RepPidFlt_Alloc(MS_U8 u8Eng, MS_U16 *pu16PidFltId);
TSO_Result MDrv_TSO_RepPidFlt_Set(MS_U8 u8Eng, MS_U16 u16PidFltId, DrvTSO_If eIf, MS_U16 u16OrgPid, MS_U16 u16NewPid);
TSO_Result MDrv_TSO_RepPidFlt_Enable(MS_U8 u8Eng, MS_U16 u16PidFltId, MS_BOOL bEnable);
TSO_Result MDrv_TSO_RepPidFlt_Free(MS_U8 u8Eng, MS_U16 u16PidFltId);

TSO_Result MDrv_TSO_Filein_CmdQ_Reset(MS_U8 u8FileEng);
TSO_Result MDrv_TSO_Filein_CmdQFifo_Status(MS_U8 u8FileEng, MS_U8 *pu8FifoLevel);
TSO_Result MDrv_TSO_Filein_CmdQ_GetSlot(MS_U8 u8FileEng, MS_U32 *pu32EmptySlot);

TSO_Result MDrv_TSO_SetPlaybackTimeStamp(MS_U8 u8FileEng, MS_U32 u32timestamp);
TSO_Result MDrv_TSO_GetPlaybackTimeStamp(MS_U8 u8FileEng, MS_U32 *pu32timestamp);

TSO_Result MDrv_TSO_GetFileInTimeStamp(MS_U8 u8FileEng, MS_U32* pu32Timestamp);
TSO_Result MDrv_TSO_Filein_GetReadAddr(MS_U8 u8FileEng, MS_PHY *pphyReadAddr);
TSO_Result MDrv_TSO_Filein_SetAddr(MS_U8 u8FileEng, MS_PHY phyStreamAddr);
TSO_Result MDrv_TSO_Filein_SetSize(MS_U8 u8FileEng, MS_U32 u32StreamSize);
TSO_Result MDrv_TSO_SetPacketMode(MS_U8 u8FileEng, DrvTSO_PacketMode ePktMode);
TSO_Result MDrv_TSO_Set_InputPktSize(MS_U8 u8Eng, DrvTSO_If eIf, MS_U16 u16InPktSize);
TSO_Result MDrv_TSO_OutputPktSize(MS_U8 u8Eng, MS_U16* pu16OutPktSize, MS_BOOL bSet);
TSO_Result MDrv_TSO_Configure(DrvTSO_Cfg* pstTsoCfg);
TSO_Result MDrv_TSO_Filein_192Mode_En(MS_U8 u8FileEng, MS_BOOL bEnable);
TSO_Result MDrv_TSO_Filein_192BlockScheme_En(MS_U8 u8FileEng, MS_BOOL bEnable);

TSO_Result MDrv_TSO_TEI_SkipPkt(MS_U8 u8Eng, DrvTSO_If eIf, MS_BOOL bEnable);
TSO_Result MDrv_TSO_Clr_ByteCnt(MS_U8 u8Eng, DrvTSO_If eIf);
TSO_Result MDrv_TSO_GetCap(DrvTSO_Cap eCap, void* pOutput);

TSO_Result MDrv_TSO_Filein_GetState(MS_U8 u8FileEng, DrvTSO_FileinState *peFileinState);
TSO_Result MDrv_TSO_Filein_ChkStatus(MS_U8 u8Eng, MS_U8 u8FileEng, DrvTSO_Status* pStatus);
TSO_Result MDrv_TSO_Filein_ClearHWInt(MS_U8 u8Eng, MS_U16 u16int);

TSO_Result MDrv_TSO_Filein_Start(MS_U8 u8FileEng);
TSO_Result MDrv_TSO_Filein_Stop(MS_U8 u8FileEng);
TSO_Result MDrv_TSO_Filein_Abort(MS_U8 u8FileEng);

TSO_Result MDrv_TSO_ResetAll(MS_U8 u8Eng);
TSO_Result MDrv_TSO_SelPad(MS_U8 u8Eng, DrvTSO_If eIf, DrvTSO_If_Set* pIfSet);
TSO_Result MDrv_TSO_OutputPad(MS_U8 u8Eng, DrvTSO_PadOut* peOutPad, MS_BOOL bSet);
TSO_Result MDrv_TSO_Set_SVQBuf(MS_U8 u8Eng, MS_PHY phyBufAddr, MS_U32 u32Size);
TSO_Result MDrv_TSO_Set_SVQ_LocalSysTimestamp(MS_U8 u8Eng, MS_U32 u32timestamp);
TSO_Result MDrv_TSO_LocalStreamId(MS_U8 u8Eng, DrvTSO_If eIf, MS_U8 *pu8Id, MS_BOOL bSet);

MS_BOOL    MDrv_TSO_Check_IsSVQTxBusy(MS_U8 u8Eng, DrvTSO_If eIf);
MS_BOOL    MDrv_TSO_Check_IsSVQTxEmpty(MS_U8 u8Eng, DrvTSO_If eIf);

#ifdef TSO_PVR_SUPPORT
TSO_Result MDrv_TSO_PVR_SelSrc(MS_U8 u8PVRId, DrvTSO_PvrSrc ePvrSrc);
TSO_Result MDrv_TSO_PVR_SetBuf(MS_U8 u8PVRId, MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1);
TSO_Result MDrv_TSO_PVR_SetRecordStampClk(MS_U8 u8PVRId, MS_U32 u32Clk);
TSO_Result MDrv_TSO_PVR_SetPacketMode(MS_U8 u8PVRId, MS_BOOL bSet);
TSO_Result MDrv_TSO_PVR_TimeStampSetRecordStamp(MS_U8 u8PVRId, MS_U32 u32Stamp);
TSO_Result MDrv_TSO_PVR_TimeStampGetRecordStamp(MS_U8 u8PVRId, MS_U32* pu32Stamp);
TSO_Result MDrv_TSO_PVR_Start(MS_U8 u8PVRId, DrvTSO_RecMode eRecMode, MS_BOOL bStart);
TSO_Result MDrv_TSO_PVR_GetWriteAddr(MS_U8 u8PVRId, MS_PHY *pphy2WriteAddr);
#endif  //TSO_PVR_SUPPORT


#ifdef MSOS_TYPE_LINUX_KERNEL
TSO_Result MDrv_TSO_Suspend(void);
TSO_Result MDrv_TSO_Resume(void);
#endif //MSOS_TYPE_LINUX_KERNEL


#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif // _DRV_TSO_H_


