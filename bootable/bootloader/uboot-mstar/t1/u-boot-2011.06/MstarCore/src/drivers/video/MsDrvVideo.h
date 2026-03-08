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


#ifndef _MS_DRV_VIDEO_
#define _MS_DRV_VIDEO_

/// Stream type
typedef enum
{
    ///Live stream
    E_MSDRV_VIDEO_STREAM_LIVE = 0,
    ///TS file
    E_MSDRV_VIDEO_STREAM_TSFILE,
    ///file
    E_MSDRV_VIDEO_STREAM_FILE,
    ///Stream type number
    E_MSDRV_VIDEO_STREAM_MAX
} MsDrv_VIDEO_StreamType_e;

typedef enum
{
    ///unsupported codec type
    E_MSDRV_VIDEO_CODEC_TYPE_NONE = 0,
    ///MPEG 1/2
    E_MSDRV_VIDEO_CODEC_TYPE_MPEG2,
    ///H263 (short video header)
    E_MSDRV_VIDEO_CODEC_TYPE_H263,
    ///MPEG4 (default)
    E_MSDRV_VIDEO_CODEC_TYPE_MPEG4,
    ///MPEG4 (Divx311)
    E_MSDRV_VIDEO_CODEC_TYPE_DIVX311,
    ///MPEG4 (Divx412)
    E_MSDRV_VIDEO_CODEC_TYPE_DIVX412,
    ///FLV
    E_MSDRV_VIDEO_CODEC_TYPE_FLV,
    ///VC1 advanced profile (VC1)
    E_MSDRV_VIDEO_CODEC_TYPE_VC1_ADV,
    ///VC1 main profile (RCV)
    E_MSDRV_VIDEO_CODEC_TYPE_VC1_MAIN,
    ///Real Video version 8
    E_MSDRV_VIDEO_CODEC_TYPE_RV8,
    ///Real Video version 9 and 10
    E_MSDRV_VIDEO_CODEC_TYPE_RV9,
    ///H264
    E_MSDRV_VIDEO_CODEC_TYPE_H264,
    ///AVS
    E_MSDRV_VIDEO_CODEC_TYPE_AVS,
    ///MJPEG
    E_MSDRV_VIDEO_CODEC_TYPE_MJPEG,
    ///MVC
    E_MSDRV_VIDEO_CODEC_TYPE_MVC,
    ///VP8
    E_MSDRV_VIDEO_CODEC_TYPE_VP8,
    ///HEVC
    E_MSDRV_VIDEO_CODEC_TYPE_HEVC,
    E_MSDRV_VIDEO_CODEC_TYPE_MAX
} MsDrv_VIDEO_CodecType_e;

int MsDrv_VideoStart(MsDrv_VIDEO_StreamType_e eStreamType, MsDrv_VIDEO_CodecType_e eCodecType);
int MsDrv_IFrameStart(MsDrv_VIDEO_StreamType_e eStreamType, MsDrv_VIDEO_CodecType_e eCodecType, char * FileName);
MS_BOOL MsDrv_VideoSetXcMvopCfg(void);
int MsDrv_VideoStop(void);
MS_U32 MsDrv_VideoShowDecInfo(void);
void MsDrv_VideoRoutinePolling(void);
MS_U32 MsDrv_VideoGetDecFrmCnt(void);
MS_U32 MsDrv_VideoGetPtsAddr(MsDrv_VIDEO_CodecType_e eCodecType, MS_U32 *pu32PtsKhzUnit);
MS_BOOL MsDrv_VideoGetFrameCountAddr(MsDrv_VIDEO_CodecType_e eCodecType, MS_U32 *pu32FrameCntAddr, MS_U32 *pu32SkipFrameCntAddr);
MS_BOOL MsDrv_VideoIsSyncOn(void);
MS_BOOL MsDrv_VideoIsReachSync(MS_BOOL bWaitSync);
void MsDrv_VideoUnMute(void);

MS_U32 MsDrv_VideoGetCurPts(void);

MS_BOOL MsDrv_VideoPause(void);
MS_BOOL MsDrv_VideoResume(void);
MS_U32 MsDrv_VideoGetDecFrmCnt(void);
MS_U32 MsDrv_VideoGetDispCnt(void);


#endif  // #ifndef _MS_DRV_VIDEO_
