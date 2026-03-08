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



#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "mdrv_mfe_math.h"
#include "ms_dprintf.h"



#include "m4v_header.h"
#include "msRateCtrl.h"



#define MARKER_BIT						1UL

#define START_CODE_PREFIX				1UL
#define NUMBITS_START_CODE_PREFIX		24UL
//	Added for data partitioning mode By Toshiba(1998-1-16:DP+RVLC)
#define NUMBITS_START_CODE_SUFFIX		8UL
//	End Toshiba(1998-1-16:DP+RVLC)

// session overhead information
#define USER_DATA_START_CODE			0xB2UL
#define VSS_START_CODE					0xB0UL	// 8-bit
#define VSS_END_CODE					0xB1UL	// 8-bit
#define NUMBITS_VSS_PROFILE				8UL
#define VSO_START_CODE					0xB5UL	// 8-bit
#define VSO_VERID						1UL
#define VSO_TYPE						1UL
#define NUMBITS_VSO_VERID				4UL
#define NUMBITS_VSO_PRIORITY			3UL
#define NUMBITS_VSO_TYPE				4UL

// VO overhead information
#define NUMBITS_VO_START_CODE			3UL
#define VO_START_CODE					0UL
#define NUMBITS_VO_ID					5UL

#define NUMBITS_NEWPRED_ENABLE			1UL

// VOL overhead information
#define NUMBITS_SHORT_HEADER_START_CODE 22UL // Added by KPN for short headers (1998-02-07, DS)
#define SHORT_VIDEO_START_MARKER        32UL // Added by KPN for short headers (1998-02-07, DS)
#define NUMBITS_VOL_START_CODE			4UL
#define VOL_START_CODE					2UL
#define NUMBITS_VOL_ID					4UL
#define NUMBITS_VOL_SHAPE				2UL
#define NUMBITS_TIME_RESOLUTION			16UL
#define NUMBITS_VOL_FCODE				3UL
#define NUMBITS_SEP_MOTION_TEXTURE		1UL
#define NUMBITS_QMATRIX					8UL

// VOP overhead information
#define VOP_START_CODE					0xB6UL
#define NUMBITS_VOP_START_CODE			8UL
#define NUMBITS_VOP_TIMEINCR			10UL
#define NUMBITS_VOP_HORIZONTAL_SPA_REF	13UL
#define NUMBITS_VOP_VERTICAL_SPA_REF	13UL
#define NUMBITS_VOP_WIDTH				13UL
#define NUMBITS_VOP_HEIGHT				13UL
#define NUMBITS_VOP_PRED_TYPE			2UL
#define NUMBITS_VOP_QUANTIZER			5UL
#define NUMBITS_VOP_ALPHA_QUANTIZER		6UL
#define NUMBITS_VOP_FCODE				3UL



void codeSequenceHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, (MS_S8*)("VSS_Start_Code"));
	osPutBits(pStream, VSS_START_CODE, NUMBITS_START_CODE_SUFFIX, (MS_S8*)("VSS_Start_Code"));
	osPutBits(pStream, 0xF5/*m_volmd.uiProfileAndLevel*/, NUMBITS_VSS_PROFILE, (MS_S8*)("VSS_Profile_Level"));

	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, (MS_S8*)("VSO_Start_Code"));
	osPutBits(pStream, VSO_START_CODE, NUMBITS_START_CODE_SUFFIX, (MS_S8*)("VSO_Start_Code"));
	osPutBits(pStream, 0, 1, (MS_S8*)("VSO_IsVisualObjectIdentifier"));
	osPutBits(pStream, VSO_TYPE, NUMBITS_VSO_TYPE, (MS_S8*)("VSO_Type"));
	osPutBits(pStream, 0, 1, (MS_S8*)("VSO_VideoSignalType"));

// 	UInt uiNumBits = NUMBITS_START_CODE_PREFIX + NUMBITS_START_CODE_SUFFIX + NUMBITS_VSS_PROFILE
// 		+ NUMBITS_START_CODE_PREFIX + 1 + NUMBITS_VSO_TYPE + 1;
//	m_statsVOL.nBitsHead += uiNumBits;

