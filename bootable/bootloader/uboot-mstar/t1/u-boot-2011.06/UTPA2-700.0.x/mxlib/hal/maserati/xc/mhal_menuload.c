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
//==============================================================================
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "xc_hwreg_utility2.h"
#include "xc_Analog_Reg.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Auto.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#endif
#include "XC_private.h"
// Registers
#include "mhal_menuload.h"
#include "hwreg_sc.h"
#include "mdrv_sc_dynamicscaling.h"

MS_BOOL Hal_XC_MLoad_GetCaps(void *pInstance)
{
    return TRUE;
}

MS_U16 Hal_XC_MLoad_get_status(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L) & 0x8000)>>15);
}

void Hal_XC_MLoad_set_on_off(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(bEn)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x8000, 0x8000);
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x0000, 0x8000);
}

void Hal_XC_MLoad_set_len(void *pInstance, MS_U16 u16Len)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u16Len &= 0x7FF;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, u16Len, 0x7FF);
}

void Hal_XC_MLoad_set_depth(void *pInstance, MS_U16 u16depth)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_01_L, u16depth);
}

void Hal_XC_MLoad_set_miusel(void *pInstance, MS_U8 u8MIUSel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (u8MIUSel == 0)
    {
      SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1E_L, 0x0000, 0x0003);
    }
    else if (u8MIUSel == 1)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1E_L, 0x0001, 0x0003);
    }
    //else if (u8MIUSel == 2)
    //{
    //    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1E_L, 0x0002, 0x0003);
    //}
}

void Hal_XC_MLoad_set_base_addr(void *pInstance, MS_PHY u32addr)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u32addr /= MS_MLOAD_MEM_BASE_UNIT;

    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_03_L, (MS_U16)(u32addr & 0xFFFF));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_04_L, (MS_U16)((u32addr & 0x3FF0000)>>16), 0x003FF);
}

void Hal_XC_MLoad_Set_riu(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (bEn)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_10_L, 0x1000, 0x1000);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_10_L, 0x0000, 0x1000);
    }
}

void Hal_XC_MLoad_set_trigger_timing(void *pInstance, MS_U16 u16sel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u16sel = (u16sel & 0x0003)<<12;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_19_L, u16sel, 0x3000);
}

void Hal_XC_MLoad_set_opm_lock(void *pInstance, MS_U16 u16sel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    u16sel = (u16sel & 0x0003)<<8;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_19_L, u16sel, 0x0300);
}

void Hal_XC_MLoad_set_trigger_delay(void *pInstance, MS_U16 u16delay)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_22_L, u16delay, 0x0FFF);
}

//___|T_________________........__|T____ VSync
//__________|T__________________         ATP(refer the size befor memory to cal the pip sub and main length)
//_________________|T___________         Disp

//Generate TRAIN_TRIG_P from delayed line of Vsync(Setting the delay line for Auto tune area)
//Generate DISP_TRIG_P from delayed line of Vsync(Setting the delay line for Display area)
void Hal_XC_MLoad_set_trig_p(void *pInstance, MS_U16 u16train, MS_U16 u16disp)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1A_L, u16train, 0x0FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1B_L, u16disp,  0x0FFF);
}

//Get the delay line for Auto tune area
//Get the delay line for Display area
MS_BOOL Hal_XC_MLoad_get_trig_p(void *pInstance, MS_U16 *pu16Train, MS_U16 *pu16Disp)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    *pu16Train = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1A_L, 0x0FFF);
    *pu16Disp = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1B_L, 0x0FFF);
    return TRUE;
}

void Hal_XC_MLoad_set_riu_cs(void *pInstance, MS_BOOL bEn)
{
    if(bEn)
    {
        MDrv_WriteByteMask(0x100104, 0x10, 0x10);
    }
    else
    {
        MDrv_WriteByteMask(0x100104, 0x00, 0x10);
    }
}


void Hal_XC_MLoad_set_sw_dynamic_idx_en(void *pInstance, MS_BOOL ben)
{
    ben = ben;
}

