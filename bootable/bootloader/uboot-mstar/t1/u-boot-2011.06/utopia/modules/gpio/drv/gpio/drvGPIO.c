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

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#if defined(MSOS_TYPE_LINUX)
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#endif


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_MSTAR_UTPA2K_GPIO_ENABLE
#define GPIO_UTOPIA20   (1)
#else
#define GPIO_UTOPIA20   (0)
#endif


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

static GPIO_Attribute _GPIO_Attri = E_GPIO_ATTRI_USER_DRV;

#if defined(MSOS_TYPE_LINUX)
struct GPIO_Reg
{
    MS_U32 u32Reg;
    MS_U8 u8Enable;
    MS_U8 u8BitMsk;
} __attribute__ ((packed));

typedef struct GPIO_Reg GPIO_Reg_t;

#define GPIO_IOC_MAGIC               'g'
#define MDRV_GPIO_INIT               _IO(GPIO_IOC_MAGIC, 0)
#define MDRV_GPIO_SET                _IOW(GPIO_IOC_MAGIC, 1, MS_U8)
#define MDRV_GPIO_OEN                _IOW(GPIO_IOC_MAGIC, 2, MS_U8)
#define MDRV_GPIO_ODN                _IOW(GPIO_IOC_MAGIC, 3, MS_U8)
#define MDRV_GPIO_READ               _IOWR(GPIO_IOC_MAGIC, 4, MS_U8)
#define MDRV_GPIO_PULL_HIGH          _IOW(GPIO_IOC_MAGIC, 5, MS_U8)
#define MDRV_GPIO_PULL_LOW           _IOW(GPIO_IOC_MAGIC, 6, MS_U8)
#define MDRV_GPIO_INOUT              _IOWR(GPIO_IOC_MAGIC, 7, MS_U8)
#define MDRV_GPIO_WREGB              _IOW(GPIO_IOC_MAGIC, 8, GPIO_Reg_t)
#define KERN_GPIO_OEN                _IOW(GPIO_IOC_MAGIC, 9, MS_U8)
#define KERN_GPIO_PULL_HIGH          _IOW(GPIO_IOC_MAGIC, 10, MS_U8)
#define KERN_GPIO_PULL_LOW           _IOW(GPIO_IOC_MAGIC, 11, MS_U8)
#define GPIO_FW_NAME      "/dev/gpio"
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------
#if (GPIO_UTOPIA20)
MS_BOOL bU20GpioOpened = FALSE;
void* pInstantGpio;
void* pAttributeGpio;
#include "drvGPIO_private.h"
#include "utopia.h"
MS_BOOL _MDrv_GPIO_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	if (FALSE == bU20GpioOpened)
	{
		if(UtopiaOpen(MODULE_GPIO|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
		{
			GPIO_DBG_ERR("Open GPIO fail\n");
			return FALSE;
		}
		bU20GpioOpened= TRUE;
	}
	return TRUE;
}
#endif

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

void MDrv_GPIO_GetStatus_U2K(GPIO_Status *pStatus)
{
    GPIO_DBG_FUNC();

    if(!_GPIO_Is_Init())
        return;

    pStatus->bInit = _gbInitGPIO;
    pStatus->u8DbgLevel = _geDbfLevel;
}

void MDrv_GPIO_GetStatus(GPIO_Status *pStatus)
{
#if (GPIO_UTOPIA20)
	PGPIO_GET_STS_PARAM  pGpioGetStsParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		printf("GPIO Get Status Open Fail \n");
	pGpioGetStsParam = (PGPIO_GET_STS_PARAM)malloc(sizeof(GPIO_GET_STS_PARAM));
	pGpioGetStsParam->pSts=pStatus;
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_GetStauts,(MS_U32*)pGpioGetStsParam);
	free(pGpioGetStsParam);
#else
	MDrv_GPIO_GetStatus_U2K(pStatus);
#endif
}

void mdrv_gpio_init_U2K(void)
{
    GPIO_DBG_FUNC();
	if(_GPIO_Attri == E_GPIO_ATTRI_USER_DRV)
	{
    if (_gbInitGPIO == true)
        return;
    _gbInitGPIO = TRUE;
    gpio_op = &__gpio;
    MDrv_GPIO_SetIOMapBase();
    HAL_GPIO_Interrupt_Init();
}
	else if(_GPIO_Attri == E_GPIO_ATTRI_KERNEL_DRV)
	{
#if defined(MSOS_TYPE_LINUX)
		int fd = -1;
		fd = open(GPIO_FW_NAME, O_RDWR|O_EXCL);
		if (fd == -1)
		{
			perror("");
		}
		else
		{
			if(-1 == ioctl(fd, MDRV_GPIO_INIT))
			{
				perror("");
				close(fd);
			}
		}
		close(fd);
#endif
	}
}

