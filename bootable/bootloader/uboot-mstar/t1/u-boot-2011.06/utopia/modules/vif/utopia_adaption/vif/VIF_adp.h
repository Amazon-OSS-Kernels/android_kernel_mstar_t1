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

#ifndef _VIF_ADP_H_
#define _VIF_ADP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"


MS_U32 VIF_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs);


#ifdef __cplusplus
}
#endif
#endif // _VIF_ADP_H_