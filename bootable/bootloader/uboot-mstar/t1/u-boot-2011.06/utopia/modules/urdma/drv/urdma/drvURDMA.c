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
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvUrDMA.c
/// @brief  UART DMA Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsTypes.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvMMIO.h"
#include "halURDMA.h"
#include "regURDMA.h"
#include "drvURDMA.h"

#include "ULog.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TAG_UARTDMA "UARTDMA"

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
/* register base address */
static dma_interface_t *p_uart_dma = NULL;
static MS_VIRT virt_urdma_base = 0;//HS_UART_DMA_REG_BASE

/* Callback function for interrupt serivce routine. */
static void (*fast_uart_dma_rx_cb)(void) = NULL;
static void (*fast_uart_dma_tx_cb)(void) = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MSIF_Version _urdmaVersion = {
    .DDI = { URDMA_DRV_VERSION },
};

static URDMA_DbgLv _urdmaDbgLevel = E_URDMA_DBGLV_ERR_ONLY;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define DEBUG_UART_DMA(debug_level, x)     do { if (_urdmaDbgLevel >= (debug_level)) (x); } while(0)

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#define URDMA_LOU16(u32Val)   ((MS_U16)(u32Val)&0xffff)
#define URDMA_HIU16(u32Val)   ((MS_U16)((u32Val) >> 16)&0x07ff)

static void _MDrv_UrDMA_RegisterCallback(void *rx_cb, void *tx_cb)//register_hs_uart_dma_cb(void *rx_cb, void *tx_cb)
{
    /* disable interrupt */
    MsOS_DisableInterrupt(FAST_UART_DMA_INT);

    fast_uart_dma_rx_cb = rx_cb;
    fast_uart_dma_tx_cb = tx_cb;

    /* enable interrupt */
    MsOS_EnableInterrupt(FAST_UART_DMA_INT);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

DRVURDMA_RESULT MDrv_UrDMA_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return DRVURDMA_INVALID_PARAM;
    }

    *ppVersion = &_urdmaVersion;

    return DRVURDMA_PASS;
}

void MDrv_UrDMA_SetDbgLevel(MS_U8 level)
{
    _urdmaDbgLevel = (URDMA_DbgLv)level;
}

DRVURDMA_RESULT MDrv_UrDMA_TxInit(MS_PHYADDR *tx_buf, MS_U16 tx_size)//U32 hs_uart_dma_prepare_tx(void *tx_buf, U16 tx_size)
{
	DRVURDMA_RESULT ret = DRVURDMA_PASS;
    // hank
    // extern int dma_dbug;

    // if (dma_dbug == 1)
    // {
       // }

    /* disable interrupt */
	MsOS_DisableInterrupt(FAST_UART_DMA_INT);

    if (p_uart_dma == NULL)
    {
        ret = DRVURDMA_FATAL_ERROR;
        goto dma_prepare_tx_end;
    }

    p_uart_dma->tx_urdma_en = 0; /* disable dma */
    while (p_uart_dma->tx_busy)
    {
        int i = 0;
        ret = DRVURDMA_TX_BUSY;
		DEBUG_UART_DMA(E_URDMA_DBGLV_ERR_ONLY, ULOGE(TAG_UARTDMA,"tx_busy!\n"));
        if (1000 == (i++))
        {
        	//reg dump, here
            goto dma_prepare_tx_end;
        }
    }

    p_uart_dma->tx_buf_base_h = URDMA_HIU16((MS_PHYADDR)tx_buf);//(U16)((((U32)PHYADDR((U32)tx_buf))>>16)&0x7ff);
    p_uart_dma->tx_buf_base_l = URDMA_LOU16((MS_PHYADDR)tx_buf);//(U16)(((U32)PHYADDR((U32)tx_buf))&0xffff);
    // p_uart_dma->tx_buf_size = (U16)(tx_size>>3);
    p_uart_dma->tx_buf_size = 0x1fff;
    p_uart_dma->tx_buf_wptr = (MS_VIRT)((tx_size-1) + (((MS_VIRT)tx_buf)%8)); /* alignment issue */
    p_uart_dma->tx_timeout = 1; /* send-out asap. */
    p_uart_dma->tx_intr_en = 1; /* Enable tx interrupt. */

	#ifdef MS_DEBUG
	 MDrv_UrDMA_DumpREG();
	#endif

    p_uart_dma->tx_urdma_en = 1;/* Enable tx dma and start transmission. */

dma_prepare_tx_end:
    /* enable interrupt */
	MsOS_EnableInterrupt(FAST_UART_DMA_INT);

    return ret;
}

