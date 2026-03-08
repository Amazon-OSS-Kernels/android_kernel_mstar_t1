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

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/delay.h>

#include <mstar/mstar_chip.h>

#include "mdrv_dvfs.h"

#ifndef __MHAL_DVFS_H__
#include "mhal_dvfs.h"
#endif

#ifndef __MHAL_DVFS_POWER_H__
#include "mhal_dvfs_power.h"
#endif

#include <linux/platform_device.h>
#include <linux/pm.h>
#include <asm/cputype.h>

#include <linux/metricslog.h>

#ifdef CONFIG_AMAZON_SIGN_OF_LIFE
#include <linux/sign_of_life.h>
#endif

DEFINE_MUTEX(MDrvDvfsCpuTempMutex);
DEFINE_MUTEX(MDrvDvfsStrMutex);
DEFINE_MUTEX(MDrvDvfsInitMutex);
extern unsigned int get_cpu_midr(int cpu);
#define CONFIG_DVFS_CPU_CLOCK_DISPLAY_ENABLE 1
extern void MDrvDvfsVoltageSetup(unsigned int dwCpuClock, unsigned int dwVoltage, unsigned int dwVoltageType, unsigned int dwCpu);
extern int voltage_change_result[CONFIG_NR_CPUS];
int halTotalClusterNumber;

static U32 gDVFS_auto_measurement=0;
static U16 pre_dvfs_state = CONFIG_DVFS_CORNER_CHIP_UNKNOWN;


