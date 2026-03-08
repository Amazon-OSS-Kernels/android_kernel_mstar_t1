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
/// file   drvDscmb.h
/// @brief  Descrambler (Dscmb) Driver Interface
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __HAL_DSCMB_H__
#define __HAL_DSCMB_H__

#define HAL_DSCMB_FLT_NULL                      0xFFFFFFFFUL

typedef enum
{
    E_HAL_DSCMB_TYPE_CSA = 0,   // CSA, NSA as ESA
    E_HAL_DSCMB_TYPE_CSA_CONF,  // CSA conformance mechanism
    E_HAL_DSCMB_TYPE_DES,       // CI+ DES
    E_HAL_DSCMB_TYPE_AES,       // CI+ AES
    E_HAL_DSCMB_TYPE_AES_CTR,   // HDCP2 AES CTR
    E_HAL_DSCMB_TYPE_NSA_AS_ESA,// NSA as ESA
    E_HAL_DSCMB_TYPE_AES_LEADING_CLR_CBC,
    E_HAL_DSCMB_TYPE_AES_LEADING_CLR_EBC,
    E_HAL_DSCMB_TYPE_CSA3,
    E_HAL_DSCMB_TYPE_DES_CBC,
    E_HAL_DSCMB_TYPE_TDES_CBC,
    E_HAL_DSCMB_TYPE_AES_ECB,
    E_HAL_DSCMB_TYPE_AES_SCTE52, // DBook, AES+CBC, T8 new
    E_HAL_DSCMB_TYPE_TDES_ECB,
    E_HAL_DSCMB_TYPE_TDES_SCTE52,

    E_HAL_DSCMB_TYPE_MULTI2,
    E_HAL_DSCMB_TYPE_OC, // OpenCable
    E_HAL_DSCMB_TYPE_INVALID = 0xFFFFFFFFUL   // NSA as ESA
} HAL_DSCMB_Type;

typedef enum
{
    E_HAL_DSCMB_NONE=           0x00000000,
    E_HAL_DSCMB_TS=             0x00000001,
    E_HAL_DSCMB_PES=            0x00000002,
    E_HAL_DSCMB_TS_PES=         (E_HAL_DSCMB_TS| E_HAL_DSCMB_PES),
} HAL_Scmb_Level;

// descrambler key ladder
typedef enum
{
    E_HAL_DSCMB_KLAD_SRC_ACPU =             0,
    E_HAL_DSCMB_KLAD_SRC_SECRET_KEY1 =      1,
    E_HAL_DSCMB_KLAD_SRC_SECRET_KEY2 =      2,
    E_HAL_DSCMB_KLAD_SRC_SECRET_KEY3 =      3,
    E_HAL_DSCMB_KLAD_SRC_SECRET_KEY4 =      4,
    E_HAL_DSCMB_KLAD_SRC_VGK =              7,
} HAL_DscmbKLadderSrc;

typedef enum
{
    E_HAL_DSCMB_KLAD_DST_KTAB_ESA =         0x4,
    E_HAL_DSCMB_KLAD_DST_KTAB_NSA =         0x2,
    E_HAL_DSCMB_KLAD_DST_AESDMA_AES =       0x8,
    E_HAL_DSCMB_KLAD_DST_AESDMA_TDES =      0x10,
    E_HAL_DSCMB_KLAD_DST_ACPU =             0x1,
} HAL_DscmbKLadderDst;

typedef enum
{
    // descrambler engine
    E_HAL_DSCMB_CAP_ENGINE_NUM,
    // descrambler slot
    E_HAL_DSCMB_CAP_FLT_NUM,
    // descrambler type supported
    E_HAL_DSCMB_CAP_SUPPORT_ALGORITHM,
    // descrambler mapping, start tsp pid filter
    E_HAL_DSCMB_CAP_PIDFILTER_MAP_START,
    // descrambler mapping, end tsp pid filter
    E_HAL_DSCMB_CAP_PIDFILTER_MAP_END,
    // share key slot max number
    E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_NUM,
    // share key slot max number
    E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_MAX_NUM,
    // share key region number
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM,
    // share key region start
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_START,
    // share key region end
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_END,
} HAL_DSCMB_Query_Type;

