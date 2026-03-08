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

#ifndef _REGBDMA_H_
#define _REGBDMA_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
//v: value n: shift n bits
#define __BIT(x)    ((MS_U8)(1UL << (x)))
#define __BIT0       __BIT(0UL)
#define __BIT1       __BIT(1UL)
#define __BIT2       __BIT(2UL)
#define __BIT3       __BIT(3UL)
#define __BIT4       __BIT(4UL)
#define __BIT5       __BIT(5UL)
#define __BIT6       __BIT(6UL)
#define __BIT7       __BIT(7UL)

#define BDMA_REG_BASE               (0x0900UL)
#define BDMA_REG_CH0_BASE           (BDMA_REG_BASE+0UL)
#define BDMA_CH_REG_OFFSET          (0x20UL)
#define BDMA_SET_CH0_REG(x)         (BDMA_REG_CH0_BASE+(x))
#define BDMA_SET_CH1_REG(x)         (BDMA_SET_CH0_REG(x)+BDMA_CH_REG_OFFSET)

#define BDMA_REG_CH0_CTRL           BDMA_SET_CH0_REG(0UL)
#define BDMA_REG_CH0_STATUS         BDMA_SET_CH0_REG(0x02UL)
#define BDMA_REG_CH0_SRC_SEL        BDMA_SET_CH0_REG(0x04UL)
#define BDMA_REG_CH0_DST_SEL        BDMA_SET_CH0_REG(0x05UL)
#define BDMA_REG_CH0_MISC           BDMA_SET_CH0_REG(0x06UL)
#define BDMA_REG_CH0_DWUM_CNT       BDMA_SET_CH0_REG(0x07UL)
#define BDMA_REG_CH0_SRC_ADDR_L     BDMA_SET_CH0_REG(0x08UL)
#define BDMA_REG_CH0_SRC_ADDR_H     BDMA_SET_CH0_REG(0x0AUL)
#define BDMA_REG_CH0_DST_ADDR_L     BDMA_SET_CH0_REG(0x0CUL)
#define BDMA_REG_CH0_DST_ADDR_H     BDMA_SET_CH0_REG(0x0EUL)
#define BDMA_REG_CH0_SIZE_L         BDMA_SET_CH0_REG(0x10UL)
#define BDMA_REG_CH0_SIZE_H         BDMA_SET_CH0_REG(0x12UL)
//Ch0 Special command 0
#define BDMA_REG_CH0_CMD0_L         BDMA_SET_CH0_REG(0x14UL)
#define BDMA_REG_CH0_CMD0_H         BDMA_SET_CH0_REG(0x16UL)
//Ch0 Special command 1
#define BDMA_REG_CH0_CMD1_L         BDMA_SET_CH0_REG(0x18UL)
#define BDMA_REG_CH0_CMD1_H         BDMA_SET_CH0_REG(0x1AUL)
//Ch0 Special command 2
#define BDMA_REG_CH0_CMD2_L         BDMA_SET_CH0_REG(0x1CUL)
#define BDMA_REG_CH0_CMD2_H         BDMA_SET_CH0_REG(0x1EUL)

//---------------------------------------------
// definition for BDMA_REG_CH0_CTRL/BDMA_REG_CH1_CTRL
//---------------------------------------------
#define BDMA_CH_TRIGGER             __BIT0
#define BDMA_CH_STOP                __BIT4

//---------------------------------------------
// definition for REG_BDMA_CH0_STATUS/REG_BDMA_CH1_STATUS
//---------------------------------------------
#define BDMA_CH_QUEUED              __BIT0
#define BDMA_CH_BUSY                __BIT1
#define BDMA_CH_INT                 __BIT2
#define BDMA_CH_DONE                __BIT3
#define BDMA_CH_RESULT              __BIT4
#define BDMA_CH_CLEAR_STATUS        (BDMA_CH_INT|BDMA_CH_DONE|BDMA_CH_RESULT)
//---------------------------------------------
// definition for REG_BDMA_CH0_MISC/REG_BDMA_CH1_MISC
//---------------------------------------------
#define BDMA_CH_ADDR_DECDIR         __BIT0
#define BDMA_CH_DONE_INT_EN         __BIT1
#define BDMA_CH_CRC_REFLECTION      __BIT4
#define BDMA_CH_MOBF_EN             __BIT5
      
#define BDMA_REG_SRC_TLB            __BIT8
#define BDMA_REG_DST_TLB            __BIT9

#endif
