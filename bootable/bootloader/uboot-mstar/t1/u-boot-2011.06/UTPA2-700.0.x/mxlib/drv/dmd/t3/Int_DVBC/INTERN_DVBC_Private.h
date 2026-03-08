/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INTERN_DVBC_PRIVATE_H_
#define _INTERN_DVBC_PRIVATE_H_




//--------------------------------------------------------------------

// #define     DEMOD_DYNAMIC_SLAVE_ID_1          0x32
// #define     DEMOD_DYNAMIC_SLAVE_ID_2          0x72
// #define     DEMOD_DYNAMIC_SLAVE_ID_3          0xB2
// #define     DEMOD_DYNAMIC_SLAVE_ID_4          0xF2

#define     DEMOD_ADDR_H            0x00
#define     DEMOD_ADDR_L            0x01
#define     DEMOD_WRITE_REG         0x02
#define     DEMOD_WRITE_REG_EX      0x03
#define     DEMOD_READ_REG          0x04
#define     DEMOD_RAM_CONTROL       0x05

// #define INTERN_DVBC_IIC_CONTROL                 0x8400
// #define INTERN_DVBC_IIC_SCL_CLK                 0x8401
// #define INTERN_DVBC_IIC_READ_DATA               0x8402
// #define INTERN_DVBC_IIC_STATUS                  0x8403
// #define INTERN_DVBC_IIC_WRITE_DATA              0x8404

// #define     COFDM_DEM_I2C_ID            DEMOD_DYNAMIC_SLAVE_ID_1
// #define     COFDM_DEM_I2C_ID_2          DEMOD_DYNAMIC_SLAVE_ID_2
// #define     COFDM_DEM_I2C_ID_3          DEMOD_DYNAMIC_SLAVE_ID_3
// #define     COFDM_DEM_I2C_ID_4          DEMOD_DYNAMIC_SLAVE_ID_4

// #define INTERN_DVBC_REG_INVERSION              0x8024
#define BOOL    BOOLEAN

#if  DTV_SCAN_AUTO_FINE_TUNE_ENABLE
    //INTERN_DVBC_ Capture Range fix to 500K
    #define DEMOD_CAPTURE_RANGE_500_K            500
        #define DEMOD_CAPTURE_RANGE_SIZE                                      DEMOD_CAPTURE_RANGE_500_K
#endif




//--------------------------------------------------------------------
typedef enum
{
    E_SYS_UNKOWN = -1,
    E_SYS_DVBT,
    E_SYS_DVBC,
    E_SYS_ATSC,
    E_SYS_VIF,

    E_SYS_NUM
}E_SYSTEM;

typedef enum
{
    CMD_SYSTEM_INIT = 0,
    CMD_DAC_CALI,
    CMD_DVBT_CONFIG,
    CMD_DVBC_CONFIG,
    CMD_VIF_CTRL,
    CMD_FSM_CTRL,
    CMD_INDIR_RREG,
    CMD_INDIR_WREG,
    CMD_GET_INFO,
    CMD_TS_CTRL,
    CMD_TUNED_VALUE,

    CMD_MAX_NUM
}E_CMD_CODE;

typedef enum
{
    pc_op_code = 0,
    pc_if_freq,
    pc_sound_sys,
    pc_vif_vga_maximum_l,
    pc_vif_vga_maximum_h,
    pc_scan_mode,
    pc_vif_top,
    pc_gain_distribution_thr_l,
    pc_gain_distribution_thr_h,

    VIF_PARAM_MAX_NUM
}E_VIF_PARAM;

typedef enum
{
    pc_system = 0,

    SYS_PARAM_MAX_NUM
}E_SYS_PARAM;

typedef enum
{
    SET_IF_FREQ = 0,
    SET_SOUND_SYS,
    VIF_INIT,
    SET_VIF_HANDLER,
    VIF_TOP_ADJUST,

    VIF_CMD_MAX_NUM
}E_VIF_CMD;

typedef enum
{
    TS_PARALLEL = 0,
    TS_SERIAL = 1,

    TS_MODE_MAX_NUM
}E_TS_MODE;

typedef enum
{
    dac_op_code = 0,
    dac_idac_ch0,
    dac_idac_ch1,

    DAC_PARAM_MAX_NUM
}
E_DAC_PARAM;

typedef enum
{
    DAC_RUN_CALI = 0,
    DAC_IDAC_ASSIGN,

    DAC_CMD_MAX_NUM
}
E_DAC_CMD;

typedef enum
{
    agc_ref_small,
    agc_ref_large,
    agc_ref_aci,
    ripple_switch_th_l,
    ripple_switch_th_h,

    TUNED_PARAM_MAX_NUM
}E_TUNED_PARAM;


typedef struct
{
    U8        cmd_code;
    U8        param[64];
} S_CMDPKTREG;

#define REG_CMD_CTRL    0x20CC
#define REG_DTA_CTRL    0x20CD
#define REG_CMD_ADDR    0x20CE
#define REG_CMD_DATA    0x20CF

#define _REG_START      REG_CMD_CTRL
#define _REG_END        REG_CMD_CTRL
#define _REG_DRQ        REG_DTA_CTRL
#define _REG_FSM        REG_CMD_CTRL
#define _REG_ERR        REG_DTA_CTRL

#define _BIT_START      BIT1
#define _BIT_END        BIT0
#define _BIT_DRQ        BIT0
#define _BIT_FSM        BIT3
#define _BIT_ERR        BIT7

//--------------------------------------------------------------------
typedef enum
{
    // OP Mode Settings
    p_opmode_rfagc_en = 0,
    p_opmode_humdet_en,
    p_opmode_dcr_en,
    p_opmode_iqb_en,
    p_opmode_auto_iq_swap,
    p_opmode_auto_fsa_left,
    p_opmode_auto_rfmax,
    p_opmode_mode_forced,
    p_opmode_cp_forced,

    // Config Params
    pc_config_rssi,
    pc_config_zif,
    pc_config_fc_l,
    pc_config_fc_h,
    pc_config_fs_l,
    pc_config_fs_h,
    pc_config_bw,
    pc_config_fsa_left,
    pc_config_rfmax,
    pc_config_lp_sel,
    pc_config_cp,
    pc_config_mode,
    pc_config_iq_swap,
    pc_config_atv_system,
    pc_config_serial_ts,
    pc_config_ts_out_inv,
    pc_config_ts_data_swap,
    pc_config_icfo_range,

    DVBT_PARAM_LEN,
} DVBT_Param;

//--------------------------------------------------------------------
BOOLEAN INTERN_DVBC_Reset ( void );
BOOLEAN INTERN_DVBC_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, U8 param_cnt);
BOOLEAN INTERN_DVBC_Get_TPS_Parameter_Const( WORD * TSP_parameter);
//--------------------------------------------------------------------

#endif

