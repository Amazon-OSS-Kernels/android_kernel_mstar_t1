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

#include "MsCommon.h"
#include "halMPool.h"
#include "halCHIP.h"

/******* for nos Mpool Mapping***********/
//for temporary using, if miu mapping is moved from mhal_init_Mmu function, va_start = 0x20000000, va_end = 0xFFFFFFFF
static unsigned int va_start = 0x60000000;
static unsigned int va_end   = 0xA0000000;
typedef struct
{
     unsigned int va;
     unsigned int pa;
     unsigned int size;
     int         access;
     int         attributes;
}
_cpu_memory_t;

typedef struct
{
	MS_VIRT pVirtStart;
	MS_VIRT pVirtEnd;
	MS_PHY pPhyaddr;
        MS_PHY u64MpoolSize;
        MS_BOOL bIsUsed;
        MS_SIZE s32V2Poff;
        MS_BOOL bNonCache;
        MS_U8 u8MiuSel;
 } MPOOL_INFO;

static MsOSMPool_DbgLevel	_u64MPoolDBGLevel;
extern void _cpu_mmu_map_memory (_cpu_memory_t *entry);
extern void mhal_tlb_flush(void);

#define MAX_MAPPINGSIZE 100
#define MPOOL_DBG_MSG(debug_level, x)     do { if (_u64MPoolDBGLevel >= (debug_level)) (x); } while(0)

#define MMAP_NONCACHE true
#define MMAP_CACHE false
static MPOOL_INFO mpool_info[MAX_MAPPINGSIZE];

/*for nos mpool mapping*/
 #define MMU_AP_NONE         0
 #define MMU_AP_PRIVILEGED   1
 #define MMU_AP_READ         2
 #define MMU_AP_FULL         3

 #define MMU_BUFFERED        1
 #define MMU_CACHED          2
/**********************/

/****************************************/
//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_Init(void)
{
    return TRUE;
}

MS_BOOL MsOS_MPool_Close(void)
{
    return TRUE;
}

// @FIXME: support one pool at this stage
MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
    return TRUE;
}
static void _MPool_Check_aligned(MS_PHY u64Offset, MS_PHY u64MapSize)
{
     if(u64Offset&0xFFFFFUL)
     {
         printf("!!! Important issue !!!\n");
         printf("The memory mapping address is not aligned, please make sure of that\n");
         MS_ASSERT(0);
     }

     if(u64MapSize&0xFFFFFUL)
     {
         printf("!!! Important issue !!!\n");
         printf("The memory mapping size is not aligned, please make sure of that\n");
                 MS_ASSERT(0);
     }

     return;
}
MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE u64Offset, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
	_cpu_memory_t entry;
	MS_PHY Phyaddr,PhyaddrEnd;
	MS_PHY Busaddr;
	//TRUE means mapping successfully,FALSE doesn't
	MS_BOOL bRet = FALSE;

	unsigned int size;
    unsigned int va;
    unsigned int pa;
    int access;
    int attributes;

	int i,idx = 0;
	//1M alignment for first level memory mapping
	_MPool_Check_aligned(u64Offset, u64MapSize);
	 MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64Offset = %tX, u64MapSize = %tX, bNonCache = %02x\n", (ptrdiff_t)u64Offset, (ptrdiff_t)u64MapSize, bNonCache));

    if(mpool_info[MAX_MAPPINGSIZE-1].bIsUsed)
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
    	goto Mapping_End;
    }

	//handle the situation that if there are more than 2 miu on chip
	 _miu_offset_to_phy(u8MiuSel, u64Offset, Phyaddr);

	PhyaddrEnd = Phyaddr + u64MapSize - 1;
	/*Phyaddr has been already done the miu operation according to offset
          handle the situation that is more than 2 miu*/
	Busaddr = HAL_MsOS_MPool_PA2BA(Phyaddr);

	/*record the pa va mapping relationship*/
	for (i=0;i<MAX_MAPPINGSIZE; i++)
        {
	        if(mpool_info[i].bIsUsed==false)
	        {
	        	mpool_info[i].bIsUsed = true;

    	        	if(bNonCache)
 		                mpool_info[i].bNonCache = MMAP_NONCACHE;
	                else
		                mpool_info[i].bNonCache = MMAP_CACHE;

	         	idx = i;
        	        break;
                }
		else
        	{
	                if (bNonCache != mpool_info[i].bNonCache)
        	        	continue;

	        	if ( (mpool_info[i].pPhyaddr <= Phyaddr && Phyaddr < mpool_info[i].pPhyaddr + mpool_info[idx].u64MpoolSize) ||
        	           (mpool_info[i].pPhyaddr <= PhyaddrEnd && PhyaddrEnd < mpool_info[i].pPhyaddr + mpool_info[idx].u64MpoolSize) )
            		{
        	                printf("[Utopia2_nos] Duplicated PA mapping.\n");
        	                MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("[Utopia2_nos] Duplicated PA(0x%tX ~ 0x%tX) mapping with Table[%d](0x%tX ~0x%tX).\n",
                                (ptrdiff_t)Phyaddr, (ptrdiff_t)PhyaddrEnd, i, (ptrdiff_t)mpool_info[i].pPhyaddr, (ptrdiff_t)(mpool_info[i].pPhyaddr + mpool_info[idx].u64MpoolSize -1)));
                    	       //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("[Utopia2_nos] Duplicated PA mapping.\n"));
                               //Duplicated mapping or Overlap mapping.
				bRet = TRUE;
               			goto Mapping_End;
                        }
        	}
	}
	mpool_info[idx].pVirtStart = va_start;
        mpool_info[idx].pVirtEnd = (va_start + u64MapSize);
        mpool_info[idx].u8MiuSel = u8MiuSel;
	mpool_info[idx].pPhyaddr = Phyaddr;
	mpool_info[idx].u64MpoolSize = u64MapSize;
	mpool_info[idx].s32V2Poff = mpool_info[idx].pVirtStart - Busaddr;
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("The phy and the bus address is: %tX and :%tX\n,offset is:%tX!!!!!!", (ptrdiff_t)Phyaddr, (ptrdiff_t)Busaddr, (ptrdiff_t)mpool_info[idx].s32V2Poff));
	/***************************************/


	entry.pa = Busaddr;
	entry.size = mpool_info[idx].u64MpoolSize;
	entry.va = mpool_info[idx].pVirtStart;
	entry.access =  MMU_AP_FULL;
	/*non-cache buffered,*/
	if(bNonCache)
		entry.attributes = MMU_BUFFERED;
	/*cached buffered*/
	else
		entry.attributes = MMU_CACHED | MMU_BUFFERED;

	/*this functtion is implemented in prana*/
	_cpu_mmu_map_memory (&entry);

	MsOS_Dcache_Flush(va_start,u64MapSize);
	mhal_tlb_flush();
	va_start += u64MapSize;
	bRet = TRUE;

	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pVirtStart = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pVirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pVirtEnd =  %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pVirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pPhyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pPhyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));
