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

#if defined(_MIPS_PLATFORM_)&&defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
//#include <linux/kernel.h>
//#include <linux/string.h>
//#include "chip_int.h"
//#include <linux/interrupt.h>
#else
#include <string.h>
#endif


#include "mhal_mfe.h"
#include "mfe_reg.h"
#ifdef _AEON_PLATFORM_
#ifdef _MFE_T8_
#include "mdrv_irq.h"
#include "drvISR.h"
#endif
#elif defined(_WIN32)
#include <windows.h>
#include "pthread.h"
#endif //_AEON_PLATFORM_

#ifdef _MIPS_PLATFORM_
#ifdef _MFE_BIG2_
#include "utility.h"
#include "shellcfg.h"

#if !defined(_MFE_T8_)
#ifdef _ENABLE_ISR_
#include "cyg/hal/hal_intr.h"
#endif //_ENABLE_ISR_
#endif

#endif //MFE_BIG2
#endif //_MIPS_PLATFORM_

#ifdef _FPGA_
#include "fpga_def.h"
#endif



MFE_REG mfe_reg;

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
unsigned int u32MFERegOSBase;
#endif

// RIU base
#if defined(_AEON_PLATFORM_) && defined(_MFE_T8_)
    MFE_U32 RIU_BASE = 0xA0000000;
#elif defined(_AEON_PLATFORM_)
    MFE_U32 RIU_BASE = 0xA0000000;
//extern U8 FSwrite_ready;
#elif defined(_MFE_BIG2_) && defined(_MIPS_PLATFORM_)
    MFE_U32 RIU_BASE = 0xBF834000;
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
    MFE_U32 RIU_BASE = 0xBF200000; //CH4
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
//    #define RIU_BASE U32MFERegOSBase //CH4
#elif defined(_HIF_) && defined(_MFE_BIG2_)
    MFE_U32 RIU_BASE = 0xA0000000;
#elif defined(_FPGA_)
    MFE_U32 RIU_BASE = 0xA0000000;
#else //if defined(_WIN32)//defined(_BCB_PLATFORM_)
    unsigned short REG_BANK_MFE[0x100];
#endif

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)

void MHAL_MFE_InitRegBase(MFE_U32 U32RegBase)
{
    ms_dprintk(HAL_L1,"HAL_MFE_InitRegBase 0x%X\n", (unsigned int)U32RegBase);
    u32MFERegOSBase = U32RegBase;
}
#endif


void MHal_MFE_PowerOff(MFE_U32 is_off,MFE_U32 clock_level)
{
#ifndef _FPGA_
#if defined(_MFE_BIG2_)&&defined(_MIPS_PLATFORM_) && !defined(Enable_CLKMGR)
    if (is_off) {
        *(short *)(0xBF8000C0) |= (0<<2); // speed down to 15M Hz
    } else
        *(short *)(0xBF8000C0) |= (6<<2); //ken: speed up to 156M Hz
#elif defined(_MFE_T8_) &&defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
    if (is_off) {
        *(unsigned short *)(0xbf206600+(0x18)*4) = 1;//disable MFE clock
    } else {
        //*(unsigned short *)(0xbf206600+(0x18)*4) = 0; // 4'b0000 123
        //*(unsigned short *)(0xbf206600+(0x18)*4) = 4; // 4'b0100 144
        //*(unsigned short *)(0xbf206600+(0x18)*4) = 8; // 4'b1000 172
        //*(unsigned short *)(0xbf206600+(0x18)*4) = 12; // 4'b1100 192

        if((clock_level >>2) == 0 )
            *(unsigned short *)(0xbf206600+(0x18)*4) = clock_level*4;
        else
            *(unsigned short *)(0xbf206600+(0x18)*4) = 8;
    }
    //MFE clock;
    //*(unsigned short *)(0xbf200000+(0x1980+0x18)*4) = 2<<2; //2<<2
#elif defined(_MFE_T8_) &&defined(_MIPS_PLATFORM_)
    if (is_off) {
        *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 1;//disable MFE clock
    } else {
        //*(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 0; // 4'b0000 123
        //*(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 4; // 4'b0100 144
        //*(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 8; // 4'b1000 172
        //*(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 12; // 4'b1100 192

        if((clock_level >>2) == 0 )
            *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = clock_level*4;
        else
            *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 8;
    }
#endif

#endif // _FPGA_
}

