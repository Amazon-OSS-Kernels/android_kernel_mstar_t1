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
#define MHAL_DLC_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "dlc_hwreg_utility2.h"
#include "mhal_dlc.h"
#include "hwreg_dlc.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAIN_WINDOW     0
#define SUB_WINDOW      1


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

MS_VIRT _DLC_RIU_BASE;

MS_U8 Hal_DLC_init(void)
{
    return TRUE;
}

// Put this function here because hwreg_utility2 only for hal.
void Hal_DLC_init_riu_base( MS_VIRT ptr_riu_base)
{
    _DLC_RIU_BASE = ptr_riu_base;
}

MS_BOOL Hal_DLC_GetLumaCurveStatus(void)
{
    MS_BOOL bStatus;

    halDlc_FuncEnter();

    bStatus = ( SC_R2BYTEMSK(REG_SC_BK1A_04_L, BIT(7) ) )? TRUE:FALSE;

    halDlc_FuncExit();

    return  bStatus;
}

void Hal_DLC_CGC_ResetCGain(void)
{
    halDlc_FuncEnter();

    SC_W2BYTEMSK(REG_SC_BK1A_14_L, (0xFF) <<8, HBMASK);

    halDlc_FuncExit();
}

MS_U8 Hal_DLC_CGC_GetCGain(void)
{
    MS_U8 u8Data;

    halDlc_FuncEnter();

    u8Data = ((SC_R2BYTEMSK(REG_SC_BK1A_14_L, HBMASK)) >>8) & 0xFF;

    halDlc_FuncExit();

    return u8Data;
}

void Hal_DLC_CGC_SetCGain(MS_U8 u8CGain)
{
    halDlc_FuncEnter();

    SC_W2BYTEMSK(REG_SC_BK1A_14_L, (u8CGain) <<8, HBMASK);

    halDlc_FuncExit();
}

void Hal_DLC_CGC_ResetYGain(void)
{
    halDlc_FuncEnter();

    SC_W2BYTEMSK(REG_SC_BK1A_14_L, 0xFF, LBMASK);

    halDlc_FuncExit();
}

MS_U8 Hal_DLC_CGC_GetYGain(void)
{
    MS_U8 u8Data;

    halDlc_FuncEnter();

    u8Data = (SC_R2BYTEMSK(REG_SC_BK1A_14_L, LBMASK)) & 0xFF;

    halDlc_FuncExit();

    return u8Data;
}

void Hal_DLC_CGC_SetYGain(MS_U8 u8YGain)
{
    halDlc_FuncEnter();

    SC_W2BYTEMSK(REG_SC_BK1A_14_L, u8YGain, LBMASK);

    halDlc_FuncExit();
}

#if(ENABLE_AVERAGELUMA)

MS_U8 Hal_DLC_get_dlc_method(void)
{
    MS_U8 u8Data;

    halDlc_FuncEnter();

    u8Data = (SC_R2BYTEMSK(REG_SC_BK1A_20_L, 0x00FF)) & 0xFF;

    halDlc_FuncExit();

    return u8Data;
}

MS_U16 Hal_DLC_get_averageluma_delta(void)
{
    MS_U16 u16Data;

    halDlc_FuncEnter();

    u16Data = SC_R2BYTE(REG_SC_BK1A_24_L);

    halDlc_FuncExit();

    return u16Data;
}


MS_U16 Hal_DLC_get_averageluma_boundary_low(void)
{
    MS_U16 u16Data;

    halDlc_FuncEnter();

    u16Data = SC_R2BYTE(REG_SC_BK1A_26_L) & 0x00FF;

    halDlc_FuncExit();

    return u16Data;
}

MS_U16 Hal_DLC_get_averageluma_boundary_midlow(void)
{
    MS_U16 u16Data;

    halDlc_FuncEnter();

    u16Data = ((SC_R2BYTE(REG_SC_BK1A_26_L) & 0xFF00)>>8);

    halDlc_FuncExit();

    return u16Data;
}


MS_U16 Hal_DLC_get_averageluma_boundary_midhigh(void)
{
    MS_U16 u16Data;

    halDlc_FuncEnter();

    u16Data = ((SC_R2BYTE(REG_SC_BK1A_27_L) & 0x00FF));

    halDlc_FuncExit();

    return u16Data;
}

MS_U16 Hal_DLC_get_averageluma_boundary_high(void)
{
    MS_U16 u16Data;

    halDlc_FuncEnter();

    u16Data = ((SC_R2BYTE(REG_SC_BK1A_27_L) & 0xFF00)>>8);

    halDlc_FuncExit();

    return u16Data;
}

MS_BOOL Hal_DLC_get_c_gain_ctrl(void)
{
    MS_BOOL bStatus;

    halDlc_FuncEnter();

    if (SC_R2BYTE(REG_SC_BK1A_20_L) & BIT(6))
        bStatus = TRUE;
    else
        bStatus = FALSE;

    halDlc_FuncExit();

    return bStatus;
}

MS_U16 Hal_DLC_get_step(void)
{
    MS_U16 u16Data;

    halDlc_FuncEnter();

    u16Data = ((SC_R2BYTE(REG_SC_BK1A_23_L) & 0xFF00)>>8);

    halDlc_FuncExit();

    return u16Data;
}

MS_U16 Hal_DLC_get_flick_alpha(void)
{
    MS_U16 u16Data;

    halDlc_FuncEnter();

    u16Data = (SC_R2BYTE(REG_SC_BK1A_23_L) & 0x00FF);

    halDlc_FuncExit();

    return u16Data;
}

