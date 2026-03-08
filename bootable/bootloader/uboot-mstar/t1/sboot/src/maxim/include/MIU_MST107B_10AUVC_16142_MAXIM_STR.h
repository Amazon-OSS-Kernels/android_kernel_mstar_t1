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

#ifndef _MIU_MST107B_10AUVC_16142_MAXIM_STR_H_
#define _MIU_MST107B_10AUVC_16142_MAXIM_STR_H_

#if (ENABLE_MSTAR_BD_MST107B_10AUVC_16142_MAXIM == 1)

#ifndef CONFIG_MIU0_DRAM_NONE
const MS_REG_INIT MIU0_DDR_Init_Str[] =
{
#if defined(CONFIG_MIU0_DDR3_1666)
    #error "Not support DDR3-1600"
#elif defined(CONFIG_MIU0_DDR3_1866)
#if defined(CONFIG_MIU0_4X_MODE)
    _RV32_2(0x101202, 0xf2a7),
    _RV32_2(0x101204, 0x0051),
    _RV32_2(0x101206, 0x1681),
    _RV32_2(0x101208, 0x24ee),
    _RV32_2(0x10120a, 0x3288),
    _RV32_2(0x10120c, 0xd80a),
    _RV32_2(0x10120e, 0xc118),
    _RV32_2(0x101210, 0x1124),
    _RV32_2(0x101212, 0x4004),
    _RV32_2(0x101214, 0x8028),
    _RV32_2(0x101216, 0xc000),
    _RV32_2(0x101228, 0x00c0),
    _RV32_2(0x1012d2, 0xa000),
    _RV32_2(0x152b22, 0x0003),
    _RV32_2(0x152b3e, 0x0000),
    _RV32_2(0x152b40, 0x0e0e),
    _RV32_2(0x152b42, 0x0824),
    _RV32_2(0x152b44, 0x3208),
    _RV32_2(0x152b46, 0x100a),
    _RV32_2(0x152b48, 0x0d08),
    _RV32_2(0x152b4a, 0x0604),
    _RV32_2(0x152b4c, 0x0528),
    _RV32_2(0x152b4e, 0x0118),
    _RV32_2(0x152b50, 0xe000),
    _RV32_2(0x152b52, 0x0000),
    _RV32_2(0x152b54, 0x0e00),
    _RV32_2(0x152b80, 0x0000),
    _RV32_2(0x152b86, 0x0000),
    _RV32_2(0x152bfe, 0x0001), //enable DIG_E bank
    _RV32_2(0x161560, 0x0000),
    _RV32_2(0x161562, 0x0000),
    _RV32_2(0x161564, 0x0000),
    _RV32_2(0x161566, 0x0030),
    _RV32_2(0x161568, 0x5000),
    _RV32_2(0x110d02, 0xaaaa),
    _RV32_2(0x110d04, 0x0008),
    _RV32_2(0x110d0a, 0x1100),
    _RV32_2(0x110d0e, 0x0099),
    _RV32_2(0x110d2e, 0x0000),
    _RV32_2(0x110d38, 0x0077),
    _RV32_2(0x110d3a, 0xa0a5),
    _RV32_2(0x110d3c, 0x9233),
    _RV32_2(0x110d3e, 0xa022),
    _RV32_2(0x110d48, 0x0077),
    _RV32_2(0x110d4a, 0xa0a0),
    _RV32_2(0x110d4c, 0x0233),
    _RV32_2(0x110d4e, 0x0022),
    _RV32_2(0x110d50, 0x1111),
    _RV32_2(0x110d52, 0x0066),
    _RV32_2(0x110d6c, 0x0808),
    _RV32_2(0x110d6e, 0x0808),

#if !defined(CONFIG_ENABLE_AUTO_DQS)
    _RV32_2(0x110da0, 0x1111),
    _RV32_2(0x110da2, 0x1111),
    _RV32_2(0x110da4, 0x1111),
    _RV32_2(0x110da6, 0x1111),
    _RV32_2(0x110da8, 0x1111),
    _RV32_2(0x110daa, 0x1111),
    _RV32_2(0x110dac, 0x1111),
    _RV32_2(0x110dae, 0x1111),

    _RV32_2(0x110d94, 0x0a0a),
    _RV32_2(0x110d96, 0x0a0a),
#endif

    _RV32_2(0x110db6, 0x0000),
    _RV32_2(0x110db8, 0x2222),
    _RV32_2(0x110dba, 0x0222),
    _RV32_2(0x110dbc, 0x0222),
    _RV32_2(0x110dbe, 0x0222),
    _RV32_2(0x110dd0, 0x5555),
    _RV32_2(0x110dd2, 0x5555),
    _RV32_2(0x110dd4, 0x5555),
    _RV32_2(0x110dd6, 0x5555),
    _RV32_2(0x110dd8, 0x0055),
    _RV32_2(0x110de0, 0x5555),
    _RV32_2(0x110de2, 0x5555),
    _RV32_2(0x110de4, 0x5555),
    _RV32_2(0x110de6, 0x5555),
    _RV32_2(0x110de8, 0x0055),

    //-----------------
    //program DLL
    //-----------------
    _RV32_2(0x110d62, 0x007f),
    _RV32_2(0x110d64, 0xf000),
    _RV32_2(0x110d60, 0x00cb),
    _RV32_2(0x110d60, 0x00cf),
    _RV32_2(0x110d60, 0x00cb),
    _RV32_2(0x110d60, 0x00c3),
    _RV32_2(0x110d60, 0x00cb),
    _RV32_2(0x110d60, 0x00c3),
    _RV32_2(0x110d60, 0x00cb),
    _RV32_2(0x110d60, 0x00c2),
    _RV32_2(0x110d60, 0x00c0),
    _RV32_2(0x110d60, 0x33c8),
    _RV32_2(0x110d70, 0x0000),
    _RV32_2(0x110d98, 0x0000),
    _RV32_2(0x110d9a, 0x0000),
    _RV32_2(0x110d90, 0xf0f1),
    _RV32_2(0x110d70, 0x0800),

    //---------------------------
    //program dig general setting
    //---------------------------
    _RV32_2(0x1615de, 0x1000),
    _RV32_2(0x10122c, 0x8221),
    _RV32_2(0x1012fc, 0x951a),

    //--------------------------------------
    //Chose last_done_z after sync_fifo
    //--------------------------------------
    //_RV32_2(0x101252, 0xffff),
    //_RV32_2(0x101272, 0xffff),
    //_RV32_2(0x101292, 0xffff),
    //_RV32_2(0x1012b2, 0xffff),
    _RV32_2(0x161512, 0xffff),
    _RV32_2(0x161532, 0xffff),
    _RV32_2(0x152012, 0xffff),

    //---------------------
    // Default BW setting
    //---------------------
    _RV32_2(0x101240, 0x8015),
    _RV32_2(0x101260, 0x8015),
    _RV32_2(0x101280, 0x8015),
    _RV32_2(0x1012a0, 0x8015),
    _RV32_2(0x161500, 0x8015),
    _RV32_2(0x161520, 0x8015),
    _RV32_2(0x152000, 0x8015),

    //---------------------------
    //program ana general setting
    //---------------------------
    _RV32_2(0x110d70, 0x0800),

    //driving setting
    _RV32_2(0x110d58, 0x0c0c), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x110d5a, 0xaaaa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x110d5c, 0xccaa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x110d5e, 0xccaa), //driving: @20160427 hsin-yen suggest default value

    //trig. lvl setting
    _RV32_2(0x110d74, 0x0808), //trig. lvl: @20160415 hsin-yen suggest default value
    _RV32_2(0x110d76, 0x0808), //trig. lvl: @20160415 hsin-yen suggest default value

    _RV32_2(0x110d1a, 0x8333),
    _RV32_2(0x110d1c, 0x0020),
    _RV32_2(0x110d08, 0x0000),

    //---------------------------
    //Release miu SW reset
    //---------------------------
    //_RV32_2(0x10121e, 0x8c01),
    //_RV32_2(0x10121e, 0x8c00),

    //---------------------------
    //disable GPIO
    //---------------------------
    _RV32_2(0x110d00, 0x201b),
    _RV32_2(0x110d00, 0x000b),
    _RV32_2(0x110d18, 0x0000),
    _RV32_2(0x110d7c, 0x0000),

    //---------------------------
    //Release ATOP PD mode
    //---------------------------
    _RV32_2(0x110d54, 0xc070),

    //---------------------------
    //select Mapping
    //---------------------------
    _RV32_2(0x110d00, 0x000b),
    _RV32_2(0x101200, 0x0000),
    _RV32_2(0x110d00, 0x000b), //reg_pkg_sel = 3