void MHal_MFE_SWReset(void)
{
    MFE_U16 temp;
    mfe_reg.reg_mfe_g_soft_rstz = 1;
    WriteRegMFE(0x0, mfe_reg.reg00, "", 0, "");
    ReadRegMFE(0x0, &temp);
    ms_dprintk(HAL_L1, "[HAL] SWReset reg00: %04x\n", (unsigned int)temp);
}

MFE_U32 MHal_MFE_GetBitstreamEncodedLen(void)
{
    MFE_REG reg;
    int nHwBytes;

    // Read bit count
    reg.reg42 = 0;
    reg.reg43 = 0;
    ReadRegMFE(0x42, &reg.reg42);
    ReadRegMFE(0x43, &reg.reg43);
    // Convert into byte count
    nHwBytes = (((int)reg.reg_mfe_s_bsp_bit_cnt_high<<16) + reg.reg_mfe_s_bsp_bit_cnt_low ) >> 3;
    return nHwBytes;
}

/*
void MHal_MFE_SetIrqMask(MFE_REG* pMfeReg, U16 mask)
{
    pMfeReg->reg_mfe_g_irq_mask = mask&0xff;
    WriteRegMFE(0x1c, mfe_reg.reg1c, "", 0, "");
}
*/

void MHal_MFE_ClearIRQ(MFE_U16 irq_bits)
{
    irq_bits = irq_bits&0x7f;
    WriteRegMFE(0x1d, irq_bits, "", 0, "");
}

void MHal_MFE_GetIRQ(MFE_U16 *irq_bits)
{
    ReadRegMFE(0x1e, irq_bits);
    ms_dprintk(HAL_L1, "[HAL] GetIRQ reg1e: 0x%x\n", (int)(*irq_bits));
}

void MHal_MFE_set_outbitsbuf(OutBitSBUF *bitsbuf)
{
    MFE_WORD sadr_low, sadr_high, eadr_low, eadr_high;

    sadr_low = LOWORD(bitsbuf->start_addr>>MIU_SHIFT);
    sadr_high = HIWORD(bitsbuf->start_addr>>MIU_SHIFT);
    eadr_low = LOWORD(((bitsbuf->end_addr)>>MIU_SHIFT)-1);
    eadr_high = HIWORD(((bitsbuf->end_addr)>>MIU_SHIFT)-1);
    SetObufAddr(sadr_low, sadr_high, eadr_low, eadr_high);
}

void MHal_MFE_SetCLKCTL(void)
{
   WriteRegMFE(0x0a, (MFE_U16)mfe_reg.reg0a, "", 0, "");
}

void MHal_MFE_ResetReg()
{
    memset(&mfe_reg, 0, sizeof(MFE_REG));
}

void MHal_MFE_GetCRC(MFE_U8 checksum_HW[8])
{
    int i;
    MFE_U16 u16Reg1;

    ReadRegMFE(0x73, &u16Reg1);
    u16Reg1 = u16Reg1 & 0xFDFF;
    WriteRegMFE(0x73, u16Reg1, "", 0, "");

    //call HW CRC64
    for(i=0;i<4;i++){
        ReadRegMFE(0x76+i, &u16Reg1);
        checksum_HW[2*i] = (MFE_U8)(u16Reg1&0xFF);
        checksum_HW[2*i+1] = u16Reg1>>8;
    }

    ReadRegMFE(0x73, &u16Reg1);
    u16Reg1 = u16Reg1 | 0x0100;
    WriteRegMFE(0x73, u16Reg1, "", 0, "");
}

