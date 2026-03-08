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
/// file   apiHDMITx.c
/// @brief  HDMI Tx Api
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdlib.h>
#include <string.h>
#else
#include <linux/time.h>
#endif /* #include MSOS_TYPE_LINUX_KERNEL */

#include "MsCommon.h"
#include "drvHDMITx2.h"
#include "apiHDMITx2.h"
//#include "drvMAD.h"

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

//*********************//
//        DVI / HDMI   //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine initial HDMI Tx Module
/// @return None
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx2_Init(void)
{
    return(MDrv_HDMITx2_Init());
}

//------------------------------------------------------------------------------
/// @brief This routine exits HDMI Tx Module
/// @return None
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx2_Exit(void)
{
    MDrv_HDMITx2_Power_OnOff(FALSE);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief This routine exits HDMI Tx Module
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx2_SetInitTiming(HDMITX_VIDEO_TIMING TFlag1)
{
    MsHDMITX_VIDEO_TIMING temp = E_HDMITX_RES_MAX;
	
    switch (TFlag1)
    {
        case HDMITX_RES_640x480p:
            temp = E_HDMITX_RES_640x480p;
            break;
        case HDMITX_RES_720x480i:
            temp = E_HDMITX_RES_720x480i;
            break;
        case HDMITX_RES_720x576i:
            temp = E_HDMITX_RES_720x576i;
            break;
        case HDMITX_RES_720x480p:
            temp = E_HDMITX_RES_720x480p;
            break;
        case HDMITX_RES_720x576p:
            temp = E_HDMITX_RES_720x576p;
            break;
        case HDMITX_RES_1280x720p_50Hz:
            temp = E_HDMITX_RES_1280x720p_50Hz;
            break;
        case HDMITX_RES_1280x720p_60Hz:
            temp = E_HDMITX_RES_1280x720p_60Hz;
            break;
        case HDMITX_RES_1920x1080i_50Hz:
            temp = E_HDMITX_RES_1920x1080i_50Hz;
            break;
        case HDMITX_RES_1920x1080i_60Hz:
            temp = E_HDMITX_RES_1920x1080i_60Hz;
            break;
        case HDMITX_RES_1920x1080p_24Hz:
            temp = E_HDMITX_RES_1920x1080p_24Hz;
            break;
        case HDMITX_RES_1920x1080p_25Hz:
            temp = E_HDMITX_RES_1920x1080p_25Hz;
            break;
        case HDMITX_RES_1920x1080p_30Hz:
            temp = E_HDMITX_RES_1920x1080p_30Hz;
            break;
        case HDMITX_RES_1920x1080p_50Hz:
            temp = E_HDMITX_RES_1920x1080p_50Hz;
            break;
        case HDMITX_RES_1920x1080p_60Hz:
            temp = E_HDMITX_RES_1920x1080p_60Hz;
            break;
        default:
            printf("[HDMITx]: Error video timing: %d\n", TFlag1);
            return;
    }

    MDrv_HDMITx2_SetInitTiming(temp);
}