//	m_statsVOL.nBitsStuffing += m_pbitstrmOut->flush ();
	osNextStartCode(pStream);
}

void codeVOHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, (MS_S8*)("VO_Start_Code"));
	osPutBits(pStream, VO_START_CODE, NUMBITS_VO_START_CODE, (MS_S8*)("VO_Start_Code"));		//plus 3 bits
	osPutBits(pStream, /*m_uiVOId*/1, NUMBITS_VO_ID, (MS_S8*)("VO_Id"));
// 	U32 uiNumBits = NUMBITS_START_CODE_PREFIX + NUMBITS_VO_START_CODE + NUMBITS_VO_ID;
// 	m_statsVOL.nBitsHead += uiNumBits;
}

void codeVOLHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;
	CVBRRateControl* rcCtx = &pConfig->ctxRateControl;

	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, (MS_S8*)("VOL_Start_Code_Prefix"));
	osPutBits(pStream, VOL_START_CODE, NUMBITS_VOL_START_CODE, (MS_S8*)("VOL_Start_Code"));		//plus 4 bits
	osPutBits(pStream, 0, NUMBITS_VOL_ID, (MS_S8*)("VOL_Id")); // by katata
//	m_statsVOL.nBitsHead+=NUMBITS_START_CODE_PREFIX+NUMBITS_VOL_START_CODE+NUMBITS_VOL_ID;

	osPutBits(pStream, (MS_S32) 0, 1, (MS_S8*)("VOL_Random_Access"));		//isn't this a system level flg?
//	m_statsVOL.nBitsHead++;

	osPutBits(pStream, (MS_S32) 1, 8, (MS_S8*)("VOL_Type_Indicator")); // Set to indicate SIMPLE profile.
//	m_statsVOL.nBitsHead+=8;

	osPutBits(pStream, (MS_S32) 1, 1, (MS_S8*)("VOL_Is_Object_Layer_Identifier"));
	osPutBits(pStream, (MS_S32) 2/*m_volmd.uiVerID*/, 4, (MS_S8*)("VOL_Verid"));
	osPutBits(pStream, (MS_S32) 1, 3, (MS_S8*)("VOL_Priority"));
//	m_statsVOL.nBitsHead+=8;

	osPutBits(pStream, (MS_S32) 1, 4, (MS_S8*)("aspect_ratio_info")); // square pix
//	m_statsVOL.nBitsHead+=4;

	osPutBits(pStream, /*m_volmd.uiVolControlParameters*/0, 1, (MS_S8*)("VOL_Control_Parameters")); //useless flag for now
//	m_statsVOL.nBitsHead++;

	{
		osPutBits(pStream, /*iAUsage*/0, NUMBITS_VOL_SHAPE, (MS_S8*)("VOL_Shape_Type"));
		osPutBits(pStream, 1, 1, (MS_S8*)("Marker"));
//		m_statsVOL.nBitsStuffing ++;

		osPutBits(pStream, pInfo->nTimeResolution/*m_volmd.iClockRate*/, NUMBITS_TIME_RESOLUTION, (MS_S8*)("VOL_Time_Increment_Resolution"));
		osPutBits(pStream, 1, 1, (MS_S8*)("Marker"));
//		m_statsVOL.nBitsStuffing ++;

		osPutBits(pStream, 0, 1, (MS_S8*)("VOL_Fixed_Vop_Rate"));
//		m_statsVOL.nBitsHead += NUMBITS_VOL_SHAPE + NUMBITS_TIME_RESOLUTION + 1;

		osPutBits(pStream, MARKER_BIT, 1, (MS_S8*)("Marker_Bit"));
//		m_statsVOL.nBitsStuffing ++;
		osPutBits(pStream, pConfig->nBufWidth, NUMBITS_VOP_WIDTH, (MS_S8*)("VOL_Width"));
		osPutBits(pStream, MARKER_BIT, 1, (MS_S8*)("Marker_Bit"));
//		m_statsVOL.nBitsStuffing ++;
		osPutBits(pStream, pConfig->nBufHeight, NUMBITS_VOP_HEIGHT, (MS_S8*)("VOL_Height"));
		osPutBits(pStream, MARKER_BIT, 1, (MS_S8*)("Marker_Bit"));
//		m_statsVOL.nBitsStuffing ++;
//		m_statsVOL.nBitsHead += NUMBITS_VOP_WIDTH + NUMBITS_VOP_HEIGHT;
	}
	osPutBits(pStream, pInfo->bInterlace, 1, (MS_S8*)("VOL_interlace"));
