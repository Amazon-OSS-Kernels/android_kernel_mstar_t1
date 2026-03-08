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

#ifndef _HAL_SYS_DVFS_H_
#define _HAL_SYS_DVFS_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DVFS_INFO(x, args...)                   {printf(x, ##args);}
#define DVFS_DEBUG(x, args...)                  //{printf(x, ##args);}

#define CONFIG_DVFS_ENABLE_PATTERN              0x3697
#define CONFIG_DVFS_DYNAMIC_POWER_ADJUST_INIT   0x2454

#define CONFIG_DVFS_CPU_POWER_I2C_ENABLE        1
#define CONFIG_DVFS_CORE_POWER_I2C_ENABLE       0
#define CONFIG_DVFS_CPU_POWER_GPIO_ENABLE       0
#define CONFIG_DVFS_CORE_POWER_GPIO_ENABLE      1

#define CONFIG_DVFS_DELAY_US                    1
#define CONFIG_DVFS_MUTEX_WAIT_TIME             50

#define CONFIG_DVFS_CPU_POWER                   0
#define CONFIG_DVFS_CORE_POWER                  1
#define CONFIG_DVFS_STR_INIT                    0xFE

#define CONFIG_DVFS_CPU_POWER_SHIFT_PRADO(x)    (x ? 67 : 67)
//#define CONFIG_DVFS_CPU_POWER_SHIFT_PRADO(x)    (x ? 78 : 78)
#define CONFIG_DVFS_CPU_POWER_DEFAULT(x)        (x ? 105 : 105)
#define CONFIG_DVFS_CPU_POWER_STEP(x)           (x ? 3 : 3)

#define CONFIG_DVFS_CORE_POWER_SHIFT            66
#define CONFIG_DVFS_CORE_POWER_DEFAULT          95
#define CONFIG_DVFS_CORE_POWER_L1               90
#define CONFIG_DVFS_CORE_POWER_L2               100
#define CONFIG_DVFS_CORE_POWER_STEP             3

#define CONFIG_DVFS_POWER_SWI2C_BUS             0x00
#if (CONFIG_DVFS_CPU_POWER_I2C_ENABLE)
#define CONFIG_DVFS_POWER_SWI2C_ADDR_CPU(x)     (x ? 0x82 : 0x80)
#define CONFIG_DVFS_POWER_SWI2C_CLUSTER0        0x00
#define CONFIG_DVFS_POWER_SWI2C_CLUSTER1        0x01
#endif
#if (CONFIG_DVFS_CORE_POWER_I2C_ENABLE)
#define CONFIG_DVFS_POWER_SWI2C_CORE_ID         0x82    //0x82
#define CONFIG_DVFS_POWER_SWI2C_ADDR_CORE       ((CONFIG_DVFS_POWER_SWI2C_BUS << 8) | CONFIG_DVFS_POWER_SWI2C_CORE_ID)
#endif

#define CONFIG_DVFS_CHIP_ID_UNKNOWN             0xFF
#define CONFIG_DVFS_CHIP_ID_PRADA               0x79
#define CONFIG_DVFS_CHIP_ID_PRADO               0x9A
#define CONFIG_DVFS_CORE_POWER_SHIFT_PRADO      (69 - 2)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32  bDvfsInitOk;
    MS_U32  dwVidSetting;
    MS_U32  dwPowerChipId;

} MSTAR_DVFS_INFO;

typedef struct
{
    unsigned int    dwCpuClock;
    unsigned int    dwVoltage;
    unsigned int    dwVoltageType;
    unsigned int    dwDataExchangeCount;
    unsigned int    dwCluster;

} MSTAR_DVFS_READ_INFO;

typedef struct
{
    unsigned int    dwDataExchangeCount;
    int             dwDataExchangeResult;
    unsigned int    dwCluster;
} MSTAR_DVFS_WRITE_INFO;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void SysDvfsProc(void);
void SysDvfsInit(void);
void SysDvfsCpuPowerInit(void);
MS_BOOL SysDvfsCpuPowerAdjustment(MS_U32 dwCpuPowerVoltage, MS_U8 byCluster);
void SysDvfsCorePowerInit(void);
MS_BOOL SysDvfsCorePowerAdjustment(MS_U32 dwCorePowerVoltage, MS_U8 byCluster);

#endif // _HAL_SYS_DVFS_H_