static volatile MSTAR_DVFS_REG_INFO *DvfsRegInfo = 0;
static MSTAR_DVFS_INFO hMstarDvfsInfo[2] =
{
    {
        .bDvfsInitOk                    = 0,
        .bDvfsModeChange                = 0,
        .dwMaxCpuClockByTemperature     = 0,

        .dwFinalCpuClock                = 0,
        .dwFinalCpuPowerVoltage         = 0,
        .dwFinalCorePowerVoltage        = 0,

        .dwCpuTemperature               = 0,
        .dwRefTemperature               = 0,
        .dwAvgCpuTempCounter            = 0,
		.dwTempErrReading				= 0,
        .bSystemResumeFlag              = 0,
        .dwResetCounter                 = 0,
        .dwTemperatureCounter           = 0,
        .dwBootTimeCounter              = 0,
        .dwCpuPartId                    = 0xd03, //CA53
        .dwClusterCpuMask               = 0x0, //This will be used in classify_cpu_cluster for cluster mask.
		.dwFreqRiuAddr                  = 0x1f200a04, /* bus address */
		.dwSpecialCpuClkddr             = 0x1f200a0c, /* bus address */
        .dwAnaMiscBank                  = 0x110c00,
        .dwMcuArmBank                   = 0x101d00,
		.dwPradoProtect                 = 0,
		.dwPradoCounter                 = 0,

        .DvfsModeInfo =
        {
            //CA53
            {
                //Normal
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1000,
                    .dwMinimumCpuClock          = 1000,
                    .dwMaximumCpuClock          = 1300,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 105,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1000,
                        .dwUpperCpuClock        = 1000,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 105,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 105,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1200,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 110,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 1000MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1301MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1301,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 1000MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1301MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1301,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // ENG6 HCP+
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1100,
                    .dwMinimumCpuClock          = 1100,
                    .dwMaximumCpuClock          = 1400,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 100,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1100,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1200,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 113,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 113,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 108,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // ENG6 HCP
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1100,
                    .dwMinimumCpuClock          = 1100,
                    .dwMaximumCpuClock          = 1400,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 100,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1100,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1400,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 113,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 113,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 108,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },

                },
            },
            {
                // ENG4 HCP
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1000,
                    .dwMinimumCpuClock          = 1000,
                    .dwMaximumCpuClock          = 1400,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 100,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1000,
                        .dwUpperCpuClock        = 1000,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1200,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 110,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 1000MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 1000MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // raptor
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1100,
					.dwMinimumCpuClock          = 216,
                    .dwMaximumCpuClock          = 1400,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 100,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1100,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1400,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 113,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 113,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 108,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = 216,
						.dwUpperCpuClock        = 216,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 92,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 92,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 216MHz - 400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 216,
                            .dwUpperCpuClock    = 400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 400MHz - 600MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 400,
                            .dwUpperCpuClock    = 600,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 600MHz - 1000MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 600,
                            .dwUpperCpuClock    = 1000,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1000MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 216MHz - 400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 216,
                            .dwUpperCpuClock    = 400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 400MHz - 600MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 400,
                            .dwUpperCpuClock    = 600,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 600MHz - 1000MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 600,
                            .dwUpperCpuClock    = 1000,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1000MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // amazon
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1100,
                    .dwMinimumCpuClock          = 1100,
                    .dwMaximumCpuClock          = 1400,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 100,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1100,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1400,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 113,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 113,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 108,
                            .dwCorePower        = 92,
                        },
                    },
                },
					.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 216MHz - 400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 216,
                            .dwUpperCpuClock    = 400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 400MHz - 600MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 400,
                            .dwUpperCpuClock    = 600,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 600MHz - 1000MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 600,
                            .dwUpperCpuClock    = 1000,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1000MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 216MHz - 400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 216,
                            .dwUpperCpuClock    = 400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 400MHz - 600MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 400,
                            .dwUpperCpuClock    = 600,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 600MHz - 1000MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 600,
                            .dwUpperCpuClock    = 1000,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1000MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
        },
    },
    {
        .bDvfsInitOk = 0,
        .bDvfsModeChange = 0,

        .dwMaxCpuClockByTemperature = 0,
        .dwFinalCpuClock = 0,

        .dwFinalCpuPowerVoltage = 0,
        .dwFinalCorePowerVoltage = 0,

        .dwCpuTemperature = 0,
        .dwRefTemperature = 0,
        .dwAvgCpuTempCounter = 0,

        .bSystemResumeFlag = 0,
        .dwResetCounter = 0,
        .dwTemperatureCounter = 0,
	.dwTempErrReading = 0,
		.dwBootTimeCounter = 0,
        .dwCpuPartId       = 0xd08, //CA72
        .dwClusterCpuMask  = 0x0, //This will be used in classify_cpu_cluster for cluster mask.
		.dwFreqRiuAddr                  = 0x1f200a14, /* bus address */
		.dwSpecialCpuClkddr             = 0x1f200a1c, /* bus address */
        .dwAnaMiscBank = 0x111600,
        .dwMcuArmBank  = 0x110900,
		.dwPradoProtect = 0,
		.dwPradoCounter = 0,

        .DvfsModeInfo =
        {
            //CA72
            {
                //Normal
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 850,
                    .dwMinimumCpuClock          = 850,
                    .dwMaximumCpuClock          = 1100,
                    .dwProtectedCpuClock        = 750,
                    .dwDefaultCpuPower          = 105,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 105,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 105,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 950,
                        .dwUpperCpuClock        = 1000,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 110,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 750,
                        .dwUpperCpuClock        = 750,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 100,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 850MHz - 950MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 850,
                            .dwUpperCpuClock    = 950,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 950MHz - 1000MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 950,
                            .dwUpperCpuClock    = 1000,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1000MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1100MHz - 1101MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1101,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 850MHz - 950MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 850,
                            .dwUpperCpuClock    = 950,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 100,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 950MHz - 1000MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 950,
                            .dwUpperCpuClock    = 1000,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 105,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1000MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1100MHz - 1101MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1101,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // ENG6 HCP+
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1100,
                    .dwMinimumCpuClock          = 1100,
                    .dwMaximumCpuClock          = 1740,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 103,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1100,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 98,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1200,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 110,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1740MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1740,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1740MHz - 1741MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1740,
                            .dwUpperCpuClock    = 1741,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 140,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 140,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 135,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1740MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1740,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1740MHz - 1741MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1740,
                            .dwUpperCpuClock    = 1741,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 140,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 140,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 135,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // ENG6 HCP
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1100,
                    .dwMinimumCpuClock          = 1100,
                    .dwMaximumCpuClock          = 1900,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 103,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
					.dwLowerLevelTemperature    = 110,
					.dwUpperLevelTemperature    = 125,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1100,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 98,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1400,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 110,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1700MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1700,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1900MHz - 1901MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1700,
                            .dwUpperCpuClock    = 1901,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 150,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 150,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 150,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1700MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1700,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1700MHz - 1701MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1700,
                            .dwUpperCpuClock    = 1901,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 135,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 135,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 130,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // ENG4 HCP
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1000,
                    .dwMinimumCpuClock          = 1000,
                    .dwMaximumCpuClock          = 1400,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 103,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature = CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1000,
                        .dwUpperCpuClock        = 1000,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 98,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1200,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 125,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 125,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 120,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 1000MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 125,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 125,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 1000MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1000,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 125,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 125,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 120,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // raptor
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1100,
					.dwMinimumCpuClock          = 216,
                    .dwMaximumCpuClock          = 1700,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 103,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1100,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 98,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1400,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 110,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = 216,
						.dwUpperCpuClock        = 216,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 92,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 92,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 216MHz - 400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 216,
                            .dwUpperCpuClock    = 400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 400MHz - 600MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 400,
                            .dwUpperCpuClock    = 600,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 600MHz - 900MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 600,
                            .dwUpperCpuClock    = 900,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 900MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 900,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1700MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1700,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1900MHz - 1901MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1700,
                            .dwUpperCpuClock    = 1901,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Freeze Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 150,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 150,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 150,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 216MHz - 400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 216,
                            .dwUpperCpuClock    = 400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 400MHz - 600MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 400,
                            .dwUpperCpuClock    = 600,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 600MHz - 900MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 600,
                            .dwUpperCpuClock    = 900,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 900MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 900,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1700MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1700,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1700MHz - 1701MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1700,
                            .dwUpperCpuClock    = 1701,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            //Normal Mode
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 135,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 135,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 130,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
            {
                // amazon
                .DvfsSysInfo =
                {
                    .dwDefaultCpuClock          = 1100,
                    .dwMinimumCpuClock          = 1100,
                    .dwMaximumCpuClock          = 1400,
                    .dwProtectedCpuClock        = 850,
                    .dwDefaultCpuPower          = 103,
                    .dwDefaultCorePower         = 97,
                },
                .DvfsTemperatureInfo =
                {
                    .dwLowerFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwUpperFreezeTemperature   = CONFIG_DVFS_TEMPERATURE_DISABLE, //CONFIG_DVFS_TEMPERATURE_DISABLE,
                    .dwLowerLevelTemperature    = 120,
                    .dwUpperLevelTemperature    = 135,
                    .dwMaxLevelTemperature      = 160,
                },
                .DvfsInitModeInfo =
                {
                    //Initial Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1100,
                        .dwUpperCpuClock        = 1100,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 103,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 98,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsBootModeInfo =
                {
                    //Boot Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 1400,
                        .dwUpperCpuClock        = 1400,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 115,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 110,
                            .dwCorePower        = 92,
                        },
                    },
                },
				.DvfsLightModeInfo = {
					.DvfsCpuInfo = {
						.dwLowerCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
						.dwUpperCpuClock        = CONFIG_DVFS_LIGHTMODE_DISABLE,
					},
					.DvfsGpuInfo = {
						.dwLowerGpuClock        = 550,
						.dwUpperGpuClock        = 550,
					},
					.DvfsPowerInfo = {
						{
							/* SS Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* TT Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
						{
							/* FF Corner Chip */
							.dwCpuPower         = 85,
							.dwCorePower        = 85,
						},
					},
				},
                .DvfsOverTemperatureModeInfo =
                {
                    //Over Temperature Mode
                    .DvfsCpuInfo =
                    {
                        .dwLowerCpuClock        = 850,
                        .dwUpperCpuClock        = 850,
                    },
                    .DvfsGpuInfo =
                    {
                        .dwLowerGpuClock        = 550,
                        .dwUpperGpuClock        = 550,
                    },
                    .DvfsPowerInfo =
                    {
                        {
                            //SS Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //TT Corner Chip
                            .dwCpuPower         = 95,
                            .dwCorePower        = 97,
                        },
                        {
                            //FF Corner Chip
                            .dwCpuPower         = 90,
                            .dwCorePower        = 92,
                        },
                    },
                },
                .DvfsFreezeModeInfo =
                {
                    {
                        //Freeze Mode: 216MHz - 400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 216,
                            .dwUpperCpuClock    = 400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 400MHz - 600MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 400,
                            .dwUpperCpuClock    = 600,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 600MHz - 900MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 600,
                            .dwUpperCpuClock    = 900,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 900MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 900,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Freeze Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
                .DvfsNormalModeInfo =
                {
                    {
                        //Normal Mode: 216MHz - 400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 216,
                            .dwUpperCpuClock    = 400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 400MHz - 600MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 400,
                            .dwUpperCpuClock    = 600,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 600MHz - 900MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 600,
                            .dwUpperCpuClock    = 900,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 85,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 900MHz - 1100MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 900,
                            .dwUpperCpuClock    = 1100,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 95,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1100MHz - 1200MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1100,
                            .dwUpperCpuClock    = 1200,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 98,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1200MHz - 1300MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1200,
                            .dwUpperCpuClock    = 1300,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 103,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1300MHz - 1400MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1300,
                            .dwUpperCpuClock    = 1400,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 113,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 108,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                    {
                        //Normal Mode: 1400MHz - 1401MHz
                        .DvfsCpuInfo =
                        {
                            .dwLowerCpuClock    = 1400,
                            .dwUpperCpuClock    = 1401,
                        },
                        .DvfsGpuInfo =
                        {
                            .dwLowerGpuClock    = 550,
                            .dwUpperGpuClock    = 550,
                        },
                        .DvfsPowerInfo =
                        {
                            {
                                //SS Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //TT Corner Chip
                                .dwCpuPower     = 115,
                                .dwCorePower    = 97,
                            },
                            {
                                //FF Corner Chip
                                .dwCpuPower     = 110,
                                .dwCorePower    = 92,
                            },
                        },
                    },
                },
            },
        },
    },
};

#define DVFS_HAL_TOTAL_CLUSTER_NUM  (sizeof(hMstarDvfsInfo)/sizeof(MSTAR_DVFS_INFO))

static struct platform_device mstar_dvfs_dev = {
	.name   = "mstar_dvfs",
	.id     = 0,
};

#if defined(CONFIG_AMAZON_METRICS_LOG)
#define NORMAL_THERMAL_REPORT_INTERVAL_MINS 60
#define OVER_TEMP_THERMAL_REPORT_INTERVAL_MINS 5
#define METRICS_BUF_SIZE 512

static U32 prev_mode;
static struct timespec last_reported_time;
static struct timespec curr_time;

static temp_log_metrics(S32 temperature, U32 mode, U8 type)
{
    char buf[512];
    char mode_name[32];
    int throttling_state = 0;
	if (type == 0) {
		switch (mode) {
		case CONFIG_DVFS_INIT_MODE:
			snprintf(mode_name, sizeof(mode_name), "%s", "DVFS_INIT_MODE");
			break;
		case CONFIG_DVFS_FREEZE_MODE:
			snprintf(mode_name, sizeof(mode_name), "%s", "DVFS_FREEZE_MODE");
			break;
		case CONFIG_DVFS_NORMAL_MODE:
			snprintf(mode_name, sizeof(mode_name), "%s", "DVFS_NORMAL_MODE");
			break;
		case CONFIG_DVFS_OVER_TEMPERATURE_MODE:
			snprintf(mode_name, sizeof(mode_name), "%s", "DVFS_OVER_TEMPERATURE_MODE");
			throttling_state = 1;
			break;
		default:
			snprintf(mode_name, sizeof(mode_name), "%s", "unknown mode");
			break;
		}
		snprintf(buf, sizeof(buf),
			"Thermal:def:Throttle %s_trip%d temperature=%d;CT;1:NR",
			mode_name, throttling_state, temperature);
	} else
		snprintf(buf, sizeof(buf),
		"Thermal:def:Shutdown SOC_Overheated_Thermal_Shutdown temperature = %d;CT;1:NR",
		temperature);

    log_to_metrics(ANDROID_LOG_INFO, "ThermalEvent", buf);
}
#endif

//=================================================================================================
int getCpuCluster(unsigned int cpu)
{
    U8 cluster = 0;
    for (cluster = 0; cluster < DVFS_HAL_TOTAL_CLUSTER_NUM; cluster ++)
    {
        if (hMstarDvfsInfo[cluster].dwClusterCpuMask & (0x01 << cpu))
        {
            return cluster;
        }
    }

    printk("%s %d: ERROR can't find cluster:%d\n",__func__,__LINE__, cluster);
    return cluster;
}

//=================================================================================================
int getClusterMainCpu(unsigned int cpu)
{
    int cluster = getCpuCluster(cpu);
    int i = 0;
    for (i = 0; i < CONFIG_NR_CPUS; i ++)
    {
        if (hMstarDvfsInfo[cluster].dwClusterCpuMask >> i & 0x1)
        {
            return i;
        }
    }

    printk("%s %d: cpu:%d ERROR can't find master cpu\n",__func__,__LINE__, cpu);
    //only one cluster, return 0
    return 0;
}

//=================================================================================================
U32 getFreqRiuAddr(unsigned int cpu)
{
    int cluster = getCpuCluster(cpu);

    return hMstarDvfsInfo[cluster].dwFreqRiuAddr;
}

//=================================================================================================
U32 MHalDvfsProc(U32 dwCpuClock, U8 dwCpu)
{

    int    dwCluster = 0;
	mutex_lock(&MDrvDvfsStrMutex);
	dwCluster = getCpuCluster(dwCpu);
	if (hMstarDvfsInfo[dwCluster].bDvfsInitOk == 0) {
		pr_info("dvfs is not init, init it firstly\n");
		if (MHalDvfsInit(dwCluster) == TRUE) {
			DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_INIT_MODE;
		}
	}
    if (DvfsRegInfo == NULL) {
        DvfsRegInfo = (volatile MSTAR_DVFS_REG_INFO *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100500 << 1));
    }

    //Check Specific Register to Check DVFS Running State (0x1005_00 = 0x3697)
    if(DvfsRegInfo->dvfs_reg[dwCluster].reg_vid_dvfs_id == CONFIG_DVFS_ENABLE_PATTERN)
    {
        if(hMstarDvfsInfo[dwCluster].bDvfsInitOk == 0)
        {
            //Initial DVFS Default Settings and Data Structure
            if(MHalDvfsInit(dwCluster) == TRUE)
            {
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_INIT_MODE;
            }
            else
            {
				hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
                goto _MHalDvfsProcExit;
            }
        }

        //Get CPU Temperature by PM_SAR
		/* MHalDvfsCpuTemperature(dwCluster); */

        if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_INIT_MODE)
        {
            //Initial Mode
            //Use Default CPU Clock in Init Mode
            MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsInitModeInfo.DvfsCpuInfo.dwLowerCpuClock, dwCpu);
            if(hMstarDvfsInfo[dwCluster].dwCpuTemperature >
               hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperLevelTemperature)
            {
                //Change to Over-Temperature Mode
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_OVER_TEMPERATURE_MODE;
            }
            else
            {
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_BOOT_MODE;  //CONFIG_DVFS_NORMAL_MODE;
            }

			hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
        }
        else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_BOOT_MODE)
        {
            //Boot Mode
            if(hMstarDvfsInfo[dwCluster].dwBootTimeCounter > CONFIG_DVFS_BOOT_MODE_TIME)
            {
                //Change to Normal Mode
                hMstarDvfsInfo[dwCluster].dwBootTimeCounter = 0;

                //Use Default CPU Clock in Boot Mode
                MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsBootModeInfo.DvfsCpuInfo.dwLowerCpuClock, dwCpu);

                //Change to Normal Mode
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;

				hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
            }
            else
            {
                //Use Maximum CPU Clock in Boot Mode
                MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsBootModeInfo.DvfsCpuInfo.dwUpperCpuClock, dwCpu);
				hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
            }
        }
        else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_OVER_TEMPERATURE_MODE)
        {
#if defined(CONFIG_AMAZON_METRICS_LOG)
			struct timespec delta_time;
			char buf[METRICS_BUF_SIZE];
			getnstimeofday(&curr_time);
			delta_time = timespec_sub(curr_time, last_reported_time);
			/* Periodically log thermal value every OVER_TEMP_THERMAL_REPORT_INTERVAL_MINS minutes  */
			if (abs(delta_time.tv_sec) >= OVER_TEMP_THERMAL_REPORT_INTERVAL_MINS*60) {
				/*Report to Thermal metrics*/
				snprintf(buf, sizeof(buf), "thermzone:def:soc=%d;CT;1;NR",
						hMstarDvfsInfo[dwCluster].dwCpuTemperature);
				log_to_metrics(ANDROID_LOG_INFO, "ThermalEvent", buf);
				last_reported_time = curr_time;
			}
#endif
            //Over-Temperature Mode
            if(hMstarDvfsInfo[dwCluster].dwCpuTemperature >
               hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwMaxLevelTemperature)
            {
                //Retry 10 Times to Confirm the State of Over Temperature
                if(hMstarDvfsInfo[dwCluster].dwResetCounter < CONFIG_DVFS_RESET_MAX_COUNT)
                {
                    DVFS_HAL_DEBUG("\033[1;31m[DVFS] Over Temperature Protection: Count = %d / Temperature = %d\033[0m\n",
                        (unsigned int) hMstarDvfsInfo[dwCluster].dwResetCounter, (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);

#if defined(CONFIG_AMAZON_METRICS_LOG)
					if (prev_mode != CONFIG_DVFS_OVER_TEMPERATURE_MODE) {
						temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
						prev_mode = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state;
					}
#endif
                    hMstarDvfsInfo[dwCluster].dwResetCounter ++;
                }
                else
                {
                    DVFS_HAL_INFO("\033[1;31m[DVFS] Current Temperature: %d\033[0m\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                    DVFS_HAL_INFO("\033[1;31m[DVFS] Over Temperature Mode: SYSTEM RESET\033[0m\n");

#ifdef CONFIG_AMAZON_SIGN_OF_LIFE
					pr_err("Over temperature %d, Reboot System!!! \n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
					life_cycle_set_boot_reason(WARMBOOT_BY_SOC_OVER_TEMP);
#endif

#if defined(CONFIG_AMAZON_METRICS_LOG)
					temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, 0, 1);
#endif

                    //Trigger a WDT Reset
                    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00300a << 1)) = 0x00;
                    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x003008 << 1)) = 0x00;
                    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00300a << 1)) = 0x05;
                    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x003000 << 1)) = 0x01;

                    while(1);
                }
			} else {
                //Keep at Over-Temperature Mode
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_OVER_TEMPERATURE_MODE;
#if defined(CONFIG_AMAZON_METRICS_LOG)
				if (prev_mode != CONFIG_DVFS_OVER_TEMPERATURE_MODE) {
				temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
				prev_mode = CONFIG_DVFS_OVER_TEMPERATURE_MODE;
			}
#endif

                DVFS_HAL_DEBUG("\033[1;31m[DVFS] Current Temperature: %d\033[0m\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                DVFS_HAL_DEBUG("\033[1;31m[DVFS] Over-Temperature Mode: CPU Clock: %dMHz\033[0m\n", hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwProtectedCpuClock);

                MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwProtectedCpuClock, dwCpu);
				hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;

                hMstarDvfsInfo[dwCluster].dwResetCounter = 0;
            }
        }
        else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_FREEZE_MODE)
        {
            //Freeze Mode
            if((hMstarDvfsInfo[dwCluster].dwCpuTemperature >= hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperFreezeTemperature) && \
               (hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperFreezeTemperature != CONFIG_DVFS_TEMPERATURE_DISABLE))
            {
                //Return to Normal Mode
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;
#if defined(CONFIG_AMAZON_METRICS_LOG)
				if (prev_mode != CONFIG_DVFS_NORMAL_MODE) {
					temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
					prev_mode = CONFIG_DVFS_NORMAL_MODE;
				}
#endif

                DVFS_HAL_DEBUG("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                DVFS_HAL_DEBUG("[DVFS] Normal Mode: CPU Clock: %dMHz\n", dwCpuClock);
            }
            else
            {
                //Keep at Freeze Mode
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_FREEZE_MODE;
#if defined(CONFIG_AMAZON_METRICS_LOG)
				if (prev_mode != CONFIG_DVFS_FREEZE_MODE) {
					temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
					prev_mode = CONFIG_DVFS_FREEZE_MODE;
				}
#endif

                DVFS_HAL_DEBUG("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                DVFS_HAL_DEBUG("[DVFS] Freeze Mode: CPU Clock: %dMHz\n", dwCpuClock);
            }

            //dwCpuClock = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMaximumCpuClock;

            MHalDvfsCpuClockAdjustment(dwCpuClock, dwCpu);
			hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
		} else if (DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_LIGHT_MODE) {
			/*
			printk(KERN_DEBUG"\033[32m[DVFS] eason.lien [%s, %d]\033[0m\n", __func__, __LINE__);
			*/
			/* Light Mode */
			MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo
				->dvfs_reg[dwCluster].reg_chip_package].DvfsLightModeInfo.DvfsCpuInfo
				.dwLowerCpuClock, dwCpu);
			hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;

		} else {
            //Normal Mode
#if defined(CONFIG_AMAZON_METRICS_LOG)
		struct timespec delta_time;
		char buf[METRICS_BUF_SIZE];
		getnstimeofday(&curr_time);
		delta_time = timespec_sub(curr_time, last_reported_time);
		/* Periodically log thermal value every NORMAL_THERMAL_REPORT_INTERVAL_MINS minutes  */
		if (abs(delta_time.tv_sec) >= NORMAL_THERMAL_REPORT_INTERVAL_MINS*60) {
			/* Report to Thermal metrics */
			snprintf(buf, sizeof(buf), "thermzone:def:soc=%d;CT;1;NR",
						hMstarDvfsInfo[dwCluster].dwCpuTemperature);
			log_to_metrics(ANDROID_LOG_INFO, "ThermalEvent", buf);
			last_reported_time = curr_time;
		}
#endif
            if(hMstarDvfsInfo[dwCluster].dwCpuTemperature >=
               hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperLevelTemperature)
            {

                //Change to Over-Temperature Mode
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_OVER_TEMPERATURE_MODE;
#if defined(CONFIG_AMAZON_METRICS_LOG)
				if (prev_mode != CONFIG_DVFS_OVER_TEMPERATURE_MODE) {
					temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
					prev_mode = CONFIG_DVFS_OVER_TEMPERATURE_MODE;
				}
#endif


                DVFS_HAL_DEBUG("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                DVFS_HAL_DEBUG("[DVFS] Over-Temperature Mode: CPU Clock: %dMHz\n", hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwProtectedCpuClock);

                MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwProtectedCpuClock, dwCpu);

				hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
            }
            else if((hMstarDvfsInfo[dwCluster].dwCpuTemperature < hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwLowerFreezeTemperature) && \
                    (hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwLowerFreezeTemperature != CONFIG_DVFS_TEMPERATURE_DISABLE))
            {
                //Change to Freeze Mode
                DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_FREEZE_MODE;
#if defined(CONFIG_AMAZON_METRICS_LOG)
				if (prev_mode != CONFIG_DVFS_FREEZE_MODE) {
					temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
					prev_mode = CONFIG_DVFS_FREEZE_MODE;
				}
#endif
                DVFS_HAL_DEBUG("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                DVFS_HAL_DEBUG("[DVFS] Freeze Mode: CPU Clock: %dMHz\n", dwCpuClock);

                MHalDvfsCpuClockAdjustment(dwCpuClock, dwCpu);
				hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
            }
            else
            {
                if(DvfsRegInfo->dvfs_reg[dwCluster].reg_special_cpu_clk != CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT)
                {
                    U32 dwRegisterValue = 0;

                    dwRegisterValue = DvfsRegInfo->dvfs_reg[dwCluster].reg_special_cpu_clk;
					if ((dwRegisterValue == CONFIG_DVFS_CPU_CLOCK_LIGHT_MODE)  &&
						(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster]
						.reg_chip_package].DvfsLightModeInfo.DvfsCpuInfo.dwLowerCpuClock
						!= CONFIG_DVFS_LIGHTMODE_DISABLE)) {
						/*
						printk(KERN_DEBUG"\033[32m[DVFS] eason.lien [%s, %d] \033[0m\n",
							__func__, __LINE__);
						*/
						DvfsRegInfo->dvfs_reg[dwCluster]
							.reg_special_cpu_clk = CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT;
						DvfsRegInfo->dvfs_reg[dwCluster]
							.reg_cur_dvfs_state = CONFIG_DVFS_LIGHT_MODE;
						MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster]
							.DvfsModeInfo[DvfsRegInfo
							->dvfs_reg[dwCluster].reg_chip_package]
							.DvfsLightModeInfo.DvfsCpuInfo
							.dwLowerCpuClock, dwCpu);
						hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;

					} else if ((dwRegisterValue >= hMstarDvfsInfo[dwCluster]
							.DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster]
							.reg_chip_package].DvfsSysInfo.dwMinimumCpuClock) &&
                       (dwRegisterValue <= hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMaximumCpuClock))
                    {
                        //Special Clock Mode
                        DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_SPECIAL_CLOCK_MODE;

                        DVFS_HAL_DEBUG("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                        DVFS_HAL_DEBUG("[DVFS] Special Clock Mode: CPU Clock: %dMHz\n", dwRegisterValue);

                        MHalDvfsCpuClockAdjustment(dwRegisterValue, dwCpu);

                        DvfsRegInfo->dvfs_reg[dwCluster].reg_special_cpu_clk = CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT;
						hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
                    }
                    else
                    {
                        U32 dwClockLevel = 0;
                        //Return to  Normal Mode
                        DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;

                        dwClockLevel = MHalDvfsSearchCpuClockLevel(dwCpuClock, dwCpu);
#if defined(CONFIG_AMAZON_METRICS_LOG)
						if (prev_mode != CONFIG_DVFS_NORMAL_MODE) {
							temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
							prev_mode = CONFIG_DVFS_NORMAL_MODE;
						}
#endif
                        //DVFS_HAL_DEBUG("[DVFS] dwClockLevel: %d dwCpuClock: %d \n", dwClockLevel, dwCpuClock);
                        DVFS_HAL_DEBUG("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                        DVFS_HAL_DEBUG("[DVFS] Normal Mode: CPU Clock: %dMHz\n", hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwClockLevel].DvfsCpuInfo.dwLowerCpuClock);


                        MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwClockLevel].DvfsCpuInfo.dwLowerCpuClock, dwCpu);

                        DvfsRegInfo->dvfs_reg[dwCluster].reg_special_cpu_clk = CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT;
						hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
                    }
                }
                else
                {
                    U32 dwClockLevel = 0;
                    //Keep at Normal Mode
                    DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;

#if defined(CONFIG_AMAZON_METRICS_LOG)
					if (prev_mode != CONFIG_DVFS_NORMAL_MODE) {
						temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
						prev_mode = CONFIG_DVFS_NORMAL_MODE;
					}
#endif
                    dwClockLevel = MHalDvfsSearchCpuClockLevel(dwCpuClock, dwCpu);
                    DVFS_HAL_DEBUG("[DVFS] dwClockLevel: %d dwCpuClock: %d \n", dwClockLevel, dwCpuClock);
                    DVFS_HAL_DEBUG("[DVFS] Current Temperature: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                    DVFS_HAL_DEBUG("[DVFS] Normal Mode: CPU Clock: %dMHz\n", hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwClockLevel].DvfsCpuInfo.dwLowerCpuClock);

                    MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwClockLevel].DvfsCpuInfo.dwLowerCpuClock, dwCpu);

                    DvfsRegInfo->dvfs_reg[dwCluster].reg_special_cpu_clk = CONFIG_DVFS_DYNAMIC_CLOCK_ADJUST_INIT;
					hMstarDvfsInfo[dwCluster].dwFinalCpuClock = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock;
                }
            }
        }

        //MsOS_ReleaseMutex(_s32SAR_Dvfs_Mutex);
    }
    else
    {
        //Disable DVFS
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xb0) << 1)) = 0;
    }

