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
//------------------------------------------------------------------------------
// jpegmain.h
// Last updated: Nov. 16, 2000
// Copyright (C) 1994-2000 Rich Geldreich
// richgel@voicenet.com
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//------------------------------------------------------------------------------

#ifndef JPEGMAIN_H
#define JPEGMAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jpeg_cmodel_def.h"

/*
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <assert.h>
*/
//------------------------------------------------------------------------------
// Uncomment to disable output file writing (for VTune benchmarking)
//#define NO_OUTPUT

//typedef   signed short int16;       /* 16 bits     */
//typedef unsigned int   uint;        /* 16/32+ bits */

#ifndef max
#define max(a,b) (((a)>(b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a)<(b)) ? (a) : (b))
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef true
#define true (1)
#endif

#ifndef false
#define false (0)
#endif

#define MS_ASSERT(j)

#define JPEG_CMODEL_DEBUG(format, args...)  do{printf(format, ##args);}while(0)

#endif

