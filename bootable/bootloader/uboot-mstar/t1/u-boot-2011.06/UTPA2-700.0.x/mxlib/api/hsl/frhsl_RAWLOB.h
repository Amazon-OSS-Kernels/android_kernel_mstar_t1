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
/***************************************************************************
 *
 * Copyright ?2008-2010 MStar Semiconductor, Inc. All Rights Reserved
 *
 **************************************************************************/
/** \file
 * \brief This is the header file for RAWLOB format constant.
 */
#if defined (MSOS_TYPE_LINUX)
#ifndef FRHSL_RAWLOB_H
#define FRHSL_RAWLOB_H

/*******************************************************************************
** Constants
*******************************************************************************/

/** Id value setting boundary between representation of 16bit and 32bit log
* point IDs.
*/
#define FR_HSL_TRANSITION_ID 0xFFFF

/** If over 0xff00, it must cut off the high and low bytes...
*
*/



/* RAWLOB format

    Timer Wrap (1-byte message type only):                  type(1 byte)
    Simeple(SP) message (5-byte header only):           type(1 byte), ID(2 bytes), time stamp(2 bytes)
    Variable-length(VL) message (5-byte header + 3-bytes packet counts + n*(3-bytes packets) ):

    Add sync marker every 7 bytes
*/

// TODO: remove this 3 types later
#define HSL_RAWLOB_TW  HSL_RAWLOB_MSG_TYPE_TW_CHAN0
#define HSL_RAWLOB_SP HSL_RAWLOB_MSG_TYPE_SP_CHAN0
#define HSL_RAWLOB_VL HSL_RAWLOB_MSG_TYPE_VL_CHAN0

// synchronization marker
#define HSL_RAWLOB_SYNC_MARKER                  0x20
#define HSL_RAWLOB_SYNC_MARKER_INTERVAL 7

// message types, 4 channels per message type
// TimerWrap(TW):
#define HSL_RAWLOB_MSG_TYPE_TW_CHAN0    0x17
#define HSL_RAWLOB_MSG_TYPE_TW_CHAN1    0x1C
#define HSL_RAWLOB_MSG_TYPE_TW_CHAN2    0x27
#define HSL_RAWLOB_MSG_TYPE_TW_CHAN3    0x37
// Simple(SP) Log Point
#define HSL_RAWLOB_MSG_TYPE_SP_CHAN0    0x13
#define HSL_RAWLOB_MSG_TYPE_SP_CHAN1    0x73
#define HSL_RAWLOB_MSG_TYPE_SP_CHAN2    0x23
#define HSL_RAWLOB_MSG_TYPE_SP_CHAN3    0x33
// Variable-Length(VL) Log Point
#define HSL_RAWLOB_MSG_TYPE_VL_CHAN0    0x12
#define HSL_RAWLOB_MSG_TYPE_VL_CHAN1    0x72
#define HSL_RAWLOB_MSG_TYPE_VL_CHAN2    0x22
#define HSL_RAWLOB_MSG_TYPE_VL_CHAN3    0x32

// payload markers
#define HSL_RAWLOB_MORE_PAYLOAD_MARKER  0x11
#define HSL_RAWLOB_LAST_PAYLOAD_MARKER  0x10



/* length settings */
#define HSL_RAWLOB_LENGTH_OF_SYNC_MARKER            1
#define HSL_RAWLOB_LENGTH_OF_MSG_TYPE                  1
#define HSL_RAWLOB_LENGTH_OF_MSG_FIRST_ID           2
#define HSL_RAWLOB_LENGTH_OF_TIME_STAMP            2
#define HSL_RAWLOB_LENGTH_OF_HEADER                    (HSL_RAWLOB_LENGTH_OF_MSG_TYPE + HSL_RAWLOB_LENGTH_OF_MSG_FIRST_ID + HSL_RAWLOB_LENGTH_OF_TIME_STAMP)
#define HSL_RAWLOB_LENGTH_OF_PAYLOAD_MARKER    1
#define HSL_RAWLOB_LENGTH_OF_PAYLOAD                   2
#define HSL_RAWLOB_LENGTH_OF_PACKET                    (HSL_RAWLOB_LENGTH_OF_PAYLOAD_MARKER + HSL_RAWLOB_LENGTH_OF_PAYLOAD)

#define HSL_RAWLOB_LENGTH_OF_TW_MSG                     HSL_RAWLOB_LENGTH_OF_MSG_TYPE
#define HSL_RAWLOB_LENGTH_OF_SP_MSG                     HSL_RAWLOB_LENGTH_OF_HEADER
#define HSL_RAWLOB_LENGTH_OF_VL_MSG(u32Packets) (HSL_RAWLOB_LENGTH_OF_HEADER + HSL_RAWLOB_LENGTH_OF_PACKET*(u32Packets))

/* offset settings */
#define HSL_RAWLOB_OFFSET_OF_MSG_TYPE                  0
#define HSL_RAWLOB_OFFSET_OF_MSG_ID                 HSL_RAWLOB_LENGTH_OF_MSG_TYPE
#define HSL_RAWLOB_OFFSET_OF_TIME_STAMP          (HSL_RAWLOB_LENGTH_OF_MSG_TYPE + HSL_RAWLOB_LENGTH_OF_TIME_STAMP)

// 1 packet = 3 bytes = payload marker(1 byte) + payload(2 bytes)
#define HSL_RAWLOB_OFFSET_OF_PAYLOAD_MARKER_IN_PACKET 0
#define HSL_RAWLOB_OFFSET_OF_PAYLOAD_IN_PACKET              HSL_RAWLOB_LENGTH_OF_PAYLOAD_MARKER
#define HSL_RAWLOB_OFFSET_OF_PACKET(u32nthPacket)           (HSL_RAWLOB_LENGTH_OF_HEADER+HSL_RAWLOB_LENGTH_OF_PACKET*(u32nthPacket))

#define HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_PACKET_COUNTS 1
#define HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_ADDRESS            2
#define HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_SECOND_ID          1

// special purpose constant
#define HSL_RAWLOB_INVALID_PRINTF_MSG_ID  0x4444 // if user uses printf directly

#define HSL_RAWLOB_DROPPED_MSG_ID  0x83C2 // if ring buffer is full


/*******************************************************************************
** Macros
*******************************************************************************/


#endif
#endif
/* END OF FILE */
