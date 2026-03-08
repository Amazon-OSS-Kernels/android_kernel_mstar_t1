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
/// @file  regMVD.h
/// @brief Hardware register definition for Video Decoder
//
///////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_MVD_H_
#define _REG_MVD_H_


////////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Base Address
//------------------------------------------------------------------------------
#define MVD_REG_BASE                            0x1100UL  // 0x1100 - 0x11FF
#define CHIP_REG_BASE                           0x1E00UL  // 0x1E00 - 0x1EFF

#define MIU0_REG_BASE                           0x1200UL
#define MIU1_REG_BASE                           0x0600UL


//------------------------------------------------------------------------------
// MIU register
//------------------------------------------------------------------------------
//MIU request mask
#define MIU0_RQ0_MASK_L                 (MIU0_REG_BASE + 0x23*2)
#define MIU0_RQ0_MASK_H                 (MIU0_REG_BASE + 0x23*2 +1)
#define MIU0_RQ1_MASK_L                 (MIU0_REG_BASE + 0x33*2)
#define MIU0_RQ1_MASK_H                 (MIU0_REG_BASE + 0x33*2 +1)
#define MIU0_RQ2_MASK_L                 (MIU0_REG_BASE + 0x43*2)
#define MIU0_RQ2_MASK_H                 (MIU0_REG_BASE + 0x43*2 +1)
#define MIU0_RQ3_MASK_L                 (MIU0_REG_BASE + 0x53*2)
#define MIU0_RQ3_MASK_H                 (MIU0_REG_BASE + 0x53*2 +1)
#define MIU0_SEL0_L                     (MIU0_REG_BASE + 0xF0)
#define MIU0_SEL0_H                     (MIU0_REG_BASE + 0xF1)
#define MIU0_SEL2_L                     (MIU0_REG_BASE + 0xF4)
#define MIU0_SEL2_H                     (MIU0_REG_BASE + 0xF5)
#define MIU0_SEL3_L                     (MIU0_REG_BASE + 0xF6)
#define MIU0_SEL3_H                     (MIU0_REG_BASE + 0xF7)

#define MIU1_RQ0_MASK_L                 (MIU1_REG_BASE + 0x23*2)
#define MIU1_RQ0_MASK_H                 (MIU1_REG_BASE + 0x23*2 +1)
#define MIU1_RQ1_MASK_L                 (MIU1_REG_BASE + 0x33*2)
#define MIU1_RQ1_MASK_H                 (MIU1_REG_BASE + 0x33*2 +1)
#define MIU1_RQ2_MASK_L                 (MIU1_REG_BASE + 0x43*2)
#define MIU1_RQ2_MASK_H                 (MIU1_REG_BASE + 0x43*2 +1)
#define MIU1_RQ3_MASK_L                 (MIU1_REG_BASE + 0x53*2)
#define MIU1_RQ3_MASK_H                 (MIU1_REG_BASE + 0x53*2 +1)


//------------------------------------------------------------------------------
// MVD Reg
//------------------------------------------------------------------------------
    #define MVD_CTRL_RST                        BIT0//1: reset MVD; 0: release reset
    #define MVD_CTRL_CLR_INT                    BIT2//Clear MVD interrupt.
    #define MVD_CTRL_CLK_SYNCMODE               BIT4//1: sync_mode; 0: async_mode
    #define MVD_CTRL_CLK_ALLON                  BIT5//1: enable all clocks in mvd
    #define MVD_CTRL_DISCONNECT_MIU             BIT6//1: disconnect; 0: release reset
#define MVD_CTRL                                (MVD_REG_BASE + 0x00)

    #define MVD_STATUS_READY                    BIT1
#define MVD_STATUS                              (MVD_REG_BASE + 0x01)
    #define MVD_T8_MIU_128_0                   BIT2  // enable MVD to 128 bit mode
    #define MVD_T8_MIU_128_1                   BIT3  // enable MVD to 128 bit mode
