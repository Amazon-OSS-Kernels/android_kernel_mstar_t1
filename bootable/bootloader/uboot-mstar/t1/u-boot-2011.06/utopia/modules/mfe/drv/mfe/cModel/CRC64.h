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
#ifndef _CRC64_H_
#define _CRC64_H_


typedef struct BitArray {
	unsigned int bit0 : 1;
	unsigned int bit1 : 1;
	unsigned int bit2 : 1;
	unsigned int bit3 : 1;
	unsigned int bit4 : 1;
	unsigned int bit5 : 1;
	unsigned int bit6 : 1;
	unsigned int bit7 : 1;
	unsigned int bit8 : 1;
	unsigned int bit9 : 1;
	unsigned int bit10 : 1;
	unsigned int bit11 : 1;
	unsigned int bit12 : 1;
	unsigned int bit13 : 1;
	unsigned int bit14 : 1;
	unsigned int bit15 : 1;
	unsigned int bit16 : 1;
	unsigned int bit17 : 1;
	unsigned int bit18 : 1;
	unsigned int bit19 : 1;
	unsigned int bit20 : 1;
	unsigned int bit21 : 1;
	unsigned int bit22 : 1;
	unsigned int bit23 : 1;
	unsigned int bit24 : 1;
	unsigned int bit25 : 1;
	unsigned int bit26 : 1;
	unsigned int bit27 : 1;
	unsigned int bit28 : 1;
	unsigned int bit29 : 1;
	unsigned int bit30 : 1;
	unsigned int bit31 : 1;
	unsigned int bit32 : 1;
	unsigned int bit33 : 1;
	unsigned int bit34 : 1;
	unsigned int bit35 : 1;
	unsigned int bit36 : 1;
	unsigned int bit37 : 1;
	unsigned int bit38 : 1;
	unsigned int bit39 : 1;
	unsigned int bit40 : 1;
	unsigned int bit41 : 1;
	unsigned int bit42 : 1;
	unsigned int bit43 : 1;
	unsigned int bit44 : 1;
	unsigned int bit45 : 1;
	unsigned int bit46 : 1;
	unsigned int bit47 : 1;
	unsigned int bit48 : 1;
	unsigned int bit49 : 1;
	unsigned int bit50 : 1;
	unsigned int bit51 : 1;
	unsigned int bit52 : 1;
	unsigned int bit53 : 1;
	unsigned int bit54 : 1;
	unsigned int bit55 : 1;
	unsigned int bit56 : 1;
	unsigned int bit57 : 1;
	unsigned int bit58 : 1;
	unsigned int bit59 : 1;
	unsigned int bit60 : 1;
	unsigned int bit61 : 1;
	unsigned int bit62 : 1;
	unsigned int bit63 : 1;
} BitArray;

typedef struct _CRC_INFO {
	// Input 
	unsigned char bIsLast;	// When bIsLast=1, the last nBufSize%8 bytes are zero-padded to 8-byte for the last-round CRC.
							// When bIsLast=0, nLastBytes=nBufSize%8 and those bytes are stored in TempBuf for later CRC round.
	unsigned char *pBuf;	// The input byte data for CRC calculation
	int nBufSize;			// How many bytes in pBuf
	// Output
	unsigned char CRCValue[8];
	// Internal usage
	int nLastBytes;
	unsigned char TempBuf[8];
} CRC_INFO;

void CRC_Init(CRC_INFO* pInfo);
void CRC_Generator(CRC_INFO* pInfo);
void get_CRC_ACC_CRC(CRC_INFO* pAllFrames, CRC_INFO* pCurFrame);
void CRC_ACC_HW_Generator(CRC_INFO* pAllFrames, unsigned char* crc);

#endif
