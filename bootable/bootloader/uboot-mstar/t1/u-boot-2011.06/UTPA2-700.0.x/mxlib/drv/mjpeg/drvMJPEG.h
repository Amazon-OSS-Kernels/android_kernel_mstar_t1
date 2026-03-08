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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvMJPEG.h
/// @brief  MJPEG Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MJPEG_H_
#define _DRV_MJPEG_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------
#define JPD_DISP_QUEUE_MAX_SIZE         32

// Chip Define
typedef enum
{
    E_MJPEG_MSTAR_CHIP_NONE = 0,
    E_MJPEG_MSTAR_CHIP_U3,
    E_MJPEG_MSTAR_CHIP_T3,
    E_MJPEG_MSTAR_CHIP_T4,
    E_MJPEG_MSTAR_CHIP_JANUS,
    E_MJPEG_MSTAR_CHIP_U4,
    E_MJPEG_MSTAR_CHIP_T8,
    E_MJPEG_MSTAR_CHIP_T9,
    E_MJPEG_MSTAR_CHIP_T13,
    E_MJPEG_MSTAR_CHIP_A1,
    E_MJPEG_MSTAR_CHIP_A6,
    E_MJPEG_MSTAR_CHIP_A7,
    E_MJPEG_MSTAR_CHIP_AMETHYST,
    E_MJPEG_MSTAR_CHIP_EAGLE,
    E_MJPEG_MSTAR_CHIP_J2,
    E_MJPEG_MSTAR_CHIP_EMERALD,
    E_MJPEG_MSTAR_CHIP_M12,
} MJPEG_MSTAR_CHIP_ID;

// Mimic fwJPD_if.h
typedef enum
{
    // Invalid cmd
    E_MJPEG_CMD_INVALID = 0xffffffffUL,

    E_MJPEG_CMD_GO                             = 0x00, // Start to show
    E_MJPEG_CMD_SET_FRAME_BUFF_START_ADDR      = 0x01, // Set frame buffer address
    E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE       = 0x02, // Set frame buffer size
    E_MJPEG_CMD_SET_FRAME_BUFF_TOTAL_NUM       = 0x03, // Set total number of frame buffer
    E_MJPEG_CMD_SET_FRAME_BUFF_IDX             = 0x04, // Set frame buffer index
    E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY       = 0x05, // Set frame buffer index ready for display
    E_MJPEG_CMD_SET_WIDTH                      = 0x06, // Set frame width
    E_MJPEG_CMD_SET_HEIGHT                     = 0x07, // Set frame height
    E_MJPEG_CMD_SET_PITCH                      = 0x08, // Set pitch
    E_MJPEG_CMD_SET_FRAME_ID_L                 = 0x09, // Set frame ID_L
    E_MJPEG_CMD_SET_FRAME_ID_H                 = 0x0A, // Set frame ID_H
    E_MJPEG_CMD_SET_TIMESTAMP                  = 0x0B, // Set Time Stamp
    E_MJPEG_CMD_SET_FRAMERATE                  = 0x0C, // Set FrameRate
    E_MJPEG_CMD_SET_FRAMERATE_BASE             = 0x0D, // Set FrameRate Base
    E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID       = 0x0E, // Set frame buffer index available
    E_MJPEG_CMD_SET_CHIP_ID                    = 0x0F, // Set frame buffer index available

    E_MJPEG_CMD_PLAY                           = 0x20, // Play
    E_MJPEG_CMD_PAUSE                          = 0x21, // Pause
    E_MJPEG_CMD_RESUME                         = 0x22, // Resume
    E_MJPEG_CMD_STEP_PLAY                      = 0x23, // Step play
    E_MJPEG_CMD_SET_SPEED_TYPE                 = 0x24, // Set play speed type: default, fast, slow
    E_MJPEG_CMD_SET_SPEED                      = 0x25, // Set play speed
    E_MJPEG_CMD_FLUSH_DISP_QUEUE               = 0X26, // Flush display queue
    E_MJPEG_CMD_FREEZE_DISP                    = 0x27, // Freeze display
    E_MJPEG_CMD_ENABLE_AVSYNC                  = 0x28, // Enable AV sync
    E_MJPEG_CMD_SET_AVSYNC_DELAY               = 0x29, // Set AV sync delay
    E_MJPEG_CMD_SET_AVSYNC_TOLERENCE           = 0x2A, // Set AV sync tolerence
    E_MJPEG_CMD_SET_PTS_BASE                   = 0x2B, // Set PTS base
    E_MJPEG_CMD_SET_STC_BASE                   = 0x2C, // Set STC base
    E_MJPEG_CMD_SET_BLUE_SCREEN                = 0x2D, // Set Blue Screen

    E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX   = 0x40, // Get next free frame buffer index
    E_MJPEG_CMD_COMPENSATE_PTS                 = 0x41, // Ask firmware to compensate PTS

    // Display Command Queue
    E_MJPEG_CMD_ENABLE_DISP_CMD_QUEUE          = 0x80, // Enable Display Command Queue
    E_MJPEG_CMD_PUSH_DISP_CMD                  = 0x81, // Push Display Command
    E_MJPEG_CMD_GET_DISP_CMD_Q_VACANCY         = 0x82, // Check if the display command queue full or not

    E_MJPEG_CMD_IS_STEP_PLAY_DONE              = 0xFF, //
    E_MJPEG_CMD_IS_DISP_FINISH                 = 0xFE, //
    E_MJPEG_CMD_IS_PLAYING                     = 0xFC, //
    E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL          = 0xFB, //
    E_MJPEG_CMD_IS_AVSYNC_ON                   = 0xFA, //
    E_MJPEG_CMD_IS_REACH_AVSYNC                = 0xF9, //
    E_MJPEG_CMD_IS_FLUSH_DONE                  = 0xF8, // Check if flush done

} MJPEG_User_Cmd;