#define MVD_COMMAND                             (MVD_REG_BASE + 0x02)
#define MVD_ARG0                                (MVD_REG_BASE + 0x04)
#define MVD_ARG1                                (MVD_REG_BASE + 0x05)
#define MVD_ARG2                                (MVD_REG_BASE + 0x06)
#define MVD_ARG3                                (MVD_REG_BASE + 0x07)
#define MVD_ARG4                                (MVD_REG_BASE + 0x08)
#define MVD_ARG5                                (MVD_REG_BASE + 0x09)

    #define MVD_SLQCTRL_WADR_RELOAD             BIT0 //reload "slq_wadr" into write address
                                                //w reload: program 1, then program 0, and reload complete
    #define MVD_SLQCTRL_RADR_PROBE              BIT1 //SLQ read address probe
    #define MVD_SLQCTRL_WADR_PROBE              BIT2 //SLQ write address probe
                                                //r/w probe: program 1, then program 0, and read "slq_caddr"
#define MVD_SLQCTRL                             (MVD_REG_BASE + 0x16)

//SLQ write address value[24:0]
#define MVD_SLQ_WADR0                           (MVD_REG_BASE + 0x18)
#define MVD_SLQ_WADR1                           (MVD_REG_BASE + 0x19)
#define MVD_SLQ_WADR2                           (MVD_REG_BASE + 0x1A)
#define MVD_SLQ_WADR3                           (MVD_REG_BASE + 0x1B)

//SLQ probe address value[24:0]
#define MVD_SLQ_CADR0                           (MVD_REG_BASE + 0x1C)
#define MVD_SLQ_CADR1                           (MVD_REG_BASE + 0x1D)
#define MVD_SLQ_CADR2                           (MVD_REG_BASE + 0x1E)
#define MVD_SLQ_CADR3                           (MVD_REG_BASE + 0x1F)

//CRC in/out
#define MVD_CRC_CTL                             (MVD_REG_BASE + 0x23)
    #define MVD_CRC_CTL_FIRE                    BIT6
    #define MVD_CRC_CTL_DONE                    BIT7
#define MVD_CRC_HSIZE                           (MVD_REG_BASE + 0x22)   //CRC hsize[13:4]
#define MVD_CRC_VSIZE                           (MVD_REG_BASE + 0x24)   //CRC vsize[13:0]
#define MVD_CRC_STRIP                           (MVD_REG_BASE + 0x26)   //CRC strip[13:0]
#define MVD_CRC_Y_START                         (MVD_REG_BASE + 0x28)   //CRC y start address[25:0]
    #define MVD_CRC_Y_START_LEN                 BMASK(25:0)
#define MVD_CRC_UV_START                        (MVD_REG_BASE + 0x2C)   //CRC uv start address[25:0]
    #define MVD_CRC_UV_START_LEN                BMASK(25:0)
#define MVD_CRC_Y_L                             (MVD_REG_BASE + 0x30)
#define MVD_CRC_Y_H                             (MVD_REG_BASE + 0x32)
#define MVD_CRC_UV_L                            (MVD_REG_BASE + 0x34)
#define MVD_CRC_UV_H                            (MVD_REG_BASE + 0x36)

#define REG_CHIPTOP_BASE        0x0b00UL

#define REG_CKG_MVD_SYNC        (REG_CHIPTOP_BASE + 0x38*2 +1)
    #define CKG_MVD_SYNC_GATED      BIT0

#define REG_CKG_MVD             (REG_CHIPTOP_BASE + 0x39*2)
    #define CKG_MVD_GATED           BIT0
    #define CKG_MVD_INVERT          BIT1
    #define CKG_MVD_MASK            (BIT4 | BIT3 | BIT2)
    #define CKG_MVD_216MHZ          (0 << 2)
    #define CKG_MVD_192MHZ          (1 << 2)
    #define CKG_MVD_172MHZ          (2 << 2)
    #define CKG_MVD_144MHZ          (3 << 2)
    #define CKG_MVD_CLK_MIU         (4 << 2)    //clk_miu_p
    #define CKG_MVD_123MHZ          (5 << 2)
    #define CKG_MVD_MPLL_DIV2       (6 << 2)    //mempll_clk_buf_div2
    #define CKG_MVD_XTAL_CLK        (7 << 2)    //XTAL clock

#define REG_CKG_MVD2            (REG_CHIPTOP_BASE + 0x39*2 +1)
    #define CKG_MVD2_GATED           BIT0
    #define CKG_MVD2_INVERT          BIT1
    #define CKG_MVD2_MASK            (BIT4 | BIT3 | BIT2)
    #define CKG_MVD2_216MHZ          (0 << 2)
    #define CKG_MVD2_192MHZ          (1 << 2)
    #define CKG_MVD2_172MHZ          (2 << 2)
    #define CKG_MVD2_144MHZ          (3 << 2)
    #define CKG_MVD2_CLK_MIU         (4 << 2)   //clk_miu_p
    #define CKG_MVD2_123MHZ          (5 << 2)
    #define CKG_MVD2_MPLL_DIV2       (6 << 2)    //mempll_clk_buf_div2
    #define CKG_MVD2_XTAL_CLK        (7 << 2)     //XTAL clock

