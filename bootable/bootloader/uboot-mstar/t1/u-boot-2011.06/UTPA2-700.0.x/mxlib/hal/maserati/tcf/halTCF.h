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
// file   halTCF.h
// @brief  TCF HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_TCF_H__
#define __HAL_TCF_H__


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
typedef struct      // MSTAR_CF_VERSION //
{
    MS_U8                         _u8VersionEpoch;
    MS_U8                         _u8ManufacturerId;
    MS_U8                         _u8NetlistVersion;
    MS_U8                         _u8VersionBuildId;
} HAL_CF_VERSION_INFO;

typedef struct      // MSTAR_CF_FEATURE //
{
    MS_U32                        _u32FeatureVector;
} HAL_CF_FEATURE_VECTOR;

typedef struct      // MSTAR_CF_TRANS_STATUS //
{
    MS_U8                         _eTransStatus;
    MS_U8                         _u8UseNvmKey;
    MS_U8                         _eOperationType;
    MS_U8                         _eDecmSrc;
    MS_U8                         _eOutputUsage;
    MS_U8                         _u8ProductRange;
    MS_U8                         _u8ProductOffset;
} HAL_CF_TRANS_STATUS;

typedef struct      // MSTAR_CF_CF_STATUS //
{
    MS_U8                         _eNvmStatus;
    MS_U8                         _eDiffStatus;
    MS_U8                         _u8RecentReset;
    MS_U8                         _u8CfAlert;
    MS_U8                         _u8DevelopmentMode;
    MS_U8                         _u8FuseActivate;
    MS_U8                         _u8FuseBlock;
} HAL_CF_CF_STATUS;



typedef struct
{
    MS_U8                         _u8CwcPid;
    MS_U8                         _u8CwcFld;
    MS_U8                         _u8CwcFscb;
    MS_U8                         _u8CwcScb;
    MS_U8                         _u8CwcTsid;
} HAL_CFB_CWCCFG;


////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_TCF_SetBank(MS_U32 u32NonPmBankAddr);
void HAL_TCF_Clk(MS_BOOL bEnable);


// CF
//
void HAL_CF_Version_Info(HAL_CF_VERSION_INFO *_pstVersion);
void HAL_CF_Trans_Status(HAL_CF_TRANS_STATUS *_pstTransStatus);
void HAL_CF_Cf_Status(HAL_CF_CF_STATUS *_pstCfStatus);
void HAL_CF_Cf_FeatureVector(HAL_CF_FEATURE_VECTOR *_pstCfFeature);

MS_U8 HAL_CF_Get_Trans_Status(void);
MS_U8 HAL_CF_Get_Operation_Type(void);
MS_U8 HAL_CF_Get_CwcValid(void);
MS_U32 HAL_CF_Read_Input(void);
MS_U32 HAL_CF_Read_Output(void);
void HAL_CF_Write_Input(MS_U32 u32Cmd);
void HAL_CF_Write_Output(MS_U32 u32Cmd);
MS_U8 HAL_CF_Get_CF_IsFinished(void);


// CFKE
//
MS_U8 HAL_CFKE_Get_OperationStatus(void);
MS_U8 HAL_CFKE_Get_OperationCount(void);
MS_U8 HAL_CFKE_Get_ErrorCode(void);
MS_U8 HAL_CFKE_Get_OperationType(void);
MS_U8 HAL_CFKE_Get_DiffAllowNonUnw(void);
MS_U8 HAL_CFKE_Get_DiffAllowCk(void);
MS_U8 HAL_CFKE_Get_DiffContentOutputAllowed(void);

MS_U32 HAL_CFKE_Read_Command(void);
void HAL_CFKE_Write_Command(MS_U32 u32Cmd);

MS_U32 HAL_CFKE_Read_Platform(void);
MS_BOOL HAL_CFKE_IsTransactionDone(void);

MS_U32 HAL_CFKE_Read_UnwPath(void);
void HAL_CFKE_Write_UnwPath(MS_U32 u32Cmd);

