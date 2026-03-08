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

#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "ms_dprintf.h"
#include "udma_share.h"


MS_BOOL MMAPInit(MS_PHY buf_base, MS_U32 buf_size, MS_PHY* ppBufStart)
{
    *ppBufStart = buf_base;
    ms_dprintk(HAL_L1, "MMAPInit: start:0x%x, size:0x%x\n", (unsigned int)buf_base, (unsigned int)buf_size);
#ifdef WIN32
    return TRUE;
#else
    if(buf_base == (MS_VIRT)NULL) {
        ms_dprintk(HAL_L0, "MMAPInit: start:0x%x is NULL\n", (unsigned int)buf_base);
        return FALSE;
    }
    else
        return TRUE;
#endif
}

MS_BOOL MMAPMalloc(MS_PHY* ppBufStart, MS_PHY pBufEnd, MS_U32 size, MEMMAP_t * memmap, MS_PHY nAlignBytes, const MS_S8* msg)
{
    //MS_ASSERT(memmap != NULL);
    ms_dprintk(HAL_L1, "MMAPMalloc[%s] 0x%x, size = %u\n", msg, (unsigned int)(*ppBufStart), (unsigned int)size);
    //    size = (size + (nAlignBytes - 1)) & (~(nAlignBytes - 1));

    memmap->size = size;
    memmap->miuPointer = 0;
    memmap->miuAddress = 0;

    *ppBufStart = (*ppBufStart + (nAlignBytes - 1)) & (~(nAlignBytes - 1));
    memmap->miuAddress = *ppBufStart;
    *ppBufStart += size;
    if (*ppBufStart > pBufEnd) {
        ms_dprintk(HAL_L0, "MFE MMAPMalloc over size!!, 0x%x, 0x%x, 0x%x\n",
            (unsigned int)(*ppBufStart), (unsigned int)pBufEnd, (unsigned int)size);
        MS_ASSERT(0);
        return FALSE;
    }
    memmap->miuPointer = (MS_U8*)addr_phy2log(memmap->miuAddress);

    //ms_dprintk(DRV_L1,"MMAPMalloc:\n");
    ms_dprintk(HAL_L1,"memmap:miuPointer: %p\n", memmap->miuPointer);
    ms_dprintk(HAL_L1,"             size: 0x%x\n", (unsigned int)memmap->size);
    ms_dprintk(HAL_L1,"       miuAddress: 0x%x\n", (unsigned int)memmap->miuAddress);

    return TRUE;
}

