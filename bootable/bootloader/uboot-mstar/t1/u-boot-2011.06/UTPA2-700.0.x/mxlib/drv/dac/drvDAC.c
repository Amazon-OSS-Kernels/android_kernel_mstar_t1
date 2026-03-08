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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvDAC.c
/// @brief  DAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#define  MDRV_DAC_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "drvDAC.h"
#include "halDAC.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

void MDrv_DAC_SetIOMapBase(MS_VIRT virtNPMBase, MS_VIRT virtPMBase)
{
    Hal_DAC_SetIOMapBase(virtNPMBase, virtPMBase);
}

void MDrv_DAC_Init(void)
{
    Hal_DAC_Power_Saving(FALSE);
    Hal_HDMITx_InitSeq();
}

void MDrv_DAC_Exit(void)
{
    Hal_DAC_Power_Saving(TRUE);
}

void MDrv_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_Enable(bEnable, bIsYPbPr);
}

void MDrv_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetClkInv(bEnable, bIsYPbPr);
}

//DAC MUX select: YPbPr -> E_DAC_TO_HDGEN; CVBS/S-Video -> E_DAC_TO_VE
void MDrv_DAC_SetOutputSource(EN_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputSource(enOutputType, bIsYPbPr);
}

//by default, YPbPr -> E_DAC_OUTPUT_LEVEL_LOW; CVBS/S-Video -> E_DAC_OUTPUT_LEVEL_HIGH
void MDrv_DAC_SetOutputLevel(EN_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputLevel(enLevel, bIsYPbPr);
}

void MDrv_DAC_SetOutputSwapSel(EN_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputSwapSel(enSwap,bIsYPbPr);
}

void MDrv_DAC_SetOutputTiming(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming)
{
    Hal_HDGEN_SetTiming(enTiming);
}

void MDrv_ClkSel_Set(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming, EN_OUTPUT_BIT_TYPE enBits)
{
    //Hal_ClkSel_Set(enTiming, enBits);
}

void MDrv_DAC_OnOffSD(EN_DAC_SD_ENABLE_CTRL enBit)
{
    Hal_DAC_OnOffSD(enBit);
}

void MDrv_DAC_OnOffHD(EN_DAC_HD_ENABLE_CTRL enBit)
{
    Hal_DAC_OnOffHD(enBit);
}

void MDrv_DAC_DumpTable(MS_U8 *pDACTable, MS_U8 u8DACtype)
{
    Hal_DAC_DumpTable(pDACTable, u8DACtype);
}


//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetIHalfOutput(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// DAC - set quart output current
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetQuartOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetQuartOutput(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_SetDACState - Set DAC init state
// @param: bEnable: TRUE for DAC is initialized, FALSE for not initialized
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetDACState(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetDACState(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_GetDACState - Get DAC init state
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return: TRUE is DAC is initialized
//----------------------------------------------------------------
MS_BOOL MDrv_DAC_GetDACState(MS_BOOL bIsYPbPr)
{
    return Hal_DAC_GetDACState(bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MDrv_DAC_HotPlugDetect(EN_DAC_DETECT SelDAC, EN_DAC_DETECT_TYPE DetectType, MS_BOOL *State)
{
    return Hal_DAC_HotPlugDetect(SelDAC, DetectType, State);
}
