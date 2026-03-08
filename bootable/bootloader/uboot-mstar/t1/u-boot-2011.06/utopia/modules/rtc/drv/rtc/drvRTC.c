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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvWDT.c
/// @brief  Piu Watch Dog Timer Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Include Files
////////////////////////////////////////////////////////////////////////////////
#include "MsDevice.h"
#include "MsVersion.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "drvMMIO.h"
#include "drvRTC.h"
#include "halRTC.h"
#include "MsOS.h"
#include "utopia_dapi.h"

#include "ULog.h"

#define RTC_DEBUG   1

#ifdef RTC_DEBUG
    #define RTC_DBG_FUNC()            if (_rtcDbfLevel >= E_RTC_DBGLV_ALL) \
                                           {ULOGE("RTC","\t====   %s   ====\n", __FUNCTION__);}
    #define RTC_DBG_INFO(x, args...)  if (_rtcDbfLevel >= E_RTC_DBGLV_INFO ) \
                                           {ULOGE("RTC", x, ##args);}
    #define RTC_DBG_ERR(x, args...)   if (_rtcDbfLevel >= E_RTC_DBGLV_ERR_ONLY) \
                                           {ULOGE("RTC", x, ##args);}
#else
    #define RTC_DBG_FUNC()             while (0)
    #define RTC_DBG_INFO(x, args...)   while (0)
    #define RTC_DBG_ERR(x, args...)    while (0)
#endif

#define RTC_MUTEX_LOCK()            
#define RTC_MUTEX_UNLOCK()          
#define RTC_MUTEX_DELETE()

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL  _gbInitRTC = FALSE;
static RTC_DbgLv _rtcDbfLevel = E_RTC_DBGLV_ERR_ONLY;
static MSIF_Version _drv_rtc_version = {
    .DDI = { RTC_DRV_VERSION },
};
// for Fastboot 
static E_MS_DRV_RTC g_eRtc = 0;
static MS_U32 g_u32Freq = 0;
static MS_U32 g_u32Xtal =0;
static MS_U32 g_u32Sec = 0;
static InterruptCb g_pIntCb = NULL;

static void MDrv_RTC_Int_Mode(E_MS_DRV_RTC eRtc, InterruptCb pIntCb)
{
    if(pIntCb)
    {
        HAL_RTC_IntMask((E_MS_RTC)eRtc, DISABLE);
        MsOS_AttachInterrupt(E_INT_IRQ_RTC,pIntCb);
        MsOS_EnableInterrupt(E_INT_IRQ_RTC);
    }
    else
    {
        HAL_RTC_IntMask((E_MS_RTC)eRtc, ENABLE);
        MsOS_DetachInterrupt(E_INT_IRQ_RTC);
        MsOS_DisableInterrupt(E_INT_IRQ_RTC);
    }
}

MS_BOOL MDrv_RTC_GetLibVer(const MSIF_Version **ppVersion)
{
    RTC_DBG_FUNC();

    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_rtc_version;
    return TRUE;
}


MS_BOOL MDrv_RTC_Init(E_MS_DRV_RTC eRtc, MS_U32 u32Freq, MS_U32 u32Xtal, MS_U32 u32Sec,InterruptCb pIntCb)
{
    if(u32Xtal<u32Freq || u32Freq==0)
        return FALSE;
	// store for FastBoot 
	g_eRtc = eRtc;
	g_u32Freq = u32Freq;
	g_u32Xtal = u32Xtal;
	g_u32Sec = u32Sec;
	g_pIntCb = pIntCb;
    if (!(_gbInitRTC& (1<<eRtc)))
    {
        MS_VIRT virtBaseAddr;
        MS_PHY u32BaseSize;
        if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            RTC_DBG_ERR("Get IOMAP Base faill!\n");
            return FALSE;
        }
        HAL_RTC_SetIOMapBase(virtBaseAddr);
        //enable rtc
        HAL_RTC_RESET((E_MS_RTC)eRtc, ENABLE);
        //set control word
        HAL_RTC_Set_Frequency((E_MS_RTC)eRtc,u32Freq, u32Xtal);
//        RTC_DBG_INFO("u32Freq*u32Freq:%lx\n",u32Freq*u32Freq);
        //set the initialized value
        HAL_RTC_Set_Counter((E_MS_RTC)eRtc,0); //start from 0
        HAL_RTC_Loading ((E_MS_RTC)eRtc, ENABLE);//enabling this, the initialized value will be fed in
        //set the target value
        HAL_RTC_Match_Counter((E_MS_RTC)eRtc,u32Sec*u32Freq);
        //set callback function
        MDrv_RTC_Int_Mode(eRtc,pIntCb);
        //start counting
        HAL_RTC_Reading((E_MS_RTC)eRtc, ENABLE);
        HAL_RTC_Counter((E_MS_RTC)eRtc, ENABLE);
        HAL_RTC_Wrap_Count ((E_MS_RTC)eRtc, ENABLE);
        _gbInitRTC|=(1<<eRtc);
        return TRUE;
    }
    else
        return FALSE;
}

MS_U32 MDrv_RTC_GetCount(E_MS_DRV_RTC eRtc)
{
    if(_gbInitRTC)
        return HAL_RTC_Read_Counter((E_MS_RTC)eRtc);
    else
        return 0;
}

void MDrv_RTC_DeInit(E_MS_DRV_RTC eRtc)
{
    //if(_gbInitRTC&(1<<eRtc))
    //{
    HAL_RTC_RESET((E_MS_RTC)eRtc, DISABLE);
    HAL_RTC_Counter((E_MS_RTC)eRtc, DISABLE);
    //    _gbInitRTC&=(~(1<<eRtc));
    //}
}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register RTC module
//--------------------------------------------------------------------------------------------------
enum
{
    RTC_POOL_ID_RTC0=0
} eRTC_PoolID;

void RTCRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. create a module(module_name, SHM_size), and register to utopia2.0
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_RTC, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)RTCOpen, (FUtopiaClose)RTCClose, (FUtopiaIOctl)RTCIoctl);

    // 2. Resource register
    void* psResource = NULL;
    
    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, RTC_POOL_ID_RTC0);
    
    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("rtc0", sizeof(RTC_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, RTC_POOL_ID_RTC0);

    UtopiaModuleAddResourceEnd(pUtopiaModule, RTC_POOL_ID_RTC0);

    
}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to get a instance to use RTC
// @ \b in: 32ModuleVersion => this is for checking if API version is the same
//--------------------------------------------------------------------------------------------------
MS_U32 RTCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MS_VIRT virtRetAddr;
    MS_U32 u32RetShmID, u32RetShmSize;
    MS_U32 u32Ret, u32RetTmp;

    //Check DRV_REVISION
    if(u32ModuleVersion != RTC_VERSION)
    {
        RTC_DBG_ERR("\033[35mFunction = %s, Line = %d, ERROR, DRV_VERSION is not RTC_VERSION\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        //return FALSE;
    }

    // for multi-process safe, check if already other Instance exist
    RTC_MUTEX_LOCK();
    u32Ret = MsOS_SHM_GetId((MS_U8*)"RTC_Instance", sizeof(MS_U32), &u32RetShmID, &virtRetAddr, &u32RetShmSize, MSOS_SHM_QUERY);
    if(u32Ret == 0)
    {
        // first create, set value as 0
        u32RetTmp = MsOS_SHM_GetId((MS_U8*)"RTC_Instance", sizeof(MS_U32), &u32RetShmID, &virtRetAddr, &u32RetShmSize, MSOS_SHM_CREATE);
        
        if (u32RetTmp == 0)
        {
            RTC_DBG_ERR("[%s error] something wrong in MsOS_SHM_GetId\n", __FUNCTION__);
            RTC_DBG_ERR("is SHM_SIZE reached?\n");
            RTC_DBG_ERR("is MAX_SHM_CLIENT_NUM reached?\n");
            MS_ASSERT(0);
        }
        *(MS_VIRT *)virtRetAddr = 0;
    }

    if(*(MS_VIRT *)virtRetAddr == 1)
    {
        RTC_DBG_ERR("\033[35mFunction = %s, Line = %d, [Multi-process Safe] A Instance is existed!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        RTC_MUTEX_UNLOCK();
        return FALSE;
    }
    else
    {
        *(MS_VIRT *)virtRetAddr = 1;
        RTC_MUTEX_UNLOCK();
    }

    RTC_INSTANT_PRIVATE *pRTCPri = NULL;

    // instance is allocated here, also can allocate private for internal use, ex, RTC_INSTANT_PRIVATE, ppInstance will point to a pointer of the created UTOPIA_INSTANCE
    UtopiaInstanceCreate(sizeof(RTC_INSTANT_PRIVATE), ppInstance);
    // set the pRTCPri point to the private of UTOPIA_INSTANCE
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pRTCPri);
    
    RTC_INIT_PARAM *tmp = (RTC_INIT_PARAM*)pAttribute;

    MDrv_RTC_Init(tmp->eRtc, tmp->u32Freq, tmp->u32Xtal, tmp->u32Sec, tmp->pIntCb);

    return TRUE;
}

