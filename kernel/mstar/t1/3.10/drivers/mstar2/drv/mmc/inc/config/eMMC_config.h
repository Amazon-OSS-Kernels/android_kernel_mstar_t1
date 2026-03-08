/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

#define UNIFIED_eMMC_DRIVER		1

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
#define S32  signed int
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif


#if defined(CONFIG_MSTAR_EAGLE)
#define eMMC_DRV_EAGLE_LINUX    1
#endif

#if defined(CONFIG_MSTAR_EDISON)
#define eMMC_DRV_EDISON_LINUX   1
#endif

#if defined(CONFIG_MSTAR_EIFFEL)
#define eMMC_DRV_EIFFEL_LINUX   1
#endif

#if defined(CONFIG_MSTAR_NIKE)
#define eMMC_DRV_NIKE_LINUX     1
#endif

#if defined(CONFIG_MSTAR_MADISON)
#define eMMC_DRV_MADISON_LINUX     1
#endif


#if defined(CONFIG_MSTAR_EINSTEIN)
#define eMMC_DRV_EINSTEIN_LINUX   1
#endif

#if defined(CONFIG_MSTAR_AMBER3)
#define eMMC_DRV_AMBER3_LINUX    1
#endif

#if defined(CONFIG_MSTAR_AGATE)
#define eMMC_DRV_AGATE_LINUX    1
#endif

#if defined(CONFIG_MSTAR_KAISER)
#define eMMC_DRV_KAISER_LINUX    1
#endif

#if defined(CONFIG_MSTAR_KAISERS)
#define eMMC_DRV_KAISERS_LINUX    1
#endif

#if defined(CONFIG_MSTAR_NAPOLI)
#define eMMC_DRV_NAPOLI_LINUX       1
#endif

#if defined(CONFIG_MSTAR_MONACO)
#define eMMC_DRV_MONACO_LINUX		1
#endif

#if defined(CONFIG_MSTAR_MIAMI)
#define eMMC_DRV_MIAMI_LINUX		1
#endif

#if defined(CONFIG_MSTAR_MUJI)
#define eMMC_DRV_MUJI_LINUX         1
#endif

#if defined(CONFIG_MSTAR_MONET)
#define eMMC_DRV_MONET_LINUX        1
#endif

#if defined(CONFIG_MSTAR_CLIPPERS)
#define eMMC_DRV_CLIPPERS_LINUX		1
#endif

#if defined(CONFIG_MSTAR_MANHATTAN)
#define eMMC_DRV_MANHATTAN_LINUX		1
#endif

#if defined(CONFIG_MSTAR_KANO)
#define eMMC_DRV_KANO_LINUX         1
#endif

#if defined(CONFIG_MSTAR_MALDIVES)
#define eMMC_DRV_MALDIVES_LINUX         1
#endif

#if defined(CONFIG_MSTAR_MESSI)
#define eMMC_DRV_MESSI_LINUX         1
#endif

#if defined(CONFIG_MSTAR_MASERATI)
#define eMMC_DRV_MASERATI_LINUX         1
#endif

#if defined(CONFIG_MSTAR_MAXIM)
#define eMMC_DRV_MAXIM_LINUX         1
#endif

#define eMMC_SET_ACP_MIU0()
#define eMMC_SET_ACP_MIU1()

#if (defined(CONFIG_MSTAR_AMBER3) || \
     defined(CONFIG_MSTAR_AGATE)  || \
     defined(CONFIG_MSTAR_EAGLE)  || \
     defined(CONFIG_MSTAR_EDISON) || \
     defined(CONFIG_MSTAR_EIFFEL) || \
     defined(CONFIG_MSTAR_NIKE) || \
     defined(CONFIG_MSTAR_MADISON) || \
     defined(CONFIG_MSTAR_EINSTEIN) || \
     defined(CONFIG_MSTAR_KAISER) || \
     defined(CONFIG_MSTAR_KAISERS) || \
     defined(CONFIG_MSTAR_NAPOLI) )
#define eMMC_FCIE_MIU0_MIU1_SEL(dmaaddr) \
	if( dmaaddr >= MSTAR_MIU1_BUS_BASE){    \
        dmaaddr -= MSTAR_MIU1_BUS_BASE;     \
        REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);\
		eMMC_SET_ACP_MIU1(); \
    }else{                                  \
        dmaaddr -= MSTAR_MIU0_BUS_BASE;     \
        REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);\
		eMMC_SET_ACP_MIU0();  \
    }
#else
#define eMMC_FCIE_MIU0_MIU1_SEL(dmaaddr)
#endif
//=====================================================
// select a config h file
//=====================================================
#if defined(eMMC_DRV_G2P_ROM) && eMMC_DRV_G2P_ROM
  #include "eMMC_g2p_rom.h"
#elif defined(eMMC_DRV_G2P_UBOOT) && eMMC_DRV_G2P_UBOOT
  #include "eMMC_msw8x68_uboot.h"
#elif defined(eMMC_DRV_G2E_UBOOT) && eMMC_DRV_G2E_UBOOT
  #include "eMMC_msw8x68t_uboot.h"
