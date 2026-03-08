/**
* Copyright (c) 2006  2016 MStar Semiconductor, Inc.
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

#ifndef __DRVFCIE_ERR_H__
#define __DRVFCIE_ERR_H__

typedef enum
{
    EC_NO_ERROR = 0,
    EC_CRC_ERROR,
    EC_CMD_TIME_OUT,
    EC_DMA_TIME_OUT,
    EC_CMD_NO_RSP,

} ERR_CODE_SD;

typedef enum
{
    IO_SUCCESS              = 0x00,
    IO_TIME_OUT             = 0x01,
    IO_CARD_REMOVED         = 0x02,
    IO_ERROR                = 0x03,
    IO_ERROR_DETECT         = 0x04,

    IO_VOLTAGE_NOT_ACCEPT   = 0x05,
    IO_MISS_CHECK_PATTERN   = 0x06,

    IO_CMD_NO_RSP           = 0x16,
    IO_RSP_CRC_ERROR        = 0x17,
    IO_RSP_ERROR            = 0x18,

    IO_R_DATA_CRC_ERROR     = 0x20,
    IO_W_DATA_STS_ERROR     = 0x21,
    IO_W_DATA_STS_NEGATIVE  = 0x22,

    IO_STILL_POWERING_UP    = 0x30,

    IO_CARD_CHANGED         = 0x40,

} E_IO_STS;

#endif