//	m_statsVOL.nBitsHead++;
	osPutBits(pStream, 1/*m_volmd.bAdvPredDisable*/, 1, (MS_S8*)("VOL_OBMC_Disable"));
//	m_statsVOL.nBitsHead++;

	// code sprite info
	osPutBits(pStream, 0/*m_uiSprite*/, 2, (MS_S8*)("VOL_Sprite_Usage"));
//	m_statsVOL.nBitsHead += 2;

	// NBIT
	osPutBits(pStream, (MS_S32) 0/*m_volmd.bNot8Bit*/, 1, (MS_S8*)("VOL_NOT_8_BIT_VIDEO"));
//	m_statsVOL.nBitsHead++;

	osPutBits(pStream, (MS_S32) pInfo->bQuantizerType/*m_volmd.fQuantizer*/, 1, (MS_S8*)("VOL_Quant_Type"));
//	m_statsVOL.nBitsHead++;
	if (pInfo->bQuantizerType/*m_volmd.fQuantizer*/==1) {
		osPutBits(pStream, 0/*m_volmd.bLoadIntraMatrix*/, 1, (MS_S8*)("VOL_Load_Q_Matrix (intra)"));
//		m_statsVOL.nBitsHead++;
		osPutBits(pStream, 0/*m_volmd.bLoadInterMatrix*/, 1, (MS_S8*)("VOL_Load_Q_Matrix (inter)"));
//		m_statsVOL.nBitsHead++;
	}

	osPutBits(pStream, 0/*m_volmd.bQuarterSample*/, 1, (MS_S8*)("VOL_Quarter_Sample"));
//	m_statsVOL.nBitsHead += 1;

	// START: Complexity Estimation syntax support - Marc Mongenet (EPFL) - 16 Jun 1998
	osPutBits(pStream, 1/*m_volmd.bComplexityEstimationDisable*/, 1, (MS_S8*)("complexity_estimation_disable"));
//	m_statsVOL.nBitsHead += 1;

	osPutBits(pStream, (rcCtx->m_nVPMbRow<=0&&rcCtx->m_nVPSize<=0)?1:0/*m_volmd.bResyncMarkerDisable*/ , 1, (MS_S8*)("VOL_resync_marker_disable"));
//	m_statsVOL.nBitsHead ++;

	osPutBits(pStream, 0/*m_volmd.bDataPartitioning*/, 1, (MS_S8*)("VOL_data_partitioning"));
// 	m_statsVOL.nBitsHead ++;
// 	if( m_volmd.bDataPartitioning )
// 	{
// 		osPutBits(pStream, m_volmd.bReversibleVlc, 1, (MS_S8*)("VOL_reversible_vlc"));
// 		m_statsVOL.nBitsHead ++;
// 	}

	osPutBits(pStream, 0/*m_volmd.bNewpredEnable*/, NUMBITS_NEWPRED_ENABLE, (MS_S8*)("*VOL_newpred_enable"));
//	m_statsVOL.nBitsHead += NUMBITS_NEWPRED_ENABLE;
	osPutBits(pStream, 0/*m_volmd.breduced_resolution_vop_enable*/, 1, (MS_S8*)("reduced_resolution_vop_enable"));
