/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef __MS_DECOMPRESS_H__
#define __MS_DECOMPRESS_H__

BOOLEAN ms_DecompressInit(U8 *pOutStream);
int ms_Decompress(const U8 *pInStream, U32 u32DataLen);
void ms_DecompressDeInit(void);

BOOLEAN DoMsDecompression(U8 *pSrc, U8 *pDst, U8 *pTmp, U32 srclen);
BOOLEAN DoMsDecompression7(U8 *pSrc, U8 *pDst, U8 *pTmp, U32 srclen);

#endif /* __MS_DECOMPRESS_H__ */
