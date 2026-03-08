/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

#include <drvGPIO.h>
#include <config.h>
#include <common.h>
#include <MsTypes.h>

#include <apiSWI2C.h>
#include <audio/Audio_amplifier.h>
#include <CusI2C.h>

#define msAPI_Timer_Delayms(x) udelay(1000*x)

#if (CONFIG_DIGITAL_TI5707) //(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5707)
#include "DevTAS5707.c"
#elif (CONFIG_DIGITAL_STA333)//(DIGITAL_I2S_SELECT==AUDIO_I2S_STA333)
#include "DevI2S_STA333.c"
#elif (CONFIG_DIGITAL_TI5708) //(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5708)
#include "Amplifier_TI5708.c"
#elif (CONFIG_DIGITAL_TI5711)//(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5711)
#include "Amplifier_TI5711.c"
#elif (CONFIG_DIGITAL_TI5727_HISENSE)//(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5727_HISENSE)
#include "Amplifier_TI5727_hisense.c"
#elif (CONFIG_DIGITAL_RT9113B)//(DIGITAL_I2S_SELECT==AUDIO_I2S_TAS5727_HISENSE)
#include "Amplifier_RT9113B.c"
#endif


#if 0// move all this to msI2C.c
#define MIIC_BUS_SYS    PAD_DDCR_CK,    PAD_DDCR_DA,    SWI2C_SPEED_MAPPING_100K
#define MIIC_BUS_DDCA0  PAD_DDCA_CK,    PAD_DDCA_DA,    SWI2C_SPEED_MAPPING_100K
#define MIIC_BUS_DDCD0  PAD_DDCDA_CK,   PAD_DDCDA_DA,   SWI2C_SPEED_MAPPING_100K
#define MIIC_BUS_DDCD1  PAD_DDCDB_CK,   PAD_DDCDB_DA,   SWI2C_SPEED_MAPPING_100K
#define MIIC_BUS_DDCD2  PAD_DDCDC_CK,   PAD_DDCDC_DA,   SWI2C_SPEED_MAPPING_100K

#define __I2C_BUS(scl, sda, dly)    scl, sda, dly
#define I2C_BUS( bus )      __I2C_BUS( bus )

typedef struct
{
    MS_U16 padSCL;
    MS_U16 padSDA;
    MS_U16 defDelay;
} I2C_Bus;

static MS_U8 u8BusSel = 0;

SWI2C_BusCfg g_I2CBus[] =
{
    { I2C_BUS( MIIC_BUS_SYS   ) },
	{ I2C_BUS( MIIC_BUS_DDCA0 ) },
	{ I2C_BUS( MIIC_BUS_DDCD0 ) },
	{ I2C_BUS( MIIC_BUS_DDCD1 ) },
	{ I2C_BUS( MIIC_BUS_DDCD2 ) },
};
#endif
void drvAudio_AMP_Init(void)
{

#if (CONFIG_DIGITAL_TI5707)
    //MApi_SWI2C_Init(g_I2CBus,u8BusSel);
    drvTAS5707_SW_Init();
#elif (CONFIG_DIGITAL_STA333)
    //MApi_SWI2C_Init(g_I2CBus,u8BusSel);
    Audio_Amplifier_ON();
    msAPI_Timer_Delayms(20);
    Audio_Amplifier_OFF();
    msAPI_Timer_Delayms(100);
    DevI2s_STA333_Init();
#elif (CONFIG_DIGITAL_TI5708)
    //MApi_SWI2C_Init(g_I2CBus,u8BusSel);
    Amplifier_TI5708_Init();
    Amplifier_TI5708_VolumeForSource();
#elif (CONFIG_DIGITAL_TI5711)
    msI2C_init();
    Amplifier_TI5711_Init();
    Amplifier_TI5711_Mute(0);
    Amplifier_TI5711_SWMute(0);
#elif (CONFIG_DIGITAL_TI5727_HISENSE)
   // MApi_SWI2C_Init(g_I2CBus,u8BusSel);
    Amplifier_TI5727_hisense_Init();
    Amplifier_TI5727_hisense_Mute(0);
    Amplifier_TI5727_hisense_SWMute(0);
#elif (CONFIG_DIGITAL_RT9113B)
    Amplifier_RT9113_Init();
#endif
}
