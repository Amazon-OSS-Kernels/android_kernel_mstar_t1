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

#ifndef _REG_COPRO_H_
#define _REG_COPRO_H_


////////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Base Address
//------------------------------------------------------------------------------
// For FRC-R2
#if defined MSOS_TYPE_LINUX
#define FRC_R2_REG_BASE                         0x400700UL    //0x400700 //frc_r2
#define FRC_R2_MAULV_REG_BASE                   0x400800UL    //0x400800 //sec_r2_maulv1
#define FRC_R2_MAULV_2_0_REG_BASE               0x400900UL    //0x400800 //sec_r2_maulv1
#define FRC_R2_MAULV_2_1_REG_BASE               0x400A00UL    //0x400800 //sec_r2_maulv1
#else
#define FRC_R2_REG_BASE                         0x000700UL    //0x400700 //frc_r2
#define FRC_R2_MAULV_REG_BASE                   0x000800UL    //0x400800 //sec_r2_maulv1
#define FRC_R2_MAULV_2_0_REG_BASE               0x000900UL    //0x400800 //sec_r2_maulv1
#define FRC_R2_MAULV_2_1_REG_BASE               0x000A00UL    //0x400800 //sec_r2_maulv1
#endif
// For Co-Processor
#define R2_REG_BASE                             0x022A00UL    //0x122A00 //sec_r2
#define R2_MAULV1_REG_BASE                      0x022B00UL    //0x122B00 //sec_r2_maulv1
#define R2_MAULV2_REG_BASE                      0x063A00UL    //0X163A00 //sec_r2_maulv2
// For Non-PM
#define NPM_REG_CLKGEN0                         0x000B00UL    //0x100b00 //clkgen0
#define NPM_REG_CLKGEN1                         0x003300UL    //0x103300 //clkgen1
#define NPM_REG_CHIPTOP                         0x001E00UL    //0x101E00 //chiptop
#define NPM_REG_MIU0                            0x001200UL    //0x101200 //miu0
#define NPM_REG_MIU1                            0x000600UL    //0x100600 //miu1
#define NPM_REG_ANAMISC                         0x010C00UL    //0x110c00 //ana misc
#define NPM_REG_MIPSPLL1                        0x011600UL    //0x111600 //ana misc
//For PM
#define PM_REG_BASE_SLEEP                       0x000E00UL    //0x002E00 //pm_sleep
#define PM_REG_BASE_MISC                        0x002E00UL    //0x002E00 //pm_misc

//------------------------------------------------------------------------------
// MCU and PIU Reg
//------------------------------------------------------------------------------

// For FRC-R2
//frcr2_integration###
#define FRC_R2_REG_STOP                         (FRC_R2_REG_BASE+0x0080UL)
#define FRC_R2_REG_SDR_LO_INST_BASE             (FRC_R2_REG_BASE+0x0082UL)
#define FRC_R2_REG_SDR_HI_INST_BASE             (FRC_R2_REG_BASE+0x0084UL)
#define FRC_R2_REG_SDR_LO_DATA_BASE             (FRC_R2_REG_BASE+0x0086UL)
#define FRC_R2_REG_SDR_HI_DATA_BASE             (FRC_R2_REG_BASE+0x0088UL)
#define FRC_R2_REG_RIU_BASE                     (FRC_R2_REG_BASE+0x008AUL)
#define FRC_R2_REG_RST_BASE                     (FRC_R2_REG_BASE+0x00B4UL)
#define FRC_R2_REG_IO1_BASE                     (FRC_R2_REG_BASE+0x00AAUL)
#define FRC_R2_REG_SPI_BASE                     (FRC_R2_REG_BASE+0x0090UL)
#define FRC_R2_REG_SPI_BASE1                    (FRC_R2_REG_BASE+0x00ACUL)
#define FRC_R2_REG_DQMEM_BASE                   (FRC_R2_REG_BASE+0x009CUL)
#define FRC_R2_REG_SPACE_EN                     (FRC_R2_REG_BASE+0x00B0UL)
#define FRC_R2_REG_QMEM_MASK_HIGH               (FRC_R2_REG_BASE+0x00A0UL)
#define FRC_R2_REG_QMEM_BASE_HIGH               (FRC_R2_REG_BASE+0x009CUL)

