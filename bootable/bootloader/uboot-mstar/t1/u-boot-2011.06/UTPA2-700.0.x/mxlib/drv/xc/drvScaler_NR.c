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
#define DRV_SCALER_NR_C
/******************************************************************************/
/*                           Header Files                                     */
/******************************************************************************/
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
//#include "Debug.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "mvideo_context.h"
#include "apiXC_PCMonitor.h"
#include "drv_sc_ip.h"
#include "mhal_xc_chip_config.h"
#include "mhal_sc.h"

#if TEST_NEW_DYNAMIC_NR

/******************************************************************************/
/*                           Constants                                        */
/******************************************************************************/

/******************************************************************************/
/*                           Local Prototypes                                 */
/******************************************************************************/
#define DynamicNR_DBG(x) //x

/******************************************************************************/
/*                           Variables                                        */
/******************************************************************************/


/******************************************************************************/
///Enable/Disable 3DNR
///@param bEnable \b IN: enable/disable
/******************************************************************************/
void MDrv_Scaler_Enable3DNR(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (gSrcInfo[eWindow].bForceNRoff) // force NR to be disabled when GWIN is consuming bandwidth in HD modes
    {
        bEnable = DISABLE;
    }

    HAL_SC_enable_dnr(pInstance, 0x01, eWindow);
    HAL_SC_enable_field_avg_y(pInstance, bEnable, eWindow); //Main Window Y average mode when dotline cycle
    HAL_SC_enable_field_avg_c(pInstance, DISABLE, eWindow); //Main Window C average mode when dotline cycle
                                                 //always DISABLE
}


void MDrv_Scaler_EnableNR(void *pInstance, MS_BOOL bEn, SCALER_WIN eWindow)
{
    if (bEn == 0)
    {
        mvideo_sc_set_3dnr(pInstance, DISABLE, eWindow);
        MDrv_Scaler_Enable3DNR(pInstance, DISABLE, eWindow);
    }
    else
    {
        mvideo_sc_set_3dnr(pInstance, ENABLE, eWindow);
        // NR start to work immediately, but it does not fetch memory until next frame
        // enable NR in vblank to avoid wrong result
        MDrv_XC_wait_input_vsync(pInstance, 1, 50, eWindow);
        MDrv_Scaler_Enable3DNR(pInstance, ENABLE, eWindow);
    }

//T2 doesn't have Pre CCS
//    if( MApi_XC_IsYUVSpace() )
//        MDrv_Scaler_SetCNR(MS_CNR_ON);
//    else
//        MDrv_Scaler_SetCNR(MS_CNR_OFF);

//    MDrv_Scaler_SetDNR(MS_DNR_OFF);
}

#endif  //TEST_NEW_DYNAMIC_NR