#define REG_CKG_MVD_CHROMA_A      (REG_CHIPTOP_BASE + 0x3d*2)
    #define CKG_MVD_CHROMA_A_GATED           BIT0
    #define CKG_MVD_CHROMA_A_INVERT          BIT1

#define REG_CKG_MVD_CHROMA_B      (REG_CHIPTOP_BASE + 0x3d*2)
    #define CKG_MVD_CHROMA_B_GATED           BIT4
    #define CKG_MVD_CHROMA_B_INVERT          BIT5

#define REG_CKG_MVD_CHROMA_C      (REG_CHIPTOP_BASE + 0x3d*2 + 1)
    #define CKG_MVD_CHROMA_C_GATED           BIT0
    #define CKG_MVD_CHROMA_C_INVERT          BIT1

#define REG_CKG_MVD_LUMA_A      (REG_CHIPTOP_BASE + 0x3a*2 + 1)
    #define CKG_MVD_LUMA_A_GATED           BIT0
    #define CKG_MVD_LUMA_A_INVERT          BIT1

#define REG_CKG_MVD_LUMA_B      (REG_CHIPTOP_BASE + 0x3b*2)
    #define CKG_MVD_LUMA_B_GATED           BIT0
    #define CKG_MVD_LUMA_B_INVERT          BIT1

#define REG_CKG_MVD_LUMA_C      (REG_CHIPTOP_BASE + 0x3b*2 + 1)
    #define CKG_MVD_LUMA_C_GATED           BIT0
    #define CKG_MVD_LUMA_C_INVERT          BIT1


#define REG_CKG_MVD_RMEM        (REG_CHIPTOP_BASE + 0x3c*2)
    #define CKG_MVD_RMEM_GATED           BIT0
    #define CKG_MVD_RMEM_INVERT          BIT1

#define REG_CKG_MVD_RMEM1       (REG_CHIPTOP_BASE + 0x3c*2 + 1)
    #define CKG_MVD_RMEM1_GATED           BIT0
    #define CKG_MVD_RMEM1_INVERT          BIT1

#define REG_CKG_MVD_RREFDAT       (REG_CHIPTOP_BASE + 0x3e*2)
    #define CKG_MVD_RREFDAT_GATED           BIT0
    #define CKG_MVD_RREFDAT_INVERT          BIT1

#define REG_CKG_VD_AEON        (REG_CHIPTOP_BASE + 0x30*2)
    #define CKG_VD_AEON_GATED             BIT0
    #define CKG_VD_AEON_INVERT            BIT1
    #define CKG_VD_AEON_MASK            (BIT6 | BIT5 | BIT4 | BIT3 | BIT2)
    #define CKG_VD_AEON_160MHZ          (0 << 2)
                //Notice: The clock 160M comes from UTMI.
                //Please start UTMI's clock before you switch to 160M
    #define CKG_VD_AEON_144MHZ          (1 << 2)
    #define CKG_VD_AEON_123MHZ          (2 << 2)
    #define CKG_VD_AEON_108MHZ          (3 << 2)
    #define CKG_VD_AEON_96MHZ           (4 << 2)
    #define CKG_VD_AEON_72MHZ           (5 << 2)
    #define CKG_VD_AEON_DISABLE0        (6 << 2)    //disable
    #define CKG_VD_AEON_DISABLE1        (7 << 2)    //disable
    #define CKG_VD_AEON_CLK_MCU         (1 << 5)    //01xxx
    #define CKG_VD_AEON_CLK_MIU         (2 << 5)    //10xxx
    #define CKG_VD_AEON_XTAL            (3 << 5)    //11xxx


#define REG_CHIP_ID_MAJOR                       (CHIP_REG_BASE + 0xCC)
#define REG_CHIP_ID_MINOR                       (CHIP_REG_BASE + 0xCD)
#define REG_CHIP_VERSION                        (CHIP_REG_BASE + 0xCE)
#define REG_CHIP_REVISION                       (CHIP_REG_BASE + 0xCF)

#endif // _REG_MVD_H_

