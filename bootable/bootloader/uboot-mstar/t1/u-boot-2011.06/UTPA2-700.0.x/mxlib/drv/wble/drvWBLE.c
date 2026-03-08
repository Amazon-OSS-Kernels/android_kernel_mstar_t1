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
#define  _DRV_WBLE_C_


#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"

#include "MsOS.h"
#include "MsTypes.h"
#include "wble_hwreg_utility2.h"

#include "drvMMIO.h"
#include "drvWBLE.h"
#include "mhal_wble.h"

/******************************************************************************/
/*                           Constant                                         */
/******************************************************************************/
#define BLE_DEFLICK_BLEND_FACTOR        32UL
#define BLE_DEFLICK_PRECISION_SHIFT     64UL


#define BLE_METHOD_FIX_START          1   // Fixed starting point, variable slop
#define BLE_METHOD_FIX_SLOP           2   // Variable staring point, fixed slop
#define BLE_METHOD_DYNAMIC_START_SLOP 3   // Variable staring point, variable slop

#define BLE_SLOP_PARAM  0
#define BLE_START_PARAM 1


/******************************************************************************/
/*                           Local                                            */
/******************************************************************************/

static MS_U8 g_u8BLE_OriSlop = 0x80;
static MS_U8 g_u8WLE_OriSlop = 0x80;

static MS_U16 u16PreHistogramTotal;
static MS_U16 u16PreBLESlope;
static MS_U16 u16PreBLEStart;
static MS_U8  u8SlopeDefalut;

static MSIF_Version _api_xc_wble_version = {
    .DDI = { WBLE_API_VERSION },
};

//-------------------------------------------------------------------------------------------------
/// Get XC library version
/// @param  ppVersion                  \b OUT: store the version in this member
/// @return @ref E_APIXC_ReturnValue
//-------------------------------------------------------------------------------------------------
E_DRVWBLE_ReturnValue MDrv_WBLE_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_DRVWBLE_RET_FAIL;
    }

    *ppVersion = &_api_xc_wble_version;
    return E_DRVWBLE_RET_OK;
}


void MDrv_WBLE_Init(void)
{
    u16PreHistogramTotal = 0;
    u16PreBLESlope = Hal_WBLE_get_black_slop() * BLE_DEFLICK_PRECISION_SHIFT;
    u16PreBLEStart = Hal_WBLE_get_black_start() * BLE_DEFLICK_PRECISION_SHIFT;
}

void MDrv_WBLE_EnableBLE(MS_BOOL bEnable)
{
   Hal_WBLE_set_ble(bEnable);
}


void MDrv_WBLE_EnableWLE(MS_BOOL bEnable)
{
   Hal_WBLE_set_wle(bEnable);
}


void MDrv_WBLE_SetBLE(MS_U8 u8Start, MS_U8 u8Slop)
{
    Hal_WBLE_set_ble_start(u8Start);
    Hal_WBLE_set_ble_slop(u8Slop);
    g_u8BLE_OriSlop = u8Slop;
}

void MDrv_WBLE_SetWLE(MS_U8 u8Start, MS_U8 u8Slop)
{
    Hal_WBLE_set_wle_start(u8Start);
    Hal_WBLE_set_wle_slop(u8Slop);
    g_u8WLE_OriSlop = u8Slop;
}


