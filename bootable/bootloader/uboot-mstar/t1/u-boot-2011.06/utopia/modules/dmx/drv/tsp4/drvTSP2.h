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
/// file   drvTSP2.h
/// @brief  Transport Stream Processer (TSP) Driver Interface
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_TSP2_H__
#define __DRV_TSP2_H__



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

#define TSP_VER_1_0                     0x0100
#define TSP_VER_2_0                     0x0200
#define TSP_VER_3_0                     0x0300
#define TSP_VER_4_0                     0x0400


#define TSP_VERSION                     TSP_VER_4_0

/// Transport stream null PID
#define MSIF_TSP_LIB_CODE               {'T','S','P','4'}                                           // Lib code
#define MSIF_TSP_LIBVER                 {'0','1'}                                                   // LIB version
#define MSIF_TSP_BUILDNUM               {'0','0'}                                                   // Build Number
#define MSIF_TSP_CHANGELIST             {'0','0','5','8','5','9','0','3'}                           // P4 ChangeList Number

#define TSP_DRV_VERSION                 /* Character String for DRV/API version         */  \
    MSIF_TAG,                           /* 'MSIF'                                       */  \
    MSIF_CLASS,                         /* '00'                                         */  \
    MSIF_CUS,                           /* 0x0000                                       */  \
    MSIF_MOD,                           /* 0x0000                                       */  \
    MSIF_CHIP,                                                                              \
    MSIF_CPU,                                                                               \
    MSIF_TSP_LIB_CODE,                  /* IP__                                         */  \
    MSIF_TSP_LIBVER,                    /* 0.0 ~ Z.Z                                    */  \
    MSIF_TSP_BUILDNUM,                  /* 00 ~ 99                                      */  \
    MSIF_TSP_CHANGELIST,                /* CL#                                          */  \
    MSIF_OS

#define __LEGACY__                      1 // only for development

#define TSP_PID_NULL                    0x1FFF

#define TSP_FILTER_DEPTH                16                                                          // TSP_FILTER_DEPTH

/// TSP byte address alignment unit
#define TSP_ALIGN_UNIT                  16

/// TSP byte address alignment macro
#define TSP_ALIGN(_x)                   ALIGN_16((MS_U32)_x)

// TSP event define
#define TSP_EVENT_SECTION               0x00000001
#define TSP_EVENT_DISABLEFLT            0x00000080
#define TSP_EVENT_FREEFLT               0x00000040
#define TSP_EVENT_TASKEND               0x80000000

#define TSP_EVENT_SECTION_POLL          0x00000002

#define TSP_TASK_EVENTS                 ( TSP_EVENT_SECTION | TSP_EVENT_FREEFLT|TSP_EVENT_DISABLEFLT | TSP_EVENT_TASKEND)
#define TSP_POLL_EVENTS                 ( TSP_EVENT_SECTION_POLL )

#if __LEGACY__

// @FIXME: move to RASP
#undef  TSP_TASK_EVENTS
#define TSP_TASK_EVENTS                 ( TSP_EVENT_SECTION  | TSP_EVENT_FREEFLT |TSP_EVENT_DISABLEFLT | \
                                          TSP_EVENT_PVR0_RDY | TSP_EVENT_PVR1_RDY | TSP_EVENT_TASKEND  )

#define TSP_EVENT_PVR0_RDY              0x00000004
#define TSP_EVENT_PVR1_RDY              0x00000008
// @FIXME: remove
#define TSP_EVENT_PVR0_RDY_POLL         0x00000010
#define TSP_EVENT_PVR1_RDY_POLL         0x00000020

// @FIXME: remove
#undef  TSP_POLL_EVENTS
#define TSP_POLL_EVENTS                 ( TSP_EVENT_SECTION_POLL  | \
                                          TSP_EVENT_PVR0_RDY_POLL | \
                                          TSP_EVENT_PVR1_RDY_POLL   )

#endif // __LEGACY__


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum _TSP_Id
{
    E_TSP_0                         =   0,
    E_TSP_1,
    E_TSP_2,
    E_TSP_NULL                      =   0xFF,
} TSP_Id;


/// TSP DDI return value
/// @name TSP_Result
/// @ref TSP_Result
/// return value
/// @{
typedef enum
{
    E_TSP_FAIL                      =   0,
    E_TSP_OK,
    E_TSP_FAIL_PARAMETER,
    E_TSP_FAIL_FUNCTION,
    E_TSP_FAIL_NOT_SUPPORTED,
    E_TSP_FAIL_INVALID,
    E_TSP_FAIL_NODATA,

} TSP_Result;
/// @}

/// TSP Init Parameter
/// @name TSP_InitParam
/// @{
typedef struct _TSP_InitParam
{
    MS_PHYADDR                          phyFWAddr;                                                  /// Firmware source physical address
    MS_U32                              u32FWSize;

    MS_PHYADDR                          phyVQAddr;                                                  /// Internal buffer physical address
    MS_U32                              u32VQSize;

} TSP_InitParam;
/// @}


// @NOTE should be Exclusive usage
typedef MS_U32                          TSP_TSIFCfg;
#define E_TSP_TSIF_EN                   0x0001              // 1: enable ts interface 0 and vice versa
#define E_TSP_TSIF_DIS                  0x0000
#define E_TSP_TSIF_SERL                 0x0000
#define E_TSP_TSIF_PARL                 0x0002
#define E_TSP_TSIF_EXTSYNC              0x0004
#define E_TSP_TSIF_BITSWAP              0x0008
#define E_TSP_TSIF_3WIRE                0x0010


/// TSP TS Input Source
typedef enum _TSP_TSPad
{
    E_TSP_TS_PAD0,
    E_TSP_TS_PAD1,
    E_TSP_TS_PAD2,
    E_TSP_TS_PAD3,

    E_TSP_TS_PAD_RESERVED,

    E_TSP_TS_DEMOD0,
    E_TSP_TS_DEMOD1,

    E_TSP_TS_DEMOD_RESERVED,

    E_TSP_TS_PAD_TSO0,
    E_TSP_TS_PAD_TSO1,

    E_TSP_TS_PAD0_3WIRE,
    E_TSP_TS_PAD1_3WIRE,
    E_TSP_TS_PAD2_3WIRE,
    E_TSP_TS_PAD3_3WIRE,

    E_TSP_TS_PAD4,
    E_TSP_TS_PAD5,

    E_TSP_TS_INVALID,
} TSP_TSPad;

typedef enum _TSP_InPadMode
{
    E_TSP_IN_PADMODE_PARALLEL,
    E_TSP_IN_PADMODE_SERIAL,       //4-wire
    E_TSP_IN_PADMODE_SERIAL_3WIRE, //3-wire

    E_TSP_IN_PADMODE_INVALID
} EN_TSP_InPadMode;

