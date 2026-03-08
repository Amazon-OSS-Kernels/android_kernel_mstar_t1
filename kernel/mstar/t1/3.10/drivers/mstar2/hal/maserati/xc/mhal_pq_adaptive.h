#if 1
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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   mhal_pq_adaptive.h
/// @brief  MStar PQ Driver DDI HAL Level
/// @author MStar Semiconductor Inc.
/// @attention
/// <b>(OBSOLETED) <em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_PQ_ADAPTIVE_H
#define _HAL_PQ_ADAPTIVE_H

//#include "mdrv_types.h"
//#include "mdrv_xc_st.h"

#ifdef _HAL_PQ_ADAPTIVE_H
#define INTERFACE
#else
#define INTERFACE extern
#endif

//=============================================================================
// Includs
//=============================================================================
//#include "mhal_dlc.h"


//=============================================================================
// Type and Structure Declaration


//=============================================================================
// HAL Driver Function
//=============================================================================
#define MENULOAD

#ifdef MENULOAD
extern MS_BOOL KApi_XC_MLoad_WriteCmd(EN_MLOAD_CLIENT_TYPE _client_type, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
extern MS_BOOL KApi_XC_MLoad_WriteCmd_NonXC(EN_MLOAD_CLIENT_TYPE _client_type,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);

#define MLoad_W2BYTEMSK(ADDR,  VAL, MASK) do {\
                                                  \
                KApi_XC_MLoad_WriteCmd(E_CLIENT_MAIN_HDR, ADDR, ((VAL) & 0xFFFF), MASK);  \
                                                                 \
            } while(0)
                
#define MLoad_UFSC_W2BYTEMSK(ADDR,  VAL, MASK) do {\
                                                  \
                KApi_XC_MLoad_WriteCmd_NonXC(E_CLIENT_MAIN_HDR, (ADDR & 0xFFFF00)>>8, (ADDR & 0xFF), ((VAL) & 0xFFFF), MASK);  \
                                                                 \
            } while(0)                          
#else

    
#endif


//-------------------------------------------------------------------------------------------------
    //  Local Defines
    //-------------------------------------------------------------------------------------------------
#define DRVSCA_DBG(x)       //x
#define DBG_DEFEATHERING    0
#define DBG_DEFLICKERING    0
#define DBG_DEBOUNCING      0
#define DBG_DYNAMIC_DNR     0

    // DeFeathering
#define MDP_CNT     2
#define ENABLE_MDP  0   //Motion decrase progressively enable

#define DEFETHERING_LV1_TH              30000
#define DEFETHERING_LV2_TH              5000
#define DEFETHERING_LV3_TH              2000
#define DEFETHERING_LV1_CNT             25
#define DEFETHERING_LV2_CNT             20
#define DEFETHERING_LV3_CNT             5

#define SST_STATIC_CORE_TH_LV1_VALUE    0x14              //ryan update
#define SST_STATIC_CORE_TH_LV2_VALUE    0x0C              //ryan update
#define SST_STATIC_CORE_TH_LV3_VALUE    0x06              //ryan update
#define SST_STATIC_CORE_TH_LV4_VALUE    0x00              //ryan update

#define SST_MOTION_TH_LV1_VALUE         0x03              //YY for SZ feathering 151201
#define SST_MOTION_TH_LV2_VALUE         0x03              //YY for SZ feathering 151201
#define SST_MOTION_TH_LV3_VALUE         0x02              //YY for SZ feathering 151201 
#define SST_MOTION_TH_LV4_VALUE         0x01              //YY for SZ feathering 151201

#define SST_DYNAMIC_CORE_TH_LV1_VALUE   0x0F              //YY for SZ feathering 151201 
#define SST_DYNAMIC_CORE_TH_LV2_VALUE   0x18              //YY for SZ feathering 151201
#define SST_DYNAMIC_CORE_TH_LV3_VALUE   0x1F              //YY for SZ feathering 151201
#define SST_DYNAMIC_CORE_TH_LV4_VALUE   0x3F              //YY for SZ feathering 151201

    // DeFlickering
#define DEFLICKERING_TH                 52000
#define DEFLICKERING_CNT                150

    // DeBouncing
#define DEBOUNCING_TH                   35000
#define DEBOUNCING_CNT                  10

    // Dynamic SNR
#define DYNAMIC_SNR_TH                  2000
#define DYNAMIC_SNR_CNT                 30

    // Dynamic DNR
    //#define DYNAMIC_DNR_TH                  6000

#define DNR_TABLEY_0L_Zero_VALUE        0xDD
#define DNR_TABLEY_0H_Zero_VALUE        0xBD
#define DNR_TABLEY_1L_Zero_VALUE        0x79
#define DNR_TABLEY_1H_Zero_VALUE        0x35
#define DNR_TABLEY_2L_Zero_VALUE        0x11
#define DNR_TABLEY_2H_Zero_VALUE        0x00
#define DNR_TABLEY_3L_Zero_VALUE        0x00
#define DNR_TABLEY_3H_Zero_VALUE        0x00

#define DNR_TABLEY_0L_LV2_VALUE         0xCC
#define DNR_TABLEY_0H_LV2_VALUE         0xAC
#define DNR_TABLEY_1L_LV2_VALUE         0x68
#define DNR_TABLEY_1H_LV2_VALUE         0x24
#define DNR_TABLEY_2L_LV2_VALUE         0x00
#define DNR_TABLEY_2H_LV2_VALUE         0x00
#define DNR_TABLEY_3L_LV2_VALUE         0x00
#define DNR_TABLEY_3H_LV2_VALUE         0x00

#define DNR_TABLEY_0L_LV3_VALUE         0x67
#define DNR_TABLEY_0H_LV3_VALUE         0x45
#define DNR_TABLEY_1L_LV3_VALUE         0x33
#define DNR_TABLEY_1H_LV3_VALUE         0x22
#define DNR_TABLEY_2L_LV3_VALUE         0x11
#define DNR_TABLEY_2H_LV3_VALUE         0x00
#define DNR_TABLEY_3L_LV3_VALUE         0x00
#define DNR_TABLEY_3H_LV3_VALUE         0x00

    // Dynamic Film 22
#define DYNAMIC_FILM22_TH               520000

#define PRJ_MCNR                        TRUE

#define REG_LPLL_11_L                   (0x103100 | 0x11<< 1)
#define REG_LPLL_11_H                   (0x103100 | ((0x11<< 1)+1))
#define REG_LPLL_13_L                   (0x103100 | 0x13<< 1)
#define REG_LPLL_13_H                   (0x103100 | ((0x13<< 1)+1))

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define PQ_FUNCTION                      1
#if PQ_FUNCTION

#define BYTE                                    MS_U8
#define WORD                                    MS_U16
#define DWORD                                   MS_U32

#define REG_SC_BK30_04_L                        (0x133000 | 0x04<< 1)
#define REG_SC_BK30_05_L                        (0x133000 | 0x05<< 1)

#define REG_SC_BK12_3A_L                        (0x131200 | 0x3A<< 1)
#define REG_SC_BK30_07_L                        (0x133000 | (0x07<< 1))
#define REG_FSC_BK76_44_L                        (0x147600 | (0x44<< 1))
#define REG_FSC_BK75_58_L                        (0x147500 | (0x58<< 1))
#define REG_FSC_BK75_59_L                        (0x147500 | (0x59<< 1))
#define REG_FSC_BK4C_25_L                        (0x144C00 | (0x25<< 1))
#define REG_FSC_BK4C_48_L                        (0x144C00 | (0x48<< 1))
#define REG_FSC_BK72_41_L                        (0x147200 | (0x41<< 1))

#define REG_FSC_BK132_4A_L                        (0x413200 | (0x4A<< 1)) //need check again


#define REG_SC_BK12_19_L                        (0x131200 | 0x19<< 1)

#define REG_SC_BK02_70_L                        (0x130200 | (0x70<< 1))
#define REG_SC_BK02_71_L                        (0x130200 | (0x71<< 1))
#define REG_SC_BK02_72_L                        (0x130200 | (0x72<< 1))
#define REG_SC_BK02_73_L                        (0x130200 | (0x73<< 1))
#define REG_SC_BK02_74_L                        (0x130200 | (0x74<< 1))
#define REG_SC_BK02_75_L                        (0x130200 | (0x75<< 1))
#define REG_SC_BK02_76_L                        (0x130200 | (0x76<< 1))
#define REG_SC_BK02_77_L                        (0x130200 | (0x77<< 1))
#define REG_SC_BK02_78_L                        (0x130200 | (0x78<< 1))
#define REG_SC_BK02_79_L                        (0x130200 | (0x79<< 1))
#define REG_SC_BK02_7F_L                        (0x130200 | (0x7F<< 1))

#define REG_SC_BK22_0D_L                        (0x132200 | (0x0D<< 1))
#define REG_SC_BK22_0E_L                        (0x132200 | (0x0E<< 1))

#define REG_FSC_BK19_10_L                        (0x141900 | (0x10<< 1))
#define REG_FSC_BK19_13_L                        (0x141900 | (0x13<< 1))

#define REG_FSC_BK1A_04_L                        (0x141A00 | (0x04<< 1))
#define REG_FSC_BK1A_0B_L                        (0x141A00 | (0x0B<< 1))

#define REG_FSC_BK1B_1C_L                        (0x141B00 | (0x1C<< 1))
#define REG_FSC_BK1B_1D_L                        (0x141B00 | (0x1D<< 1))

#define REG_FSC_BK2C_79_L                        (0x142C00 | (0x79<< 1))

#define REG_FSC_BK55_30_L                        (0x145500 | (0x30<< 1))
#define REG_FSC_BK55_31_L                        (0x145500 | (0x31<< 1))
#define REG_FSC_BK55_32_L                        (0x145500 | (0x32<< 1))
#define REG_FSC_BK55_33_L                        (0x145500 | (0x33<< 1))
#define REG_FSC_BK55_34_L                        (0x145500 | (0x34<< 1))
#define REG_FSC_BK55_35_L                        (0x145500 | (0x35<< 1))
#define REG_FSC_BK55_36_L                        (0x145500 | (0x36<< 1))
#define REG_FSC_BK55_37_L                        (0x145500 | (0x37<< 1))
#define REG_FSC_BK55_38_L                        (0x145500 | (0x38<< 1))
#define REG_FSC_BK55_39_L                        (0x145500 | (0x39<< 1))
#define REG_FSC_BK55_3A_L                        (0x145500 | (0x3A<< 1))
#define REG_FSC_BK55_3B_L                        (0x145500 | (0x3B<< 1))
#define REG_FSC_BK55_3C_L                        (0x145500 | (0x3C<< 1))
#define REG_FSC_BK55_3D_L                        (0x145500 | (0x3D<< 1))
#define REG_FSC_BK55_3E_L                        (0x145500 | (0x3E<< 1))
#define REG_FSC_BK55_3F_L                        (0x145500 | (0x3F<< 1))
#define REG_FSC_BK55_40_L                        (0x145500 | (0x40<< 1))
#define REG_FSC_BK55_41_L                        (0x145500 | (0x41<< 1))
#define REG_FSC_BK55_42_L                        (0x145500 | (0x42<< 1))
#define REG_FSC_BK55_43_L                        (0x145500 | (0x43<< 1))
#define REG_FSC_BK55_44_L                        (0x145500 | (0x44<< 1))
#define REG_FSC_BK55_45_L                        (0x145500 | (0x45<< 1))
#define REG_FSC_BK55_46_L                        (0x145500 | (0x46<< 1))
#define REG_FSC_BK55_47_L                        (0x145500 | (0x47<< 1))
#define REG_FSC_BK55_48_L                        (0x145500 | (0x48<< 1))
#define REG_FSC_BK55_49_L                        (0x145500 | (0x49<< 1))
#define REG_FSC_BK55_4A_L                        (0x145500 | (0x4A<< 1))
#define REG_FSC_BK55_4B_L                        (0x145500 | (0x4B<< 1))
#define REG_FSC_BK55_4C_L                        (0x145500 | (0x4C<< 1))
#define REG_FSC_BK55_4D_L                        (0x145500 | (0x4D<< 1))
#define REG_FSC_BK55_4E_L                        (0x145500 | (0x4E<< 1))
#define REG_FSC_BK55_4F_L                        (0x145500 | (0x4F<< 1))
#define REG_FSC_BK55_50_L                        (0x145500 | (0x50<< 1))
#define REG_FSC_BK55_51_L                        (0x145500 | (0x51<< 1))
#define REG_FSC_BK55_52_L                        (0x145500 | (0x52<< 1))
#define REG_FSC_BK55_53_L                        (0x145500 | (0x53<< 1))
#define REG_FSC_BK55_54_L                        (0x145500 | (0x54<< 1))
#define REG_FSC_BK55_55_L                        (0x145500 | (0x55<< 1))
#define REG_FSC_BK55_56_L                        (0x145500 | (0x56<< 1))
#define REG_FSC_BK55_57_L                        (0x145500 | (0x57<< 1))
#define REG_FSC_BK55_58_L                        (0x145500 | (0x58<< 1))
#define REG_FSC_BK55_59_L                        (0x145500 | (0x59<< 1))
#define REG_FSC_BK55_5A_L                        (0x145500 | (0x5A<< 1))
#define REG_FSC_BK55_5B_L                        (0x145500 | (0x5B<< 1))
#define REG_FSC_BK55_5C_L                        (0x145500 | (0x5C<< 1))
#define REG_FSC_BK55_5D_L                        (0x145500 | (0x5D<< 1))
#define REG_FSC_BK55_5E_L                        (0x145500 | (0x5E<< 1))
#define REG_FSC_BK55_5F_L                        (0x145500 | (0x5F<< 1))
#define REG_FSC_BK55_60_L                        (0x145500 | (0x60<< 1))
#define REG_FSC_BK55_61_L                        (0x145500 | (0x61<< 1))
#define REG_FSC_BK55_62_L                        (0x145500 | (0x62<< 1))
#define REG_FSC_BK55_63_L                        (0x145500 | (0x63<< 1))
#define REG_FSC_BK55_64_L                        (0x145500 | (0x64<< 1))
#define REG_FSC_BK55_65_L                        (0x145500 | (0x65<< 1))
#define REG_FSC_BK55_66_L                        (0x145500 | (0x66<< 1))
#define REG_FSC_BK55_67_L                        (0x145500 | (0x67<< 1))
#define REG_FSC_BK55_68_L                        (0x145500 | (0x68<< 1))
#define REG_FSC_BK55_69_L                        (0x145500 | (0x69<< 1))
#define REG_FSC_BK55_6A_L                        (0x145500 | (0x6A<< 1))
#define REG_FSC_BK55_6B_L                        (0x145500 | (0x6B<< 1))
#define REG_FSC_BK55_6C_L                        (0x145500 | (0x6C<< 1))
#define REG_FSC_BK55_6D_L                        (0x145500 | (0x6D<< 1))
#define REG_FSC_BK55_6E_L                        (0x145500 | (0x6E<< 1))
#define REG_FSC_BK55_6F_L                        (0x145500 | (0x6F<< 1))

