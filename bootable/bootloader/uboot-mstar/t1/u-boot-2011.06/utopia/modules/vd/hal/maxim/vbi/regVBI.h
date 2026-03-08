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

#ifndef _REG_VBI_H
#define _REG_VBI_H

#define VBI_REG_BASE                0x3700UL  // 0x3700 - 0x37FF
#define AFEC_REG_BASE               0x3500UL  // 0x3500 - 0x35FF
#define CHIP_REG_BASE               0x1E00UL  // 0x1E00 - 0x1EFF
#define ADC_ATOP_REG_BASE           0x2500UL  // 0x2500 - 0x25FF

#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)

#define CC_BUF_BIT 28
#define TTX_BUF_BIT 28
////////////////////////////////////////////////////////////////////////////////
// VBI register
////////////////////////////////////////////////////////////////////////////////
#define BK_VBI_1D  (VBI_REG_BASE+0x1D)
#define BK_VBI_2A  (VBI_REG_BASE+0x2A)
#define BK_VBI_40  (VBI_REG_BASE+0x40)
#define BK_VBI_41  (VBI_REG_BASE+0x41)
#define BK_VBI_42  (VBI_REG_BASE+0x42)
#define BK_VBI_44  (VBI_REG_BASE+0x44)
#define BK_VBI_45  (VBI_REG_BASE+0x45)
#define BK_VBI_46  (VBI_REG_BASE+0x46)
#define BK_VBI_4A  (VBI_REG_BASE+0x4A)
#define BK_VBI_4B  (VBI_REG_BASE+0x4B)
#define BK_VBI_4D  (VBI_REG_BASE+0x4D)
#define BK_VBI_4F  (VBI_REG_BASE+0x4F)
#define BK_VBI_50  (VBI_REG_BASE+0x50)
#define BK_VBI_51  (VBI_REG_BASE+0x51)
#define BK_VBI_53  (VBI_REG_BASE+0x53)
#define BK_VBI_55  (VBI_REG_BASE+0x55)
#define BK_VBI_56  (VBI_REG_BASE+0x56)
#define BK_VBI_57  (VBI_REG_BASE+0x57)
#define BK_VBI_58  (VBI_REG_BASE+0x58)
#define BK_VBI_59  (VBI_REG_BASE+0x59)
#define BK_VBI_5A  (VBI_REG_BASE+0x5A)
#define BK_VBI_5B  (VBI_REG_BASE+0x5B)
#define BK_VBI_5C  (VBI_REG_BASE+0x5C)
#define BK_VBI_5D  (VBI_REG_BASE+0x5D)
#define BK_VBI_5E  (VBI_REG_BASE+0x5E)
#define BK_VBI_5F  (VBI_REG_BASE+0x5F)
#define BK_VBI_70  (VBI_REG_BASE+0x70)
#define BK_VBI_71  (VBI_REG_BASE+0x71)
#define BK_VBI_72  (VBI_REG_BASE+0x72)
#define BK_VBI_77  (VBI_REG_BASE+0x77)
#define BK_VBI_7C  (VBI_REG_BASE+0x7C)
#define BK_VBI_7D  (VBI_REG_BASE+0x7D)
#define BK_VBI_7E  (VBI_REG_BASE+0x7E)
#define BK_VBI_7F  (VBI_REG_BASE+0x7F)
#define BK_VBI_81  (VBI_REG_BASE+0x81)
#define BK_VBI_82  (VBI_REG_BASE+0x82)
#define BK_VBI_83  (VBI_REG_BASE+0x83)
#define BK_VBI_86  (VBI_REG_BASE+0x86)
#define BK_VBI_89  (VBI_REG_BASE+0x89)
#define BK_VBI_8A  (VBI_REG_BASE+0x8A)
#define BK_VBI_8B  (VBI_REG_BASE+0x8B)
#define BK_VBI_8D  (VBI_REG_BASE+0x8D)
#define BK_VBI_90  (VBI_REG_BASE+0x90)
#define BK_VBI_91  (VBI_REG_BASE+0x91)
#define BK_VBI_92  (VBI_REG_BASE+0x92)
#define BK_VBI_99  (VBI_REG_BASE+0x99)
#define BK_VBI_9A  (VBI_REG_BASE+0x9A)
#define BK_VBI_A6  (VBI_REG_BASE+0xA6)
#define BK_VBI_A7  (VBI_REG_BASE+0xA7)
#define BK_VBI_AD  (VBI_REG_BASE+0xAD)
#define BK_VBI_AE  (VBI_REG_BASE+0xAE)
#define BK_VBI_AF  (VBI_REG_BASE+0xAF)
#define BK_VBI_B4  (VBI_REG_BASE+0xB4)
#define BK_VBI_B5  (VBI_REG_BASE+0xB5)
#define BK_VBI_B7  (VBI_REG_BASE+0xB7)
#define BK_VBI_B8  (VBI_REG_BASE+0xB8)
#define BK_VBI_BB  (VBI_REG_BASE+0xBB)
#define BK_VBI_C4  (VBI_REG_BASE+0xC4)
#define BK_VBI_CA  (VBI_REG_BASE+0xCA)
#define BK_VBI_CB  (VBI_REG_BASE+0xCB)
#define BK_VBI_CC  (VBI_REG_BASE+0xCC)
#define BK_VBI_CD  (VBI_REG_BASE+0xCD)
#define BK_VBI_CE  (VBI_REG_BASE+0xCE)

