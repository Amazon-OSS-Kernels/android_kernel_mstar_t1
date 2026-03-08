/**
* Copyright (c) 2006 ??2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/**
* @file    drv_eMMC.h
* @version
* @brief
*
*/

#ifndef __DRV_eMMC_H__
#define __DRV_eMMC_H__

#ifndef U32
#define U32  unsigned long
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S32
#define S32  signed long
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif

/*=============================================================*/
// Include files
/*=============================================================*/
#include "../config/eMMC_config.h"


/*=============================================================*/
// Macro definition
/*=============================================================*/

/*=============================================================*/
// Data type definition
/*=============================================================*/

/*=============================================================*/
// Variable definition
/*=============================================================*/

/*=============================================================*/
// Global function definition
/*=============================================================*/
extern U32 eMMC_LoadImages(U32 u32_Addr, U32 u32_ByteCnt, U32 u32_BlkAddr);
//--------------------------------------------
// CAUTION: u32_DataByteCnt has to be 512B x n
//--------------------------------------------
extern U32  eMMC_ReadData_CIFD(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32  eMMC_ReadData_MIU(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32  eMMC_WriteData_MIU(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
//--------------------------------------------

extern U32 eMMC_GetExtCSD(U8* pu8_Ext_CSD);
extern U32 eMMC_SetExtCSD(U8 u8_AccessMode, U8 u8_ByteIdx, U8 u8_Value);

/*=============================================================*/
// internal function definition
/*=============================================================*/
extern U32  eMMC_ReadBootPart(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32  eMMC_CheckIfReady(void);

extern  U32 eMMC_CMD18_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CMD23_RPMB(U16 u16_BlkCnt, U8 u8_Is_Reliable);
extern  U32 eMMC_CMD25_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt, U8 u8_Is_Reliable);
extern  void eMMC_SetError(U16 ErrCode, U16 Pos);
extern  void *eMMC_get_RPMBContext_address(void);
extern  void eMMC_GetCID(U8 *pu8_CID);
extern  U32  GetHmacKey(U32 u32KeyBufPA, U32 u32key_len);
extern  void MDrv_AESDMA_Hmac_sha256(const U8 *text, U32 text_len, const U8 *key, U32 key_len, void *digest);
extern  U32  eMMC_RPMB_Get_Counter(volatile U32 *pu32_WCnt);
extern  U32  eMMC_RPMB_IfKeyWritten(void);
extern  U32  eMMC_RPMB_Read_Result(U16 *pu16_Result);
extern  void eMMC_RPMB_Copy_RESULT(U8 *u8_dest,U8 *u8_src, U32 u32_cnt);
extern  U32  eMMC_RPMB_CheckResultError(void);
extern  void eMMC_RPMB_SWAP_memcpy(U8 *u8_dest,U8 *u8_src, U32 u32_cnt);
extern  U32  eMMC_ComapreData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt);
extern  U32  eMMC_RPMB_program_auth_key(void);
extern  U32  eMMC_RPMB_Test(void);
extern  U32  eMMC_RPMB_Verify_Hmac(U8* pu8_DataBuf, U16 u16_BlkCnt);
extern  U32  eMMC_RPMB_Read_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_BlkAddr);
extern  U32  eMMC_RPMB_Write_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_BlkAddr);
extern  U32  eMMC_RPMB_IfKeyWritten(void);
extern  U32  eMMC_RPMB_NoDataReq(void);
#endif //__DRV_eMMC_H__