void mdrv_gpio_init(void)
{
#if (GPIO_UTOPIA20)
      printf("mdrv_gpio_init \n");
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		printf("GPIO Init Open Fail \n");
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Init,(MS_U32*)NULL);
#else
	mdrv_gpio_init_U2K();
#endif
}


void mdrv_gpio_set_high_U2K(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();
	if(_GPIO_Attri == E_GPIO_ATTRI_USER_DRV)
	{
    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_high(gpio);
    }
}
	else if(_GPIO_Attri == E_GPIO_ATTRI_KERNEL_DRV)
	{
#if defined(MSOS_TYPE_LINUX)
		int fd = -1;
		fd = open(GPIO_FW_NAME, O_RDWR|O_EXCL);
		if (fd == -1)
		{
			perror("");
		}
		else
		{
			if(-1 ==ioctl(fd, MDRV_GPIO_PULL_HIGH, &gpio))
			{
				perror("");
				close(fd);
			}
		}
		close(fd);
#endif
	}

}

void mdrv_gpio_set_high(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
	PGPIO_SET_HIGH_PARAM  pGpioSetHighParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		printf("GPIO Set High Open Fail \n");
	pGpioSetHighParam = (PGPIO_SET_HIGH_PARAM)malloc(sizeof(GPIO_SET_HIGH_PARAM));
	pGpioSetHighParam->gpio_num = gpio;
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Set_High,(MS_U32*)pGpioSetHighParam);
	free(pGpioSetHighParam);
#else
	mdrv_gpio_set_high_U2K(gpio);
#endif
}

void mdrv_gpio_set_low_U2K(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();
	if(_GPIO_Attri == E_GPIO_ATTRI_USER_DRV)
	{
    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_low(gpio);
    }
}
	else if(_GPIO_Attri == E_GPIO_ATTRI_KERNEL_DRV)
	{
#if defined(MSOS_TYPE_LINUX)
		int fd = -1;
		fd = open(GPIO_FW_NAME, O_RDWR|O_EXCL);
		if (fd == -1)
		{
			perror("");
		}
		else
		{
			if(-1 ==ioctl(fd, MDRV_GPIO_PULL_LOW, &gpio))
			{
				perror("");
				close(fd);
			}
		}
		close(fd);
#endif
	}
}

void mdrv_gpio_set_low(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
	PGPIO_SET_LOW_PARAM  pGpioSetLowParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		printf("GPIO Set Low Open Fail \n");
	pGpioSetLowParam = (PGPIO_SET_LOW_PARAM)malloc(sizeof(GPIO_SET_LOW_PARAM));
	pGpioSetLowParam->gpio_num = gpio;
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Set_Low,(MS_U32*)pGpioSetLowParam);
	free(pGpioSetLowParam);
#else
	mdrv_gpio_set_low_U2K(gpio);
#endif
}

void mdrv_gpio_set_input_U2K(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
	if(_GPIO_Attri == E_GPIO_ATTRI_USER_DRV)
	{
    if (gpio_op)
    {
        gpio_op->set_input(gpio);
    }
}
	else if(_GPIO_Attri == E_GPIO_ATTRI_KERNEL_DRV)
	{
#if defined(MSOS_TYPE_LINUX)
		int fd = -1;
		fd = open(GPIO_FW_NAME, O_RDWR|O_EXCL);
		if (fd == -1)
		{
			perror("");
		}
		else
		{
			if(-1 ==ioctl(fd, MDRV_GPIO_ODN, &gpio))
			{
				perror("");
				close(fd);
			}
		}
		close(fd);
#endif
	}
}

void mdrv_gpio_set_input(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
	PGPIO_SET_INPUT_PARAM  pGpioSetInputParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		printf("GPIO Set Low Open Fail \n");
	pGpioSetInputParam = (PGPIO_SET_INPUT_PARAM)malloc(sizeof(GPIO_SET_INPUT_PARAM));
	pGpioSetInputParam->gpio_num = gpio;
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Set_Input,(MS_U32*)pGpioSetInputParam);
	free(pGpioSetInputParam);
#else
	mdrv_gpio_set_input_U2K(gpio);