#define BK_VBI_DE  (VBI_REG_BASE+0xDE)
#define BK_VBI_DF  (VBI_REG_BASE+0xDF)
#define BK_VBI_E0  (VBI_REG_BASE+0xE0)
#define BK_VBI_E1  (VBI_REG_BASE+0xE1)
#define BK_VBI_E2  (VBI_REG_BASE+0xE2)
#define BK_VBI_E3  (VBI_REG_BASE+0xE3)
#define BK_VBI_E4  (VBI_REG_BASE+0xE4)
#define BK_VBI_E5  (VBI_REG_BASE+0xE5)
#define BK_VBI_E6  (VBI_REG_BASE+0xE6)
#define BK_VBI_E7  (VBI_REG_BASE+0xE7)

#define VBI_INTERRUPT_MASK      0x6C + (VBI_REG_BASE)
#define VBI_INTERRUPT_CLEAR     0x6D + (VBI_REG_BASE)
#define VBI_INTERRUPT_STATUS    0x6E + (VBI_REG_BASE)
#define VBI_INTERRUPT_RAW       0x6F + (VBI_REG_BASE)

#define VBI_VPS_COUNT           0xA5 + (VBI_REG_BASE)
#define VBI_WSS_COUNT           0xA5 + (VBI_REG_BASE)

////////////////////////////////////////////////////////////////////////////////
// Other registers
////////////////////////////////////////////////////////////////////////////////
#define BK_AFEC_6B              (AFEC_REG_BASE + 0x6B)
#define L_BK_ADC_ATOP(x)        BK_REG_L(ADC_ATOP_REG_BASE, x)
#define H_BK_ADC_ATOP(x)        BK_REG_H(ADC_ATOP_REG_BASE, x)
#define L_BK_CHIPTOP(x)         BK_REG_L(CHIP_REG_BASE, x)
#define H_BK_CHIPTOP(x)         BK_REG_H(CHIP_REG_BASE, x)

/********************************************************************************/
/*                           Start of Teletext Register definitions                                                          */
/********************************************************************************/
#define TT_ENABLE           0x10 + (VBI_REG_BASE)

// DMA and Tag Search Register
#define DMASRC_ADR_L     0x11 + (VBI_REG_BASE) // DMA Source Linear Address
#define DMASRC_ADR_M    0x12 + (VBI_REG_BASE)
#define DMASRC_ADR_H    0x13 + (VBI_REG_BASE)

#define DMADES_ADR_L     0x14 + (VBI_REG_BASE)// DMA Destination Linear Address
#define DMADES_ADR_M    0x15 + (VBI_REG_BASE)
#define DMADES_ADR_H    0x16 + (VBI_REG_BASE)

#define DMAQW_CNT_L     0x17 + (VBI_REG_BASE)// DMA Block Move Count
#define DMAQW_CNT_H     0x18 + (VBI_REG_BASE)

#define DMA_FUNC            0x19 + (VBI_REG_BASE)// DMA Function Seletction
    #define DMA_HEADER                  0x01
    #define DMA_PACKET1_TO_25    0x02
    #define DMA_BTT                         0x04
    #define DMA_AIT                         0x05
    #define DMA_MCU_READ              0x0D
    #define DMA_MCU_WRITE            0x0E
    #define DMA_PACKET26_28_29   0x03
    #define DMA_PACKET27               0x06
    #define DMA_TAG_READ              0x1D
    #define DMA_TAG_WRITE            0x1E
    #define DMA_ERASE                     0x1F

