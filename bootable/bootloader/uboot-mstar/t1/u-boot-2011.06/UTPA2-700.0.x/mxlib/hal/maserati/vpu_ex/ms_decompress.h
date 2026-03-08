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

#ifndef __MS_DECOMPRESS_H__
#define __MS_DECOMPRESS_H__

MS_BOOL ms_VDECDecompressInit(MS_U8 *pSliding, MS_U8 *pOut);
int ms_VDECDecompress(const MS_U8 *pInStream, MS_U32 u32DataLen);
void ms_VDECDecompressDeInit(void);


#endif /* __MS_DECOMPRESS_H__ */
