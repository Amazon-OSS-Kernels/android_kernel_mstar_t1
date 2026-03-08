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
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef MHAL_MENULOAD_H
#define MHAL_MENULOAD_H

//Select the source to trigger menuload
#define TRIG_SRC_FE_VFDE        0
#define TRIG_SRC_RE_VSYNC       1
#define TRIG_SRC_FE_VSYNC       2
#define TRIG_SRC_DELAY_LINE     3

#define OPM_LOCK_INIT_STATE     0
#define OPM_LOCK_FE_VSYNC       1
#define OPM_LOCK_TRAIN_TRIG_P   2
#define OPM_LOCK_DS             3

#define  MS_MLOAD_CMD_ALIGN     4
#define  MS_MLOAD_REG_LEN       0x04 // how many data in one MIU request
#define  MS_MLOAD_CMD_LEN       MS_MLOAD_MEM_BASE_UNIT
#define  MS_MLOAD_MEM_BASE_UNIT BYTE_PER_WORD
#define  MS_MLOAD_BUS_WIDTH     (32)
#define  MS_MLOAD_MAX_CMD_CNT   192
#define  MS_MLOAD_NULL_CMD      0xFF010000
#define  MS_MLOAD_DUMMY_CMD_CNT(x)  ((x+0x0003)&~0x0003)
#define  MS_MLOAD_END_CMD(x) (0x1F020000 | (MS_MLOAD_REG_LEN) | (_XC_Device_Offset[psXCInstPri->u32DeviceID] << 24))
#define  MS_MLOAD_MIU_BUS_SEL   0x00

#define  MS_MLOAD_CMD_LEN_64BITS 8 //8 bytes
#define  MS_MLOAD_NULL_CMD_SPREAD_MODE    (((MS_U64)0x13FF<<23)|((MS_U64)0x01<<16)|((MS_U64)0x0000))  //0x00000009FF810000
#define  MS_MLOAD_END_CMD_SPREAD_MODE(x)   ( ((MS_U64)0x0000<<48) | (((MS_U64)0x131F<<23)|((MS_U64)0x02<<16)|((MS_U64)0x0000)) | (MS_MLOAD_REG_LEN) ) //(0x000000098F820000 | (MS_MLOAD_REG_LEN)) = 0x000000098F820004
#define  MS_MLOAD_END_CMD_DEV1_SPREAD_MODE(x)   ( ((MS_U64)0x0000<<48) | (((MS_U64)0x139F<<23)|((MS_U64)0x02<<16)|((MS_U64)0x0000)) | (MS_MLOAD_REG_LEN) ) //(0x000000098F820000 | (MS_MLOAD_REG_LEN)) = 0x000000098F820004


#define  MS_MLG_REG_LEN         0x40 // how many data in one MIU request
#define  MS_MLG_CMD_LEN         BYTE_PER_WORD
#define  MS_MLG_MEM_BASE_UNIT   BYTE_PER_WORD

typedef struct
{
    union
    {
        struct
        {
        	MS_U16 u16Data;
        	MS_U8 u8Addr;
        	MS_U8 u8Bank;
        };
        MS_U32 u32Cmd;
    };
}MS_MLoad_Data;

typedef struct
{
    union
    {
        struct
        {
        	MS_U16 u16Data;
        	MS_U8  u8Addr; //addr 0 ~ 0x7F
        	MS_U8  u8Bank; //subbank 0 ~ 0xFF
        	MS_U32 u32NoUse;
        };
        MS_U64 u64Cmd;
    };
}MS_MLoad_Data_64Bits_SubBank;

#ifdef MLG_1024 // Gamma_1024
typedef struct
{
    MS_U64 BData0 : 12;     //0
    MS_U64 BData1 : 12;
    MS_U64 BData2 : 12;
    MS_U64 GData0 : 12;
    MS_U64 GData1 : 12;
    MS_U64 GData2_L : 4;      //63

    MS_U64 GData2_H : 8;      //64
    MS_U64 RData0 : 12;
    MS_U64 RData1 : 12;
    MS_U64 RData2 : 12;
    MS_U64 Dummy0 :20;      //127

    MS_U64 Dummy1 : 16 ;       //128
    MS_U64 BEnable : 1 ;
    MS_U64 GEnable : 1 ;
    MS_U64 REnable : 1 ;
    MS_U64 Dummy2 :45;      //191

    MS_U64 Dummy3;      //192~255
} MS_SC_MLG_TBL;
#else // Gamma_256 or not support MLG case
typedef struct
{
    MS_U16 u16B;
    MS_U16 u16G;
    MS_U16 u16R;
    MS_U16 u16Enable;
    MS_U16 u16Dummy[4];
} MS_SC_MLG_TBL;
#endif

typedef enum
{
    MLOAD_TRIGGER_BY_OP_SYNC=0,
    MLOAD_TRIGGER_BY_IP_MAIN_SYNC=1,
    MLOAD_TRIGGER_BY_IP_SUB_SYNC=2,
    MLOAD_TRIGGER_BY_SW=3,
    MLOAD_TRIGGER_MAX,
}MLoad_Trigger_Sync;

