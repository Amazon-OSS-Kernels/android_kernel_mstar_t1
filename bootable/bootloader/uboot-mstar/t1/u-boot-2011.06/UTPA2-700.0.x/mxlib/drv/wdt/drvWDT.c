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
#include "drvWDT.h"
#include "halWDT.h"
#include "regWDT.h"
#include "MsOS.h"

#include "ULog.h"

#define WDT_DBG_FUNC()            if (_geDbfLevel >= E_WDT_DBGLV_ALL) \
                                           {MS_DEBUG_MSG(ULOGI("WDT", "\t====   %s   ====\n", __FUNCTION__));}
#define WDT_DBG_INFO(x, args...)  if (_geDbfLevel >= E_WDT_DBGLV_INFO ) \
                                           {MS_DEBUG_MSG(ULOGI("WDT", x, ##args));}
#define WDT_DBG_ERR(x, args...)   if (_geDbfLevel >= E_WDT_DBGLV_ERR_ONLY) \
										   {MS_DEBUG_MSG(ULOGE("WDT", x, ##args));}

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static tmr_interrupt tTmrTbl[MAX_TIMER_NUM];	/* Table of timers managed by this module		*/
static MS_BOOL  _gbInitWDT = FALSE;
static WDT_DbgLv _geDbfLevel = E_WDT_DBGLV_ERR_ONLY;
static MSIF_Version _drv_wdt_version = {
    .DDI = { WDT_DRV_VERSION },
};

inline static void _MDrv_TIMER_Trigger(E_PIU_Timer eTimer, MS_U32 u32count);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_WDT_GetLibVer
/// @brief \b Function \b Description: Show the WDT driver version
/// @param <IN>        \b None:
/// @param <OUT>       \b **pVerString : output WDT driver version
/// @param <RET>       \b eRETCODE :
/// @param <GLOBAL>    \b VER: Library version string
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_GetLibVer(const MSIF_Version **ppVersion)
{
    WDT_DBG_FUNC();

    if (!ppVersion)
        return E_WDT_FAIL;

    *ppVersion = &_drv_wdt_version;
    return E_WDT_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_IsEnable
/// @brief \b Function  \b Description: check if WDT is Enable
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_IsEnable(void)
{
    WDT_DBG_FUNC();

    _gbInitWDT = HAL_WDT_IsEnable();

    if (!_gbInitWDT)
        WDT_DBG_ERR("Call MDrv_WDT_Start first!\n");
    return (WDT_Result)_gbInitWDT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Init
/// @brief \b Function  \b Description: Init and Start WDT
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Init(WDT_DbgLv eLevel)
{
    MS_VIRT virtBaseAddr;
    MS_PHY u32BaseSize = 0; /* No use */

    _geDbfLevel = eLevel;
    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);
    /*  This is not really necessary, WDT is enabled after MCU reset
        *   but it is important to get the IOMAP Base to access RIU.
        */
    if (!_gbInitWDT)
    {
        if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            WDT_DBG_ERR("Get IOMAP Base faill!\n");
            return E_WDT_FAIL;
        }

        HAL_WDT_SetIOMapBase(virtBaseAddr);
        HAL_WDT_Start();

        _gbInitWDT = TRUE;
        return E_WDT_OK;
    }
    else
    {
        WDT_DBG_ERR("WDT had initial!\n");
        return E_WDT_OK;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Stop
/// @brief \b Function  \b Description: Init WDT Stop
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Stop(WDT_DbgLv eLevel)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_Stop();

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Clear
/// @brief \b Function  \b Description: clear WDT.
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Clear(void)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_Clear();

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_ClearRstFlag
/// @brief \b Function  \b Description: clear WDT reset flag.
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_ClearRstFlag(void)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_ClearRstFlag();

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_IsReset
/// @brief \b Function  \b Description: check if WDT is reset
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_IsReset(void)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_IsReset();

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: sec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer(WDT_DbgLv eLevel, MS_U16 sec)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer(sec);

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: msec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer_ms(WDT_DbgLv eLevel, MS_U16 msec)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer_ms(msec);

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: usec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer_us(WDT_DbgLv eLevel, MS_U16 usec)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer_us(usec);

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetIntTimer
/// @brief \b Function  \b Description: Set INT Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: sec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetIntTimer(WDT_DbgLv eLevel, MS_U16 sec)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetIntTimer(sec);

    return eWDTRe;
}

//------- ------------------------------------------------------------------------------------------
/// Description : Store and resume WDT initial status for fast booting.
/// @param  EN_POWER_MODE    \b IN: The mode of power on/off
/// @return TRUE : succeed
/// @return FALSE : failed
/// @return 1: Resume OK; 2: Suspend OK; 3: Suspend failed
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_WDT_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		_gbInitWDT = FALSE;
		MDrv_TIMER_Exit();
		u16Return = 2;				// suspend OK
	}
	else if (u16PowerState == E_POWER_RESUME)
	{
		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
			MDrv_WDT_Init(E_WDT_DBGLV_ERR_ONLY);
			MDrv_TIMER_Init();
			_prev_u16PowerState = u16PowerState;
            u16Return = 1;		// resume OK
		}
		else
		{
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u16Return = 3;		// resume failed
		}
	}
	else
	{
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u16Return = FALSE;
	}

	return u16Return;
}
// PIU TIMER
inline static void _MDrv_TIMER_Trigger(E_PIU_Timer eTimer, MS_U32 u32count)
{
	if ( eTimer == E_TIMER_0 )
	{
		HAL_WDT_Write4Byte(TIMER_0_MAX_REG,u32count);
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,TIMER_DISABLE);
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_TRIGGER);
	}
	else
	{
		HAL_WDT_Write4Byte(TIMER_1_MAX_REG,u32count);
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,TIMER_DISABLE);
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_TRIGGER);
	}
	while(!MDrv_TIMER_HitMaxMatch(eTimer));
}

