/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdio.h>

#include "datatype.h"
#include "drvRIU.h"
#include "Board.h"
#include "c_riubase.h"
#include "hwreg_maserati.h"
#include "chip/bond.h"
#if (ENABLE_ONEBIN_ENABLE == 1)
#define PREBUFFERSIZE                  512      //array size
#define BUFFERSIZE                     1024      //array size
#endif
#define GPIO_NONE               0       // Not GPIO pin (default)
#define GPIO_IN                 1       // GPI
#define GPIO_OUT_LOW            2       // GPO output low
#define GPIO_OUT_HIGH           3       // GPO output high

#if defined(ARM_CHAKRA) || defined (MIPS_CHAKRA) || defined(MSOS_TYPE_LINUX)
#define _MapBase_nonPM_MASERATI      (RIU_MAP + 0x200000)
#define _MapBase_PM_MASERATI         RIU_MAP
#else
#define _MapBase_nonPM_MASERATI      0xA0200000
#define _MapBase_PM_MASERATI         0xA0000000
#endif

#define _MEMMAP_nonPM_              _RVM1(0x0000, 0x10, 0xFF)
#define _MEMMAP_nonPM_11_           _RVM1(0x0000, 0x11, 0xFF)
#define _MEMMAP_nonPM_12_           _RVM1(0x0000, 0x12, 0xFF)
#define _MEMMAP_PM_                 _RVM1(0x0000, 0x00, 0xFF)
#if (ENABLE_ONEBIN_ENABLE == 1)
//U8 PRERomBuffer[PREBUFFERSIZE] ={0};
U8 MIUBuffer[BUFFERSIZE] ={0};
U8 MIUSTRBuffer[BUFFERSIZE] ={0};
//U8 RamBuffer[BUFFERSIZE] ={0};
#endif
const U8 padInitTbl_PreInit[] =
{
    0xFF, 0xFF, 0xFF, 0xFF,         // magic code for ISP_Tool

    // programable device number
    // spi flash count
    0,
    0x00,                           // nor
    0x00,                           // nand
    0x00,                           // reserved
    0x00,                           // reserved
    0x00,                           // reserved

//---------------------------------------------------------------------
// GPIO Configuartion
//---------------------------------------------------------------------
    _MEMMAP_PM_,

    #if(PAD_GPIO_PM9_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM9_OEN (PAD_GPIO_PM9_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM9_OUT (PAD_GPIO_PM9_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f12, PAD_GPIO_PM9_OUT, BIT1),
    _RVM1(0x0f12, PAD_GPIO_PM9_OEN, BIT0),
    #endif

    #if(PAD_GPIO_PM10_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM10_OEN (PAD_GPIO_PM10_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM10_OUT (PAD_GPIO_PM10_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f14, PAD_GPIO_PM10_OUT, BIT1),
    _RVM1(0x0f14, PAD_GPIO_PM10_OEN, BIT0),
    //reg_spicsz2_gpio
    _RVM1(0x0e6a, BIT3 , BIT3 ),   //reg[0e6a]#3 = 1b
    #endif

    #if(PAD_VID0_IS_GPIO != GPIO_NONE)
    #define PAD_VID0_OEN (PAD_VID0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_VID0_OUT (PAD_VID0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2e84, PAD_VID0_OUT, BIT0),
    _RVM1(0x2e84, PAD_VID0_OEN, BIT1),
    //reg_vid_is_gpio
    _RVM1(0x0e39, BIT2 , BIT2 ),   //reg[0e39]#2 = 1b
    #endif

    #if(PAD_VID1_IS_GPIO != GPIO_NONE)
    #define PAD_VID1_OEN (PAD_VID1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_VID1_OUT (PAD_VID1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2e85, PAD_VID1_OUT, BIT0),
    _RVM1(0x2e85, PAD_VID1_OEN, BIT1),
    //reg_vid_is_gpio
    _RVM1(0x0e39, BIT2 , BIT2 ),   //reg[0e39]#2 = 1b
    #endif

    #if(PAD_VID2_IS_GPIO != GPIO_NONE)
    #define PAD_VID2_OEN (PAD_VID2_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_VID2_OUT (PAD_VID2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f22, PAD_VID2_OUT, BIT1),
    _RVM1(0x0f22, PAD_VID2_OEN, BIT0),
    //reg_vid_is_gpio
    _RVM1(0x0e39, BIT2 , BIT2 ),   //reg[0e39]#2 = 1b
    #endif

    #if(PAD_VID3_IS_GPIO != GPIO_NONE)
    #define PAD_VID3_OEN (PAD_VID3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_VID3_OUT (PAD_VID3_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f24, PAD_VID3_OUT, BIT1),
    _RVM1(0x0f24, PAD_VID3_OEN, BIT0),
    //reg_vid_is_gpio
    _RVM1(0x0e39, BIT2 , BIT2 ),   //reg[0e39]#2 = 1b
    #endif

    #if(PAD_VID4_IS_GPIO != GPIO_NONE)
    #define PAD_VID4_OEN (PAD_VID4_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_VID4_OUT (PAD_VID4_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0e, PAD_VID4_OUT, BIT1),
    _RVM1(0x0f0e, PAD_VID4_OEN, BIT0),
    #endif

    #if(PAD_VID5_IS_GPIO != GPIO_NONE)
    #define PAD_VID5_OEN (PAD_VID5_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_VID5_OUT (PAD_VID5_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f10, PAD_VID5_OUT, BIT1),
    _RVM1(0x0f10, PAD_VID5_OEN, BIT0),
    #endif

//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------

    _MEMMAP_nonPM_,

//---------------------------------------------------------------------
    #ifdef PADS_NAND_MODE
        #if (PADS_NAND_MODE != CONFIG_PADMUX_UNKNOWN)
        #define _CONFIG_NAND_MODE   ((PADS_NAND_MODE == CONFIG_PADMUX_MODE1) ? BIT6 : 0)
            _RVM1(0x1ede, _CONFIG_NAND_MODE, (BIT7 | BIT6)),
        #else
            _RVM1(0x1ede, 0, (BIT7 | BIT6)),
        #endif
    #endif

    #ifdef PADS_NAND_CS1_EN
        #if (PADS_NAND_CS1_EN != CONFIG_PADMUX_UNKNOWN)
        #define _CONFIG_NAND_CS1_EN ((PADS_NAND_CS1_EN == ENABLE) ? BIT5 : 0)
            _RVM1(0x1ede, _CONFIG_NAND_CS1_EN, BIT5),
        #else
            _RVM1(0x1ede, 0, BIT5),
        #endif
    #endif

//---------------------------------------------------------------------

    #ifdef PADS_EMMC_MODE
        #if (PADS_EMMC_MODE != CONFIG_PADMUX_UNKNOWN)
        #define _CONFIG_EMMC_MODE   ((PADS_EMMC_MODE == CONFIG_PADMUX_MODE1) ? BIT6 : 0)
            _RVM1(0x1edc, _CONFIG_EMMC_MODE, (BIT7 | BIT6)),
        #else
            _RVM1(0x1edc, 0, (BIT7 | BIT6)),
        #endif
    #endif

    #ifdef PADS_EMMC_RSTN_EN
        #if (PADS_EMMC_RSTN_EN != CONFIG_PADMUX_UNKNOWN)
        #define EMMC_RSTN_EN ((PADS_EMMC_RSTN_EN == ENABLE) ? BIT2 : 0)
            _RVM1(0x1E9E, EMMC_RSTN_EN, BIT2),
        #endif
    #endif

    #ifdef PADS_EMMC_RSTN_EN_PM
        #if (PADS_EMMC_RSTN_EN_PM != CONFIG_PADMUX_UNKNOWN)

            _MEMMAP_PM_,

        #define EMMC_RSTN_EN_PM ((PADS_EMMC_RSTN_EN_PM == ENABLE) ? BIT4 : 0)
            _RVM1(0x0E62, EMMC_RSTN_EN_PM, BIT4),

            _MEMMAP_nonPM_,

        #endif
    #endif

//=============================================================================
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),     //reg[101ea1]#7 = 0b
    _END_OF_TBL_,
};
const U8 padInitTbl[] =
{
    0x39, 0xB6, 0x5B, 0x53,     // magic code for ISP_Tool

    // programable device number
    // spi flash count
#if 1
    1 + (PIN_SPI_CZ1 != 0) + (PIN_SPI_CZ2 != 0) + (PIN_SPI_CZ3 != 0),
    0x00,                       // nor
    0x00,                       // nand
    0x00,                       // reserved
    0x00,                       // reserved
    0x00,                       // reserved

//---------------------------------------------------------------------
// GPIO Configuartion
//---------------------------------------------------------------------
    _MEMMAP_PM_,

    #if(PAD_PM_SPI_CZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CZ_OEN (PAD_PM_SPI_CZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CZ_OUT (PAD_PM_SPI_CZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
        _RVM1(0x0f2e, PAD_PM_SPI_CZ_OUT, BIT1),
        _RVM1(0x0f2e, PAD_PM_SPI_CZ_OEN, BIT0),
        //reg_spicsz0_gpio
        _RVM1(0x0e6a, BIT1 , BIT1 ),   //reg[0e6a]#1 = 1
    #endif

    #if(PAD_PM_SPI_CK_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CK_OEN (PAD_PM_SPI_CK_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CK_OUT (PAD_PM_SPI_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f30, PAD_PM_SPI_CK_OUT, BIT1),
    _RVM1(0x0f30, PAD_PM_SPI_CK_OEN, BIT0),
    //reg_spi_gpio
    _RVM1(0x0e6a, BIT0 , BIT0 ),   //reg[0e6a]#0 = 1b
    #endif

    #if(PAD_PM_SPI_DI_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DI_OEN (PAD_PM_SPI_DI_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DI_OUT (PAD_PM_SPI_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f32, PAD_PM_SPI_DI_OUT, BIT1),
    _RVM1(0x0f32, PAD_PM_SPI_DI_OEN, BIT0),
    //reg_spi_gpio
    _RVM1(0x0e6a, BIT0 , BIT0 ),   //reg[0e6a]#0 = 1b
    #endif

    #if(PAD_PM_SPI_DO_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DO_OEN (PAD_PM_SPI_DO_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DO_OUT (PAD_PM_SPI_DO_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f34, PAD_PM_SPI_DO_OUT, BIT1),
    _RVM1(0x0f34, PAD_PM_SPI_DO_OEN, BIT0),
    //reg_spi_gpio
    _RVM1(0x0e6a, BIT0 , BIT0 ),   //reg[0e6a]#0 = 1b
    #endif

    #if(PAD_IRIN_IS_GPIO != GPIO_NONE)
    #define PAD_IRIN_OEN (PAD_IRIN_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_IRIN_OUT (PAD_IRIN_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f26, PAD_IRIN_OUT, BIT1),
    _RVM1(0x0f26, PAD_IRIN_OEN, BIT0),
    //reg_ir_is_gpio
    _RVM1(0x0e38, BIT4 , BIT4 ),   //reg[0e38]#4 = 1b
    #endif

    #if(PAD_CEC0_IS_GPIO != GPIO_NONE)
    #define PAD_CEC0_OEN (PAD_CEC0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_CEC0_OUT (PAD_CEC0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2a, PAD_CEC0_OUT, BIT1),
    _RVM1(0x0f2a, PAD_CEC0_OEN, BIT0),
    //reg_cec_is_gpio
    _RVM1(0x0e38, BIT6 , BIT6 ),   //reg[0e38]#6 = 1b
    #endif

    #if(PAD_DDCA_CK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCA_CK_OEN (PAD_DDCA_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCA_CK_OUT (PAD_DDCA_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x0494, PAD_DDCA_CK_OUT, BIT2),
    _RVM1(0x0494, PAD_DDCA_CK_OEN, BIT1),
    //reg_gpio2a0_en
    _RVM1(0x0494, BIT7 , BIT7 ),   //reg[0494]#7 = 1b
    #endif

    #if(PAD_DDCA_DA_IS_GPIO != GPIO_NONE)
    #define PAD_DDCA_DA_OEN (PAD_DDCA_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DDCA_DA_OUT (PAD_DDCA_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0494, PAD_DDCA_DA_OUT, BIT6),
    _RVM1(0x0494, PAD_DDCA_DA_OEN, BIT5),
    //reg_gpio2a0_en
    _RVM1(0x0494, BIT7 , BIT7 ),   //reg[0494]#7 = 1b
    #endif

    #if(PAD_GPIO_PM0_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM0_OEN (PAD_GPIO_PM0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM0_OUT (PAD_GPIO_PM0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f00, PAD_GPIO_PM0_OUT, BIT1),
    _RVM1(0x0f00, PAD_GPIO_PM0_OEN, BIT0),
    #endif

    #if(PAD_GPIO_PM1_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM1_OEN (PAD_GPIO_PM1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM1_OUT (PAD_GPIO_PM1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f02, PAD_GPIO_PM1_OUT, BIT1),
    _RVM1(0x0f02, PAD_GPIO_PM1_OEN, BIT0),
    //reg_uart_is_gpio[1]
    _RVM1(0x0e6b, 0, BIT1 ),   //reg[0e6b]#1 = 0b
    //reg_uart_is_gpio[3]
    _RVM1(0x0e6b, 0, BIT3 ),   //reg[0e6b]#3 = 0b
    #endif

    #if(PAD_GPIO_PM2_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM2_OEN (PAD_GPIO_PM2_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM2_OUT (PAD_GPIO_PM2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f04, PAD_GPIO_PM2_OUT, BIT1),
    _RVM1(0x0f04, PAD_GPIO_PM2_OEN, BIT0),
    #endif

    #if(PAD_GPIO_PM3_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM3_OEN (PAD_GPIO_PM3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM3_OUT (PAD_GPIO_PM3_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f06, PAD_GPIO_PM3_OUT, BIT1),
    _RVM1(0x0f06, PAD_GPIO_PM3_OEN, BIT0),
    #endif

    #if(PAD_GPIO_PM4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM4_OEN (PAD_GPIO_PM4_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM4_OUT (PAD_GPIO_PM4_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f08, PAD_GPIO_PM4_OUT, BIT1),
    _RVM1(0x0f08, PAD_GPIO_PM4_OEN, BIT0),
    //reg_gpio_pm_lock
    _RVM1(0x0e24, 0xFF , 0xFF ),   //reg[0e24]#7 ~ #0 = 10111110b
    _RVM1(0x0e25, 0xFF , 0xFF ),   //reg[0e25]#7 ~ #0 = 10111010b
    //reg_gpio_pm4_inv
    _RVM1(0x0e38, 0, BIT1 ),   //reg[0e38]#1 = 0b
    #endif

    #if(PAD_GPIO_PM5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM5_OEN (PAD_GPIO_PM5_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM5_OUT (PAD_GPIO_PM5_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0a, PAD_GPIO_PM5_OUT, BIT1),
    _RVM1(0x0f0a, PAD_GPIO_PM5_OEN, BIT0),
    #endif

    #if(PAD_GPIO_PM6_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM6_OEN (PAD_GPIO_PM6_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM6_OUT (PAD_GPIO_PM6_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0c, PAD_GPIO_PM6_OUT, BIT1),
    _RVM1(0x0f0c, PAD_GPIO_PM6_OEN, BIT0),
    //reg_spicsz1_gpio
    _RVM1(0x0e6a, BIT2 , BIT2 ),   //reg[0e6a]#2 = 1b
    #endif

    #if(PAD_GPIO_PM9_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM9_OEN (PAD_GPIO_PM9_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM9_OUT (PAD_GPIO_PM9_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f12, PAD_GPIO_PM9_OUT, BIT1),
    _RVM1(0x0f12, PAD_GPIO_PM9_OEN, BIT0),
    #endif

    #if(PAD_GPIO_PM10_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM10_OEN (PAD_GPIO_PM10_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM10_OUT (PAD_GPIO_PM10_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f14, PAD_GPIO_PM10_OUT, BIT1),
    _RVM1(0x0f14, PAD_GPIO_PM10_OEN, BIT0),
    //reg_spicsz2_gpio
    _RVM1(0x0e6a, BIT3 , BIT3 ),   //reg[0e6a]#3 = 1b
    #endif

    #if(PAD_GPIO_PM11_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM11_OEN (PAD_GPIO_PM11_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM11_OUT (PAD_GPIO_PM11_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f16, PAD_GPIO_PM11_OUT, BIT1),
    _RVM1(0x0f16, PAD_GPIO_PM11_OEN, BIT0),
    #endif

    #if(PAD_GPIO_PM12_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM12_OEN (PAD_GPIO_PM12_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM12_OUT (PAD_GPIO_PM12_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f18, PAD_GPIO_PM12_OUT, BIT1),
    _RVM1(0x0f18, PAD_GPIO_PM12_OEN, BIT0),
    //reg_uart_is_gpio_1[0]
    _RVM1(0x0e6b, 0, BIT6 ),   //reg[0e6b]#6 = 0b
    //reg_uart_is_gpio_1[1]
    _RVM1(0x0e6b, 0, BIT7 ),   //reg[0e6b]#7 = 0b
    #endif

    #if(PAD_GPIO_PM13_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM13_OEN (PAD_GPIO_PM13_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM13_OUT (PAD_GPIO_PM13_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1a, PAD_GPIO_PM13_OUT, BIT1),
    _RVM1(0x0f1a, PAD_GPIO_PM13_OEN, BIT0),
    #endif

    #if(PAD_GPIO_PM14_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM14_OEN (PAD_GPIO_PM14_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM14_OUT (PAD_GPIO_PM14_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1c, PAD_GPIO_PM14_OUT, BIT1),
    _RVM1(0x0f1c, PAD_GPIO_PM14_OEN, BIT0),
    //reg_mhl_cable_detect_sel
    _RVM1(0x0ee4, 0, BIT6 ),   //reg[0ee4]#6 = 0b
    #endif

    #if(PAD_GPIO_PM15_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM15_OEN (PAD_GPIO_PM15_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM15_OUT (PAD_GPIO_PM15_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1e, PAD_GPIO_PM15_OUT, BIT1),
    _RVM1(0x0f1e, PAD_GPIO_PM15_OEN, BIT0),
    //reg_vbus_en_sel
    _RVM1(0x0ee4, 0, BIT7 ),   //reg[0ee4]#7 = 0b
    #endif

    #if(PAD_GPIO_PM16_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM16_OEN (PAD_GPIO_PM16_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO_PM16_OUT (PAD_GPIO_PM16_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f20, PAD_GPIO_PM16_OUT, BIT1),
    _RVM1(0x0f20, PAD_GPIO_PM16_OEN, BIT0),
    //reg_cbus_debug_sel
    _RVM1(0x0ee5, 0, BIT0 ),   //reg[0ee5]#0 = 0b
    #endif

    #if(PAD_LED0_IS_GPIO != GPIO_NONE)
    #define PAD_LED0_OEN (PAD_LED0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_LED0_OUT (PAD_LED0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2e80, PAD_LED0_OUT, BIT0),
    _RVM1(0x2e80, PAD_LED0_OEN, BIT1),
    //reg_seperate_wol_led_is_gpio
    _RVM1(0x0e39, BIT7 , BIT7 ),   //reg[0e39]#7 = 1b
    //reg_led_is_gpio
    _RVM1(0x0e39, BIT0 , BIT0 ),   //reg[0e39]#0 = 1b
    #endif

    #if(PAD_LED1_IS_GPIO != GPIO_NONE)
    #define PAD_LED1_OEN (PAD_LED1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_LED1_OUT (PAD_LED1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2e81, PAD_LED1_OUT, BIT0),
    _RVM1(0x2e81, PAD_LED1_OEN, BIT1),
    //reg_seperate_wol_led_is_gpio
    _RVM1(0x0e39, BIT7 , BIT7 ),   //reg[0e39]#7 = 1b
    //reg_led_is_gpio
    _RVM1(0x0e39, BIT0 , BIT0 ),   //reg[0e39]#0 = 1b
    #endif

    #if(PAD_HOTPLUGB_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGB_OEN (PAD_HOTPLUGB_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_HOTPLUGB_OUT (PAD_HOTPLUGB_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x0e4e, PAD_HOTPLUGB_OUT, BIT5),
    _RVM1(0x0e4e, PAD_HOTPLUGB_OEN, BIT1),
    //default is GPIO
    #endif

    #if(PAD_HOTPLUGC_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGC_OEN (PAD_HOTPLUGC_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_HOTPLUGC_OUT (PAD_HOTPLUGC_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0e4e, PAD_HOTPLUGC_OUT, BIT6),
    _RVM1(0x0e4e, PAD_HOTPLUGC_OEN, BIT2),
    //reg_hplugc_mhl_en
    _RVM1(0x0ee6, 0, BIT0 ),   //reg[0ee6]#0 = 0
    //reg_hpluga_gpio_en
    _RVM1(0x0ee6, BIT6 , BIT6 ),   //reg[0ee6]#6 = 1
    #endif

    #if(PAD_HOTPLUGD_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGD_OEN (PAD_HOTPLUGD_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_HOTPLUGD_OUT (PAD_HOTPLUGD_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x0e4e, PAD_HOTPLUGD_OUT, BIT7),
    _RVM1(0x0e4e, PAD_HOTPLUGD_OEN, BIT3),
    #endif

    #if(PAD_HOTPLUGA_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGA_HDMI20_5V_OEN (PAD_HOTPLUGA_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HOTPLUGA_HDMI20_5V_OUT (PAD_HOTPLUGA_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x0218, PAD_HOTPLUGA_HDMI20_5V_OUT, BIT4),
    _RVM1(0x0218, PAD_HOTPLUGA_HDMI20_5V_OEN, BIT5),
    #endif

    #if(PAD_HOTPLUGB_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGB_HDMI20_5V_OEN (PAD_HOTPLUGB_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HOTPLUGB_HDMI20_5V_OUT (PAD_HOTPLUGB_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x0318, PAD_HOTPLUGB_HDMI20_5V_OUT, BIT4),
    _RVM1(0x0318, PAD_HOTPLUGB_HDMI20_5V_OEN, BIT5),
    #endif

    #if(PAD_HOTPLUGC_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGC_HDMI20_5V_OEN (PAD_HOTPLUGC_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HOTPLUGC_HDMI20_5V_OUT (PAD_HOTPLUGC_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x0418, PAD_HOTPLUGC_HDMI20_5V_OUT, BIT4),
    _RVM1(0x0418, PAD_HOTPLUGC_HDMI20_5V_OEN, BIT5),
    #endif

    #if(PAD_HOTPLUGD_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGD_HDMI20_5V_OEN (PAD_HOTPLUGD_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HOTPLUGD_HDMI20_5V_OUT (PAD_HOTPLUGD_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x0518, PAD_HOTPLUGD_HDMI20_5V_OUT, BIT4),
    _RVM1(0x0518, PAD_HOTPLUGD_HDMI20_5V_OEN, BIT5),
    #endif

    #if(PAD_DDCDA_CK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDA_CK_OEN (PAD_DDCDA_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCDA_CK_OUT (PAD_DDCDA_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x0496, PAD_DDCDA_CK_OUT, BIT2),
    _RVM1(0x0496, PAD_DDCDA_CK_OEN, BIT1),
    //reg_ej_mode
    _RVM1(0x1e70, 0, BIT0 ),   //reg[1e70]#0 = 0b
    //reg_gpio2do_en
    _RVM1(0x0496, BIT7 , BIT7 ),   //reg[0496]#7 = 1b
    #endif

    #if(PAD_DDCDA_DA_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDA_DA_OEN (PAD_DDCDA_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DDCDA_DA_OUT (PAD_DDCDA_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0496, PAD_DDCDA_DA_OUT, BIT6),
    _RVM1(0x0496, PAD_DDCDA_DA_OEN, BIT5),
    //reg_ej_mode
    _RVM1(0x1e70, 0, BIT0 ),   //reg[1e70]#0 = 0b
    //reg_gpio2do_en
    _RVM1(0x0496, BIT7 , BIT7 ),   //reg[0496]#7 = 1b
    #endif

    #if(PAD_DDCDB_CK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDB_CK_OEN (PAD_DDCDB_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCDB_CK_OUT (PAD_DDCDB_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x0497, PAD_DDCDB_CK_OUT, BIT2),
    _RVM1(0x0497, PAD_DDCDB_CK_OEN, BIT1),
    //reg_ej_mode
    _RVM1(0x1e70, 0, BIT0 ),   //reg[1e70]#0 = 0b
    //reg_gpio2d1_en
    _RVM1(0x0497, BIT7 , BIT7 ),   //reg[0497]#7 = 1b
    #endif

    #if(PAD_DDCDB_DA_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDB_DA_OEN (PAD_DDCDB_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DDCDB_DA_OUT (PAD_DDCDB_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0497, PAD_DDCDB_DA_OUT, BIT6),
    _RVM1(0x0497, PAD_DDCDB_DA_OEN, BIT5),
    //reg_ej_mode
    _RVM1(0x1e70, 0, BIT0 ),   //reg[1e70]#0 = 0b
    //reg_gpio2d1_en
    _RVM1(0x0497, BIT7 , BIT7 ),   //reg[0497]#7 = 1b
    #endif

    #if(PAD_DDCDC_CK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDC_CK_OEN (PAD_DDCDC_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCDC_CK_OUT (PAD_DDCDC_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x0498, PAD_DDCDC_CK_OUT, BIT2),
    _RVM1(0x0498, PAD_DDCDC_CK_OEN, BIT1),
    //reg_gpio2d2_en
    _RVM1(0x0498, BIT7 , BIT7 ),   //reg[0498]#7 = 1b
    #endif

    #if(PAD_DDCDC_DA_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDC_DA_OEN (PAD_DDCDC_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DDCDC_DA_OUT (PAD_DDCDC_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0498, PAD_DDCDC_DA_OUT, BIT6),
    _RVM1(0x0498, PAD_DDCDC_DA_OEN, BIT5),
    //reg_gpio2d2_en
    _RVM1(0x0498, BIT7 , BIT7 ),   //reg[0498]#7 = 1b
    #endif

    #if(PAD_DDCDD_CK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDD_CK_OEN (PAD_DDCDD_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCDD_CK_OUT (PAD_DDCDD_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x0499, PAD_DDCDD_CK_OUT, BIT2),
    _RVM1(0x0499, PAD_DDCDD_CK_OEN, BIT1),
    //reg_gpio2d3_en
    _RVM1(0x0499, BIT7 , BIT7 ),   //reg[0499]#7 = 1b
    #endif

    #if(PAD_DDCDD_DA_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDD_DA_OEN (PAD_DDCDD_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DDCDD_DA_OUT (PAD_DDCDD_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0499, PAD_DDCDD_DA_OUT, BIT6),
    _RVM1(0x0499, PAD_DDCDD_DA_OEN, BIT5),
    //reg_gpio2d3_en
    _RVM1(0x0499, BIT7 , BIT7 ),   //reg[0499]#7 = 1b
    #endif

    #if(PAD_SAR0_IS_GPIO != GPIO_NONE)
    #define PAD_SAR0_OEN (PAD_SAR0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SAR0_OUT (PAD_SAR0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1424, PAD_SAR0_OUT, BIT0),
    _RVM1(0x1423, PAD_SAR0_OEN, BIT0),
    //reg_sar_aisel[0]
    _RVM1(0x1422, 0, BIT0 ),   //reg[1422]#0 = 0b
    #endif

    #if(PAD_SAR1_IS_GPIO != GPIO_NONE)
    #define PAD_SAR1_OEN (PAD_SAR1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SAR1_OUT (PAD_SAR1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1424, PAD_SAR1_OUT, BIT1),
    _RVM1(0x1423, PAD_SAR1_OEN, BIT1),
    //reg_sar_aisel[1]
    _RVM1(0x1422, 0, BIT1 ),   //reg[1422]#1 = 0b
    #endif

    #if(PAD_SAR2_IS_GPIO != GPIO_NONE)
    #define PAD_SAR2_OEN (PAD_SAR2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_SAR2_OUT (PAD_SAR2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1424, PAD_SAR2_OUT, BIT2),
    _RVM1(0x1423, PAD_SAR2_OEN, BIT2),
    //reg_sar_aisel[2]
    _RVM1(0x1422, 0, BIT2 ),   //reg[1422]#2 = 0b
    #endif

    #if(PAD_SAR3_IS_GPIO != GPIO_NONE)
    #define PAD_SAR3_OEN (PAD_SAR3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_SAR3_OUT (PAD_SAR3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1424, PAD_SAR3_OUT, BIT3),
    _RVM1(0x1423, PAD_SAR3_OEN, BIT3),
    //reg_sar_aisel[3]
    _RVM1(0x1422, 0, BIT3 ),   //reg[1422]#3 = 0b
    #endif

    #if(PAD_SAR4_IS_GPIO != GPIO_NONE)
    #define PAD_SAR4_OEN (PAD_SAR4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_SAR4_OUT (PAD_SAR4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1424, PAD_SAR4_OUT, BIT4),
    _RVM1(0x1423, PAD_SAR4_OEN, BIT4),
    //reg_sar_aisel[4]
    _RVM1(0x1422, 0, BIT4 ),   //reg[1422]#4 = 0b
    #endif

    #if(PAD_VPLGUIN_IS_GPIO != GPIO_NONE)
    #define PAD_VPLGUIN_OEN (PAD_VPLGUIN_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_VPLGUIN_OUT (PAD_VPLGUIN_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1424, PAD_VPLGUIN_OUT, BIT5),
    _RVM1(0x1423, PAD_VPLGUIN_OEN, BIT5),
    //reg_sar_aisel[5]
    _RVM1(0x1422, 0, BIT5 ),   //reg[1422]#5 = 0b
    #endif

    #if(PAD_WOL_INT_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_WOL_INT_OUT_OEN (PAD_WOL_INT_OUT_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_WOL_INT_OUT_OUT (PAD_WOL_INT_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2e82, PAD_WOL_INT_OUT_OUT, BIT0),
    _RVM1(0x2e82, PAD_WOL_INT_OUT_OEN, BIT1),
    //reg_wol_is_gpio
    _RVM1(0x0e39, BIT1 , BIT1 ),   //reg[0e39]#1 = 1b
    #endif

    _MEMMAP_nonPM_,

    #if(PAD_DDCR_DA_IS_GPIO != GPIO_NONE)
    #define PAD_DDCR_DA_OEN (PAD_DDCR_DA_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCR_DA_OUT (PAD_DDCR_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b86, PAD_DDCR_DA_OUT, BIT0),
    _RVM1(0x2b86, PAD_DDCR_DA_OEN, BIT1),
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ddcrmode[1:0]
    _RVM1(0x1eae, 0, BIT1 | BIT0 ),   //reg[101eae]#1 ~ #0 = 0b
    //reg_ddcrmode[1:0]
    _RVM1(0x1eae, 0, BIT1 | BIT0 ),   //reg[101eae]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_DDCR_CK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCR_CK_OEN (PAD_DDCR_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCR_CK_OUT (PAD_DDCR_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b87, PAD_DDCR_CK_OUT, BIT0),
    _RVM1(0x2b87, PAD_DDCR_CK_OEN, BIT1),
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ddcrmode[1:0]
    _RVM1(0x1eae, 0, BIT1 | BIT0 ),   //reg[101eae]#1 ~ #0 = 0b
    //reg_ddcrmode[1:0]
    _RVM1(0x1eae, 0, BIT1 | BIT0 ),   //reg[101eae]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO0_OEN (PAD_GPIO0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO0_OUT (PAD_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b00, PAD_GPIO0_OUT, BIT0),
    _RVM1(0x2b00, PAD_GPIO0_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcm_config
    _RVM1(0x1ef6, 0, BIT1 ),   //reg[101ef6]#1 = 0b
    //reg_p1_enable[0]
    _RVM1(0x1ea4, 0, BIT0 ),   //reg[101ea4]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO1_OEN (PAD_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO1_OUT (PAD_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b01, PAD_GPIO1_OUT, BIT0),
    _RVM1(0x2b01, PAD_GPIO1_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcm_config
    _RVM1(0x1ef6, 0, BIT1 ),   //reg[101ef6]#1 = 0bs
    //reg_p1_enable[1]
    _RVM1(0x1ea4, 0, BIT1 ),   //reg[101ea4]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO2_OEN (PAD_GPIO2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO2_OUT (PAD_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b02, PAD_GPIO2_OUT, BIT0),
    _RVM1(0x2b02, PAD_GPIO2_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcm_config
    _RVM1(0x1ef6, 0, BIT1 ),   //reg[101ef6]#1 = 0b
    //reg_p1_enable[2]
    _RVM1(0x1ea4, 0, BIT2 ),   //reg[101ea4]#2 = 0b
    //reg_3dflagconfig[1:0]
    _RVM1(0x1eb3, 0, BIT7 | BIT6 ),   //reg[101eb3]#7 ~ #6 = 0b
    //reg_osd3dflag_config
    _RVM1(0x1ef6, 0, BIT7 | BIT6 ),   //reg[101ef6]#7 ~ #6 = 0b
    //reg_usb30vctl_config[1:0]
    _RVM1(0x1e08, 0, BIT5 | BIT4 ),   //reg[101e08]#5 ~ #4 = 0b
    //reg_usb30vctl1_config[1:0]
    _RVM1(0x1e09, 0, BIT1 | BIT0 ),   //reg[101e09]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO3_OEN (PAD_GPIO3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO3_OUT (PAD_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b03, PAD_GPIO3_OUT, BIT0),
    _RVM1(0x2b03, PAD_GPIO3_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcm_config
    _RVM1(0x1ef6, 0, BIT1 ),   //reg[101ef6]#1 = 0b
    //reg_seconduartmode[1:0]
    _RVM1(0x1e05, 0, BIT1 | BIT0 ),   //reg[101e05]#1 ~ #0 = 0b
    //reg_od2nduart[1:0]
    _RVM1(0x1ea9, 0, BIT1 | BIT0 ),   //reg[101ea9]#1 ~ #0 = 0b
    //reg_p1_enable[3]
    _RVM1(0x1ea4, 0, BIT3 ),   //reg[101ea4]#3 = 0b
    //reg_usb30vctl_config[1:0]
    _RVM1(0x1e08, 0, BIT5 | BIT4 ),   //reg[101e08]#5 ~ #4 = 0b
    //reg_usb30vctl1_config[1:0]
    _RVM1(0x1e09, 0, BIT1 | BIT0 ),   //reg[101e09]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO4_OEN (PAD_GPIO4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO4_OUT (PAD_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b04, PAD_GPIO4_OUT, BIT0),
    _RVM1(0x2b04, PAD_GPIO4_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_seconduartmode[1:0]
    _RVM1(0x1e05, 0, BIT1 | BIT0 ),   //reg[101e05]#1 ~ #0 = 0b
    //reg_od2nduart[1:0]
    _RVM1(0x1ea9, 0, BIT1 | BIT0 ),   //reg[101ea9]#1 ~ #0 = 0b
    //reg_p1_enable[4]
    _RVM1(0x1ea4, 0, BIT4 ),   //reg[101ea4]#4 = 0b
    //reg_usb30vctl_config[1:0]
    _RVM1(0x1e08, 0, BIT5 | BIT4 ),   //reg[101e08]#5 ~ #4 = 0b
    //reg_usb30vctl1_config[1:0]
    _RVM1(0x1e09, 0, BIT1 | BIT0 ),   //reg[101e09]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO5_OEN (PAD_GPIO5_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO5_OUT (PAD_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b05, PAD_GPIO5_OUT, BIT0),
    _RVM1(0x2b05, PAD_GPIO5_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_fastuartmode[1:0]
    _RVM1(0x1e04, 0, BIT5 | BIT4 ),   //reg[101e04]#5 ~ #4 = 0b
    //reg_odfuart[1:0]
    _RVM1(0x1ea9, 0, BIT7 | BIT6 ),   //reg[101ea9]#7 ~ #6 = 0b
    //reg_p1_enable[5]
    _RVM1(0x1ea4, 0, BIT5 ),   //reg[101ea4]#5 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO6_OEN (PAD_GPIO6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO6_OUT (PAD_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b06, PAD_GPIO6_OUT, BIT0),
    _RVM1(0x2b06, PAD_GPIO6_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_fastuartmode[1:0]
    _RVM1(0x1e04, 0, BIT5 | BIT4 ),   //reg[101e04]#5 ~ #4 = 0b
    //reg_odfuart[1:0]
    _RVM1(0x1ea9, 0, BIT7 | BIT6 ),   //reg[101ea9]#7 ~ #6 = 0b
    //reg_p1_enable[6]
    _RVM1(0x1ea4, 0, BIT6 ),   //reg[101ea4]#6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO7_OEN (PAD_GPIO7_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO7_OUT (PAD_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b07, PAD_GPIO7_OUT, BIT0),
    _RVM1(0x2b07, PAD_GPIO7_OEN, BIT1),
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_seconduartmode[1:0]
    _RVM1(0x1e05, 0, BIT1 | BIT0 ),   //reg[101e05]#1 ~ #0 = 0b
    //reg_od2nduart[1:0]
    _RVM1(0x1ea9, 0, BIT1 | BIT0 ),   //reg[101ea9]#1 ~ #0 = 0b
    //reg_p1_enable[7]
    _RVM1(0x1ea4, 0, BIT7 ),   //reg[101ea4]#7 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO8_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO8_OEN (PAD_GPIO8_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO8_OUT (PAD_GPIO8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b08, PAD_GPIO8_OUT, BIT0),
    _RVM1(0x2b08, PAD_GPIO8_OEN, BIT1),
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_ts4config[1:0]
    _RVM1(0x1e19, 0, BIT3 | BIT2 ),   //reg[101e19]#3 ~ #2 = 0b
    //reg_seconduartmode[1:0]
    _RVM1(0x1e05, 0, BIT1 | BIT0 ),   //reg[101e05]#1 ~ #0 = 0b
    //reg_fifthuartmode[1:0]
    _RVM1(0x1e08, 0, BIT3 | BIT2 ),   //reg[101e08]#3 ~ #2 = 0b
    //reg_od2nduart[1:0]
    _RVM1(0x1ea9, 0, BIT1 | BIT0 ),   //reg[101ea9]#1 ~ #0 = 0b
    //reg_od5thuart[1:0]
    _RVM1(0x1eaa, 0, BIT5 | BIT4 ),   //reg[101eaa]#5 ~ #4 = 0b
    //reg_pdtracectrl[1:0]
    _RVM1(0x1e9f, 0, BIT1 | BIT0 ),   //reg[101e9f]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO16_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO16_OEN (PAD_GPIO16_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO16_OUT (PAD_GPIO16_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b10, PAD_GPIO16_OUT, BIT0),
    _RVM1(0x2b10, PAD_GPIO16_OEN, BIT1),
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_fifthuartmode[1:0]
    _RVM1(0x1e08, 0, BIT3 | BIT2 ),   //reg[101e08]#3 ~ #2 = 0b
    //reg_od5thuart[1:0]
    _RVM1(0x1eaa, 0, BIT5 | BIT4 ),   //reg[101eaa]#5 ~ #4 = 0b
    //reg_tserrout[1:0]
    _RVM1(0x1ec9, 0, BIT1 | BIT0 ),   //reg[101ec9]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO17_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO17_OEN (PAD_GPIO17_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO17_OUT (PAD_GPIO17_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b11, PAD_GPIO17_OUT, BIT0),
    _RVM1(0x2b11, PAD_GPIO17_OEN, BIT1),
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_fifthuartmode[1:0]
    _RVM1(0x1e08, 0, BIT3 | BIT2 ),   //reg[101e08]#3 ~ #2 = 0b
    //reg_od5thuart[1:0]
    _RVM1(0x1eaa, 0, BIT5 | BIT4 ),   //reg[101eaa]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO18_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO18_OEN (PAD_GPIO18_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO18_OUT (PAD_GPIO18_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b12, PAD_GPIO18_OUT, BIT0),
    _RVM1(0x2b12, PAD_GPIO18_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO19_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO19_OEN (PAD_GPIO19_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO19_OUT (PAD_GPIO19_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b13, PAD_GPIO19_OUT, BIT0),
    _RVM1(0x2b13, PAD_GPIO19_OEN, BIT1),
    //reg_agc_dbg
    _RVM1(0x1e9e, 0, BIT7 ),   //reg[101e9e]#7 = 0b
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_led_mode
    _RVM1(0x1eb4, 0, BIT4 ),   //reg[101eb4]#4 = 0b
    //reg_seconduartmode[1:0]
    _RVM1(0x1e05, 0, BIT1 | BIT0 ),   //reg[101e05]#1 ~ #0 = 0b
    //reg_od2nduart[1:0]
    _RVM1(0x1ea9, 0, BIT1 | BIT0 ),   //reg[101ea9]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO20_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO20_OEN (PAD_GPIO20_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO20_OUT (PAD_GPIO20_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b14, PAD_GPIO20_OUT, BIT0),
    _RVM1(0x2b14, PAD_GPIO20_OEN, BIT1),
    //reg_agc_dbg
    _RVM1(0x1e9e, 0, BIT7 ),   //reg[101e9e]#7 = 0b
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_led_mode
    _RVM1(0x1eb4, 0, BIT4 ),   //reg[101eb4]#4 = 0b
    //reg_seconduartmode[1:0]
    _RVM1(0x1e05, 0, BIT1 | BIT0 ),   //reg[101e05]#1 ~ #0 = 0b
    //reg_od2nduart[1:0]
    _RVM1(0x1ea9, 0, BIT1 | BIT0 ),   //reg[101ea9]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO21_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO21_OEN (PAD_GPIO21_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO21_OUT (PAD_GPIO21_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b15, PAD_GPIO21_OUT, BIT0),
    _RVM1(0x2b15, PAD_GPIO21_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_thirduartmode[1:0]
    _RVM1(0x1e05, 0, BIT3 | BIT2 ),   //reg[101e05]#3 ~ #2 = 0b
    //reg_od3rduart[1:0]
    _RVM1(0x1ea9, 0, BIT3 | BIT2 ),   //reg[101ea9]#3 ~ #2 = 0b
    //reg_ext_int[5]
    _RVM1(0x1ea5, 0, BIT5 ),   //reg[101ea5]#5 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO22_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO22_OEN (PAD_GPIO22_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO22_OUT (PAD_GPIO22_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b16, PAD_GPIO22_OUT, BIT0),
    _RVM1(0x2b16, PAD_GPIO22_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_thirduartmode[1:0]
    _RVM1(0x1e05, 0, BIT3 | BIT2 ),   //reg[101e05]#3 ~ #2 = 0b
    //reg_od3rduart[1:0]
    _RVM1(0x1ea9, 0, BIT3 | BIT2 ),   //reg[101ea9]#3 ~ #2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO23_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO23_OEN (PAD_GPIO23_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO23_OUT (PAD_GPIO23_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b17, PAD_GPIO23_OUT, BIT0),
    _RVM1(0x2b17, PAD_GPIO23_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_fourthuartmode[1:0]
    _RVM1(0x1e04, 0, BIT7 | BIT6 ),   //reg[101e04]#7 ~ #6 = 0b
    //reg_od4thuart[1:0]
    _RVM1(0x1ea9, 0, BIT5 | BIT4 ),   //reg[101ea9]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO24_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO24_OEN (PAD_GPIO24_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO24_OUT (PAD_GPIO24_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b18, PAD_GPIO24_OUT, BIT0),
    _RVM1(0x2b18, PAD_GPIO24_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_fourthuartmode[1:0]
    _RVM1(0x1e04, 0, BIT7 | BIT6 ),   //reg[101e04]#7 ~ #6 = 0b
    //reg_od4thuart[1:0]
    _RVM1(0x1ea9, 0, BIT5 | BIT4 ),   //reg[101ea9]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO25_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO25_OEN (PAD_GPIO25_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO25_OUT (PAD_GPIO25_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b19, PAD_GPIO25_OUT, BIT0),
    _RVM1(0x2b19, PAD_GPIO25_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_fastuartmode[1:0]
    _RVM1(0x1e04, 0, BIT5 | BIT4 ),   //reg[101e04]#5 ~ #4 = 0b
    //reg_odfuart[1:0]
    _RVM1(0x1ea9, 0, BIT7 | BIT6 ),   //reg[101ea9]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO26_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO26_OEN (PAD_GPIO26_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO26_OUT (PAD_GPIO26_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b1a, PAD_GPIO26_OUT, BIT0),
    _RVM1(0x2b1a, PAD_GPIO26_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_fastuartmode[1:0]
    _RVM1(0x1e04, 0, BIT5 | BIT4 ),   //reg[101e04]#5 ~ #4 = 0b
    //reg_odfuart[1:0]
    _RVM1(0x1ea9, 0, BIT7 | BIT6 ),   //reg[101ea9]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO27_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO27_OEN (PAD_GPIO27_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO27_OUT (PAD_GPIO27_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b1b, PAD_GPIO27_OUT, BIT0),
    _RVM1(0x2b1b, PAD_GPIO27_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_et_mode
    _RVM1(0x1edf, 0, BIT0 ),   //reg[101edf]#0 = 0b
    //reg_ext_int[4]
    _RVM1(0x1ea5, 0, BIT4 ),   //reg[101ea5]#4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO28_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO28_OEN (PAD_GPIO28_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO28_OUT (PAD_GPIO28_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba0, PAD_GPIO28_OUT, BIT0),
    _RVM1(0x2ba0, PAD_GPIO28_OEN, BIT1),
    //reg_miic_mode0
    _RVM1(0x1edc, 0, BIT0 ),   //reg[101edc]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO29_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO29_OEN (PAD_GPIO29_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO29_OUT (PAD_GPIO29_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba1, PAD_GPIO29_OUT, BIT0),
    _RVM1(0x2ba1, PAD_GPIO29_OEN, BIT1),
    //reg_miic_mode0
    _RVM1(0x1edc, 0, BIT0 ),   //reg[101edc]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO30_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO30_OEN (PAD_GPIO30_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO30_OUT (PAD_GPIO30_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba2, PAD_GPIO30_OUT, BIT0),
    _RVM1(0x2ba2, PAD_GPIO30_OEN, BIT1),
    //reg_miic_mode4
    _RVM1(0x1ede, 0, BIT0 ),   //reg[101ede]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO31_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO31_OEN (PAD_GPIO31_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO31_OUT (PAD_GPIO31_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba3, PAD_GPIO31_OUT, BIT0),
    _RVM1(0x2ba3, PAD_GPIO31_OEN, BIT1),
    //reg_miic_mode4
    _RVM1(0x1ede, 0, BIT0 ),   //reg[101ede]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO32_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO32_OEN (PAD_GPIO32_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO32_OUT (PAD_GPIO32_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba4, PAD_GPIO32_OUT, BIT0),
    _RVM1(0x2ba4, PAD_GPIO32_OEN, BIT1),
    //reg_miic_mode5
    _RVM1(0x1ede, 0, BIT1 ),   //reg[101ede]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO33_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO33_OEN (PAD_GPIO33_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO33_OUT (PAD_GPIO33_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba5, PAD_GPIO33_OUT, BIT0),
    _RVM1(0x2ba5, PAD_GPIO33_OEN, BIT1),
    //reg_miic_mode5
    _RVM1(0x1ede, 0, BIT1 ),   //reg[101ede]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO34_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO34_OEN (PAD_GPIO34_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO34_OUT (PAD_GPIO34_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba6, PAD_GPIO34_OUT, BIT0),
    _RVM1(0x2ba6, PAD_GPIO34_OEN, BIT1),
    //reg_sixthuartmode
    _RVM1(0x1e08, 0, BIT7 | BIT6 ),   //reg[101e08]#7 ~ #6 = 00b
    //reg_od6thuart[1:0]
    _RVM1(0x1eaa, 0, BIT7 | BIT6 ),   //reg[101eaa]#7 ~ #6 = 00b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO35_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO35_OEN (PAD_GPIO35_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO35_OUT (PAD_GPIO35_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba7, PAD_GPIO35_OUT, BIT0),
    _RVM1(0x2ba7, PAD_GPIO35_OEN, BIT1),
    //reg_sixthuartmode
    _RVM1(0x1e08, 0, BIT7 | BIT6 ),   //reg[101e08]#7 ~ #6 = 00b
    //reg_od6thuart[1:0]
    _RVM1(0x1eaa, 0, BIT7 | BIT6 ),   //reg[101eaa]#7 ~ #6 = 00b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO36_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO36_OEN (PAD_GPIO36_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO36_OUT (PAD_GPIO36_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba8, PAD_GPIO36_OUT, BIT0),
    _RVM1(0x2ba8, PAD_GPIO36_OEN, BIT1),
    //reg_miic_mode3
    _RVM1(0x1edc, 0, BIT3 ),   //reg[101edc]#3 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_GPIO37_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO37_OEN (PAD_GPIO37_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO37_OUT (PAD_GPIO37_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2ba9, PAD_GPIO37_OUT, BIT0),
    _RVM1(0x2ba9, PAD_GPIO37_OEN, BIT1),
    //reg_miic_mode3
    _RVM1(0x1edc, 0, BIT3 ),   //reg[101edc]#3 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_IN_WS_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_IN_WS_OEN (PAD_I2S_IN_WS_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_IN_WS_OUT (PAD_I2S_IN_WS_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b36, PAD_I2S_IN_WS_OUT, BIT0),
    _RVM1(0x2b36, PAD_I2S_IN_WS_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_i2sinconfig
    _RVM1(0x1eae, 0, BIT2 ),   //reg[101eae]#2 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_thirduartmode[1:0]
    _RVM1(0x1e05, 0, BIT3 | BIT2 ),   //reg[101e05]#3 ~ #2 = 0b
    //reg_od3rduart[1:0]
    _RVM1(0x1ea9, 0, BIT3 | BIT2 ),   //reg[101ea9]#3 ~ #2 = 0b
    //reg_ext_int[6]
    _RVM1(0x1ea5, 0, BIT6 ),   //reg[101ea5]#6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_IN_BCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_IN_BCK_OEN (PAD_I2S_IN_BCK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_IN_BCK_OUT (PAD_I2S_IN_BCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b37, PAD_I2S_IN_BCK_OUT, BIT0),
    _RVM1(0x2b37, PAD_I2S_IN_BCK_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_i2sinconfig
    _RVM1(0x1eae, 0, BIT2 ),   //reg[101eae]#2 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_thirduartmode[1:0]
    _RVM1(0x1e05, 0, BIT3 | BIT2 ),   //reg[101e05]#3 ~ #2 = 0b
    //reg_od3rduart[1:0]
    _RVM1(0x1ea9, 0, BIT3 | BIT2 ),   //reg[101ea9]#3 ~ #2 = 0b
    //reg_miic_mode2
    _RVM1(0x1edc, 0, BIT2 ),   //reg[101edc]#2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_IN_SD_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_IN_SD_OEN (PAD_I2S_IN_SD_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_IN_SD_OUT (PAD_I2S_IN_SD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b38, PAD_I2S_IN_SD_OUT, BIT0),
    _RVM1(0x2b38, PAD_I2S_IN_SD_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_i2sinconfig
    _RVM1(0x1eae, 0, BIT2 ),   //reg[101eae]#2 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_tserrout[1:0]
    _RVM1(0x1ec9, 0, BIT1 | BIT0 ),   //reg[101ec9]#1 ~ #0 = 0b
    //reg_miic_mode2
    _RVM1(0x1edc, 0, BIT2 ),   //reg[101edc]#2 = 0b
    //reg_3dflagconfig[1:0]
    _RVM1(0x1eb3, 0, BIT7 | BIT6 ),   //reg[101eb3]#7 ~ #6 = 0b
    //reg_osd3dflag_config
    _RVM1(0x1ef6, 0, BIT7 | BIT6 ),   //reg[101ef6]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPDIF_IN_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_IN_OEN (PAD_SPDIF_IN_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SPDIF_IN_OUT (PAD_SPDIF_IN_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b39, PAD_SPDIF_IN_OUT, BIT0),
    _RVM1(0x2b39, PAD_SPDIF_IN_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_spdifinconfig
    _RVM1(0x1eae, 0, BIT6 ),   //reg[101eae]#6 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_3dflagconfig[1:0]
    _RVM1(0x1eb3, 0, BIT7 | BIT6 ),   //reg[101eb3]#7 ~ #6 = 0b
    //reg_osd3dflag_config
    _RVM1(0x1ef6, 0, BIT7 | BIT6 ),   //reg[101ef6]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPDIF_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_OUT_OEN (PAD_SPDIF_OUT_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SPDIF_OUT_OUT (PAD_SPDIF_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b3a, PAD_SPDIF_OUT_OUT, BIT0),
    _RVM1(0x2b3a, PAD_SPDIF_OUT_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_spdifoutconfig
    _RVM1(0x1eae, 0, BIT7 ),   //reg[101eae]#7 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_WS_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_WS_OEN (PAD_I2S_OUT_WS_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_OUT_WS_OUT (PAD_I2S_OUT_WS_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b3b, PAD_I2S_OUT_WS_OUT, BIT0),
    _RVM1(0x2b3b, PAD_I2S_OUT_WS_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_ej_config2[1:0]
    _RVM1(0x1e27, 0, BIT1 | BIT0 ),   //reg[101e27]#1 ~ #0 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_i2soutconfig0
    _RVM1(0x1eae, 0, BIT4 ),   //reg[101eae]#4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_MCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MCK_OEN (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_OUT_MCK_OUT (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b3c, PAD_I2S_OUT_MCK_OUT, BIT0),
    _RVM1(0x2b3c, PAD_I2S_OUT_MCK_OEN, BIT1),
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_i2soutconfig0
    _RVM1(0x1eae, 0, BIT4 ),   //reg[101eae]#4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_BCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_BCK_OEN (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_OUT_BCK_OUT (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b3d, PAD_I2S_OUT_BCK_OUT, BIT0),
    _RVM1(0x2b3d, PAD_I2S_OUT_BCK_OEN, BIT1),
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_i2soutconfig0
    _RVM1(0x1eae, 0, BIT4 ),   //reg[101eae]#4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_SD_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD_OEN (PAD_I2S_OUT_SD_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_OUT_SD_OUT (PAD_I2S_OUT_SD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b3e, PAD_I2S_OUT_SD_OUT, BIT0),
    _RVM1(0x2b3e, PAD_I2S_OUT_SD_OEN, BIT1),
    //reg_ej_config[1:0]
    _RVM1(0x1e05, 0, BIT5 | BIT4 ),   //reg[101e05]#5 ~ #4 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_i2soutconfig1
    _RVM1(0x1eae, 0, BIT5 ),   //reg[101eae]#5 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_SD1_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD1_OEN (PAD_I2S_OUT_SD1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_OUT_SD1_OUT (PAD_I2S_OUT_SD1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b3f, PAD_I2S_OUT_SD1_OUT, BIT0),
    _RVM1(0x2b3f, PAD_I2S_OUT_SD1_OEN, BIT1),
    //reg_spdifoutconfig2
    _RVM1(0x1e04, 0, BIT1 | BIT0 ),   //reg[101e04]#1 ~ #0 = 0b
    //reg_i2soutconfig2
    _RVM1(0x1eb3, 0, BIT0 ),   //reg[101eb3]#0 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_ext_int[7]
    _RVM1(0x1ea5, 0, BIT7 ),   //reg[101ea5]#7 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_SD2_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD2_OEN (PAD_I2S_OUT_SD2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_OUT_SD2_OUT (PAD_I2S_OUT_SD2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b40, PAD_I2S_OUT_SD2_OUT, BIT0),
    _RVM1(0x2b40, PAD_I2S_OUT_SD2_OEN, BIT1),
    //reg_i2soutconfig3
    _RVM1(0x1eb3, 0, BIT4 ),   //reg[101eb3]#4 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_fourthuartmode[1:0]
    _RVM1(0x1e04, 0, BIT7 | BIT6 ),   //reg[101e04]#7 ~ #6 = 0b
    //reg_od4thuart[1:0]
    _RVM1(0x1ea9, 0, BIT5 | BIT4 ),   //reg[101ea9]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_I2S_OUT_SD3_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD3_OEN (PAD_I2S_OUT_SD3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_I2S_OUT_SD3_OUT (PAD_I2S_OUT_SD3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b41, PAD_I2S_OUT_SD3_OUT, BIT0),
    _RVM1(0x2b41, PAD_I2S_OUT_SD3_OEN, BIT1),
    //reg_i2soutconfig4
    _RVM1(0x1eb3, 0, BIT5 ),   //reg[101eb3]#5 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_fourthuartmode[1:0]
    _RVM1(0x1e04, 0, BIT7 | BIT6 ),   //reg[101e04]#7 ~ #6 = 0b
    //reg_od4thuart[1:0]
    _RVM1(0x1ea9, 0, BIT5 | BIT4 ),   //reg[101ea9]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_VSYNC_LIKE_IS_GPIO != GPIO_NONE)
    #define PAD_VSYNC_LIKE_OEN (PAD_VSYNC_LIKE_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_VSYNC_LIKE_OUT (PAD_VSYNC_LIKE_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bb0, PAD_VSYNC_LIKE_OUT, BIT0),
    _RVM1(0x2bb0, PAD_VSYNC_LIKE_OEN, BIT1),
    //reg_vsync_like_config[1:0]
    _RVM1(0x1eb4, 0, BIT7 | BIT6 ),   //reg[101eb4]#7 ~ #6 = 0b
    //reg_vsync_like_config[1:0]
    _RVM1(0x1eb4, 0, BIT7 | BIT6 ),   //reg[101eb4]#7 ~ #6 = 0b
    //reg_vsync_like_config[1:0]
    _RVM1(0x1eb4, 0, BIT7 | BIT6 ),   //reg[101eb4]#7 ~ #6 = 0b
    //reg_ext_int[1]
    _RVM1(0x1ea5, 0, BIT1 ),   //reg[101ea5]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPI1_CK_IS_GPIO != GPIO_NONE)
    #define PAD_SPI1_CK_OEN (PAD_SPI1_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SPI1_CK_OUT (PAD_SPI1_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bb1, PAD_SPI1_CK_OUT, BIT0),
    _RVM1(0x2bb1, PAD_SPI1_CK_OEN, BIT1),
    //reg_ld_spi1_config
    _RVM1(0x1eb4, 0, BIT0 ),   //reg[101eb4]#0 = 0b
    //reg_ld_spi3_config
    _RVM1(0x1eb4, 0, BIT2 ),   //reg[101eb4]#2 = 0b
    //reg_dim_config
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPI1_DI_IS_GPIO != GPIO_NONE)
    #define PAD_SPI1_DI_OEN (PAD_SPI1_DI_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SPI1_DI_OUT (PAD_SPI1_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bb2, PAD_SPI1_DI_OUT, BIT0),
    _RVM1(0x2bb2, PAD_SPI1_DI_OEN, BIT1),
    //reg_ld_spi1_config
    _RVM1(0x1eb4, 0, BIT0 ),   //reg[101eb4]#0 = 0b
    //reg_ld_spi3_config
    _RVM1(0x1eb4, 0, BIT2 ),   //reg[101eb4]#2 = 0b
    //reg_dim_config
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPI2_CK_IS_GPIO != GPIO_NONE)
    #define PAD_SPI2_CK_OEN (PAD_SPI2_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SPI2_CK_OUT (PAD_SPI2_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bb3, PAD_SPI2_CK_OUT, BIT0),
    _RVM1(0x2bb3, PAD_SPI2_CK_OEN, BIT1),
    //reg_ld_spi2_config
    _RVM1(0x1eb4, 0, BIT1 ),   //reg[101eb4]#1 = 0b
    //reg_ld_spi3_config
    _RVM1(0x1eb4, 0, BIT2 ),   //reg[101eb4]#2 = 0b
    //reg_dim_config
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPI2_DI_IS_GPIO != GPIO_NONE)
    #define PAD_SPI2_DI_OEN (PAD_SPI2_DI_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SPI2_DI_OUT (PAD_SPI2_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bb4, PAD_SPI2_DI_OUT, BIT0),
    _RVM1(0x2bb4, PAD_SPI2_DI_OEN, BIT1),
    //reg_ld_spi2_config
    _RVM1(0x1eb4, 0, BIT1 ),   //reg[101eb4]#1 = 0b
    //reg_ld_spi3_config
    _RVM1(0x1eb4, 0, BIT2 ),   //reg[101eb4]#2 = 0b
    //reg_dim_config
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_DIM0_IS_GPIO != GPIO_NONE)
    #define PAD_DIM0_OEN (PAD_DIM0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DIM0_OUT (PAD_DIM0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bee, PAD_DIM0_OUT, BIT0),
    _RVM1(0x2bee, PAD_DIM0_OEN, BIT1),
    //reg_dim_config
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_DIM1_IS_GPIO != GPIO_NONE)
    #define PAD_DIM1_OEN (PAD_DIM1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DIM1_OUT (PAD_DIM1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bef, PAD_DIM1_OUT, BIT0),
    _RVM1(0x2bef, PAD_DIM1_OEN, BIT1),
    //reg_dim_config
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_DIM2_IS_GPIO != GPIO_NONE)
    #define PAD_DIM2_OEN (PAD_DIM2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DIM2_OUT (PAD_DIM2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf0, PAD_DIM2_OUT, BIT0),
    _RVM1(0x2bf0, PAD_DIM2_OEN, BIT1),
    //reg_dim_config
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]#0 = 0b
    //reg_fifthuartmode[1:0]
    _RVM1(0x1e08, 0, BIT3 | BIT2 ),   //reg[101e08]#3 ~ #2 = 0b
    //reg_od5thuart[1:0]
    _RVM1(0x1eaa, 0, BIT5 | BIT4 ),   //reg[101eaa]#5 ~ #4 = 0b
    //reg_vx1gpi_mode
    _RVM1(0x1e4a, 0, BIT0 ),   //reg[101e4a]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_DIM3_IS_GPIO != GPIO_NONE)
    #define PAD_DIM3_OEN (PAD_DIM3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DIM3_OUT (PAD_DIM3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf1, PAD_DIM3_OUT, BIT0),
    _RVM1(0x2bf1, PAD_DIM3_OEN, BIT1),
    //reg_dim_config
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]#0 = 0b
    //reg_fifthuartmode[1:0]
    _RVM1(0x1e08, 0, BIT3 | BIT2 ),   //reg[101e08]#3 ~ #2 = 0b
    //reg_od5thuart[1:0]
    _RVM1(0x1eaa, 0, BIT5 | BIT4 ),   //reg[101eaa]#5 ~ #4 = 0b
    //reg_vx1gpi_mode
    _RVM1(0x1e4a, 0, BIT0 ),   //reg[101e4a]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_CE_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM2_CE_N_OEN (PAD_PCM2_CE_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM2_CE_N_OUT (PAD_PCM2_CE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b63, PAD_PCM2_CE_N_OUT, BIT0),
    _RVM1(0x2b63, PAD_PCM2_CE_N_OEN, BIT1),
    //reg_pcm2ctrlconfig
    _RVM1(0x1ec8, 0, BIT3 ),   //reg[101ec8]#3 = 0b
    //reg_ext_int[0]
    _RVM1(0x1ea5, 0, BIT0 ),   //reg[101ea5]#0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_IRQA_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM2_IRQA_N_OEN (PAD_PCM2_IRQA_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM2_IRQA_N_OUT (PAD_PCM2_IRQA_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b64, PAD_PCM2_IRQA_N_OUT, BIT0),
    _RVM1(0x2b64, PAD_PCM2_IRQA_N_OEN, BIT1),
    //reg_pcm2ctrlconfig
    _RVM1(0x1ec8, 0, BIT3 ),   //reg[101ec8]#3 = 0b
    //reg_ext_int[2]
    _RVM1(0x1ea5, 0, BIT2 ),   //reg[101ea5]#2 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_WAIT_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM2_WAIT_N_OEN (PAD_PCM2_WAIT_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM2_WAIT_N_OUT (PAD_PCM2_WAIT_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b65, PAD_PCM2_WAIT_N_OUT, BIT0),
    _RVM1(0x2b65, PAD_PCM2_WAIT_N_OEN, BIT1),
    //reg_pcm2ctrlconfig
    _RVM1(0x1ec8, 0, BIT3 ),   //reg[101ec8]#3 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_RESET_IS_GPIO != GPIO_NONE)
    #define PAD_PCM2_RESET_OEN (PAD_PCM2_RESET_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM2_RESET_OUT (PAD_PCM2_RESET_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b66, PAD_PCM2_RESET_OUT, BIT0),
    _RVM1(0x2b66, PAD_PCM2_RESET_OEN, BIT1),
    //reg_pcm2ctrlconfig
    _RVM1(0x1ec8, 0, BIT3 ),   //reg[101ec8]#3 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM2_CD_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM2_CD_N_OEN (PAD_PCM2_CD_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM2_CD_N_OUT (PAD_PCM2_CD_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b67, PAD_PCM2_CD_N_OUT, BIT0),
    _RVM1(0x2b67, PAD_PCM2_CD_N_OEN, BIT1),
    //reg_pcm2ctrlconfig
    _RVM1(0x1ec8, 0, BIT3 ),   //reg[101ec8]#3 = 0b
    //reg_pcm2ctrlconfig_cd_n
    _RVM1(0x1e9e, 0, BIT0 ),   //reg[101e9e]#0 = 0b
    //reg_ext_int[3]
    _RVM1(0x1ea5, 0, BIT3 ),   //reg[101ea5]#3 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D3_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D3_OEN (PAD_PCM_D3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D3_OUT (PAD_PCM_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b42, PAD_PCM_D3_OUT, BIT0),
    _RVM1(0x2b42, PAD_PCM_D3_OEN, BIT1),
    //reg_pcm_pe[3]
    _RVM1(0x1e12, BIT3 , BIT3 ),   //reg[101e12]#3 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D4_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D4_OEN (PAD_PCM_D4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D4_OUT (PAD_PCM_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b43, PAD_PCM_D4_OUT, BIT0),
    _RVM1(0x2b43, PAD_PCM_D4_OEN, BIT1),
    //reg_pcm_pe[4]
    _RVM1(0x1e12, BIT4 , BIT4 ),   //reg[101e12]#4 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D5_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D5_OEN (PAD_PCM_D5_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D5_OUT (PAD_PCM_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b44, PAD_PCM_D5_OUT, BIT0),
    _RVM1(0x2b44, PAD_PCM_D5_OEN, BIT1),
    //reg_pcm_pe[5]
    _RVM1(0x1e12, BIT5 , BIT5 ),   //reg[101e12]#5 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D6_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D6_OEN (PAD_PCM_D6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D6_OUT (PAD_PCM_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b45, PAD_PCM_D6_OUT, BIT0),
    _RVM1(0x2b45, PAD_PCM_D6_OEN, BIT1),
    //reg_pcm_pe[6]
    _RVM1(0x1e12, BIT6 , BIT6 ),   //reg[101e12]#6 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D7_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D7_OEN (PAD_PCM_D7_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D7_OUT (PAD_PCM_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b46, PAD_PCM_D7_OUT, BIT0),
    _RVM1(0x2b46, PAD_PCM_D7_OEN, BIT1),
    //reg_pcm_pe[7]
    _RVM1(0x1e12, BIT7 , BIT7 ),   //reg[101e12]#7 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_CE_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_CE_N_OEN (PAD_PCM_CE_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_CE_N_OUT (PAD_PCM_CE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b47, PAD_PCM_CE_N_OUT, BIT0),
    _RVM1(0x2b47, PAD_PCM_CE_N_OEN, BIT1),
    //reg_pcm_pe[8]
    _RVM1(0x1e13, BIT0 , BIT0 ),   //reg[101e13]#0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_cictrlconfig
    _RVM1(0x1ec8, 0, BIT1 ),   //reg[101ec8]#1 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A10_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A10_OEN (PAD_PCM_A10_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A10_OUT (PAD_PCM_A10_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b48, PAD_PCM_A10_OUT, BIT0),
    _RVM1(0x2b48, PAD_PCM_A10_OEN, BIT1),
    //reg_pcm_pe[26]
    _RVM1(0x1e15, BIT2 , BIT2 ),   //reg[101e15]#2 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_OE_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_OE_N_OEN (PAD_PCM_OE_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_OE_N_OUT (PAD_PCM_OE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b49, PAD_PCM_OE_N_OUT, BIT0),
    _RVM1(0x2b49, PAD_PCM_OE_N_OEN, BIT1),
    //reg_pcm_pe[9]
    _RVM1(0x1e13, BIT1 , BIT1 ),   //reg[101e13]#1 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A11_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A11_OEN (PAD_PCM_A11_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A11_OUT (PAD_PCM_A11_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b4a, PAD_PCM_A11_OUT, BIT0),
    _RVM1(0x2b4a, PAD_PCM_A11_OEN, BIT1),
    //reg_pcm_pe[27]
    _RVM1(0x1e15, BIT3 , BIT3 ),   //reg[101e15]#3 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_IORD_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_IORD_N_OEN (PAD_PCM_IORD_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_IORD_N_OUT (PAD_PCM_IORD_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b4b, PAD_PCM_IORD_N_OUT, BIT0),
    _RVM1(0x2b4b, PAD_PCM_IORD_N_OEN, BIT1),
    //reg_pcm_pe[10]
    _RVM1(0x1e13, BIT2 , BIT2 ),   //reg[101e13]#2 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_cictrlconfig
    _RVM1(0x1ec8, 0, BIT1 ),   //reg[101ec8]#1 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A9_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A9_OEN (PAD_PCM_A9_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A9_OUT (PAD_PCM_A9_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b4c, PAD_PCM_A9_OUT, BIT0),
    _RVM1(0x2b4c, PAD_PCM_A9_OEN, BIT1),
    //reg_pcm_pe[25]
    _RVM1(0x1e15, BIT1 , BIT1 ),   //reg[101e15]#1 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_IOWR_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_IOWR_N_OEN (PAD_PCM_IOWR_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_IOWR_N_OUT (PAD_PCM_IOWR_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b4d, PAD_PCM_IOWR_N_OUT, BIT0),
    _RVM1(0x2b4d, PAD_PCM_IOWR_N_OEN, BIT1),
    //reg_pcm_pe[11]
    _RVM1(0x1e13, BIT3 , BIT3 ),   //reg[101e13]#3 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_cictrlconfig
    _RVM1(0x1ec8, 0, BIT1 ),   //reg[101ec8]#1 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A8_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A8_OEN (PAD_PCM_A8_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A8_OUT (PAD_PCM_A8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b4e, PAD_PCM_A8_OUT, BIT0),
    _RVM1(0x2b4e, PAD_PCM_A8_OEN, BIT1),
    //reg_pcm_pe[24]
    _RVM1(0x1e15, BIT0 , BIT0 ),   //reg[101e15]#0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A13_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A13_OEN (PAD_PCM_A13_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A13_OUT (PAD_PCM_A13_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b4f, PAD_PCM_A13_OUT, BIT0),
    _RVM1(0x2b4f, PAD_PCM_A13_OEN, BIT1),
    //reg_pcm_pe[29]
    _RVM1(0x1e15, BIT5 , BIT5 ),   //reg[101e15]#5 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A14_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A14_OEN (PAD_PCM_A14_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A14_OUT (PAD_PCM_A14_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b50, PAD_PCM_A14_OUT, BIT0),
    _RVM1(0x2b50, PAD_PCM_A14_OEN, BIT1),
    //reg_pcm_pe[30]
    _RVM1(0x1e15, BIT6 , BIT6 ),   //reg[101e15]#6 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_WE_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_WE_N_OEN (PAD_PCM_WE_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_WE_N_OUT (PAD_PCM_WE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b51, PAD_PCM_WE_N_OUT, BIT0),
    _RVM1(0x2b51, PAD_PCM_WE_N_OEN, BIT1),
    //reg_pcm_pe[12]
    _RVM1(0x1e13, BIT4 , BIT4 ),   //reg[101e13]#4 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_IRQA_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_IRQA_N_OEN (PAD_PCM_IRQA_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_IRQA_N_OUT (PAD_PCM_IRQA_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b52, PAD_PCM_IRQA_N_OUT, BIT0),
    _RVM1(0x2b52, PAD_PCM_IRQA_N_OEN, BIT1),
    //reg_pcm_pe[13]
    _RVM1(0x1e13, BIT5 , BIT5 ),   //reg[101e13]#5 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_cictrlconfig
    _RVM1(0x1ec8, 0, BIT1 ),   //reg[101ec8]#1 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A12_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A12_OEN (PAD_PCM_A12_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A12_OUT (PAD_PCM_A12_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b53, PAD_PCM_A12_OUT, BIT0),
    _RVM1(0x2b53, PAD_PCM_A12_OEN, BIT1),
    //reg_pcm_pe[28]
    _RVM1(0x1e15, BIT4 , BIT4 ),   //reg[101e15]#4 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A7_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A7_OEN (PAD_PCM_A7_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A7_OUT (PAD_PCM_A7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b54, PAD_PCM_A7_OUT, BIT0),
    _RVM1(0x2b54, PAD_PCM_A7_OEN, BIT1),
    //reg_pcm_pe[23]
    _RVM1(0x1e14, BIT7 , BIT7 ),   //reg[101e14]#7 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A6_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A6_OEN (PAD_PCM_A6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A6_OUT (PAD_PCM_A6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b55, PAD_PCM_A6_OUT, BIT0),
    _RVM1(0x2b55, PAD_PCM_A6_OEN, BIT1),
    //reg_pcm_pe[22]
    _RVM1(0x1e14, BIT6 , BIT6 ),   //reg[101e14]#6 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A5_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A5_OEN (PAD_PCM_A5_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A5_OUT (PAD_PCM_A5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b56, PAD_PCM_A5_OUT, BIT0),
    _RVM1(0x2b56, PAD_PCM_A5_OEN, BIT1),
    //reg_pcm_pe[21]
    _RVM1(0x1e14, BIT5 , BIT5 ),   //reg[101e14]#5 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_WAIT_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_WAIT_N_OEN (PAD_PCM_WAIT_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_WAIT_N_OUT (PAD_PCM_WAIT_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b57, PAD_PCM_WAIT_N_OUT, BIT0),
    _RVM1(0x2b57, PAD_PCM_WAIT_N_OEN, BIT1),
    //reg_pcm_pe[14]
    _RVM1(0x1e13, BIT6 , BIT6 ),   //reg[101e13]#6 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_cictrlconfig
    _RVM1(0x1ec8, 0, BIT1 ),   //reg[101ec8]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A4_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A4_OEN (PAD_PCM_A4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A4_OUT (PAD_PCM_A4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b58, PAD_PCM_A4_OUT, BIT0),
    _RVM1(0x2b58, PAD_PCM_A4_OEN, BIT1),
    //reg_pcm_pe[20]
    _RVM1(0x1e14, BIT4 , BIT4 ),   //reg[101e14]#4 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A3_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A3_OEN (PAD_PCM_A3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A3_OUT (PAD_PCM_A3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b59, PAD_PCM_A3_OUT, BIT0),
    _RVM1(0x2b59, PAD_PCM_A3_OEN, BIT1),
    //reg_pcm_pe[19]
    _RVM1(0x1e14, BIT3 , BIT3 ),   //reg[101e14]#3 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A2_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A2_OEN (PAD_PCM_A2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A2_OUT (PAD_PCM_A2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b5a, PAD_PCM_A2_OUT, BIT0),
    _RVM1(0x2b5a, PAD_PCM_A2_OEN, BIT1),
    //reg_pcm_pe[18]
    _RVM1(0x1e14, BIT2 , BIT2 ),   //reg[101e14]#2 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_REG_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_REG_N_OEN (PAD_PCM_REG_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_REG_N_OUT (PAD_PCM_REG_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b5b, PAD_PCM_REG_N_OUT, BIT0),
    _RVM1(0x2b5b, PAD_PCM_REG_N_OEN, BIT1),
    //reg_pcm_pe[15]
    _RVM1(0x1e13, BIT7 , BIT7 ),   //reg[101e13]#7 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_cictrlconfig
    _RVM1(0x1ec8, 0, BIT1 ),   //reg[101ec8]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A1_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A1_OEN (PAD_PCM_A1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A1_OUT (PAD_PCM_A1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b5c, PAD_PCM_A1_OUT, BIT0),
    _RVM1(0x2b5c, PAD_PCM_A1_OEN, BIT1),
    //reg_pcm_pe[17]
    _RVM1(0x1e14, BIT1 , BIT1 ),   //reg[101e14]#1 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_A0_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A0_OEN (PAD_PCM_A0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A0_OUT (PAD_PCM_A0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b5d, PAD_PCM_A0_OUT, BIT0),
    _RVM1(0x2b5d, PAD_PCM_A0_OEN, BIT1),
    //reg_pcm_pe[16]
    _RVM1(0x1e14, BIT0 , BIT0 ),   //reg[101e14]#0 = 1b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D0_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D0_OEN (PAD_PCM_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D0_OUT (PAD_PCM_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b5e, PAD_PCM_D0_OUT, BIT0),
    _RVM1(0x2b5e, PAD_PCM_D0_OEN, BIT1),
    //reg_pcm_pe[0]
    _RVM1(0x1e12, BIT0 , BIT0 ),   //reg[101e12]#0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D1_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D1_OEN (PAD_PCM_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D1_OUT (PAD_PCM_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b5f, PAD_PCM_D1_OUT, BIT0),
    _RVM1(0x2b5f, PAD_PCM_D1_OEN, BIT1),
    //reg_pcm_pe[1]
    _RVM1(0x1e12, BIT1 , BIT1 ),   //reg[101e12]#1 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_D2_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D2_OEN (PAD_PCM_D2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D2_OUT (PAD_PCM_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b60, PAD_PCM_D2_OUT, BIT0),
    _RVM1(0x2b60, PAD_PCM_D2_OEN, BIT1),
    //reg_pcm_pe[2]
    _RVM1(0x1e12, BIT2 , BIT2 ),   //reg[101e12]#2 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pcmadconfig
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 0b
    //reg_ciadconfig
    _RVM1(0x1ec8, 0, BIT0 ),   //reg[101ec8]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_RESET_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_RESET_OEN (PAD_PCM_RESET_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_RESET_OUT (PAD_PCM_RESET_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b61, PAD_PCM_RESET_OUT, BIT0),
    _RVM1(0x2b61, PAD_PCM_RESET_OEN, BIT1),
    //reg_pcm_pe[31]
    _RVM1(0x1e15, BIT7 , BIT7 ),   //reg[101e15]#7 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_cictrlconfig
    _RVM1(0x1ec8, 0, BIT1 ),   //reg[101ec8]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PCM_CD_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_CD_N_OEN (PAD_PCM_CD_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_CD_N_OUT (PAD_PCM_CD_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b62, PAD_PCM_CD_N_OUT, BIT0),
    _RVM1(0x2b62, PAD_PCM_CD_N_OEN, BIT1),
    //reg_pcm_pe[32]
    _RVM1(0x1e16, BIT0 , BIT0 ),   //reg[101e16]#0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_pcmctrlconfig
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PWM0_IS_GPIO != GPIO_NONE)
    #define PAD_PWM0_OEN (PAD_PWM0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PWM0_OUT (PAD_PWM0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b88, PAD_PWM0_OUT, BIT0),
    _RVM1(0x2b88, PAD_PWM0_OEN, BIT1),
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pwm0_mode
    _RVM1(0x1ec8, 0, BIT2 ),   //reg[101ec8]#2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PWM1_IS_GPIO != GPIO_NONE)
    #define PAD_PWM1_OEN (PAD_PWM1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PWM1_OUT (PAD_PWM1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b89, PAD_PWM1_OUT, BIT0),
    _RVM1(0x2b89, PAD_PWM1_OEN, BIT1),
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pwm1_mode
    _RVM1(0x1ec8, 0, BIT6 ),   //reg[101ec8]#6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PWM2_IS_GPIO != GPIO_NONE)
    #define PAD_PWM2_OEN (PAD_PWM2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PWM2_OUT (PAD_PWM2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b8a, PAD_PWM2_OUT, BIT0),
    _RVM1(0x2b8a, PAD_PWM2_OEN, BIT1),
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pwm2_mode
    _RVM1(0x1ec8, 0, BIT7 ),   //reg[101ec8]#7 = 0b
    //reg_ire_mode[1:0]
    _RVM1(0x1ede, 0, BIT5 | BIT4 ),   //reg[101ede]#5 ~ #4 = 00b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PWM3_IS_GPIO != GPIO_NONE)
    #define PAD_PWM3_OEN (PAD_PWM3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PWM3_OUT (PAD_PWM3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b8b, PAD_PWM3_OUT, BIT0),
    _RVM1(0x2b8b, PAD_PWM3_OEN, BIT1),
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_pwm3_mode
    _RVM1(0x1ec9, 0, BIT4 ),   //reg[101ec9]#4 = 0b
    //reg_ire_mode[1:0]
    _RVM1(0x1ede, 0, BIT5 | BIT4 ),   //reg[101ede]#5 ~ #4 = 00b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_PWM4_IS_GPIO != GPIO_NONE)
    #define PAD_PWM4_OEN (PAD_PWM4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PWM4_OUT (PAD_PWM4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b8c, PAD_PWM4_OUT, BIT0),
    _RVM1(0x2b8c, PAD_PWM4_OEN, BIT1),
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_i2smutemode[1:0]
    _RVM1(0x1e05, 0, BIT7 | BIT6 ),   //reg[101e05]#7 ~ #6 = 0b
    //reg_pwm4_mode
    _RVM1(0x1ec9, 0, BIT5 ),   //reg[101ec9]#5 = 0b
    //reg_ire_mode[1:0]
    _RVM1(0x1ede, 0, BIT5 | BIT4 ),   //reg[101ede]#5 ~ #4 = 00b
    //reg_tserrout[1:0]
    _RVM1(0x1ec9, 0, BIT1 | BIT0 ),   //reg[101ec9]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TGPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_TGPIO0_OEN (PAD_TGPIO0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TGPIO0_OUT (PAD_TGPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b8d, PAD_TGPIO0_OUT, BIT0),
    _RVM1(0x2b8d, PAD_TGPIO0_OEN, BIT1),
    //reg_vsync_vif_out_en
    _RVM1(0x1ea3, 0, BIT6 ),   //reg[101ea3]#6 = 0b
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TGPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_TGPIO1_OEN (PAD_TGPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TGPIO1_OUT (PAD_TGPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b8e, PAD_TGPIO1_OUT, BIT0),
    _RVM1(0x2b8e, PAD_TGPIO1_OEN, BIT1),
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TGPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_TGPIO2_OEN (PAD_TGPIO2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TGPIO2_OUT (PAD_TGPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b8f, PAD_TGPIO2_OUT, BIT0),
    _RVM1(0x2b8f, PAD_TGPIO2_OEN, BIT1),
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_miic_mode1
    _RVM1(0x1edc, 0, BIT1 ),   //reg[101edc]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TGPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_TGPIO3_OEN (PAD_TGPIO3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TGPIO3_OUT (PAD_TGPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b90, PAD_TGPIO3_OUT, BIT0),
    _RVM1(0x2b90, PAD_TGPIO3_OEN, BIT1),
    //reg_mcujtagmode[1:0]
    _RVM1(0x1ede, 0, BIT3 | BIT2 ),   //reg[101ede]#3 ~ #2 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_miic_mode1
    _RVM1(0x1edc, 0, BIT1 ),   //reg[101edc]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D0_OEN (PAD_TS0_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D0_OUT (PAD_TS0_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b1c, PAD_TS0_D0_OUT, BIT0),
    _RVM1(0x2b1c, PAD_TS0_D0_OEN, BIT1),
    //reg_ts0_pe[0]
    _RVM1(0x1e1c, BIT0 , BIT0 ),   //reg[101e1c]#0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D1_OEN (PAD_TS0_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D1_OUT (PAD_TS0_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b1d, PAD_TS0_D1_OUT, BIT0),
    _RVM1(0x2b1d, PAD_TS0_D1_OEN, BIT1),
    //reg_ts0_pe[1]
    _RVM1(0x1e1c, BIT1 , BIT1 ),   //reg[101e1c]#1 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D2_OEN (PAD_TS0_D2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D2_OUT (PAD_TS0_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b1e, PAD_TS0_D2_OUT, BIT0),
    _RVM1(0x2b1e, PAD_TS0_D2_OEN, BIT1),
    //reg_ts0_pe[2]
    _RVM1(0x1e1c, BIT2 , BIT2 ),   //reg[101e1c]#2 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D3_OEN (PAD_TS0_D3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D3_OUT (PAD_TS0_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b1f, PAD_TS0_D3_OUT, BIT0),
    _RVM1(0x2b1f, PAD_TS0_D3_OEN, BIT1),
    //reg_ts0_pe[3]
    _RVM1(0x1e1c, BIT3 , BIT3 ),   //reg[101e1c]#3 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D4_OEN (PAD_TS0_D4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D4_OUT (PAD_TS0_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b20, PAD_TS0_D4_OUT, BIT0),
    _RVM1(0x2b20, PAD_TS0_D4_OEN, BIT1),
    //reg_ts0_pe[4]
    _RVM1(0x1e1c, BIT4 , BIT4 ),   //reg[101e1c]#4 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D5_OEN (PAD_TS0_D5_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D5_OUT (PAD_TS0_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b21, PAD_TS0_D5_OUT, BIT0),
    _RVM1(0x2b21, PAD_TS0_D5_OEN, BIT1),
    //reg_ts0_pe[5]
    _RVM1(0x1e1c, BIT5 , BIT5 ),   //reg[101e1c]#5 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D6_OEN (PAD_TS0_D6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D6_OUT (PAD_TS0_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b22, PAD_TS0_D6_OUT, BIT0),
    _RVM1(0x2b22, PAD_TS0_D6_OEN, BIT1),
    //reg_ts0_pe[6]
    _RVM1(0x1e1c, BIT6 , BIT6 ),   //reg[101e1c]#6 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D7_OEN (PAD_TS0_D7_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D7_OUT (PAD_TS0_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b23, PAD_TS0_D7_OUT, BIT0),
    _RVM1(0x2b23, PAD_TS0_D7_OEN, BIT1),
    //reg_ts0_pe[7]
    _RVM1(0x1e1c, BIT7 , BIT7 ),   //reg[101e1c]#7 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_VLD_OEN (PAD_TS0_VLD_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_VLD_OUT (PAD_TS0_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b24, PAD_TS0_VLD_OUT, BIT0),
    _RVM1(0x2b24, PAD_TS0_VLD_OEN, BIT1),
    //reg_ts0_pe[8]
    _RVM1(0x1e1d, BIT0 , BIT0 ),   //reg[101e1d]#0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_SYNC_OEN (PAD_TS0_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_SYNC_OUT (PAD_TS0_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b25, PAD_TS0_SYNC_OUT, BIT0),
    _RVM1(0x2b25, PAD_TS0_SYNC_OEN, BIT1),
    //reg_ts0_pe[9]
    _RVM1(0x1e1d, BIT1 , BIT1 ),   //reg[101e1d]#1 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_CLK_OEN (PAD_TS0_CLK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_CLK_OUT (PAD_TS0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b26, PAD_TS0_CLK_OUT, BIT0),
    _RVM1(0x2b26, PAD_TS0_CLK_OEN, BIT1),
    //reg_ts0_pe[10]
    _RVM1(0x1e1d, BIT2 , BIT2 ),   //reg[101e1d]#2 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_ts0config[2:0]
    _RVM1(0x1eaf, 0, BIT2 | BIT1 | BIT0 ),   //reg[101eaf]#2 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_CLK_OEN (PAD_TS1_CLK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_CLK_OUT (PAD_TS1_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b27, PAD_TS1_CLK_OUT, BIT0),
    _RVM1(0x2b27, PAD_TS1_CLK_OEN, BIT1),
    //reg_ts1_pe[10]
    _RVM1(0x1e0d, BIT2 , BIT2 ),   //reg[101e0d]#2 = 1b
    //reg_lgd_rgbw_pwmtin_ts1
    _RVM1(0x1e3e, 0, BIT0 ),   //reg[101e3e]#0 = 0b
    //reg_tconconfig10_ts1
    _RVM1(0x1e3f, 0, BIT7 ),   //reg[101e3f]#7 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_pdtracectrl[1:0]
    _RVM1(0x1e9f, 0, BIT1 | BIT0 ),   //reg[101e9f]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_SYNC_OEN (PAD_TS1_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_SYNC_OUT (PAD_TS1_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b28, PAD_TS1_SYNC_OUT, BIT0),
    _RVM1(0x2b28, PAD_TS1_SYNC_OEN, BIT1),
    //reg_ts1_pe[9]
    _RVM1(0x1e0d, BIT1 , BIT1 ),   //reg[101e0d]#1 = 1b
    //reg_tconconfig9_ts1
    _RVM1(0x1e3f, 0, BIT6 ),   //reg[101e3f]#6 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_pdtracectrl[1:0]
    _RVM1(0x1e9f, 0, BIT1 | BIT0 ),   //reg[101e9f]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_VLD_OEN (PAD_TS1_VLD_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_VLD_OUT (PAD_TS1_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b29, PAD_TS1_VLD_OUT, BIT0),
    _RVM1(0x2b29, PAD_TS1_VLD_OEN, BIT1),
    //reg_ts1_pe[8]
    _RVM1(0x1e0d, BIT0 , BIT0 ),   //reg[101e0d]#0 = 1b
    //reg_lgd_rgbw_reverse_ts1
    _RVM1(0x1e3e, 0, BIT4 ),   //reg[101e3e]#4 = 0b
    //reg_tconconfig8_ts1
    _RVM1(0x1e3f, 0, BIT5 ),   //reg[101e3f]#5 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_pdtracectrl[1:0]
    _RVM1(0x1e9f, 0, BIT1 | BIT0 ),   //reg[101e9f]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D7_OEN (PAD_TS1_D7_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D7_OUT (PAD_TS1_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b2a, PAD_TS1_D7_OUT, BIT0),
    _RVM1(0x2b2a, PAD_TS1_D7_OEN, BIT1),
    //reg_ts1_pe[7]
    _RVM1(0x1e0c, BIT7 , BIT7 ),   //reg[101e0c]#7 = 1b
    //reg_tconconfig7_ts1
    _RVM1(0x1e3f, 0, BIT4 ),   //reg[101e3f]#4 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_pdtracectrl[1:0]
    _RVM1(0x1e9f, 0, BIT1 | BIT0 ),   //reg[101e9f]#1 ~ #0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D6_OEN (PAD_TS1_D6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D6_OUT (PAD_TS1_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b2b, PAD_TS1_D6_OUT, BIT0),
    _RVM1(0x2b2b, PAD_TS1_D6_OEN, BIT1),
    //reg_ts1_pe[6]
    _RVM1(0x1e0c, BIT6 , BIT6 ),   //reg[101e0c]#6 = 1b
    //reg_lgd_rgbw_pwmtout3_ts1
    _RVM1(0x1e3e, 0, BIT3 ),   //reg[101e3e]#3 = 0b
    //reg_tconconfig6_ts1
    _RVM1(0x1e3f, 0, BIT3 ),   //reg[101e3f]#3 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_pdtracectrl[1:0]
    _RVM1(0x1e9f, 0, BIT1 | BIT0 ),   //reg[101e9f]#1 ~ #0 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D5_OEN (PAD_TS1_D5_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D5_OUT (PAD_TS1_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b2c, PAD_TS1_D5_OUT, BIT0),
    _RVM1(0x2b2c, PAD_TS1_D5_OEN, BIT1),
    //reg_ts1_pe[5]
    _RVM1(0x1e0c, BIT5 , BIT5 ),   //reg[101e0c]#5 = 1b
    //reg_lgd_rgbw_pwmtout2_ts1
    _RVM1(0x1e3e, 0, BIT2 ),   //reg[101e3e]#2 = 0b
    //reg_tconconfig5_ts1
    _RVM1(0x1e3f, 0, BIT2 ),   //reg[101e3f]#2 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_pdtracectrl[1:0]
    _RVM1(0x1e9f, 0, BIT1 | BIT0 ),   //reg[101e9f]#1 ~ #0 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D4_OEN (PAD_TS1_D4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D4_OUT (PAD_TS1_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b2d, PAD_TS1_D4_OUT, BIT0),
    _RVM1(0x2b2d, PAD_TS1_D4_OEN, BIT1),
    //reg_ts1_pe[4]
    _RVM1(0x1e0c, BIT4 , BIT4 ),   //reg[101e0c]#4 = 1b
    //reg_lgd_rgbw_pwmtout1_ts1
    _RVM1(0x1e3e, 0, BIT1 ),   //reg[101e3e]#1 = 0b
    //reg_tconconfig4_ts1
    _RVM1(0x1e3f, 0, BIT1 ),   //reg[101e3f]#1 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_dspejtagmode[1:0]
    _RVM1(0x1e07, 0, BIT1 | BIT0 ),   //reg[101e07]#1 ~ #0 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D3_OEN (PAD_TS1_D3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D3_OUT (PAD_TS1_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b2e, PAD_TS1_D3_OUT, BIT0),
    _RVM1(0x2b2e, PAD_TS1_D3_OEN, BIT1),
    //reg_ts1_pe[3]
    _RVM1(0x1e0c, BIT3 , BIT3 ),   //reg[101e0c]#3 = 1b
    //reg_tconconfig3_ts1
    _RVM1(0x1e3f, 0, BIT0 ),   //reg[101e3f]#0 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D2_OEN (PAD_TS1_D2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D2_OUT (PAD_TS1_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b2f, PAD_TS1_D2_OUT, BIT0),
    _RVM1(0x2b2f, PAD_TS1_D2_OEN, BIT1),
    //reg_ts1_pe[2]
    _RVM1(0x1e0c, BIT2 , BIT2 ),   //reg[101e0c]#2 = 1b
    //reg_tconconfig2_ts1
    _RVM1(0x1e3e, 0, BIT7 ),   //reg[101e3e]#7 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D1_OEN (PAD_TS1_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D1_OUT (PAD_TS1_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b30, PAD_TS1_D1_OUT, BIT0),
    _RVM1(0x2b30, PAD_TS1_D1_OEN, BIT1),
    //reg_ts1_pe[1]
    _RVM1(0x1e0c, BIT1 , BIT1 ),   //reg[101e0c]#1 = 1b
    //reg_tconconfig1_ts1
    _RVM1(0x1e3e, 0, BIT6 ),   //reg[101e3e]#6 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_sm_config[1:0]
    _RVM1(0x1edc, 0, BIT5 | BIT4 ),   //reg[101edc]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS1_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D0_OEN (PAD_TS1_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D0_OUT (PAD_TS1_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b31, PAD_TS1_D0_OUT, BIT0),
    _RVM1(0x2b31, PAD_TS1_D0_OEN, BIT1),
    //reg_ts1_pe[0]
    _RVM1(0x1e0c, BIT0 , BIT0 ),   //reg[101e0c]#0 = 1b
    //reg_tconconfig0_ts1
    _RVM1(0x1e3e, 0, BIT5 ),   //reg[101e3e]#5 = 0b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts1config[2:0]
    _RVM1(0x1eaf, 0, BIT5 | BIT4 | BIT3 ),   //reg[101eaf]#5 ~ #3 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_ts_out_mode
    _RVM1(0x1e18, 0, BIT6 | BIT5 | BIT4 ),   //reg[101e18]#6 ~#4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO9_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO9_OEN (PAD_EMMC_IO9_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO9_OUT (PAD_EMMC_IO9_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b78, PAD_EMMC_IO9_OUT, BIT0),
    _RVM1(0x2b78, PAD_EMMC_IO9_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO12_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO12_OEN (PAD_EMMC_IO12_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO12_OUT (PAD_EMMC_IO12_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b79, PAD_EMMC_IO12_OUT, BIT0),
    _RVM1(0x2b79, PAD_EMMC_IO12_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_nand_cs1_en
    _RVM1(0x1ede, 0, BIT5 ),   //reg[101ede]#5 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO14_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO14_OEN (PAD_EMMC_IO14_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO14_OUT (PAD_EMMC_IO14_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b7a, PAD_EMMC_IO14_OUT, BIT0),
    _RVM1(0x2b7a, PAD_EMMC_IO14_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO10_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO10_OEN (PAD_EMMC_IO10_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO10_OUT (PAD_EMMC_IO10_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b7b, PAD_EMMC_IO10_OUT, BIT0),
    _RVM1(0x2b7b, PAD_EMMC_IO10_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO16_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO16_OEN (PAD_EMMC_IO16_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO16_OUT (PAD_EMMC_IO16_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b7c, PAD_EMMC_IO16_OUT, BIT0),
    _RVM1(0x2b7c, PAD_EMMC_IO16_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO17_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO17_OEN (PAD_EMMC_IO17_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO17_OUT (PAD_EMMC_IO17_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b7d, PAD_EMMC_IO17_OUT, BIT0),
    _RVM1(0x2b7d, PAD_EMMC_IO17_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO15_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO15_OEN (PAD_EMMC_IO15_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO15_OUT (PAD_EMMC_IO15_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b7e, PAD_EMMC_IO15_OUT, BIT0),
    _RVM1(0x2b7e, PAD_EMMC_IO15_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO11_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO11_OEN (PAD_EMMC_IO11_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO11_OUT (PAD_EMMC_IO11_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b7f, PAD_EMMC_IO11_OUT, BIT0),
    _RVM1(0x2b7f, PAD_EMMC_IO11_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_in_mode[2:0]
    //_RVM1(0x1e24, 0, BIT2 | BIT1 | BIT0 ),   //reg[101e24]#2 ~ #0 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_rstn_en_pm
    _RVM1(0x0e62, 0, BIT4 ),   //reg[0e62]#4 = 0b
    //reg_emmc_rstn_en
    _RVM1(0x1e9e, 0, BIT2 ),   //reg[101e9e]#2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO8_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO8_OEN (PAD_EMMC_IO8_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO8_OUT (PAD_EMMC_IO8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b80, PAD_EMMC_IO8_OUT, BIT0),
    _RVM1(0x2b80, PAD_EMMC_IO8_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO4_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO4_OEN (PAD_EMMC_IO4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO4_OUT (PAD_EMMC_IO4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bac, PAD_EMMC_IO4_OUT, BIT0),
    _RVM1(0x2bac, PAD_EMMC_IO4_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO3_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO3_OEN (PAD_EMMC_IO3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO3_OUT (PAD_EMMC_IO3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bad, PAD_EMMC_IO3_OUT, BIT0),
    _RVM1(0x2bad, PAD_EMMC_IO3_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO0_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO0_OEN (PAD_EMMC_IO0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO0_OUT (PAD_EMMC_IO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bae, PAD_EMMC_IO0_OUT, BIT0),
    _RVM1(0x2bae, PAD_EMMC_IO0_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b
    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_test_out_mode[2:0]
    //_RVM1(0x1e24, 0, BIT6 ~ BIT4 ),   //reg[101e24]#6 ~ #4 = 00b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D0_OEN (PAD_TS2_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_D0_OUT (PAD_TS2_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b32, PAD_TS2_D0_OUT, BIT0),
    _RVM1(0x2b32, PAD_TS2_D0_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_VLD_OEN (PAD_TS2_VLD_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_VLD_OUT (PAD_TS2_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b33, PAD_TS2_VLD_OUT, BIT0),
    _RVM1(0x2b33, PAD_TS2_VLD_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_SYNC_OEN (PAD_TS2_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_SYNC_OUT (PAD_TS2_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b34, PAD_TS2_SYNC_OUT, BIT0),
    _RVM1(0x2b34, PAD_TS2_SYNC_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_CLK_OEN (PAD_TS2_CLK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_CLK_OUT (PAD_TS2_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b35, PAD_TS2_CLK_OUT, BIT0),
    _RVM1(0x2b35, PAD_TS2_CLK_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D1_OEN (PAD_TS2_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_D1_OUT (PAD_TS2_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf2, PAD_TS2_D1_OUT, BIT0),
    _RVM1(0x2bf2, PAD_TS2_D1_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D2_OEN (PAD_TS2_D2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_D2_OUT (PAD_TS2_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf3, PAD_TS2_D2_OUT, BIT0),
    _RVM1(0x2bf3, PAD_TS2_D2_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D3_OEN (PAD_TS2_D3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_D3_OUT (PAD_TS2_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf4, PAD_TS2_D3_OUT, BIT0),
    _RVM1(0x2bf4, PAD_TS2_D3_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D4_OEN (PAD_TS2_D4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_D4_OUT (PAD_TS2_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf5, PAD_TS2_D4_OUT, BIT0),
    _RVM1(0x2bf5, PAD_TS2_D4_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D5_OEN (PAD_TS2_D5_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_D5_OUT (PAD_TS2_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf6, PAD_TS2_D5_OUT, BIT0),
    _RVM1(0x2bf6, PAD_TS2_D5_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D6_OEN (PAD_TS2_D6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_D6_OUT (PAD_TS2_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf7, PAD_TS2_D6_OUT, BIT0),
    _RVM1(0x2bf7, PAD_TS2_D6_OEN, BIT1),
    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif



    #if(PAD_TS2_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D7_OEN (PAD_TS2_D7_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS2_D7_OUT (PAD_TS2_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2bf8, PAD_TS2_D7_OUT, BIT0),
    _RVM1(0x2bf8, PAD_TS2_D7_OEN, BIT1),

    _MEMMAP_nonPM_11_,

    //reg_sense_en
    _RVM1(0x0321, 0, BIT0 ),   //reg[110321]#0 = 0b
    //reg_vsense_pe
    _RVM1(0x0321, BIT1 , BIT1 ),   //reg[110321]#1 = 1b

    _MEMMAP_nonPM,

    //reg_ts2config[1:0]
    _RVM1(0x1eaf, 0, BIT7 | BIT6 ),   //reg[101eaf]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_D0_OEN (PAD_TS3_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_D0_OUT (PAD_TS3_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0300, PAD_TS3_D0_OUT, BIT0),
    _RVM1(0x0300, PAD_TS3_D0_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_tconconfig[0]
    _RVM1(0x1ea0, 0, BIT0 ),   //reg[101ea0]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_D1_OEN (PAD_TS3_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_D1_OUT (PAD_TS3_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0301, PAD_TS3_D1_OUT, BIT0),
    _RVM1(0x0301, PAD_TS3_D1_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_tconconfig[1]
    _RVM1(0x1ea0, 0, BIT1 ),   //reg[101ea0]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_D2_OEN (PAD_TS3_D2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_D2_OUT (PAD_TS3_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0302, PAD_TS3_D2_OUT, BIT0),
    _RVM1(0x0302, PAD_TS3_D2_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_tconconfig[2]
    _RVM1(0x1ea0, 0, BIT2 ),   //reg[101ea0]#2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_D3_OEN (PAD_TS3_D3_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_D3_OUT (PAD_TS3_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0303, PAD_TS3_D3_OUT, BIT0),
    _RVM1(0x0303, PAD_TS3_D3_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_tconconfig[3]
    _RVM1(0x1ea0, 0, BIT3 ),   //reg[101ea0]#3 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_D4_OEN (PAD_TS3_D4_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_D4_OUT (PAD_TS3_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0304, PAD_TS3_D4_OUT, BIT0),
    _RVM1(0x0304, PAD_TS3_D4_OEN, BIT1),
    //reg_lgd_rgbw_pwmtout1
    _RVM1(0x1e3c, 0, BIT1 ),   //reg[101e3c]#1 = 0b
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_i2sout_in_tcon
    _RVM1(0x1e3c, 0, BIT0 ),   //reg[101e3c]#0 = 0b
    //reg_tconconfig[4]
    _RVM1(0x1ea0, 0, BIT4 ),   //reg[101ea0]#4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_D5_OEN (PAD_TS3_D5_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_D5_OUT (PAD_TS3_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0305, PAD_TS3_D5_OUT, BIT0),
    _RVM1(0x0305, PAD_TS3_D5_OEN, BIT1),
    //reg_lgd_rgbw_pwmtout2
    _RVM1(0x1e3c, 0, BIT2 ),   //reg[101e3c]#2 = 0b
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_i2sout_in_tcon
    _RVM1(0x1e3c, 0, BIT0 ),   //reg[101e3c]#0 = 0b
    //reg_tconconfig[5]
    _RVM1(0x1ea0, 0, BIT5 ),   //reg[101ea0]#5 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_D6_OEN (PAD_TS3_D6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_D6_OUT (PAD_TS3_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0306, PAD_TS3_D6_OUT, BIT0),
    _RVM1(0x0306, PAD_TS3_D6_OEN, BIT1),
    //reg_lgd_rgbw_pwmtout3
    _RVM1(0x1e3c, 0, BIT3 ),   //reg[101e3c]#3 = 0b
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_i2sout_in_tcon
    _RVM1(0x1e3c, 0, BIT0 ),   //reg[101e3c]#0 = 0b
    //reg_tconconfig[6]
    _RVM1(0x1ea0, 0, BIT6 ),   //reg[101ea0]#6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_D7_OEN (PAD_TS3_D7_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_D7_OUT (PAD_TS3_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0307, PAD_TS3_D7_OUT, BIT0),
    _RVM1(0x0307, PAD_TS3_D7_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_i2sout_in_tcon
    _RVM1(0x1e3c, 0, BIT0 ),   //reg[101e3c]#0 = 0b
    //reg_tconconfig[7]
    _RVM1(0x1ea0, 0, BIT7 ),   //reg[101ea0]#7 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_VLD_OEN (PAD_TS3_VLD_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_VLD_OUT (PAD_TS3_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0308, PAD_TS3_VLD_OUT, BIT0),
    _RVM1(0x0308, PAD_TS3_VLD_OEN, BIT1),
    //reg_lgd_rgbw_reverse
    _RVM1(0x1e3c, 0, BIT4 ),   //reg[101e3c]#4 = 0b
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_i2sout_in_tcon
    _RVM1(0x1e3c, 0, BIT0 ),   //reg[101e3c]#0 = 0b
    //reg_tconconfig[8]
    _RVM1(0x1ea1, 0, BIT0 ),   //reg[101ea1]#0 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_SYNC_OEN (PAD_TS3_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_SYNC_OUT (PAD_TS3_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0309, PAD_TS3_SYNC_OUT, BIT0),
    _RVM1(0x0309, PAD_TS3_SYNC_OEN, BIT1),
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_i2sout_in_tcon
    _RVM1(0x1e3c, 0, BIT0 ),   //reg[101e3c]#0 = 0b
    //reg_tconconfig[9]
    _RVM1(0x1ea1, 0, BIT1 ),   //reg[101ea1]#1 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS3_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS3_CLK_OEN (PAD_TS3_CLK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS3_CLK_OUT (PAD_TS3_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x030a, PAD_TS3_CLK_OUT, BIT0),
    _RVM1(0x030a, PAD_TS3_CLK_OEN, BIT1),
    //reg_lgd_rgbw_pwmtin
    _RVM1(0x1e3c, 0, BIT0 ),   //reg[101e3c]#0 = 0b
    //reg_tso_evd_mode
    _RVM1(0x1e21, 0, BIT1 ),   //reg[101e21]#1 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_ts3config[2:0]
    _RVM1(0x1ecf, 0, BIT7 | BIT6 | BIT5 ),   //reg[101ecf]#7 ~ #5 = 0b
    //reg_i2sout_in_tcon
    _RVM1(0x1e3c, 0, BIT0 ),   //reg[101e3c]#0 = 0b
    //reg_tconconfig[10]
    _RVM1(0x1ea1, 0, BIT2 ),   //reg[101ea1]#2 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO13_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO13_OEN (PAD_EMMC_IO13_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO13_OUT (PAD_EMMC_IO13_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)

    _MEMMAP_nonPM_12_,

    _RVM1(0x2b81, PAD_EMMC_IO13_OUT, BIT0),
    _RVM1(0x2b81, PAD_EMMC_IO13_OEN, BIT1),
    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b

    _MEMMAP_nonPM_,

    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_sd_config[1:0]
    _RVM1(0x1eb5, 0, BIT1 | BIT0 ),   //reg[101eb5]#1 ~ #0 = 0b
    //reg_sdio_config[1:0]
    _RVM1(0x1ef6, 0, BIT5 | BIT4 ),   //reg[101ef6]#5 ~ #4 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO1_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO1_OEN (PAD_EMMC_IO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO1_OUT (PAD_EMMC_IO1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b82, PAD_EMMC_IO1_OUT, BIT0),
    _RVM1(0x2b82, PAD_EMMC_IO1_OEN, BIT1),

    _MEMMAP_nonPM_12_,

    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b

    _MEMMAP_nonPM_,

    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO2_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO2_OEN (PAD_EMMC_IO2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO2_OUT (PAD_EMMC_IO2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b83, PAD_EMMC_IO2_OUT, BIT0),
    _RVM1(0x2b83, PAD_EMMC_IO2_OEN, BIT1),

    _MEMMAP_nonPM_12_,

    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b

    _MEMMAP_nonPM_,

    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO7_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO7_OEN (PAD_EMMC_IO7_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO7_OUT (PAD_EMMC_IO7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b84, PAD_EMMC_IO7_OUT, BIT0),
    _RVM1(0x2b84, PAD_EMMC_IO7_OEN, BIT1),

    _MEMMAP_nonPM_12_,

    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b

    _MEMMAP_nonPM_,

    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO6_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO6_OEN (PAD_EMMC_IO6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO6_OUT (PAD_EMMC_IO6_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b85, PAD_EMMC_IO6_OUT, BIT0),
    _RVM1(0x2b85, PAD_EMMC_IO6_OEN, BIT1),

    _MEMMAP_nonPM_12_,

    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b

    _MEMMAP_nonPM_,

    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_EMMC_IO5_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO5_OEN (PAD_EMMC_IO5_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_EMMC_IO5_OUT (PAD_EMMC_IO5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2b76, PAD_EMMC_IO5_OUT, BIT0),
    _RVM1(0x2b76, PAD_EMMC_IO5_OEN, BIT1),

    _MEMMAP_nonPM_12_,

    //reg_lb_en
    _RVM1(0x3f80, 0, BIT0 ),   //reg[123f80]#0 = 0b
    //reg_lb_bypass_mode
    _RVM1(0x3f80, 0, BIT2 ),   //reg[123f80]#2 = 0b

    _MEMMAP_nonPM_,

    //reg_fcie2macro_sd_bypass
    _RVM1(0x1e21, BIT0 , BIT0 ),   //reg[101e21] #0 = 1b
    //reg_nand_mode[1:0]
    _RVM1(0x1ede, 0, BIT7 | BIT6 ),   //reg[101ede]#7 ~ #6 = 0b
    //reg_emmc_config[1:0]
    _RVM1(0x1edc, 0, BIT7 | BIT6 ),   //reg[101edc]#7 ~ #6 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_ARC0_IS_GPIO != GPIO_NONE)
    #define PAD_ARC0_OEN (PAD_ARC0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_ARC0_OUT (PAD_ARC0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x0320, PAD_ARC0_OUT, BIT0),
    _RVM1(0x0320, PAD_ARC0_OEN, BIT1),

    _MEMMAP_nonPM_11_,

    //reg_arc_gpio_en
    _RVM1(0x0320, BIT4 , BIT4 ),   //reg[110320]#4 = 1b
    //reg_arc_mode
    _RVM1(0x0320, 0, BIT3 ),   //reg[110320]#3 = 0b
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    #endif



//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------

    _MEMMAP_nonPM_,

//---------------------------------------------------------------------
#ifdef PADS_LDG_RGBW_REVERSE_TS1
#if (PADS_LDG_RGBW_REVERSE_TS1 != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_REVERSE_TS1 ((PADS_LDG_RGBW_REVERSE_TS1 == ENABLE) ? BIT4 : 0)
    _RVM1(0x1E4C, LDG_RGBW_REVERSE_TS1, BIT4),
#endif
#endif

#ifdef PADS_LDG_RGBW_PWMTIN_TS1
#if (PADS_LDG_RGBW_PWMTIN_TS1 != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_PWMTIN_TS1 ((PADS_LDG_RGBW_PWMTIN_TS1 == ENABLE) ? BIT0 : 0)
    _RVM1(0x1E4C, LDG_RGBW_PWMTIN_TS1, BIT0),
#endif
#endif

#ifdef PADS_LDG_RGBW_PWMTOUT1_TS1
#if (PADS_LDG_RGBW_PWMTOUT1_TS1 != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_PWMTOUT1_TS1 ((PADS_LDG_RGBW_PWMTOUT1_TS1 == ENABLE) ? BIT1 : 0)
    _RVM1(0x1E4C, LDG_RGBW_PWMTOUT1_TS1, BIT1),
#endif
#endif

#ifdef PADS_LDG_RGBW_PWMTOUT2_TS1
#if (PADS_LDG_RGBW_PWMTOUT2_TS1 != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_PWMTOUT2_TS1 ((PADS_LDG_RGBW_PWMTOUT2_TS1 == ENABLE) ? BIT2 : 0)
    _RVM1(0x1E4C, LDG_RGBW_PWMTOUT2_TS1, BIT2),
#endif
#endif

#ifdef PADS_LDG_RGBW_PWMTOUT3_TS1
#if (PADS_LDG_RGBW_PWMTOUT3_TS1 != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_PWMTOUT3_TS1 ((PADS_LDG_RGBW_PWMTOUT3_TS1 == ENABLE) ? BIT3 : 0)
    _RVM1(0x1E4C, LDG_RGBW_PWMTOUT3_TS1, BIT3),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_TCON_CFG0_TS1
#if (PADS_TCON_CFG0_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG0_TS1 ((PADS_TCON_CFG0_TS1 == ENABLE)  ? BIT0 : 0)
    _RVM2(0x1E8A, TCON_CFG0_TS1, BIT0),
#endif
#endif

#ifdef PADS_TCON_CFG1_TS1
#if (PADS_TCON_CFG1_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG1_TS1 ((PADS_TCON_CFG1_TS1 == ENABLE)  ? BIT1 : 0)
    _RVM2(0x1E8A, TCON_CFG1_TS1, BIT1),
#endif
#endif

#ifdef PADS_TCON_CFG2_TS1
#if (PADS_TCON_CFG2_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG2_TS1 ((PADS_TCON_CFG2_TS1 == ENABLE)  ? BIT2 : 0)
    _RVM2(0x1E8A, TCON_CFG2_TS1, BIT2),
#endif
#endif

#ifdef PADS_TCON_CFG3_TS1
#if (PADS_TCON_CFG3_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG3_TS1 ((PADS_TCON_CFG3_TS1 == ENABLE)  ? BIT3 : 0)
    _RVM2(0x1E8A, TCON_CFG3_TS1, BIT3),
#endif
#endif

#ifdef PADS_TCON_CFG4_TS1
#if (PADS_TCON_CFG4_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG4_TS1 ((PADS_TCON_CFG4_TS1 == ENABLE)  ? BIT4 : 0)
    _RVM2(0x1E8A, TCON_CFG4_TS1, BIT4),
#endif
#endif

#ifdef PADS_TCON_CFG5_TS1
#if (PADS_TCON_CFG5_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG5_TS1 ((PADS_TCON_CFG5_TS1 == ENABLE)  ? BIT5 : 0)
    _RVM2(0x1E8A, TCON_CFG5_TS1, BIT5),
#endif
#endif

#ifdef PADS_TCON_CFG6_TS1
#if (PADS_TCON_CFG6_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG6_TS1 ((PADS_TCON_CFG6_TS1 == ENABLE)  ? BIT6 : 0)
    _RVM2(0x1E8A, TCON_CFG6_TS1, BIT6),
#endif
#endif

#ifdef PADS_TCON_CFG7_TS1
#if (PADS_TCON_CFG7_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG7_TS1 ((PADS_TCON_CFG7_TS1 == ENABLE)  ? BIT7 : 0)
    _RVM2(0x1E8A, TCON_CFG7_TS1, BIT7),
#endif
#endif

#ifdef PADS_TCON_CFG8_TS1
#if (PADS_TCON_CFG8_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG8_TS1 ((PADS_TCON_CFG8_TS1 == ENABLE)  ? BIT0 : 0)
    _RVM2(0x1E8B, TCON_CFG8_TS1, BIT0),
#endif
#endif

#ifdef PADS_TCON_CFG9_TS1
#if (PADS_TCON_CFG9_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG9_TS1 ((PADS_TCON_CFG9_TS1 == ENABLE)  ? BIT1 : 0)
    _RVM2(0x1E8B, TCON_CFG9_TS1, BIT1),
#endif
#endif

#ifdef PADS_TCON_CFG10_TS1
#if (PADS_TCON_CFG10_TS1 != CONFIG_PADMUX_UNKNOWN)
#define TCON_CFG10_TS1 ((PADS_TCON_CFG10_TS1 == ENABLE)  ? BIT2 : 0)
    _RVM2(0x1E8B, TCON_CFG10_TS1, BIT2),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_LDG_RGBW_REVERSE
#if (PADS_LDG_RGBW_REVERSE != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_REVERSE ((PADS_LDG_RGBW_REVERSE == ENABLE) ? BIT4 : 0)
    _RVM1(0x1E3C, LDG_RGBW_REVERSE, BIT4),
#endif
#endif

#ifdef PADS_LDG_RGBW_PWMTIN
#if (PADS_LDG_RGBW_PWMTIN != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_PWMTIN ((PADS_LDG_RGBW_PWMTIN == ENABLE) ? BIT0 : 0)
    _RVM1(0x1E3C, LDG_RGBW_PWMTIN, BIT0),
#endif
#endif

#ifdef PADS_LDG_RGBW_PWMTOUT1
#if (PADS_LDG_RGBW_PWMTOUT1 != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_PWMTOUT1 ((PADS_LDG_RGBW_PWMTOUT1 == ENABLE) ? BIT1 : 0)
    _RVM1(0x1E3C, LDG_RGBW_PWMTOUT1, BIT1),
#endif
#endif

#ifdef PADS_LDG_RGBW_PWMTOUT2
#if (PADS_LDG_RGBW_PWMTOUT2 != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_PWMTOUT2 ((PADS_LDG_RGBW_PWMTOUT2 == ENABLE) ? BIT2 : 0)
    _RVM1(0x1E3C, LDG_RGBW_PWMTOUT2, BIT2),
#endif
#endif

#ifdef PADS_LDG_RGBW_PWMTOUT3
#if (PADS_LDG_RGBW_PWMTOUT3 != CONFIG_PADMUX_UNKNOWN)
#define LDG_RGBW_PWMTOUT3 ((PADS_LDG_RGBW_PWMTOUT3 == ENABLE) ? BIT3 : 0)
    _RVM1(0x1E3C, LDG_RGBW_PWMTOUT3 , BIT3),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_AGC_DBG
#if (PADS_AGC_DBG != CONFIG_PADMUX_UNKNOWN)
#define AGC_DBG ((PADS_AGC_DBG == ENABLE) ? BIT7 : 0)
    _RVM1(0x1E9E, AGC_DBG, BIT7),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_JTAG_MODE
#if (PADS_JTAG_MODE != CONFIG_PADMUX_UNKNOWN)
#define JTAG_MODE_OPEN ((PADS_JTAG_MODE == CONFIG_PADMUX_MODE1) ? BIT4 : \
                        (PADS_JTAG_MODE == CONFIG_PADMUX_MODE2) ? BIT5 : \
                        (PADS_JTAG_MODE == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : 0)
    _RVM1(0x1e05, JTAG_MODE_OPEN, (BIT5|BIT4)),

#define JTAG_MODE_CLOSE ((PADS_JTAG_MODE == CONFIG_PADMUX_MODE1) ? BIT0 : \
                         (PADS_JTAG_MODE == CONFIG_PADMUX_MODE2) ? BIT1 : \
                         (PADS_JTAG_MODE == CONFIG_PADMUX_MODE3) ? (BIT1|BIT0) : 0)
    _RVM1(0x1e27, JTAG_MODE_CLOSE, BITMASK(1:0)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_PCM_CFG
#if (PADS_PCM_CFG != CONFIG_PADMUX_UNKNOWN)
#define PCM_CFG ((PADS_PCM_CFG == ENABLE) ? BIT1 : 0)
    _RVM1(0x1EF6, PCM_CFG, BIT1),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_VSYNC_VIF_OUT_EN
#if (PADS_VSYNC_VIF_OUT_EN != CONFIG_PADMUX_UNKNOWN)
#define VSYNC_VIF_OUT_EN ((PADS_VSYNC_VIF_OUT_EN == ENABLE) ? BIT6 : 0)
    _RVM1(0x1EA3, VSYNC_VIF_OUT_EN, BIT6),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_TS0_CFG
#if (PADS_TS0_CFG != CONFIG_PADMUX_UNKNOWN)
#define TS0_CFG ((PADS_TS0_CFG == CONFIG_PADMUX_MODE1) ? BIT0 :        \
                 (PADS_TS0_CFG == CONFIG_PADMUX_MODE2) ? BIT1 :        \
                 (PADS_TS0_CFG == CONFIG_PADMUX_MODE3) ? (BIT1|BIT0) : \
                 (PADS_TS0_CFG == CONFIG_PADMUX_MODE4) ? BIT2 : 0)
    _RVM1(0x1EAF, TS0_CFG, BITMASK(2:0)),
#endif
#endif

#ifdef PADS_TS1_CFG
#if (PADS_TS1_CFG != CONFIG_PADMUX_UNKNOWN)
#define TS1_CFG ((PADS_TS1_CFG == CONFIG_PADMUX_MODE1) ? BIT3 :        \
                 (PADS_TS1_CFG == CONFIG_PADMUX_MODE2) ? BIT4 :        \
                 (PADS_TS1_CFG == CONFIG_PADMUX_MODE3) ? (BIT4|BIT3) : \
                 (PADS_TS1_CFG == CONFIG_PADMUX_MODE4) ? BIT5 :        \
                 (PADS_TS1_CFG == CONFIG_PADMUX_MODE5) ? (BIT5|BIT3) : 0)
    _RVM1(0x1EAF, TS1_CFG, BITMASK(5:3)),
#endif
#endif

#ifdef PADS_TS_OUT_MODE
#if (PADS_TS_OUT_MODE != CONFIG_PADMUX_UNKNOWN)
#define TS_OUT_MODE ((PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE1) ? BIT4 :        \
                     (PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE2) ? BIT5 :        \
                     (PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : \
                     (PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE4) ? BIT6 :        \
                     (PADS_TS_OUT_MODE == CONFIG_PADMUX_MODE5) ? (BIT6|BIT4) : 0)
    _RVM1(0x1E80, TS_OUT_MODE, BITMASK(6:4)),
#endif
#endif

#ifdef PADS_TSO_EVD_MODE
#if (PADS_TSO_EVD_MODE != CONFIG_PADMUX_UNKNOWN)
#define TSO_EVD_MODE ((PADS_TSO_EVD_MODE == CONFIG_PADMUX_MODE1) ? BIT1 : \
                      (PADS_TSO_EVD_MODE == CONFIG_PADMUX_MODE2) ? BIT2 : 0)
    _RVM1(0x1E21, TSO_EVD_MODE, BITMASK(2:1)),
#endif
#endif

#ifdef PADS_TS2_CFG
#if (PADS_TS2_CFG != CONFIG_PADMUX_UNKNOWN)
#define TS2_CFG ((PADS_TS2_CFG == CONFIG_PADMUX_MODE1) ? BIT4 :        \
                 (PADS_TS2_CFG == CONFIG_PADMUX_MODE2) ? BIT5 :        \
                 (PADS_TS2_CFG == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : \
                 (PADS_TS2_CFG == CONFIG_PADMUX_MODE4) ? BIT6 : 0)
    _RVM1(0x1EB5, TS2_CFG, BITMASK(6:4)),
#endif
#endif

#ifdef PADS_TS3_CFG
#if (PADS_TS3_CFG != CONFIG_PADMUX_UNKNOWN)
#define TS3_CFG ((PADS_TS3_CFG == CONFIG_PADMUX_MODE1) ? BIT4 :        \
                 (PADS_TS3_CFG == CONFIG_PADMUX_MODE2) ? BIT5 :        \
                 (PADS_TS3_CFG == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : \
                 (PADS_TS3_CFG == CONFIG_PADMUX_MODE4) ? BIT6 :        \
                 (PADS_TS3_CFG == CONFIG_PADMUX_MODE5) ? (BIT6|BIT4) : \
                 (PADS_TS3_CFG == CONFIG_PADMUX_MODE6) ? (BIT6|BIT5) : \
                 (PADS_TS3_CFG == CONFIG_PADMUX_MODE7) ? (BIT6|BIT5|BIT4) : \
                 (PADS_TS3_CFG == CONFIG_PADMUX_MODE8) ? BIT7 : \
                 (PADS_TS3_CFG == CONFIG_PADMUX_MODE9) ? (BIT7|BIT4) : 0)
    _RVM1(0x1ECF, TS3_CFG, BITMASK(7:4)),
#endif
#endif

#ifdef PADS_TS4_CFG
#if (PADS_TS4_CFG != CONFIG_PADMUX_UNKNOWN)
#define TS4_CFG ((PADS_TS4_CFG == CONFIG_PADMUX_MODE1) ? BIT1 :        \
                 (PADS_TS4_CFG == CONFIG_PADMUX_MODE3) ? (BIT2|BIT1) : \
                 (PADS_TS4_CFG == CONFIG_PADMUX_MODE4) ? BIT3 : 0)
    _RVM1(0x1E81, TS4_CFG, BITMASK(3:1)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_NAND_MODE
#if (PADS_NAND_MODE != CONFIG_PADMUX_UNKNOWN)
#define NAND_MODE ((PADS_NAND_MODE == CONFIG_PADMUX_MODE1) ? BIT6 : \
                   (PADS_NAND_MODE == CONFIG_PADMUX_MODE2) ? BIT7 : 0)
    _RVM1(0x1EDE, NAND_MODE, BITMASK(7:6)),
#endif
#endif

#ifdef PADS_NAND_CS1_EN
#if (PADS_NAND_CS1_EN != CONFIG_PADMUX_UNKNOWN)
#define NAND_CS1_EN  ((PADS_NAND_CS1_EN == ENABLE) ? BIT5 : 0)
    _RVM1(0x1EDE, NAND_CS1_EN, BIT5),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_PCM_CTRL_CFG
#if (PADS_PCM_CTRL_CFG != CONFIG_PADMUX_UNKNOWN)
#define PCM_CTRL_CFG ((PADS_PCM_CTRL_CFG == ENABLE) ? BIT5 : 0)
    _RVM1(0x1EC8, PCM_CTRL_CFG, BIT5),
#endif
#endif

#ifdef PADS_PCM_ADC_CFG
#if (PADS_PCM_ADC_CFG != CONFIG_PADMUX_UNKNOWN)
#define PCM_ADC_CFG ((PADS_PCM_ADC_CFG == ENABLE) ? BIT4 : 0)
    _RVM1(0x1EC8, PCM_ADC_CFG, BIT4),
#endif
#endif

#ifdef PADS_CI_CTRL_CFG
#if (PADS_CI_CTRL_CFG != CONFIG_PADMUX_UNKNOWN)
#define CI_CTRL_CFG ((PADS_CI_CTRL_CFG == ENABLE) ? BIT1 : 0)
    _RVM1(0x1EC8, CI_CTRL_CFG, BIT1),
#endif
#endif

#ifdef PADS_CI_ADC_CFG
#if (PADS_CI_ADC_CFG != CONFIG_PADMUX_UNKNOWN)
#define CI_ADC_CFG ((PADS_CI_ADC_CFG == ENABLE) ? BIT0 : 0)
    _RVM1(0x1EC8, CI_ADC_CFG, BIT0),
#endif
#endif

#ifdef PADS_PCM2_CTRL_CFG
#if (PADS_PCM2_CTRL_CFG != CONFIG_PADMUX_UNKNOWN)
#define PCM2_CTRL_CFG ((PADS_PCM2_CTRL_CFG == ENABLE) ? BIT3 : 0)
    _RVM1(0x1EC8, PCM2_CTRL_CFG, BIT3),
#endif
#endif

#ifdef PADS_PCM2_CDN_CFG
#if (PADS_PCM2_CDN_CFG != CONFIG_PADMUX_UNKNOWN)
#define PCM2_CDN_CFG ((PADS_PCM2_CDN_CFG == ENABLE) ? BIT0 : 0)
    _RVM1(0x1E9E, PCM2_CDN_CFG, BIT0),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_SPDIF_OUT_CFG
#if (PADS_SPDIF_OUT_CFG != CONFIG_PADMUX_UNKNOWN)
#define SPDIF_OUT_CFG ((PADS_SPDIF_OUT_CFG == ENABLE) ? BIT7 : 0)
    _RVM1(0x1EAE, SPDIF_OUT_CFG, BIT7),
#endif
#endif

#ifdef PADS_SPDIF_IN_CFG
#if (PADS_SPDIF_IN_CFG != CONFIG_PADMUX_UNKNOWN)
#define SPDIF_IN_CFG ((PADS_SPDIF_IN_CFG == ENABLE) ? BIT6 : 0)
    _RVM1(0x1EAE, SPDIF_IN_CFG, BIT6),
#endif
#endif

#ifdef PADS_SPDIF_OUT2_CFG
#if (PADS_SPDIF_OUT2_CFG != CONFIG_PADMUX_UNKNOWN)
#define SPDIF_OUT2_CFG ((PADS_SPDIF_OUT2_CFG == ENABLE) ? BIT0 : 0)
    _RVM1(0x1EB3, SPDIF_OUT2_CFG, BIT0),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_I2S_IN_CFG
#if (PADS_I2S_IN_CFG != CONFIG_PADMUX_UNKNOWN)
#define I2S_IN_CFG ((PADS_I2S_IN_CFG == ENABLE) ? BIT2 : 0)
    _RVM1(0x1EAE, I2S_IN_CFG, BIT2),
#endif
#endif

#ifdef PADS_I2S_MUTE_MODE
#if (PADS_I2S_MUTE_MODE != CONFIG_PADMUX_UNKNOWN)
#define I2S_MUTE_MODE ((PADS_I2S_MUTE_MODE == CONFIG_PADMUX_MODE1) ? BIT6 : \
                       (PADS_I2S_MUTE_MODE == CONFIG_PADMUX_MODE2) ? BIT7 : 0)
    _RVM1(0x1E05, I2S_MUTE_MODE, BITMASK(7:6)),
#endif
#endif

#ifdef PADS_I2S_OUT_IN_TCON
#if (PADS_I2S_OUT_IN_TCON != CONFIG_PADMUX_UNKNOWN)
#define I2S_OUT_IN_TCON ((PADS_I2S_OUT_IN_TCON == ENABLE) ? BIT0 : 0)
    _RVM1(0x1E81, I2S_OUT_IN_TCON, BIT0),
#endif
#endif

#ifdef PADS_I2S_OUT_0_CFG
#if (PADS_I2S_OUT_0_CFG != CONFIG_PADMUX_UNKNOWN)
#define I2S_OUT_0_CFG ((PADS_I2S_OUT_0_CFG == ENABLE) ? BIT4 : 0)
    _RVM1(0x1EAE, I2S_OUT_0_CFG, BIT4),
#endif
#endif

#ifdef PADS_I2S_OUT_1_CFG
#if (PADS_I2S_OUT_1_CFG != CONFIG_PADMUX_UNKNOWN)
#define I2S_OUT_1_CFG ((PADS_I2S_OUT_1_CFG == ENABLE) ? BIT5 : 0)
    _RVM1(0x1EAE, I2S_OUT_1_CFG, BIT5),
#endif
#endif

#ifdef PADS_I2S_OUT_2_CFG
#if (PADS_I2S_OUT_2_CFG != CONFIG_PADMUX_UNKNOWN)
#define I2S_OUT_2_CFG ((PADS_I2S_OUT_2_CFG == ENABLE) ? BIT3 : 0)
    _RVM1(0x1EB3, I2S_OUT_2_CFG, BIT3),
#endif
#endif

#ifdef PADS_I2S_OUT_3_CFG
#if (PADS_I2S_OUT_3_CFG != CONFIG_PADMUX_UNKNOWN)
#define I2S_OUT_3_CFG ((PADS_I2S_OUT_3_CFG == ENABLE) ? BIT4 : 0)
    _RVM1(0x1EB3, I2S_OUT_3_CFG, BIT4),
#endif
#endif

#ifdef PADS_I2S_OUT_4_CFG
#if (PADS_I2S_OUT_4_CFG != CONFIG_PADMUX_UNKNOWN)
#define I2S_OUT_4_CFG ((PADS_I2S_OUT_4_CFG == ENABLE) ? BIT5 : 0)
    _RVM1(0x1EB3, I2S_OUT_4_CFG, BIT5),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_ET_MODE
#if (PADS_ET_MODE != CONFIG_PADMUX_UNKNOWN)
#define ET_MODE ((PADS_ET_MODE== ENABLE) ? BIT0 : 0)
    _RVM1(0x1EDF, ET_MODE, BIT0),
#endif
#endif

#ifdef PADS_LED_MODE
#if (PADS_LED_MODE != CONFIG_PADMUX_UNKNOWN)
#define LED_MODE ((PADS_LED_MODE == ENABLE) ? BIT4 : 0)
    _RVM1(0x1EB4, LED_MODE, BIT4),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_VSYNC_LIKE_CFG
#if (PADS_VSYNC_LIKE_CFG != CONFIG_PADMUX_UNKNOWN)
#define VSYNC_LIKE_CFG ((PADS_VSYNC_LIKE_CFG == CONFIG_PADMUX_MODE1) ? BIT6 :     \
                        (PADS_VSYNC_LIKE_CFG == CONFIG_PADMUX_MODE2) ? BIT7 :     \
                        (PADS_VSYNC_LIKE_CFG == CONFIG_PADMUX_MODE3) ? (BIT7|BIT6) : 0)
    _RVM1(0x1EB4, VSYNC_LIKE_CFG, BITMASK(7:6)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_LD_SPI1_CFG
#if (PADS_LD_SPI1_CFG != CONFIG_PADMUX_UNKNOWN)
#define LD_SPI1_CFG ((PADS_LD_SPI1_CFG == ENABLE) ? BIT0 : 0)
    _RVM1(0x1EB4, LD_SPI1_CFG, BIT0),
#endif
#endif

#ifdef PADS_LD_SPI2_CFG
#if (PADS_LD_SPI2_CFG != CONFIG_PADMUX_UNKNOWN)
#define LD_SPI2_CFG ((PADS_LD_SPI2_CFG == ENABLE) ? BIT1 : 0)
    _RVM1(0x1EB4, LD_SPI2_CFG, BIT1),
#endif
#endif

#ifdef PADS_LD_SPI3_CFG
#if (PADS_LD_SPI3_CFG != CONFIG_PADMUX_UNKNOWN)
#define LD_SPI3_CFG ((PADS_LD_SPI3_CFG == ENABLE) ? BIT2 : 0)
    _RVM1(0x1EB4, LD_SPI3_CFG, BIT2),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_DIM_CFG
#if (PADS_DIM_CFG != CONFIG_PADMUX_UNKNOWN)
#define DIM_CFG ((PADS_DIM_CFG == ENABLE) ? BIT0 : 0)
    _RVM1(0x1EF6, DIM_CFG, BIT0),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_MCU_JTAG_MODE
#if (PADS_MCU_JTAG_MODE != CONFIG_PADMUX_UNKNOWN)
#define MCU_JTAG_MODE ((PADS_MCU_JTAG_MODE == CONFIG_PADMUX_MODE1) ? BIT2 :     \
                       (PADS_MCU_JTAG_MODE == CONFIG_PADMUX_MODE2) ? BIT3 :     \
                       (PADS_MCU_JTAG_MODE == CONFIG_PADMUX_MODE3) ? (BIT3|BIT2) : 0)
    _RVM1(0x1EDE, MCU_JTAG_MODE, BITMASK(3:2)),
#endif
#endif

#ifdef PADS_DSP_JTAG_MODE
#if (PADS_DSP_JTAG_MODE != CONFIG_PADMUX_UNKNOWN)
#define DSP_JTAG_MODE ((PADS_DSP_JTAG_MODE == CONFIG_PADMUX_MODE1) ? BIT0 :     \
                       (PADS_DSP_JTAG_MODE == CONFIG_PADMUX_MODE2) ? BIT1 :     \
                       (PADS_DSP_JTAG_MODE == CONFIG_PADMUX_MODE3) ? (BIT1|BIT0) : 0)
    _RVM1(0x1E07, DSP_JTAG_MODE, BITMASK(2:0)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_UART2_MODE
#if (PADS_UART2_MODE != CONFIG_PADMUX_UNKNOWN)
#define UART2_MODE  ((PADS_UART2_MODE == CONFIG_PADMUX_MODE1) ? BIT0 :     \
                     (PADS_UART2_MODE == CONFIG_PADMUX_MODE2) ? BIT1 :     \
                     (PADS_UART2_MODE == CONFIG_PADMUX_MODE3) ? (BIT1|BIT0) : 0)
    _RVM1(0x1E05, UART2_MODE , BITMASK(2:0)),
#endif
#endif

#ifdef PADS_UART3_MODE
#if (PADS_UART3_MODE != CONFIG_PADMUX_UNKNOWN)
#define UART3_MODE ((PADS_UART3_MODE == CONFIG_PADMUX_MODE1) ? BIT2 :     \
                    (PADS_UART3_MODE == CONFIG_PADMUX_MODE2) ? BIT3 :     \
                    (PADS_UART3_MODE == CONFIG_PADMUX_MODE3) ? (BIT3|BIT2) : 0)
    _RVM1(0x1E05, UART3_MODE, BITMASK(3:2)),
#endif
#endif

#ifdef PADS_UART4_MODE
#if (PADS_UART4_MODE != CONFIG_PADMUX_UNKNOWN)
#define UART4_MODE ((PADS_UART4_MODE == CONFIG_PADMUX_MODE1) ? BIT6 :     \
                    (PADS_UART4_MODE == CONFIG_PADMUX_MODE2) ? BIT7 :     \
                    (PADS_UART4_MODE == CONFIG_PADMUX_MODE3) ? (BIT7|BIT6) : 0)
    _RVM1(0x1E04, UART4_MODE, BITMASK(7:6)),
#endif
#endif

#ifdef PADS_UART5_MODE
#if (PADS_UART5_MODE != CONFIG_PADMUX_UNKNOWN)
#define UART5_MODE ((PADS_UART5_MODE == CONFIG_PADMUX_MODE1) ? BIT2 :     \
                    (PADS_UART5_MODE == CONFIG_PADMUX_MODE2) ? BIT3 :     \
                    (PADS_UART5_MODE == CONFIG_PADMUX_MODE3) ? (BIT3|BIT2) : 0)
    _RVM1(0x1E08, UART5_MODE, BITMASK(3:2)),
#endif
#endif

#ifdef PADS_UART6_MODE
#if (PADS_UART6_MODE != CONFIG_PADMUX_UNKNOWN)
#define UART6_MODE ((PADS_UART6_MODE == ENABLE) ? BIT6 : 0)
    _RVM1(0x1E08, UART6_MODE, BITMASK(7:6)),
#endif
#endif

#ifdef PADS_FAST_UART_MODE
#if (PADS_FAST_UART_MODE != CONFIG_PADMUX_UNKNOWN)
#define FAST_UART_MODE ((PADS_FAST_UART_MODE == CONFIG_PADMUX_MODE1) ? BIT4 :     \
                        (PADS_FAST_UART_MODE == CONFIG_PADMUX_MODE2) ? BIT5 :     \
                        (PADS_FAST_UART_MODE == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : 0)
    _RVM1(0x1E04, FAST_UART_MODE, BITMASK(5:4)),
#endif
#endif

#ifdef PADS_UART0_INV_MODE
#if (PADS_UART0_INV_MODE != CONFIG_PADMUX_UNKNOWN)
#define _CONFIG_ENABLE_UART0_INV_MODE   ((PADS_UART0_INV_MODE == ENABLE) ? BIT0 : 0)
    _RVM1(0x1eab, _CONFIG_ENABLE_UART0_INV_MODE, BIT0),
#endif
#endif

#ifdef PADS_UART1_INV_MODE
#if (PADS_UART1_INV_MODE != CONFIG_PADMUX_UNKNOWN)
#define _CONFIG_ENABLE_UART1_INV_MODE   ((PADS_UART1_INV_MODE == ENABLE) ? BIT1 : 0)
    _RVM1(0x1eab, _CONFIG_ENABLE_UART1_INV_MODE, BIT1),
#endif
#endif

#ifdef PADS_UART2_INV_MODE
#if (PADS_UART2_INV_MODE != CONFIG_PADMUX_UNKNOWN)
#define _CONFIG_ENABLE_UART2_INV_MODE   ((PADS_UART2_INV_MODE == ENABLE) ? BIT2 : 0)
    _RVM1(0x1eab, _CONFIG_ENABLE_UART2_INV_MODE, BIT2),
#endif
#endif

#ifdef PADS_UART3_INV_MODE
#if (PADS_UART3_INV_MODE != CONFIG_PADMUX_UNKNOWN)
#define _CONFIG_ENABLE_UART3_INV_MODE   ((PADS_UART3_INV_MODE == ENABLE) ? BIT3 : 0)
    _RVM1(0x1eab, _CONFIG_ENABLE_UART3_INV_MODE, BIT3),
#endif
#endif

#ifdef PADS_UART4_INV_MODE
#if (PADS_UART4_INV_MODE != CONFIG_PADMUX_UNKNOWN)
#define _CONFIG_ENABLE_UART4_INV_MODE   ((PADS_UART4_INV_MODE == ENABLE) ? BIT4 : 0)
    _RVM1(0x1eab, _CONFIG_ENABLE_UART4_INV_MODE, BIT4),
#endif
#endif

#ifdef PADS_UART5_INV_MODE
#if (PADS_UART5_INV_MODE != CONFIG_PADMUX_UNKNOWN)
#define _CONFIG_ENABLE_UART5_INV_MODE   ((PADS_UART5_INV_MODE == ENABLE) ? BIT5 : 0)
    _RVM1(0x1eab, _CONFIG_ENABLE_UART5_INV_MODE, BIT5),
#endif
#endif

#ifdef PADS_UART6_INV_MODE
#if (PADS_UART6_INV_MODE != CONFIG_PADMUX_UNKNOWN)
#define _CONFIG_ENABLE_UART6_INV_MODE   ((PADS_UART6_INV_MODE == ENABLE) ? BIT6 : 0)
    _RVM1(0x1eab, _CONFIG_ENABLE_UART6_INV_MODE, BIT6),
#endif
#endif

#ifdef PADS_FAST_UART_INV_MODE
#if (PADS_FAST_UART_INV_MODE != CONFIG_PADMUX_UNKNOWN)
#define _CONFIG_ENABLE_FAST_UART_INV_MODE   ((PADS_FAST_UART_INV_MODE == ENABLE) ? BIT7 : 0)
    _RVM1(0x1eab, _CONFIG_ENABLE_FAST_UART_INV_MODE, BIT7),
#endif
#endif

    _RVM1(0x1ea6, ((UART1_SRC_SEL << 4) | UART0_SRC_SEL), BITMASK(7:0)),
    _RVM1(0x1ea7, ((UART3_SRC_SEL << 4) | UART2_SRC_SEL), BITMASK(7:0)),
    _RVM1(0x1ea8, ((UART5_SRC_SEL << 4) | UART4_SRC_SEL), BITMASK(7:0)),

#if ((UART0_SRC_SEL == UART1_SRC_SEL) || \
     (UART0_SRC_SEL == UART2_SRC_SEL) || \
     (UART0_SRC_SEL == UART3_SRC_SEL) || \
     (UART0_SRC_SEL == UART4_SRC_SEL) || \
     (UART0_SRC_SEL == UART5_SRC_SEL) || \
     (UART1_SRC_SEL == UART0_SRC_SEL) || \
     (UART1_SRC_SEL == UART2_SRC_SEL) || \
     (UART1_SRC_SEL == UART3_SRC_SEL) || \
     (UART1_SRC_SEL == UART4_SRC_SEL) || \
     (UART1_SRC_SEL == UART5_SRC_SEL) || \
     (UART2_SRC_SEL == UART0_SRC_SEL) || \
     (UART2_SRC_SEL == UART1_SRC_SEL) || \
     (UART2_SRC_SEL == UART3_SRC_SEL) || \
     (UART2_SRC_SEL == UART4_SRC_SEL) || \
     (UART2_SRC_SEL == UART5_SRC_SEL) || \
     (UART3_SRC_SEL == UART0_SRC_SEL) || \
     (UART3_SRC_SEL == UART1_SRC_SEL) || \
     (UART3_SRC_SEL == UART2_SRC_SEL) || \
     (UART3_SRC_SEL == UART4_SRC_SEL) || \
     (UART3_SRC_SEL == UART5_SRC_SEL) || \
     (UART4_SRC_SEL == UART0_SRC_SEL) || \
     (UART4_SRC_SEL == UART1_SRC_SEL) || \
     (UART4_SRC_SEL == UART2_SRC_SEL) || \
     (UART4_SRC_SEL == UART3_SRC_SEL) || \
     (UART4_SRC_SEL == UART5_SRC_SEL))
    #error "[ERROR] UART Source Selection Conflict"
#endif

    _MEMMAP_PM_,

    _RVM1(0x0e13, 0, BIT4),         //Select UART Source to reg_uart_sel0
    _RVM1(0x0e13, BIT3, BIT3),      //Enable UART Rx

    _MEMMAP_nonPM_,

#ifdef PADS_OD2_UART
#if (PADS_OD2_UART != CONFIG_PADMUX_UNKNOWN)
#define OD2_UART ((PADS_OD2_UART == CONFIG_PADMUX_MODE1) ? BIT0 :     \
                  (PADS_OD2_UART == CONFIG_PADMUX_MODE2) ? BIT1 :     \
                  (PADS_OD2_UART == CONFIG_PADMUX_MODE3) ? (BIT1|BIT0) : 0)
    _RVM1(0x1EA9, OD2_UART, BITMASK(1:0)),
#endif
#endif

#ifdef PADS_OD3_UART
#if (PADS_OD3_UART != CONFIG_PADMUX_UNKNOWN)
#define OD3_UART ((PADS_OD3_UART == CONFIG_PADMUX_MODE1) ? BIT2 :     \
                  (PADS_OD3_UART == CONFIG_PADMUX_MODE2) ? BIT3 :     \
                  (PADS_OD3_UART == CONFIG_PADMUX_MODE3) ? (BIT3|BIT2) : 0)
    _RVM1(0x1EA9, OD3_UART, BITMASK(3:2)),
#endif
#endif

#ifdef PADS_OD4_UART
#if (PADS_OD4_UART != CONFIG_PADMUX_UNKNOWN)
#define OD4_UART ((PADS_OD4_UART == CONFIG_PADMUX_MODE1) ? BIT4 :     \
                  (PADS_OD4_UART == CONFIG_PADMUX_MODE2) ? BIT5 :     \
                  (PADS_OD4_UART == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : 0)
    _RVM1(0x1EA9, OD4_UART, BITMASK(5:4)),
#endif
#endif

#ifdef PADS_OD5_UART
#if (PADS_OD5_UART != CONFIG_PADMUX_UNKNOWN)
#define OD5_UART ((PADS_OD5_UART == CONFIG_PADMUX_MODE1) ? BIT4 :     \
                  (PADS_OD5_UART == CONFIG_PADMUX_MODE2) ? BIT5 :     \
                  (PADS_OD5_UART == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : 0)
    _RVM1(0x1EAA, OD5_UART, BITMASK(5:4)),
#endif
#endif

#ifdef PADS_OD6_UART
#if (PADS_OD6_UART != CONFIG_PADMUX_UNKNOWN)
#define OD6_UART ((PADS_OD6_UART == ENABLE) ? BIT6 : 0)
    _RVM1(0x1EAA, OD6_UART, BITMASK(7:6)),
#endif
#endif

#ifdef PADS_OD_FAST_UART
#if (PADS_OD_FAST_UART != CONFIG_PADMUX_UNKNOWN)
#define OD_FAST_UART ((PADS_OD_FAST_UART == CONFIG_PADMUX_MODE1) ? BIT6 :     \
                      (PADS_OD_FAST_UART == CONFIG_PADMUX_MODE2) ? BIT7 :     \
                      (PADS_OD_FAST_UART == CONFIG_PADMUX_MODE3) ? (BIT7|BIT6) : 0)
    _RVM1(0x1EA9, OD_FAST_UART, BITMASK(7:6)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_PWM0_MODE
#if (PADS_PWM0_MODE != CONFIG_PADMUX_UNKNOWN)
#define PWM0_MODE ((PADS_PWM0_MODE == ENABLE) ? BIT2 : 0)
 _RVM1(0x1EC8, PWM0_MODE, BIT2),
#endif
#endif

#ifdef PADS_PWM1_MODE
#if (PADS_PWM1_MODE != CONFIG_PADMUX_UNKNOWN)
#define PWM1_MODE ((PADS_PWM1_MODE == ENABLE) ? BIT6 : 0)
    _RVM1(0x1EC8, PWM1_MODE, BIT6),
#endif
#endif

#ifdef PADS_PWM2_MODE
#if (PADS_PWM2_MODE != CONFIG_PADMUX_UNKNOWN)
#define PWM2_MODE ((PADS_PWM2_MODE == ENABLE) ? BIT7 : 0)
    _RVM1(0x1EC8, PWM2_MODE, BIT7),
#endif
#endif

#ifdef PADS_PWM3_MODE
#if (PADS_PWM3_MODE != CONFIG_PADMUX_UNKNOWN)
#define PWM3_MODE ((PADS_PWM3_MODE == ENABLE) ? BIT4 : 0)
    _RVM1(0x1EC9, PWM3_MODE, BIT4),
#endif
#endif

#ifdef PADS_PWM4_MODE
#if (PADS_PWM4_MODE != CONFIG_PADMUX_UNKNOWN)
#define PWM4_MODE ((PADS_PWM4_MODE == ENABLE) ? BIT5 : 0)
    _RVM1(0x1EC9, PWM4_MODE, BIT5),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_VX1_GPI_MODE
#if (PADS_VX1_GPI_MODE != CONFIG_PADMUX_UNKNOWN)
#define VX1_GPI_MODE ((PADS_VX1_GPI_MODE == ENABLE) ? BIT0 : 0)
    _RVM1(0x1E4A, VX1_GPI_MODE, BIT0),
#endif
#endif

#ifdef PADS_VX1_GPI_OSD_MODE
#if (PADS_VX1_GPI_OSD_MODE != CONFIG_PADMUX_UNKNOWN)
#define VX1_GPI_OSD_MODE ((PADS_VX1_GPI_OSD_MODE == ENABLE) ? BIT1 : 0)
    _RVM1(0x1E4A, VX1_GPI_OSD_MODE, BIT1),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_IRE_MODE
#if (PADS_IRE_MODE != CONFIG_PADMUX_UNKNOWN)
#define IRE_MODE ((PADS_IRE_MODE == CONFIG_PADMUX_MODE1) ? BIT4 :     \
                  (PADS_IRE_MODE == CONFIG_PADMUX_MODE2) ? BIT5 :     \
                  (PADS_IRE_MODE == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : 0)
    _RVM1(0x1EDF, IRE_MODE, BITMASK(5:4)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_TS_ERR_OUT
#if (PADS_TS_ERR_OUT != CONFIG_PADMUX_UNKNOWN)
#define TS_ERR_OUT ((PADS_TS_ERR_OUT == CONFIG_PADMUX_MODE1) ? BIT0 :     \
                    (PADS_TS_ERR_OUT == CONFIG_PADMUX_MODE2) ? BIT1 :     \
                    (PADS_TS_ERR_OUT == CONFIG_PADMUX_MODE3) ? (BIT1|BIT0) : 0)
    _RVM1(0x1EC9, TS_ERR_OUT, BITMASK(1:0)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_DDCR_MODE
#if (PADS_DDCR_MODE != CONFIG_PADMUX_UNKNOWN)
#define DDCR_MODE ((PADS_DDCR_MODE == CONFIG_PADMUX_MODE1) ? BIT0 :     \
                   (PADS_DDCR_MODE == CONFIG_PADMUX_MODE2) ? BIT1 : 0)
    _RVM1(0x1EAE, DDCR_MODE, BITMASK(1:0)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_MIIC_MODE0
#if (PADS_MIIC_MODE0 != CONFIG_PADMUX_UNKNOWN)
#define MIIC_MODE0 ((PADS_MIIC_MODE0 == ENABLE) ? BIT0 : 0)
    _RVM1(0x1EDC, MIIC_MODE0, BIT0),
#endif
#endif

#ifdef PADS_MIIC_MODE1
#if (PADS_MIIC_MODE1 != CONFIG_PADMUX_UNKNOWN)
#define MIIC_MODE1 ((PADS_MIIC_MODE1 == ENABLE) ? BIT1 : 0)
    _RVM1(0x1EDC, MIIC_MODE1, BIT1),
#endif
#endif

#ifdef PADS_MIIC_MODE2
#if (PADS_MIIC_MODE2 != CONFIG_PADMUX_UNKNOWN)
#define MIIC_MODE2 ((PADS_MIIC_MODE2 == ENABLE) ? BIT2 : 0)
    _RVM1(0x1EDC, MIIC_MODE2, BIT2),
#endif
#endif

#ifdef PADS_MIIC_MODE3
#if (PADS_MIIC_MODE3 != CONFIG_PADMUX_UNKNOWN)
#define MIIC_MODE3 ((PADS_MIIC_MODE3 == CONFIG_PADMUX_MODE1) ? BIT1 : \
                    (PADS_MIIC_MODE3 == CONFIG_PADMUX_MODE2) ? BIT2 :0)
    _RVM1(0x1EDF, MIIC_MODE3, BITMASK(2:1)),
#endif
#endif

#ifdef PADS_MIIC_MODE4
#if (PADS_MIIC_MODE4 != CONFIG_PADMUX_UNKNOWN)
#define MIIC_MODE4 ((PADS_MIIC_MODE4 == ENABLE) ? BIT0 : 0)
    _RVM1(0x1EDE, MIIC_MODE4, BIT0),
#endif
#endif

#ifdef PADS_MIIC_MODE5
#if (PADS_MIIC_MODE5 != CONFIG_PADMUX_UNKNOWN)
#define MIIC_MODE5 ((PADS_MIIC_MODE5 == ENABLE) ? BIT1 : 0)
    _RVM1(0x1EDE, MIIC_MODE5, BIT1),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_TCON0_CFG
#if (PADS_TCON0_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON0_CFG ((PADS_TCON0_CFG == ENABLE)  ? BIT0 : 0)
    _RVM1(0x1EA0, TCON0_CFG, BIT0),
#endif
#endif

#ifdef PADS_TCON1_CFG
#if (PADS_TCON1_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON1_CFG ((PADS_TCON1_CFG == ENABLE)  ? BIT1 : 0)
    _RVM1(0x1EA0, TCON1_CFG, BIT1),
#endif
#endif

#ifdef PADS_TCON2_CFG
#if (PADS_TCON2_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON2_CFG ((PADS_TCON2_CFG == ENABLE)  ? BIT2 : 0)
    _RVM1(0x1EA0, TCON2_CFG, BIT2),
#endif
#endif

#ifdef PADS_TCON3_CFG
#if (PADS_TCON3_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON3_CFG ((PADS_TCON3_CFG == ENABLE)  ? BIT3 : 0)
    _RVM1(0x1EA0, TCON3_CFG, BIT3),
#endif
#endif

#ifdef PADS_TCON4_CFG
#if (PADS_TCON4_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON4_CFG ((PADS_TCON4_CFG == ENABLE)  ? BIT4 : 0)
    _RVM1(0x1EA0, TCON4_CFG, BIT4),
#endif
#endif

#ifdef PADS_TCON5_CFG
#if (PADS_TCON5_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON5_CFG ((PADS_TCON5_CFG == ENABLE)  ? BIT5 : 0)
    _RVM1(0x1EA0, TCON5_CFG, BIT5),
#endif
#endif

#ifdef PADS_TCON6_CFG
#if (PADS_TCON6_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON6_CFG ((PADS_TCON6_CFG == ENABLE)  ? BIT6 : 0)
    _RVM1(0x1EA0, TCON6_CFG, BIT6),
#endif
#endif

#ifdef PADS_TCON7_CFG
#if (PADS_TCON7_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON7_CFG ((PADS_TCON7_CFG == ENABLE)  ? BIT7 : 0)
    _RVM1(0x1EA0, TCON7_CFG, BIT7),
#endif
#endif

#ifdef PADS_TCON8_CFG
#if (PADS_TCON8_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON8_CFG ((PADS_TCON8_CFG == ENABLE)  ? BIT0 : 0)
    _RVM1(0x1EA1, TCON8_CFG, BIT0),
#endif
#endif

#ifdef PADS_TCON9_CFG
#if (PADS_TCON9_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON9_CFG ((PADS_TCON9_CFG == ENABLE)  ? BIT1 : 0)
    _RVM1(0x1EA1, TCON9_CFG, BIT1),
#endif
#endif

#ifdef PADS_TCON10_CFG
#if (PADS_TCON10_CFG!= CONFIG_PADMUX_UNKNOWN)
#define TCON10_CFG ((PADS_TCON10_CFG == ENABLE)  ? BIT2 : 0)
    _RVM1(0x1EA1, TCON10_CFG, BIT2),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_EXT0_INT
#if (PADS_EXT0_INT != CONFIG_PADMUX_UNKNOWN)
#define _EXT0_INT ((PADS_EXT0_INT == ENABLE) ? BIT0 : 0)
    _RVM1(0x1EA5, _EXT0_INT, BIT0),
#endif
#endif

#ifdef PADS_EXT1_INT
#if (PADS_EXT1_INT != CONFIG_PADMUX_UNKNOWN)
#define _EXT1_INT ((PADS_EXT1_INT == ENABLE) ? BIT1 : 0)
    _RVM1(0x1EA5, _EXT1_INT, BIT1),
#endif
#endif

#ifdef PADS_EXT2_INT
#if (PADS_EXT2_INT != CONFIG_PADMUX_UNKNOWN)
#define _EXT2_INT ((PADS_EXT2_INT == ENABLE) ? BIT2 : 0)
    _RVM1(0x1EA5, _EXT2_INT, BIT2),
#endif
#endif

#ifdef PADS_EXT3_INT
#if (PADS_EXT3_INT != CONFIG_PADMUX_UNKNOWN)
#define _EXT3_INT ((PADS_EXT3_INT == ENABLE) ? BIT3 : 0)
    _RVM1(0x1EA5, _EXT3_INT, BIT3),
#endif
#endif

#ifdef PADS_EXT4_INT
#if (PADS_EXT4_INT != CONFIG_PADMUX_UNKNOWN)
#define _EXT4_INT ((PADS_EXT4_INT == ENABLE) ? BIT4 : 0)
    _RVM1(0x1EA5, _EXT4_INT, BIT4),
#endif
#endif

#ifdef PADS_EXT5_INT
#if (PADS_EXT5_INT != CONFIG_PADMUX_UNKNOWN)
#define _EXT5_INT ((PADS_EXT5_INT == ENABLE) ? BIT5 : 0)
    _RVM1(0x1EA5, _EXT5_INT, BIT5),
#endif
#endif

#ifdef PADS_EXT6_INT
#if (PADS_EXT6_INT != CONFIG_PADMUX_UNKNOWN)
#define _EXT6_INT ((PADS_EXT6_INT == ENABLE) ? BIT6 : 0)
    _RVM1(0x1EA5, _EXT6_INT, BIT6),
#endif
#endif

#ifdef PADS_EXT7_INT
#if (PADS_EXT7_INT != CONFIG_PADMUX_UNKNOWN)
#define _EXT7_INT ((PADS_EXT7_INT == ENABLE) ? BIT7 : 0)
    _RVM1(0x1EA5, _EXT7_INT, BIT7),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_PD_TRACE_CTRL
#if (PADS_PD_TRACE_CTRL != CONFIG_PADMUX_UNKNOWN)
#define PD_TRACE_CTRL ((PADS_PD_TRACE_CTRL == CONFIG_PADMUX_MODE1) ? BIT0 : \
                       (PADS_PD_TRACE_CTRL == CONFIG_PADMUX_MODE1) ? BIT1 : 0)
    _RVM1(0x1E9F, PD_TRACE_CTRL, BITMASK(1:0)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_P0_ENABLE
#if (PADS_P0_ENABLE != CONFIG_PADMUX_UNKNOWN)
#define P0_ENABLE ((PADS_P0_ENABLE == ENABLE) ? BIT0 : 0)
    _RVM1(0x1EA4, P0_ENABLE, BIT0),
#endif
#endif

#ifdef PADS_P1_ENABLE
#if (PADS_P1_ENABLE != CONFIG_PADMUX_UNKNOWN)
#define P1_ENABLE ((PADS_P1_ENABLE == ENABLE) ? BIT1 : 0)
    _RVM1(0x1EA4, P1_ENABLE, BIT1),
#endif
#endif

#ifdef PADS_P2_ENABLE
#if (PADS_P2_ENABLE != CONFIG_PADMUX_UNKNOWN)
#define P2_ENABLE ((PADS_P2_ENABLE == ENABLE) ? BIT2 : 0)
    _RVM1(0x1EA4, P2_ENABLE, BIT2),
#endif
#endif

#ifdef PADS_P3_ENABLE
#if (PADS_P3_ENABLE != CONFIG_PADMUX_UNKNOWN)
#define P3_ENABLE ((PADS_P3_ENABLE == ENABLE) ? BIT3 : 0)
    _RVM1(0x1EA4, P3_ENABLE, BIT3),
#endif
#endif

#ifdef PADS_P4_ENABLE
#if (PADS_P4_ENABLE != CONFIG_PADMUX_UNKNOWN)
#define P4_ENABLE ((PADS_P4_ENABLE == ENABLE) ? BIT4 : 0)
    _RVM1(0x1EA4, P4_ENABLE, BIT4),
#endif
#endif

#ifdef PADS_P5_ENABLE
#if (PADS_P5_ENABLE != CONFIG_PADMUX_UNKNOWN)
#define P5_ENABLE ((PADS_P5_ENABLE == ENABLE) ? BIT5 : 0)
    _RVM1(0x1EA4, P5_ENABLE, BIT5),
#endif
#endif

#ifdef PADS_P6_ENABLE
#if (PADS_P6_ENABLE != CONFIG_PADMUX_UNKNOWN)
#define P6_ENABLE ((PADS_P6_ENABLE == ENABLE) ? BIT6 : 0)
    _RVM1(0x1EA4, P6_ENABLE, BIT6),
#endif
#endif

#ifdef PADS_P7_ENABLE
#if (PADS_P7_ENABLE != CONFIG_PADMUX_UNKNOWN)
#define P7_ENABLE ((PADS_P7_ENABLE == ENABLE) ? BIT7 : 0)
    _RVM1(0x1EA4, P7_ENABLE, BIT7),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_SM_CFG
#if (PADS_SM_CFG != CONFIG_PADMUX_UNKNOWN)
#define SM_CFG ((PADS_SM_CFG == CONFIG_PADMUX_MODE1) ? BIT4 :  \
                (PADS_SM_CFG == CONFIG_PADMUX_MODE2) ? BIT5 : 0)
    _RVM1(0x1EDC, SM_CFG, BITMASK(5:4)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_SD_CFG
#if (PADS_SD_CFG != CONFIG_PADMUX_UNKNOWN)
#define SD_CFG ((PADS_SD_CFG == CONFIG_PADMUX_MODE1) ? BIT0 :  \
                (PADS_SD_CFG == CONFIG_PADMUX_MODE2) ? BIT1 :  \
                (PADS_SD_CFG == CONFIG_PADMUX_MODE3) ? (BIT1|BIT0) : 0)
    _RVM1(0x1EB5, SD_CFG, BITMASK(1:0)),
#endif
#endif

#ifdef PADS_SDIO_CFG
#if (PADS_SDIO_CFG != CONFIG_PADMUX_UNKNOWN)
#define SDIO_CFG ((PADS_SDIO_CFG == CONFIG_PADMUX_MODE1) ? BIT4 :  \
                  (PADS_SDIO_CFG == CONFIG_PADMUX_MODE2) ? BIT5 :  \
                  (PADS_SDIO_CFG == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : 0)
    _RVM1(0x1EF6, SDIO_CFG, BITMASK(5:4)),
#endif
#endif

#ifdef PADS_SDIO_30_CFG
#if (PADS_SDIO_30_CFG != CONFIG_PADMUX_UNKNOWN)
#define SDIO_30_CFG ((PADS_SDIO_30_CFG == ENABLE) ? BIT2 : 0)
    _RVM1(0x1E80, SDIO_30_CFG, BIT2),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_EMMC_RSTN_EN
#if (PADS_EMMC_RSTN_EN != CONFIG_PADMUX_UNKNOWN)
#define EMMC_RSTN_EN ((PADS_EMMC_RSTN_EN == ENABLE) ? BIT2 : 0)
    _RVM1(0x1E9E, EMMC_RSTN_EN, BIT2),
#endif
#endif

#ifdef PADS_EMMC_RSTN_EN_PM
#if (PADS_EMMC_RSTN_EN_PM != CONFIG_PADMUX_UNKNOWN)

    _MEMMAP_PM_,

#define EMMC_RSTN_EN_PM ((PADS_EMMC_RSTN_EN_PM == ENABLE) ? BIT4 : 0)
    _RVM1(0x0E62, EMMC_RSTN_EN_PM, BIT4),

    _MEMMAP_nonPM_,

#endif
#endif

#ifdef PADS_EMMC_MODE
#if (PADS_EMMC_MODE != CONFIG_PADMUX_UNKNOWN)
#define EMMC_MODE ((PADS_EMMC_MODE == ENABLE) ? BIT6 : 0)
    _RVM1(0x1EDC, EMMC_MODE, BITMASK(7:6)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_3D_FLAG_CFG
#if (PADS_3D_FLAG_CFG != CONFIG_PADMUX_UNKNOWN)
#define _3D_FLAG_CFG ((PADS_3D_FLAG_CFG == CONFIG_PADMUX_MODE1) ? BIT6 :  \
                      (PADS_3D_FLAG_CFG == CONFIG_PADMUX_MODE2) ? BIT7 :  \
                      (PADS_3D_FLAG_CFG == CONFIG_PADMUX_MODE3) ? (BIT7|BIT6) : 0)
    _RVM1(0x1EB3, _3D_FLAG_CFG, BITMASK(7:6)),
#endif
#endif

#ifdef PADS_OSD_3D_FLAG_CFG
#if (PADS_OSD_3D_FLAG_CFG != CONFIG_PADMUX_UNKNOWN)
#define OSD_3D_FLAG_CFG ((PADS_OSD_3D_FLAG_CFG == CONFIG_PADMUX_MODE1) ? BIT6 :  \
                         (PADS_OSD_3D_FLAG_CFG == CONFIG_PADMUX_MODE2) ? BIT7 :  \
                         (PADS_OSD_3D_FLAG_CFG == CONFIG_PADMUX_MODE3) ? (BIT7|BIT6) : 0)
    _RVM1(0x1EF6, OSD_3D_FLAG_CFG, BITMASK(7:6)),
#endif
#endif

//---------------------------------------------------------------------
#ifdef PADS_USB30V_CTRL0_CFG
#if (PADS_USB30V_CTRL0_CFG != CONFIG_PADMUX_UNKNOWN)
#define USB30V_CTRL0_CFG ((PADS_USB30V_CTRL0_CFG == CONFIG_PADMUX_MODE1) ? BIT4 :  \
                          (PADS_USB30V_CTRL0_CFG == CONFIG_PADMUX_MODE2) ? BIT5 :  \
                          (PADS_USB30V_CTRL0_CFG == CONFIG_PADMUX_MODE3) ? (BIT5|BIT4) : 0)
    _RVM1(0x1E08, USB30V_CTRL0_CFG, BITMASK(5:4)),
#endif
#endif

#ifdef PADS_USB30V_CTRL1_CFG
#if (PADS_USB30V_CTRL1_CFG != CONFIG_PADMUX_UNKNOWN)
#define USB30V_CTRL1_CFG ((PADS_USB30V_CTRL1_CFG == CONFIG_PADMUX_MODE1) ? BIT0 :  \
                          (PADS_USB30V_CTRL1_CFG == CONFIG_PADMUX_MODE2) ? BIT1 :  \
                          (PADS_USB30V_CTRL1_CFG == CONFIG_PADMUX_MODE3) ? (BIT1|BIT0) : 0)
    _RVM1(0x1E09, USB30V_CTRL1_CFG, BITMASK(1:0)),
#endif
#endif

    // Clear all pad in
    _RVM1(0x1EA1, 0, BIT7),
    _END_OF_TBL_,

//---------------------------------------------------------------------
// ISP_TOOL Write Protect
//---------------------------------------------------------------------

    //external flash #1 //Alex tung
    _MEMMAP_PM_,
    _RVM1(0x0e6a, 0, BIT0),             //Select to spi_do, spi_di and spi_ck
    _RVM1(0x0e6a, 0, BIT1),             //Select to spi_cz0
    _RVM1(0x08f4, 0, (BIT1 | BIT0)),    //Select to External #1 SPI Flash
 	#if (PIN_FLASH_WP0)
		#if ((MS_BOARD_TYPE_SEL == BD_MST244A_D01A_S_MASERATI))
    	_RVM1(0x0e21, BIT2, BIT2),          //Output Value of PM10
    	_RVM1(0x0e1f, 0, BIT2),             //Output Enable of PM10
    	//Output Enable of PM1

		#else
    		#error [ERROR] Invalid WP0 Settings in Unknown Board
		#endif
	#endif
    _END_OF_TBL_,

    //external flash #2
    _MEMMAP_PM_,
    _RVM1(0x0e6a, 0, BIT0),             //Select to spi_do, spi_di and spi_ck

  #if (PIN_SPI_CZ1)
    _RVM1(0x0e6a, 0, BIT2),             //Select to spi_cz1
    _RVM1(0x08f4, 1, (BIT1 | BIT0)),    //Select to External #2 SPI Flash

	#if (PIN_FLASH_WP1)
		#if ((MS_BOARD_TYPE_SEL == BD_MST244A_D01A_S_MASERATI))
    		_RVM1(0x0e21, BIT2, BIT2),          //Output Value of PM10
    		_RVM1(0x0e1f, 0, BIT2),             //Output Enable of PM10
    		//Output Enable of PM10

		#else
    		#error [ERROR] Invalid WP1 Settings in Unknown Board
		#endif
	#endif
    _END_OF_TBL_,
  #endif
#endif
};

