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
typedef struct
{
        U32     MsgID;
        U32     Para1;
        U32     Para2;
} ShellMessage;

#define SHELL_MSG_COUNT 5
#define SHELL_MSG_SIZE          (sizeof(ShellMessage))
#define SHELL_MSG_QUEUE_SIZE        (SHELL_MSG_SIZE*SHELL_MSG_COUNT)
#define USB_MSG         1
#define USB_DEVICE_CONNECT  1
#define USB_DEVICE_DISCONNECT   2
