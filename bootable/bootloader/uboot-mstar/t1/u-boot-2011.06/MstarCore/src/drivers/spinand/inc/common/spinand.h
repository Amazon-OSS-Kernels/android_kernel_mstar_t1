/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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

#ifndef _SPINAND_H_
#define _SPINAND_H_

#include "drvSPINAND.h"

#define USFD_LOGI_PART			0x8000

#define USFD_PART_HWCONFIG		1
#define USFD_PART_BOOTLOGO		2
#define USFD_PART_BL			3
#define USFD_PART_OS			4
#define USFD_PART_CUS			5
#define USFD_PART_UBOOT			6
#define USFD_PART_SECINFO		7
#define USFD_PART_OTP			8
#define USFD_PART_RECOVERY		9
#define USFD_PART_E2PBAK		10
#define USFD_PART_NVRAMBAK		11
#define USFD_PART_APANIC		12
#define USFD_PART_ENV			13
#define USFD_PART_MISC			14
#define USFD_PART_TBL			15
#define USFD_PART_CTRL			16
#define USFD_PART_FDD			17
#define USFD_PART_TDD			18
#define USFD_PART_UBIRO			19

typedef struct
{
	U16 u16_StartBlk;		// the start block index
	U16 u16_BlkCnt;			// project team defined
	U16 u16_PartType;		// project team defined
	U16 u16_BackupBlkCnt;	// reserved good blocks count for backup, UNFD internal use.
							// e.g. u16BackupBlkCnt  = u16BlkCnt * 0.03 + 2
} PARTITION_RECORD_t;

typedef struct
{
	U32 u32_ChkSum;
	U16	u16_SpareByteCnt;
	U16	u16_PageByteCnt;
	U16	u16_BlkPageCnt;
	U16	u16_BlkCnt;
	U16 u16_PartCnt;
	U16 u16_UnitByteCnt;
	PARTITION_RECORD_t records[62];

} PARTITION_INFO_t;

typedef struct _SPI_NAND_DRIVER
{
	SPINAND_FLASH_INFO_t tSpinandInfo;

	PARTITION_INFO_t tPartInfo;
	U8 u8_HasPNI;

	U8 *pu8_pagebuf;
	U8 *pu8_sparebuf;
	U32 u32_column;
	U8 u8_status;

	U8  u8_SectorByteCntBits;
	U8  u8_PageSectorCntBits;
	U8  u8_PageByteCntBits;
	U8  u8_BlkPageCntBits;
	U16 u16_PageSectorCnt;
} SPI_NAND_DRIVER_t;

typedef struct
{
	U8  au8_Tag[16];
	SPINAND_FLASH_INFO_t tSpiNandInfo;
	U8 au8_Reserved[];

} SPINAND_FLASH_INFO_TAG_t;

#define SPINAND_FLASH_INFO_TAG "MSTARSEMIUSFDCIS"

// this definition is used to measure nand read/write speed
#define NAND_SPEED_TEST                     0

extern void dump_mem(unsigned char *buf, int cnt);
#endif

