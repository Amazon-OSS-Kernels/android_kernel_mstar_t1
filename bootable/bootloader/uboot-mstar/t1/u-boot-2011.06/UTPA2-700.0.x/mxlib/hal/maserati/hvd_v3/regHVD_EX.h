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
#define HVD_RIU_BASE        (u32HVDRegOSBase)

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

#if 1//Note: this setting should be set according client table of each chip
#define MIU0_REG_BASE                           0x1200
#define MIU1_REG_BASE                           0x0600

#define MIU_CLIENT_SELECT_GP2          (MIU0_REG_BASE + (0x007A<<1))
    #define MIU_CLIENT_SELECT_GP2_MVD   BIT(4)
#endif



//------------------------------------------------------------------------------
// HVD Reg
//------------------------------------------------------------------------------
#define REG_HVD_BASE                            (0x1B00)
#define REG_EVD_BASE                            (0x60B00)
#define REG_G2VP9_BASE                          (0x60E00)

#define HVD_REG_REV_ID                          (REG_HVD_BASE + ((0x0000) << 1))
#define HVD_REG_RESET                           (REG_HVD_BASE + ((0x0001) << 1))
    #define HVD_REG_RESET_SWRST                 BIT(0)
    #define HVD_REG_RESET_IDB_MIU_256           BIT(1)
    #define HVD_REG_RESET_SWRST_FIN             BIT(2)
    #define HVD_REG_RESET_STOP_BBU              BIT(3)
    #define HVD_REG_RESET_MIU_RDY               BIT(4)
    #define HVD_REG_RESET_MIU1_128              BIT(5)
    #define HVD_REG_RESET_MIU1_256              BIT(6)
    #define HVD_REG_MC_MIU_256                  BIT(7)
    #define HVD_REG_RESET_HK_AVS_MODE           BIT(8)
    #define HVD_REG_RESET_HK_RM_MODE            BIT(9)
    #define HVD_REG_RESET_HK_RV9_DEC_MODE       BIT(10)
    #define HVD_REG_RESET_MIU_128               BIT(11)
    #define HVD_REG_RESET_CPUIF_SEL             BIT(12)
    #define HVD_REG_RESET_ALL_SRAM_SD_EN        BIT(13)
    #define HVD_REG_RESET_MIU_256               BIT(14)
    #define HVD_REG_RESET_BOND_HD               BIT(15)

#define HVD_REG_ESB_ST_ADDR_L(reg_base)                   (reg_base + ((0x0002) << 1))
#define HVD_REG_ESB_ST_ADDR_H(reg_base)                   (reg_base + ((0x0003) << 1))

#define HVD_REG_ESB_LENGTH_L(reg_base)                    (reg_base + ((0x0004) << 1))
#define HVD_REG_ESB_LENGTH_H(reg_base)                    (reg_base + ((0x0005) << 1))

#define HVD_REG_ESB_RPTR(reg_base)                        (reg_base + ((0x0006) << 1))
    #define HVD_REG_ESB_RPTR_POLL               BIT(0)

#define HVD_REG_ESB_RPTR_H(reg_base)                      (reg_base + ((0x0007) << 1))

#define HVD_REG_MIF_BBU(reg_base)                         (reg_base + ((0x0008) << 1))
    #define HVD_REG_MIF_OFFSET_L_BITS           7
    #define HVD_REG_MIF_OFFSET_H                BIT(12)
    #define HVD_REG_BBU_TSP_INPUT               BIT(8)
    #define HVD_REG_BBU_PASER_MASK              (BIT(10) | BIT(9))
    #define HVD_REG_BBU_PASER_DISABLE           0
    #define HVD_REG_BBU_PASER_ENABLE_ALL        BIT(9)
    #define HVD_REG_BBU_PASER_ENABLE_03         (BIT(9) | BIT(10))
    #define HVD_REG_BBU_AUTO_NAL_TAB            BIT(11)

