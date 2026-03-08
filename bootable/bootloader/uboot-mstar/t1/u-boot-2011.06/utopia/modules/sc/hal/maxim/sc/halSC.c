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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvSC.c
/// @brief  SmartCard Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvSC.h"

// Internal Definition
#include "regSC.h"
#include "halSC.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define SC_DBG_ENABLE               2UL
#define SC_DEV_NUM_MAX				2UL
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if SC_DBG_ENABLE
#define SC_DBG(fmt, args...)        printf("[DEV][SMART][%06d] " fmt, __LINE__, ##args)
#define SC_ERR(fmt, args...)        printf("[DEV][SMART][%06d] " fmt, __LINE__, ##args)
#else
#define SC_DBG(fmt, args...)        {}
#define SC_ERR(fmt, args...)        printf("[DEV][SMART][%06d] " fmt, __LINE__, ##args)
#endif

// Setting
//#define SC_EVENT()                  MsOS_SetEvent(s32CardEventId, SC_EVENT_CARD)
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

MS_VIRT                              _regSCBase[SC_DEV_NUM_MAX];
MS_VIRT                              _regSCHWBase = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void HAL_SC_RegMap(MS_VIRT u32RegBase)
{
    _regSCBase[0] = u32RegBase;
}

void HAL_SC_HW_RegMap(MS_VIRT u32RegBase)
{
    _regSCHWBase = u32RegBase;
}

void HAL_SC_SetClk(MS_U8 u8SCID,MS_U16 u16clk)
{
    MS_U16              reg;

    reg = HW_READ(REG_TOP_CKG_SM_CA);
    if (u8SCID == 0)
    {
        reg &= (~TOP_CKG_SM_CA0_CLK_MASK);
        switch (u16clk)
        {
            case E_SC_CLK_3M :
                reg |= TOP_CKG_SM_CA0_CLK_27M_D8;
                break;
            case E_SC_CLK_4P5M :
                reg |= TOP_CKG_SM_CA0_CLK_27M_D6;
                break;
            case E_SC_CLK_6M :
                reg |= TOP_CKG_SM_CA0_CLK_27M_D4;
                break;
            case E_SC_CLK_13M :
                reg |= TOP_CKG_SM_CA0_CLK_27M_D2;
                break;
            case E_SC_CLK_4M :
                HW_WRITE(REG_TOP_CKG_SM_CA0_N, 0x6c);
                HW_WRITE(REG_TOP_CKG_SM_CA0_M, HW_READ(REG_TOP_CKG_SM_CA0_M) & (~TOP_CKG_SM_CA0_M_MASK));
                HW_WRITE(REG_TOP_CKG_SM_CA0_M, HW_READ(REG_TOP_CKG_SM_CA0_M) | 0x0100);

                HW_WRITE(REG_TOP_CKG_SM_CA0_EXT, HW_READ(REG_TOP_CKG_SM_CA0_EXT) & (~TOP_CKG_SM_CA0_EXT_MASK));
                HW_WRITE(REG_TOP_CKG_SM_CA0_EXT, HW_READ(REG_TOP_CKG_SM_CA0_EXT) | TOP_CKG_SM_CA0_LOAD);
                HW_WRITE(REG_TOP_CKG_SM_CA0_EXT, HW_READ(REG_TOP_CKG_SM_CA0_EXT) & (~TOP_CKG_SM_CA0_LOAD));

                HW_WRITE(REG_TOP_CKG_SM_CA0_EXT, HW_READ(REG_TOP_CKG_SM_CA0_EXT) | TOP_CKG_SW_SM_CA0_CLK);
                break;
            default:
                SC_ERR("unknown SC_Set_Clk\n");
            break;
        }
    }
    else if (u8SCID == 1)
    {
        SC_ERR("[SC] not support SM1\n");
    }
    HW_WRITE(REG_TOP_CKG_SM_CA, reg);
}

