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
/// file    drvHDMITx.c
/// @brief  HDMI Tx Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#define  MDRV_HDMITX_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "regHDMITx2.h"
#include "halHDMIUtilTx2.h"
#include "halHDMITx2.h"
#include "drvHDMITx2.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief This routine initializes HDMI Tx module
/// @return None
/// HDMI driver is in @ref E_TASK_PRI_HIGH level
//------------------------------------------------------------------------------
MS_BOOL MDrv_HDMITx2_Init(void)
{
    MS_U32 u32PMRIUBaseAddress = 0, u32PMBankSize = 0;

    if(!MDrv_MMIO_GetBASE( &u32PMRIUBaseAddress, &u32PMBankSize, MS_MODULE_PM ))
    {
        printf("MApi_XC_Init Get PM BASE failure\n");
        return FALSE;
    }
    printf("Get IOMAP ID:%u Base:%lx!\n", MS_MODULE_PM, u32PMRIUBaseAddress);
    MHal_HDMITx_SetIOMapBase(u32PMRIUBaseAddress, u32PMRIUBaseAddress);

    MDrv_HDMITx2_Power_OnOff(TRUE);
    MHal_HDMITx2_DefaultTiming();
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief This routine will power on/off HDMITx clock (power saving)
/// @return None
//------------------------------------------------------------------------------
void MDrv_HDMITx2_Power_OnOff(MS_BOOL bEnable)
{
    MHal_HDMITx2_Power_OnOff(bEnable);
}

void MDrv_HDMITx2_SetInitTiming(MsHDMITX_VIDEO_TIMING TFlag1)
{
    MHal_HDMITx2_SetInitTiming(TFlag1);
}

