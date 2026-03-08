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

/******************************************************************************/
/*                           Header Files                                     */
/******************************************************************************/

// Common Definition
#include "MsCommon.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
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
#include "mhal_offline.h"
#include "xc_hwreg_utility2.h"
#include "drv_sc_mux.h"
#include "mhal_mux.h"

//----------------------------
//
//----------------------------
#define    AIS_Debug(x)    //x


//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
MS_U8 MDrv_XC_GetOffLineDetection(void *pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MS_U8 u8CheckSrc = INPUT_PORT_NONE_PORT;

    u8CheckSrc = MDrv_XC_MUX_MapInputSourceToVDYMuxPORT(pInstance, u8InputSourceType);

    AIS_Debug(printf("u8CheckSrc=%u\n", u8CheckSrc);)

    return Hal_XC_GetOffLineDetection(pInstance, u8CheckSrc);
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
void MDrv_XC_SetOffLineDetection (void *pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MS_U8 u8CheckSrc = INPUT_PORT_NONE_PORT;

    _XC_ENTRY(pInstance);

    u8CheckSrc = MDrv_XC_MUX_MapInputSourceToVDYMuxPORT(pInstance, u8InputSourceType);
    AIS_Debug(printf("u8InputSourceType = %x, u8CheckSrc = %x\n", u8InputSourceType, (MS_U16)u8CheckSrc);)

    Hal_XC_SetOfflineDetectClk(pInstance);

    if (u8CheckSrc == INPUT_PORT_NONE_PORT)
    {
        Hal_XC_OffLineExit(pInstance);
    }
    _XC_RETURN(pInstance);

    if ((u8CheckSrc >= (MS_U8)INPUT_PORT_YMUX_CVBS0) &&
        (u8CheckSrc <= (MS_U8)INPUT_PORT_YMUX_CVBS7))
    {
        MsOS_DelayTask(10);

        _XC_ENTRY(pInstance);
        AIS_Debug(printf("MDrv_SetOffLineToSog VD Related Signal!\n");)
        Hal_XC_SetOffLineToSog_AV(pInstance, u8CheckSrc - INPUT_PORT_YMUX_CVBS0);

        MsOS_DelayTask(10);
        Hal_XC_OffLineInit(pInstance);

        _XC_RETURN(pInstance);

        MsOS_DelayTask(20);

        _XC_ENTRY(pInstance);
        Hal_XC_WaitForHVCleared_AV(pInstance, u8CheckSrc - INPUT_PORT_YMUX_CVBS0);
        Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_ADC_B);
         _XC_RETURN(pInstance);

        MsOS_DelayTask(50);
    }
    else if((u8CheckSrc >= (MS_U8)INPUT_PORT_ANALOG0)&&
           (u8CheckSrc <= (MS_U8)INPUT_PORT_ANALOG4))
    {
        MsOS_DelayTask(10);

        _XC_ENTRY(pInstance);
        AIS_Debug(printf("MDrv_SetOffLineToSog_YUV Component!\n");)
        Hal_XC_SetOffLineToSog_YUV(pInstance, u8CheckSrc - INPUT_PORT_ANALOG0);

        MsOS_DelayTask(10);
        Hal_XC_OffLineInit(pInstance);
        _XC_RETURN(pInstance);

        MsOS_DelayTask(20);

        _XC_ENTRY(pInstance);
        Hal_XC_WaitForHVCleared(pInstance, u8CheckSrc - INPUT_PORT_ANALOG0);
        Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_ADC_B);
        _XC_RETURN(pInstance);

        MsOS_DelayTask(50);
    }
    else if ((u8CheckSrc >= INPUT_PORT_ANALOG0_SYNC)  &&
             (u8CheckSrc <= (MS_U8)INPUT_PORT_ANALOG4_SYNC) )
    {
        MsOS_DelayTask(10);

        _XC_ENTRY(pInstance);
        AIS_Debug(printf("MDrv_SetOffLineToHV VGA!\n");)
        Hal_XC_SetOffLineToHv(pInstance, u8CheckSrc - INPUT_PORT_ANALOG0_SYNC);
        _XC_RETURN(pInstance);

        MsOS_DelayTask(20);

        _XC_ENTRY(pInstance);
        Hal_XC_WaitForHVCleared(pInstance, u8CheckSrc - INPUT_PORT_ANALOG0_SYNC);
        Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_ADC_B);
        _XC_RETURN(pInstance);

        MsOS_DelayTask(50);
    }
    else if ((u8CheckSrc >= INPUT_PORT_DVI0) && (u8CheckSrc <= INPUT_PORT_DVI3))
    {
        AIS_Debug(printf("MDrv_SetOffLineToHDMI 1~3!\n");)
        Hal_XC_SetOffLineToHDMI(pInstance, u8CheckSrc);
    }
    else if (u8CheckSrc == INPUT_PORT_MVOP )
    {
        AIS_Debug(printf("MDrv_SetOffLineToUSB!\n");)
        Hal_XC_SetOffLineToUSB(pInstance, u8CheckSrc);
    }
}

