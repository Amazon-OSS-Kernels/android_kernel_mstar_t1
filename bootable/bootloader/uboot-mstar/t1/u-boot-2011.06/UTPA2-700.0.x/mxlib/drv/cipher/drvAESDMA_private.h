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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVAESDMA_PRIV_H_
#define _DRVAESDMA_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvCIPHER.h"


typedef MS_U32 (*IOCTL_AESDMA_INIT)(MS_U32, MS_U32, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETIV)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_RESET)(void);
typedef MS_U32 (*IOCTL_AESDMA_SETFILEINOUT)(MS_U32, MS_U32, MS_U32, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETKEY)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_SELENG)(DrvAESDMA_CipherMode, MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_START)(MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_GETSTATUS)(void);
typedef MS_U32 (*IOCTL_AESDMA_ISFINISHED)(void);
typedef MS_U32 (*IOCTL_AESDMA_SETSECUREKEY)(void);
typedef MS_U32 (*IOCTL_AESDMA_SETPS)(MS_U32, MS_U32, MS_BOOL, MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_PSRELEASE)(void);
typedef MS_U32 (*IOCTL_AESDMA_GETPSMATCHEDBYTECNT)(void);
typedef MS_U32 (*IOCTL_AESDMA_GETPSMATCHEDPTN)(void);
typedef MS_U32 (*IOCTL_AESDMA_NOTIFY)(DrvAESDMA_Event, P_DrvAESDMA_EvtCallback);
typedef MS_U32 (*IOCTL_AESDMA_RAND)(MS_U32 *, MS_U32);
typedef MS_U32 (*IOCTL_RSA_CALCULATE)(DrvAESDMA_RSASig *, DrvAESDMA_RSAKey *, DrvAESDMA_RSAMode);
typedef MS_U32 (*IOCTL_RSA_ISFINISHED)(void);
typedef MS_U32 (*IOCTL_RSA_OUTPUT)(DrvAESDMA_RSAMode, DrvAESDMA_RSAOut *);
typedef MS_U32 (*IOCTL_SHA_CALCULATE)(DrvAESDMA_SHAMode, MS_U32, MS_U32, MS_U32);
typedef MS_U32 (*IOCTL_SHA_CALCULATE_MANUAL)(DrvAESDMA_HASHCFG, DrvAESDMA_HASH_STAGE, MS_U32, MS_U8 *);
typedef MS_U32 (*IOCTL_AESDMA_SETKEY_EX)(const MS_U8 *, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETIV_EX)(const MS_U8 *, MS_U32);

typedef struct _AESDMA_RESOURCE_PRIVATE
{
    MS_U32 AES_Dummy;
}AESDMA_RESOURCE_PRIVATE;

typedef struct _SHA_RESOURCE_PRIVATE
{
    MS_U32 SHA_Dummy;
}SHA_RESOURCE_PRIVATE;

typedef struct _RSA_RESOURCE_PRIVATE
{
    MS_U32 RSA_Dummy;
}RSA_RESOURCE_PRIVATE;

typedef struct _AESDMA_RESOURCE_SHARED
{
    void* pAESResource;
}AESDMA_RESOURCE_SHARED;

typedef struct _RSA_RESOURCE_SHARED
{
    void* pRSAResource;
}RSA_RESOURCE_SHARED;