#define FRC_R2_MAULV_REG                        (FRC_R2_MAULV_REG_BASE + 0x0002UL) //0x400800
#define FRC_R2_MAULV_REG_2						(FRC_R2_MAULV_REG_BASE + 0x0070UL)
#define FRC_R2_MAULV_2_0_REG					(FRC_R2_MAULV_2_0_REG_BASE + 0x0002UL)
#define FRC_R2_MAULV_2_0_REG_2					(FRC_R2_MAULV_2_0_REG_BASE + 0x0070UL)
#define FRC_R2_MAULV_2_1_REG_2					(FRC_R2_MAULV_2_1_REG_BASE + 0x0070UL)

// For Co-Processor
#define R2_REG_STOP                             (R2_REG_BASE+0x0080UL)
#define R2_REG_SDR_LO_INST_BASE                 (R2_REG_BASE+0x0082UL)
#define R2_REG_SDR_HI_INST_BASE                 (R2_REG_BASE+0x0084UL)
#define R2_REG_SDR_LO_DATA_BASE                 (R2_REG_BASE+0x0086UL)
#define R2_REG_SDR_HI_DATA_BASE                 (R2_REG_BASE+0x0088UL)
#define R2_REG_RIU_BASE                         (R2_REG_BASE+0x008AUL)
#define R2_REG_RST_BASE                         (R2_REG_BASE+0x00B4UL)
#define R2_REG_IO1_BASE                         (R2_REG_BASE+0x00AAUL)
#define R2_REG_SPI_BASE                         (R2_REG_BASE+0x0090UL)
#define R2_REG_SPI_BASE1                        (R2_REG_BASE+0x00ACUL)
#define R2_REG_DQMEM_BASE                       (R2_REG_BASE+0x009CUL)
#define R2_REG_SPACE_EN                         (R2_REG_BASE+0x00B0UL)
#define R2_REG_QMEM_MASK_HIGH                   (R2_REG_BASE+0x00A0UL)
#define R2_REG_QMEM_BASE_HIGH                   (R2_REG_BASE+0x009CUL)

#define R2_MAULV1_REG                           (R2_MAULV1_REG_BASE + 0x0002UL)    //0x122B00 //sec_r2_maulv1
#define R2_MAULV2_REG                           (R2_MAULV2_REG_BASE + 0x0002UL)    //0X163A00 //sec_r2_maulv2

// For Non-PM
//frcr2_integration###
#define NPM_REG_CLKGEN1_FRCR2                   (NPM_REG_CLKGEN1+0x0060UL) //30*2 //0x1033_30[4:0]
#define NPM_REG_CLKGEN1_FRCMCU                  (NPM_REG_CLKGEN1+0x0061UL) //30*2 //0x1033_30[13:8]

#define NPM_REG_CLKGEN1_SECR2                   (NPM_REG_CLKGEN1+0x007BUL) //3D*2+1
#define NPM_REG_CHIPTOP_UART                    (NPM_REG_CHIPTOP+0x00A6UL)
#define NPM_REG_MIPS_PLLCLK                     (NPM_REG_ANAMISC+0x0026UL)

#define NPM_REG_LPF_LOW                         (NPM_REG_ANAMISC + 0x00C0UL)
#define NPM_REG_LPF_HIGH                        (NPM_REG_ANAMISC + 0x00C2UL)
#define NPM_REG_LPF_LOW_1                       (NPM_REG_MIPSPLL1 + 0x00C0UL)
#define NPM_REG_LPF_HIGH_1                      (NPM_REG_MIPSPLL1 + 0x00C2UL)

// For PM
#define PM_REG_CPUX_SW_RSTZ                     (PM_REG_BASE_MISC+0x0052UL)

#define NONE_CACHEABLE                          0x80000000UL

#endif // _REG_COPRO_H_