typedef enum _TSP_OutPadMode
{
    E_TSP_OUT_PADMODE_TSO   = 0x10000000,
    E_TSP_OUT_PADMODE_S2P   = 0x20000000,
    E_TSP_OUT_PADMODE_S2P1  = 0x30000000,
    E_TSP_OUT_PADMODE_DEMOD = 0x40000000,

    E_TSP_OUT_PADMODE_MASK  = 0xF0000000
} EN_TSP_OutPadMode;

typedef enum _TSP_TSId // for k3 TSP Path //@FIXME modify _TSP_TSId -> _TSP_TSPATH
{
    E_TSP_TSID_TSIF0          = 0 ,
    E_TSP_TSID_TSIF1          = 1 ,
    E_TSP_TSID_TSIF01_FILE    = 2 ,
    E_TSP_TSID_TSIF2          = 3 ,
    E_TSP_TSID_TSIF3          = 4 ,
    E_TSP_TSID_TSIF23_FILE    = 5 ,
    E_TSP_TSID_MMFI0          = 6 ,
    E_TSP_TSID_MMFI1          = 7 ,

    E_TSP_TSID_INVALID,

} TSP_TSId; //@FIXME modify TSP_TSId -> TSP_TSPath

typedef enum _TSP_TSIF // for HW TSIF
{
    E_TSP_TSIF_0              = 0 ,
    E_TSP_TSIF_1              = 1 ,
    E_TSP_TSIF_2              = 2 ,
    E_TSP_TSIF_3              = 3 ,
    E_TSP_TSIF_TSP_MAX        = 3 ,
    E_TSP_TSIF_CB             = 4 ,
    E_TSP_TSIF_TSO0           = 5 ,
    E_TSP_TSIF_TSO1           = 6 ,
    E_TSP_TSIF_RASP0          = 7 ,
    E_TSP_TSIF_RASP1          = 8 ,
    E_TSP_TSIF_EMMFLT         = 9 ,
    // @NOTE There are no real TSIFs for TSIF_PVRx , just use those for PVR backward competiable.
    E_TSP_TSIF_PVR0           = 10 ,
    E_TSP_TSIF_PVR1           = 11 ,
    E_TSP_TSIF_PVR2           = 12 ,
    E_TSP_TSIF_PVR3           = 13 ,
    E_TSP_TSIF_INVALID,

} TSP_TSIF;

typedef enum _TSP_FLOW // for Dst type
{
    E_DRV_TSP_FLOW_PLAYBACK0,
    E_DRV_TSP_FLOW_PLAYBACK1,
    E_DRV_TSP_FLOW_PVR0,
    E_DRV_TSP_FLOW_PVR1,
    E_DRV_TSP_FLOW_PVR2,
    E_DRV_TSP_FLOW_PVR3,
    E_DRV_TSP_FLOW_PVRCB,
    E_DRV_TSP_FLOW_RASP0,
    E_DRV_TSP_FLOW_RASP1,
    E_DRV_TSP_FLOW_TSO0,
    E_DRV_TSP_FLOW_TSO1,
    E_DRV_TSP_FLOW_PLAYBACK2,
    E_DRV_TSP_FLOW_PLAYBACK3,
    E_DRV_TSP_FLOW_INVALID,
} DRV_TSP_FLOW;

typedef enum // merge this with drvtsp2.h->TSP_TSIF
{
    E_DRVTSP_IF_TSIF0               =   0x0,
    E_DRVTSP_IF_TSIF1               =   0x1,
    E_DRVTSP_IF_TSIF2               =   0x2,
    E_DRVTSP_IF_TSIF3               =   0x3,
    E_DRVTSP_IF_TSIF0_FILE          =   0x4,
    E_DRVTSP_IF_TSIF2_FILE          =   0x5,
    E_DRVTSP_IF_NUM,
} DrvTSP_If;

//----------------------------------
/// DMX debug table information structure
//----------------------------------
typedef enum
{
    E_DRVTSP_DEBUG_CMD_NONE,
    E_DRVTSP_DEBUG_CMD_CLEAR,
    E_DRVTSP_DEBUG_CMD_ENABLE,
    E_DRVTSP_DEBUG_CMD_DISABLE,

} DrvTSP_Debug_Cmd;

typedef enum
{
    E_DRVTSP_FLOW_LIVE0,
    E_DRVTSP_FLOW_LIVE1,
    E_DRVTSP_FLOW_LIVE2,
    E_DRVTSP_FLOW_LIVE3,
    E_DRVTSP_FLOW_FILE0,
    E_DRVTSP_FLOW_FILE1,
    E_DRVTSP_FLOW_FILE2,
    E_DRVTSP_FLOW_FILE3,
    E_DRVTSP_FLOW_MMFI0,
    E_DRVTSP_FLOW_MMFI1,

    E_DRVTSP_FLOW_INVALID,

} DrvTSP_Flow;

typedef enum
{
    E_DRVTSP_AVFIFO_VIDEO,
    E_DRVTSP_AVFIFO_AUDIO,
    E_DRVTSP_AVFIFO_VIDEO3D,
    E_DRVTSP_AVFIFO_AUDIOB,
    E_DRVTSP_AVFIFO_AUDIOC,
    E_DRVTSP_AVFIFO_AUDIOD,

    E_DRVTSP_AVFIFO_INVALID,

} DrvTSP_AVFIFO;

// @TODO : add flag number define in each chip hal
#ifndef TSP_MAX_PVRSIGFLAG_NUM
#define TSP_MAX_PVRSIGFLAG_NUM 6
#endif
#ifndef TSP_MAX_SIGFLAG_NUM
#define TSP_MAX_SIGFLAG_NUM 10
#endif

typedef struct DLL_PACKED
{
    MS_U32                 u32Eng;
    MS_U32                 u32EvtWaitOpt;
    MS_U32                 u32DmxEvt;
    MS_U32                 u32TspEvt;

    MS_U32                 u32PvrBufFullFlag[TSP_MAX_PVRSIGFLAG_NUM];

    MS_U32                 u32SecRdyFlag[TSP_MAX_SIGFLAG_NUM];
    MS_U32                 u32SecOvfFlag[TSP_MAX_SIGFLAG_NUM];
    MS_U32                 u32SecCrcErrFlag[TSP_MAX_SIGFLAG_NUM];
}DrvTSP_IoSignal;


