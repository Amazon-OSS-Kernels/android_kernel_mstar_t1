/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef _DRV_AESDMA_H_
#include "drvAESDMA.h"
//#include "halAESDMA.h"
#include "../../mmc/inc/api/drv_eMMC.h"
#endif

static const U32 u8Padding1[] = { 0x05000420, 0x03040201, 0x86480165, 0x0d060960, 0x00303130 };
static const U32 u8Padding2[] = { 0x0001ffff };


void MDrv_AESDMA_SecureAuthen(U32 u32SHAInBuf, U32 u32Size, U32 u32RSAInBuf, U32 u32SHAOutBuf, U32 u32RSAOutBuf, U32 u32SramSel)
{
    U32 i;
    U32 *ptr_SHAIn = (U32 *)(u32SHAInBuf);
    U32 *ptr_RSAext = (U32 *)(SECURE_WB_FIFO_OUT_ADDRESS);
    U32 *ptr_RSAIn = (U32 *)(u32RSAInBuf);
    U32 *ptr_RSAOut=(U32 *)(u32RSAOutBuf);
    U32 *ptr_RSAextE=0;
    U32 *ptr_RSAextN=0;
    U32 WFIFO_times = u32Size / 16;//the cycle of XIU read/write
    U32 RFIFO_times = u32Size / 16;
    U32 WFIFO_len,RFIFO_len;
    U32 WFIFO_offset=0;
    U32 RFIFO_offset=0;


#ifdef CONFIG_MSTAR_MUJI  //RSA_KEY TO DRAM
    if (u32SramSel == 0)
	{
        //ptr_RSAextE = (U32 *)(0x20208100);
        //ptr_RSAextN = (U32 *)(0x20208000);
        ptr_RSAextE = (U32 *)(0x20010100);
        ptr_RSAextN = (U32 *)(0x20010000);
    }
    else
	{
        ptr_RSAextE = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]));
        ptr_RSAextN = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]));
    }
#else
    ptr_RSAextE = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]));
    ptr_RSAextN = (U32 *)(CONFIG_EMMC_BASE_ADDRESS+((U32)RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]));
#endif

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0003);

    HAL_RSA_ClearInt();

    HAL_RSA_Reset();

    while (( HAL_RSA_GetStatus() & RSA_STATUS_RSA_BUSY ) != 0);

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0004);

    HAL_RSA_Ind32Ctrl(1);//ind32_ctrl=0xE0

    HAL_RSA_LoadSiganature(ptr_RSAIn);

    HAL_RSA_LoadKeyE(ptr_RSAextE);
#ifdef CONFIG_MSTAR_MUJI //RSA_KEY TO DRAM
    if (u32SramSel == 0)
	{
        HAL_RSA_LoadKeyNInverse(ptr_RSAextN);
    }
    else
    {
        HAL_RSA_LoadKeyN(ptr_RSAextN);
    }
#else
    HAL_RSA_LoadKeyN(ptr_RSAextN);
