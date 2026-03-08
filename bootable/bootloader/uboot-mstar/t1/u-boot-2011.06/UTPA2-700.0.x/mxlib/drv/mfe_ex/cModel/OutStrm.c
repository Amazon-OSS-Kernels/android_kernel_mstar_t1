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



#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "ms_dprintf.h"

#include "OutStrm.h"

/********************************************************/
/* Implementation of class OutStream					*/
/********************************************************/

void osCreate(OutStream* pStream)
{
	// Check endian
	MS_U32 tmp = 0x04030201;
	if (*(MS_U8*)&tmp == 0x1)
		pStream->m_bUseLittleEndian = 1;
	else if (*(MS_U8*)&tmp == 0x4)
		pStream->m_bUseLittleEndian = 0;
	else {
		MS_ASSERT(0);
		pStream->m_bUseLittleEndian = 0;
	}
	ms_dprintk(DRV_L2,"OutStream: LittleEndian=%d\n", pStream->m_bUseLittleEndian);

	pStream->m_pbFrameBuffer = 0;
	pStream->m_nByteCount = 0;
	pStream->m_nBufferEmptyBits = 32;
	pStream->m_SmallBuffer = 0;
	pStream->BC_nCumulativeBits = 0;
}

void osDestroy(OutStream* pStream)
{
}

void osSetWriteBuffer(OutStream* pStream, MS_U8 *pWBuf)
{
	pStream->m_SmallBuffer = 0;
    pStream->m_nByteCount = 0;
    pStream->m_nBufferEmptyBits = 32;
	pStream->m_pbFrameBuffer = pWBuf;
}

void osReset(OutStream* pStream)
{
	pStream->m_SmallBuffer = 0;
    pStream->m_nByteCount = 0;
    pStream->m_nBufferEmptyBits = 32;
	pStream->BC_nCumulativeBits = 0;
}

void osFlushAll(OutStream* pStream)
{
    MS_S32 nUnusedByteCount = 0;
	MS_S32 nUnusedBits = pStream->m_nBufferEmptyBits;
    if (pStream->m_nBufferEmptyBits != 32)
    {
        if (pStream->m_nBufferEmptyBits>=24)
            nUnusedByteCount = 3;
        else if (pStream->m_nBufferEmptyBits>=16)
            nUnusedByteCount = 2;
        else if (pStream->m_nBufferEmptyBits>=8)
            nUnusedByteCount = 1;
        osPutBits(pStream, 0, pStream->m_nBufferEmptyBits, (MS_S8*)(""));
    }
    pStream->BC_nCumulativeBits -= nUnusedBits;
    pStream->m_nByteCount -= nUnusedByteCount;
}

void osPutBits(OutStream* pStream, MS_U32 value, MS_S32 bits, MS_S8* name)
{
//    MS_U32 *ptwrite;
    MS_U32 tlongbuf;

    // BitRate Control
    pStream->BC_nCumulativeBits += bits;
	//printf("PutBits() %s Val=%x Bits=%d\n", name, value, bits);

    MS_ASSERT(bits!=0);
//	if (bits) {
        //
        // Less than empty bits
        //
        if (bits < pStream->m_nBufferEmptyBits) {
            pStream->m_nBufferEmptyBits -= bits;
            pStream->m_SmallBuffer += value<<pStream->m_nBufferEmptyBits;
        }
		//
		// overflow of 32 bits. so value should be segement.
		//
		else if (bits>=pStream->m_nBufferEmptyBits)
	    {
            // to reduce code size, no shortcut if bits == m_nBufferEmptyBits
            // since the probability is low.
			pStream->m_SmallBuffer+=value>>(bits-pStream->m_nBufferEmptyBits); //shift right

//			ptwrite = (MS_U32 *)(pStream->m_pbFrameBuffer+pStream->m_nByteCount);
            tlongbuf = pStream->m_SmallBuffer;

			// ------ Little Endian in PC --------
			if (pStream->m_bUseLittleEndian) {
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 0] = (MS_U8)(tlongbuf >> 24);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 1] = (MS_U8)(tlongbuf >> 16);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 2] = (MS_U8)(tlongbuf >>  8);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 3] = (MS_U8)(tlongbuf);
			}
			else {
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 3] = (MS_U8)(tlongbuf >> 24);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 2] = (MS_U8)(tlongbuf >> 16);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 1] = (MS_U8)(tlongbuf >>  8);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 0] = (MS_U8)(tlongbuf);
			}

			// -----------------------------------
			pStream->m_nByteCount+=4;
            if (bits==pStream->m_nBufferEmptyBits) {
                pStream->m_nBufferEmptyBits = 32;
                pStream->m_SmallBuffer = 0;
            } else {
			    pStream->m_nBufferEmptyBits = 32 - bits + pStream->m_nBufferEmptyBits;
			    pStream->m_SmallBuffer = value << pStream->m_nBufferEmptyBits;
            }
		}
//	}
}

// 14496-2
void osNextStartCode(OutStream* pStream)
{
    osPutBits(pStream, 0, 1, (MS_S8*)("NextStartCode"));
    osStuffing(pStream);
}

MS_U32 GetBitsCount(OutStream* pStream)
{
    return pStream->BC_nCumulativeBits;
}

void osStuffing(OutStream* pStream)
{
	MS_S32 nStuffingBits = pStream->m_nBufferEmptyBits&7;
    if (pStream->m_nBufferEmptyBits & 7)
        osPutBits(pStream, 0xFF>>(8-nStuffingBits), nStuffingBits, (MS_S8*)("stuffing"));
}
