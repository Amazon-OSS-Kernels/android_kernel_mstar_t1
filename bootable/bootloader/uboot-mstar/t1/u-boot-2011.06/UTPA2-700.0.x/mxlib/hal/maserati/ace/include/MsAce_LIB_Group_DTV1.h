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



//==============================================================================
/// [MsAce_LIB_Group_DTV1.h]
/// Date: 20090414
/// Descriptions:
//==============================================================================


#ifndef MSACE_LIB_GROUP_DTV1_H
#define MSACE_LIB_GROUP_DTV1_H

#include "ace_hwreg_utility2.h"
#include "hwreg_ace.h"

/////////////// Chakra2 define start

#define BOOL    MS_BOOL
#define BYTE    MS_U8
#define WORD    MS_U16
#define DWORD   MS_U32
#define U8      MS_U8
#define code


#define BK_SELECT_00                BK_SCALER_BASE
#define REG_BANK_VOP                0x10
#define REG_BANK_S_VOP              0x0F
#define REG_BANK_ACE                0x18

#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define L_BK_VOP(x)                 BK_REG_L(BK_SCALER_BASE,x)
#define L_BK_ACE(x)                 BK_REG_L(BK_SCALER_BASE,x)

//////////////// Chakra2 define end


#define XDATA

#define UNAVAILABLE             0xFF

#define REG_ADDR_SC_BANK_SEL    BK_SELECT_00
#define BANK_CM_MAIN            REG_BANK_VOP
#define BANK_CM_SUB             REG_BANK_S_VOP
#define BANK_ACE                REG_BANK_ACE
#define REG_ADDR_CM_MAIN        L_BK_VOP(0x26)
#define REG_ADDR_CM_SUB         L_BK_VOP(0x1D)
#define REG_ADDR_CM_CTL_MAIN    L_BK_VOP(0x2F)
#define REG_ADDR_CM_CTL_SUB     L_BK_VOP(0x26)
#define REG_ADDR_ACE_R2Y_EQ_SEL L_BK_ACE(0x6F)
#define ENABLE_CM_CTL1          (0x35)
#define ENABLE_CM_CTL2          (0x30)
#define MASK_CM_CTL             (0x37)
#define MASK_CM_CTL_EN          (0x10)
#define MASK_CM_CTL_RB_RANGE    (0x05)

#define DECLARA_CMCTL_VARIABLE              U8 CTLVal;
#if SCALER_REGISTER_SPREAD
#define DECLARA_BANK_VARIABLE               MS_U32 u32Bank = 0;
#define BACKUP_SC_BANK                      u32Bank = u32Bank;
#define RECOVER_SC_BANK
#define ACE_WRITE_CM(id, addr, data)	        SC_W2BYTE(id, ((DWORD)addr | (u32Bank << 8)),data)
#define ACE_WRITE_CM_CTL(id, addr, data, mask)	SC_W2BYTEMSK(id, ((DWORD)addr | (u32Bank << 8)),data,mask)
#define ACE_READ_CTL(id, addr, mask)			SC_R2BYTEMSK(id, ((DWORD)addr | (u32Bank << 8)),mask)
#else
#define DECLARA_BANK_VARIABLE               U8 u8Bank;
#define BACKUP_SC_BANK                      {u8Bank = GET_SC_BANK();}
#define RECOVER_SC_BANK                     {SET_SC_BANK(u8Bank);}
#define ACE_WRITE_CM(id, addr, data)		    MDrv_Write2Byte(L_BK_VOP((addr)/2), (data))
#define ACE_WRITE_CM_CTL(id, addr, data, mask)	MDrv_WriteByteMask(addr, data, mask)
#define ACE_READ_CTL(id, addr, mask)			MDrv_ReadRegBit(addr, mask)
#endif

// For scaler bank switch
#if SCALER_REGISTER_SPREAD
#define GET_SC_BANK()
#define SET_SC_BANK(bank)       u32Bank = bank;
#else
#define GET_SC_BANK()           MDrv_ReadByte(REG_ADDR_SC_BANK_SEL)
#define SET_SC_BANK(bank)       MDrv_WriteByte(REG_ADDR_SC_BANK_SEL, bank)
#endif
// Control sin&cos function
#define USE_ACE_INTERNAL_SIN_COS	0
#define sin     clampsin
#define cos     clampcos

#endif
