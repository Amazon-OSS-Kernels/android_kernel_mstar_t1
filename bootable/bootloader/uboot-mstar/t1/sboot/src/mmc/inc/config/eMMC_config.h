/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef __eMMC_CONFIG_H__
#define __eMMC_CONFIG_H__

//=====================================================
// select a HW platform:
//   - 1: enable, 0: disable.
//   - only one platform can be 1, others have to be 0.
//   - search and check all [FIXME] if need modify or not
//=====================================================
//#include <config.h>

#define UNIFIED_eMMC_DRIVER         1

#ifdef CONFIG_MSTAR_EAGLE
#define eMMC_DRV_EAGLE_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_EDISON
#define eMMC_DRV_EDISON_BOOTRAM     1
#endif

#ifdef CONFIG_MSTAR_EIFFEL
#define eMMC_DRV_EIFFEL_BOOTRAM     1
#endif

#ifdef CONFIG_MSTAR_NIKE
#define eMMC_DRV_NIKE_BOOTRAM       1
#endif

#ifdef CONFIG_MSTAR_MADISON
#define eMMC_DRV_MADISON_BOOTRAM    1
#endif

#ifdef CONFIG_MSTAR_CLIPPERS
#define eMMC_DRV_CLIPPERS_BOOTRAM   1
#endif

#ifdef CONFIG_MSTAR_MIAMI
#define eMMC_DRV_MIAMI_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MONACO
#define eMMC_DRV_MONACO_BOOTRAM     1
#endif

#ifdef CONFIG_MSTAR_MUJI
#define eMMC_DRV_MUJI_BOOTRAM       1
#endif

#ifdef CONFIG_MSTAR_CELTICS
#define eMMC_DRV_CELTICS_BOOTRAM    1
#endif

#ifdef CONFIG_MSTAR_NADAL
#define eMMC_DRV_NADAL_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_EINSTEIN
#define eMMC_DRV_EINSTEIN_BOOTRAM   1
#endif

#ifdef CONFIG_MSTAR_EINSTEIN3
#define eMMC_DRV_EINSTEIN3_BOOTRAM  1
#endif

#ifdef CONFIG_MSTAR_NAPOLI
#define eMMC_DRV_NAPOLI_BOOTRAM     1
#endif

#ifdef CONFIG_MSTAR_MONET
#define eMMC_DRV_MONET_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MESSI
#define eMMC_DRV_MESSI_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MUNICH
#define eMMC_DRV_MUNICH_BOOTRAM      1
#endif

#ifdef CONFIG_MSTAR_MANHATTAN
#define eMMC_DRV_MANHATTAN_BOOTRAM       1
#endif

#ifdef CONFIG_MSTAR_MASERATI
#define eMMC_DRV_MASERATI_BOOTRAM       1
#endif

#ifdef CONFIG_MSTAR_MACAN
#define eMMC_DRV_MACAN_BOOTRAM       1
#endif

#ifdef CONFIG_MSTAR_MUSTANG
#define eMMC_DRV_MUSTANG_BOOTRAM       1
#endif

//=====================================================
// do NOT edit the following content.
//=====================================================
#if defined(eMMC_DRV_EAGLE_BOOTRAM) && eMMC_DRV_EAGLE_BOOTRAM
  #include "eMMC_eagle_bootram.h"
#elif defined(eMMC_DRV_EDISON_BOOTRAM) && eMMC_DRV_EDISON_BOOTRAM
  #include "eMMC_edison_bootram.h"
#elif defined(eMMC_DRV_EIFFEL_BOOTRAM) && eMMC_DRV_EIFFEL_BOOTRAM
  #include "eMMC_eiffel_bootram.h"
#elif defined(eMMC_DRV_NIKE_BOOTRAM) && eMMC_DRV_NIKE_BOOTRAM
  #include "eMMC_nike_bootram.h"
#elif defined(eMMC_DRV_MADISON_BOOTRAM) && eMMC_DRV_MADISON_BOOTRAM
  #include "eMMC_madison_bootram.h"
#elif defined(eMMC_DRV_CLIPPERS_BOOTRAM) && eMMC_DRV_CLIPPERS_BOOTRAM
  #include "eMMC_clippers_bootram.h"
#elif defined(eMMC_DRV_MIAMI_BOOTRAM) && eMMC_DRV_MIAMI_BOOTRAM
  #include "eMMC_miami_bootram.h"
