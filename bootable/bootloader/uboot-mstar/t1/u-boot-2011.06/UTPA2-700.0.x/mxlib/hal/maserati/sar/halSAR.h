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

#ifndef _HAL_SAR_H_
#define _HAL_SAR_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define HAL_SAR_CH1                     0
#define HAL_SAR_CH2                     1
#define HAL_SAR_CH3                     2
#define HAL_SAR_CH4                     3
#define HAL_SAR_CH5                     4
#define HAL_SAR_CH6                     5
#define HAL_SAR_CH7                     6
#define HAL_SAR_CH8                     7
//sar channel max channel id
#define HAL_SAR_CH_MAXID                (HAL_SAR_CH8+1)
#define HAL_SAR_ADC_DATA_MAX            0xFF

#define HAL_SAR_LEVEL                   1
#define HAL_SAR_EDGE                    0
#define HAL_SAR_FREERUN                 1
#define HAL_SAR_ONESHOT                 0

#define HAL_SAR_KPD_SUPPCHS             5
#define HAL_SAR_KPD_AVLBCHS             5

//Support RGB ADC HSync Channel Switch
#define HAL_SAR_ADC_HSYNC_CH0           0
#define HAL_SAR_ADC_HSYNC_CH1           1
#define HAL_SAR_ADC_HSYNC_CH2           2
#define HAL_SAR_ADC_HSYNC_CH3           3
#define HAL_SAR_ADC_HSYNC_MAX           4

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef void(*HAL_SAR_IsrProc)(void* pData);

typedef struct
{
    MS_U8 u8UpBnd;  //upper bound
    MS_U8 u8LoBnd;  //low bound
} HAL_SAR_BndCfg;

typedef struct
{
    MS_U8 u8SARChID;
    HAL_SAR_BndCfg tSARChBnd;
    MS_U8 u8KeyLevelNum;
    MS_U8 u8KeyThreshold[8];
    MS_U8 u8KeyCode[8];
} HAL_SAR_RegCfg;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_SAR_SetIOMapBase(MS_VIRT virtBase);
MS_VIRT HAL_SAR_GetIOMapBase(void);
void HAL_SAR_SetIOMapBase_NPM(MS_VIRT virtBase);
MS_VIRT HAL_SAR_GetIOMapBase_NPM(void);

MS_U8 HAL_SAR_CfgChannelBound(MS_U8 u8Channel ,HAL_SAR_BndCfg *psarBndCfg);
void HAL_SAR_CfgSingleChannel(MS_U8 u8Channel);
void HAL_SAR_CfgTriggerMode(MS_U8 u8Mode);
void HAL_SAR_CfgSingleChannelEn(MS_U8 bEnable);
void HAL_SAR_CfgDigitOperMode(MS_U8 u8Mode);
void HAL_SAR_CfgDigitPowerdown(MS_U8 bEnable);
void HAL_SAR_CfgStart(MS_U8 bEnable);
void HAL_SAR_CfgAtopPowerdown(MS_U8 bEnable);
void HAL_SAR_CfgAtopFreeRun(MS_U8 bEnable);
void HAL_SAR_CfgSelection(MS_U8 bEnable);
void HAL_SAR_CfgHighChannel(MS_U8 bEnable);
void HAL_SAR_CfgClockSamplePeriod(MS_U8 u8ClkSmpPrd);
void HAL_SAR_CfgAanlogInputSelect(MS_U8 u8Channel, MS_BOOL bEnable);
void HAL_SAR_CfgOutputEnable(MS_U8 u8Channel, MS_BOOL bEnable);
void HAL_SAR_SetOutput(MS_U8 u8Channel, MS_BOOL bHighLow);
MS_BOOL HAL_SAR_GetInput(MS_U8 u8Channel);
void HAL_SAR_CfgIntMask(MS_U8 bEnable);
void HAL_SAR_CfgIntClear(MS_U8 bEnable);
void HAL_SAR_CfgIntForce(MS_U8 bEnable);
MS_U8 HAL_SAR_GetIntStatus(void);
MS_U8 HAL_SAR_GetChannelADC(MS_U8 u8Channel);
MS_BOOL HAL_SAR_SetAdcHSyncChannel(MS_U8 u8Channel);
MS_BOOL HAL_SAR_Interrupt_EN(MS_U8 u8Channel, MS_BOOL bEnable);
MS_BOOL HAL_SAR_Wakeup_EN(MS_BOOL bEnable);
void HAL_SAR_CLR_INT(MS_U8 u8Channel);
MS_BOOL HAL_SAR_INT_Status(MS_U8 u8Channel);
void HAL_SAR_SET_LEVEL(MS_BOOL bLevel);


#endif // _HAL_SAR_H_

