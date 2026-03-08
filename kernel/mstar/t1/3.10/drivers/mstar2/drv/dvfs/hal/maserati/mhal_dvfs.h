/**
* Copyright (c) 2006-2016 MStar Semiconductor, Inc.
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

#ifndef __MHAL_DVFS_H__
#define __MHAL_DVFS_H__

#ifndef __MDRV_TYPES_H__
#include "mdrv_types.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DVFS_HAL_INFO(x, args...)                       {printk(KERN_INFO x, ##args);}
#define DVFS_HAL_DEBUG(x, args...)                      {pr_debug(x, ##args); }

#if defined(CONFIG_ARM) || defined(CONFIG_MIPS)
#define CONFIG_REGISTER_BASE_ADDRESS                    0xfd000000
#elif defined(CONFIG_ARM64)
extern ptrdiff_t mstar_pm_base;
#define CONFIG_REGISTER_BASE_ADDRESS                    mstar_pm_base
#endif

#define CONFIG_DVFS_ENABLE_PATTERN                      0x3697
#define CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT           0x2454

#define CONFIG_DVFS_DATA_COUNT                          5
#define CONFIG_DVFS_AVERAGE_COUNT                       5
#define CONFIG_DVFS_AVERAGE_LOW_TEMP_COUNT              10
#define CONFIG_DVFS_AVERAGE_HIGH_TEMP_COUNT             0
#define CONFIG_DVFS_AVERAGE_PRADO_COUNT                 5
#define CONFIG_DVFS_RESET_MAX_COUNT                     5
#define CONFIG_DVFS_CLOCK_DELAY_US                      200
#define CONFIG_DVFS_MUTEX_WAIT_TIME                     50
#define CONFIG_DVFS_BOOT_MODE_TIME                      30

#define CONFIG_DVFS_CORNER_CHIP_SS                      0x00
#define CONFIG_DVFS_CORNER_CHIP_TT                      0x01
#define CONFIG_DVFS_CORNER_CHIP_FF                      0x02
#define CONFIG_DVFS_CORNER_CHIP_MAX                     0x03
#define CONFIG_DVFS_CORNER_CHIP_UNKNOWN                 0xFF

#define CONFIG_DVFS_INIT_MODE                           0x00
#define CONFIG_DVFS_FREEZE_MODE                         0x01
#define CONFIG_DVFS_NORMAL_MODE                         0x02
#define CONFIG_DVFS_OVER_TEMPERATURE_MODE               0x03
#define CONFIG_DVFS_SPECIAL_CLOCK_MODE                  0x04
#define CONFIG_DVFS_BOOT_MODE                           0x05
#define CONFIG_DVFS_LIGHT_MODE                          0x06
#define CONFIG_DVFS_CORNER_CHIP_UNKNOWN                 0xFF

#define CONFIG_DVFS_CPU_IRBOOST_CLOCK(dwCpu)            MDrvDvfsQueryCpuClock(CONFIG_DVFS_IR_BOOTS_CPU_CLOCK, dwCpu)
#define CONFIG_DVFS_CPU_CLOCK_MAX(dwCpu)                MDrvDvfsQueryCpuClock(CONFIG_DVFS_MAX_CPU_CLOCK, dwCpu)
#define CONFIG_DVFS_CPU_CLOCK_MIN(dwCpu)                MDrvDvfsQueryCpuClock(CONFIG_DVFS_MIN_CPU_CLOCK, dwCpu)
#define CONFIG_DVFS_CPU_CLOCK_SPECIAL                   100
#define CONFIG_DVFS_CPU_CLOCK_LIGHT_MODE                200
#define CONFIG_DVFS_CPU_CLOCK_DISABLE                   0xFFFF
#define CONFIG_DVFS_TEMPERATURE_DISABLE                 0xFFFF
#define CONFIG_DVFS_LIGHTMODE_DISABLE                   0xFFFF


#define CONFIG_DVFS_PACKAGE_SEGMENT                     6
#define CONFIG_DVFS_POWER_CTL_SEGMENT                   9

#define CONFIG_DVFS_T_SENSOR_SHIFT                      27000

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct  __attribute__((packed))
{
    U32     reg_vid_dvfs_id: 16;        //0x1005_00[15:0]: Identify VID Only or VID with DVFS
    U32     reg_reserved_00: 16;        //0x1005_00[31:16]: Reserved

    U32     reg_cur_cpu_clock: 12;      //0x1005_02[11:0]: Current Value of CPU Clock
    U32     reg_chip_package: 3;        //0x1005_02[13:12]: Chip Package Type
    U32     reg_reserved_01: 17;        //0x1005_02[31:14]: Reserved

    U32     reg_cur_dvfs_state: 3;      //0x1005_04[2:0]: Current DVFS Running State
    U32     reg_cpu_pwr_type: 2;        //0x1005_04[4:3]: CPU Power Type for cluster
    U32     reg_core_pwr_type: 2;       //0x1005_04[6:5]: CORE Power Type for cluster
    U32     reg_reserved_03: 1;         //0x1005_04[7:7]: Reserved
    U32     reg_cur_cpu_temp: 8;        //0x1005_04[15:8]: Current value of CPU temperature for cluster
    U32     reg_reserved_04: 16;        //0x1005_04[31:16]: Reserved

    U32     reg_special_cpu_clk: 16;    //0x1005_06[15:0]: User-defined Specific CPU Clock
    U32     reg_reserved_05: 16;        //0x1005_06[31:16]: Reserved

} REG_INFO;


typedef struct  __attribute__((packed))
{
    REG_INFO dvfs_reg[2];

} MSTAR_DVFS_REG_INFO;

typedef struct
{
    U32     dwLowerCpuClock;    //Lower Level Threshold CPU Clock
    U32     dwUpperCpuClock;    //Upper Level Threshold CPU Clock

} MSTAR_DVFS_CPU_INFO;

typedef struct
{
    U32     dwLowerGpuClock;    //Lower Level Threshold GPU Clock
    U32     dwUpperGpuClock;    //Upper Level Threshold GPU Clock

} MSTAR_DVFS_GPU_INFO;

typedef struct
{
    U32     dwCpuPower;         //The Value of CPU Power
    U32     dwCorePower;        //The Value of Core Power

} MSTAR_DVFS_POWER_INFO;

typedef struct
{
    S32     dwLowerFreezeTemperature;   //Lower Level Threshold Temperature for Freeze Mode
    S32     dwUpperFreezeTemperature;   //Upper Level Threshold Temperature for Freeze Mode
    S32     dwLowerLevelTemperature;    //Lower Level Threshold Temperature
    S32     dwUpperLevelTemperature;    //Upper Level Threshold Temperature
    S32     dwMaxLevelTemperature;      //Maximum Level Threshold Temperature

} MSTAR_DVFS_TEMPERATURE_INFO;

typedef struct
{
    MSTAR_DVFS_CPU_INFO         DvfsCpuInfo;                //CPU Clock
    MSTAR_DVFS_GPU_INFO         DvfsGpuInfo;                //GPU Clock
    MSTAR_DVFS_POWER_INFO       DvfsPowerInfo[CONFIG_DVFS_CORNER_CHIP_MAX]; //Power Settings for Corner Chip

} MSTAR_DVFS_ENTRY_INFO;

typedef struct
{
    U32     dwDefaultCpuClock;      //Default CPU Clock
    U32     dwMinimumCpuClock;      //Minimum CPU Clock
    U32     dwMaximumCpuClock;      //Maximum CPU Clock
    U32     dwProtectedCpuClock;    //Protect Mode CPU Clock
    U32     dwDefaultCpuPower;      //Default CPU Power
    U32     dwDefaultCorePower;     //Default Core Power

} MSTAR_DVFS_SYS_INFO;

typedef struct
{
    MSTAR_DVFS_SYS_INFO         DvfsSysInfo;                //System Configuration
    MSTAR_DVFS_TEMPERATURE_INFO DvfsTemperatureInfo;        //Temperature
    MSTAR_DVFS_ENTRY_INFO       DvfsInitModeInfo;           //Initial Mode
    MSTAR_DVFS_ENTRY_INFO       DvfsBootModeInfo;           //Boot Mode
	MSTAR_DVFS_ENTRY_INFO       DvfsLightModeInfo;          /* Boot Mode */
    MSTAR_DVFS_ENTRY_INFO       DvfsOverTemperatureModeInfo;//Over-temperature Mode
    MSTAR_DVFS_ENTRY_INFO       DvfsFreezeModeInfo[CONFIG_DVFS_POWER_CTL_SEGMENT];  //Freeze Mode
    MSTAR_DVFS_ENTRY_INFO       DvfsNormalModeInfo[CONFIG_DVFS_POWER_CTL_SEGMENT];  //Normal Mode

} MSTAR_DVFS_MODE_INFO;