#define REG_FSC_BK5F_3F_L                        (0x145F00 | (0x3F<< 1))

#define REG_FSC_BKAC_79_L                        (0x14AC00 | (0x79<< 1))


/*********************Eva modify 2 start********************************************/
#define REG_SC_BK19_10_L                        (0x131900 | 0x10<< 1)
#define REG_SC_BK19_13_L                        (0x131900 | 0x13<< 1)
#define REG_SC_BK19_50_L                        (0x131900 | 0x50<< 1)
#define REG_SC_BK19_51_L                        (0x131900 | 0x51<< 1)
#define REG_SC_BK19_52_L                        (0x131900 | 0x52<< 1)
#define REG_SC_BK19_53_L                        (0x131900 | 0x53<< 1)
#define REG_SC_BK19_54_L                        (0x131900 | 0x54<< 1)
#define REG_SC_BK19_55_L                        (0x131900 | 0x55<< 1)
#define REG_SC_BK19_56_L                        (0x131900 | 0x56<< 1)
#define REG_SC_BK19_57_L                        (0x131900 | 0x57<< 1)
#define REG_SC_BK19_58_L                        (0x131900 | 0x58<< 1)
#define REG_SC_BK19_59_L                        (0x131900 | 0x59<< 1)
#define REG_SC_BK19_5A_L                        (0x131900 | 0x5A<< 1)
#define REG_SC_BK19_5B_L                        (0x131900 | 0x5B<< 1)
#define REG_SC_BK19_5C_L                        (0x131900 | 0x5C<< 1)
#define REG_SC_BK19_5D_L                        (0x131900 | 0x5D<< 1)
#define REG_SC_BK19_5E_L                        (0x131900 | 0x5E<< 1)
#define REG_SC_BK19_5F_L                        (0x131900 | 0x5F<< 1)

#define REG_FSC_BK26_44_L                        (0x142600 | (0x44<< 1))
#define REG_FSC_BK26_45_L                        (0x142600 | (0x45<< 1))
#define REG_FSC_BK26_46_L                        (0x142600 | (0x46<< 1))

#define REG_FSC_BK30_00_L                        (0x143000 | (0x00<< 1))
#define REG_FSC_BK30_01_L                        (0x143000 | (0x01<< 1))
#define REG_FSC_BK30_02_L                        (0x143000 | (0x02<< 1))
#define REG_FSC_BK30_03_L                        (0x143000 | (0x03<< 1))
#define REG_FSC_BK30_04_L                        (0x143000 | (0x04<< 1))
#define REG_FSC_BK30_05_L                        (0x143000 | (0x05<< 1))
#define REG_FSC_BK30_06_L                        (0x143000 | (0x06<< 1))
#define REG_FSC_BK30_07_L                        (0x143000 | (0x07<< 1))
#define REG_FSC_BK30_08_L                        (0x143000 | (0x08<< 1))
#define REG_FSC_BK30_09_L                        (0x143000 | (0x09<< 1))
#define REG_FSC_BK30_0A_L                        (0x143000 | (0x0A<< 1))
#define REG_FSC_BK30_0B_L                        (0x143000 | (0x0B<< 1))
#define REG_FSC_BK30_0C_L                        (0x143000 | (0x0C<< 1))
#define REG_FSC_BK30_0D_L                        (0x143000 | (0x0D<< 1))
#define REG_FSC_BK30_0E_L                        (0x143000 | (0x0E<< 1))
#define REG_FSC_BK30_0F_L                        (0x143000 | (0x0F<< 1))
#define REG_FSC_BK30_10_L                        (0x143000 | (0x10<< 1))
#define REG_FSC_BK30_11_L                        (0x143000 | (0x11<< 1))
#define REG_FSC_BK30_12_L                        (0x143000 | (0x12<< 1))
#define REG_FSC_BK30_13_L                        (0x143000 | (0x13<< 1))
#define REG_FSC_BK30_14_L                        (0x143000 | (0x14<< 1))
#define REG_FSC_BK30_15_L                        (0x143000 | (0x15<< 1))
#define REG_FSC_BK30_16_L                        (0x143000 | (0x16<< 1))
#define REG_FSC_BK30_17_L                        (0x143000 | (0x17<< 1))
#define REG_FSC_BK30_18_L                        (0x143000 | (0x18<< 1))
#define REG_FSC_BK30_19_L                        (0x143000 | (0x19<< 1))
#define REG_FSC_BK30_1A_L                        (0x143000 | (0x1A<< 1))
#define REG_FSC_BK30_1B_L                        (0x143000 | (0x1B<< 1))
#define REG_FSC_BK30_1C_L                        (0x143000 | (0x1C<< 1))
#define REG_FSC_BK30_1D_L                        (0x143000 | (0x1D<< 1))
#define REG_FSC_BK30_1E_L                        (0x143000 | (0x1E<< 1))
#define REG_FSC_BK30_1F_L                        (0x143000 | (0x1F<< 1))
#define REG_FSC_BK30_20_L                        (0x143000 | (0x20<< 1))
#define REG_FSC_BK30_21_L                        (0x143000 | (0x21<< 1))
#define REG_FSC_BK30_22_L                        (0x143000 | (0x22<< 1))
#define REG_FSC_BK30_23_L                        (0x143000 | (0x23<< 1))
#define REG_FSC_BK30_24_L                        (0x143000 | (0x24<< 1))
#define REG_FSC_BK30_25_L                        (0x143000 | (0x25<< 1))
#define REG_FSC_BK30_26_L                        (0x143000 | (0x26<< 1))
#define REG_FSC_BK30_27_L                        (0x143000 | (0x27<< 1))
#define REG_FSC_BK30_28_L                        (0x143000 | (0x28<< 1))
#define REG_FSC_BK30_29_L                        (0x143000 | (0x29<< 1))
#define REG_FSC_BK30_2A_L                        (0x143000 | (0x2A<< 1))
#define REG_FSC_BK30_2B_L                        (0x143000 | (0x2B<< 1))
#define REG_FSC_BK30_2C_L                        (0x143000 | (0x2C<< 1))
#define REG_FSC_BK30_2D_L                        (0x143000 | (0x2D<< 1))
#define REG_FSC_BK30_2E_L                        (0x143000 | (0x2E<< 1))
#define REG_FSC_BK30_2F_L                        (0x143000 | (0x2F<< 1))
#define REG_FSC_BK30_30_L                        (0x143000 | (0x30<< 1))
#define REG_FSC_BK30_31_L                        (0x143000 | (0x31<< 1))
#define REG_FSC_BK30_32_L                        (0x143000 | (0x32<< 1))
#define REG_FSC_BK30_33_L                        (0x143000 | (0x33<< 1))
#define REG_FSC_BK30_34_L                        (0x143000 | (0x34<< 1))
#define REG_FSC_BK30_35_L                        (0x143000 | (0x35<< 1))
#define REG_FSC_BK30_36_L                        (0x143000 | (0x36<< 1))
#define REG_FSC_BK30_37_L                        (0x143000 | (0x37<< 1))
#define REG_FSC_BK30_38_L                        (0x143000 | (0x38<< 1))
#define REG_FSC_BK30_39_L                        (0x143000 | (0x39<< 1))
#define REG_FSC_BK30_3A_L                        (0x143000 | (0x3A<< 1))
#define REG_FSC_BK30_3B_L                        (0x143000 | (0x3B<< 1))
#define REG_FSC_BK30_3C_L                        (0x143000 | (0x3C<< 1))
#define REG_FSC_BK30_3D_L                        (0x143000 | (0x3D<< 1))
#define REG_FSC_BK30_3E_L                        (0x143000 | (0x3E<< 1))
#define REG_FSC_BK30_3F_L                        (0x143000 | (0x3F<< 1))

//FSC BK 4C
#define REG_FSC_BK4C_52_L                        (0x144C00 | (0x52<< 1))
#define REG_FSC_BK4C_53_L                        (0x144C00 | (0x53<< 1))

//FSC BK 4D
#define REG_FSC_BK4D_00_L                        (0x144D00 | (0x00<< 1))
#define REG_FSC_BK4D_01_L                        (0x144D00 | (0x01<< 1))
#define REG_FSC_BK4D_02_L                        (0x144D00 | (0x02<< 1))
#define REG_FSC_BK4D_03_L                        (0x144D00 | (0x03<< 1))
#define REG_FSC_BK4D_04_L                        (0x144D00 | (0x04<< 1))
#define REG_FSC_BK4D_05_L                        (0x144D00 | (0x05<< 1))
#define REG_FSC_BK4D_06_L                        (0x144D00 | (0x06<< 1))
#define REG_FSC_BK4D_07_L                        (0x144D00 | (0x07<< 1))
#define REG_FSC_BK4D_08_L                        (0x144D00 | (0x08<< 1))
#define REG_FSC_BK4D_09_L                        (0x144D00 | (0x09<< 1))
#define REG_FSC_BK4D_0A_L                        (0x144D00 | (0x0A<< 1))
#define REG_FSC_BK4D_0B_L                        (0x144D00 | (0x0B<< 1))
#define REG_FSC_BK4D_0C_L                        (0x144D00 | (0x0C<< 1))
#define REG_FSC_BK4D_0D_L                        (0x144D00 | (0x0D<< 1))
#define REG_FSC_BK4D_0E_L                        (0x144D00 | (0x0E<< 1))
#define REG_FSC_BK4D_0F_L                        (0x144D00 | (0x0F<< 1))
#define REG_FSC_BK4D_10_L                        (0x144D00 | (0x10<< 1))
#define REG_FSC_BK4D_11_L                        (0x144D00 | (0x11<< 1))
#define REG_FSC_BK4D_12_L                        (0x144D00 | (0x12<< 1))
#define REG_FSC_BK4D_13_L                        (0x144D00 | (0x13<< 1))
#define REG_FSC_BK4D_14_L                        (0x144D00 | (0x14<< 1))
#define REG_FSC_BK4D_15_L                        (0x144D00 | (0x15<< 1))
#define REG_FSC_BK4D_16_L                        (0x144D00 | (0x16<< 1))
#define REG_FSC_BK4D_17_L                        (0x144D00 | (0x17<< 1))
#define REG_FSC_BK4D_18_L                        (0x144D00 | (0x18<< 1))
#define REG_FSC_BK4D_19_L                        (0x144D00 | (0x19<< 1))
#define REG_FSC_BK4D_1A_L                        (0x144D00 | (0x1A<< 1))
#define REG_FSC_BK4D_1B_L                        (0x144D00 | (0x1B<< 1))
#define REG_FSC_BK4D_1C_L                        (0x144D00 | (0x1C<< 1))
#define REG_FSC_BK4D_1D_L                        (0x144D00 | (0x1D<< 1))
#define REG_FSC_BK4D_1E_L                        (0x144D00 | (0x1E<< 1))
#define REG_FSC_BK4D_1F_L                        (0x144D00 | (0x1F<< 1))
#define REG_FSC_BK4D_20_L                        (0x144D00 | (0x20<< 1))
#define REG_FSC_BK4D_21_L                        (0x144D00 | (0x21<< 1))
#define REG_FSC_BK4D_22_L                        (0x144D00 | (0x22<< 1))
#define REG_FSC_BK4D_23_L                        (0x144D00 | (0x23<< 1))
#define REG_FSC_BK4D_24_L                        (0x144D00 | (0x24<< 1))
#define REG_FSC_BK4D_25_L                        (0x144D00 | (0x25<< 1))
#define REG_FSC_BK4D_26_L                        (0x144D00 | (0x26<< 1))
#define REG_FSC_BK4D_27_L                        (0x144D00 | (0x27<< 1))
#define REG_FSC_BK4D_28_L                        (0x144D00 | (0x28<< 1))
#define REG_FSC_BK4D_29_L                        (0x144D00 | (0x29<< 1))
#define REG_FSC_BK4D_2A_L                        (0x144D00 | (0x2A<< 1))
#define REG_FSC_BK4D_2B_L                        (0x144D00 | (0x2B<< 1))
#define REG_FSC_BK4D_2C_L                        (0x144D00 | (0x2C<< 1))
#define REG_FSC_BK4D_2D_L                        (0x144D00 | (0x2D<< 1))
#define REG_FSC_BK4D_2E_L                        (0x144D00 | (0x2E<< 1))
#define REG_FSC_BK4D_2F_L                        (0x144D00 | (0x2F<< 1))
#define REG_FSC_BK4D_30_L                        (0x144D00 | (0x30<< 1))
#define REG_FSC_BK4D_31_L                        (0x144D00 | (0x31<< 1))
#define REG_FSC_BK4D_32_L                        (0x144D00 | (0x32<< 1))
#define REG_FSC_BK4D_33_L                        (0x144D00 | (0x33<< 1))
#define REG_FSC_BK4D_34_L                        (0x144D00 | (0x34<< 1))
#define REG_FSC_BK4D_35_L                        (0x144D00 | (0x35<< 1))
#define REG_FSC_BK4D_36_L                        (0x144D00 | (0x36<< 1))
#define REG_FSC_BK4D_37_L                        (0x144D00 | (0x37<< 1))
#define REG_FSC_BK4D_38_L                        (0x144D00 | (0x38<< 1))
#define REG_FSC_BK4D_39_L                        (0x144D00 | (0x39<< 1))
#define REG_FSC_BK4D_3A_L                        (0x144D00 | (0x3A<< 1))
#define REG_FSC_BK4D_3B_L                        (0x144D00 | (0x3B<< 1))
#define REG_FSC_BK4D_3C_L                        (0x144D00 | (0x3C<< 1))
#define REG_FSC_BK4D_3D_L                        (0x144D00 | (0x3D<< 1))
#define REG_FSC_BK4D_3E_L                        (0x144D00 | (0x3E<< 1))
#define REG_FSC_BK4D_3F_L                        (0x144D00 | (0x3F<< 1))
#define REG_FSC_BK4D_40_L                        (0x144D00 | (0x40<< 1))
#define REG_FSC_BK4D_41_L                        (0x144D00 | (0x41<< 1))
#define REG_FSC_BK4D_42_L                        (0x144D00 | (0x42<< 1))
#define REG_FSC_BK4D_43_L                        (0x144D00 | (0x43<< 1))
#define REG_FSC_BK4D_44_L                        (0x144D00 | (0x44<< 1))
#define REG_FSC_BK4D_45_L                        (0x144D00 | (0x45<< 1))
#define REG_FSC_BK4D_46_L                        (0x144D00 | (0x46<< 1))
#define REG_FSC_BK4D_47_L                        (0x144D00 | (0x47<< 1))
#define REG_FSC_BK4D_48_L                        (0x144D00 | (0x48<< 1))
#define REG_FSC_BK4D_49_L                        (0x144D00 | (0x49<< 1))
#define REG_FSC_BK4D_4A_L                        (0x144D00 | (0x4A<< 1))
#define REG_FSC_BK4D_4B_L                        (0x144D00 | (0x4B<< 1))
#define REG_FSC_BK4D_4C_L                        (0x144D00 | (0x4C<< 1))
#define REG_FSC_BK4D_4D_L                        (0x144D00 | (0x4D<< 1))
#define REG_FSC_BK4D_4E_L                        (0x144D00 | (0x4E<< 1))
#define REG_FSC_BK4D_4F_L                        (0x144D00 | (0x4F<< 1))
#define REG_FSC_BK4D_50_L                        (0x144D00 | (0x50<< 1))
#define REG_FSC_BK4D_51_L                        (0x144D00 | (0x51<< 1))
#define REG_FSC_BK4D_52_L                        (0x144D00 | (0x52<< 1))
#define REG_FSC_BK4D_53_L                        (0x144D00 | (0x53<< 1))
#define REG_FSC_BK4D_54_L                        (0x144D00 | (0x54<< 1))
#define REG_FSC_BK4D_55_L                        (0x144D00 | (0x55<< 1))
#define REG_FSC_BK4D_56_L                        (0x144D00 | (0x56<< 1))
#define REG_FSC_BK4D_57_L                        (0x144D00 | (0x57<< 1))
#define REG_FSC_BK4D_58_L                        (0x144D00 | (0x58<< 1))
#define REG_FSC_BK4D_59_L                        (0x144D00 | (0x59<< 1))
#define REG_FSC_BK4D_5A_L                        (0x144D00 | (0x5A<< 1))
#define REG_FSC_BK4D_5B_L                        (0x144D00 | (0x5B<< 1))
#define REG_FSC_BK4D_5C_L                        (0x144D00 | (0x5C<< 1))
#define REG_FSC_BK4D_5D_L                        (0x144D00 | (0x5D<< 1))
#define REG_FSC_BK4D_5E_L                        (0x144D00 | (0x5E<< 1))
#define REG_FSC_BK4D_5F_L                        (0x144D00 | (0x5F<< 1))
#define REG_FSC_BK4D_60_L                        (0x144D00 | (0x60<< 1))
#define REG_FSC_BK4D_61_L                        (0x144D00 | (0x61<< 1))
#define REG_FSC_BK4D_62_L                        (0x144D00 | (0x62<< 1))
#define REG_FSC_BK4D_63_L                        (0x144D00 | (0x63<< 1))
#define REG_FSC_BK4D_64_L                        (0x144D00 | (0x64<< 1))
#define REG_FSC_BK4D_65_L                        (0x144D00 | (0x65<< 1))
#define REG_FSC_BK4D_66_L                        (0x144D00 | (0x66<< 1))
#define REG_FSC_BK4D_67_L                        (0x144D00 | (0x67<< 1))
#define REG_FSC_BK4D_68_L                        (0x144D00 | (0x68<< 1))
#define REG_FSC_BK4D_69_L                        (0x144D00 | (0x69<< 1))
#define REG_FSC_BK4D_6A_L                        (0x144D00 | (0x6A<< 1))
#define REG_FSC_BK4D_6B_L                        (0x144D00 | (0x6B<< 1))
#define REG_FSC_BK4D_6C_L                        (0x144D00 | (0x6C<< 1))
#define REG_FSC_BK4D_6D_L                        (0x144D00 | (0x6D<< 1))
#define REG_FSC_BK4D_6E_L                        (0x144D00 | (0x6E<< 1))
#define REG_FSC_BK4D_6F_L                        (0x144D00 | (0x6F<< 1))
#define REG_FSC_BK4D_70_L                        (0x144D00 | (0x70<< 1))
#define REG_FSC_BK4D_71_L                        (0x144D00 | (0x71<< 1))
#define REG_FSC_BK4D_72_L                        (0x144D00 | (0x72<< 1))
#define REG_FSC_BK4D_73_L                        (0x144D00 | (0x73<< 1))
#define REG_FSC_BK4D_74_L                        (0x144D00 | (0x74<< 1))
#define REG_FSC_BK4D_75_L                        (0x144D00 | (0x75<< 1))
#define REG_FSC_BK4D_76_L                        (0x144D00 | (0x76<< 1))
#define REG_FSC_BK4D_77_L                        (0x144D00 | (0x77<< 1))
#define REG_FSC_BK4D_78_L                        (0x144D00 | (0x78<< 1))
#define REG_FSC_BK4D_79_L                        (0x144D00 | (0x79<< 1))
#define REG_FSC_BK4D_7A_L                        (0x144D00 | (0x7A<< 1))
#define REG_FSC_BK4D_7B_L                        (0x144D00 | (0x7B<< 1))
#define REG_FSC_BK4D_7C_L                        (0x144D00 | (0x7C<< 1))
#define REG_FSC_BK4D_7D_L                        (0x144D00 | (0x7D<< 1))
#define REG_FSC_BK4D_7E_L                        (0x144D00 | (0x7E<< 1))
#define REG_FSC_BK4D_7F_L                        (0x144D00 | (0x7F<< 1))

