/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////
///    Header File
////////////////////////////////////////////////////
#include <command.h>
#include <MsUartComm.h>
#ifdef CONFIG_UARTCOMM

U_BOOT_CMD(
    uartcomm, CONFIG_SYS_MAXARGS, 1,    do_uartcomm,
    "Uart communication : support init¡¢read and write",
	"init  <port> <pad> <baudrate>  - init uart port default:uart1,register uart interrupt callback for read\n"
	"uartcomm write <cmd> ...                - uart write cmd,cmd_size <= 30\n"
	"uartcomm read  <szie>                   - read uart data and print\n"
);
#endif
