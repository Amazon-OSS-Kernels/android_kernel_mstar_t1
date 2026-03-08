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
/// file    drvMMFilein.h
/// @brief  Multimedia File In (MMFILEIN 2) Driver Interface
/// @attention
/// All MMFILEIN DDI are not allowed to use in any interrupt context other than MMFILEIN ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MMFILEIN_H_
#define _DRV_MMFILEIN_H_

#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

#define MMFI_VER_1_0                     0x0100UL
#define MMFI_VER_2_0                     0x0200UL

#define MMFI_VERSION                     MMFI_VER_2_0


/// TSP byte address alignment unit
#define DRVMMFI_ALIGN_UNIT           16

/// TSP byte address alignment macro
#define DRVMMFI_ALIGN(_x)            ALIGN_16((MS_U32)_x)


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// Transport stream null PID
#define DRVMMFI_PID_NULL             0x1FFF
#define MSIF_MMFI_LIB_CODE           {'M','M','F','I','2'}                                               // Lib code
#define MSIF_MMFI_LIBVER             {'0','2'}                                                       // LIB version
#define MSIF_MMFI_BUILDNUM           {'0','0'}                                                       // Build Number
#define MSIF_MMFI_CHANGELIST         {'0','0','6','0','7','2','6','6'}                               // P4 ChangeList Number

#define MMFI_DRV_VERSION             /* Character String for DRV/API version             */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    MSIF_CUS,                       /* 0x0000                                           */  \
    MSIF_MOD,                       /* 0x0000                                           */  \
    MSIF_CHIP,                                                                              \
    MSIF_CPU,                                                                               \
    MSIF_MMFI_LIB_CODE,              /* IP__                                             */  \
    MSIF_MMFI_LIBVER,                /* 0.0 ~ Z.Z                                        */  \
    MSIF_MMFI_BUILDNUM,              /* 00 ~ 99                                          */  \
    MSIF_MMFI_CHANGELIST,            /* CL#                                              */  \
    MSIF_OS


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// MMFILEIN DDI return value

/// @name MMFI_Result
/// @ref MMFI_Result
/// return value
/// @{
typedef enum
{
    DRVMMFI_FAIL                     = 0,
    DRVMMFI_OK                       = 1,
    DRVMMFI_INVALID_PARAM            = 2,
    DRVMMFI_FUNC_ERROR               = 3,
    DRVMMFI_NOTSUPPORT               = 4,
} MMFI_Result;
/// @}

typedef enum
{
    E_DRVMMFI_PATH0   = 0x00,
    E_DRVMMFI_PATH1   = 0x01,

}DrvMMFI_Path;

typedef enum
{
    E_DRVMMFI_MODE_NO_BYPASS_AU     = 0x0000,
    E_DRVMMFI_MODE_PS_AU            = 0x0001,
    E_DRVMMFI_MODE_PS_AUB           = 0x0002,
    E_DRVMMFI_MODE_APID_BYPASS      = 0x0004,
    E_DRVMMFI_MODE_APIDB_BYPASS     = 0x0008,

    E_DRVMMFI_MODE_NO_BYPASS_V3D    = 0x0010,
    E_DRVMMFI_MODE_PS_V3D           = 0x0020,
    E_DRVMMFI_MODE_V3DPID_BYPASS    = 0x0040,

    E_DRVMMFI_MODE_NO_BYPASS_VD     = 0x0080,
    E_DRVMMFI_MODE_VDPID_BYPASS     = 0x0100,
    E_DRVMMFI_MODE_NO_BYPASS_TS     = 0x0200,
    E_DRVMMFI_MODE_PS_VD            = 0x0400,

    E_DRVMMFI_MODE_PS_AUC           = 0x0800,
    E_DRVMMFI_MODE_PS_AUD           = 0x1000,

    E_DRVMMFI_MODE_APIDC_BYPASS     = 0x2000,
    E_DRVMMFI_MODE_APIDD_BYPASS     = 0x4000,

    E_DRVMMFI_MODE_MMFI1_MASK       = 0x8000,   //must set, when using MMFI1 path
} DrvMMFI_FileinMode;

typedef enum
{
    /// Command Queue is Idle
    E_DRVMMFI_STATE_IDLE        = 0x00,
    /// Command Queue is Busy
    E_DRVMMFI_STATE_BUSY        = 0x01,

    E_DRVMMFI_STATE_UNKNOWN     = 0xFF,
} DrvMMFI_FileinState;