typedef struct
{
    U32     bDvfsInitOk;
    U32     bDvfsModeChange;

    U32     dwMaxCpuClockByTemperature;
    U32     dwFinalCpuClock;

    U32     dwFinalCpuPowerVoltage;
    U32     dwFinalCorePowerVoltage;

    S32     dwCpuTemperature;
    U32     dwRefTemperature;
    U32     dwAvgCpuTempCounter;
    U32     dwAvgCpuTempBuffer;

    U32     bSystemResumeFlag;
    U32     dwResetCounter;
    U32     dwTemperatureCounter;
    U32     dwBootTimeCounter;
    U32     dwCpuPartId;
    U32     dwClusterCpuMask;
    U32     dwFreqRiuAddr;
	U32     dwSpecialCpuClkddr;
    U32     dwAnaMiscBank;
    U32     dwMcuArmBank;

	U32     dwPradoProtect;
	U32     dwPradoCounter;
    MSTAR_DVFS_MODE_INFO    DvfsModeInfo[CONFIG_DVFS_PACKAGE_SEGMENT];

} MSTAR_DVFS_INFO;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void MDrvHalDvfsInit(void);
U32  MHalDvfsProc(U32 dwCpuClock, U8 dwCpu);
U32  MHalDvfsInit(U8 dwCluster);
void MHalDvfsCpuDisplay(U8 dwCluster);
void MHalDvfsCpuTemperature(U8 dwCluster);
void MHalDvfsCpuClockAdjustment(U32 dwCpuClock, U8 dwCpu);
U32  MHalDvfsSearchCpuClockLevel(U32 dwCpuClock, U8 dwCpu);
int MHalDvfsPowerControl(U32 dwCpuClock, U8 dwCpu);
int MHalDvfsCpuPowerAdjustment(U32 dwCpuPowerVoltage, U8 dwCpu);
int MHalDvfsCorePowerAdjustment(U32 dwCorePowerVoltage, U8 dwCpu);
U32  MHalDvfsQueryCpuClock(U32 dwCpuClockType, U8 dwCpu);
U32  MHalDvfsQueryCpuClockByTemperature(U8 dwCpu);

void MHalDvfsCpuDisplayInit(U8 dwCluster);
void MHalDvfsRefTemperature(U8 dwCluster);
void MHalDvfsCpuPowerInit(U8 dwCluster);
void MHalDvfsCorePowerInit(U8 dwCluster);
int getCpuCluster(unsigned int cpu);

void MHalDvfsSetAutoMeasurement(U32 auto_measurement);

U32 MHalDvfsGetCpuTemperature(U8 dwCpu);
U32 MHalDvfsGetVoltage(U8 dwCpu);
U32 MHalDvfsGetSidd(void);
U32 MHalDvfsGetOsc(U8 dwCpu);

void MHalDvfsPradoMonitor(U8 dwCpu);

void MHalDvfs_set_high_bound_temperature(U8 dwCpu, U32 temperature);
void MHalDvfs_set_low_bound_temperature(U8 dwCpu, U32 temperature);
void MHalDvfs_set_max_bound_temperature(U8 dwCpu, U32 temperature);

#endif