#define DMA_COMMAND             0x1A + (VBI_REG_BASE)
    #define DMA_READY           _BIT1
    #define DMA_FIRE              _BIT0

#define DMAERASE_DATA         0x1B + (VBI_REG_BASE)

#define VBI_BIT24_ADDR      0x1D + (VBI_REG_BASE)
    #define CCBASE_ADDR24   _BIT7
    #define TTBASE_ADDR24   _BIT6
    #define DMASRC_ADDR24   _BIT5
    #define DMADES_ADDR24   _BIT4
    #define TAGBASE_ADDR24   _BIT2
    #define PAGEBASE_ADDR24   _BIT1
    #define TTBASE2_ADDR24   _BIT0

#define TTDEC_COMMAND         0x1F + (VBI_REG_BASE)

#define TTDEC_STATIS            0x20 + (VBI_REG_BASE)
    #define VBIREADY                _BIT7

#define SLICERREADY             0x21 + (VBI_REG_BASE)

#define MCU_ADDR_PORT       0x22 + (VBI_REG_BASE)
    #define BURST_RD_MD        _BIT7
    #define BURST_WR_MD       _BIT6


#define MCU_DATA_PORT       0x23 + (VBI_REG_BASE)

#define TAGRW_POS_L             0x24 + (VBI_REG_BASE)
#define TAGRW_POS_H             0x25 + (VBI_REG_BASE)

#define PAGEBUF_ADDR_L      0x26 + (VBI_REG_BASE)
#define PAGEBUF_ADDR_M      0x27 + (VBI_REG_BASE)
#define PAGEBUF_ADDR_H      0x28 + (VBI_REG_BASE)

#define PAGEBUF_CNT_L           0x29 + (VBI_REG_BASE)
#define PAGEBUF_CNT_H           0x2A + (VBI_REG_BASE)

#define FUNCTION                    0x2B + (VBI_REG_BASE)

#define TAG_MAGAZINE           0x2C + (VBI_REG_BASE)
#define TAG_PAGE                   0x2D + (VBI_REG_BASE)
#define TAG_SUBCODE_H        0x2E + (VBI_REG_BASE)
#define TAG_SUBCODE_L         0x2F + (VBI_REG_BASE)


#define TAG_BASE_L              0x30 + (VBI_REG_BASE)
#define TAG_BASE_H              0x31 + (VBI_REG_BASE)

#define PAGE_BASE_L             0x32 + (VBI_REG_BASE)
#define PAGE_BASE_H             0x33 + (VBI_REG_BASE)

#define PAGE_SIZE               0x34 + (VBI_REG_BASE) // Per 8 Bytes

#define TAG_PHT_SIZE_L      0x35 + (VBI_REG_BASE)
#define TAG_PHT_SIZE_H      0x36 + (VBI_REG_BASE)


#define TAG_COMMAND             0x37 + (VBI_REG_BASE)
    #define BUF_ADDR_RDY        _BIT3
    #define HIT_STATUS             _BIT2
    #define TAG_READY              _BIT1
    #define TAG_FIRE                 _BIT0


#define VBI_BASEADDR_L      0x38 + (VBI_REG_BASE)
#define VBI_BASEADDR_M      0x39 + (VBI_REG_BASE)
#define VBI_BASEADDR_H      0x3A + (VBI_REG_BASE)
#define VBI_BUF_LEN           0x3B + (VBI_REG_BASE)
#define VBI_BUF_LEN_H           0x3C + (VBI_REG_BASE)

#define VBI_W_COUNT            0x3D + (VBI_REG_BASE)
#define VBI_PKTCNT_L             0x3D + (VBI_REG_BASE)


#define TT_LineUpdDef              0x72 + (VBI_REG_BASE)
#define TT_DAT_LN_STR1          0x7C + (VBI_REG_BASE)
#define TT_DAT_LN_END1          0x7D + (VBI_REG_BASE)
#define TT_DAT_LN_STR2          0x7E + (VBI_REG_BASE)
#define TT_DAT_LN_END2          0x7F + (VBI_REG_BASE)
#define TTSLCTHRD                   0x8D + (VBI_REG_BASE)

#define TT_CLK_RUN_IN_START_POINT 0x77 + (VBI_REG_BASE)

#endif

