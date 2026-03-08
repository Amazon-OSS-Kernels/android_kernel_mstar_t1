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

#ifndef _MSDBC_LIB_GROUP_DTV2_H_
#define _MSDBC_LIB_GROUP_DTV2_H_


#ifdef IOUTIL_C
#define INTERFACE
#else
#define    INTERFACE extern
#endif

#include "dlc_hwreg_utility2.h"
#include "drvDBC_Cus.h"


/////////////// Chakra2 define start

#define BOOL    MS_BOOL
#define BYTE    MS_U8
#define WORD    MS_U16
#define DWORD   MS_U32
#define XDATA
#define code

#define _BIT0           0x0001
#define _BIT1           0x0002
#define _BIT2           0x0004
#define _BIT3           0x0008
#define _BIT4           0x0010
#define _BIT5           0x0020
#define _BIT6           0x0040
#define _BIT7           0x0080

#define SC1_REG_BASE                BK_SCALER_BASE
#define REG_BANK_DLC                0x1A
#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)
#define L_BK_DLC(x)                 BK_REG_L(BK_SCALER_BASE,x)
#define H_BK_DLC(x)                 BK_REG_H(BK_SCALER_BASE,x)

//////////////// Chakra2 define end


/////#define BOOL    BOOLEAN

#define msReadByte(_reg_)                   MDrv_ReadByte(_reg_)
#define msWriteByte(_reg_, _val_)           MDrv_WriteByte(_reg_, _val_)
#define msWriteBit(_reg_, _val_, _pos_)     MDrv_WriteRegBit(_reg_, _val_, _pos_)

#define REG_NULL        0xFF // empty register




//////////////////////////////////////////////////////////////////////////
// Common Definition
//typedef struct _DBCRegUnitType
//{
//    WORD wReg;
//    BYTE ucValue;
//}DBCRegUnitType;


//////////////////////////////////////////////////////////////////////////
#define REG_ADDR_DBC_Y_GAIN              L_BK_DLC(0x14) //SC1A_28 (8bit)
#define REG_ADDR_DBC_C_GAIN              H_BK_DLC(0x14) //SC1A_29 (8bit)


#define msDBC_FunctionEnter()               BYTE  u8Bank; \
                                            u8Bank = msReadByte(SC1_REG_BASE); \
                                            msWriteByte(SC1_REG_BASE, REG_BANK_DLC);// for register bank switch...

#define msDBC_FunctionExit()                msWriteByte(SC1_REG_BASE, u8Bank)// for register bank switch...


//code DBCRegUnitType tDBC_Initialize[] =
//{
//    {_END_OF_TBL_, 0x00}
//};
#endif

