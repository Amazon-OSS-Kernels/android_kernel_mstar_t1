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
#ifndef __eMMC_CONFIG_H__
#define __eMMC_CONFIG_H__

#define UNIFIED_eMMC_DRIVER     1
#include <common.h>
//=====================================================
// select a HW platform:
//   - 1: enable, 0: disable.
//   - only one platform can be 1, others have to be 0.
//   - search and check all [FIXME] if need modify or not
//=====================================================

#ifndef U64
#define U64  unsigned long long
#endif
#ifndef U32
#define U32  unsigned int
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S64
#define S64  signed long long
#endif
#ifndef S32
#define S32  signed long
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif


#if ENABLE_AMBER3
#define eMMC_DRV_A3_UBOOT               1
#elif ENABLE_AGATE
#define eMMC_DRV_AGATE_UBOOT            1
#elif ENABLE_EAGLE
#define eMMC_DRV_EAGLE_UBOOT            1
#elif ENABLE_NIKE
#define eMMC_DRV_NIKE_UBOOT             1
#elif ENABLE_MADISON
#define eMMC_DRV_MADISON_UBOOT          1
#elif ENABLE_CLIPPERS
#define eMMC_DRV_CLIPPERS_UBOOT          1
#elif ENABLE_MIAMI
#define eMMC_DRV_MIAMI_UBOOT            1
#elif ENABLE_NADAL
#define eMMC_DRV_NADAL_UBOOT            1
#elif ENABLE_EDISON
#define eMMC_DRV_EDISON_UBOOT           1
#elif ENABLE_EIFFEL
#define eMMC_DRV_EIFFEL_UBOOT           1
#elif ENABLE_AMBER5
#define eMMC_DRV_A5_UBOOT               1
#elif ENABLE_EINSTEIN
#define eMMC_DRV_EINSTEIN_UBOOT         1
#elif ENABLE_NAPOLI
#define eMMC_DRV_NAPOLI_UBOOT           1
#elif ENABLE_MONACO
#define eMMC_DRV_MONACO_UBOOT           1
#elif ENABLE_MUJI
#define eMMC_DRV_MUJI_UBOOT             1
#elif ENABLE_MANHATTAN
#define eMMC_DRV_MANHATTAN_UBOOT        1
#elif ENABLE_MASERATI
#define eMMC_DRV_MASERATI_UBOOT        1
#elif ENABLE_CELTICS
#define eMMC_DRV_CELTICS_UBOOT          1
#elif ENABLE_KAISER
#define eMMC_DRV_KAISER_UBOOT           1
#elif ENABLE_KERES
#define eMMC_DRV_KERES_UBOOT            1
#elif ENABLE_KIRIN
#define eMMC_DRV_KIRIN_UBOOT            1
#elif ENABLE_MONET
#define eMMC_DRV_MONET_UBOOT            1
#elif ENABLE_MESSI
#define eMMC_DRV_MESSI_UBOOT            1
#elif ENABLE_MUNICH
#define eMMC_DRV_MUNICH_UBOOT           1
#elif ENABLE_KRIS
#define eMMC_DRV_KRIS_UBOOT             1
#elif ENABLE_KANO
#define eMMC_DRV_KANO_UBOOT             1
#elif ENABLE_MACAN
#define eMMC_DRV_MACAN_UBOOT            1
#elif ENABLE_CURRY
#define eMMC_DRV_CURRY_UBOOT            1
#elif ENABLE_KAYLA
#define eMMC_DRV_KAYLA_UBOOT            1
#elif ENABLE_KASTOR
#define eMMC_DRV_KASTOR_UBOOT           1
#elif ENABLE_MAXIM
#define eMMC_DRV_MAXIM_UBOOT            1
#elif ENABLE_MUSTANG
#define eMMC_DRV_MUSTANG_UBOOT            1
#endif


//=====================================================
// do NOT edit the following content.
//=====================================================
#if defined(eMMC_DRV_A3_UBOOT) && eMMC_DRV_A3_UBOOT
  #include "eMMC_a3_uboot.h"
#elif defined(eMMC_DRV_AGATE_UBOOT) && eMMC_DRV_AGATE_UBOOT
  #include "eMMC_agate_uboot.h"
#elif defined(eMMC_DRV_EAGLE_UBOOT) && eMMC_DRV_EAGLE_UBOOT
  #include "eMMC_eagle_uboot.h"
#elif defined(eMMC_DRV_EDISON_UBOOT) && eMMC_DRV_EDISON_UBOOT
  #include "eMMC_edison_uboot.h"
#elif defined(eMMC_DRV_NIKE_UBOOT) && eMMC_DRV_NIKE_UBOOT
  #include "eMMC_nike_uboot.h"
#elif defined(eMMC_DRV_MADISON_UBOOT) && eMMC_DRV_MADISON_UBOOT
  #include "eMMC_madison_uboot.h"
#elif defined(eMMC_DRV_CLIPPERS_UBOOT) && eMMC_DRV_CLIPPERS_UBOOT
  #include "eMMC_clippers_uboot.h"
#elif defined(eMMC_DRV_MIAMI_UBOOT) && eMMC_DRV_MIAMI_UBOOT
  #include "eMMC_miami_uboot.h"
#elif defined(eMMC_DRV_NADAL_UBOOT) && eMMC_DRV_NADAL_UBOOT
    #include "eMMC_nadal_uboot.h"
#elif defined(eMMC_DRV_EIFFEL_UBOOT) && eMMC_DRV_EIFFEL_UBOOT
  #include "eMMC_eiffel_uboot.h"
#elif defined(eMMC_DRV_A5_UBOOT) && eMMC_DRV_A5_UBOOT
  #include "eMMC_a5_uboot.h"
