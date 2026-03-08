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

#ifndef _XC_VMARK_H_
#define _XC_VMARK_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct DLL_PACKED
{
    MS_U8 u8ServiceIndex;
    XC_VMARK_PARAMETER *pstXC_VMarkParameters;
    E_APIXC_ReturnValue eReturnValue;
} stXC_VMARK_SET_PARAMETERS, *pstXC_VMARK_SET_PARAMETERS;

#ifdef __cplusplus
}
#endif


#endif //_XC_VMARK_H_