#define REG_FSC_BK5F_62_L                        (0x145F00 | (0x62<< 1))

#define REG_FSC_BK70_2A_L                        (0x147000 | (0x2A<< 1))
#define REG_FSC_BK70_31_L                        (0x147000 | (0x31<< 1))

#define REG_FSC_BK72_25_L                        (0x147200 | (0x25<< 1))
#define REG_FSC_BK72_26_L                        (0x147200 | (0x26<< 1))
#define REG_FSC_BK72_2D_L                        (0x147200 | (0x2D<< 1))
#define REG_FSC_BK72_33_L                        (0x147200 | (0x33<< 1))
#define REG_FSC_BK72_7A_L                        (0x147200 | (0x7A<< 1))
#define REG_FSC_BK72_7B_L                        (0x147200 | (0x7B<< 1))
#define REG_FSC_BK72_7C_L                        (0x147200 | (0x7C<< 1))
#define REG_FSC_BK72_7D_L                        (0x147200 | (0x7D<< 1))

#define REG_FSC_BK74_2E_L                        (0x147400 | (0x2E<< 1))
#define REG_FSC_BK74_41_L                        (0x147400 | (0x41<< 1))

#define REG_FSC_BK75_1A_L                        (0x147500 | (0x1A<< 1))

#define REG_FSC_BK76_42_L                        (0x147600 | (0x42<< 1))

//FSC BKCD
#define REG_FSC_BKCD_00_L                        (0x14CD00 | (0x00<< 1))
#define REG_FSC_BKCD_01_L                        (0x14CD00 | (0x01<< 1))
#define REG_FSC_BKCD_02_L                        (0x14CD00 | (0x02<< 1))
#define REG_FSC_BKCD_03_L                        (0x14CD00 | (0x03<< 1))
#define REG_FSC_BKCD_04_L                        (0x14CD00 | (0x04<< 1))
#define REG_FSC_BKCD_05_L                        (0x14CD00 | (0x05<< 1))
#define REG_FSC_BKCD_06_L                        (0x14CD00 | (0x06<< 1))
#define REG_FSC_BKCD_07_L                        (0x14CD00 | (0x07<< 1))
#define REG_FSC_BKCD_08_L                        (0x14CD00 | (0x08<< 1))
#define REG_FSC_BKCD_09_L                        (0x14CD00 | (0x09<< 1))
#define REG_FSC_BKCD_0A_L                        (0x14CD00 | (0x0A<< 1))
#define REG_FSC_BKCD_0B_L                        (0x14CD00 | (0x0B<< 1))
#define REG_FSC_BKCD_0C_L                        (0x14CD00 | (0x0C<< 1))
#define REG_FSC_BKCD_0D_L                        (0x14CD00 | (0x0D<< 1))
#define REG_FSC_BKCD_0E_L                        (0x14CD00 | (0x0E<< 1))
#define REG_FSC_BKCD_0F_L                        (0x14CD00 | (0x0F<< 1))
#define REG_FSC_BKCD_10_L                        (0x14CD00 | (0x10<< 1))
#define REG_FSC_BKCD_11_L                        (0x14CD00 | (0x11<< 1))
#define REG_FSC_BKCD_12_L                        (0x14CD00 | (0x12<< 1))
#define REG_FSC_BKCD_13_L                        (0x14CD00 | (0x13<< 1))
#define REG_FSC_BKCD_14_L                        (0x14CD00 | (0x14<< 1))
#define REG_FSC_BKCD_15_L                        (0x14CD00 | (0x15<< 1))
#define REG_FSC_BKCD_16_L                        (0x14CD00 | (0x16<< 1))
#define REG_FSC_BKCD_17_L                        (0x14CD00 | (0x17<< 1))
#define REG_FSC_BKCD_18_L                        (0x14CD00 | (0x18<< 1))
#define REG_FSC_BKCD_19_L                        (0x14CD00 | (0x19<< 1))
#define REG_FSC_BKCD_1A_L                        (0x14CD00 | (0x1A<< 1))
#define REG_FSC_BKCD_1B_L                        (0x14CD00 | (0x1B<< 1))
#define REG_FSC_BKCD_1C_L                        (0x14CD00 | (0x1C<< 1))
#define REG_FSC_BKCD_1D_L                        (0x14CD00 | (0x1D<< 1))
#define REG_FSC_BKCD_1E_L                        (0x14CD00 | (0x1E<< 1))
#define REG_FSC_BKCD_1F_L                        (0x14CD00 | (0x1F<< 1))
#define REG_FSC_BKCD_20_L                        (0x14CD00 | (0x20<< 1))
#define REG_FSC_BKCD_21_L                        (0x14CD00 | (0x21<< 1))
#define REG_FSC_BKCD_22_L                        (0x14CD00 | (0x22<< 1))
#define REG_FSC_BKCD_23_L                        (0x14CD00 | (0x23<< 1))
#define REG_FSC_BKCD_24_L                        (0x14CD00 | (0x24<< 1))
#define REG_FSC_BKCD_25_L                        (0x14CD00 | (0x25<< 1))
#define REG_FSC_BKCD_26_L                        (0x14CD00 | (0x26<< 1))
#define REG_FSC_BKCD_27_L                        (0x14CD00 | (0x27<< 1))
#define REG_FSC_BKCD_28_L                        (0x14CD00 | (0x28<< 1))
#define REG_FSC_BKCD_29_L                        (0x14CD00 | (0x29<< 1))
#define REG_FSC_BKCD_2A_L                        (0x14CD00 | (0x2A<< 1))
#define REG_FSC_BKCD_2B_L                        (0x14CD00 | (0x2B<< 1))
#define REG_FSC_BKCD_2C_L                        (0x14CD00 | (0x2C<< 1))
#define REG_FSC_BKCD_2D_L                        (0x14CD00 | (0x2D<< 1))
#define REG_FSC_BKCD_2E_L                        (0x14CD00 | (0x2E<< 1))
#define REG_FSC_BKCD_2F_L                        (0x14CD00 | (0x2F<< 1))
#define REG_FSC_BKCD_30_L                        (0x14CD00 | (0x30<< 1))
#define REG_FSC_BKCD_31_L                        (0x14CD00 | (0x31<< 1))
#define REG_FSC_BKCD_32_L                        (0x14CD00 | (0x32<< 1))
#define REG_FSC_BKCD_33_L                        (0x14CD00 | (0x33<< 1))
#define REG_FSC_BKCD_34_L                        (0x14CD00 | (0x34<< 1))
#define REG_FSC_BKCD_35_L                        (0x14CD00 | (0x35<< 1))
#define REG_FSC_BKCD_36_L                        (0x14CD00 | (0x36<< 1))
#define REG_FSC_BKCD_37_L                        (0x14CD00 | (0x37<< 1))
#define REG_FSC_BKCD_38_L                        (0x14CD00 | (0x38<< 1))
#define REG_FSC_BKCD_39_L                        (0x14CD00 | (0x39<< 1))
#define REG_FSC_BKCD_3A_L                        (0x14CD00 | (0x3A<< 1))
#define REG_FSC_BKCD_3B_L                        (0x14CD00 | (0x3B<< 1))
#define REG_FSC_BKCD_3C_L                        (0x14CD00 | (0x3C<< 1))
#define REG_FSC_BKCD_3D_L                        (0x14CD00 | (0x3D<< 1))
#define REG_FSC_BKCD_3E_L                        (0x14CD00 | (0x3E<< 1))
#define REG_FSC_BKCD_3F_L                        (0x14CD00 | (0x3F<< 1))
#define REG_FSC_BKCD_40_L                        (0x14CD00 | (0x40<< 1))
#define REG_FSC_BKCD_41_L                        (0x14CD00 | (0x41<< 1))
#define REG_FSC_BKCD_42_L                        (0x14CD00 | (0x42<< 1))
#define REG_FSC_BKCD_43_L                        (0x14CD00 | (0x43<< 1))
#define REG_FSC_BKCD_44_L                        (0x14CD00 | (0x44<< 1))
#define REG_FSC_BKCD_45_L                        (0x14CD00 | (0x45<< 1))
#define REG_FSC_BKCD_46_L                        (0x14CD00 | (0x46<< 1))
#define REG_FSC_BKCD_47_L                        (0x14CD00 | (0x47<< 1))
#define REG_FSC_BKCD_48_L                        (0x14CD00 | (0x48<< 1))
#define REG_FSC_BKCD_49_L                        (0x14CD00 | (0x49<< 1))
#define REG_FSC_BKCD_4A_L                        (0x14CD00 | (0x4A<< 1))
#define REG_FSC_BKCD_4B_L                        (0x14CD00 | (0x4B<< 1))
#define REG_FSC_BKCD_4C_L                        (0x14CD00 | (0x4C<< 1))
#define REG_FSC_BKCD_4D_L                        (0x14CD00 | (0x4D<< 1))
#define REG_FSC_BKCD_4E_L                        (0x14CD00 | (0x4E<< 1))
#define REG_FSC_BKCD_4F_L                        (0x14CD00 | (0x4F<< 1))
#define REG_FSC_BKCD_50_L                        (0x14CD00 | (0x50<< 1))
#define REG_FSC_BKCD_51_L                        (0x14CD00 | (0x51<< 1))
#define REG_FSC_BKCD_52_L                        (0x14CD00 | (0x52<< 1))
#define REG_FSC_BKCD_53_L                        (0x14CD00 | (0x53<< 1))
#define REG_FSC_BKCD_54_L                        (0x14CD00 | (0x54<< 1))
#define REG_FSC_BKCD_55_L                        (0x14CD00 | (0x55<< 1))
#define REG_FSC_BKCD_56_L                        (0x14CD00 | (0x56<< 1))
#define REG_FSC_BKCD_57_L                        (0x14CD00 | (0x57<< 1))
#define REG_FSC_BKCD_58_L                        (0x14CD00 | (0x58<< 1))
#define REG_FSC_BKCD_59_L                        (0x14CD00 | (0x59<< 1))
#define REG_FSC_BKCD_5A_L                        (0x14CD00 | (0x5A<< 1))
#define REG_FSC_BKCD_5B_L                        (0x14CD00 | (0x5B<< 1))
#define REG_FSC_BKCD_5C_L                        (0x14CD00 | (0x5C<< 1))
#define REG_FSC_BKCD_5D_L                        (0x14CD00 | (0x5D<< 1))
#define REG_FSC_BKCD_5E_L                        (0x14CD00 | (0x5E<< 1))
#define REG_FSC_BKCD_5F_L                        (0x14CD00 | (0x5F<< 1))
#define REG_FSC_BKCD_60_L                        (0x14CD00 | (0x60<< 1))
#define REG_FSC_BKCD_61_L                        (0x14CD00 | (0x61<< 1))
#define REG_FSC_BKCD_62_L                        (0x14CD00 | (0x62<< 1))
#define REG_FSC_BKCD_63_L                        (0x14CD00 | (0x63<< 1))
#define REG_FSC_BKCD_64_L                        (0x14CD00 | (0x64<< 1))
#define REG_FSC_BKCD_65_L                        (0x14CD00 | (0x65<< 1))
#define REG_FSC_BKCD_66_L                        (0x14CD00 | (0x66<< 1))
#define REG_FSC_BKCD_67_L                        (0x14CD00 | (0x67<< 1))
#define REG_FSC_BKCD_68_L                        (0x14CD00 | (0x68<< 1))
#define REG_FSC_BKCD_69_L                        (0x14CD00 | (0x69<< 1))
#define REG_FSC_BKCD_6A_L                        (0x14CD00 | (0x6A<< 1))
#define REG_FSC_BKCD_6B_L                        (0x14CD00 | (0x6B<< 1))
#define REG_FSC_BKCD_6C_L                        (0x14CD00 | (0x6C<< 1))
#define REG_FSC_BKCD_6D_L                        (0x14CD00 | (0x6D<< 1))
#define REG_FSC_BKCD_6E_L                        (0x14CD00 | (0x6E<< 1))
#define REG_FSC_BKCD_6F_L                        (0x14CD00 | (0x6F<< 1))
#define REG_FSC_BKCD_70_L                        (0x14CD00 | (0x70<< 1))
#define REG_FSC_BKCD_71_L                        (0x14CD00 | (0x71<< 1))
#define REG_FSC_BKCD_72_L                        (0x14CD00 | (0x72<< 1))
#define REG_FSC_BKCD_73_L                        (0x14CD00 | (0x73<< 1))
#define REG_FSC_BKCD_74_L                        (0x14CD00 | (0x74<< 1))
#define REG_FSC_BKCD_75_L                        (0x14CD00 | (0x75<< 1))
#define REG_FSC_BKCD_76_L                        (0x14CD00 | (0x76<< 1))
#define REG_FSC_BKCD_77_L                        (0x14CD00 | (0x77<< 1))
#define REG_FSC_BKCD_78_L                        (0x14CD00 | (0x78<< 1))
#define REG_FSC_BKCD_79_L                        (0x14CD00 | (0x79<< 1))
#define REG_FSC_BKCD_7A_L                        (0x14CD00 | (0x7A<< 1))
#define REG_FSC_BKCD_7B_L                        (0x14CD00 | (0x7B<< 1))
#define REG_FSC_BKCD_7C_L                        (0x14CD00 | (0x7C<< 1))
#define REG_FSC_BKCD_7D_L                        (0x14CD00 | (0x7D<< 1))
#define REG_FSC_BKCD_7E_L                        (0x14CD00 | (0x7E<< 1))
#define REG_FSC_BKCD_7F_L                        (0x14CD00 | (0x7F<< 1))

