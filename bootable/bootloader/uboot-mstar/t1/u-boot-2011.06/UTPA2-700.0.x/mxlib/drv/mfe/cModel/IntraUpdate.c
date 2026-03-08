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

#include "IntraUpdate.h"
//#include <stdlib.h>



/////////////////////////////////////////////////////
// Example App codes
/////////////////////////////////////////////////////

/*
IntraUpdateContext gIUContext;
void app_init()
{
	gIUContext.nWidth = 720;
	gIUContext.nHeight = 480;
	gIUContext.nTotalMb = (gIUContext.nWidth*gIUContext.nHeight)>>8;
	gIUContext.pHwMbMap = (HW_MB_MAP*)malloc(gIUContext.nTotalMb*sizeof(HW_MB_MAP));
	gIUContext.pSwMbMap = (SW_MB_MAP*)malloc(gIUContext.nTotalMb*sizeof(SW_MB_MAP));

	gIUContext.bFIR = 0;
	gIUContext.bCIR = 1;
	gIUContext.bAIR = 0;

	gIUContext.FIR_THR = 132;
	gIUContext.CIR_COUNT = 10;
	gIUContext.AIR_COUNT = 10;
	gIUContext.AIR_ROUND = 1;

	IntraUpdate_Init(&gIUContext);
}

void app_frame_done()
{
	// HW already fill hw map
	IntraUpdate_FrameUpdate(&gIUContext);
}

void app_exit()
{
	if (gIUContext.pHwMbMap) {
		free(gIUContext.pHwMbMap);
		gIUContext.pHwMbMap = NULL;
	}
	if (gIUContext.pSwMbMap) {
		free(gIUContext.pSwMbMap);
		gIUContext.pSwMbMap = NULL;
	}
}
*/



/////////////////////////////////////////////////////
// IntraUpdate implementation
/////////////////////////////////////////////////////

void IntraUpdate_Init(IntraUpdateContext* ctx)
{
	int i;

	ctx->nIndexCIR = 0;
	ctx->nIndexAIR = 0;
	for (i=0; i<ctx->nTotalMb; i++) {
		ctx->pHwMbMap[i].intra = 0;
		ctx->pSwMbMap[i].inter_count = 0;
		ctx->pSwMbMap[i].intra_count = 0;
	}
}

void IntraUpdate_Frame(IntraUpdateContext* ctx)
{
	int i, intra_count, check_count;

	if (!(ctx->bFIR || ctx->bCIR || ctx->bAIR))
		return;

	for (i=0; i<ctx->nTotalMb; i++)
	{
		if (ctx->bAIR) {
			if (ctx->pHwMbMap[i].motion) // 1bit
				ctx->pSwMbMap[i].intra_count = ctx->AIR_ROUND;
		}
		if (ctx->bFIR) {
			if (ctx->pHwMbMap[i].intra)
				ctx->pSwMbMap[i].inter_count = 0;
			else {
				if (ctx->pSwMbMap[i].inter_count<ctx->FIR_THR)
					ctx->pSwMbMap[i].inter_count++;
			}
		}
		// Reset
		ctx->pHwMbMap[i].intra = 0;
	}

	if (ctx->bCIR)
	{
		intra_count = 0;
		i = ctx->nIndexCIR;
		while (intra_count<ctx->CIR_COUNT) {
			if (i==ctx->nTotalMb)
				i=0;
			ctx->pHwMbMap[i].intra = 1;
			intra_count++;
			i++;
		}
		ctx->nIndexCIR = i;
	}

	if (ctx->bFIR || ctx->bAIR)
	{
		// AIR and FIR share the same maximal intra count
		i = ctx->nIndexAIR;
		check_count = 0, intra_count = 0;
		while (check_count<ctx->nTotalMb) {
			if (i>=ctx->nTotalMb)
				i = 0;
			if (ctx->pSwMbMap[i].inter_count>=ctx->FIR_THR) {
				ctx->pHwMbMap[i].intra = 1;
				intra_count++;
				if (ctx->pSwMbMap[i].intra_count>0)
					ctx->pSwMbMap[i].intra_count--;
			}
			else if (ctx->pSwMbMap[i].intra_count>0 && intra_count<ctx->AIR_COUNT) {
				ctx->pHwMbMap[i].intra = 1;
				intra_count++;
				ctx->pSwMbMap[i].intra_count--;
			}
			i++;
			check_count++;
			if (intra_count==ctx->AIR_COUNT) {
				if (ctx->bFIR)
					ctx->nIndexAIR = i;	// The AIR starting pos for next frame
				if (!ctx->bFIR)
					break;	// Short-cut when AIR only
			}
		}
	}
}
