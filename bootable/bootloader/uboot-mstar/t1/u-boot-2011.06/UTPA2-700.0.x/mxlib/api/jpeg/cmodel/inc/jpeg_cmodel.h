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
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    jpeg_cmodel.h
/// @brief  JPEG CMODEL
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _JPEG_CMODEL_H_
#define _JPEG_CMODEL_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define AEON_NON_CACHE_MASK  0x0

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_U8 *gpu8Sample_buf;
MS_S32 gs32Crr[256];  //h2v2.c
MS_S32 gs32Cbb[256];  //h2v2.c
MS_S32 gs32Crg[256];  //h2v2.c
MS_S32 gs32Cbg[256];  //h2v2.c

MS_U8 *pgu8Scan_line_0;
MS_U8 *pgu8scan_line_1;

MS_U16 gu16Real_dest_bytes_per_scan_line;
MS_U16 gu16Dest_bytes_per_scan_line;        /* rounded up */
MS_U8 gu8Dest_bytes_per_pixel;            /* currently, 4 (RGB) or 1 (Y) */
JPEG_BLOCK_TYPE *gps16Block_seg[JPEG_MAXBLOCKSPERROW];
MS_U8 gu8Block_max_zag_set[JPEG_MAXBLOCKSPERROW];

void JPEG_CMODEL_create_look_ups( void );
void JPEG_CMODEL_DumpTables(void);  //main.c
MS_S16 JPEG_CMODEL_decode( void **Pscan_line_ofs, MS_U32 *Pscan_line_len );  //main.c
MS_U8 JPEG_CMODEL_get_bytes_per_pixel( void );  //main.c
MS_U8 JPEG_CMODEL_get_num_components( void );  //main.c
MS_U32 JPEG_CMODEL_get_total_bytes_read( void );  //main.c
MS_U8 JPEG_CMODEL_clamp( MS_S32 i );  //h2v2.c

#endif // _JPEG_CMODEL_H_

