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


#ifndef _YUVGEN_H_
#define _YUVGEN_H_

// #define _MAIN_
#ifndef _MAIN_
//#include "drvM4VE.h"
#endif
void yuv2tile(/*FILE *fp*/MS_U8 *output, MS_S32 width, MS_S32 height, MS_U8 *buff , MS_S32 Ypart);
void YuvGen(MS_U8 seed, MS_S32 width, MS_S32 height, volatile MS_U8 *buff);
void ConvertYuvFromTile(MS_S32 width, MS_S32 height, MS_S32 mode, MS_U8 *in_yuv, MS_U8 *out_yuv);
MS_BOOL ConvertYV12ToHVDTile(MS_U8 *pSrc1, MS_U8 *pSrc2, MS_S32 nSrcWidth, MS_S32 nSrcHeight, MS_S32 nSrcStride, MS_U8 *pDst, MS_S32 nDstSize, MS_S32 YPlane);
#endif
