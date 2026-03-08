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


/////////////////////////////////////////////////////
// IntraUpdate implementation
/////////////////////////////////////////////////////

void IntraUpdate_Init(IntraUpdateContext* ctx)
{
	MS_S32 i;

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
	MS_S32 i, intra_count, check_count;

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
