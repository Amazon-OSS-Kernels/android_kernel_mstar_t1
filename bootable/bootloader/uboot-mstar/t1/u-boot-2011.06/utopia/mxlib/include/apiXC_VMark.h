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
/// @file  apiXC_ModeParse.h
/// @brief MIU header file
///
////////////////////////////////////////////////////////////////////////////////
#ifndef _API_XC_VMARK_H_
#define _API_XC_VMARK_H_
#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "apiXC.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

/// vmark data
#define XC_VMARK_PARAMETER_VERSION             1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


typedef struct __attribute__((packed))
{
    //versioning
    MS_U32 u32XC_VMark_version;                   ///<Version of current structure.
    MS_U32 u32XC_VMarK_Length;                   ///<Length of this structure, u32XC_VMarK_Length=sizeof(XC_VMARK_PARAMETER)
    //embedding part
    MS_U8 u8WatermarkOn;
    MS_U8 u8FrequencyDistance[3][3];
    MS_U8 u8BackgroundEmbeddingOn;
    MS_U16 u16EmbeddingStrengthThreshold[48];
    MS_U16 u16EmbeddingStrengthThresholdBG[48];
    MS_U16 u16DirectionMax;
    MS_S8 s8StrengthMultiply;
    //rendering part
    MS_U8 u8PayloadSymbols[1920];
    MS_U8 u8SymbolsRows;
    MS_U8 u8SymbolsCols;
    MS_U8 u8SymbolsXpos;
    MS_U8 u8SymbolsYpos;
    MS_U8 u8SymbolSize;
    MS_U16 u16SpacingVert;
    MS_U16 u16SpacingHorz;
    MS_U8 u8SymbolScaleControl;
}XC_VMARK_PARAMETER;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#ifdef UFO_XC_VMARK
//-------------------------------------------------------------------------------------------------
/// MApi_XC_VMark_SetParameters
/// @param  u8ServiceIndex                       \b IN: Identifies uniquely a video stream inside the decoder. May not be supported if the support of a single decode is sufficient.
/// @param  pstXC_VMarkParameters           \b IN: VMark parameters for HW embedding
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_VMark_SetParameters(MS_U8 u8ServiceIndex, XC_VMARK_PARAMETER *pstXC_VMarkParameters);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _API_XC_VMARK_H_ */
