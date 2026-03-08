/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef _DRV_MIU_INIT_STR_H_
#define _DRV_MIU_INIT_STR_H_

#ifndef _BOARD_H_
#include "Board.h"
#endif

#ifndef _C_RIUBASE_H_
#include "c_riubase.h"
#endif

#define MIU_VER                         'M','I','U','_','M','A','S','E','R','A','T','I',' ','V','e','r',':','1','_','0','_','0'
#define REG_ADDR_BASE                   0x1f000000

#ifndef __ASSEMBLER__

#ifndef __DRV_RIU_H__
#include "drvRIU.h"
#endif
#if (ENABLE_ONEBIN_ENABLE != 1)
#ifndef _MIU_FPGA_MASERATI_H_
#include "MIU_FPGA_MASERATI.h"
#endif

#ifndef _MIU_MST244A_D01A_S_MASERATI_STR_H_
#include "MIU_MST244A_D01A_S_MASERATI_STR.h"
#endif

#ifndef _MIU_MST244B_D01A_S_MASERATI_STR_H_
#include "MIU_MST244B_D01A_S_MASERATI_STR.h"
#endif

#ifndef _MIU_MST244C_D01A_S_MASERATI_STR_H_
#include "MIU_MST244C_D01A_S_MASERATI_STR.h"
#endif

#ifndef _MIU_MST095D_MASERATI_STR_H_
#include "MIU_MST095D_10AVPC_MASERATI_STR.h"
#endif

#ifndef _MIU_MST098D_MASERATI_STR_H_
#include "MIU_MST098D_10AVPC_MASERATI_STR.h"
#endif

#ifndef _MIU_AMAZON_MARGO_STR_H_
#include "MIU_AMAZON_MARGO_STR.h"
#endif
#endif
const MS_REG_INIT MIU_PreSetting_Str[] =
{
    _RV32_2(0x16159C, 0x8004),
    _RV32_2(0x16159E, 0xFF08),
    _RV32_2(0x1615AC, 0x09F0),
    _RV32_2(0x1615AE, 0x2011),

    _RV32_2(0x16229C, 0x8004),
    _RV32_2(0x16229E, 0xFF08),
    _RV32_2(0x1622AC, 0x09F0),
    _RV32_2(0x1622AE, 0x2011),

    _RV32_2(0x16158E, 0x0001),     //GPU Highway

    _END_OF_TBL32_,
    MIU_VER
};

const MS_REG_INIT MIU_PreInit_Str[] =
{
//  _RV32_2(0x113BEC, 0x8000),  //enable dramobf

#if !defined(CONFIG_MIU0_DRAM_NONE)
    //MIU0 Software reset
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c00),
    _RV32_2(0x10121e, 0x0c01),
   //_RV32_2(0x10121e, 0x0c00),

    //MIU0 Request mask
    _RV32_2(0x101246, 0xFFFE),
    _RV32_2(0x101266, 0xFFFF),
    _RV32_2(0x101286, 0xFFFF),
    _RV32_2(0x1012A6, 0xFFFF),
    _RV32_2(0x161506, 0xFFFF),
    _RV32_2(0x161526, 0xFFFF),
    _RV32_2(0x152006, 0xFFFF),
    _RV32_2(0x152026, 0xFFFF),
    _RV32_2(0x1615E6, 0xfff8),
#endif

#if !defined(CONFIG_MIU1_DRAM_NONE)
    //MIU1 Software reset
    _RV32_2(0x10061e, 0x0000),
    _RV32_2(0x10061e, 0x0000),
    _RV32_2(0x10061e, 0x0000),
    _RV32_2(0x10061e, 0x0c01),
    //_RV32_2(0x10061e, 0x0c00),

    //MIU1 Request mask
    _RV32_2(0x100646, 0xFFFE),
    _RV32_2(0x100666, 0xFFFF),
    _RV32_2(0x100686, 0xFFFF),
    _RV32_2(0x1006A6, 0xFFFF),
    _RV32_2(0x162206, 0xFFFF),
    _RV32_2(0x162226, 0xFFFF),
    _RV32_2(0x152106, 0xFFFF),
    _RV32_2(0x152126, 0xFFFF),
    _RV32_2(0x1622E6, 0xfff8),
#endif

#if !defined(CONFIG_MIU2_DRAM_NONE)
    //MIU2 Software reset
    _RV32_2(0x16201e, 0x0000),
    _RV32_2(0x16201e, 0x0000),
    _RV32_2(0x16201e, 0x0000),
    _RV32_2(0x16201e, 0x0c01),
    //_RV32_2(0x16201e, 0x0c00),

    //MIU2 Request mask
    _RV32_2(0x162046, 0xFFFE),
    _RV32_2(0x162066, 0xFFFF),
    _RV32_2(0x162086, 0xFFFF),
    _RV32_2(0x1620a6, 0xFFFF),
    _RV32_2(0x162306, 0xFFFF),
    _RV32_2(0x162326, 0xFFFF),
    _RV32_2(0x152206, 0xFFFF),
    _RV32_2(0x152226, 0xFFFF),
    _RV32_2(0x1623e6, 0xfff8),
