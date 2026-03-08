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
#ifndef _OUT_STREAM_H_
#define _OUT_STREAM_H_

#include "MFE_chip.h"
#include "mfe_type.h"

typedef struct _OutStream {
	// 1st Level buffer (32-bit)
    int m_nBufferEmptyBits;
    MFE_U32 m_SmallBuffer;

	// 2nd level buffer
    MFE_U8* m_pbFrameBuffer;
    MFE_U32 m_nByteCount;

	// Bit count
    MFE_U32 BC_nCumulativeBits;

	// Endian flag
    MFE_U8 m_bUseLittleEndian;
} OutStream;


void osCreate(OutStream* pStream);
void osDestroy(OutStream* pStream);

void osSetWriteBuffer(OutStream* pStream, MFE_BYTE *pWBuf);
// Reset
void osReset(OutStream* pStream);
// write all remaining bits in buffer_empty_bit
void osFlushAll(OutStream* pStream);

MFE_U32 GetBitsCount(OutStream* pStream);

// put bits number of bits to output buffer , at almost 32 bits
void osPutBits(OutStream* pStream, MFE_U32 value, int bits, char* name);

// stuffing '0' to byte align about size byte...
// zero means align to the next byte...
// for byte aligned
void osStuffing(OutStream* pStream);

void osNextStartCode(OutStream* pStream);


#endif
