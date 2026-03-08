/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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
#ifndef _PANEL_H_
#define _PANEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <apiPNL.h>

#define INTERFACE extern

#ifndef LVDS_PN_SWAP_L
#define LVDS_PN_SWAP_L                  0x00
#endif
#ifndef LVDS_PN_SWAP_H
#define LVDS_PN_SWAP_H                  0x00
#endif
////////////////////////////////////////////////////
//      Type define
////////////////////////////////////////////////////

typedef enum{
    PANEL_RES_MIN = 0,

    PNL_FULLHD_SEC_LE32A = PANEL_RES_MIN,
    // FOR NORMAL LVDS PANEL
    PNL_SXGA_AU17_EN05       = 1,            // 1280X1024, PNL_AU17_EN05_SXGA
    PNL_WXGA_AU20_T200XW02   = 2,            // 1366X768, PNL_AU20_T200XW02_WXGA,
    PNL_WXGA_PLUS_CMO190_M190A1 = 3,         // 1440X900, PNL_CMO19_M190A1_WXGA, PNL_AU19PW01_WXGA
    PNL_WSXGA_AU22_M201EW01  = 4,            // 1680X1050, PNL_AU20_M201EW01_WSXGA,
    PNL_FULLHD_CMO216_H1L01  = 5,            // 1920X1080, PNL_AU37_T370HW01_HD, PNL_CMO216H1_L01_HD.H
    PANEL_RES_FULL_HD       = 5,            // for auotbuild compatitable

    // FOR DAC/HDMI TX OUTPUT
    DACOUT_480I              = 6,            // JUST FOR U3 DAC OUTPUT 480I TIMING USAGE
    DACOUT_480P              = 7,            // JUST FOR U3 DAC OUTPUT 480P TIMING USAGE
    DACOUT_576I              = 8,            // JUST FOR U3 DAC OUTPUT 576I TIMING USAGE
    DACOUT_576P              = 9,            // JUST FOR U3 DAC OUTPUT 576P TIMING USAGE
    DACOUT_720P_50           = 10,           // JUST FOR U3 DAC OUTPUT 720P TIMING USAGE
    DACOUT_720P_60           = 11,           // JUST FOR U3 DAC OUTPUT 720P TIMING USAGE
    DACOUT_1080I_50          = 12,           // JUST FOR U3 DAC OUTPUT 1080I TIMING USAGE
    DACOUT_1080I_60          = 13,           // JUST FOR U3 DAC OUTPUT 1080I TIMING USAGE
    DACOUT_1080P_50          = 14,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1080P_60          = 15,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE

    DACOUT_1080P_30          = 16,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1080P_25          = 17,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1080P_24          = 18,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_640x480_60        = 19,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1920x2205P_24     = 20,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE

    DACOUT_1280x1470P_50     = 21,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_1280x1470P_60     = 22,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_3840x2160P_24     = 23,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_3840x2160P_25     = 24,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE
    DACOUT_3840x2160P_30     = 25,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE

    DACOUT_3840x2160P_50    =26,
    DACOUT_3840x2160P_60    =27,
    DACOUT_4096x2160P_24    =28,
    DACOUT_4096x2160P_25    =29,
    DACOUT_4096x2160P_30    =30,

    DACOUT_4096x2160P_50    =31,
    DACOUT_4096x2160P_60    =32,

    PNL_WUXGA_CMO260_J2,           // 1920*1200
#if ENABLE_MFC
    MFC_DEFAULT,
    MFC120_FULLHD_CMO216_H1L01 = MFC_DEFAULT,
    MFC60_FULLHD_CMO216_H1L01,
#endif
#if ENABLE_TCON
    PNL_FULLHD_TCON,
#endif
    PNL_RES_MAX_NUM,
}PANEL_RESOLUTION_TYPE;

PanelType * MApi_GetPanelSpec(PANEL_RESOLUTION_TYPE enResolutionType);
void MApi_PNL_En(MS_BOOL bPanelOn, MS_U16 u16PanelTiming);
void MApi_PNL_On(MS_U16 u16PanelOnTiming );
void MApi_PNL_SetBackLight(MS_BOOL bEnable);
PANEL_RESOLUTION_TYPE MApi_PNL_GetPnlTypeSetting(void);
void MApi_PNL_Dump(void);
MS_BOOL MApi_SetEnv2BootArgs(const char* name,const char* value);
unsigned long MApi_PNL_GetPanelVccOnTime(void);
#endif

