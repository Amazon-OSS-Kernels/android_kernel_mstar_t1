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

#ifndef _UTOPIA_RELATION_H_
#define _UTOPIA_RELATION_H_



#ifdef __cplusplus
extern "C"
{
#endif



#ifdef CONFIG_UTOPIA_SIZE_CUSTOMER
#define USECTION(a) U1_##a##_SECTION
#define _URELATION_ASSIGN(func_ptr, u2_implement, u1_api) func_ptr = USECTION(u1_api) ? u2_implement : NULL;
#else
#define USECTION(a) 1
#define _URELATION_ASSIGN(func_ptr, u2_implement, u1_api) func_ptr = u2_implement;
#endif

#ifdef __cplusplus
}
#endif

#endif //_UTOPIA_RELATION_H_