typedef enum
{
    E_DRVMMFI_STATUS_UNKNOWN            = 0,
    E_DRVMMFI_STATUS_FILEINDONE0        = 0x01,    //bit 0
    E_DRVMMFI_STATUS_AU_PKTERR0         = 0x02,    //bit 1
    E_DRVMMFI_STATUS_FILEINDONE1        = 0x04,    //bit 2
    E_DRVMMFI_STATUS_VD_PKTERR0         = 0x08,    //bit 3
    E_DRVMMFI_STATUS_AU_PKTERR1         = 0x10,
    E_DRVMMFI_STATUS_VD_PKTERR1         = 0x20,
} DrvMMFI_Status;


/// MMFI file in Packet mode
typedef enum //_DrvMMFI_PacketMode
{
    E_DRVMMFI_PKTMODE_188              = 0x0000,
    E_DRVMMFI_PKTMODE_192              = 0x0001,
    E_DRVMMFI_PKTMODE_204              = 0x0002,
} DrvMMFI_PacketMode;

/// TSP TS Input Source
typedef enum
{
    E_DRVMMFI_FLTTYPE_NONE = 0,
    E_DRVMMFI_FLTTYPE_AUD  = 1,
    E_DRVMMFI_FLTTYPE_AUDB = 2,
    E_DRVMMFI_FLTTYPE_VD3D = 3,
    E_DRVMMFI_FLTTYPE_VD   = 4,
    E_DRVMMFI_FLTTYPE_AUDC = 5,
    E_DRVMMFI_FLTTYPE_AUDD = 6,
    E_DRVMMFI_FLTTYPE_MAX
} DrvMMFI_FltType;

typedef enum //_DrvMMFI_DbgLevel
{
    E_DRVMMFI_DBG_Release = 0,
    E_DRVMMFI_DBG_ERR, // display error msg
    E_DRVMMFI_DBG_INFO,
} DrvMMFI_DbgLevel;

typedef enum
{
    E_DRVMMFI_CAP_FILTER0_NUM = 0,                  // Get filter number
    E_DRVMMFI_CAP_FILTER1_NUM,                      // Get pvr filter number
    E_DRVMMFI_CAP_RESOURCE_SIZE,                    // Get the data structure size of private resource (share resource)

    E_DRVMMFI_CAP_FILTER_ALL_NUM,
} DrvMMFI_Cap;


typedef struct _DrvPIDFLT_HEADER
{
    MS_U32      CCounter        : 4;
    MS_U32      Adp_Ctrl        : 2;
    MS_U32      Scramble        : 2;
    MS_U32      PID             : 13;
    MS_U32      TS_Priority     : 1;
    MS_U32      Payload_Start   : 1;
    MS_U32      Err_Flag        : 1;
    MS_U32      reserved        : 8;
}DrvPIDFLT_HEADER;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_InitLibResource(void* pResMemAddr);
MMFI_Result MDrv_MMFI_Init(void);
MMFI_Result MDrv_MMFI_Exit(void);
MMFI_Result MDrv_MMFI_Get_HWIntStatus(MS_U32* pu32status);
MMFI_Result MDrv_MMFI_SetDbgLevel(DrvMMFI_DbgLevel DbgLevel);
MMFI_Result MDrv_MMFI_AudPath_Enable(MS_BOOL bEnable);
MMFI_Result MDrv_MMFI_File_SetTimer(DrvMMFI_Path epath, MS_U8 u8timer);
MMFI_Result MDrv_MMFI_File_ByteTimer_En(DrvMMFI_Path epath, MS_BOOL bEnable);

// PIDFlt API
MMFI_Result MDrv_MMFI_PidFlt_Alloc(DrvMMFI_Path epath, DrvMMFI_FltType eFilterType, MS_U8 *pu8PidFltId);
MMFI_Result MDrv_MMFI_PidFlt_Free(MS_U8 u8PidFltId);
MMFI_Result MDrv_MMFI_PidFlt_Enable(MS_U8 u8PidFltId, MS_BOOL bEnable);
MMFI_Result MDrv_MMFI_PidFlt_SetPid(MS_U8 u8PidFltId, MS_U16 u16Pid);

