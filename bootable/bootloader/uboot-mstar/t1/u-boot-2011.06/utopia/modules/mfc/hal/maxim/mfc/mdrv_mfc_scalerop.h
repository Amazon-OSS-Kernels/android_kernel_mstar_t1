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
#ifndef _MSSCALEROP_H_
#define _MSSCALEROP_H_

#ifdef _MSSCALEROP_C_
#define _MSSCALEROPDEC_
#else
#define _MSSCALEROPDEC_ extern
#endif

#define Cable_effect     2
#if(CODEBASE_SEL == CODEBASE_51)
    #if(ENABLE_USER_TOTAL)
    #define USER_HT_50        2200
    #define USER_VT_50        1272
    #define USER_HT_60        2100
    #define USER_VT_60        1150
    #endif
void msSetFPLLOutDClk(U8 ucVfreq, U8 ucVHzFrmT2, BOOL enableFPLL);
void msSetOutDClk(U8 ucVfreq, U8 ucVHzFrmT2, BOOL enableFPLL);
#else
    void MDrv_MFC_SetOutDClk(U16 u16InputfreqX100, BOOL enableFPLL);
#endif
void MDrv_MFC_SetGainPhase(void);
void MDrv_MFC_InitializeScalerOP(void);
void MDrv_MFC_SetLvdsSSC(U16 u16KHz, U8  u8Percent);
#define msCalculateDecimal(dwX,dwY)         ((((U32)dwX) + (dwY / 2)) / dwY)
#endif

