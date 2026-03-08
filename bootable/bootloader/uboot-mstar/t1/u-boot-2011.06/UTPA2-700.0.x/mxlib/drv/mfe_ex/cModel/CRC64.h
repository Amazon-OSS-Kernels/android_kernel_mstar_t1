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
#ifndef _CRC64_H_
#define _CRC64_H_

#include "mfe_type.h"


typedef struct BitArray {
	MS_U32 bit0 : 1;
	MS_U32 bit1 : 1;
	MS_U32 bit2 : 1;
	MS_U32 bit3 : 1;
	MS_U32 bit4 : 1;
	MS_U32 bit5 : 1;
	MS_U32 bit6 : 1;
	MS_U32 bit7 : 1;
	MS_U32 bit8 : 1;
	MS_U32 bit9 : 1;
	MS_U32 bit10 : 1;
	MS_U32 bit11 : 1;
	MS_U32 bit12 : 1;
	MS_U32 bit13 : 1;
	MS_U32 bit14 : 1;
	MS_U32 bit15 : 1;
	MS_U32 bit16 : 1;
	MS_U32 bit17 : 1;
	MS_U32 bit18 : 1;
	MS_U32 bit19 : 1;
	MS_U32 bit20 : 1;
	MS_U32 bit21 : 1;
	MS_U32 bit22 : 1;
	MS_U32 bit23 : 1;
	MS_U32 bit24 : 1;
	MS_U32 bit25 : 1;
	MS_U32 bit26 : 1;
	MS_U32 bit27 : 1;
	MS_U32 bit28 : 1;
	MS_U32 bit29 : 1;
	MS_U32 bit30 : 1;
	MS_U32 bit31 : 1;
	MS_U32 bit32 : 1;
	MS_U32 bit33 : 1;
	MS_U32 bit34 : 1;
	MS_U32 bit35 : 1;
	MS_U32 bit36 : 1;
	MS_U32 bit37 : 1;
	MS_U32 bit38 : 1;
	MS_U32 bit39 : 1;
	MS_U32 bit40 : 1;
	MS_U32 bit41 : 1;
	MS_U32 bit42 : 1;
	MS_U32 bit43 : 1;
	MS_U32 bit44 : 1;
	MS_U32 bit45 : 1;
	MS_U32 bit46 : 1;
	MS_U32 bit47 : 1;
	MS_U32 bit48 : 1;
	MS_U32 bit49 : 1;
	MS_U32 bit50 : 1;
	MS_U32 bit51 : 1;
	MS_U32 bit52 : 1;
	MS_U32 bit53 : 1;
	MS_U32 bit54 : 1;
	MS_U32 bit55 : 1;
	MS_U32 bit56 : 1;
	MS_U32 bit57 : 1;
	MS_U32 bit58 : 1;
	MS_U32 bit59 : 1;
	MS_U32 bit60 : 1;
	MS_U32 bit61 : 1;
	MS_U32 bit62 : 1;
	MS_U32 bit63 : 1;
} BitArray;

typedef struct _CRC_INFO {
	// Input
	MS_U8 bIsLast;	// When bIsLast=1, the last nBufSize%8 bytes are zero-padded to 8-byte for the last-round CRC.
							// When bIsLast=0, nLastBytes=nBufSize%8 and those bytes are stored in TempBuf for later CRC round.
	MS_U8 *pBuf;	// The input byte data for CRC calculation
	MS_S32 nBufSize;			// How many bytes in pBuf
	// Output
	MS_U8 CRCValue[8];
	// Internal usage
	MS_S32 nLastBytes;
	MS_U8 TempBuf[8];
} CRC_INFO;

void CRC_Init(CRC_INFO* pInfo);
void CRC_Generator(CRC_INFO* pInfo);
void get_CRC_ACC_CRC(CRC_INFO* pAllFrames, CRC_INFO* pCurFrame);
void CRC_ACC_HW_Generator(CRC_INFO* pAllFrames, MS_U8* crc);

#endif
