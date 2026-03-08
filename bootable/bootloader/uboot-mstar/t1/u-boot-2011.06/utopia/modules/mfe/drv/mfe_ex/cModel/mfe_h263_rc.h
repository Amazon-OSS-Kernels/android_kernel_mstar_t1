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
#ifndef _MFE_RATECTRL_H_
#define _MFE_RATECTRL_H_

//#define CHECK_1SEC_BITRATE

#include "mfe_type.h"
#include "msRateCtrl.h"
#define MAX_FRAMERATE   30UL

#define SPEC_MAX_QP (31UL)
#define SPEC_MIN_QP (1UL)
/* Input for rate control */
typedef struct {
    MS_S32 nWidth, nHeight;
    MS_FLOAT fTargetFrameRate, fMinFrameRate;
    MS_S32 m_nBitrate;
    MS_S32 nPCount;    // Number of P-frames betweeen I-frames
    RC_GRANULARITY rcGranularity;
    RC_METHOD rcMethod;
} H263RCInfo;

typedef struct RateCtrl {
    /* Input parameters */

    MS_S32 m_nWidth, m_nHeight;
    MS_FLOAT m_fTargetFrameRate, m_fMinFrameRate;
    MS_S32 m_nBitrate;
    MS_S32 m_nPCount;
    RC_GRANULARITY m_rcGranularity;
    RC_METHOD m_rcMethod;
    // Derived variables
    MS_FLOAT m_fAvgBitsPerFrame;

    /* rate control variables */

    MS_S32 m_nFrameCount;  // How many frame coded
    MS_S32 m_nBufFullness;  // Rate control buffer
    MS_U64 m_nTotalBits;
    // Last-frame status
    MS_S32 m_nLastFrameBits, m_nLastFrameAvgQP;
    MS_S32 m_nLastTargetBits;

    /* Bitrate usage compensation */
    MS_S32 m_nTargetFullness;
    MS_S32 m_nDeputyCount, m_nMinDeputyCount;

    /* Variable bitrate */
    MS_FLOAT m_fLongTermQP;

    // Model parameters
    MS_S32 m_nTargetBits;   // target number of bits of current frame
    MS_S32 m_nNFrame;    // number of macroblocks in a frame
    MS_S32 m_nWidthInMb;   // Frame width in MB

    /* Below are for STRICT_CONST_BITRATE */

    // Dynamic framerate
    MS_U8 m_bDropFrameEnabled;  // Enabled when min-framerate < max-framerate.
    MS_S32 m_nSkipCount;          // The historic accumulated skip-frame count in one second
    MS_S32 m_nSkipCountMax;       // Constant: The allowable maximal dropped frame count in 1sec, depends on Min&Max framerate.
    MS_S32 m_nContSkipCount;   // Result: The determined continuous skip-frame count.
    // Keep track of historic skip-frame status.
    MS_U32 m_nWinIndex;
    MS_U32 m_nWinIndexMax;    // Constant: corresponds to 1 sec.
    MS_U8 m_SkipFrameHistory[MAX_FRAMERATE]; // Skip-frame history: 1 is Skipped, 0 is not.
    // Bitrate monitoring
    // Over-usage: Skip following frame(s) to compensate immediately
    // Shortage: Keep a lower-bound to assure low-delay in constant-bandwidth network.
    MS_S32 m_nBitrateOverUsage;   // Accumulated bitrate over-usage.
    // Determine how many spare bitcount of skipped frames will be transfered into next coded frame.
    MS_S32 m_nCompensateBitrate;    // Bitrate transferred to next coded frame.
    MS_S32 m_nCompensateFrame;     // How many skipped frame will contribute bitrate compenstaion.
    // The maximal bitrate is set to caller-input bitrate
    // And re-calculate internal target bitrate < m_nBitrateMax.
    MS_S32 m_nBitrateMax;
    // When continuous good frame, gently reducing frameerate to avoid CPU high usage.
    MS_S32 m_nContGoodFrameCount;

#if defined(CHECK_1SEC_BITRATE)
    // Runtime bitrate
    MS_S32 m_nBrHistIndex;
    MS_S32 m_BitrateHistory[MAX_FRAMERATE];
    // Runtime delay: Assuming constant-bandwidth channel
    MS_S32 m_nUnsentBits;
#endif
} RateCtrl_t;

void rc_Create(RateCtrl_t* rcctx);
void rc_Destroy(RateCtrl_t* rcctx);
// Global
void rc_Init(RateCtrl_t* rcctx, H263RCInfo* pRCInfo);
void rc_Finish(RateCtrl_t* rcctx);
// Each frame
MS_S32 rc_CheckSkippedFrame(RateCtrl_t* rcctx);
MS_S32 rc_InitFrame(RateCtrl_t* rcctx, MS_S8 chFrameType);    // Return the initial frame qp
void rc_UpdateFrame(RateCtrl_t* rcctx, const MS_S32 totalUsedBits,const MS_S32 lastAvgQP, MS_BOOL bDummyFrame);

#endif
