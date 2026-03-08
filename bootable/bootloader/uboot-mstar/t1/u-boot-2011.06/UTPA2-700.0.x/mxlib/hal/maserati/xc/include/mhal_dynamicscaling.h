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
#ifndef MHAL_DYNAMICSCALING_H
#define MHAL_DYNAMICSCALING_H

#ifdef MHAL_DYNAMICSCALING_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DS_OP_CMD_LEN 4  //byte
#define MS_DS_NULL_CMD_32BITS    0xFFFF0000;         //Dummy cmd BKFF_FF=0x0000
#define MS_DS_NULL_CMD_64BITS    0x00000009FFFF0000; //Dummy cmd BKFF_7F=0x0000
//----------------------------------------------
//  BKFF_7F just for dummy ds cmd
//----------------------------------------------
#define REG_SC_BKFF_7F_L      _PK_L_(0xFF, 0x7F)
#define REG_SC_BKFF_7F_H      _PK_H_(0xFF, 0x7F)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    DS_IP,
    DS_OP,
} ds_reg_ip_op_sel;

typedef enum
{
    DS_XC,
    DS_MVOP,
    DS_GOP,
} ds_reg_source_sel;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------


INTERFACE MS_BOOL MHAL_SC_Set_DynamicScaling(void *pInstance,MS_PHY u32MemBaseAddr, MS_U8 u8MIU_Select, MS_U8 u8IdxDepth, MS_BOOL bOP_On, MS_BOOL bIPS_On, MS_BOOL bIPM_On,SCALER_WIN eWindow);
INTERFACE void MHAL_SC_Set_DynamicScalingFlag(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_BOOL MHAL_SC_Get_DynamicScaling_Status(void *pInstance);
INTERFACE MS_BOOL MHAL_SC_Enable_IPMTuneAfterDS(void *pInstance, MS_BOOL bEnable);
INTERFACE void MHal_SC_DynamicScaling_SWReset(void);
INTERFACE void Hal_SC_ResetSWDSCommand(void *pInstance,SCALER_WIN eWindow, MS_U8 u8DSIndex);
INTERFACE void Hal_SC_WriteSWDSCommand(void *pInstance,SCALER_WIN eWindow, MS_U32 u32CmdRegAddr, MS_U16 u16CmdRegValue,ds_reg_ip_op_sel IPOP_Sel,ds_reg_source_sel Source_Select,XC_DS_CMDCNT *pstXC_DS_CmdCnt);
INTERFACE void Hal_SC_WriteSWDSCommand_NonXC(void *pInstance,SCALER_WIN eWindow,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask,ds_reg_ip_op_sel IPOP_Sel,XC_DS_CMDCNT *pstXC_DS_CmdCnt);
INTERFACE void Hal_SC_Add_NullCommand(void *pInstance,SCALER_WIN eWindow,ds_reg_ip_op_sel IPOP_Sel,XC_DS_CMDCNT *pstXC_DS_CmdCnt);

#define Hal_SC_SetSeamlessZapping(args...) 0
#define Hal_SC_GetSeamlessZappingStatus(args...) 0
#define Hal_SC_PrepareSeamlessZapping(args...)
INTERFACE MS_BOOL MHal_XC_Get_DSForceIndexSupported(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MHal_XC_Set_DSIndexSourceSelect(void *pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow);
INTERFACE void MHal_XC_Set_DSForceIndex(void *pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow);
INTERFACE void Hal_XC_Enable_DS_64Bits_Command(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_XC_Enable_DS_64Bits_Sread_Mode(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_XC_Enable_DS_4_Baseaddress_Mode(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_XC_Set_DS_64Bits_MIU_Bus_Sel(void *pInstance);
INTERFACE void Hal_XC_Set_DS_BaseAddress(void *pInstance,MS_PHY u32Base);
INTERFACE void Hal_XC_Set_DS_IndexDepth(void *pInstance);
INTERFACE void Hal_XC_Set_DS_MIU_Sel(void *pInstance,MS_U16 u16DSOnOff,MS_U16 u16DS_MIU_Sel_bit1);
INTERFACE MS_U64 Hal_SC_GenSpreadModeCmd(void *pInstance, MS_U32 u32CmdRegAddr, MS_U16 u16CmdRegValue, MS_U16 u16Mask, ds_reg_source_sel eSourceSelect);
#undef INTERFACE
#endif //MHAL_DYNAMICSCALING_H

