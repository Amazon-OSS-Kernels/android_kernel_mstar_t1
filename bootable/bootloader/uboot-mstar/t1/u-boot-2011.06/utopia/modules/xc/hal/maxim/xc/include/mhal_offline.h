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
//==============================================================================
// [mhal_offline.h]
// Date: 20090220
// Descriptions: Add a new mux layer for HW setting
//==============================================================================
#ifndef MHAL_OFFLINE_H
#define MHAL_OFFLINE_H


#ifdef MHAL_OFFLINE_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void Hal_XC_SetOffLineToSog_AV( void *pInstance, MS_U8 Channel );
INTERFACE void Hal_XC_SetOffLineToSog_YUV( void *pInstance, MS_U8 Channel );
INTERFACE void Hal_XC_SetOffLineToHv( void *pInstance, MS_U8 Channel );
INTERFACE void Hal_XC_SetOffLineToHDMI( void *pInstance, MS_U8 Channel );
INTERFACE void Hal_XC_SetOffLineToUSB( void *pInstance, MS_U8 Channel );

INTERFACE MS_U16 Hal_XC_GetOffLineOfV(void *pInstance);
INTERFACE MS_U16 Hal_XC_GetOffLineOfH(void *pInstance);
INTERFACE MS_U16 Hal_XC_GetOffLineOfDVI01(void *pInstance);
INTERFACE MS_U16 Hal_XC_GetOffLineOfDVI23(void *pInstance);
INTERFACE MS_U16 Hal_XC_GetOffLineOfStorage(void *pInstance);

INTERFACE void Hal_XC_TurnOnDetectClkPath(void *pInstance, MS_U8 u8DetectClkPath);
INTERFACE MS_BOOL Hal_XC_SetOffLineSogThreshold(void *pInstance, MS_U8 u8Threshold);
INTERFACE MS_BOOL Hal_XC_SetOffLineSogBW(void *pInstance, MS_U8 u8BW);
INTERFACE void Hal_XC_OffLineInit(void *pInstance);
INTERFACE void Hal_XC_OffLineExit(void *pInstance);
INTERFACE void Hal_XC_SetOfflineDetectClk(void *pInstance);
INTERFACE MS_U8 Hal_XC_GetOffLineDetection(void *pInstance, MS_U8 u8CheckSrc);
INTERFACE void Hal_XC_WaitForHVCleared_AV(void *pInstance, MS_U8 Channel);
INTERFACE void Hal_XC_WaitForHVCleared(void *pInstance, MS_U8 Channel);
INTERFACE void Hal_XC_SetOffLineToSog_AV2( void *pInstance, MS_U8 Channel );

#undef INTERFACE
#endif // MHAL_OFFLINE_H
