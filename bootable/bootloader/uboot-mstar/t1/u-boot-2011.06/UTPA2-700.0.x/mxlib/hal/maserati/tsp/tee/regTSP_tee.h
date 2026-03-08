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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regTSP_tee.h
//  Description: Transport Stream Processor (TSP) Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TSP_TEE_REG_H_
#define _TSP_TEE_REG_H_

//--------------------------------------------------------------------------------------------------
//  Abbreviation
//--------------------------------------------------------------------------------------------------
// Addr                             Address
// Buf                              Buffer
// Clr                              Clear
// CmdQ                             Command queue
// Cnt                              Count
// Ctrl                             Control
// Flt                              Filter
// Hw                               Hardware
// Int                              Interrupt
// Len                              Length
// Ovfw                             Overflow
// Pkt                              Packet
// Rec                              Record
// Recv                             Receive
// Rmn                              Remain
// Reg                              Register
// Req                              Request
// Rst                              Reset
// Scmb                             Scramble
// Sec                              Section
// Stat                             Status
// Sw                               Software
// Ts                               Transport Stream


//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------
#define MIU_BUS                                 4
#define TSP_VQ_PITCH                            208
#define TSP_VQ_NUM                              4    //VQ0, VQ_file, VQ1, VQ_2

#define TSP_QMEM_SIZES                          0x1000 // 16K bytes, 32bit aligment  //0x4000

//------------------------------------------------
// TS0 Bank
//------------------------------------------------
#define REG_TSP0_FW_DMA_ADDR_L                0x78
    #define TSP_FW_DMA_ADDR_MASK              0xFFFFFFUL
    #define TSP_DNLD_ADDR_ALI_SHIFT           4
#define REG_TSP1_FW_DMA_ADDR_H                0x0A
    #define TSP_FW_DMA_ADDR_H_MASK            0xFF
#define REG_TSP0_FW_DMA_NUM                   0x79

#define REG_TSP1_ONEWAY                       0x42
    #define TSP_FW_ONEWAY                     0x0008

#define REG_TSP0_PVR_HEAD1_L                  0x50
#define REG_TSP0_PVR_HEAD1_H                  0x51
#define REG_TSP0_PVR_MID1_L                   0x52
#define REG_TSP0_PVR_MID1_H                   0x53
#define REG_TSP0_PVR_TAIL1_L                  0x54
#define REG_TSP0_PVR_TAIL1_H                  0x55

#define REG_TSP0_PVR_HEAD2_L                  0x01
#define REG_TSP0_PVR_HEAD2_H                  0x02
#define REG_TSP0_PVR_MID2_L                   0x03
#define REG_TSP0_PVR_MID2_H                   0x04
#define REG_TSP0_PVR_TAIL2_L                  0x05
#define REG_TSP0_PVR_TAIL2_H                  0x06

#define REG_TSP0_PVR1_HEAD1_L                 0x12
#define REG_TSP0_PVR1_HEAD1_H                 0x13
#define REG_TSP0_PVR1_MID1_L                  0x14
#define REG_TSP0_PVR1_MID1_H                  0x15
#define REG_TSP0_PVR1_TAIL1_L                 0x16
#define REG_TSP0_PVR1_TAIL1_H                 0x17

#define REG_TSP0_PVR1_HEAD2_L                 0x18
#define REG_TSP0_PVR1_HEAD2_H                 0x19
#define REG_TSP0_PVR1_MID2_L                  0x1A
#define REG_TSP0_PVR1_MID2_H                  0x1B
#define REG_TSP0_PVR1_TAIL2_L                 0x1C
#define REG_TSP0_PVR1_TAIL2_H                 0x1D

#define REG_TSP0_FILE_ADDR_L                  0x3A
#define REG_TSP0_FILE_ADDR_H                  0x3B
#define REG_TSP0_FILE_SIZE_L                  0x3C
#define REG_TSP0_FILE_SIZE_H                  0x3D

//------------------------------------------------
// TS1 Bank
//------------------------------------------------
#define REG_TSP1_VQ0_BASE_L                   0x20
#define REG_TSP1_VQ0_BASE_H                   0x21
#define REG_TSP1_VQ0_SIZE                     0x22
#define REG_TSP1_VQ1_BASE_L                   0x56
#define REG_TSP1_VQ1_BASE_H                   0x57
#define REG_TSP1_VQ1_SIZE                     0x5C
#define REG_TSP1_VQ2_BASE_L                   0x5E
#define REG_TSP1_VQ2_BASE_H                   0x5F
#define REG_TSP1_VQ2_SIZE                     0x64
#define REG_TSP1_VQ3_BASE_L                   0x74
#define REG_TSP1_VQ3_BASE_H                   0x75
#define REG_TSP1_VQ3_SIZE                     0x76

//------------------------------------------------
// MMFI Bank
//------------------------------------------------
#define REG_MMFI_FILE_ADDR_L                  0x08
#define REG_MMFI_FILE_ADDR_H                  0x09
#define REG_MMFI_FILE_SIZE_L                  0x0A
#define REG_MMFI_FILE_SIZE_H                  0x0B

#define REG_MMFI1_FILE_ADDR_L                 0x28
#define REG_MMFI1_FILE_ADDR_H                 0x29
#define REG_MMFI1_FILE_SIZE_L                 0x2A
#define REG_MMFI1_FILE_SIZE_H                 0x2B

#endif // _TSP_TEE_REG_H_
