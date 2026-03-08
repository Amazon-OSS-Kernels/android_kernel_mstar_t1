/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
/*===========================================================
 * MStar Semiconductor Inc.
 *
 * eMMC Driver for FCIE4 - eMMC.h
 *
 * History
 *    - initial version, 2011.09.22, Hill.Sung
 *      please modify the eMMC_platform.h for your platform.
 *
 *
 *===========================================================*/
#ifndef eMMC_DRIVER_H
#define eMMC_DRIVER_H


//===========================================================
// debug macro
//===========================================================


//=====================================================================================
#include "../config/eMMC_config.h" // [CAUTION]: edit eMMC_config.h for your platform
//=====================================================================================
#include "eMMC_err_codes.h"


//===========================================================
// macro for Spec.
//===========================================================
#define ADDRESSING_MODE_BYTE      1 // 1 byte
#define ADDRESSING_MODE_SECTOR    2 // 512 bytes
#define ADDRESSING_MODE_4KB       3 // 4KB bytes

#define eMMC_SPEED_OLD            0
#define eMMC_SPEED_HIGH           1
#define eMMC_SPEED_HS200          2

#define eMMC_FLAG_TRIM            BIT0
#define eMMC_FLAG_HPI_CMD12       BIT1
#define eMMC_FLAG_HPI_CMD13       BIT2

//-------------------------------------------------------
// Devices has to be in 512B block length mode by default
// after power-on, or software reset.
//-------------------------------------------------------
#define eMMC_SECTOR_512BYTE       0x200
#define eMMC_SECTOR_512BYTE_BITS  9
#define eMMC_SECTOR_512BYTE_MASK  (eMMC_SECTOR_512BYTE-1)

#define eMMC_SECTOR_BUF_16KB      (eMMC_SECTOR_512BYTE * 0x20)

#define eMMC_SECTOR_BYTECNT       eMMC_SECTOR_512BYTE
#define eMMC_SECTOR_BYTECNT_BITS  eMMC_SECTOR_512BYTE_BITS
//-------------------------------------------------------

#define eMMC_ExtCSD_SetBit        1
#define eMMC_ExtCSD_ClrBit        2
#define eMMC_ExtCSD_WByte         3

#define eMMC_CMD_BYTE_CNT         5
#define eMMC_R1_BYTE_CNT          5
#define eMMC_R1b_BYTE_CNT         5
#define eMMC_R2_BYTE_CNT          16
#define eMMC_R3_BYTE_CNT          5
#define eMMC_R4_BYTE_CNT          5
#define eMMC_R5_BYTE_CNT          5
#define eMMC_MAX_RSP_BYTE_CNT     eMMC_R2_BYTE_CNT
#define eMMC_MIN_RSP_BYTE_CNT     6

//===========================================================
// driver structures
//===========================================================
#define FCIE_FLAG_GET_PART_INFO   BIT1
#define FCIE_FLAG_RSP_WAIT_D0H    BIT2 // currently only R1b
#define FCIE_FLAG_DDR_MODE        BIT3
#define FCIE_FLAG_DDR_TUNING      BIT4
#define FCIE_FLAG_SPEED_MASK      (BIT5|BIT6)
#define FCIE_FLAG_SPEED_HIGH      BIT5
#define FCIE_FLAG_SPEED_HS200     BIT6
#define FCIE_FLAG_TESTING         BIT7
#define FCIE_FLAG_PADTYPE_MASK    (BIT8|BIT9)
#define FCIE_FLAG_PADTYPE_DDR     BIT8
#define FCIE_FLAG_PADTYPE_SDR     BIT9
#define FCIE_FLAG_PADTYPE_BYPASS  (BIT8|BIT9)


typedef struct _eMMC_DRIVER
{
	// ----------------------------------------
    // FCIE
    // ----------------------------------------
	U16 u16_RCA;
	U32 u32_Flag;
	U8  au8_Rsp[eMMC_MIN_RSP_BYTE_CNT];
    U8  au8_CID[eMMC_MAX_RSP_BYTE_CNT];
	U16 u16_Reg10_Mode;

    // ----------------------------------------
    // eMMC
    // ----------------------------------------
	U8  u8_IfSectorMode;
	// ExtCSD
	U8  u8_BUS_WIDTH;
} eMMC_DRIVER, *P_eMMC_DRIVER;

typedef eMMC_PACK0 struct _eMMC_RPMB_DATA
{
   U16 u16_req_rsp;
   U16 u16_result;
   U16 u16_blk_cnt;
   U16 u16_addr;
   U32 u32_writecnt;
   U8  u8_nonce[16];
   U8  u8_data[256];
   U8  u8_auth_key[32];
   U8  u8_stuff[196];
}eMMC_PACK1 eMMC_RPMB_DATA;

#if 0
typedef eMMC_PACK0 struct _eMMC_TEST_ALIGN_PACK {

    U8  u8_0;
    U16 u16_0;
    U32 u32_0, u32_1;

} eMMC_PACK1 eMMC_TEST_ALIGN_PACK_t;
#endif

//===========================================================
// exposed APIs
//===========================================================
#include "../api/drv_eMMC.h"

//===========================================================
// internal used functions
//===========================================================
#include "eMMC_utl.h"
#include "eMMC_hal.h"

#define eMMC_RPMB_REQ_AUTH_KEY         0x01
#define eMMC_RPMB_REQ_WRITE_CNT_VAL    0x02
#define eMMC_RPMB_REQ_AUTH_DATA_W      0x03
#define eMMC_RPMB_REQ_AUTH_DATA_R      0x04
#define eMMC_RPMB_REQ_RESULT_R         0x05

#define CONFIG_PROGRAM_EMMC_RPMB_KEY 1

#endif // eMMC_DRIVER_H

