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

#ifndef MSAPI_POWER_H
#define MSAPI_POWER_H
#include <MsTypes.h>

//--------------------------
// REG_1040
//---------------------------
#define IR_SWITCH_PROTOCOL      0x1040


typedef enum
{
    EN_POWER_AC_BOOT,
    EN_POWER_DC_BOOT,
}EN_POWER_ON_MODE;

typedef enum
{
    PM_MODE_STANBY      = 0x01,
    PM_MODE_SLEEP       = 0x02,
    PM_MODE_DEEPSLEEP   = 0x03,
    PM_MODE_DEFAULT     = 0xFF,
}EN_PM_MODE;

typedef enum
{
    E_IR_NEC_IN = 0x01,
    E_IR_NEC_Toshiba = 0x02,
    E_IR_MAX = 0xFF,
} E_CUS_MULTI_IR;

MS_BOOL msAPI_PM_Reset(void);
MS_BOOL msAPI_PM_MBX_Init(void);

EN_POWER_ON_MODE msAPI_Power_QueryPowerOnMode(void);
void msAPI_Power_PowerDown_EXEC(void);
void msAPI_SetSPIOffsetForMCU(void);
void msAPI_Power_SetStrConfig(int para);

#endif