#define HVD_REG_NAL_TBL_ST_ADDR_L(reg_base)               (reg_base + ((0x0009) << 1))
#define HVD_REG_NAL_TBL_ST_ADDR_H(reg_base)               (reg_base + ((0x000A) << 1))

#define HVD_REG_HI_MBOX0_L(reg_base)                      (reg_base + ((0x000B) << 1))
#define HVD_REG_HI_MBOX0_H(reg_base)                      (reg_base + ((0x000C) << 1))
#define HVD_REG_HI_MBOX1_L(reg_base)                      (reg_base + ((0x000D) << 1))
#define HVD_REG_HI_MBOX1_H(reg_base)                      (reg_base + ((0x000E) << 1))
#define HVD_REG_HI_MBOX_SET(reg_base)                     (reg_base + ((0x000F) << 1))
    #define HVD_REG_HI_MBOX0_SET                BIT(0)
    #define HVD_REG_HI_MBOX1_SET                BIT(8)

#define HVD_REG_RISC_MBOX_CLR(reg_base)                   (reg_base + ((0x0010) << 1))
    #define HVD_REG_RISC_MBOX0_CLR              BIT(0)
    #define HVD_REG_RISC_MBOX1_CLR              BIT(1)
    #define HVD_REG_RISC_ISR_CLR                BIT(2)
    #define HVD_REG_NAL_WPTR_SYNC               BIT(3)
    #define HVD_REG_RISC_ISR_MSK                BIT(6)
    #define HVD_REG_RISC_ISR_FORCE              BIT(10)

#define HVD_REG_RISC_MBOX_RDY(reg_base)                   (reg_base + ((0x0011) << 1))
    #define HVD_REG_RISC_MBOX0_RDY              BIT(0)
    #define HVD_REG_RISC_MBOX1_RDY              BIT(4)
    #define HVD_REG_RISC_ISR_VALID              BIT(8)

#define HVD_REG_HI_MBOX_RDY(reg_base)                     (reg_base + ((0x0012) << 1))
    #define HVD_REG_HI_MBOX0_RDY                BIT(0)
    #define HVD_REG_HI_MBOX1_RDY                BIT(8)

#define HVD_REG_RISC_MBOX0_L(reg_base)                    (reg_base + ((0x0013) << 1))
#define HVD_REG_RISC_MBOX0_H(reg_base)                    (reg_base + ((0x0014) << 1))
#define HVD_REG_RISC_MBOX1_L(reg_base)                    (reg_base + ((0x0015) << 1))
#define HVD_REG_RISC_MBOX1_H(reg_base)                    (reg_base + ((0x0016) << 1))

#define HVD_REG_POLL_NAL_RPTR(reg_base)                   (reg_base + ((0x0017) << 1))
    #define HVD_REG_POLL_NAL_RPTR_BIT           BIT(0)
#define HVD_REG_NAL_RPTR_HI(reg_base)                     (reg_base + ((0x0018) << 1))
#define HVD_REG_NAL_WPTR_HI(reg_base)                     (reg_base + ((0x0019) << 1))
#define HVD_REG_NAL_TAB_LEN(reg_base)                     (reg_base + ((0x0020) << 1))

#define HVD_REG_DEBUG_DAT_L                     (REG_HVD_BASE + ((0x0023) << 1))
#define HVD_REG_DEBUG_DAT_H                     (REG_HVD_BASE + ((0x0024) << 1))
#define HVD_REG_DEBUG_SEL                       (REG_HVD_BASE + ((0x0025) << 1))

/* Second bitstream registers definition */
#define HVD_REG_MODE_BS2                        (REG_HVD_BASE + ((0x0030) << 1))
    #define HVD_REG_MODE_HK_AVS_MODE_BS2        BIT(8)
    #define HVD_REG_MODE_HK_RM_MODE_BS2         BIT(9)
    #define HVD_REG_MODE_HK_RV9_DEC_MODE_BS2    BIT(10)

#define HVD_REG_ESB_ST_ADDR_L_BS2(reg_base)               (reg_base + ((0x0032) << 1))
#define HVD_REG_ESB_ST_ADDR_H_BS2(reg_base)               (reg_base + ((0x0033) << 1))

