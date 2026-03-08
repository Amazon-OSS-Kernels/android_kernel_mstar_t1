/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#ifndef _ENUM_DEVICE_H_
#include "Enum_Device.h"
#endif

#ifndef __MEMORY_MAP_SEL_H__
#include "mmap/MMap.h"
#endif

//------------------------------------------------------------------------------

#define ENABLE                          1
#define DISABLE                         0
#define OBA2                            0
#define USE_SW_I2C                      1
#define ENABLE_UART1_DEBUG              0

//------------------------------UNUSED MACROS-----------------------------------
#define _FUNC_NOT_USED()                do {} while ( 0 )

//------------------------------MS_BOARD_TYPE_SEL-------------------------------
// Demo board made in Taiwan

#define BD_FPGA_MAXIM                       0xC000
#define BD_MST253A_D01A_S_MAXIM             0xC001
#define BD_MST253B_D01A_S_MAXIM             0xC002
#define BD_MST253C_D01A_S_MAXIM             0xC003
#define BD_MST215C_D01B_S_MAXIM             0xC004
#define BD_MST107B_10AUVC_16142_MAXIM       0xC005
#define BD_MST110B_20AUYGO_16183_MAXIM      0xC006
#define BD_MST108B_X_X_MAXIM                0xC007
#define BD_MST111D_X_X_MAXIM                0xC008
#define BD_MST108B_X_X_MAXIM_23p5           0xC009
#define BD_MST110B_20AUYGO_16183_MAXIM_23p5 0xC00A

#define BD_UNKNOWN                    0xFFFF

#ifndef MS_BOARD_TYPE_SEL
#if defined (CONFIG_MSTAR_TITANIA_BD_FPGA)
#define MS_BOARD_TYPE_SEL               BD_FPGA_MAXIM
#include "BD_FPGA_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST253A_D01A_S_MAXIM)
#define MS_BOARD_TYPE_SEL               BD_MST253A_D01A_S_MAXIM
#include "BD_MST253A_D01A_S_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST253B_D01A_S_MAXIM)
#define MS_BOARD_TYPE_SEL               BD_MST253B_D01A_S_MAXIM
#include "BD_MST253B_D01A_S_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST253C_D01A_S_MAXIM)
#define MS_BOARD_TYPE_SEL               BD_MST253C_D01A_S_MAXIM
#include "BD_MST253C_D01A_S_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST215C_D01B_S_MAXIM)
#define MS_BOARD_TYPE_SEL               BD_MST215C_D01B_S_MAXIM
#include "BD_MST215C_D01B_S_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST107B_10AUVC_16142_MAXIM)
#define MS_BOARD_TYPE_SEL               BD_MST107B_10AUVC_16142_MAXIM
#include "BD_MST107B_10AUVC_16142_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST110B_20AUYGO_16183_MAXIM)
#define MS_BOARD_TYPE_SEL               BD_MST110B_20AUYGO_16183_MAXIM
#include "BD_MST110B_20AUYGO_16183_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST108B_X_X_MAXIM)
#define MS_BOARD_TYPE_SEL               BD_MST108B_X_X_MAXIM
#include "BD_MST108B_X_X_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST111D_X_X_MAXIM)
#define MS_BOARD_TYPE_SEL               BD_MST111D_X_X_MAXIM
#include "BD_MST111D_X_X_MAXIM.h"

#elif defined(CONFIG_MSTAR_BD_MST108B_X_X_MAXIM_23p5)
#define MS_BOARD_TYPE_SEL               BD_MST108B_X_X_MAXIM_23p5
#include "BD_MST108B_X_X_MAXIM_23p5.h"

#elif defined(CONFIG_MSTAR_BD_MST110B_20AUYGO_16183_MAXIM_23p5)
#define MS_BOARD_TYPE_SEL               BD_MST110B_20AUYGO_16183_MAXIM_23p5
#include "BD_MST110B_20AUYGO_16183_MAXIM_23p5.h"

#else
#error "Unknown Board Selection"

#endif
#endif

#endif // _BOARD_H_
