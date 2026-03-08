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
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   osalTSP.h
// @brief  Transport Stream Processer (TSP) OSAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __OSAL_TSP_H__
#define __OSAL_TSP_H__

#if defined(MSOS_TYPE_ECOS)
#include "osalTSP_ecos.h"
#elif defined(MSOS_TYPE_LINUX)
#include "osalTSP_linux.h"
#elif defined(MSOS_TYPE_LINUX_KERNEL)
#include "osalTSP_linux_kernel.h"
#else
#include "osalTSP_nos.h"
#endif

#endif // #ifndef __OSAL_TSP_H__
