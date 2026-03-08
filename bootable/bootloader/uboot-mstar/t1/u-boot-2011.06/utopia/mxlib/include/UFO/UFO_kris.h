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
/////////////////////////////////////////////////////////
/// GFX function
/////////////////////////////////////////////////////////
#define UFO_GFX_TRIANGLE
#define UFO_GFX_SPAN
#define UFO_KRIS_FECINFO
//2016/3/17 modify from kratos
#define UFO_DEMOD_BLINDSCAN_NEW_FLOW
#define UFO_DEMOD_GetParam_NEW_FLOW
/////////////////////////////////////////////////////////
/// VE lib feature define
/////////////////////////////////////////////////////////
#define UFO_VE_INITEX //extension for VE intialization

///////////////////////////////////////////////////////////
///DEMOD lib feature define
//////////////////////////////////////////////////////////
//#define UFO_DEMOD_DVBS_SUPPORT_DMD_INT
#define UFO_DEMOD_DVBS_SUPPORT_DISEQC_RX
#define UFO_DEMOD_GET_SIGNAL_LEVEL_PWR
#define UFO_DEMOD_GET_AGC_INFO
#define UFO_DEMOD_DVBS_UNICABLE_SEND_CMD
#define UFO_DEMOD_DVBS_CUSTOMIZED_DISEQC_SEND_CMD
#endif // _UFO_H_
