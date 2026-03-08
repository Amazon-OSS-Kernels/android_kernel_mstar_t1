/**
* Copyright (c) 2006 â€“ 2018 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "../../../drv/usbhost/include/drvGlobal.h"
#include "../../../drv/usbhost/include/_drvUSB.h"

U16 HAL_USB_GetChipID(void)
{
    return CHIPID_MUJI;
}

void HAL_USB_BC_Enable(U8 uPort, BOOLEAN bEnable)
{
    static U8 PortBCStaus[4] = {1, 1, 1, 1}; //Set port BC enabled at first time
    U32 utmi_base, bc_base;

    if (uPort == 0)
    {
        utmi_base = 0x103A80;
        bc_base = 0x123600;
    }
    else if (uPort == 1)
    {
        utmi_base = 0x103A00;
        bc_base = 0x123620;
    }
    else if (uPort == 2)
    {
        utmi_base = 0x103900;
        bc_base = 0x123640;
    }
    else if (uPort == 3)
    {
        utmi_base = 0x122080;
        bc_base = 0x123660;
    }
    else
    {
        printf("HAL_USB_BC_Enable: invalid port number %d\n", uPort);
        return;
    }

    if (bEnable)
    {
        if ( !PortBCStaus[uPort] )
        {
            printf("USB BC mode enabled\n");
            MDrv_WriteRegBit(utmi_base+0x01, ENABLE, 0x40); //IREF_PDN=1¡¦b1. (utmi+0x01[6] )
            MDrv_WriteRegBit(bc_base+0x03, ENABLE, 0x40); // [6]= reg_host_bc_en
            MDrv_WriteRegBit(bc_base+0x0C, ENABLE, 0x40); // [6]= reg_into_host_bc_sw_tri
            MDrv_Write2Byte(bc_base, 0x0); // [15:0] = bc_ctl_ov_en
            MDrv_WriteRegBit(bc_base+0x0A, ENABLE, 0x80); // [7]=reg_bc_switch_en
            PortBCStaus[uPort] = 1;
        }
    }
    else
    {
        if ( PortBCStaus[uPort] )
        {
            printf("USB BC mode disabled\n");
            MDrv_WriteRegBit(bc_base+0x0C, DISABLE, 0x40); // [6]= reg_into_host_bc_sw_tri
            MDrv_WriteRegBit(bc_base+0x03, DISABLE, 0x40); // [6]= reg_host_bc_en
            MDrv_WriteRegBit(utmi_base+0x01, DISABLE, 0x40); //IREF_PDN=1¡¦b1. (utmi+0x01[6] )
            PortBCStaus[uPort] = 0;
        }
    }
}

