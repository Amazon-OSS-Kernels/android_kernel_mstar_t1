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
/// file  drvMVD.c
/// @brief MPEG-2/4 Video Decoder Driver
///
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>
#include "MsCommon.h"
#include "MsTypes.h"
#include "drvMFC.h"
#include "madp_mfc.h"

void MDrv_URSA_Init( PMST_MFC_DRV_SYS_INFO_t mfcSysInfo, MS_U8 u8IsS7M )
{
	MAdp_MFC_Init((PMST_MFC_SYS_INFO_t)mfcSysInfo, u8IsS7M);
}

void MDrv_URSA_ControlFrameLockMode(MS_U8 u8type)
{
	MAdp_MFC_ControlFrameLockMode(u8type);
}

void MDrv_URSA_SetSpreadSpectrum(MS_U16 u16KHz, MS_U16 u16Percent, MS_BOOL bEnable, MS_BOOL bMiuLVDS)
{
	MAdp_MFC_SetSpreadSpectrum(u16KHz, u16Percent, bEnable, bMiuLVDS);
}

void MDrv_URSA_SetVFreq(MS_U16 u16Inputfreq, MS_BOOL bEnableFPLL)
{
    if(u16Inputfreq < 100) u16Inputfreq *=100;

#if 0
    if(u16Inputfreq < 5500 && u16Inputfreq >= 4500)
        u16Inputfreq = 5000;

    if(u16Inputfreq < 6500 && u16Inputfreq >= 5500)
        u16Inputfreq = 6000;
#else
    if(u16Inputfreq > 6000)
        u16Inputfreq = 6000;
    else if(u16Inputfreq < 5000)
        u16Inputfreq = 5000;

#endif

	MAdp_MFC_SetVFreq(u16Inputfreq, bEnableFPLL);
}