_MHalDvfsProcExit:

//  hMstarDvfsInfo[dwCluster].dwFinalCpuClock = dwCpuClock;// = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->reg_chip_package].DvfsSysInfo.dwDefaultCpuClock;
    DVFS_HAL_DEBUG("[DVFS] Current DVFS State: %d\n", (unsigned int) DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state);

	char data[25];
	char *envp[] = {data, NULL};
	bool stateChanged = (pre_dvfs_state != DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state);
	bool stateWatched = (CONFIG_DVFS_NORMAL_MODE == DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state
						|| CONFIG_DVFS_OVER_TEMPERATURE_MODE == DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state);
	if (stateChanged && stateWatched) {
		pre_dvfs_state = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state;
		int thermalStateValue = (CONFIG_DVFS_NORMAL_MODE == DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state) ? 0 : 1;
		snprintf(data, sizeof(data), "THERMAL_STATE=%d", thermalStateValue);
		kobject_uevent_env(&mstar_dvfs_dev.dev.kobj, KOBJ_CHANGE, envp);
		pr_debug("[DVFS] DVFS Thermal State change (%d)\n", thermalStateValue);
#if defined(CONFIG_AMAZON_METRICS_LOG)
	temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
	prev_mode = pre_dvfs_state;
#endif

	}

    DVFS_HAL_DEBUG("[DVFS] hMstarDvfsInfo[%d].dwFinalCpuClock: %d\n", dwCluster, hMstarDvfsInfo[dwCluster].dwFinalCpuClock);
	mutex_unlock(&MDrvDvfsStrMutex);
    return hMstarDvfsInfo[dwCluster].dwFinalCpuClock;
}