void Hal_XC_MLoad_set_miu_bus_sel(void *pInstance, MS_U8 u8BitMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_13_L, (u8BitMode << 14), 0xC000); //00: 64bit, 01:128bit, 11:256bit, ML/DS is use 0x00, DS seperate mode is use 0x01
}

void Hal_XC_MLoad_enable_watch_dog(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(bEn)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_05_L, 0xC000, 0xF000);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_05_L, 0x0000, 0xF000);
    }
}

void Hal_XC_MLoad_enable_watch_dog_reset(void *pInstance, MLoad_WD_Timer_Reset_Type enMLWDResetType)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_06_L, (enMLWDResetType << 8), BIT(8)|BIT(9));
}

void Hal_XC_MLoad_set_watch_dog_time_delay(void *pInstance, MS_U32 value)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_05_L, value, 0x03FF);
}

void Hal_XC_MLoad_set_opm_arbiter_bypass(void *pInstance, MS_BOOL ben)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (ben)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_70_L, BIT(2), BIT(2));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_70_L, 0x00, BIT(2));
    }
}

MS_BOOL Hal_XC_MLG_GetCaps(void *pInstance)
{
    return TRUE;
}

MS_U16 Hal_XC_MLG_get_status(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_22_L) & 0x8000)>>15);
}

void Hal_XC_MLG_set_on_off(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(bEn)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_22_L, 0x8000, 0x8000);
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_22_L, 0x0000, 0x8000);
}

void Hal_XC_MLG_set_len(void *pInstance, MS_U16 u16Len)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u16Len &= 0x7FF;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_22_L, u16Len, 0x7FF);
}


void Hal_XC_MLG_set_depth(void *pInstance, MS_U16 u16depth)
{
    // actual u16CmdCnt is based on different MIU bus width(BYTE_PER_WORD)
    // so if it is a 128bit bus width chip with 1024 gamma entry of 256 bit table width
    // we must multiply the command count by 256/128 so all of the entris are downloaded
    // into SRAM.
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u16depth = u16depth * sizeof(MS_SC_MLG_TBL) / MS_MLG_MEM_BASE_UNIT;
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_21_L, u16depth);
}

void Hal_XC_MLG_set_base_addr(void *pInstance, MS_PHY u32addr)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u32addr /= MS_MLG_MEM_BASE_UNIT;
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_23_L, (MS_U16)(u32addr & 0xFFFF));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_24_L, (MS_U16)((u32addr & 0x3FF0000)>>16), 0x03FF);
}

void Hal_XC_MLG_set_trigger_timing(void *pInstance, MS_U16 u16sel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_20_L, ((u16sel & 0x0003)<<14), 0xC000);
}

void Hal_XC_MLG_set_trigger_delay(void *pInstance, MS_U16 u16delay)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_20_L, (u16delay & 0x0FFF), 0x0FFF);
}

void Hal_XC_MLG_set_init_addr(void *pInstance, MS_U16 u16addr)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_25_L, (u16addr & 0x03FF), 0x03FF);
}

void Hal_XC_MLoad_Enable_64BITS_COMMAND(void *pInstance,MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK1F_70_L, bEn?BIT(0):0x00, BIT(0));
}

void Hal_XC_MLoad_Enable_64BITS_SPREAD_MODE(void *pInstance,MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK1F_70_L, bEn?BIT(15):0x00, BIT(15));
}

void Hal_XC_MLoad_Set_64Bits_MIU_Bus_Sel(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16sel = 0x00;

    if( MS_MLOAD_BUS_WIDTH == 8 )
    {
        u16sel = 0x00;
    }
    else if( MS_MLOAD_BUS_WIDTH == 16 )
    {
        u16sel = 0x01;
    }
    else if( MS_MLOAD_BUS_WIDTH == 32 )
    {
        u16sel = 0x3;
    }
    else
    {
        printf("MIU Bus not support !!!!!!!!!!!!!!!!!\n");
        u16sel = 0x00;
    }

    u16sel = (u16sel & 0x0003)<<14;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK1F_13_L, u16sel, 0xC000);
}

