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


#ifndef _MS_DRV_DEMUX_
#define _MS_DRV_DEMUX_

#include <apiDMX.h>
#include "video/MsDrvVideo.h"
#include "audio/MsDrvAudio.h"

typedef struct _MsDrv_DmxProg_s
{
    MS_U16 u16VideoPid;
    MS_U16 u16AudioPid;
    MS_U16 u16PcrPid;

    MsDrv_VIDEO_CodecType_e eVdecCodecType;
    MsDrv_AUDIO_CodecType_e eAdecCodecType;

}MsDrv_DmxProg_t;

// dmx initialization with the specified source input
int MsDrv_DmxInit(DMX_FLOW_INPUT inSrc);

// open video pid filter, source is the 1st parameter of MsDrv_DmxInit()
int MsDrv_DmxOpenVideoFilter(MS_U16 u16VideoPid, MS_U8 *pu8DmxId);

// open audio pid filter, source is the 1st parameter of MsDrv_DmxInit()
int MsDrv_DmxOpenAudioFilter(MS_U16 u16AudioPid, MS_U8 *pu8DmxId);

// open pcr pid filter, source is the 1st parameter of MsDrv_DmxInit()
int MsDrv_DmxOpenPcrFilter(MS_U16 u16PcrPid, MS_U8 *pu8DmxId);

// close dmx filter
void MsDrv_DmxCloseFilter(MS_U8 u8DmxId);

void MsDrv_DmxRoutinePolling(void);

int MsDrv_DmxScan(MS_BOOL bLive, MsDrv_DmxProg_t *pstProg1st);

int MsDrv_DmxFileInInit(MS_U32 u32FileinAddr, MS_U32 u32FileSize, MS_U32 u32Repeat);

void MsDrv_DmxEnableAvFifo(void);

#endif  // #ifndef DEMUX