MS_U32 RTCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	return 0; // FIXME: error code
}

MS_U32 RTCClose(void* pInstance)
{
    MS_VIRT virtRetAddr;
    MS_U32 u32RetShmID, u32RetShmSize;
    MS_U32 u32Ret;

    virtRetAddr = 0 ; //fix coverity isue    
    UtopiaInstanceDelete(pInstance);
    
    u32Ret = MsOS_SHM_GetId((MS_U8*)"RTC_Instance", sizeof(MS_U32), &u32RetShmID, &virtRetAddr, &u32RetShmSize, MSOS_SHM_QUERY);
    
    if( (u32Ret == 0) || (*(MS_VIRT *)virtRetAddr == 0) )
    {
        return FALSE;
    }
    else
    {
        RTC_MUTEX_LOCK();
        *(MS_VIRT *)virtRetAddr = 0;
        RTC_MUTEX_UNLOCK();
        return TRUE;
    }
}


MS_U16 MDrv_RTC_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		_gbInitRTC = FALSE;
		u16Return = 2;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_RTC_Init( g_eRtc, g_u32Freq, g_u32Xtal, g_u32Sec,g_pIntCb);

			_prev_u16PowerState = u16PowerState;
			u16Return = 1;//RESUME_OK;
		}
		else
		{
			RTC_DBG_ERR("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = 3;//SUSPEND_FAILED;
		}
	}
	else
	{
		RTC_DBG_ERR("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = FALSE;
	}

	return u16Return;// for success
}