// @TODO: ?
#define TSP_FltType                     MS_U32
/// TS stream fifo type
#define E_TSP_FLT_FIFO_MASK             0x000000FF
#define E_TSP_FLT_FIFO_INVALID          0x00000000                                                  ///<\n
#define E_TSP_FLT_FIFO_VIDEO            0x00000001                                                  ///<\n
#define E_TSP_FLT_FIFO_AUDIO            0x00000002                                                  ///<\n
#define E_TSP_FLT_FIFO_AUDIO2           0x00000003                                                  ///<\n
#define E_TSP_FLT_FIFO_VIDEO3D          0x00000004
#define E_TSP_FLT_FIFO_REC              0x00000005
#define E_TSP_FLT_FIFO_PVR0             0x00000006
#define E_TSP_FLT_FIFO_PVR              E_TSP_FLT_FIFO_PVR0
#define E_TSP_FLT_FIFO_PVR1             0x00000007
#define E_TSP_FLT_FIFO_PVR2             0x00000008
#define E_TSP_FLT_FIFO_PVR3             0x00000009
#define E_TSP_FLT_FIFO_PVR4             0x0000000A
#define E_TSP_FLT_FIFO_PVR5             0x0000000B
#define E_TSP_FLT_FIFO_AUDIO3           0x0000000C
#define E_TSP_FLT_FIFO_AUDIO4           0x0000000D

#define E_TSP_RASP0                     0x0
#define E_TSP_RASP1                     0x1

/// Section user filter type (Exclusive Usage)
#define E_TSP_FLT_USER_MASK             0x00000F00                                                  ///<\n
#define E_TSP_FLT_USER_SHFT             8                                                           ///<\n
#define E_TSP_FLT_USER_NULL             0x00000000
#define E_TSP_FLT_USER_SEC              0x00000100                                                  ///<\n
#define E_TSP_FLT_USER_PES              0x00000200                                                  ///<\n
#define E_TSP_FLT_USER_PKT              0x00000300                                                  //[Reserved]
#define E_TSP_FLT_USER_PCR              0x00000400                                                  ///<\n
#define E_TSP_FLT_USER_TTX              0x00000500
#define E_TSP_FLT_USER_EMM              0x00000600                                                  //NDS
#define E_TSP_FLT_USER_ECM              0x00000700                                                  //NDS
#define E_TSP_FLT_USER_OAD              0x00000800                                                  //[Reserved]


#define E_TSP_FLT_FLAG_MASK             0x0000F000
#define E_TSP_FLT_FLAG_CA               0x00001000

/// TSIF Source (Exclusive Usage)
#define E_TSP_FLT_SRC_MASK              0xF00F0000
#define E_TSP_FLT_SRC_LIVE0             0x80000000
#define E_TSP_FLT_SRC_LIVE1             0x40000000
#define E_TSP_FLT_SRC_LIVE2             0x20000000
#define E_TSP_FLT_SRC_LIVE3             0x10000000
#define E_TSP_FLT_SRC_FILE0             0x00010000
#define E_TSP_FLT_SRC_FILE1             0x00020000
#define E_TSP_FLT_SRC_FILE2             0x00040000
#define E_TSP_FLT_SRC_FILE3             0x00080000

/// Source ID define.  For merge stream
#define E_TSP_FLT_SRCID_MASK            0x00F00000
#define E_TSP_FLT_SRCID_SHIFT           20
#define E_TSP_FLT_SRCID_0               0x00000000
#define E_TSP_FLT_SRCID_1               0x00100000
#define E_TSP_FLT_SRCID_2               0x00200000
#define E_TSP_FLT_SRCID_3               0x00300000
#define E_TSP_FLT_SRCID_4               0x00400000
#define E_TSP_FLT_SRCID_5               0x00500000
#define E_TSP_FLT_SRCID_6               0x00600000
#define E_TSP_FLT_SRCID_7               0x00700000
#define E_TSP_FLT_SRCID_8               0x00800000
#define E_TSP_FLT_SRCID_9               0x00900000
#define E_TSP_FLT_SRCID_10              0x00a00000
#define E_TSP_FLT_SRCID_11              0x00b00000
#define E_TSP_FLT_SRCID_12              0x00c00000
#define E_TSP_FLT_SRCID_13              0x00d00000
#define E_TSP_FLT_SRCID_14              0x00e00000
#define E_TSP_FLT_SRCID_15              0x00f00000


#define TSP_FltMode                     MS_U32
#define E_TSP_FLT_MODE_CONTI            0x00000000
#define E_TSP_FLT_MODE_ONESHOT          0x00000001

// optional
#define E_TSP_FLT_MODE_CRCCHK           0x00000002
//#define E_TSP_FLT_MODE_AUTO_ADDR        0x00000004 // auto move read point mode
#define E_TSP_FLT_MODE_AUTO_CRCCHK      0x00000008 //not really for setting, mapped to TSP_SECFLT_MODE_AUTO_CRCCHK


/// TSP channel state bit flags
typedef enum //_DrvTSP_FltState // @FIXME we current use this enum with == not &. doesn't have to be exclusive use
{
    E_TSP_FLT_STATE_FREE            =   0x00000000,                                                 ///<\n
    E_TSP_FLT_STATE_ALLOC           =   0x00000001,                                                 ///<\n
    E_TSP_FLT_STATE_ENABLE          =   0x00000002,                                                 ///<\n
    E_TSP_FLT_STATE_SCRAMBLED       =   0x00000004,                                                 //[TODO]
    E_TSP_FLT_STATE_STREAM_AVAIL    =   0x00000008,                                                 //[TODO]
    E_TSP_FLT_STATE_SCRAMBLED_ORG   =   0x00000010,
    E_TSP_FLT_STATE_ISR_FREE        =   0x00000020,                                                 ///<\n
    E_TSP_FLT_STATE_OVERFLOW        =   0x00010000,                                                 //[Reserved]
    E_TSP_FLT_STATE_NA              =   0xFFFFFFFF,
} TSP_FltState;


/// TSP record mode
typedef enum //_DrvTSP_RecMode
{
    //[TODO] rename REC_MODE to PVR_MODE

    /// Record ENG0 by @ref E_DRVTSP_FLT_TYPE_PVR
    E_TSP_REC_MODE_ENG0_FLTTYPE     =   0x00000000,                                                 // TSP_PVR_CTRL_ENG(0)
    /// Record ENG1 by @ref E_DRVTSP_FLT_TYPE_PVR
    E_TSP_REC_MODE_ENG1_FLTTYPE     =   0x00000001,                                                 // TSP_PVR_CTRL_ENG(1)
    /// Record ENG0 bypass PID fliter
    E_TSP_REC_MODE_ENG0_BYPASS      =   0x00000002,                                                 // TSP_PVR_CTRL_ENG(0) + TSP_PVR_CTRL_BYPASS
    /// Record ENG1 bypass PID fliter
    E_TSP_REC_MODE_ENG1_BYPASS      =   0x00000003,                                                 // TSP_PVR_CTRL_ENG(1) + TSP_PVR_CTRL_BYPASS

} TSP_RecMode;


/// TSP Control Mode
typedef enum //_DrvTSP_CtrlMode
{
    /// Input From Stream Source 0
    E_TSP_CTRL_MODE_TS0,
    /// Input From Stream Source 1
    E_TSP_CTRL_MODE_TS1,
    /// Input From Memory
    E_TSP_CTRL_MODE_MEM,
} TSP_CtrlMode;


