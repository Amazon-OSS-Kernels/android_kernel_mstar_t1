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

/*!
 **************************************************************************************
 * \file
 *    parset.h
 * \brief
 *    Picture and Sequence Parameter Sets, encoder operations
 *
 * \date 25 November 2002
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *      - Stephan Wenger        <stewe@cs.tu-berlin.de>
 ***************************************************************************************
 */


#ifndef _PARSET_H_
#define _PARSET_H_

//#include "sei.h"

void MallocSPSPPSBuf(MFE_CONFIG* pConfig);
void FreeSPSPPSBuf(MFE_CONFIG* pConfig);
void GenerateSPSPPS(MFE_CONFIG* pConfig);

void codeSPSPPS(MFE_CONFIG* pConfig,OutStream* pStream);
void codeSliceHeader(MFE_CONFIG* pConfig, OutStream* pStream);

MS_U8 get_NALU_1stbyte(H264INFO *pInfo, MS_U8 vopPredType);

// The following are local helpers, but may come handy in the future, hence public
void GenerateSequenceParameterSet(MFE_CONFIG* pConfig, seq_parameter_set_rbsp_t *sps, MS_S32 SPS_id);
void GeneratePictureParameterSet (MFE_CONFIG* pConfig, pic_parameter_set_rbsp_t *pps, seq_parameter_set_rbsp_t *sps, MS_S32 PPS_id);

#endif