#if !defined(CONFIG_FRC)
static void MDrv_TIMER_Stop(E_PIU_Timer eTimer)
{
    if ( eTimer == E_TIMER_0 )
    {
        HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)&(~TIMER_ENABLE));
        HAL_WDT_WriteByte(TIMER_0_MATCH_REG,HAL_WDT_ReadByte(TIMER_0_MATCH_REG)|(TIMER_MATCH));
    }
    else
    {
        HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)&(~TIMER_ENABLE));
        HAL_WDT_WriteByte(TIMER_1_MATCH_REG,HAL_WDT_ReadByte(TIMER_1_MATCH_REG)|(TIMER_MATCH));
    }
}
#endif

void MDrv_TIMER_Count(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

    tmr_interrupt *ptmr;

    ptmr = &tTmrTbl[eTimer];
	ptmr->bTmrEn = bEnable;

	if ( eTimer == E_TIMER_0 )
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_ENABLE);
		else
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)&(~TIMER_ENABLE));
	}
	else
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_ENABLE);
		else
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)&(~TIMER_ENABLE));
	}
}

void MDrv_TIMER_INT(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

	if ( eTimer == E_TIMER_0 )
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_0_CTRL_REG+1)|TIMER_INTEN);
		else
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_0_CTRL_REG+1)&(~TIMER_INTEN));
	}
	else
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_1_CTRL_REG+1)|TIMER_INTEN);
		else
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_1_CTRL_REG+1)&(~TIMER_INTEN));
	}
}

MS_BOOL MDrv_TIMER_HitMaxMatch(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

	if ( eTimer == E_TIMER_0 )
	{
		return (HAL_WDT_ReadByte(TIMER_0_MATCH_REG)&TIMER_MATCH);
	}
	else
	{
		return (HAL_WDT_ReadByte(TIMER_1_MATCH_REG)&TIMER_MATCH);
	}
}

void MDrv_TIMER_Rst(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

	if ( eTimer == E_TIMER_0 )
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_ENABLE);
	else
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_ENABLE);
}

void MDrv_TIMER_SetMaxMatch(E_PIU_Timer eTimer, MS_U32 u32MaxTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

    tmr_interrupt *ptmr;

    ptmr = &tTmrTbl[eTimer];
	ptmr->u32TmrMax = u32MaxTimer;

	if ( eTimer == E_TIMER_0 )
		HAL_WDT_Write4Byte(TIMER_0_MAX_REG, u32MaxTimer);
	else
		HAL_WDT_Write4Byte(TIMER_1_MAX_REG, u32MaxTimer);
}

MS_U32 MDrv_TIMER_GetMaxMatch(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_MAX_REG));
	else
		return (HAL_WDT_Read4Byte(TIMER_1_MAX_REG));
}

MS_U32 MDrv_TIMER_GetCounter(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG));
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG));
}

MS_U32 MDrv_TIMER_GetSecond(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_HZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_HZ);
}

MS_U32 MDrv_TIMER_GetMs(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_KHZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_KHZ);
}

MS_U32 MDrv_TIMER_GetUs(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_MHZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_MHZ);
}

void MDrv_TIMER_Delay(E_PIU_Timer eTimer, MS_U32 u32Second)//@@@need restore status
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

    MS_U32 u32Salt = HAL_WDTCycles(u32Second);
	MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

	_MDrv_TIMER_Trigger(eTimer,u32Salt);
	MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
	MDrv_TIMER_Count(eTimer, ENABLE);
}

void MDrv_TIMER_DelayMs(E_PIU_Timer eTimer, MS_U32 u32MS)//@@@need restore status
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

	MS_U32 u32Salt = HAL_WDTCyclesM(u32MS);
	MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

	_MDrv_TIMER_Trigger(eTimer,u32Salt);
	MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
	MDrv_TIMER_Count(eTimer, ENABLE);
}