MS_U8 Hal_XC_MLoad_Get_64Bits_MIU_Bus_Sel(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    return (SC_R2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK1F_13_L, 0xC000) >>14);
}

void Hal_XC_MLoad_Command_Format_initial(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (IS_SUPPORT_64BITS_COMMAND(ENABLE_64BITS_COMMAND, psXCInstPri->u32DeviceID))
    {
        Hal_XC_MLoad_Enable_64BITS_COMMAND(pInstance,TRUE);
        if(ENABLE_64BITS_SPREAD_MODE)
        {
            Hal_XC_MLoad_Enable_64BITS_SPREAD_MODE(pInstance,TRUE);
        }
        //select MIU Bus : 00: 64bit, 01:128bit, 11:256bit
        Hal_XC_MLoad_Set_64Bits_MIU_Bus_Sel(pInstance);
    }
    else
    {
        Hal_XC_MLoad_set_miu_bus_sel(pInstance, MS_MLOAD_MIU_BUS_SEL);
    }
}

MS_U64 Hal_XC_MLoad_Gen_64bits_spreadMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U64 u64CmdTemp = 0;
    MS_U8  u8AddrTemp = 0;
    MS_U16 u16BankTemp = 0;
    MS_U16 u16DataTemp = 0;
    MS_U16 u16MaskTemp = 0;

    u16MaskTemp = 0xFFFF;
    u16DataTemp = (SC_R2BYTE(psXCInstPri->u32DeviceID,u32Addr) & ~u16Mask) | (u16Data & u16Mask);

    u8AddrTemp= (u32Addr & 0xFF) >> 1;
    u16BankTemp= (0x1300 | ((u32Addr >> 8) & 0xFF)) + _XC_Device_Offset[psXCInstPri->u32DeviceID];

    u64CmdTemp|= (MS_U64)u16DataTemp;
    u64CmdTemp|= ((MS_U64)u8AddrTemp<<16);
    u64CmdTemp|= ((MS_U64)u16BankTemp<<23);
    u64CmdTemp|= ((MS_U64)u16MaskTemp<<48);
    return u64CmdTemp;
}

MS_BOOL Hal_XC_MLoad_parsing_64bits_spreadMode_NonXC(void *pInstance,MS_U64 u64Cmd, MS_U32 *u32Addr, MS_U16 *u16Data)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U8  u8AddrTemp = 0;
    MS_U16 u16BankTemp = 0;

    *u16Data = (MS_U16)(0xFFFF&(MS_U16)u64Cmd);
    u8AddrTemp= (MS_U8)((u64Cmd>>16 & 0x7F) << 1);
    u16BankTemp= (MS_U16)((u64Cmd >> 23) & 0xFFFF);
    *u32Addr = (MS_U32)(u8AddrTemp|u16BankTemp<<8);

    return TRUE;
}

MS_U64 Hal_XC_MLoad_Gen_64bits_spreadMode_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U64 u64CmdTemp = 0;

    u64CmdTemp|= (MS_U64)u16Data;
    u64CmdTemp|= ((MS_U64) ((u32Addr<<16) >>1));
    u64CmdTemp|= ((MS_U64)u32Bank<<23);
    u64CmdTemp|= ((MS_U64)u16Mask<<48);

    return u64CmdTemp;
}

MS_U64 Hal_XC_MLoad_Gen_64bits_subBankMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_MLoad_Data_64Bits_SubBank data_SBank_Mode;
    data_SBank_Mode.u32NoUse = 0x0;
    data_SBank_Mode.u8Addr = (u32Addr & 0xFF) >> 1;
    data_SBank_Mode.u8Bank = ((u32Addr >> 8) & 0xFF) + _XC_Device_Offset[psXCInstPri->u32DeviceID];

    if( u16Mask == 0xFFFF )
    {
        data_SBank_Mode.u16Data = u16Data;
    }
    else
    {
        data_SBank_Mode.u16Data = (SC_R2BYTE(psXCInstPri->u32DeviceID,u32Addr) & ~u16Mask) | (u16Data & u16Mask);
    }
    return data_SBank_Mode.u64Cmd;
}