typedef struct _AESDMA_INSTANT_PRIVATE
{
    MS_U8                              InitVector[16];
    MS_U32                             u32IVLen;
    MS_U32                             u32FileinAddr;
    MS_U32                             u32FileInNum;
    MS_U32                             u32FileOutSAddr;
    MS_U32                             u32FileOutEAddr;
    MS_U32                             ptrKey;
    MS_U8                              CipherKey[16];
    MS_U32                             u32KeyLen;
    DrvAESDMA_CipherMode               eMode;
    MS_BOOL                            bDescrypt;
    MS_BOOL                            bSecretKey;
    MS_U32                             u32CmdId;
    IOCTL_AESDMA_INIT                  fpAESDMAInit;
    IOCTL_AESDMA_SETIV                 fpAESDMASetIV;
    IOCTL_AESDMA_RESET                 fpAESDMAReset;
    IOCTL_AESDMA_SETFILEINOUT          fpAESDMASetFileInOut;
    IOCTL_AESDMA_SETKEY                fpAESDMASetKey;
    IOCTL_AESDMA_SELENG                fpAESDMASelEng;
    IOCTL_AESDMA_START                 fpAESDMAStart;
    IOCTL_AESDMA_GETSTATUS             fpAESDMAGetStatus;
    IOCTL_AESDMA_ISFINISHED            fpAESDMAIsFinished;
    IOCTL_AESDMA_SETSECUREKEY          fpAESDMASetSecureKey;
    IOCTL_AESDMA_SETPS                 fpAESDMASetPS;
    IOCTL_AESDMA_PSRELEASE             fpAESDMAPSRelease;
    IOCTL_AESDMA_GETPSMATCHEDBYTECNT   fpAESDMAGetPSMatchedByteCNT;
    IOCTL_AESDMA_GETPSMATCHEDPTN       fpAESDMAGetPSMatchedPTN;
    IOCTL_AESDMA_NOTIFY                fpAESDMANotify;
    IOCTL_AESDMA_RAND                  fpAESDMARand;
    IOCTL_RSA_CALCULATE                fpRSACalculate;
    IOCTL_RSA_ISFINISHED               fpRSAIsFinished;
    IOCTL_RSA_OUTPUT                   fpRSAOutput;
    IOCTL_SHA_CALCULATE                fpSHACalculate;
    IOCTL_SHA_CALCULATE_MANUAL         fpSHACalculateManual;
    IOCTL_AESDMA_SETKEY_EX             fpAESDMASetKey_Ex;
    IOCTL_AESDMA_SETIV_EX              fpAESDMASetIV_Ex;
}AESDMA_INSTANT_PRIVATE;

void AESDMARegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 AESDMAOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 AESDMAClose(void* pInstantTmp);
MS_U32 AESDMAIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);

DRVAESDMA_RESULT _MDrv_AESDMA_Init(MS_U32 u32miu0addr , MS_U32 u32miu1addr , MS_U32 u32miunum);
DRVAESDMA_RESULT _MDrv_AESDMA_QueryCipherMode(DrvAESDMA_CipherMode eMode);
DRVAESDMA_RESULT _MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt);
DRVAESDMA_RESULT _MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len);
DRVAESDMA_RESULT _MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len);
DRVAESDMA_RESULT _MDrv_AESDMA_SetKey(MS_U32 *pCipherKey);
DRVAESDMA_RESULT _MDrv_AESDMA_SetSecureKey(void);
DRVAESDMA_RESULT _MDrv_AESDMA_SetIV(MS_U32 *pInitVector);
DRVAESDMA_RESULT _MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable);
DRVAESDMA_RESULT _MDrv_AESDMA_PSRelease(void);
DRVAESDMA_RESULT _MDrv_AESDMA_SetFileInOut(MS_U32 u32FileinAddr, MS_U32 u32FileInNum, MS_U32 u32FileOutSAddr, MS_U32 u32FileOutEAddr);
DRVAESDMA_RESULT _MDrv_AESDMA_Start(MS_BOOL bStart);
DRVAESDMA_RESULT _MDrv_AESDMA_Reset(void);
DRVAESDMA_RESULT _MDrv_AESDMA_GetStatus(void);
DRVAESDMA_RESULT _MDrv_AESDMA_IsFinished(void);
MS_U32 _MDrv_AESDMA_GetPSMatchedByteCNT(void);
MS_U32 _MDrv_AESDMA_GetPSMatchedPTN(void);
DRVAESDMA_RESULT _MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback);
DRVAESDMA_RESULT _MDrv_AESDMA_SetDbgLevel(DrvAESDMA_DbgLevel DbgLevel);
DRVAESDMA_RESULT _MDrv_AESDMA_GetLibVer(const MSIF_Version **ppVersion);
DRVAESDMA_RESULT _MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size);
DRVAESDMA_RESULT _MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_U32 u32PAInBuf, MS_U32 u32Size, MS_U32 u32PAOutBuf);
DRVAESDMA_RESULT _MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV);
DRVAESDMA_RESULT _MDrv_MOBF_DmaOnly(void);
DRVAESDMA_RESULT _MDrv_MOBF_Encrypt(MS_U32 u32Key, MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_MOBF_Decrypt(MS_U32 u32Key, MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_MOBF_OneWay(void);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern);
MS_U8 _MDrv_AESDMA_Parser_QueryPidCount(void);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode);
MS_BOOL _MDrv_AESDMA_Lock(void);
MS_BOOL _MDrv_AESDMA_Unlock(void);
DRVAESDMA_RESULT _MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode);
DRVAESDMA_RESULT _MDrv_RSA_Calculate_Hw_Key(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAHwKeyMode eMode);
DRVAESDMA_RESULT _MDrv_RSA_IsFinished(void);
DRVAESDMA_RESULT _MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut);

#ifdef __cplusplus
}
#endif
#endif // _DRVAESDMA_PRIV_H_