#elif defined(CONFIG_MIU0_8X_MODE)
    #error "No Support DDR3-2133 (8x Mode)"
#endif

#elif defined(CONFIG_MIU0_DDR3_2133)
#if defined(CONFIG_MIU0_4X_MODE)
    _RV32_2(0x110d30, 0x34e6),
    _RV32_2(0x110d32, 0x001a),
    _RV32_2(0x110d36, 0x0405),
    _RV32_2(0x110d2a, 0x0527),
    _RV32_2(0x110d28, 0xc00d),

    _RV32_2(0x101202, 0xf2a7),
    _RV32_2(0x101204, 0x0054),
    _RV32_2(0x101206, 0x1681),
    _RV32_2(0x101208, 0x24ee),
    _RV32_2(0x10120a, 0x3288),
    _RV32_2(0x10120c, 0xe80a),
    _RV32_2(0x10120e, 0xc118),
    _RV32_2(0x101210, 0x1124),
    _RV32_2(0x101212, 0x4004),
    _RV32_2(0x101214, 0x8028),
    _RV32_2(0x101216, 0xc000),
    _RV32_2(0x101228, 0x00b0),
    _RV32_2(0x1012d2, 0xa000),
    _RV32_2(0x152b22, 0x0003),
    _RV32_2(0x152b3e, 0x0000),
    _RV32_2(0x152b40, 0x0e0e),
    _RV32_2(0x152b42, 0x0824),
    _RV32_2(0x152b44, 0x3208),
    _RV32_2(0x152b46, 0x100a),
    _RV32_2(0x152b48, 0x0e08),
    _RV32_2(0x152b4a, 0x0604),
    _RV32_2(0x152b4c, 0x0528),
    _RV32_2(0x152b4e, 0x0118),
    _RV32_2(0x152b50, 0xe000),
    _RV32_2(0x152b52, 0x0000),
    _RV32_2(0x152b54, 0x0e00),
    _RV32_2(0x152b80, 0x0000),
    _RV32_2(0x152b86, 0x0000),
    _RV32_2(0x152bfe, 0x0001), //enable DIG_E bank
    _RV32_2(0x161560, 0x0000),
    _RV32_2(0x161562, 0x0000),
    _RV32_2(0x161564, 0x0000),
    _RV32_2(0x161566, 0x0030),
    _RV32_2(0x161568, 0x5000),
    _RV32_2(0x110d02, 0xaaaa),
    _RV32_2(0x110d04, 0x0008),
    _RV32_2(0x110d0a, 0x1177),
    _RV32_2(0x110d0e, 0x0099),
    _RV32_2(0x110d2e, 0x0000),
    _RV32_2(0x110d38, 0x0077),
    _RV32_2(0x110d3a, 0xa0d0),
    _RV32_2(0x110d3c, 0x1333),
    _RV32_2(0x110d3e, 0xa033),
    _RV32_2(0x110d48, 0x0077),
    _RV32_2(0x110d4a, 0xc0d0),
    _RV32_2(0x110d4c, 0x0233),
    _RV32_2(0x110d4e, 0x0022),
    _RV32_2(0x110d50, 0x1111),
    _RV32_2(0x110d52, 0x0024),
    _RV32_2(0x110d6c, 0x0808),
    _RV32_2(0x110d6e, 0x0808),
    _RV32_2(0x110d8a, 0x0124),

