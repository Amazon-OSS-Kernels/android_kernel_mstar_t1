/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

#ifndef _HAL_AESDMA_H_
#include "halAESDMA.h"
#endif

void HAL_RSA_LoadKeyE_v2(U32 *ptr_E)
{
    U32 i;

    RIU[(RSA_BASE_ADDR+(0x22<<1))]= RSA_E_BASE_ADDR; //RSA E addr
    RIU[(RSA_BASE_ADDR+(0x20<<1))]= ((RIU[(RSA_BASE_ADDR+(0x20<<1))])|(RSA_IND32_START)); //RSA start

    //RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]=(U16)((0x0000ffff)&(U32)(ptr_E)); //write ptr_E addr to por_status(0x10050A)

    for( i = 0; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)(((*(ptr_E+i))>>8)&0xFF00)|(((*(ptr_E+i))>>24)&0xFF);
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)(((*(ptr_E+i))>>8)&0xFF)|(((*(ptr_E+i))<<8)&0xFF00);
        break;
    }

    for( i = 1; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)0;
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)0;
    }
}

void HAL_RSA_LoadKeyN_v2(U32 *ptr_N)
{
    U32 i;
    ptr_N += 63;

    RIU[(RSA_BASE_ADDR+(0x22<<1))]= RSA_N_BASE_ADDR; //RSA N addr
    RIU[(RSA_BASE_ADDR+(0x20<<1))]= ((RIU[(RSA_BASE_ADDR+(0x20<<1))])|(RSA_IND32_START)); //RSA start

    //RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]=(U16)((0x0000ffff)&(U32)(ptr_N)); //write ptr_N addr to por_status(0x10050B)

    for( i = 0; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)(((*(ptr_N-i))>>8)&0xFF00)|(((*(ptr_N-i))>>24)&0xFF);
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)(((*(ptr_N-i))>>8)&0xFF)|(((*(ptr_N-i))<<8)&0xFF00);
    }
}



void HAL_SHA_SetByPassTable(U8 bEnable)
{
    if (bEnable == TRUE)
    {
        //Enable SHA ByPass Table, 0x8[11] = 1
        RIU[(AESDMA_BASE_ADDR+(0x08<<1))] |= 0x800;
    }
    else
    {
        //Enable SHA ByPass Table, 0x8[11] = 1
        RIU[(AESDMA_BASE_ADDR+(0x08<<1))] &= ~(0x800);
    }
}

void HAL_SHA_SetAddress(U32 u32Addr)
{
    RIU[(AESDMA_BASE_ADDR+(0x0A<<1))] = (U16)((0x0000ffff)&(u32Addr));
    RIU[(AESDMA_BASE_ADDR+(0x0B<<1))] = (U16)((((0xffff0000)&(u32Addr)) - CONFIG_MIU0_BUSADDR)>>16);
}

void HAL_SHA_Start(void)
{
    //Enable SHA ByPass Table, 0x8[0] = 1
    RIU[(AESDMA_BASE_ADDR+(0x08<<1))] &= ~(0x0001);
    RIU[(AESDMA_BASE_ADDR+(0x08<<1))] |= 0x0001;
}

void HAL_AESDMA_Start_FileOutEn(U8 u8AESDMAStart)
{
    // AESDMA file start
    if(u8AESDMAStart==1)
    {
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])&(~(AESDMA_CTRL_FILE_ST|AESDMA_CTRL_FOUT_EN)));
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])|(AESDMA_CTRL_FILE_ST)|(AESDMA_CTRL_FOUT_EN));
    }
    else
    {
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])&(~(AESDMA_CTRL_FILE_ST|AESDMA_CTRL_FOUT_EN)));
    }
}

void HAL_SHA_Clear_Reset(void)
{
    //SHA_Reset
    RIU[SHARNG_BASE_ADDR+(0x08<<1)]= 0x0;
    RIU[SHARNG_BASE_ADDR+(0x08<<1)]= (RIU[SHARNG_BASE_ADDR+(0x08<<1)]|(SHARNG_CTRL_SHA_RST));
    RIU[SHARNG_BASE_ADDR+(0x08<<1)]= (RIU[SHARNG_BASE_ADDR+(0x08<<1)]&(~SHARNG_CTRL_SHA_RST));
}

void HAL_AESDMA_DisableHwKey(void)
{
    // disable normal bank to use efuse key
    RIU[(DMA_SECURE_BASE_ADDR+(0x01<<1))]= ((RIU[(DMA_SECURE_BASE_ADDR+(0x01<<1))])|(REG_USE_SECRET_KEY));
}

#ifdef CONFIG_MSTAR_NS_UBOOT
void HAL_AESDMA_Reset(void)
{
    RIU[(AESDMA_BASE_ADDR+(0x5F<<1))] = 0;
    RIU[(AESDMA_BASE_ADDR+(0x50<<1))] = 0;
}