typedef enum
{
    MLoad_WD_Timer_Reset_DMA = 0,
    MLoad_WD_Timer_Reset_MIU = 1,
    MLoad_WD_Timer_Reset_ALL = 3,
    MLoad_WD_Timer_Reset_MAX,
}MLoad_WD_Timer_Reset_Type;
#define Mhal_XC_MLoad_Fire                                             MDrv_XC_MLoad_Fire
#define Mhal_XC_MLoad_GetStatus                                        MDrv_XC_MLoad_GetStatus
#define Mhal_XC_MLoad_WriteCmd                                         MDrv_XC_MLoad_WriteCmd
#define Mhal_XC_MLoad_set_IP_trig_p                                    MDrv_XC_MLoad_set_IP_trig_p
#define Mhal_XC_MLoad_set_trigger_sync                                 MDrv_XC_MLoad_set_trigger_sync

MS_U16 Hal_XC_MLoad_get_status(void *pInstance);
void Hal_XC_MLoad_set_on_off(void *pInstance, MS_BOOL bEn);
void Hal_XC_MLoad_set_len(void *pInstance, MS_U16 u16Len);
void Hal_XC_MLoad_set_depth(void *pInstance, MS_U16 u16depth);
void Hal_XC_MLoad_set_miusel(void *pInstance, MS_U8 u8MIUSel);
void Hal_XC_MLoad_set_base_addr(void *pInstance, MS_PHY u32addr);
void Hal_XC_MLoad_set_trigger_timing(void *pInstance, MS_U16 u16sel);
void Hal_XC_MLoad_set_opm_lock(void *pInstance, MS_U16 u16sel);
void Hal_XC_MLoad_set_trigger_delay(void *pInstance, MS_U16 u16delay);
void Hal_XC_MLoad_set_trig_p(void *pInstance, MS_U16 u16train, MS_U16 u16disp);
MS_BOOL Hal_XC_MLoad_get_trig_p(void *pInstance, MS_U16 *pu16Train, MS_U16 *pu16Disp);
void Hal_XC_MLoad_Set_riu(void *pInstance, MS_BOOL bEn);
MS_BOOL Hal_XC_MLoad_GetCaps(void *pInstance);
void Hal_XC_MLoad_set_riu_cs(void *pInstance, MS_BOOL bEn);
void Hal_XC_MLoad_set_sw_dynamic_idx_en(void *pInstance, MS_BOOL ben);
void Hal_XC_MLoad_set_opm_arbiter_bypass(void *pInstance, MS_BOOL ben);
void Hal_XC_MLoad_set_miu_bus_sel(void *pInstance, MS_U8 u8BitMode);
void Hal_XC_MLoad_enable_watch_dog(void *pInstance, MS_BOOL bEn);
void Hal_XC_MLoad_set_watch_dog_time_delay(void *pInstance, MS_U32 value);
void Hal_XC_MLoad_enable_watch_dog_reset(void *pInstance, MLoad_WD_Timer_Reset_Type enMLWDResetType);

MS_BOOL Hal_XC_MLG_GetCaps(void *pInstance);
MS_U16 Hal_XC_MLG_get_status(void *pInstance);
void Hal_XC_MLG_set_on_off(void *pInstance, MS_BOOL bEn);
void Hal_XC_MLG_set_len(void *pInstance, MS_U16 u16Len);
void Hal_XC_MLG_set_depth(void *pInstance, MS_U16 u16depth);
void Hal_XC_MLG_set_base_addr(void *pInstance, MS_PHY u32addr);
void Hal_XC_MLG_set_trigger_timing(void *pInstance, MS_U16 u16sel);
void Hal_XC_MLG_set_trigger_delay(void *pInstance, MS_U16 u16delay);
void Hal_XC_MLG_set_init_addr(void *pInstance, MS_U16 u16addr);

void Hal_XC_MLoad_Enable_64BITS_COMMAND(void *pInstance,MS_BOOL bEn);
void Hal_XC_MLoad_Enable_64BITS_SPREAD_MODE(void *pInstance,MS_BOOL bEn);
void Hal_XC_MLoad_Command_Format_initial(void *pInstance);
void Hal_XC_MLoad_Set_64Bits_MIU_Bus_Sel(void *pInstance);
MS_U8 Hal_XC_MLoad_Get_64Bits_MIU_Bus_Sel(void *pInstance);
MS_U64 Hal_XC_MLoad_Gen_64bits_spreadMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL Hal_XC_MLoad_parsing_64bits_spreadMode_NonXC(void *pInstance,MS_U64 u64Cmd, MS_U32 *u32Addr, MS_U16 *u16Data);
MS_U64 Hal_XC_MLoad_Gen_64bits_spreadMode_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_U64 Hal_XC_MLoad_Gen_64bits_subBankMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_U32 Hal_XC_MLoad_Gen_32bits_subBankMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL Hal_XC_MLoad_parsing_32bits_subBankMode(void *pInstance, MS_U32 u32MloadData, MS_U32 *pu32Addr, MS_U16 *pu16Data);
MS_U16 Hal_XC_MLoad_Get_Depth(void *pInstance, MS_U16 u16CmdCnt);

void Hal_XC_MLoad_set_trigger_sync(void *pInstance, MLoad_Trigger_Sync eTriggerSync);
void Hal_SC_ControlMloadTrig(void *pInstance);
void Hal_XC_MLoad_set_BitMask(void *pInstance,MS_BOOL enable);
#endif