#define REG_FSC_BKF2_33_L                        (0x14F200 | (0x33<< 1))
#define REG_FSC_BKF2_7A_L                        (0x14F200 | (0x7A<< 1))
#define REG_FSC_BKF2_7B_L                        (0x14F200 | (0x7B<< 1))
#define REG_FSC_BKF2_7C_L                        (0x14F200 | (0x7C<< 1))
#define REG_FSC_BKF2_7D_L                        (0x14F200 | (0x7D<< 1))

//===================================== register about film or dms ...etc =============
//SC
#define REG_SC_BK00_10_L                        (0x130000 | (0x10<< 1))
#define REG_SC_BK00_12_L                        (0x130000 | (0x12<< 1))

#define REG_SC_BK01_00_L                        (0x130100 | 0x00<< 1)
#define REG_SC_BK01_01_L                        (0x130100 | 0x01<< 1)
#define REG_SC_BK01_02_L                        (0x130100 | 0x02<< 1)
#define REG_SC_BK01_03_L                        (0x130100 | 0x03<< 1)
#define REG_SC_BK01_04_L                        (0x130100 | 0x04<< 1)
#define REG_SC_BK01_05_L                        (0x130100 | 0x05<< 1)
#define REG_SC_BK01_06_L                        (0x130100 | 0x06<< 1)
#define REG_SC_BK01_07_L                        (0x130100 | 0x07<< 1)
#define REG_SC_BK01_08_L                        (0x130100 | 0x08<< 1)
#define REG_SC_BK01_09_L                        (0x130100 | 0x09<< 1)
#define REG_SC_BK01_0A_L                        (0x130100 | 0x0A<< 1)
#define REG_SC_BK01_0B_L                        (0x130100 | 0x0B<< 1)
#define REG_SC_BK01_0C_L                        (0x130100 | 0x0C<< 1)
#define REG_SC_BK01_0D_L                        (0x130100 | 0x0D<< 1)
#define REG_SC_BK01_0E_L                        (0x130100 | 0x0E<< 1)
#define REG_SC_BK01_0F_L                        (0x130100 | 0x0F<< 1)
#define REG_SC_BK01_10_L                        (0x130100 | 0x10<< 1)
#define REG_SC_BK01_11_L                        (0x130100 | 0x11<< 1)
#define REG_SC_BK01_12_L                        (0x130100 | 0x12<< 1)
#define REG_SC_BK01_13_L                        (0x130100 | 0x13<< 1)
#define REG_SC_BK01_14_L                        (0x130100 | 0x14<< 1)
#define REG_SC_BK01_15_L                        (0x130100 | 0x15<< 1)
#define REG_SC_BK01_16_L                        (0x130100 | 0x16<< 1)
#define REG_SC_BK01_17_L                        (0x130100 | 0x17<< 1)
#define REG_SC_BK01_18_L                        (0x130100 | 0x18<< 1)
#define REG_SC_BK01_19_L                        (0x130100 | 0x19<< 1)
#define REG_SC_BK01_1A_L                        (0x130100 | 0x1A<< 1)
#define REG_SC_BK01_1B_L                        (0x130100 | 0x1B<< 1)
#define REG_SC_BK01_1C_L                        (0x130100 | 0x1C<< 1)
#define REG_SC_BK01_1D_L                        (0x130100 | 0x1D<< 1)
#define REG_SC_BK01_1E_L                        (0x130100 | 0x1E<< 1)
#define REG_SC_BK01_1F_L                        (0x130100 | 0x1F<< 1)
#define REG_SC_BK01_20_L                        (0x130100 | 0x20<< 1)
#define REG_SC_BK01_21_L                        (0x130100 | 0x21<< 1)
#define REG_SC_BK01_22_L                        (0x130100 | 0x22<< 1)
#define REG_SC_BK01_23_L                        (0x130100 | 0x23<< 1)
#define REG_SC_BK01_24_L                        (0x130100 | 0x24<< 1)
#define REG_SC_BK01_25_L                        (0x130100 | 0x25<< 1)
#define REG_SC_BK01_26_L                        (0x130100 | 0x26<< 1)
#define REG_SC_BK01_27_L                        (0x130100 | 0x27<< 1)
#define REG_SC_BK01_28_L                        (0x130100 | 0x28<< 1)
#define REG_SC_BK01_29_L                        (0x130100 | 0x29<< 1)
#define REG_SC_BK01_2A_L                        (0x130100 | 0x2A<< 1)
#define REG_SC_BK01_2B_L                        (0x130100 | 0x2B<< 1)
#define REG_SC_BK01_2C_L                        (0x130100 | 0x2C<< 1)
#define REG_SC_BK01_2D_L                        (0x130100 | 0x2D<< 1)
#define REG_SC_BK01_2E_L                        (0x130100 | 0x2E<< 1)
#define REG_SC_BK01_2F_L                        (0x130100 | 0x2F<< 1)

#define REG_SC_BK02_7C_L                        (0x130200 | 0x7C<< 1)
#define REG_SC_BK02_7D_L                        (0x130200 | 0x7D<< 1)

#define REG_SC_BK05_22_L                        (0x130500 | 0x22<< 1)
#define REG_SC_BK05_26_L                        (0x130500 | 0x26<< 1)
#define REG_SC_BK05_2B_L                        (0x130500 | 0x2B<< 1)
#define REG_SC_BK05_53_L                        (0x130500 | 0x53<< 1)

#define REG_SC_BK06_77_L                        (0x130600 | 0x77<< 1)

#define REG_SC_BK08_01_L                        (0x130800 | 0x01<< 1)

#define REG_SC_BK09_5C_L                        (0x130900 | 0x5C<< 1)
#define REG_SC_BK09_60_L                        (0x130900 | 0x60<< 1)
#define REG_SC_BK09_61_L                        (0x130900 | 0x61<< 1)
#define REG_SC_BK09_62_L                        (0x130900 | 0x62<< 1)
#define REG_SC_BK09_63_L                        (0x130900 | 0x63<< 1)
#define REG_SC_BK09_64_L                        (0x130900 | 0x64<< 1)
#define REG_SC_BK09_65_L                        (0x130900 | 0x65<< 1)
#define REG_SC_BK09_66_L                        (0x130900 | 0x66<< 1)
#define REG_SC_BK09_67_L                        (0x130900 | 0x67<< 1)
#define REG_SC_BK09_68_L                        (0x130900 | 0x68<< 1)
#define REG_SC_BK09_69_L                        (0x130900 | 0x69<< 1)
#define REG_SC_BK09_6A_L                        (0x130900 | 0x6A<< 1)
#define REG_SC_BK09_6B_L                        (0x130900 | 0x6B<< 1)
#define REG_SC_BK09_6C_L                        (0x130900 | 0x6C<< 1)
#define REG_SC_BK09_6D_L                        (0x130900 | 0x6D<< 1)
#define REG_SC_BK09_6E_L                        (0x130900 | 0x6E<< 1)
#define REG_SC_BK09_6F_L                        (0x130900 | 0x6F<< 1)
#define REG_SC_BK09_70_L                        (0x130900 | 0x70<< 1)
#define REG_SC_BK09_71_L                        (0x130900 | 0x71<< 1)
#define REG_SC_BK09_72_L                        (0x130900 | 0x72<< 1)
#define REG_SC_BK09_73_L                        (0x130900 | 0x73<< 1)
#define REG_SC_BK09_74_L                        (0x130900 | 0x74<< 1)
#define REG_SC_BK09_75_L                        (0x130900 | 0x75<< 1)
#define REG_SC_BK09_76_L                        (0x130900 | 0x76<< 1)
#define REG_SC_BK09_77_L                        (0x130900 | 0x77<< 1)
#define REG_SC_BK09_78_L                        (0x130900 | 0x78<< 1)
#define REG_SC_BK09_79_L                        (0x130900 | 0x79<< 1)
#define REG_SC_BK09_7A_L                        (0x130900 | 0x7A<< 1)
#define REG_SC_BK09_7B_L                        (0x130900 | 0x7B<< 1)
#define REG_SC_BK09_7C_L                        (0x130900 | 0x7C<< 1)
#define REG_SC_BK09_7D_L                        (0x130900 | 0x7D<< 1)
#define REG_SC_BK09_7E_L                        (0x130900 | 0x7E<< 1)
#define REG_SC_BK09_7F_L                        (0x130900 | 0x7F<< 1)

#define REG_SC_BK0A_1A_L                        (0x130A00 | 0x1A<< 1)
#define REG_SC_BK0A_2E_L                        (0x130A00 | 0x2E<< 1)

#define REG_SC_BK0B_05_L                        (0x130B00 | 0x05<< 1)
#define REG_SC_BK0B_0D_L                        (0x130B00 | 0x0D<< 1)
#define REG_SC_BK0B_0E_L                        (0x130B00 | 0x0E<< 1)
#define REG_SC_BK0B_0F_L                        (0x130B00 | 0x0F<< 1)
#define REG_SC_BK0B_10_L                        (0x130B00 | 0x10<< 1)
#define REG_SC_BK0B_15_L                        (0x130B00 | 0x15<< 1)
#define REG_SC_BK0B_16_L                        (0x130B00 | 0x16<< 1)

