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
 *************************************************************************************
 * \file vlc.h
 *
 * \brief
 *    Prototypes for VLC coding funtions
 * \author
 *     Karsten Suehring
 *************************************************************************************
 */

#ifndef _VLC_H_
#define _VLC_H_

	#define TRACE 0UL
	
//! Bitstream
typedef struct
{
    MS_S32             byte_pos;           //!< current position in bitstream;
    MS_S32             bits_to_go;         //!< current bitcounter
    MS_U8            byte_buf;           //!< current buffer for last written byte
    MS_U8            *streamBuffer;      //!< actual buffer for written bytes
    MS_S32             zerocount;
#if TRACE
    MS_BOOL             trace_enabled;
#endif
} Bitstream;

//! Syntax Element
typedef struct syntaxelement
{
    MS_S32                 type;           //!< type of syntax element for data part.
    MS_S32                 value1;         //!< numerical value of syntax element
    MS_S32                 value2;         //!< for blocked symbols, e.g. run/level
    MS_S32                 len;            //!< length of code
    MS_S32                 inf;            //!< info part of UVLC code
    MS_U32        bitpattern;     //!< UVLC bitpattern
    MS_S32                 context;        //!< CABAC context
    
#if TRACE
#define             TRACESTRING_SIZE 100UL            //!< size of trace string
    MS_S8                tracestring[TRACESTRING_SIZE];  //!< trace string
#endif
    
    //!< for mapping of syntaxElement to UVLC
    void    (*mapping)(MS_S32 value1, MS_S32 value2, MS_S32* len_ptr, MS_S32* info_ptr);
    
} SyntaxElement;

void SODBtoRBSP(Bitstream *currStream);
MS_S32 RBSPtoEBSP(MS_U8 *streamBuffer, MS_S32 begin_bytepos, MS_S32 end_bytepos, MS_S32 min_num_bytes);

MS_S32 u_1  (MS_S8 *tracestring, MS_S32 value, Bitstream *bitstream);
MS_S32 se_v (MS_S8 *tracestring, MS_S32 value, Bitstream *bitstream);
MS_S32 ue_v (MS_S8 *tracestring, MS_S32 value, Bitstream *bitstream);
MS_S32 u_v  (MS_S32 n, MS_S8 *tracestring, MS_S32 value, Bitstream *bitstream);

MS_S32 symbol2uvlc(SyntaxElement *se);
void ue_linfo(MS_S32 n, MS_S32 dummy, MS_S32 *len, MS_S32 *info);
void se_linfo(MS_S32 mvd, MS_S32 dummy, MS_S32 *len, MS_S32 *info);
void writeVlcByteAlign(Bitstream* currStream);
#endif

