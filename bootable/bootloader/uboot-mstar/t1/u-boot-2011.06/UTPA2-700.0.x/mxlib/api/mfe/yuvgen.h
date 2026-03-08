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
void yuv2tile(/*FILE *fp*/unsigned char *output, int width, int height, unsigned char *buff , int Ypart);
void YuvGen(unsigned char seed, int width, int height, volatile unsigned char *buff);
void ConvertYuvFromTile(int width, int height, int mode, unsigned char *in_yuv, unsigned char *out_yuv);
#endif