#if !defined(CONFIG_ENABLE_AUTO_DQS)
    _RV32_2(0x110da0, 0x4509),
    _RV32_2(0x110da2, 0x5638),
    _RV32_2(0x110da4, 0x4014),
    _RV32_2(0x110da6, 0x4143),
    _RV32_2(0x110da8, 0x1406),
    _RV32_2(0x110daa, 0x5726),
    _RV32_2(0x110dac, 0x4004),
    _RV32_2(0x110dae, 0x4033),

    _RV32_2(0x110d94, 0x0a0f),
    _RV32_2(0x110d96, 0x0a0c),
#endif

    _RV32_2(0x110db6, 0x0000),
    _RV32_2(0x110db8, 0x3333),
    _RV32_2(0x110dba, 0x0333),
    _RV32_2(0x110dbc, 0x0333),
    _RV32_2(0x110dbe, 0x0333),
    _RV32_2(0x110dd0, 0x5555),
    _RV32_2(0x110dd2, 0x5555),
    _RV32_2(0x110dd4, 0x5555),
    _RV32_2(0x110dd6, 0x5555),
    _RV32_2(0x110dd8, 0x0055),
    _RV32_2(0x110de0, 0x5555),
    _RV32_2(0x110de2, 0x5555),
    _RV32_2(0x110de4, 0x5555),
    _RV32_2(0x110de6, 0x5555),
    _RV32_2(0x110de8, 0x0055),

    _RV32_2(0x110dfc, 0x1f1f),
    _RV32_2(0x110dfe, 0x1f1f),

    //-----------------
    //program DLL
    //-----------------
    _RV32_2(0x110d62, 0x007f),
    _RV32_2(0x110d64, 0xf000),
    _RV32_2(0x110d60, 0x00cb),
    _RV32_2(0x110d60, 0x00cf),
    _RV32_2(0x110d60, 0x00cb),
    _RV32_2(0x110d60, 0x00c3),
    _RV32_2(0x110d60, 0x00cb),
    _RV32_2(0x110d60, 0x00c3),
    _RV32_2(0x110d60, 0x00cb),
    _RV32_2(0x110d60, 0x00c2),
    _RV32_2(0x110d60, 0x00c0),
    _RV32_2(0x110d60, 0x33c8),
    _RV32_2(0x110d70, 0x0000),
    _RV32_2(0x110d98, 0x0000),
    _RV32_2(0x110d9a, 0x0000),
    _RV32_2(0x110d90, 0xf0f1),
    _RV32_2(0x110d70, 0x0800),

    //---------------------------
    //program dig general setting
    //---------------------------
    _RV32_2(0x1615de, 0x1000),
    _RV32_2(0x10122c, 0x8221),
    _RV32_2(0x1012fc, 0x951a),

    //--------------------------------------
    //Chose last_done_z after sync_fifo
    //--------------------------------------
    //_RV32_2(0x101252, 0xffff),
    //_RV32_2(0x101272, 0xffff),
    //_RV32_2(0x101292, 0xffff),
    //_RV32_2(0x1012b2, 0xffff),
    _RV32_2(0x161512, 0xffff),
    _RV32_2(0x161532, 0xffff),
    _RV32_2(0x152012, 0xffff),

    //---------------------
    // Default BW setting
    //---------------------
    _RV32_2(0x101240, 0x8015),
    _RV32_2(0x101260, 0x8015),
    _RV32_2(0x101280, 0x8015),
    _RV32_2(0x1012a0, 0x8015),
    _RV32_2(0x161500, 0x8015),
    _RV32_2(0x161520, 0x8015),
    _RV32_2(0x152000, 0x8015),

    //---------------------------
    //program ana general setting
    //---------------------------
    _RV32_2(0x110d70, 0x0800),

    //driving setting
    _RV32_2(0x110d58, 0x0808), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x110d5a, 0xfafa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x110d5c, 0xc8aa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x110d5e, 0xc8aa), //driving: @20160427 hsin-yen suggest default value

    //trig. lvl setting
    _RV32_2(0x110d74, 0x0909), //trig. lvl: @20160415 hsin-yen suggest default value
    _RV32_2(0x110d76, 0x0909), //trig. lvl: @20160415 hsin-yen suggest default value

    _RV32_2(0x110d1a, 0x8333),
    _RV32_2(0x110d1c, 0x0020),
    _RV32_2(0x110d08, 0x0000),

    //---------------------------
    //Release miu SW reset
    //---------------------------
    //_RV32_2(0x10121e, 0x8c01),
    //_RV32_2(0x10121e, 0x8c00),

    //---------------------------
    //disable GPIO
    //---------------------------
    _RV32_2(0x110d00, 0x201b),
    _RV32_2(0x110d00, 0x000b),
    _RV32_2(0x110d18, 0x0000),
    _RV32_2(0x110d7c, 0x0000),

    //---------------------------
    //Release ATOP PD mode
    //---------------------------
    _RV32_2(0x110d54, 0xc070),

    //---------------------------
    //select Mapping
    //---------------------------
    _RV32_2(0x110d00, 0x000b),
    _RV32_2(0x101200, 0x0000),
    _RV32_2(0x110d00, 0x000b), //reg_pkg_sel = 3

