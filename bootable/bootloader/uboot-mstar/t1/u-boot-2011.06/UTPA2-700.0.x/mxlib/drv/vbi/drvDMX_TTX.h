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
/// @file   drvDMX_TTX.h
/// @brief  MStar DVT TTX Driver Interface
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DMX_TTX_H
#define _DRV_DMX_TTX_H

#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define VBI_TTX_UTOPIA20 (1)

#define VBI_TTX_DATA_LENGTH 48

typedef struct
{
    MS_BOOL _bTTXInit;
    MS_BOOL _bSuspend;
    MS_PHY bufferAddr;
    MS_U16 packetCount;
    MS_PHY _TTXBufAddr;
    MS_U16 _TTXBufLen;
    MS_U8 _FilterID;
} DMX_TTX_StoreInfo;

typedef enum
{
    DMX_TTX_INIT,
    DMX_TTX_EXIT,
    DMX_TTX_SUSPEND,
    DMX_TTX_RESUME,
} EN_DMX_TTX_CMD;

typedef MS_U32(*DMX_CB_FN)(MS_U8 SecFltNo);
//-------------------------------------------------------------------------------------------------
/// Reset TTX ring buffer
/// @ingroup TTX_Task
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_RingBuffer_Reset(void);
//-------------------------------------------------------------------------------------------------
/// Initialize DMX TTX module
/// @ingroup TTX_General
/// @param  bufferAddr       \b IN: Set TTX ring buffer address
/// @param  packetCount    \b IN: Set TTX ring buffer size
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_Init(MS_PHY bufferAddr, MS_U16 packetCount);
//-------------------------------------------------------------------------------------------------
/// Finalize DMX TTX module
/// @ingroup TTX_General
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_Exit(void);

//-----------------
// TTX_Task
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set TTX callback function.
/// @ingroup TTX_Task
/// @param  fn         \b IN: callback sunction
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_SetCB(DMX_CB_FN fn);
//-------------------------------------------------------------------------------------------------
/// Set TTX PID filter index to use callback function get the write address of section buffer.
/// @ingroup TTX_Task
/// @param  fid         \b IN: PID filter index
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_SetFilterID(MS_U8 fid);
//-------------------------------------------------------------------------------------------------
/// Check TTX buffer is empty or not.
/// @ingroup TTX_Task
/// @return TRUE  : Buffer is empty.
/// @return FALSE : Buffer is not empty.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_PacketBufferIsEmpty(void);
//-------------------------------------------------------------------------------------------------
/// Get TTX data (Get all of the packets in the TTX buffer (if the input buffer is big enough))
/// @ingroup TTX_Task
/// @param  dataAddr       \b IN: Set TTX buffer address
/// @param  length           \b OUT: Get total packt length
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length);
//-------------------------------------------------------------------------------------------------
/// Get TTX data (Get 1 of the packets in the TTX buffer)
/// @ingroup TTX_Task
/// @param  packetAddress \b OUT: Get TTX buffer address
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_GetPacket(MS_PHY *packetAddress);

//-------------------------------------------------------------------------------------------------
/// Check if there is a packet buffer overflow. If there is an overflow,
/// the the packet buffer should be cleared from the reading task.
/// @ingroup TTX_Task
/// @return TRUE  : If there is packet buffer overflow
/// @return FALSE : Otherwise
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_PacketBufferIsOverflow( void );

//-------------------------------------------------------------------------------------------------
/// Return the nomber of packet buffer overflows since the last reset or creation.
/// @ingroup TTX_Task
/// @return : The number of packet buffer overflows.
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_DMX_TTX_PacketBufferGetNoOfOverflows( void );

//-----------------
// TTX_STR
//-------------------------------------------------------------------------------------------------
/// Save DMX_TTX driver states to DRAM.
/// @ingroup TTX_STR
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_Suspend(void);

//-------------------------------------------------------------------------------------------------
/// Restore DMX_TTX driver states from DRAM
/// @ingroup TTX_STR
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_Resume(void);

//-------------------------------------------------------------------------------------------------
/// Control TTX STR functions.
/// @ingroup TTX_STR
/// @param  u16PowerState  \b IN: STR power mode. (to control STR suspend & resume case.)
/// @return : Utopia status - UTOPIA_STATUS_SUCCESS (Succeed) or UTOPIA_STATUS_FAIL (Fail)
//-------------------------------------------------------------------------------------------------
MS_U32  MDrv_DMX_TTX_SetPowerState(EN_POWER_MODE u16PowerState);

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif

