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
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// [mhal_mux.h]
// Date: 20090220
// Descriptions: Add a new mux layer for HW setting
//==============================================================================
#ifndef MHAL_MUX_H
#define MHAL_MUX_H

///////////////////////////////
//  Mux Hardware Option
///////////////////////////////
// Please set this #define after Mux tree is ready
#define MUX_TREE_HEIGHT 4

#define NUMBER_OF_ANALOG_PORT 3
#define NUMBER_OF_CVBS_PORT 11
#define NUMBER_OF_DVI_PORT 4
#define NUMBER_OF_MVOP_PORT 2
#define NUMBER_OF_CVBS_OUT_PORT 2
#define NUMBER_OF_YPBPR_OUT_PORT 0
#define PIP_SUPPORTED (MAX_WINDOW_NUM > 1)
#define NUMBER_OF_SCALER_OP_PORT 1

#define HAL_DVI_IP_A    0 //alex_tung
#define HAL_DVI_IP_B    1
#define HAL_DVI_IP_C    2
#define HAL_DVI_IP_D    3

/*
    Input ports.
    It is interface between Hal and Driver level.
*/

typedef enum
{
    HAL_INPUTPORT_NONE = INPUT_PORT_NONE_PORT,

    HAL_INPUTPORT_ANALOG0 = INPUT_PORT_ANALOG0,
    HAL_INPUTPORT_ANALOG1,
    HAL_INPUTPORT_ANALOG2,

    HAL_INPUTPORT_ANALOG_SYNC0 = INPUT_PORT_ANALOG0_SYNC,
    HAL_INPUTPORT_ANALOG_SYNC1,
    HAL_INPUTPORT_ANALOG_SYNC2,
    HAL_INPUTPORT_ANALOG_SYNC3,

    HAL_INPUTPORT_YMUX_CVBS0 = INPUT_PORT_YMUX_CVBS0,
    HAL_INPUTPORT_YMUX_CVBS1,
    HAL_INPUTPORT_YMUX_CVBS2,
    HAL_INPUTPORT_YMUX_CVBS3,
    HAL_INPUTPORT_YMUX_CVBS4,
    HAL_INPUTPORT_YMUX_CVBS5,
    HAL_INPUTPORT_YMUX_CVBS6,
    HAL_INPUTPORT_YMUX_CVBS7,
    HAL_INPUTPORT_YMUX_G0,
    HAL_INPUTPORT_YMUX_G1,
    HAL_INPUTPORT_YMUX_G2,

    HAL_INPUTPORT_CMUX_CVBS0 = INPUT_PORT_CMUX_CVBS0,
    HAL_INPUTPORT_CMUX_CVBS1,
    HAL_INPUTPORT_CMUX_CVBS2,
    HAL_INPUTPORT_CMUX_CVBS3,
    HAL_INPUTPORT_CMUX_CVBS4,
    HAL_INPUTPORT_CMUX_CVBS5,
    HAL_INPUTPORT_CMUX_CVBS6,
    HAL_INPUTPORT_CMUX_CVBS7,
    HAL_INPUTPORT_CMUX_R0,
    HAL_INPUTPORT_CMUX_R1,
    HAL_INPUTPORT_CMUX_R2,

    HAL_INPUTPORT_DVI0 = INPUT_PORT_DVI0,
    HAL_INPUTPORT_DVI1,
    HAL_INPUTPORT_DVI2,
    HAL_INPUTPORT_DVI3,

    HAL_INPUTPORT_MVOP = INPUT_PORT_MVOP,
    HAL_INPUTPORT_MVOP2,

    HAL_INPUTPORT_SCALER_OP = INPUT_PORT_SCALER_OP,
} E_INPUT_PORT_TYPE;

// Output ports.


typedef enum
{
    HAL_OUTPUTPORT_NONE_PORT = OUTPUT_PORT_NONE_PORT,

    HAL_OUTPUTPORT_SCALER_MAIN_WINDOW = OUTPUT_PORT_SCALER_MAIN_WINDOW,
    HAL_OUTPUTPORT_SCALER2_MAIN_WINDOW = OUTPUT_PORT_SCALER2_MAIN_WINDOW,

    HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 = OUTPUT_PORT_SCALER_SUB_WINDOW1,
    HAL_OUTPUTPORT_SCALER2_SUB_WINDOW = OUTPUT_PORT_SCALER2_SUB_WINDOW,

    HAL_OUTPUTPORT_CVBS1 = OUTPUT_PORT_CVBS1,
    HAL_OUTPUTPORT_CVBS2 ,

    HAL_OUTPUTPORT_YPBPR1 = OUTPUT_PORT_YPBPR1,

    HAL_OUTPUTPORT_DIP_WINDOW = OUTPUT_PORT_DWIN,


}E_OUTPUT_PORT_TYPE;

