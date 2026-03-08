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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvAESDMA_v2.h
/// @brief  AESDMA Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_AESDMA_V2_H_
#define _DRV_AESDMA_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    MDrv_CMD_AESDMA_Init,
    MDrv_CMD_AESDMA_SetIV,
    MDrv_CMD_AESDMA_Reset,
    MDrv_CMD_AESDMA_SetFileInOut,
    MDrv_CMD_AESDMA_SetKey,
    MDrv_CMD_AESDMA_SelEng,
    MDrv_CMD_AESDMA_Start,
    MDrv_CMD_AESDMA_GetStatus,
    MDrv_CMD_AESDMA_IsFinished,
    MDrv_CMD_AESDMA_SetClk,
    MDrv_CMD_AESDMA_SetSecureKey,
    MDrv_CMD_AESDMA_SetPS,
    MDrv_CMD_AESDMA_PSRelease,
    MDrv_CMD_AESDMA_GetPSMatchedByteCNT,
    MDrv_CMD_AESDMA_GetPSMatchedPTN,
    MDrv_CMD_AESDMA_Notify,
    MDrv_CMD_AESDMA_Rand,
    MDrv_CMD_RSA_Calculate,
    MDrv_CMD_RSA_IsFinished,
    MDrv_CMD_RSA_Output,
    MDrv_CMD_SHA_Calculate,
    MDrv_CMD_AESDMA_Memcpy,    
    MDrv_CMD_SHA_CalculateManual,
    MDrv_CMD_AESDMA_PARSER_MASKSCRMB,
    MDrv_CMD_AESDMA_PARSER_SETSCRMBPATTERN,
    MDrv_CMD_AESDMA_PARSER_BYPASSPID,
    MDrv_CMD_AESDMA_PARSER_SETPID,
    MDrv_CMD_AESDMA_PARSER_ENCRYPT,
    MDrv_CMD_AESDMA_PARSER_DECRYPT,
    MDrv_CMD_AESDMA_PARSER_Start,
    MDrv_CMD_AESDMA_PARSER_SETADDEDSCRMBPATTERN,
    MDrv_CMD_AESDMA_PARSER_QUERYPIDCOUNT,
    MDrv_CMD_AESDMA_ENABLE_Two_Key,
    MDrv_CMD_AESDMA_SetOddKey,
    MDrv_CMD_AESDMA_SetOddIV,
    MDrv_CMD_IS_SECRETKEY_IN_NORMAL_BLANK,
    MDrv_CMD_AESDMA_SetKey_Ex,
    MDrv_CMD_AESDMA_SetIV_Ex,
    MDrv_CMD_HDCP_MDrv_PROCESSCIPHER,
    MDrv_CMD_HDCP_GetHdcpCipherState,

} eAesdmaIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct _AESDMA_INIT
{
    MS_PHY u32miu0addr;
    MS_PHY u32miu1addr;
    MS_U32 u32miunum;
}AESDMA_INIT, *PAESDMA_INIT;

typedef struct _AESDMA_FILE_INOUT
{
    MS_PHY u32FileinAddr;
    MS_U32 u32FileInNum;
    MS_PHY u32FileOutSAddr;
    MS_PHY u32FileOutEAddr;
}AESDMA_FILE_INOUT, *PAESDMA_FILE_INOUT;

typedef struct _AESDMA_SEL_ENG
{
    DrvAESDMA_CipherMode eMode;
    MS_BOOL bDescrypt;
}AESDMA_SEL_ENG, *PAESDMA_SEL_ENG;

typedef struct _AESDMA_SET_IV
{
    MS_U32 *pInitVector;
}AESDMA_SET_IV, *PAESDMA_SET_IV;

typedef struct _AESDMA_SET_KEY
{
    MS_U32 *pCipherKey;
}AESDMA_SET_KEY, *PAESDMA_SET_KEY;

typedef struct _AESDMA_SET_KEY_EX
{
    const MS_U8 *pu8Key;
    MS_U32 u32Len;
}AESDMA_SET_KEY_EX, *PAESDMA_SET_KEY_EX;

typedef struct _AESDMA_SET_IV_EX
{
    const MS_U8 *pu8IV;
    MS_U32 u32Len;
}AESDMA_SET_IV_EX, *PAESDMA_SET_IV_EX;


typedef struct _AESDMA_SET_PS
{
    MS_U32 u32PTN;
    MS_U32 u32Mask;
    MS_BOOL bPSin_Enable;
    MS_BOOL bPSout_Enable;
}AESDMA_SET_PS, *PAESDMA_SET_PS;

typedef struct _AESDMA_START
{
    MS_BOOL bStart;
}AESDMA_START, *PAESDMA_START;


typedef struct _AESDMA_NOTIFY
{
    DrvAESDMA_Event eEvents;
    P_DrvAESDMA_EvtCallback pfCallback;
}AESDMA_NOTIFY, *PAESDMA_NOTIFY;

typedef struct _AESDMA_RAND
{
    MS_U32 *u32PABuf;
    MS_U32 u32Size;
}AESDMA_RAND, *PAESDMA_RAND;

typedef struct _RSA_CALCULATE
{
    DrvAESDMA_RSASig *pstSign;
    DrvAESDMA_RSAKey *pstKey;
    DrvAESDMA_RSAMode eMode;
}RSA_CALCULATE, *PRSA_CALCULATE;

typedef struct _RSA_OUTPUT
{
    DrvAESDMA_RSAMode eMode;
    DrvAESDMA_RSAOut *pstRSAOut;
}RSA_OUTPUT, *PRSA_OUTPUT;

typedef struct _SHA_CALCULATE
{
    DrvAESDMA_SHAMode eMode;
    MS_PHY u32PAInBuf;
    MS_U32 u32Size;
    MS_PHY u32PAOutBuf;
}SHA_CALCULATE, *PSHA_CALCULATE;

typedef struct _SHA_CALCULATE_MANUAL
{
    DrvAESDMA_HASHCFG stCfg;
    DrvAESDMA_HASH_STAGE eStage;
    MS_U32 u32DoneBytes;
    MS_U8 *pu8SetIV;
}SHA_CALCULATE_MANUAL, *PSHA_CALCULATE_MANUAL;

typedef struct _AESDMA_PARSER_CONFIGURATIONS
{
    DrvAESDMA_ScrmbPattern eScrmbPattern;
    DrvAESDMA_ParserMode   eParserMode;
    DrvAESDMA_ScrmbPattern eAddedScrmbPattern;
}AESDMA_PARSER_CONFIGURATIONS, *PAESDMA_PARSER_CONFIGURATIONS;

typedef struct _AESDMA_PARSER_PID_CONFIGURATIONS
{
    //Don't care PidIndex and Pid if BypassPid is true;
    MS_BOOL bBypassPid;
    MS_U8   u8PidIndex;
    MS_U16  u16Pid;
}AESDMA_PARSER_PID_CONFIGURATIONS, *PAESDMA_PARSER_PID_CONFIGURATIONS;

typedef struct _HDCP_CIPHER
{
    MS_U8 u8Idx;
    MS_U8 pu8Riv[8];
    MS_U8 pu8ContentKey[16];
}HDCP_CIPHER, *PHDCP_CIPHER;

typedef struct _HDCP_CIPHER_STATE
{
    MS_U8 u8Idx;         //Port Index.
    MS_U8 *pu8State;    //For return Status.
}HDCP_CIPHER_STATE, *PHDCP_CIPHER_STATE;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_AESDMA_V2_H_