void MHal_MFE_Enable_MIU_Protection(int MIU_TEST_MODE,MFE_CONFIG* pConfig)
{

    MFE_REG mfe_reg;
    int width, height;
    BufInfo* pBufInfo;


    pBufInfo = &pConfig->ctxBufInfo;
    width = pConfig->nBufWidth;
    height = pConfig->nBufHeight;

#ifdef _MFE_A3_
    mfe_reg.reg_mfe_s_marb_miu_bound_en_0 = 0x1;
    mfe_reg.reg_mfe_s_marb_miu_bound_en_1 = 0x1;
    mfe_reg.reg_mfe_s_marb_miu_bound_en_2 = (pBufInfo->m_bEnableMvStore==1) ? 1 : 0;
    mfe_reg.reg_mfe_s_marb_miu_bound_en_3 = 0x1;

    ms_dprintk(HAL_L1, "MIU PROTECTION reg_mfe_s_marb_miu_bound_en[0 to 3] = %x %x %x %x\n", \
        mfe_reg.reg_mfe_s_marb_miu_bound_en_0,mfe_reg.reg_mfe_s_marb_miu_bound_en_1, \
        mfe_reg.reg_mfe_s_marb_miu_bound_en_2,mfe_reg.reg_mfe_s_marb_miu_bound_en_3);
#else
    if (pBufInfo->m_bEnableMvStore==1)
        mfe_reg.reg_mfe_s_marb_miu_bound_en = 0xF; // (1111) 4 mode
    else
        mfe_reg.reg_mfe_s_marb_miu_bound_en = 0xB; // (1011) 3 mode without MV

    ms_dprintk(HAL_L1, "MIU PROTECTION reg_mfe_s_marb_miu_bound_en = %x\n",mfe_reg.reg_mfe_s_marb_miu_bound_en);
#endif




//    MIU_TEST_MODE = 0;

//BSP
//    mfe_reg.reg_mfe_s_marb_miu_bound_en = 1;

    mfe_reg.reg_mfe_s_marb_ubound_0_low =  (MFE_U16)(((pBufInfo->m_nOutBufAddr[pBufInfo->m_nOutBuffer-1].miuAddress+pBufInfo->m_OutBufferSize)>>3)&0xFFFF);
    mfe_reg.reg_mfe_s_marb_ubound_0_high = (MFE_U16)((pBufInfo->m_nOutBufAddr[pBufInfo->m_nOutBuffer-1].miuAddress+pBufInfo->m_OutBufferSize)>>(3+16));
    mfe_reg.reg_mfe_s_marb_lbound_0_low =  (MFE_U16)(((pBufInfo->m_nOutBufAddr[0].miuAddress+MIU_TEST_MODE*8)>>3)&0xFFFF);
    mfe_reg.reg_mfe_s_marb_lbound_0_high = (MFE_U16)((pBufInfo->m_nOutBufAddr[0].miuAddress+MIU_TEST_MODE*8)>>(3+16));


//BSP end

//    MIU_TEST_MODE = 1;
//MC
//    mfe_reg.reg_mfe_s_marb_miu_bound_en = 2;

    //upper bound: Chrome rec addr + ((W/2)*(H/2) ) * 2
    //lower bound: Luma rec addr
    mfe_reg.reg_mfe_s_marb_ubound_1_low  = (MFE_U16)(((pBufInfo->m_nRecCAddr.miuAddress+height*width/2 -MIU_TEST_MODE*8)>>3)&0xFFFF);
    mfe_reg.reg_mfe_s_marb_ubound_1_high = (MFE_U16)((pBufInfo->m_nRecCAddr.miuAddress+height*width/2 -MIU_TEST_MODE*8)>>(3+16));
    mfe_reg.reg_mfe_s_marb_lbound_1_low  = (MFE_U16)(((pBufInfo->m_nRecYAddr.miuAddress)>>3)&0xFFFF);
    mfe_reg.reg_mfe_s_marb_lbound_1_high = (MFE_U16)((pBufInfo->m_nRecYAddr.miuAddress)>>(3+16));


//MC end
//    MIU_TEST_MODE = 0;
//MV
    //upper bound: start addr + (number of MB)*8
    //lower bound:start addr
    mfe_reg.reg_mfe_s_marb_ubound_2_low  = (MFE_U16)(((pBufInfo->m_nMvStoreAddr.miuAddress+ (height/MIU_MB_SIZE)*(width/MIU_MB_SIZE)*8 -MIU_TEST_MODE*8)>>3)&0xFFFF);
    mfe_reg.reg_mfe_s_marb_ubound_2_high = (MFE_U16)((pBufInfo->m_nMvStoreAddr.miuAddress+ (height/MIU_MB_SIZE)*(width/MIU_MB_SIZE)*8 -MIU_TEST_MODE*8)>>(3+16));
    mfe_reg.reg_mfe_s_marb_lbound_2_low  = (MFE_U16)(((pBufInfo->m_nMvStoreAddr.miuAddress)>>3)&0xFFFF);
    mfe_reg.reg_mfe_s_marb_lbound_2_high = (MFE_U16)((pBufInfo->m_nMvStoreAddr.miuAddress)>>(3+16));


//MV end

//GN
//        mfe_reg.reg_mfe_s_marb_miu_bound_en = 8;

    //upper bound: start addr + ( #.MB per width)*64
    //lower bound:start addr
    mfe_reg.reg_mfe_s_marb_ubound_3_low  = (MFE_U16)(((pBufInfo->m_nGNAddr.miuAddress + (width/MIU_MB_SIZE)*64 -MIU_TEST_MODE*64)>>3)&0xFFFF);
    mfe_reg.reg_mfe_s_marb_ubound_3_high = (MFE_U16)((pBufInfo->m_nGNAddr.miuAddress + (width/MIU_MB_SIZE)*64 -MIU_TEST_MODE*64)>>(3+16));
    mfe_reg.reg_mfe_s_marb_lbound_3_low  = (MFE_U16)(((pBufInfo->m_nGNAddr.miuAddress)>>3)&0xFFFF);
    mfe_reg.reg_mfe_s_marb_lbound_3_high = (MFE_U16)((pBufInfo->m_nGNAddr.miuAddress)>>(3+16));

//GN end



    WriteRegMFE(0x58, mfe_reg.reg58, "", 1, "bsp ubound");
    WriteRegMFE(0x59, mfe_reg.reg59, "[%d]", 1, "en & bsp ubound");
    WriteRegMFE(0x5a, mfe_reg.reg5a, "", 1, "bsp lbound");
    WriteRegMFE(0x5b, mfe_reg.reg5b, "", 1, "bsp lbound");

    WriteRegMFE(0x5c, mfe_reg.reg5c, "", 1, "rec ubound");
    WriteRegMFE(0x5d, mfe_reg.reg5d, "[%d]", 1, "rec ubound");
    WriteRegMFE(0x5e, mfe_reg.reg5e, "", 1, "rec lbound");
    WriteRegMFE(0x5f, mfe_reg.reg5f, "", 1, "rec lbound");

    WriteRegMFE(0x60, mfe_reg.reg60, "", 1,        "mv obuf ubound");
    WriteRegMFE(0x61, mfe_reg.reg61, "[%d]", 1, "mv obuf ubound");
    WriteRegMFE(0x62, mfe_reg.reg62, "", 1,        "mv obuf lbound");
    WriteRegMFE(0x63, mfe_reg.reg63, "", 1,        "mv obuf lbound");

    WriteRegMFE(0x64, mfe_reg.reg64, "", 1,        "mv obuf ubound");
    WriteRegMFE(0x65, mfe_reg.reg65, "[%d]", 1, "mv obuf ubound");
    WriteRegMFE(0x66, mfe_reg.reg66, "", 1,        "mv obuf lbound");
    WriteRegMFE(0x67, mfe_reg.reg67, "", 1,        "mv obuf lbound");


}