typedef enum
{
    SC_MAINWIN_IPMUX_ADC_A      = 0,            ///< ADC A
    SC_MAINWIN_IPMUX_HDMI_DVI   = 1,            ///< DVI
    SC_MAINWIN_IPMUX_VD         = 2,            ///< VD
    SC_MAINWIN_IPMUX_MVOP       = 3,            ///< MPEG/DC0
    SC_MAINWIN_IPMUX_SC_VOP     = 4,            ///< Scaler VOP output
    SC_MAINWIN_IPMUX_EXT_VD     = 5,            ///< External VD
    SC_MAINWIN_IPMUX_ADC_B      = 6,            ///< ADC B
    SC_MAINWIN_IPMUX_CAPTURE    = 7,            ///< Capture
    SC_MAINWIN_IPMUX_MVOP2      = 8,            ///< MPEG/DC1
    SC_MAINWIN_IPMUX_IP_SUB     = 9,            ///< IP SUB/VE
    SC_MAINWIN_IPMUX_NOUSE      = 10,           ///< no use now
    SC_MAINWIN_IPMUX_DC2        = 11,           ///< DC2
    SC_MAINWIN_IPMUX_IP_SUB2    = 12,           ///< IP SUB2/DIP
    SC_MAINWIN_IPMUX_MHL        = 13,           ///< MHL
    SC_MAINWIN_IPMUX_VD2        = 14,           ///< VD2
} SC_MAINWIN_IPMUX_TYPE;

typedef enum
{
    SC_SUBWIN_IPMUX_ADC_A      = 0,            ///< ADC A
    SC_SUBWIN_IPMUX_HDMI_DVI   = 1,            ///< DVI
    SC_SUBWIN_IPMUX_VD         = 2,            ///< VD
    SC_SUBWIN_IPMUX_MVOP       = 3,            ///< MPEG/DC0
    SC_SUBWIN_IPMUX_SC_VOP     = 4,            ///< Scaler VOP output
    SC_SUBWIN_IPMUX_EXT_VD     = 5,            ///< External VD
    SC_SUBWIN_IPMUX_ADC_B      = 6,            ///< ADC B
    SC_SUBWIN_IPMUX_MLINK      = 8,            ///< MLink
    SC_SUBWIN_IPMUX_CAPTURE    = 7,            ///< Capture
    SC_SUBWIN_IPMUX_MVOP2      = 8,            ///< MPEG/DC1
    SC_SUBWIN_IPMUX_IP_SUB     = 9,            ///< IP SUB/VE
    SC_SUBWIN_IPMUX_NOUSE      = 10,           ///< no use now
    SC_SUBWIN_IPMUX_DC2        = 11,           ///< DC2
    SC_SUBWIN_IPMUX_IP_SUB2    = 12,           ///< IP SUB2/DIP
    SC_SUBWIN_IPMUX_MHL        = 13,           ///< MHL
    SC_SUBWIN_IPMUX_VD2        = 14,           ///< VD2
} SC_SUBWIN_IPMUX_TYPE;

typedef enum
{
    SC_CLK_ADC_A      = 0,            ///< ADC A
    SC_CLK_HDMI_DVI   = 1,            ///< DVI
    SC_CLK_VD         = 2,            ///< VD
    SC_CLK_DC0        = 3,            ///< MPEG/DC0
    SC_CLK_ADC_B      = 4,            ///< ADC_B

    SC_CLK_ODCLK      = 7,            ///< ODLCK
    SC_CLK_DC1        = 8,            ///< MPEG/DC1
    SC_CLK_ADC_C      = 9,            ///< ADC
    SC_CLK_ODCLK_B    = 10,           ///<ODCLK
    SC_CLK_MHL        = 11,           ///<MHL
} SC_IDCLK_TYPE;

typedef enum
{
    SC_OFFLINE_IPMUX_ADC_A      = 0,            ///< ADC A
    SC_OFFLINE_IPMUX_HDMI_DVI   = 1,            ///< DVI
    SC_OFFLINE_IPMUX_VD         = 2,            ///< VD
    SC_OFFLINE_IPMUX_MVOP       = 3,            ///< MPEG/DC0
    SC_OFFLINE_IPMUX_SC_VOP     = 4,            ///< Scaler VOP output
    SC_OFFLINE_IPMUX_EXT_VD     = 5,            ///< External VD
    SC_OFFLINE_IPMUX_ADC_B      = 6,            ///< ADC B
    SC_OFFLINE_IPMUX_CAPTURE    = 7,            ///< Capture
    SC_OFFLINE_IPMUX_MVOP2      = 8,            ///< MPEG/DC1
    SC_OFFLINE_IPMUX_IP_SUB     = 9,            ///< IP SUB/VE
    SC_OFFLINE_IPMUX_NOUSE      = 10,           ///< no use now
    SC_OFFLINE_IPMUX_DC2        = 11,           ///< DC2
    SC_OFFLINE_IPMUX_IP_SUB2    = 12,           ///< IP SUB2/DIP
    SC_OFFLINE_IPMUX_MHL        = 13,           ///< MHL
} SC_OFFLINE_IPMUX_TYPE;