typedef enum
{
    /// Streaming to TSP
    E_TSP_FILE_2_TSP                =   0x00000000,                                                 //TSP_TSDMA_CTRL_ENG0
    /// Streaming to VIDEO FIFO
    E_TSP_FILE_2_VIDEO              =   0x00000004,                                                 //TSP_TSDMA_CTRL_VPES0
    /// Streaming to AUDIO FIFO
    E_TSP_FILE_2_AUDIO              =   0x00000008,                                                 //TSP_TSDMA_CTRL_APES0
    /// Streaming to AUDIO2 FIFO
    E_TSP_FILE_2_AUDIO2             =   0x00000010,                                                 //TSP_TSDMA_CTRL_A2PES0
    /// Streaming to VIDEO3D FIFO
    E_TSP_FILE_2_VIDEO3D            =   0x00000020,                                                 //TSP_TSDMA_CTRL_V3DPES0
    /// Streaming to AUDIO3 FIFO
    E_TSP_FILE_2_AUDIO3             =   0x00000040,                                                 //TSP_TSDMA_CTRL_A3PES0
    /// Streaming to AUDIO4 FIFO
    E_TSP_FILE_2_AUDIO4             =   0x00000080,                                                 //TSP_TSDMA_CTRL_A4PES0

} TSP_FileMode;


typedef enum _TSP_FileinState
{
    /// Command Queue is Idle
    E_TSP_FILE_STATE_IDLE           =   0000000000,
    /// Command Queue is Busy
    E_TSP_FILE_STATE_BUSY           =   0x00000001,
    /// Command Queue is Paused.
    E_TSP_FILE_STATE_PAUSE          =   0x00000002,
} TSP_FileState;


#define TSP_Event                       MS_U32
#define E_TSP_EVENT_DATA_IDLE           0x00000000
/// Section Data Ready
#define E_TSP_EVENT_DATA_READY          0x00000001
/// Section data CRC Error
#define E_TSP_EVENT_SEC_CRCERROR        0x00000004
/// Section Buffer Overflow
#define E_TSP_EVENT_BUF_OVERFLOW        0x00000002
/// PVR Buffer is Full
#define E_TSP_EVENT_PVRBUF_FULL         0x00000010
/// PVR Double Buffer Overflow
#define E_TSP_EVENT_PVRBUF_OVERFLOW     0x00000020

/// TSP self task callback // optional --> default is CB by poll
#define E_TSP_EVENT_CB_MASK             0x80000000
#define E_TSP_EVENT_CB_POLL             0x00000000
#define E_TSP_EVENT_CB_AUTO             0x80000000

/// TSP file in Engine
typedef enum
{
    E_TSP_FILE_ENG_TSIF0 = 0,
    E_TSP_FILE_ENG_TSIF1 = 1,
    E_TSP_FILE_ENG_TSIF2 = 2,
    E_TSP_FILE_ENG_TSIF3 = 3,
    E_TSP_FILE_ENG_INVALID =4,
} TSP_FILE_ENG;

/// TSP file in Packet mode
typedef enum
{
    E_TSP_PKTMODE_188               =   0x00000000,
    E_TSP_PKTMODE_192               =   0x00000001,
    E_TSP_PKTMODE_204               =   0x00000002,
    E_TSP_PKTMODE_130               =   0x00000003,     // RVU
    E_TSP_PKTMODE_134               =   0x00000004,     // RVU with timestamp
    E_TSP_PKTMODE_FILEIN_MIN        =   E_TSP_PKTMODE_188,
    E_TSP_PKTMODE_FILEIN_MAX        =   E_TSP_PKTMODE_134,

    E_TSP_PKTMODE_CIPLUS            =   0x00000005,     // CI+ (188 bytes)
    E_TSP_PKTMODE_ATS               =   0x00000006,     // ATS (192 bytes)
    E_TSP_PKTMODE_OPENCABLE         =   0x00000007,     // OpenCable (200 bytes)

    E_TSP_PKTMODE_MXL192            =   0x00000008,     // MxL (192 bytes)
    E_TSP_PKTMODE_MXL196            =   0x00000009,     // MxL (196 bytes)
    E_TSP_PKTMODE_MXL200            =   0x0000000A,     // MxL (200 bytes)
    E_TSP_PKTMODE_LIVEIN_MIN        =   E_TSP_PKTMODE_CIPLUS,
    E_TSP_PKTMODE_LIVEIN_MAX        =   E_TSP_PKTMODE_MXL200

} TSP_PktMode;

/// TSP notification event message
typedef struct _TSP_EventMsg
{
    /// Union data type of message
    union
    {
        /// FltInfo message
        ///   - Byte[0] : Section filter id
        ///   - Byte[1] : TSP id
        MS_U32                          FltInfo;
        /// PvrBufId
        ///   - Byte[0] : PVR buffer id
        MS_U32                          PvrBufId;
    };

} TSP_EventMsg;

typedef enum
{
    E_TSP_DBG_L0                    =   0,
    E_TSP_DBG_L1                    =   1,                                                          // display error message
    E_TSP_DBG_L3                    =   3,                                                          // display warning message
    E_TSP_DBG_L5                    =   5,                                                          // display more info and while(1)
    E_TSP_DBG_L7                    =   7,                                                          // display info, callee, while(1)

    E_TSP_DBG_ALL,

    E_TSP_DBG_ERROR                 =   E_TSP_DBG_L0,
    E_TSP_DBG_WARNING               =   E_TSP_DBG_L1,
    E_TSP_DBG_FAIL                  =   E_TSP_DBG_L3,
    E_TSP_DBG_INFO                  =   E_TSP_DBG_L5,
    E_TSP_DBG_TRACK                 =   E_TSP_DBG_L7,

} TSP_DbgLevel;

/// @name TSP_EventMsg
/// Macro definitions for manipulating TSP_EventMsg
/// @{
#define TSP_MSG_FLTID_MASK              0x000000FF
#define TSP_MSG_FLTID_SHFT              0
#define TSP_MSG_GETFLTID(msg)           ((msg) & TSP_MSG_FLTID_MASK)
#define TSP_MSG_SETFLTID(msg, fltid)    (((msg) & ~TSP_MSG_FLTID_MASK)) | ((fltid) << TSP_MSG_FLTID_SHFT))
#define TSP_MSG_ENGID_MASK              0x0000FF00
#define TSP_MSG_ENGID_SHFT              8
#define TSP_MSG_PVRID_MASK              0x000000FF
#define TSP_MSG_PVRID_SHFT              0
#define TSP_MSG_PVRID_NULL              0xFF
/// @}