MS_BOOL MDrv_XC_SetOffLineSogThreshold(void *pInstance, MS_U8 u8Threshold)
{
    return Hal_XC_SetOffLineSogThreshold(pInstance, u8Threshold);
}

MS_BOOL MDrv_XC_SetOffLineSogBW(void *pInstance, MS_U8 u8BW)
{
    return Hal_XC_SetOffLineSogBW(pInstance, u8BW);
}

//-------------------------------------------------------------------------------------------------
/// Get Offline detection status to specific input source
/// @param  u8InputSourceType                \b IN: the specific input source type to do offline detection
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_GetOffLineDetection_U2( void* pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MS_U8 u8Ret = 0;
    _XC_ENTRY(pInstance);
    u8Ret = MDrv_XC_GetOffLineDetection(pInstance, u8InputSourceType);
    _XC_RETURN(pInstance);
    return u8Ret;
}

MS_U8 MApi_XC_GetOffLineDetection( INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_OFFLINE_DETECTION XCArgs;
    XCArgs.u8InputSourceType = u8InputSourceType;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OFFLINE_DETECTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Offline detection to specific input source
/// @param  u8InputSourceType                \b IN: the specific input source type to do offline detection
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetOffLineDetection_U2 ( void* pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MDrv_XC_SetOffLineDetection(pInstance, u8InputSourceType);
}

void MApi_XC_SetOffLineDetection ( INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OFFLINE_DETECTION XCArgs;
    XCArgs.u8InputSourceType = u8InputSourceType;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OFFLINE_DETECTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Offline detection Sog Threshold
/// @param  u8Threshold                \b IN: threshold value t2,u3,u4: 0~31 other chip: 0~255
/// @Return TRUE: Successful FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetOffLineSogThreshold_U2(void* pInstance, MS_U8 u8Threshold)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    bRet = MDrv_XC_SetOffLineSogThreshold(pInstance, u8Threshold);
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_SetOffLineSogThreshold(MS_U8 u8Threshold)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_OFFLINE_SOG_THRESHOLD XCArgs;
    XCArgs.u8Threshold = u8Threshold;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OFFLINE_SOG_THRESHOLD, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Offline Detection Sog filter bandwidth
/// @param  u8BW                \b IN: bandwidth value t2,u3,u4: 0~7 other chip: 0~31
/// @Return TRUE: Successful FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetOffLineSogBW_U2(void* pInstance, MS_U8 u8BW)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    bRet = MDrv_XC_SetOffLineSogBW(pInstance, u8BW);
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_SetOffLineSogBW(MS_U8 u8BW)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_OFFLINE_SOG_BW XCArgs;
    XCArgs.u8BW = u8BW;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OFFLINE_SOG_BW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_OffLineInit_U2(void* pInstance)
{
    _XC_ENTRY(pInstance);
    Hal_XC_OffLineInit(pInstance);
    MDrv_XC_SetOffLineSogThreshold(pInstance, 0x40);
    MDrv_XC_SetOffLineSogBW(pInstance, 0x1F);
    _XC_RETURN(pInstance);
    return TRUE;
}

MS_BOOL MApi_XC_OffLineInit(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_OFFLINE_INIT XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OFFLINE_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