//=================================================================================================
U32 MHalDvfsInit(U8 dwCluster)
{
    U32 bDvfsInitStatus = TRUE;
	mutex_lock(&MDrvDvfsInitMutex);
	if (hMstarDvfsInfo[dwCluster].bDvfsInitOk == 1) {
		pr_info("DVFS is inited already, return directly\n");
		mutex_unlock(&MDrvDvfsInitMutex);
		return bDvfsInitStatus;
	}

    if(DvfsRegInfo == NULL)
    {
        DvfsRegInfo = (volatile MSTAR_DVFS_REG_INFO *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100500 << 1));
    }

    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xb2) << 1)) &= ~(0x01);

    hMstarDvfsInfo[dwCluster].bDvfsInitOk = 0;
    hMstarDvfsInfo[dwCluster].bDvfsModeChange = 0;
    hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = 0;
    hMstarDvfsInfo[dwCluster].dwFinalCpuClock = 0;
    hMstarDvfsInfo[dwCluster].dwFinalCpuPowerVoltage = 0;
    hMstarDvfsInfo[dwCluster].dwFinalCorePowerVoltage = 0;
    hMstarDvfsInfo[dwCluster].dwCpuTemperature = 0;
    hMstarDvfsInfo[dwCluster].dwRefTemperature = 0;
	hMstarDvfsInfo[dwCluster].dwAvgCpuTempBuffer = 0;
    hMstarDvfsInfo[dwCluster].dwAvgCpuTempCounter = 0;
	hMstarDvfsInfo[dwCluster].dwTempErrReading = 0;
    hMstarDvfsInfo[dwCluster].bSystemResumeFlag = 0;
    hMstarDvfsInfo[dwCluster].dwResetCounter = 0;
    hMstarDvfsInfo[dwCluster].dwTemperatureCounter = 0;
    hMstarDvfsInfo[dwCluster].dwBootTimeCounter = 0;
	hMstarDvfsInfo[dwCluster].dwPradoProtect = 0;
	hMstarDvfsInfo[dwCluster].dwPradoCounter = 0;

    //Get Reference Level of 25-degree Temperature in eFuse
    MHalDvfsCpuTemperature(dwCluster);

    //Init Test Bus to Measure CPU Clock
    MHalDvfsCpuDisplay(dwCluster);

#if defined(CONFIG_MSTAR_IIC) && defined(CONFIG_MSTAR_DVFS_KERNEL_IIC)
    if(SysDvfsPowerInit(dwCluster) == TRUE)
    {
        hMstarDvfsInfo[dwCluster].bDvfsInitOk = 1;
    }
    else
    {
        hMstarDvfsInfo[dwCluster].bDvfsInitOk = 0;
        bDvfsInitStatus = FALSE;
    }
#else
    hMstarDvfsInfo[dwCluster].bDvfsInitOk = 1;
#endif
	mutex_unlock(&MDrvDvfsInitMutex);
    return bDvfsInitStatus;
}

//=================================================================================================
void MDrvHalDvfsInit(void)
{
    int i = 0;
    for (i = 0; i < DVFS_HAL_TOTAL_CLUSTER_NUM; i ++)
    {
        MHalDvfsCpuDisplayInit(i);
        MHalDvfsCpuTemperature(i);
        MHalDvfsRefTemperature(i);
        MHalDvfsCpuPowerInit(i);
        MHalDvfsCorePowerInit(i);
    }
}

//=================================================================================================
void MHalDvfsCpuDisplay(U8 dwCluster)
{
#ifdef CONFIG_DVFS_CPU_CLOCK_DISPLAY_ENABLE
    U32     dwRegisterValue = 0;


    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf6) << 1));
    dwRegisterValue = dwRegisterValue & ~0x01;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf6) << 1)) = dwRegisterValue;

    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf6) << 1));
    dwRegisterValue = dwRegisterValue | 0x01;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf6) << 1)) = dwRegisterValue;

    udelay(CONFIG_DVFS_CLOCK_DELAY_US);

    //CPU Clock = (reg_calc_cnt_report(DEC) / 83333) * 16
    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf4) << 1));
    dwRegisterValue *= 16;
    dwRegisterValue *= 1000;
    dwRegisterValue /= 83333;
    DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock = dwRegisterValue;

    DVFS_HAL_DEBUG("[DVFS] CPU Clock: %dMHz\n", (unsigned int) dwRegisterValue);
#endif
}

//=================================================================================================
void MHalDvfsCpuTemperature(U8 dwCluster)
{
	U32 dwTempData = 0;
    mutex_lock(&MDrvDvfsCpuTempMutex);
    if(hMstarDvfsInfo[dwCluster].dwRefTemperature == 0)
    {
        MHalDvfsRefTemperature(dwCluster);
    }

	preempt_disable();
    //Read CH8 of PM_SAR to Get CPU Temperature
    if (dwCluster == 0)
    {
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001420 << 1)) &= ~0x1;;
    }
    else
    {
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001420 << 1)) |= 0x01;
    }
    udelay(CONFIG_DVFS_CLOCK_DELAY_US);
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001400 << 1)) |= (0x01 << 14);
    dwTempData = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00148e << 1));
    if(dwTempData == 0)
    {
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001400 << 1)) = 0x0A20;

        udelay(CONFIG_DVFS_CLOCK_DELAY_US);

        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x001400 << 1)) |= (0x01 << 14);

        dwTempData = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00148e << 1));
    }
	preempt_enable();

	if (dwTempData > 0x200 || dwTempData < 0x100) {
		hMstarDvfsInfo[dwCluster].dwTempErrReading++;
		pr_warn(" Warning: cluster %d temp reading error %d times, skip this reading, re-enabling thermal sensor.\n", dwCluster, hMstarDvfsInfo[dwCluster].dwTempErrReading);
		*(volatile U16*)(CONFIG_REGISTER_BASE_ADDRESS + (0x000E5E << 1)) |= 0x04;
		if (hMstarDvfsInfo[dwCluster].dwTempErrReading > 4) {
			pr_err("cluster %d get more than 4 times temperature reading continuously, need to reboo the system \n", dwCluster);
			pr_err("!!!!! REBOOT SYSTEM !!!!!\n");
			panic("Temperature reading Error");
		}
		mutex_unlock(&MDrvDvfsCpuTempMutex);
		return;
	}
    hMstarDvfsInfo[dwCluster].dwAvgCpuTempBuffer += dwTempData;
    hMstarDvfsInfo[dwCluster].dwAvgCpuTempCounter ++;
    if((hMstarDvfsInfo[dwCluster].dwAvgCpuTempCounter >= CONFIG_DVFS_DATA_COUNT) || \
       (DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_INIT_MODE))
    {
	U32    dwTempValue = 0;

		DVFS_HAL_DEBUG("[DVFS] [[====================================\n");
		hMstarDvfsInfo[dwCluster].dwTempErrReading = 0;
		MHalDvfsCpuDisplay(dwCluster);

		DVFS_HAL_DEBUG("[DVFS] Reference Temperature Data (%d): 0x%04x\n",
			dwCluster, (unsigned int) hMstarDvfsInfo[dwCluster].dwRefTemperature);

        if(hMstarDvfsInfo[dwCluster].dwAvgCpuTempBuffer != 0)
        {
            dwTempValue = hMstarDvfsInfo[dwCluster].dwAvgCpuTempBuffer / hMstarDvfsInfo[dwCluster].dwAvgCpuTempCounter;

			DVFS_HAL_DEBUG("[DVFS] Current Temperature Sensor Data (%d): 0x%04x\n",
				dwCluster, (unsigned int) dwTempValue);

            if(hMstarDvfsInfo[dwCluster].dwRefTemperature >= dwTempValue)
            {
                dwTempData = (((hMstarDvfsInfo[dwCluster].dwRefTemperature - dwTempValue) * 1280) + CONFIG_DVFS_T_SENSOR_SHIFT);
            }
            else
            {
                dwTempData = ((dwTempValue - hMstarDvfsInfo[dwCluster].dwRefTemperature) * 1280);
                dwTempData = (CONFIG_DVFS_T_SENSOR_SHIFT - dwTempData);
            }

            hMstarDvfsInfo[dwCluster].dwCpuTemperature = (dwTempData / 1000);
            DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_temp = hMstarDvfsInfo[dwCluster].dwCpuTemperature;
        }

		DVFS_HAL_DEBUG("[DVFS] Temperature (%d): %d\n", dwCluster, hMstarDvfsInfo[dwCluster].dwCpuTemperature);

        hMstarDvfsInfo[dwCluster].dwAvgCpuTempBuffer = 0;
        hMstarDvfsInfo[dwCluster].dwAvgCpuTempCounter = 0;

		DVFS_HAL_DEBUG("[DVFS] CPU Power: %d0mV\n", hMstarDvfsInfo[dwCluster].dwFinalCpuPowerVoltage);
		DVFS_HAL_DEBUG("[DVFS] Core Power: %d0mV\n", hMstarDvfsInfo[dwCluster].dwFinalCorePowerVoltage);

		DVFS_HAL_DEBUG("[DVFS] ====================================]]\n");
    }
    mutex_unlock(&MDrvDvfsCpuTempMutex);
}

