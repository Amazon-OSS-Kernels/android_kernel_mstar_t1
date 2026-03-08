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

#ifndef _HAL_MIU_H_
#define _HAL_MIU_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _FUNC_NOT_USED()        do {} while ( 0 )

#define MIU_MAX_DEVICE           (3)
//Max MIU Group number
#define MIU_MAX_GROUP            (8)
#define MIU_MAX_GP_CLIENT        (16)
#define MIU_MAX_TBL_CLIENT       (MIU_MAX_GROUP*MIU_MAX_GP_CLIENT)
#define MIU_PAGE_SHIFT           (13) //Unit for MIU protect
#define MIU_PROTECT_ADDRESS_UNIT (0x20) //Unit for MIU hitted address
#define MIU_MAX_PROTECT_BLOCK    (4)
#define MIU_MAX_PROTECT_ID       (16)
#define MIU_BLOCK0_CLIENT_NUMBER (16)
#define MIU_BLOCK1_CLIENT_NUMBER (16)
#define MIU_BLOCK2_CLIENT_NUMBER (16)
#define MIU_BLOCK3_CLIENT_NUMBER (16)
#define MIU_BLOCK4_CLIENT_NUMBER (16)
#define MIU_BLOCK5_CLIENT_NUMBER (16)

#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

#define MIU_OPM_R_MASK 0x0667
#define MIU_OPM_W_MASK 0x0666
#define MIU_MVD_R_MASK 0x06F6
#define MIU_MVD_W_MASK 0x06F7

//$ MIU0 Request Mask functions
#define _MaskMiuReq_OPM_R( m )     HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT1)

#define _MaskMiuReq_DNRB_W( m )    HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT2)
#define _MaskMiuReq_DNRB_R( m )    HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT3)
#define _MaskMiuReq_DNRB_RW( m )   HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT2|BIT3)

#define _MaskMiuReq_SC_RW( m )     HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT1|BIT2|BIT3)

#define _MaskMiuReq_MVOP_R( m )    HAL_MIU_WriteRegBit(MIU_RQ1L_MASK, m, BIT3)

#define _MaskMiuReq_MVD_R( m )     do { HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT4); HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT5); } while(0)
#define _MaskMiuReq_MVD_W( m )     do { HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT4); HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT5); } while(0)
#define _MaskMiuReq_MVD_RW( m )    do { _MaskMiuReq_MVD_R( m ); _MaskMiuReq_MVD_W( m ); } while (0)

#define _MaskMiuReq_AUDIO_RW( m )  _FUNC_NOT_USED()


//$ MIU1 Request Mask functions
#define _MaskMiu1Req_OPM_R( m )     HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT1)

#define _MaskMiu1Req_DNRB_W( m )    HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT2)
#define _MaskMiu1Req_DNRB_R( m )    HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT3)
#define _MaskMiu1Req_DNRB_RW( m )   HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT2|BIT3)

#define _MaskMiu1Req_SC_RW( m )     HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT1|BIT2|BIT3)

#define _MaskMiu1Req_MVOP_R( m )    HAL_MIU_WriteRegBit(MIU_RQ1L_MASK, m, BIT3)

#define _MaskMiu1Req_MVD_R( m )     do { HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT4); HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT5); } while(0)
#define _MaskMiu1Req_MVD_W( m )     do { HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT4); HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT5); } while(0)
#define _MaskMiu1Req_MVD_RW( m )    do { _MaskMiuReq_MVD_R( m ); _MaskMiuReq_MVD_W( m ); } while (0)

#define _MaskMiu1Req_AUDIO_RW( m )  _FUNC_NOT_USED()

#define MIU_GET_CLIENT_POS(x)       (x & 0x0F)
#define MIU_GET_CLIENT_GROUP(x)     ((x & 0xF0) >> 4)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
  E_MIU_BLOCK_0 = 0,
  E_MIU_BLOCK_1,
  E_MIU_BLOCK_2,
  E_MIU_BLOCK_3,
  E_MIU_BLOCK_NUM,
} MIU_BLOCK_ID;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    HAL_MIU_SetIOMapBase(MS_VIRT virtBase);
void    HAL_MIU_SetPMIOMapBase(MS_VIRT virtBase);
MS_BOOL HAL_MIU_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable);
MS_U32  HAL_MIU_ProtectAlign(void);
MS_BOOL HAL_MIU_Dram_Size(MS_U8 MiuID, MS_U8 DramSize);
MS_BOOL HAL_MIU_Dram_ReadSize(MS_U8 MiuID, MIU_DDR_SIZE *pDramSize);
MS_U8 HAL_MIU_ClinetNumber(MS_U8 u8Blockx);
MS_BOOL HAL_MIU_Protect(MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_PHY phy64Start,
                        MS_PHY phy64End, MS_BOOL  bSetFlag);
MS_BOOL HAL_MIU_SetSsc(MS_U8 u8MiuDev, MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable);
MS_BOOL HAL_MIU_IsInitMiu1(void);
MS_BOOL HAL_MIU_SelMIU(eMIU_SelType eType, eMIUClientID eClientID);
MS_BOOL HAL_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo);
MS_BOOL HAL_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority);
MS_BOOL HAL_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask);

MS_U8* HAL_MIU_GetDefaultClientID_KernelProtect(void);

MS_U16  HAL_MIU_GET_MUX(void);
MS_U16  HAL_MIU_IsI64Mode(void);
MS_U16  HAL_MIU_GET_Group0(void);
MS_U16  HAL_MIU_GET_Group1(void);
MS_U16  HAL_MIU_GET_Group2(void);
MS_S16 HAL_MIU_GetClientInfo(MS_U8 u8MiuDev, eMIUClientID eClientID);

void HAL_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID u16ClientID);
void HAL_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID u16ClientID);
void HAL_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_VOP_SwitchMIU(MS_U8 u8MiuID);
void HAL_MIU_PrintMIUProtectInfo(void);

MS_BOOL HAL_MIU_EnableScramble(MS_BOOL bEnable);
MS_BOOL HAL_MIU_IsScrambleEnabled(void);
MS_BOOL HAL_MIU_SetLoadingRequest(MS_U8 u8MiuDev);
MS_BOOL HAL_MIU_GetLoadingRequest(MS_U8 u8MiuDev, MS_U32 *u32Loading);
MS_BOOL HAL_MIU_ParseOccupiedResource(void);
MS_BOOL HAL_MIU_GetClientWidth(MS_U8 u8MiuDevi, eMIUClientID eClientID, eMIU_ClientWidth *pClientWidth);

#endif // _HAL_MIU_H_

