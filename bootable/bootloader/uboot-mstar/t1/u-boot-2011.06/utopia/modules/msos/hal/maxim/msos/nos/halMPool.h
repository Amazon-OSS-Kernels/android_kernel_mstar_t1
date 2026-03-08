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
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_MPOOL_H_
#define _HAL_MPOOL_H_



//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_PHY HAL_MsOS_MPool_VA2PA(MS_VIRT u64Virt);
MS_VIRT HAL_MsOS_MPool_PA2KSEG0(MS_PHY u64Phys);
MS_VIRT HAL_MsOS_MPool_PA2KSEG1(MS_PHY u64Phys);
MS_PHY HAL_MsOS_MPool_PA2BA(MS_PHY u64PhyAddr);
#endif // _HAL_MPOOL_H_
