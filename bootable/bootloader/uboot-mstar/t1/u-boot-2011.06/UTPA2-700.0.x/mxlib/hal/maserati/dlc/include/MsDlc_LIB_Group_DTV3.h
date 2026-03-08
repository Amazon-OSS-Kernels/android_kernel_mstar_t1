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
#ifndef _MSDLC_LIB_GROUP_DTV2_H_
#define _MSDLC_LIB_GROUP_DTV2_H_


#ifdef IOUTIL_C
#define INTERFACE
#else
#define    INTERFACE extern
#endif

#include "drvDLC.h"
#include "dlc_hwreg_utility2.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvDLC_Cus.h"

/////////////// Chakra2 define start

#define BOOL    MS_BOOL
#define BYTE    MS_U8
#define WORD    MS_U16
#define DWORD   MS_U32
#define XDATA
#define code

#define MAIN_WINDOW     0
#define SUB_WINDOW      1

#define REG_SC_BK_VOP                           0x10
#define REG_SC_BK_ACE                           0x18
#define REG_SC_BK_DLC                           0x1A
#define REG_SC_BK_OP                            0x20
#define REG_IP_BK_HVSP                          0x23
#define REG_SC_BK_DLC3                          0x55
#define REG_IP_BK_DLC                           0x5C

#define L_BK_VOP(_x_)                           (REG_SCALER_BASE | (REG_SC_BK_VOP << 8) | (_x_ << 1))
#define L_BK_ACE(x)                             (REG_SCALER_BASE | (REG_SC_BK_ACE << 8) | (_x_ << 1))
#define L_BK_OP(_x_)                            (REG_SCALER_BASE | (REG_SC_BK_OP << 8) | (_x_ << 1))

#define L_BK_HVSP(_x_)                          (REG_SCALER_FSC_BASE | (REG_IP_BK_HVSP << 8) | (_x_ << 1))

#define L_BK_DLC(_x_)                           (REG_SCALER_FSC_BASE | (REG_SC_BK_DLC << 8) | (_x_ << 1))
#define H_BK_DLC(_x_)                           (REG_SCALER_FSC_BASE | (REG_SC_BK_DLC << 8) | ((_x_ << 1)+1))
#define L_BK_DLC3(_x_)                           (REG_SCALER_FSC_BASE | (REG_SC_BK_DLC3 << 8) | (_x_ << 1))
#define H_BK_DLC3(_x_)                           (REG_SCALER_FSC_BASE | (REG_SC_BK_DLC3 << 8) | ((_x_ << 1)+1))

#define L_BK_IP_DLC(_x_)                           (REG_SCALER_BASE | (REG_IP_BK_DLC << 8) | (_x_ << 1))
#define H_BK_IP_DLC(_x_)                           (REG_SCALER_BASE | (REG_IP_BK_DLC << 8) | ((_x_ << 1)+1))

//////////////// Chakra2 define end

#define _END_OF_TBL_    0xFFFF

#define _BIT0           0x0001
#define _BIT1           0x0002
#define _BIT2           0x0004
#define _BIT3           0x0008
#define _BIT4           0x0010
#define _BIT5           0x0020
#define _BIT6           0x0040
#define _BIT7           0x0080
#define _ENABLE             1
#define _DISABLE            0
#define BYTE            MS_U8
//////////////////////////////////////////////////////////////////////////
// Common Definition
typedef struct _DlcRegUnitType
{
    WORD wReg;
    BYTE ucValue;
}DlcRegUnitType;


//#define BOOL    BOOLEAN

#define msReadByte(_reg_)                   MDrv_ReadByte(_reg_)
#define msWriteByte(_reg_, _val_)           MDrv_WriteByte(_reg_, _val_)
#define msWriteBit(_reg_, _val_, _pos_)     MDrv_WriteRegBit(_reg_, _val_, _pos_)
#define msDlc_Delay1ms(x)                   MsOS_DelayTask(x)

#define REG_NULL        0xFF // empty register


