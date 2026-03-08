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
#ifndef __DRVNAND_UTL_H__
#define __DRVNAND_UTL_H__

#include "drvNAND.h"

extern U32 drvNAND_ProbeReadSeq(void);
extern U32 drvNAND_CheckPartInfo(U32 u32_PageIdx);
extern U32 drvNAND_SearchCIS(void);
extern U32 drvNAND_SearchCIS1(U8 *u8_CIS1Blk);
extern U32 drvNAND_InitBBT(INIT_BBT_t *pInitBBT);
extern void drvNAND_ParseNandInfo(NAND_FLASH_INFO_t *pNandInfo);


typedef UNFD_PACK0 struct _TEST_ALIGN_PACK {

	U8	u8_0;
	U16	u16_0;
	U32	u32_0, u32_1;

} UNFD_PACK1 TEST_ALIGN_PACK_t;
extern U32  drvNAND_CheckAlignPack(U8 u8_AlignByteCnt);
extern U32  nand_CompareData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt);
extern void dump_mem(unsigned char *buf, int cnt);
extern void dump_nand_info(NAND_FLASH_INFO_t *pNandInfo);
extern void dump_part_records(PARTITION_RECORD_t *pRecord, int cnt);
extern void dump_part_info(PARTITION_INFO_t *pPartInfo);
extern void dump_miu_records(MIU_RECORD_t *records, int cnt);
extern void dump_miu_part(MIU_PART_t *pMiuPart);
extern void dump_general_blk_info(BLK_INFO_t *pBlkInfo);
extern void dump_nand_driver(NAND_DRIVER *pNandDrv);
extern void dump_nand_PPM(void);

extern U32 drvNAND_CompareCISTag(U8 *tag);
extern U8  drvNAND_CountBits(U32 u32_x);
extern U32 drvNAND_CheckSum(U8 *pu8_Data, U32 u32_ByteCnt);

extern PARTITION_RECORD_t *drvNAND_SearchPartition(PARTITION_RECORD_t *pRecord,
												   U16 u16_PartType);

extern U32 drvNAND_ReadBlk(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
							U16 u16_LBA, U32 u32_StartSector, 
							U32 u32_SectorCnt);
extern U32 drvNAND_WriteBlk(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
							U16 u16_LBA, U32 u32_StartSector,
							U32 u32_SectorCnt);

extern U32 drvNAND_WriteBlkNonBackup(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA,
					 U16 u16_LBA,	U32 u32_StartSector,
					 U32 u32_SectorCnt);

extern U32 drvNAND_CheckSpareSpacePattern(U8 *au8_SpareBuf, U32 u32_PageIdx);
extern U32 drvNAND_IsGoodBlk(U16 u16_BlkIdx);
extern U32 drvNAND_IsFreeBlk(U16 u16_BlkIdx);
extern U8  drvNAND_CheckAll0xFF(U8* pu8_Buf, U32 u32_ByteCnt);
extern U16 drvNAND_FindFreePage(U16 u16_BlkIdx);
extern U16 drvNAND_FindFreeLSBPageIdx(U16 u16_BlkIdx);
extern U32 drvNAND_MoveBlkData(U16 u16_DestBlk, U16 u16_SrcBlk, U16 u16_PageCnt);

extern U32 drvNAND_MarkBadBlk(U16 u16_BlkIdx);
extern U32 drvNAND_ErasePhyBlk(U16 u16_BlkIdx);
extern U32 drvNAND_ReadPhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_WritePhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_LFSRReadPhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_LFSRWritePhyPage(U32 u32_PageIdx, U8 *pu8_Data, U8 *pu8_Spare);
extern U32 drvNAND_WriteDummyToMSBPage(U32 u32_PairPageIdx, U32 u32_BlkRow, U8* pu8_PageDataBuf, U8* pu8_SpareDataBuf, U32 (*nand_markbad)(U32));
extern int nand_CheckEmptyPageFalseAlarm(U8 *main, U8 *spare);
extern int nand_ReadDisturbance_BigImg(U32  u32_BlkRow, U8 u8_torture, U32 (*nand_markbad)(U32),  U8 u8_CustSpareMarker, U8 u8_CustMarkerIndex);
extern U32 nand_ReadDisturbance_BigImgRestore(U32 (*nand_markbad)(U32));

extern U32 drvNAND_WriteBlkByteOffset(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA, U16 u16_LBA, U32 u32_StartByte, U32 u32_ByteCnt);
extern U32 drvNAND_ReadBlkByteOffset(U8 *pu8_DestAddr, U8 u8_PartType, U16 u16_PBA, U16 u16_LBA, U32 u32_StartByte, U32 u32_ByteCnt);

extern U32 drvNAND_ParseAddrToADMA(U8 * pu8_DMADataBuf,  U32 u32_ByteCnt, U8 u8_Direction, ADMA_DESC_t* ADMADesc, U8 u8_IsEnd);

extern U32 drvNAND_WriteBlockECCByPass(U16 u16_PBA, U8* pu8_DataBuf, U16 u16_PageCnt, U16 u16_PhySpareByteCnt);
extern U16 drvNAND_BBT_Rev_StartBlk(void);
extern U16 drvNAND_GetBackupBlk(void);
/*
 * Return Zero Bit Count of input byte against 0xFF
 */
static inline int ZeroBitCountfor0xFFByte(U8 u8_Byte)
{
	int i;
	int sum = 0;
	if(u8_Byte ^ 0xFF)
	{
		for(i = 0; i < 8; i ++)
			sum += ((u8_Byte >> i)  & 0x1) ^ 1;
	}
	return sum;
}

#endif /* __DRVNAND_UTL_H__ */