//	m_statsVOL.nBitsHead ++;

	osPutBits(pStream, 0/*m_volmd.volType == ENHN_LAYER*/, 1, (MS_S8*)("VOL_Scalability"));
//	m_statsVOL.nBitsHead++;

	osNextStartCode(pStream);
}

static void codeVOPHeadInitial(MFE_CONFIG *pConfig, OutStream* pStream)
{
	MS_S32 tCurrSec;
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;

	// Start code
	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, (MS_S8*)("VOP_Start_Code"));
	osPutBits(pStream, VOP_START_CODE, NUMBITS_VOP_START_CODE, (MS_S8*)("VOP_Start_Code"));
//	m_statsVOP.nBitsHead += NUMBITS_START_CODE_PREFIX + NUMBITS_VOP_START_CODE;

	// prediction type
	osPutBits(pStream, pConfig->vopPredType==I_VOP?0:(pConfig->vopPredType==P_VOP?1:2), NUMBITS_VOP_PRED_TYPE, (MS_S8*)("VOP_Pred_Type"));
//	m_statsVOP.nBitsHead += NUMBITS_VOP_PRED_TYPE;

	// Time reference
	tCurrSec = (MS_S32)MFE_DIV_S64((MS_S64)pInfo->m_t * pInfo->nFixedIncrement, pInfo->nTimeResolution) ;
	//tCurrSec = (MS_S32)((INT64)pInfo->m_t * pInfo->nFixedIncrement / pInfo->nTimeResolution);// current time in seconds

//      printk("\nm_t = %d,nFixedIncrement = %d nTimeResolution=%d\n",pInfo->m_t ,pInfo->nFixedIncrement,pInfo->nTimeResolution);
//      printk("\n tCurrSec = %d,pInfo->m_tModuloBaseDecd = %d\n",tCurrSec,pInfo->m_tModuloBaseDecd);

	pInfo->m_nBitsModuloBase = tCurrSec - ((pConfig->vopPredType != B_VOP) ?
			pInfo->m_tModuloBaseDecd : pInfo->m_tModuloBaseDisp);
	MS_ASSERT(pInfo->m_nBitsModuloBase<=31);

	osPutBits(pStream, (MS_S32) 0xFFFFFFFE & (0xFFFFFFFF>>(32-(pInfo->m_nBitsModuloBase+1))), pInfo->m_nBitsModuloBase + 1, (MS_S8*)("VOP_Modulo_Time_Base"));
//	m_statsVOP.nBitsHead += m_nBitsModuloBase + 1;

	pInfo->m_iVopTimeIncr = (MS_S32)(MFE_DIV_S64((MS_S64)pInfo->m_t * pInfo->iClockRate * pInfo->nFixedIncrement, pInfo->nTimeResolution)
	       - (MS_S64)tCurrSec * pInfo->iClockRate) ;
	//pInfo->m_iVopTimeIncr = (MS_S32)((INT64)pInfo->m_t * pInfo->iClockRate * pInfo->nFixedIncrement / pInfo->nTimeResolution- (INT64)tCurrSec * pInfo->iClockRate);

	//printf("codeVOPHeadInitial: m_t=%d, iClockRate=%d, nFixedIncrement=%d, nTimeResolution=%d, tCurrSec=%d\n",
	//	pInfo->m_t, pInfo->iClockRate, pInfo->nFixedIncrement, pInfo->nTimeResolution, tCurrSec);

	osPutBits(pStream, (MS_S32) 1, 1, (MS_S8*)("Marker")); // marker bit	Added for error resilient mode by Toshiba(1997-11-14)
//	m_statsVOP.nBitsStuffing ++;

	if(pInfo->nNumBitsTimeIncr!=0)
		osPutBits(pStream, pInfo->m_iVopTimeIncr, pInfo->nNumBitsTimeIncr, (MS_S8*)("VOP_Time_Incr"));
//	m_statsVOP.nBitsHead += m_iNumBitsTimeIncr;// Modified for error resilient mode by Toshiba(1997-11-14)

	osPutBits(pStream, (MS_S32) 1, 1, (MS_S8*)("Marker")); // marker bit