// JPD debug level enum
typedef enum
{
    E_JPD_DEBUG_DRV_NONE = 0x0
  , E_JPD_DEBUG_DRV_MSG  = 0x02
  , E_JPD_DEBUG_DRV_ERR  = 0x08
} JPD_DrvDbgLevel;

typedef enum
{
    E_JPD_FAILED        = 0
  , E_JPD_OK            = 1
  , E_JPD_INVALID_PARAM = 2
} JPD_Return;

// Command
typedef enum
{
    // Invalid cmd
    E_JPD_CMD_INVALID = 0xffffffffUL,

    E_JPD_CMD_GO                             = 0x00, // Start to show
    E_JPD_CMD_SET_FRAME_BUFF_START_ADDR      = 0x01, // Set frame buffer address
    E_JPD_CMD_SET_FRAME_BUFF_UNIT_SIZE       = 0x02, // Set frame buffer size
    E_JPD_CMD_SET_FRAME_BUFF_TOTAL_NUM       = 0x03, // Set total number of frame buffer
    E_JPD_CMD_SET_FRAME_BUFF_IDX             = 0x04, // Set frame buffer index
    E_JPD_CMD_SET_FRAME_BUFF_IDX_READY       = 0x05, // Set frame buffer index ready for display
    E_JPD_CMD_SET_WIDTH                      = 0x06, // Set frame width
    E_JPD_CMD_SET_HEIGHT                     = 0x07, // Set frame height
    E_JPD_CMD_SET_PITCH                      = 0x08, // Set pitch
    E_JPD_CMD_SET_FRAME_ID_L                 = 0x09, // Set frame ID_L
    E_JPD_CMD_SET_FRAME_ID_H                 = 0x0A, // Set frame ID_H
    E_JPD_CMD_SET_TIMESTAMP                  = 0x0B, // Set Time Stamp
    E_JPD_CMD_SET_FRAMERATE                  = 0x0C, // Set FrameRate
    E_JPD_CMD_SET_FRAMERATE_BASE             = 0x0D, // Set FrameRate Base
    E_JPD_CMD_SET_FRAME_BUFF_IDX_VALID       = 0x0E, // Set frame buffer index available
    E_JPD_CMD_SET_CHIP_ID                    = 0x0F, // Set Chip ID

    E_JPD_CMD_PLAY                           = 0x20, // Play
    E_JPD_CMD_PAUSE                          = 0x21, // Pause
    E_JPD_CMD_RESUME                         = 0x22, // Resume
    E_JPD_CMD_STEP_PLAY                      = 0x23, // Step play
    E_JPD_CMD_SET_SPEED_TYPE                 = 0x24, // Set play speed type: default, fast, slow
    E_JPD_CMD_SET_SPEED                      = 0x25, // Set play speed
    E_JPD_CMD_FLUSH_DISP_QUEUE               = 0X26, // Flush display queue
    E_JPD_CMD_FREEZE_DISP                    = 0x27, // Freeze display
    E_JPD_CMD_ENABLE_AVSYNC                  = 0x28, // Enable AV Sync
    E_JPD_CMD_SET_AVSYNC_DELAY               = 0x29, // Set AV sync delay
    E_JPD_CMD_SET_AVSYNC_TOLERENCE           = 0x2A, // Set AV sync tolerence
    E_JPD_CMD_SET_PTS_BASE                   = 0x2B, // Set PTS base
    E_JPD_CMD_SET_STC_BASE                   = 0x2C, // Set STC base
    E_JPD_CMD_SET_BLUE_SCREEN                = 0x2D, // Set Blue Screen

    E_JPD_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX   = 0x40, // Get next free frame buffer index
    E_JPD_CMD_COMPENSATE_PTS                 = 0x41, // Ask firmware to compensate PTS

    // Display Command Queue
    E_JPD_CMD_ENABLE_DISP_CMD_QUEUE          = 0x80, // Enable Display Command Queue
    E_JPD_CMD_PUSH_DISP_CMD                  = 0x81, // Push Display Command
    E_JPD_CMD_GET_DISP_CMD_Q_VACANCY         = 0x82, // Check if the display command queue full or not

    E_JPD_CMD_IS_STEP_PLAY_DONE              = 0xFF, //
    E_JPD_CMD_IS_DISP_FINISH                 = 0xFE, //
    E_JPD_CMD_IS_PLAYING                     = 0xFC, //
    E_JPD_CMD_IS_DISPLAY_QUEUE_FULL          = 0xFB, //
    E_JPD_CMD_IS_AVSYNC_ON                   = 0xFA, //
    E_JPD_CMD_IS_REACH_AVSYNC                = 0xF9, //
    E_JPD_CMD_IS_FLUSH_DONE                  = 0xF8, // Check if flush done

} JPD_User_Cmd;