#elif defined(CONFIG_MIU0_8X_MODE)
    #error "No Support DDR3-2133 (8x Mode)"
#endif

#elif defined(CONFIG_MIU0_DDR4_2400)
    #error "Not support DDR4-2400"
#else
    #error "Invalid DRAM Setting"
#endif

    _END_OF_TBL32_,
    MIU_VER
};
#endif // CONFIG_MIU0_DRAM_NONE

#ifndef CONFIG_MIU1_DRAM_NONE
const MS_REG_INIT MIU1_DDR_Init_Str[] =
{
#if defined(CONFIG_MIU1_DDR3_1666)
    #error "Not support DDR3-1600"
#elif defined(CONFIG_MIU1_DDR3_1866)
#if defined(CONFIG_MIU1_4X_MODE)
    _RV32_2(0x100602, 0xf2a7),
    _RV32_2(0x100604, 0x0051),
    _RV32_2(0x100606, 0x1681),
    _RV32_2(0x100608, 0x24ee),
    _RV32_2(0x10060a, 0x3288),
    _RV32_2(0x10060c, 0xd80a),
    _RV32_2(0x10060e, 0xc118),
    _RV32_2(0x100610, 0x1124),
    _RV32_2(0x100612, 0x4004),
    _RV32_2(0x100614, 0x8028),
    _RV32_2(0x100616, 0xc000),
    _RV32_2(0x100628, 0x00c0),
    _RV32_2(0x1006d2, 0xa000),
    _RV32_2(0x152c22, 0x0003),
    _RV32_2(0x152c3e, 0x0000),
    _RV32_2(0x152c40, 0x0e0e),
    _RV32_2(0x152c42, 0x0824),
    _RV32_2(0x152c44, 0x3208),
    _RV32_2(0x152c46, 0x100a),
    _RV32_2(0x152c48, 0x0d08),
    _RV32_2(0x152c4a, 0x0604),
    _RV32_2(0x152c4c, 0x0528),
    _RV32_2(0x152c4e, 0x0118),
    _RV32_2(0x152c50, 0xe000),
    _RV32_2(0x152c52, 0x0000),
    _RV32_2(0x152c54, 0x0e00),
    _RV32_2(0x152c80, 0x0000),
    _RV32_2(0x152c86, 0x0000),
    _RV32_2(0x152cfe, 0x0001), //enable DIG_E bank
    _RV32_2(0x162260, 0x0000),
    _RV32_2(0x162262, 0x0000),
    _RV32_2(0x162264, 0x0000),
    _RV32_2(0x162266, 0x0030),
    _RV32_2(0x162268, 0x5000),
    _RV32_2(0x161602, 0xaaaa),
    _RV32_2(0x161604, 0x0008),
    _RV32_2(0x16160a, 0x1100),
    _RV32_2(0x16160e, 0x0099),
    _RV32_2(0x16162e, 0x1111),
    _RV32_2(0x161638, 0x0077),
    _RV32_2(0x16163a, 0x5055),
    _RV32_2(0x16163c, 0x9233),
    _RV32_2(0x16163e, 0xa022),
    _RV32_2(0x161648, 0x0077),
    _RV32_2(0x16164a, 0x5050),
    _RV32_2(0x16164c, 0x0233),
    _RV32_2(0x16164e, 0x0022),
    _RV32_2(0x161650, 0x1111),
    _RV32_2(0x161652, 0x0066),
    _RV32_2(0x16166c, 0x0808),
    _RV32_2(0x16166e, 0x0808),

#if !defined(CONFIG_ENABLE_AUTO_DQS)
    _RV32_2(0x1616a0, 0x1111),
    _RV32_2(0x1616a2, 0x1111),
    _RV32_2(0x1616a4, 0x1111),
    _RV32_2(0x1616a6, 0x1111),
    _RV32_2(0x1616a8, 0x1111),
    _RV32_2(0x1616aa, 0x1111),
    _RV32_2(0x1616ac, 0x1111),
    _RV32_2(0x1616ae, 0x1111),

    _RV32_2(0x161694, 0x0a0a),  // k/2 offset
    _RV32_2(0x161696, 0x0a0a),  // k/2 offset
#endif

    _RV32_2(0x1616b6, 0x0000),
    _RV32_2(0x1616b8, 0x2222),
    _RV32_2(0x1616ba, 0x0222),
    _RV32_2(0x1616bc, 0x0222),
    _RV32_2(0x1616be, 0x0222),
    _RV32_2(0x1616d0, 0x4444),
    _RV32_2(0x1616d2, 0x4444),
    _RV32_2(0x1616d4, 0x4444),
    _RV32_2(0x1616d6, 0x4444),
    _RV32_2(0x1616d8, 0x0044),
    _RV32_2(0x1616e0, 0x5555),
    _RV32_2(0x1616e2, 0x5555),
    _RV32_2(0x1616e4, 0x5555),
    _RV32_2(0x1616e6, 0x5555),
    _RV32_2(0x1616e8, 0x0055),

    //-----------------
    //program DLL
    //-----------------
    _RV32_2(0x161662, 0x007f),
    _RV32_2(0x161664, 0xf000),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00cf),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c3),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c3),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c2),
    _RV32_2(0x161660, 0x00c0),
    _RV32_2(0x161660, 0x33c8),
    _RV32_2(0x161670, 0x0000),
    _RV32_2(0x161698, 0x0000),
    _RV32_2(0x16169a, 0x0000),
    _RV32_2(0x161690, 0xf0f1),
    _RV32_2(0x161670, 0x0800),

    //---------------------------
    //program dig general setting
    //---------------------------
    _RV32_2(0x1622de, 0x1000),
    _RV32_2(0x10062c, 0x8221),
    _RV32_2(0x1006fc, 0x951a),

    //--------------------------------------
    //Chose last_done_z after sync_fifo
    //--------------------------------------
    //_RV32_2(0x100652, 0xffff),
    //_RV32_2(0x100672, 0xffff),
    //_RV32_2(0x100692, 0xffff),
    //_RV32_2(0x1006b2, 0xffff),
    _RV32_2(0x162212, 0xffff),
    _RV32_2(0x162232, 0xffff),
    _RV32_2(0x152112, 0xffff),

    //---------------------
    // Default BW setting
    //---------------------
    _RV32_2(0x100640, 0x8015),
    _RV32_2(0x100660, 0x8015),
    _RV32_2(0x100680, 0x8015),
    _RV32_2(0x1006a0, 0x8015),
    _RV32_2(0x162200, 0x8015),
    _RV32_2(0x162220, 0x8015),
    _RV32_2(0x152100, 0x8015),

    //---------------------------
    //program ana general setting
    //---------------------------
    _RV32_2(0x161670, 0x0800),

    //driving setting
    _RV32_2(0x161658, 0x0c0c), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165a, 0xaaaa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165c, 0xccaa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165e, 0xccaa), //driving: @20160427 hsin-yen suggest default value

    //trig. lvl setting
    _RV32_2(0x161674, 0x0808), //trig. lvl: @20160415 hsin-yen suggest default value
    _RV32_2(0x161676, 0x0808), //trig. lvl: @20160415 hsin-yen suggest default value
    _RV32_2(0x16161a, 0x8333),
    _RV32_2(0x16161c, 0x0020),
    _RV32_2(0x161608, 0x0000),

    //---------------------------
    //Release miu SW reset
    //---------------------------
    //_RV32_2(0x10061e, 0x8c01),
    //_RV32_2(0x10061e, 0x8c00),

    //---------------------------
    //disable GPIO
    //---------------------------
    _RV32_2(0x161600, 0x201a),
    _RV32_2(0x161600, 0x000a),
    _RV32_2(0x161618, 0x0000),
    _RV32_2(0x16167c, 0x0000),

    //---------------------------
    //Release ATOP PD mode
    //---------------------------
    _RV32_2(0x161654, 0xc070),

    //---------------------------
    //select Mapping
    //---------------------------
    _RV32_2(0x161600, 0x000a),
    _RV32_2(0x100600, 0x0000),
    _RV32_2(0x161600, 0x000a), //reg_pkg_sel = 2

