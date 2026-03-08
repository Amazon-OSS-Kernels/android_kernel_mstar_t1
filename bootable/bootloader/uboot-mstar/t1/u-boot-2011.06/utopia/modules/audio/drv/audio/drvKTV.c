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
//******************************************************************************
//
//  [Module Name]:
//      drvKTV.c
//  [Abstract]:
//      This module contains code for Audio Processor driver
//      procedure and subroutin
//  [Author(s)]:
//      Allen Chen
//  [Reversion History]:
//      Initial release:    15 Aprial, 2009
//
//  [Doxygen]
/// file drvKTV.c
/// @brief Driver for KTV functions
/// @author MStarSemi Inc.
//*******************************************************************************

#define _DRVKTV_C_

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
// Internal
#include "drvAUDIO.h"
#include "drvAUDIO_if.h"

#include "./internal/drvKTV.h"
#include "./internal/drvSOUND.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvAUDIO_internal.h"

#include "halAUDIO.h"
#include "halMAD2.h"
#include "halKTV.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetKTVInfo()
/// @brief \b Function \b Description: This routine is used to set KTV parameter to DSP .
/// @param <IN>        \b modelType: KTV model type (T3, U3, Janus)
/// @param <IN>        \b infoType : KTV info type
/// @param <IN>        \b param1   : KTV parameter1
/// @param <IN>        \b param2   : KTV parameter2
/// @param <OUT>       \b NONE     :
/// @param <RET>       \b NONE     :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_SetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
{
    return (HAL_KTV_SetKTVInfo(modelType, paramType, param1, param2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetKTVInfo()
/// @brief \b Function \b Description: This routine is used to get KTV data from DSP .
/// @param <IN>        \b modelType: KTV model type (T3, U3, Janus)
/// @param <IN>        \b infoType : KTV info type
/// @param <OUT>       \b NONE     :
/// @param <RET>       \b MS_U32   : Return parameter
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_AUDIO_GetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType)
{
    return (HAL_KTV_GetKTVInfo (modelType, infoType));
}

