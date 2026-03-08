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
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"


#include "udma_share.h"
#include "memmap.h"

#include "MFE_chip.h"



#if (defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&(!defined(_KERNEL_MODE_)))
	#include "MsIRQ.h"
	#include "MsOS.h"
#endif

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
	#include "mdrv_system.h" //for addr_phy2log
#endif
#define MMAP_FAILED      0
#define MMAP_OK          0
#define MMAP_ERROR      -1

void MMAPInit(MFE_U32 buf_base, MFE_U32 buf_size, MFE_U32* ppBufStart)
{
	*ppBufStart = buf_base;
    ms_dprintk(DRV_L1, "MMAPInit: start:0x%x size:0x%x\n", (unsigned int)buf_base,(unsigned int) buf_size);
}

long MMAPMalloc(MFE_U32* ppBufStart, MFE_U32 pBufEnd, MFE_U32 size, MEMMAP_t * memmap, int nAlignBytes,const char* msg)
{
    //assert(memmap != NULL);
    ms_dprintk(DRV_L1, "MMAPMalloc[%s] 0x%x %d\n",msg, (unsigned int)*ppBufStart,(unsigned int) size);
//    size = (size + (nAlignBytes - 1)) & (~(nAlignBytes - 1));

    memmap->size = size;
    memmap->miuPointer = MMAP_FAILED;
    memmap->miuAddress = 0;

    //*ppBufStart == 0 is posspile in MIU1
/*
    if(*ppBufStart == 0) {
        ms_dprintk(DRV_L1, "MMAPMalloc: global_buf_phy == 0 \n");
        return MMAP_ERROR;
    }
*/
	*ppBufStart = (*ppBufStart + (nAlignBytes - 1)) & (~(nAlignBytes - 1));
    memmap->miuAddress = *ppBufStart;
    *ppBufStart += size;
    if (*ppBufStart > pBufEnd) {
        ms_dprintk(DRV_L1, "Warning!! MFE MMAPMalloc over size!!, 0x%x 0x%x 0x%x\n"
            ,(unsigned int) *ppBufStart, (unsigned int)pBufEnd, (unsigned int)size);
    }
    memmap->miuPointer = (MFE_U8*)addr_phy2log(memmap->miuAddress);

    //ms_dprintk(DRV_L1,"MMAPMalloc:\n");
    ms_dprintk(DRV_L1,"memmap:miuPointer: 0x%08x,", (unsigned int)memmap->miuPointer);
    ms_dprintk(DRV_L1,"size  : 0x%08x,", (unsigned int)memmap->size);
    ms_dprintk(DRV_L1,"miuAddress: 0x%08x\n",(unsigned int) memmap->miuAddress);

    return MMAP_OK;
}

int reg_scan(unsigned short *reg_mask, MFE_U32 num_reg, proto_write write_func, proto_read read_func)
{
    MFE_U32 i=0, j=0;
    ms_dprintk(DRV_L1, "get in reg_scan %d\n", (unsigned int)num_reg);
    for (i=0; i<num_reg; i++) {
        unsigned short val, temp;
        for (j=1; j<reg_mask[i]; j<<=1) {
            if (!(i==0 && j==1)) {
                val = ((unsigned short)j)&reg_mask[i];
                write_func(i, val);
                read_func(i, &temp);
                if (val != temp) {
                    ms_dprintk(DRV_L1, "register scan error: reg:%d write:%d read:%d\n", (unsigned int)i, (unsigned int)val&reg_mask[i], (unsigned int)temp);
                }
            } else {
                ms_dprintk(DRV_L1, "passed reg %d\n", (unsigned int)i);
            }
        }
    }
    ms_dprintk(DRV_L1, "passed reg_scan\n");
    return 0;
}
