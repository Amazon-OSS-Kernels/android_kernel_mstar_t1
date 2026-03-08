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


#include <MsCommon.h>
#include "include/drvKernel.h"
#include "include/drvCPE_AMBA.h"
#include "drvHUB.h"

#if 1
extern struct cpe_driver ehci_hcd_cpe_driver;
extern struct cpe_dev cpe_ehci_dev;

int AMBA_match(struct device_s * dev, struct device_driver * drv)
{
	if ( (dev == &cpe_ehci_dev.dev) && (drv == &ehci_hcd_cpe_driver.drv) )
		return 1; // matched CPE EHCI

	return 0;
}

struct bus_type CPE_AMBA_bus_type = {
    "CPE_AMBA",
    {&CPE_AMBA_bus_type.drivers_list,&CPE_AMBA_bus_type.drivers_list},
    {&CPE_AMBA_bus_type.devices_list,&CPE_AMBA_bus_type.devices_list},
    AMBA_match,
    NULL,
};

extern struct cpe_driver ehci_hcd_cpe_driver_Port2;
extern struct cpe_dev cpe_ehci_dev_Port2;

int AMBA_match_Port2(struct device_s * dev, struct device_driver * drv)
{
	if ( (dev == &cpe_ehci_dev_Port2.dev) && (drv == &ehci_hcd_cpe_driver_Port2.drv) )
		return 1; // matched CPE EHCI

	return 0;
}

struct bus_type CPE_AMBA_bus_type_Port2 = {
    "CPE_AMBA_2",
    {&CPE_AMBA_bus_type_Port2.drivers_list,&CPE_AMBA_bus_type_Port2.drivers_list},
    {&CPE_AMBA_bus_type_Port2.devices_list,&CPE_AMBA_bus_type_Port2.devices_list},
    AMBA_match_Port2,
    NULL,
};

#endif

int AMBA_match_EX(struct device_s * dev, struct device_driver * drv)
{        
        struct cpe_dev *_dev;        
        struct s_gVar4UsbPort *p_rootHub;
        const struct device_s *__mptr = dev;
        _dev = (struct cpe_dev *)( (char *)__mptr - (char *)offsetof(struct cpe_dev,dev) );
        p_rootHub = cpe_to_gVar(_dev);

        if ((p_rootHub->portNum < NUM_OF_ROOT_HUB) && (drv == &ehci_hcd_cpe_driver.drv) )
            return 1;
        else
	    return 0;
}

struct bus_type CPE_AMBA_bus_type_EX = {
    "CPE_AMBA_EX",
    {&CPE_AMBA_bus_type_EX.drivers_list,&CPE_AMBA_bus_type_EX.drivers_list},
    {&CPE_AMBA_bus_type_EX.devices_list,&CPE_AMBA_bus_type_EX.devices_list},
    AMBA_match_EX,
    NULL,
};