void MDrv_TIMER_DelayUs(E_PIU_Timer eTimer, MS_U32 u32US)//@@@need restore status
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

    printf("TIMER_DelayUs unSupported!! \n");

    if (0)
    {
      MS_U32 u32Salt = HAL_WDTCyclesU(u32US);
      MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

      _MDrv_TIMER_Trigger(eTimer,u32Salt);
      MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
      MDrv_TIMER_Count(eTimer, ENABLE);
    }

}

void MDrv_TIMER_Init(void)
{
    MS_VIRT virtBaseAddr;
    MS_PHY u32BaseSize = 0, idx;
    tmr_interrupt *ptmr;

    // Clear & disable all timers
    ptmr = &tTmrTbl[0];
    for( idx = 0; idx < MAX_TIMER_NUM; idx++ )
    {
		ptmr->bTmrEn = FALSE;
		ptmr->u32TmrInit = 0x00000000;
		ptmr->u32TmrMax = 0xFFFFFFFF;
		ptmr->TmrFnct = NULL;
		ptmr++;
    }

    // Get Memory Base
    if (!_gbInitWDT)
    {
        if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            WDT_DBG_ERR("Get IOMAP Base faill!\n");
            return;
        }

        HAL_WDT_SetIOMapBase(virtBaseAddr);

        _gbInitWDT = TRUE;
        return;
    }
    else
    {
        WDT_DBG_ERR("PIU timer had initial!\n");
        return;
    }
}

#if !defined(CONFIG_FRC)
static void DRV_TIMER0_PIU_Isr(void)
{
	tmr_interrupt *ptmr;

	ptmr = &tTmrTbl[E_TIMER_0];

	if(MDrv_TIMER_HitMaxMatch(E_TIMER_0))
	{
            WDT_DBG_INFO("%s(%d) =======> comes\n", __FUNCTION__ ,__LINE__);
            MDrv_TIMER_Stop(E_TIMER_0);
            ptmr->TmrFnct(ptmr->TmrFnctArg0,ptmr->TmrFnctArg1,ptmr->TmrFnctArg2);
            // restore tmr default
            //MDrv_TIMER_Rst(E_TIMER_0);
	}
}
static void DRV_TIMER1_PIU_Isr(void)
{
 	tmr_interrupt *ptmr;

	ptmr = &tTmrTbl[E_TIMER_1];

	if(MDrv_TIMER_HitMaxMatch(E_TIMER_1))
	{
            WDT_DBG_INFO("%s(%d) =======> comes\n", __FUNCTION__ ,__LINE__);
            MDrv_TIMER_Stop(E_TIMER_1);
            ptmr->TmrFnct(ptmr->TmrFnctArg0,ptmr->TmrFnctArg1,ptmr->TmrFnctArg2);
            // restore tmr default
            //MDrv_TIMER_Rst(E_TIMER_1);
	}
}
#endif

void MDrv_TIMER_CfgFnct(E_PIU_Timer eTimer, void (*fnct)(void *, void *, void *), void *arg0, void *arg1, void *arg2 )
{
	tmr_interrupt *ptmr;

	if( fnct )
	{
		if( eTimer < MAX_TIMER_NUM )
		{
			ptmr = &tTmrTbl[eTimer];
			//OS_ENTER_CRITICAL();
			ptmr->TmrFnct		= fnct;
			ptmr->TmrFnctArg0 	= arg0;
			ptmr->TmrFnctArg1 	= arg1;
			ptmr->TmrFnctArg2	= arg2;
			//OS_EXIT_CRITICAL();
			MDrv_TIMER_INT(eTimer, 1);
#if !defined(CONFIG_FRC)
			if( eTimer )
			{
				 MsOS_AttachInterrupt(E_INT_FIQ_EXTIMER1, (InterruptCb)DRV_TIMER1_PIU_Isr);
        			 MsOS_EnableInterrupt(E_INT_FIQ_EXTIMER1);
			}
			else
			{
				 MsOS_AttachInterrupt(E_INT_FIQ_EXTIMER0, (InterruptCb)DRV_TIMER0_PIU_Isr);
        			 MsOS_EnableInterrupt(E_INT_FIQ_EXTIMER0);
			}
#endif
		}
	}
}

void MDrv_TIMER_Exit(void)
{
    MS_U32 idx;
    tmr_interrupt *ptmr;

    // Clear & disable all timers
    for( idx = 0; idx < MAX_TIMER_NUM; idx++ )
    {
        ptmr = &tTmrTbl[idx];
		ptmr->bTmrEn = FALSE;
		ptmr->u32TmrInit = 0x00000000;
		ptmr->u32TmrMax = 0xFFFFFFFF;
		ptmr->TmrFnct = NULL;
		MDrv_TIMER_Count((E_PIU_Timer) idx, 0);
		MDrv_TIMER_SetMaxMatch((E_PIU_Timer) idx, ptmr->u32TmrMax);
		MDrv_TIMER_INT((E_PIU_Timer) idx, 0);
    }
}
