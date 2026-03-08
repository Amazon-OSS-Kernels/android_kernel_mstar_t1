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
/// file    regHVD.h
/// @brief  HVD Module Register Definition
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_HVD_H_
#define _REG_HVD_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//*****************************************************************************
// RIU macro
#define HVD_MACRO_START     do {
#define HVD_MACRO_END       } while (0)
#define HVD_RIU_BASE        u32HVDRegOSBase

#define HVD_HIGHBYTE(u16)               ((MS_U8)((u16) >> 8))
#define HVD_LOWBYTE(u16)                ((MS_U8)(u16))
#define HVD_RIU_READ_BYTE(addr)   ( READ_BYTE( HVD_RIU_BASE + (addr) ) )
#define HVD_RIU_READ_WORD(addr)   ( READ_WORD( HVD_RIU_BASE + (addr) ) )
#define HVD_RIU_WRITE_BYTE(addr, val)      { WRITE_BYTE( HVD_RIU_BASE+(addr), val); }
#define HVD_RIU_WRITE_WORD(addr, val)      { WRITE_WORD( HVD_RIU_BASE+(addr), val); }


#define _HVD_ReadByte( u32Reg )   HVD_RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define _HVD_Read2Byte( u32Reg )    (HVD_RIU_READ_WORD((u32Reg)<<1))

#define _HVD_Read4Byte( u32Reg )   ( (MS_U32)HVD_RIU_READ_WORD((u32Reg)<<1) | ((MS_U32)HVD_RIU_READ_WORD(((u32Reg)+2)<<1)<<16 )  )

#define _HVD_ReadRegBit( u32Reg, u8Mask )   (HVD_RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define _HVD_ReadWordBit( u32Reg, u16Mask )   (_HVD_Read2Byte( u32Reg ) & (u16Mask))