Mapping_End:
	return bRet;
}

inline MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt)
{
#ifdef MCU_MIPS_4KE
    return (MS_U32)(((MS_U32)pAddrVirt)&0x1fffffff);
#else
    return (MS_PHY)pAddrVirt;
#endif
}

inline MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys)
{
#ifdef MCU_MIPS_4KE
    return (MS_U32)(((MS_U32)pAddrPhys)|0xa0000000);
#else
    return (MS_VIRT)pAddrPhys;
#endif
}
/*PA->BA->VA, through the VA<->BA offset to get the return VA*/
MS_VIRT _MPool_PA2VA(MS_PHY pAddrPhys, MS_BOOL bNonCache)
{
    MS_VIRT pAddrVirt=0;
    int i, idx;

    for(i=0;i<MAX_MAPPINGSIZE;i++)
    {
        // if not used, convert searching is over...
        if(!mpool_info[i].bIsUsed)
        {
            break;
        }

        // check if cache_policy is correct
        if(mpool_info[i].bNonCache != bNonCache)
        {
            continue;
        }

        // we do PA_REGION check here, to check if pAddrPhys in located in mpool_info[i]
        if(!((mpool_info[i].pPhyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].pPhyaddr + mpool_info[i].u64MpoolSize))))
        {
            continue;
        }

        pAddrVirt = HAL_MsOS_MPool_PA2BA(pAddrPhys) + mpool_info[i].s32V2Poff;

        if(pAddrVirt != 0)
            return pAddrVirt;
        else
            break;
    }

    // Debug
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("=== MPool Table Print! ===\n"));
    for(idx=0;idx<MAX_MAPPINGSIZE;idx++)
    {
        if(!mpool_info[idx].bIsUsed)
            continue;

		MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u8MiuSel = %d\n", (ptrdiff_t)idx, mpool_info[idx].u8MiuSel));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].bNonCache = %d\n", (ptrdiff_t)idx, mpool_info[idx].bNonCache));
		MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pVirtStart = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pVirtStart));
    	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pVirtEnd =  %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pVirtEnd));
    	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pPhyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pPhyaddr));
    	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));

        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("###\n"));
    }
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("==========================\n"));

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("[Utopia2_nos] ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM PA(%tX) TO VA.\n", (ptrdiff_t)pAddrPhys));


PA2VA_End:
  return 0x0;
}