#define HVD_REG_ESB_LENGTH_L_BS2(reg_base)                (reg_base + ((0x0034) << 1))
#define HVD_REG_ESB_LENGTH_H_BS2(reg_base)                (reg_base + ((0x0035) << 1))

#define HVD_REG_ESB_RPTR_L_BS2(reg_base)                  (reg_base + ((0x0036) << 1))
#define HVD_REG_ESB_RPTR_H_BS2(reg_base)                  (reg_base + ((0x0037) << 1))

#define HVD_REG_MIF_BBU_BS2(reg_base)                     (reg_base + ((0x0038) << 1))
    #define HVD_REG_MIF_OFFSET_L_BITS_BS2       7
    #define HVD_REG_MIF_OFFSET_H_BS2            BIT(12)
    #define HVD_REG_BBU_TSP_INPUT_BS2           BIT(8)
    #define HVD_REG_BBU_PASER_MASK_BS2          (BIT(10) | BIT(9))
    #define HVD_REG_BBU_PASER_DISABLE_BS2       0
    #define HVD_REG_BBU_PASER_ENABLE_ALL_BS2    BIT(9)
    #define HVD_REG_BBU_PASER_ENABLE_03_BS2     (BIT(9) | BIT(10))
    #define HVD_REG_BBU_AUTO_NAL_TAB_BS2        BIT(11)

#define HVD_REG_NAL_TBL_ST_ADDR_L_BS2(reg_base)           (reg_base + ((0x0039) << 1))
#define HVD_REG_NAL_TBL_ST_ADDR_H_BS2(reg_base)           (reg_base + ((0x003A) << 1))

#define HVD_REG_NAL_RPTR_HI_BS2(reg_base)                 (reg_base + ((0x003B) << 1))
#define HVD_REG_NAL_WPTR_HI_BS2(reg_base)                 (reg_base + ((0x003C) << 1))
#define HVD_REG_NAL_TAB_LEN_BS2(reg_base)                 (reg_base + ((0x003D) << 1))

#define HVD_REG_ESB_WPTR_L_BS2                 (REG_HVD_BASE + ((0x003E) << 1))
#define HVD_REG_ESB_WPTR_H_BS2                 (REG_HVD_BASE + ((0x003F) << 1))

/* VP8 Registers */
#define HVD_REG_HK_VP8                          (REG_HVD_BASE + ((0x0040) << 1))
    #define HVD_REG_HK_VP8_DEC_MODE             BIT(0)
    #define HVD_REG_HK_PLAYER_FM                BIT(1)

#define HVD_REG_ESB_ST_ADR_L_BS34               (REG_HVD_BASE + ((0x0042) << 1))
#define HVD_REG_ESB_ST_ADR_H_BS34               (REG_HVD_BASE + ((0x0043) << 1))
#define HVD_REG_ESB_LENGTH_L_BS34               (REG_HVD_BASE + ((0x0044) << 1))
#define HVD_REG_ESB_LENGTH_H_BS34               (REG_HVD_BASE + ((0x0045) << 1))

#define HVD_REG_MIF_BS34                        (REG_HVD_BASE + ((0x0048) << 1))
    #define HVD_REG_BS34_MIF_OFFSET_L_BITS       7
    #define HVD_REG_BS34_MIF_OFFSET_H            BIT(12)
    #define HVD_REG_BS34_TSP_INPUT               BIT(8)
    #define HVD_REG_BS34_PASER_MASK              (BIT(10) | BIT(9))
    #define HVD_REG_BS34_PASER_DISABLE           0
    #define HVD_REG_BS34_PASER_ENABLE_ALL        BIT(9)
    #define HVD_REG_BS34_PASER_ENABLE_03         (BIT(9) | BIT(10))
    #define HVD_REG_BS34_AUTO_NAL_TAB            BIT(11)
    #define HVD_REG_BS34_NAL_BUF_SKIP            BIT(13)
    #define HVD_REG_BS34_NAL_BUF_SKIP_RDY        BIT(14)

