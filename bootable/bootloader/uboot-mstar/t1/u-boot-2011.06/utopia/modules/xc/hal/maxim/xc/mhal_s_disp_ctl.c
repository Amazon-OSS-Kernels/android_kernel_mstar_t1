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
// Common Definition
#ifdef STELLAR

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <asm/div64.h>
#else
#include <string.h>
#define do_div(x,y) ((x)/=(y))
#endif
#include "UFO.h"

// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_pm_sleep.h"//alex_tung
#include "hwreg_dvi_atop.h"//alex_tung
#include "hwreg_adc_atop.h"
#include "hwreg_adc_dtop.h"
#include "hwreg_adc_dtopb.h"
#include "hwreg_hdmi.h"
#include "hwreg_ipmux.h"
#include "hwreg_ddc.h"

#include "xc_Analog_Reg.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Auto.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#include "mhal_dynamicscaling.h"
#include "mdrv_sc_dynamicscaling.h"
#include "drvXC_HDMI_Internal.h"
#include "drvMVOP.h"
#include "drvIPAUTH.h"
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
#include "mhal_menuload.h"
#include "mhal_adc.h"
#include "halCHIP.h"
#include "drvSYS.h"
#include "drv_sc_scaling.h"
#include "drvSEAL.h"

#include "mhal_s_disp_ctl.h"


#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "apiPNL.h"

// input resolution
typedef struct
{
    MS_U16 u16DisplayWidth;  // width
    MS_U16 u16DisplayHeight; // height

}MS_MODE_RESOLUTION;

static const MS_MODE_RESOLUTION astStandardModeResolution[RES_MAXIMUM] =
{
    { 640,  350}, // 00: RES_640X350
    { 640,  400}, // 01: RES_640X400
    { 720,  400}, // 02: RES_720X400
    { 640,  480}, // 03: RES_640X480
    { 800,  600}, // 04: RES_800X600
    { 832,  624}, // 05: RES_832X624
    {1024,  768}, // 06: RES_1024X768
    {1280, 1024}, // 07: RES_1280X1024
    {1600, 1200}, // 08: RES_1600X1200
    {1152,  864}, // 09: RES_1152X864
    {1152,  870}, // 10: RES_1152X870
    {1280,  768}, // 11: RES_1280x768
    {1280,  960}, // 12: RES_1280X960
    { 720,  480}, // 13: RES_720X480
    {1920, 1080}, // 14: RES_1920X1080
    {1280,  720}, // 15: RES_1280X720
    { 720,  576}, // 16: RES_720X576
    {1920, 1200}, // 17: RES_1920X1200
    {1400, 1050}, // 18: RES_1400X1050
    {1440,  900}, // 19: RES_1440X900
    {1680, 1050}, // 20: RES_1680X1050
    {1280,  800}, // 21: RES_1280X800
    {1600, 1024}, // 22: RES_1600X1024
    {1600,  900}, // 23: RES_1600X900
    {1360,  768}, // 24: RES_1360X768
    { 848,  480}, // 25: RES_848X480
    {1920, 1080}, // 26: RES_1920X1080P
    {1366,  768}, // 27: RES_1366X768,
    { 864,  648}, // 28: RES_864X648,
    {1280, 1470}, // 29: RES_1280X1470,
    {1920, 2205}, // 30: RES_1920X2205,
    { 720,  240}, // 31: RES_720x240,
    { 720,  288}, // 32: RES_720x288,
    {1152,  900}, // 33: RES_1152X900,
    { 856,  480}, // 34: RES_856x480,
    {1536,  960}, // 35: RES_1536X960,
    {1600, 1000}, // 36: RES_1600X1000,
    {1704,  960}, // 37: RES_1704X960,
    {1728, 1080}, // 38: RES_1728X1080,
    {1864, 1050}, // 39: RES_1864X1050,
};

static MS_BOOL bIsSmartZoomOn = FALSE;
static MS_BOOL bIsKR3DMode = FALSE;
static MS_BOOL bDualWinForAPVR = FALSE;
static MS_BOOL bIsGstreamerProcess = FALSE;
static MS_BOOL bIsCaptureProcess = FALSE;

