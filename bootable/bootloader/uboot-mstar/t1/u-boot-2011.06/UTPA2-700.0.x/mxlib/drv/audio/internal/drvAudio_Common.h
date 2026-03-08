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
/// @file drvAudio_Common.h
/// @brief Subroutine for advance sound effect
/// @author MStarSemi Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVIPCHECK_H_
#define _DRVIPCHECK_H_

#include "MsTypes.h"
#include "drvAUDIO_if.h"


#ifdef __cplusplus
extern "C"
{
#endif

//====== STAOS PIP START 2012/02/02 ========
MS_BOOL MDrv_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param );
MS_BOOL MDrv_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem);
MS_BOOL MDrv_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t * p_AudioDecStatus);
void  MDrv_SOUND_VDS_Enable(MS_U8 u8enVDS);
void  MDrv_SOUND_VSPK_Enable(MS_U8 u8enVSPK);
void  MDrv_SOUND_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value);
void  MDrv_SOUND_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value);
void  MDrv_SOUND_SRS_Enable(MS_U8 u8enSRS);
void  MDrv_SOUND_TSHD_Enable(MS_U8 u8enTSHD);
void  MDrv_SOUND_VSPK_SetWMod(MS_U8 u8VSPK_WMode);
void  MDrv_SOUND_VSPK_SetSMod(MS_U8 u8VSPK_SMode);
void  MDrv_SOUND_BBE_SetBbeMode(MS_U8 u8EnBBE, MS_U8 u8BBEMode);
void  MDrv_SOUND_SRS_DC(MS_U8 u8SRSenDC);
void  MDrv_SOUND_SRS_TruBass(MS_U8 u8SRSenTruBass);
void  MDrv_SOUND_TSHD_TruBass(MS_U8 u8TSHDenTruBass);
void  MDrv_SOUND_TSHD_Definition(MS_U8 u8TSHDenDefinition);
void  MDrv_SOUND_TSHD_Clarity(MS_U8 u8TSHDenClarity);
MS_BOOL MDrv_ADVSOUND_ProcessEnable(ADVSND_TYPE type);
MS_BOOL MDrv_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable);
void MDrv_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable);
MS_U32 MDrv_ADVSOUND_GetInfo(AdvSound_GET_Type Type);

#ifdef __cplusplus
}
#endif

#endif //_DRVIPCHECK_H_
