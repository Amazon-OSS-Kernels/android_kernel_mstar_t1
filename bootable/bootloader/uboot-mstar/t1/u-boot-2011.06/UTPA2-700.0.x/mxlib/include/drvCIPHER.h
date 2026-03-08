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
/// @file   drvCIPHER.h
/// @brief  CIPHER Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DRVCIPHER_H__
#define __DRVCIPHER_H__

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define HASH_TIMEOUT_VALUE      (5000UL)

typedef enum
{
    CIPHER_DBG_NONE,
    CIPHER_DBG_ERR,
    CIPHER_DBG_WARN,
    CIPHER_DBG_INFO,
}CIPHER_DBGMSG_LEVEL;

#define MSIF_CIPHER_LIB_CODE                     {'D','M','A','_'}    //Lib code
#define MSIF_CIPHER_LIBVER                       {'0','1'}            //LIB version
#define MSIF_CIPHER_BUILDNUM                     {'0','1'}            //Build Number
#define MSIF_CIPHER_CHANGELIST                   {'0','0','8','2','9','9','2','7'} //P4 ChangeList Number

#define CIPHER_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_CIPHER_LIB_CODE,                  /* IP__                                             */  \
    MSIF_CIPHER_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_CIPHER_BUILDNUM,                          /* 00 ~ 99                                          */  \
    MSIF_CIPHER_CHANGELIST,  /* CL#                                              */  \
    MSIF_OS


//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------
/// AESDMA DDI return value
typedef MS_U32                         DRV_CIPHER_RET;

typedef void (*P_DrvCIPHER_EvtCallback)(MS_U32 u32CmdId, MS_BOOL bSuccess, MS_U32 u32Exception);

typedef enum
{
    E_CIPHER_MAIN_NONE,
    E_CIPHER_MAIN_AES,
    E_CIPHER_MAIN_DES,
    E_CIPHER_MAIN_TDES,
    E_CIPHER_MAIN_M6_S56_CCBC,
    E_CIPHER_MAIN_M6_S56,
    E_CIPHER_MAIN_M6_KE56,
    E_CIPHER_MAIN_RC4,
}CIPHER_MAIN_ALGO;

typedef enum
{
    E_CIPHER_SUB_NONE,
    E_CIPHER_SUB_ECB,
    E_CIPHER_SUB_CBC,
    E_CIPHER_SUB_CTR,
    E_CIPHER_SUB_CBC_MAC,
    E_CIPHER_SUB_CTR_64,
    E_CIPHER_SUB_CMAC_Key,
    E_CIPHER_SUB_CMAC_Algo,
    E_CIPHER_SUB_PCBC_ADD,
    E_CIPHER_SUB_PCBC_XOR,
}CIPHER_SUB_ALGO;

typedef enum
{
    E_CIPHER_RES_NONE,
    E_CIPHER_RES_CLR,
    E_CIPHER_RES_CTS,
    E_CIPHER_RES_SCTE52,
}CIPHER_RES_ALGO;

typedef enum
{
    E_CIPHER_SB_NONE,
    E_CIPHER_SB_CLR,
    E_CIPHER_SB_IV1,
    E_CIPHER_SB_IV2,
}CIPHER_SB_ALGO;

typedef enum
{
    E_CIPHER_KSRC_CPU,
    E_CIPHER_KSRC_KL,
    E_CIPHER_KSRC_OTP ,
}CIPHER_KEY_SRC;


typedef struct
{
    CIPHER_MAIN_ALGO    eMainAlgo;
    CIPHER_SUB_ALGO     eSubAlgo;
    CIPHER_RES_ALGO     eResAlgo;
    CIPHER_SB_ALGO      eSBAlgo;
}DRV_CIPHER_ALGO;

typedef struct
{
    CIPHER_KEY_SRC  eKeySrc;
    MS_U8           u8KeyIdx;
    MS_U8           u8KeyLen;
    MS_VIRT         ptrKeyData;
    MS_U8           u8IVLen;
    MS_VIRT         ptrIVData;
}DRV_CIPHER_KEY;


typedef enum
{
	E_CIPHER_DRAM = 1,
	E_CIPHER_DQMEM,
	E_CIPHER_IQMEM,
	E_CIPHER_DIRECT,
}CIPHER_MEM_TYPE;


typedef struct
{
    MS_PHY phyAddr;
    MS_U32   u32Size;
	CIPHER_MEM_TYPE    eMemoryType;
}DRV_CIPHER_DATA;

