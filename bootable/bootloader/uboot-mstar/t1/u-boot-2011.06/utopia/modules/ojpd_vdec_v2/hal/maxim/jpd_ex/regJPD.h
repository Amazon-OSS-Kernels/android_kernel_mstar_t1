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
/// file    regJPD.h
/// @brief  JPD Register Table
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_JPD_H_
#define _REG_JPD_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/******* Internal table SRAM address *******/
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
#define JPD_MEM_SCWGIF_BASE         0x0000
#define JPD_MEM_SYMIDX_BASE         0x0400
#define JPD_MEM_QTBL_BASE           0x0800
#else
#define JPD_MEM_SCWGIF_BASE         0x0000
#define JPD_MEM_SYMIDX_BASE         0x0200
#define JPD_MEM_QTBL_BASE           0x0400
#endif
/**************************************/

//M-Config
#define JPD_MWC_WPEN        JPD_BIT(0) // bit0 of 2nd byte of BK_JPD_MCONFIG
#define JPD_MIF_RST         JPD_BIT(3)
#define JPD_MIU_SEL         (JPD_BIT(1) | JPD_BIT(2)) //bit1 and bit2 of 2nd byte of BK_JPD_MCONFIG
#if (JPD_SUPPORT_AUTO_PROTECT==true)
#define JPD_AUTO_PROTECT    JPD_BIT(0)
#endif

#define JPD_REG_BASE                0x1700

#define BK_JPD_SCONFIG              (JPD_REG_BASE+JPD_OFFSET(0x00))
#define BK_JPD_MCONFIG              (JPD_REG_BASE+JPD_OFFSET(0x01))
#define BK_JPD_RSTINTV              (JPD_REG_BASE+JPD_OFFSET(0x02))
#define BK_JPD_PIC_H                (JPD_REG_BASE+JPD_OFFSET(0x03))
#define BK_JPD_PIC_V                (JPD_REG_BASE+JPD_OFFSET(0x04))
#define BK_JPD_ROI_H                (JPD_REG_BASE+JPD_OFFSET(0x05))
#define BK_JPD_ROI_V                (JPD_REG_BASE+JPD_OFFSET(0x06))
#define BK_JPD_ROI_WIDTH            (JPD_REG_BASE+JPD_OFFSET(0x07))
#define BK_JPD_ROI_HEIGHT           (JPD_REG_BASE+JPD_OFFSET(0x08))
#define BK_JPD_INTEN                (JPD_REG_BASE+JPD_OFFSET(0x09))
#define BK_JPD_EVENTFLAG            (JPD_REG_BASE+JPD_OFFSET(0x0A))
#define BK_JPD_RCSMADR_L            (JPD_REG_BASE+JPD_OFFSET(0x0B))
#define BK_JPD_RCSMADR_H            (JPD_REG_BASE+JPD_OFFSET(0x0C))
#define BK_JPD_RBUF_FLOOR_L         (JPD_REG_BASE+JPD_OFFSET(0x0D))
#define BK_JPD_RBUF_FLOOR_H         (JPD_REG_BASE+JPD_OFFSET(0x0E))
#define BK_JPD_RBUF_CEIL_L          (JPD_REG_BASE+JPD_OFFSET(0x0F))
#define BK_JPD_RBUF_CEIL_H          (JPD_REG_BASE+JPD_OFFSET(0x10))
#define BK_JPD_MWBF_SADR_L          (JPD_REG_BASE+JPD_OFFSET(0x11))
#define BK_JPD_MWBF_SADR_H          (JPD_REG_BASE+JPD_OFFSET(0x12))
#define BK_JPD_MWBF_LINE_NUM        (JPD_REG_BASE+JPD_OFFSET(0x13))
#define BK_JPD_CUR_MADR_L           (JPD_REG_BASE+JPD_OFFSET(0x14))
#define BK_JPD_CUR_MADR_H           (JPD_REG_BASE+JPD_OFFSET(0x15))
#define BK_JPD_CUR_ROWP             (JPD_REG_BASE+JPD_OFFSET(0x16))
#define BK_JPD_CUR_CLNP             (JPD_REG_BASE+JPD_OFFSET(0x17))
#define BK_JPD_CUR_VIDX             (JPD_REG_BASE+JPD_OFFSET(0x18))
#define BK_JPD_BIST_FAIL            (JPD_REG_BASE+JPD_OFFSET(0x19))
#define BK_JPD_DBG_MATCHV           (JPD_REG_BASE+JPD_OFFSET(0x1A))
#define BK_JPD_DBG_CTRL             (JPD_REG_BASE+JPD_OFFSET(0x1B))
#define BK_JPD_SPARE                (JPD_REG_BASE+JPD_OFFSET(0x1C))

