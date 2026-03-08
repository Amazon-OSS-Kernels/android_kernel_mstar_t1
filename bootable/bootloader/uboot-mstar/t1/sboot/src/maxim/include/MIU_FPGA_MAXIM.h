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
#ifndef _MIU_FPGA_MAXIM_H_
#define _MIU_FPGA_MAXIM_H_

#if (ENABLE_MSTAR_TITANIA_BD_FPGA)

const MS_REG_INIT MIU0_DDR_Init[] =
{
    _RV32_2(0x002bc6 ,0x3c11),
    _RV32_2(0x002bca ,0x403c),
    _RV32_2(0x002bc4 ,0x0004),
    _RV32_2(0x101202 ,0x0000),
    _RV32_2(0x101202 ,0x0818),
    _RV32_2(0x101204 ,0x0185),
    _RV32_2(0x101206 ,0x0120),
    _RV32_2(0x101208 ,0x0833),
    _RV32_2(0x10120a ,0x0c33),
    _RV32_2(0x10120c ,0x7111),
    _RV32_2(0x10120e ,0x100e),
    _RV32_2(0x101210 ,0x0031),
    _RV32_2(0x101228 ,0x4000),
    _RV32_2(0x10121e ,0x0C01),
    _RV32_2(0x10121e ,0x0C00),
    _RV32_2(0x101200 ,0x0008),
    _RV32_2(0x101200 ,0x000c),
    _RV32_2(0x101200 ,0x000e),
    _RV32_2(0x101200 ,0x001f),
    _RV32_2(0x10121e ,0x0C08),
    _RV32_2(0x1012f8 ,0x0000),
    _RV32_2(0x1012fa ,0x03FF),

    _END_OF_TBL32_,
    MIU_VER
};

#ifndef CONFIG_MIU1_DRAM_NONE
const MS_REG_INIT MIU1_DDR_Init[] =
{
    _END_OF_TBL32_,
};
#endif  //CONFIG_MIU1_DRAM_NONE

#endif  //ENABLE_MSTAR_TITANIA_BD_FPGA

#endif  //_MIU_FPGA_MAXIM_H_
