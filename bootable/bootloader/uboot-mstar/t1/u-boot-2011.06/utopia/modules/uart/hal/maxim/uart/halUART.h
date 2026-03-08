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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_TEMP_H_
#define _HAL_TEMP_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define UART_DEVICE(_dev_)  ((struct uart_dev_struct *)(_dev_))

#define UNUSED( var )       (void)((var) = (var))

#define UART_DEV_MAX        20

#define TX_FLAG_DISABLED    0x0001
#define TX_FLAG_DIRECT      0x0002

#define RX_FLAG_DISABLED    0x0001
#define RX_FLAG_OVERFLOW    0x0002

#define IOCTL_SET_BAUDRATE          0
#define IOCTL_SET_RXCALLBACK        1
#define IOCTL_SET_RXCALLBACK_HALREG 2

#define UART_BASE       (u->regbase)

#define AEON_REG8(_x_)  ((MS_U8 volatile *)(UART_BASE))[(_x_)]
#define UART_REG8(_x_)  ((MS_U8 volatile *)(_gMIO_MapBase+UART_BASE))[((_x_) * 4) - ((_x_) & 1)]



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
struct uart_dev_struct;

typedef struct uart_dev_struct * ms_uart_dev_t;

struct uart_dev_struct
{
    struct device          device;
    void                   (*rx_callback)(int c);
    void                   (*rx_callback_halreg)(int c, MS_U32 pHalReg);
    MS_U32                 regbase;
    MS_U8                  *tx_buf;
    MS_U16                 tx_buf_in;
    volatile MS_U16        tx_buf_out;     // updated by ISR
    MS_U16                 tx_buf_len;
    MS_U16                 tx_flags;
    MS_U8                  *rx_buf;
    volatile MS_U16        rx_buf_in;      // updated by ISR
    MS_U16                 rx_buf_out;
    MS_U16                 rx_buf_len;
    MS_U16                 rx_flags;
    MS_U8                  uarttype ;
};

extern struct uart_dev_struct _uart_dev[UART_DEV_MAX] ;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_UART_SetIOMapBase(MS_VIRT u32Base, MS_VIRT u32PMBase);
MS_VIRT HAL_UART_GetIOMapBase(void);
MS_BOOL HAL_UART_SetPMRxEnable(MS_BOOL bEnable);
UART_DEVICE_TYPE HAL_UART_Get_Connection(UART_PORT_TYPE uart_port);
MS_U16 HAL_UART_Connect(UART_PORT_TYPE uart_port, UART_DEVICE_TYPE uart_dev);
MS_BOOL HAL_UART_SetPadMux(UART_PAD_TYPE uart_pad);
MS_U16 HAL_UART_Invert(UART_PORT_TYPE uart_port, MS_BOOL bInv);
int HAL_UART_Aeon_Close(device_t dev);
int HAL_UART_Aeon_Write(device_t dev, const char *buf, MS_U32 len, int off);
int HAL_UART_Aeon_Read(device_t dev, char *buf, MS_U32 len, int off);
int HAL_UART_Aeon_Poll(device_t dev, int types);
void HAL_UART_Aeon_Isr(MHAL_SavedRegisters *pHalReg, MS_U32 u32Data);
int HAL_UART_Aeon_Ioctl(device_t dev, int request, unsigned long arg);
int HAL_UART_Aeon_Open(device_t dev, int mode);


int HAL_UART_PIU_Close(device_t dev);
int HAL_UART_PIU_Write(device_t dev, const char *buf, MS_U32 len, int off);
int HAL_UART_PIU_Read(device_t dev, char *buf, MS_U32 len, int off);
int HAL_UART_PIU_Poll(device_t dev, int types);
void HAL_FUART_PIU_Isr(MS_U32 *pHalReg, MS_U32 u32Data);
void HAL_UART0_PIU_Isr(MS_U32 *pHalReg, MS_U32 u32Data);
void HAL_UART1_PIU_Isr(MS_U32 *pHalReg, MS_U32 u32Data);
int HAL_UART_PIU_Ioctl(device_t dev, int request, unsigned long arg);
int HAL_UART_PIU_Open(device_t dev, int mode);





#endif // _HAL_TEMP_H_