MS_BOOL bSTROn = FALSE;

extern XC_PNL_OBJ g_IPanel;

extern MS_BOOL MHal_PNL_MOD_Control_Out_PE_Current (void *pInstance, MS_U16 u16Current_Level);


// ADC
MS_BOOL Hal_XC_S_ADC_ScartOverlay_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    if(stBuf->bParam)	// Show SCART-CVBS & SCART-RGB simutaneously
    {
        W2BYTEMSK(REG_ADC_ATOP_42_L, 0x3 << 6, (BIT(7)|BIT(6)));
        W2BYTEMSK(REG_ADC_ATOP_43_L, 0x1F, 0x3F);
	}
    else
    {
        W2BYTEMSK(REG_ADC_ATOP_42_L, 0x1 << 6, (BIT(7)|BIT(6)));
        W2BYTEMSK(REG_ADC_ATOP_43_L, 0x00, 0x3F);
    }
    return TRUE;
}

MS_BOOL Hal_XC_S_ADC_Gain_SET(void *pInstance,APIXC_AdcGainOffsetSetting *stBuf)
{
    Hal_ADC_SetGainR(pInstance, stBuf->u16RedGain);
    Hal_ADC_SetGainG(pInstance, stBuf->u16GreenGain);
    Hal_ADC_SetGainB(pInstance, stBuf->u16BlueGain);
    return TRUE;
}

MS_BOOL Hal_XC_S_ADC_Gain_GET(void *pInstance,APIXC_AdcGainOffsetSetting *stBuf)
{
    stBuf->u16RedGain= Hal_ADC_GetGainR(pInstance);
    stBuf->u16GreenGain = Hal_ADC_GetGainG(pInstance);
    stBuf->u16BlueGain = Hal_ADC_GetGainB(pInstance);
    return TRUE;
}

MS_BOOL Hal_XC_S_ADC_Offset_SET(void *pInstance,APIXC_AdcGainOffsetSetting *stBuf)
{
    Hal_ADC_SetOffsetR(pInstance, stBuf->u16RedOffset);
    Hal_ADC_SetOffsetG(pInstance, stBuf->u16GreenOffset);
    Hal_ADC_SetOffsetB(pInstance, stBuf->u16BlueOffset);
    return TRUE;
}

MS_BOOL Hal_XC_S_ADC_Offset_GET(void *pInstance,APIXC_AdcGainOffsetSetting *stBuf)
{
    stBuf->u16RedOffset = Hal_ADC_GetOffsetR(pInstance);
    stBuf->u16GreenOffset = Hal_ADC_GetOffsetG(pInstance);
    stBuf->u16BlueOffset = Hal_ADC_GetOffsetB(pInstance);
    return TRUE;
}

// HDMI
MS_BOOL Hal_XC_S_HDMI_HDE_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    stBuf->u16Param = R2BYTEMSK(REG_HDMI_65_L, 0x3FFF);
    return TRUE;
}

MS_BOOL Hal_XC_S_HDMI_HTT_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    stBuf->u16Param = R2BYTEMSK(REG_HDMI2_32_L, 0x3FFF);
    return TRUE;
}

MS_BOOL Hal_XC_S_HDMI_VDE_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    stBuf->u16Param = R2BYTEMSK(REG_HDMI2_31_L, 0x3FFF);
    return TRUE;
}

MS_BOOL Hal_XC_S_HDMI_VTT_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    stBuf->u16Param = 0;
    return TRUE;
}

