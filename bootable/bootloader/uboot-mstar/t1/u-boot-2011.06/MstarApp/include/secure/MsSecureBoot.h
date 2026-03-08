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

#ifndef __CMD_MS_SECURE_BOOT_H__
#define __CMD_MS_SECURE_BOOT_H__
#include <secure/MsSecureCommon.h>

typedef struct
{
    _SUB_SECURE_INFO customer;
    U8 u8RSABootPublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAUpgradePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAImagePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESBootKey[AES_KEY_LEN];
    U8 u8AESUpgradeKey[AES_KEY_LEN];
    U8 u8MagicID[16];
    U8 crc[4];
}CUSTOMER_KEY_BANK;

//-------------------------------------------------------------------------------------------------
/// Decrypt and authenticate RawData image
/// @param KLName                \b IN: RawData image name
/// @param u32kernelAddr             \b IN: RawData image adderss
/// @return  int                               \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int RawData_Authentication(char *KLName,unsigned int u32kernelAddr);

//-------------------------------------------------------------------------------------------------
/// Authenticate rootfs.ubifs, mslib.ubifs and application.ubifs
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int FileSystem_Authentication(void);

//-------------------------------------------------------------------------------------------------
/// get RSA N Public Key according different KEY_INDEX.
/// @param  enKey                 \b IN: select the enum key you need.
/// @param  buffer                \b IN: The dram address of RSA Key
/// @param  len                   \b IN: RSA Key Length
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int GetPublicKeyN(KEY_INDEX enKey, U8 *buffer, U32 len);

//-------------------------------------------------------------------------------------------------
/// get RSA E Public Key according different KEY_INDEX.
/// @param  enKey                 \b IN: select the enum key you need.
/// @param  buffer                \b IN: The dram address of RSA Key
/// @param  len                   \b IN: RSA Key Length
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int GetPublicKeyE(KEY_INDEX enKey, U8 *buffer, U32 len);

//-------------------------------------------------------------------------------------------------
/// get AES Key according different KEY_INDEX.
/// @param  enKey                 \b IN: select the enum key you need.
/// @param  keyBuf                \b IN: The dram address of AES Key
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int GetAESKey(KEY_INDEX enKey,U8 *keyBuf);
int do_ReadKL(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_BootKL(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_SecureBootCmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_verify_sha256(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_sha256(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_rsa(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_Secure_Init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase1(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase2(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase3(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase4(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase5(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_Authenticate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DumpSignature(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_performance_test (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_AN_Authenticate(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
void Secure_HaltCPU(void);
int CopyCustomerKeyBank(U32 u32DramAddr);
int DecryptKernelImage(char *dataBuf, unsigned int len);
int GetPublicKeyN(KEY_INDEX enKey, U8 *buffer, U32 len);
int GetPublicKeyE(KEY_INDEX enKey, U8 *buffer, U32 len);
int GetSignature(char *volume, U8 num, U8 *buffer, U32 len);
int GetImageSize(char *volume, U8 num, U32 *size);
int do_secure_image_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