#elif defined(eMMC_DRV_MONACO_BOOTRAM) && eMMC_DRV_MONACO_BOOTRAM
  #include "eMMC_monaco_bootram.h"
#elif defined(eMMC_DRV_MUJI_BOOTRAM) && eMMC_DRV_MUJI_BOOTRAM
  #include "eMMC_muji_bootram.h"
#elif defined(eMMC_DRV_CELTICS_BOOTRAM) && eMMC_DRV_CELTICS_BOOTRAM
  #include "eMMC_celtics_bootram.h"
#elif defined(eMMC_DRV_NADAL_BOOTRAM) && eMMC_DRV_NADAL_BOOTRAM
    #include "eMMC_nadal_bootram.h"
#elif defined(eMMC_DRV_EINSTEIN_BOOTRAM) && eMMC_DRV_EINSTEIN_BOOTRAM
  #include "eMMC_einstein_bootram.h"
#elif defined(eMMC_DRV_EINSTEIN3_BOOTRAM) && eMMC_DRV_EINSTEIN3_BOOTRAM
  #include "eMMC_einstein3_bootram.h"
#elif defined(eMMC_DRV_NAPOLI_BOOTRAM) && eMMC_DRV_NAPOLI_BOOTRAM
  #include "eMMC_napoli_bootram.h"
#elif defined(eMMC_DRV_MONET_BOOTRAM) && eMMC_DRV_MONET_BOOTRAM
  #include "eMMC_monet_bootram.h"
#elif defined(eMMC_DRV_MESSI_BOOTRAM) && eMMC_DRV_MESSI_BOOTRAM
  #include "eMMC_messi_bootram.h"
#elif defined(eMMC_DRV_MUNICH_BOOTRAM) && eMMC_DRV_MUNICH_BOOTRAM
  #include "eMMC_munich_bootram.h"
#elif defined(eMMC_DRV_MANHATTAN_BOOTRAM) && eMMC_DRV_MANHATTAN_BOOTRAM
  #include "eMMC_manhattan_bootram.h"
#elif defined(eMMC_DRV_MASERATI_BOOTRAM) && eMMC_DRV_MASERATI_BOOTRAM
  #include "eMMC_maserati_bootram.h"
#elif defined(eMMC_DRV_MACAN_BOOTRAM) && eMMC_DRV_MACAN_BOOTRAM
  #include "eMMC_macan_bootram.h"
#elif defined(eMMC_DRV_MUSTANG_BOOTRAM) && eMMC_DRV_MUSTANG_BOOTRAM
  #include "eMMC_mustang_bootram.h"
#else
  #error "Error! no platform selected."
#endif


//=====================================================
// misc. do NOT edit the following content.
//=====================================================
#define eMMC_DMA_RACING_PATCH        1
#define eMMC_DMA_PATCH_WAIT_TIME     DELAY_10ms_in_us
#define eMMC_DMA_RACING_PATTERN0     (((U32)'M'<<24)|((U32)0<<16)|((U32)'S'<<8)|(U32)1)
#define eMMC_DMA_RACING_PATTERN1     (((U32)'T'<<24)|((U32)6<<16)|((U32)'A'<<8)|(U32)8)

//===========================================================
// Time Dalay, do NOT edit the following content
//===========================================================
#if defined(eMMC_UPDATE_FIRMWARE) && (eMMC_UPDATE_FIRMWARE)
#define TIME_WAIT_DAT0_HIGH            (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RESET           (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT  (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FIFOCLK_RDY          (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_CMDRSP_END           (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_1_BLK_END            (HW_TIMER_DELAY_1s*5)
#define TIME_WAIT_n_BLK_END            (HW_TIMER_DELAY_1s*10) // safe for 512 blocks
#else
#define TIME_WAIT_DAT0_HIGH            HW_TIMER_DELAY_1s
#define TIME_WAIT_ERASE_DAT0_HIGH      (HW_TIMER_DELAY_1s*60*10) //10*60 s
#define TIME_WAIT_FCIE_RESET           HW_TIMER_DELAY_10ms
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT  HW_TIMER_DELAY_1us
#define TIME_WAIT_FIFOCLK_RDY          HW_TIMER_DELAY_10ms
#define TIME_WAIT_CMDRSP_END           HW_TIMER_DELAY_10ms
#define TIME_WAIT_1_BLK_END            HW_TIMER_DELAY_1s
#define TIME_WAIT_n_BLK_END            HW_TIMER_DELAY_1s // safe for 512 blocks
#endif




#endif /* __eMMC_CONFIG_H__ */
