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
//////////////////////////////////////////////////////////////////////////
// MStar simple rate control
//////////////////////////////////////////////////////////////////////////

#ifndef _RATECTRL_H_
#define _RATECTRL_H_

#include "mfe_type.h"

/************************************************************************/
/* Configuration                                                        */
/************************************************************************/
#define QS_SHIFT_FACTOR	5UL
#define MAX_BUF_FULL	(100L*1024L*1024L)
// Define this to support MB-level rate control
#define _SUPPORT_MBLEVEL_RC_

// Define this to declare RC variables outside RC kernel
// NOTE: MFE_MPEG4_CMODEL-- define this.
//#define _USE_EXT_RCDATA_

// The maximal allowable MB width
#define MAX_MB_WIDTH (1920UL>>4)

/* ~Configuration *******************************************************/

#ifndef MSRC_MAX
#define  MSRC_MAX(a,b)              (((a) > (b)) ? (a) : (b))
#endif
#ifndef MSRC_MIN
#define  MSRC_MIN(a,b)              (((a) < (b)) ? (a) : (b))
#endif

#ifdef _SUPPORT_MBLEVEL_RC_
	#define TOP_QP_DIFF_LIMIT          3UL // Must >= SPEC_QP_DIFF_LIMIT
	#define LEFT_QP_DIFF_LIMIT         3UL // Must >= SPEC_QP_DIFF_LIMIT
#endif

typedef enum {
	CONST_QUALITY = 1,								//!< Constant qscale: Use specified QP.
	CONST_BITRATE = 2,								//!< Constant bitrate.
	VARIABLE_BITRATE = 3,							//!< Variable bitrate: instant bitrate is unrestricted.
	CONSTRAINED_VARIABLE_BITRATE = 4,	//!< Variable bitrate: instant maximal bitrate is restricted.
       STRICT_CONST_BITRATE = 5,
} RC_METHOD;

typedef enum {
	FRAMELEVELRC = 1,   //!< All MB's in one frame uses the same qscale.
	MBLEVELRC = 2       //!< Allow qscale changing within frame, for better bitrate achievement.
} RC_GRANULARITY;


/* Input for rate control */
typedef struct {
	MS_S32 nCodecType;	// 0/1/2/3: MPEG4/H263/H264/JPEG
	MS_S32 nWidth, nHeight;
	MS_BOOL fTargetFrameRate_is_float;
	MS_FLOAT fTargetFrameRate;
	MS_S32 int_fTargetFrameRate;
	MS_S32 nBitrate;     // Meaningful only when rcMethod!=CONST_QUALITY (bits)
										// If rcMethod=CONSTRAINED_VARIABLE_BITRATE, 0 means internally-decided.
	MS_S32 nMaxBitrate;  // Meaningful only when rcMethod=CONSTRAINED_VARIABLE_BITRATE (Bits)
										// For others, this should be 0.
	MS_S32 nConstQP;			// Meaningful only when rcMethod=CONST_QUALITY
	//
	// For coded frame slicing: GOB (H.263), video packet (MPEG4), or slice (H.264)
	MS_S32 nVPSize;	// Video packet size in bits.
	MS_S32 nVPMbRow;   // Video packet size in number of MB rows. If H.263, this must be multiple of "MBrow per GOB".
	//
	MS_BOOL bFixedFrameRate;	// If FALSE, rate control can signal frame-skipping.
	MS_S32 nPCount;			// Number of P-frames between I-frames
	MS_S32 nBCount;			// Number of B-frames between P-frames // Suggest: 0, 1, 2
	RC_GRANULARITY rcGranularity;
	RC_METHOD rcMethod;
} CVBRRCInfo;