//=================================================================================================
void MHalDvfsCpuClockAdjustment(U32 dwCpuClock, U8 dwCpu)
{
    U32     dwRegisterValue = 0;
    U32     dwTempCpuClock  = 0;
    int     dwCluster       = getCpuCluster(dwCpu);
    static  bool bLowClkFg[2];
	int		result			= 1;

    dwTempCpuClock = (dwCpuClock - (dwCpuClock % 4));
    if((DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock != dwTempCpuClock) || (hMstarDvfsInfo[dwCluster].bDvfsModeChange == 1))
    {
        if((DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock < dwCpuClock) && (gDVFS_auto_measurement == 0))
        {
			result = MHalDvfsPowerControl(dwCpuClock, dwCpu);
        }

        udelay(CONFIG_DVFS_CLOCK_DELAY_US);
	if (unlikely(!result)) {
		pr_warn("Voltage change failed, ignore clock chagne %d request \n", dwCpuClock);
		hMstarDvfsInfo[dwCluster].bDvfsModeChange = 0;
		return;
	} else {
		if (dwCpuClock < 700)
        {
            if (bLowClkFg[dwCluster] == 0)
            {
                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xfc) << 1));
                dwRegisterValue = (dwRegisterValue & (~0x01));
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xfc) << 1)) = dwRegisterValue;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xfa) << 1));
                dwRegisterValue = (dwRegisterValue | 0x01);
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xfa) << 1)) = dwRegisterValue;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf8) << 1));
                dwRegisterValue = (dwRegisterValue & (~0x01));
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf8) << 1)) = dwRegisterValue;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0x2a) << 1));
                dwRegisterValue = (dwRegisterValue | 0x02);
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0x2a) << 1)) = dwRegisterValue;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf8) << 1));
                dwRegisterValue = (dwRegisterValue | 0x01);
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf8) << 1)) = dwRegisterValue;

                bLowClkFg[dwCluster] = 1;
            }

            //Adjust User Defined CPU Clock
            dwRegisterValue = ((3623878UL / (dwCpuClock << 1)) * 1000);
        }
        else
        {
            if (bLowClkFg[dwCluster] == 1)
            {
                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xfc) << 1));
                dwRegisterValue = (dwRegisterValue & (~0x01));
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xfc) << 1)) = dwRegisterValue;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xfa) << 1));
                dwRegisterValue = (dwRegisterValue | 0x01);
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xfa) << 1)) = dwRegisterValue;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf8) << 1));
                dwRegisterValue = (dwRegisterValue & (~0x01));
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf8) << 1)) = dwRegisterValue;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0x2a) << 1));
                dwRegisterValue = (dwRegisterValue & (~0x02));
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0x2a) << 1)) = dwRegisterValue;

                dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf8) << 1));
                dwRegisterValue = (dwRegisterValue | 0x01);
                *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf8) << 1)) = dwRegisterValue;

                bLowClkFg[dwCluster] = 0;
            }

            //Adjust User Defined CPU Clock
            dwRegisterValue = ((3623878UL / dwCpuClock) * 1000);
        }

        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xa4) << 1))
                            = (dwRegisterValue & 0xFFFF);
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xa6) << 1))
                            = ((dwRegisterValue >> 16) & 0xFFFF);
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xb0) << 1))
                            = 0x01;   //switch to LPF control
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xaa) << 1))
                            = 0x06;   //mu[2:0]
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xae) << 1))
                            = 0x08;   //lpf_update_cnt[7:0]

        //Set LPF is Low to High
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xb2) << 1))
                            |= (0x01 << 12);
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xa8) << 1))
                            = 0x00;
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwAnaMiscBank + 0xa8) << 1))
                            = 0x01;
		}
        udelay(CONFIG_DVFS_CLOCK_DELAY_US);

        if((DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock >= dwCpuClock) && (gDVFS_auto_measurement == 0))
        {
            MHalDvfsPowerControl(dwCpuClock, dwCpu);
        }

		if (result == 1)
			DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock = dwCpuClock;

        hMstarDvfsInfo[dwCluster].bDvfsModeChange = 0;
    }
}

//=================================================================================================
U32 MHalDvfsSearchCpuClockLevel(U32 dwCpuClock, U8 dwCpu)
{
    U32     dwLoopCounter = 0;
    U32     dwCpuLevel = 0;
    int     dwCluster = getCpuCluster(dwCpu);

    //Confirm Corresponding Level by User Defined CPU Clock
    for(dwLoopCounter = 0; dwLoopCounter < CONFIG_DVFS_POWER_CTL_SEGMENT; dwLoopCounter++)
    {
        if((dwCpuClock >= hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsFreezeModeInfo[dwLoopCounter].DvfsCpuInfo.dwLowerCpuClock) && \
           (dwCpuClock < hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsFreezeModeInfo[dwLoopCounter].DvfsCpuInfo.dwUpperCpuClock)  && \
           (hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwLowerFreezeTemperature != CONFIG_DVFS_TEMPERATURE_DISABLE))
        {
            dwCpuLevel = dwLoopCounter;
            return dwCpuLevel;
        }
        else if((dwCpuClock >= hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwLoopCounter].DvfsCpuInfo.dwLowerCpuClock) && \
           (dwCpuClock < hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwLoopCounter].DvfsCpuInfo.dwUpperCpuClock))
        {
            dwCpuLevel = dwLoopCounter;
            return dwCpuLevel;
        }
    }
    return dwCpuLevel;
}

//=================================================================================================
int MHalDvfsPowerControl(U32 dwCpuClock, U8 dwCpu)
{
    U32     dwClockLevel = 0;
    int     dwCluster = getCpuCluster(dwCpu);
	int     result_cpu = 0;
	int     result_core = 0;

    dwClockLevel = MHalDvfsSearchCpuClockLevel(dwCpuClock, dwCpu);
    if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_INIT_MODE)
    {
        //Init Mode
        //Adjust CPU Power
		result_cpu = MHalDvfsCpuPowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsInitModeInfo.DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_cpu_pwr_type].dwCpuPower, dwCpu);

        //Adjust Core Power
		result_core = MHalDvfsCorePowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsInitModeInfo.DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_core_pwr_type].dwCorePower, dwCpu);
    }
    else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_BOOT_MODE)
    {
        //Boot Mode
        //Adjust CPU Power
		result_cpu = MHalDvfsCpuPowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsBootModeInfo.DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_cpu_pwr_type].dwCpuPower, dwCpu);

        //Adjust Core Power
		result_core = MHalDvfsCorePowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsBootModeInfo.DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_core_pwr_type].dwCorePower, dwCpu);
	} else if (DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_LIGHT_MODE) {
		/* LIGHT Mode */

		/* Adjust CPU Power */
		result_cpu = MHalDvfsCpuPowerAdjustment(hMstarDvfsInfo[dwCluster]
			.DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster]
			.reg_chip_package].DvfsLightModeInfo
			.DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster]
			.reg_cpu_pwr_type].dwCpuPower, dwCpu);

		/* Adjust Core Power */
		result_core = MHalDvfsCorePowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo
			->dvfs_reg[dwCluster].reg_chip_package].DvfsLightModeInfo
			.DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_core_pwr_type]
			.dwCorePower, dwCpu);
	}
    else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_FREEZE_MODE)
    {
        //Freeze Mode
        if(dwClockLevel == CONFIG_DVFS_CPU_CLOCK_DISABLE)
        {
            //Adjust CPU Power
			result_cpu = MHalDvfsCpuPowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwDefaultCpuPower, dwCpu);

            //Adjust Core Power
			result_core = MHalDvfsCorePowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwDefaultCorePower, dwCpu);
        }
        else
        {
            //Adjust CPU Power
			result_cpu = MHalDvfsCpuPowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsFreezeModeInfo[dwClockLevel].DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_cpu_pwr_type].dwCpuPower, dwCpu);

            //Adjust Core Power
			result_core = MHalDvfsCorePowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsFreezeModeInfo[dwClockLevel].DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_core_pwr_type].dwCorePower, dwCpu);
        }
    }
    else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_OVER_TEMPERATURE_MODE)
    {
        //Over-Temperature Mode
        //Adjust CPU Power
		result_cpu = MHalDvfsCpuPowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsOverTemperatureModeInfo.DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_cpu_pwr_type].dwCpuPower, dwCpu);

        //Adjust Core Power
		result_core = MHalDvfsCorePowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsOverTemperatureModeInfo.DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_core_pwr_type].dwCorePower, dwCpu);
    }
    else
    {
        //Normal Mode and Special Clock Mode
        if(dwClockLevel == CONFIG_DVFS_CPU_CLOCK_DISABLE)
        {
            //Adjust CPU Power
			result_cpu = MHalDvfsCpuPowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwDefaultCpuPower, dwCpu);

            //Adjust Core Power
			result_core = MHalDvfsCorePowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwDefaultCorePower, dwCpu);
        }
        else
        {
            //Adjust CPU Power
			result_cpu = MHalDvfsCpuPowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwClockLevel].DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_cpu_pwr_type].dwCpuPower, dwCpu);

            //Adjust Core Power
			result_core = MHalDvfsCorePowerAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwClockLevel].DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_core_pwr_type].dwCorePower, dwCpu);
        }
    }

	return result_core & result_cpu;
}

