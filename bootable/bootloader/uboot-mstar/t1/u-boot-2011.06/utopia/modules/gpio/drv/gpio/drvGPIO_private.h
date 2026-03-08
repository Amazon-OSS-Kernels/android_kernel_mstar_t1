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

#ifndef _DRVGPIO_PRIV_H_
#define _DRVGPIO_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
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
#include "drvGPIO.h"

typedef enum {
    MDrv_CMD_GPIO_Init,
    MDrv_CMD_GPIO_GetStauts,
    MDrv_CMD_GPIO_Set_High,
    MDrv_CMD_GPIO_Set_Low,
    MDrv_CMD_GPIO_Set_Input,
    MDrv_CMD_GPIO_Get_Inout,
    MDrv_CMD_GPIO_Get_Level,
    MDrv_CMD_GPIO_Attach_Interrupt,
    MDrv_CMD_GPIO_Detach_Interrupt,
    MDrv_CMD_GPIO_En_Interrupt,
    MDrv_CMD_GPIO_Dis_Interrupt,
    MDrv_CMD_GPIO_Dis_Interrupt_All,
    MDrv_CMD_GPIO_En_Interrupt_All,
    MDrv_CMD_GPIO_Interrupt_Action,
} eGpioIoctlOpt;


typedef struct _GPIO_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}GPIO_RESOURCE_PRIVATE;

typedef struct _GPIO_GET_STS_PARAM
{
    GPIO_Status *pSts;
}GPIO_GET_STS_PARAM, *PGPIO_GET_STS_PARAM;

typedef struct _GPIO_SET_HIGH_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_SET_HIGH_PARAM, *PGPIO_SET_HIGH_PARAM;

typedef struct _GPIO_SET_LOW_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_SET_LOW_PARAM, *PGPIO_SET_LOW_PARAM;

typedef struct _GPIO_SET_INPUT_PARAM
{
		MS_GPIO_NUM gpio_num;
}GPIO_SET_INPUT_PARAM, *PGPIO_SET_INPUT_PARAM;

typedef struct _GPIO_GET_INOUT_PARAM
{
    MS_U32 u32InOut;
    MS_GPIO_NUM gpio_num;
}GPIO_GET_INOUT_PARAM, *PGPIO_GET_INOUT_PARAM;

typedef struct _GPIO_GET_LEVEL_PARAM
{
    MS_U32 u32Level;
    MS_GPIO_NUM gpio_num;
}GPIO_GET_LEVEL_PARAM, *PGPIO_GET_LEVEL_PARAM;

typedef struct _GPIO_ATTACH_INTERRUPT_PARAM
{
		MS_GPIO_NUM gpio_num;
		GPIO_Edge gpio_edge_type;
		void* GPIO_Callback;
}GPIO_ATTACH_INTERRUPT_PARAM, *PGPIO_ATTACH_INTERRUPT_PARAM;

typedef struct _GPIO_DETACH_INTERRUPT_PARAM
{
    MS_GPIO_NUM gpio_num;
}GPIO_DETACH_INTERRUPT_PARAM, *PGPIO_DETACH_INTERRUPT_PARAM;

typedef struct _GPIO_ENABLE_INTERRUPT_PARAM
{
	MS_GPIO_NUM gpio_num;
}GPIO_ENABLE_INTERRUPT_PARAM, *PGPIO_ENABLE_INTERRUPT_PARAM;

typedef struct _GPIO_DISABLE_INTERRUPT_PARAM
{
	MS_GPIO_NUM gpio_num;
}GPIO_DISABLE_INTERRUPT_PARAM, *PGPIO_DISABLE_INTERRUPT_PARAM;


typedef union _GPIO_PRIVATE_PARAM
{
	 GPIO_GET_STS_PARAM							privatGpioSts;
	 GPIO_SET_HIGH_PARAM						privateGpioSetHigh;
	 GPIO_SET_LOW_PARAM							privateGpioSetLow;
	 GPIO_SET_INPUT_PARAM						privateGpioInput;
	 GPIO_GET_INOUT_PARAM						privateGpioGetInout;
	 GPIO_GET_LEVEL_PARAM						privateGpioGetLevel;
	 GPIO_ATTACH_INTERRUPT_PARAM		privateGpioAttachInterrupt;
	 GPIO_DETACH_INTERRUPT_PARAM		privateGpioDetachInterrupt;
	 GPIO_ENABLE_INTERRUPT_PARAM		privateGpioEnableInterrupt;
	 GPIO_DISABLE_INTERRUPT_PARAM		privateGpioDisableInterrupt;
}GPIO_PRIVATE_PARAM;