#define REG_SC_BK0C_00_L                        (0x130C00 | 0x00<< 1)
#define REG_SC_BK0C_01_L                        (0x130C00 | 0x01<< 1)
#define REG_SC_BK0C_02_L                        (0x130C00 | 0x02<< 1)
#define REG_SC_BK0C_03_L                        (0x130C00 | 0x03<< 1)
#define REG_SC_BK0C_04_L                        (0x130C00 | 0x04<< 1)
#define REG_SC_BK0C_05_L                        (0x130C00 | 0x05<< 1)
#define REG_SC_BK0C_06_L                        (0x130C00 | 0x06<< 1)
#define REG_SC_BK0C_07_L                        (0x130C00 | 0x07<< 1)
#define REG_SC_BK0C_08_L                        (0x130C00 | 0x08<< 1)
#define REG_SC_BK0C_09_L                        (0x130C00 | 0x09<< 1)
#define REG_SC_BK0C_0A_L                        (0x130C00 | 0x0A<< 1)
#define REG_SC_BK0C_0B_L                        (0x130C00 | 0x0B<< 1)
#define REG_SC_BK0C_0C_L                        (0x130C00 | 0x0C<< 1)
#define REG_SC_BK0C_0D_L                        (0x130C00 | 0x0D<< 1)
#define REG_SC_BK0C_0E_L                        (0x130C00 | 0x0E<< 1)
#define REG_SC_BK0C_0F_L                        (0x130C00 | 0x0F<< 1)
#define REG_SC_BK0C_10_L                        (0x130C00 | 0x10<< 1)
#define REG_SC_BK0C_11_L                        (0x130C00 | 0x11<< 1)
#define REG_SC_BK0C_12_L                        (0x130C00 | 0x12<< 1)
#define REG_SC_BK0C_13_L                        (0x130C00 | 0x13<< 1)
#define REG_SC_BK0C_14_L                        (0x130C00 | 0x14<< 1)
#define REG_SC_BK0C_15_L                        (0x130C00 | 0x15<< 1)
#define REG_SC_BK0C_16_L                        (0x130C00 | 0x16<< 1)
#define REG_SC_BK0C_17_L                        (0x130C00 | 0x17<< 1)
#define REG_SC_BK0C_18_L                        (0x130C00 | 0x18<< 1)
#define REG_SC_BK0C_19_L                        (0x130C00 | 0x19<< 1)
#define REG_SC_BK0C_1A_L                        (0x130C00 | 0x1A<< 1)
#define REG_SC_BK0C_1B_L                        (0x130C00 | 0x1B<< 1)
#define REG_SC_BK0C_1C_L                        (0x130C00 | 0x1C<< 1)
#define REG_SC_BK0C_1D_L                        (0x130C00 | 0x1D<< 1)
#define REG_SC_BK0C_1E_L                        (0x130C00 | 0x1E<< 1)
#define REG_SC_BK0C_1F_L                        (0x130C00 | 0x1F<< 1)
#define REG_SC_BK0C_30_L                        (0x130C00 | 0x30<< 1)
#define REG_SC_BK0C_31_L                        (0x130C00 | 0x31<< 1)
#define REG_SC_BK0C_32_L                        (0x130C00 | 0x32<< 1)
#define REG_SC_BK0C_33_L                        (0x130C00 | 0x33<< 1)
#define REG_SC_BK0C_34_L                        (0x130C00 | 0x34<< 1)
#define REG_SC_BK0C_35_L                        (0x130C00 | 0x35<< 1)
#define REG_SC_BK0C_36_L                        (0x130C00 | 0x36<< 1)
#define REG_SC_BK0C_37_L                        (0x130C00 | 0x37<< 1)
#define REG_SC_BK0C_38_L                        (0x130C00 | 0x38<< 1)
#define REG_SC_BK0C_39_L                        (0x130C00 | 0x39<< 1)
#define REG_SC_BK0C_3A_L                        (0x130C00 | 0x3A<< 1)
#define REG_SC_BK0C_3B_L                        (0x130C00 | 0x3B<< 1)
#define REG_SC_BK0C_3C_L                        (0x130C00 | 0x3C<< 1)
#define REG_SC_BK0C_3D_L                        (0x130C00 | 0x3D<< 1)
#define REG_SC_BK0C_3E_L                        (0x130C00 | 0x3E<< 1)
#define REG_SC_BK0C_3F_L                        (0x130C00 | 0x3F<< 1)
#define REG_SC_BK0C_40_L                        (0x130C00 | 0x40<< 1)
#define REG_SC_BK0C_41_L                        (0x130C00 | 0x41<< 1)
#define REG_SC_BK0C_42_L                        (0x130C00 | 0x42<< 1)
#define REG_SC_BK0C_43_L                        (0x130C00 | 0x43<< 1)
#define REG_SC_BK0C_44_L                        (0x130C00 | 0x44<< 1)
#define REG_SC_BK0C_45_L                        (0x130C00 | 0x45<< 1)
#define REG_SC_BK0C_46_L                        (0x130C00 | 0x46<< 1)
#define REG_SC_BK0C_47_L                        (0x130C00 | 0x47<< 1)
#define REG_SC_BK0C_48_L                        (0x130C00 | 0x48<< 1)
#define REG_SC_BK0C_49_L                        (0x130C00 | 0x49<< 1)
#define REG_SC_BK0C_4A_L                        (0x130C00 | 0x4A<< 1)
#define REG_SC_BK0C_4B_L                        (0x130C00 | 0x4B<< 1)
#define REG_SC_BK0C_4C_L                        (0x130C00 | 0x4C<< 1)
#define REG_SC_BK0C_4D_L                        (0x130C00 | 0x4D<< 1)
#define REG_SC_BK0C_4E_L                        (0x130C00 | 0x4E<< 1)
#define REG_SC_BK0C_4F_L                        (0x130C00 | 0x4F<< 1)
#define REG_SC_BK0C_71_L                        (0x130C00 | 0x71<< 1)
#define REG_SC_BK0C_72_L                        (0x130C00 | 0x72<< 1)




#define REG_SC_BK17_48_L                        (0x131700 | 0x48<< 1)
#define REG_SC_BK17_49_L                        (0x131700 | 0x49<< 1)
#define REG_SC_BK17_4A_L                        (0x131700 | 0x4A<< 1)
#define REG_SC_BK17_4B_L                        (0x131700 | 0x4B<< 1)
#define REG_SC_BK17_4C_L                        (0x131700 | 0x4C<< 1)
#define REG_SC_BK17_4D_L                        (0x131700 | 0x4D<< 1)
#define REG_SC_BK17_50_L                        (0x131700 | 0x50<< 1)
#define REG_SC_BK17_51_L                        (0x131700 | 0x51<< 1)
#define REG_SC_BK17_52_L                        (0x131700 | 0x52<< 1)
#define REG_SC_BK17_53_L                        (0x131700 | 0x53<< 1)
#define REG_SC_BK17_54_L                        (0x131700 | 0x54<< 1)
#define REG_SC_BK17_55_L                        (0x131700 | 0x55<< 1)
#define REG_SC_BK17_56_L                        (0x131700 | 0x56<< 1)
#define REG_SC_BK17_57_L                        (0x131700 | 0x57<< 1)
#define REG_SC_BK17_58_L                        (0x131700 | 0x58<< 1)
#define REG_SC_BK17_59_L                        (0x131700 | 0x59<< 1)
#define REG_SC_BK17_5A_L                        (0x131700 | 0x5A<< 1)
#define REG_SC_BK17_5B_L                        (0x131700 | 0x5B<< 1)
#define REG_SC_BK17_5C_L                        (0x131700 | 0x5C<< 1)
#define REG_SC_BK17_5D_L                        (0x131700 | 0x5D<< 1)
#define REG_SC_BK17_5E_L                        (0x131700 | 0x5E<< 1)
#define REG_SC_BK17_5F_L                        (0x131700 | 0x5F<< 1)
#define REG_SC_BK17_60_L                        (0x131700 | 0x60<< 1)
#define REG_SC_BK17_61_L                        (0x131700 | 0x61<< 1)
#define REG_SC_BK17_62_L                        (0x131700 | 0x62<< 1)
#define REG_SC_BK17_63_L                        (0x131700 | 0x63<< 1)
#define REG_SC_BK17_64_L                        (0x131700 | 0x64<< 1)
#define REG_SC_BK17_65_L                        (0x131700 | 0x65<< 1)
#define REG_SC_BK17_66_L                        (0x131700 | 0x66<< 1)
#define REG_SC_BK17_67_L                        (0x131700 | 0x67<< 1)
#define REG_SC_BK17_68_L                        (0x131700 | 0x68<< 1)
#define REG_SC_BK17_69_L                        (0x131700 | 0x69<< 1)
#define REG_SC_BK17_6A_L                        (0x131700 | 0x6A<< 1)
#define REG_SC_BK17_6B_L                        (0x131700 | 0x6B<< 1)
#define REG_SC_BK17_6C_L                        (0x131700 | 0x6C<< 1)
#define REG_SC_BK17_79_L                        (0x131700 | 0x79<< 1)

#define REG_SC_BK20_10_L                        (0x132000 | 0x10<< 1)
#define REG_SC_BK21_3D_L                        (0x132100 | 0x3D<< 1)

#define REG_SC_BK22_0A_L                        (0x132200 | 0x0A<< 1)
#define REG_SC_BK22_0E_L                        (0x132200 | 0x0E<< 1)
#define REG_SC_BK22_10_L                        (0x132200 | 0x10<< 1)
#define REG_SC_BK22_18_L                        (0x132200 | 0x18<< 1)
#define REG_SC_BK22_19_L                        (0x132200 | 0x19<< 1)
#define REG_SC_BK22_1A_L                        (0x132200 | 0x1A<< 1)
#define REG_SC_BK22_1E_L                        (0x132200 | 0x1E<< 1)
#define REG_SC_BK22_28_L                        (0x132200 | 0x28<< 1)
#define REG_SC_BK22_29_L                        (0x132200 | 0x29<< 1)
#define REG_SC_BK22_77_L                        (0x132200 | 0x77<< 1)
#define REG_SC_BK22_78_L                        (0x132200 | 0x78<< 1)

#define REG_SC_BK23_54_L                        (0x132300 | 0x54<< 1)
#define REG_SC_BK23_56_L                        (0x132300 | 0x56<< 1)
#define REG_SC_BK23_5C_L                        (0x132300 | 0x5C<< 1)
#define REG_SC_BK23_5D_L                        (0x132300 | 0x5D<< 1)
#define REG_SC_BK23_6C_L                        (0x132300 | 0x6C<< 1)
#define REG_SC_BK23_6D_L                        (0x132300 | 0x6D<< 1)

#define REG_SC_BK26_10_L                        (0x132600 | 0x10<< 1)


#define REG_SC_BK2A_00_L                        (0x132A00 | 0x00<< 1)
#define REG_SC_BK2A_01_L                        (0x132A00 | 0x01<< 1)
#define REG_SC_BK2A_02_L                        (0x132A00 | 0x02<< 1)
#define REG_SC_BK2A_03_L                        (0x132A00 | 0x03<< 1)
#define REG_SC_BK2A_04_L                        (0x132A00 | 0x04<< 1)
#define REG_SC_BK2A_05_L                        (0x132A00 | 0x05<< 1)
#define REG_SC_BK2A_06_L                        (0x132A00 | 0x06<< 1)
#define REG_SC_BK2A_07_L                        (0x132A00 | 0x07<< 1)
#define REG_SC_BK2A_08_L                        (0x132A00 | 0x08<< 1)
#define REG_SC_BK2A_09_L                        (0x132A00 | 0x09<< 1)
#define REG_SC_BK2A_0A_L                        (0x132A00 | 0x0A<< 1)
#define REG_SC_BK2A_0B_L                        (0x132A00 | 0x0B<< 1)
#define REG_SC_BK2A_0C_L                        (0x132A00 | 0x0C<< 1)
#define REG_SC_BK2A_0D_L                        (0x132A00 | 0x0D<< 1)
#define REG_SC_BK2A_0E_L                        (0x132A00 | 0x0E<< 1)
#define REG_SC_BK2A_0F_L                        (0x132A00 | 0x0F<< 1)
#define REG_SC_BK2A_10_L                        (0x132A00 | 0x10<< 1)
#define REG_SC_BK2A_11_L                        (0x132A00 | 0x11<< 1)
#define REG_SC_BK2A_12_L                        (0x132A00 | 0x12<< 1)
#define REG_SC_BK2A_13_L                        (0x132A00 | 0x13<< 1)
#define REG_SC_BK2A_14_L                        (0x132A00 | 0x14<< 1)
#define REG_SC_BK2A_15_L                        (0x132A00 | 0x15<< 1)
#define REG_SC_BK2A_16_L                        (0x132A00 | 0x16<< 1)
#define REG_SC_BK2A_17_L                        (0x132A00 | 0x17<< 1)
#define REG_SC_BK2A_18_L                        (0x132A00 | 0x18<< 1)
#define REG_SC_BK2A_19_L                        (0x132A00 | 0x19<< 1)
#define REG_SC_BK2A_1A_L                        (0x132A00 | 0x1A<< 1)
#define REG_SC_BK2A_1B_L                        (0x132A00 | 0x1B<< 1)
#define REG_SC_BK2A_1C_L                        (0x132A00 | 0x1C<< 1)
#define REG_SC_BK2A_1D_L                        (0x132A00 | 0x1D<< 1)
#define REG_SC_BK2A_1E_L                        (0x132A00 | 0x1E<< 1)
#define REG_SC_BK2A_1F_L                        (0x132A00 | 0x1F<< 1)
#define REG_SC_BK2A_20_L                        (0x132A00 | 0x20<< 1)
#define REG_SC_BK2A_21_L                        (0x132A00 | 0x21<< 1)
#define REG_SC_BK2A_22_L                        (0x132A00 | 0x22<< 1)
#define REG_SC_BK2A_23_L                        (0x132A00 | 0x23<< 1)
#define REG_SC_BK2A_24_L                        (0x132A00 | 0x24<< 1)
#define REG_SC_BK2A_25_L                        (0x132A00 | 0x25<< 1)
#define REG_SC_BK2A_26_L                        (0x132A00 | 0x26<< 1)
#define REG_SC_BK2A_27_L                        (0x132A00 | 0x27<< 1)
#define REG_SC_BK2A_28_L                        (0x132A00 | 0x28<< 1)
#define REG_SC_BK2A_29_L                        (0x132A00 | 0x29<< 1)
#define REG_SC_BK2A_2A_L                        (0x132A00 | 0x2A<< 1)
#define REG_SC_BK2A_2B_L                        (0x132A00 | 0x2B<< 1)
#define REG_SC_BK2A_2C_L                        (0x132A00 | 0x2C<< 1)
#define REG_SC_BK2A_2D_L                        (0x132A00 | 0x2D<< 1)
#define REG_SC_BK2A_2E_L                        (0x132A00 | 0x2E<< 1)
#define REG_SC_BK2A_2F_L                        (0x132A00 | 0x2F<< 1)
#define REG_SC_BK2A_36_L                        (0x132A00 | 0x36<< 1)
#define REG_SC_BK2A_3B_L                        (0x132A00 | 0x3B<< 1)
#define REG_SC_BK2A_40_L                        (0x132A00 | 0x40<< 1)
#define REG_SC_BK2A_41_L                        (0x132A00 | 0x41<< 1)
#define REG_SC_BK2A_42_L                        (0x132A00 | 0x42<< 1)
#define REG_SC_BK2A_43_L                        (0x132A00 | 0x43<< 1)
#define REG_SC_BK2A_44_L                        (0x132A00 | 0x44<< 1)
#define REG_SC_BK2A_45_L                        (0x132A00 | 0x45<< 1)
#define REG_SC_BK2A_46_L                        (0x132A00 | 0x46<< 1)
#define REG_SC_BK2A_47_L                        (0x132A00 | 0x47<< 1)
#define REG_SC_BK2A_48_L                        (0x132A00 | 0x48<< 1)
#define REG_SC_BK2A_49_L                        (0x132A00 | 0x49<< 1)
#define REG_SC_BK2A_4A_L                        (0x132A00 | 0x4A<< 1)
#define REG_SC_BK2A_4B_L                        (0x132A00 | 0x4B<< 1)
#define REG_SC_BK2A_4C_L                        (0x132A00 | 0x4C<< 1)
#define REG_SC_BK2A_4D_L                        (0x132A00 | 0x4D<< 1)
#define REG_SC_BK2A_4E_L                        (0x132A00 | 0x4E<< 1)
#define REG_SC_BK2A_4F_L                        (0x132A00 | 0x4F<< 1)
#define REG_SC_BK2A_50_L                        (0x132A00 | 0x50<< 1)
#define REG_SC_BK2A_51_L                        (0x132A00 | 0x51<< 1)
#define REG_SC_BK2A_52_L                        (0x132A00 | 0x52<< 1)
#define REG_SC_BK2A_53_L                        (0x132A00 | 0x53<< 1)
#define REG_SC_BK2A_54_L                        (0x132A00 | 0x54<< 1)
#define REG_SC_BK2A_55_L                        (0x132A00 | 0x55<< 1)
#define REG_SC_BK2A_56_L                        (0x132A00 | 0x56<< 1)
#define REG_SC_BK2A_57_L                        (0x132A00 | 0x57<< 1)
#define REG_SC_BK2A_58_L                        (0x132A00 | 0x58<< 1)
#define REG_SC_BK2A_59_L                        (0x132A00 | 0x59<< 1)
#define REG_SC_BK2A_5A_L                        (0x132A00 | 0x5A<< 1)
#define REG_SC_BK2A_5B_L                        (0x132A00 | 0x5B<< 1)
#define REG_SC_BK2A_5C_L                        (0x132A00 | 0x5C<< 1)
#define REG_SC_BK2A_5D_L                        (0x132A00 | 0x5D<< 1)
#define REG_SC_BK2A_5E_L                        (0x132A00 | 0x5E<< 1)
#define REG_SC_BK2A_5F_L                        (0x132A00 | 0x5F<< 1)
#define REG_SC_BK2A_60_L                        (0x132A00 | 0x60<< 1)
#define REG_SC_BK2A_61_L                        (0x132A00 | 0x61<< 1)
#define REG_SC_BK2A_62_L                        (0x132A00 | 0x62<< 1)
#define REG_SC_BK2A_63_L                        (0x132A00 | 0x63<< 1)
#define REG_SC_BK2A_64_L                        (0x132A00 | 0x64<< 1)
#define REG_SC_BK2A_65_L                        (0x132A00 | 0x65<< 1)
#define REG_SC_BK2A_66_L                        (0x132A00 | 0x66<< 1)
#define REG_SC_BK2A_67_L                        (0x132A00 | 0x67<< 1)
#define REG_SC_BK2A_68_L                        (0x132A00 | 0x68<< 1)
#define REG_SC_BK2A_69_L                        (0x132A00 | 0x69<< 1)
#define REG_SC_BK2A_6A_L                        (0x132A00 | 0x6A<< 1)
#define REG_SC_BK2A_6B_L                        (0x132A00 | 0x6B<< 1)
#define REG_SC_BK2A_6C_L                        (0x132A00 | 0x6C<< 1)
#define REG_SC_BK2A_6D_L                        (0x132A00 | 0x6D<< 1)
#define REG_SC_BK2A_6E_L                        (0x132A00 | 0x6E<< 1)
#define REG_SC_BK2A_6F_L                        (0x132A00 | 0x6F<< 1)
#define REG_SC_BK2A_75_L                        (0x132A00 | 0x75<< 1)
#define REG_SC_BK2A_76_L                        (0x132A00 | 0x76<< 1)
#define REG_SC_BK2A_77_L                        (0x132A00 | 0x77<< 1)
#define REG_SC_BK2A_78_L                        (0x132A00 | 0x78<< 1)
#define REG_SC_BK2A_7B_L                        (0x132A00 | 0x7B<< 1)
#define REG_SC_BK2A_7F_L                        (0x132A00 | 0x7F<< 1)