/* ================================================================================================= */
void MHalDvfsPradoMonitor(U8 dwCpu)
{
	int     dwCluster = getCpuCluster(dwCpu);
	int result = 0;
	mutex_lock(&MDrvDvfsStrMutex);
	if (hMstarDvfsInfo[dwCluster].bSystemResumeFlag != 0) {
		pr_info("\033[1;31m[DVFS] Re-init Power Control Flow\033[0m \n");


#if defined(CONFIG_MSTAR_IIC) && defined(CONFIG_MSTAR_DVFS_KERNEL_IIC)
	SysDvfsCpuPowerInit();
#else
	MDrvDvfsVoltageSetup(0, 0, 0xFE, dwCpu);
	result = voltage_change_result[dwCpu];
		if (result == 0) {
			pr_err("Failure, PowerIC init fail, need to disable dvfs!!!!! %d\n", __LINE__);
			mutex_unlock(&MDrvDvfsStrMutex);
			return result;
		}
#endif
	hMstarDvfsInfo[dwCluster].bSystemResumeFlag = 0;
    }

	MDrvDvfsVoltageSetup(0, 0, 0xfc, dwCpu);
	result = voltage_change_result[dwCpu];
	if (result == 0)
			pr_err("voltage change error %d\n", __LINE__);
	mutex_unlock(&MDrvDvfsStrMutex);
}

/* ================================================================================================= */
int MHalDvfsCpuPowerAdjustment(U32 dwCpuPowerVoltage, U8 dwCpu)
{
    int     dwCluster = getCpuCluster(dwCpu);
	int     result = 0;

    if(hMstarDvfsInfo[dwCluster].bSystemResumeFlag != 0)
    {
		pr_info("\033[1;31m[DVFS] Re-init Power Control Flow\033[0m \n");


#if defined(CONFIG_MSTAR_IIC) && defined(CONFIG_MSTAR_DVFS_KERNEL_IIC)
        SysDvfsCpuPowerInit();
#else
        MDrvDvfsVoltageSetup(0, 0, 0xFE, dwCpu);
		result = voltage_change_result[dwCpu];
		if (result == 0) {
			pr_err("Failure, PowerIC init fail, need to disable dvfs!!!!! %d\n", __LINE__);
			return result;
		}
#endif
        hMstarDvfsInfo[dwCluster].bSystemResumeFlag = 0;
    }
        #if defined(CONFIG_MSTAR_IIC) && defined(CONFIG_MSTAR_DVFS_KERNEL_IIC)
            SysDvfsCpuPowerAdjustment(dwCpuPowerVoltage, dwCluster);
        #else
            MDrvDvfsVoltageSetup(0, dwCpuPowerVoltage, 0, dwCpu);
			result = voltage_change_result[dwCpu];
			if (result == 0)
				printk("voltage change error %d\n", __LINE__);
        #endif
	DVFS_HAL_DEBUG("[DVFS] dwCpu:%d, result:%d, CPU Power: %d\n", dwCpu, result, (unsigned int) dwCpuPowerVoltage);
	if (result == 1)
		hMstarDvfsInfo[dwCluster].dwFinalCpuPowerVoltage = dwCpuPowerVoltage;
	else
		pr_err("Cpu Voltage change fail !!!, dwCpu: %d, CPU Power: %d \n", dwCpu, (unsigned int) dwCpuPowerVoltage);

	return result;
}

//=================================================================================================
int MHalDvfsCorePowerAdjustment(U32 dwCorePowerVoltage, U8 dwCpu)
{
    int    dwCluster = getCpuCluster(dwCpu);
	int    result = 0;
#if defined(CONFIG_MSTAR_IIC) && defined(CONFIG_MSTAR_DVFS_KERNEL_IIC)
    SysDvfsCorePowerAdjustment(dwCorePowerVoltage, dwCluster);
#else
    MDrvDvfsVoltageSetup(0, dwCorePowerVoltage, 1, dwCpu);
	result = voltage_change_result[dwCpu];
#endif
	DVFS_HAL_DEBUG("[DVFS] dwCpu:%d, result:%d, Core Power: %d\n", dwCpu, result, (unsigned int) dwCorePowerVoltage);
	if (result == 1)
		hMstarDvfsInfo[dwCluster].dwFinalCorePowerVoltage = dwCorePowerVoltage;
	return result;
}

//=================================================================================================
U32  MHalDvfsQueryCpuClock(U32 dwCpuClockType, U8 dwCpu)
{
    U32     dwOutputCpuClock = 0;
    int     dwCluster = getCpuCluster(dwCpu);

    if(DvfsRegInfo == NULL)
    {
        DvfsRegInfo = (volatile MSTAR_DVFS_REG_INFO *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100500 << 1));
    }

    if(dwCpuClockType == CONFIG_DVFS_MAX_CPU_CLOCK)
    {
		dwOutputCpuClock = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMaximumCpuClock;
    }
    else if(dwCpuClockType == CONFIG_DVFS_MIN_CPU_CLOCK)
    {
        dwOutputCpuClock = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMinimumCpuClock;
    }
    else if(dwCpuClockType == CONFIG_DVFS_IR_BOOTS_CPU_CLOCK)
    {
		dwOutputCpuClock = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsBootModeInfo.DvfsCpuInfo.dwUpperCpuClock;
    }
    else
    {
        dwOutputCpuClock = CONFIG_DVFS_CPU_CLOCK_DISABLE;
    }

    return dwOutputCpuClock;
}

