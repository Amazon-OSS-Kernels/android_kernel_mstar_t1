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

// Define this to support MB-level rate control
#define _SUPPORT_MBLEVEL_RC_

// Define this to declare RC variables outside RC kernel
// NOTE: MFE_MPEG4_CMODEL-- define this.
//#define _USE_EXT_RCDATA_

// The maximal allowable MB width
#define MAX_MB_WIDTH (960>>4)

/* ~Configuration *******************************************************/

#ifndef MSRC_MAX
#define  MSRC_MAX(a,b)              (((a) > (b)) ? (a) : (b))
#endif
#ifndef MSRC_MIN
#define  MSRC_MIN(a,b)              (((a) < (b)) ? (a) : (b))
#endif

#ifdef _SUPPORT_MBLEVEL_RC_
	#define TOP_QP_DIFF_LIMIT          3 // Must >= SPEC_QP_DIFF_LIMIT
	#define LEFT_QP_DIFF_LIMIT         3 // Must >= SPEC_QP_DIFF_LIMIT
#endif

typedef enum {
	CONST_QUALITY = 1,								//!< Constant qscale: Use specified QP.
	CONST_BITRATE = 2,								//!< Constant bitrate. 
	VARIABLE_BITRATE = 3,							//!< Variable bitrate: instant bitrate is unrestricted.
	CONSTRAINED_VARIABLE_BITRATE = 4,	//!< Variable bitrate: instant maximal bitrate is restricted.
} RC_METHOD;

typedef enum {
	FRAMELEVELRC = 1,   //!< All MB's in one frame uses the same qscale.
	MBLEVELRC = 2       //!< Allow qscale changing within frame, for better bitrate achievement.
} RC_GRANULARITY;

// For CONSTRAINED_VARIABLE_BITRATE
#define MAX_GAUGE_SIZE  60

/* Input for rate control */
typedef struct {
	int nCodecType;	// 0/1/2/3: MPEG4/H263/H264/JPEG
	int nWidth, nHeight;
	MFE_BOOL fTargetFrameRate_is_float;
	float fTargetFrameRate;
	int int_fTargetFrameRate;
	int nBitrate;     // Meaningful only when rcMethod!=CONST_QUALITY (bits)
										// If rcMethod=CONSTRAINED_VARIABLE_BITRATE, 0 means internally-decided.
	int nMaxBitrate;  // Meaningful only when rcMethod=CONSTRAINED_VARIABLE_BITRATE (Bits)
										// For others, this should be 0.
	int nConstQP;			// Meaningful only when rcMethod=CONST_QUALITY
	// 
	// For coded frame slicing: GOB (H.263), video packet (MPEG4), or slice (H.264)
	int nVPSize;	// Video packet size in bits.
	int nVPMbRow;   // Video packet size in number of MB rows. If H.263, this must be multiple of "MBrow per GOB".
	//
	MFE_BOOL bFixedFrameRate;	// If FALSE, rate control can signal frame-skipping.
	int nPCount;			// Number of P-frames between I-frames
	int nBCount;			// Number of B-frames between P-frames // Suggest: 0, 1, 2
	RC_GRANULARITY rcGranularity;
	RC_METHOD rcMethod;
} CVBRRCInfo;

typedef struct {
	/* Input parameters */

	int m_nCodecType;
	int m_nWidth, m_nHeight;
	int m_nTargetFrameRateNum, m_nTargetFrameRateDeNum, m_nTargetFrameRateInt;	// The target rate is Num/Denum
	int m_nBitrate;
	int m_nMaxBitrate; // Only when CONSTRAINED_VARIABLE_BITRATE
	int m_nConstQP;   // Only when CONST_QUALITY
	int m_nVPSize, m_nVPMbRow;
	MFE_BOOL m_bFixedFrameRate;
	int m_nPCount, m_nBCount;
	RC_GRANULARITY m_rcGranularity;
	RC_METHOD m_rcMethod;
	// Derived variables
	int m_nAvgBitsPerFrame;
	int m_nBitsPerFrame[3];	// I, P, B

	/* rate control variables */

	int m_nFrameCount;  // How many frame coded
	int m_nBufFullness;  // Rate control buffer
	MFE_S64 m_nTotalBits;
	// Last-frame status
	MFE_BOOL m_bIsBotField;
	int m_nLastFrameBits, m_nLastFrameAvgQP[2];
	int m_nLastTargetBits;

	/* Bitrate usage compensation */
	int m_nTargetFullness;
	int m_nDeputyCount, m_nMinDeputyCount;

	/* Variable bitrate */
	int m_nLongTermQP64;

	// Model parameters

	int m_nTargetBits;   // target number of bits of current frame
	int m_nTotalMB;			// number of macroblocks in a frame
	int m_nMBN;					// Accumulated handled MB count (within one frame)
	int m_nFrameQP;
	int m_nFrameType;

	/* Only for CONSTRAINED_VARIABLE_BITRATE */
	int m_nMaxOffset;
	int m_BitrateGauge[MAX_GAUGE_SIZE];
	int m_nGaugeCount, m_nGaugeIndex, m_nGaugeBitrate;
	int m_nMaxFrozenFrame;

	// QP, QStep: Min, Max 
	int m_nMinQP, m_nMaxQP;
	int m_nMinQStep, m_nMaxQStep;
	int m_nFrameSkipThrQP;

	/* MB-level rate control */
#ifdef _SUPPORT_MBLEVEL_RC_
	int m_nTargetMbBits;
	int m_nTargetUsedBits;
	int m_nUsedBits[3];	// For HW pipeline delay
	int m_nSumAct;
	int m_nMbWidth, m_nPrevQP, m_nLeftQP, m_nPrevTopQP[MAX_MB_WIDTH];
	int m_nLastTargetQP;	// For sig_dump
	// For coded frame slicing
	int m_nNewPacket;	// New GOB, video packet, slice... Simulating HW er_en
	int m_nLastVPBits;
#endif

	int* m_pMBBitsArray;	// Only for DEBUG usage.
} CVBRRateControl;

// Global
void cvbr_InitRateControl(CVBRRateControl* ct, CVBRRCInfo* pRCInfo);
void cvbr_CloseRateControl(CVBRRateControl* ct);
// Each frame
int cvbr_InitFrame(CVBRRateControl* ct, int nFrameType, char FieldType);    // Return the initial frame qp
int cvbr_UpdateFrame(CVBRRateControl* ct, int totalUsedBits, char bDummyFrame, char FieldType);
// Each macroblock
int cvbr_InitMB(CVBRRateControl* ct, int nVar, const int nPrevQP, const int nBits, int IsIntra, int IsP4MV, int BPredType, int nResetDQ);
void output_MBR_reg(void* hd, CVBRRateControl* ct);

#endif
