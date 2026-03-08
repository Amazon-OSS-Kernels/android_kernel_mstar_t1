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
#ifndef _INTRA_UDPATE_H_
#define _INTRA_UDPATE_H_

#include "mfe_type.h"

// "in" means sw-->hw
// "out" means hw-->sw
typedef struct {
	MS_U32 intra : 1;    // [in, out]: Forced-intra; intra-coded
	MS_U32 motion : 1;   // [out] This MB is regarded as motion area
} HW_MB_MAP;

typedef struct {
	MS_S32 inter_count;	// For FIR usage
	MS_S32 intra_count;	// For AIR usage
} SW_MB_MAP;

typedef struct _IntraUpdateContext {
	// Input
	MS_S32 nWidth, nHeight;
	MS_S32 nTotalMb;
	HW_MB_MAP *pHwMbMap;	// Hw/sw shared. of size nTotalSize
	SW_MB_MAP *pSwMbMap;	// sw only. of size nTotalSize
	// Intra Refresh switches
	MS_U8 bFIR;		// H.263 Forced Updating
	MS_U8 bCIR;		// Cyclic intra updating
	MS_U8 bAIR;		// Adaptive Intra-Refresh. Must be FALSE if bCIR is FALSE
	// Intra Refresh constants
	MS_S32 FIR_THR;
	MS_S32 CIR_COUNT;
	MS_S32 AIR_COUNT, AIR_ROUND;
	// Internal counter
	MS_S32 nIndexCIR;
	MS_S32 nIndexAIR;
} IntraUpdateContext;

#ifdef __cplusplus
extern "C" {
#endif

void IntraUpdate_Init(IntraUpdateContext* ctx);
void IntraUpdate_Frame(IntraUpdateContext* ctx);

#ifdef __cplusplus
}
#endif

#endif // _INTRA_UDPATE_H_