MS_BOOL Hal_XC_S_HDMI_DDCChannel_EN(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    if(stBuf->bParam)
    {
        switch(stBuf->enInputPortType)
        {
            case INPUT_PORT_DVI0://A
            {
                PM_W2BYTE(REG_DDC_4B_L, BIT(1)|BIT(5), BIT(1)|BIT(5)|BIT(7));
                break;
            }
            case INPUT_PORT_DVI1://B
            {
                PM_W2BYTE(REG_DDC_4B_L, BIT(9)|BIT(13), BIT(9)|BIT(13)|BIT(15));
                break;
            }
            case INPUT_PORT_DVI3://D
            {
                PM_W2BYTE(REG_DDC_4C_L, BIT(9)|BIT(13), BIT(9)|BIT(13)|BIT(15));
                break;
            }
            case INPUT_PORT_DVI2://C
            {
                PM_W2BYTE(REG_DDC_4C_L, BIT(1)|BIT(5), BIT(1)|BIT(5)|BIT(7));
                break;
            }
            default:
            {
                return FALSE;
            }
        }
    }
    else
    {
        switch(stBuf->enInputPortType)
        {
            case INPUT_PORT_DVI0://A
            {
                PM_W2BYTE(REG_DDC_4B_L, BIT(7), BIT(1)|BIT(5)|BIT(7));
                break;
            }
            case INPUT_PORT_DVI1://B
            {
                PM_W2BYTE(REG_DDC_4B_L, BIT(15), BIT(9)|BIT(13)|BIT(15));
                break;
            }
            case INPUT_PORT_DVI3://D
            {
                PM_W2BYTE(REG_DDC_4C_L, BIT(15), BIT(9)|BIT(13)|BIT(15));
                break;
            }
            case INPUT_PORT_DVI2://C
            {
                PM_W2BYTE(REG_DDC_4C_L, BIT(7), BIT(1)|BIT(5)|BIT(7));
                break;
            }
            default:
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

// PCMode
MS_BOOL Hal_XC_S_PCMode_VResolution_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    MS_BOOL bRet = FALSE;
    if(stBuf->u32Param < RES_MAXIMUM)
    {
        stBuf->u16Param = astStandardModeResolution[stBuf->u32Param].u16DisplayHeight;
		bRet = TRUE;
    }
    return bRet;
}

MS_BOOL Hal_XC_S_PCMode_HResolution_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    MS_BOOL bRet = FALSE;
    if(stBuf->u32Param < RES_MAXIMUM)
    {
        stBuf->u16Param = astStandardModeResolution[stBuf->u32Param].u16DisplayWidth;
		bRet = TRUE;
    }
    return bRet;
}

MS_BOOL Hal_XC_S_PCMode_Htotal_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, stBuf->eWindow);
    if((stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_UNSTABLE) || (stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        stBuf->u16Param = 0;
    }
    else
    {
        stBuf->u16Param = stPCMonitorInfo.u16Hperiod;
    }
    return TRUE;
}

// XC
MS_BOOL Hal_XC_S_XC_FrameBufferNumber_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    if(stBuf->eWindow == MAIN_WINDOW)
    {
        stBuf->u16Param = R2BYTEMSK(REG_SC_BK12_19_L, 0x001F);
    }
	else
	{
        stBuf->u16Param = R2BYTEMSK(REG_SC_BK12_59_L, 0x001F);
	}
    return TRUE;
}

MS_BOOL Hal_XC_S_XC_FrameBufferNumber_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    if(stBuf->eWindow == MAIN_WINDOW)
    {
        W2BYTEMSK(REG_SC_BK12_19_L, stBuf->u16Param, 0x001F);
    }
	else
	{
        W2BYTEMSK(REG_SC_BK12_59_L, stBuf->u16Param, 0x001F);
	}
    return TRUE;
}

MS_BOOL Hal_XC_S_XC_RwPointDiff_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    if(stBuf->eWindow == MAIN_WINDOW)
    {
        W2BYTEMSK(REG_SC_BK12_07_L, stBuf->u16Param, BIT(13)|BIT(14)|BIT(15));
    }
	else
	{
        W2BYTEMSK(REG_SC_BK12_47_L, stBuf->u16Param, BIT(13)|BIT(14)|BIT(15));
	}
    return TRUE;
}

