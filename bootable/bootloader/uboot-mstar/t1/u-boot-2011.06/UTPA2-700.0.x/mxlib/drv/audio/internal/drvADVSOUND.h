/**
 * Copyright (c) 2006  2016 MStar Semiconductor, Inc.
 * This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

//
//  [for Doxygen]
/// @file drvADVSOUND.h
/// @brief Subroutine for advance sound effect
/// @author MStarSemi Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVADVSOUND_H_
#define _DRVADVSOUND_H_

#include "MsTypes.h"
#include "drvAUDIO_if.h"


#ifdef __cplusplus
extern "C"
{
#endif

//=====================================================
// Declare audio function here.
//=====================================================
void  MDrv_SOUND_AseSetBinAddress(MS_U8 u8Index, MS_U32 BinAddr);
//void  MDrv_SOUND_ReLoadAseCode(MS_U8 u8Type);
void  MDrv_SOUND_SetPara(MS_U8 u8Addr1 , MS_U8 u8Addr2, MS_U8 u8Value1, MS_U8 u8Value2, MS_U8 u8Value3);
void  MDrv_SOUND_TSHD_TruSurr(MS_U8 u8TSHDenTruSurr);
void  MDrv_SOUND_TSHD_Limiter(MS_U8 u8TSHDenLimiter);
void  MDrv_SOUND_TSHD_SRS3D(MS_U8 u8TSHDenSRS3D);
void  MDrv_SOUND_TruVolume_Enable(MS_U8 u8TSHDenTVol);
void  MDrv_SOUND_ADSY_SetPara(MS_U8 u8mode, MS_U8 u8value);
MS_BOOL MDrv_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2);
void  MDrv_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr, MS_U32 size);
void MDrv_DBXTV_SetVolPath(AUDIO_OUTPUT_TYPE path);
#ifdef __cplusplus
}
#endif
#endif //_DRVADVSOUND_H_
