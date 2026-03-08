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

#ifndef _DRVPWM_PRIV_H_
#define _DRVPWM_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvPWM.h
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvPWM.h"

typedef PWM_Result (*IOCTL_PWM_OEN)(PWM_ChNum, MS_BOOL);
typedef PWM_Result (*IOCTL_PWM_PERIOD)(PWM_ChNum, MS_U32);
typedef PWM_Result (*IOCTL_PWM_DUTYCYCLE)(PWM_ChNum, MS_U32);
typedef PWM_Result (*IOCTL_PWM_UNITDIV)(MS_U16);
typedef void (*IOCTL_PWM_DIV)(PWM_ChNum, MS_U16);
typedef void (*IOCTL_PWM_POLARITY)(PWM_ChNum, MS_BOOL);
typedef void (*IOCTL_PWM_VDBEN)(PWM_ChNum, MS_BOOL);
typedef void (*IOCTL_PWM_RESETEN)(PWM_ChNum, MS_BOOL);
typedef void (*IOCTL_PWM_DBEN)(PWM_ChNum, MS_BOOL);
typedef PWM_Result (*IOCTL_PWM_SHIFT)(PWM_ChNum, MS_U32);
typedef void (*IOCTL_PWM_NVSYNC)(PWM_ChNum, MS_BOOL);

typedef struct _PWM_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}PWM_RESOURCE_PRIVATE;

typedef struct _PWM_INSTANT_PRIVATE
{
    IOCTL_PWM_OEN		fpPWMOen;
    IOCTL_PWM_PERIOD		fpPWMPeriod;
    IOCTL_PWM_DUTYCYCLE		fpPWMDutyCycle;
    IOCTL_PWM_UNITDIV		fpPWMUnitDiv;
    IOCTL_PWM_DIV		fpPWMDiv;
    IOCTL_PWM_POLARITY		fpPWMPolarity;
    IOCTL_PWM_VDBEN		fpPWMVdben;
    IOCTL_PWM_RESETEN		fpPWMResetEn;
    IOCTL_PWM_DBEN		fpPWMDben;
    IOCTL_PWM_SHIFT		fpPWMShift;
    IOCTL_PWM_NVSYNC		fpPWMNvsync;

}PWM_INSTANT_PRIVATE;

void PWMRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 PWMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 PWMClose(void* pInstance);
MS_U32 PWMIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif // _DRVPWM_PRIV_H_