MS_BOOL Hal_XC_S_XC_RwPointDiff_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    if(stBuf->eWindow == MAIN_WINDOW)
    {
        stBuf->u16Param = R2BYTEMSK(REG_SC_BK12_07_L, BIT(13)|BIT(14)|BIT(15));
    }
	else
	{
        stBuf->u16Param = R2BYTEMSK(REG_SC_BK12_47_L, BIT(13)|BIT(14)|BIT(15));
	}
    return TRUE;
}

MS_BOOL Hal_XC_S_XC_OpwOff_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    stBuf->bParam = R2BYTEMSK(REG_SC_BK12_67_L, BIT(1))? TRUE: FALSE;
    return TRUE;
}

MS_BOOL Hal_XC_S_XC_OpwOff_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    W2BYTEMSK(REG_SC_BK12_67_L, stBuf->bParam? BIT(1): 0, BIT(1));
    return TRUE;
}

MS_BOOL Hal_XC_S_XC_InterlaceStatus_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    MS_U8 reg_IP1F2_1D, reg_IP1F2_23;
    MS_U16 reg_IP1F2_21;

    if(stBuf->bParam)
    {
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_21 = 0x0403;
        reg_IP1F2_23 = 0x30;
    }
    else
    {
        reg_IP1F2_1D = 0x21;
        reg_IP1F2_21 = 0x0400;
        reg_IP1F2_23 = 0x00;
    }

    if( stBuf->eWindow == MAIN_WINDOW )
    {
        W2BYTEMSK(REG_SC_BK01_1D_L, (reg_IP1F2_1D<<8), 0xEF00);
        W2BYTEMSK(REG_SC_BK01_21_L, reg_IP1F2_21, 0x3FFF);
        W2BYTEMSK(REG_SC_BK01_23_L, (reg_IP1F2_23<<8), HBMASK);
    }
    else if(stBuf->eWindow == SUB_WINDOW)
    {
        W2BYTEMSK(REG_SC_BK03_1D_L, (reg_IP1F2_1D<<8), 0xEF00);
        W2BYTEMSK(REG_SC_BK03_21_L, reg_IP1F2_21, 0x3FFF);
        W2BYTEMSK(REG_SC_BK03_23_L, (reg_IP1F2_23<<8), HBMASK);
    }
    else if(stBuf->eWindow == OFFLINE_WINDOW)
    {
        W2BYTEMSK(REG_SC_BK13_1D_L, (reg_IP1F2_1D<<8), 0xEF00);
        W2BYTEMSK(REG_SC_BK13_21_L, reg_IP1F2_21, 0x3FFF);
        W2BYTEMSK(REG_SC_BK13_23_L, (reg_IP1F2_23<<8), HBMASK);
    }
	else
	{
	    return FALSE;
	}

    return TRUE;
}