void HAL_SC_Init(MS_U8 u8SCID)
{
    //reg_allpad_in
    printf("HAL_SC_Init In\n");
    HW_WRITE(REG_TOP_ALLPAD_IN, HW_READ(REG_TOP_ALLPAD_IN) & ~TOP_ALLPAD_IN_EN);

    HAL_SC_PowerCtrl(u8SCID, TRUE);

    // Let the related I/O Low
    SC_WRITE(u8SCID,UART_LCR, UART_LCR_SBC); //I/O
    // mask tx level int and set tx level to 31->32 trigger int mode.
    // mask BGWT and CGWT at initialization
    SC_WRITE(u8SCID, UART_CTRL2, SC_READ(u8SCID, UART_CTRL2)|0x3E);    
    // BGT default value will be 0x16, reset to 0 here to disable the BGT int
    SC_WRITE(u8SCID, UART_SCBGT, 0x00)
    if (u8SCID == 0)
    {
    	printf("Do reg_sm0_open\n");
        //reg_sm0_open
        //HW_WRITE(REG_TOP_TS1_PE, HW_READ(REG_TOP_TS1_PE)|BIT(3)|BIT(6)); //for Edison new pad
        //HW_WRITE(REG_TOP_SM0,HW_READ(REG_TOP_SM0)|0x20);
        //HW_WRITE(REG_TOP_TEST_MODE,HW_READ(REG_TOP_TEST_MODE)&(~(BIT(2)|BIT(1)|BIT(0))));
        //HW_WRITE(REG_TOP_CONFIG,HW_READ(REG_TOP_CONFIG)&(~(BIT(13)|BIT(12)|BIT(11))));

    }
    if (u8SCID ==1)
    {
        SC_ERR("[SC] not support SM1\n");
    }
}

void HAL_SC_Exit(MS_U8 u8SCID)
{
    HAL_SC_PowerCtrl(u8SCID, FALSE);
}

void HAL_SC_PowerCtrl(MS_U8 u8SCID, MS_BOOL bEnable)
{   
    if (u8SCID == 0)
    {
        if (bEnable)
        {
            // SM0: enable switch to specified clk
            HW_WRITE(REG_TOP_CKG_UART_CLK, (HW_READ(REG_TOP_CKG_UART_CLK) & ~(TOP_CKG_UART1_CLK_DIS | TOP_CKG_UART1_CLK_MASK)) | TOP_CKG_UART1_CLK_144M);
            HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS); //how
        }
        else
        {
            // SM0: disable clk
            HW_WRITE(REG_TOP_CKG_UART_CLK, (HW_READ(REG_TOP_CKG_UART_CLK) | TOP_CKG_UART1_CLK_DIS));//how
            HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA0_DIS)); //how
        }
    }
    if (u8SCID ==1)
    {
        SC_ERR("[SC] not support SM1\n");
    }
}

void HAL_SC_CardInvert(MS_U8 u8SCID, SC_CardDetType eTpye)
{
    if (eTpye == E_SC_LOW_ACTIVE)
    {
        SC_OR(u8SCID,UART_CTRL3, UART_INVERT_CD);
    } 
    else 
    {
        SC_AND(u8SCID,UART_CTRL3, ~UART_INVERT_CD);
    }
}

MS_BOOL HAL_SC_CardVoltage_Config(MS_U8 u8SCID, SC_VoltageCtrl eVoltage)
{
    if (E_SC_VOLTAGE_MAX <= eVoltage)
        return FALSE;
    if (E_SC_VOLTAGE_3_POINT_3V == eVoltage)
    {
        HW_WRITE(REG_TOP_3V_5V_SELECT, (HW_READ(REG_TOP_3V_5V_SELECT) & ~(0x1)));
    }
    else if (E_SC_VOLTAGE_5V == eVoltage)
    {
        HW_WRITE(REG_TOP_3V_5V_SELECT, (HW_READ(REG_TOP_3V_5V_SELECT) | (0x1)));
    }
    else
        return FALSE;
    return TRUE;
}