#endif
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
int mdrv_gpio_get_inout_U2K(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();
	if(_GPIO_Attri == E_GPIO_ATTRI_USER_DRV)
	{
    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        return gpio_op->get_inout(gpio);
    }
    else
			return 0;
	}
	else if(_GPIO_Attri == E_GPIO_ATTRI_KERNEL_DRV)
	{
#if defined(MSOS_TYPE_LINUX)
		int fd = -1;
		fd = open(GPIO_FW_NAME, O_RDWR|O_EXCL);
		int Ret;
		if (fd == -1)
		{
			perror("");
			return 0;
		}
		else
		{
			Ret=ioctl(fd, MDRV_GPIO_INOUT, &gpio);
			if(-1 ==Ret)
    {
				perror("");
				close(fd);
        return 0;
    }
}
		close(fd);
	return Ret;
#endif
	}
	return 0;
}


int mdrv_gpio_get_inout(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
	int u32InOutVal;
	PGPIO_GET_INOUT_PARAM pGpioGetInOutParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		return 0;
	pGpioGetInOutParam = (PGPIO_GET_INOUT_PARAM)malloc(sizeof(GPIO_GET_INOUT_PARAM));
	pGpioGetInOutParam->gpio_num=gpio;
	pGpioGetInOutParam->u32InOut=0;
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Get_Inout,(MS_U32*)pGpioGetInOutParam);
	u32InOutVal = pGpioGetInOutParam->u32InOut;
	free(pGpioGetInOutParam);
	return (int)u32InOutVal;
#else
	return mdrv_gpio_get_inout_U2K(gpio);
#endif
}



int mdrv_gpio_get_level_U2K(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();
	if(_GPIO_Attri == E_GPIO_ATTRI_USER_DRV)
	{
    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        return gpio_op->get_level(gpio);
    }
    else
			return 0;
	}
	else if(_GPIO_Attri == E_GPIO_ATTRI_KERNEL_DRV)
	{
#if defined(MSOS_TYPE_LINUX)
		int fd = -1;
		fd = open(GPIO_FW_NAME, O_RDWR|O_EXCL);
		int Ret;
		if (fd == -1)
    {
			perror("");
        return 0;
    }
		else
		{
			Ret=ioctl(fd, MDRV_GPIO_READ, &gpio);
			if(-1 ==Ret)
    {
				perror("");
				close(fd);
        return 0;
    }
}
		close(fd);
	return Ret;
#endif
	}
	return 0;
}

int mdrv_gpio_get_level(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
	int u32Level;
	PGPIO_GET_LEVEL_PARAM pGpioGetLevelParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		return 0;
	pGpioGetLevelParam = (PGPIO_GET_LEVEL_PARAM)malloc(sizeof(GPIO_GET_LEVEL_PARAM));
	pGpioGetLevelParam->gpio_num=gpio;
	pGpioGetLevelParam->u32Level=0;
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Get_Level,(MS_U32*)pGpioGetLevelParam);
	u32Level = pGpioGetLevelParam->u32Level;
	free(pGpioGetLevelParam);
	return (int)u32Level;
#else
	return mdrv_gpio_get_level_U2K(gpio);
#endif
}

GPIO_Result mdrv_gpio_attach_interrupt_U2K(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback)
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

GPIO_Result  mdrv_gpio_attach_interrupt(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback)
{
#if (GPIO_UTOPIA20)
	GPIO_Result  u32Ret;
	PGPIO_ATTACH_INTERRUPT_PARAM pGpioAttIntParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		return 0;
	pGpioAttIntParam = (PGPIO_ATTACH_INTERRUPT_PARAM)malloc(sizeof(GPIO_ATTACH_INTERRUPT_PARAM));
	pGpioAttIntParam->GPIO_Callback=pCallback;
	pGpioAttIntParam->gpio_edge_type=gpio_edge_type;
	pGpioAttIntParam->gpio_num=gpio_num;
	u32Ret=UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Attach_Interrupt,(MS_U32*)pGpioAttIntParam);
	free(pGpioAttIntParam);
	return (GPIO_Result)u32Ret;
#else
	return mdrv_gpio_attach_interrupt_U2K(gpio_num,gpio_edge_type,pCallback);
#endif
}

GPIO_Result mdrv_gpio_detach_interrupt_U2K(MS_GPIO_NUM gpio_num)
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