void MDrv_WBLE_Handler_BLEAvgLuma(MS_U8 u8AvgY, MS_U16 u16YTotalSum)
{
    MS_U8 u8Reg;
    MS_BOOL  bEnableBLE;
    MS_U8 u8Method, u8Strength, u8NewTarget;
    MS_U8 u8FlickAlpha;
    MS_U8 u8Step;
    MS_U16 u16Blend;

    MS_U8 u8Max[2], u8Min[2];
    MS_U16 u16PreTarget[2], u16TmpTarget[2];
    MS_U8 u8Start, u8End;
    MS_U8 i;
    MS_U16 u16Tmp;

    // control register
    u8Reg = Hal_WBLE_get_sw_ble_method();

    bEnableBLE = u8Reg & BIT(0);
    switch(u8Reg & (BIT(1)|BIT(2)))
    {
    case 0x00:
        u8Method = BLE_METHOD_FIX_START;
        u8Start = BLE_SLOP_PARAM;
        u8End   = BLE_SLOP_PARAM;
        break;
    case BIT(1):
        u8Method = BLE_METHOD_FIX_SLOP;
        u8Start = BLE_START_PARAM;
        u8End   = BLE_START_PARAM;
        Hal_WBLE_set_ble_slop(u8SlopeDefalut);
        break;

    default:
    case BIT(2):
        u8Method = BLE_METHOD_DYNAMIC_START_SLOP;
        u8Start = BLE_SLOP_PARAM;
        u8End   = BLE_START_PARAM;
        break;
    }

    if(bEnableBLE)
    {
        u8Strength = Hal_WBLE_get_ble_stregth();
        u8Step = Hal_WBLE_get_ble_step();

        u16Tmp = Hal_WBLE_get_ble_slop_boundary();
        u8Max[BLE_SLOP_PARAM]  = (MS_U8)((u16Tmp & 0xFF00)>>8);
        u8Min[BLE_SLOP_PARAM]  = (MS_U8)(u16Tmp & 0x00FF);

        u16Tmp = Hal_WBLE_get_ble_start_boundary();
        u8Max[BLE_START_PARAM] = (MS_U8)((u16Tmp & 0xFF00)>>8);
        u8Min[BLE_START_PARAM] = (MS_U8)(u16Tmp & 0x00FF);

        if(u8Method & BLE_METHOD_FIX_SLOP)
        { // fix slope
            MS_U8 u8Tmp;
            u8Tmp = (u8AvgY > 0x10) ? (u8AvgY - 0x10) : 0x00;
            u16TmpTarget[BLE_START_PARAM]= (MS_U32)u8Tmp * (MS_U32)u8Strength / 128 + 0x00;
            u16PreTarget[BLE_START_PARAM] = u16PreBLEStart;
        }

        if(u8Method & BLE_METHOD_FIX_START)
        { // fix start
            u16TmpTarget[BLE_SLOP_PARAM] = (MS_U32)u8AvgY * (MS_U32)u8Strength / 128 + 0x70;
            u16PreTarget[BLE_SLOP_PARAM] = u16PreBLESlope;
        }


        // de-flicker
        if (u16PreHistogramTotal >= u16YTotalSum)
        {
            if ((u16PreHistogramTotal - u16YTotalSum) < Hal_WBLE_get_ble_delta())
                u8FlickAlpha = Hal_WBLE_get_ble_flickalpha();
            else
                u8FlickAlpha = BLE_DEFLICK_BLEND_FACTOR;
        }
        else
        {
            if ((u16YTotalSum - u16PreHistogramTotal) < Hal_WBLE_get_ble_delta())
                u8FlickAlpha = Hal_WBLE_get_ble_flickalpha();
            else
                u8FlickAlpha = BLE_DEFLICK_BLEND_FACTOR;
        }

        u16PreHistogramTotal = u16YTotalSum;

        for(i=u8Start; i<=u8End; i++)
        {
            if(u16TmpTarget[i] > (MS_U16)u8Max[i])
                u8NewTarget = u8Max[i];
            else if(u16TmpTarget[i] < (MS_U16)u8Min[i])
                u8NewTarget = u8Min[i];
            else
                u8NewTarget = u16TmpTarget[i];

            u16Blend =(MS_U16)( (((MS_U32)u8FlickAlpha * (MS_U32)u8NewTarget * (MS_U32)BLE_DEFLICK_PRECISION_SHIFT) +
                              (((MS_U32)BLE_DEFLICK_BLEND_FACTOR-(MS_U32)u8FlickAlpha) * (MS_U32)u16PreTarget[i])) / BLE_DEFLICK_BLEND_FACTOR);

            // the difference can't be too big
            if(u16Blend > u16PreTarget[i])
            {
                if((u16Blend - u16PreTarget[i])> (BLE_DEFLICK_PRECISION_SHIFT * (MS_U16)u8Step))
                    u16Blend = u16PreTarget[i] + (BLE_DEFLICK_PRECISION_SHIFT * (MS_U16)u8Step);
            }
            else if(u16Blend < u16PreTarget[i])
            {
                if((u16PreTarget[i] - u16Blend)> (BLE_DEFLICK_PRECISION_SHIFT * (MS_U16)u8Step))
                    u16Blend = u16PreTarget[i] - (BLE_DEFLICK_PRECISION_SHIFT * (MS_U16)u8Step);
            }

            if(i == BLE_SLOP_PARAM)
            {
                u16PreBLESlope = u16Blend;
            }
            else
            {
                u16PreBLEStart = u16Blend;
            }


            u8NewTarget = (MS_U8)(u16Blend / BLE_DEFLICK_PRECISION_SHIFT);

            if(i == BLE_SLOP_PARAM)
            {
                Hal_WBLE_set_ble_slop(u8NewTarget);
            }
            if(i == BLE_START_PARAM)
            {
               Hal_WBLE_set_ble_start(u8NewTarget);
            }
        }
    }
    else
    {
        Hal_WBLE_set_ble_slop(0x80);
        Hal_WBLE_set_ble_start(0x70);
    }
}


void MDrv_WBLE_Set_SlopeValue(MS_U8 u8Slope)
{
    u8SlopeDefalut = u8Slope;

}