//////////////////////////////////////////////////////////////////////////
#define REG_ADDR_HISTOGRAM_RANGE_M_VST      L_BK_DLC3(0x10)// DLC3_10[9:0]
#define REG_ADDR_HISTOGRAM_RANGE_M_VEN      L_BK_DLC3(0x11)// DLC3_11[9:0]
#define REG_ADDR_HISTOGRAM_RANGE_M_HST      L_BK_DLC3(0x12)// DLC3_12[8:0]
#define REG_ADDR_HISTOGRAM_RANGE_M_HEN      L_BK_DLC3(0x13)// DLC3_13[8:0]
#define REG_ADDR_HISTOGRAM_RANGE_M_HST_SEC  L_BK_DLC3(0x14)// DLC3_14[8:0]
#define REG_ADDR_HISTOGRAM_RANGE_M_HEN_SEC  L_BK_DLC3(0x15)// DLC3_15[8:0]

#define REG_ADDR_HISTOGRAM_TOTAL_COUNT_L        L_BK_DLC3(0x18)
#define REG_ADDR_HISTOGRAM_TOTAL_SUM_L          L_BK_DLC3(0x1A)
#define REG_ADDR_HISTOGRAM_DATA_32              L_BK_DLC3(0x30)

#define REG_ADDR_H_GUARD_BAND_OUTPUT_SIZE_MAIN   L_BK_HVSP(0x1D)

#define REG_ADDR_DLC_HANDSHAKE              L_BK_DLC(0x04)
#define REG_ADDR_HISTOGRAM_RANGE_ENABLE     L_BK_DLC(0x08)
#define REG_ADDR_HISTOGRAM_8_RANGE_START    L_BK_DLC(0x1C)
#define REG_ADDR_BLE_UPPER_BOND             L_BK_DLC(0x09)
#define REG_ADDR_BLE_LOWER_BOND             H_BK_DLC(0x09)
#define REG_ADDR_WLE_UPPER_BOND             L_BK_DLC(0x0A)
#define REG_ADDR_WLE_LOWER_BOND             H_BK_DLC(0x0A)
#define REG_ADDR_MAIN_MAX_VALUE             L_BK_DLC(0x0B)
#define REG_ADDR_MAIN_MIN_VALUE             H_BK_DLC(0x0B)
#define REG_ADDR_DLC_DATA_START_MAIN        L_BK_DLC(0x30)
#define REG_ADDR_DLC_DATA_EXTEND_N0_MAIN    L_BK_DLC(0x76)
#define REG_ADDR_DLC_DATA_EXTEND_16_MAIN    L_BK_DLC(0x77)
#define REG_ADDR_DLC_DATA_LSB_START_MAIN    L_BK_DLC(0x78)
#define REG_ADDR_DLC_Y_GAIN                 L_BK_DLC(0x14)
#define REG_ADDR_DLC_C_GAIN                 H_BK_DLC(0x14)
#define REG_ADDR_VOP_OUTPUT_H_TOTAL         L_BK_VOP(0x0C)
#define REG_ADDR_VOP_SCREEN_CONTROL         L_BK_VOP(0x19)
#define REG_ADDR_VIP_HISTOGRAM_4K2K_MODE    L_BK_ACE(0x75)


#define msDlc_FunctionEnter()

#define msDlc_FunctionExit()

#define msVop_FunctionEnter()

#define msVop_FunctionExit()


#ifdef _MSDLC_C_
code DlcRegUnitType tDLC_Initialize[] =
{
    {_END_OF_TBL_, 0x00}
};
#else
extern code DlcRegUnitType tDLC_Initialize[];
#endif

//////////////////////////////////////////////////////////////////////////
// Align to msdlc library
#ifdef _MSDLC_C_
    typedef void   (*MApi_XC_DLC_Print_Callback)(MS_U8 /*PWM_VALUE*/);

    extern  MS_U16                       gu16PNL_Width;
    extern  MS_U16                       gu16PNL_Height;
    extern  MApi_XC_DLC_Print_Callback   gfnPutchar;
    extern  StuDbc_CAPTURE_Range         g_DlcCapRange;

    #ifdef  putchar
    #undef  putchar
    #endif

    #define putchar(x)                   (gfnPutchar(x))
    #define msDlc_AP_ReinitWithoutPara() msDlcInitWithCurve( g_DlcCapRange.wHStart, g_DlcCapRange.wHEnd, gu16PNL_Height*1/8, gu16PNL_Height*7/8)//g_DlcCapRange.wVStart, g_DlcCapRange.wVEnd )
    #define Delay1ms(x)                  (MsOS_DelayTask(x))
#endif //_MSDLC_C_

#endif