typedef enum
{
    E_JPD_DISPQ_STATUS_NONE = 0,            //FW
    E_JPD_DISPQ_STATUS_INIT,                //FW
    E_JPD_DISPQ_STATUS_VIEW,                //HK
    E_JPD_DISPQ_STATUS_DISP,                //HK
    E_JPD_DISPQ_STATUS_FREE,                //HK
} JPD_DISPQ_STATUS;

/// frame information
typedef struct
{
    /// frame buffer base + the start offset of current displayed luma data. Unit: byte.
    MS_U32 u32LumaAddr;
    /// frame buffer base + the start offset of current displayed chroma data. Unit: byte.
    MS_U32 u32ChromaAddr;
    /// Time stamp(DTS, PTS) of current displayed frame. Unit: ms (todo: 90khz)
    MS_U32 u32TimeStamp;
    /// low part of ID number
    MS_U32 u32ID_L;
    /// high part of ID number
    MS_U32 u32ID_H;
    /// pitch
    MS_U16 u16Pitch;
    /// width
    MS_U16 u16Width;
    /// hight
    MS_U16 u16Height;
    ///< Frame type: I, P, B frame
    MS_U8 u8FrameType;
    /// 4-byte alignment
    MS_U8 u8Reserved;
    ///< 0:None, 1:Init, 2:View, 3:Disp, 4:Free
    MS_U32 u32Status;
} MJPG_FrameInfo;