//	m_statsVOP.nBitsStuffing ++;

	if ( pConfig->vopPredType != B_VOP)
	{	//update modulo time base
		pInfo->m_tModuloBaseDisp = pInfo->m_tModuloBaseDecd;			//of the most recently displayed I/Pvop
		pInfo->m_tModuloBaseDecd = tCurrSec;							//of the most recently decoded I/Pvop
	}

// 	if (m_tFrameInterval == 0 && pInfo->bInterlace && pInfo->vopPredType == B_VOP) {
// 		m_tFrameInterval = pInfo->m_t - pInfo->m_tPastRef;
// 	}
}

void codeNonCodedVOPHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	codeVOPHeadInitial(pConfig, pStream);

	osPutBits(pStream, 0, 1, (MS_S8*)("VOP_Coded"));
//	m_statsVOP.nBitsHead++;
}


void codeVOPHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;

	codeVOPHeadInitial(pConfig, pStream);

	osPutBits(pStream, 1, 1, (MS_S8*)("VOP_Coded"));
//	m_statsVOP.nBitsHead++;

	if (pConfig->vopPredType == P_VOP) {
		osPutBits(pStream, pInfo->iRoundingControl, 1, (MS_S8*)("VOP_Rounding_Type"));
//		m_statsVOP.nBitsHead++;
	}

	osPutBits(pStream, 0/*m_vopmd.iIntraDcSwitchThr*/, 3, (MS_S8*)("IntraDCSwitchThr"));
//	m_statsVOP.nBitsHead+=3;

// INTERLACE_
	if (pInfo->bInterlace) {
		osPutBits(pStream, 1/*m_vopmd.bTopFieldFirst*/, 1, (MS_S8*)("Top_Field_First"));
		osPutBits(pStream, 0/*m_vopmd.bAlternateScan*/, 1, (MS_S8*)("Alternate_Scan"));
//		m_statsVOP.nBitsHead += 2;
	}

// INTERLACE

	if (pConfig->vopPredType == I_VOP) {
		osPutBits(pStream, pInfo->intQP, 5/*m_volmd.uiQuantPrecision*/, (MS_S8*)("VOP_QUANT"));
//		m_statsVOP.nBitsHead += m_volmd.uiQuantPrecision;
	}
	else if (pConfig->vopPredType == P_VOP) {
		osPutBits(pStream, pInfo->intQP, 5/*m_volmd.uiQuantPrecision*/, (MS_S8*)("VOP_QUANT"));
//		m_statsVOP.nBitsHead += m_volmd.uiQuantPrecision;
		osPutBits(pStream, pInfo->iFCode/*m_vopmd.mvInfoForward.uiFCode*/, NUMBITS_VOP_FCODE, (MS_S8*)("VOP_Fcode_Forward"));
//		m_statsVOP.nBitsHead += NUMBITS_VOP_FCODE;
	}
	else if (pConfig->vopPredType == B_VOP) {
		osPutBits(pStream, pInfo->intQP, 5/*m_volmd.uiQuantPrecision*/, (MS_S8*)("VOP_QUANT"));
//		m_statsVOP.nBitsHead += m_volmd.uiQuantPrecision;
		osPutBits(pStream, pInfo->iFCode/*m_vopmd.mvInfoForward.uiFCode*/, NUMBITS_VOP_FCODE, (MS_S8*)("VOP_Fcode_Forward"));
//		m_statsVOP.nBitsHead += NUMBITS_VOP_FCODE;
		osPutBits(pStream, pInfo->iFCode/*m_vopmd.mvInfoBackward.uiFCode*/, NUMBITS_VOP_FCODE, (MS_S8*)("VOP_Fcode_Backward"));
//		m_statsVOP.nBitsHead += NUMBITS_VOP_FCODE;
	}
}

