/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

#define BD_FPGA_MASERATI              0xC000
#define BD_MST244A_D01A_S_MASERATI    0xC001
#define BD_MST244B_D01A_S_MASERATI    0xC002

#define BD_MST095D_10AVPC_MASERATI    0xD000
#define BD_MST098D_10AVPC_MASERATI    0xD001
#define BD_AMAZON_MARGO               0xD001

#define BD_UNKNOWN                    0xFFFF

#ifndef MS_BOARD_TYPE_SEL
#if defined (CONFIG_MSTAR_TITANIA_BD_FPGA)
#define MS_BOARD_TYPE_SEL               BD_FPGA_MASERATI
#include "BD_FPGA_MASERATI.h"

#elif defined(CONFIG_MSTAR_BD_MST244A_D01A_S_MASERATI)
#define MS_BOARD_TYPE_SEL               BD_MST244A_D01A_S_MASERATI
#include "BD_MST244A_D01A_S_MASERATI.h"

#elif defined(CONFIG_MSTAR_BD_MST244C_D01A_S_MASERATI)
#define MS_BOARD_TYPE_SEL               BD_MST244C_D01A_S_MASERATI
#include "BD_MST244C_D01A_S_MASERATI.h"

#elif defined(CONFIG_MSTAR_BD_MST244B_D01A_S_MASERATI)
#define MS_BOARD_TYPE_SEL               BD_MST244B_D01A_S_MASERATI
#include "BD_MST244B_D01A_S_MASERATI.h"

#elif defined(CONFIG_MSTAR_BD_MST095D_10AVPC_MASERATI)
#define MS_BOARD_TYPE_SEL               BD_MST095D_10AVPC_MASERATI
#include "BD_MST095D_10AVPC_MASERATI.h"

#elif defined(CONFIG_MSTAR_BD_MST098D_10AVPC_MASERATI)
#define MS_BOARD_TYPE_SEL               BD_MST0958D_10AVPC_MASERATI
#include "BD_MST098D_10AVPC_MASERATI.h"

#elif defined(CONFIG_MSTAR_BD_AMAZON_MARGO)
#define MS_BOARD_TYPE_SEL               BD_AMAZON_MARGO
#include "BD_AMAZON_MARGO.h"

#else
#error "Unknown Board Selection"

#endif
#endif

#endif // _BOARD_H_
