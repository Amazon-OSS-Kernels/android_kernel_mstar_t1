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

	#define TRACE 0
	
//! Bitstream
typedef struct
{
    int             byte_pos;           //!< current position in bitstream;
    int             bits_to_go;         //!< current bitcounter
    MFE_BYTE            byte_buf;           //!< current buffer for last written byte
    MFE_BYTE            *streamBuffer;      //!< actual buffer for written bytes
    int             zerocount;
#if TRACE
    MFE_BOOL             trace_enabled;
#endif
} Bitstream;

//! Syntax Element
typedef struct syntaxelement
{
    int                 type;           //!< type of syntax element for data part.
    int                 value1;         //!< numerical value of syntax element
    int                 value2;         //!< for blocked symbols, e.g. run/level
    int                 len;            //!< length of code
    int                 inf;            //!< info part of UVLC code
    unsigned int        bitpattern;     //!< UVLC bitpattern
    int                 context;        //!< CABAC context
    
#if TRACE
#define             TRACESTRING_SIZE 100            //!< size of trace string
    char                tracestring[TRACESTRING_SIZE];  //!< trace string
#endif
    
    //!< for mapping of syntaxElement to UVLC
    void    (*mapping)(int value1, int value2, int* len_ptr, int* info_ptr);
    
} SyntaxElement;

void SODBtoRBSP(Bitstream *currStream);
int RBSPtoEBSP(MFE_BYTE *streamBuffer, int begin_bytepos, int end_bytepos, int min_num_bytes);

int u_1  (char *tracestring, int value, Bitstream *bitstream);
int se_v (char *tracestring, int value, Bitstream *bitstream);
int ue_v (char *tracestring, int value, Bitstream *bitstream);
int u_v  (int n, char *tracestring, int value, Bitstream *bitstream);

int   symbol2uvlc(SyntaxElement *se);
void  ue_linfo(int n, int dummy, int *len,int *info);
void  se_linfo(int mvd, int dummy, int *len,int *info);

#endif

