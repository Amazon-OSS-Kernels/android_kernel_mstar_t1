/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvGPIO.h
/// @brief  GPIO Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*!
	\defgroup G_GPIO GPIO interface
    \ingroup  G_PERIPHERAL

	\brief
	GPIO (Generic Purpose Input/Output) is a generic pin on an integrated circuit, which can be programmed to be used to either
	accept input or provide output to external devices depending on user desires and application requirements

	<b> Features </b>
	- GPIO pin direction can be set by software
	- User/Device can read value of GPIO register
	- Support interrupt enable/disable mode
	- Support diffrent power state of certain GPIO pin

    \defgroup G_GPIO_INIT Initialization Task relative
    \ingroup  G_GPIO
    \defgroup G_GPIO_COMMON Common Task relative
    \ingroup  G_GPIO
    \defgroup G_GPIO_CONTROL Control relative
    \ingroup  G_GPIO
    \defgroup G_GPIO_INT  Interrupt relative
    \ingroup  G_GPIO
    \defgroup G_GPIO_ToBeModified GPIO api to be modified
    \ingroup  G_GPIO
    \defgroup G_GPIO_ToBeRemove GPIO api to be removed
    \ingroup  G_GPIO
*/

#ifndef __DRVGPIO_H__
#define __DRVGPIO_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

#define MSIF_GPIO_LIB_CODE                     {'G','P','I','O'}    //Lib code
#define MSIF_GPIO_LIBVER                       {'0','3'}            //LIB version
#define MSIF_GPIO_BUILDNUM                     {'0','6'}            //Build Number
#define MSIF_GPIO_CHANGELIST                   {'0','0','5','5','7','0','9','3'} //P4 ChangeList Number

#define GPIO_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_GPIO_LIB_CODE,                        /* IP__                                             */  \
    MSIF_GPIO_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_GPIO_BUILDNUM,                        /* 00 ~ 99                                          */  \
    MSIF_GPIO_CHANGELIST,                      /* CL#                                              */  \
    MSIF_OS

typedef enum _GPIO_DbgLv
{
    E_GPIO_DBGLV_NONE,          //no debug message
    E_GPIO_DBGLV_ERR_ONLY,      //show error only
    E_GPIO_DBGLV_REG_DUMP,      //show error & reg dump
    E_GPIO_DBGLV_INFO,          //show error & informaiton
    E_GPIO_DBGLV_ALL            //show error, information & funciton name
}GPIO_DbgLv;

typedef enum _GPIO_Result
{
    E_GPIO_NOT_SUPPORT = -1,
    E_GPIO_FAIL = 0,
    E_GPIO_OK = 1
}GPIO_Result;

typedef struct _GPIO_Info
{
    MS_U32      u32GPIONum;
    MS_U32      u32IOMap;
    MS_U32      u32IOMap_PM;
}GPIO_Info;

typedef struct _GPIO_Status
{
    MS_U8   u8DbgLevel;
    MS_BOOL bInit;
}GPIO_Status;

typedef int MS_GPIO_NUM;        ///< GPIO handle, for default operations,

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GPIO_SetIOMapBase(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
GPIO_Result MDrv_GPIO_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
GPIO_Result MDrv_GPIO_SetDbgLevel(GPIO_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
const GPIO_Info* MDrv_GPIO_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_GPIO_GetStatus(GPIO_Status *pStatus);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void mdrv_gpio_init(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void mdrv_gpio_set_high(MS_GPIO_NUM gpio);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void mdrv_gpio_set_low(MS_GPIO_NUM gpio);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void mdrv_gpio_set_input(MS_GPIO_NUM gpio);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
//void mdrv_gpio_set_output(MS_GPIO_NUM gpio);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
int mdrv_gpio_get_inout(MS_GPIO_NUM gpio);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
int mdrv_gpio_get_level(MS_GPIO_NUM gpio);

typedef enum _GPIO_Edge
{
    E_GPIO_RISING_EDGE,
    E_GPIO_FALLING_EDGE
} GPIO_Edge;

/// Callback function which is called in GPIO ISR.
typedef void ( *GPIO_Callback ) (void);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_INT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
GPIO_Result mdrv_gpio_attach_interrupt(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_INT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
GPIO_Result mdrv_gpio_detach_interrupt(MS_GPIO_NUM gpio_num);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_INT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void mdrv_gpio_interrupt_action(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_INT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void mdrv_gpio_enable_interrupt_all(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_INT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void mdrv_gpio_disable_interrupt_all(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_INT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
GPIO_Result mdrv_gpio_enable_interrupt(MS_GPIO_NUM gpio);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_INT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
GPIO_Result mdrv_gpio_disable_interrupt(MS_GPIO_NUM gpio);

// control power state for fast booting
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_GPIO_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_GPIO_SetPowerState(EN_POWER_MODE u16PowerState);

#ifdef __cplusplus
}
#endif

#endif /* __DRVGPIO_H__ */

