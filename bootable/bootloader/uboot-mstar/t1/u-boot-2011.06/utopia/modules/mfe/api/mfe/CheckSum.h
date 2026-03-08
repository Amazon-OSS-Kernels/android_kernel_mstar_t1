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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#ifndef CHECKSUM_H
#define CHECKSUM_H

//extern unsigned int adler;

void InitCheckSum(unsigned int *p_adler);
void UpdateCheckSum(unsigned char* ptr, int len, unsigned int *p_adler);

#if 0
void crco_add_frame(uchar* y, uchar* u, uchar* v,
                    int width, int height,
                    int stride_y, int stride_uv,
                    int active_x, int active_y
                    );
#endif

#endif