#endif



    HAL_RSA_SetKeyLength(0x3F);

    HAL_RSA_SetKeyType(0,1);//sw key, public key

    HAL_RSA_ExponetialStart();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0005);

    HAL_SHA_Reset();

    HAL_SHA_SetLength(u32Size);

    HAL_AESDMA_SetXIULength(u32Size);

    HAL_SHA_SelMode(1);//sha256

    RFIFO_times = 0x0;
    HAL_AESDMA_FileOutEnable(0);

    if(u32SramSel==0)
    {
        HAL_AESDMA_SetFileinAddr(u32SHAInBuf);
    }

    HAL_AESDMA_Start(1);

    if(u32SramSel==1)
    {
        //read XIU status
        while((XIU[0x8] & XIU_STATUS_W_RDY) == 0);

        //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0006);

        while( (WFIFO_times != 0) || (RFIFO_times != 0) )
        {
            WFIFO_len = (XIU[0x8]&XIU_STATUS_W_LEN)>>4;

            if((XIU[0x8] & XIU_STATUS_W_RDY) == 1)
            {
                for( i = 0; i < WFIFO_len; i++ )
                {
                    XIU[0x0] = *(ptr_SHAIn + ((i+WFIFO_offset)*0x4) + 0x0);
                    XIU[0x1] = *(ptr_SHAIn + ((i+WFIFO_offset)*0x4) + 0x1);
                    XIU[0x2] = *(ptr_SHAIn + ((i+WFIFO_offset)*0x4) + 0x2);
                    XIU[0x3] = *(ptr_SHAIn + ((i+WFIFO_offset)*0x4) + 0x3);
                }
                WFIFO_offset += WFIFO_len;
                WFIFO_times -= WFIFO_len;
            }

            RFIFO_len = (XIU[0x8]&XIU_STATUS_R_LEN)>>12;

            //pop FIFO out
            if(((XIU[0x8] & XIU_STATUS_R_RDY)>>8) == 1)
            {
                for( i = 0; i < RFIFO_len; i++)
                {
                    *(ptr_RSAext + ((i+RFIFO_offset)*0x4) + 0x0) = XIU[0x4];
                    *(ptr_RSAext + ((i+RFIFO_offset)*0x4) + 0x1) = XIU[0x5];
                    *(ptr_RSAext + ((i+RFIFO_offset)*0x4) + 0x2) = XIU[0x6];
                    *(ptr_RSAext + ((i+RFIFO_offset)*0x4) + 0x3) = XIU[0x7];
                }
                RFIFO_offset += RFIFO_len;
                RFIFO_times -= RFIFO_len;
            }
        }
    }
    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0007);

    while(( HAL_AESDMA_GetStatus() & AESDMA_CTRL_DMA_DONE ) == 0);

    while(( HAL_SHA_GetStatus() & SHARNG_CTRL_SHA_READY ) == 0);

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0008);

    HAL_SHA_Out(u32SHAOutBuf);//write sha_out to sram

    HAL_SHA_Clear();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0009);

    while (( HAL_SHA_GetStatus() & SHARNG_CTRL_SHA_BUSY ) != 0);

    while (( HAL_RSA_GetStatus() & RSA_STATUS_RSA_BUSY ) != 0);

    HAL_RSA_Ind32Ctrl(0);//ind32_ctrl=0xC0

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x000A);

//    for( i = 0; i<(SHA256_DIGEST_SIZE/4); i++)
    for( i = 0; i<(RSA_SIGNATURE_LEN/4); i++)
    {
        HAL_RSA_SetFileOutAddr(i);
        HAL_RSA_FileOutStart();
        *(ptr_RSAOut+i) = HAL_RSA_FileOut();//write rsa_out to sram
    }

    HAL_RSA_Reset();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x000B);
}

U32 MDrv_AESDMA_SecureMain(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U32 u32SramSel)
{
    U32 i, j;
    U32 u32RSAInBuf;

    U32 *ptr_SHAOut = (U32 *)(SECURE_SHA_OUT_SRAM_ADDRESS);
    U32 *ptr_RSAOut = (U32 *)(SECURE_RSA_OUT_SRAM_ADDRESS);

    u32RSAInBuf = u32SignatureAddr + RSA_SIGNATURE_LEN - 4;

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0001);

    if(u32SramSel==0)
    {
        HAL_AESDMA_DisableXIUSelectCA9();
        HAL_AESDMA_WB2DMADisable();
    }

    HAL_AESDMA_Disable();

    HAL_AESDMA_ShaFromInput();

    //SECURE_DEBUG(SECURE_DEBUG_REG, 0x0002);

    MDrv_AESDMA_SecureAuthen(u32PlaintextAddr,u32Size,u32RSAInBuf,SECURE_SHA_OUT_SRAM_ADDRESS,SECURE_RSA_OUT_SRAM_ADDRESS,u32SramSel);

    // compare sha256
    for(i = 0; i<(SHA256_DIGEST_SIZE/4); i++)
    {
        if (*(ptr_SHAOut + i) != *(ptr_RSAOut + i))
        {
           //SECURE_DEBUG(SECURE_DEBUG_REG, 0x7788);
           return FALSE;
        }
    }

    // compare pkcs1.5 padding
    for(j = 0; j<5; ++i, ++j)
    {
        if (ptr_RSAOut[i]-u8Padding1[j])
        {
           return FALSE;
        }
    }
    for(j = 0; j<50; ++i, ++j)
    {
        if (ptr_RSAOut[i]-0xffffffff)
        {
           return FALSE;
        }
    }
    if (u8Padding2[0]-ptr_RSAOut[i])
    {
       return FALSE;
    }

    return TRUE;
}