MMFI_Result MDrv_MMFI_File_CmdQ_Reset(DrvMMFI_Path epath);
MMFI_Result MDrv_MMFI_File_CmdQFifo_Status(DrvMMFI_Path epath, MS_U8 *pu8FifoLevel);
MMFI_Result MDrv_MMFI_File_CmdQ_GetSlot(DrvMMFI_Path epath, MS_U32 *pu32EmptySlot);

MMFI_Result MDrv_MMFI_SetPlaybackTimeStamp(DrvMMFI_Path epath, MS_U32 u32lpcr2);
MMFI_Result MDrv_MMFI_GetPlaybackTimeStamp(DrvMMFI_Path epath, MS_U32 *pu32lpcr2);
MMFI_Result MDrv_MMFI_GetFileInTimeStamp(DrvMMFI_Path epath, MS_U32* pu32TSLen);

MMFI_Result MDrv_MMFI_File_SetAddr(DrvMMFI_Path epath, MS_PHY phyStreamAddr);
MMFI_Result MDrv_MMFI_File_SetSize(DrvMMFI_Path epath, MS_U32 u32StreamSize);
MMFI_Result MDrv_MMFI_File_SetPacketMode(DrvMMFI_Path epath, DrvMMFI_PacketMode PacketMode);
MMFI_Result MDrv_MMFI_File_192Mode_En(DrvMMFI_Path epath, MS_BOOL bEnable);
MMFI_Result MDrv_MMFI_File_192BlockScheme_En(DrvMMFI_Path epath, MS_BOOL bEnable);
MMFI_Result MDrv_MMFI_File_SetPacketMode(DrvMMFI_Path epath, DrvMMFI_PacketMode PacketMode);
MMFI_Result MDrv_MMFI_File_SetTimeStampClk(DrvMMFI_Path ePath, MS_U32 u32ClkSrc);
MMFI_Result MDrv_MMFI_File_GetReadAddr(DrvMMFI_Path epath, MS_PHY* pphyReadAddr);

MMFI_Result MDrv_MMFI_DupPktSkip(DrvMMFI_Path epath, MS_BOOL bEnable);
MMFI_Result MDrv_MMFI_TEI_SkipPktf(DrvMMFI_Path epath, MS_BOOL bEnable);
MMFI_Result MDrv_MMFI_Clr_Pidflt_ByteCnt(DrvMMFI_Path epath);
MMFI_Result MDrv_MMFI_TSDataEndian_Swap(DrvMMFI_Path epath, MS_BOOL bEnable);
MMFI_Result MDrv_MMFI_TSOrder_Swap(DrvMMFI_Path epath, MS_BOOL bEnable);

MMFI_Result MDrv_MMFI_File_GetState(DrvMMFI_Path epath, DrvMMFI_FileinState *peFileinState);
MMFI_Result MDrv_MMFI_File_ChkStatus(DrvMMFI_Status* pStatus);
MMFI_Result MDrv_MMFI_File_ClearHWInt(MS_U32 u32int);
MMFI_Result MDrv_MMFI_Get_TsHeader(DrvMMFI_Path epath, DrvPIDFLT_HEADER *pheader);

MMFI_Result MDrv_MMFI_File_Start(DrvMMFI_Path epath);
MMFI_Result MDrv_MMFI_File_Abort(DrvMMFI_Path epath);
MMFI_Result MDrv_MMFI_File_ModeEnable(DrvMMFI_FileinMode fileinmode, MS_BOOL bEnable);

MMFI_Result MDrv_MMFI_ResetAll(void);
MMFI_Result MDrv_MMFI_GetCap(DrvMMFI_Cap eCap, void* pOutput);
MMFI_Result MDrv_MMFI_RemoveDupAVPkt(MS_BOOL bEnable);

MMFI_Result MDrv_MMFI_VD3DPath_Enable(MS_BOOL bEnable);

MMFI_Result MDrv_MMFI_Set_MOBF_FileinKey(DrvMMFI_Path epath, MS_U32 u32Key);
MMFI_Result MDrv_MMFI_MOBF_FileIn_Enable(DrvMMFI_Path epath, MS_BOOL benable);
MMFI_Result MDrv_MMFI_MOBF_FileIn_SetLevel(DrvMMFI_Path epath, MS_U8 u8level);

#ifdef MSOS_TYPE_LINUX_KERNEL
MMFI_Result MDrv_MMFI_Suspend(void);
MMFI_Result MDrv_MMFI_Resume(void);
#endif  //MSOS_TYPE_LINUX_KERNEL


#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif // _DRV_MMFILEIN_H_