#define HVD_REG_NAL_TAB_ST_L_BS3                 (REG_HVD_BASE + ((0x0049) << 1))
#define HVD_REG_NAL_TAB_ST_H_BS3                 (REG_HVD_BASE + ((0x004A) << 1))
#define HVD_REG_NAL_RPTR_HI_BS3                  (REG_HVD_BASE + ((0x004B) << 1))
#define HVD_REG_NAL_WPTR_HI_BS3                  (REG_HVD_BASE + ((0x004C) << 1))
#define HVD_REG_NAL_TAB_LEN_BS3                  (REG_HVD_BASE + ((0x004D) << 1))
#define HVD_REG_NAL_TAB_ST_L_BS4                 (REG_HVD_BASE + ((0x0059) << 1))
#define HVD_REG_NAL_TAB_ST_H_BS4                 (REG_HVD_BASE + ((0x005A) << 1))
#define HVD_REG_NAL_RPTR_HI_BS4                  (REG_HVD_BASE + ((0x005B) << 1))
#define HVD_REG_NAL_WPTR_HI_BS4                  (REG_HVD_BASE + ((0x005C) << 1))
#define HVD_REG_NAL_TAB_LEN_BS4                  (REG_HVD_BASE + ((0x005D) << 1))

//------------------------------------------------------------------------------
// EVD Reg
//------------------------------------------------------------------------------
#define REG_EVDPLL_BASE                         (0x10B00)
#define REG_EVDPLL_PD                           (REG_EVDPLL_BASE + ((0x0041) << 1))
    #define REG_EVDPLL_PD_DIS                   BIT(8)

#define REG_EVDPLL_LOOP_DIV_SECOND                (REG_EVDPLL_BASE+(0x0043<<1))
    #define REG_EVDPLL_LOOP_DIV_SECOND_MASK       BMASK(7:0)
    #define REG_EVDPLL_LOOP_DIV_SECOND_456MHZ     BITS(7:0, 19)

#define EVD_REG_RESET                           (REG_EVD_BASE + ((0x0001) << 1))
    #define EVD_REG_RESET_SWRST                 BIT(0)
    #define EVD_REG_RESET_SWRST_FIN             BIT(2)
    #define EVD_REG_RESET_STOP_BBU              BIT(3)
    #define EVD_REG_RESET_MIU_RDY               BIT(4)
    #define EVD_REG_RESET_MIU1_128              BIT(5)
    #define EVD_REG_RESET_MIU1_256              BIT(6)
    #define EVD_REG_RESET_USE_HVD_MIU_EN        BIT(7)
    #define EVD_REG_RESET_HK_HEVC_MODE          BIT(8)
    #define EVD_REG_RESET_HK_TSP2EVD_EN         BIT(9)
    #define EVD_REG_RESET_MIU0_256              BIT(10)
    #define EVD_REG_RESET_MIU0_128              BIT(11)
    #define EVD_REG_RESET_CPUIF_SEL             BIT(12)
    #define EVD_REG_RESET_ALL_SRAM_SD_EN        BIT(13)
    #define EVD_REG_RESET_BOND_UHD              BIT(14)
    #define EVD_REG_RESET_BOND_HD               BIT(15)

//------------------------------------------------------------------------------
// G2 VP9 Reg
//------------------------------------------------------------------------------
#define VP9_REG_RESET                           (REG_G2VP9_BASE + ((0x0001) << 1))
    #define VP9_REG_RESET_SWRST                 BIT(0)
    #define VP9_REG_RESET_SWRST_FIN             BIT(2)
    #define VP9_REG_RESET_MIU_RDY               BIT(4)
    #define VP9_REG_RESET_ALL_SRAM_SD_EN        BIT(13)
    #define VP9_REG_RESET_APB_SEL               BIT(15)