#elif defined(CONFIG_MIU1_8X_MODE)
    #error "No Support DDR3-2133 (8x Mode)"
#endif
#elif defined(CONFIG_MIU1_DDR3_2133)
#if defined(CONFIG_MIU1_4X_MODE)
    _RV32_2(0x161630, 0x34e6),
    _RV32_2(0x161632, 0x001a),
    _RV32_2(0x161636, 0x0400),
    _RV32_2(0x16162a, 0x0527),
    _RV32_2(0x161628, 0xc00d),

    _RV32_2(0x100602, 0xf2a7),
    _RV32_2(0x100604, 0x0055),
    _RV32_2(0x100606, 0x1681),
    _RV32_2(0x100608, 0x24ee),
    _RV32_2(0x10060a, 0x3288),
    _RV32_2(0x10060c, 0xe80a),
    _RV32_2(0x10060e, 0xc118),
    _RV32_2(0x100610, 0x1124),
    _RV32_2(0x100612, 0x4004),
    _RV32_2(0x100614, 0x8028),
    _RV32_2(0x100616, 0xc000),
    _RV32_2(0x100628, 0x00b0),
    _RV32_2(0x1006d2, 0xa000),
    _RV32_2(0x152c22, 0x0003),
    _RV32_2(0x152c3e, 0x0000),
    _RV32_2(0x152c40, 0x0e0e),
    _RV32_2(0x152c42, 0x0824),
    _RV32_2(0x152c44, 0x3208),
    _RV32_2(0x152c46, 0x100a),
    _RV32_2(0x152c48, 0x0e08),
    _RV32_2(0x152c4a, 0x0604),
    _RV32_2(0x152c4c, 0x0528),
    _RV32_2(0x152c4e, 0x0118),
    _RV32_2(0x152c50, 0xe000),
    _RV32_2(0x152c52, 0x0000),
    _RV32_2(0x152c54, 0x0e00),
    _RV32_2(0x152c80, 0x0000),
    _RV32_2(0x152c86, 0x0000),
    _RV32_2(0x152cfe, 0x0001), //enable DIG_E bank
    _RV32_2(0x162260, 0x0000),
    _RV32_2(0x162262, 0x0000),
    _RV32_2(0x162264, 0x0000),
    _RV32_2(0x162266, 0x0030),
    _RV32_2(0x162268, 0x5000),
    _RV32_2(0x161602, 0xaaaa),
    _RV32_2(0x161604, 0x0008),
    _RV32_2(0x16160a, 0x2200),
    _RV32_2(0x16160e, 0x009b),
    _RV32_2(0x16162e, 0x3333),
    _RV32_2(0x161638, 0x4477),
    _RV32_2(0x16163a, 0x6095),
    _RV32_2(0x16163c, 0x1322),
    _RV32_2(0x16163e, 0xb033),
    _RV32_2(0x161648, 0x4477),
    _RV32_2(0x16164a, 0x6075),
    _RV32_2(0x16164c, 0x0222),
    _RV32_2(0x16164e, 0x0022),
    _RV32_2(0x161650, 0x1111),
    _RV32_2(0x161652, 0x00bb),
    _RV32_2(0x16166c, 0x0808),
    _RV32_2(0x16166e, 0x0808),
    _RV32_2(0x16168a, 0x0124),

