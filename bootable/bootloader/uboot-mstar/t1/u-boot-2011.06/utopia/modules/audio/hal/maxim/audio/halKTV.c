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
//      halKTV.c
//  [Abstract]:
//      This module contains code for Audio Processor driver
//      procedure and subroutin
//  [Author(s)]:
//      Desker Chuo
//  [Reversion History]:
//      Initial release:    15 July, 2005
//
//  [Doxygen]
/// @file DrvAuProcess.h
/// @brief Subroutine for sound effect
/// @author MStarSemi Inc.
///
/// This module contains code for Audio Processor driver
/// procedure and subroutine.
///
///@par Example
///@code
///  // Audio set mute
///  // Input: enable/disable
///  void msAPI_AUD_SetMute(BOOLEAN mute_en)
///  {
///      // Set mute enable for audio path 0
///      HAL_AUDIO_ProcessSetMute( AUDIO_PATH_0, mute_en );
///     MDrv_VAmp_Mute(mute_en);
///  }
///@endcode
//*******************************************************************************

#define _HALKTV_C_

// Internal
#include "halAUDIO.h"
#include "halAUR2.h"
#include "halKTV.h"

///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_KTV_SetKTVInfo()
/// @brief \b Function \b Description: This routine set different paramter to ktv module
/// @param <IN>        \b AUDIO_KTV_MODEL    : common ktv module type
/// @param <IN>        \b AUDIO_KTV_PARAMTYPE: common kt  parameter type
/// @param <IN>        \b MS_U32    : common ktv parameter 1
/// @param <IN>        \b MS_U32    : common ktv parameter 2
/// @param <OUT>       \b NONE      :
/// @param <RET>       \b MS_BOOL   : TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE      :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_KTV_SetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
{
    switch (modelType)
    {
    /* For T12 Multak KTV */
    case AUD_KTV_MODEL2: 
        switch (paramType)
        {
        case AUD_KTV_ParamType1:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_CUSFUNC_xxxx, AASE1, (MS_U16)param1, (MS_U16)param2);
            break;        
        case AUD_KTV_ParamType2:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_CUSFUNC_xxxx2, AASE1, (MS_U16)param1, (MS_U16)param2);
            break;
            
        default:
            break;
        }
        break;
        
    default:
        break;
    }
    
    return (TRUE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_KTV_GetKTVInfo()
/// @brief \b Function \b Description: This routine is used to get KTV data from DSP .
/// @param <IN>        \b modelType: KTV model type 
/// @param <IN>        \b infoType : KTV info type
/// @param <OUT>       \b NONE     :
/// @param <RET>       \b MS_U32   : Return parameter
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_KTV_GetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType)
{
    MS_U32 result = FALSE;

    switch (modelType)
    {
    /* For T12 Multak KTV */
    case AUD_KTV_MODEL2:
        switch (infoType)
        {
        case AUD_KTV_infoType1:
            result = HAL_SND_R2_GetCommInfo(AUR2_CommInfo_AASE_CUSFUNC_xxxx, AASE1, 0, 0);
            break;        
            
        case AUD_KTV_infoType2:
            result = HAL_SND_R2_GetCommInfo(AUR2_CommInfo_AASE_CUSFUNC_xxxx2, AASE1, 0, 0);
            break;        
            
        default:
            break;
        }
        break;

    default:
        break;
    }

    return (result);
}

