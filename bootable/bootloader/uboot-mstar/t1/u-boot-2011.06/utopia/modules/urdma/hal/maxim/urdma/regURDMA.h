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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regURDMA.h
//  Description: FAST UART DMA Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FAST_UART_DMA_H__
#define __FAST_UART_DMA_H__

//#if FAST_UART_DMA_ENABLE

//#include "fast_uart_dma_param.h" //halURDMA.h
#include "halURDMA.h"


#define Struct_Register __attribute__((packed))


/* Warning: Little Endian */
typedef struct dma_interface
{
    union
    {
        volatile MS_U16 reg00; /* 00h */
        struct
        {
            volatile MS_U16 sw_rst          :   1;  /* BIT0 */
            volatile MS_U16 urdma_mode      :   1;  /* BIT1 */
            volatile MS_U16 tx_urdma_en     :   1;  /* BIT2 */
            volatile MS_U16 rx_urdma_en     :   1;  /* BIT3 */
            volatile MS_U16 tx_endian       :   1;  /* BIT4 */
            volatile MS_U16 rx_endian       :   1;  /* BIT5 */
            volatile MS_U16 tx_sw_rst       :   1;  /* BIT6 */
            volatile MS_U16 rx_sw_rst       :   1;  /* BIT7 */
            volatile MS_U16 reserve00       :   3;  /* BIT8 ~ BIT10 */
            volatile MS_U16 rx_op_mode      :   1;  /* BIT11 */
            volatile MS_U16 tx_busy         :   1;  /* BIT12 */
            volatile MS_U16 rx_busy         :   1;  /* BIT13 */
            volatile MS_U16 reserve01       :   2;  /* BIT14 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space00;
#endif
    union
    {
        volatile MS_U16 reg01; /* 02h */
        struct
        {
            volatile MS_U16 intr_threshold  :   12; /* BIT0 ~ BIT11 */
            volatile MS_U16 reserve02       :   4;  /* BIT12 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space01;
#endif
    union
    {
        volatile MS_U16 reg02; /* 04h */
        struct
        {
            volatile MS_U16 tx_buf_base_h   :   8;  /* BIT0 ~ BIT7 */
            volatile MS_U16 reserve03       :   8;  /* BIT8 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space02;
#endif
    union
    {
        volatile MS_U16 reg03; /* 06h */
        struct
        {
            volatile MS_U16 tx_buf_base_l   :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space03;
#endif
    union
    {
        volatile MS_U16 reg04; /* 08h */
        struct
        {
            volatile MS_U16 tx_buf_size     :   13;  /* BIT0 ~ BIT12 */
            volatile MS_U16 reserve04       :   3;   /* BIT13 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space04;
#endif
    union
    {
        volatile MS_U16 reg05; /* 0Ah */
        struct
        {
            volatile MS_U16 tx_buf_rptr     :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space05;
#endif
    union
    {
        volatile MS_U16 reg06; /* 0Ch */
        struct
        {
            volatile MS_VIRT tx_buf_wptr     :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space06;
#endif
    union
    {
        volatile MS_U16 reg07; /* 0Eh */
        struct
        {
            volatile MS_U16 tx_timeout      :   4;   /* BIT0 ~ BIT3 */
            volatile MS_U16 reserve05       :   12;  /* BIT4 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space07;
#endif
    union
    {
        volatile MS_U16 reg08; /* 10h */
        struct
        {
            volatile MS_U16 rx_buf_base_h   :   8;  /* BIT0 ~ BIT7 */
            volatile MS_U16 reserve06       :   8;  /* BIT8 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space08;
#endif
    union
    {
        volatile MS_U16 reg09; /* 12h */
        struct
        {
            volatile MS_U16 rx_buf_base_l   :   16; /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space09;
#endif
    union
    {
        volatile MS_U16 reg0a; /* 14h */
        struct
        {
            volatile MS_U16 rx_buf_size     :   13; /* BIT0 ~ BIT12 */
            volatile MS_U16 reserve07       :   3;  /* BIT13 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space0a;
#endif
    union
    {
        volatile MS_U16 reg0b; /* 16h */
        struct
        {
            volatile MS_U16 rx_buf_wptr     :   16; /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space0b;
#endif
    union
    {
        volatile MS_U16 reg0c; /* 18h */
        struct
        {
            volatile MS_U16 rx_timeout      :   4;  /* BIT0 ~ BIT3 */
            volatile MS_U16 reserve08       :   12; /* BIT4 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#if TWO_BYTE_SPACE
    MS_U16 space0c;
#endif
    union
    {
        volatile MS_U16 reg0d;  /* 1Ah */
        struct
        {
            volatile MS_U16 rx_intr_clr     :   1; /* BIT0 */
            volatile MS_U16 rx_intr1_en     :   1; /* BIT1 */
            volatile MS_U16 rx_intr2_en     :   1; /* BIT2 */
            volatile MS_U16 reserve09       :   1; /* BIT3 */
            volatile MS_U16 rx_intr1        :   1; /* BIT4 */
            volatile MS_U16 rx_intr2        :   1; /* BIT5 */
            volatile MS_U16 reserve0a       :   1; /* BIT6 */
            volatile MS_U16 rx_mcu_intr     :   1; /* BIT7 */
            volatile MS_U16 tx_intr_clr     :   1; /* BIT8 */
            volatile MS_U16 tx_intr_en      :   1; /* BIT9 */
            volatile MS_U16 reserve0b       :   5; /* BIT10 ~ BIT14 */
            volatile MS_U16 tx_mcu_intr     :   1; /* BIT15 */
        } Struct_Register;
    } Struct_Register;
} Struct_Register dma_interface_t;

//#endif /* #if FAST_UART_DMA_ENABLE */

#endif /* __FAST_UART_DMA_H__ */
