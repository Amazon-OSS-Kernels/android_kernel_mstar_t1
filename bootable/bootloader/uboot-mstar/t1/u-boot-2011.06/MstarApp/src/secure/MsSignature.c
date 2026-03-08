/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    cmd_secure.c
/// @brief  SCS Main Function
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <secure/MsSecureCommon.h>
#include <secure/MsSignature.h>
#include <command.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <MsDebug.h>
#include <exports.h>
#include <MsEnvironment.h>
#include <MsSystem.h>
#include <MsRawIO.h>
#include <MsAppCB.h>
#if defined (CONFIG_SECURITY_STORE_IN_SPI_FLASH)
#include <MsApiSpi.h>
#elif defined (CONFIG_SECURITY_STORE_IN_NAND_FLASH)
//wait for implement
#elif defined (CONFIG_SECURITY_STORE_IN_EMMC_FLASH)
#include <MsMmc.h>
#else
#error "please set the correct security storage!!\n"
#endif

//-------------------------------------------------------------------------------------------------
//  Debug
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define NUMBER_OF_SECURE_INFO 2 // 2 : One is for continue mode, and the other one is for interleave mode
#if defined (CONFIG_SECURITY_STORE_IN_SPI_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_SPI
#define FLASH_DEFAULT_PARTITION SPI_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME SPI_DEFAULT_VOLUME
#define SECTOR_SIZE   0x10000
#elif defined (CONFIG_SECURITY_STORE_IN_NAND_FLASH)
extern  int ubi_get_volume_size(char *, size_t* );
extern int ubi_get_leb_size(void);
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_NAND
#define FLASH_DEFAULT_PARTITION NAND_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME NAND_DEFAULT_VOLUME
#define SECTOR_SIZE   ubi_get_leb_size()
#elif defined  (CONFIG_SECURITY_STORE_IN_EMMC_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_MMC
#define FLASH_DEFAULT_PARTITION MMC_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME MMC_DEFAULT_VOLUME
#define SECTOR_SIZE   0x200
#define SECURE_INFOR_BACK_OFFSET 0x6000
#else
#error "please set the correct security storage!!\n"
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

static SignatureLoad_cb fpSignatureLoad = NULL;
static SignatureSave_cb fpSignatureSave = NULL;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
int SignatureSave(SECURITY_INFO *pBufferAddr);
int SignatureLoad(SECURITY_INFO *pBufferAddr);


//-------------------------------------------------------------------------------------------------
//  extern function
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);

