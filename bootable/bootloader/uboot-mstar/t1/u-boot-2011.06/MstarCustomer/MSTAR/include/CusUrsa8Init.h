/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CUS_ENVIRONMENT_H__
#define __CUS_ENVIRONMENT_H__
#include <MsTypes.h>

#define UINSIDE "uinside"

#ifdef CONFIG_ANDROID_BOOT
#define CONFIG   "tvservice"
#else
#define CONFIG   ""
#endif

#define CONFIG_PATH   "/config"
#define CSOT_BIN_PATH "ukulele/MERGE_VB1.bin"
#define CMI_BIN_PATH "ukulele/MERGE_4CH_LVDS.bin"
#define FULLHD_BIN_PATH "ukulele/MERGE_LVDS.bin"
#define FULLHD_PANEL_NAME "FullHD_CMO216_H1L01"
#define CSOT_PANEL_NAME "FullHD_CMO216_H1L01_NIKEU_CSOT"
#define CMI_PANEL_NAME "FullHD_CMO216_H1L01_NIKEU_CMI"
#define Dram_Addr (CONFIG_UBOOT_LOADADDR-CONFIG_MIU0_BUSADDR)  //0x300000
#define HEADER_LEN 0xA080
#define UKULELE_AP_START_ADDR 0x10000 //AP addr in ursa miu
#define UKULELE_BIN_START_ADDR 0x100000 //bin file addr in ursa miu
#define UKULELE_BIN_FILE_ADDR_IN_NIKE_MIU (Dram_Addr+CONFIG_MIU0_BUSADDR+0xA00C) //0x2030A00C //bin file aaddr in nike miu, file header A00C~A00E is bin file addr,0x20000000 is nike VA,0x300000 is Dram_Addr, 0x20000000+0x300000+A00C=0x2030A00C
#define UKULELE_BIN_GROUP_ADDR_IN_NIKE_MIU (Dram_Addr+CONFIG_MIU0_BUSADDR+0xA018)//18~1B: BIN_GROUP_MEM_ADDR

#define RIU_RT1_CMD    0x11
#define RIU_W1_CMD     0x1D
#define RIU_R1T_CMD      0x1C
#define MIU_W_CMD      0x25
#define MIU_R_CMD   0x20
#define MIU_ST_CMD       0x21
#define SPI_DEVICE_BUFFER_SIZE           256

#define BIT0  0x000001
#define BIT1  0x000002
#define BIT2  0x000004
#define BIT3  0x000008
#define BIT4  0x000010
#define BIT5  0x000020
#define BIT6  0x000040
#define BIT7  0x000080
#define BIT8  0x000100
#define BIT9  0x000200
#define BIT10 0x000400
#define BIT11 0x000800
#define BIT12 0x001000
#define BIT13 0x002000
#define BIT14 0x004000
#define BIT15 0x008000
#define BIT16 0x010000


MS_BOOL MDrv_SS_RIU_Write8(MS_U16 u16Addr, MS_U8 data);
MS_BOOL MDrv_SS_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata);
MS_BOOL MDrv_SS_RIU_Write16(MS_U16 u16Addr, MS_U16 data);
MS_BOOL MDrv_SS_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
MS_BOOL MDrv_SS_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U32 u16Size);
MS_BOOL MDrv_SS_RIU_Write8_Mask(MS_U16 u16Addr, MS_U8 data, MS_U8 mask);

void InitialSystemPowerUp( void );
void MDrv_Macaw12_MiuInitial(void);
void MDrv_MIU_SimpleBist(void);



int do_cusUrsa8Init( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);



#endif

