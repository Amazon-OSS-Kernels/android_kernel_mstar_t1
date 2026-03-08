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
/// @file   drvDAC.h
/// @brief  DAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DAC_H_
#define _DRV_DAC_H_


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    RES_720x480I_60Hz = 0,
    RES_720x480P_60Hz,
    RES_720x576I_50Hz,
    RES_720x576P_50Hz,
    RES_1280x720P_50Hz,
    RES_1280x720P_60Hz,
    RES_1920x1080I_50Hz,
    RES_1920x1080I_60Hz,
    RES_1920x1080P_50Hz,
    RES_1920x1080P_60Hz,
    RES_1920x1080P_24Hz,
    RES_1920x1080P_25Hz,
    RES_1920x1080P_30Hz,
    RES_640x480P_60Hz,
    RES_VGA640x480P_60Hz,
    RES_TTL480X272P_60Hz,
} EN_OUTPUT_VIDEO_TIMING_TYPE;

typedef enum
{
    COLOR_8_BIT,
    COLOR_10_BIT,
    COLOR_12_BIT,
}EN_OUTPUT_BIT_TYPE;

typedef enum
{
    DAC_CLK_FROM_DISP,
    DAC_CLK_FROM_VEDAC,
    DAC_CLK_FROM_DVBC_INN,
    DAC_CLK_FROM_DVBC_EQ,
    DAC_CLK_FROM_ADCD_D,
    DAC_CLK_FROM_DFT,
} EN_DAC_CLK_TYPE;

typedef enum
{
    DAC_TO_HDGEN = 0,   //HD Gen output
    DAC_TO_VE = 1,      //VE output (CVBS + S-Video)
    DAC_TO_VE_YPBPR =3  //VE output (Y/Pb/Pr)
} EN_DAC_OUTPUT_TYPE;

typedef enum
{
    DAC_R_G_B = 0,
    DAC_R_B_G = 1,
    DAC_G_R_B = 2,
    DAC_B_G_R = 4,
    DAC_G_B_R = 5,
    DAC_B_R_G = 6,
    DAC_R_R_R = 7,
} EN_DAC_SWAP_SEL;

typedef enum
{
    DAC_OUTPUT_LEVEL_HIGH = 0,  //1.3V
    DAC_OUTPUT_LEVEL_LOW = 1    //1V
} EN_DAC_MAX_OUTPUT_LEVEL;

typedef enum
{
    DAC_SD_OFF =0,
    DAC_SD_X = 1,
    DAC_SD_Y = 2,
    DAC_SD_XY = 3,
    DAC_SD_C = 4,
    DAC_SD_XC = 5,
    DAC_SD_YC = 6,
    DAC_SD_XYC = 7,

}EN_DAC_SD_ENABLE_CTRL;

typedef enum
{
    DAC_HD_OFF =0,
    DAC_HD_R = 1,
    DAC_HD_G = 2,
    DAC_HD_RG = 3,
    DAC_HD_B = 4,
    DAC_HD_RB = 5,
    DAC_HD_GB = 6,
    DAC_HD_RGB = 7,

}EN_DAC_HD_ENABLE_CTRL;

typedef enum
{
    DAC_DETECT_PLUGIN,       //Detect  Plug_IN
    DAC_DETECT_PLUGOUT,     // Detect Plug_OUT
}EN_DAC_DETECT_TYPE;

typedef enum
{
    DAC_DETECT_HD,       //Detect HD_DAC
    DAC_DETECT_SD,     // Detect SD_DAC
}EN_DAC_DETECT;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef MDRV_DAC_C
#define MDRV_INTERFACE
#else
#define MDRV_INTERFACE extern
#endif

MDRV_INTERFACE void MDrv_DAC_SetIOMapBase(MS_VIRT virtNPMBase, MS_VIRT virtPMBase);
MDRV_INTERFACE void MDrv_DAC_Init(void);
MDRV_INTERFACE void MDrv_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetOutputSource(EN_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetOutputLevel(EN_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetOutputSwapSel(EN_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr);
MDRV_INTERFACE void MDrv_DAC_SetOutputTiming(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming);
MDRV_INTERFACE void MDrv_ClkSel_Set(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming, EN_OUTPUT_BIT_TYPE enBits);
MDRV_INTERFACE void MDrv_DAC_OnOffSD(EN_DAC_SD_ENABLE_CTRL enBit);
MDRV_INTERFACE void MDrv_DAC_OnOffHD(EN_DAC_HD_ENABLE_CTRL enBit);
MDRV_INTERFACE void MDrv_DAC_Exit(void);
MDRV_INTERFACE void MDrv_DAC_DumpTable(MS_U8 * pTCONTable, MS_U8 u8Tcontype);

//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DAC_SetQuartOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// MDrv_DAC_SetDACState - Set DAC init state
// @param: bEnable: TRUE for DAC is initialized, FALSE for not initialized
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return none
//----------------------------------------------------------------
MDRV_INTERFACE void MDrv_DAC_SetDACState(MS_BOOL bEnable, MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// MDrv_DAC_GetDACState - Get DAC init state
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return: TRUE is DAC is initialized
//----------------------------------------------------------------
MDRV_INTERFACE MS_BOOL MDrv_DAC_GetDACState(MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// MDrv_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
MDRV_INTERFACE MS_BOOL MDrv_DAC_HotPlugDetect(EN_DAC_DETECT SelDAC, EN_DAC_DETECT_TYPE DetectType, MS_BOOL *State);

#ifdef __cplusplus
}
#endif

#endif // _DRV_HDMITX_H_

