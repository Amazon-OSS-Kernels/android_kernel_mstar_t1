/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   msAPI_DAC.h
/// @brief  Scaler API layer Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//****************************************************
// msAPI_DAC SW Version: 1.00
// msAPI_DAC update date: 2010/02/21 13:49
//****************************************************

#ifndef _MSAPI_DAC_H_
#define _MSAPI_DAC_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_MSAPI_DAC_OUTPUT_CVBS = BIT(0),       // CVBS
    E_MSAPI_DAC_OUTPUT_SVIDEO = BIT(1),     // YC
    E_MSAPI_DAC_OUTPUT_YPBPR  = BIT(2),      // component YPbPr
    E_MSAPI_DAC_OUTPUT_RGB  = BIT(3),        // RGB
    E_MSAPI_DAC_OUTPUT_SCART = BIT(4),   // CVBS + SVIDEO + YC(if possible)
}E_MSAPI_DAC_OUTPUT_TYPE;

// A bitmap indecate the current off/on status of DAC output.
typedef MS_U32 MSAPI_DAC_OUTPUT_BITMAP;

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------
/// @brief: msAPI DAC_SetOutputSource                     \n
/// return MSAPI_DAC_OUTPUT_BITMAP:                         \n
//----------------------------------------------------------------------------
MS_BOOL msAPI_DAC_SetOutputSource(MSAPI_DAC_OUTPUT_BITMAP);

#ifdef __cplusplus
}
#endif

#endif  // _MSAPI_DAC_H_