#define EVD_REG_VP9_MODE                        (REG_EVD_BASE + ((0x001b) << 1))
    #define EVD_REG_SET_VP9_MODE                BIT(0)


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
    #define TOP_CKG_VPU_MASK                  BMASK(4:0)
    #define TOP_CKG_VPU_DIS                   BIT(0)
    #define TOP_CKG_VPU_INV                   BIT(1)
    #define TOP_CKG_VPU_CLK_MASK              BMASK(4:2)
    #define TOP_CKG_VPU_240MHZ                BITS(4:2, 0)
    #define TOP_CKG_VPU_216MHZ                BITS(4:2, 1)
    #define TOP_CKG_VPU_192MHZ                BITS(4:2, 2)
    #define TOP_CKG_VPU_12MHZ                 BITS(4:2, 3)
    #define TOP_CKG_VPU_320MHZ                BITS(4:2, 4)
    #define TOP_CKG_VPU_288MHZ                BITS(4:2, 5)
    #define TOP_CKG_VPU_432MHZ                BITS(4:2, 6)
    #define TOP_CKG_VPU_384MHZ                BITS(4:2, 7)

#define REG_TOP_HVD_IDB         (CLKGEN0_REG_BASE+(0x0030<<1))
    #define TOP_CKG_HVD_IDB_CLK_MASK          BMASK(10:8)
    #define TOP_CKG_HVD_IDB_432MHZ            BITS(10:8, 0)  // default use this
    #define TOP_CKG_HVD_IDB_384MHZ            BITS(10:8, 1)
    #define TOP_CKG_HVD_IDB_345MHZ            BITS(10:8, 2)
    #define TOP_CKG_HVD_IDB_480MHZ            BITS(10:8, 3)  // for overclocking
    #define TOP_CKG_HVD_IDB_320MHZ            BITS(10:8, 4)
    #define TOP_CKG_HVD_IDB_288MHZ            BITS(10:8, 5)
    #define TOP_CKG_HVD_IDB_240MHZ            BITS(10:8, 6)
    #define TOP_CKG_HVD_IDB_216MHZ            BITS(10:8, 7)

#define REG_TOP_HVD             (CLKGEN0_REG_BASE+(0x0031<<1))
    #define TOP_CKG_HVD_MASK                  BMASK(4:0)
    #define TOP_CKG_HVD_DIS                   BIT(0)
    #define TOP_CKG_HVD_INV                   BIT(1)
    #define TOP_CKG_HVD_CLK_MASK              BMASK(4:2)
    #define TOP_CKG_HVD_384MHZ                BITS(4:2, 0)  // default use this
    #define TOP_CKG_HVD_345MHZ                BITS(4:2, 1)
    #define TOP_CKG_HVD_320MHZ                BITS(4:2, 2)
    #define TOP_CKG_HVD_288MHZ                BITS(4:2, 3)
    #define TOP_CKG_HVD_240MHZ                BITS(4:2, 4)
    #define TOP_CKG_HVD_216MHZ                BITS(4:2, 5)
    #define TOP_CKG_HVD_172MHZ                BITS(4:2, 6)
    #define TOP_CKG_HVD_432MHZ                BITS(4:2, 7)  // for overclocking

#define REG_TOP_VP8             (CLKGEN0_REG_BASE+(0x0031<<1))
    #define TOP_CKG_VP8_MASK                  BMASK(11:8)
    #define TOP_CKG_VP8_DIS                   BIT(8)
    #define TOP_CKG_VP8_INV                   BIT(9)
    #define TOP_CKG_VP8_CLK_MASK              BMASK(11:10)
    #define TOP_CKG_VP8_288MHZ                BITS(11:10, 0)  // default use this
    #define TOP_CKG_VP8_240MHZ                BITS(11:10, 1)
    #define TOP_CKG_VP8_216MHZ                BITS(11:10, 2)
    #define TOP_CKG_VP8_320MHZ                BITS(11:10, 3)  // for overclocking