typedef enum
{
    SC_DWIN_IPMUX_ADC_A      = 0,            ///< ADC A
    SC_DWIN_IPMUX_HDMI_DVI   = 1,            ///< DVI
    SC_DWIN_IPMUX_VD         = 2,            ///< VD
    SC_DWIN_IPMUX_MVOP       = 3,            ///< MPEG/DC0
    SC_DWIN_IPMUX_SC_VOP     = 4,            ///< Scaler VOP output
    SC_DWIN_IPMUX_EXT_VD     = 5,            ///< External VD
    SC_DWIN_IPMUX_ADC_B      = 6,            ///< ADC B
    SC_DWIN_IPMUX_MLINK      = 8,            ///< MLink
    SC_DWIN_IPMUX_CAPTURE    = 7,            ///< Capture
    SC_DWIN_IPMUX_MVOP2      = 8,            ///< MPEG/DC1
    SC_DWIN_IPMUX_IP_SUB     = 9,            ///< IP SUB/VE
    SC_DWIN_IPMUX_NOUSE      = 10,           ///< no use now
    SC_DWIN_IPMUX_DC2        = 11,           ///< DC2
    SC_DWIN_IPMUX_IP_SUB2    = 12,           ///< IP SUB2/DIP
    SC_DWIN_IPMUX_MHL        = 13,           ///< MHL
    SC_DWIN_IPMUX_VD2        = 14,           ///< VD2
} SC_DWIN_IPMUX_TYPE;

typedef enum
{
    SC2_MAINWIN_IPMUX_ADC_A      = 0,            ///< ADC A
    SC2_MAINWIN_IPMUX_HDMI_DVI   = 1,            ///< DVI
    SC2_MAINWIN_IPMUX_VD         = 2,            ///< VD
    SC2_MAINWIN_IPMUX_MVOP       = 3,            ///< MPEG/DC0

    SC2_MAINWIN_IPMUX_CAPTURE    = 7,            ///< Capture
    SC2_MAINWIN_IPMUX_MVOP2      = 8,            ///< MPEG/DC1

    SC2_MAINWIN_IPMUX_SPT4K_CH0  = 11,           ///< 4K spliter ch0

    SC2_MAINWIN_IPMUX_MHL        = 13,           ///< MHL
    SC2_MAINWIN_IPMUX_SPT4K_CH1  = 14,           ///< 4K spliter ch1
} SC2_MAINWIN_IPMUX_TYPE;

typedef enum
{
    SC2_CLK_ADC_A      = 0,            ///< ADC A
    SC2_CLK_HDMI_DVI   = 1,            ///< DVI
    SC2_CLK_VD         = 2,            ///< VD
    SC2_CLK_DC0        = 3,            ///< MPEG/DC0
    SC2_CLK_ADC_B      = 4,            ///< ADC_B

    SC2_CLK_ODCLK      = 7,            ///< ODLCK
    SC2_CLK_DC1        = 8,            ///< MPEG/DC1
    SC2_CLK_ADC_C      = 9,            ///< ADC
    SC2_CLK_ODCLK_B    = 10,           ///<ODCLK
    SC2_CLK_MHL        = 13,           ///<MHL
} SC2_IDCLK_TYPE;

void Hal_SC_mux_dispatch(void *pInstance, E_MUX_INPUTPORT src , E_MUX_OUTPUTPORT dest);
void Hal_SC_mux_set_dvi_mux(void *pInstance, MS_U8 PortId, E_OUTPUT_PORT_TYPE enDstPort);
void Hal_SC_mux_set_adc_y_mux(void *pInstance, MS_U8 PortId);
void Hal_SC_mux_set_adc_c_mux(void *pInstance, MS_U8 PortId);
void Hal_SC_set_sync_port_by_dataport(void *pInstance, E_MUX_INPUTPORT src_port );

void Hal_SC_mux_set_mainwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux);
MS_BOOL Hal_SC_mux_get_mainwin_ip_mux(void *pInstance, MS_U8 *pU8Data_Mux, MS_U8 *pU8Clk_Mux);
void Hal_SC_set_subwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux);
void Hal_SC_mux_set_dipwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux);
void Hal_SC_EnableCLK_for_DIP(void *pInstance, MS_BOOL bEnable);

void Hal_SC2_mux_set_mainwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux);
MS_BOOL Hal_SC2_mux_get_mainwin_ip_mux(void *pInstance, MS_U8 *pU8Data_Mux, MS_U8 *pU8Clk_Mux);
void Hal_SC_mux_SetScalerIndexInformationToADC(void *pInstance, MS_U8 u8ScalerIndex, MS_U16 u16InputSourceType);

#endif // MHAL_MUX_H
