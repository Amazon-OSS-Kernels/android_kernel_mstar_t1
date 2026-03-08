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
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#define CRYPTO_AUTH_C

#include <common.h>
#include <uboot_mmap.h>
#include <linux/types.h>
#include <secure/crypto_rsa.h>
#include <secure/crypto_func.h>
#include <secure/crypto_auth.h>
#include <secure/crypto_sha.h>

#include <MsTypes.h>
#include <MsIRQ.h>
#include <MsSystem.h>
//#include <drvISR.h>
#include <drvMBX.h>
//#if !defined(CONFIG_KAISERIN)
//#include <drvSMBX.h>
//#endif
#include <drvAESDMA.h>
#include <secure/apiSecureBoot.h>
#include <MsDebug.h>

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Public Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  External Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int Secure_doAuthentication(U8 *u8PublicKey_N, U8 *u8PublicKey_E, U8 *u8Signature, U8 *u8AuthData, U32 u32AuthDataLen)
{
    UBOOT_TRACE("IN\n");
    int ret;
    unsigned char digest[SHA256_DIGEST_SIZE]= "\0";
    unsigned char signature[RSA_KEY_DIGI_LEN*4] = "\0";
    ret = rsa_main(u8Signature, u8PublicKey_N, u8PublicKey_E, signature);
    if(-1 == ret)
    {
        return -1;
    }
    flush_cache((U32)u8AuthData,u32AuthDataLen);
    flush_cache((U32)digest,sizeof(digest));
    ret = CommonSHA((unsigned int)u8AuthData, (unsigned int)digest,u32AuthDataLen);
    if(-1 == ret)
    {
        return -1;
    }
    if(memcmp(signature,digest,sizeof(digest))!=0)
    {
        UBOOT_ERROR("Error: Authentication Fail!!\n");
        UBOOT_ERROR("\033[0;31m Image address=0x%x\033[0m\n",(unsigned int)u8AuthData);
        UBOOT_ERROR("\033[0;31m RSA encryption's result=\033[0m\n");
        UBOOT_ERRDUMP((unsigned int)signature, SHA256_DIGEST_SIZE);
        UBOOT_ERROR("\033[0;31m Image's SHA result=\033[0m\n");
        UBOOT_ERRDUMP((unsigned int)digest, SHA256_DIGEST_SIZE);
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}


#undef CRYPTO_AUTH_C