#define REG_SC_BK30_00_L                        (0x133000 | 0x00<< 1)
#define REG_SC_BK30_01_L                        (0x133000 | 0x01<< 1)
#define REG_SC_BK30_02_L                        (0x133000 | 0x02<< 1)
#define REG_SC_BK30_03_L                        (0x133000 | 0x03<< 1)
#define REG_SC_BK30_04_L                        (0x133000 | 0x04<< 1)
#define REG_SC_BK30_05_L                        (0x133000 | 0x05<< 1)
#define REG_SC_BK30_06_L                        (0x133000 | 0x06<< 1)
#define REG_SC_BK30_07_L                        (0x133000 | 0x07<< 1)
#define REG_SC_BK30_08_L                        (0x133000 | 0x08<< 1)
#define REG_SC_BK30_09_L                        (0x133000 | 0x09<< 1)
#define REG_SC_BK30_0A_L                        (0x133000 | 0x0A<< 1)
#define REG_SC_BK30_0B_L                        (0x133000 | 0x0B<< 1)
#define REG_SC_BK30_0C_L                        (0x133000 | 0x0C<< 1)
#define REG_SC_BK30_0D_L                        (0x133000 | 0x0D<< 1)
#define REG_SC_BK30_0E_L                        (0x133000 | 0x0E<< 1)
#define REG_SC_BK30_0F_L                        (0x133000 | 0x0F<< 1)
#define REG_SC_BK30_30_L                        (0x133000 | 0x30<< 1)
#define REG_SC_BK30_31_L                        (0x133000 | 0x31<< 1)
#define REG_SC_BK30_32_L                        (0x133000 | 0x32<< 1)
#define REG_SC_BK30_33_L                        (0x133000 | 0x33<< 1)
#define REG_SC_BK30_34_L                        (0x133000 | 0x34<< 1)
#define REG_SC_BK30_35_L                        (0x133000 | 0x35<< 1)
#define REG_SC_BK30_36_L                        (0x133000 | 0x36<< 1)
#define REG_SC_BK30_37_L                        (0x133000 | 0x37<< 1)
#define REG_SC_BK30_38_L                        (0x133000 | 0x38<< 1)
#define REG_SC_BK30_39_L                        (0x133000 | 0x39<< 1)
#define REG_SC_BK30_3A_L                        (0x133000 | 0x3A<< 1)
#define REG_SC_BK30_3B_L                        (0x133000 | 0x3B<< 1)
#define REG_SC_BK30_3C_L                        (0x133000 | 0x3C<< 1)
#define REG_SC_BK30_3D_L                        (0x133000 | 0x3D<< 1)
#define REG_SC_BK30_3E_L                        (0x133000 | 0x3E<< 1)
#define REG_SC_BK30_3F_L                        (0x133000 | 0x3F<< 1)
#define REG_SC_BK30_40_L                        (0x133000 | 0x40<< 1)
#define REG_SC_BK30_41_L                        (0x133000 | 0x41<< 1)
#define REG_SC_BK30_42_L                        (0x133000 | 0x42<< 1)
#define REG_SC_BK30_43_L                        (0x133000 | 0x43<< 1)
#define REG_SC_BK30_44_L                        (0x133000 | 0x44<< 1)
#define REG_SC_BK30_45_L                        (0x133000 | 0x45<< 1)
#define REG_SC_BK30_46_L                        (0x133000 | 0x46<< 1)
#define REG_SC_BK30_47_L                        (0x133000 | 0x47<< 1)
#define REG_SC_BK30_48_L                        (0x133000 | 0x48<< 1)
#define REG_SC_BK30_49_L                        (0x133000 | 0x49<< 1)
#define REG_SC_BK30_4A_L                        (0x133000 | 0x4A<< 1)
#define REG_SC_BK30_4B_L                        (0x133000 | 0x4B<< 1)
#define REG_SC_BK30_4C_L                        (0x133000 | 0x4C<< 1)
#define REG_SC_BK30_4D_L                        (0x133000 | 0x4D<< 1)
#define REG_SC_BK30_4E_L                        (0x133000 | 0x4E<< 1)
#define REG_SC_BK30_4F_L                        (0x133000 | 0x4F<< 1)
#define REG_SC_BK30_50_L                        (0x133000 | 0x50<< 1)
#define REG_SC_BK30_51_L                        (0x133000 | 0x51<< 1)
#define REG_SC_BK30_52_L                        (0x133000 | 0x52<< 1)
#define REG_SC_BK30_53_L                        (0x133000 | 0x53<< 1)
#define REG_SC_BK30_54_L                        (0x133000 | 0x54<< 1)
#define REG_SC_BK30_55_L                        (0x133000 | 0x55<< 1)
#define REG_SC_BK30_56_L                        (0x133000 | 0x56<< 1)
#define REG_SC_BK30_57_L                        (0x133000 | 0x57<< 1)
#define REG_SC_BK30_58_L                        (0x133000 | 0x58<< 1)
#define REG_SC_BK30_59_L                        (0x133000 | 0x59<< 1)
#define REG_SC_BK30_5A_L                        (0x133000 | 0x5A<< 1)
#define REG_SC_BK30_5B_L                        (0x133000 | 0x5B<< 1)
#define REG_SC_BK30_5C_L                        (0x133000 | 0x5C<< 1)
#define REG_SC_BK30_5D_L                        (0x133000 | 0x5D<< 1)
#define REG_SC_BK30_5E_L                        (0x133000 | 0x5E<< 1)
#define REG_SC_BK30_5F_L                        (0x133000 | 0x5F<< 1)

#define REG_SC_BK39_00_L                        (0x133900 | 0x00<< 1)
#define REG_SC_BK39_01_L                        (0x133900 | 0x01<< 1)
#define REG_SC_BK39_02_L                        (0x133900 | 0x02<< 1)
#define REG_SC_BK39_03_L                        (0x133900 | 0x03<< 1)
#define REG_SC_BK39_04_L                        (0x133900 | 0x04<< 1)
#define REG_SC_BK39_05_L                        (0x133900 | 0x05<< 1)
#define REG_SC_BK39_06_L                        (0x133900 | 0x06<< 1)
#define REG_SC_BK39_07_L                        (0x133900 | 0x07<< 1)
#define REG_SC_BK39_08_L                        (0x133900 | 0x08<< 1)
#define REG_SC_BK39_09_L                        (0x133900 | 0x09<< 1)
#define REG_SC_BK39_0A_L                        (0x133900 | 0x0A<< 1)
#define REG_SC_BK39_0B_L                        (0x133900 | 0x0B<< 1)
#define REG_SC_BK39_0C_L                        (0x133900 | 0x0C<< 1)
#define REG_SC_BK39_0D_L                        (0x133900 | 0x0D<< 1)
#define REG_SC_BK39_0E_L                        (0x133900 | 0x0E<< 1)
#define REG_SC_BK39_0F_L                        (0x133900 | 0x0F<< 1)
#define REG_SC_BK39_10_L                        (0x133900 | 0x10<< 1)
#define REG_SC_BK39_11_L                        (0x133900 | 0x11<< 1)
#define REG_SC_BK39_12_L                        (0x133900 | 0x12<< 1)
#define REG_SC_BK39_13_L                        (0x133900 | 0x13<< 1)
#define REG_SC_BK39_14_L                        (0x133900 | 0x14<< 1)
#define REG_SC_BK39_15_L                        (0x133900 | 0x15<< 1)
#define REG_SC_BK39_16_L                        (0x133900 | 0x16<< 1)
#define REG_SC_BK39_17_L                        (0x133900 | 0x17<< 1)
#define REG_SC_BK39_18_L                        (0x133900 | 0x18<< 1)
#define REG_SC_BK39_19_L                        (0x133900 | 0x19<< 1)
#define REG_SC_BK39_1A_L                        (0x133900 | 0x1A<< 1)
#define REG_SC_BK39_1B_L                        (0x133900 | 0x1B<< 1)
#define REG_SC_BK39_1C_L                        (0x133900 | 0x1C<< 1)
#define REG_SC_BK39_1D_L                        (0x133900 | 0x1D<< 1)
#define REG_SC_BK39_1E_L                        (0x133900 | 0x1E<< 1)
#define REG_SC_BK39_1F_L                        (0x133900 | 0x1F<< 1)
#define REG_SC_BK39_20_L                        (0x133900 | 0x20<< 1)
#define REG_SC_BK39_21_L                        (0x133900 | 0x21<< 1)
#define REG_SC_BK39_22_L                        (0x133900 | 0x22<< 1)
#define REG_SC_BK39_23_L                        (0x133900 | 0x23<< 1)
#define REG_SC_BK39_24_L                        (0x133900 | 0x24<< 1)
#define REG_SC_BK39_25_L                        (0x133900 | 0x25<< 1)
#define REG_SC_BK39_26_L                        (0x133900 | 0x26<< 1)
#define REG_SC_BK39_27_L                        (0x133900 | 0x27<< 1)
#define REG_SC_BK39_28_L                        (0x133900 | 0x28<< 1)
#define REG_SC_BK39_29_L                        (0x133900 | 0x29<< 1)
#define REG_SC_BK39_2A_L                        (0x133900 | 0x2A<< 1)
#define REG_SC_BK39_2B_L                        (0x133900 | 0x2B<< 1)
#define REG_SC_BK39_2C_L                        (0x133900 | 0x2C<< 1)
#define REG_SC_BK39_2D_L                        (0x133900 | 0x2D<< 1)
#define REG_SC_BK39_2E_L                        (0x133900 | 0x2E<< 1)
#define REG_SC_BK39_2F_L                        (0x133900 | 0x2F<< 1)

