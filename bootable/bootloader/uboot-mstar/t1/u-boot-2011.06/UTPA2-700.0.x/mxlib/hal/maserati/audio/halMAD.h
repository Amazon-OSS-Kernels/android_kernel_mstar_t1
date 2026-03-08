/**
 * Copyright (c) 2006 <96> 2016 MStar Semiconductor, Inc.
 * This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _DEV_MAD_H_
#define _DEV_MAD_H_

#include "MsTypes.h"
#include "halAUDIO.h"
#include "drvAUDIO_if.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define XPCM_PARA_BASEADDR  0x27F0 //0x3F00
#define ENC_BUF_SIZE 6;

#define RTSP_MEM_ADDR   0x10000 //the base addr is a temporary space borrowed from MAD ES2(assuming it is idle in streaming application)
#define RTSP_MEM_SIZE   0x1000

#define DMA_READER_TEMP_BUFFER_ADDR   0x052200  //ring tone read buffer (Size: 4KB)

#define ACMOD_CH_MASK_C        0x0001
#define ACMOD_CH_MASK_L        0x0002
#define ACMOD_CH_MASK_R        0x0004
#define ACMOD_CH_MASK_LS       0x0008
#define ACMOD_CH_MASK_RS       0x0010
#define ACMOD_CH_MASK_LFE      0x0020
#define ACMOD_CH_MASK_LF       0x0040
#define ACMOD_CH_MASK_RF       0x0080
#define ACMOD_CH_MASK_NULL     0x0000

//=====================================================
// AUDIO_MAD System Relational Hal Function
//=====================================================
MS_U32  HAL_MAD_Read_DSP_sram(MS_U16 dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
MS_BOOL HAL_MAD_Write_DSP_sram(MS_U16 dsp_addr, MS_U32 value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
void    HAL_MAD_Init(void);
void    HAL_MAD_SetMemInfo(void);
void    HAL_MAD_SetDecCmd(AU_DVB_DECCMD decCmd);
void    HAL_MAD_SetDecCmd2(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd);
void    HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD decCmd);
//void HAL_AUDIO_SetEncodeDoneFlag(MS_BOOL bSet);
void    HAL_MAD_SetEncCmd(AU_DVB_ENCCMD u8EncCmd);
void    HAL_MAD_EncodeInit(AU_DVB_ENCBR u8EncBr, MS_U8 u8EncFrameNum);
void    HAL_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *FrameInfo);
AU_DVB_DECCMD   HAL_MAD_GetDecCmd(void);
AU_DVB_DECCMD   HAL_MAD_GetDSP2DecCmd(void);
void    HAL_MAD_DisEn_MIUREQ(void);
void    HAL_MAD_Dis_MIUREQ(void);
void    HAL_MAD_RSTMAD_DisEn_MIUREQ(void);
MS_U8   HAL_MAD_GetReloadCodeAck(void);
MS_U8   HAL_MAD_GetLoadCodeAck(void);
MS_U8   HAL_MAD_Rpt_DTVES(void);
void    HAL_MAD_SetStreamID_Mod(MS_U8 stream_id_mod);
void    HAL_MAD_SetAdMixMode (AD_MIX_MODE u8Mix_mode, MS_U8  u8En_mix);
void    HAL_MAD_SetDspIDMA(void);
void    HAL_MAD_SetMcuCmd(MS_U8 cmd);
void    HAL_MAD_SetPIOCmd(MS_U8 cmd);
void    HAL_MAD_TriggerPIO8(void);
void    HAL_MAD_DvbFLockSynthesizer_En(void);
MS_BOOL HAL_MAD_LoadCode(AUDIO_DSP_CODE_TYPE u8Type);
MS_BOOL HAL_AUDIO_SetCertMode( Audio_CertMode CertType,Audio_CertEnable enCert );
MS_BOOL HAL_MAD_MM2_initAesInfo( AUDIO_DEC_ID  dec_id );
MS_BOOL HAL_MAD_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info );
MS_BOOL HAL_MAD_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts );
//=====================================================
// Function
//=====================================================
MS_BOOL HAL_MAD_SetCommInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U64 HAL_MAD_GetCommInfo( Audio_COMM_infoType infoType );
MS_BOOL HAL_MAD_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetMpegInfo( Audio_MPEG_infoType infoType );
MS_BOOL HAL_MAD_SetWmaInfo( Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetWmaInfo( Audio_WMA_infoType infoType );
MS_BOOL HAL_MAD_SetAC3Info( Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetAC3Info( Audio_AC3_infoType infoType );
MS_BOOL HAL_MAD_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetAC3PInfo( Audio_AC3P_infoType infoType );
MS_BOOL HAL_MAD_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type );
MS_U32  HAL_MAD_GetDTSInfo( Audio_DTS_infoType infoType );
MS_BOOL HAL_MAD_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetAACInfo( Audio_AAC_infoType infoType );
MS_BOOL HAL_MAD_SetDtsInfo( Audio_DTS_infoType infoType, MS_U32 param1, MS_U32 param2 );
MS_U32  HAL_MAD_GetDtsInfo( Audio_DTS_infoType infoType );
MS_BOOL HAL_MAD_SetRA8LBRInfo( Audio_RA8LBR_infoType infoType, MS_U32 param1, MS_U32 param2 );
MS_U32  HAL_MAD_GetRA8LBRInfo( Audio_RA8LBR_infoType infoType );

//=====================================================
// AUDIO_MAD MEPG Relational Hal Function
//=====================================================
MS_U32  HAL_MAD_MPEG_GetHeaderInfo(void);
MS_U16  HAL_MAD_MPEG_GetSampleRate(void);
MS_U16  HAL_MAD_MPEG_GetBitRate(void);
MPEG_SOUND_MODE   HAL_MAD_MPEG_GetSoundMode(void);

//=====================================================
// AUDIO_MAD Dolby Relational Hal Function
//=====================================================
void    HAL_MAD_Monitor_DDPlus_SPDIF_Rate( void );

//=====================================================
// AUDIO_MAD WMA Relational Hal Function
//=====================================================
void    HAL_MAD_WMA_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value);

//=====================================================
// AUDIO_MAD AAC Relational Hal Function
//=====================================================

//=====================================================
// AUDIO_MAD OTHERS Relational Driver Function
//=====================================================
MS_U16  HAL_MAD_XPCM_GetParaBase(void);
MS_U8   HAL_MAD_RA8_setParam(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                           MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                           MS_U16* FrameSize);

MS_U16  HAL_MAD_Ra8_GetParaBase(void);
extern  MS_BOOL HAL_AUDSP_CheckDecIdmaReady(MS_U8 u8IdmaChk_type );

MS_U8   HAL_MAD_XPCM_setParam(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);

//=====================================================
// AUDIO_MAD MM Mode Hal Function
//=====================================================
MS_U32  HAL_MAD_ReadTimeStamp(void);

//====== STAOS PIP START 2012/02/02 ========

MS_BOOL HAL_MAD_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param );
MS_BOOL HAL_MAD_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo );
MS_BOOL HAL_MAD_GetAudioCapability(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus);

//====== STAOS PIP END 2012/02/02 ========

#endif // _DEV_MAD_H_
