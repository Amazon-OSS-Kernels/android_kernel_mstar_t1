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
#ifndef _HAL_MSPI_H_
#define _HAL_MSPI_H_

#include "MsCommon.h"
#include "drvMSPI.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSPI_READ_INDEX                0x0
#define MSPI_WRITE_INDEX               0x1
/* check if chip support MSPI*/
#define HAL_MSPI_HW_Support()          TRUE
#define DEBUG_MSPI(debug_level, x)     do { if (_u8MSPIDbgLevel >= (debug_level)) (x); } while(0)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum _HAL_CLK_Config_P1
{
    E_MSPI_CLK_P1_27M = 27,
    E_MSPI_CLK_P1_48M  = 48,
    E_MSPI_CLK_P1_62M  = 62,
    E_MSPI_CLK_P1_72M  = 72,
    E_MSPI_CLK_P1_86M  = 86,
    E_MSPI_CLK_P1_108M = 108,
    E_MSPI_CLK_P1_123M = 123,
    E_MSPI_CLK_P1_160M = 160,
}eCLK_config_P1;
typedef enum _HAL_CLK_Config_P2
{
    E_MSPI_CLK_P2_DIV2   = 2,
    E_MSPI_CLK_P2_DIV4   = 4,
    E_MSPI_CLK_P2_DIV8   = 8,
    E_MSPI_CLK_P2_DIV16  = 16,
    E_MSPI_CLK_P2_DIV32  = 32,
    E_MSPI_CLK_P2_DIV64  = 64,
    E_MSPI_CLK_P2_DIV128 = 128,
    E_MSPI_CLK_P2_DIV256 = 256,
}eCLK_config_P2;

typedef enum _HAL_DC_Config
{
    E_MSPI_TRSTART,
    E_MSPI_TREND,
    E_MSPI_TB,
    E_MSPI_TRW
}eDC_config;

typedef enum _HAL_CLK_Config
{
    E_MSPI_POL,
    E_MSPI_PHA,
    E_MSPI_CLK
}eCLK_config;

typedef struct
{
    MS_U8 u8ClkSpi_P1;
    MS_U8 u8ClkSpi_P2;
    MS_U8 u8ClkSpi_DIV;
    MS_U32 u32ClkSpi;
}ST_DRV_MSPI_CLK;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
extern MS_U8 _u8MSPIDbgLevel;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Description : MSPI initial
/// @return void:
//------------------------------------------------------------------------------
void HAL_MSPI_Init(void);

//------------------------------------------------------------------------------
/// Description : MSPI    interrupt enable
/// @param bEnable \b OUT: enable or disable mspi interrupt
/// @return void:
//------------------------------------------------------------------------------
void HAL_MSPI_IntEnable(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description : Set MSPI chip select
/// @param u8CS \u8 OUT: MSPI chip select
/// @return void:
//------------------------------------------------------------------------------
void HAL_MSPI_SetChipSelect(MS_U8 u8CS);

//------------------------------------------------------------------------------
/// Description : Config MSP MMIO base address
/// @param u32PMBankBaseAddr \b IN :base address of MMIO (PM domain)
/// @param u32NONPMRegBaseAddr \b IN :base address of MMIO
/// @param u8DeviceIndex \b IN: index of HW IP
//------------------------------------------------------------------------------
void HAL_MSPI_MMIOConfig(MS_U32 u32PMBankBaseAddr, MS_U32 u32NONPMRegBaseAddr, MS_U8 u8DeviceIndex);

//-------------------------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b IN :pointer to receive data from MSPI read buffer
/// @param u16Size \ b OTU : read data size
/// @return TRUE  : read data success
/// @return FALSE : read data fail
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Read(MS_U8 *pData, MS_U16 u16Size);

//------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b OUT :pointer to write  data to MSPI write buffer
/// @param u16Size \ b OTU : write data size
/// @return TRUE  : write data success
/// @return FALSE : wirte data fail
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Wirte(MS_U8 *pData, MS_U16 u16Size);

//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param eDCField    \enum OUT  type of DC timing
/// @param u8DCtiming \u8 OUT timing of eDCField
//------------------------------------------------------------------------------
void HAL_MSPI_SetDcTiming (eDC_config eDCField, MS_U8 u8DCtiming);

//------------------------------------------------------------------------------
/// Description : config spi clock setting
/// @param eCLKField    \enum OUT  type of Clock setting
/// @param u8DCtiming \u8 OUT setting of eCLKField
//------------------------------------------------------------------------------
void HAL_MSPI_SetCLKTiming(eCLK_config eCLKField, MS_U8 u8CLKVal);

//------------------------------------------------------------------------------
/// Description : config spi per-buffer size
/// @param bDirect              \b direction of operation(read/write)
/// @param u8BufOffset       \u8 offset of per-buffer size register
/// @param u8PerFrameSize \u8 setting of per-buffer size
//------------------------------------------------------------------------------
void HAL_MSPI_SetPerFrameSize(MS_BOOL bDirect, MS_U8 u8BufOffset, MS_U8 u8PerFrameSize);

//------------------------------------------------------------------------------
/// Description : Reset  DC register setting of MSPI
/// @param NONE
/// @return TRUE  : reset complete
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Reset_DCConfig(void);

//------------------------------------------------------------------------------
/// Description : Reset  Frame register setting of MSPI
/// @param NONE
/// @return TRUE  : reset complete
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Reset_FrameConfig(void);

//------------------------------------------------------------------------------
/// Description : Reset  CLK register setting of MSPI
/// @param NONE
/// @return TRUE  : reset complete
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Reset_CLKConfig(void);

//------------------------------------------------------------------------------
/// Description : get read/write buffer size
/// @param bDirection    \b OUT   specify to get read/write buffer size
/// @return buffer sizel
//------------------------------------------------------------------------------
MS_U8 HAL_MSPI_GetBufSize(MS_BOOL bDirection);

//------------------------------------------------------------------------------
/// Description : Trigger MSPI operation
/// @return TRUE  : operation success
/// @return FALSE : operation timeout
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Trigger(void);


void HAL_MSPI_SlaveEnable(MS_BOOL Enable);

MS_U8 HAL_MSPI_DCConfigMax(eDC_config eDCField);

MS_U8 HAL_MSPI_CLKConfigMax(eCLK_config eCLKField);

MS_U8 HAL_MSPI_FrameConfigMax(void);

MS_U8 HAL_MSPI_ChipSelectMax(void);

MS_BOOL HAL_MSPI_CLOCK_Config(MS_U32 u32MaxClock);

//mspi for local dimming clk config
MS_BOOL HAL_MSPI_CLK_Config(MS_U8 u8Chanel,MS_U32 u32MspiClk);
//mspi for local dimming DMA ,set read write trigger buffer size
MS_BOOL HAL_MSPI_RWBytes(MS_BOOL Direct, MS_U8 u8Bytes);
#endif