#define REG_SC_BK3A_00_L                        (0x133A00 | 0x00<< 1)
#define REG_SC_BK3A_01_L                        (0x133A00 | 0x01<< 1)
#define REG_SC_BK3A_02_L                        (0x133A00 | 0x02<< 1)
#define REG_SC_BK3A_03_L                        (0x133A00 | 0x03<< 1)
#define REG_SC_BK3A_04_L                        (0x133A00 | 0x04<< 1)
#define REG_SC_BK3A_05_L                        (0x133A00 | 0x05<< 1)
#define REG_SC_BK3A_06_L                        (0x133A00 | 0x06<< 1)
#define REG_SC_BK3A_07_L                        (0x133A00 | 0x07<< 1)
#define REG_SC_BK3A_08_L                        (0x133A00 | 0x08<< 1)
#define REG_SC_BK3A_09_L                        (0x133A00 | 0x09<< 1)
#define REG_SC_BK3A_0A_L                        (0x133A00 | 0x0A<< 1)
#define REG_SC_BK3A_0B_L                        (0x133A00 | 0x0B<< 1)
#define REG_SC_BK3A_0C_L                        (0x133A00 | 0x0C<< 1)
#define REG_SC_BK3A_0D_L                        (0x133A00 | 0x0D<< 1)
#define REG_SC_BK3A_0E_L                        (0x133A00 | 0x0E<< 1)
#define REG_SC_BK3A_0F_L                        (0x133A00 | 0x0F<< 1)
#define REG_SC_BK3A_10_L                        (0x133A00 | 0x10<< 1)
#define REG_SC_BK3A_11_L                        (0x133A00 | 0x11<< 1)
#define REG_SC_BK3A_12_L                        (0x133A00 | 0x12<< 1)
#define REG_SC_BK3A_13_L                        (0x133A00 | 0x13<< 1)
#define REG_SC_BK3A_14_L                        (0x133A00 | 0x14<< 1)
#define REG_SC_BK3A_15_L                        (0x133A00 | 0x15<< 1)
#define REG_SC_BK3A_16_L                        (0x133A00 | 0x16<< 1)
#define REG_SC_BK3A_17_L                        (0x133A00 | 0x17<< 1)
#define REG_SC_BK3A_18_L                        (0x133A00 | 0x18<< 1)
#define REG_SC_BK3A_19_L                        (0x133A00 | 0x19<< 1)
#define REG_SC_BK3A_1A_L                        (0x133A00 | 0x1A<< 1)
#define REG_SC_BK3A_1B_L                        (0x133A00 | 0x1B<< 1)
#define REG_SC_BK3A_1C_L                        (0x133A00 | 0x1C<< 1)
#define REG_SC_BK3A_1D_L                        (0x133A00 | 0x1D<< 1)
#define REG_SC_BK3A_1E_L                        (0x133A00 | 0x1E<< 1)
#define REG_SC_BK3A_1F_L                        (0x133A00 | 0x1F<< 1)
#define REG_SC_BK3A_20_L                        (0x133A00 | 0x20<< 1)
#define REG_SC_BK3A_21_L                        (0x133A00 | 0x21<< 1)
#define REG_SC_BK3A_22_L                        (0x133A00 | 0x22<< 1)
#define REG_SC_BK3A_23_L                        (0x133A00 | 0x23<< 1)
#define REG_SC_BK3A_24_L                        (0x133A00 | 0x24<< 1)
#define REG_SC_BK3A_25_L                        (0x133A00 | 0x25<< 1)
#define REG_SC_BK3A_26_L                        (0x133A00 | 0x26<< 1)
#define REG_SC_BK3A_27_L                        (0x133A00 | 0x27<< 1)
#define REG_SC_BK3A_28_L                        (0x133A00 | 0x28<< 1)
#define REG_SC_BK3A_29_L                        (0x133A00 | 0x29<< 1)
#define REG_SC_BK3A_2A_L                        (0x133A00 | 0x2A<< 1)
#define REG_SC_BK3A_2B_L                        (0x133A00 | 0x2B<< 1)
#define REG_SC_BK3A_2C_L                        (0x133A00 | 0x2C<< 1)
#define REG_SC_BK3A_2D_L                        (0x133A00 | 0x2D<< 1)
#define REG_SC_BK3A_2E_L                        (0x133A00 | 0x2E<< 1)
#define REG_SC_BK3A_2F_L                        (0x133A00 | 0x2F<< 1)
#define REG_SC_BK3A_30_L                        (0x133A00 | 0x30<< 1)
#define REG_SC_BK3A_31_L                        (0x133A00 | 0x31<< 1)
#define REG_SC_BK3A_32_L                        (0x133A00 | 0x32<< 1)
#define REG_SC_BK3A_33_L                        (0x133A00 | 0x33<< 1)
#define REG_SC_BK3A_34_L                        (0x133A00 | 0x34<< 1)
#define REG_SC_BK3A_35_L                        (0x133A00 | 0x35<< 1)
#define REG_SC_BK3A_36_L                        (0x133A00 | 0x36<< 1)
#define REG_SC_BK3A_37_L                        (0x133A00 | 0x37<< 1)
#define REG_SC_BK3A_38_L                        (0x133A00 | 0x38<< 1)
#define REG_SC_BK3A_39_L                        (0x133A00 | 0x39<< 1)
#define REG_SC_BK3A_3A_L                        (0x133A00 | 0x3A<< 1)
#define REG_SC_BK3A_3B_L                        (0x133A00 | 0x3B<< 1)
#define REG_SC_BK3A_3C_L                        (0x133A00 | 0x3C<< 1)
#define REG_SC_BK3A_3D_L                        (0x133A00 | 0x3D<< 1)
#define REG_SC_BK3A_3E_L                        (0x133A00 | 0x3E<< 1)
#define REG_SC_BK3A_3F_L                        (0x133A00 | 0x3F<< 1)
#define REG_SC_BK3A_40_L                        (0x133A00 | 0x40<< 1)
#define REG_SC_BK3A_41_L                        (0x133A00 | 0x41<< 1)
#define REG_SC_BK3A_42_L                        (0x133A00 | 0x42<< 1)
#define REG_SC_BK3A_43_L                        (0x133A00 | 0x43<< 1)
#define REG_SC_BK3A_44_L                        (0x133A00 | 0x44<< 1)
#define REG_SC_BK3A_45_L                        (0x133A00 | 0x45<< 1)
#define REG_SC_BK3A_46_L                        (0x133A00 | 0x46<< 1)
#define REG_SC_BK3A_47_L                        (0x133A00 | 0x47<< 1)
#define REG_SC_BK3A_48_L                        (0x133A00 | 0x48<< 1)
#define REG_SC_BK3A_49_L                        (0x133A00 | 0x49<< 1)
#define REG_SC_BK3A_4A_L                        (0x133A00 | 0x4A<< 1)
#define REG_SC_BK3A_4B_L                        (0x133A00 | 0x4B<< 1)
#define REG_SC_BK3A_4C_L                        (0x133A00 | 0x4C<< 1)
#define REG_SC_BK3A_4D_L                        (0x133A00 | 0x4D<< 1)
#define REG_SC_BK3A_4E_L                        (0x133A00 | 0x4E<< 1)
#define REG_SC_BK3A_4F_L                        (0x133A00 | 0x4F<< 1)
#define REG_SC_BK3A_50_L                        (0x133A00 | 0x50<< 1)
#define REG_SC_BK3A_51_L                        (0x133A00 | 0x51<< 1)
#define REG_SC_BK3A_52_L                        (0x133A00 | 0x52<< 1)
#define REG_SC_BK3A_53_L                        (0x133A00 | 0x53<< 1)
#define REG_SC_BK3A_54_L                        (0x133A00 | 0x54<< 1)
#define REG_SC_BK3A_55_L                        (0x133A00 | 0x55<< 1)
#define REG_SC_BK3A_56_L                        (0x133A00 | 0x56<< 1)
#define REG_SC_BK3A_57_L                        (0x133A00 | 0x57<< 1)
#define REG_SC_BK3A_58_L                        (0x133A00 | 0x58<< 1)
#define REG_SC_BK3A_59_L                        (0x133A00 | 0x59<< 1)
#define REG_SC_BK3A_5A_L                        (0x133A00 | 0x5A<< 1)
#define REG_SC_BK3A_5B_L                        (0x133A00 | 0x5B<< 1)
#define REG_SC_BK3A_5C_L                        (0x133A00 | 0x5C<< 1)
#define REG_SC_BK3A_5D_L                        (0x133A00 | 0x5D<< 1)
#define REG_SC_BK3A_5E_L                        (0x133A00 | 0x5E<< 1)
#define REG_SC_BK3A_5F_L                        (0x133A00 | 0x5F<< 1)


#define REG_SC_BK47_08_L                        (0x134700 | 0x08<< 1)
#define REG_SC_BK47_0B_L                        (0x134700 | 0x0B<< 1)
#define REG_SC_BK47_10_L                        (0x134700 | 0x10<< 1)
#define REG_SC_BK47_11_L                        (0x134700 | 0x11<< 1)
#define REG_SC_BK47_12_L                        (0x134700 | 0x12<< 1)
#define REG_SC_BK47_13_L                        (0x134700 | 0x13<< 1)
#define REG_SC_BK47_14_L                        (0x134700 | 0x14<< 1)
#define REG_SC_BK47_15_L                        (0x134700 | 0x15<< 1)
#define REG_SC_BK47_16_L                        (0x134700 | 0x16<< 1)
#define REG_SC_BK47_17_L                        (0x134700 | 0x17<< 1)
#define REG_SC_BK47_18_L                        (0x134700 | 0x18<< 1)
#define REG_SC_BK47_19_L                        (0x134700 | 0x19<< 1)
#define REG_SC_BK47_1A_L                        (0x134700 | 0x1A<< 1)
#define REG_SC_BK47_1B_L                        (0x134700 | 0x1B<< 1)
#define REG_SC_BK47_1C_L                        (0x134700 | 0x1C<< 1)
#define REG_SC_BK47_1D_L                        (0x134700 | 0x1D<< 1)
#define REG_SC_BK47_1E_L                        (0x134700 | 0x1E<< 1)
#define REG_SC_BK47_1F_L                        (0x134700 | 0x1F<< 1)
#define REG_SC_BK47_20_L                        (0x134700 | 0x20<< 1)
#define REG_SC_BK47_21_L                        (0x134700 | 0x21<< 1)
#define REG_SC_BK47_22_L                        (0x134700 | 0x22<< 1)
#define REG_SC_BK47_23_L                        (0x134700 | 0x23<< 1)
#define REG_SC_BK47_24_L                        (0x134700 | 0x24<< 1)
#define REG_SC_BK47_25_L                        (0x134700 | 0x25<< 1)
#define REG_SC_BK47_26_L                        (0x134700 | 0x26<< 1)
#define REG_SC_BK47_27_L                        (0x134700 | 0x27<< 1)
#define REG_SC_BK47_28_L                        (0x134700 | 0x28<< 1)
#define REG_SC_BK47_29_L                        (0x134700 | 0x29<< 1)
#define REG_SC_BK47_2A_L                        (0x134700 | 0x2A<< 1)
#define REG_SC_BK47_2B_L                        (0x134700 | 0x2B<< 1)
#define REG_SC_BK47_2C_L                        (0x134700 | 0x2C<< 1)
#define REG_SC_BK47_2D_L                        (0x134700 | 0x2D<< 1)
#define REG_SC_BK47_2E_L                        (0x134700 | 0x2E<< 1)
#define REG_SC_BK47_2F_L                        (0x134700 | 0x2F<< 1)
#define REG_SC_BK47_30_L                        (0x134700 | 0x30<< 1)
#define REG_SC_BK47_31_L                        (0x134700 | 0x31<< 1)
#define REG_SC_BK47_32_L                        (0x134700 | 0x32<< 1)
#define REG_SC_BK47_33_L                        (0x134700 | 0x33<< 1)
#define REG_SC_BK47_34_L                        (0x134700 | 0x34<< 1)
#define REG_SC_BK47_35_L                        (0x134700 | 0x35<< 1)
#define REG_SC_BK47_36_L                        (0x134700 | 0x36<< 1)
#define REG_SC_BK47_37_L                        (0x134700 | 0x37<< 1)
#define REG_SC_BK47_38_L                        (0x134700 | 0x38<< 1)
#define REG_SC_BK47_39_L                        (0x134700 | 0x39<< 1)
#define REG_SC_BK47_3A_L                        (0x134700 | 0x3A<< 1)
#define REG_SC_BK47_3B_L                        (0x134700 | 0x3B<< 1)
#define REG_SC_BK47_3C_L                        (0x134700 | 0x3C<< 1)
#define REG_SC_BK47_3D_L                        (0x134700 | 0x3D<< 1)
#define REG_SC_BK47_3E_L                        (0x134700 | 0x3E<< 1)
#define REG_SC_BK47_3F_L                        (0x134700 | 0x3F<< 1)
#define REG_SC_BK47_40_L                        (0x134700 | 0x40<< 1)
#define REG_SC_BK47_41_L                        (0x134700 | 0x41<< 1)
#define REG_SC_BK47_42_L                        (0x134700 | 0x42<< 1)
#define REG_SC_BK47_43_L                        (0x134700 | 0x43<< 1)
#define REG_SC_BK47_44_L                        (0x134700 | 0x44<< 1)
#define REG_SC_BK47_45_L                        (0x134700 | 0x45<< 1)
#define REG_SC_BK47_46_L                        (0x134700 | 0x46<< 1)
#define REG_SC_BK47_47_L                        (0x134700 | 0x47<< 1)
#define REG_SC_BK47_48_L                        (0x134700 | 0x48<< 1)
#define REG_SC_BK47_49_L                        (0x134700 | 0x49<< 1)
#define REG_SC_BK47_4A_L                        (0x134700 | 0x4A<< 1)
#define REG_SC_BK47_4B_L                        (0x134700 | 0x4B<< 1)
#define REG_SC_BK47_4C_L                        (0x134700 | 0x4C<< 1)
#define REG_SC_BK47_4D_L                        (0x134700 | 0x4D<< 1)
#define REG_SC_BK47_4E_L                        (0x134700 | 0x4E<< 1)
#define REG_SC_BK47_4F_L                        (0x134700 | 0x4F<< 1)
#define REG_SC_BK47_50_L                        (0x134700 | 0x50<< 1)
#define REG_SC_BK47_51_L                        (0x134700 | 0x51<< 1)
#define REG_SC_BK47_52_L                        (0x134700 | 0x52<< 1)
#define REG_SC_BK47_53_L                        (0x134700 | 0x53<< 1)
#define REG_SC_BK47_54_L                        (0x134700 | 0x54<< 1)
#define REG_SC_BK47_55_L                        (0x134700 | 0x55<< 1)
#define REG_SC_BK47_56_L                        (0x134700 | 0x56<< 1)
#define REG_SC_BK47_57_L                        (0x134700 | 0x57<< 1)
#define REG_SC_BK47_58_L                        (0x134700 | 0x58<< 1)
#define REG_SC_BK47_59_L                        (0x134700 | 0x59<< 1)
#define REG_SC_BK47_5A_L                        (0x134700 | 0x5A<< 1)
#define REG_SC_BK47_5B_L                        (0x134700 | 0x5B<< 1)
#define REG_SC_BK47_5C_L                        (0x134700 | 0x5C<< 1)
#define REG_SC_BK47_5D_L                        (0x134700 | 0x5D<< 1)
#define REG_SC_BK47_5E_L                        (0x134700 | 0x5E<< 1)
#define REG_SC_BK47_5F_L                        (0x134700 | 0x5F<< 1)
#define REG_SC_BK47_60_L                        (0x134700 | 0x60<< 1)
#define REG_SC_BK47_61_L                        (0x134700 | 0x61<< 1)
#define REG_SC_BK47_62_L                        (0x134700 | 0x62<< 1)
#define REG_SC_BK47_63_L                        (0x134700 | 0x63<< 1)
#define REG_SC_BK47_64_L                        (0x134700 | 0x64<< 1)
#define REG_SC_BK47_65_L                        (0x134700 | 0x65<< 1)
#define REG_SC_BK47_66_L                        (0x134700 | 0x66<< 1)
#define REG_SC_BK47_67_L                        (0x134700 | 0x67<< 1)
#define REG_SC_BK47_68_L                        (0x134700 | 0x68<< 1)
#define REG_SC_BK47_69_L                        (0x134700 | 0x69<< 1)
#define REG_SC_BK47_6A_L                        (0x134700 | 0x6A<< 1)
#define REG_SC_BK47_6B_L                        (0x134700 | 0x6B<< 1)
#define REG_SC_BK47_6C_L                        (0x134700 | 0x6C<< 1)
#define REG_SC_BK47_6D_L                        (0x134700 | 0x6D<< 1)
#define REG_SC_BK47_6E_L                        (0x134700 | 0x6E<< 1)
#define REG_SC_BK47_6F_L                        (0x134700 | 0x6F<< 1)
#define REG_SC_BK47_78_L                        (0x134700 | 0x78<< 1)

