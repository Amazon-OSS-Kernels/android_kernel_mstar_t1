/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

#ifndef _DRV_MIU_INIT_H_
#define _DRV_MIU_INIT_H_

#ifndef _BOARD_H_
#include "Board.h"
#endif

#ifndef _C_RIUBASE_H_
#include "c_riubase.h"
#endif

#define MIU_VER                         'M','I','U','_','M','A','X','I','M',' ','V','e','r',':','1','_','0','_','0'
#define REG_ADDR_BASE                   0x1f000000

#ifndef __ASSEMBLER__

#ifndef __DRV_RIU_H__
#include "drvRIU.h"
#endif

#ifndef CONFIG_ONEBIN_ENABLE
#ifndef _MIU_FPGA_MAXIM_H_
#include "MIU_FPGA_MAXIM.h"
#endif

#ifndef _MIU_MST110B_20AUYGO_16183_MAXIM_23p5_H_
#include "MIU_MST110B_20AUYGO_16183_MAXIM_23p5.h"
#endif

#ifndef _MIU_MST110B_20AUYGO_16183_MAXIM_H_
#include "MIU_MST110B_20AUYGO_16183_MAXIM.h"
#endif

#ifndef _MIU_MST107B_10AUVC_16142_MAXIM_H_
#include "MIU_MST107B_10AUVC_16142_MAXIM.h"
#endif

#ifndef _MIU_MST108B_X_X_MAXIM_H_
#include "MIU_MST108B_X_X_MAXIM.h"
#endif

#ifndef _MIU_MST108B_X_X_MAXIM_23p5_H_
#include "MIU_MST108B_X_X_MAXIM_23p5.h"
#endif

#ifndef _MIU_MST111D_X_X_MAXIM_H_
#include "MIU_MST111D_X_X_MAXIM.h"
#endif

#ifndef _MIU_MST215C_D01B_S_MAXIM_H_
#include "MIU_MST215C_D01B_S_MAXIM.h"
#endif

#ifndef _MIU_MST253A_D01A_S_MAXIM_H_
#include "MIU_MST253A_D01A_S_MAXIM.h"
#endif

#ifndef _MIU_MST253B_D01A_S_MAXIM_H_
#include "MIU_MST253B_D01A_S_MAXIM.h"
#endif

#ifndef _MIU_MST253C_D01A_S_MAXIM_H_
#include "MIU_MST253C_D01A_S_MAXIM.h"
#endif
#endif

const MS_REG_INIT MIU_PreInit[] =
{
//  _RV32_2(0x113BEC, 0x8000),  //enable dramobf

#if !defined(CONFIG_MIU0_DRAM_NONE)
    //MIU0 Software Reset
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c01),

    //MIU0 Request Mask
    _RV32_2(0x101246, 0xFFFE),
    _RV32_2(0x101266, 0xFFFF),
    _RV32_2(0x101286, 0xFFFF),
    _RV32_2(0x1012A6, 0xFFFF),
    _RV32_2(0x161506, 0xFFFF),
    _RV32_2(0x161526, 0xFFFF),
    _RV32_2(0x152006, 0xFFFF),
    _RV32_2(0x152026, 0xFFFF),
#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    //MIU1 Software Reset
    _RV32_2(0x10061e, 0x0000),
    _RV32_2(0x10061e, 0x0000),
    _RV32_2(0x10061e, 0x0000),
    _RV32_2(0x10061e, 0x0c01),

    //MIU1 Request Mask
    _RV32_2(0x100646, 0xFFFE),
    _RV32_2(0x100666, 0xFFFF),
    _RV32_2(0x100686, 0xFFFF),
    _RV32_2(0x1006A6, 0xFFFF),
    _RV32_2(0x162206, 0xFFFF),
    _RV32_2(0x162226, 0xFFFF),
    _RV32_2(0x152106, 0xFFFF),
    _RV32_2(0x152126, 0xFFFF),
#endif

    _END_OF_TBL32_,
    MIU_VER
};

const MS_REG_INIT MIU_PostInit[] =
{
#if !defined(CONFIG_MIU0_DRAM_NONE)
    //open all MIU0 request mask (All IP can access MIU resource)
    _RV32_2(0x101246, 0x0000),  //Mask MIU0_group_0
    _RV32_2(0x101266, 0x0000),  //Mask MIU0_group_1
    _RV32_2(0x101286, 0x0000),  //Mask MIU0_group_2
    _RV32_2(0x1012a6, 0x0000),  //Mask MIU0_group_3
    //_RV32_2(0x161506, 0x0000),  //Mask MIU0_group_4
    _RV32_2(0x161526, 0x0000),  //Mask MIU0_group_5

    _RV32_2(0x1615E2, 0x007E),
    _RV32_2(0x1615E8, 0x0078),
    _RV32_2(0x1615EA, 0x0418),
    _RV32_2(0x1615EC, 0x0101),

    _RV32_2(0x152006, 0x0000),
    _RV32_2(0x152026, 0x0000),
#endif

    _RV32_2(0x10121e, 0x8c08),  //SW initial done and turn on arbitor

#if !defined(CONFIG_MIU1_DRAM_NONE)
    //open all MIU1 request mask (All IP can access MIU resource)
    _RV32_2(0x100646, 0x0000),  //Mask MIU1_group_0
    _RV32_2(0x100666, 0x0000),  //Mask MIU1_group_1
    _RV32_2(0x100686, 0x0000),  //Mask MIU1_group_2
    _RV32_2(0x1006a6, 0x0000),  //Mask MIU1_group_3
    //_RV32_2(0x162206, 0x0000),  //Mask MIU1_group_4
    _RV32_2(0x162226, 0x0000),  //Mask MIU1_group_5
    _RV32_2(0x152106, 0x0000),
    _RV32_2(0x152126, 0x0000),
#endif

    _RV32_2(0x1012fe, 0xa0e1),

    _RV32_2(0x1012f8, 0x0000),
    _RV32_2(0x1006f8, 0x0000),

    _END_OF_TBL32_,
    MIU_VER
};

#endif /* !__ASSEMBLER__ */

#endif /* _DRV_MIU_INIT_H_ */