typedef struct
{
    DRV_CIPHER_ALGO stAlgo;
    DRV_CIPHER_KEY  stKey;
    DRV_CIPHER_DATA stInput;
    DRV_CIPHER_DATA stOutput;
    MS_BOOL         bDecrypt;
	MS_U32          u32CAVid;
    P_DrvCIPHER_EvtCallback pfCallback;
}DRV_CIPHER_DMACFG;

typedef enum
{
    E_CIPHER_HASH_ALGO_SHA1,
    E_CIPHER_HASH_ALGO_SHA256 ,
    E_CIPHER_HASH_ALGO_MD5 ,
}CIPHER_HASH_ALGO;

typedef enum
{
    E_CIPHER_HASH_IV_FIPS,
    E_CIPHER_HASH_IV_CMD,
    E_CIPHER_HASH_IV_PRV,
}CIPHER_HASH_IV;

typedef enum
{
    E_CIPHER_HASH_STAGE_FIRST ,
    E_CIPHER_HASH_STAGE_UPDATE ,
    E_CIPHER_HASH_STAGE_LAST ,
}CIPHER_HASH_STAGE;

typedef enum
{
    E_CIPHER_HMAC_IKPAD,
    E_CIPHER_HMAC_OKPAD,
}CIPHER_HMAC_KPAD;

typedef struct
{
    CIPHER_KEY_SRC  eKeySrc;
    MS_U8           u8KeyIdx;
    MS_U8           u8KeyLen;
    MS_VIRT         ptrKeyData;
}DRV_CIPHER_HMAC_KEY;

typedef struct
{
    CIPHER_HASH_ALGO        eAlgo;
    DRV_CIPHER_DATA         stInput;
    MS_VIRT                 ptrDigest_Buf;
    MS_U32                  u32Digest_Buf_Size;
    MS_U32                  u32CAVid;
    P_DrvCIPHER_EvtCallback pfCallback;
}DRV_CIPHER_HASHCFG;

typedef struct
{
    CIPHER_HASH_ALGO      eAlgo;
    DRV_CIPHER_DATA       stInput;
    MS_VIRT               ptrDigest_Buf;
    MS_U32                u32Digest_Buf_Size;
    MS_U32                u32CAVid;
    MS_U32                u32StartByte;
    MS_VIRT               ptrSetIV;
    CIPHER_HASH_IV        eIVMode;
    CIPHER_HASH_STAGE     eStage;
    CIPHER_HMAC_KPAD      eHMACKPad;
    DRV_CIPHER_HMAC_KEY   stHMACKey;
}DRV_CIPHER_HASHCFG_MANUAL;

typedef struct
{
    CIPHER_HASH_ALGO    eAlgo;
    DRV_CIPHER_DATA     stInput;
    MS_VIRT             ptrDigest_Buf;
    MS_U32              u32Digest_Buf_Size;
    MS_U32              u32CAVid;
    DRV_CIPHER_HMAC_KEY stHMACKey;
    P_DrvCIPHER_EvtCallback pfCallback;
}DRV_CIPHER_HMACCFG;

#define DRV_CIPHER_OK                   (0x00000000UL)
#define DRV_CIPHER_FAIL                 (0x00000001UL)
#define DRV_CIPHER_BAD_PARAM            (0x00000002UL)

//Exception Message
#define DRV_CIPHER_EXCEPT_CAVID         (0x00000001UL)
#define DRV_CIPHER_EXCEPT_DATA          (0x00000002UL)
#define DRV_CIPHER_EXCEPT_ALGO          (0x00000004UL)
#define DRV_CIPHER_EXCEPT_DMA_KEY       (0x00000008UL)
#define DRV_CIPHER_EXCEPT_HMAC_KEY      (0x00000010UL)


//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Init(void);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Reset(void);

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMACalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASH(DRV_CIPHER_HASHCFG stCfg, MS_U32 *pu32CmdId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASHManual(DRV_CIPHER_HASHCFG_MANUAL stCfg, MS_U32 *pu32CmdId);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HMAC(DRV_CIPHER_HMACCFG stCfg, MS_U32 *pu32CmdId);

DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsDMADone(MS_U32 u32CmdId, MS_U32 *pu32Exception);
DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsHASHDone(MS_U32 u32CmdId, MS_U32 *pu32Exception);

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_SetDbgLevel(CIPHER_DBGMSG_LEVEL eDBGMsgLevel);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_GetLibVer(const MSIF_Version **ppVersion);

#ifdef ENABLE_BGC
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_BGC_Enable(MS_U32 u32EngId,MS_U32 u32Algo, MS_VIRT ptrGolden, MS_PHY phySAddr, MS_U32 u32Size);
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_BGC_Check(MS_U32 u32EngId, MS_BOOL *bOK);
#endif

//////// Old Interface /////////////////
//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------
#include "drvAESDMA.h"

#endif
