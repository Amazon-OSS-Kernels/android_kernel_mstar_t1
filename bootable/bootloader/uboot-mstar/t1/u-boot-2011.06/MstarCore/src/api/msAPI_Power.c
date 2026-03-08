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

#define _MSPOWER_C_

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <command.h>
#include <MsTypes.h>
#include <MsIRQ.h>
#include <MsOS.h>
#include <ShareType.h>
#include <MsSystem.h>
#include <drvPM.h>
#include <drvMBX.h>
#include <drvBDMA.h>
#include <malloc.h>
#ifndef  CUS_IR_HEAD_FILE//Please define it in board file for customization
#include "IR_MSTAR_DTV.h"
#else
#include CUS_IR_HEAD_FILE
#endif
#include <msAPI_Power.h>


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PM_MBX_QUEUESIZE 6
#define PM_CMDIDX_GLOBAL_CHIP_RESET 0x21

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

extern BOOLEAN msAPI_IR_Initialize(void);
extern BOOLEAN msAPI_KeyPad_Initialize(void);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#define WKUP_SRC_NONE    0x00
#define WKUP_SRC_IR    0x01
#define WKUP_SRC_DVI    0x02
#define WKUP_SRC_DVI2    0x03
#define WKUP_SRC_CEC    0x04
#define WKUP_SRC_SAR    0x05
#define WKUP_SRC_ESYNC    0x06
#define WKUP_SRC_SYNC    0x07
#define WKUP_SRC_RTC    0x08
#define WKUP_SRC_RTC2    0x09
#define WKUP_SRC_AVLINK    0x0a
#define WKUP_SRC_UART    0x0b
#define WKUP_SRC_GPIO    0x0c
#define WKUP_SRC_MHL    0x0d
#define WKUP_SRC_WOL    0x0e
#define WKUP_SRC_MAX    0x0f

/* Define unused wakeup source as bluetooth and wifi, but you need
     cooperation with PM51 owner, here's some examples:
*/
#define WKUP_SRC_BT    WKUP_SRC_DVI
#define WKUP_SRC_WIFI    WKUP_SRC_DVI2

#define KEYPAD_POWER_CODE  0x1E
#define IR_POWER_CODE 0x46
#define IR_CUSTOM_1_CODE 0x5F
#define IR_CUSTOM_2_CODE_SEIKI 0x5E   /* Toon Goggles */
#define IR_CUSTOM_3_CODE_SEIKI 0x3B   /* AccuWeather */
#define IR_CUSTOM_2_CODE_PEPPER 0x8C
#define IR_CUSTOM_3_CODE_PEPPER 0x8D
#define PM_REG_BASE             (0x0700 * 2)
#define REG_PM_DUMMY_WAKEUP_SOURCE             ((PM_REG_BASE + 0x39 * 2))

static int str_cfg = 0;

static void  report_wakeup_event(void)
{
        int ret = 0;
        int pm_wakeup_source = Read2Byte(REG_PM_DUMMY_WAKEUP_SOURCE);
	printf("wakeup source 0x%x\n", pm_wakeup_source);
        switch (pm_wakeup_source) {
        case WKUP_SRC_IR:
		printf("Wake up by IR\n");
		break;
        case WKUP_SRC_SAR:
		printf("Wake up by Power Key on TV keypad \n");
		break;
        case WKUP_SRC_CEC:
		printf("Wake up by CEC \n");
		break;
        case WKUP_SRC_WOL:
		printf("Wake up by WOL \n");
		break;
        case WKUP_SRC_GPIO:
		printf("Wake up by GPIO \n");
		break;
        case WKUP_SRC_BT:
		printf("Wake up by BT \n");
		break;
        case WKUP_SRC_WIFI:
		printf("Wake up by WIFI \n");
                break;
        case WKUP_SRC_RTC:
		printf("Wake up by RTC \n");
		break;
        default:
		printf("Unknown reason wake up the system 0x%x\n",pm_wakeup_source);
                break;
        }
}
MS_BOOL msAPI_PM_MBX_Init(void)
{
    MsApi_MBX_Init();
    if( E_MBX_SUCCESS != MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_NOWAIT, PM_MBX_QUEUESIZE))
    {
        printf("E_MBX_CLASS_PM_NOWAIT - MBX register msg error\n");
    }
   return TRUE;
    }


