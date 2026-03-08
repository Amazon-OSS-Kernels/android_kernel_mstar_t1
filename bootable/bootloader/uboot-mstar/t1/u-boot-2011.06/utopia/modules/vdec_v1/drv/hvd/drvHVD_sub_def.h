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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvHVD_sub_def.h
/// @brief  The definiton of SubHVD Driver
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HVD_SUB_DEF_H_
#define _DRV_HVD_SUB_DEF_H_

#include "drvHVD_Common.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// debug switch
// DEBUG
#define HVD_SUB_MSG_MUST(format, args...)  HVD_SUB_UART_PRINTF( E_HVD_UART_CTRL_MUST ,format, ##args)
#define HVD_SUB_MSG_ERR(format, args...)  HVD_SUB_UART_PRINTF( E_HVD_UART_CTRL_ERR ,format, ##args)
#define HVD_SUB_MSG_INFO(format, args...)  HVD_SUB_UART_PRINTF( E_HVD_UART_CTRL_INFO , format, ##args)
#define HVD_SUB_MSG_DEG(format, args...)  HVD_SUB_UART_PRINTF( E_HVD_UART_CTRL_DBG , format, ##args)
#define HVD_SUB_MSG_TRACE()  HVD_SUB_UART_PRINTF( E_HVD_UART_CTRL_TRACE , "HVD TRACE:%s ; %s ; %d \n" , __FILE__ , __FUNCTION__, __LINE__)
//#define HVD_SUB_FW_DEG(format, args...)     HVD_SUB_UART_PRINTF( E_HVD_UART_CTRL_FW , format, ##args)

#if defined(REDLION_LINUX_KERNEL_ENVI)
#define HVD_SUB_UART_PRINTF( u32type, format , args...) \
    do{ \
        if( u32SubUartCtrl & u32type ) \
        { \
            printk( format , ##args  ); \
        } \
    }while(0)
#else
#define HVD_SUB_UART_PRINTF( u32type, format , args...) \
    do{ \
        if( u32SubUartCtrl & u32type ) \
        { \
            printf( format , ##args  ); \
        } \
    }while(0)
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//extern HVD_Drv_Ctrl    gSubHVDCtrl;
extern MS_U32 u32SubUartCtrl;
extern MS_U32 u32SubInitSysTimeBase;
//extern MS_U8 pu8HVD_DMAcpy_pool[];

void _HVD_Sub_Chip_Flush_Memory(void);
void _HVD_Sub_Chip_Read_Memory(void);

#endif // _DRV_HVD_SUB_DEF_H_

