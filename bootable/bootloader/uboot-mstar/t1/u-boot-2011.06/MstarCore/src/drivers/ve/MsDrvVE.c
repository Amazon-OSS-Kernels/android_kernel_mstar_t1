/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

//--------------------------------------------------------------------------------------------------
// Include Files
//--------------------------------------------------------------------------------------------------
#include <common.h>
#include <MsCommon.h>
#include <drvTVEncoder.h>
#include <uboot_mmap.h>
#include <MsMmap.h>
#include <MsDebug.h>
#include <apiPNL.h>
#if (CONFIG_HDMITX_MSTAR_ROCKET==1)
#include <apilth.h>
#endif
#include <apiXC.h>
#include <apiXC_Adc.h>

#include <apiGOP.h>
#include <ve/MsDrvVE.h>
#include <panel/MsDrvPanel.h>


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_VE_VIDEOSYS getVEVideoSys(HDMITX_VIDEO_TIMING outPutType)
{
    switch (outPutType)
    {
        case HDMITX_RES_640x480p:
        case HDMITX_RES_720x480i:
        case HDMITX_RES_720x480p:
        case HDMITX_RES_1280x720p_60Hz:
        case HDMITX_RES_1920x1080i_60Hz:
        case HDMITX_RES_1920x1080p_24Hz:
        case HDMITX_RES_1920x1080p_30Hz:
        case HDMITX_RES_1920x1080p_60Hz:
        case HDMITX_RES_4K2Kp_30Hz:
        case HDMITX_RES_1280x1470p_60Hz:
        case HDMITX_RES_1280x1470p_24Hz:
        case HDMITX_RES_1280x1470p_30Hz:
        case HDMITX_RES_1920x2205p_24Hz:
        case HDMITX_RES_1920x2205p_30Hz:
        case HDMITX_RES_4K2Kp_25Hz:
        case HDMITX_RES_MAX:

            return MS_VE_NTSC;

        case HDMITX_RES_720x576i:
        case HDMITX_RES_720x576p:
        case HDMITX_RES_1280x720p_50Hz:
        case HDMITX_RES_1920x1080i_50Hz:
        case HDMITX_RES_1920x1080p_25Hz:
        case HDMITX_RES_1920x1080p_50Hz:
        case HDMITX_RES_1280x1470p_50Hz:

            return MS_VE_PAL;
        default:
            UBOOT_ERROR("ERROR: Not support current panel resolution!\n");
            break;
    }

    return MS_VE_VIDEOSYS_NUM;
}

static MS_U16 getFrameRates(HDMITX_VIDEO_TIMING outPutType)
{
    switch (outPutType)
    {
        case HDMITX_RES_640x480p:
        case HDMITX_RES_720x480i:
        case HDMITX_RES_720x480p:
        case HDMITX_RES_1280x720p_60Hz:
        case HDMITX_RES_1920x1080i_60Hz:
        case HDMITX_RES_1920x1080p_24Hz:
        case HDMITX_RES_1920x1080p_30Hz:
        case HDMITX_RES_1920x1080p_60Hz:
        case HDMITX_RES_4K2Kp_30Hz:
        case HDMITX_RES_1280x1470p_60Hz:
        case HDMITX_RES_1280x1470p_24Hz:
        case HDMITX_RES_1280x1470p_30Hz:
        case HDMITX_RES_1920x2205p_24Hz:
        case HDMITX_RES_1920x2205p_30Hz:
        case HDMITX_RES_MAX:
            return 6000 / 2;
        case HDMITX_RES_720x576i:
        case HDMITX_RES_720x576p:
        case HDMITX_RES_1280x720p_50Hz:
        case HDMITX_RES_1920x1080i_50Hz:
        case HDMITX_RES_1920x1080p_25Hz:
        case HDMITX_RES_1920x1080p_50Hz:
        case HDMITX_RES_1280x1470p_50Hz:
        default:
            break;
    }

    return 5000 / 2;
}


MS_BOOL mvideo_sc_is_interlace(void)
{
    return 0;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
///Entry function
//--------------------------------------------------------------------------------------------------
int MsDrv_VE_init(HDMITX_VIDEO_TIMING outPutType)
{

    U32 u32Addr=0,u32Size=0;

    UBOOT_TRACE("IN\n");


    //0. VE initialization
    if(get_addr_from_mmap("E_MMAP_ID_VE", &u32Addr)!=0)
    {
        UBOOT_ERROR("get E_MMAP_ID_VE fail\n");
        return -1;
    }

    MApi_XC_SetOutputCapture(ENABLE,E_XC_OP2);      // Enable op2 to ve path
    MDrv_VE_Init(u32Addr);
    UBOOT_DEBUG("E_MMAP_ID_VE 0x%x \n",u32Addr);
    MDrv_VE_SetOSD(FALSE);

    MDrv_VE_SetOutputVideoStd(getVEVideoSys(outPutType));
    //setHDMITxAnalogTuning(outPutType);    //set bank21_0x2e(16bit mode)
    //1. VE configuration
    MS_Switch_VE_Src_Info SwitchInputSrc;
    SwitchInputSrc.InputSrcType = MS_VE_SRC_SCALER;

    MDrv_VE_SwitchInputSource(&SwitchInputSrc);

    MS_Switch_VE_Dest_Info SwitchOutputDest;
	SwitchOutputDest.OutputDstType = MS_VE_DEST_SCART;

    MDrv_VE_SwitchOuputDest(&SwitchOutputDest);

    MS_VE_Set_Mode_Type SetModeType;
    SetModeType.u16H_CapSize     = g_IPanel.Width();

#if (CONFIG_HDMITX_MSTAR_ROCKET==1 && ENABLE_MSTAR_NAPOLI==1)
    extern MS_BOOL is_Interlace(HDMITX_VIDEO_TIMING outputTiming);

    if(is_Interlace(outPutType))
    {
        SetModeType.u16V_CapSize     = g_IPanel.Height()*2;
        SetModeType.bSrcInterlace = TRUE;
        UBOOT_DEBUG("interlaced u16V_CapSize X2 !!!\n");
    }
    else
#endif

    {
        SetModeType.bSrcInterlace = FALSE;
        SetModeType.u16V_CapSize     = g_IPanel.Height();
    }
    SetModeType.u16H_CapStart    = g_IPanel.HStart();
    SetModeType.u16V_CapStart    = g_IPanel.VStart();
    SetModeType.u16H_SC_CapSize  = g_IPanel.Width();
    SetModeType.u16V_SC_CapSize  = g_IPanel.Height();
    SetModeType.u16H_SC_CapStart = g_IPanel.HStart();
    SetModeType.u16V_SC_CapStart = g_IPanel.VStart();
    SetModeType.u16InputVFreq = getFrameRates(outPutType)*2/10;

    SetModeType.bHDuplicate   = FALSE;

    MDrv_VE_SetMode(&SetModeType);

    MS_VE_Output_Ctrl OutputCtrl;
    OutputCtrl.bEnable = TRUE;

    OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;

    MDrv_VE_SetOutputCtrl(&OutputCtrl);
    MDrv_VE_SetRGBIn(FALSE);
    MApi_XC_ADC_SetCVBSOut(ENABLE, OUTPUT_CVBS1, INPUT_SOURCE_DTV, TRUE);
    UBOOT_TRACE("OUT\n");
    return 1;

}



