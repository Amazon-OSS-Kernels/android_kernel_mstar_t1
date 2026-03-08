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
////////////////////////////////////////////////////////////////////////////////
#ifndef _HAL_DMD_COMMON_H_
#define _HAL_DMD_COMMON_H_

#define MBRegBase   0x112600UL //Demod MailBox
#define DMD_CLK_GEN 0x103300
#define VDMcuBase   0x103400UL //DmdMCU51 (40-4F)
#define DMDMcuBase  0x103480UL
#define XDMIU_REG_BASE  0x1d00
#define BACKEND_REG_BASE  0x1f00
#define TOP_REG_BASE  0x2000
#define TDP_REG_BASE  0x2100
#define FDP_REG_BASE  0x2200
#define	ISDBT_FDPE_REG_BASE  0x1600
#define FEC_REG_BASE  0x2300
#define TDF_REG_BASE  0x2800
#define TDFE_REG_BASE  0x2900
#define TDFE2_REG_BASE  0x2a00
#define DTOP_REG_BASE  0x2e00
#define T2TDP_REG_BASE  0x3000
#define T2FDP_REG_BASE  0x3100
#define T2FEC_REG_BASE  0x3300
#define DVBTM_REG_BASE  0x3400
#define T2L1_REG_BASE  0x2b00
#define T2SNR_REG_BASE  0x2c00
#define T2DJB_REG_BASE  0x2d00

#define REG_CMD_CTRL    MBRegBase + 0x1C
#define REG_DTA_CTRL    MBRegBase + 0x1D
#define REG_CMD_ADDR    MBRegBase + 0x1E
#define REG_CMD_DATA    MBRegBase + 0x1F

#define _REG_START      REG_CMD_CTRL
#define _REG_END        REG_CMD_CTRL
#define _REG_DRQ        REG_DTA_CTRL
#define _REG_FSM        REG_CMD_CTRL
#define _REG_ERR        REG_DTA_CTRL

#define _BIT_START      ((BIT_(1)))
#define _BIT_END        ((BIT_(0)))
#define _BIT_DRQ        ((BIT_(0)))
#define _BIT_FSM        ((BIT_(3)))
#define _BIT_ERR        ((BIT_(7)))

typedef struct _s_MDRV_DMD_INTERFACE_FUNCTION
{
    MS_BOOL (*MDrv_DMD_WriteReg)(MS_U32 u32Reg, MS_U8 u8Value);
    MS_BOOL (*MDrv_DMD_ReadReg)(MS_U32 u32Reg, MS_U8 *u8Value);
    MS_BOOL (*MDrv_DMD_WriteRegs)(MS_U32 u32Reg, MS_U8 *u8Value, MS_U8 u8Length);
    MS_BOOL (*MDrv_DMD_I2C_Channel_Change)(MS_U8 ch_num);
    MS_BOOL (*MDrv_DMD_I2C_Channel_Set)(MS_U8  ch_num);
    float        (*Log10Approx)(float flt_x);
}s_MDRV_DMD_INTERFACE_FUNCTION;



MS_BOOL HAL_DMD_RegInit (void);
MS_U8 HAL_DMD_RIU_ReadByte(MS_U32 u32Addr);
MS_U8 HAL_DMD_RIU_ReadRegBit(MS_U32 u32Addr, MS_U8 u8Mask);
MS_U16 HAL_DMD_RIU_Read2Byte(MS_U32 u32Addr);
void HAL_DMD_RIU_WriteByte(MS_U32 u32Addr, MS_U8 u8Value);
void HAL_DMD_RIU_WriteRegBit(MS_U32 u32Addr, MS_BOOL bEnable, MS_U8 u8Mask);
void HAL_DMD_RIU_WriteByteMask(MS_U32 u32Addr, MS_U8 u8Value, MS_U8 u8Mask);
void HAL_DMD_RIU_Write2Byte(MS_U32 u32Addr, MS_U16 u16Value);

//waiting add
MS_BOOL HAL_DMD_IFAGC_RegRead(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err);
/*
MS_BOOL HAL_DMD_GetRFLevel(float *fRFPowerDbmResult, float fRFPowerDbm, MS_U8 u8SarValue,
                                                     DMD_RFAGC_SSI *pRfagcSsi, MS_U16 u16RfagcSsi_Size,
                                                     DMD_IFAGC_SSI *pIfagcSsi_HiRef, MS_U16 u16IfagcSsi_HiRef_Size,
                                                     DMD_IFAGC_SSI *pIfagcSsi_LoRef, MS_U16 u16IfagcSsi_LoRef_Size,
                                                     DMD_IFAGC_ERR *pIfagcErr_LoRef, MS_U16 u16IfagcErr_LoRef_Size,
                                                     DMD_IFAGC_ERR *pIfagcErr_HiRef, MS_U16 u16IfagcErr_HiRef_Size);
*/
void HAL_DMD_GetNordigSSI(float fPrel, MS_U16 *strength);
void HAL_DMD_IFAGC_TS_Tristate(MS_BOOL bEnable);
void HAL_DMD_TS1_Tristate(MS_BOOL bEnable);
MS_BOOL HAL_DMD_TS_GetClockRate(float *fTS_CLK);
void HAL_DMD_RFAGC_Tristate(MS_BOOL bEnable);
void HAL_DMD_IFAGC_Tristate(MS_BOOL bEnable);
MS_BOOL HAL_DMD_ADC_IQ_Switch(MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain);
MS_BOOL HAL_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array);
//float Log10Approx(float flt_x);
MS_U8 HAL_DMD_IIC_ReadByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *pu8Data);
MS_U8 HAL_DMD_IIC_WriteByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data);
MS_U8 HAL_DMD_IIC_WriteBytes(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *u8Data, MS_U8 u8Len);
MS_BOOL HAL_DMD_I2C_Channel_Change(MS_U16 u16SlaveAddr, MS_U8 ch_num);
MS_BOOL HAL_DMD_I2C_Channel_Set(MS_U16 u16SlaveAddr, MS_U8 ch_num);
#endif