void MHal_MFE_Enable_MIU_Protection_Check(int MIU_TEST_MODE,int TYPE)
{

    ms_dprintk(HAL_L1, "TEST_MIU_PROTECTION\n");

    //MV
    if(TYPE == 3){
        MIU_PRO_REG miu_reg59;
        MIU_PRO_REG2 miu_reg19;


        ReadRegMFE(0x19, &miu_reg19.reg19);
        if(miu_reg19.reg_mfe_g_mp4_direct_mvstore==1){
            ReadRegMFE(0x59, &miu_reg59.reg59);
            if(MIU_TEST_MODE !=  miu_reg59.reg_mfe_s_marb_miu_bound_err ){
                ms_dprintk(HAL_L2, "\n[error] TEST_MIU_PROTECTION reg_mfe_s_marb_miu_bound_err = %d\n",miu_reg59.reg_mfe_s_marb_miu_bound_err);
            }
            else
                ms_dprintk(HAL_L2,         "\nTEST_MIU_PROTECTION reg_mfe_s_marb_miu_bound_err = %d\n",miu_reg59.reg_mfe_s_marb_miu_bound_err);


        }
        else
            ms_dprintk(HAL_L2, "\n no test MV miu_PROTECTION in this frame.\n");
    }

    else
    {
        MIU_PRO_REG miu_reg59;
        ReadRegMFE(0x59, &miu_reg59.reg59);
        //read HW error flag
            if(MIU_TEST_MODE !=  miu_reg59.reg_mfe_s_marb_miu_bound_err ){
                ms_dprintk(HAL_L2, "\n[error] TEST_MIU_PROTECTION reg_mfe_s_marb_miu_bound_err = %d\n",miu_reg59.reg_mfe_s_marb_miu_bound_err);
            }
            else
                ms_dprintk(HAL_L2,         "\nTEST_MIU_PROTECTION reg_mfe_s_marb_miu_bound_err = %d\n",miu_reg59.reg_mfe_s_marb_miu_bound_err);

    }


}