MS_BOOL msAPI_PM_Reset(void)
    {
    MBX_Msg MB_Command;
    printf("\033[0;33m Do mailbox init\033[0m\n");

    msAPI_PM_MBX_Init();
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
    MB_Command.u8Index = PM_CMDIDX_GLOBAL_CHIP_RESET;
    MB_Command.u8ParameterCount = 0;
    printf("\033[0;33m Send reset message tp PM51.\033[0m\n\n");
    MDrv_MBX_SendMsg(&MB_Command);
        return TRUE;

}

EN_POWER_ON_MODE msAPI_Power_QueryPowerOnMode(void)
{
    U8 u8PowerDownMode =  (U8)MDrv_PM_PowerOnMode();

    switch ( u8PowerDownMode )
    {
        // DC ON
        case PM_MODE_STANBY:    // 0x01
        case PM_MODE_SLEEP:     // 0x02
        case PM_MODE_DEEPSLEEP: // 0x03
        {
            printf("DC on \n");
		report_wakeup_event();
            return EN_POWER_DC_BOOT;
        }
        // AC ON
        case PM_MODE_DEFAULT:   // 0xFF
        {
            printf("AC on\n");
            return EN_POWER_AC_BOOT;
        }
        // EXCEPTION
        default:
        {
            if(u8PowerDownMode == 0xF1) {
                printf("DC on !! \n");
		report_wakeup_event();
		}
            else
                printf("read PM_SLEEP_AC_DC_ON error\n");

            return EN_POWER_DC_BOOT;
        }
    }
}
void msAPI_SetSPIOffsetForMCU(void)
{
    MDrv_BDMA_SetSPIOffsetForMCU();
}
void msAPI_Power_PowerDown_EXEC(void)
{

    PM_Result pmResult;
    static PM_PowerDownCfg PmPowerDownCfg;

#if 0//eric.peng
    MDrv_BDMA_SetSPIOffsetForMCU();
#endif
    if(msAPI_PM_MBX_Init()==FALSE)
    {
        return;
    }
    //MDrv_MBX_SetDbgLevel(MBX_DBG_LEVEL_ALL);
    //MDrv_PM_SetDbgLevel(E_PM_DBGLV_INFO);

	//To Do: MDrv_PM_Check_Version is used on checking PM version(zeo 2.0 or not?). However, the utopia driver for A5 doesn't update to the newest.
    // So the current head file doen't include this protoype.
    if(MDrv_PM_Check_Version(TRUE))
	{
        // Initialize IR before power down
        msAPI_IR_Initialize();
    }

    #if (CONFIG_KEYPAD)
    // Initialize SAR before power down
    msAPI_KeyPad_Initialize();
    #endif

    static PM_WakeCfg PmWakeCfg =
    {
        .bPmWakeEnableIR = TRUE,
        .bPmWakeEnableSAR = TRUE,
        .bPmWakeEnableGPIO0 = FALSE,
        .bPmWakeEnableGPIO1 = FALSE,
        .bPmWakeEnableUART1 = FALSE,
        .bPmWakeEnableSYNC = FALSE,
        .bPmWakeEnableESYNC = FALSE,

        .bPmWakeEnableRTC0 = FALSE,
        .bPmWakeEnableRTC1 = FALSE,
        .bPmWakeEnableDVI0 = FALSE,
        .bPmWakeEnableDVI2 = FALSE,
        .bPmWakeEnableAVLINK = FALSE,
        #if (CONFIG_ENABLE_WOL)
        .bPmWakeEnableWOL = TRUE,
        #else
        .bPmWakeEnableWOL = FALSE,
        #endif
		.u8PmStrMode = TRUE,

        .u8PmWakeIR =
        {   //IR wake-up key define
            IRKEY_POWER, 0x1e, 0x5F, 0x5E, 0x3B, 0x8C, 0x8D, 0xFF, //Mstar IR key code
            0x0F, 0x61, 0xA4, 0xA5, 0xFF, 0xFF, 0xFF, 0xFF,        //INSIGNIA, NEC, IR key code
            0x12, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,        //TOSHIBA IR key code
            0x46, 0xA0, 0x9F, 0x4A, 0xA1, 0x5F, 0xA3, 0xA2         //AMAZON IR key code
        },

        .u8PmWakeIR2 =
        {   //IR wake-up key define
            IRKEY_POWER, 0x1e, 0x5F, 0x5E,
            0x3B, 0x8C, 0x8D, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xA4, 0xA5, 0xFF, 0xFF   //AMAZON NEC type remote customized keys
        }
    };

    char* autoWakeup = getenv(HDMI_AUTO_WAKEUP_NAME);
    if (ENABLE_WAKE_CEC == 1 && (autoWakeup == NULL || autoWakeup[0] == '1'))
    {
        PmWakeCfg.bPmWakeEnableCEC = TRUE;
    }
    else
    {
        PmWakeCfg.bPmWakeEnableCEC = FALSE;
    }

    char* s = getenv(POWER_KEY_NAME);
    if (s)
    {
        PmWakeCfg.u8PmWakeIR[0] = simple_strtoul(s, NULL, 16);
    }
	PmWakeCfg.u8PmStrMode = str_cfg;

    //wol wakeup
    char *ethaddr;
    char *pmac;

    if ( NULL != (ethaddr = getenv("macaddr"))
                    && strlen(ethaddr) == strlen("XX:XX:XX:XX:XX:XX")
                    && ':' == ethaddr[2]
                    && ':' == ethaddr[5]
                    && ':' == ethaddr[8]
                    && ':' == ethaddr[11]
                    && ':' == ethaddr[14]
        )
    {
        pmac = (char*) malloc(strlen("XX:XX:XX:XX:XX:XX")+1);

        if(pmac != NULL)
        {
            memcpy(pmac, ethaddr, strlen("XX:XX:XX:XX:XX:XX"));
            pmac[2]  = '\0';
            pmac[5]  = '\0';
            pmac[8]  = '\0';
            pmac[11] = '\0';
            pmac[14] = '\0';
            pmac[17] = '\0';

            PmWakeCfg.u8PmWakeMACAddress[0]   = (u8)simple_strtoul(&(pmac[0]),  NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[1]   = (u8)simple_strtoul(&(pmac[3]),  NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[2]   = (u8)simple_strtoul(&(pmac[6]),  NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[3]   = (u8)simple_strtoul(&(pmac[9]),  NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[4]   = (u8)simple_strtoul(&(pmac[12]), NULL, 16);
            PmWakeCfg.u8PmWakeMACAddress[5]   = (u8)simple_strtoul(&(pmac[15]), NULL, 16);

            free(pmac);
         }
    }

    //printf("PM_Init...\n");
    flush_cache((MS_U32)&PmWakeCfg, sizeof(PM_WakeCfg));
    pmResult= MDrv_PM_Init((PM_WakeCfg*)VA2PA((MS_U32) &PmWakeCfg));


    PmPowerDownCfg.u8WakeAddress = E_PM_WAKE_ZERO ;//E_PM_LAST_TWOSTAGE_POWERDOWN;
    PmPowerDownCfg.u8PowerDownMode = E_PM_STANDBY;//E_PM_STANDBY;//E_PM_SLEEP;

    flush_cache((MS_U32)&PmPowerDownCfg, sizeof(PmPowerDownCfg));
    //printf("\n\r==== Enter Standby Mode !! ==== %d \n",(int)pmResult);
    MDrv_PM_PowerDown((PM_PowerDownCfg*)VA2PA((MS_U32) &PmPowerDownCfg));

    while(1);
}
void msAPI_Power_SetStrConfig(int para)
{
    str_cfg = para;
}
