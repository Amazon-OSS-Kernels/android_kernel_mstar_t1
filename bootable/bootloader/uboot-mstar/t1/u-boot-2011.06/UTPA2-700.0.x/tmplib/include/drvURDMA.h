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
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvURDMA.h
/// @brief  UART DMA Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MDRV_URDMA_H__
#define __MDRV_URDMA_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// #define UART_TYPE_DEF
/// Version string.
#define URDMA_DRV_VERSION               /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'U','D','M','A'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','1'},                          /* 00 ~ 99                                          */  \
    {'0','0','0','0','0','0','0','0'},  /* CL#                                              */  \
    MSIF_OS


typedef MS_U32                         DRVURDMA_RESULT;

/// @name DRVURDMA_RESULT
/// @ref DRVURDMA_RESULT
/// return value
/// @{
#define DRVURDMA_PASS                 0x00000000
#define DRVURDMA_TX_BUSY              0x00000001
#define DRVURDMA_RX_BUSY			  0x00000002
#define DRVURDMA_INVALID_PARAM        0x00000003
#define DRVURDMA_MIU_ADDR_ERROR       0x00000004
#define DRVURDMA_FATAL_ERROR          0x00000005
/// }@

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum _URDMA_DbgLv
{
     E_URDMA_DBGLV_NONE          //no debug message
    ,E_URDMA_DBGLV_ERR_ONLY      //show error only
    ,E_URDMA_DBGLV_INFO          //show error & informaiton
    ,E_URDMA_DBGLV_ALL           //show error, information & funciton name
}URDMA_DbgLv;

typedef enum
{
    E_URDMA_UART = 0,
    E_URDMA_DMA,
} URDMA_Mode;

typedef enum
{
    E_URDMA_CIRCULAR = 0,
    E_URDMA_LINE,
} URDMA_OPmode;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
DRVURDMA_RESULT MDrv_UrDMA_GetLibVer(const MSIF_Version **ppVersion);
void MDrv_UrDMA_SetDbgLevel(MS_U8 level);
DRVURDMA_RESULT MDrv_UrDMA_TxInit(MS_PHYADDR *tx_buf, MS_U16 tx_size);
DRVURDMA_RESULT MDrv_UrDMA_RxInit(MS_PHYADDR *rx_buf, MS_U16 rx_size);
DRVURDMA_RESULT MDrv_UrDMA_Polling_Tx_Done(void);
void MDrv_UrDMA_ISR(void);
void MDrv_UrDMA_SelMode(URDMA_Mode eMode);
void MDrv_UrDMA_Engine(MS_BOOL bEnable);
void MDrv_UrDMA_Init(void *dma_rx_cb, void *dma_tx_cb, URDMA_OPmode eOp_mode);
#ifdef MS_DEBUG
void MDrv_UrDMA_DumpREG(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MDRV_URDMA_H__ */