void HAL_AESDMA_SetEngine(U32 engine)
{
    RIU[(AESDMA_BASE_ADDR+(0x51<<1))] = engine;
}

void HAL_AESDMA_SetDecrypt(U32 decrypt)
{
    if (decrypt)
        RIU[(AESDMA_BASE_ADDR+(0x51<<1))] |= 0x0200;
}

void HAL_AESDMA_SetKey(U32* key)
{
    int i;
    if (key)
    {
        // AESDMA using SW key
        RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])& (~AESDMA_CTRL_USE_SECRET_KEY));
        for (i=0; i<8; i+=2)
        {
            RIU[(AESDMA_BASE_ADDR+((0x60+i)<<1))] = key[i>>1] & 0xFFFF;
            RIU[(AESDMA_BASE_ADDR+((0x60+i+1)<<1))] = (key[i>>1]>>16) & 0xFFFF;
        }
    }
    else
    {
        // AESDMA using HW key
        RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY);
    }
}


unsigned int HAL_SYS_Query(E_SYS_QUERY query, unsigned int* result)
{
    switch(query)
    {
        case E_SYS_QUERY_MOBF_KEY_SAVED:
        {
            *result = RIU[(TZPC_PM_BASE_ADDR+(0x02<<1))] & 0x0FU;
            return TRUE;
        }
        case E_SYS_QUERY_MOBF_ENABLED:
        {
            *result = RIU[(MIU_BASE_ADDR+(0x15<<1))] & (0x1U << 15);
            return TRUE;
        }
        case E_SYS_QUERY_TRNG_KEY_SAVED:
        {
            *result = RIU[(STR_PM_BASE_ADDR+(0x22<<1))] & (0x1U << 0);
            return TRUE;
        }
        default:
        {
            return FALSE;
        }
    }
    return FALSE;
}

// use a random key which is different in each boot
void HAL_AESDMA_EnableRNGKey(void)
{
    // STR_PM, reg_hmac_auth = 1
    // mapping efuse key to RNG key
    RIU[(STR_PM_BASE_ADDR+(0x20<<1))]= ((RIU[(STR_PM_BASE_ADDR+(0x20<<1))])| 0x1U);

    // AESDMA using HW key
    RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY);
}

void HAL_AESDMA_DisableRNGKey(void)
{
    // STR_PM, reg_hmac_auth = 0
    // disable mapping efuse key to RNG key
    RIU[(STR_PM_BASE_ADDR+(0x20<<1))]= ((RIU[(STR_PM_BASE_ADDR+(0x20<<1))]) & (~0x1U));
}

void HAL_AESDMA_ClearRNGKey(void)
{
    // STR_PM, reg_trng_saved_key_clean = 1
    // clear RNG key to ensure next time use different key
    RIU[(STR_PM_BASE_ADDR+(0x22<<1))]= ((RIU[(STR_PM_BASE_ADDR+(0x22<<1))]) | (0x1U << 4));
}

void HAL_AESDMA_SetIV(U32* key)
{
    int i;
    if (key)
    {
        // SW IV
        for (i=0; i<8; i+=2)
        {
            RIU[(AESDMA_BASE_ADDR+((0x68+i)<<1))] = key[i>>1] & 0xFFFF;
            RIU[(AESDMA_BASE_ADDR+((0x68+i+1)<<1))] = (key[i>>1]>>16) & 0xFFFF;
        }
    }
    else
    {
        // Zero IV
        for (i=0; i<8; i+=2)
        {
            RIU[(AESDMA_BASE_ADDR+((0x68+i)<<1))] = 0x0000;
            RIU[(AESDMA_BASE_ADDR+((0x68+i+1)<<1))] = 0x0000;
        }
    }
}


void HAL_AESDMA_SetFileInAddr(U32 addr)
{
    RIU[(AESDMA_BASE_ADDR+(0x52<<1))] = addr & 0xFFFF;
    RIU[(AESDMA_BASE_ADDR+(0x53<<1))] = (addr>>16) & 0xFFFF;
}

void HAL_AESDMA_SetFileOutAddr(U32 addr)
{
    RIU[(AESDMA_BASE_ADDR+(0x56<<1))] = addr & 0xFFFF;
    RIU[(AESDMA_BASE_ADDR+(0x57<<1))] = (addr>>16) & 0xFFFF;
}

void HAL_AESDMA_SetFileSize(U32 len)
{
    RIU[(AESDMA_BASE_ADDR+(0x54<<1))] = len & 0xFFFF;
    RIU[(AESDMA_BASE_ADDR+(0x55<<1))] = (len>>16) & 0xFFFF;
}

U32 HAL_AESDMA_IsFinished(void)
{
//    RIU[(0x1012+(0x74<<1))] = 0x1234;
    return RIU[(AESDMA_BASE_ADDR+(0x7F<<1))] & 0x0001;
}
#endif
