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
/// file   drvRASP.h
/// @brief  RASP Driver Interface
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVRASP_H_
#define _DRVRASP_H_

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define RASP_PIDFLT_START_NUM        0
#define RASP_PIDFLT_END_NUM          24

#define RASP_PIDFLT_NUM_ALL          24

#ifdef HW_PVR_ENABLE
    #define PVR_PIDFLT_START_NUM        0
    #define PVR_PIDFLT_END_NUM          16

    #define PVR_PIDFLT_NUM_ALL          16
#endif //#endif HW_PVR_ENABLE

#define CALLBACK_SIZE_MIN               16//due to current MIU alignment's up to 16 bytes

#define DRVRASP_PID_NULL             0x1FFF                                                 // Transport stream null PID
#define MSIF_RASP_LIB_CODE           {'R','A','S','P'}                                      // Lib code
#define MSIF_RASP_LIBVER             {'0','2'}                                              // LIB version
#define MSIF_RASP_BUILDNUM           {'0','0'}                                              // Build Number
#define MSIF_RASP_CHANGELIST         {'0','0','5','1','1','4','7','0'}                      // P4 ChangeList Number

#define RASP_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                            /* 'MSIF'                                           */  \
    MSIF_CLASS,                          /* '00'                                             */  \
    MSIF_CUS,                            /* 0x0000                                           */  \
    MSIF_MOD,                            /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                   \
    MSIF_CPU,                                                                                    \
    MSIF_RASP_LIB_CODE,                  /* IP__                                             */  \
    MSIF_RASP_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_RASP_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_RASP_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------

/// RASP DDI return value
/// @name TSP_Result
/// @ref TSP_Result
/// return value
/// @{
typedef enum
{
    DRVRASP_FAIL                   = 0,
    DRVRASP_OK,
    DRVRASP_INVALID_PARAM,
    DRVRASP_FUNC_ERROR,
} RASP_Result;

#define DrvRASP_Event                    MS_U32
#define E_DRVRASP_EVENT_DATA_INIT        0x00000000
//PVR buffer callback size met
#define E_DRVRASP_EVENT_CALLBACK_SIZE_MET 0x00000040

// Payload/Event Mask flag
//#define RASP_BYPASS_MASK            0xffffffff
#define E_DRVRASP_BYPASS_AFE             0x00000001      // adaptation field extension
#define E_DRVRASP_BYPASS_TPD             0x00000002      // transport private data
#define E_DRVRASP_BYPASS_SP              0x00000004      // splicing point
#define E_DRVRASP_BYPASS_OPCR            0x00000008      // OPCR
#define E_DRVRASP_BYPASS_PCR             0x00000010      // PCR
#define E_DRVRASP_BYPASS_ESPI            0x00000020      // elementary stream priority indicator
#define E_DRVRASP_BYPASS_RAI             0x00000040      // random access indicator
#define E_DRVRASP_BYPASS_DI              0x00000080      // discontinue indicator
#define E_DRVRASP_BYPASS_ESNS            0x00000100      // elementary stream not scrambled
#define E_DRVRASP_BYPASS_ESES            0x00000200      // elementary stream even scrambled
#define E_DRVRASP_BYPASS_ESOS            0x00000400      // elementary stream odd scrambled
#define E_DRVRASP_BYPASS_PUSI            0x00000800      // payload unit start indicator
#define E_DRVRASP_BYPASS_FPR             0x00001000      // first packet recorded
#define E_DRVRASP_BYPASS_RASP_Tick       0x80000000      // rasp tick

typedef enum
{
    E_DRVRASP_CAP_FILTER_NUM = 0,                   // Get filter number
    E_DRVRASP_CAP_FILTER_PVR_NUM,                   // Get pvr filter number
    E_DRVRASP_CAP_PVR_ALIGN,                        // Get pvr buffer minimal alignment
    E_DRVRASP_CAP_RESOURCE_SIZE,                    // Get the data structure size of private resource (share resource)
    E_DRVRASP_CAP_RASP_NUM,                    // Get RASP num

    E_DRVRASP_CAP_NULL,
} DrvRASP_Cap;

/// RASP channel state bit flags
typedef enum //_DrvTSP_FltState
{
    E_DRVRASP_FLT_STATE_FREE         = 0x00000000,                                                   ///<\n
    E_DRVRASP_FLT_STATE_ALLOC        = 0x00000001,                                                   ///<\n
    E_DRVRASP_FLT_STATE_ENABLE       = 0x00000002,                                                   ///<\n
    E_DRVRASP_FLT_STATE_OVERFLOW     = 0x00010000,                                                   //[Reserved]
    E_DRVRASP_FLT_STATE_NA           = 0xFFFFFFFF,
} DrvRASP_FltState;

/// TSP record mode
typedef enum //_DrvRASP_RecMode
{
    // Record PID
    E_DRVRASP_REC_MODE_PID  = 0x00000000,
    // Record ALL
    E_DRVRASP_REC_MODE_ALL  = 0x00000001,
} DrvRASP_RecMode;

/// TSP interface
typedef enum
{
    E_DRVRASP_IF_PLAYBACK            = 0x0, // TS interface 0
    E_DRVRASP_IF_PVR0                = 0x1, // TS interface 1, mainly for PVR
} DrvRASP_If;

