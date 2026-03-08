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
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVUART_PRIV_H_
#define _DRVUART_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvUART_private.h
/// @brief UART control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "MsTypes.h"

enum {
    MDrv_CMD_UART_Open,
    MDrv_CMD_UART_Close,
    MDrv_CMD_UART_Connect,
    MDrv_CMD_UART_SetBaudRate,
} eMbxIoctlOpt;

void UARTRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 UARTOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 UARTClose(void* pInstance);
MS_U32 UARTIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);

typedef MS_U32 (*IOCTL_UART_OPEN)(UART_DEVICE_TYPE);
typedef MS_U16 (*IOCTL_UART_CLOSE)(MS_VIRT);
typedef MS_U16 (*IOCTL_UART_CONNECT)(UART_PORT_TYPE, UART_DEVICE_TYPE);
typedef MS_U16 (*IOCTL_UART_SET_BAUDRATE)(MS_VIRT, MS_U32);

typedef struct _UART_OPEN
{
    UART_DEVICE_TYPE eUartType;
}UART_OPEN, *PUART_OPEN;

typedef struct _UART_CLOSE
{
    MS_U32 u32Fd;
}UART_CLOSE, *PUART_CLOSE;

typedef struct _UART_CONNECT
{
    UART_PORT_TYPE ePortType;
    UART_DEVICE_TYPE eDeviceType;
}UART_CONNECT, *PUART_CONNECT;

typedef struct _UART_SET_BAUDRATE
{
    MS_U32 u32Uart;
    MS_U32 u32BaudRate;
}UART_SET_BUADRATE, *PUART_SET_BUAD_RATE;

typedef struct _UART_INSTANT_PRIVATE
{
    IOCTL_UART_OPEN                       fpUARTOpen;
    IOCTL_UART_CLOSE                      fpUARTClose;
    IOCTL_UART_CONNECT                    fpUARTConnect;
    IOCTL_UART_SET_BAUDRATE               fpUARTSetBaudRate;
}UART_INSTANT_PRIVATE;

typedef struct _UART_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}UART_RESOURCE_PRIVATE;

//instant
typedef struct _UART_INSTANT_ATTRIBUTE
{
    UART_DEVICE_TYPE  eUartType;
    MS_U32 u32BaudRate;
}UART_INSTANT_ATTRIBUTE;

#ifdef __cplusplus
}
#endif
#endif //_DRVUART_PRIV_H_

