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
////////////////////////////////////////////////////////////////////////////////

#ifndef __DRVGPIO_C__
#define __DRVGPIO_C__

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsTypes.h"
#include "drvGPIO.h"
#include "halGPIO.h"
#include "drvMMIO.h"
#include "ULog.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TAG_GPIO "GPIO"

#define GPIO_DBG_FUNC()             if (_geDbfLevel >= E_GPIO_DBGLV_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_GPIO, "\t====   %s   ====\n", __FUNCTION__);)}
#define GPIO_DBG_INFO(x, args...)   if (_geDbfLevel >= E_GPIO_DBGLV_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_GPIO, x, ##args);)}
#define GPIO_DBG_ERR(x, args...)    if (_geDbfLevel >= E_GPIO_DBGLV_ERR_ONLY) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_GPIO, x, ##args);)}

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static const struct gpio_operations *gpio_op;
extern const struct gpio_operations __gpio;

static MS_BOOL _gbInitGPIO = FALSE;
static GPIO_Info _gsInfo;
static GPIO_DbgLv _geDbfLevel = E_GPIO_DBGLV_ERR_ONLY;
static MSIF_Version _drv_gpio_version = {
    .DDI = { GPIO_DRV_VERSION },
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------
static MS_BOOL _GPIO_Is_Init(void)
{
    GPIO_DBG_FUNC();
    if (!_gbInitGPIO)
        GPIO_DBG_ERR("Call MDrv_GPIO_Init first!\n");
    return _gbInitGPIO;
}

MS_BOOL MDrv_GPIO_SetIOMapBase(void)
{
    MS_VIRT u32MMIOBaseAdr;
    MS_PHY u32NonPMBankSize;

    GPIO_DBG_FUNC();

    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_GPIO))
    {
        GPIO_DBG_ERR("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    HAL_GPIO_SetIOMapBase(u32MMIOBaseAdr);

    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_PM))
    {
        GPIO_DBG_ERR("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    HAL_GPIO_SetIOMapBase_PM(u32MMIOBaseAdr);

    return TRUE;
}


//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
GPIO_Result MDrv_GPIO_GetLibVer(const MSIF_Version **ppVersion)
{
    GPIO_DBG_FUNC();

    if(!ppVersion)
        return E_GPIO_FAIL;

    *ppVersion = &_drv_gpio_version;
    return E_GPIO_OK;
}

GPIO_Result MDrv_GPIO_SetDbgLevel(GPIO_DbgLv eLevel)
{
    GPIO_DBG_INFO("%s level: %u\n", __FUNCTION__, eLevel);

    _geDbfLevel = eLevel;
    return E_GPIO_OK;
}

const GPIO_Info* MDrv_GPIO_GetInfo(void)
{
    GPIO_DBG_FUNC();

    if(!_GPIO_Is_Init())
        return (GPIO_Info *)0;
    _gsInfo.u32GPIONum = END_GPIO_NUM;
    _gsInfo.u32IOMap = 0;
    _gsInfo.u32IOMap_PM = 0;

    return (&_gsInfo);
}

void MDrv_GPIO_GetStatus(GPIO_Status *pStatus)
{
    GPIO_DBG_FUNC();

    if(!_GPIO_Is_Init())
        return;

    pStatus->bInit = _gbInitGPIO;
    pStatus->u8DbgLevel = _geDbfLevel;
}

void mdrv_gpio_init(void)
{
    GPIO_DBG_FUNC();

    if (_gbInitGPIO == true)
    {
        return;
    }
    _gbInitGPIO = TRUE;
    gpio_op = &__gpio;
    MDrv_GPIO_SetIOMapBase();
    HAL_GPIO_Interrupt_Init();
}


void mdrv_gpio_set_high(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_high(gpio);
    }
}

void mdrv_gpio_set_low(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_low(gpio);
    }
}

void mdrv_gpio_set_input(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_input(gpio);
    }
}
#if 0
void mdrv_gpio_set_output(MS_GPIO_NUM gpio)
{
    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_output(gpio);
    }
}
#endif
int mdrv_gpio_get_inout(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        return gpio_op->get_inout(gpio);
    }
    else
    {
        return 0;
    }
}

int mdrv_gpio_get_level(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        return gpio_op->get_level(gpio);
    }
    else
    {
        return 0;
    }
}

GPIO_Result mdrv_gpio_attach_interrupt(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback)
{
    GPIO_Result ret_val;

    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        ret_val = HAL_GPIO_Attach_Interrupt(gpio_num,gpio_edge_type,pCallback);
    }
    else
    {
        ret_val = E_GPIO_FAIL;
    }

    return ret_val;
}

GPIO_Result mdrv_gpio_detach_interrupt(MS_GPIO_NUM gpio_num)
{
    GPIO_Result ret_val;

    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        ret_val = HAL_GPIO_Datach_Interrupt(gpio_num);
    }
    else
    {
        ret_val = E_GPIO_FAIL;
    }

    return ret_val;
}

GPIO_Result mdrv_gpio_enable_interrupt(MS_GPIO_NUM gpio)
{
    GPIO_Result ret_val;

    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        ret_val = HAL_GPIO_Enable_Interrupt(gpio);
    }
    else
    {
        ret_val = E_GPIO_FAIL;
    }

    return ret_val;
}

GPIO_Result mdrv_gpio_disable_interrupt(MS_GPIO_NUM gpio)
{
    GPIO_Result ret_val;

    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        ret_val = HAL_GPIO_Disable_Interrupt(gpio);
    }
    else
    {
        ret_val = E_GPIO_FAIL;
    }

    return ret_val;
}

void mdrv_gpio_disable_interrupt_all(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Disable_Interrupt_all();
    }
}

void mdrv_gpio_enable_interrupt_all(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Enable_Interrupt_all();
    }
}

void mdrv_gpio_interrupt_action(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Interrupt_Action();
    }
}

MS_U16 MDrv_GPIO_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_gbInitGPIO = FALSE;
		_prev_u16PowerState = u16PowerState;
		return 2;				// suspend OK
	}
	else if (u16PowerState == E_POWER_RESUME)
	{
		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
			mdrv_gpio_init();
			_prev_u16PowerState = u16PowerState;
            u16Return = 1;		// resume OK
		}
		else
		{
            ULOGE(TAG_GPIO, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u16Return = 3;		// suspend failed
		}
	}
	else
	{
        ULOGE(TAG_GPIO, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u16Return = FALSE;
	}

	return u16Return;
}
#endif      //__DRVGPIO_C__

