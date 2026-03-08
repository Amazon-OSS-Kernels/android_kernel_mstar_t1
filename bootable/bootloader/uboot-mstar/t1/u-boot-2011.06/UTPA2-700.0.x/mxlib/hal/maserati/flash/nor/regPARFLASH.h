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
/// file    regPARFLASH.h
/// @brief  Parallel Flash Register Definition
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_PARFLASH_H_
#define _REG_PARFLASH_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// BASEADDR & BK
#define BASEADDR_RIU                    0xBF000000  // TODO: <-@@@ CHIP SPECIFIC

#define BK_CHIPTOP                      0x3C00
#define BK_PFSH                         0xB00
#define BK_PIU							0x7800

//-------- PIU register ------------------------
#define REG_PIU_SPI_CLK_SRC         0x26
	#define PSCS_CLK_SRC_SEL_MASK       	BMASK(5:5)
    #define PSCS_CLK_SRC_SEL_XTAL           BITS(5:5, 0)
    #define PSCS_CLK_SRC_SEL_CLK            BITS(5:5, 1)
	#define PSCS_CLK_SEL_MASK           	BMASK(4:2)
    #define PSCS_CLK_SEL_XTAL               BITS(4:2, 0)
    #define PSCS_CLK_SEL_27MHZ              BITS(4:2, 1)
    #define PSCS_CLK_SEL_36MHZ              BITS(4:2, 2)
    #define PSCS_CLK_SEL_43MHZ              BITS(4:2, 3)
    #define PSCS_CLK_SEL_54MHZ              BITS(4:2, 4)
    #define PSCS_CLK_SEL_72MHZ              BITS(4:2, 5)
    #define PSCS_CLK_SEL_86MHZ              BITS(4:2, 6)
    #define PSCS_CLK_SEL_108MHZ             BITS(4:2, 7)
    #define PSCS_CLK_SRC_SEL_MASK       	BMASK(5:5)
    #define PSCS_CLK_SRC_SEL_XTAL           BITS(5:5, 0)
    #define PSCS_CLK_SRC_SEL_CLK            BITS(5:5, 1)

//-------------------------------------------

//----- Chip top -------------------------
#define REG_ALLPAD_IN                   0x50
#define REG_PCMCONFIG                   0x6E
#define REG_PF_MODE                     0x6F
#define REG_PCMISGPIO					0x70

//---- Nor flash register ------------------
#define REG_NORPF_CLOCK_LENGTH          0x00
#define REG_NORPF_FLASH_DATA_WIDTH8     0x00
#define REG_NORPF_WRITE_RUN             0x01
#define REG_NORPF_DIRECT_READ_MODE      0x01
#define REG_NORPF_CEB_START             0x02
#define REG_NORPF_CEB_END               0x02
#define REG_NORPF_OEB_START             0x02
#define REG_NORPF_OEB_END               0x02
#define REG_NORPF_WEB_START             0x03
#define REG_NORPF_WEB_END               0x03
#define REG_NORPF_DATAOEN_START         0x03
#define REG_NORPF_DATAOEN_END           0x03
#define REG_NORPF_DATA_LATCH_CNT        0x04
#define REG_NORPF_CTRL                  0x06
#define REG_NORPF_BRIDGE_CTRL           0x07
#define REG_NORPF_XIU_CTRL              0x08
#define REG_NORPF_XIU_ADDR_L            0x09
#define REG_NORPF_XIU_ADDR_H            0x0A
#define REG_NORPF_XIU_WDATA             0x0B
#define REG_NORPF_XIU_STATUS            0x0C
#define REG_NORPF_XIU_RDATA_L           0x0D
#define REG_NORPF_XIU_RDATA_H           0x0E
#define REG_NORPF_WRITE_ADDR0_L         0x10
#define REG_NORPF_WRITE_ADDR0_H         0x11
#define REG_NORPF_WRITE_ADDR1_L         0x12
#define REG_NORPF_WRITE_ADDR1_H         0x13
#define REG_NORPF_WRITE_ADDR2_L         0x14
#define REG_NORPF_WRITE_ADDR2_H         0x15
#define REG_NORPF_WRITE_ADDR3_L         0x16
#define REG_NORPF_WRITE_ADDR3_H         0x17
#define REG_NORPF_WRITE_ADDR4_L         0x18
#define REG_NORPF_WRITE_ADDR4_H         0x19
#define REG_NORPF_WRITE_ADDR5_L         0x1A
#define REG_NORPF_WRITE_ADDR5_H         0x1B
#define REG_NORPF_WRITE_ADDR6_L         0x1C
#define REG_NORPF_WRITE_ADDR6_H         0x1D
#define REG_NORPF_WRITE_ADDR7_L         0x1E
#define REG_NORPF_WRITE_ADDR7_H         0x1F
#define REG_NORPF_WRITE_DATA0           0x20
#define REG_NORPF_WRITE_DATA1           0x21
#define REG_NORPF_WRITE_DATA2           0x22
#define REG_NORPF_WRITE_DATA3           0x23
#define REG_NORPF_WRITE_DATA4           0x24
#define REG_NORPF_WRITE_DATA5           0x25
#define REG_NORPF_WRITE_DATA6           0x26
#define REG_NORPF_WRITE_DATA7           0x27


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_PARFLASH_H_