#elif defined(eMMC_DRV_G2P_BL) && eMMC_DRV_G2P_BL
  #include "eMMC_g2p_bl.h"
#elif defined(eMMC_DRV_A3_UBOOT) && eMMC_DRV_A3_UBOOT
  #include "eMMC_a3_uboot.h"
#elif defined(eMMC_DRV_AGATE_UBOOT) && eMMC_DRV_AGATE_UBOOT
  #include "eMMC_agate_uboot.h"
#elif defined(eMMC_DRV_EAGLE_UBOOT) && eMMC_DRV_EAGLE_UBOOT
  #include "eMMC_eagle_uboot.h"
#elif defined(eMMC_DRV_EDISON_UBOOT) && eMMC_DRV_EDISON_UBOOT
  #include "eMMC_edison_uboot.h"
#elif defined(eMMC_DRV_EIFFEL_UBOOT) && eMMC_DRV_EIFFEL_UBOOT
  #include "eMMC_eiffel_uboot.h"
#elif defined(eMMC_DRV_NIKE_UBOOT) && eMMC_DRV_NIKE_UBOOT
  #include "eMMC_nike_uboot.h"
#elif defined(eMMC_DRV_G2P_LINUX) && eMMC_DRV_G2P_LINUX
  #include "eMMC_msw8x68_linux.h"
// [FIXME] add a .h file for your platform
#elif defined(eMMC_DRV_EAGLE_LINUX) && eMMC_DRV_EAGLE_LINUX
  #include "eMMC_eagle_linux.h"
#elif defined(eMMC_DRV_EDISON_LINUX) && eMMC_DRV_EDISON_LINUX
  #include "eMMC_edison_linux.h"
#elif defined(eMMC_DRV_EIFFEL_LINUX) && eMMC_DRV_EIFFEL_LINUX
  #include "eMMC_eiffel_linux.h"
#elif defined(eMMC_DRV_NIKE_LINUX) && eMMC_DRV_NIKE_LINUX
  #include "eMMC_nike_linux.h"
#elif defined(eMMC_DRV_MADISON_LINUX) && eMMC_DRV_MADISON_LINUX
  #include "eMMC_madison_linux.h"
#elif defined(eMMC_DRV_EINSTEIN_LINUX) && eMMC_DRV_EINSTEIN_LINUX
  #include "eMMC_einstein_linux.h"
#elif defined(eMMC_DRV_AMBER3_LINUX) && eMMC_DRV_AMBER3_LINUX
  #include "eMMC_a3_linux.h"
#elif defined(eMMC_DRV_AGATE_LINUX) && eMMC_DRV_AGATE_LINUX
	#include "eMMC_agate_linux.h"
#elif defined(eMMC_DRV_KAISER_LINUX) && eMMC_DRV_KAISER_LINUX
	#include "eMMC_kaiser_linux.h"
#elif defined(eMMC_DRV_KAISERS_LINUX) && eMMC_DRV_KAISERS_LINUX
	#include "eMMC_kaisers_linux.h"
#elif defined(eMMC_DRV_NAPOLI_LINUX) && eMMC_DRV_NAPOLI_LINUX
  #include "eMMC_napoli_linux.h"
#elif defined(eMMC_DRV_MIAMI_LINUX) && eMMC_DRV_MIAMI_LINUX
	#include "eMMC_miami_linux.h"
#elif defined(eMMC_DRV_MUJI_LINUX) && eMMC_DRV_MUJI_LINUX
	#include "eMMC_muji_linux.h"
#elif defined(eMMC_DRV_MONET_LINUX) && eMMC_DRV_MONET_LINUX
	#include "eMMC_monet_linux.h"
#elif defined(eMMC_DRV_MONACO_LINUX) && eMMC_DRV_MONACO_LINUX
	#include "eMMC_monaco_linux.h"
#elif defined(eMMC_DRV_CLIPPERS_LINUX) && eMMC_DRV_CLIPPERS_LINUX
	#include "eMMC_clippers_linux.h"
#elif defined(eMMC_DRV_MANHATTAN_LINUX) && eMMC_DRV_MANHATTAN_LINUX
	#include "eMMC_manhattan_linux.h"
#elif defined(eMMC_DRV_KANO_LINUX) && eMMC_DRV_KANO_LINUX
	#include "eMMC_kano_linux.h"
#elif defined(eMMC_DRV_MASERATI_LINUX) && eMMC_DRV_MASERATI_LINUX
	#include "eMMC_maserati_linux.h"
#elif defined(eMMC_DRV_MAXIM_LINUX) && eMMC_DRV_MAXIM_LINUX
	#include "eMMC_maxim_linux.h" 
#else
  #error "Error! no platform selected."
#endif

#if defined(eMMC_DRV_G2P_UBOOT) && eMMC_DRV_G2P_UBOOT
#define eMMC_FCIE_LINUX_DRIVER  1
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
#define TIME_WAIT_1_BLK_END            (HW_TIMER_DELAY_1s*1)
#define TIME_WAIT_n_BLK_END            (HW_TIMER_DELAY_1s*2) // safe for 512 blocks
#endif


extern void mdelay_MacroToFun(u32 time);

#endif /* __eMMC_CONFIG_H__ */
