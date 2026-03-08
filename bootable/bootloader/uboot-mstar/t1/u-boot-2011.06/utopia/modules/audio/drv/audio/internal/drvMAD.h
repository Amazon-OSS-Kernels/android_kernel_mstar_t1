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
//  [for Doxygen]
/// file drvMAD.h
/// @brief Subroutine for DVB
/// @author MStarSemi Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVMAD_H_
#define _DRVMAD_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "drvAUDIO_if.h"
#include "../internal/drvAUDIO_internal.h"

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

#define IS_MADGOOD() ( MDrv_MAD_Rpt_DTVES() == 0x41 || MDrv_MAD_Rpt_DTVES() == 0x81 )

//=====================================================
// AUDIO_MAD System Relational Driver Function
//=====================================================
//MS_U32  MDrv_MAD_Read_DSP_sram(MS_U16 dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
//MS_BOOL MDrv_MAD_Write_DSP_sram(MS_U16 dsp_addr, MS_U32 value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
void    MDrv_MAD_Init(void);
void    MDrv_MAD_SetMemInfo(void);
void    MDrv_MAD_DisEn_MIUREQ(void);
void    MDrv_MAD_Dis_MIUREQ(void);
MS_BOOL MDrv_MAD_LoadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
void    MDrv_MAD_SetAdMixMode(AD_MIX_MODE mix_mode, MIX_ONOFF en_mix);

void    MDrv_AUD_Monitor_SPDIF_NONPCM_SmpRate(void);
MS_BOOL MDrv_AUDIO_SetCertMode( Audio_CertMode CertType, Audio_CertEnable enCert );
MS_BOOL MDrv_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID  dec_id );
MS_BOOL MDrv_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info );
MS_BOOL MDrv_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts );


//=====================================================
// AUDIO_MAD Common decoder Relational Driver Function
//=====================================================
//Multi Player
MS_BOOL MDrv_MAD_DecoderCmd(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd);
MS_BOOL MDrv_MAD_GetAudioCapability(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL MDrv_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U64 MDrv_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType );

void    MDrv_MAD_SetDecCmd(AU_DVB_DECCMD decCmd);
AU_DVB_DECCMD   MDrv_MAD_GetDecCmd(En_DVB_AudioDecoder AdoDecoderNo);

MS_U8   MDrv_MAD_Rpt_DTVES(void);
//=====================================================
// AUDIO_MAD MEPG Relational Driver Function
//=====================================================
MS_BOOL MDrv_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  MDrv_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType );

//=====================================================
// AUDIO_MAD Dolby Relational Driver Function
//=====================================================
MS_BOOL MDrv_AUDIO_SetAC3Info( Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_BOOL MDrv_AUDIO_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  MDrv_AUDIO_GetAC3Info( Audio_AC3_infoType infoType );
MS_U32  MDrv_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType );

//=====================================================
// AUDIO_MAD DTS Relational Driver Function
//=====================================================
MS_BOOL MDrv_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type );
MS_U32  MDrv_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType );

//=====================================================
// AUDIO_MAD WMA Relational Hal Function
//=====================================================
MS_BOOL MDrv_AUDIO_SetWmaInfo( Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  MDrv_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType );

//=====================================================
// AUDIO_MAD LPCM/XPCM Relational Driver Function
//=====================================================
MS_U8   MDrv_MAD_XPCM_setParam( XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);

//=====================================================
// AUDIO_MAD RA Relational Driver Function
//=====================================================
MS_U8   MDrv_MAD_RA8_setParam(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                                MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                                MS_U16* FrameSize);

//=====================================================
// AUDIO_MAD MPEG Encoder Related Driver Function
//=====================================================
void    MDrv_MAD_SetEncCmd(AU_DVB_ENCCMD u8EncCmd);
void    MDrv_AUDIO_SetEncodeInit(AU_DVB_ENCBR u8EncBitrate, MS_U8 u8EncFrameNum);
void    MDrv_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *FrameInfo);

//=====================================================
// AUDIO_MAD AAC Relational Driver Function
//=====================================================
MS_U32  MDrv_AUDIO_GetAACInfo( Audio_AAC_infoType infoType );
MS_U32  MDrv_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 );

//====== STAOS PIP START 2012/02/02 ========
MS_BOOL MDrv_AUDIO_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo );

//====== STAOS PIP END 2012/02/02 ========

#ifdef __cplusplus
}
#endif

#endif   //#ifndef _DRVMAD_H_