// Share Memory
typedef struct
{
    // HK -> FW
//    MS_U32 u32FrameBufAddr;
//    MS_U32 u32FrameBufSize;

    // FW -> HK
    MS_U32 u32FWVersionID;              // FW version ID

    //
//    MS_U32 u32DispBufAddr;
    MJPG_FrameInfo infoDispFrame;      // Information of currently displayed frame


    // PTS table
//    MS_U32 u32TimeStamp;            // current PTS
//    MS_U32 u32PTStableWptrAddr;     // The address of PTS table write pointer.
//    MS_U32 u32PTStableRptrAddr;     // The address of PTS table read pointer.

    MS_U32 u16DispQSize;
    MS_U16 u16DispQNumb;
    MS_U16 u16DispQPtr;
    MJPG_FrameInfo DispQueue[JPD_DISP_QUEUE_MAX_SIZE];
} JPD_ShareMem;

//#define HVD_PTS_TABLE_ST_OFFSET     0x50000
#define JPD_PTS_TABLE_ST_OFFSET     0x70000
//#define HVD_SHARE_MEM_ST_OFFSET     0x79000
#define JPD_SHARE_MEM_ST_OFFSET     0x80000
#define JPD_DEBUG_MEM_ST_OFFSET     0x81000


/* =============================== MJPEG =====================================*/
void MDrv_MJPEG_SetDbgLevel(MS_U8 u8DbgLevel);
void MDrv_JPD_StopVPU(void);
MS_BOOL MDrv_JPD_RstVPU(void);
void MDrv_JPD_ReleaseVPU(void);
MS_BOOL MDrv_JPD_LoadCode(MS_PHYADDR u32Addr, MS_U32 u32Size, MS_U32 u32BinAddr, MS_U8 u8FwSrcType);
void MDrv_JPD_ConfigVPU(MS_PHYADDR u32StAddr, MS_U8 u8dlend_en);
MS_BOOL MDrv_JPD_IsFirmwareReady(void);

#if 0
MS_BOOL MDrv_JPD_SetVPUCmd(MS_U32 u32Cmd);
MS_BOOL MDrv_JPD_SetVPUCmdArg(MS_U32 u32Arg);
MS_U32 MDrv_JPD_CheckVPUStatus(MS_U32 u32Cmd);
MS_U32 MDrv_JPD_GetNextFreeFrameBuffIdx(void);
#endif
MS_U32 MDrv_JPD_SendVPUCommand(MJPEG_User_Cmd eCmd, MS_U32 u32Arg);

MS_BOOL MDrv_JPD_IsVPUOnMiu1(void);
MS_U32 MDrv_JPD_GetMiuBase(void);
MS_U32 MDrv_JPD_GetShareMemAddr(void);
void MDrv_JPD_ClearShareMemory(void);
MS_U32 MDrv_JPD_GetPtsTableAddr(void);

MS_BOOL MDrv_JPD_ResetPTS(MS_U32 u32PtsBase);
MS_U32 MDrv_JPD_GetPTS(void);
void MDrv_JPD_GetDispFrameInfo(void *pInfo);

void MDrv_JPD_DispFrame(MS_U32 u32DispIdx);
void MDrv_JPD_FreeFrame(MS_U32 u32DispIdx);
void* MDrv_JPD_GetNextDispFrame(void);
MS_U32 MDrv_JPD_GetNextDispQPtr(void);

void MDrv_JPD_EnableTurboFWMode(MS_BOOL bEnable);

JPD_Return MDrv_JPD_InitFw(MS_PHYADDR u32Addr, MS_U32 u32Size, MS_PHYADDR u32BinAddr, MS_U8 u8FwSrcType);
MJPEG_MSTAR_CHIP_ID MDrv_JPD_Get_Chip_ID(void);


#ifdef __cplusplus
}
#endif

#endif // _DRV_MJPEG_H_

