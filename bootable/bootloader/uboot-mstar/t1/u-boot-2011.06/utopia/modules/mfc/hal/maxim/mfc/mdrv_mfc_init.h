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

#ifndef _MSINIT_H_
#define _MSINIT_H_

#ifdef _MSINIT_C_
#define _MSINITDEC_
#else
#define _MSINITDEC_ extern
#endif

void MDrv_MFC_InitializeChip(void);
//void MDrv_MFC_PowerDownChip(void);
void MDrv_MFC_PowerDownChipU3(void);
#endif

