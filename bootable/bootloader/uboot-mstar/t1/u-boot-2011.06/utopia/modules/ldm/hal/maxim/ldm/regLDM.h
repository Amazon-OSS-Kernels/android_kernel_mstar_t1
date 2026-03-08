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
/// file    regLDMA.h
/// @brief  Master local dimming Dma Register Definition
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_LDM_H_
#define _REG_LDM_H_

//-------------------------------------------------------------------------------------------------
// Include File
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// BASEADDR & BK
#define BASEADDR_RIU                   0xBF000000

#define REG_LD_BASE     (0x2F00*2) //102F  LDM_60HZ:132E bank  LDM_120HZ:13CE bank

//102F Subbank C9
#define REG_FO_DATA_PATH  (0x02)

/* ========== 102F subbank 2E/CE  register define for local dimming  ==============*/
#define REG_LD_ENABLE     (0x01)

#define REG_BL_WIDTH_DMA  (0x60) //DMA width :BIT0~BIT5  height : bit8 ~bit13
#define REG_BL_WIDTH_LED  (0x63)//LED width :BIT0~BIT5  height : bit8 ~bit13

//LDF baseaddr
#define REG_LDF_BASEADDR0_LOW_L   (0x04)
#define REG_LDF_BASEADDR0_HIGH_L   (0x05)

#define REG_LDF_BASEADDR1_LOW_L   (0x06)
#define REG_LDF_BASEADDR1_HIGH_L   (0x07)

#define REG_LDF_BASEADDR0_LOW_R   (0x08)
#define REG_LDF_BASEADDR0_HIGH_R   (0x09)

#define REG_LDF_BASEADDR1_LOW_R   (0x0A)
#define REG_LDF_BASEADDR1_HIGH_R  (0x0B)

//LDB baseaddr
#define REG_LDB_BASEADDR0_LOW_L   (0x4D)
#define REG_LDB_BASEADDR0_HIGH_L   (0x4E)

#define REG_LDB_BASEADDR1_LOW_L   (0x4F)
#define REG_LDB_BASEADDR1_HIGH_L   (0x50)

#define REG_LDB_BASEADDR0_LOW_R   (0x51)
#define REG_LDB_BASEADDR0_HIGH_R   (0x52)

#define REG_LDB_BASEADDR1_LOW_R   (0x53)
#define REG_LDB_BASEADDR1_HIGH_R  (0x54)
//Edge2d baseaddr
#define REG_EDGE_2D_BASEADDR_LOW  (0x79)
#define REG_EDGE_2D_BASEADDR_HIGH  (0x7A)

//DMA0 Pack offset
#define REG_PACK_OFFSET0      (0x64)
#define REG_PACK_LENGTH0      (0x65)
#define REG_DMA0_ENABLE       (0x65)
#define REG_DMA0_ENABLE_BIT   BIT8
//DMA1 Pack offset
#define REG_PACK_OFFSET1      (0x66)
#define REG_PACK_LENGTH1      (0x67)
#define REG_DMA1_ENABLE       (0x67)
#define REG_DMA1_ENABLE_BIT   BIT8
//DMA2 Pack offset
#define REG_PACK_OFFSET2      (0x68)
#define REG_PACK_LENGTH2      (0x69)
#define REG_DMA2_ENABLE       (0x69)
#define REG_DMA2_ENABLE_BIT   BIT8
//DMA3 Pack offset
#define REG_PACK_OFFSET3      (0x6A)
#define REG_PACK_LENGTH3      (0x6B)
#define REG_DMA3_ENABLE       (0x6B)
#define REG_DMA3_ENABLE_BIT   BIT8

#define REG_DMA_YOFF_END_0    (0x6C)
#define REG_DMA_YOFF_END_1    (0x6D)
#define REG_DMA_YOFF_END_2    (0x6E)
#define REG_DMA_YOFF_END_3    (0x6F)

#define REG_BASEADDR_OFFSET   (0x7E)

/* ========== 102F subbank 2E/CE  register define for local dimming end ==============*/

#endif
