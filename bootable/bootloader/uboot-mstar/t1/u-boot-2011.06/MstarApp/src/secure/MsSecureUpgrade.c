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
#include <command.h>
#include <secure/MsSecureBoot.h>
#include <secure/MsSecureCommon.h>
#include <secure/crypto_rsa.h>
#include <secure/crypto_sha.h>
#include <secure/crypto_func.h>
#include <secure/crypto_aes.h>
#include <secure/crypto_auth.h>
#include <secure/apiSecureBoot.h>
#include <MsTypes.h>
#include <MsVfs.h>
#include <MsSystem.h>
#include <exports.h>
#include <ShareType.h>
#include <MsDebug.h>
#include <MsUtility.h>
#include <MsUpgradeUtility.h>
#include <MsEnvironment.h>


//-------------------------------------------------------------------------------------------------
//  Debug
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DEFAULT_SEGMENT_SIZE 8192
#define CERTIFICATE_LOCATION        "certificate"
#define RSA_PUBLIC_KEY_PATH "/RSAPublicKeyAPP.bin"
#define AES_KEY_PATH "/Kcust.bin"





//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static unsigned char aesKey[0x10];
static unsigned int segmentSize=0;
static unsigned char fileSizeReady=FALSE;
static unsigned int gfileSize=0;

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
U32 Secure_SegmentSize (void);
int GetfileSizeforAESUsbUpgrade(const char *Interface,const char *device,char *file);



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------