typedef void (*IOCTL_GPIO_GET_STS)(GPIO_Status*);
typedef void (*IOCTL_GPIO_INIT)(void);
typedef void (*IOCTL_GPIO_SET_H)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_SET_L)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_SET_INPUT)(MS_GPIO_NUM);
typedef int  (*IOCTL_GPIO_GET_INOUT)(MS_GPIO_NUM);
typedef int  (*IOCTL_GPIO_GET_LEVEL)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_ATTACH_INTERRUPT)(MS_GPIO_NUM,GPIO_Edge,void* GPIO_Callback);
typedef GPIO_Result (*IOCTL_GPIO_DETACH_INTERRUPT)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_ENALBE_INTERRUPT)(MS_GPIO_NUM);
typedef GPIO_Result (*IOCTL_GPIO_DISABLE_INTERRUPT)(MS_GPIO_NUM);
typedef void (*IOCTL_GPIO_DISABLE_INTERRUPT_ALL)(void);
typedef void (*IOCTL_GPIO_ENABLE_INTERRUPT_ALL)(void);
typedef void (*IOCTL_GPIO_INTERRUPT_ACTION)(void);

typedef struct _GPIO_INSTANT_PRIVATE
{
	IOCTL_GPIO_GET_STS								fpGpioGetSts;
	IOCTL_GPIO_INIT  									fpGpioInit;
	IOCTL_GPIO_SET_H									fpGpioSetH;
	IOCTL_GPIO_SET_L									fpGpioSetL;
	IOCTL_GPIO_SET_INPUT							fpGpioSetInput;
	IOCTL_GPIO_GET_INOUT							fpGpioGetInout;
	IOCTL_GPIO_GET_LEVEL							fpGpioGetLevel;
	IOCTL_GPIO_ATTACH_INTERRUPT				fpGpioAttachInterrupt;
	IOCTL_GPIO_DETACH_INTERRUPT				fpGpioDetachInterrupt;
	IOCTL_GPIO_DISABLE_INTERRUPT				fpGpioDisableInterrupt;
	IOCTL_GPIO_ENALBE_INTERRUPT				fpGpioEnableInterrupt;
	IOCTL_GPIO_DISABLE_INTERRUPT_ALL		fpGpioDisableInterruptAll;
	IOCTL_GPIO_ENABLE_INTERRUPT_ALL		fpGpioEnableInterruptAll;
	IOCTL_GPIO_INTERRUPT_ACTION				fpGpioInterruptAction;
}GPIO_INSTANT_PRIVATE;

void GPIORegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GPIOOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 GPIOClose(void* pInstance);
MS_U32 GPIOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 GPIOStr(MS_U32 u32PowerState, void* pModule);

void MDrv_GPIO_GetStatus_U2K(GPIO_Status *pStatus);
void mdrv_gpio_init_U2K(void);
void mdrv_gpio_set_high_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_set_low_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_set_input_U2K(MS_GPIO_NUM gpio);
int mdrv_gpio_get_inout_U2K(MS_GPIO_NUM gpio);
int mdrv_gpio_get_level_U2K(MS_GPIO_NUM gpio);
GPIO_Result mdrv_gpio_attach_interrupt_U2K(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback);
GPIO_Result mdrv_gpio_detach_interrupt_U2K(MS_GPIO_NUM gpio_num);
GPIO_Result mdrv_gpio_enable_interrupt_U2K(MS_GPIO_NUM gpio);
GPIO_Result mdrv_gpio_disable_interrupt_U2K(MS_GPIO_NUM gpio);
void mdrv_gpio_disable_interrupt_all_U2K(void);
void mdrv_gpio_enable_interrupt_all_U2K(void);
void mdrv_gpio_interrupt_action_U2K(void);

#ifdef __cplusplus
}
#endif
#endif // _DRVGPIO_PRIV_H_