typedef enum
{
    E_TSP_CAP_PIDFLT_NUM                    = 0,
    E_TSP_CAP_SECFLT_NUM                    = 1,
    E_TSP_CAP_SECBUF_NUM                    = 2,

    E_TSP_CAP_RECENG_NUM                    = 3,
    E_TSP_CAP_RECFLT_NUM                    = 4,
    E_TSP_CAP_RECFLT1_NUM                   = 5,

    E_TSP_CAP_MMFI_AUDIO_FILTER_NUM         = 6,
    E_TSP_CAP_MMFI_V3D_FILTER_NUM           = 7,

    E_TSP_CAP_TSIF_NUM                      = 8,
    E_TSP_CAP_DEMOD_NUM                     = 9,
    E_TSP_CAP_TSPAD_NUM                     = 10,
    E_TSP_CAP_VQ_NUM                        = 11,

    E_TSP_CAP_CAFLT_NUM                     = 12,
    E_TSP_CAP_CAKEY_NUM                     = 13,

    E_TSP_CAP_FW_ALIGN                      = 14,
    E_TSP_CAP_VQ_ALIGN                      = 15,
    E_TSP_CAP_VQ_PITCH                      = 16,
    E_TSP_CAP_SECBUF_ALIGN                  = 17,
    E_TSP_CAP_PVR_ALIGN                     = 18,

    E_TSP_CAP_PVRCA_PATH_NUM                = 19,
    E_TSP_CAP_SHAREKEY_FLT_RANGE            = 20,
    E_TSP_CAP_PVRCA0_FLT_RANGE              = 21,
    E_TSP_CAP_PVRCA1_FLT_RANGE              = 22,
    E_TSP_CAP_PVRCA2_FLT_RANGE              = 23,
    E_TSP_CAP_SHAREKEY_FLT1_RANGE           = 24,
    E_TSP_CAP_SHAREKEY_FLT2_RANGE           = 25,

    E_TSP_CAP_HW_TYPE                       = 26,

    E_TSP_CAP_RESOURCE_SIZE                 = 27,       // Get the data structure size of private resource (share resource)

    E_TSP_CAP_VFIFO_NUM                     = 28,
    E_TSP_CAP_AFIFO_NUM                     = 29,

    E_TSP_CAP_HWPCR_SUPPORT                 = 30,
    E_TSP_CAP_PCRFLT_START_IDX              = 31,

    E_TSP_CAP_HWWP_SET_NUM                  = 32,       // Get TSP write protect set numbers

    E_TSP_CAP_CAP_DSCMB_ENG_NUM             = 33,       // Get DSCMB engine numbers

    E_TSP_CAP_MAX_MERGESTR_NUM              = 34,       // Get Maxumum merge stream number
    E_TSP_CAP_MAX_SEC_FLT_DEPTH             = 35,
    E_TSP_CAP_FW_BUF_SIZE                   = 36,
    E_TSP_CAP_FW_BUF_RANGE                  = 37,
    E_TSP_CAP_VQ_BUF_RANGE                  = 38,
    E_TSP_CAP_SEC_BUF_RANGE                 = 39,
    E_TSP_CAP_FIQ_NUM                       = 40,
    E_TSP_CAP_RECFLT_IDX                    = 41,       // [NOTE] internal usage
    E_TSP_CAP_NULL,
} TSP_Caps;



typedef enum
{
    E_TSP_HW_STATUS                 =   0,
    E_TSP_HW_INIT,
    E_TSP_HW_ALIVE,
    E_TSP_HW_DEALIVE,
} TSP_HW_Status;

typedef enum
{
    E_TSP_SW_INIT                   =   0,
    E_TSP_SW_INIT_FAIL,
    E_TSP_SW_SUSPEND,
    E_TSP_SW_RESUME,
    E_TSP_SW_POWEROFF,
    E_TSP_SW_CLOSE,
    E_TSP_SW_EXIT,
} TSP_SW_Status;


typedef enum
{
    E_TSP_DRV_FILE_SOURCE_FILE,
    E_TSP_DRV_FILE_SOURCE_FILE1,
    E_TSP_DRV_FILE_SOURCE_FILE2,
    E_TSP_DRV_FILE_SOURCE_FILE3,
} TSP_DRV_FILTER_FILE_SOURCE;

typedef enum
{
    E_TSP_DRV_PVR_RecMode_PID,
    E_TSP_DRV_PVR_RecMode_ALL,
} TSP_DRV_PVR_RecMode;


typedef struct
{
    TSP_TSPad                           ePad;
    MS_BOOL                             bClkInv;
    MS_BOOL                             bExtSync;
    MS_BOOL                             bParallel;
} DrvTSP_TsIf_Set;

typedef enum
{
    E_TSP_PIDFLT_FIFO_FULL_LV_100                 =   0x00000000,
    E_TSP_PIDFLT_FIFO_FULL_LV_50                   =   0x60000000
} TSP_FltFIFOFullLV;

/// Cmd for DSCMB to do DSCMB functionality accessing registers which are shared with TSP
typedef enum//DSCMB owner plz add enum if needed
{
    E_TSP_DSCMB_CMD_NUM
} TSP_DSCMB_FuncCMD;


/// TSP TEI  Remove Error Packet Infomation
typedef enum
{
    E_TSP_DRV_TEI_REMOVE_AUDIO_PKT,          ///< DMX TEI Remoce Audio Packet
    E_TSP_DRV_TEI_REMOVE_VIDEO_PKT,          ///< DMX TEI Remoce Video Packet

    E_TSP_DRV_TEI_REMOVE_PKT0_LIVE,
    E_TSP_DRV_TEI_REMOVE_PKT0_FILE,
    E_TSP_DRV_TEI_REMOVE_PKT1,
    E_TSP_DRV_TEI_REMOVE_PKT2,

}TSP_DRV_TEI_RmPktType;


/// TSP notification function
typedef void (*P_TSP_Callback)(TSP_Event eEvent, TSP_EventMsg *pMsg);

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

// TSP Setting DDIs
TSP_Result MDrv_TSP_InitLibResource(void *pu32ResMemAddr);
TSP_Result MDrv_TSP_Init(TSP_InitParam *pParam);
TSP_Result MDrv_TSP_PowerOff(void);
TSP_Result MDrv_TSP_Exit(void);

