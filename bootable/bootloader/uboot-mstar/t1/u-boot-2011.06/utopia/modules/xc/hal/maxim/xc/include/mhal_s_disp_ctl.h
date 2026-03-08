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
#ifndef MHAL_S_DISP_CTL_H
#define MHAL_S_DISP_CTL_H
#ifdef STELLAR
#include "apiXC_cus.h"

// ADC
MS_BOOL Hal_XC_S_ADC_ScartOverlay_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_ADC_Gain_SET(void *pInstance,APIXC_AdcGainOffsetSetting *stBuf);
MS_BOOL Hal_XC_S_ADC_Gain_GET(void *pInstance,APIXC_AdcGainOffsetSetting *stBuf);
MS_BOOL Hal_XC_S_ADC_Offset_SET(void *pInstance,APIXC_AdcGainOffsetSetting *stBuf);
MS_BOOL Hal_XC_S_ADC_Offset_GET(void *pInstance,APIXC_AdcGainOffsetSetting *stBuf);

// HDMI
MS_BOOL Hal_XC_S_HDMI_HDE_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_HDMI_HTT_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_HDMI_VDE_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_HDMI_VTT_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_HDMI_DDCChannel_EN(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);

// PCMode
MS_BOOL Hal_XC_S_PCMode_VResolution_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PCMode_HResolution_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PCMode_Htotal_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);

// XC
MS_BOOL Hal_XC_S_XC_FrameBufferNumber_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_XC_FrameBufferNumber_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_XC_RwPointDiff_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_XC_RwPointDiff_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_XC_OpwOff_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_XC_OpwOff_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_XC_InterlaceStatus_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_XC_FieldDetect_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_XC_DispDeWin_GET(void *pInstance,MS_WINDOW_TYPE *stBuf);

// PIP / Smart Zoom
MS_BOOL Hal_XC_S_PIP_SmartZoom_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PIP_SmartZoom_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);

// 3D
MS_BOOL Hal_XC_S_3D_KR3DMode_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_3D_KR3DMode_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);

// PVR
MS_BOOL Hal_XC_S_PVR_DualWinForAPVR_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PVR_DualWinForAPVR_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);

// PQ
MS_BOOL Hal_XC_S_PQ_PicSetHSC_SET(void *pInstance,MS_XC_CUS_PQ_PARA *stBuf);

// DS
MS_BOOL Hal_XC_S_DS_Status_GET(void *pInstance,DSLOAD_TYPE *stBuf);
MS_BOOL Hal_XC_S_DS_GST_PROCESS_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_DS_GST_PROCESS_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_DS_CAPTURE_PROCESS_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_DS_CAPTURE_PROCESS_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);


// PNL
MS_BOOL Hal_XC_S_PNL_STR_EN(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PNL_OutPECurrent_SET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PNL_PanelHStart_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PNL_PanelVStart_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PNL_PanelWidth_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);
MS_BOOL Hal_XC_S_PNL_PanelHeight_GET(void *pInstance,MS_XC_CUS_CTRL_PARA *stBuf);

#endif
#endif