GPIO_Result  mdrv_gpio_detach_interrupt(MS_GPIO_NUM gpio_num)
{
#if (GPIO_UTOPIA20)
	GPIO_Result  u32Ret;
	PGPIO_DETACH_INTERRUPT_PARAM pGpioDetachIntParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		return 0;
	pGpioDetachIntParam = (PGPIO_DETACH_INTERRUPT_PARAM)malloc(sizeof(GPIO_DETACH_INTERRUPT_PARAM));
	pGpioDetachIntParam->gpio_num=gpio_num;
	u32Ret=UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Detach_Interrupt,(MS_U32*)pGpioDetachIntParam);
	free(pGpioDetachIntParam);
	return (GPIO_Result)u32Ret;
#else
	return mdrv_gpio_detach_interrupt_U2K(gpio_num);
#endif
}

GPIO_Result mdrv_gpio_enable_interrupt_U2K(MS_GPIO_NUM gpio)
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

GPIO_Result  mdrv_gpio_enable_interrupt(MS_GPIO_NUM gpio_num)
{
#if (GPIO_UTOPIA20)
	GPIO_Result  u32Ret;
	PGPIO_ENABLE_INTERRUPT_PARAM pGpioEnableIntParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		return 0;
	pGpioEnableIntParam = (PGPIO_ENABLE_INTERRUPT_PARAM)malloc(sizeof(GPIO_ENABLE_INTERRUPT_PARAM));
	pGpioEnableIntParam->gpio_num=gpio_num;
	u32Ret=UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_En_Interrupt,(MS_U32*)pGpioEnableIntParam);
	free(pGpioEnableIntParam);
	return (GPIO_Result)u32Ret;
#else
	return mdrv_gpio_enable_interrupt_U2K(gpio_num);
#endif
}

GPIO_Result mdrv_gpio_disable_interrupt_U2K(MS_GPIO_NUM gpio)
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

GPIO_Result  mdrv_gpio_disable_interrupt(MS_GPIO_NUM gpio_num)
{
#if (GPIO_UTOPIA20)
	GPIO_Result  u32Ret;
	PGPIO_DISABLE_INTERRUPT_PARAM pGpioDisableIntParam = NULL;
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		return 0;
	pGpioDisableIntParam = (PGPIO_DISABLE_INTERRUPT_PARAM)malloc(sizeof(GPIO_DISABLE_INTERRUPT_PARAM));
	pGpioDisableIntParam ->gpio_num=gpio_num;
	u32Ret=UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Dis_Interrupt,(MS_U32*)pGpioDisableIntParam);
	free(pGpioDisableIntParam);
	return (GPIO_Result)u32Ret;
#else
	return mdrv_gpio_disable_interrupt_U2K(gpio_num);
#endif
}

void mdrv_gpio_disable_interrupt_all_U2K(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Disable_Interrupt_all();
    }
}

void  mdrv_gpio_disable_interrupt_all(void)
{
#if (GPIO_UTOPIA20)
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		printf("gpio_disable Open Fail");
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Dis_Interrupt_All,(MS_U32*)NULL);
#else
	mdrv_gpio_disable_interrupt_all_U2K();
#endif
}

void mdrv_gpio_enable_interrupt_all_U2K(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Enable_Interrupt_all();
    }
}

void  mdrv_gpio_enable_interrupt_all(void)
{
#if (GPIO_UTOPIA20)
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		printf("gpio_disable Open Fail");
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_En_Interrupt_All,(MS_U32*)NULL);
#else
	mdrv_gpio_enable_interrupt_all_U2K();
#endif
}

void mdrv_gpio_interrupt_action_U2K(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Interrupt_Action();
    }
}

void  mdrv_gpio_interrupt_action(void)
{
#if (GPIO_UTOPIA20)
	if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
		printf("gpio_disable Open Fail");
	UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_Interrupt_Action,(MS_U32*)NULL);
#else
	mdrv_gpio_interrupt_action_U2K();
#endif
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

MS_BOOL MDrv_GPIO_Attribute(GPIO_Attribute GpioAttri)
{
      switch (GpioAttri)
      {
      case E_GPIO_ATTRI_USER_DRV:
         _GPIO_Attri = E_GPIO_ATTRI_USER_DRV;
          break;
      case E_GPIO_ATTRI_KERNEL_DRV:
          _GPIO_Attri = E_GPIO_ATTRI_KERNEL_DRV;
          break;
      case E_GPIO_ATTRI_UNKNOWN:

      default:
          return FALSE;

          break;
      }
      return TRUE;
}


#endif      //__DRVGPIO_C__

