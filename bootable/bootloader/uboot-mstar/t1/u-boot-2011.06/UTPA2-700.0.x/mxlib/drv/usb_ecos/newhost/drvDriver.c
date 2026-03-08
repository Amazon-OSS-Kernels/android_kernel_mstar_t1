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

//#include <MsCommon.h> // NUSED
//#include "include/drvKernel.h" // NUSED
//#include "include/drvList.h" // NUSED
#include "include/drvDevice.h"

/**
     * @brief               generic driver registering
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          error code
     */
int ms_driver_register(struct device_driver * ms_drv)
{
    ms_list_init(&ms_drv->devices);
    // no mutex lock needed, only for USB driver
    return ms_bus_add_driver(ms_drv);
}

/**
     * @brief               generic driver unregistering
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          none
     */
void ms_driver_unregister(struct device_driver * ms_drv)
{
    ms_bus_remove_driver(ms_drv);
    // no semaphore-function needed
}

