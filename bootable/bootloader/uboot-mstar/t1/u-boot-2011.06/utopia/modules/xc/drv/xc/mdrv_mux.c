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
//==============================================================================
// [Mdrv_mux.c]
// Date: 20090225
// Descriptions: Add a new mux layer for HW setting
//==============================================================================

#define  MDRV_MUX_C

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#include "MsCommon.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_mux.h"

void MDrv_XC_mux_set_sync_port_by_dataport(void *pInstance, E_MUX_INPUTPORT src_port )
{
    Hal_SC_set_sync_port_by_dataport(pInstance, src_port);
}

void MDrv_XC_mux_dispatch(void *pInstance, E_MUX_INPUTPORT src , E_MUX_OUTPUTPORT dest)
{
    Hal_SC_mux_dispatch(pInstance, src, dest);
}

void MDrv_XC_mux_turnoff_ymux(void *pInstance)
{
    Hal_SC_mux_set_adc_y_mux(pInstance, 0x0F);
}

void MDrv_XC_mux_turnoff_cmux(void *pInstance)
{
    Hal_SC_mux_set_adc_c_mux(pInstance, 0x0F);
}

void MDrv_SC_EnableCLK_for_DIP(void *pInstance, MS_BOOL bEnable)
{
    Hal_SC_EnableCLK_for_DIP(pInstance, bEnable);
}

E_MUX_INPUTPORT MDrv_XC_Mux_GetHDMIPort(void *pInstance, INPUT_SOURCE_TYPE_t src )
{
    // Get Inputport
    E_MUX_INPUTPORT enPorts[4] = {INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT};
    MS_U8 u8Port_count = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    if ( !IsSrcTypeHDMI(src) )
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return INPUT_PORT_NONE_PORT;
    }

    // Need refine.
    MDrv_XC_Mux_GetPort(pInstance, src , enPorts , &u8Port_count );
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return (enPorts[0]);
}

MS_U8 MDrv_XC_MUX_MapInputSourceToVDYMuxPORT(void *pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MS_U8 u8Checksrc = INPUT_PORT_NONE_PORT;
    E_MUX_INPUTPORT enPorts[3] ={INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT};
    MS_U8 u8Port_count = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MDrv_XC_Mux_GetPort(pInstance, u8InputSourceType, enPorts , &u8Port_count );
    if(IsSrcTypeAV(u8InputSourceType)
       || IsSrcTypeSV(u8InputSourceType)
       || IsSrcTypeYPbPr(u8InputSourceType)
       || IsSrcTypeHDMI(u8InputSourceType))
    {
        u8Checksrc = enPorts[0];
    }
    else if(IsSrcTypeVga(u8InputSourceType)
       || IsSrcTypeScart(u8InputSourceType))
    {
        u8Checksrc = enPorts[1];
    }
    else if(IsSrcTypeStorage(u8InputSourceType))
    {
        u8Checksrc = INPUT_PORT_MVOP;
    }
    else
    {
        //XC_LOG_TRACE(XC_DBGLEVEL_MUX, "no such kind of VD Y Mux Port for the input source type:%u\n",u8InputSourceType);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return u8Checksrc;
}

void MDrv_XC_mux_SetScalerIndexInformationToADC(void *pInstance, MS_U8 u8ScalerIndex, MS_U16 u16InputSourceType)
{
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    Hal_SC_mux_SetScalerIndexInformationToADC(pInstance,u8ScalerIndex,u16InputSourceType);
#else
    UNUSED(pInstance);
    UNUSED(u8ScalerIndex);
    UNUSED(u16InputSourceType);
#endif
}

#undef MDRV_MUX_C