#if (JPD_SUPPORT_AUTO_PROTECT==true)
#define BK_JPD_AUTO_PROTECT         (JPD_REG_BASE+JPD_OFFSET(0x1F))
#define BK_JPD_MWC_WPEN_L           (JPD_REG_BASE+JPD_OFFSET(0x20))
#define BK_JPD_MWC_WPEN_H           (JPD_REG_BASE+JPD_OFFSET(0x21))
#endif

#define BK_JPD_IP_VERSION           (JPD_REG_BASE+JPD_OFFSET(0x28))

#define BK_JPD_TID_ADR              (JPD_REG_BASE+JPD_OFFSET(0x40))
#define BK_JPD_TID_DAT              (JPD_REG_BASE+JPD_OFFSET(0x41))

// Chip Top
#define JPD_CHIPTOP_REG_BASE        0x0B00
//#define JPD_CLOCK_S4                (JPD_CHIPTOP_REG_BASE+JPD_OFFSET(0x2D))  // Eris
//#define JPD_CLOCK_S4L               (JPD_CHIPTOP_REG_BASE+JPD_OFFSET(0x4D))  // Titania
//#define JPD_CLOCK                   (JPD_CHIPTOP_REG_BASE+JPD_OFFSET(0x4D))  // Titania 1/2
#define JPD_CLOCK                   (JPD_CHIPTOP_REG_BASE+JPD_OFFSET(0x35))  // T3/T4/T7/JANUS/U4: (0x580 + 0x35)*2
//#define JPD_CLOCK                   (JPD_CHIPTOP_REG_BASE+JPD_OFFSET(0x16))  // U3:

// MIU
#define JPD_MIU0_BASE               0x1200
#define JPD_MIU1_BASE               0x0600
#define JPD_MIU0_RQ2_MASK           (JPD_MIU0_BASE+JPD_OFFSET(0x43)) //group2
#define JPD_MIU1_RQ2_MASK           (JPD_MIU1_BASE+JPD_OFFSET(0x43)) //group2
#define JPD_MIU0_MIU_SEL2           (JPD_MIU0_BASE+JPD_OFFSET(0x7a)) //group2
#define JPD_MIU1_MIU_SEL2           (JPD_MIU1_BASE+JPD_OFFSET(0x7a)) //group2
#define JPD_MIU0_CLIENT_JPD         JPD_BIT(4) //group3, bit4 of the second byte
#define JPD_MIU1_CLIENT_JPD         JPD_BIT(4) //group3, bit4 of the second byte


// CHIPTOP
#define JPD_CHIPTOP_BASE               0x1E00
#define JPD_MIU_GROUP2_I64             (JPD_CHIPTOP_BASE + JPD_OFFSET(0x22))
#define JPD_MIU0_CLIENT_JPD_CS2       JPD_BIT(4) // bit 4 of the second byte
#define JPD_MIU0_CLIENT_JPD_CS3       JPD_BIT(2) // bit 2 of the second byte


// NJPD
#define NJPD1_REG_BASE                   0x23200
#define NJPD2_REG_BASE                   0x23300
#define BK_NJPD1_SPARE00                             (NJPD1_REG_BASE+JPD_OFFSET(0x48))
#define BK_NJPD2_SPARE00                             (NJPD2_REG_BASE+JPD_OFFSET(0x48))
#define BK_NJPD1_TOP_MARB_PORT_ENABLE                (NJPD1_REG_BASE+JPD_OFFSET(0x76))
#define BK_NJPD2_TOP_MARB_PORT_ENABLE                (NJPD2_REG_BASE+JPD_OFFSET(0x76))
#define NJPD_TOP_MARB_P0_ENABLE                     JPD_BIT(0)
#define NJPD_TOP_MARB_P1_ENABLE                     JPD_BIT(1)
#define NJPD_TOP_MARB_P2_ENABLE                     JPD_BIT(2)
#define NJPD_TOP_MARB_P0_W_BYPASS_ENABLE            JPD_BIT(4)
#define NJPD_TOP_MARB_P1_W_BYPASS_ENABLE            JPD_BIT(5)
#define NJPD_TOP_MARB_P2_W_BYPASS_ENABLE            JPD_BIT(6)
#define NJPD_TOP_MARB_P0_R_BYPASS_ENABLE            JPD_BIT(7)
#define NJPD_TOP_MARB_P1_R_BYPASS_ENABLE            JPD_BIT(0)  // second byte
#define NJPD_TOP_MARB_P2_R_BYPASS_ENABLE            JPD_BIT(1)  // second byte

