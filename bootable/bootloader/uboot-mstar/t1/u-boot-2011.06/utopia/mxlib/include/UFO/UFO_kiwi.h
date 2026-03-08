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
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   UFO.h
/// @brief  MStar Common Interface Header File
/// @note   utopia feature definition file
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UFO_H_
#define _UFO_H_

/////////////////////////////////////////////////////////
/// UTOPIA CODELINE, DO NOT MODIFY
/////////////////////////////////////////////////////////
#define UFO_PUBLIC_HEADER

/////////////////////////////////////////////////////////
/// DMX lib feature define
/////////////////////////////////////////////////////////
#define UFO_DMX_FQ
#define UFO_DMX_TSO
//#define UFO_DMX_TSO2
/////////////////////////////////////////////////////////
/// GFX function
/////////////////////////////////////////////////////////
#define UFO_GFX_TRIANGLE
#define UFO_GFX_SPAN

/////////////////////////////////////////////////////////
/// MVOP function
/////////////////////////////////////////////////////////
#define UFO_MVOP_10B_TO_8B

/////////////////////////////////////////////////////////
/// VE lib feature define
/////////////////////////////////////////////////////////
#define UFO_VE_INITEX //extension for VE intialization

#define UFO_DEMOD_DVBT_SUPPORT_DMD_INT
#define UFO_DEMOD_DVBC_SUPPORT_DMD_INT

#endif // _UFO_H_
