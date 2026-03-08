/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <command.h>
#include <common.h>
#include <drvMBX.h>
#include <MsDebug.h>
#include <MsOS.h>
#include <MsSystem.h>
#include <ShareType.h>
#include <drvGPIO.h>
#include <drvPWM.h>
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PWM_CONFIG_LED_FREQUENCY     12  //PWM_CONFIG_FLICKER 0~255
#define PWM_CONFIG_PWM_PORT          E_PWM_CH4

typedef enum
{
    //Aeon-->51 Command Index
    PM_CMDIDX_INIT = 0x01,                  /// PM Command Index is INIT
    PM_CMDIDX_GET_STATUS = 0x02,            /// PM Command Index is GET STATUS
    PM_CMDIDX_GET_LIBVER = 0x03,            /// PM Command Index is GET LIBVER
    PM_CMDIDX_POWER_DOWN = 0x04,            /// PM Command Index is POWER DOWN
    PM_CMDIDX_RTC_INIT = 0x05,              /// PM Command Index is RTC INIT
    PM_CMDIDX_RTC_SETCOUNTER = 0x06,        /// PM Command Index is SET COUNTER
    PM_CMDIDX_RTC_GETCOUNTER = 0x07,        /// PM Command Index is GET COUNTER
    PM_CMDIDX_RTC_SETMATCHCOUNTER = 0x08,   /// PM Command Index is SET MATCH COUNTER
    PM_CMDIDX_RTC_GETMATCHCOUNTER = 0x09,   /// PM Command Index is GET MATCH COUNTER
    PM_CMDIDX_GET_INFO = 0x0A,              /// PM Command Index is GET INFO
    PM_CMDIDX_GET_MASTERKEY = 0x0B,
    PM_CMDIDX_GET_DEVICEID = 0x0C,
    PM_CMDIDX_GET_CHIPID = 0x0D,
    PM_CMDIDX_GPIO_INIT = 0x0E,

    PM_CMDIDX_SET_ONBOARD_LED = 0x11,
    PM_CMDIDX_SET_POWER_LED_N_PROGRAM_TIMER_LED = 0x12,
    PM_CMDIDX_GET_SYSTEM_ALIVE_STATUS    = 0x23,
    PM_CMDIDX_WDT_STATUS  = 0x24,
    PM_CMDIDX_UPATE_POWER_LED_STATUS = 0X25,

    PM_CMDIDX_ACK_51ToAEON = 0x30,          /// PM Command Index is ACK 51 To AEON

    //51->Aeon Command Index
    PM_CMDIDX_ACK_AEONTo51 = 0xA0,          /// PM Command Index is ACK AEON To 51

} PM_CmdIndex;

#define BIT(x)                              (1<<x)
#define MBX_CTRL_INSTANT         BIT(2)

typedef enum
{
    E_ON_BOARD_LED_OFF= 0,
    E_ON_BOARD_LED_PATTERN_1 = 1,
    E_ON_BOARD_LED_PATTERN_2 = 2,
    E_ON_BOARD_LED_PATTERN_3 = 3,
    E_ON_BOARD_LED_PATTERN_4 = 4,
    E_ON_BOARD_LED_PATTERN_5 = 5,
    E_ON_BOARD_LED_ON= 255,
} USB_LED_PATTERN;

typedef enum
{
    E_ON_BOARD_LED_STAGE_BIST= 0,
    E_ON_BOARD_LED_STAGE_MBOOT_SUCCESS=1,
    E_ON_BOARD_LED_STAGE_APL_SUCCESS= 2,
    E_ON_BOARD_LED_STAGE_UPDATE_SW= 3,
    E_ON_BOARD_LED_STAGE_MBOOT_FAIL=4,
} ON_BOARD_LED_STAGE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
U8 Is_MBX_Init = FALSE;

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void do_Led_Pwm_Flicker_On(void)
{
    PWM_Result ret = 0;
    PWM_ChNum PWMPort = PWM_CONFIG_PWM_PORT;
    MS_U32 u32Duty = 0x8000;
    MS_U32 u32Period = 0xffff;
    MS_U16 u16DivPWM = PWM_CONFIG_LED_FREQUENCY;

    ret = MDrv_PWM_Init(0);
    if (ret == E_PWM_FAIL)
    {
        printf ("LED_PWM_ON:MDrv_PWM_Init fail\n");
    }

    ret =MDrv_PWM_Oen(PWMPort, FALSE);    // Set 0 for output enable
    if (ret == E_PWM_FAIL)
    {
        printf ("LED_PWM_ON:MDrv_PWM_Oen fail\n");
    }

    ret = MDrv_PWM_Period(PWMPort, u32Period);
    if (ret == E_PWM_FAIL)
    {
        printf ("LED_PWM_ON:MDrv_PWM_Period fail\n");
    }

    ret = MDrv_PWM_DutyCycle(PWMPort, u32Duty);
    if (ret == E_PWM_FAIL)
    {
        printf ("LED_PWM_ON:MDrv_PWM_DutyCycle fail\n");
    }

    MDrv_PWM_Div(PWMPort, u16DivPWM);
    MDrv_PWM_Polarity(PWMPort, 1);
    MDrv_PWM_Vdben(PWMPort, FALSE);
    MDrv_PWM_Dben(PWMPort,FALSE);
}

