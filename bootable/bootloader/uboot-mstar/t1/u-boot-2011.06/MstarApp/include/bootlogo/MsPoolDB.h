/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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
#ifndef _SPI_DATABASE_H__
#define _SPI_DATABASE_H__
#include <ShareType.h>
#include <MsApiKeypad.h>
#include <apiPNL.h>

#include <MsApiPanel.h>
#include <gop/MsDrvGop.h>
#include <MsSystem.h>

#define BUFFER_SIZE                 128
#define spi_sector_size             0x10000
#define env_sector_size             0x20000

#define Kpd_ChID_Offset             20

#define PWM_DUTY   "u32PWMDuty"
#define PWM_PERIOD "u32PWMPeriod"
#define PWM_DIVPWM "u16DivPWM"
#define PWM_POLPWM "bPolPWM"
#define PWM_MAXPWM "u16MaxPWMvalue"
#define PWM_MINPWM "u16MinPWMvalue"
#define PWM_CH "pnl_pwmch"

#define KEYPAD_SEL "KEYPAD_SEL"

typedef enum
{
    E_DB_LOGO=0,
    E_DB_MUSIC,
    E_DB_BOARD_PARA,
    E_DB_PANEL_PARA,
    E_DB_URSA_6M30,
    E_DB_URSA,
    E_DB_TCON,
    E_DB_PNL_VB1,
    E_DB_PNL_GOPMUX,
    E_DB_PWM,
    E_DB_MISC,
    E_DB_MAX,
}DB_INDEX;

typedef struct
{
    U32 Offset;
    U32 Size;
    U32 u32CrcCheck;
}ST_DBDATA;

typedef struct
{
    ST_DBDATA dbdata[E_DB_MAX];
    U32 u32CrcCheck;
}ST_DBTABLE;


int write_dbtable(U32 u32LogoOffset);
int read_dbtable(U32 u32LogoOffset);

int Read_LogoToDisplayAddr(U32 u32LogoDramAddr);
int Read_PanelParaFromflash(PanelType * panel_data);
int Read_BoardParaFromflash(st_board_para * board_data);
int Read_KeypadParaFromflash(st_keypad_para *keypad_data);
int Read_MusicToPlayAddr(MS_U32 u32MusicDramAddr);
int Read_Ursa_6m30_Para(ursa_6m30_cmd_table * cmd_table);
int Read_Gop_Mux_ParaFromFlash(st_gopmux_para * gopmux_para);

#if (ENABLE_ENABLE_URSA == 1)
#if (ENABLE_URSA_6M30 == 1)
int iniparser_6m30_cmd(ursa_6m30_cmd_table * cmd_table);
int Load_Ursa6m30Para_ToFlash(void);
int Ursa_6M30_Setting(ursa_6m30_cmd_table * cmd_table);
#elif (ENABLE_URSA_8 == 1)|| (ENABLE_URSA_6M40 == 1)
int Read_Ursa_Para(ursa_cmd_table * cmd_table);
int Load_UrsaPara_ToFlash(void);
#if (ENABLE_URSA_8 == 1)
int Ursa_8_Setting(ursa_cmd_table * cmd_table);
#else
int Ursa_6M40_Syetting(ursa_cmd_table * cmd_table);
#endif

int Parser_Ursa_Ini(ursa_cmd_table * cmd_table);
#endif
#endif

int Read_TConFromFlash(U32 u32TConAddr);

int Read_PnlVb1FromFlash(U32 u32PnlVb1Addr);
int Read_MiscSetting_ToFlash(st_sys_misc_setting * misc_data);
int Read_PWMSetting_ToFlash(st_pwm_setting * pwm_data);
#endif //__SPI_DATABASE_H__