//-------------------------------------------------------------------------------------------------
//  inline
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local function
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int get_signature_offset(unsigned int *u32SigOffset,unsigned int *u32SigBkOffset)
{
    int ret = 0;
    unsigned int u32Size = 0;
    UBOOT_TRACE("IN\n");

    ret = MsApiChunkHeader_GetValue(CH_SECURITY_INFO_AP_ROM_OFFSET,u32SigOffset);

    if(ret != 0)
    {
        UBOOT_ERROR("MsApiChunkHeader_GetValue fail!\n");
        return -1;
    }

#if defined (CONFIG_SECURITY_STORE_IN_SPI_FLASH)
    ret = getSpiSize(&u32Size);
    *u32SigOffset = u32Size - ((*u32SigOffset)*SECTOR_SIZE);
#elif defined (CONFIG_SECURITY_STORE_IN_NAND_FLASH)
    check_ubi_partition(NAND_DEFAULT_VOLUME,NAND_DEFAULT_PARTITION);
    ubi_get_volume_size(NAND_DEFAULT_VOLUME,&u32Size);
    UBOOT_DEBUG("u32Size : 0x%x\n",u32Size);
    UBOOT_DEBUG("ubi_get_leb_size : 0x%x\n",SECTOR_SIZE);
    *u32SigOffset = u32Size - ((*u32SigOffset)*SECTOR_SIZE);
#elif defined  (CONFIG_SECURITY_STORE_IN_EMMC_FLASH)
    ret = get_mmc_partsize(FLASH_DEFAULT_PARTITION,&u32Size);
    *u32SigOffset = u32Size - ((*u32SigOffset)*SECTOR_SIZE);
#else
    #error "please set the correct security storage!!\n"
#endif

    #if defined  (CONFIG_SECURITY_STORE_IN_EMMC_FLASH)
    *u32SigBkOffset=*u32SigOffset+SECURE_INFOR_BACK_OFFSET;
    #else
    *u32SigBkOffset=*u32SigOffset+SECTOR_SIZE;
    #endif

    UBOOT_DEBUG("u32SigOffset : 0x%x\n",*u32SigOffset);
    UBOOT_DEBUG("u32SigBkOffset : 0x%x\n",*u32SigBkOffset);

    if(EN_SUCCESS == ret)
    {
        ret=0;
        UBOOT_TRACE("OK\n");
    }
    else
    {
        ret=-1;
        UBOOT_ERROR("get_signature_offset fail\n");
    }

    return ret;
}
int _SignatureSave(SECURITY_INFO *pBufferAddr)
{
   int ret=-1;
   int ret_bk =-1;
   unsigned int u32SigOffset = 0;
   unsigned int u32SigBkOffset = 0;
   unsigned int u32SecuritySize= 0;
   UBOOT_TRACE("IN\n");

   //Here, we check the CRC of SECUREITY_INFO, and the check range is from "pBufferAddr->data" to "pBufferAddr->data_interleave"
   u32SecuritySize = sizeof(_SECURITY_INFO_DATA) * NUMBER_OF_SECURE_INFO;

   if(pBufferAddr==NULL)
   {
      UBOOT_ERROR("The input parameter pBufferAddr' is a null pointer\n");
      return -1;
   }

   ret = raw_io_config(FLASH_DEFAULT_TARGET,FLASH_DEFAULT_PARTITION,FLASH_DEFAULT_VOLUME);
   if(ret != 0)
   {
       UBOOT_ERROR("raw_io_config setting fail!\n");
       return -1;
   }

   ret = get_signature_offset(&u32SigOffset,&u32SigBkOffset);
   if(ret != 0)
   {
       UBOOT_ERROR("get_signature_offset fail!\n");
       return -1;
   }

   UBOOT_DEBUG("u32SigOffset : 0x%x\n",u32SigOffset);
   UBOOT_DEBUG("u32SigBkOffset : 0x%x\n",u32SigBkOffset);

   // update CRC
   pBufferAddr->crc = crc32(0, (unsigned char const *)&pBufferAddr->data,u32SecuritySize);
   ret = raw_write((unsigned int)pBufferAddr,u32SigOffset,sizeof(SECURITY_INFO));
   ret_bk = raw_write((unsigned int)pBufferAddr,u32SigBkOffset,sizeof(SECURITY_INFO));

   if(EN_SUCCESS == ret || EN_SUCCESS == ret_bk)
   {
       UBOOT_DEBUG("ret : %d , ret_bk : %d \n",ret,ret_bk);
       UBOOT_TRACE("OK\n");
       ret = 0;
   }
   else
   {
       ret= -1;
       UBOOT_ERROR("SignatureSave fail\n");
   }

    return ret;
}

void RegisterCBSignatureSave(SignatureSave_cb cb)
{
    fpSignatureSave=cb;
}


int SignatureSave(SECURITY_INFO *pBufferAddr)
{
    if(fpSignatureSave!=NULL)
    {
        return fpSignatureSave(pBufferAddr);
    }
    else
    {
        return _SignatureSave(pBufferAddr);
    }
}