TSP_TSIF   MDrv_TSP_TsifMapping(TSP_TSIF eTSIF);
TSP_Result MDrv_TSP_SetFlowSource(DRV_TSP_FLOW eDst, TSP_TSId eTid);
TSP_Result MDrv_TSP_PVR_SetFlowSource(MS_U32 u32PvrEng, MS_U32 u32PvrSrc);
TSP_Result MDrv_TSP_SelPad(TSP_TSIF eTSIF, TSP_TSPad eTSPad);
TSP_Result MDrv_TSP_SelPad_ClkInv(TSP_TSIF eTSIF ,MS_BOOL bClkInv);
TSP_Result MDrv_TSP_SelPad_ClkDis(TSP_TSIF eTSIF ,MS_BOOL bClkDis);
TSP_Result MDrv_TSP_OutputPadCfg(TSP_TSPad eOutTSPad, TSP_TSPad eInTSPad, MS_BOOL bInParallel, MS_BOOL bEnable);
TSP_Result MDrv_TSP_TSO_ConfigPad(MS_U32 u32TSOEng, TSP_TSPad eTSPad);
TSP_Result MDrv_TSP_SetTSIF(TSP_TSIF eTSIF, TSP_TSIFCfg u16Cfg, MS_BOOL bFileIn);
TSP_Result MDrv_TSP_FIFO_Reset(TSP_FltType eFltType, MS_BOOL bFlush);
TSP_Result MDrv_TSP_FIFO_BlockEnable(TSP_FltType eFltType, MS_BOOL bDisable);
TSP_Result MDrv_TSP_FIFO_GetStatus(TSP_FltType eFltType, MS_U32 *u32FifoLevel);
TSP_Result MDrv_TSP_FIFO_SourceSelect(TSP_TSId eTspTSid, TSP_FileMode eFifoType);
TSP_Result MDrv_TSP_FIFO_Overflow_Status(TSP_FltType eFltType, MS_BOOL *pbOverflow);
//TSP_Result MDrv_TSP_Scmb_Status(MS_U32 u32EngId, TSP_Scmb_Level* pScmbLevel);
TSP_Result MDrv_TSP_AU_BD_Mode_Enable(MS_BOOL bEnable);
TSP_Result MDrv_TSP_GetTSIFStatus(TSP_TSIF eTSIF, TSP_TSPad* ePad, MS_BOOL* pbClkInv, MS_BOOL* pbExtSyc, MS_BOOL* pbParl);
TSP_Result MDrv_TSP_PARL_BitOrderSwap(MS_U16 u16TSIF, MS_BOOL bInvert);


TSP_Result MDrv_TSP_GetLastErr(void);
TSP_Result MDrv_TSP_CheckInitialClockStatus(void);
TSP_Result MDrv_TSP_FILE_CmdSTC(MS_U32 u32TSPId, MS_U32 u32STC_H, MS_U32 u32STC);
TSP_Result MDrv_TSP_GetPCR(MS_U32 u32TSPId, MS_U32 *pu32PCR_32, MS_U32 *pu32PCR);
TSP_Result MDrv_TSP_FLT_GetState(MS_U32 u32FltId, TSP_FltState *pState);

TSP_Result MDrv_TSP_Proc(MS_U32 u32TSPId, MS_U32 u32FltId, TSP_Event* pEvt);

// TSP FLT DDis
TSP_Result MDrv_TSP_FLT_Alloc(MS_U32 u32TSPId, TSP_FltType eFltType, MS_U32 *pu32FltId);
TSP_Result MDrv_TSP_FLT_Free(MS_U32 u32FltId);
TSP_Result MDrv_TSP_FLT_SetPID(MS_U32 u32FltId, MS_U32 u32PID);
TSP_Result MDrv_TSP_FLT_GetPID(MS_U32 u32FltId, MS_U32 *pu32PID);                                   ///RESERVED
TSP_Result MDrv_TSP_FLT_SelSEC(MS_U32 u32FltId, MS_U32 u32BufId);
TSP_Result MDrv_TSP_FLT_GetSEC(MS_U32 u32FltId, MS_U32 *pu32BufId);                                 ///RESERVED
TSP_Result MDrv_TSP_FLT_SetCallback(MS_U32 u32FltId, TSP_Event eEvents, P_TSP_Callback pfCallback);
TSP_Result MDrv_TSP_FLT_SetMode(MS_U32 u32FltId, TSP_FltMode eFltMode);
TSP_Result MDrv_TSP_FLT_SetMatch(MS_U32 u32FltId, MS_U8 *pu8Match, MS_U8 *pu8Mask, MS_U8 *pu8NMask);
TSP_Result MDrv_TSP_FLT_Enable(MS_U32 u32FltId, MS_BOOL bEnable);
TSP_Result MDrv_TSP_FLT_SetPCRSrc(MS_U32 u32FltId,MS_U32 u32PCRSrc);
TSP_Result MDrv_TSP_FLT_SetFltRushPass(MS_U32 u32FltId, MS_U8 u8Enable);
TSP_Result MDrv_TSP_Flt_GetScmbSts(TSP_FltType eFltType, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts);
TSP_Result MDrv_TSP_DropScmbPkt(TSP_FltType eFilterType, MS_BOOL bEnable);


// TSP SEC DDIs
TSP_Result MDrv_TSP_SEC_Alloc(MS_U32 u32TSPId, MS_U32 *pu32BufId);
TSP_Result MDrv_TSP_SEC_Free(MS_U32 u32BufId);
TSP_Result MDrv_TSP_SEC_SetBuffer(MS_U32 u32BufId, MS_PHYADDR u32Start, MS_U32 u32Size);
TSP_Result MDrv_TSP_SEC_GetSize(MS_U32 u32BufId, MS_U32 *pu32Size);
TSP_Result MDrv_TSP_SEC_GetStart(MS_U32 u32BufId, MS_PHY *pStart);
TSP_Result MDrv_TSP_SEC_SetRead(MS_U32 u32BufId, MS_PHY u32Read);
TSP_Result MDrv_TSP_SEC_GetRead(MS_U32 u32BufId, MS_PHY *pu32Read);
TSP_Result MDrv_TSP_SEC_GetWrite(MS_U32 u32BufId, MS_PHY *pu32Write);
TSP_Result MDrv_TSP_SEC_Reset(MS_U32 u32BufId);
TSP_Result MDrv_TSP_SECFLT_BurstLen(MS_U32 u32BurstMode);
#ifdef MSOS_TYPE_LINUX_KERNEL
TSP_Result MDrv_TSP_SEC_ClrEvent(MS_U32 u32BufId);
TSP_Result MDrv_TSP_SEC_GetEvent(MS_U32 u32BufId, MS_U32 *pu32Event);
TSP_Result MDrv_TSP_SEC_SetEvent(MS_U32 u32BufId, MS_U32 u32Event);
#endif

// TSP FILE DDIs
TSP_Result MDrv_TSP_FILE_SetPktMode(TSP_PktMode mode);
TSP_Result MDrv_TSP_FILE_SetRate(MS_U32 u32Div2);
TSP_Result MDrv_TSP_FILE_CmdAddr(MS_U32 u32Addr);
TSP_Result MDrv_TSP_FILE_CmdSize(MS_U32 u32Size);
TSP_Result MDrv_TSP_FILE_CmdSTC(MS_U32 u32EngId, MS_U32 u32STC_H, MS_U32 u32STC);
TSP_Result MDrv_TSP_FILE_CmdStart(TSP_FileMode mode);
TSP_Result MDrv_TSP_FILE_GetCmdSlot(MS_U32 *pu32EmptySlot);
TSP_Result MDrv_TSP_FILE_Pause(void);
TSP_Result MDrv_TSP_FILE_Resume(void);
TSP_Result MDrv_TSP_FILE_Stop(void);
TSP_Result MDrv_TSP_FILE_GetState(TSP_FileState *pState);
TSP_Result MDrv_TSP_FILE_Reset(void);
TSP_Result MDrv_TSP_File_192BlockScheme_En(MS_BOOL bEnable);
TSP_Result MDrv_TSP_FILE_PS_Path_Enable(TSP_FileMode mode);

