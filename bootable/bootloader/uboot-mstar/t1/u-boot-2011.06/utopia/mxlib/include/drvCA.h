/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvCA.h
/// @brief  CA Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CA_H__
#define __DRV_CA_H__

#include "MsTypes.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MSIF_CA_LIB_CODE              {'C','A', '_', '_'}               //Lib code
#define MSIF_CA_LIBVER                {'0','1'}                         //LIB version
#define MSIF_CA_BUILDNUM              {'1','1'}                         //Build Number
#define MSIF_CA_CHANGELIST            {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define CA_DRV_VERSION               /* Character String for DRV/API version             */  \
    MSIF_TAG,                        /* 'MSIF'                                           */  \
    MSIF_CLASS,                      /* '00'                                             */  \
    MSIF_CUS,                        /* 0x0000                                           */  \
    MSIF_MOD,                        /* 0x0000                                           */  \
    MSIF_CHIP,                                                                               \
    MSIF_CPU,                                                                                \
    MSIF_CA_LIB_CODE,                /* IP__                                             */  \
    MSIF_CA_LIBVER,                  /* 0.0 ~ Z.Z                                        */  \
    MSIF_CA_BUILDNUM,                /* 00 ~ 99                                          */  \
    MSIF_CA_CHANGELIST,              /* CL#                                              */  \
    MSIF_OS


// CA_TA_TDC_CFG version of current CA library
#define CA_TA_TDC_CFG_VERSION    1


typedef enum
{
    E_CA_DEBUG_PORT_I2C,
    E_CA_DEBUG_PORT_JTAG,
    E_CA_DEBUG_PORT_SCAN,
} CA_DEBUG_PORT;

typedef enum
{
    E_CA_DEBUG_PORT_OPEN,
    E_CA_DEBUG_PORT_PASSWD,
    E_CA_DEBUG_PORT_CLOSE
} CA_DEBUG_PORT_MODE;

typedef enum
{
    E_CA_LOCK_WRITE = 1 << 0,
    E_CA_LOCK_READ = 1 << 1
} CA_LOCK_TYPE;

typedef enum
{
    E_BOOT_LockWord_W0000_0511_Addr,
    E_BOOT_LockWord_W0512_1023_Addr,
    E_BOOT_LockWord_W1024_1535_Addr,
    E_BOOT_LockWord_W1536_2047_Addr,
    E_BOOT_LockWord_W2048_2559_Addr,
    E_BOOT_LockWord_W2560_3071_Addr,
    E_BOOT_LockWord_W3072_3583_Addr,
    E_BOOT_LockWord_W3584_3759_Addr,
    E_LockWord_v_PubOtpOID,
    E_LockWord_v_PubOtpVID,
    E_LockWord_v_pubOtpUniqueID1,
    E_LockWord_v_pubOtpUniqueID2,
    E_LockWord_CA_Reserved,
    E_LockWord_CA_MSID,

    E_BOOT_LockWord_W3072_3391_Addr,
    E_LockWord_v_pubOtpUniqueID3,
    E_LockWord_v_pubOtpUniqueID4,

    E_LockWord_CAV_SecretKey6_W=0x106,
    E_LockWord_CAV_SecretKey6_R=0x116,

    E_LockWord_RSA_1_W=0x201,

    E_LockWord_Debugport_PWD_W=0x301,
    E_LockWord_Debugport_PWD_R=0x311,

    E_LockWord_CA_reserved0=0x401,
    E_LockWord_CA_reserved1=0x402,
    E_LockWord_CA_reserved2=0x403,
    E_LockWord_CA_reserved3=0x404,
} CA_OTP_LOCK;

/*
    CA Vendor code rule
    First 8 bits: code by initial of CA Vendor
    "A": 01, "B": 02, "C": 03, "D": 04, ... , "Y": 19, "Z": 1A
    Second 4 bits: Increase if the initial of CA Vendor repeat
    Thrid 4 bits: Serial number of CA Vendor

    0x0000 reserve for NULL
    0xFFFF for unknow CA
  */
typedef enum
{
    E_CA_VENDOR_NULL,
    E_CA_VENDOR_CONAX       = 0x0300,
    E_CA_VENDOR_ECHOSTAR    = 0x0500,
    E_CA_VENDOR_IRDETO      = 0x0900,
    E_CA_VENDOR_LATENS      = 0x0C00,
    E_CA_VENDOR_NDS         = 0x0E00,
    E_CA_VENDOR_NAGRA       = 0x0E10,
    E_CA_VENDOR_SMI         = 0x1300,
    E_CA_VENDOR_VIACCESS    = 0x1600,
    E_CA_VENDOR_VERIMATRIX  = 0x1610,
    E_CA_VENDOR_UNKNOW      = 0xFFFF
} CA_VENDOR_CODE;

typedef struct
{
    MS_U32 u32FwAddr;
    MS_U32 u32FwSize;
} CA_BGC_CFG, CA_TA_CFG;

typedef struct
{
    MS_U32 u32BGCMode;        //0:dynamic, 1:static
    MS_U32 u32ProtectAddr;    //protected address, physicall address
    MS_U32 u32ProtectSize;    //protected size
    MS_U8  u8Golden[32];      //password is used for reset section
    MS_U8  u8ResetPassword[4];//only use for dynamic mode
    MS_U16 u16Interval;       //unit: millisecond, the frequency to check the proteced area
} CA_BGC_SEC_CFG;