MS_BOOL Hal_XC_S_XC_FieldDetect_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    MS_U8 reg_IP1F2_1D, reg_IP1F2_23;
    MS_U16 reg_IP1F2_21;
    MS_U16 u16Vtt = 0;

    // Note:
    // IP1F2_1D[13]: Enable auto no signal filter mode.
    // This functionality is to improve timing detection stability.

    if(IsSrcTypeDigitalVD(stBuf->enInputSrcType))
    {
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_21 = 0x0403;
        reg_IP1F2_23 = 0x30;
    }
    else if(IsSrcTypeDTV(stBuf->enInputSrcType) || IsSrcTypeStorage(stBuf->enInputSrcType))
    {
        if ((EN_VIDEO_SCAN_TYPE)stBuf->u16Param == SCAN_INTERLACE)
        {
            reg_IP1F2_1D = 0xA1;
            reg_IP1F2_21 = 0x0403;
            reg_IP1F2_23 = 0x30;
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0400;
            reg_IP1F2_23 = 0x00;
        }
    }
    else if (IsSrcTypeHDMI(stBuf->enInputSrcType))
    {
        if (((EN_VIDEO_SCAN_TYPE)stBuf->u16Param == SCAN_INTERLACE)
                || (MDrv_SC_GetInterlaceInPModeStatus(pInstance, stBuf->eWindow)
                    && (MDrv_XC_GetForceiSupportType(pInstance) == ForceI_SW)))
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0003;      // enable DE -> no field invert, disable DE -> use field invert
            reg_IP1F2_23 = 0x20;

            u16Vtt = MDrv_SC_ip_get_verticaltotal(pInstance, stBuf->eWindow);
            if((u16Vtt/2)%2 )
            {
                reg_IP1F2_21 |= BIT(8);
            }
            else
            {
                reg_IP1F2_21 &= ~BIT(8);
            }
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0000;
            reg_IP1F2_23 = 0x00;
        }

        reg_IP1F2_21 |= BIT(12);
        if( (MApi_XC_GetHdmiSyncMode() == HDMI_SYNC_HV)
                &&((EN_VIDEO_SCAN_TYPE)stBuf->u16Param == SCAN_INTERLACE) )
        {
            XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
            UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
            MS_U16 u16Cur_IP1F2_21Val = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L);
            if( u16Cur_IP1F2_21Val&0xC000 )
            {
                if( reg_IP1F2_21&BIT(8) )
                {
                    reg_IP1F2_21 &= ~BIT(8);
                }
                else
                {
                    reg_IP1F2_21 |= BIT(8);
                }
                //printf("daniel_test patch REG_SC_BK01_21_L: FieldInvert\n");
            }
            else
            {
            }
        }

    }
    else if(IsSrcTypeVga(stBuf->enInputSrcType))
    {
        if ((EN_VIDEO_SCAN_TYPE)stBuf->u16Param == SCAN_INTERLACE)
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0103;
            reg_IP1F2_23 = 0x10;
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0000;
            reg_IP1F2_23 = 0x00;
        }
    }
    else if (IsSrcTypeCapture(stBuf->enInputSrcType))
    {
        reg_IP1F2_21 = 0x0101;//IP1 sometimes will detect opcapture to interlace,we force it to progressive
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_23 = 0x08; //0x20  // Vtt will use "IP1F2_23 * 16" as lower bound.
    }
    else // YPbPr
    {
        if(MDrv_XC_ip_GetEuroHdtvStatus(pInstance, MAIN_WINDOW) == EURO_AUS_HDTV_NORMAL)
        {
            reg_IP1F2_21 = 0x0100;
        }
        else
        {
            reg_IP1F2_21 = 0x0000;
            //reg_IP1F2_21 = 0x0100;
        }
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_23 = 0x08; //0x20  // Vtt will use "IP1F2_23 * 16" as lower bound.
    }

    if( stBuf->eWindow == MAIN_WINDOW )
    {
        W2BYTEMSK(REG_SC_BK01_1D_L, (reg_IP1F2_1D<<8), 0xEF00);
        W2BYTEMSK(REG_SC_BK01_21_L, reg_IP1F2_21, 0x3FFF);
        W2BYTEMSK(REG_SC_BK01_23_L, (reg_IP1F2_23<<8), HBMASK);
    }
    else if(stBuf->eWindow == SUB_WINDOW)
    {
        W2BYTEMSK(REG_SC_BK03_1D_L, (reg_IP1F2_1D<<8), 0xEF00);
        W2BYTEMSK(REG_SC_BK03_21_L, reg_IP1F2_21, 0x3FFF);
        W2BYTEMSK(REG_SC_BK03_23_L, (reg_IP1F2_23<<8), HBMASK);
    }
    else if(stBuf->eWindow == OFFLINE_WINDOW)
    {
        W2BYTEMSK(REG_SC_BK13_1D_L, (reg_IP1F2_1D<<8), 0xEF00);
        W2BYTEMSK(REG_SC_BK13_21_L, reg_IP1F2_21, 0x3FFF);
        W2BYTEMSK(REG_SC_BK13_23_L, (reg_IP1F2_23<<8), HBMASK);
    }
	else
	{
	    return FALSE;
	}

    return TRUE;
}

