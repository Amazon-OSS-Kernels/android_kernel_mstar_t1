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
//------------------------------------------------------------------------------
// h2v2.cpp
// Upsampling/colorspace conversion (H2V2, YCbCr)
// Last updated: Nov. 16, 2000
// Copyright (C) 1994-2000 Rich Geldreich
// richgel@voicenet.com
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//------------------------------------------------------------------------------

#include "jpegmain.h"
#include "apiJPEG.h"
#include "jpeg_cmodel.h"
//------------------------------------------------------------------------------
#define FIX(x, b) ((long) ((x) * (1L<<(b)) + 0.5))
//------------------------------------------------------------------------------
// YCbCr H2V2 (2x2:1:1, 6 blocks per MCU) to 24-bit RGB
// FIXME: Create all-asm version, so Intel Compiler isn't needed.

extern MS_U8 gu8Max_mcu_y_size;         /* MCU's max. Y size in pixels */
extern MS_U16 gu16Max_mcus_per_row;
extern MS_U16 gu16Mcu_lines_left;       /* total # lines left in this MCU */

void msAPI_JPEG_H2V2Convert( void )
{
    U8 row = gu8Max_mcu_y_size - gu16Mcu_lines_left;
    U8 *d0 = pgu8Scan_line_0;
    U8 *d1 = pgu8scan_line_1;
    U8 *y;
    U8 *c;
    U16 i, l, j;
//printf("H2V2::msAPI_JPEG_H2V2Convert\n");
    if ( row < 8 )
    {
        y = gpu8Sample_buf + row * 8;
    }
    else
    {
        y = gpu8Sample_buf + 64 * 2 + ( row & 7 ) * 8;
    }

    c = gpu8Sample_buf + 64 * 4 + ( row >> 1 ) * 8;

    for ( i = gu16Max_mcus_per_row; i > 0; i-- )
    {
        for ( l = 0; l < 2; l++ )
        {
            for ( j = 0; j < 8; j += 2 )
            {
                U8 cb = c[0];
                U8 cr = c[64];

                S32 rc = gs32Crr[cr];
                S32 gc = ( ( gs32Crg[cr] + gs32Cbg[cb] ) >> 16 );
                S32 bc = gs32Cbb[cb];

                U16 yy = y[j];
                d0[0] = JPEG_CMODEL_clamp( yy + rc );
                d0[1] = JPEG_CMODEL_clamp( yy + gc );
                d0[2] = JPEG_CMODEL_clamp( yy + bc );

                yy = y[j + 1];
                d0[4] = JPEG_CMODEL_clamp( yy + rc );
                d0[5] = JPEG_CMODEL_clamp( yy + gc );
                d0[6] = JPEG_CMODEL_clamp( yy + bc );

                yy = y[j + 8];
                d1[0] = JPEG_CMODEL_clamp( yy + rc );
                d1[1] = JPEG_CMODEL_clamp( yy + gc );
                d1[2] = JPEG_CMODEL_clamp( yy + bc );

                yy = y[j + 8 + 1];
                d1[4] = JPEG_CMODEL_clamp( yy + rc );
                d1[5] = JPEG_CMODEL_clamp( yy + gc );
                d1[6] = JPEG_CMODEL_clamp( yy + bc );

                d0 += 8;
                d1 += 8;

                c++;
            }
            y += 64;
        }

        y += 64 * 6 - 64 * 2;
        c += 64 * 6 - 8;
    }
}

