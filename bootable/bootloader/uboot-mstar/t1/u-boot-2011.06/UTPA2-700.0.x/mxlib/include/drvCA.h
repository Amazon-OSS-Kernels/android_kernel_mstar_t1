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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvCA.h
/// @brief  CA Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CA_H__
#define __DRV_CA_H__

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
} CA_OTP_LOCK;

DLL_PUBLIC MS_BOOL MDrv_CA_Init(void);
DLL_PUBLIC MS_U32 MDrv_CA_MaxDeviceIdSize(void);

//-------------------------------------------------------------------------------------------------
// The functions will read/write directly from/to OTP
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_EnableSecureBoot(void);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsSecureBootEnabled(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *eLockType);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetRSAextID(MS_U32 u32Value);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_GetRSAextID(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_EnableSecureCWMode(void);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsSecureCWMode(void);


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


#ifdef __cplusplus
}
#endif

#endif