#define REG_TOP_HVD_AEC         (CLKGEN0_REG_BASE+(0x0034<<1))
    #define TOP_CKG_HVD_AEC_MASK                  BMASK(4:0)
    #define TOP_CKG_HVD_AEC_DIS                   BIT(0)
    #define TOP_CKG_HVD_AEC_INV                   BIT(1)
    #define TOP_CKG_HVD_AEC_CLK_MASK              BMASK(3:2)
    #define TOP_CKG_HVD_AEC_288MHZ                BITS(3:2, 0) //default use this
    #define TOP_CKG_HVD_AEC_240MHZ                BITS(3:2, 1)
    #define TOP_CKG_HVD_AEC_216MHZ                BITS(3:2, 2)
    #define TOP_CKG_HVD_AEC_320MHZ                BITS(3:2, 3)
    #define TOP_CKG_HVD_AEC_CLK_FROM_HVD_AEC_NEW  BIT(4)  //no need to set; hw switch automatically

#define REG_TOP_VP9             (CLKGEN0_REG_BASE+(0x0032<<1))
    #define TOP_CKG_VP9_MASK                  BMASK(8:4)
    #define TOP_CKG_VP9_DIS                   BIT(4)
    #define TOP_CKG_VP9_INV                   BIT(5)
    #define TOP_CKG_VP9_CLK_MASK              BMASK(8:6)
    #define TOP_CKG_VP9_432MHZ                BITS(8:6,0)
    #define TOP_CKG_VP9_384MHZ                BITS(8:6,1)
    #define TOP_CKG_VP9_345MHZ                BITS(8:6,2)
    #define TOP_CKG_VP9_320MHZ                BITS(8:6,3)
    #define TOP_CKG_VP9_288MHZ                BITS(8:6,4)
    #define TOP_CKG_VP9_240MHZ                BITS(8:6,5)
    #define TOP_CKG_VP9_216MHZ                BITS(8:6,6)
    #define TOP_CKG_VP9_172MHZ                BITS(8:6,7)

#define REG_TOP_MVD             (CLKGEN0_REG_BASE+(0x0039<<1))
    #define TOP_CKG_MVD_MASK                  BMASK(3:0)
    #define TOP_CKG_MHVD_DIS                  BIT(0)
    #define TOP_CKG_MVD_INV                   BIT(1)
    #define TOP_CKG_MVD_CLK_MASK              BMASK(3:2)
    #define TOP_CKG_MVD_144MHZ                BITS(3:2, 0)
    #define TOP_CKG_MVD_123MHZ                BITS(3:2, 1)
    #define TOP_CKG_MVD_MIU                   BITS(3:2, 2)
    #define TOP_CKG_MVD_XTAL                  BITS(3:2, 3)

#define REG_TOP_MVD2             (CLKGEN0_REG_BASE+(0x0039<<1))
    #define TOP_CKG_MVD2_MASK                  BMASK(11:8)
    #define TOP_CKG_MHVD2_DIS                  BIT(8)
    #define TOP_CKG_MVD2_INV                   BIT(9)
    #define TOP_CKG_MVD2_CLK_MASK              BMASK(11:10)
    #define TOP_CKG_MVD2_170MHZ                BITS(11:10, 0)
    #define TOP_CKG_MVD2_144MHZ                BITS(11:10, 1)
    #define TOP_CKG_MVD2_160MHZ                BITS(11:10, 1)
    #define TOP_CKG_MVD2_CLK_MIU_P             BITS(11:10, 1)

#define REG_TOP_CKG_EVD_PPU             (CLKGEN0_REG_BASE+(0x0033<<1))
    #define TOP_CKG_EVD_PPU_MASK                BMASK(13:10)
    #define TOP_CKG_EVD_PPU_DIS                 BIT(8)
    #define TOP_CKG_EVD_PPU_INV                 BIT(9)
    #define TOP_CKG_EVD_PPU_PLL_BUF             BITS(13:10, 0)
    #define TOP_CKG_EVD_PPU_MIU128PLL           BITS(13:10, 1)
    #define TOP_CKG_EVD_PPU_MIU256PLL           BITS(13:10, 2)
    #define TOP_CKG_EVD_PPU_480MHZ              BITS(13:10, 3)
    #define TOP_CKG_EVD_PPU_384MHZ              BITS(13:10, 4)
    #define TOP_CKG_EVD_PPU_320MHZ              BITS(13:10, 5)
    #define TOP_CKG_EVD_PPU_240MHZ              BITS(13:10, 6)
    #define TOP_CKG_EVD_PPU_192MHZ              BITS(13:10, 7)

