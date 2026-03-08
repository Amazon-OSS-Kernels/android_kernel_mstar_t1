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
//#include "include/drvPorts.h" // NUSED
//#include "include/drvConfig.h" // NUSED
//#include "include/drvCompiler.h" // NUSED
//#include "include/drvList.h" // NUSED
#include "include/drvKernel.h"

/**
     * @brief               binding generic device to driver
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          none
     */
void ms_device_bind_driver(struct device_s * ms_gdev)
{
    ms_usbhost_debug("bound device '%s' to driver '%s'\n",
        ms_gdev->bus_id,ms_gdev->driver->name);
    ms_insert_list_before(&ms_gdev->driver_list,&ms_gdev->driver->devices);
}

/**
     * @brief               generic device and driver matching
     *
     * @param           struct device_s * ms_gdev
     * @param           struct device_driver * ms_drv
     *
     * @return          error code
     */
static int ms_bus_match(struct device_s * ms_gdev, struct device_driver * ms_drv)
{
    int ms_err = -ENODEV;

    // Call BUS provided match function to match device ID witch specific comparing method
    if (ms_gdev->bus_type->match_func(ms_gdev,ms_drv))
    {
        ms_gdev->driver = ms_drv;
        // Match device ==> call driver's probe function
        if (ms_drv->probe)
        {
            if ((ms_err = ms_drv->probe(ms_gdev)))
            {
                ms_gdev->driver = NULL;
                return ms_err;
            }
        }
        ms_device_bind_driver(ms_gdev);
        ms_err = 0;
    }

    return ms_err;
}

/**
     * @brief               generic device attaching to bus
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          error code
     */
static int ms_device_attach(struct device_s * ms_gdev)
{
    struct ms_bus_type * ms_bus = ms_gdev->bus_type;
    struct list_head * entry;
    struct device_driver  * ms_drv;
    struct list_head *_mtr;

    if (ms_gdev->driver)
    {
        ms_device_bind_driver(ms_gdev);
        return 1;
    }

    if (ms_bus->match_func)
    {
        list_for_loop(entry,&ms_bus->drivers_list)
        {
        _mtr = entry ;
        ms_drv = (struct device_driver *)((char *)_mtr - (char *)offsetof(struct device_driver, bus_list) );
        if (!ms_bus_match(ms_gdev,ms_drv))
            return 1;
        }
    }

    return 0;
}

/**
     * @brief               generic driver attaching to bus
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          none
     */
void ms_driver_attach(struct device_driver * ms_drv)
{
    struct ms_bus_type * ms_bus = ms_drv->bus;
    struct list_head * entry;
    struct device_s * ms_gdev;
    const struct list_head *_mtr;

    if (!ms_bus->match_func)
        return;

    list_for_loop(entry,&ms_bus->devices_list)
    {
        _mtr = entry ;
        ms_gdev = (struct device_s *)((char *)_mtr - (char *)offsetof(struct device_s, bus_list) );
        if (!ms_gdev->driver)
        {
            ms_bus_match(ms_gdev,ms_drv);
        }
    }
}

/**
     * @brief               generic device release its driver
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          none
     */
void ms_device_release_driver(struct device_s * ms_gdev)
{
    struct device_driver * ms_drv = ms_gdev->driver;

    if (ms_drv)
    {
        ms_list_remove_and_init(&ms_gdev->driver_list);
        if (ms_drv->remove)
            ms_drv->remove(ms_gdev);
        ms_gdev->driver = NULL;
    }
}

/**
     * @brief               generic driver release its binding device
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          none
     */
static void ms_driver_detach(struct device_driver * ms_drv)
{
    struct list_head * entry, * next;
    struct device_s *ms_gdev;
    struct list_head *_mptr;

    list_for_loop_ex(entry,next,&ms_drv->devices)
    {
        // struct device * dev = container_of(entry,struct device,driver_list);
        _mptr = entry;
        ms_gdev = (struct device_s *)( (char *)_mptr - (char *)offsetof(struct device_s, driver_list) );
        ms_device_release_driver(ms_gdev);
    }
}

/**
     * @brief               adding generic device to bus
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          error code
     */
int ms_bus_add_device(struct device_s * ms_gdev)
{
    int ms_err = 0;

    if (ms_gdev->bus_type)
    {
        ms_usbhost_debug("bus USB: add device %s\n",ms_gdev->bus_id);
        ms_insert_list_before(&ms_gdev->bus_list,&ms_gdev->bus_type->devices_list);
        ms_device_attach(ms_gdev);
    }
    return ms_err;
}

/**
     * @brief               generic device removing from bus
     *
     * @param           struct device_s * ms_gdev
     *
     * @return          none
     */
void ms_bus_remove_device(struct device_s * ms_gdev)
{
    if (ms_gdev->bus_type)
    {
        ms_usbhost_debug("bus USB: remove device %s\n",ms_gdev->bus_id);
        ms_device_release_driver(ms_gdev);
        ms_list_remove_and_init(&ms_gdev->bus_list);
    }
}

/**
     * @brief               adding generic driver to bus
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          error code
     */
int ms_bus_add_driver(struct device_driver * ms_drv)
{
    struct ms_bus_type * ms_bus = ms_drv->bus;
    int ms_err = 0;

    if (ms_bus)
    {
        ms_usbhost_debug("bus USB: add driver %s\n",ms_drv->name);

        // Add driver to bus driver list
        ms_insert_list_before(&ms_drv->bus_list,&ms_bus->drivers_list);
        ms_driver_attach(ms_drv);
    }
    return ms_err;
}

/**
     * @brief               generic driver removing from bus
     *
     * @param           struct device_driver * ms_drv
     *
     * @return          none
     */
void ms_bus_remove_driver(struct device_driver * ms_drv)
{
    if (ms_drv->bus)
    {
        ms_usbhost_debug("bus USB: remove driver %s\n",ms_drv->name);
        ms_driver_detach(ms_drv);
        ms_list_remove_and_init(&ms_drv->bus_list);
    }
}