//#define FORCE_30_CLOCK
void codeVOPShortHead(MFE_CONFIG *pConfig, OutStream* pStream)
{
	MS_S32 nSourceFormat;
	MS_S32 iTemporalRef;
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;

    osPutBits(pStream, SHORT_VIDEO_START_MARKER, NUMBITS_SHORT_HEADER_START_CODE, (MS_S8*)("VOP_SH_Start_Code"));
#ifdef FORCE_30_CLOCK
	{
		MS_S32 tmp=1;
		if(m_t<=1)
			tmp=0;
		iTemporalRef = (MS_S32)MFE_DIV_S64((MS_S64)pInfo->m_t *(15+tmp) *  pInfo->nFixedIncrement, pInfo->nTimeResolution) -tmp;
		//iTemporalRef = (MS_S32)((INT64)pInfo->m_t * (15+tmp) *  pInfo->nFixedIncrement / pInfo->nTimeResolution) -tmp;
	}
#else
		iTemporalRef = (MS_S32)MFE_DIV_S64((MS_S64)pInfo->m_t * pInfo->iClockRate *  pInfo->nFixedIncrement, pInfo->nTimeResolution);
		//iTemporalRef = (MS_S32)((INT64)pInfo->m_t * pInfo->iClockRate *  pInfo->nFixedIncrement / pInfo->nTimeResolution);

#endif
    osPutBits(pStream, iTemporalRef&0xff, 8, (MS_S8*)("temporal_reference"));
    osPutBits(pStream, 1, 1, (MS_S8*)("marker_bit"));
    osPutBits(pStream, 0, 1, (MS_S8*)("zero_bit"));
    osPutBits(pStream, 0, 1, (MS_S8*)("split_screen_indicator"));
    osPutBits(pStream, 0, 1, (MS_S8*)("document_camera_indicator"));
    osPutBits(pStream, 0, 1, (MS_S8*)("full_picture_freeze_release"));
//    m_statsVOP.nBitsHead += NUMBITS_SHORT_HEADER_START_CODE+8+5;

    if (pConfig->nBufWidth == 128 && pConfig->nBufHeight == 96) {
		nSourceFormat = 1;
    } else if (pConfig->nBufWidth == 176 && pConfig->nBufHeight == 144) {
		nSourceFormat = 2;
    } else if (pConfig->nBufWidth == 352 && pConfig->nBufHeight == 288) {
		nSourceFormat = 3;
    } else if (pConfig->nBufWidth == 704 && pConfig->nBufHeight == 576) {
		nSourceFormat = 4;
    } else if (pConfig->nBufWidth == 1408 && pConfig->nBufHeight == 1152) {
		nSourceFormat = 5;
    } else {
		nSourceFormat = 7;	// 111: Extended PTYPE
    }
	osPutBits(pStream, nSourceFormat, 3, (MS_S8*)("source_format"));
//    m_statsVOP.nBitsHead += 3;

	if (nSourceFormat<=5) {
		osPutBits(pStream, pConfig->vopPredType==P_VOP, 1, (MS_S8*)("picture_coding_type"));
		osPutBits(pStream, 0, 4, (MS_S8*)("four_reserved_zero_bits"));
//		m_statsVOP.nBitsHead += 5;
	}
	else {	// PLUSTYPE
		MS_S32 UFEP, OPPTYPE, MPPTYPE, CPFMT;
		// Update Full Extended PTYPE (UFEP) (3 bits)
		if (pConfig->vopPredType==I_VOP)
			UFEP = 1;	// 001
		else
			UFEP = 0;	// 000
		osPutBits(pStream, UFEP, 3, (MS_S8*)("UFEP"));
//		m_statsVOP.nBitsHead += 3;

		// The Optional Part of PLUSPTYPE (OPPTYPE) (18 bits)
		if (UFEP==1) {
			OPPTYPE = (6<<15)	// Source Format: "110" custom source format
				| (0 <<14)	// Optional Custom PCF
				| (0 <<13)	// Optional Unrestricted Motion Vector (UMV) mode (see Annex D)
				| (0 <<12)	// Optional Syntax-based Arithmetic Coding (SAC) mode (see Annex E)
				| (0 <<11)	// Optional Advanced Prediction (AP) mode (see Annex F)
				| (0 <<10)	// Optional Advanced INTRA Coding (AIC) mode (see Annex I)
				| (0 << 9)	// Optional Deblocking Filter (DF) mode (see Annex J)
				| (0 << 8)	// Optional Slice Structured (SS) mode (see Annex K)
				| (0 << 7)	// Optional Reference Picture Selection (RPS) mode (see Annex N)
				| (0 << 6)	// Optional Independent Segment Decoding (ISD) mode (see Annex R)
				| (0 << 5)	// Optional Alternative INTER VLC (AIV) mode (see Annex S)
				| (0 << 4)	// Optional Modified Quantization (MQ) mode (see Annex T)
				| (1 << 3);	// Equal to "1" to prevent start code emulation
			osPutBits(pStream, OPPTYPE, 18, (MS_S8*)("OPPTYPE"));
//			m_statsVOP.nBitsHead += 18;
		}

		// The mandatory part of PLUSPTYPE when PLUSPTYPE present (MPPTYPE) (9 bits)
		MPPTYPE = ((pConfig->vopPredType==I_VOP?0:1)<<6)
			| (0 << 5)		// Optional Reference Picture Resampling (RPR) mode (see Annex P)
			| (0 << 4)		// Optional Reduced-Resolution Update (RRU) mode (see Annex Q)
			| (0 << 3)		// Rounding Type (RTYPE)
			| (1     );		// Equal to "1" to prevent start code emulation
		osPutBits(pStream, MPPTYPE, 9, (MS_S8*)("MPPTYPE"));
//		m_statsVOP.nBitsHead += 9;

		// CPM=0, PSBI=N/A
		osPutBits(pStream, 0, 1, (MS_S8*)("CPM=0, PSBI=N/A"));
//		m_statsVOP.nBitsHead += 1;

		if (UFEP==1) {
			// Custom Picture Format (CPFMT) (23 bits)
			CPFMT = (2 << 19)	// Pixel Aspect Ratio Code. 2: 12:11 (CIF for 4:3 picture)
				| ((pConfig->nBufWidth/4-1) << 10)	// Picture Width Indication: Range [0, ... , 511]; Number of pixels per line = (PWI + 1) * 4
				| (1 << 9)					// Equal to "1" to prevent start code emulation
				| (pConfig->nBufHeight/4);			// Picture Height Indication: Range [1, ... , 288]; Number of lines = PHI * 4
			osPutBits(pStream, CPFMT, 23, (MS_S8*)("CPFMT"));
//			m_statsVOP.nBitsHead += 23;
		}
	}

    osPutBits(pStream, pInfo->intQP, 5, (MS_S8*)("vop_quant"));
//    m_statsVOP.nBitsHead += 5;
	if (nSourceFormat<=5) {
		osPutBits(pStream, 0, 1, (MS_S8*)("zero_bit"));
		osPutBits(pStream, 0, 1, (MS_S8*)("pei"));
//		m_statsVOP.nBitsHead += 2;
	}
	else {
		osPutBits(pStream, 0, 1, (MS_S8*)("pei"));
//		m_statsVOP.nBitsHead += 1;
	}

	// Set GOB info
	pInfo->m_iGobFrameId = (pConfig->vopPredType==P_VOP ? 1 : 0);//dec_count&0x3;
}

void codeNonCodedVOPShortHead(MFE_CONFIG *pConfig, OutStream* pStream)
{
	MS_S32 i, nTotalMBs;

	codeVOPShortHead(pConfig, pStream);

	// Every MB is skipped MB
	nTotalMBs = (pConfig->nBufWidth*pConfig->nBufHeight)>>8;
	for (i=0; i<nTotalMBs; i++)
		osPutBits(pStream, 1, 1, (MS_S8*)("not_coded"));
}
