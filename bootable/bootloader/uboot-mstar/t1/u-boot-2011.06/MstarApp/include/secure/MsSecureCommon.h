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

#ifndef __CMD_MS_SECURE_COMMON_H__
#define __CMD_MS_SECURE_COMMON_H__
#include <ShareType.h>
#include <config.h>


#define SIGNATURE_LEN               256
#define RSA_PUBLIC_KEY_N_LEN 256
#define RSA_PUBLIC_KEY_E_LEN 4
#define RSA_PUBLIC_KEY_LEN          (RSA_PUBLIC_KEY_N_LEN+RSA_PUBLIC_KEY_E_LEN)

#define AES_IV_LEN 16
#define AES_KEY_LEN 16
#define HMAC_KEY_LEN 32

#define FRAGMENT_NUM 8

typedef enum
{
    SECURE_ENG_AES_MODE_ECB=0x01,
    SECURE_ENG_AES_MODE_CBC=0x00
    /*0xFF has specail meaning, don't use this value*/
} SECURE_ENG_AES_MOD;



typedef enum{
    E_RSA_BOOT_PUBLIC_KEY=0,
    E_RSA_UPGRADE_PUBLIC_KEY,
    E_RSA_IMAGE_PUBLIC_KEY,
    E_AES_BOOT_KEY,
    E_AES_UPGRADE_KEY,
    E_AES_EFUSE_KEY,
    E_HMAC_KEY
}KEY_INDEX;


typedef enum{
    E_KEY_NO_SWAP=1,
    E_KEY_SWAP=0,
}EN_KEY_SWAP;



typedef enum
{
    EN_SUCCESS      =0,
    EN_ERROR_OF_CMD   ,
    EN_ERROR_OF_CRC
}EN_SIGNATURE_STORAGE;

typedef enum
{
    E_SECUREMERGE_NONE =0,
    E_SECUREMERGE_CONTINUE,
    E_SECUREMERGE_INTERLEAVE
}EN_SECUREMERGE_TYPE;

typedef enum
{
    E_SECUREINFO =0,
    E_SECUREIMAGE,
    E_NUTTXCONFIG
}EN_SECUREMERGE_DATA;

typedef struct
{
    U32 u32Num;
    U32 u32Size;
}IMAGE_INFO;

typedef struct
{
     U8 u8RSAKernelPublicKey[RSA_PUBLIC_KEY_LEN];
     U8 u8RSARootFsPublicKey[RSA_PUBLIC_KEY_LEN];
     U8 u8RSAMSLIBPublicKey[RSA_PUBLIC_KEY_LEN];
     U8 u8RSACONFIGPublicKey[RSA_PUBLIC_KEY_LEN];
     U8 u8RSAAPPPublicKey[RSA_PUBLIC_KEY_LEN];
     U8 u8AESKernelKey[AES_KEY_LEN];
}SECURE_KEY_SET;

typedef struct
{
  U8 u8SecIdentify[8];
  IMAGE_INFO info;
  U8 u8Signature[SIGNATURE_LEN];
}_SUB_SECURE_INFO;


typedef struct
{
  _SUB_SECURE_INFO sInfo;
  _SUB_SECURE_INFO sInfo_Interleave;
}SUB_SECURE_INFO;

typedef struct
{
  _SUB_SECURE_INFO sInfo[FRAGMENT_NUM];
  _SUB_SECURE_INFO sInfo_Interleave[FRAGMENT_NUM];
}SUB_SECURE_INFO_FOR_PARTIAL_AUTH;


typedef struct
{
    //Dont change this sequence of tee,recovery and Boot,because android have this struct.
    //If this sequence is changed,android struct need change.
    _SUB_SECURE_INFO Reserve1; //Reserve this space is for backward compatible. It's for Key in the beginning
    _SUB_SECURE_INFO tee;
    _SUB_SECURE_INFO Recovery;
    #if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && defined (CONFIG_SECURITY_BOOT)
    _SUB_SECURE_INFO ArmFw;
    #endif
    #if (ENABLE_MODULE_ANDROID_BOOT == 1)
    _SUB_SECURE_INFO Boot;
    #else
    _SUB_SECURE_INFO Kernel;
    _SUB_SECURE_INFO RootFs[FRAGMENT_NUM];
    _SUB_SECURE_INFO MsLib[FRAGMENT_NUM];
    _SUB_SECURE_INFO Lib[FRAGMENT_NUM];
    _SUB_SECURE_INFO MwLib[FRAGMENT_NUM];
    _SUB_SECURE_INFO Config[FRAGMENT_NUM];
    _SUB_SECURE_INFO App[FRAGMENT_NUM];
    #endif
    SECURE_KEY_SET Key;
    #if defined (CONFIG_SEC_SYSTEM)
    _SUB_SECURE_INFO secKernel;
    #endif
}_SECURITY_INFO_DATA;


typedef struct
{
    unsigned long crc;                  /* CRC32 over data bytes    */
    _SECURITY_INFO_DATA data;
    _SECURITY_INFO_DATA data_interleave;
}SECURITY_INFO;


typedef struct _rawdata_header_ {
    U32 offset;
    U32 size;
    U32 crc;
    char flag;
    char res1;
    char res2;
    char res3;
    char name[112];
}rawdata_header;

#endif
