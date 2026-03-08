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
#ifndef __UNFD_CONFIG_H__
#define __UNFD_CONFIG_H__

//=====================================================
// select a HW platform:
//   - 1: enable, 0: disable.
//   - only one platform can be 1, others have to be 0.
//=====================================================
#define NAND_DRV_TV_MBOOT           1


//=====================================================
// do NOT edit the following content.
//=====================================================
#include <config.h>

#if defined(NAND_DRV_TV_MBOOT) && NAND_DRV_TV_MBOOT
  #include <config.h>

  #if (ENABLE_EAGLE)
  #include "drvNAND_eagle_uboot.h"
  #elif (ENABLE_EIFFEL)
  #include "drvNAND_eiffel_uboot.h"
  #elif (ENABLE_NIKE)
  #include "drvNAND_nike_uboot.h"
  #elif (ENABLE_MADISON)
  #include "drvNAND_madison_uboot.h"
  #elif (ENABLE_CLIPPERS)
  #include "drvNAND_clippers_uboot.h"
  #elif (ENABLE_MIAMI)
  #include "drvNAND_miami_uboot.h"
  #elif (ENABLE_NADAL)
  #include "drvNAND_nadal_uboot.h"
  #elif (ENABLE_NUGGET)
  #include "drvNAND_nugget_uboot.h"
  #elif (ENABLE_NIKON)
  #include "drvNAND_nikon_uboot.h"
  #elif (ENABLE_EINSTEIN)
  #include "drvNAND_einstein_uboot.h"
  #elif (ENABLE_AMBER3)
  #include "drvNAND_a3_uboot.h"
  #elif (ENABLE_AGATE)
  #include "drvNAND_agate_uboot.h"
  #elif (ENABLE_EDISON)
  #include "drvNAND_edison_uboot.h"
  #elif (ENABLE_AMBER5)
  #include "drvNAND_a5_uboot.h"
  #elif (ENABLE_AMETHYST)
  #include "drvNAND_amethyst_uboot.h"
  #elif (ENABLE_EMERALD)
  #include "drvNAND_emerald_uboot.h"
  #elif (ENABLE_KAISERIN)
  #include "drvNAND_kaiserin_uboot.h"
  #elif (ENABLE_KRONUS)
  #include "drvNAND_kronus_uboot.h"
  #elif (ENABLE_KENYA)
  #include "drvNAND_kenya_uboot.h"
  #elif (ENABLE_KAISER)
  #include "drvNAND_kaiser_uboot.h"
  #elif (ENABLE_KERES)
  #include "drvNAND_keres_uboot.h"
  #elif (ENABLE_KIRIN)
  #include "drvNAND_kirin_uboot.h"
  #elif (ENABLE_KRIS)
  #include "drvNAND_kris_uboot.h"
  #elif (ENABLE_KAYLA)
  #include "drvNAND_kayla_uboot.h"
  #elif (ENABLE_KRATOS)
  #include "drvNAND_kratos_uboot.h"
  #elif (ENABLE_KIWI)
  #include "drvNAND_kiwi_uboot.h"
  #elif (ENABLE_NAPOLI)
  #include "drvNAND_napoli_uboot.h"
  #elif (ENABLE_MONACO)
  #include "drvNAND_monaco_uboot.h"
  #elif (ENABLE_MONET)
  #include "drvNAND_monet_uboot.h"
   #elif (ENABLE_MESSI)
  #include "drvNAND_messi_uboot.h"
  #elif (ENABLE_MUJI)
  #include "drvNAND_muji_uboot.h"
  #elif (ENABLE_MILAN)
  #include "drvNAND_milan_uboot.h"
  #elif (ENABLE_MUNICH)
  #include "drvNAND_munich_uboot.h"
  #elif (ENABLE_MULAN)
  #include "drvNAND_mulan_uboot.h"
  #elif (ENABLE_MANHATTAN)
  #include "drvNAND_manhattan_uboot.h"
  #elif (ENABLE_MASERATI)
  #include "drvNAND_maserati_uboot.h"
  #elif (ENABLE_KANO)
  #include "drvNAND_kano_uboot.h"
  #elif (ENABLE_MACAN)
  #include "drvNAND_macan_uboot.h"
  #elif (ENABLE_CURRY)
  #include "drvNAND_curry_uboot.h"
  #elif (ENABLE_MAXIM)
  #include "drvNAND_maxim_uboot.h"
  #elif (ENABLE_MUSTANG)
  #include "drvNAND_mustang_uboot.h"
  #else
  #error "Wrong chip setting!"
  #endif
#else
  #error "Error! no platform selected."
#endif


//do NOT edit the following content, for HAL use.
//=====================================================
// misc. do NOT edit the following content.
//=====================================================
#define NAND_DMA_RACING_PATCH       1
#define NAND_DMA_PATCH_WAIT_TIME    10000 // us -> 10ms
#define NAND_DMA_RACING_PATTERN0    (((U32)'M'<<24)|((U32)0<<16)|((U32)'S'<<8)|(U32)1)
#define NAND_DMA_RACING_PATTERN1    (((U32)'T'<<24)|((U32)6<<16)|((U32)'A'<<8)|(U32)8)


//===========================================================
// Time Dalay, do NOT edit the following content, for NC_WaitComplete use.
//===========================================================
#define DELAY_100us_in_us           100
#define DELAY_500us_in_us           500
#define DELAY_1ms_in_us             1000
#define DELAY_10ms_in_us            10000
#define DELAY_100ms_in_us           100000
#define DELAY_500ms_in_us           500000
#define DELAY_1s_in_us              1000000

#define WAIT_ERASE_TIME             (DELAY_100ms_in_us)
#define WAIT_WRITE_TIME             (DELAY_100ms_in_us)
#define WAIT_READ_TIME              (DELAY_1ms_in_us<<1)  //DELAY_500us_in_us
#define WAIT_PROBE_TIME             (DELAY_100us_in_us)
#define WAIT_RESET_TIME             (DELAY_10ms_in_us)

//for system cache direction calls
#define WRITE_TO_NAND               0
#define READ_FROM_NAND              1
#define BIDIRECTIONAL			    2

extern void nand_reset_WatchDog(void);
extern U32  nand_clock_setting(U32 u32ClkParam);
extern U32  nand_config_clock(U16 u16_SeqAccessTime);
extern void nand_retry_dqs_post(void);
extern U32  nand_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt, int Mode);
extern U32  nand_translate_Spare_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt, int Mode);
extern U32  nand_translate_DMA_MIUSel(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void nand_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size);
extern U32  nand_WaitCompleteIntr(U16 u16_WaitEvent, U32 u32_MicroSec);
extern void nand_enable_intr_mode(void);
extern void nand_flush_miu_pipe(void);
extern void nand_check_blank_page_post_read(U8* main, U8* spare);
extern void nand_read_dma_post_flush(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern U32  nand_pads_release(void);

extern int  drvNAND_CHECK_FLASH_TYPE(void);
extern void CheckPowerCut(void);

extern void HWTimer_Start(void);
extern U32  HWTimer_End(void);

#define NAND_PARTITAION_BYTE_CNT    0x200

#endif /* __UNFD_CONFIG_H__ */