MS_U32 HAL_CFKE_Read_Validator0(void);
void HAL_CFKE_Write_Validator0(MS_U32 u32Cmd);
MS_U32 HAL_CFKE_Read_Validator1(void);
void HAL_CFKE_Write_Validator1(MS_U32 u32Cmd);

void HAL_CFKE_Write_SlotA(MS_U32 u32Cmd);
void HAL_CFKE_Write_SlotB(MS_U32 u32Cmd);
void HAL_CFKE_Write_SlotC(MS_U32 u32Cmd);
void HAL_CFKE_Write_SlotD(MS_U32 u32Cmd);
void HAL_CFKE_Write_UserHash(MS_U32 u32Cmd);


// CRIKL
// Mirror of CF, CFKE Resigers
MS_U32 HAL_CRIKL_Read_CfStatus(void);
MS_U32 HAL_CRIKL_Read_CfkeStatus(void);
MS_U32 HAL_CRIKL_Read_CfPlatform(void);
MS_U32 HAL_CRIKL_Read_CfFeature(void);

// Key Table control
void HAL_CRIKL_Set_KTIndex( MS_U8 eScmbCtl, MS_U8 u8DscmbId);
void HAL_CRIKL_Write_KTPara_High_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KTPara_2nd_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KTPara_3rd_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KTPara_Low_HalfWord(MS_U16 u16HalfWord);
MS_U16 HAL_CRIKL_Read_KTIndex(void);
MS_U16 HAL_CRIKL_Read_KTPara_High_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KTPara_2nd_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KTPara_3rd_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KTPara_Low_HalfWord(void);
// Key Slot control
void HAL_CRIKL_Set_KSIndex(MS_U8 u8KsIdx);
void HAL_CRIKL_Write_KSPara_High_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KSPara_2nd_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KSPara_3rd_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KSPara_Low_HalfWord(MS_U16 u16HalfWord);
MS_U16 HAL_CRIKL_Read_KSIndex(void);
MS_U16 HAL_CRIKL_Read_KSPara_High_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KSPara_2nd_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KSPara_3rd_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KSPara_Low_HalfWord(void);

// Key Parameter
void HAL_CRIKL_Reset_KeyPara(void);
void HAL_CRIKL_Write_KeyPara(MS_BOOL bDscmbKey);

void HAL_CRIKL_Setup_VendorNo(MS_BOOL bDscmbKey, MS_U8 u8VendorNo);
void HAL_CRIKL_Setup_VendorAtt(MS_BOOL bDscmbKey, MS_U16 u16VendorAtt);
void HAL_CRIKL_Setup_OutputAllowed(MS_BOOL bDscmbKey, MS_U8 u8OutputAllowed);
void HAL_CRIKL_Setup_bMultiEng(MS_BOOL bDscmbKey, MS_BOOL bMultiEng);
void HAL_CRIKL_Setup_bLsb64(MS_BOOL bDscmbKey, MS_BOOL bLsb64);
void HAL_CRIKL_Setup_KeySize(MS_BOOL bDscmbKey, MS_U8 eKeySize);
void HAL_CRIKL_Setup_KeyEngine(MS_BOOL bDscmbKey, MS_U8 uKeyEng);
void HAL_CRIKL_Setup_Drop(MS_BOOL bDscmbKey, MS_U8 u8Drop);
void HAL_CRIKL_Setup_Cnt(MS_BOOL bDscmbKey, MS_U8 u8Cnt);
void HAL_CRIKL_Setup_KeyDest(MS_BOOL bDscmbKey, MS_U8 u8KeyDest);
void HAL_CRIKL_Setup_OpType(MS_BOOL bDscmbKey, MS_U8 u8Op);
void HAL_CRIKL_Setup_KteSel(MS_BOOL bDscmbKey, MS_U8 eKteSel);
void HAL_CRIKL_Setup_bLock(MS_BOOL bDscmbKey, MS_BOOL bLock);
void HAL_CRIKL_Setup_bParaValid(MS_BOOL bDscmbKey, MS_BOOL bParaValid);
void HAL_CRIKL_Setup_bKlEn(MS_BOOL bDscmbKey, MS_BOOL bKlEn);

#endif // #ifndef __HAL_TCF_H__
