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

#include <command.h>
#include <common.h>
#include <MsDebug.h>
#include <uart/MsDrvUART.h>

int do_uart_rx_enable(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    MS_BOOL bEnable = TRUE;

    UBOOT_TRACE("IN\n");

    const char *uart_str = getenv("uart_rx");

    if (uart_str != NULL)
    {
        if(strncmp(uart_str, "0", 1) == 0)
        {
            bEnable = FALSE;
        }
        else if (strncmp(uart_str, "1", 1) == 0)
        {
            bEnable = TRUE;
        }
        else
        {
            cmd_usage(cmdtp);
            return ret;
        }
    }
    else
    {
        return ret;
    }

    ret =MsDrv_UART_Rx_Enable(bEnable);

    UBOOT_TRACE("OK\n");
    return ret;
}