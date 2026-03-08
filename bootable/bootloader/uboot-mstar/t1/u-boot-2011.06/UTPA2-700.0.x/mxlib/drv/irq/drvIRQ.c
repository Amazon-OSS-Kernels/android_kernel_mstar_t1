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
/// file    drvIRQ.c
/// @brief  IRQ Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvIRQ.h"
#include "halIRQ.h"
#include "drvMMIO.h"
#include "MsVersion.h"
#include "regIRQ.h"

#if defined(MSOS_TYPE_NUTTX)
#include "debug.h"
#endif
#include "utopia.h"

#include "ULog.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL gbIRQ_InitFg=FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  External Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_drvirq_version = {
    .DDI = { DRVIRQ_DRV_VERSION },
};
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MSOS_GetLibVer
/// @brief \b Function  \b Description: Show the MSOS driver version
/// @param ppVersion    \b Out: Library version string
/// @return             \b Result
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_IRQ_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_drvirq_version;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_IRQ_Init()
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    MS_VIRT u32Base = 0;
    MS_PHY u32Size = 0;

    if (gbIRQ_InitFg)
        return true;
    if (MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_IRQ))
    {
        HAL_IRQ_Set_IOMap(u32Base);
        HAL_IRQ_Init();
        gbIRQ_InitFg=true;
        return TRUE;
    }
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    return FALSE;
}

void MDrv_IRQ_Attach(MS_U32 u32Vector, void *pIntCb)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_Attach(u32Vector, pIntCb,0);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_Detech(MS_U32 u32Vector)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_Detech(u32Vector);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_Restore()
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_Restore();
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_MaskAll()
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_MaskAll(TRUE);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_Mask(MS_U32 u32Vector)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_Mask(u32Vector);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_UnMaskAll( )
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_MaskAll(FALSE);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_UnMask(MS_U32 u32Vector)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_UnMask(u32Vector);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_NotifyCpu(IRQ_CPU_TYPE eType)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_NotifyCpu(eType);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

MS_BOOL MDrv_IRQ_InISR()
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    return HAL_IRQ_InISR();
#else
    return FALSE;
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

MS_U32 MDrv_IRQ_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = UTOPIA_STATUS_FAIL;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		u16Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_IRQ_Init();

			_prev_u16PowerState = u16PowerState;
			u16Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
		}
		else
		{
			ULOGE("IRQ", "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
		}
	}
	else
	{
		ULOGE("IRQ", "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = UTOPIA_STATUS_FAIL;
	}

	return u16Return;// for success
}