#define REG_TOP_CKG_EVD             (CLKGEN0_REG_BASE+(0x0034<<1))
    #define TOP_CKG_EVD_MASK                    BMASK(13:10)
    #define TOP_CKG_EVD_DIS                     BIT(8)
    #define TOP_CKG_EVD_INV                     BIT(9)
    #define TOP_CKG_EVD_PLL_BUF              BITS(13:10, 0)
    #define TOP_CKG_EVD_MIU128PLL               BITS(13:10, 1)
    #define TOP_CKG_EVD_MIU256PLL               BITS(13:10, 2)
    #define TOP_CKG_EVD_480MHZ                  BITS(13:10, 3)
    #define TOP_CKG_EVD_384MHZ                  BITS(13:10, 4)
    #define TOP_CKG_EVD_320MHZ                  BITS(13:10, 5)
    #define TOP_CKG_EVD_240MHZ                  BITS(13:10, 6)
    #define TOP_CKG_EVD_192MHZ                  BITS(13:10, 7)

#define REG_TOP_UART_SEL0             (CHIPTOP_REG_BASE+(0x0053<<1))
    #define REG_TOP_UART_SEL_0_MASK            BMASK(3:0)
    #define REG_TOP_UART_SEL_MHEG5             BITS(3:0, 1)
    #define REG_TOP_UART_SEL_VD_MHEG5          BITS(3:0, 2)
    #define REG_TOP_UART_SEL_TSP               BITS(3:0, 3)
    #define REG_TOP_UART_SEL_PIU_0             BITS(3:0, 4)
    #define REG_TOP_UART_SEL_PIU_1             BITS(3:0, 5)
    #define REG_TOP_UART_SEL_PIU_FAST          BITS(3:0, 7)
    #define REG_TOP_UART_SEL_VD_MCU_51_TXD0    BITS(3:0, 10)
    #define REG_TOP_UART_SEL_VD_MCU_51_TXD1    BITS(3:0, 11)

//------------------------------------------------------------------------------
// MIU Reg
//------------------------------------------------------------------------------
#define MIU0_REG_HVD_BASE             	(0x1200)
#define MIU0_REG_HVD_BASE2             	(0x61500)

#define MIU1_REG_HVD_BASE             	(0x0600)
#define MIU1_REG_HVD_BASE2             	(0x62200)

#define MIU2_REG_HVD_BASE             	(0x62000)
#define MIU2_REG_HVD_BASE2             	(0x62300)


#define MIU0_CLIENT_SELECT_GP4          (MIU0_REG_HVD_BASE + (0x007C<<1))
    #define MIU0_CLIENT_SELECT_GP4_HVD_MIF0   BIT(2)
    #define MIU0_CLIENT_SELECT_GP4_HVD_MIF1   BIT(3)
    #define MIU0_CLIENT_SELECT_GP4_HVD_MALI   BIT(4)

#define MIU2_CLIENT_SELECT_GP4          (MIU2_REG_HVD_BASE + (0x007C<<1))
    #define MIU2_CLIENT_SELECT_GP4_HVD_MIF0   BIT(2)
    #define MIU2_CLIENT_SELECT_GP4_HVD_MIF1   BIT(3)
    #define MIU2_CLIENT_SELECT_GP4_HVD_MALI   BIT(4)


//#define MIU2_REG_HVD_BASE             	(0x62000)
//#define MIU2_REG_HVD_BASE2             	(0x62300)