//=================================================================================================
U32 MHalDvfsQueryCpuClockByTemperature(U8 dwCpu)
{
//  S32     dwCpuTemperature = 0;
    int    dwCluster = getCpuCluster(dwCpu);

    if(DvfsRegInfo == NULL)
    {
        DvfsRegInfo = (volatile MSTAR_DVFS_REG_INFO *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100500 << 1));
    }

    if(DvfsRegInfo->dvfs_reg[dwCluster].reg_vid_dvfs_id == CONFIG_DVFS_ENABLE_PATTERN)
    {
        if(hMstarDvfsInfo[dwCluster].bDvfsModeChange == 1)
        {
            goto _MHalDvfsQueryCpuClockByTemperatureExit;
        }

        if(hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature == 0)
        {
            hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwDefaultCpuClock;
        }

        if(hMstarDvfsInfo[dwCluster].bDvfsInitOk == 0)
        {
            if(MHalDvfsInit(dwCluster) == TRUE)
            {
                hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = CONFIG_DVFS_CPU_CLOCK_SPECIAL;
                hMstarDvfsInfo[dwCluster].bDvfsModeChange = 1;
            }
            else
            {
                hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwDefaultCpuClock;
            }
        }
        else
        {
            MHalDvfsCpuTemperature(dwCluster);   //Get current CPU temperature
			if (dwCluster == 1) {
				MHalDvfsPradoMonitor(dwCluster);
				if ((*(volatile U16*) (CONFIG_REGISTER_BASE_ADDRESS + (0x100514 << 1))) == 0x1) {
					DVFS_HAL_INFO("\033[31m [DVFS] Cluster 1 Prado protect \033[0m\n");
					hMstarDvfsInfo[dwCluster].dwPradoProtect = 1;
					hMstarDvfsInfo[dwCluster].dwPradoCounter = 0;
					DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state
						= CONFIG_DVFS_OVER_TEMPERATURE_MODE;
				} else if (hMstarDvfsInfo[dwCluster].dwPradoProtect == 1) {
					if (hMstarDvfsInfo[dwCluster].dwPradoCounter
						< CONFIG_DVFS_AVERAGE_PRADO_COUNT) {
						/*
						DVFS_HAL_INFO("\033[31m [DVFS] Prado Over-Temperature Counter:
						%d \033[0m\n",
						(unsigned int) hMstarDvfsInfo[dwCluster].dwPradoCounter);
						*/
						hMstarDvfsInfo[dwCluster].dwPradoCounter++;
					} else {
						DVFS_HAL_INFO("\033[31m [DVFS] prado protect disable \033[0m\n");
						hMstarDvfsInfo[dwCluster].dwPradoProtect = 0;
						hMstarDvfsInfo[dwCluster].dwPradoCounter = 0;
					}
				}
			}

		if ((*(volatile U16*)(CONFIG_REGISTER_BASE_ADDRESS + (0x100516 << 1))
			== CONFIG_DVFS_CPU_CLOCK_LIGHT_MODE)  &&
			(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state != CONFIG_DVFS_LIGHT_MODE)  &&
			(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster]
			.reg_chip_package].DvfsLightModeInfo.DvfsCpuInfo.dwLowerCpuClock
			!= CONFIG_DVFS_LIGHTMODE_DISABLE)) {

			/*
			printk(KERN_DEBUG"\033[32m[DVFS] eason.lien [%s, %d] \033[0m\n",
			__func__, __LINE__);
			*/
				DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state  = CONFIG_DVFS_NORMAL_MODE;
				DvfsRegInfo->dvfs_reg[dwCluster].reg_special_cpu_clk = CONFIG_DVFS_CPU_CLOCK_LIGHT_MODE;
				hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = CONFIG_DVFS_CPU_CLOCK_LIGHT_MODE;
				hMstarDvfsInfo[dwCluster].bDvfsModeChange            = 1;
	} else if (DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_INIT_MODE) {
                hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsInitModeInfo.DvfsCpuInfo.dwLowerCpuClock;
            }
            else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_OVER_TEMPERATURE_MODE)
            {
                if(hMstarDvfsInfo[dwCluster].dwCpuTemperature >
                   hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwMaxLevelTemperature)
                {
                    if(hMstarDvfsInfo[dwCluster].dwTemperatureCounter < CONFIG_DVFS_AVERAGE_COUNT)
                    {
                        DVFS_HAL_DEBUG("\033[1;31m[DVFS] Over Temperature Protection: %d\033[0m\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwTemperatureCounter);
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter ++;
                    }
                    else
                    {
                        //Maximum Level Threshold Temperature in Over-Temperature Mode
                        DVFS_HAL_INFO("\033[1;31m[DVFS] Current Temperature: %d\033[0m\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
                        DVFS_HAL_INFO("\033[1;31m[DVFS] Over Temperature Mode: SYSTEM RESET\033[0m\n");

#ifdef CONFIG_AMAZON_SIGN_OF_LIFE
		pr_err("Over temperature %d, Reboot System!!! \n", (unsigned int) hMstarDvfsInfo[dwCluster].dwCpuTemperature);
		life_cycle_set_boot_reason(WARMBOOT_BY_SOC_OVER_TEMP);
#endif
#if defined(CONFIG_AMAZON_METRICS_LOG)
			temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, 0, 1);
#endif

                        //Trigger a WDT Reset
                        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00300a << 1)) = 0x00;
                        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x003008 << 1)) = 0x00;
                        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00300a << 1)) = 0x05;
                        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x003000 << 1)) = 0x01;

                        while(1);
                    }
		} else if (hMstarDvfsInfo[dwCluster].dwPradoProtect == 1) {
					hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster]
						.DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package]
						.DvfsSysInfo.dwProtectedCpuClock;
				}
                else if(hMstarDvfsInfo[dwCluster].dwCpuTemperature <
                        hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwLowerLevelTemperature)
                {
		if (hMstarDvfsInfo[dwCluster].dwTemperatureCounter < CONFIG_DVFS_AVERAGE_LOW_TEMP_COUNT) {
                        DVFS_HAL_DEBUG("[DVFS] Over-Temperature->Normal Mode Counter: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwTemperatureCounter);
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter ++;
                    }
                    else
                    {
                        //Return to Normal Mode
                        DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;
                        hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMaximumCpuClock;
                        DVFS_HAL_DEBUG("[DVFS] Normal Mode, Support CPU Clock: %dMHz\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature);

                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter = 0;
                    }
                }
                else
                {
                    //Keep at Over-Temperature Mode
                    hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwProtectedCpuClock;
                }
            }
            else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_BOOT_MODE)
            {
                if(hMstarDvfsInfo[dwCluster].dwCpuTemperature >=
                   hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperLevelTemperature)
                {
		if (hMstarDvfsInfo[dwCluster].dwTemperatureCounter < CONFIG_DVFS_AVERAGE_HIGH_TEMP_COUNT) {
                        DVFS_HAL_DEBUG("[DVFS] Boot->Over-Temperature Mode Counter: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwTemperatureCounter);
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter ++;
                    }
                    else
                    {
                        //Upper Level Threshold Temperature in Over-Temperature Mode
                        DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_OVER_TEMPERATURE_MODE;
                        //hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwProtectedCpuClock;
                        hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = CONFIG_DVFS_CPU_CLOCK_SPECIAL;
                        DVFS_HAL_DEBUG("[DVFS] Over Temperature Mode, Support CPU Clock: %dMHz\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature);

                        hMstarDvfsInfo[dwCluster].bDvfsModeChange = 1;
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter = 0;
                    }

                    hMstarDvfsInfo[dwCluster].dwBootTimeCounter = 0;
                }
                else
                {
                    //Keep at Boot Mode
                    hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMaximumCpuClock;

                    hMstarDvfsInfo[dwCluster].dwBootTimeCounter ++;
                    if(hMstarDvfsInfo[dwCluster].dwBootTimeCounter > CONFIG_DVFS_BOOT_MODE_TIME)
                    {
                        //Return to Normal Mode
//                      DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;
                        if(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperFreezeTemperature == CONFIG_DVFS_TEMPERATURE_DISABLE)
                        {
                            DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;
                        }
                        else
                        {
                            DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_FREEZE_MODE;
                        }

                        hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = CONFIG_DVFS_CPU_CLOCK_SPECIAL;
                        DVFS_HAL_DEBUG("[DVFS] Boot Mode->Normal Mode, Support CPU Clock: %dMHz\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature);

                        hMstarDvfsInfo[dwCluster].bDvfsModeChange = 1;
                    }
                    else
                    {
                        DVFS_HAL_DEBUG("[DVFS] Boot Mode Counter: %d\n", hMstarDvfsInfo[dwCluster].dwBootTimeCounter);
                    }
                }
            }
            else if(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_FREEZE_MODE)
            {
                if((hMstarDvfsInfo[dwCluster].dwCpuTemperature >= hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperFreezeTemperature) && \
                   (hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperFreezeTemperature != CONFIG_DVFS_TEMPERATURE_DISABLE))
                {
                    if(hMstarDvfsInfo[dwCluster].dwTemperatureCounter < CONFIG_DVFS_AVERAGE_COUNT)
                    {
                        DVFS_HAL_DEBUG("[DVFS] Freeze->Normal Mode Counter: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwTemperatureCounter);
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter ++;
                    }
                    else
                    {
                        //Return to Normal Mode
                        DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;
                        hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = CONFIG_DVFS_CPU_CLOCK_SPECIAL;
                        DVFS_HAL_DEBUG("[DVFS] Normal Mode, Support CPU Clock: %dMHz\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature);

                        hMstarDvfsInfo[dwCluster].bDvfsModeChange = 1;
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter = 0;
                    }
                }
                else
                {
                    hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMaximumCpuClock;
                }
	} else if (DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_LIGHT_MODE) {
                if(hMstarDvfsInfo[dwCluster].dwCpuTemperature >=
                   hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperLevelTemperature)
                {
                    if(hMstarDvfsInfo[dwCluster].dwTemperatureCounter < CONFIG_DVFS_AVERAGE_COUNT)
                    {
				DVFS_HAL_DEBUG("[DVFS] Light Mode ->Over-Temperature Mode Counter: %d\n",
					(unsigned int) hMstarDvfsInfo[dwCluster].dwTemperatureCounter);
				hMstarDvfsInfo[dwCluster].dwTemperatureCounter++;
			} else {
				/* Upper Level Threshold Temperature in Over-Temperature Mode */
				DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_OVER_TEMPERATURE_MODE;
				hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster]
					.DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package]
					.DvfsSysInfo.dwProtectedCpuClock;
				DVFS_HAL_DEBUG("[DVFS] Over Temperature Mode, Support CPU Clock: %dMHz\n",
					(unsigned int) hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature);

				hMstarDvfsInfo[dwCluster].bDvfsModeChange = 1;
				hMstarDvfsInfo[dwCluster].dwTemperatureCounter = 0;
			}
		} else {
			if (*(volatile U16*)(CONFIG_REGISTER_BASE_ADDRESS + (0x100516 << 1))
				== CONFIG_DVFS_CPU_CLOCK_LIGHT_MODE) {
				DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_LIGHT_MODE;
				hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster]
				.DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package]
				.DvfsLightModeInfo.DvfsCpuInfo.dwLowerCpuClock;
			} else if (*(volatile U16*)(CONFIG_REGISTER_BASE_ADDRESS + (0x100516 << 1))
						!= CONFIG_DVFS_LIGHTMODE_DISABLE) {
					DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_NORMAL_MODE;
					hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature
						 = CONFIG_DVFS_CPU_CLOCK_SPECIAL;
					hMstarDvfsInfo[dwCluster].bDvfsModeChange = 1;
			}
		}
	} else if ((DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state == CONFIG_DVFS_NORMAL_MODE) ||
				(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state
				== CONFIG_DVFS_SPECIAL_CLOCK_MODE)) {
		if (hMstarDvfsInfo[dwCluster].dwCpuTemperature >=
			hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package]
			.DvfsTemperatureInfo.dwUpperLevelTemperature) {
			if (hMstarDvfsInfo[dwCluster].dwTemperatureCounter < CONFIG_DVFS_AVERAGE_HIGH_TEMP_COUNT) {
				DVFS_HAL_DEBUG("[DVFS] Normal->Over-Temperature Mode Counter: %d\n",
				(unsigned int) hMstarDvfsInfo[dwCluster].dwTemperatureCounter);
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter ++;
                    }
                    else
                    {
                        //Upper Level Threshold Temperature in Normal Mode
                        DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_OVER_TEMPERATURE_MODE;
                        hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwProtectedCpuClock;
                        DVFS_HAL_DEBUG("[DVFS] Over Temperature Mode, Support CPU Clock: %dMHz\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature);

                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter = 0;
                    }
                }
                else if((hMstarDvfsInfo[dwCluster].dwCpuTemperature < hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwLowerFreezeTemperature) && \
                        (hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwLowerFreezeTemperature != CONFIG_DVFS_TEMPERATURE_DISABLE))
                {
                    if(hMstarDvfsInfo[dwCluster].dwTemperatureCounter < CONFIG_DVFS_AVERAGE_COUNT)
                    {
                        DVFS_HAL_DEBUG("[DVFS] Normal->Freeze Mode Counter: %d\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwTemperatureCounter);
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter ++;
                    }
                    else
                    {
                        //Freeze Threshold Temperature in Normal Mode
                        DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state = CONFIG_DVFS_FREEZE_MODE;
                        hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = CONFIG_DVFS_CPU_CLOCK_SPECIAL;
                        DVFS_HAL_DEBUG("[DVFS] Freeze Mode, Support CPU Clock: %dMHz\n", (unsigned int) hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature);

                        hMstarDvfsInfo[dwCluster].bDvfsModeChange = 1;
                        hMstarDvfsInfo[dwCluster].dwTemperatureCounter = 0;
                    }
                }
                else
                {
                    //Keep at Normal Mode
                    hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMaximumCpuClock;
                }
            }
            else
            {
                hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature = hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsSysInfo.dwMaximumCpuClock;
            }
        }
    }

