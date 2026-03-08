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
/// file    apiXC_VMark.c
/// @brief  Scaler API layer Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_XC_VMark_C_
#define _API_XC_VMark_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif

// XC related include files
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
// Driver
#include "drvXC_IOPort.h"
#include "apiXC.h"
#ifdef UFO_XC_VMARK

#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#include "mhal_xc_chip_config.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#include "apiXC_VMark_v2.h"
#include "xc_hwreg_utility2.h"
// Internal Definition

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
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

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


/********************************************************************************/
/*                   Local Function Prototypes                  */
/********************************************************************************/
/********************************************************************************/
/*                   Functions                      */
/********************************************************************************/

E_APIXC_ReturnValue MApi_XC_VMark_SetParameters_U2(void* pInstance, MS_U8 u8ServiceIndex, XC_VMARK_PARAMETER *pstXC_VMarkParameters)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
     eReturn = MHal_XC_VMark_SetParameters(pInstance, u8ServiceIndex, pstXC_VMarkParameters);
    return eReturn;
}
#ifndef UTOPIAXP_REMOVE_WRAPPER
E_APIXC_ReturnValue MApi_XC_VMark_SetParameters(MS_U8 u8ServiceIndex, XC_VMARK_PARAMETER *pstXC_VMarkParameters)
{
#ifdef MSOS_TYPE_OPTEE
    if(MDrv_XC_SetIOMapBase_i(NULL) == FALSE)
    {
        printf("Get MDrv_XC_SetIOMapBase_i failed\n");
        return E_APIXC_RET_FAIL;
    }
    MApi_XC_VMark_SetParameters_U2(NULL,u8ServiceIndex,pstXC_VMarkParameters);
    return E_APIXC_RET_OK;
#else
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_VMARK_SET_PARAMETERS XCArgs;
    XCArgs.u8ServiceIndex = u8ServiceIndex;
    XCArgs.pstXC_VMarkParameters = pstXC_VMarkParameters;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_VMARK_CMD_SET_VMARK_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
#endif
}
#endif
#endif

#undef _API_XC_VMARK_C_
#endif  // _API_XC_VMARK_C_