#define MIU0_REG_RQ0_MASK                 (MIU0_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU0_REG_RQ1_MASK                 (MIU0_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU0_REG_RQ2_MASK                 (MIU0_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU0_REG_RQ3_MASK                 (MIU0_REG_HVD_BASE+(( 0x0053)<<1))
#define MIU0_REG_RQ4_MASK                 (MIU0_REG_HVD_BASE2+(( 0x0003)<<1))
#define MIU0_REG_RQ5_MASK                 (MIU0_REG_HVD_BASE2+(( 0x0013)<<1))

#define MIU1_REG_RQ0_MASK                 (MIU1_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU1_REG_RQ1_MASK                 (MIU1_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU1_REG_RQ2_MASK                 (MIU1_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU1_REG_RQ3_MASK                 (MIU1_REG_HVD_BASE+(( 0x0053)<<1))
#define MIU1_REG_RQ4_MASK                 (MIU1_REG_HVD_BASE2+(( 0x0003)<<1))
#define MIU1_REG_RQ5_MASK                 (MIU1_REG_HVD_BASE2+(( 0x0013)<<1))

#define MIU2_REG_RQ0_MASK                 (MIU2_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU2_REG_RQ1_MASK                 (MIU2_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU2_REG_RQ2_MASK                 (MIU2_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU2_REG_RQ3_MASK                 (MIU2_REG_HVD_BASE+(( 0x0053)<<1))
#define MIU2_REG_RQ4_MASK                 (MIU2_REG_HVD_BASE2+(( 0x0003)<<1))
#define MIU2_REG_RQ5_MASK                 (MIU2_REG_HVD_BASE2+(( 0x0013)<<1))




#define MIU0_REG_SEL0                 (MIU0_REG_HVD_BASE+(( 0x0078)<<1))
#define MIU0_REG_SEL1                 (MIU0_REG_HVD_BASE+(( 0x0079)<<1))
#define MIU0_REG_SEL2                 (MIU0_REG_HVD_BASE+(( 0x007A)<<1))
#define MIU0_REG_SEL3                 (MIU0_REG_HVD_BASE+(( 0x007B)<<1))
#define MIU0_REG_SEL4                 (MIU0_REG_HVD_BASE+(( 0x007C)<<1))
#define MIU0_REG_SEL5                 (MIU0_REG_HVD_BASE+(( 0x007D)<<1))

#define MIU2_REG_SEL0                 (MIU2_REG_HVD_BASE+(( 0x0078)<<1))
#define MIU2_REG_SEL1                 (MIU2_REG_HVD_BASE+(( 0x0079)<<1))
#define MIU2_REG_SEL2                 (MIU2_REG_HVD_BASE+(( 0x007A)<<1))
#define MIU2_REG_SEL3                 (MIU2_REG_HVD_BASE+(( 0x007B)<<1))
#define MIU2_REG_SEL4                 (MIU2_REG_HVD_BASE+(( 0x007C)<<1))
#define MIU2_REG_SEL5                 (MIU2_REG_HVD_BASE+(( 0x007D)<<1))


//#define MIU1_REG_SEL0                 (MIU1_REG_HVD_BASE+(( 0x0078)<<1))


#define MIU_HVD_RW      (BIT(10)|BIT(11))
#define MIU_MVD_RW      (BIT(5)|BIT(6))

//------------------------------------------------------------------------------
// SRAM Reg
//------------------------------------------------------------------------------

#ifdef CONFIG_MSTAR_SRAMPD
#define REG_PATGEN_HI_BASE                      0x71300
#define REG_PATGEN_VP9_BASE                     0x71800

#define REG_HICODEC_SRAM_SD_EN              (REG_PATGEN_HI_BASE+(( 0x0010)<<1))
    #define HICODEC_SRAM_HICODEC0               BIT(0)
    #define HICODEC_SRAM_HICODEC1               BIT(1)

#define REG_HICODEC_LITE_SRAM_SD_EN         (REG_PATGEN_VP9_BASE+(( 0x0010)<<1))
    #define HICODEC_LITE_SRAM_HICODEC0          BIT(0)
    #define HICODEC_LITE_SRAM_HICODEC1          BIT(1)
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_HVD_H_