#define _HVD_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    HVD_MACRO_START                                                                     \
    HVD_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (HVD_RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (HVD_RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    HVD_MACRO_END

#define _HVD_WriteByte( u32Reg, u8Val )                                                 \
    HVD_MACRO_START                                                                     \
    HVD_RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);   \
    HVD_MACRO_END

#define _HVD_Write2Byte( u32Reg, u16Val )                                               \
    HVD_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                               \
        HVD_RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                  \
        HVD_RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        HVD_RIU_WRITE_WORD( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    HVD_MACRO_END

#define _HVD_Write3Byte( u32Reg, u32Val )   \
    if ((u32Reg) & 0x01)                                                                \
    {                                                                                               \
        HVD_RIU_WRITE_BYTE((u32Reg << 1) - 1, u32Val);                                    \
        HVD_RIU_WRITE_WORD( (u32Reg + 1)<<1 , ((u32Val) >> 8));                                      \
    }                                                                                           \
    else                                                                                        \
    {                                                                                               \
        HVD_RIU_WRITE_WORD( (u32Reg) << 1,  u32Val);                                                         \
        HVD_RIU_WRITE_BYTE( (u32Reg + 2) << 1 ,  ((u32Val) >> 16));                             \
    }

#define _HVD_Write4Byte( u32Reg, u32Val )                                               \
    HVD_MACRO_START                                                                     \
    if ((u32Reg) & 0x01)                                                      \
    {                                                                                               \
        HVD_RIU_WRITE_BYTE( ((u32Reg) << 1) - 1 ,  u32Val);                                         \
        HVD_RIU_WRITE_WORD( ((u32Reg) + 1)<<1 , ( (u32Val) >> 8));                                      \
        HVD_RIU_WRITE_BYTE( (((u32Reg) + 3) << 1) ,  ((u32Val) >> 24));                           \
    }                                                                                               \
    else                                                                                                \
    {                                                                                                   \
        HVD_RIU_WRITE_WORD( (u32Reg) <<1 ,  u32Val);                                                             \
        HVD_RIU_WRITE_WORD(  ((u32Reg) + 2)<<1 ,  ((u32Val) >> 16));                                             \
    }                                                                     \
    HVD_MACRO_END

#define _HVD_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    HVD_MACRO_START                                                                     \
    HVD_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (HVD_RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    HVD_MACRO_END

#define _HVD_WriteWordMask( u32Reg, u16Val , u16Msk)                                               \
    HVD_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                                           \
        _HVD_WriteByteMask( ((u32Reg)+1) , (((u16Val) & 0xff00)>>8) , (((u16Msk)&0xff00)>>8) );                                                                          \
        _HVD_WriteByteMask( (u32Reg) , ((u16Val) & 0x00ff) , ((u16Msk)&0x00ff) );                                                                          \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        HVD_RIU_WRITE_WORD( ((u32Reg)<<1) ,  (((u16Val) & (u16Msk))  | (_HVD_Read2Byte( u32Reg  ) & (~( u16Msk ))))  );                                                       \
    }                                                                               \
    HVD_MACRO_END

//------------------------------------------------------------------------------
// MVD Reg
//------------------------------------------------------------------------------
#define REG_MVD_BASE                    (0x1100)

#define MVD_REG_STAT_CTRL               (REG_MVD_BASE)
    #define MVD_REG_CTRL_RST            BIT(0)
    #define MVD_REG_CTRL_INIT           BIT(2)
    #define MVD_REG_DISCONNECT_MIU      BIT(6)

//------------------------------------------------------------------------------
// HVD Reg
//------------------------------------------------------------------------------
#define REG_HVD_BASE                            (0x1B00)

#define HVD_REG_REV_ID                          (REG_HVD_BASE+(( 0x0000)<<1))
#define HVD_REG_RESET                           (REG_HVD_BASE+(( 0x0001)<<1))
    #define HVD_REG_RESET_SWRST                 BIT(0)
    #define HVD_REG_RESET_SWRST_FIN             BIT(2)
    #define HVD_REG_RESET_STOP_BBU              BIT(3)
    #define HVD_REG_RESET_MIU_RDY               BIT(4)
    #define HVD_REG_RESET_HK_AVS_MODE           BIT(8)
    #define HVD_REG_RESET_HK_RM_MODE            BIT(9)
    #define HVD_REG_RESET_HK_RV9_DEC_MODE       BIT(10)
    #define HVD_REG_RESET_MIU_128               BIT(11)
#define HVD_REG_ESB_ST_ADDR_L                   (REG_HVD_BASE+(( 0x0002)<<1))
#define HVD_REG_ESB_ST_ADDR_H                   (REG_HVD_BASE+(( 0x0003)<<1))
//    #define HVD_REG_ESB_ST_ADDR_H_MASK          10
#define HVD_REG_ESB_LENGTH_L                    (REG_HVD_BASE+(( 0x0004)<<1))
#define HVD_REG_ESB_LENGTH_H                    (REG_HVD_BASE+(( 0x0005)<<1))
//    #define HVD_REG_ESB_LENGTH_H_BITS           10
#define HVD_REG_ESB_RPTR                        (REG_HVD_BASE+(( 0x0006)<<1))
    #define HVD_REG_ESB_RPTR_POLL               BIT(0)
//    #define HVD_REG_ESB_RPTR_L_OFFSET           7
//    #define HVD_REG_ESB_RPTR_L_BITS             9
//    #define HVD_REG_ESB_RPTR_L_MASK             BMASK(  ( HVD_REG_ESB_RPTR_L_BITS+HVD_REG_ESB_RPTR_L_OFFSET  )  : HVD_REG_ESB_RPTR_L_OFFSET )//0xFF80
#define HVD_REG_ESB_RPTR_H                      (REG_HVD_BASE+(( 0x0007)<<1))
#define HVD_REG_MIF_BBU                         (REG_HVD_BASE+(( 0x0008)<<1))
    #define HVD_REG_MIF_OFFSET_L_BITS           7
    #define HVD_REG_MIF_OFFSET_H                BIT(12)
    #define HVD_REG_BBU_TSP_INPUT               BIT(8)
    #define HVD_REG_BBU_PASER_MASK              (BIT(10) | BIT(9))
    #define HVD_REG_BBU_PASER_DISABLE           0
    #define HVD_REG_BBU_PASER_ENABLE_ALL        BIT(9)
    #define HVD_REG_BBU_PASER_ENABLE_03         (BIT(9)|BIT(10))
    #define HVD_REG_BBU_AUTO_NAL_TAB            BIT(11)
#define HVD_REG_NAL_TBL_ST_ADDR_L               (REG_HVD_BASE+(( 0x0009)<<1))
#define HVD_REG_NAL_TBL_ST_ADDR_H               (REG_HVD_BASE+(( 0x000A)<<1))
#define HVD_REG_HI_MBOX0_L                      (REG_HVD_BASE+(( 0x000B)<<1))
#define HVD_REG_HI_MBOX0_H                      (REG_HVD_BASE+(( 0x000C)<<1))
#define HVD_REG_HI_MBOX1_L                      (REG_HVD_BASE+(( 0x000D)<<1))
#define HVD_REG_HI_MBOX1_H                      (REG_HVD_BASE+(( 0x000E)<<1))
#define HVD_REG_HI_MBOX_SET                     (REG_HVD_BASE+(( 0x000F)<<1))
    #define HVD_REG_HI_MBOX0_SET                BIT(0)
    #define HVD_REG_HI_MBOX1_SET                BIT(8)
#define HVD_REG_RISC_MBOX_CLR                   (REG_HVD_BASE+(( 0x0010)<<1))
    #define HVD_REG_RISC_MBOX0_CLR              BIT(0)
    #define HVD_REG_RISC_MBOX1_CLR              BIT(1)
    #define HVD_REG_RISC_ISR_CLR                BIT(2)
    #define HVD_REG_NAL_WPTR_SYNC               BIT(3)
    #define HVD_REG_RISC_ISR_MSK                BIT(6)
    #define HVD_REG_RISC_ISR_FORCE              BIT(10)
#define HVD_REG_RISC_MBOX_RDY                   (REG_HVD_BASE+(( 0x0011)<<1))
    #define HVD_REG_RISC_MBOX0_RDY              BIT(0)
    #define HVD_REG_RISC_MBOX1_RDY              BIT(4)
    #define HVD_REG_RISC_ISR_VALID              BIT(8)
#define HVD_REG_HI_MBOX_RDY                     (REG_HVD_BASE+(( 0x0012)<<1))
    #define HVD_REG_HI_MBOX0_RDY                BIT(0)
    #define HVD_REG_HI_MBOX1_RDY                BIT(8)
#define HVD_REG_RISC_MBOX0_L                    (REG_HVD_BASE+(( 0x0013)<<1))
#define HVD_REG_RISC_MBOX0_H                    (REG_HVD_BASE+(( 0x0014)<<1))
#define HVD_REG_RISC_MBOX1_L                    (REG_HVD_BASE+(( 0x0015)<<1))
#define HVD_REG_RISC_MBOX1_H                    (REG_HVD_BASE+(( 0x0016)<<1))
#define HVD_REG_POLL_NAL_RPTR                   (REG_HVD_BASE+(( 0x0017)<<1))
    #define HVD_REG_POLL_NAL_RPTR_BIT           BIT(0)
#define HVD_REG_NAL_RPTR_HI                     (REG_HVD_BASE+(( 0x0018)<<1))
#define HVD_REG_NAL_WPTR_HI                     (REG_HVD_BASE+(( 0x0019)<<1))
#define HVD_REG_NAL_TAB_LEN                     (REG_HVD_BASE+(( 0x0020)<<1))

#define HVD_REG_DEBUG_DAT_L                     (REG_HVD_BASE+(( 0x0023)<<1))
#define HVD_REG_DEBUG_DAT_H                     (REG_HVD_BASE+(( 0x0024)<<1))
#define HVD_REG_DEBUG_SEL                       (REG_HVD_BASE+(( 0x0025)<<1))


/* hvd bs2 reg */
#define HVD_REG_ESB_ST_ADDR_L_BS2               (REG_HVD_BASE+(( 0x0032)<<1))
#define HVD_REG_ESB_ST_ADDR_H_BS2               (REG_HVD_BASE+(( 0x0033)<<1))
#define HVD_REG_ESB_LENGTH_L_BS2                (REG_HVD_BASE+(( 0x0034)<<1))
#define HVD_REG_ESB_LENGTH_H_BS2                (REG_HVD_BASE+(( 0x0035)<<1))
#define HVD_REG_ESB_RPTR_BS2                    (REG_HVD_BASE+(( 0x0036)<<1))
#define HVD_REG_ESB_RPTR_H_BS2                  (REG_HVD_BASE+(( 0x0037)<<1))
#define HVD_REG_MIF_BBU_BS2                     (REG_HVD_BASE+(( 0x0038)<<1))
    #define HVD_REG_MIF_OFFSET_L_BITS_BS2       7
    #define HVD_REG_MIF_OFFSET_H_BS2            BIT(12)
    #define HVD_REG_BBU_TSP_INPUT_BS2           BIT(8)
    #define HVD_REG_BBU_PASER_MASK_BS2          (BIT(10) | BIT(9))
    #define HVD_REG_BBU_PASER_DISABLE_BS2       0
    #define HVD_REG_BBU_PASER_ENABLE_ALL_BS2    BIT(9)
    #define HVD_REG_BBU_PASER_ENABLE_03_BS2     (BIT(9)|BIT(10))
    #define HVD_REG_BBU_AUTO_NAL_TAB_BS2        BIT(11)
#define HVD_REG_NAL_TBL_ST_ADDR_L_BS2           (REG_HVD_BASE+(( 0x0039)<<1))
#define HVD_REG_NAL_TBL_ST_ADDR_H_BS2           (REG_HVD_BASE+(( 0x003A)<<1))
#define HVD_REG_NAL_RPTR_HI_BS2                 (REG_HVD_BASE+(( 0x003B)<<1))
#define HVD_REG_NAL_WPTR_HI_BS2                 (REG_HVD_BASE+(( 0x003C)<<1))
#define HVD_REG_NAL_TAB_LEN_BS2                 (REG_HVD_BASE+(( 0x003D)<<1))

//------------------------------------------------------------------------------
// ChipTop Reg
//------------------------------------------------------------------------------

#define CHIPTOP_REG_BASE               (0x1E00 )
#define CLKGEN0_REG_BASE               (0x0B00 )

#define REG_TOP_PSRAM0_1_MIUMUX            (CHIPTOP_REG_BASE+(0x002D<<1))   //TODO
    #define TOP_CKG_PSRAM0_MASK                 BMASK(1:0)
    #define TOP_CKG_PSRAM0_DIS                  BIT(0)
    #define TOP_CKG_PSRAM0_INV                  BIT(1)
    #define TOP_CKG_PSRAM1_MASK                 BMASK(3:2)
    #define TOP_CKG_PSRAM1_DIS                  BIT(0)
    #define TOP_CKG_PSRAM1_INV                  BIT(1)
    #define TOP_MIU_MUX_G07_MASK                BMASK(7:6)
	#define TOP_MIU_MUX_G07_OD_LSB_R            BITS(7:6,0)
	#define TOP_MIU_MUX_G07_GOP2_R              BITS(7:6,1)
    #define TOP_MIU_MUX_G08_MASK                BMASK(9:8)
	#define TOP_MIU_MUX_G08_OD_LSB_W            BITS(9:8,0)
	#define TOP_MIU_MUX_G08_VE_W                BITS(9:8,1)
    #define TOP_MIU_MUX_G15_MASK                BMASK(11:10)
	#define TOP_MIU_MUX_G15_GOP2_R              BITS(11:10,0)
	#define TOP_MIU_MUX_G15_OD_LSB_R            BITS(11:10,1)
    #define TOP_MIU_MUX_G1A_MASK                BMASK(13:12)
	#define TOP_MIU_MUX_G1A_VE_W                BITS(13:12,0)
	#define TOP_MIU_MUX_G1A_OD_LSB_W            BITS(13:12,1)
    #define TOP_MIU_MUX_G26_MASK                BMASK(15:14)
	#define TOP_MIU_MUX_G26_RVD_RW              BITS(15:14,0)
	#define TOP_MIU_MUX_G26_SVD_INTP_R          BITS(15:14,1)
	#define TOP_MIU_MUX_G26_MVD_R               BITS(15:14,2)

#define REG_TOP_VPU             (CLKGEN0_REG_BASE+(0x0030<<1))
    #define TOP_CKG_VPU_MASK                    BMASK(3:0)
    #define TOP_CKG_VPU_DIS                     BIT(0)
    #define TOP_CKG_VPU_INV                     BIT(1)
    #define TOP_CKG_VPU_CLK_MASK                BMASK(3:2)
    #define TOP_CKG_VPU_160MHZ                  BITS(3:2,0)
    #define TOP_CKG_VPU_144MHZ                  BITS(3:2,1)
    #define TOP_CKG_VPU_123MHZ                  BITS(3:2,2)
    #define TOP_CKG_VPU_108MHZ                  BITS(3:2,3)
//    #define TOP_CKG_VPU_96MHZ                 BITS(6:2,4)
//    #define TOP_CKG_VPU_72MHZ                 BITS(6:2,5)
//    #define TOP_CKG_VPU_MCU                   BITS(6:2,8)
//    #define TOP_CKG_VPU_MIU                   BITS(6:2,16)
//    #define TOP_CKG_VPU_XTALI                 BITS(6:2,24)

#define REG_TOP_HVD             (CLKGEN0_REG_BASE+(0x0031<<1))
    #define TOP_CKG_HVD_MASK                    BMASK(3:0)
    #define TOP_CKG_HVD_DIS                     BIT(0)
    #define TOP_CKG_HVD_INV                     BIT(1)
    #define TOP_CKG_HVD_CLK_MASK                BMASK(3:2)
    #define TOP_CKG_HVD_216MHZ                  BITS(3:2,0)
    #define TOP_CKG_HVD_172MHZ                  BITS(3:2,1)
    #define TOP_CKG_HVD_160MHZ                  BITS(3:2,2)
    #define TOP_CKG_HVD_144MHZ                  BITS(3:2,3)
//    #define TOP_CKG_HVD_XTAL                  BITS(5:2,4)
//    #define TOP_CKG_HVD_MIU                   BITS(5:2,5)


#define REG_TOP_MVD             (CLKGEN0_REG_BASE+(0x0039<<1))
    #define TOP_CKG_MVD_MASK                    BMASK(4:0)
    #define TOP_CKG_MHVD_DIS                    BIT(0)
    #define TOP_CKG_MVD_INV                     BIT(1)
    #define TOP_CKG_MVD_CLK_MASK                BMASK(4:2)
    #define TOP_CKG_MVD_216MHZ                  BITS(4:2,0)
    #define TOP_CKG_MVD_192MHZ                  BITS(4:2,1)
    #define TOP_CKG_MVD_172MHZ                  BITS(4:2,2)
    #define TOP_CKG_MVD_144MHZ                  BITS(4:2,3)
    #define TOP_CKG_MVD_MIU                     BITS(4:2,4)
    #define TOP_CKG_MVD_123MHZ                  BITS(4:2,5)
    #define TOP_CKG_MVD_XTAL                    BITS(4:2,7)

#define REG_TOP_MVD2             (CLKGEN0_REG_BASE+(0x0039<<1))
    #define TOP_CKG_MVD2_MASK                   BMASK(12:8)
    #define TOP_CKG_MHVD2_DIS                   BIT(8)
    #define TOP_CKG_MVD2_INV                    BIT(9)
    #define TOP_CKG_MVD2_CLK_MASK               BMASK(12:10)
    #define TOP_CKG_MVD2_216MHZ                 BITS(12:10,0)
    #define TOP_CKG_MVD2_192MHZ                 BITS(12:10,1)
    #define TOP_CKG_MVD2_172MHZ                 BITS(12:10,2)
    #define TOP_CKG_MVD2_144MHZ                 BITS(12:10,3)
    #define TOP_CKG_MVD2_MIU                    BITS(12:10,4)
    #define TOP_CKG_MVD2_123MHZ                 BITS(12:10,5)
    #define TOP_CKG_MVD2_XTAL                   BITS(12:10,7)

#define REG_TOP_HVD_CLK             (CLKGEN0_REG_BASE+(0x003F<<1))
    #define TOP_CLK_HVD_VP8_MASK                BMASK(1:0)
        #define TOP_CLK_HVD_VP8_DIS             BIT(0)
        #define TOP_CLK_HVD_VP8_INV             BIT(1)
    #define TOP_CLK_MIU_HVD_MASK                BMASK(5:4)
        #define TOP_CLK_MIU_HVD_DIS             BIT(0)
        #define TOP_CLK_MIU_HVD_INV             BIT(1)
    #define TOP_CLK_RM_MIU_HVD_MASK             BMASK(9:8)
        #define TOP_CLK_RM_MIU_HVD_DIS          BIT(0)
        #define TOP_CLK_RM_MIU_HVD_INV          BIT(1)
    #define TOP_CLK_MIU_HVD_VP8_MASK            BMASK(13:12)
        #define TOP_CLK_MIU_HVD_VP8_DIS         BIT(0)
        #define TOP_CLK_MIU_HVD_VP8_INV         BIT(1)

#define REG_TOP_UART_SEL0             (CHIPTOP_REG_BASE+(0x0053<<1))
    #define REG_TOP_UART_SEL_0_MASK             BMASK(3:0)
    #define REG_TOP_UART_SEL_MHEG5              BITS(3:0 , 1)
    #define REG_TOP_UART_SEL_VD_MHEG5           BITS(3:0 , 2)
    #define REG_TOP_UART_SEL_TSP                BITS(3:0 , 3)
    #define REG_TOP_UART_SEL_PIU_0              BITS(3:0 , 4)
    #define REG_TOP_UART_SEL_PIU_1              BITS(3:0 , 5)
    #define REG_TOP_UART_SEL_PIU_FAST           BITS(3:0 , 7)
    #define REG_TOP_UART_SEL_VD_MCU_51_TXD0     BITS(3:0 , 10)
    #define REG_TOP_UART_SEL_VD_MCU_51_TXD1     BITS(3:0 , 11)
//------------------------------------------------------------------------------
// MIU Reg
//------------------------------------------------------------------------------
#define MIU0_REG_HVD_BASE             	(0x1200)
#define MIU1_REG_HVD_BASE             	(0x0600)

#define MIU0_REG_RQ0_MASK               (MIU0_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU0_REG_RQ1_MASK               (MIU0_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU0_REG_RQ2_MASK               (MIU0_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU0_REG_RQ3_MASK               (MIU0_REG_HVD_BASE+(( 0x0053)<<1))
#define MIU1_REG_RQ0_MASK               (MIU1_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU1_REG_RQ1_MASK               (MIU1_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU1_REG_RQ2_MASK               (MIU1_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU1_REG_RQ3_MASK               (MIU1_REG_HVD_BASE+(( 0x0053)<<1))

#define MIU0_REG_SEL0                   (MIU0_REG_HVD_BASE+(( 0x0078)<<1))
#define MIU0_REG_SEL1                   (MIU0_REG_HVD_BASE+(( 0x0079)<<1))
#define MIU0_REG_SEL2                   (MIU0_REG_HVD_BASE+(( 0x007A)<<1))
#define MIU0_REG_SEL3                   (MIU0_REG_HVD_BASE+(( 0x007B)<<1))
#define MIU1_REG_SEL0                   (MIU1_REG_HVD_BASE+(( 0x0078)<<1))


#define MIU_HVD_RW      (BIT(10)|BIT(11))
#define MIU_MVD_RW      (BIT(5)|BIT(6))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_HVD_H_

