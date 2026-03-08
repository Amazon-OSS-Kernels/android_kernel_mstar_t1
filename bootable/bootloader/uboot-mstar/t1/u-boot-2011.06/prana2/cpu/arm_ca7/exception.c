/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//******************************************************************************
#include "datatype.h"
#include "mhal_arch.h"

void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, U32 data)
{
    expt_num = expt_num;
    isr = isr;
    data = data;
}

void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num)
{
    expt_num = expt_num;
}

