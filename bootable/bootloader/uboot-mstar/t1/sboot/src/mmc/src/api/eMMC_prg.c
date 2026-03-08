/**
* Copyright (c) 2006 ??2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//#include <config.h>
//#include <common.h>
//#include <command.h>
//#include <mmc.h>
//#include <part.h>
//#include <malloc.h>
//#include <asm/errno.h>

#include "../../inc/common/eMMC.h"


#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER
#if 0
static U32 eMMC_ReadPartitionInfo_Ex(void);
static U32 eMMC_GetPartitionIndex(U16 u16_PartType,
							               U32 u32_LogicIdx,
							               volatile U16 *pu16_PartIdx);
#endif
#define eMMC_NOT_READY_MARK    ~(('e'<<24)|('M'<<16)|('M'<<8)|'C')
static U32 sgu32_IfReadyGuard = eMMC_NOT_READY_MARK;



//========================================================
U32 eMMC_LoadImages(U32 u32_Addr, U32 u32_ByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;
    #if defined(eMMC_ROM_ReLoadImages) && eMMC_ROM_ReLoadImages
	U32 (*pf)(U32,U32,U32);

	pf = (void*)eMMC_ROM_LoadImages_Addr();

    if(u32_Addr<=(BOOT_SRAM_START_ADDRESS+Total_SRAM_SIZE))
    {
		u32_err = (*pf)(u32_Addr,u32_ByteCnt,1);
		if(u32_err != eMMC_ST_SUCCESS)
		{
			goto LABEL_LOAD_IMAGE_END;
		}
    }
	else
	{
		//eMMC Identify
		u32_err = eMMC_CheckIfReady();

		if(u32_err != eMMC_ST_SUCCESS)
		{
			goto LABEL_LOAD_IMAGE_END;
		}

		u32_err = eMMC_ReadBootPart((U8*)u32_Addr, u32_ByteCnt,u32_BlkAddr);
		if(u32_err != eMMC_ST_SUCCESS)
		{
			goto LABEL_LOAD_IMAGE_END;
		}
	}

	// --------------------------------
    return eMMC_ST_SUCCESS;
	LABEL_LOAD_IMAGE_END:
	return u32_err;
    #else
	//eMMC Identify
	u32_err = eMMC_CheckIfReady();

    if(u32_err != eMMC_ST_SUCCESS)
    {
	    goto LABEL_LOAD_IMAGE_END;
    }

	u32_err = eMMC_ReadBootPart((U8*)u32_Addr, u32_ByteCnt,u32_BlkAddr);
    if(u32_err != eMMC_ST_SUCCESS)
    {
	    goto LABEL_LOAD_IMAGE_END;
    }
	// --------------------------------
    return eMMC_ST_SUCCESS;
	LABEL_LOAD_IMAGE_END:
    while(1);
	return u32_err;
	#endif
}




//========================================================

eMMC_ALIGN0 U8 g_eMMCDrv_array[32] eMMC_ALIGN1={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static U32 eMMC_Init_Ex(void)
{
	U32 u32_err;
	U8  u8_retry=0;

    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;
	// ---------------------------------
    // init platform & FCIE
    g_eMMCDrv->u16_RCA=1;
	eMMC_PlatformInit();
    LABEL_INIT_START:
	eMMC_RST_L();
	eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
	g_eMMCDrv->u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_1;
	g_eMMCDrv->u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
	eMMC_RST_H();
    eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
	u32_err = eMMC_FCIE_Init();
	if(u32_err)
        goto  LABEL_INIT_END;

	// ---------------------------------
	// init eMMC device
	u32_err = eMMC_Identify();
	if(eMMC_ST_SUCCESS != u32_err){
		if(u8_retry < 10)
		{
			u8_retry++;
			goto LABEL_INIT_START;
		}
        goto  LABEL_INIT_END;
	}


    REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT_NAND_BOOT_MODE_EN);
	eMMC_clock_setting(FCIE_SLOW_CLK);

    // setup eMMC device
	// CMD7
	u32_err = eMMC_CMD3_CMD7(g_eMMCDrv->u16_RCA, 7);
	if(eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;


#ifdef IP_FCIE_VERSION_5

	u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
	if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_INIT_END;
#endif

	// ---------------------------------
	u32_err = eMMC_SetBusWidth(8);
	if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_INIT_END;

	sgu32_IfReadyGuard = ~eMMC_NOT_READY_MARK;
	LABEL_INIT_END:
	return u32_err;
}

U32 eMMC_CheckIfReady(void)
{
	if(eMMC_NOT_READY_MARK != sgu32_IfReadyGuard)
	{
		return eMMC_ST_SUCCESS;
	}
	else
	{
    	// boot end
    	#ifndef IP_FCIE_VERSION_5
        REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT_BOOT_END_EN);
        while(1)
        {
            if(REG_FCIE(FCIE_MIE_EVENT)&BIT_CARD_BOOT_DONE)
            {
                REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT_EMMC_BOOT_MODE_EN);
    			break;
            }
    		eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
        }
		REG_FCIE_SETBIT(FCIE_MIE_EVENT, BIT_CARD_BOOT_DONE);
		#endif
	    return eMMC_Init_Ex();
	}
}

U32 eMMC_ReadBootPart(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err=0;
	U16 u16_SecCnt;

	// set Access Boot Partition 1
	#ifdef IP_FCIE_VERSION_5
    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT0);
	#else
	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT6|BIT3|BIT0);
	#endif
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}

	// read Boot Code
	u16_SecCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) + ((u32_DataByteCnt&0x1FF)?1:0);
    #if defined(eMMC_ROM_ReLoadImages) && eMMC_ROM_ReLoadImages
	u32_err = eMMC_ReadData_MIU(pu8_DataBuf, u16_SecCnt<<eMMC_SECTOR_512BYTE_BITS, u32_BlkAddr);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}
	#else
	u32_err = eMMC_ReadData_CIFD(pu8_DataBuf, u16_SecCnt<<eMMC_SECTOR_512BYTE_BITS, u32_BlkAddr);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}
    #endif

	// clear Access Boot Partition
	#ifdef IP_FCIE_VERSION_5
    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);
	#else
	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT6|BIT3);
	#endif
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}

	return u32_err;
}



// =======================================================
// u32_DataByteCnt: has to be 512B-boundary !
// =======================================================
U32 eMMC_ReadData_CIFD(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;
	U16 u16_BlkCnt;

	u16_BlkCnt = 1;
    #ifdef IP_FCIE_VERSION_5
    eMMC_pads_switch_default();
    #endif
 	while(u32_DataByteCnt)
	{
	    u32_err = eMMC_CMD17_CIFD(u32_BlkAddr, pu8_DataBuf);

		if(eMMC_ST_SUCCESS != u32_err)
		{
			break;
		}

		u32_BlkAddr += u16_BlkCnt;
		pu8_DataBuf += u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
		u32_DataByteCnt -= u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
	}

	return u32_err;
}

// =======================================================
// u32_DataByteCnt: has to be 512B-boundary !
// =======================================================
U32 eMMC_ReadData_MIU(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;
	U16 u16_BlkCnt;
    #ifdef IP_FCIE_VERSION_5
    eMMC_pads_switch();
    #endif
	// read data
	// first 512 bytes, special handle if not cache line aligned
    u16_BlkCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) + ((u32_DataByteCnt&0x1FF)?1:0);
	u32_err = eMMC_CMD18_MIU(u32_BlkAddr, pu8_DataBuf, u16_BlkCnt);

	if(eMMC_ST_SUCCESS != u32_err)
	{
        while(1);
	}

	return eMMC_ST_SUCCESS;
}

U32 __ATTR_DRAM_CODE__ eMMC_WriteData_MIU(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;
	U16 u16_BlkCnt;
    #ifdef IP_FCIE_VERSION_5
    eMMC_pads_switch();
    #endif

	// read data
	// first 512 bytes, special handle if not cache line aligned
    u16_BlkCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) + ((u32_DataByteCnt&0x1FF)?1:0);
	u32_err = eMMC_CMD25_MIU(u32_BlkAddr, pu8_DataBuf, u16_BlkCnt);

	if(eMMC_ST_SUCCESS != u32_err)
	{
        while(1);
	}

	return eMMC_ST_SUCCESS;
}


void __ATTR_DRAM_CODE__ eMMC_GetCID(U8 *pu8_CID)
{
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;
    U8 u8_i;

    for(u8_i=0;u8_i<15;u8_i++)
        pu8_CID[u8_i]= g_eMMCDrv->au8_CID[u8_i+1];
}


#if defined(CONFIG_DUAL_SYSTEM)
#define BOOT_MODE_TMP_BUF_ADDR  (CONFIG_UBOOT_LOADADDR)

U32 eMMC_CheckBootFlag(void)
{
    U32 u32_err;
    U8 bootflag = 0;

    u32_err = eMMC_CMD18_MIU(0x7880, (U32 *)BOOT_MODE_TMP_BUF_ADDR, 1);  //bootfalg partition addr = 0xF10000
    if(eMMC_ST_SUCCESS != u32_err)
    {
        while(1);
    }

    bootflag = *(U8 *)(BOOT_MODE_TMP_BUF_ADDR);
    if (0 == bootflag)  //bring up MBootBAK
    {
        return 0;
    }
    else  //bring up MBoot
    {
        return 1;
    }
}
#endif

#endif

