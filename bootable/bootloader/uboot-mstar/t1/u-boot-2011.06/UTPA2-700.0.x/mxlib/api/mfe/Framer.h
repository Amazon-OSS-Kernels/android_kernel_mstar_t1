/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _FRAMER_H_
#define _FRAMER_H_

#include "MuxTypes.h"
//#define SAMPLES_IN_AUDIO_FRAME 1152
//typedef unsigned char BYTE;
//typedef unsigned char boolean;
typedef struct BitstreamInfo* BitstreamInfo_ptr;
typedef struct BitstreamInfo {
    // Input
    MFE_BOOL bIsAudio;
    MFE_BOOL (*GetFrameBits)(BitstreamInfo_ptr pBsInfo, unsigned int nFrameCount);
    // Output: Frame Info
    MFE_U8 *pFrameBits;
    unsigned int nFrameSize;
    // Internal usage
    MFE_U8 *pBits;
    unsigned int nFileSize;
    MFE_U8 *pCur, *pEnd, *pLast;
    unsigned int nFrameIndex;    // The overall frame index.
    unsigned int nFrameCount;    // Frame count in this round. If video, always 1.
    // MP4V only
    MFE_BOOL bPrevVOP;    // Previous start code is VOP-start
    // MP2A only
    unsigned int nAudFrameSize;
} BitstreamInfo;

#ifdef _WIN32
// API's
MFE_BOOL OpenMappedFile(const char* strFileName, void/*BitstreamInfo*/* pBsInfo);
MFE_BOOL CloseMappedFile(void/*BitstreamInfo*/* pBsInfo);
// GetFrameBits() implementation
MFE_BOOL GetOneFrameBitsMP4V(void/*BitstreamInfo*/* pBsInfo, unsigned int nFrameCount/*DontCare*/);
MFE_BOOL GetOneFrameBitsMP2V(void/*BitstreamInfo*/* pBsInfo, unsigned int nFrameCount/*DontCare*/);
#endif
MFE_BOOL GetOneFrameBitsMP1A(BitstreamInfo* pBsInfo, unsigned int nFrameCount);


#endif // _FRAMER_H_
