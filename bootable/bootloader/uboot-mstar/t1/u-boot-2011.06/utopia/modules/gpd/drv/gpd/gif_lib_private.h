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
#ifndef _GIF_LIB_PRIVATE_H
#define _GIF_LIB_PRIVATE_H

#include "gif_lib.h"

#define PROGRAM_NAME	"GIFLIB"

#define LZ_MAX_CODE	4095		/* Biggest code possible in 12 bits. */
#define LZ_BITS		12

#define FLUSH_OUTPUT		4096    /* Impossible code, to signal flush. */
#define FIRST_CODE		4097    /* Impossible code, to signal first. */
#define NO_SUCH_CODE		4098    /* Impossible code, to signal empty. */

#define FILE_STATE_WRITE    0x01
#define FILE_STATE_SCREEN   0x02
#define FILE_STATE_IMAGE    0x04
#define FILE_STATE_READ     0x08

#define IS_READABLE(Private)    (Private->FileState & FILE_STATE_READ)
#define IS_WRITEABLE(Private)   (Private->FileState & FILE_STATE_WRITE)


typedef struct GifFilePrivateType {
    MS_U32 FileState,
	FileHandle,			     /* Where all this data goes to! */
	BitsPerPixel,	    /* Bits per pixel (Codes uses at least this + 1). */
	ClearCode,				       /* The CLEAR LZ code. */
	EOFCode,				         /* The EOF LZ code. */
	RunningCode,		    /* The next code algorithm can generate. */
	RunningBits,/* The number of bits required to represent RunningCode. */
	MaxCode1,  /* 1 bigger than max. possible code, in RunningBits bits. */
	LastCode,		        /* The code before the current code. */
	CrntCode,				  /* Current algorithm code. */
	StackPtr,		         /* For character stack (see below). */
	CrntShiftState;		        /* Number of bits in CrntShiftDWord. */
    MS_U32 CrntShiftDWord;     /* For bytes decomposition into codes. */
    MS_U32 PixelCount;		       /* Number of pixels in image. */
    //FILE *File;						  /* File as stream. */
    InputFunc Read;                 /* function to read gif input (TVT) */
    OutputFunc Write;               /* function to write gif output (MRB) */
    GifByteType Buf[256];	       /* Compressed input is buffered here. */
    GifByteType Stack[LZ_MAX_CODE];	 /* Decoded pixels are stacked here. */
    GifByteType Suffix[LZ_MAX_CODE+1];	       /* So we can trace the codes. */
    MS_U32 Prefix[LZ_MAX_CODE+1];
} GifFilePrivateType;

extern MS_U32 _MstGifError;

#ifdef SYSV
static char *VersionStr =
        "Gif library module,\t\tEric S. Raymond\n\
	(C) Copyright 1997 Eric S. Raymond\n";
#else
#if 0
static char *VersionStr =
	PROGRAM_NAME
	"	IBMPC "
	GIF_LIB_VERSION
	"	Eric S. Raymond,	"
	__DATE__ ",   " __TIME__ "\n"
	"(C) Copyright 1997 Eric S. Raymond\n";
#endif
#endif /* SYSV */

#endif /* _GIF_LIB_PRIVATE_H */
