/**
 * Copyright (c) 2006 <96> 2016 MStar Semiconductor, Inc.
 * This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

//
//  [for Doxygen]
/// @file drvSIF.h
/// @brief Subroutine for SIF
/// @author MStarSemi Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVSIF_H_
#define _DRVSIF_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsTypes.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO_internal.h"

#ifdef __cplusplus
extern "C"
{
#endif
////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

//================================================================
//  Enum
//================================================================

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    MDrv_SIF_Init(void);
MS_BOOL MDrv_SIF_ReLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
MS_BOOL MDrv_SIF_SetSoundMode(AUDIO_SIF_SOUND_MODE mode_type);
MS_BOOL MDrv_SIF_SetSystem(En_DVB_decSystemType SystemType);
MS_BOOL MDrv_SIF_StartAutoDetection (void);
MS_U32 MDrv_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE rw_standard_type, AUDIO_SIF_THRESHOLD_TYPE  u8Threshold_type, MS_U16 u16Value);
void    MDrv_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl, MS_U8 start_i, MS_U8 num, AUDIO_SIF_SYSTEM_TYPE standard_type);
MS_BOOL MDrv_SIF_GetOrginalGain(void);
MS_BOOL MDrv_SIF_SetPrescale(AUDIO_SIF_GAIN_TYPE gainType, MS_S32 db_value);

//////////////////////////////////////////////////////
// add for the Chakara2  & for the ¦n¥Í¬¡ coding
MS_BOOL MDrv_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type);
MS_BOOL MDrv_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type);
void  MDrv_SIF_SetStandardType(AUDIO_SIF_SET_STANDARD sif_type);
MS_U8 MDrv_SIF_GetDspType(void);
AUDIO_DSP_CODE_TYPE MDrv_SIF_GetDspCodeType(void);

MS_U8 MDrv_SIF_SendCmd(AU_SIF_CMD u8DecCmd, MS_U8 comm_arg1, MS_U8 comm_arg2);
MS_BOOL MDrv_SIF_CheckDecSys(En_DVB_decSystemType enDecSystem);

#ifdef __cplusplus
}
#endif

#endif