// TSP FILE Eng
MS_U32     MDrv_TSP_FILE_Eng_TsifMapping(TSP_FILE_ENG Eng);
TSP_Result MDrv_TSP_FILE_Eng_SetPktMode(TSP_FILE_ENG Eng, TSP_PktMode mode);
TSP_Result MDrv_TSP_FILE_Eng_SetRate(TSP_FILE_ENG Eng, MS_U32 u32Div2);
TSP_Result MDrv_TSP_FILE_Eng_CmdAddr(TSP_FILE_ENG Eng, MS_U32 u32Addr);
TSP_Result MDrv_TSP_FILE_Eng_CmdSize(TSP_FILE_ENG Eng, MS_U32 u32Size);
TSP_Result MDrv_TSP_FILE_Eng_CmdSTC(TSP_FILE_ENG Eng, MS_U32 u32EngId, MS_U32 u32STC_H, MS_U32 u32STC);
TSP_Result MDrv_TSP_FILE_Eng_CmdStart(TSP_FILE_ENG Eng, TSP_FileMode mode);
TSP_Result MDrv_TSP_FILE_Eng_GetCmdSlot(TSP_FILE_ENG Eng, MS_U32 *pu32EmptySlot);
TSP_Result MDrv_TSP_FILE_Eng_Pause(TSP_FILE_ENG Eng);
TSP_Result MDrv_TSP_FILE_Eng_Resume(TSP_FILE_ENG Eng);
TSP_Result MDrv_TSP_FILE_Eng_Stop(TSP_FILE_ENG Eng);
TSP_Result MDrv_TSP_FILE_Eng_GetState(TSP_FILE_ENG Eng, TSP_FileState *pState);
TSP_Result MDrv_TSP_FILE_Eng_Reset(TSP_FILE_ENG Eng);
TSP_Result MDrv_TSP_FILE_Eng_192BlockScheme_En(TSP_FILE_ENG Eng, MS_BOOL bEnable);
TSP_Result MDrv_TSP_FILE_Eng_PS_Path_Enable(TSP_FILE_ENG Eng, TSP_FileMode mode);
TSP_Result MDrv_TSP_FILE_Eng_CmdQFifo_Status(TSP_FILE_ENG Eng, MS_U8 *pu8FifoLevel);
TSP_Result MDrv_TSP_FILE_Eng_GetFileInTimeStamp(TSP_FILE_ENG Eng, MS_U32* u32TSLen);
TSP_Result MDrv_TSP_FILE_Eng_TimeStampEnablePlaybackStamp(TSP_FILE_ENG Eng,MS_BOOL bEnable);
TSP_Result MDrv_TSP_FILE_Eng_TimeStampSetPlaybackStamp(TSP_FILE_ENG Eng, MS_U32 u32Stamp);
TSP_Result MDrv_TSP_FILE_Eng_TimeStampGetPlaybackStamp(TSP_FILE_ENG Eng, MS_U32* u32Stamp);
TSP_Result MDrv_TSP_FILE_Eng_GetFileInCurReadAddr(TSP_FILE_ENG Eng, MS_PHY* pu32Addr);
TSP_Result MDrv_TSP_FILE_Eng_MOBF_Enable(TSP_FILE_ENG eEng, MS_BOOL bEnable, MS_U32 u32Key);

// Merge stream
TSP_Result MDrv_TSP_MStr_SyncByte(DrvTSP_If eTsIf, MS_U8 u8StrId, MS_U8 *pu8SyncByte, MS_BOOL bSet);
TSP_Result MDrv_TSP_SetPacketMode(TSP_TSIF u32TSIf, TSP_PktMode eDrvPktMode);

// Debug table
TSP_Result MDrv_TSP_Get_DisContiCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_AVFIFO eAvType, DrvTSP_Flow eFlow, MS_U32* pu32Cnt);
TSP_Result MDrv_TSP_Get_DropPktCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_AVFIFO eAvType, DrvTSP_Flow eFlow, MS_U32* pu32Cnt);
TSP_Result MDrv_TSP_Get_LockPktCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_Flow eFlow, MS_U32* pu32Cnt);
TSP_Result MDrv_TSP_Get_AVPktCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_AVFIFO eAvType, DrvTSP_Flow eFlow, MS_U32* pu32Cnt);
// @TODO ErrPktCnt Implement
//TSP_Result MDrv_TSP_Get_ErrPktCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_Flow eFlow, MS_U32* pu32Cnt);

// Protection range
TSP_Result MDrv_TSP_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY *pphyStartAddr, MS_PHY *pphyEndAddr);
TSP_Result MDrv_TSP_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr);

// Utopia 2K relative functions
TSP_Result MDrv_TSP_Wait_IoSignal(DrvTSP_IoSignal *pstIoSignal);
TSP_Result MDrv_TSP_Proc_IoSignal(DrvTSP_IoSignal *pstIoSignal);

// @TODO
TSP_Result MDrv_TSP_PVR_Select(TSP_FltType eFltType);
TSP_Result MDrv_TSP_PVR_FltAlloc(TSP_FltType eFltType, MS_U32 *pu32FltId);
TSP_Result MDrv_TSP_PVR_FltFree(MS_U32 u32FltId);
TSP_Result MDrv_TSP_PVR_FltSetPID(MS_U32 u32FltId, MS_U32 u32PID);
TSP_Result MDrv_TSP_PVR_SetBuffer(MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1);
TSP_Result MDrv_TSP_PVR_Start(TSP_RecMode eRecMode, MS_BOOL bStart);
TSP_Result MDrv_TSP_PVR_GetWriteAddr(MS_PHYADDR *pu32WriteAddr);
TSP_Result MDrv_TSP_PVR_Notify(TSP_Event eEvents, P_TSP_Callback pfCallback);
TSP_Result MDrv_TSP_PVR_SetPacketMode(MS_BOOL bSet);
TSP_Result MDrv_TSP_PVR_TimeStampSetRecordStamp(MS_U32 u32Stamp);
TSP_Result MDrv_TSP_PVR_TimeStampGetRecordStamp(MS_U32* u32Stamp);
TSP_Result MDrv_TSP_PVR_TimeStampSetPlaybackStamp(MS_U32 u32Stamp);
TSP_Result MDrv_TSP_PVR_TimeStampGetPlaybackStamp(MS_U32* u32Stamp);
TSP_Result MDrv_TSP_PVR_TimeStamp(MS_BOOL bEnable);

