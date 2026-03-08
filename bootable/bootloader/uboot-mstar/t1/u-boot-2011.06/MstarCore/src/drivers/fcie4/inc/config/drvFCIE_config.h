/**
* Copyright (c) 2006  2016 MStar Semiconductor, Inc.
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

#ifndef __DRVFCIE_CONFIG_H__
#define __DRVFCIE_CONFIG_H__

#define FALSE           0
#define TRUE            1

#ifdef CONFIG_AMBER5
#include "drvFCIE_a5_uboot.h"
#endif

#if (ENABLE_EAGLE)
#include "drvFCIE_eagle_uboot.h"
#elif (ENABLE_EIFFEL)
#include "drvFCIE_eiffel_uboot.h"
#elif (ENABLE_NIKE)
#include "drvFCIE_nike_uboot.h"
#elif (ENABLE_MADISON)
#include "drvFCIE_madison_uboot.h"
#elif (ENABLE_CLIPPERS)
#include "drvFCIE_clippers_uboot.h"
#elif (ENABLE_MIAMI)
#include "drvFCIE_miami_uboot.h"
#elif (ENABLE_NADAL)
#include "drvFCIE_nadal_uboot.h"
#elif (ENABLE_AMBER3)
#include "drvFCIE_a3_uboot.h"
#elif (ENABLE_AGATE)
#include "drvFCIE_agate_uboot.h"
#elif (ENABLE_EINSTEIN)
#include "drvFCIE_einstein_uboot.h"
#elif (ENABLE_NAPOLI)
#include "drvFCIE_napoli_uboot.h"
#elif (ENABLE_MONACO)
#include "drvFCIE_monaco_uboot.h"
#elif (ENABLE_MONET)
#include "drvFCIE_monet_uboot.h"
#elif (ENABLE_MESSI)
#include "drvFCIE_messi_uboot.h"
#elif (ENABLE_MUJI)
#include "drvFCIE_muji_uboot.h"
#elif (ENABLE_CELTICS)
#include "drvFCIE_celtics_uboot.h"
#elif (ENABLE_KAISER)
#include "drvFCIE_kaiser_uboot.h"
#elif (ENABLE_MUNICH)
#include "drvFCIE_munich_uboot.h"
#elif (ENABLE_MANHATTAN)
#include "drvFCIE_manhattan_uboot.h"
#elif (ENABLE_KANO)
#include "drvFCIE_kano_uboot.h"
#elif (ENABLE_MACAN)
#include "drvFCIE_macan_uboot.h"
#elif (ENABLE_CURRY)
#include "drvFCIE_curry_uboot.h"
#elif (ENABLE_C2P)
#include "drvFCIE_c2p_uboot.h"
#elif (ENABLE_KASTOR)
#include "drvFCIE_kastor_uboot.h"
#elif (ENABLE_MASERATI)
#include "drvFCIE_maserati_uboot.h"
#elif (ENABLE_KAYLA)
#include "drvFCIE_kayla_uboot.h"
#elif (ENABLE_K5TN)
#include "drvFCIE_kentucky_uboot.h"
#else
#error "Wrong chip setting!"
#endif

#define CARDDET_CDZ         BIT00
#define CARDDET_INSERT_LOW  BIT01
#define CARDDET_INSERT_HIGH BIT02

#define NONE			"\033[m"
#define RED				"\033[0;32;31m"
#define LIGHT_RED		"\033[1;31m"
#define GREEN			"\033[0;32;32m"
#define LIGHT_GREEN		"\033[1;32m"
#define BLUE			"\033[0;32;34m"
#define LIGHT_BLUE		"\033[1;34m"
#define DARY_GRAY		"\033[1;30m"
#define CYAN			"\033[0;36m"
#define LIGHT_CYAN		"\033[1;36m"
#define PURPLE			"\033[0;35m"
#define LIGHT_PURPLE	"\033[1;35m"
#define BROWN			"\033[0;33m"
#define YELLOW			"\033[1;33m"
#define LIGHT_GRAY		"\033[0;37m"
#define WHITE			"\033[1;37m"

#define U32BEND2LEND(X) ( ((X&0x000000FF)<<24) + ((X&0x0000FF00)<<8) + ((X&0x00FF0000)>>8) + ((X&0xFF000000)>>24) )
#define U16BEND2LEND(X) ( ((X&0x00FF)<<8) + ((X&0xFF00)>>8) )

void	HalSdio_ResetIP(void);
void	HalSdio_Platform_InitChiptop(void);
U8		HalSdio_GetPadType(void);
void	HalSdio_SwitchPad(unsigned char);
U32		HalSdio_clock_setting(U16 u16_ClkParam);
void	HalSdioDelayMs(U32 u32Ms);
void	HalSdioDelayUs(U32 u32Us);
U32		HalSdio_SetClock(U32 u32Clock);
void	HalSdio_DumpDebugBus(void);

#endif // #ifndef __DRVFCIE_CONFIG_H__