#if !defined(CONFIG_ENABLE_AUTO_DQS)
    _RV32_2(0x1616a0, 0x0519),
    _RV32_2(0x1616a2, 0x2929),
    _RV32_2(0x1616a4, 0x8126),
    _RV32_2(0x1616a6, 0x7072),
    _RV32_2(0x1616a8, 0x0607),
    _RV32_2(0x1616aa, 0x2524),
    _RV32_2(0x1616ac, 0x7267),
    _RV32_2(0x1616ae, 0x6031),

    _RV32_2(0x161694, 0x0a0a),  // k/2 offset
    _RV32_2(0x161696, 0x0a0a),  // k/2 offset
#endif

    _RV32_2(0x1616b6, 0x0000),
    _RV32_2(0x1616b8, 0x3333),
    _RV32_2(0x1616ba, 0x0333),
    _RV32_2(0x1616bc, 0x0333),
    _RV32_2(0x1616be, 0x0333),
    _RV32_2(0x1616d0, 0x4444),
    _RV32_2(0x1616d2, 0x4444),
    _RV32_2(0x1616d4, 0x4444),
    _RV32_2(0x1616d6, 0x4444),
    _RV32_2(0x1616d8, 0x0044),
    _RV32_2(0x1616e0, 0x4444),
    _RV32_2(0x1616e2, 0x4444),
    _RV32_2(0x1616e4, 0x4444),
    _RV32_2(0x1616e6, 0x4444),
    _RV32_2(0x1616e8, 0x0044),

    _RV32_2(0x1616fc, 0x2f2f),
    _RV32_2(0x1616fe, 0x2f2f),

    //-----------------
    //program DLL
    //-----------------
    _RV32_2(0x161662, 0x007f),
    _RV32_2(0x161664, 0xf000),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00cf),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c3),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c3),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c2),
    _RV32_2(0x161660, 0x00c0),
    _RV32_2(0x161660, 0x33c8),
    _RV32_2(0x161670, 0x0000),
    _RV32_2(0x161698, 0x0000),
    _RV32_2(0x16169a, 0x0000),
    _RV32_2(0x161690, 0xf0f1),
    _RV32_2(0x161670, 0x0800),

    //---------------------------
    //program dig general setting
    //---------------------------
    _RV32_2(0x1622de, 0x1000),
    _RV32_2(0x10062c, 0x8221),
    _RV32_2(0x1006fc, 0x951a),

    //--------------------------------------
    //Chose last_done_z after sync_fifo
    //--------------------------------------
    //_RV32_2(0x100652, 0xffff),
    //_RV32_2(0x100672, 0xffff),
    //_RV32_2(0x100692, 0xffff),
    //_RV32_2(0x1006b2, 0xffff),
    _RV32_2(0x162212, 0xffff),
    _RV32_2(0x162232, 0xffff),
    _RV32_2(0x152112, 0xffff),

    //---------------------
    // Default BW setting
    //---------------------
    _RV32_2(0x100640, 0x8015),
    _RV32_2(0x100660, 0x8015),
    _RV32_2(0x100680, 0x8015),
    _RV32_2(0x1006a0, 0x8015),
    _RV32_2(0x162200, 0x8015),
    _RV32_2(0x162220, 0x8015),
    _RV32_2(0x152100, 0x8015),

    //---------------------------
    //program ana general setting
    //---------------------------
    _RV32_2(0x161670, 0x0800),

    //driving setting
    _RV32_2(0x161658, 0x0808), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165a, 0xfafa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165c, 0xc8fa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165e, 0xc8fa), //driving: @20160427 hsin-yen suggest default value

    //trig. lvl setting
    _RV32_2(0x161674, 0x0909), //trig. lvl: @20160415 hsin-yen suggest default value
    _RV32_2(0x161676, 0x0a0a), //trig. lvl: @20160415 hsin-yen suggest default value
    _RV32_2(0x16161a, 0x8333),
    _RV32_2(0x16161c, 0x0020),
    _RV32_2(0x161608, 0x0000),

    //---------------------------
    //Release miu SW reset
    //---------------------------
    //_RV32_2(0x10061e, 0x8c01),
    //_RV32_2(0x10061e, 0x8c00),

    //---------------------------
    //disable GPIO
    //---------------------------
    _RV32_2(0x161600, 0x201a),
    _RV32_2(0x161600, 0x000a),
    _RV32_2(0x161618, 0x0000),
    _RV32_2(0x16167c, 0x0000),

    //---------------------------
    //Release ATOP PD mode
    //---------------------------
    _RV32_2(0x161654, 0xc070),

    //---------------------------
    //select Mapping
    //---------------------------
    _RV32_2(0x161600, 0x000a),
    _RV32_2(0x100600, 0x0000),
    _RV32_2(0x161600, 0x000a), //reg_pkg_sel = 2

