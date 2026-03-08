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
#ifndef _MFE_TYPE_H_
#define _MFE_TYPE_H_

#include "MFE_chip.h"

#if defined(_MFE_UTOPIA_)

#include "MsTypes.h"
#include <stdint.h>

/// data type unsigned char, data length 1 byte
typedef MS_U8       MFE_U8;                                  // 1 byte
/// data type unsigned short, data length 2 byte
typedef MS_U16     MFE_U16;                                 // 2 bytes
/// data type unsigned int, data length 4 byte
typedef MS_U32     MFE_U32;                                 // 4 bytes
/// data type unsigned int, data length 8 byte
typedef MS_U64     MFE_U64;                                 // 8 bytes
/// data type signed char, data length 1 byte
typedef MS_S8      MFE_S8;                                  // 1 byte
/// data type signed short, data length 2 byte
typedef MS_S16     MFE_S16;                                 // 2 bytes
/// data type signed int, data length 4 byte
typedef MS_S32     MFE_S32;                                 // 4 bytes

/// data type signed int, data length 4 byte
typedef MS_S64     MFE_S64;                                 // 4 bytes

/// definition for MS_BOOL, this will be removed later
typedef MS_BOOL   MFE_BOOL;

typedef MS_U8   MFE_BYTE;
typedef MS_U16  MFE_WORD;
typedef MS_U32   MFE_DWORD;

#elif defined(_MFE_LG_)
#include "mdrv_types.h"
#include <stdint.h>

/// data type unsigned char, data length 1 byte
typedef U8       MFE_U8;                                  // 1 byte
/// data type unsigned short, data length 2 byte
typedef U16     MFE_U16;                                 // 2 bytes
/// data type unsigned int, data length 4 byte
typedef U32     MFE_U32;                                 // 4 bytes
/// data type unsigned int, data length 8 byte
#if defined(CONFIG_Titania8)
typedef U64     MFE_U64;                                 // 8 bytes
#endif
/// data type signed char, data length 1 byte
typedef S8      MFE_S8;                                  // 1 byte
/// data type signed short, data length 2 byte
typedef S16     MFE_S16;                                 // 2 bytes
/// data type signed int, data length 4 byte
typedef S32     MFE_S32;                                 // 4 bytes
/// data type signed int, data length 4 byte
typedef long long     MFE_S64;                                 // 4 bytes
/// definition for MS_BOOL, this will be removed later
typedef BOOL   MFE_BOOL;

typedef U8   MFE_BYTE;
typedef U16  MFE_WORD;
typedef S32   MFE_DWORD;

#elif defined(_WIN32)
typedef unsigned char           MFE_U8;
typedef unsigned short          MFE_U16;
typedef unsigned int             MFE_U32;
typedef unsigned long long    MFE_U64;
typedef char                        MFE_S8;
typedef short                       MFE_S16;
typedef int                          MFE_S32 ;
typedef BOOL                      MFE_BOOL;

typedef unsigned char   MFE_BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;

#if __STDC_VERSION__ >= 199901L
typedef long long MFE_INT64;
#else
typedef __int64 MFE_INT64;
#endif

#endif

#if defined(_WIN32)&&defined(_MSC_VER)
#if (_MSC_VER < 1400) //vc 6 or older version
#define MFE_INT64_C(val) val##i64
#endif
#else
#define MFE_INT64_C(val) val##ULL
#endif


#ifndef _WIN32
	#define LOWORD(l)        ((unsigned short)(l))
	#define HIWORD(l)        ((unsigned short)(((unsigned int)(l) >> 16) & 0xFFFF))
	#define MAKELONG(a, b)    ((unsigned int)(((unsigned short)(a)) | ((unsigned int)((unsigned short)(b))) << 16))
#endif


#ifndef MFE_MAX
#define  MFE_MAX(a,b)              (((a) > (b)) ? (a) : (b))
#endif
#ifndef MFE_MIN
#define  MFE_MIN(a,b)              (((a) < (b)) ? (a) : (b))
#endif

#ifndef MFE_MUX
#define MFE_MUX(a, b, c)    ((a) ? (b) : (c))
#endif

#define dump(x) //for rate control debug




#define I_VOP		0		/* vop coding modes */
#define P_VOP		1
#define B_VOP           2       // assumed B VOP always without reconstructed

#endif // _MFE_TYPE_H_