_MHalDvfsQueryCpuClockByTemperatureExit:

    DVFS_HAL_DEBUG("[DVFS] Current DVFS State: %d\n", (unsigned int) DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state);

	char data[25];
	char *envp[] = {data, NULL};
	bool stateChanged = (pre_dvfs_state != DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state);
	bool stateWatched = (CONFIG_DVFS_NORMAL_MODE == DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state
						|| CONFIG_DVFS_OVER_TEMPERATURE_MODE == DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state);
	if (stateChanged && stateWatched) {
		pre_dvfs_state = DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state;
		int thermalStateValue = (CONFIG_DVFS_NORMAL_MODE == DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_dvfs_state) ? 0 : 1;
		snprintf(data, sizeof(data), "THERMAL_STATE=%d", thermalStateValue);
		kobject_uevent_env(&mstar_dvfs_dev.dev.kobj, KOBJ_CHANGE, envp);
		pr_debug("[DVFS] DVFS Thermal State change (%d)\n", thermalStateValue);
#if defined(CONFIG_AMAZON_METRICS_LOG)
	temp_log_metrics(hMstarDvfsInfo[dwCluster].dwCpuTemperature, prev_mode, 0);
	prev_mode = pre_dvfs_state;
#endif
	}

	DVFS_HAL_DEBUG("[DVFS] Current Valid CPU Clock: %dMHz\n",
		(unsigned int) hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature);

	return hMstarDvfsInfo[dwCluster].dwMaxCpuClockByTemperature;
}

//=================================================================================================
void MHalDvfsCpuDisplayInit(U8 dwCluster)
{
#ifdef CONFIG_DVFS_CPU_CLOCK_DISPLAY_ENABLE

    U32     dwRegisterValue = 0;

    //Init Test Bus to Measure CPU Clock
    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf6) << 1));
    dwRegisterValue = dwRegisterValue | 0x01;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + ((hMstarDvfsInfo[dwCluster].dwMcuArmBank + 0xf6) << 1)) = dwRegisterValue;
#endif
}

//=================================================================================================
void MHalDvfsRefTemperature(U8 dwCluster)
{
    U32     dwRegisterValue = 0;

    //Read 25-degree Reference Level in eFuse
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) = 0x0144;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) = 0x2144;
    while((*(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) & (0x01 << 13)) != 0);

    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002058 << 1));
    dwRegisterValue >>= 6;

    //If no data existed in eFuse, set the default reference level is 400
    hMstarDvfsInfo[dwCluster].dwRefTemperature = dwRegisterValue;
    if(dwRegisterValue == 0)
    {
        hMstarDvfsInfo[dwCluster].dwRefTemperature = 400;
    }
}
//=================================================================================================
U32 MHalDvfsGetCpuTemperature(U8 dwCpu)
{
    int    dwCluster = getCpuCluster(dwCpu);
    if(DvfsRegInfo == NULL)
    {
        DvfsRegInfo = (volatile MSTAR_DVFS_REG_INFO *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100500 << 1));
    }
    return DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_temp;
}
//=================================================================================================
U32 MHalDvfsGetVoltage(U8 dwCpu)
{
    U32    dwClockLevel = 0;
    int    dwCluster = getCpuCluster(dwCpu);

    if(DvfsRegInfo == NULL)
    {
        DvfsRegInfo = (volatile MSTAR_DVFS_REG_INFO *)(CONFIG_REGISTER_BASE_ADDRESS + (0x100500 << 1));
    }

    dwClockLevel = MHalDvfsSearchCpuClockLevel(DvfsRegInfo->dvfs_reg[dwCluster].reg_cur_cpu_clock, dwCpu);

    return hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsNormalModeInfo[dwClockLevel].DvfsPowerInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_cpu_pwr_type].dwCpuPower;
}

//=================================================================================================
U32 MHalDvfsGetSidd(void)
{
    U32    dwRegisterValue = 0;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) = 0x01ac;
    *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) = 0x21ac;
    while((*(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) & (0x01 << 13)) != 0);
    dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002058 << 1));
    dwRegisterValue += ((*(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00205a << 1))) << 16);
    dwRegisterValue = dwRegisterValue & 0x3ff;
    return dwRegisterValue;
}

//=================================================================================================
U32 MHalDvfsGetOsc(U8 dwCpu)
{
    U32    dwRegisterValue = 0;
    int    dwCluster = getCpuCluster(dwCpu);

    if (dwCluster == 0)
    {
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) = 0x01ac;
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) = 0x21ac;
        while((*(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) & (0x01 << 13)) != 0);
        dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002058 << 1));
        dwRegisterValue += ((*(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00205a << 1))) << 16);
        dwRegisterValue = (dwRegisterValue >> 10) & 0x3ff;
    }
    else
    {
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) = 0x01b4;
        *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) = 0x21b4;
        while((*(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002050 << 1)) & (0x01 << 13)) != 0);
        dwRegisterValue = *(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x002058 << 1));
        dwRegisterValue += ((*(volatile U16 *)(CONFIG_REGISTER_BASE_ADDRESS + (0x00205a << 1))) << 16);
        dwRegisterValue = (dwRegisterValue >> 2) & 0x3ff;
    }

    return dwRegisterValue;
}

/*=================================================================================================*/
void MHalDvfs_set_high_bound_temperature(U8 dwCpu, U32 temperature)
{
	int dwCluster = getCpuCluster(dwCpu);
	hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwUpperLevelTemperature
		 = temperature;
}

/*=================================================================================================*/
void MHalDvfs_set_low_bound_temperature(U8 dwCpu, U32 temperature)
{
	int dwCluster = getCpuCluster(dwCpu);
	hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwLowerLevelTemperature
		= temperature;
}

/*=================================================================================================*/
void MHalDvfs_set_max_bound_temperature(U8 dwCpu, U32 temperature)
{
	int dwCluster = getCpuCluster(dwCpu);
	hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsTemperatureInfo.dwMaxLevelTemperature
		= temperature;
}

//=================================================================================================
void MHalDvfsSetAutoMeasurement(U32 auto_measurement)
{
    gDVFS_auto_measurement = auto_measurement;
}

//=================================================================================================
void MHalDvfsCpuPowerInit(U8 dwCluster)
{
    //TBD
}

//=================================================================================================
void MHalDvfsCorePowerInit(U8 dwCluster)
{
    //TBD
}

//=================================================================================================

static int mstar_dvfs_drv_suspend(struct device *dev)
{
    U8 cluster = 0;
    DVFS_HAL_DEBUG("[DVFS] Enter Suspend Mode\n");
	if (!mutex_trylock(&MDrvDvfsStrMutex))
		return -EBUSY;
//  MHalDvfsCpuClockAdjustment(hMstarDvfsInfo[dwCluster].DvfsModeInfo[DvfsRegInfo->dvfs_reg[dwCluster].reg_chip_package].DvfsInitModeInfo.DvfsCpuInfo.dwLowerCpuClock);

    if (NULL == DvfsRegInfo)
    {
        printk("\033[34mFunction = %s, DvfsRegInfo was not initialized(NULL).\033[m\n", __PRETTY_FUNCTION__);
		mutex_unlock(&MDrvDvfsStrMutex);
		return 0;
    }

    for (cluster = 0; cluster < DVFS_HAL_TOTAL_CLUSTER_NUM; cluster ++)
    {
        DvfsRegInfo->dvfs_reg[cluster].reg_vid_dvfs_id = 0;
		hMstarDvfsInfo[cluster].bDvfsInitOk = 0;

    }

	mutex_unlock(&MDrvDvfsStrMutex);
    return 0;
}

static int mstar_dvfs_drv_resume(struct device *dev)
{
    U8 cluster = 0;
    DVFS_HAL_DEBUG("[DVFS] Enter Resume Mode\n");
	mutex_lock(&MDrvDvfsStrMutex);
    halTotalClusterNumber = DVFS_HAL_TOTAL_CLUSTER_NUM;
    for (cluster = 0; cluster < DVFS_HAL_TOTAL_CLUSTER_NUM; cluster ++)
    {
        //Only one cluster
        MHalDvfsInit(cluster);
		hMstarDvfsInfo[cluster].bSystemResumeFlag = 1;

        DvfsRegInfo->dvfs_reg[cluster].reg_cur_dvfs_state = CONFIG_DVFS_INIT_MODE;
    }


	mutex_unlock(&MDrvDvfsStrMutex);
    return 0;
}

static int mstar_dvfs_drv_freeze(struct device *dev)
{
    return 0;
}

static int mstar_dvfs_drv_thaw(struct device *dev)
{
    return 0;
}

static int mstar_dvfs_drv_restore(struct device *dev)
{
    return 0;
}

static int mstar_dvfs_drv_probe(struct platform_device *pdev)
{
    pdev->dev.platform_data = NULL;
    return 0;
}

static int mstar_dvfs_drv_remove(struct platform_device *pdev)
{
    pdev->dev.platform_data = NULL;
    return 0;
}

static const struct dev_pm_ops mstar_dvfs_dev_pm_ops =
{
    .suspend = mstar_dvfs_drv_suspend,
    .resume = mstar_dvfs_drv_resume,
    .freeze = mstar_dvfs_drv_freeze,
    .thaw = mstar_dvfs_drv_thaw,
    .restore = mstar_dvfs_drv_restore,
};

static struct platform_driver mstar_dvfs_driver =
{
    .probe = mstar_dvfs_drv_probe,
    .remove = mstar_dvfs_drv_remove,

    .driver =
    {
        .name = "mstar_dvfs",
        .owner = THIS_MODULE,
        .pm = &mstar_dvfs_dev_pm_ops,
    }
};

static void __init classify_cpu_cluster(void)
{
	int i, j;
    for (j = 0; j < DVFS_HAL_TOTAL_CLUSTER_NUM; j ++) {
        for_each_online_cpu(i) {
            if (MIDR_PARTNUM(get_cpu_midr(i)) == hMstarDvfsInfo[j].dwCpuPartId) {
                hMstarDvfsInfo[j].dwClusterCpuMask |= (0x1<<i);
            }
        }
	pr_debug("Cluster:%d CpuMask:%x\n", j, hMstarDvfsInfo[j].dwClusterCpuMask);
    }

}

static int __init mstar_dvfs_init(void)
{
    halTotalClusterNumber = DVFS_HAL_TOTAL_CLUSTER_NUM;
    classify_cpu_cluster();
    platform_device_register(&mstar_dvfs_dev);
    platform_driver_register(&mstar_dvfs_driver);
    return 0;
}

static void __init mstar_dvfs_exit(void)
{
    platform_device_unregister(&mstar_dvfs_dev);
    platform_driver_unregister(&mstar_dvfs_driver);
}

core_initcall(mstar_dvfs_init);
module_exit(mstar_dvfs_exit);

//=================================================================================================