#elif defined(CONFIG_MIU1_8X_MODE)
    #error "No Support DDR3-2133 (8x Mode)"
#endif

#elif defined(CONFIG_MIU1_DDR4_2400)
#if defined(CONFIG_MIU1_4X_MODE)
    #error "No Support DDR4-2400 (4x Mode)"
#elif defined(CONFIG_MIU1_8X_MODE)
    _RV32_2(0x100602, 0xf397),
    _RV32_2(0x100604, 0x000e),
    _RV32_2(0x100606, 0x1649),
    _RV32_2(0x100608, 0xe711),
    _RV32_2(0x10060a, 0x3897),
    _RV32_2(0x10060c, 0xf32c),
    _RV32_2(0x10060e, 0xc138),
    _RV32_2(0x100610, 0x0964),
    _RV32_2(0x100612, 0x2101),
    _RV32_2(0x100614, 0x40d8),
    _RV32_2(0x100616, 0x6000),
    _RV32_2(0x100628, 0x0001),
    _RV32_2(0x1006d2, 0xa000),
    _RV32_2(0x152c22, 0x0003),
    _RV32_2(0x152c3e, 0x0000),
    _RV32_2(0x152c40, 0x1111),
    _RV32_2(0x152c42, 0x0727),
    _RV32_2(0x152c44, 0x3809),
    _RV32_2(0x152c46, 0x120c),
    _RV32_2(0x152c48, 0x0f03),
    _RV32_2(0x152c4a, 0x0604),
    _RV32_2(0x152c4c, 0x0524),
    _RV32_2(0x152c4e, 0x0138),
    _RV32_2(0x152c50, 0xe000),
    _RV32_2(0x152c52, 0x0806),
    _RV32_2(0x152c54, 0x1109),
    _RV32_2(0x152c80, 0x8010),
    _RV32_2(0x152c86, 0x0000),
    _RV32_2(0x152cfe, 0x0001), //enable DIG_E bank
    _RV32_2(0x162260, 0x8000),
    _RV32_2(0x162262, 0xa400),
    _RV32_2(0x162264, 0xc800),
    _RV32_2(0x162266, 0x0039),
    _RV32_2(0x162268, 0x5986),
    _RV32_2(0x16336a, 0x0024),
    _RV32_2(0x161602, 0xaaaa),
    _RV32_2(0x161604, 0x0080),
    _RV32_2(0x16160a, 0x3300),
    _RV32_2(0x16160e, 0x008d),
    _RV32_2(0x16162e, 0x1111),
    _RV32_2(0x161638, 0x0077),
    _RV32_2(0x16163a, 0xa0d0),
    _RV32_2(0x16163c, 0x9366),
    _RV32_2(0x16163e, 0xb033),
    _RV32_2(0x161648, 0x0077),
    _RV32_2(0x16164a, 0xb0b0),
    _RV32_2(0x16164c, 0x0166),
    _RV32_2(0x16164e, 0x0033),
    _RV32_2(0x161650, 0x1111),
    _RV32_2(0x161652, 0x0065),
    _RV32_2(0x16166c, 0x0808),
    _RV32_2(0x16166e, 0x0808),