int _SignatureLoad(SECURITY_INFO *pBufferAddr)
{
    int ret = -1;
    int flag1=0, flag2=0;
    unsigned int u32SigOffset = 0;
    unsigned int u32SigBkOffset = 0;
    unsigned int u32SecuritySize= 0;
    UBOOT_TRACE("IN\n");

    //Here, we check the CRC of SECUREITY_INFO, and the check range include "pBufferAddr->data" and "pBufferAddr->data_interleave"
    u32SecuritySize = sizeof(_SECURITY_INFO_DATA) * NUMBER_OF_SECURE_INFO;

    if(pBufferAddr==NULL)
    {
        UBOOT_ERROR("The input parameter pBufferAddr' is a null pointer\n");
        return -1;
    }

    ret = raw_io_config(FLASH_DEFAULT_TARGET,FLASH_DEFAULT_PARTITION,FLASH_DEFAULT_VOLUME);
    if(ret != 0)
    {
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    ret = get_signature_offset(&u32SigOffset,&u32SigBkOffset);
    if(ret != 0)
    {
        UBOOT_ERROR("get_signature_offset fail!\n");
        return -1;
    }

    ret = raw_read((unsigned int)pBufferAddr,u32SigOffset,sizeof(SECURITY_INFO));

    if( (EN_SUCCESS == ret) && (pBufferAddr->crc == crc32(0, (unsigned char const *)&pBufferAddr->data,u32SecuritySize)) )
        flag1=1;

    ret = raw_read((unsigned int)pBufferAddr,u32SigBkOffset,sizeof(SECURITY_INFO));

    if( (EN_SUCCESS == ret) && (pBufferAddr->crc == crc32(0, (unsigned char const *)&pBufferAddr->data,u32SecuritySize)) )
        flag2=1;

    if( (flag2==0) && (flag1!=0) )
    {
        ret = raw_read((unsigned int)pBufferAddr,u32SigOffset,sizeof(SECURITY_INFO));
        if( (EN_SUCCESS == ret) && (pBufferAddr->crc == crc32(0, (unsigned char const *)&pBufferAddr->data,u32SecuritySize)))
        {
            ret = raw_write((unsigned int)pBufferAddr,u32SigBkOffset,sizeof(SECURITY_INFO));
        }
        else
        {
            UBOOT_ERROR("raw_read fail or caculate crc fail!\n");
            return -1;
        }
    }

    if((flag1==0)&&(flag2!=0))
    {
        ret = raw_write((unsigned int)pBufferAddr,u32SigOffset,sizeof(SECURITY_INFO));
    }

   if(EN_SUCCESS == ret)
   {
       ret=0;
       UBOOT_TRACE("OK\n");
   }
   else
   {
       ret=-1;
       UBOOT_ERROR("SignatureLoad fail\n");
   }

   return ret;
}

void RegisterCBSignatureLoad(SignatureLoad_cb cb)
{
    fpSignatureLoad=cb;
}

int SignatureLoad(SECURITY_INFO *pBufferAddr)
{
    if(fpSignatureLoad!=NULL)
    {
        return fpSignatureLoad(pBufferAddr);
    }
    else
    {
        return _SignatureLoad(pBufferAddr);
    }

}

int do_save_secure_info(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define OBJECT_NAME argv[1]
    #define OBJECT_DRAM_ADDR argv[2]
    int ret=0;
    SUB_SECURE_INFO *pSubInfo=(SUB_SECURE_INFO *)simple_strtoul(OBJECT_DRAM_ADDR, NULL, 16);
    #if !(ENABLE_MODULE_ANDROID_BOOT == 1)
    SUB_SECURE_INFO_FOR_PARTIAL_AUTH *pSubInfoForParAuth=(SUB_SECURE_INFO_FOR_PARTIAL_AUTH *)simple_strtoul(OBJECT_DRAM_ADDR, NULL, 16);
    #endif
    SECURITY_INFO *pSecureInfo = NULL;
    UBOOT_TRACE("IN\n");
    if(argc<3)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    pSecureInfo = (SECURITY_INFO *)malloc(sizeof(SECURITY_INFO));
    if(pSecureInfo==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }

    memset((void *)pSecureInfo,0,sizeof(SECURITY_INFO));

    ret = SignatureLoad(pSecureInfo);
    if(EN_ERROR_OF_CMD == ret)
    {
        free(pSecureInfo);
        UBOOT_ERROR("SignatureLoad fail\n");
        return -1;
    }
    else if(EN_ERROR_OF_CRC == ret)
    {
        UBOOT_INFO("\x1b[37;46m ===== [%s:%d] SECURITY_INFO might first upgrade !!! ===== \x1b[0m\n",__FUNCTION__,__LINE__);
    }

    if(strcmp(OBJECT_NAME,"keySet")==0)
    {
        UBOOT_DEBUG("do_save_secure_info: keySet\n");
        memcpy((void *)&pSecureInfo->data.Key,(void *)simple_strtoul(OBJECT_DRAM_ADDR, NULL, 16),sizeof(SECURE_KEY_SET));
        memcpy((void *)&pSecureInfo->data_interleave.Key,(void *)simple_strtoul(OBJECT_DRAM_ADDR, NULL, 16),sizeof(SECURE_KEY_SET));
    }
    else
    {
        #if (ENABLE_MODULE_ANDROID_BOOT == 1)
        if(0==strcmp(OBJECT_NAME,"bootSign"))
        {
            UBOOT_DEBUG("do_save_secure_info: bootSign\n");
            memcpy((void *)&pSecureInfo->data.Boot,(void *)(&pSubInfo->sInfo),sizeof(_SUB_SECURE_INFO));
            memcpy((void *)&pSecureInfo->data_interleave.Boot,(void *)(&pSubInfo->sInfo_Interleave),sizeof(_SUB_SECURE_INFO));
        }
        #else//#if (ENABLE_MODULE_ANDROID_BOOT == 1)
        if(strcmp(OBJECT_NAME,"kernelSign")==0 || strcmp(OBJECT_NAME,"KLSign")==0){
            UBOOT_DEBUG("do_save_secure_info: kernelSign\n");
            memcpy((void *)&pSecureInfo->data.Kernel,(void *)(&pSubInfo->sInfo),sizeof(_SUB_SECURE_INFO));
            memcpy((void *)&pSecureInfo->data_interleave.Kernel,(void *)(&pSubInfo->sInfo_Interleave),sizeof(_SUB_SECURE_INFO));
        }
        else if(strcmp(OBJECT_NAME,"ROOTFSSign")==0 || strcmp(OBJECT_NAME,"RFSSign")==0){
            UBOOT_DEBUG("do_save_secure_info: ROOTFSSign\n");
            memcpy((void *)pSecureInfo->data.RootFs,(void *)(&pSubInfoForParAuth->sInfo),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
            memcpy((void *)pSecureInfo->data_interleave.RootFs,(void *)(&pSubInfoForParAuth->sInfo_Interleave),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
        }
        else if(strcmp(OBJECT_NAME,"mslibSign")==0 || strcmp(OBJECT_NAME,"MSLIBSign")==0){
            UBOOT_DEBUG("do_save_secure_info mslibSign:\n");
            memcpy((void *)pSecureInfo->data.MsLib,(void *)(&pSubInfoForParAuth->sInfo),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
            memcpy((void *)pSecureInfo->data_interleave.MsLib,(void *)(&pSubInfoForParAuth->sInfo_Interleave),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
        }
        else if(strcmp(OBJECT_NAME,"mwlibSign")==0){
            UBOOT_DEBUG("do_save_secure_info mwlibSign:\n");
            memcpy((void *)pSecureInfo->data.MwLib,(void *)(&pSubInfoForParAuth->sInfo),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
            memcpy((void *)pSecureInfo->data_interleave.MwLib,(void *)(&pSubInfoForParAuth->sInfo_Interleave),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
        }
        else if(strcmp(OBJECT_NAME,"lib_dynamicSign")==0){
            UBOOT_DEBUG("do_save_secure_info: lib_dynamicSign\n");
            memcpy((void *)pSecureInfo->data.Lib,(void *)(&pSubInfoForParAuth->sInfo),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
            memcpy((void *)pSecureInfo->data_interleave.Lib,(void *)(&pSubInfoForParAuth->sInfo_Interleave),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
        }
        else if(strcmp(OBJECT_NAME,"configSign")==0 || strcmp(OBJECT_NAME,"CONFIGSign")==0){
            UBOOT_DEBUG("do_save_secure_info configSign:\n");
            memcpy((void *)pSecureInfo->data.Config,(void *)(&pSubInfoForParAuth->sInfo),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
            memcpy((void *)pSecureInfo->data_interleave.Config,(void *)(&pSubInfoForParAuth->sInfo_Interleave),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
        }
        else if(strcmp(OBJECT_NAME,"applicationsSign")==0 || strcmp(OBJECT_NAME,"APPSign")==0 || strcmp(OBJECT_NAME,"applicationSign")==0 ){
            UBOOT_DEBUG("do_save_secure_info: applicationsSign\n");
            memcpy((void *)pSecureInfo->data.App,(void *)(&pSubInfoForParAuth->sInfo),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
            memcpy((void *)pSecureInfo->data_interleave.App,(void *)(&pSubInfoForParAuth->sInfo_Interleave),sizeof(_SUB_SECURE_INFO)*FRAGMENT_NUM);
        }
        #endif
#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && defined (CONFIG_SECURITY_BOOT)
        else if(0==strcmp(OBJECT_NAME,"armfwSign"))
        {
            UBOOT_DEBUG("do_save_secure_info: armfwSign\n");
            memcpy((void *)&pSecureInfo->data.ArmFw,(void *)(&pSubInfo->sInfo),sizeof(_SUB_SECURE_INFO));
            memcpy((void *)&pSecureInfo->data_interleave.ArmFw,(void *)(&pSubInfo->sInfo_Interleave),sizeof(_SUB_SECURE_INFO));
        }
#endif
        else if(strcmp(OBJECT_NAME,"keySetSign")==0){
            UBOOT_DEBUG("do_save_secure_info: keySetSign\n");
        }
        else if(strcmp(OBJECT_NAME,"teeSign")==0){
            UBOOT_DEBUG("do_save_secure_info: teeSign\n");
            memcpy((void *)&pSecureInfo->data.tee,(void *)(&pSubInfo->sInfo),sizeof(_SUB_SECURE_INFO));
            memcpy((void *)&pSecureInfo->data_interleave.tee,(void *)(&pSubInfo->sInfo_Interleave),sizeof(_SUB_SECURE_INFO));
        }
        else if(0==strcmp(OBJECT_NAME,"recoverySign"))
        {
            UBOOT_DEBUG("do_save_secure_info: recoverySign\n");
            memcpy((void *)&pSecureInfo->data.Recovery,(void *)(&pSubInfo->sInfo),sizeof(_SUB_SECURE_INFO));
            memcpy((void *)&pSecureInfo->data_interleave.Recovery,(void *)(&pSubInfo->sInfo_Interleave),sizeof(_SUB_SECURE_INFO));
        }
        #if defined (CONFIG_SEC_SYSTEM)
        else if(strcmp(OBJECT_NAME,"seckernelSign")==0){
            UBOOT_DEBUG("do_save_secure_info: seckernelSign\n");
            memcpy((void *)&pSecureInfo->data.secKernel,(void *)(&pSubInfo->sInfo),sizeof(_SUB_SECURE_INFO));
            memcpy((void *)&pSecureInfo->data_interleave.secKernel,(void *)(&pSubInfo->sInfo_Interleave),sizeof(_SUB_SECURE_INFO));
        }
        #endif
        else
        {
            free(pSecureInfo);
            UBOOT_ERROR("Invalid symbol %s\n",OBJECT_NAME);
            return -1;
        }
    }


    ret = SignatureSave(pSecureInfo);
    if(EN_SUCCESS != ret)
    {
        free(pSecureInfo);
        UBOOT_ERROR("SignatureSave fail\n");
        return -1;
    }

     free(pSecureInfo);
     UBOOT_TRACE("OK\n");
     return 0;
}
