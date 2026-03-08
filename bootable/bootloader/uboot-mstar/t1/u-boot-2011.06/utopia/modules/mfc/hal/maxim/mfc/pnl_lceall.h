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
/******************************************************************************

 [Module Name]: Pnl_Sh20.h
 [Date]:        11-Feb-2004
 [Comment]:
   Panel parameters.[B7B-PH-SM]
 [Reversion History]:
*******************************************************************************/

#ifndef _PNL_LCEAll_H
#define _PNL_LCEAll_H

///////////////////////////////////////////////
// Common setting
///////////////////////////////////////////////
#define PANEL_NAME      			"PNL_LCEAll"
#define PANEL_INC_VTOTAL_FOR_50HZ				1

	extern code U8 tOD42[];
	extern code U8 tOD47[];
	extern code U8 tOD37[];
    extern code U8 tInitializeDAC[];
    extern code U8 tInitializeVCOM[];
    extern U32 gDACLen;
    extern U32 gVCOMLen;

#endif