TSP_Result MDrv_TSP_PVR_Eng_FltAlloc(MS_U32 u32Eng, MS_U32* pu32FltId);
TSP_Result MDrv_TSP_PVR_Eng_FltFree(MS_U32 u32Eng, MS_U32 u32FltId);
TSP_Result MDrv_TSP_PVR_Eng_FltSetPID(MS_U32 u32Eng, MS_U32 u32FltId, MS_U32 u32PID);
TSP_Result MDrv_TSP_PVR_Eng_SetBuffer(MS_U32 u32Eng, MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1);
TSP_Result MDrv_TSP_PVR_Eng_Start(MS_U32 u32Eng, MS_BOOL bPvrAll, MS_BOOL bStart);
TSP_Result MDrv_TSP_PVR_Eng_Pause(MS_U32 u32Eng, MS_BOOL bPause);
TSP_Result MDrv_TSP_PVR_Eng_GetWriteAddr(MS_U32 u32Eng, MS_PHY *pu32WriteAddr);
TSP_Result MDrv_TSP_PVR_Eng_Notify(MS_U32 u32Eng, TSP_Event eEvents, P_TSP_Callback pfCallback);
TSP_Result MDrv_TSP_PVR_Eng_SetPacketMode(MS_U32 u32Eng, MS_BOOL bSet);
TSP_Result MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(MS_U32 u32Eng, MS_U32 u32Stamp);
TSP_Result MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(MS_U32 u32Eng, MS_U32* u32Stamp);
TSP_Result MDrv_TSP_PVR_Eng_TimeStamp(MS_U32 u32Eng, MS_BOOL bEnable);
TSP_Result MDrv_TSP_PVR_Eng_FltEnable(MS_U32 u32Eng, MS_U32 u32FltId, MS_BOOL bEnable);
TSP_Result MDrv_TSP_PVR_Eng_FltGetPID(MS_U32 u32Eng, MS_U32 u32FltId, MS_U32* u32PID);
TSP_Result MDrv_TSP_PVR_Eng_SetCaMode(MS_U32 u32EngId, MS_U16 u16CaMode, MS_BOOL bSpsEnable);
TSP_Result MDrv_TSP_PVR_Eng_MOBF_Enable(MS_U32 u32Eng, MS_BOOL bEnable, MS_U32 u32Key);

TSP_Result MDrv_TSP_GetFileInTimeStamp(MS_U32* u32TSLen);

//TSP_Result MDrv_TSP_File_SetPacketMode(TSP_PktMode PKT_Mode);

TSP_Result MDrv_TSP_DBG_GetDBGStatus(MS_U32 u32Sel, MS_U32* u32Status);

// TSP Info DDIs
TSP_Result MDrv_TSP_GetCaps(TSP_Caps eCap, MS_U32 *pu32CapInfo);
TSP_Result MDrv_TSP_GetStatus(TSP_HW_Status *HW_Status, TSP_SW_Status *SW_Status);
TSP_Result MDrv_TSP_GetLibVer(const MSIF_Version **ppVersion);
TSP_Result MDrv_TSP_SetDbgLevel(TSP_DbgLevel DbgLevel);
TSP_Result MDrv_TSP_GetFWVER(MS_U32 *u32FWVer);
TSP_Result MDrv_TSP_CmdQFifo_Status(MS_U8 *pu8FifoLevel);



// ----- Shold be OBSOLETED when U03 return ----
TSP_Result MDrv_TSP_FLT_DropEnable(MS_BOOL bSet);
//-------------------------------------------

TSP_Result MDrv_TSP_RemoveDupAVPkt(MS_BOOL bEnable);
TSP_Result MDrv_TSP_TEI_RemoveErrorPkt(TSP_DRV_TEI_RmPktType eDrvPktType, MS_BOOL bEnable);

TSP_Result MDrv_TSP_FLT_ChangeSource(MS_U32 u32FltId, TSP_FltType eTSPFltTypeSrc);
TSP_Result MDrv_TSP_FLT_LiveSrcSwitch(TSP_FltType TgtFlow);

TSP_Result MDrv_TSP_FLTFIFO_FullSel(TSP_FltFIFOFullLV eFIFOFullLV);

// ------ Common API for TSP mode setting ------
TSP_Result MDrv_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1, void *pData);

TSP_Result MDrv_TSP_DSCMB_Functionality(TSP_DSCMB_FuncCMD eCmd, MS_U32 u32Config0, MS_U32 u32Config1 ,void *pData);


//-------------------------------------------------------------------------------------------------
//  OBSOLETED
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SetOwner(MS_U32 u32EngId, MS_U32 u32FltIdStart, MS_U32 u32FltIdEnd, MS_BOOL bOwner) __attribute__ ((deprecated)); // Set/Get the ranges of filters used, which is for AEON/MHEG5 share TSP filters resource.
TSP_Result MDrv_TSP_FLT_ChkOwner(MS_U32 u32EngId, MS_U32 u32PidFltId) __attribute__ ((deprecated));
TSP_Result MDrv_TSP_SetHK(MS_BOOL bIsHK) __attribute__ ((deprecated)) __attribute__ ((deprecated));
TSP_Result MDrv_TSP_IsAccess(MS_U32 u32Try) __attribute__ ((deprecated));
TSP_Result MDrv_TSP_UnlockAccess(void) __attribute__ ((deprecated));

TSP_Result MDrv_TSP_TTX_SecFlt_GetWriteAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHYADDR *pu32WriteAddr) __attribute__ ((deprecated)); // special case for TTX
TSP_Result MDrv_TSP_GetDbgPortInfo(MS_U32 u32DbgSel,MS_U32* u32DbgInfo) __attribute__ ((deprecated));

TSP_Result MDrv_TSP_BurstLen(MS_U32 u32Len) __attribute__ ((deprecated));

#ifdef STC64_SUPPORT
TSP_Result MDrv_TSP_STC64_Mode_Enable(MS_BOOL bEnable);
TSP_Result MDrv_TSP_SetSTC64(MS_U32 u32EngId, MS_U32 u32STC_32, MS_U32 u32STC);
TSP_Result MDrv_TSP_GetSTC64(MS_U32 u32EngId, MS_U32 *pu32STC_32, MS_U32 *pu32STC);
#endif

TSP_Result MDrv_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_U8 u8Mode);
TSP_Result MDrv_TSP_SetSTCOffset(MS_U32 u32EngId, MS_U32 u32Offset, MS_BOOL bAdd);

TSP_Result MDrv_TSP_STC_Alloc(MS_U32 *pu32EngId);
TSP_Result MDrv_TSP_STC_Free(MS_U32 u32EngId);
TSP_Result MDrv_TSP_PcrId_To_StcId(MS_U32 u32PcrFltId,MS_U32 *pu32EngId);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif


#endif // __DRV_TSP2_H__

