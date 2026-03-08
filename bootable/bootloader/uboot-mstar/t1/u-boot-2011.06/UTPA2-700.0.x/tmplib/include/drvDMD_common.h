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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvDMD_common.h
/// @brief  DVBC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Demod Demod interface


    Peripheral functions are controlled by each DEMOD standard

    <b>Features</b>

    - Registers read/write.
    - I2C, RF/IF, TS and SSPI control.

    <b>ATSC Interface brief</b>
    ATSC is the demodulator of supporting 8VSB and J83B standard.
    Receive RF signal from Tuner and transmit demodulated data to TS module.

    <b>Features</b>

    - Support 8VSB and J83B standard.
    - Pass A74 SPEC.
    - Pass the field stream.

    <b> ATSC Block Diagram: </b> \n
    \image html drvDMD_ATSC_pic01.png

    <b> Operation Code Flow: </b> \n
    -# Initialize ATSC parameters and load DEMOD FW
    -# Enable DEMOD state machine
    -# Monitor lock status

    <b>DTMB interface brief</b>
    DTMB is the demodulator of supporting DTMB(6/7/8/9M) standard.
    Receive RF signal from Tuner and transmit demodulated data to TS module.

    <b>Features</b>

    - Support DTMB(6/7/8/9M) standard.
    - Pass CHINA SPEC.
    - Pass the field stream.

    <b> DTMB Block Diagram: </b> \n
    \image html drvDMD_DTMB_pic01.png

    <b> Operation Code Flow: </b> \n
    -# Initialize DTMB parameters and load DEMOD FW
    -# Enable DEMOD state machine
    -# Monitor lock status

 *! \defgroup COMMON COMMON interface (drvDMD_common.h)
 *  \ingroup Demod

 *! \defgroup COMMON_BASIC COMMON basic control
 *  \ingroup COMMON
 */

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

typedef struct _s_I2C_Interface_func
{
    MS_BOOL (*I2C_WriteBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
    MS_BOOL (*I2C_ReadBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
}s_I2C_Interface_func;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Register DEMOD bank address
/// @ingroup COMMON_BASIC
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_PreInit(void);
//-------------------------------------------------------------------------------------------------
/// Enable RF AGC Tristate
/// @ingroup COMMON_BASIC
/// @param bEnable    \b IN: Enable as TRUE
/// @return : NULL
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC void MDrv_DMD_RFAGC_Tristate(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Enable IF AGC Tristate
/// @ingroup COMMON_BASIC
/// @param bEnable    \b IN: Enable as TRUE
/// @return : NULL
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC void MDrv_DMD_IFAGC_Tristate(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Get TS clock rate
/// @ingroup COMMON_BASIC
/// @param fTS_CLK    \b OUT: the pointer to TS clock rate
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_TS_GetClockRate(float *fTS_CLK);
//-------------------------------------------------------------------------------------------------
/// ts output clock frequency and phase configure
/// @ingroup COMMON_BASIC
/// @param u8cmd_array    \b IN: the pointer to TS clock configuration
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array);
//-------------------------------------------------------------------------------------------------
/// read register data
/// @ingroup COMMON_BASIC
/// @param u32Reg    \b IN: register address
/// @param u8Value    \b OUT: the pointer to register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value);
//-------------------------------------------------------------------------------------------------
/// write register data
/// @ingroup COMMON_BASIC
/// @param u32Reg    \b IN: register address
/// @param u8Value    \b IN: register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_WriteReg(MS_U32 u32Reg, MS_U8 u8Value);
//-------------------------------------------------------------------------------------------------
/// write register data for more than one byte
/// @ingroup COMMON_BASIC
/// @param u32Reg    \b IN: register address
/// @param u8Value    \b IN: the pointer to data
/// @param u8Length    \b IN: length of data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_WriteRegs(MS_U32 u32Reg, MS_U8 *u8Value, MS_U8 u8Length);
//-------------------------------------------------------------------------------------------------
/// Change I2C channel
/// @ingroup COMMON_BASIC
/// @param ch_num    \b IN: I2C channel number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_I2C_Channel_Change(MS_U8 ch_num);
//-------------------------------------------------------------------------------------------------
/// Set I2C channel
/// @ingroup COMMON_BASIC
/// @param ch_num    \b IN: I2C channel number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_I2C_Channel_Set(MS_U8  ch_num);
//-------------------------------------------------------------------------------------------------
/// Initialize SSPI interface
/// @ingroup COMMON_BASIC
/// @param u8DeviceNum    \b IN: SSPI device number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_Init(MS_U8  u8DeviceNum);
//-------------------------------------------------------------------------------------------------
/// MIU write data more than one byte per SSPI interface
/// @ingroup COMMON_BASIC
/// @param u32Addr    \b IN: register address
/// @param pdata    \b IN: the pointer to data
/// @param u16Size    \b IN: length of data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
//-------------------------------------------------------------------------------------------------
/// MIU read data more than one byte per SSPI interface
/// @ingroup COMMON_BASIC
/// @param u32Addr    \b IN: register address
/// @param pdata    \b IN: the pointer to data
/// @param u16Size    \b IN: length of data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
//-------------------------------------------------------------------------------------------------
/// MIU write data per SSPI interface
/// @ingroup COMMON_BASIC
/// @param u16Addr    \b IN: register address
/// @param data    \b IN: register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Write8(MS_U16 u16Addr, MS_U8 data);
//-------------------------------------------------------------------------------------------------
/// MIU read data per SSPI interface
/// @ingroup COMMON_BASIC
/// @param u16Addr    \b IN: register address
/// @param pdata    \b out: register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata);

#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBC_H_

