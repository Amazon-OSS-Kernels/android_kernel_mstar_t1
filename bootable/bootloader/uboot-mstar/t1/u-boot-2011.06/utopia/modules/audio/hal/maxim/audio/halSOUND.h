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
// reverse bEngineering and compiling of the contents of MStar Confidential
////////////////////////////////////////////////////////////////////////////////

#ifndef _DEV_SOUND_H_
#define _DEV_SOUND_H_

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    HAL_SOUND_Init(void);
void    HAL_SOUND_Init2(void);
void    HAL_SOUND_SetMute(MS_U8 u8Path, MS_BOOL bEnable);
void    HAL_SOUND_AbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2);
void    HAL_SOUND_SetPreScale(MS_U8 u8Path, MS_U8 u8Prescale);
void    HAL_SOUND_SetOutputPhaseShiftMask(AUDIO_OUTPUT_TYPE u8Output, MS_U8 u8Channel);
void    HAL_SOUND_SetBalance(MS_U8 u8Balance);
void    HAL_SOUND_SetBalanceMask(AUDIO_OUTPUT_TYPE u8Output, MS_BOOL u8Enable);
void    HAL_SOUND_SetBalance_L(MS_U8 u8Balance_L);
void    HAL_SOUND_SetBalance_R(MS_U8 u8Balance_R);
void    HAL_SOUND_SetBass(MS_U8 u8Level);
void    HAL_SOUND_SetDynamicBass(MS_U8 u8Level);
void    HAL_SOUND_AbsoluteBass(MS_U8 u8Bass);
void    HAL_SOUND_SetTreble(MS_U8 u8Level);
void    HAL_SOUND_AbsoluteTreble(MS_U8 u8Treble);
void    HAL_SOUND_SetEq(MS_U8 band, MS_U8 u8Level);
void    HAL_SOUND_SetEq7(MS_U8 band, MS_U8 u8Level);
void    HAL_SOUND_SetADCThreshold(MS_U8 u8Threshold);
void    HAL_SOUND_SetNRAttenuate(MS_U8 u8Mode);
void    HAL_SOUND_SetDRCThreshold(MS_U8 u8Level, AUDIO_SOUNDEFFECT_TYPE Type);
void    HAL_SOUND_SetDRCEntry(AUDIO_SOUNDEFFECT_TYPE Type, MS_U8 u8Enable);
void    HAL_SOUND_SetAVCThreshold(MS_U8 u8Level);
void    HAL_SOUND_SetAvcMode(MS_U8 u8AvcMode );
void    HAL_SOUND_SetAvcAT(MS_U8 u8AvcAT );
void    HAL_SOUND_SetAvcRT(MS_U8 u8AvcRT );
void    HAL_SOUND_SetSurroundXA(MS_U8 u8Mode);
void    HAL_SOUND_SetSurroundXB(MS_U8 u8Mode);
void    HAL_SOUND_SetSurroundXK(MS_U8 u8Mode);
void    HAL_SOUND_SetSurroundLPFGain(MS_U8 u8Mode);

void    HAL_SOUND_PowerDown_Wait(MS_BOOL bEnable);
void    HAL_SOUND_SetCH1AudioDelay(MS_U16 u16Delay);
void    HAL_SOUND_SetSpdifAudioDelay(MS_U16 u16Delay);
void    HAL_SOUND_PlayMenuSound(void);

MS_U16  HAL_SOUND_GetPreScale(MS_U8 u8Path);
MS_U16  HAL_SOUND_GetEq(MS_U8 u8Band);
MS_U16  HAL_SOUND_GetBalance_L(void);
MS_U16  HAL_SOUND_GetBalance_R(void);
MS_U16  HAL_SOUND_GetSurroundXA(void);
MS_U16  HAL_SOUND_GetSurroundXB(void);
MS_U16  HAL_SOUND_GetSurroundXK(void);
MS_U16  HAL_SOUND_GetSurroundLPFGain(void);
MS_U16  HAL_SOUND_GetTreble(void);
MS_U16  HAL_SOUND_GetBass(void);
MS_U16  HAL_SOUND_GetAvcMode(void);
MS_U16  HAL_SOUND_GetDRCThreshold(void);
MS_U16  HAL_SOUND_GetAVCThreshold(void);
MS_U16  HAL_SOUND_GetAvcAT(void);
MS_U16  HAL_SOUND_GetAvcRT(void);
MS_U16  HAL_SOUND_GetPEQ_Status(void);
MS_U16  HAL_SOUND_GetHPF_Status(void);
MS_U16  HAL_SOUND_GetEQ_Status(void);
MS_U16  HAL_SOUND_GetSurround_Status(void);
MS_U16  HAL_SOUND_GetTone_Status(void);
MS_U16  HAL_SOUND_GetAutoVolume_Status(void);

MS_U16  HAL_SOUND_GetCH1AudioDelay(void);
MS_U16  HAL_SOUND_GetCH5AudioDelay(void);
MS_U16  HAL_SOUND_GetCH6AudioDelay(void);
MS_U16  HAL_SOUND_GetSpdifAudioDelay(void);
MS_U16  HAL_SOUND_GetHPAudioDelay(void);
//======================================================
void    HAL_SOUND_EnablePEQ(MS_BOOL Enpeq);
void    HAL_SOUND_EnableHPF(MS_BOOL bEnable);
MS_BOOL HAL_SOUND_EnableDcRemove(MS_BOOL EnDcRemove);
void    HAL_SOUND_EnableKTVEcho(MS_BOOL bEnKTVEcho);
void    HAL_SOUND_EnableEQ(MS_BOOL bEnEQ);
void    HAL_SOUND_EnableSurround(MS_BOOL bEnSurround);
void    HAL_SOUND_EnableTone(MS_BOOL bEnTone);
void    HAL_SOUND_EnableAutoVolume(MS_BOOL bEnAVC);
void    HAL_SOUND_EnableDRC(MS_BOOL bEnDRC);
void    HAL_SOUND_EnableNR(MS_U8 u8Level);
void    HAL_SOUND_EnableBalance(MS_U8 u8Enable);
void    HAL_SOUND_EnableEaseVol(MS_U8 u8Enable);
//================From DrvAudioProcess=======================
void    HAL_SOUND_ReLoadCode(MS_U8 u8Type);

//==========================================================
void    HAL_SOUND_SetAdAbsoluteVolume(MS_U8 u8Vol1, MS_U8 u8Vol2);
void    HAL_SOUND_SetAdMute(MS_BOOL bEnable);
void    HAL_SOUND_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef);
void    HAL_SOUND_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2  );
void    HAL_SOUND_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute );
MS_BOOL HAL_SOUND_SetParam( Sound_SET_Type Type, MS_U32 *p_param );
MS_BOOL HAL_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable);
MS_BOOL HAL_SND_SetParam( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2);
MS_U16  HAL_SND_GetParam( Sound_GET_PARAM_Type Type, MS_U16 param1);

#endif  //_DEV_SOUND_H_