int do_file_segment_rsa_authendication(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    //"filSegRSA usb %d:%d %s",device, partition, upgradeFile);
    #define TARGET_INTERFACE argv[1]
    #define TARGET_DEVICE  argv[2]
    #define TARGET_FILE  argv[3]
    /// chunk file size in all-in-one segment rsa mode
    #define SEGMENT_RSA_CHUNK_FILE_SIZE 128
    /// CHUNK file's start signature
    #define CHUNK_ID "MSTAR..."
    /// CHUNK file's end signature
    #define CHUNK_END "...mstar"
    #define RSA_PUBLIC_KEN_N_LEN 256
    #define RSA_PUBLIC_KEN_E_LEN 4
    #undef RSA_PUBLIC_KEY_LEN
    #define RSA_PUBLIC_KEY_LEN          (RSA_PUBLIC_KEN_N_LEN+RSA_PUBLIC_KEN_E_LEN)
    #define SHA256_DIGEST_SIZE ( 256 / 8)

    unsigned int i=0;
    unsigned int fileDataOffset=0;
    unsigned int fileDataLen=0;
    unsigned int fileHashOffset=0;
    unsigned int fileHashLen=0;
    unsigned int fileSignatureOffset=0;
    unsigned int fileSignatureLen=0;
    unsigned int *pReadChunk=NULL;
    char buffer[CMD_BUF]="\0";
    int ret=0;
    unsigned int readLen=0;
    unsigned char rsaOutBuf[RSA_KEY_DIGI_LEN*4];
    unsigned char rsaPublicKeyN[RSA_PUBLIC_KEN_N_LEN];
    unsigned char rsaPublicKeyE[RSA_PUBLIC_KEN_E_LEN];
    unsigned char hashOutBuf[SHA256_DIGEST_SIZE];
    unsigned char *pt=NULL;
    unsigned char pu32ChunkFileBuf[SEGMENT_RSA_CHUNK_FILE_SIZE]="\0";
    unsigned int  *pu32SegmentBuf = NULL;

    UBOOT_TRACE("IN\n");
    if(argc<4)
        return -1;

    ret=GetfileSizeforAESUsbUpgrade(TARGET_INTERFACE,TARGET_DEVICE,TARGET_FILE);
    if(ret<0)
    {
        UBOOT_ERROR("GetfileSizeforAESUsbUpgrade fail\n");
        return -1;
    }

    //Read last segment , because we want to get chunk file.
    snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE, TARGET_DEVICE, (U32)pu32ChunkFileBuf, TARGET_FILE, gfileSize-SEGMENT_RSA_CHUNK_FILE_SIZE, SEGMENT_RSA_CHUNK_FILE_SIZE);
    if(run_command(buffer, 0)!=0)
    {
        UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
        return -1;
    }

    if(memcmp((const void *)((U32)pu32ChunkFileBuf),CHUNK_ID,sizeof(CHUNK_ID))!=0){
        UBOOT_ERROR("[ERROR] CHUNK_ID compare fail\n");
        return -1;
    }

    if(memcmp((const void *)((U32)pu32ChunkFileBuf+SEGMENT_RSA_CHUNK_FILE_SIZE-strlen(CHUNK_END)),CHUNK_END,strlen(CHUNK_END))!=0){
        UBOOT_ERROR("[ERROR] CHUNK_END compare fail\n");
        return -1;
    }

    //Get hash, signature infor from chunk data
    pReadChunk=(unsigned int *)((U32)pu32ChunkFileBuf);
    pReadChunk+=2;
    segmentSize=*pReadChunk;
    pReadChunk+=1;
    fileDataOffset=*pReadChunk;
    pReadChunk++;
    fileDataLen=*pReadChunk;
    pReadChunk++;

    fileHashOffset=*pReadChunk;
    pReadChunk++;
    fileHashLen=*pReadChunk;
    pReadChunk++;
    fileSignatureOffset=*pReadChunk;
    pReadChunk++;
    fileSignatureLen=*pReadChunk;
    pReadChunk++;

    UBOOT_DEBUG("segment size=0x%x\n",segmentSize);
    UBOOT_DEBUG("fileDataOffset=0x%x\n",fileDataOffset);
    UBOOT_DEBUG("fileDataLen=0x%x\n",fileDataLen);
    UBOOT_DEBUG("fileHashOffset=0x%x\n",fileHashOffset);
    UBOOT_DEBUG("fileHashLen=0x%x\n",fileHashLen);
    UBOOT_DEBUG("fileSignatureOffset=0x%x\n",fileSignatureOffset);
    UBOOT_DEBUG("fileSignatureLen=0x%x\n",fileSignatureLen);


    //Get pu32RsaAuthBuff
    pu32SegmentBuf=(unsigned int*)malloc(segmentSize+16);
    if(pu32SegmentBuf==NULL)
    {
        UBOOT_ERROR("[ERROR] pu32SegmentBuf memroy allocate fail\n");
        return -1;
    }

    //Get rsa upgrade public key
    ret = GetPublicKeyN(E_RSA_UPGRADE_PUBLIC_KEY,rsaPublicKeyN,RSA_PUBLIC_KEN_N_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("Get public key N fail\n");
        free(pu32SegmentBuf);
        return -1;
    }
    ret = GetPublicKeyE(E_RSA_UPGRADE_PUBLIC_KEY,rsaPublicKeyE,RSA_PUBLIC_KEN_E_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("Get public key E fail\n");
        free(pu32SegmentBuf);
        return -1;
    }

    //Read ***.hash.signature, and then do rsa decod
    memset(buffer,0,CMD_BUF);
    snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE,TARGET_DEVICE,(U32)pu32SegmentBuf, TARGET_FILE, fileSignatureOffset, fileSignatureLen);
    if(run_command(buffer, 0)!=0)
    {
        UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
        free(pu32SegmentBuf);
        return -1;
    }

    rsa_main((unsigned char *)pu32SegmentBuf,rsaPublicKeyN,rsaPublicKeyE,rsaOutBuf);

    memset(buffer,0,CMD_BUF);
    snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE,TARGET_DEVICE,(U32)pu32SegmentBuf, TARGET_FILE, fileHashOffset, fileHashLen);
    if(run_command(buffer, 0)!=0)
    {
        UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
        free(pu32SegmentBuf);
        return -1;
    }


    if(CommonSHA((U32)pu32SegmentBuf,(MS_U32)hashOutBuf,(MS_U32)fileHashLen)!=0)
    {
        UBOOT_ERROR("CommonSHA Fail!!\n");
        free(pu32SegmentBuf);
        return -1;
    }

    for(i = 0; i<SHA256_DIGEST_SIZE; i++)
    {
        if(hashOutBuf[i] != rsaOutBuf[i])
        {
           UBOOT_ERROR("hashOutBuf[%d]=0x%x, rsaOutBuf[%d]=0x%x \n",i,hashOutBuf[i],i,rsaOutBuf[i]);
           free(pu32SegmentBuf);
           return -1;
        }
    }

    // Do rsa authentication for ***.hash
    UBOOT_INFO("Segment RSA Authentication ....\n");
    while(fileDataLen)
    {
        UBOOT_INFO("*");
        readLen=(fileDataLen>=segmentSize)?segmentSize:fileDataLen;

        memset(buffer,0,CMD_BUF);
        snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE,TARGET_DEVICE,(U32)pu32SegmentBuf, TARGET_FILE, fileDataOffset, readLen);
        if(run_command(buffer, 0)!=0)
        {
            UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
            free(pu32SegmentBuf);
            return -1;
        }
        if(CommonSHA((U32)pu32SegmentBuf,(MS_U32)hashOutBuf,(MS_U32)readLen)!=0)
        {
            UBOOT_ERROR("CommonSHA Fail!!\n");
            free(pu32SegmentBuf);
            return -1;
        }

        memset(buffer,0,CMD_BUF);
        snprintf(buffer, CMD_BUF, "filepartloadSegAES %s %s %x %s %x %x", TARGET_INTERFACE,TARGET_DEVICE,(U32)pu32SegmentBuf, TARGET_FILE, fileHashOffset, SHA256_DIGEST_SIZE*2);
        if(run_command(buffer, 0)!=0)
        {
            UBOOT_ERROR("Cmd:'%s' fail!!\n",buffer);
            free(pu32SegmentBuf);
            return -1;
        }

        pt=(unsigned char *)(pu32SegmentBuf);//+SHA256_DIGEST_SIZE-1);
        for(i = 0; i<SHA256_DIGEST_SIZE; i++)
        {
             if(hashOutBuf[SHA256_DIGEST_SIZE-1-i] != *pt)
             {
                UBOOT_ERROR("hashOutBuf[%d]=0x%x, *pt=0x%x \n",SHA256_DIGEST_SIZE-1-i,hashOutBuf[SHA256_DIGEST_SIZE-1-i],*pt);
                free(pu32SegmentBuf);
                return -1;
             }
             pt++;
        }
        fileDataLen-=readLen;
        fileDataOffset+=readLen;
        fileHashOffset+=SHA256_DIGEST_SIZE;

        UBOOT_DEBUG("readLen=0x%x\n",readLen);
        UBOOT_DEBUG("fileDataLen=0x%x\n",fileDataLen);
        UBOOT_DEBUG("fileDataOffset=0x%x\n",fileDataOffset);
        UBOOT_DEBUG("fileHashOffset=0x%x\n",fileHashOffset);
    }

    free(pu32SegmentBuf);
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_file_part_load_with_segment_aes_decrypted(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define DOWNLOAD_INTERFACE argv[1]
    #define DOWNLOAD_DEVICE argv[2]
    #define DOWNLOAD_ADDR argv[3]
    #define DOWNLOAD_FILE argv[4]
    #define DOWNLOAD_OFFSET argv[5]
    #define DOWNLOAD_LENGTH argv[6]

    unsigned int downloadOffset=0;
    unsigned int downloadLen=0;
    unsigned int pdownloadAddr=0;

    unsigned int readSegmentAddr=0;
    unsigned int readOffsetInSeg=0;
    unsigned int readLen=0;
    unsigned int aesDecodeLen=0;
    unsigned int segmentNum=0;
    unsigned int _segmentSize=0;
    unsigned int u32HwAesBufAddr=0;
    char buffer[CMD_BUF]="\0";
    SECUREBOOT_MbxRetStruct SecureBootMbxRetStruct;
    UBOOT_TRACE("IN\n");

    if(argc<7)
    {
        return -1;
    }

    downloadOffset = (unsigned int)simple_strtoul(DOWNLOAD_OFFSET, NULL, 16);
    downloadLen = (unsigned int)simple_strtoul(DOWNLOAD_LENGTH, NULL, 16);
    pdownloadAddr = (unsigned int)simple_strtoul(DOWNLOAD_ADDR, NULL, 16);

    //Get total file size
    if(GetfileSizeforAESUsbUpgrade(DOWNLOAD_INTERFACE,DOWNLOAD_DEVICE,DOWNLOAD_FILE)<0)
    {
        UBOOT_ERROR("GetfileSizeforAESUsbUpgrade fail\n");
        return -1;
    }

    // get _segmentSize and segmentNum
    if(segmentSize==0)
    {
        _segmentSize=DEFAULT_SEGMENT_SIZE;
    }
    else
    {
        _segmentSize=segmentSize;
    }
    segmentNum=downloadOffset/_segmentSize;

    //Get AES decode key
    if(IsHouseKeepingBootingMode()==FALSE)
    {
        Secure_MailBox_ASSIGN_DECRYPTED_KEY(&SecureBootMbxRetStruct,E_AES_UPGRADE_KEY,AES_KEY_LEN,E_KEY_SWAP);
    }
    else
    {
        GetAESKey(E_AES_UPGRADE_KEY,aesKey);
    }

    //get AES buffer information
    get_value_from_env(E_MMAP_ID_HW_AES_BUF_ADR, AES_DECRYPTED_ADDR, &u32HwAesBufAddr);
    u32HwAesBufAddr=PA2VA(u32HwAesBufAddr);

    while(downloadLen)
    {
        readSegmentAddr=segmentNum*_segmentSize;
        if(downloadOffset%_segmentSize)//Only first time
        {
            readOffsetInSeg=downloadOffset-readSegmentAddr;
            readLen=((readOffsetInSeg+downloadLen)>=_segmentSize)?(_segmentSize-readOffsetInSeg):downloadLen;

        }
        else
        {
            readOffsetInSeg=0;
            readLen=(downloadLen>=_segmentSize)?_segmentSize:downloadLen;

        }
        aesDecodeLen=((readSegmentAddr+_segmentSize)>=gfileSize)?gfileSize-readSegmentAddr:_segmentSize;

        UBOOT_DEBUG("downloadOffset=0x%x\n",downloadOffset);
        UBOOT_DEBUG("downloadLen=0x%x\n",downloadLen);
        UBOOT_DEBUG("pdownloadAddr=0x%x\n",pdownloadAddr);
        UBOOT_DEBUG("readSegmentAddr=0x%x\n",readSegmentAddr);
        UBOOT_DEBUG("readOffsetInSeg=0x%x\n",readOffsetInSeg);
        UBOOT_DEBUG("readLen=0x%x\n",readLen);
        UBOOT_DEBUG("aesDecodeLen=0x%x\n",aesDecodeLen);
        UBOOT_DEBUG("segmentNum=0x%x\n",segmentNum);

        if(getUpgradeMode()==EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED)
        {
            if(vfs_read((void*)u32HwAesBufAddr,DOWNLOAD_FILE,readSegmentAddr,aesDecodeLen)!=0)
            {
              return -1;
            }
        }
        else if(getUpgradeMode()==EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED)
        {
            memset(buffer, 0, CMD_BUF);
            snprintf(buffer, CMD_BUF, "fatpartload %s %s %x %s %x %x", DOWNLOAD_INTERFACE,DOWNLOAD_DEVICE,u32HwAesBufAddr, DOWNLOAD_FILE, readSegmentAddr, aesDecodeLen);
            if(run_command(buffer, 0)!=0)
            {
              return -1;
            }
        }
        else
        {
            UBOOT_ERROR("Wrong Upgrade Mode!!\n");
            return -1;
        }

        // AES decode
        if(IsHouseKeepingBootingMode()==FALSE)
        {
            flush_cache(u32HwAesBufAddr,aesDecodeLen);
            Chip_Flush_Memory();
            Secure_MailBox_AESDecode(&SecureBootMbxRetStruct, MsOS_VA2PA(u32HwAesBufAddr), aesDecodeLen, SECURE_ENG_AES_MODE_ECB);
        }
        else
        {
            Secure_AES_ECB_Decrypt(u32HwAesBufAddr, aesDecodeLen, (U8 *)aesKey);
        }

        // copy to target address
        memcpy((void*)pdownloadAddr,(char *)(u32HwAesBufAddr+readOffsetInSeg),readLen);

        downloadLen-=readLen;
        pdownloadAddr+=readLen;
        downloadOffset+=readLen;
        segmentNum++;
    }

    // set the download length for next command
    setenv("filesize",DOWNLOAD_LENGTH);

    UBOOT_TRACE("OK\n");
    return 0;
}