MS_BOOL Hal_XC_S_XC_DispDeWin_GET(void *pInstance,MS_WINDOW_TYPE *stBuf)
{
    stBuf->x = SC_R2BYTE(0,REG_SC_BK10_04_L);
    stBuf->y = SC_R2BYTE(0,REG_SC_BK10_06_L);
    stBuf->width = SC_R2BYTE(0,REG_SC_BK10_05_L) - stBuf->x + 1;
    stBuf->height = SC_R2BYTE(0,REG_SC_BK10_07_L) - stBuf->y + 1;

    return TRUE;
}

// PIP / Smart Zoom
MS_BOOL Hal_XC_S_PIP_SmartZoom_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	bIsSmartZoomOn = stBuf->bParam;
    return TRUE;
}

MS_BOOL Hal_XC_S_PIP_SmartZoom_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->bParam = bIsSmartZoomOn;
    return TRUE;
}

// 3D
MS_BOOL Hal_XC_S_3D_KR3DMode_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	bIsKR3DMode = stBuf->bParam;
    return TRUE;
}

MS_BOOL Hal_XC_S_3D_KR3DMode_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->bParam = bIsKR3DMode;
    return TRUE;
}

// PVR
MS_BOOL Hal_XC_S_PVR_DualWinForAPVR_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	bDualWinForAPVR = stBuf->bParam;
    return TRUE;
}

MS_BOOL Hal_XC_S_PVR_DualWinForAPVR_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->bParam = bDualWinForAPVR;
    return TRUE;
}

// PQ
extern void msAdjustHSC(void *pInstance, MS_BOOL bScalerWin, MS_U8 ucHue, MS_U8 ucSaturation, MS_U8 ucContrast);
extern void    msAdjustPCContrast(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Contrast );
MS_BOOL Hal_XC_S_PQ_PicSetHSC_SET(void *pInstance,MS_XC_CUS_PQ_PARA *stBuf)
{
#if 1
    if(stBuf->bUseYUVSpace)
    {
        msAdjustHSC(pInstance, stBuf->eWindow, stBuf->u8Hue, stBuf->u8Saturation, stBuf->u8Contrast);
    }
    else
    {
        msAdjustPCContrast(pInstance, stBuf->eWindow, stBuf->u8Contrast);
    }
#endif
    return TRUE;
}

// DS
MS_BOOL Hal_XC_S_DS_Status_GET(void *pInstance,DSLOAD_TYPE *stBuf)
{
    if (!MHAL_SC_Get_DynamicScaling_Status(pInstance))
    {
        *stBuf = E_DSLOAD_DISABLED;
    }
	else
	{
        *stBuf = E_DSLOAD_ENABLED_HWMODE;
	}

    return TRUE;
}

MS_BOOL Hal_XC_S_DS_GST_PROCESS_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	bIsGstreamerProcess = stBuf->bParam;
    return TRUE;
}

MS_BOOL Hal_XC_S_DS_GST_PROCESS_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->bParam = bIsGstreamerProcess;
    return TRUE;
}

MS_BOOL Hal_XC_S_DS_CAPTURE_PROCESS_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	bIsCaptureProcess = stBuf->bParam;
    return TRUE;
}

MS_BOOL Hal_XC_S_DS_CAPTURE_PROCESS_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->bParam = bIsCaptureProcess;
    return TRUE;
}


// PNL
MS_BOOL Hal_XC_S_PNL_STR_EN(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	bSTROn = stBuf->bParam;
    return TRUE;
}

MS_BOOL Hal_XC_S_PNL_OutPECurrent_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
    return MHal_PNL_MOD_Control_Out_PE_Current(pInstance, stBuf->u16Param);
}

MS_BOOL Hal_XC_S_PNL_PanelHStart_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->u16Param = g_IPanel.HStart();
    return TRUE;
}

MS_BOOL Hal_XC_S_PNL_PanelVStart_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->u16Param = g_IPanel.VStart();
    return TRUE;
}

MS_BOOL Hal_XC_S_PNL_PanelWidth_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->u16Param = g_IPanel.Width();
    return TRUE;
}

MS_BOOL Hal_XC_S_PNL_PanelHeight_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf)
{
	stBuf->u16Param = g_IPanel.Height();
    return TRUE;
}

#endif