MS_BOOL HAL_DSCMB_SetBank(MS_VIRT u32Bank);
MS_BOOL HAL_DSCMB_Init(void);
MS_BOOL HAL_DSCMB_CheckShareConnect(MS_U32 u32EngId, MS_U32 u32FltId, DSCMB_SLOT_Type SlotType);
MS_BOOL HAL_DSCMB_GetCap(MS_U32 u32EngId, HAL_DSCMB_Query_Type eQueryType, void* pInData, void* pOutData);

// MS_BOOL HAL_DSCMB_SlotAlloc(MS_U32 u32DscmbId, HAL_DscmbKey_Type DscmbKeyType, MS_U32* pu32SlotId);
// MS_BOOL HAL_DSCMB_SlotFree(MS_U32 u32DscmbId, HAL_DscmbKey_Type, MS_U32 u32SlotId);
MS_BOOL HAL_DSCMB_SlotConnectPidFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId, MS_U32 u32PidFltId);
MS_BOOL HAL_DSCMB_SlotDisconnectPidFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId, MS_U32 u32PidFltId);
MS_BOOL HAL_DSCMB_FltTypeSet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, MS_BOOL bDscmb);
MS_BOOL HAL_DSCMB_FltKeySet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type, MS_U8* pu8Key);
MS_BOOL HAL_DSCMB_FltKeyReset(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType);
MS_BOOL HAL_DSCMB_FltIVSet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType, MS_U8* pu8IV);

MS_BOOL HAL_DSCMB_EcmCorruptSet(MS_U8 u8CorruptDataBytes, MS_U8 u8FromByte);
MS_BOOL HAL_DSCMB_EcmCorruptEnable(MS_U32 u32PidFltId, MS_BOOL bEnable);

MS_BOOL HAL_DSCMB_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, HAL_Scmb_Level* pScmbLevel);

MS_BOOL HAL_DSCMB_KLadder_Reset(void);
MS_BOOL HAL_DSCMB_KLadder_SetInput(MS_U32 u32Level, MS_U8* pu8In, MS_U32 u32InLen);
MS_BOOL HAL_DSCMB_KLadder_SetKey(HAL_DscmbKLadderSrc KLSrc, MS_U8* pu8Key, MS_U32 u32KeyLen);
MS_BOOL HAL_DSCMB_KLadder_SetDst(HAL_DscmbKLadderDst KLDst);
void HAL_DSCMB_KLadder_Decrypt(MS_BOOL bDecrypt);
void HAL_DSCMB_KLadder_ByteInv(MS_BOOL bInverse);
void HAL_DSCMB_KLadder_KeepRound(MS_BOOL bKeepRound);
void HAL_DSCMB_KLadder_Round(MS_U32 u32Round);
void HAL_DSCMB_KLadder_EngMode(MS_U32 u32Mode);
MS_BOOL HAL_DSCMB_KLadder_Start(void);
MS_BOOL HAL_DSCMB_KLadder_IsComplete(void);
MS_BOOL HAL_DSCMB_KLadder_Stop(void);
MS_BOOL HAL_DSCMB_KLadder_Output_Start(MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType);
MS_BOOL HAL_DSCMB_KLadder_Output_IsComplete(void);
MS_BOOL HAL_DSCMB_KLadder_Output_Stop(void);

MS_BOOL HAL_DSCMB_KLadder_Acpu_Data(MS_U8* pu8Out);
MS_BOOL HAL_DSCMB_KLadder_Acpu_Ack(MS_BOOL bEn);

MS_BOOL HAL_DSCMB_HDCP2_SetRiv(MS_U32 u32EngId, MS_U8* pu8RIV);
// Only for scramble
MS_BOOL HAL_DSCMB_2ndarySet(MS_U32 u32PidPrim, MS_U32 u32Pid2ndary);
MS_BOOL HAL_DSCMB_2ndaryReset(MS_U32 u32PidPrim);

MS_BOOL HAL_DSCMB_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round);
MS_BOOL HAL_DSCMB_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8SysKey);

#endif