U32 Secure_SegmentSize (void)
{
    UBOOT_TRACE("IN\n");
    UBOOT_TRACE("OK\n");
    return DEFAULT_SEGMENT_SIZE;
}

int GetfileSizeforAESUsbUpgrade(const char *Interface,const char *device,char *file)
{
    char* buffer=NULL;
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(fileSizeReady==TRUE)
    {
        UBOOT_TRACE("OK\n");
        return ret;
    }

    buffer=(char *)malloc(CMD_BUF);
    if(buffer == NULL)
    {
       UBOOT_ERROR("Error: out of memory\n");
       return -1;
    }
    //Get total file size
    memset(buffer,0,CMD_BUF);
    if(getUpgradeMode()==EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED)
    {
        gfileSize=vfs_getsize(file);

    }
    else if(getUpgradeMode()==EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED)
    {
        snprintf(buffer, CMD_BUF, "fatfilesize %s %s %s 0x%08x",Interface,device, file, &gfileSize);
        UBOOT_DEBUG("cmd:%s\n",buffer);
        ret=run_command(buffer, 0);
        if(ret<0)
        {
            free(buffer);
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("Wrong Upgrade Mode!!\n");
        free(buffer);
        return -1;
    }


    free(buffer);
    fileSizeReady=TRUE;
    UBOOT_TRACE("OK\n");
    return ret;
}
