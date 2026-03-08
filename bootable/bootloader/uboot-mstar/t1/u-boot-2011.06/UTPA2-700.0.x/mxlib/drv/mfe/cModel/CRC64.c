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

#include "CRC64.h"
#include "mfe_common.h"

static void CRC64_BIT(BitArray* D, BitArray* X_next, int interface_mode);

void CRC_Init(CRC_INFO* pInfo)
{
	int i;
	for(i=0;i<8;i++)
		pInfo->CRCValue[i] = 0;
	pInfo->nLastBytes = 0;
	pInfo->bIsLast = 0;
}

void CRC_Generator(CRC_INFO* pInfo)
{
	int i=0, j=0, m, n, N_bits;
	unsigned char data_new[8];

	// Number of bits
	N_bits = pInfo->nBufSize*8;

	// Last remaining byte special handling
	if (pInfo->nLastBytes!=0) {
		int nByteToFill = pInfo->nBufSize<8-pInfo->nLastBytes ? pInfo->nBufSize : 8-pInfo->nLastBytes;
		for (j=0; j<pInfo->nLastBytes; j++)
			data_new[j] = pInfo->TempBuf[j];
		for (i=0; i<nByteToFill; i++)
			data_new[j++] = pInfo->pBuf[i];
		if (j<8) {
			if (!pInfo->bIsLast) {
				// Rollback and wait next round
				pInfo->nLastBytes = j;
				for (i=0; i<pInfo->nLastBytes; i++)
					pInfo->TempBuf[i] = data_new[i];
				return;
			}
			else {
				for (j=j; j<8; j++)
					data_new[j] = 0;
				CRC64_BIT((BitArray*)data_new, (BitArray*)pInfo->CRCValue, 1);
				pInfo->nLastBytes = 0;
				return;
			}
		}
		else {
			CRC64_BIT((BitArray*)data_new, (BitArray*)pInfo->CRCValue, 1);
			pInfo->nLastBytes = 0;
		}
	}
	N_bits -= i*8;

	for (i=i;i<N_bits/64;i++) {
		// generate 64 bit at a time
		for (j=0; j<8; j++)
			data_new[j] = pInfo->pBuf[8*i+j];
		// call bit array CRC64 generator
		CRC64_BIT((BitArray*)data_new, (BitArray*)pInfo->CRCValue, 1);
	}
	m = N_bits%64 ;
	n = N_bits/64 ;
	if(N_bits%64 != 0) {
		if (!pInfo->bIsLast) {
			for (j=0; j<m/8; j++)
				pInfo->TempBuf[j] = pInfo->pBuf[8*n+j];
			pInfo->nLastBytes = j;
			return;
		}
		// The last bytes
		for (j=0; j<m/8; j++)
			data_new[j] = pInfo->pBuf[8*n+j];
		for (j=m/8; j<8; j++)
			data_new[j] = 0;
		// call bit array CRC64 generator
		CRC64_BIT((BitArray*)data_new, (BitArray*)pInfo->CRCValue, 1);
	}
}


void CRC_ACC_HW_Generator(CRC_INFO* pAllFrames, unsigned char* crc)
{
	int i;
	for (i=0; i<8; i++)
		pAllFrames->CRCValue[i] += crc[i];

}
void get_CRC_ACC_CRC(CRC_INFO* pAllFrames, CRC_INFO* pCurFrame)
{
	int i;
	for (i=0; i<8; i++)
		pAllFrames->CRCValue[i] += pCurFrame->CRCValue[i];
}

