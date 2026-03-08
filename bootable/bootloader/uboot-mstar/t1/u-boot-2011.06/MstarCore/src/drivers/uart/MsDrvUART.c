/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

//--------------------------------------------------------------------------------------------------
// Include Files
//--------------------------------------------------------------------------------------------------
#include <common.h>
#include <MsCommon.h>
#include <uboot_mmap.h>
#include <MsMmap.h>
#include <MsDebug.h>

#include <drvUART.h>


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


int MsDrv_UART_Rx_Enable(MS_BOOL bEnable)
{
    MDrv_UART_SetIOMapBase();
    return ((MDrv_UART_SetPMRxEnable(bEnable) == E_UART_OK)? 1 : 0) ;
}



