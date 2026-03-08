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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef DRV_DLC_H
#define DRV_DLC_H


#include "MsTypes.h"

/////////////////////////////////////////////////////////////////////////////////
// Global define
/////////////////////////////////////////////////////////////////////////////////

#define DLC_DISABLE             0
#define DLC_32SEG_HIS_MODE      1
#define DLC_8SEG_HIS_MODE       2

#ifndef ENABLE_DLC
    #define ENABLE_DLC          DLC_32SEG_HIS_MODE//DLC_8SEG_HIS_MODE
#endif

/////////////////////////////////////////////////////////////////////////////////
// Local define to transform data type between utopia & Color teams's DLC library.
/////////////////////////////////////////////////////////////////////////////////

#ifndef BOOL
#define BOOL MS_BOOL
#define DEFINE_BOOL 1
#endif

#ifndef BYTE
#define BYTE MS_U8
#define DEFINE_BYTE 1
#endif

#ifndef WORD
#define WORD MS_U16
#define DEFINE_WORD 1
#endif

#ifndef XDATA
#define XDATA
#define DEFINE_XDATA 1
#endif

/////////////////////////////////////////////////////////////////////////////////
// Files needed to sync with Jacky Kao's DLC library.
/////////////////////////////////////////////////////////////////////////////////
#include "MsDlc.h"

/////////////////////////////////////////////////////////////////////////////////
// Remove Local define.
/////////////////////////////////////////////////////////////////////////////////

#if DEFINE_BOOL
#undef BOOL 
#undef DEFINE_BOOL
#endif

#if DEFINE_BYTE
#undef BYTE 
#undef DEFINE_BYTE
#endif

#if DEFINE_WORD
#undef WORD 
#undef DEFINE_WORD
#endif

#if DEFINE_XDATA
#undef XDATA
#undef DEFINE_XDATA
#endif

#endif // DRV_DLC_H