DRVURDMA_RESULT MDrv_UrDMA_RxInit(MS_PHYADDR *rx_buf, MS_U16 rx_size)//hs_uart_dma_prepare_rx(void *rx_buf, U16 rx_size)
{
    DRVURDMA_RESULT ret = DRVURDMA_PASS;

    /* disable interrupt */
    MsOS_DisableInterrupt(FAST_UART_DMA_INT);

    if (p_uart_dma == NULL)
    {
        ret = DRVURDMA_FATAL_ERROR;
        goto dma_prepare_rx_end;
    }

    p_uart_dma->rx_urdma_en = 0; /* disable dma */
    while (p_uart_dma->rx_busy)
    {
        int i=0;
        ret = DRVURDMA_RX_BUSY;
        DEBUG_UART_DMA(E_URDMA_DBGLV_ERR_ONLY, ULOGE(TAG_UARTDMA,"rx_busy!\n"));
        if (1000 == (i++))
        {
            goto dma_prepare_rx_end;
        }
    }

    p_uart_dma->rx_buf_base_h = URDMA_HIU16((MS_PHYADDR)rx_buf);//(U16)((((U32)PHYADDR((U32)rx_buf))>>16)&0x7ff);
    p_uart_dma->rx_buf_base_l = URDMA_LOU16((MS_PHYADDR)rx_buf);//(U16)(((U32)PHYADDR((U32)rx_buf))&0xffff);
    // p_uart_dma->rx_buf_size = (U16)(rx_size>>3);
    p_uart_dma->rx_buf_size = 0x1fff;
    p_uart_dma->intr_threshold = (MS_U16)rx_size;
    p_uart_dma->rx_timeout = 10; /* receive timeout. */
    p_uart_dma->rx_intr2_en = 1; /* Enable rx threshold interrupt. */

	#ifdef MS_DEBUG
	 MDrv_UrDMA_DumpREG(); //@@@ pending function
	#endif

    p_uart_dma->rx_urdma_en = 1; /* Enable rx dma and start receiving. */

dma_prepare_rx_end:
    /* enable interrupt */
    MsOS_EnableInterrupt(FAST_UART_DMA_INT);

    return ret;
}

DRVURDMA_RESULT MDrv_UrDMA_Polling_Tx_Done(void)//hs_uart_dma_poll_tx_complete(void)
{
    DRVURDMA_RESULT ret = DRVURDMA_PASS;

    //ret = 1;

    // if (!(p_uart_dma->tx_mcu_intr))
    // {
    //     ret = 0;
    // }

    /* wait until URDMA into idle state. */
    while (p_uart_dma->tx_busy)
        ;

    p_uart_dma->tx_intr_clr = 1; /* clear int status */
    p_uart_dma->tx_intr_en = 0;  /* disable tx int */

    return ret;
}

void MDrv_UrDMA_ISR(void)//void hs_uart_dma_isr(void)
{
    if (p_uart_dma != NULL)
    {
        if (p_uart_dma->rx_mcu_intr)
        {
            while (p_uart_dma->rx_busy)
				;
            /* RX interrupt */
            if (p_uart_dma->rx_intr2) /* Caused by intr threshold */
            {
                p_uart_dma->rx_intr_clr = 1; /* clear int status */
                p_uart_dma->rx_intr2_en = 0; /* disable rx threshold int */
            }
            if (p_uart_dma->rx_intr1) /* Caused by receive timeout */
            {
                p_uart_dma->rx_intr_clr = 1; /* clear int status */
                p_uart_dma->rx_intr1_en = 0; /* disable rx timeout int */
            }
            if (fast_uart_dma_rx_cb != NULL)
            {
                fast_uart_dma_rx_cb();
            }
        }
        if (p_uart_dma->tx_mcu_intr)
        {
            while (p_uart_dma->tx_busy)
				;
            /* TX interrupt */
            p_uart_dma->tx_intr_clr = 1; /* clear int status */
            p_uart_dma->tx_intr_en = 0; /* disable tx int */

            if (fast_uart_dma_tx_cb != NULL)
            {
                fast_uart_dma_tx_cb();
            }
        }
    }

    return;
}

