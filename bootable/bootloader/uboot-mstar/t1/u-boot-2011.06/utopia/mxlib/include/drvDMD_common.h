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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvDMD_common.h
/// @brief  DVBC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DMD_COMMON_H_
#define _DRV_DMD_COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define USE_UTOPIA2P0

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    _QPSK        = 0x0,
    _16QAM        = 0x1,
    _64QAM        = 0x2,
    _UNKNOW_QAM   = 0xff,    
}DMD_CONSTEL;

typedef enum
{
    _CR1Y2        = 0x0,
    _CR2Y3        = 0x1,
    _CR3Y4        = 0x2,
    _CR5Y6        = 0x3,
    _CR7Y8        = 0x4,
    _UNKNOW_CR   = 0xff,    
}DMD_CODERATE;


//bryan temp mark
#if(1)
typedef struct
{
    float   power_db;
    MS_U8   sar3_val;
}DMD_RFAGC_SSI;

typedef struct
{
    float   power_db;
    MS_U8   agc_val;
}DMD_IFAGC_SSI;

typedef struct
{
    float   attn_db;
    MS_U8   agc_err;
}DMD_IFAGC_ERR;

typedef struct
{
    DMD_CONSTEL         constel;
    DMD_CODERATE        code_rate;
    float               p_ref;
}DMD_SSI_DBM_NORDIGP1;

typedef struct
{
    DMD_CONSTEL   constel;
    DMD_CODERATE   code_rate;
    float   cn_ref;
}DMD_SQI_CN_NORDIGP1;

typedef struct
{
    DMD_RFAGC_SSI *pRfagcSsi;
    MS_U16 u16RfagcSsi_Size;
    DMD_IFAGC_SSI *pIfagcSsi_LoRef; 
    MS_U16 u16IfagcSsi_LoRef_Size;    
    DMD_IFAGC_SSI *pIfagcSsi_HiRef;   
    MS_U16 u16IfagcSsi_HiRef_Size;    
    DMD_IFAGC_ERR *pIfagcErr_LoRef;  
    MS_U16 u16IfagcErr_LoRef_Size;    
    DMD_IFAGC_ERR *pIfagcErr_HiRef;    
    MS_U16 u16IfagcErr_HiRef_Size;    
}DMD_SSI_TABLE;
#endif
typedef struct _s_I2C_Interface_func
{
    MS_BOOL (*I2C_WriteBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
    MS_BOOL (*I2C_ReadBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
}s_I2C_Interface_func;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_PreInit(void);
DLL_PUBLIC void MDrv_DMD_RFAGC_Tristate(MS_BOOL bEnable);
DLL_PUBLIC void MDrv_DMD_IFAGC_Tristate(MS_BOOL bEnable);

/************************************************************************************************
  Subject:    MDrv_DMD_TSO_Clk_Control
  Function:   ts output clock frequency and phase configure
  Parmeter:   u8cmd_array, clock div,           0x01, div (0x00~0x1f), 
                           clock phase inv,     0x02, inv_en (0,1),
                           clock phase tuning,  0x03, phase_tuning_en (0,1), tuning_num (0x00~0x1f)
  Return:     MS_BOOL     
  Remark:
*************************************************************************************************/
DLL_PUBLIC MS_BOOL MDrv_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array);
DLL_PUBLIC MS_BOOL MDrv_DMD_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value);
DLL_PUBLIC MS_BOOL MDrv_DMD_WriteReg(MS_U32 u32Reg, MS_U8 u8Value);
DLL_PUBLIC MS_BOOL MDrv_DMD_WriteRegs(MS_U32 u32Reg, MS_U8 *u8Value, MS_U8 u8Length);
DLL_PUBLIC MS_BOOL MDrv_DMD_I2C_Channel_Change(MS_U8 ch_num);
DLL_PUBLIC MS_BOOL MDrv_DMD_I2C_Channel_Set(MS_U8  ch_num);

DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_Init(MS_U8  u8DeviceNum);
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Write8(MS_U16 u16Addr, MS_U8 data);
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata);
#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBC_H_

