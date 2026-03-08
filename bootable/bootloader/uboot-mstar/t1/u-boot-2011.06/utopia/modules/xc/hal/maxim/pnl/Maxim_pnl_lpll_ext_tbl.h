/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************

#ifndef _LPLL_EXT_TBL_H_
#define _LPLL_EXT_TBL_H_

#define LPLL_EXT_REG_NUM    31

typedef enum
{
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to340MHz,          //0
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to150MHz,          //1

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to150MHz,          //2
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to75MHz,          //3

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to75MHz,          //4
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to37_5MHz,          //5

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_200to340MHz,          //6
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to200MHz,          //7
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to150MHz,          //8

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_100to150MHz,          //9
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to100MHz,          //10
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to75MHz,          //11

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_50to75MHz,          //12
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to50MHz,          //13
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to37_5MHz,          //14

    E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_100to150MHz,          //15
    E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_50to100MHz,          //16
    E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to50MHz,          //17
    E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to25MHz,          //18

    E_PNL_SUPPORTED_LPLL_EXT_MAX,          //19
} E_PNL_SUPPORTED_LPLL_EXT_TYPE;

typedef struct
{
    MS_U8  address;
    MS_U16 value;
    MS_U16 mask;
}TBLStruct_Ext,*pTBLStruct_Ext;

TBLStruct_Ext LPLLSettingTBL_Ext[E_PNL_SUPPORTED_LPLL_EXT_MAX][LPLL_EXT_REG_NUM]=
{
    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to340MHz    NO.0
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0000,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to150MHz    NO.1
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0000,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to150MHz    NO.2
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0000,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to75MHz    NO.3
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0000,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to75MHz    NO.4
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0020,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to37_5MHz    NO.5
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0020,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_200to340MHz    NO.6
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0000,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to200MHz    NO.7
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0001,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to150MHz    NO.8
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0001,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_100to150MHz    NO.9
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0001,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to100MHz    NO.10
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to75MHz    NO.11
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_50to75MHz    NO.12
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to50MHz    NO.13
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to37_5MHz    NO.14
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_100to150MHz    NO.15
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0001,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0020,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0300,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0000,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0070,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x0000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0040,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0000,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0000,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x2000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0000,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_50to100MHz    NO.16
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0001,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0020,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0300,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0001,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0070,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x0000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0040,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0000,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0000,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x2000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0000,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to50MHz    NO.17
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0001,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0020,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0300,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0070,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0002,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x0000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0040,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0000,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0000,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x2000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0000,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to25MHz    NO.18
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0001,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0020,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0300,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0070,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0002,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x0000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0040,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0000,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0000,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x2000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0000,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

};
MS_U16 u16EXT_LoopGain[E_PNL_SUPPORTED_LPLL_EXT_MAX]=
{
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to340MHz    NO.0
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to150MHz    NO.1
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to150MHz    NO.2
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to75MHz    NO.3
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to75MHz    NO.4
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to37_5MHz    NO.5
    64,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_200to340MHz    NO.6
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to200MHz    NO.7
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to150MHz    NO.8
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_100to150MHz    NO.9
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to100MHz    NO.10
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to75MHz    NO.11
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_50to75MHz    NO.12
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to50MHz    NO.13
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to37_5MHz    NO.14
    12,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_100to150MHz    NO.15
    12,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_50to100MHz    NO.16
    12,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to50MHz    NO.17
    12,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to25MHz    NO.18
};
MS_U16 u16EXT_LoopDiv[E_PNL_SUPPORTED_LPLL_EXT_MAX]=
{
    10,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to340MHz    NO.0
    10,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to150MHz    NO.1
    20,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to150MHz    NO.2
    20,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to75MHz    NO.3
    40,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to75MHz    NO.4
    40,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to37_5MHz    NO.5
    15,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_200to340MHz    NO.6
    15,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to200MHz    NO.7
    15,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to150MHz    NO.8
    15,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_100to150MHz    NO.9
    30,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to100MHz    NO.10
    30,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to75MHz    NO.11
    30,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_50to75MHz    NO.12
    60,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to50MHz    NO.13
    60,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to37_5MHz    NO.14
    7,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_100to150MHz    NO.15
    14,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_50to100MHz    NO.16
    28,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to50MHz    NO.17
    28,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to25MHz    NO.18
};

#endif //_LPLL_TBL_H_
