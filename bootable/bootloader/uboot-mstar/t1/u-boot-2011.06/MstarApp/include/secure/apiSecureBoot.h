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


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiSecureBoot.h
/// @brief  Secure Boot Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_SECUREBOOT_H_
#define _API_SECUREBOOT_H_

#ifdef API_SECUREBOOT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#include <MsTypes.h>
#include <drvAESDMA.h>
#include <ShareType.h>
#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SECUREBOOT_MBX_QUEUESIZE        8

#define MBX_CTRL_FIRE            BIT(0)
#define MBX_CTRL_READBACK        BIT(1)
#define MBX_CTRL_INSTANT         BIT(2)
#define MBX_STATE1_OVERFLOW      BIT(13)
#define MBX_STATE1_ERROR         BIT(14)
#define MBX_STATE1_BUSY          BIT(15)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// emurate SECUREBOOT fucntion return result
typedef enum
{
    /// SECUREBOOT result for failure
    E_SECUREBOOT_FAIL =0,
    /// SECUREBOOT result for OK
    E_SECUREBOOT_OK = 1,

} SECUREBOOT_Result;

///Define SECUREBOOT Acknoledge Flags
typedef enum
{
    E_SECUREBOOT_ACKFLG_WAIT_GETKEY     = (1<<0),      
    E_SECUREBOOT_ACKFLG_WAIT_AESENCODE  = (1<<1),      
    E_SECUREBOOT_ACKFLG_WAIT_AESDECODE  = (1<<2),      
    E_SECUREBOOT_ACKFLG_SETEKEY_WITH_DECRYPTION    = (1<<3),
    E_SECUREBOOT_ACKFLG_ASSIGN_DECRYPTED_KEY    = (1<<4),   
    E_SECUREBOOT_ACKFLG_GETDID    = (1<<5), 

} SECUREBOOT_AckFlags;

typedef enum
{
	E_CMD_SECUREENG_GETKEY=0x00,
	E_CMD_SECUREENG_AES_ENCODE,
    E_CMD_SECUREENG_AES_DECODE,
    E_CMD_SECUREENG_SETEKEY_WITH_DECRYPTION,    
    E_CMD_SECUREENG_ASSIGN_DECRYPTED_KEY,    
    E_CMD_SECUREENG_GETDID,
}SECUREENG_CPUTo51_CMDIDX;

typedef enum
{
    //(1) Ack from 51 to CPU
    E_SECUREENG_51ToCPU_CMD_ACK_GETKEY     = 0x00,
    E_SECUREENG_51ToCPU_CMD_ACK_AES_ENCODE,
    E_SECUREENG_51ToCPU_CMD_ACK_AES_DECODE,
    E_SECUREENG_51ToCPU_CMD_ACK_SETEKEY_WITH_DECRYPTION,
    E_SECUREENG_51ToCPU_CMD_ACK_ASSIGN_DECRYPTED_KEY,    
    E_SECUREENG_51ToCPU_CMD_ACK_GETDID,        
} SECUREENG_51ToCPU_CMDIDX;

typedef struct
{
    MS_U32  u32Pm51ToCpuAddr;
    MS_U32  u32Length;
    MS_U8   u8Status;
} SECUREBOOT_MbxRetStruct;

#if 0
/// Callback function which is called in SECUREBOOT send MAILBOX ISR.
typedef void ( *SECUREBOOT_Callback ) (void* pData);
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL Secure_MailBox_Init(void);
INTERFACE SECUREBOOT_Result Secure_MailBox_GetKey(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct,U32 u32OutAddr,U8  u8KeyImageIndex,U16  u16LenInKeyImage);
INTERFACE SECUREBOOT_Result Secure_MailBox_AESEncode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode);
INTERFACE SECUREBOOT_Result Secure_MailBox_AESDecode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode);
INTERFACE SECUREBOOT_Result Secure_MailBox_ASSIGN_DECRYPTED_KEY(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U8  u8KeyImageIndex,U16  u16LenInKeyImage, U8 u8IsNoSwap);
INTERFACE SECUREBOOT_Result Secure_MailBox_SET_KEY_WITH_DECRYPTION(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U16 u16Len, U16 u16KeyOffset,U8 u8Mode);
INTERFACE SECUREBOOT_Result Secure_MailBox_GETDID(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U16 u16Len);

#ifdef __cplusplus
}
#endif

#undef INTERFACE
#endif // _API_SECUREBOOT_H_