#if !defined(CONFIG_ENABLE_AUTO_DQS)
    _RV32_2(0x1616a0, 0x0102),
    _RV32_2(0x1616a2, 0x1625),
    _RV32_2(0x1616a4, 0x2767),
    _RV32_2(0x1616a6, 0x3017),
    _RV32_2(0x1616a8, 0x3405),
    _RV32_2(0x1616aa, 0x3535),
    _RV32_2(0x1616ac, 0x0124),
    _RV32_2(0x1616ae, 0x4214),

    _RV32_2(0x161694, 0x0704),  // k/2 offset
    _RV32_2(0x161696, 0x0808),  // k/2 offset
#endif

    _RV32_2(0x1616b6, 0x0000),
    _RV32_2(0x1616b8, 0x3333),
    _RV32_2(0x1616ba, 0x0333),
    _RV32_2(0x1616bc, 0x0333),
    _RV32_2(0x1616be, 0x0333),
    _RV32_2(0x1616d0, 0x4444),
    _RV32_2(0x1616d2, 0x4444),
    _RV32_2(0x1616d4, 0x4444),
    _RV32_2(0x1616d6, 0x4444),
    _RV32_2(0x1616d8, 0x0044),
    _RV32_2(0x1616e0, 0x5555),
    _RV32_2(0x1616e2, 0x5555),
    _RV32_2(0x1616e4, 0x5555),
    _RV32_2(0x1616e6, 0x5555),
    _RV32_2(0x1616e8, 0x0055),

    //-----------------
    //program DLL
    //-----------------
    _RV32_2(0x161662, 0x007f),
    _RV32_2(0x161664, 0xf000),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00cf),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c3),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c3),
    _RV32_2(0x161660, 0x00cb),
    _RV32_2(0x161660, 0x00c2),
    _RV32_2(0x161660, 0x00c0),
    _RV32_2(0x161660, 0x33c8),
    _RV32_2(0x161670, 0x0000),
    _RV32_2(0x161698, 0x0000),
    _RV32_2(0x16169a, 0x0000),
    _RV32_2(0x161690, 0xf0f1),
    _RV32_2(0x161670, 0x0800),

    //---------------------------
    //program dig general setting
    //---------------------------
    _RV32_2(0x1622de, 0x1000),
    _RV32_2(0x10062c, 0x8221),
    _RV32_2(0x1006fc, 0x951a),

    //--------------------------------------
    //Chose last_done_z after sync_fifo
    //--------------------------------------
    //_RV32_2(0x100652, 0xffff),
    //_RV32_2(0x100672, 0xffff),
    //_RV32_2(0x100692, 0xffff),
    //_RV32_2(0x1006b2, 0xffff),
    _RV32_2(0x162212, 0xffff),
    _RV32_2(0x162232, 0xffff),
    _RV32_2(0x152112, 0xffff),

    //---------------------
    // Default BW setting
    //---------------------
    _RV32_2(0x100640, 0x8015),
    _RV32_2(0x100660, 0x8015),
    _RV32_2(0x100680, 0x8015),
    _RV32_2(0x1006a0, 0x8015),
    _RV32_2(0x162200, 0x8015),
    _RV32_2(0x162220, 0x8015),
    _RV32_2(0x152100, 0x8015),

    //---------------------------
    //program ana general setting
    //---------------------------
    _RV32_2(0x161670, 0x0800),

    //driving setting
    _RV32_2(0x161658, 0x0a0a), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165a, 0xcccc), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165c, 0xc8aa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x16165e, 0xc8aa), //driving: @20160427 hsin-yen suggest default value
    _RV32_2(0x1616de, 0xafaf),

    //trig. lvl setting
    _RV32_2(0x161674, 0x0707), //trig. lvl: @20160415 hsin-yen suggest default value
    _RV32_2(0x161676, 0x0707), //trig. lvl: @20160415 hsin-yen suggest default value
    _RV32_2(0x16161a, 0x0333),
    _RV32_2(0x16161c, 0x0020),
    _RV32_2(0x161608, 0x007f),

    //---------------------------
    //Release miu SW reset
    //---------------------------
    //_RV32_2(0x10061e, 0x8c01),
    //_RV32_2(0x10061e, 0x8c00),

    //---------------------------
    //disable GPIO
    //---------------------------
    _RV32_2(0x161600, 0x2010),
    _RV32_2(0x161600, 0x0000),
    _RV32_2(0x161618, 0x0000),
    _RV32_2(0x16167c, 0x0000),

    //---------------------------
    //Release ATOP PD mode
    //---------------------------
    _RV32_2(0x161654, 0xc000),

    //---------------------------
    //select Mapping
    //---------------------------
    _RV32_2(0x161600, 0x0002),
    _RV32_2(0x100600, 0x0000),
    _RV32_2(0x161600, 0x0001), //reg_pkg_sel = 2

    //---------------------------
    // Set MRx[6:0] = Choose VrefDQ value & Range
    //---------------------------
    //_RV32_2(0x103382, 0x0020), // MIU0
    _RV32_2(0x103384, 0x0020), // MIU1
#endif

#else
    #error "Invalid DRAM Setting"
#endif

    _END_OF_TBL32_,
    MIU_VER
};
#endif

#endif  //ENABLE_MSTAR_BD_MST107B_10AUVC_16142_MAXIM
#endif /* _MIU_MST107B_10AUVC_16142_MAXIM_STR_H_ */
