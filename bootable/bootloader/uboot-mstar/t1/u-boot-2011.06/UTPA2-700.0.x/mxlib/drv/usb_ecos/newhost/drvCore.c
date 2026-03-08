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
//#include "include/drvCompiler.h" // NUSED
//#include "include/drvConfig.h" // NUSED
//#include "include/drvDevice.h" // NUSED
#include "include/drvKernel.h"
//#include "include/drvList.h" // NUSED

/**
     * @brief               generic device structure initialization
     *
     * @param           struct device_s *ms_gdev
     *
     * @return          none
     */
void ms_device_initialize(struct device_s *ms_gdev)
{
    ms_list_init(&ms_gdev->node);
    ms_list_init(&ms_gdev->children);
    ms_list_init(&ms_gdev->driver_list);
    ms_list_init(&ms_gdev->bus_list);
    osapi_atomic_set(&ms_gdev->dev_refcnt,1);
}

/**
     * @brief               generic decvice adding
     *
     * @param           struct device_s *ms_gdev
     *
     * @return          error code
     */
int ms_device_add(struct device_s *ms_gdev)
{
    struct device_s * parent;

    if (!ms_gdev || !strlen(ms_gdev->bus_id))
        return -EINVAL;

    parent = ms_gdev->parent;

    ms_usbhost_debug("DEV: registering device: ID = '%s', name = %s\n",
        ms_gdev->bus_id, ms_gdev->dev_name);

    if (parent)
        ms_insert_list_before(&ms_gdev->node,&parent->children);


    ms_bus_add_device(ms_gdev);

    return ENOERR;
}

/**
     * @brief               generic device deleting
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          none
     */
static void ms_device_del(struct device_s * ms_gdev)
{
    struct device_s * parent = ms_gdev->parent;

    if (parent)
        ms_list_remove_and_init(&ms_gdev->node);

    ms_bus_remove_device(ms_gdev);
}

/**
     * @brief               generic device unregistering
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          none
     */
void ms_device_unregister(struct device_s * ms_gdev)
{
    ms_usbhost_debug("DEV: Unregistering device. ID = '%s', name = '%s'\n",
        ms_gdev->bus_id,ms_gdev->dev_name);
    ms_device_del(ms_gdev);
    //calling device registed release function to releaes all device's allocated resouces
    if (ms_gdev->release_func)
        ms_gdev->release_func(ms_gdev);
}