typedef struct
{
    // Version of current structure. Must set to "CA_TA_TDC_CFG_VERSION"
    MS_U16   u16TA_TDC_CFG_Version;
    // Length of this structure. Must set to "sizeof(CA_TA_TDC_CFG)"
    MS_U16   u16TA_TDC_CFG_Length;

    // Address of TASK encrypted Transformation Data Container (TDC)
    MS_U8   *pu8EncTDC;
    // The size of encrypted TDC in Bytes
    MS_U32  u32EncTDCSize;
    // Address of TAUK encrypted TASK or further keys, the size of each key is 128-bit
    MS_U8   *pu8EncTASK;
    // The KeyLadder level to decrypt EncTASK
    MS_U32  u32Level;
} CA_TA_TDC_CFG;

typedef enum
{
    E_CA_SECRET_KEY1,
    E_CA_SECRET_KEY2,
    E_CA_SECRET_KEY3,
    E_CA_SECRET_KEY4,
    E_CA_SECRET_KEY5,
    E_CA_SECRET_KEY6,
    E_CA_SECRET_KEY7,
    E_CA_SECRET_KEY8,
} CA_SECRET_KEY;

typedef enum
{
    E_CA_RSA_KEY0,
    E_CA_RSA_KEY1,
} CA_RSA_KEY;

typedef enum
{
    E_CA_CUSTOM_SECURE_MAGIC,
} CA_CUSTOM_DATA;


DLL_PUBLIC MS_BOOL MDrv_CA_Init(void);
DLL_PUBLIC MS_U32 MDrv_CA_MaxDeviceIdSize(void);
DLL_PUBLIC MS_U32 MDrv_CA_MaxReservedSize(void);

//-------------------------------------------------------------------------------------------------
// The functions will read/write directly from/to OTP
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_EnableSecureBoot(void);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsSecureBootEnabled(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *eLockType);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetRSAextID(MS_U32 u32Value);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_GetRSAextID(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetTEERSAextID(MS_U32 u32Value);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_GetTEERSAextID(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH1_REF_VER(MS_U32 u32HASH1_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH1_REF_VER(MS_U32 *pu32HASH1_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH_TEE_REF_VER(MS_U32 u32HASH_TEE0_REF_VER, MS_U32 u32HASH_TEE1_REF_VER, MS_U32 u32HASH_TEE2_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH_TEE_REF_VER(MS_U32 *pu32HASH_TEE0_REF_VER, MS_U32 *pu32HASH_TEE1_REF_VER, MS_U32 *pu32HASH_TEE2_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_WriteReserved(const MS_U8 *pu8RData, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_ReadReserved(MS_U8 *pu8RData, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_EnableSecureCWMode(void);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsSecureCWMode(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_DumpKeyProperty(MS_U32 u32KeyNumber);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetSecretKey(CA_SECRET_KEY eKeySel, const MS_U8 *pu8SecretKey, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetSecretKey(CA_SECRET_KEY eKeySel, MS_U8 *pu8SecretKey, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetRSAKey(CA_RSA_KEY eKeySel, const MS_U8 *pu8RSAKey, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetRSAKey(CA_RSA_KEY eKeySel, MS_U8 *pu8RSAKey, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDebugPortPassword(CA_DEBUG_PORT eDebugPort, const MS_U8 *pu8Password, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDebugPortPassword(CA_DEBUG_PORT eDebugPort, MS_U8 *pu8Password, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
// The OTP read/write
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsBlank(MS_U32 addr);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_Read(MS_U32 addr);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_Write(MS_U32 addr, MS_U32 value);

DLL_PUBLIC MS_BOOL MDrv_CA_Locked(CA_OTP_LOCK eLock);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_Lock(CA_OTP_LOCK eLock);

//-------------------------------------------------------------------------------------------------
// Hardware 16-bits Random number generator
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MDrv_CA_Random(void);

//-------------------------------------------------------------------------------------------------
// The CA Vendor
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MDrv_CA_OTP_ReadCAVendor(void);


//-------------------------------------------------------------------------------------------------
// The CA MBX between R2 and ACPU
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_MBX_Init(void);
DLL_PUBLIC MS_U32  MDrv_CA_MBX_Write(MS_U8* u8Addr, MS_U32 u32Size);
DLL_PUBLIC MS_U32  MDrv_CA_MBX_Read(MS_U8* u8Addr, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
// The CA STR PM Config
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_STR_Init(void);
DLL_PUBLIC MS_BOOL MDrv_CA_STR_SetParamAddr(MS_U8 *pu8VAddr);
DLL_PUBLIC MS_BOOL MDrv_CA_STR_SetMAC(MS_U8 *pu8MAC, MS_U32 u32MACSize);

//-------------------------------------------------------------------------------------------------
// The function will config BGC
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_Init(CA_BGC_CFG stBGCCfg);
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_SetSection(MS_U32 u32SecId, const CA_BGC_SEC_CFG* const pstSecCfg);
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_ResetSection(MS_U32 u32SecId, const MS_U8* const pu8RstPSWD);

//-------------------------------------------------------------------------------------------------
// These functions are used for setting up inverse transform algorithm
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_TA_Init(CA_TA_CFG stTACfg);
DLL_PUBLIC MS_BOOL MDrv_CA_TA_Setup(CA_TA_TDC_CFG * const pstTDCCfg);

//-------------------------------------------------------------------------------------------------
// The function is used for reading/writing a 32-bit customer specific data
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_CustomData_Write(CA_CUSTOM_DATA eData, MS_U32 u32Data);
DLL_PUBLIC MS_BOOL MDrv_CA_CustomData_Read(CA_CUSTOM_DATA eData, MS_U32* pu32Data);


#ifdef __cplusplus
}
#endif

#endif