MS_BOOL Hal_XC_MLoad_parsing_32bits_subBankMode(void *pInstance, MS_U32 u32MloadData, MS_U32 *pu32Addr, MS_U16 *pu16Data)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_MLoad_Data data;
    data.u32Cmd = u32MloadData;
    *pu32Addr = (((MS_U32)(data.u8Addr))<<1) + ((((MS_U32)(data.u8Bank)) - _XC_Device_Offset[psXCInstPri->u32DeviceID])<<8);
    *pu16Data = data.u16Data;

    return TRUE;
}

MS_U32 Hal_XC_MLoad_Gen_32bits_subBankMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_MLoad_Data data;
    data.u8Addr = (u32Addr & 0xFF) >> 1;
    data.u8Bank = ((u32Addr >> 8) & 0xFF) + _XC_Device_Offset[psXCInstPri->u32DeviceID];

    if( u16Mask == 0xFFFF )
    {
        data.u16Data = u16Data;
    }
    else
    {
        data.u16Data = (SC_R2BYTE(psXCInstPri->u32DeviceID, u32Addr) & ~u16Mask) | (u16Data & u16Mask);
    }
    return data.u32Cmd;
}

MS_U16 Hal_XC_MLoad_Get_Depth(void *pInstance, MS_U16 u16CmdCnt)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 result = u16CmdCnt;
    if (IS_SUPPORT_64BITS_COMMAND(ENABLE_64BITS_COMMAND, psXCInstPri->u32DeviceID))
    {
        MS_U16 u16CmdLength = 0;
        MS_U16 u16CmdNum = 0;

        u16CmdLength = 8;//64 bits command = 8 bytes
        u16CmdNum = MS_MLOAD_BUS_WIDTH / u16CmdLength;
        if((u16CmdCnt%u16CmdNum)!=0)
        {
           printf("KickOff: Commands are not full!!\n");
        }
        result = u16CmdCnt/u16CmdNum;
    }
    else
    {
        result = 0;
    }

    return result;
}

void Hal_XC_MLoad_set_trigger_sync(void *pInstance, MLoad_Trigger_Sync eTriggerSync)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    switch(eTriggerSync)
    {
        case MLOAD_TRIGGER_BY_IP_MAIN_SYNC:
        {//trigger by IP_Main Vsync
           SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x2000, 0x7000);
        }
        break;
        case MLOAD_TRIGGER_BY_IP_SUB_SYNC:
        {//trigger by IP_Sub Vsync
           SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x3000, 0x7000);
        }
        break;
        default:
        {
            //default: trigger by OP Vsync
            #if 1
            if( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x7000)!=0x0000)
            {
                //HW patch: menuload (triggered by ip vsync) can not stop
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x1000, 0x7000);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x0000, 0x8000);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x8000, 0x8000);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x0000, 0x8000);
            }
            #endif

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x0000, 0x7000);
        }
        break;
    }
}

void Hal_SC_ControlMloadTrig(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(!MDrv_XC_Is_SupportSWDS(pInstance))
    {
        if (  (pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] == TRUE)
            &&(pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] == TRUE))
        {
            Hal_XC_MLoad_set_trig_p(pInstance, pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG], pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_DISP_AREA_TRIG]);
        }
        else if (  (pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] == TRUE)
                 &&(pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] == FALSE))
        {
            Hal_XC_MLoad_set_trig_p(pInstance, pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG], pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_DISP_AREA_TRIG]);
        }
        else if (  (pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] == FALSE)
                 &&(pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] == TRUE))
        {
            Hal_XC_MLoad_set_trig_p(pInstance, pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG], pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_DISP_AREA_TRIG]);
        }
        else
        {
            Hal_XC_MLoad_set_trig_p(pInstance, pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG], pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_DISP_AREA_TRIG]);
        }
        pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = FALSE;
        pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] = FALSE;

    }
}
void Hal_XC_MLoad_set_BitMask(void *pInstance,MS_BOOL enable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(enable)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_07_L, 0x8000, 0x8000);
    }else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_07_L, 0x0000, 0x8000);
    }
}