void MDrv_UrDMA_SelMode(URDMA_Mode eMode)// void hs_uart_dma_mode_switch(U16 mode)
{
    if (E_URDMA_DMA == eMode) /* UART DMA mode */
    {
        while (p_uart_dma->rx_busy) /* Wait for idle state. */
            ;
        /* switch to UART DMA mode. */
        p_uart_dma->urdma_mode = 1;
    }
    else /* Normal uart mode */
    {
        p_uart_dma->tx_urdma_en = 0;/* Disable tx dma and start transmission. */
        p_uart_dma->rx_urdma_en = 0;/* Disable rx dma and start receiving. */
        while (p_uart_dma->rx_busy) /* Wait for idle state. */
            ;

        /* switch to UART mode. */
        p_uart_dma->urdma_mode = 0;
    }

    return;
}

void MDrv_UrDMA_Engine(MS_BOOL bEnable)//hs_uart_dma_enable(u_int8 enable)
{
    p_uart_dma->urdma_mode = bEnable;
}

void MDrv_UrDMA_Init(void *dma_rx_cb, void *dma_tx_cb, URDMA_OPmode eOp_mode)
{
    int i;
	MS_VIRT virtBaseAddr;
	MS_PHY  u32BaseSize;

	if(!MDrv_MMIO_GetBASE(&virtBaseAddr,&u32BaseSize,MS_MODULE_UART))
	{
		DEBUG_UART_DMA(E_URDMA_DBGLV_ERR_ONLY, ULOGE(TAG_UARTDMA,"%s():IOMap failure to get DRV_MMIO_NONPM_BANK\n",__FUNCTION__));
		return;
	}

	virt_urdma_base = virtBaseAddr + FAST_UART_DMA_REG;

    p_uart_dma = (dma_interface_t *)virt_urdma_base;
    // p_uart_dma = (dma_interface_t *)0x20000;

    /* disable tx and rx */
    p_uart_dma->tx_urdma_en = 0;
    p_uart_dma->rx_urdma_en = 0;

    /* software reset */
    p_uart_dma->sw_rst = 1;

	//
	// @@@ Dealy for HW, please use real timer!!!
	//
    for (i=0;i<0x100;i++)
        ;

    p_uart_dma->sw_rst = 0;

    /* disable tx and rx */
    p_uart_dma->tx_urdma_en = 0;
    p_uart_dma->rx_urdma_en = 0;

    /* make sure rx_busy is off */
    while (p_uart_dma->rx_busy)
        ; //@@@ add time out, here

    p_uart_dma->reg00 = 0;

    /* disable interrupt */
    p_uart_dma->tx_intr_en = 0;
    p_uart_dma->rx_intr1_en = 0;
    p_uart_dma->rx_intr2_en = 0;

    /* clear interrupt status */
    p_uart_dma->tx_intr_clr = 1;
    p_uart_dma->rx_intr_clr = 1;

    _MDrv_UrDMA_RegisterCallback((void *) dma_rx_cb, (void *)dma_tx_cb);//register_hs_uart_dma_cb((void *) dma_rx_cb, (void *)dma_tx_cb);

    /* ToDo: install_isr */

    /* Always set RX operations mode 1, Line Buffer mode */
    p_uart_dma->rx_op_mode = eOp_mode;

    /* switch to UART mode. */
    MDrv_UrDMA_SelMode(E_URDMA_DMA);//hs_uart_dma_enable(1);

}

#ifdef MS_DEBUG
void MDrv_UrDMA_DumpREG(void)
{
    if (p_uart_dma != NULL)
    {
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg00, (int)p_uart_dma->reg00));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg01, (int)p_uart_dma->reg01));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg02, (int)p_uart_dma->reg02));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg03, (int)p_uart_dma->reg03));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg04, (int)p_uart_dma->reg04));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg05, (int)p_uart_dma->reg05));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg06, (int)p_uart_dma->reg06));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg07, (int)p_uart_dma->reg07));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg08, (int)p_uart_dma->reg08));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg09, (int)p_uart_dma->reg09));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg0a, (int)p_uart_dma->reg0a));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg0b, (int)p_uart_dma->reg0b));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg0c, (int)p_uart_dma->reg0c));
        DEBUG_UART_DMA(E_URDMA_DBGLV_ALL, ULOGE(TAG_UARTDMA,"0x%x:0x%x\n", (int)&p_uart_dma->reg0d, (int)p_uart_dma->reg0d));
    }
}
#endif