/*================================ MJPEG =====================================*/

#if 0
// from regSVD.h
// SVD register
#define SVD_REG_BASE                    0x1B00  // 0x1B00 - 0x1BFF
#define REG_SVD_CPU_SDR_BASE_L          (SVD_REG_BASE + JPD_OFFSET(0x08))
#define REG_SVD_RESET                   (SVD_REG_BASE + JPD_OFFSET(0x0A))
    #define REG_SVD_RESET_SWRST         JPD_BIT(0)
    #define REG_SVD_RESET_CPURST        JPD_BIT(1)
    #define REG_SVD_RESET_SWRST_FIN     JPD_BIT(2)
    #define REG_SVD_RESET_CPURST_FIN    JPD_BIT(3)
#define REG_SVD_LDEND_EN                (SVD_REG_BASE + JPD_OFFSET(0x15))
#define REG_SVD_MIU_OFFSET_H0           (SVD_REG_BASE + JPD_OFFSET(0x1F))

// SVD Chiptop clock
#define REG_CKG_SVD                     0x1E58
    #define CKG_SVD_GATED               JPD_BIT(0)
    #define CKG_SVD_INVERT              JPD_BIT(1)
    #define CKG_SVD_MASK                JPD_BITMASK(6:2)
    #define CKG_SVD_240MHZ              (0 << 2)
    #define CKG_SVD_216MHZ              (1 << 2)
    #define CKG_SVD_CLK_MVD_P           (2 << 2)
    #define CKG_SVD_CLK_RVD_P           (3 << 2)
    #define CKG_SVD_CLK_MIU             (8 << 2)
    #define CKG_SVD_XTAL                (18 << 2)

//#define REG_SVD_INT                     (SVD_REG_BASE + JPD_OFFSET(0x16))
#define REG_SVD_HI_MBOX0_L              (SVD_REG_BASE + JPD_OFFSET(0x17))
#define REG_SVD_HI_MBOX0_H              (SVD_REG_BASE + JPD_OFFSET(0x18))
#define REG_SVD_HI_MBOX1_L              (SVD_REG_BASE + JPD_OFFSET(0x19))
#define REG_SVD_HI_MBOX1_H              (SVD_REG_BASE + JPD_OFFSET(0x1A))

#define REG_SVD_HI_MBOX_SET             (SVD_REG_BASE + JPD_OFFSET(0x1B))
    #define REG_SVD_HI_MBOX0_SET    JPD_BIT(0)
    #define REG_SVD_HI_MBOX1_SET    JPD_BIT(1)
#define REG_SVD_RISC_MBOX_CLR           (SVD_REG_BASE + JPD_OFFSET(0x1C))
    #define REG_SVD_RISC_MBOX0_CLR  JPD_BIT(0)
    #define REG_SVD_RISC_MBOX1_CLR  JPD_BIT(1)
#define REG_SVD_RISC_MBOX_RDY           (SVD_REG_BASE + JPD_OFFSET(0x1D))
    #define REG_SVD_RISC_MBOX0_RDY  JPD_BIT(0)
    #define REG_SVD_RISC_MBOX1_RDY  JPD_BIT(1)
#define REG_SVD_HI_MBOX_RDY             (SVD_REG_BASE + JPD_OFFSET(0x1E))
    #define REG_SVD_HI_MBOX0_RDY    JPD_BIT(0)
    #define REG_SVD_HI_MBOX1_RDY    JPD_BIT(1)

#define REG_SVD_RISC_MBOX0_L            (SVD_REG_BASE + JPD_OFFSET(0x23))
#define REG_SVD_RISC_MBOX0_H            (SVD_REG_BASE + JPD_OFFSET(0x24))
#define REG_SVD_RISC_MBOX1_L            (SVD_REG_BASE + JPD_OFFSET(0x25))
#define REG_SVD_RISC_MBOX1_H            (SVD_REG_BASE + JPD_OFFSET(0x26))
#endif


// TSP
#define TSP_REG_BASE                    0x1500
#define REG_TSP_CTRL                    (TSP_REG_BASE + JPD_OFFSET(0x7A))
    #define REG_TSP_CPU_ENABLE          JPD_BIT(0)
    #define REG_TSP_SW_RSTZ             JPD_BIT(1)
#define REG_TSP_STC_L                   (TSP_REG_BASE + JPD_OFFSET(0x30))
#define REG_TSP_STC_H                   (TSP_REG_BASE + JPD_OFFSET(0x31))


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_JPD_H_