static void CRC64_BIT(BitArray* D, BitArray* X_next, int interface_mode)
{
	// implement CRC encoder with 32/64 bit I/O
	// D[]: input bit array, 32-bit or 64-bit
	// X_next[]: shift registers of size 64
	// interface_mode: 0 for 32 bit I/O, 1 for 64 bit I/O
	// the generator polynomial for CRC64 is x^64 + x^4 + x^3 + x + 1 (CRC-64-ISO)

	BitArray X;         // internal buffer

	// initialization
	memcpy(&X, X_next, sizeof(BitArray));
// 	for (int i=0;i<64;i++) {
// 		X[i] = X_next[i];       // put the contents of 64 shift registers into internal buffer
// 	}

	if (interface_mode == 0) {  // 32 bit I/O
		// update states of 64 shift registers
		X_next->bit0 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26^X.bit30^D->bit30^X.bit31^D->bit31^X.bit32;
		X_next->bit1 = X.bit0^D->bit0^X.bit3^D->bit3^X.bit6^D->bit6^X.bit9^D->bit9^X.bit12^D->bit12^X.bit15^D->bit15^X.bit18^D->bit18^X.bit21^D->bit21^X.bit24^D->bit24^X.bit27^D->bit27^X.bit30^D->bit30^X.bit33;
		X_next->bit2 = X.bit1^D->bit1^X.bit4^D->bit4^X.bit7^D->bit7^X.bit10^D->bit10^X.bit13^D->bit13^X.bit16^D->bit16^X.bit19^D->bit19^X.bit22^D->bit22^X.bit25^D->bit25^X.bit28^D->bit28^X.bit31^D->bit31^X.bit34;
		X_next->bit3 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25^X.bit29^D->bit29^X.bit30^D->bit30^X.bit31^D->bit31^X.bit35;
		X_next->bit4 = X.bit36;
		X_next->bit5 = X.bit37;
		X_next->bit6 = X.bit38;
		X_next->bit7 = X.bit39;
		X_next->bit8 = X.bit40;
		X_next->bit9 = X.bit41;
		X_next->bit10 = X.bit42;
		X_next->bit11 = X.bit43;
		X_next->bit12 = X.bit44;
		X_next->bit13 = X.bit45;
		X_next->bit14 = X.bit46;
		X_next->bit15 = X.bit47;
		X_next->bit16 = X.bit48;
		X_next->bit17 = X.bit49;
		X_next->bit18 = X.bit50;
		X_next->bit19 = X.bit51;
		X_next->bit20 = X.bit52;
		X_next->bit21 = X.bit53;
		X_next->bit22 = X.bit54;
		X_next->bit23 = X.bit55;
		X_next->bit24 = X.bit56;
		X_next->bit25 = X.bit57;
		X_next->bit26 = X.bit58;
		X_next->bit27 = X.bit59;
		X_next->bit28 = X.bit60;
		X_next->bit29 = X.bit61;
		X_next->bit30 = X.bit62;
		X_next->bit31 = X.bit63;
		X_next->bit32 = X.bit0^D->bit0;
		X_next->bit33 = X.bit0^D->bit0^X.bit1^D->bit1;
		X_next->bit34 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2;
		X_next->bit35 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3;
		X_next->bit36 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4;
		X_next->bit37 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5;
		X_next->bit38 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6;
		X_next->bit39 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7;
		X_next->bit40 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8;
		X_next->bit41 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9;
		X_next->bit42 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10;
		X_next->bit43 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11;
		X_next->bit44 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12;
		X_next->bit45 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13;
		X_next->bit46 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14;
		X_next->bit47 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15;
		X_next->bit48 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16;
		X_next->bit49 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17;
		X_next->bit50 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18;
		X_next->bit51 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19;
		X_next->bit52 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20;
		X_next->bit53 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21;
		X_next->bit54 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22;
		X_next->bit55 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23;
		X_next->bit56 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24;
		X_next->bit57 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25;
		X_next->bit58 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26;
		X_next->bit59 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27;
		X_next->bit60 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22^X.bit26^D->bit26^X.bit27^D->bit27^X.bit28^D->bit28;
		X_next->bit61 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23^X.bit27^D->bit27^X.bit28^D->bit28^X.bit29^D->bit29;
		X_next->bit62 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24^X.bit28^D->bit28^X.bit29^D->bit29^X.bit30^D->bit30;
		X_next->bit63 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25^X.bit29^D->bit29^X.bit30^D->bit30^X.bit31^D->bit31;
	}
	else {  // 64-bit I/O
		// update states of 64 shift registers
		X_next->bit0 = X.bit0^D->bit0^X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22^X.bit26^D->bit26^X.bit27^D->bit27^X.bit28^D->bit28^X.bit32^D->bit32^X.bit33^D->bit33^X.bit34^D->bit34^X.bit38^D->bit38^X.bit39^D->bit39^X.bit40^D->bit40^X.bit44^D->bit44^X.bit45^D->bit45^X.bit46^D->bit46^X.bit50^D->bit50^X.bit51^D->bit51^X.bit52^D->bit52^X.bit56^D->bit56^X.bit57^D->bit57^X.bit58^D->bit58^X.bit62^D->bit62^X.bit63^D->bit63;
		X_next->bit1 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit5^D->bit5^X.bit8^D->bit8^X.bit11^D->bit11^X.bit14^D->bit14^X.bit17^D->bit17^X.bit20^D->bit20^X.bit23^D->bit23^X.bit26^D->bit26^X.bit29^D->bit29^X.bit32^D->bit32^X.bit35^D->bit35^X.bit38^D->bit38^X.bit41^D->bit41^X.bit44^D->bit44^X.bit47^D->bit47^X.bit50^D->bit50^X.bit53^D->bit53^X.bit56^D->bit56^X.bit59^D->bit59^X.bit62^D->bit62;
		X_next->bit2 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit6^D->bit6^X.bit9^D->bit9^X.bit12^D->bit12^X.bit15^D->bit15^X.bit18^D->bit18^X.bit21^D->bit21^X.bit24^D->bit24^X.bit27^D->bit27^X.bit30^D->bit30^X.bit33^D->bit33^X.bit36^D->bit36^X.bit39^D->bit39^X.bit42^D->bit42^X.bit45^D->bit45^X.bit48^D->bit48^X.bit51^D->bit51^X.bit54^D->bit54^X.bit57^D->bit57^X.bit60^D->bit60^X.bit63^D->bit63;
		X_next->bit3 = X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27^X.bit31^D->bit31^X.bit32^D->bit32^X.bit33^D->bit33^X.bit37^D->bit37^X.bit38^D->bit38^X.bit39^D->bit39^X.bit43^D->bit43^X.bit44^D->bit44^X.bit45^D->bit45^X.bit49^D->bit49^X.bit50^D->bit50^X.bit51^D->bit51^X.bit55^D->bit55^X.bit56^D->bit56^X.bit57^D->bit57^X.bit61^D->bit61^X.bit62^D->bit62^X.bit63^D->bit63;
		X_next->bit4 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4;
		X_next->bit5 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5;
		X_next->bit6 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6;
		X_next->bit7 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7;
		X_next->bit8 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8;
		X_next->bit9 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9;
		X_next->bit10 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10;
		X_next->bit11 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11;
		X_next->bit12 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12;
		X_next->bit13 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13;
		X_next->bit14 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14;
		X_next->bit15 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15;
		X_next->bit16 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16;
		X_next->bit17 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17;
		X_next->bit18 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18;
		X_next->bit19 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19;
		X_next->bit20 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20;
		X_next->bit21 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21;
		X_next->bit22 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22;
		X_next->bit23 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23;
		X_next->bit24 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24;
		X_next->bit25 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25;
		X_next->bit26 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26;
		X_next->bit27 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27;
		X_next->bit28 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22^X.bit26^D->bit26^X.bit27^D->bit27^X.bit28^D->bit28;
		X_next->bit29 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23^X.bit27^D->bit27^X.bit28^D->bit28^X.bit29^D->bit29;
		X_next->bit30 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24^X.bit28^D->bit28^X.bit29^D->bit29^X.bit30^D->bit30;
		X_next->bit31 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25^X.bit29^D->bit29^X.bit30^D->bit30^X.bit31^D->bit31;
		X_next->bit32 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26^X.bit30^D->bit30^X.bit31^D->bit31^X.bit32^D->bit32;
		X_next->bit33 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27^X.bit31^D->bit31^X.bit32^D->bit32^X.bit33^D->bit33;
		X_next->bit34 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22^X.bit26^D->bit26^X.bit27^D->bit27^X.bit28^D->bit28^X.bit32^D->bit32^X.bit33^D->bit33^X.bit34^D->bit34;
		X_next->bit35 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23^X.bit27^D->bit27^X.bit28^D->bit28^X.bit29^D->bit29^X.bit33^D->bit33^X.bit34^D->bit34^X.bit35^D->bit35;
		X_next->bit36 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24^X.bit28^D->bit28^X.bit29^D->bit29^X.bit30^D->bit30^X.bit34^D->bit34^X.bit35^D->bit35^X.bit36^D->bit36;
		X_next->bit37 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25^X.bit29^D->bit29^X.bit30^D->bit30^X.bit31^D->bit31^X.bit35^D->bit35^X.bit36^D->bit36^X.bit37^D->bit37;
		X_next->bit38 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26^X.bit30^D->bit30^X.bit31^D->bit31^X.bit32^D->bit32^X.bit36^D->bit36^X.bit37^D->bit37^X.bit38^D->bit38;
		X_next->bit39 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27^X.bit31^D->bit31^X.bit32^D->bit32^X.bit33^D->bit33^X.bit37^D->bit37^X.bit38^D->bit38^X.bit39^D->bit39;
		X_next->bit40 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22^X.bit26^D->bit26^X.bit27^D->bit27^X.bit28^D->bit28^X.bit32^D->bit32^X.bit33^D->bit33^X.bit34^D->bit34^X.bit38^D->bit38^X.bit39^D->bit39^X.bit40^D->bit40;
		X_next->bit41 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23^X.bit27^D->bit27^X.bit28^D->bit28^X.bit29^D->bit29^X.bit33^D->bit33^X.bit34^D->bit34^X.bit35^D->bit35^X.bit39^D->bit39^X.bit40^D->bit40^X.bit41^D->bit41;
		X_next->bit42 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24^X.bit28^D->bit28^X.bit29^D->bit29^X.bit30^D->bit30^X.bit34^D->bit34^X.bit35^D->bit35^X.bit36^D->bit36^X.bit40^D->bit40^X.bit41^D->bit41^X.bit42^D->bit42;
		X_next->bit43 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25^X.bit29^D->bit29^X.bit30^D->bit30^X.bit31^D->bit31^X.bit35^D->bit35^X.bit36^D->bit36^X.bit37^D->bit37^X.bit41^D->bit41^X.bit42^D->bit42^X.bit43^D->bit43;
		X_next->bit44 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26^X.bit30^D->bit30^X.bit31^D->bit31^X.bit32^D->bit32^X.bit36^D->bit36^X.bit37^D->bit37^X.bit38^D->bit38^X.bit42^D->bit42^X.bit43^D->bit43^X.bit44^D->bit44;
		X_next->bit45 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27^X.bit31^D->bit31^X.bit32^D->bit32^X.bit33^D->bit33^X.bit37^D->bit37^X.bit38^D->bit38^X.bit39^D->bit39^X.bit43^D->bit43^X.bit44^D->bit44^X.bit45^D->bit45;
		X_next->bit46 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22^X.bit26^D->bit26^X.bit27^D->bit27^X.bit28^D->bit28^X.bit32^D->bit32^X.bit33^D->bit33^X.bit34^D->bit34^X.bit38^D->bit38^X.bit39^D->bit39^X.bit40^D->bit40^X.bit44^D->bit44^X.bit45^D->bit45^X.bit46^D->bit46;
		X_next->bit47 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23^X.bit27^D->bit27^X.bit28^D->bit28^X.bit29^D->bit29^X.bit33^D->bit33^X.bit34^D->bit34^X.bit35^D->bit35^X.bit39^D->bit39^X.bit40^D->bit40^X.bit41^D->bit41^X.bit45^D->bit45^X.bit46^D->bit46^X.bit47^D->bit47;
		X_next->bit48 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24^X.bit28^D->bit28^X.bit29^D->bit29^X.bit30^D->bit30^X.bit34^D->bit34^X.bit35^D->bit35^X.bit36^D->bit36^X.bit40^D->bit40^X.bit41^D->bit41^X.bit42^D->bit42^X.bit46^D->bit46^X.bit47^D->bit47^X.bit48^D->bit48;
		X_next->bit49 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25^X.bit29^D->bit29^X.bit30^D->bit30^X.bit31^D->bit31^X.bit35^D->bit35^X.bit36^D->bit36^X.bit37^D->bit37^X.bit41^D->bit41^X.bit42^D->bit42^X.bit43^D->bit43^X.bit47^D->bit47^X.bit48^D->bit48^X.bit49^D->bit49;
		X_next->bit50 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26^X.bit30^D->bit30^X.bit31^D->bit31^X.bit32^D->bit32^X.bit36^D->bit36^X.bit37^D->bit37^X.bit38^D->bit38^X.bit42^D->bit42^X.bit43^D->bit43^X.bit44^D->bit44^X.bit48^D->bit48^X.bit49^D->bit49^X.bit50^D->bit50;
		X_next->bit51 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27^X.bit31^D->bit31^X.bit32^D->bit32^X.bit33^D->bit33^X.bit37^D->bit37^X.bit38^D->bit38^X.bit39^D->bit39^X.bit43^D->bit43^X.bit44^D->bit44^X.bit45^D->bit45^X.bit49^D->bit49^X.bit50^D->bit50^X.bit51^D->bit51;
		X_next->bit52 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22^X.bit26^D->bit26^X.bit27^D->bit27^X.bit28^D->bit28^X.bit32^D->bit32^X.bit33^D->bit33^X.bit34^D->bit34^X.bit38^D->bit38^X.bit39^D->bit39^X.bit40^D->bit40^X.bit44^D->bit44^X.bit45^D->bit45^X.bit46^D->bit46^X.bit50^D->bit50^X.bit51^D->bit51^X.bit52^D->bit52;
		X_next->bit53 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23^X.bit27^D->bit27^X.bit28^D->bit28^X.bit29^D->bit29^X.bit33^D->bit33^X.bit34^D->bit34^X.bit35^D->bit35^X.bit39^D->bit39^X.bit40^D->bit40^X.bit41^D->bit41^X.bit45^D->bit45^X.bit46^D->bit46^X.bit47^D->bit47^X.bit51^D->bit51^X.bit52^D->bit52^X.bit53^D->bit53;
		X_next->bit54 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24^X.bit28^D->bit28^X.bit29^D->bit29^X.bit30^D->bit30^X.bit34^D->bit34^X.bit35^D->bit35^X.bit36^D->bit36^X.bit40^D->bit40^X.bit41^D->bit41^X.bit42^D->bit42^X.bit46^D->bit46^X.bit47^D->bit47^X.bit48^D->bit48^X.bit52^D->bit52^X.bit53^D->bit53^X.bit54^D->bit54;
		X_next->bit55 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25^X.bit29^D->bit29^X.bit30^D->bit30^X.bit31^D->bit31^X.bit35^D->bit35^X.bit36^D->bit36^X.bit37^D->bit37^X.bit41^D->bit41^X.bit42^D->bit42^X.bit43^D->bit43^X.bit47^D->bit47^X.bit48^D->bit48^X.bit49^D->bit49^X.bit53^D->bit53^X.bit54^D->bit54^X.bit55^D->bit55;
		X_next->bit56 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26^X.bit30^D->bit30^X.bit31^D->bit31^X.bit32^D->bit32^X.bit36^D->bit36^X.bit37^D->bit37^X.bit38^D->bit38^X.bit42^D->bit42^X.bit43^D->bit43^X.bit44^D->bit44^X.bit48^D->bit48^X.bit49^D->bit49^X.bit50^D->bit50^X.bit54^D->bit54^X.bit55^D->bit55^X.bit56^D->bit56;
		X_next->bit57 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27^X.bit31^D->bit31^X.bit32^D->bit32^X.bit33^D->bit33^X.bit37^D->bit37^X.bit38^D->bit38^X.bit39^D->bit39^X.bit43^D->bit43^X.bit44^D->bit44^X.bit45^D->bit45^X.bit49^D->bit49^X.bit50^D->bit50^X.bit51^D->bit51^X.bit55^D->bit55^X.bit56^D->bit56^X.bit57^D->bit57;
		X_next->bit58 = X.bit2^D->bit2^X.bit3^D->bit3^X.bit4^D->bit4^X.bit8^D->bit8^X.bit9^D->bit9^X.bit10^D->bit10^X.bit14^D->bit14^X.bit15^D->bit15^X.bit16^D->bit16^X.bit20^D->bit20^X.bit21^D->bit21^X.bit22^D->bit22^X.bit26^D->bit26^X.bit27^D->bit27^X.bit28^D->bit28^X.bit32^D->bit32^X.bit33^D->bit33^X.bit34^D->bit34^X.bit38^D->bit38^X.bit39^D->bit39^X.bit40^D->bit40^X.bit44^D->bit44^X.bit45^D->bit45^X.bit46^D->bit46^X.bit50^D->bit50^X.bit51^D->bit51^X.bit52^D->bit52^X.bit56^D->bit56^X.bit57^D->bit57^X.bit58^D->bit58;
		X_next->bit59 = X.bit3^D->bit3^X.bit4^D->bit4^X.bit5^D->bit5^X.bit9^D->bit9^X.bit10^D->bit10^X.bit11^D->bit11^X.bit15^D->bit15^X.bit16^D->bit16^X.bit17^D->bit17^X.bit21^D->bit21^X.bit22^D->bit22^X.bit23^D->bit23^X.bit27^D->bit27^X.bit28^D->bit28^X.bit29^D->bit29^X.bit33^D->bit33^X.bit34^D->bit34^X.bit35^D->bit35^X.bit39^D->bit39^X.bit40^D->bit40^X.bit41^D->bit41^X.bit45^D->bit45^X.bit46^D->bit46^X.bit47^D->bit47^X.bit51^D->bit51^X.bit52^D->bit52^X.bit53^D->bit53^X.bit57^D->bit57^X.bit58^D->bit58^X.bit59^D->bit59;
		X_next->bit60 = X.bit0^D->bit0^X.bit4^D->bit4^X.bit5^D->bit5^X.bit6^D->bit6^X.bit10^D->bit10^X.bit11^D->bit11^X.bit12^D->bit12^X.bit16^D->bit16^X.bit17^D->bit17^X.bit18^D->bit18^X.bit22^D->bit22^X.bit23^D->bit23^X.bit24^D->bit24^X.bit28^D->bit28^X.bit29^D->bit29^X.bit30^D->bit30^X.bit34^D->bit34^X.bit35^D->bit35^X.bit36^D->bit36^X.bit40^D->bit40^X.bit41^D->bit41^X.bit42^D->bit42^X.bit46^D->bit46^X.bit47^D->bit47^X.bit48^D->bit48^X.bit52^D->bit52^X.bit53^D->bit53^X.bit54^D->bit54^X.bit58^D->bit58^X.bit59^D->bit59^X.bit60^D->bit60;
		X_next->bit61 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit5^D->bit5^X.bit6^D->bit6^X.bit7^D->bit7^X.bit11^D->bit11^X.bit12^D->bit12^X.bit13^D->bit13^X.bit17^D->bit17^X.bit18^D->bit18^X.bit19^D->bit19^X.bit23^D->bit23^X.bit24^D->bit24^X.bit25^D->bit25^X.bit29^D->bit29^X.bit30^D->bit30^X.bit31^D->bit31^X.bit35^D->bit35^X.bit36^D->bit36^X.bit37^D->bit37^X.bit41^D->bit41^X.bit42^D->bit42^X.bit43^D->bit43^X.bit47^D->bit47^X.bit48^D->bit48^X.bit49^D->bit49^X.bit53^D->bit53^X.bit54^D->bit54^X.bit55^D->bit55^X.bit59^D->bit59^X.bit60^D->bit60^X.bit61^D->bit61;
		X_next->bit62 = X.bit0^D->bit0^X.bit1^D->bit1^X.bit2^D->bit2^X.bit6^D->bit6^X.bit7^D->bit7^X.bit8^D->bit8^X.bit12^D->bit12^X.bit13^D->bit13^X.bit14^D->bit14^X.bit18^D->bit18^X.bit19^D->bit19^X.bit20^D->bit20^X.bit24^D->bit24^X.bit25^D->bit25^X.bit26^D->bit26^X.bit30^D->bit30^X.bit31^D->bit31^X.bit32^D->bit32^X.bit36^D->bit36^X.bit37^D->bit37^X.bit38^D->bit38^X.bit42^D->bit42^X.bit43^D->bit43^X.bit44^D->bit44^X.bit48^D->bit48^X.bit49^D->bit49^X.bit50^D->bit50^X.bit54^D->bit54^X.bit55^D->bit55^X.bit56^D->bit56^X.bit60^D->bit60^X.bit61^D->bit61^X.bit62^D->bit62;
		X_next->bit63 = X.bit1^D->bit1^X.bit2^D->bit2^X.bit3^D->bit3^X.bit7^D->bit7^X.bit8^D->bit8^X.bit9^D->bit9^X.bit13^D->bit13^X.bit14^D->bit14^X.bit15^D->bit15^X.bit19^D->bit19^X.bit20^D->bit20^X.bit21^D->bit21^X.bit25^D->bit25^X.bit26^D->bit26^X.bit27^D->bit27^X.bit31^D->bit31^X.bit32^D->bit32^X.bit33^D->bit33^X.bit37^D->bit37^X.bit38^D->bit38^X.bit39^D->bit39^X.bit43^D->bit43^X.bit44^D->bit44^X.bit45^D->bit45^X.bit49^D->bit49^X.bit50^D->bit50^X.bit51^D->bit51^X.bit55^D->bit55^X.bit56^D->bit56^X.bit57^D->bit57^X.bit61^D->bit61^X.bit62^D->bit62^X.bit63^D->bit63;
	}
}