MS_U16 Hal_DLC_get_c_gain_strength(void)
{
    MS_U16 u16Data;

    halDlc_FuncEnter();

    u16Data = (SC_R2BYTE(REG_SC_BK1A_25_L) & 0x00FF);

    halDlc_FuncExit();

    return u16Data;
}

void Hal_DLC_set_curve(MS_U8 *pCurve)
{
    MS_U8 i;
    MS_U16 u16Value;

    halDlc_FuncEnter();

    for(i=0; i<16; i+=2)
    {
        u16Value = (((MS_U16)(pCurve[i+1]) << 8) | (MS_U16)pCurve[i]);
        SC_W2BYTE(REG_SC_BK1A_30_L + i, u16Value);
    }

    halDlc_FuncExit();
}

void Hal_DLC_set_curveExt(MS_U16 *pCurve)
{
    MS_U8 i;
    MS_U16 u16Value;
    MS_U8 u8ForceSyncSubWin;

    halDlc_FuncEnter();

    u8ForceSyncSubWin = SC_R2BYTE(REG_SC_BK1A_20_L) & BIT(7); //@@@@, sub

    for(i=0; i<16; i+=2)
    {
        u16Value = ((((pCurve[i+1]&0x3FC)>>2) << 8) | ((pCurve[i] & 0x3FC)>>2));
        SC_W2BYTE(REG_SC_BK1A_30_L+i, u16Value);
        if (u8ForceSyncSubWin) //@@@@, sub
            SC_W2BYTE(REG_SC_BK1A_38_L+i, u16Value);
    }

    u16Value = ((pCurve[0] & 0x0003) << 2)  |
               ((pCurve[1] & 0x0003) << 4)  |
               ((pCurve[2] & 0x0003) << 6)  |
               ((pCurve[3] & 0x0003) << 8)  |
               ((pCurve[4] & 0x0003) << 10) |
               ((pCurve[5] & 0x0003) << 12) |
               ((pCurve[6] & 0x0003) << 14);
    SC_W2BYTE(REG_SC_BK1A_78_L, u16Value);
    if (u8ForceSyncSubWin) //@@@@, sub
        SC_W2BYTE(REG_SC_BK1A_7B_L, u16Value);

    u16Value = ((pCurve[7] & 0x0003) << 0)  |
               ((pCurve[8] & 0x0003) << 2)  |
               ((pCurve[9] & 0x0003) << 4)  |
               ((pCurve[10] & 0x0003) << 6)  |
               ((pCurve[11] & 0x0003) << 8)  |
               ((pCurve[12] & 0x0003) << 10) |
               ((pCurve[13] & 0x0003) << 12) |
               ((pCurve[14] & 0x0003) << 14);
    SC_W2BYTE(REG_SC_BK1A_79_L, u16Value);
    if (u8ForceSyncSubWin) //@@@@, sub
        SC_W2BYTE(REG_SC_BK1A_7C_L, u16Value);

    u16Value = ((pCurve[15] & 0x0003) << 0)  |
               ((pCurve[16] & 0x0003) << 2);
    SC_W2BYTE(REG_SC_BK1A_7A_L, u16Value);
    if (u8ForceSyncSubWin) //@@@@, sub
        SC_W2BYTE(REG_SC_BK1A_7D_L, u16Value);

    halDlc_FuncExit();
}

MS_BOOL Hal_DLC_get_histogram_ack(void)
{
    MS_BOOL bret = FALSE;

    halDlc_FuncEnter();

    if (SC_R2BYTE(REG_SC_BK1A_04_L) & 0x0008)
    {
        bret = TRUE;
    }

    halDlc_FuncExit();
    return bret;
}

void Hal_DLC_set_histogram_req(void)
{
    halDlc_FuncEnter();

    SC_W2BYTEMSK(REG_SC_BK1A_04_L, 0x0000, 0x0006);
    SC_W2BYTEMSK(REG_SC_BK1A_04_L, 0x0006, 0x0006);

    halDlc_FuncExit();
}

void Hal_DLC_get_histogram32(MS_U16 *pTable)
{
    MS_U8 i;
    halDlc_FuncEnter();

    for(i=0; i<32; i++)
    {
        pTable[i] = SC_R2BYTE(REG_SC_BK1A_40_L+ (MS_U32)(i*2));
    }

    halDlc_FuncExit();
}

void Hal_DLC_set_curve_sub(MS_U8 *pCurve)
{
    MS_U8 i;
    MS_U16 u16Value;

    halDlc_FuncEnter();

    for(i=0; i<16; i+=2)
    {
        u16Value = (((MS_U16)(pCurve[i+1]) << 8) | (MS_U16)pCurve[i]);
        SC_W2BYTE(REG_SC_BK1A_38_L + i, u16Value);
    }

    halDlc_FuncExit();
}

#endif

MS_U16 Hal_DLC_get_histogram_y_sum(void)
{
    MS_U16 u16val;

    halDlc_FuncEnter();

    u16val = SC_R2BYTE(REG_SC_BK1A_06_L);

    halDlc_FuncExit();
    return u16val;
}

MS_U16 Hal_DLC_get_histogam_pixel_sum(void)
{
    MS_U16 u16val;
    halDlc_FuncEnter();

    u16val =  SC_R2BYTE(REG_SC_BK1A_07_L);

    halDlc_FuncExit();
    return u16val;
}

void Hal_DLC_set_onoff(MS_BOOL bWindow, MS_BOOL ben)
{
    halDlc_FuncEnter();

    if(bWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(REG_SC_BK1A_04_L, ben ? 0x80 : 0x00 , 0x0080);
    }
    else
    {
        SC_W2BYTEMSK(REG_SC_BK1A_04_L, ben ? 0x40 : 0x00 , 0x0040);
    }

    halDlc_FuncExit();
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#undef MHAL_DLC_C

