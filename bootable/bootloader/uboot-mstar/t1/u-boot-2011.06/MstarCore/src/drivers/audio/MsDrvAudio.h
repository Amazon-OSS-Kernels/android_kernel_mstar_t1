/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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


#ifndef _MS_DRV_AUDIO_
#define _MS_DRV_AUDIO_

//------------------------------TV_SYSTEM---------------------------------------
#define TV_NTSC                 0
#define TV_PAL                  1
#define TV_CHINA                2

//------------------------------AUDIO_SYSTEM_SEL--------------------------------
#define AUDIO_SYSTEM_BTSC                       0
#define AUDIO_SYSTEM_A2         1
#define AUDIO_SYSTEM_EIAJ

/// Stream type
typedef enum
{
    ///Live stream
    E_MSDRV_AUDIO_STREAM_LIVE = 0,
    ///TS file
    E_MSDRV_AUDIO_STREAM_TSFILE,

    ///Stream type number
    E_MSDRV_AUDIO_STREAM_MAX
} MsDrv_AUDIO_StreamType_e;

typedef enum
{
    E_MSDRV_AUDIO_CODEC_TYPE_NONE = 0x0,
    E_MSDRV_AUDIO_CODEC_TYPE_MPEG,
    E_MSDRV_AUDIO_CODEC_TYPE_AC3,
    E_MSDRV_AUDIO_CODEC_TYPE_AC3P,
    E_MSDRV_AUDIO_CODEC_TYPE_AAC,
    E_MSDRV_AUDIO_CODEC_TYPE_MP3,
    E_MSDRV_AUDIO_CODEC_TYPE_WMA,
    E_MSDRV_AUDIO_CODEC_TYPE_CDLPCM,
    E_MSDRV_AUDIO_CODEC_TYPE_RA8LBR,
    E_MSDRV_AUDIO_CODEC_TYPE_XPCM,
    E_MSDRV_AUDIO_CODEC_TYPE_DTS,
    E_MSDRV_AUDIO_CODEC_TYPE_WMA_PRO,
    E_MSDRV_AUDIO_CODEC_TYPE_FLAC,
    E_MSDRV_AUDIO_CODEC_TYPE_VORBIS,
    E_MSDRV_AUDIO_CODEC_TYPE_DTSLBR,
    E_MSDRV_AUDIO_CODEC_TYPE_AMR_NB,
    E_MSDRV_AUDIO_CODEC_TYPE_AMR_WB,
    E_MSDRV_AUDIO_CODEC_TYPE_DRA,
    E_MSDRV_AUDIO_CODEC_TYPE_MAX,
} MsDrv_AUDIO_CodecType_e;

void MsDrv_PlayMelody(void);
int MsDrv_AudioPreInit(void);
MS_U32 MsDrv_GetEsStartAddr(void);

int MsDrv_AudioStart(MsDrv_AUDIO_StreamType_e eStreamType, MsDrv_AUDIO_CodecType_e eCodecType);
int MsDrv_AudioStop(void);
void MsDrv_AudioUnMute(MS_U8 u8Volume);
void MsDrv_AudioMute(void);
void MsDrv_AudioShowDecInfo(void);
#endif