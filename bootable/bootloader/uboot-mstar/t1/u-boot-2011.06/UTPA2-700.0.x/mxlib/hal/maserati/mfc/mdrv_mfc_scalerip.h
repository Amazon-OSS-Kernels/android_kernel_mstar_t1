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
#ifndef _MSSCALERIP_H_
#define _MSSCALERIP_H_

#ifdef _MSSCALERIP_C_
#define _MSSCALERIPDEC_
#else
#define _MSSCALERIPDEC_ extern
#endif

// type definition
typedef enum _MirrorModeType
{
    MIRROR_OFF,
    MIRROR_H_MODE,
    MIRROR_V_MODE,
    MIRROR_HV_MODE
}MirrorModeType;


U16 Ycout_LinePitch(U8 u8IpMode, U16 wPnlWidth);

void MDrv_MFC_SetMirrorMode(MirrorModeType ucMirrorMode);
void MDrv_MFC_InitializeScalerIP(void);
void MDrv_MFC_InitializeOPM(void);
void MDrv_MFC_SetOPMBaseAddr(void);
void MDrv_MFC_SoftwareResetIP(void);
void MDrv_MFC_SoftwareResetOPM(void);
void msReset2Chip(void);
//void MDrv_MFC_SoftwareResetScaler(void);
_MSSCALERIPDEC_ void MDrv_MFC_SoftwareResetScalerInt(void);

#endif