typedef struct {
	/* Input parameters */

	MS_S32 m_nCodecType;
	MS_S32 m_nWidth, m_nHeight;
	MS_S32 m_nTargetFrameRateNum, m_nTargetFrameRateDeNum, m_nTargetFrameRateInt;	// The target rate is Num/Denum
	MS_S32 m_nBitrate;
	MS_S32 m_nMaxBitrate; // Only when CONSTRAINED_VARIABLE_BITRATE
	MS_S32 m_nConstQP;   // Only when CONST_QUALITY
	MS_S32 m_nVPSize, m_nVPMbRow;
	MS_BOOL m_bFixedFrameRate;
	MS_S32 m_nPCount, m_nBCount;
	RC_GRANULARITY m_rcGranularity;
	RC_METHOD m_rcMethod;
	// Derived variables
	MS_S32 m_nAvgBitsPerFrame;
	MS_S32 m_nBitsPerFrame[3];	// I, P, B

	/* rate control variables */

	MS_S32 m_nFrameCount;  // How many frame coded
	MS_S32 m_nBufFullness;  // Rate control buffer
	MS_S64 m_nTotalBits;
	// Last-frame status
	MS_BOOL m_bIsBotField;
	MS_S32 m_nLastFrameBits, m_nLastFrameAvgQStep[2];
	MS_S32 m_nLastTargetBits;

	/* Bitrate usage compensation */
	MS_S32 m_nTargetFullness;
	MS_S32 m_nDeputyCount, m_nMinDeputyCount;

	/* Variable bitrate */
	MS_S32 m_nLongTermQP64;

	// Model parameters

	MS_S32 m_nTargetBits;   // target number of bits of current frame
	MS_S32 m_nTotalMB;			// number of macroblocks in a frame
	MS_S32 m_nMBN;					// Accumulated handled MB count (within one frame)
	MS_S32 m_nFrameQStep;
	MS_S32 m_nFrameType;

	/* Only for CONSTRAINED_VARIABLE_BITRATE */
	MS_S32 m_nMaxOffset;
	MS_S32* m_BitrateGauge;
	MS_S32 m_nGaugeCount, m_nGaugeIndex, m_nGaugeBitrate;
	MS_S32 m_nMaxFrozenFrame;

	// QP, QStep: Min, Max
	MS_S32 m_nMinQP, m_nMaxQP;
	MS_S32 m_nMinQStep, m_nMaxQStep;
	MS_S32 m_nFrameSkipThrQP;

	/* MB-level rate control */
#ifdef _SUPPORT_MBLEVEL_RC_
	MS_S32 m_nTargetMbBits;
	MS_S32 m_nTargetUsedBits;
	MS_S32 m_nUsedBits[3];	// For HW pipeline delay
	MS_S32 m_nSumAct;
	MS_S32 m_nMbWidth, m_nPrevQP, m_nLeftQP, m_nPrevTopQP[MAX_MB_WIDTH];
	MS_S32 m_nLastTargetQP;	// For sig_dump
	// For coded frame slicing
	MS_S32 m_nNewPacket;	// New GOB, video packet, slice... Simulating HW er_en
	MS_S32 m_nLastVPBits;
#endif

	MS_S32* m_pMBBitsArray;	// Only for DEBUG usage.
} CVBRRateControl;

// Global
void cvbr_InitRateControl(CVBRRateControl* ct, CVBRRCInfo* pRCInfo);
void cvbr_CloseRateControl(CVBRRateControl* ct);
// Each frame
MS_S32 cvbr_InitFrame(CVBRRateControl* ct, MS_S32 nFrameType, MS_S8 FieldType);    // Return the initial frame qp
MS_S32 cvbr_UpdateFrame(CVBRRateControl* ct, MS_S32 totalUsedBits, MS_S8 bDummyFrame, MS_S8 FieldType);
// Each macroblock
MS_S32 cvbr_InitMB(CVBRRateControl* ct, MS_S32 nVar, const MS_S32 nPrevQP, const MS_S32 nBits, MS_S32 IsIntra, MS_S32 IsP4MV, MS_S32 BPredType, MS_S32 nResetDQ);
void output_MBR_reg(void* hd, CVBRRateControl* ct);
MS_S32 rcQP2Qstep(CVBRRateControl* ct, MS_S32 QP);
MS_S32 rcQstep2QP(CVBRRateControl* ct, MS_S32 QstepX32);

#endif