#define REG_SC_BK60_18_L                        (0x136000 | 0x18<< 1)
#define REG_SC_BK60_20_L                        (0x136000 | 0x20<< 1)
#define REG_SC_BK60_21_L                        (0x136000 | 0x21<< 1)
#define REG_SC_BK60_22_L                        (0x136000 | 0x22<< 1)
#define REG_SC_BK60_23_L                        (0x136000 | 0x23<< 1)
#define REG_SC_BK60_24_L                        (0x136000 | 0x24<< 1)
#define REG_SC_BK60_25_L                        (0x136000 | 0x25<< 1)
#define REG_SC_BK60_26_L                        (0x136000 | 0x26<< 1)
#define REG_SC_BK60_27_L                        (0x136000 | 0x27<< 1)
#define REG_SC_BK60_28_L                        (0x136000 | 0x28<< 1)
#define REG_SC_BK60_29_L                        (0x136000 | 0x29<< 1)
#define REG_SC_BK60_2A_L                        (0x136000 | 0x2A<< 1)
#define REG_SC_BK60_2B_L                        (0x136000 | 0x2B<< 1)
#define REG_SC_BK60_2C_L                        (0x136000 | 0x2C<< 1)
#define REG_SC_BK60_2D_L                        (0x136000 | 0x2D<< 1)
#define REG_SC_BK60_2E_L                        (0x136000 | 0x2E<< 1)
#define REG_SC_BK60_2F_L                        (0x136000 | 0x2F<< 1)
#define REG_SC_BK60_30_L                        (0x136000 | 0x30<< 1)
#define REG_SC_BK60_40_L                        (0x136000 | 0x40<< 1)
#define REG_SC_BK60_41_L                        (0x136000 | 0x41<< 1)
#define REG_SC_BK60_42_L                        (0x136000 | 0x42<< 1)
#define REG_SC_BK60_43_L                        (0x136000 | 0x43<< 1)
#define REG_SC_BK60_44_L                        (0x136000 | 0x44<< 1)
#define REG_SC_BK60_45_L                        (0x136000 | 0x45<< 1)
#define REG_SC_BK60_46_L                        (0x136000 | 0x46<< 1)
#define REG_SC_BK60_47_L                        (0x136000 | 0x47<< 1)
#define REG_SC_BK60_48_L                        (0x136000 | 0x48<< 1)
#define REG_SC_BK60_49_L                        (0x136000 | 0x49<< 1)
#define REG_SC_BK60_4A_L                        (0x136000 | 0x4A<< 1)
#define REG_SC_BK60_4B_L                        (0x136000 | 0x4B<< 1)
#define REG_SC_BK60_4C_L                        (0x136000 | 0x4C<< 1)
#define REG_SC_BK60_4D_L                        (0x136000 | 0x4D<< 1)
#define REG_SC_BK60_4E_L                        (0x136000 | 0x4E<< 1)
#define REG_SC_BK60_4F_L                        (0x136000 | 0x4F<< 1)
#define REG_SC_BK60_50_L                        (0x136000 | 0x50<< 1)
#define REG_SC_BK60_51_L                        (0x136000 | 0x51<< 1)
#define REG_SC_BK60_52_L                        (0x136000 | 0x52<< 1)
#define REG_SC_BK60_53_L                        (0x136000 | 0x53<< 1)
#define REG_SC_BK60_54_L                        (0x136000 | 0x54<< 1)
#define REG_SC_BK60_55_L                        (0x136000 | 0x55<< 1)
#define REG_SC_BK60_56_L                        (0x136000 | 0x56<< 1)
#define REG_SC_BK60_57_L                        (0x136000 | 0x57<< 1)
#define REG_SC_BK60_58_L                        (0x136000 | 0x58<< 1)
#define REG_SC_BK60_59_L                        (0x136000 | 0x59<< 1)
#define REG_SC_BK60_5A_L                        (0x136000 | 0x5A<< 1)
#define REG_SC_BK60_5B_L                        (0x136000 | 0x5B<< 1)
#define REG_SC_BK60_5C_L                        (0x136000 | 0x5C<< 1)
#define REG_SC_BK60_5D_L                        (0x136000 | 0x5D<< 1)
#define REG_SC_BK60_5E_L                        (0x136000 | 0x5E<< 1)
#define REG_SC_BK60_5F_L                        (0x136000 | 0x5F<< 1)
#define REG_SC_BK60_60_L                        (0x136000 | 0x60<< 1)
#define REG_SC_BK60_61_L                        (0x136000 | 0x61<< 1)
#define REG_SC_BK60_62_L                        (0x136000 | 0x62<< 1)
#define REG_SC_BK60_63_L                        (0x136000 | 0x63<< 1)
#define REG_SC_BK60_64_L                        (0x136000 | 0x64<< 1)
#define REG_SC_BK60_65_L                        (0x136000 | 0x65<< 1)
#define REG_SC_BK60_66_L                        (0x136000 | 0x66<< 1)
#define REG_SC_BK60_67_L                        (0x136000 | 0x67<< 1)
#define REG_SC_BK60_68_L                        (0x136000 | 0x68<< 1)
#define REG_SC_BK60_69_L                        (0x136000 | 0x69<< 1)
#define REG_SC_BK60_6A_L                        (0x136000 | 0x6A<< 1)
#define REG_SC_BK60_6B_L                        (0x136000 | 0x6B<< 1)
#define REG_SC_BK60_6C_L                        (0x136000 | 0x6C<< 1)
#define REG_SC_BK60_6D_L                        (0x136000 | 0x6D<< 1)
#define REG_SC_BK60_6E_L                        (0x136000 | 0x6E<< 1)
#define REG_SC_BK60_6F_L                        (0x136000 | 0x6F<< 1)
#define REG_SC_BK60_78_L                        (0x136000 | 0x78<< 1)
#define REG_SC_BK60_7D_L                        (0x136000 | 0x7D<< 1)

#define REG_SC_BK61_00_L                        (0x136100 | 0x00<< 1)
#define REG_SC_BK61_01_L                        (0x136100 | 0x01<< 1)
#define REG_SC_BK61_02_L                        (0x136100 | 0x02<< 1)
#define REG_SC_BK61_03_L                        (0x136100 | 0x03<< 1)
#define REG_SC_BK61_04_L                        (0x136100 | 0x04<< 1)
#define REG_SC_BK61_05_L                        (0x136100 | 0x05<< 1)
#define REG_SC_BK61_06_L                        (0x136100 | 0x06<< 1)
#define REG_SC_BK61_07_L                        (0x136100 | 0x07<< 1)
#define REG_SC_BK61_08_L                        (0x136100 | 0x08<< 1)
#define REG_SC_BK61_09_L                        (0x136100 | 0x09<< 1)
#define REG_SC_BK61_0A_L                        (0x136100 | 0x0A<< 1)
#define REG_SC_BK61_0B_L                        (0x136100 | 0x0B<< 1)
#define REG_SC_BK61_0C_L                        (0x136100 | 0x0C<< 1)
#define REG_SC_BK61_0D_L                        (0x136100 | 0x0D<< 1)
#define REG_SC_BK61_0E_L                        (0x136100 | 0x0E<< 1)
#define REG_SC_BK61_0F_L                        (0x136100 | 0x0F<< 1)

//FSC
#define REG_FSC_BK1A_61_L                        (0x141A00 | (0x61<< 1))

#define REG_FSC_BK4D_4D_L                        (0x144D00 | (0x4D<< 1))
#define REG_FSC_BK4D_4E_L                        (0x144D00 | (0x4E<< 1))
#define REG_FSC_BK4D_4F_L                        (0x144D00 | (0x4F<< 1))
#define REG_FSC_BK4D_51_L                        (0x144D00 | (0x51<< 1))
#define REG_FSC_BK4D_52_L                        (0x144D00 | (0x52<< 1))

#define REG_FSC_BK55_1A_L                        (0x145500 | (0x1A<< 1))
#define REG_FSC_BK55_1B_L                        (0x145500 | (0x1B<< 1))
#define REG_FSC_BK55_1F_L                        (0x145500 | (0x1F<< 1))
#define REG_FSC_BK55_1E_L                        (0x145500 | (0x1E<< 1))


//===================================== register about film or dms ...etc end =========


/*static MS_S32 frameWidth = 0x780; //move to global variable
static MS_S32 frameHeight = 0x438;
static MS_U16 GuardBandWidth = 0x40;*/

typedef struct
{
    MS_U8 pbgY, pbgU, pbgV;
    MS_U32 flatSkinCnt,cplxCnt, prvBgCnt;
    MS_U32 skinRawCnt, cplxRawCnt, dswRawCnt;
    MS_U32 pbgrate; 
}XC_BackgroundState;
typedef struct
{
    MS_U16 Pk2DfinalGain,UFSCPk2DfinalGain;
    MS_U16 P3x3pkPosfinalGain,P3x3pkNegfinalGain;
    MS_U16 ERCEdgeG, ERCDetailG2 ,ERCDetailG3;
    MS_U16 NLMGain;
    MS_U16 SPNNGL,SPNNGH;
    MS_U16 LdefinalGain,LdefinalGainH,Prepk3x3finalGain;
    MS_U16 P2mfinalGain;
}XC_PqWriteReg;


//===================================== HDSD report (from pq adaptive.c to kernel) ========================================
typedef struct
{
    MS_U8 curOpFrame;
    MS_U8 maxOpFrame;

}XC_OpFrameStates;

typedef struct
{
    MS_U32 lumaBin[32];
}XC_LumaHistogramStates;
//===================================== HDSD report end ========================================

    
/*********************Eva modify 2 end********************************************/

//===================================== type define about film or dms ...etc =============
typedef struct
{
    MS_BOOL film32;
    MS_BOOL film22;
    MS_BOOL filmany;
    MS_BOOL film_act;
}XC_FilmStates;

typedef struct
{
    MS_U8 minMvXdir;
    MS_U8 minMvXBin;
    MS_U8 minMvYdir;
    MS_U8 minMvYBin;
}XC_Me1HisStates;

    typedef struct
{
    MS_U8 minMvXdir;
    MS_U8 minMvYdir;
    MS_U32 minMvXbin;
    MS_U32 minMvYbin;
    MS_U32 h1XYsum;
    MS_U32 h2XYsum;
    MS_U32 h1XYdiff;
    MS_U32 h2XYdiff;
    MS_BOOL yMvIsMovingOdd;
}XC_GMvStates;

typedef struct
{
    MS_BOOL HDSDD0_det_mode;
    MS_BOOL HDSDD1_det_mode;

    MS_U8 HDSDD0_det_mode_shift;
    MS_U8 HDSDD1_det_mode_shift;

    MS_U16 HDSDD0_det_mode_threshold;
    MS_U16 HDSDD1_det_mode_threshold;

    MS_U8 HDSDD_underflow_threshold;
    MS_U8 HDSDD_overflow_threshold;

    MS_BOOL HDSD_detection_letter_box_enable;

}XC_HpfDefaultRegSetting;

    typedef struct
    {
        MS_U16 lbCntT;
        MS_U16 lbCntB;
        MS_U16 lbCntL;
        MS_U16 lbCntR;
        MS_U8 lbRatioC;
        MS_U8 lbRatioTB;
        MS_U8 lbRatioLR;
        MS_U8 lbComfirmLevel;
    }XC_LetterBoxState;
    
    typedef struct
    {
        MS_U16 pdnrlowLut[4];
        MS_U16 pdnrlow2Lut[4];
        MS_U16 pdnrhighLut[4];
        MS_U16 mcnrLut[8];
    }XC_MCNRLutQmapSetting;
    
    typedef struct
    {
        MS_U16 filmLikePrvPixels;
        MS_U16 filmLikeNxtPixels;
        MS_BOOL vaildStates;
    }XC_McFilmStates;
//===================================== type define about film or dms ...etc end ========================================
//for MS_U8 u8Input_Source
/* typedef enum _E_CFD_MC_SOURCE
{
    /// VGA 0
    E_CFD_INPUT_SOURCE_VGA,
    /// ATV 1
    E_CFD_INPUT_SOURCE_TV,
    /// CVBS 2
    E_CFD_INPUT_SOURCE_CVBS,
    /// S-video 3
    E_CFD_INPUT_SOURCE_SVIDEO,
    /// Component 4
    E_CFD_INPUT_SOURCE_YPBPR,
    /// Scart 5
    E_CFD_INPUT_SOURCE_SCART,
    /// HDMI 6
    E_CFD_INPUT_SOURCE_HDMI,
    /// DTV 7
    E_CFD_INPUT_SOURCE_DTV,
    /// DVI 8
    E_CFD_INPUT_SOURCE_DVI,
    // Application source
    /// Storage 9
    E_CFD_INPUT_SOURCE_STORAGE,
    /// KTV 10
    E_CFD_INPUT_SOURCE_KTV,
    /// JPEG 11
    E_CFD_INPUT_SOURCE_JPEG,
    //RX for ulsa 12
    E_CFD_INPUT_SOURCE_RX,
    /// The max support number of PQ input source 13
    E_CFD_INPUT_SOURCE_RESERVED_START,
    /// None 14
    E_CFD_INPUT_SOURCE_NONE = E_CFD_INPUT_SOURCE_RESERVED_START,
} E_CFD_INPUT_SOURCE; */

#define MAIN_WINDOW 1
//#define INPUT_SOURCE_TV 0x00
//#define INPUT_SOURCE_CVBS 0x00
//#define INPUT_SOURCE_CVBS_MAX 0x00


    typedef struct
    {
        MS_U16 width;
        MS_U16 height;
        
    }XC_ApiCrop;
    
    typedef struct
    {
        XC_ApiCrop stCropWin;
        XC_ApiCrop ScaledCropWin;
        XC_ApiCrop stCapWin;
        MS_BOOL bInterlace;
        MS_U16 u16InputVFreq;
        MS_U16 u16H_SizeAfterPreScaling;
        //int enInputSourceType
        
    }XC_ApiStatus;
    
MS_BOOL MApi_XC_GetStatus(XC_ApiStatus *stXCStatus , MS_BOOL k);
    
#endif 

INTERFACE void      MApi_GFLIP_XC_W2BYTE(DWORD u32Reg, WORD u16Val );
INTERFACE WORD      MApi_GFLIP_XC_R2BYTE(DWORD u32Reg );
INTERFACE void      MApi_GFLIP_XC_W2BYTEMSK(DWORD u32Reg, WORD u16Val, WORD u16Mask );
INTERFACE WORD      MApi_GFLIP_XC_R2BYTEMSK(DWORD u32Reg, WORD u16Mask );

void MHal_Run_PQ_Adaptive(void);
void MHal_PQ_Adaptive_Top(void);
void MHal_PQ_Adaptive_Tuning(void);
#endif

#endif