#elif defined(eMMC_DRV_EINSTEIN_UBOOT) && eMMC_DRV_EINSTEIN_UBOOT
  #include "eMMC_einstein_uboot.h"
#elif defined(eMMC_DRV_NAPOLI_UBOOT) && eMMC_DRV_NAPOLI_UBOOT
  #include "eMMC_napoli_uboot.h"
#elif defined(eMMC_DRV_MONACO_UBOOT) && eMMC_DRV_MONACO_UBOOT
  #include "eMMC_monaco_uboot.h"
#elif defined(eMMC_DRV_MUJI_UBOOT) && eMMC_DRV_MUJI_UBOOT
  #include "eMMC_muji_uboot.h"
#elif defined(eMMC_DRV_MANHATTAN_UBOOT) && eMMC_DRV_MANHATTAN_UBOOT
  #include "eMMC_manhattan_uboot.h"
  #elif defined(eMMC_DRV_MASERATI_UBOOT) && eMMC_DRV_MASERATI_UBOOT
  #include "eMMC_maserati_uboot.h"
#elif defined(eMMC_DRV_CELTICS_UBOOT) && eMMC_DRV_CELTICS_UBOOT
  #include "eMMC_celtics_uboot.h"
#elif defined(eMMC_DRV_KAISER_UBOOT) && eMMC_DRV_KAISER_UBOOT
  #include "eMMC_kaiser_uboot.h"
#elif defined(eMMC_DRV_KERES_UBOOT) && eMMC_DRV_KERES_UBOOT
  #include "eMMC_keres_uboot.h"
#elif defined(eMMC_DRV_KIRIN_UBOOT) && eMMC_DRV_KIRIN_UBOOT
  #include "eMMC_kirin_uboot.h"
#elif defined(eMMC_DRV_MONET_UBOOT) && eMMC_DRV_MONET_UBOOT
  #include "eMMC_monet_uboot.h"
#elif defined(eMMC_DRV_MESSI_UBOOT) && eMMC_DRV_MESSI_UBOOT
  #include "eMMC_messi_uboot.h"
#elif defined(eMMC_DRV_MUNICH_UBOOT) && eMMC_DRV_MUNICH_UBOOT
  #include "eMMC_munich_uboot.h"
#elif defined(eMMC_DRV_KRIS_UBOOT) && eMMC_DRV_KRIS_UBOOT
  #include "eMMC_kris_uboot.h"
#elif defined(eMMC_DRV_KANO_UBOOT) && eMMC_DRV_KANO_UBOOT
  #include "eMMC_kano_uboot.h"
#elif defined(eMMC_DRV_MACAN_UBOOT) && eMMC_DRV_MACAN_UBOOT
  #include "eMMC_macan_uboot.h"
#elif defined(eMMC_DRV_CURRY_UBOOT) && eMMC_DRV_CURRY_UBOOT
  #include "eMMC_curry_uboot.h"
#elif defined(eMMC_DRV_KAYLA_UBOOT) && eMMC_DRV_KAYLA_UBOOT
  #include "eMMC_kayla_uboot.h"
#elif defined(eMMC_DRV_KASTOR_UBOOT) && eMMC_DRV_KASTOR_UBOOT
  #include "eMMC_kastor_uboot.h"
#elif defined(eMMC_DRV_MAXIM_UBOOT) && eMMC_DRV_MAXIM_UBOOT
  #include "eMMC_maxim_uboot.h"
#elif defined(eMMC_DRV_MUSTANG_UBOOT) && eMMC_DRV_MUSTANG_UBOOT
  #include "eMMC_mustang_uboot.h"
#else
  #error "Error! no platform selected."
#endif


//=====================================================
// misc. do NOT edit the following content.
//=====================================================
#define eMMC_DMA_RACING_PATCH           1
#define eMMC_DMA_PATCH_WAIT_TIME        DELAY_10ms_in_us
#define eMMC_DMA_RACING_PATTERN0        (((U32)'M'<<24)|((U32)0<<16)|((U32)'S'<<8)|(U32)1)
#define eMMC_DMA_RACING_PATTERN1        (((U32)'T'<<24)|((U32)6<<16)|((U32)'A'<<8)|(U32)8)

//===========================================================
// Time Dalay, do NOT edit the following content
//===========================================================
#if defined(eMMC_UPDATE_FIRMWARE) && (eMMC_UPDATE_FIRMWARE)
#define TIME_WAIT_DAT0_HIGH            (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RESET           (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT  (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FIFOCLK_RDY          (HW_TIMER_DELAY_10ms*10)
#define TIME_WAIT_CMDRSP_END           (HW_TIMER_DELAY_10ms*10)
#define TIME_WAIT_1_BLK_END            (HW_TIMER_DELAY_1s*5)
#define TIME_WAIT_n_BLK_END            (HW_TIMER_DELAY_1s*10) // safe for 512 blocks
#else
#define TIME_WAIT_DAT0_HIGH            (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_ERASE_DAT0_HIGH      (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RESET           HW_TIMER_DELAY_500ms
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT  HW_TIMER_DELAY_1us
#define TIME_WAIT_FIFOCLK_RDY          HW_TIMER_DELAY_10ms
#define TIME_WAIT_CMDRSP_END           HW_TIMER_DELAY_10ms
#define TIME_WAIT_R_1_BLK_END          HW_TIMER_DELAY_100ms
#define TIME_WAIT_1_BLK_END            (HW_TIMER_DELAY_1s*1)
#define TIME_WAIT_n_BLK_END            (HW_TIMER_DELAY_1s*2) // safe for 512 blocks
#endif

#endif /* __eMMC_CONFIG_H__ */