#endif

    _END_OF_TBL32_,
    MIU_VER
};

const MS_REG_INIT MIU_PostInit_Str[] =
{
#if !defined(CONFIG_MIU0_DRAM_NONE)
    _RV32_2(0x1012e0, 0x0080),  //en address toggle mode

    //open all MIU0 request mask (All IP can access MIU resource)
    _RV32_2(0x101246, 0x0000),  //Mask MIU0_group_0
    _RV32_2(0x101266, 0x0000),  //Mask MIU0_group_1
    _RV32_2(0x101286, 0x0000),  //Mask MIU0_group_2
    _RV32_2(0x1012a6, 0x0000),  //Mask MIU0_group_3
    _RV32_2(0x161506, 0x0000),  //Mask MIU0_group_4
    _RV32_2(0x161526, 0x0000),  //Mask MIU0_group_5
    //_RV32_2(0x152006, 0x0000),
    _RV32_2(0x152026, 0x0000),

    _RV32_2(0x1615E2, 0x001E),
    _RV32_2(0x1615E8, 0x0018),
    _RV32_2(0x1615EA, 0x0480),
    _RV32_2(0x1615EC, 0x0202),
    _RV32_2(0x1615E6, 0x0000),
#endif

    _RV32_2(0x10121e, 0x8c08),  //SW initial done and turn on arbitor

#if !defined(CONFIG_MIU1_DRAM_NONE)
    _RV32_2(0x1006e0, 0x0080),  //en address toggle mode

    //open all MIU1 request mask (All IP can access MIU resource)
    _RV32_2(0x100646, 0x0000),  //Mask MIU1_group_0
    _RV32_2(0x100666, 0x0000),  //Mask MIU1_group_1
    _RV32_2(0x100686, 0x0000),  //Mask MIU1_group_2
    _RV32_2(0x1006a6, 0x0000),  //Mask MIU1_group_3
    _RV32_2(0x162206, 0x0000),  //Mask MIU1_group_4
    _RV32_2(0x162226, 0x0000),  //Mask MIU1_group_5
    //_RV32_2(0x152106, 0x0000),
    _RV32_2(0x152126, 0x0000),

    //all IP choose sw select( not IP select )
    _RV32_2(0x1006f0, 0x0000),
    _RV32_2(0x1006f2, 0x0000),
    _RV32_2(0x1006f4, 0x0000),
    _RV32_2(0x1006f6, 0x0000),
    _RV32_2(0x1006f8, 0x0000),
    _RV32_2(0x1006fa, 0x0000),
    _RV32_2(0x1622e6, 0x0000),
#endif

#if !defined(CONFIG_MIU2_DRAM_NONE)
    _RV32_2(0x1620e0, 0x0080),  //en address toggle mode

    //open all MIU1 request mask (All IP can access MIU resource)
    _RV32_2(0x162046, 0x0000),  //Mask MIU1_group_0
    _RV32_2(0x162066, 0x0000),  //Mask MIU1_group_1
    _RV32_2(0x162086, 0x0000),  //Mask MIU1_group_2
    _RV32_2(0x1620a6, 0x0000),  //Mask MIU1_group_3
    _RV32_2(0x162306, 0x0000),  //Mask MIU1_group_4
    _RV32_2(0x162326, 0x0000),  //Mask MIU1_group_5
    //_RV32_2(0x152206, 0x0000),
    _RV32_2(0x152226, 0x0000),

    _RV32_2(0x1012fe, 0xa0e1),
    _RV32_2(0x1623e6, 0x0000),
#endif
    _RV32_2(0x1615E2, 0x001E),  //0x161571=001E
    _RV32_2(0x1615EA, 0x0420),  //0x161575=0420
    _RV32_2(0x1615EC, 0x0102),  //0x161576=0102
    _RV32_2(0x1622E2, 0x001E),  //0x162271=001E
    _RV32_2(0x1622EA, 0x0420),  //0x162275=0420
    _RV32_2(0x1622EC, 0x0101),  //0x162276=0101
    _RV32_2(0x162220, 0x8015),  //0x162210=8015
    _RV32_2(0x162222, 0x2020),  //0x162211=2020
    _RV32_2(0x162224, 0x0400),  //0x162212=0400
    _RV32_2(0x162228, 0xFFC3),  //0x162214=FFC3
    _RV32_2(0x162238, 0xD1FF),  //0x16221c=D1FF

    _RV32_2(0x1623E2, 0x001E),      //0x162371=001E
    _RV32_2(0x1623E8, 0x0018),      //0x162374=0018
    _RV32_2(0x1623EA, 0x0420),      //0x162375=0418
    _RV32_2(0x152202, 0x2020),      //0x162375=0418
    _RV32_2(0x152208, 0xFF0F),      //0x162375=0418

    _END_OF_TBL32_,
    MIU_VER
};

#endif /* !__ASSEMBLER__ */

#endif /* _DRV_MIU_INIT_H_ */