typedef struct //_DrvRASP_Msg
{/*//why union? by teddy.chen
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
*/
    MS_U8 u8PVREngNum;
} DrvRASP_Msg;

typedef enum //_DrvRASP_RecType
{
    STR2MIU          = 0x0,
    PAYLOAD2MIU      = 0x1,
    ECM2MIU          = 0x2,
} DrvRASP_RecType;

/// TSP notification function
typedef void (*P_DrvRASP_EvtCallback)(DrvRASP_Event eEvent, DrvRASP_Msg *pMsg);

//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------

// Initialization API
RASP_Result MDrv_RASP_InitLibResource(void* pResMemAddr);
RASP_Result MDrv_RASP_Init(void);
RASP_Result MDrv_RASP_Exit(void);
RASP_Result MDrv_RASP_Reset(void);
RASP_Result MDrv_RASP_Alive(void);
RASP_Result MDrv_RASP_ReplacePackets(void);
RASP_Result MDrv_RASP_Confi(void);
RASP_Result MDrv_RASP_PVR_SetTSIF(MS_U32 u32RASPEng, MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP);
RASP_Result MDrv_RASP_PVR_GetTSIFStatus(MS_U32 u32RASPEng, MS_BOOL* pbExtSyc, MS_BOOL* pbParl, MS_BOOL *pbDataSWP);
RASP_Result MDrv_RASP_PVR_FileinEnable(MS_U32 u32RASPEng, MS_BOOL bEn);
RASP_Result MDrv_RASP_PVR_GetFileinEnable(MS_U32 u32RASPEng, MS_BOOL* bEn);

// Capacity query
RASP_Result MDrv_RASP_GetCap(DrvRASP_Cap eCap, void* pOutput);
RASP_Result MDrv_RASP_GetLibVer(const MSIF_Version **ppVersion);
RASP_Result MDrv_RASP_GetTimerAndPacketNum(const MSIF_Version **ppVersion);
RASP_Result MDrv_RASP_GetTsPayload(const MSIF_Version **ppVersion);
RASP_Result MDrv_RASP_GetEventMask(const MSIF_Version **ppVersion);

// PVR API
RASP_Result MDrv_RASP_PVR_SetBuffer(MS_U32 u32RASPEng, MS_PHYADDR u32BufStart0, MS_PHYADDR u32BufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1);
RASP_Result MDrv_RASP_PVR_Start(MS_U32 u32RASPEng, DrvRASP_RecMode eRecMode, MS_BOOL bStart);
RASP_Result MDrv_RASP_PVR_Pause(MS_U32 u32RASPEng, MS_BOOL bPause);
RASP_Result MDrv_RASP_PVR_GetWriteAddr(MS_U32 u32RASPEng, MS_PHYADDR *pu32WriteAddr);
RASP_Result MDrv_RASP_PVR_Notify(MS_U32 u32RASPEng, DrvRASP_Event eEvents, P_DrvRASP_EvtCallback pfCallback);
RASP_Result MDrv_RASP_PVR_SetPacketMode(MS_U32 u32RASPEng, DrvRASP_RecType eRecType,MS_BOOL bSet);
RASP_Result MDrv_RASP_PVR_TimeStampSetRecordStamp(MS_U32 u32RASPEng, MS_U32 u32Stamp);
RASP_Result MDrv_RASP_PVR_TimeStampGetRecordStamp(MS_U32 u32RASPEng, MS_U32* u32Stamp);
RASP_Result MDrv_RASP_PVR_TimeStampSelRecordStampSrc(MS_U32 u32RASPEng, MS_BOOL bLocal);
RASP_Result MDrv_RASP_PVR_AllocFlt(MS_U32 u32RASPEng, MS_U32 *pu32PidFltId);
RASP_Result MDrv_RASP_PVR_SetPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 u16Pid);
RASP_Result MDrv_RASP_PVR_GetPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 *pu16Pid);
RASP_Result MDrv_RASP_PVR_ReleaseFlt(MS_U32 u32RASPEng, MS_U16 u32FltId);
RASP_Result MDrv_RASP_PVR_AttachInterrupt(InterruptCb pIntCb);
RASP_Result MDrv_RASP_PVR_EnableInterrupt(void);
RASP_Result MDrv_RASP_PVR_CallbackSize(MS_U32 u32RASPEng, MS_U32* pu32CallbackSize, MS_BOOL bSet);
//RASP_Result MDrv_RASP_PVR_TimeStampSetPlaybackStamp(MS_U32 u32Stamp);
//RASP_Result MDrv_RASP_PVR_TimeStampGetPlaybackStamp(MS_U32* u32Stamp);
//RASP_Result MDrv_RASP_PVR_TimeStamp(MS_BOOL bEnable);

// RASP API
RASP_Result MDrv_RASP_SetEvent(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Event);
RASP_Result MDrv_RASP_SetPayload(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Payload);
RASP_Result MDrv_RASP_SetDataSwap(MS_U32 u32RASPEng, MS_BOOL bEn);

RASP_Result MDrv_PROC_RASP_PVR_SizeMet(MS_U32 u32RASPEng);
RASP_Result MDrv_RASP_CallbackIntCheck(MS_U32 u32RASPEng, MS_BOOL* bInterrupted);
RASP_Result MDrv_RASP_CallbackIntClr(MS_U32 u32RASPEng);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif
#endif // _DRVRASP_H_