void do_Led_Pwm_Flicker_Off(void)
{
    PWM_Result ret = 0;
    PWM_ChNum PWMPort = PWM_CONFIG_PWM_PORT;

    ret = MDrv_PWM_Init(0);
    if (ret == E_PWM_FAIL)
    {
        printf ("LED_PWM_OFF:MDrv_PWM_Init fail\n");
    }

    ret =MDrv_PWM_Oen(PWMPort, TRUE);    // Set 1 for output disable
    if (ret == E_PWM_FAIL)
    {
        printf ("LED_PWM_OFF:MDrv_PWM_Oen fail\n");
    }
}

void do_Led_Flicker_Control(void)
{
    static U8 u8LedStatus = 0;
    if(u8LedStatus != 0)
    {
        LED_RED_ON();
    }
    else
    {
        LED_RED_OFF();
    }
    u8LedStatus = ~u8LedStatus;
}

MS_S32 g_s32TimerID = -1;
void SetTimerIsrDone(void)
{
    if(g_s32TimerID >= 0)
    {
        MsOS_StopTimer(g_s32TimerID);
        MsOS_DeleteTimer(g_s32TimerID);
        g_s32TimerID = -1;
        printf("\nTimer ISR finished\n");
    }
}

void TimerISR(MS_U32 u32Tick,MS_U32 u32TimerID)
{
    u32Tick    = u32Tick;
    u32TimerID = u32TimerID;
    do_Led_Flicker_Control();
}

void do_Timer_ISR_Register(MS_U32 u32Timer)
{
    if(g_s32TimerID == -1)
    {
        g_s32TimerID = MsOS_CreateTimer( (TimerCb)TimerISR,
                                                      0,
                                                      u32Timer,
                                                      TRUE,
                                                      "Check timer");
        if(g_s32TimerID >= 0)
            printf("\t Creat Timer Success,TimerId = %ld\n",g_s32TimerID);
        else
            printf("\t Creat Timer Failed...\n");
    }
}

MS_BOOL SetOnBoardLED(U8 u8Pattern, U8 u8Index, U8 u8Stage)
{
    MS_BOOL bRet = FALSE;

    MBX_Msg stMsg;
    MBX_Result  eMBXRet = E_MBX_ERR_NOT_INITIALIZED;

 if(Is_MBX_Init == FALSE)
 {
     MsApi_MBX_Init();
     eMBXRet = MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_WAIT, 6);
     if(E_MBX_SUCCESS != eMBXRet)
     {
         printf("E_MBX_CLASS_PM_WAIT - MBX register msg error\n");
     }
     Is_MBX_Init=TRUE;
 }

    memset(&stMsg, 0, sizeof(MBX_Msg));

    stMsg.eRoleID = E_MBX_ROLE_PM;
    stMsg.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    stMsg.u8Ctrl = MBX_CTRL_INSTANT;
    stMsg.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
    stMsg.u8Index = u8Index;
    stMsg.u8ParameterCount = 2;
    stMsg.u8Parameters[0] = u8Pattern;
    stMsg.u8Parameters[1] = u8Stage;

    if(((eMBXRet = MDrv_MBX_SendMsg(&stMsg)) != E_MBX_SUCCESS))
    {
        printf(">>>Send Mail fail: 0x%x.\n", eMBXRet);
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }

    return bRet;
}

int do_led(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char u8Pattern_mode= 0;
    char u8Index_mode= 0;
    int interval_time =0;
    if (argc < 1)
    {
        cmd_usage(cmdtp);
    }

	if(strncmp(argv[1], "mbx", 4) == 0)
	{
		{
			u8Pattern_mode=simple_strtoul(argv[2],NULL,16);
			u8Index_mode=simple_strtoul(argv[3],NULL,16);
			SetOnBoardLED(u8Pattern_mode, u8Index_mode, E_ON_BOARD_LED_STAGE_UPDATE_SW);
		}
	}
    else if(strncmp(argv[1], "time",5) == 0)
    {
        interval_time=simple_strtoul(argv[2],NULL,16);  //timer
        do_Timer_ISR_Register(interval_time);
    }
    else if(strncmp(argv[1], "stop",5) == 0)
    {
		SetTimerIsrDone();
    }
    else if(strncmp(argv[1], "pwm_start",10) == 0)
    {
        do_Led_Pwm_Flicker_On();
    }
    else if(strncmp(argv[1], "pwm_stop",9) == 0)
    {
        do_Led_Pwm_Flicker_Off();
    }
    else
    {
		cmd_usage(cmdtp);
    }

    return 0;
}

int do_led_red(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    LED_GREEN_OFF();
    LED_RED_ON();
    return 0;
}

int do_led_green(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    LED_RED_OFF();
    LED_GREEN_ON();
    return 0;
}
