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
////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halFQ.c
// @brief  FQ HAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "regFQ.h"
#include "halFQ.h"
#include "halCHIP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT          _virtRegBase                        = 0;
static MS_U32           _dramRASPBase                       = 0;
#define _RASP_DRAM_BASE_128MB_256MB  (0x08000000)
#define _RASP_DRAM_BASE_0MB_128MB    (0x0)
#define _RASP_BASE_SET(addr)         ((addr)|(_dramRASPBase))
#define _RASP_BASE_CLR(addr)         ((addr)&(~_dramRASPBase))

REG_FIQ*               _REGFIQ    = NULL;

static MS_PHY          _phyFQMiuOffset[FQ_NUM] = {[0 ... (FQ_NUM-1)] = 0UL};

#ifdef MSOS_TYPE_LINUX_KERNEL
static MS_U16         _u16FQRegArray[1][0x11];
static MS_U16         _u16FQCfgReg[1];
#endif

// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define FQ32_W(reg, value);    { (reg)->L = ((value) & 0x0000FFFF);                          \
                                  (reg)->H = ((value) >> 16);}
#define FQ16_W(reg, value);    {(reg)->data = ((value) & 0x0000FFFF);}

#define TSP5_REG(addr)           (*((volatile MS_U16*)(_virtRegBase + 0xC7600UL + ((addr)<<2UL))))
    #define REG_TSP5_FIQ_MUX                  0x0BUL
        #define REG_TSP5_FIQ_MUX_MASK                   0x0007UL
        #define REG_TSP5_FIQ_SRC_PATH0                  0x0000UL
        #define REG_TSP5_FIQ_SRC_PATHFI                 0x0001UL
        #define REG_TSP5_FIQ_SRC_PATH1                  0x0002UL
        #define REG_TSP5_FIQ_SRC_PATH2                  0x0003UL

#define FIQ_REG(addr)              (*((volatile MS_U16*)(_virtRegBase + FQ_REG_CTRL_BASE + ((addr)<<2UL))))


//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
/*static MS_U32 _HAL_REG32_R(REG32_FQ *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}*/

static MS_U16 _HAL_REG16_R(REG16_FQ *reg)
{
    MS_U16     value;
    value = (reg)->data;
    return value;
}

static MS_PHY _HAL_FQ_MIU_OFFSET(MS_PHY Phyaddr)
{
    #ifdef HAL_MIU2_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU2_BASE)
        return ((MS_PHY)HAL_MIU2_BASE & 0xFFFFFFFFUL);
    else
    #endif  //HAL_MIU2_BASE
    #ifdef HAL_MIU1_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU1_BASE)
        return ((MS_PHY)HAL_MIU1_BASE & 0xFFFFFFFFUL);
    else
    #endif //HAL_MIU1_BUS_BASE
        return ((MS_PHY)HAL_MIU0_BASE & 0xFFFFFFFFUL);
}

//--------------------------------------------------------------------------------------------------
// For MISC part
//--------------------------------------------------------------------------------------------------
MS_BOOL HAL_FQ_SetBank(MS_VIRT virtBankAddr)
{
    _virtRegBase                 = virtBankAddr;
    _REGFIQ = (REG_FIQ*)(_virtRegBase + FQ_REG_CTRL_BASE);

    return TRUE;
}

//for K1 ECO U04 switch RASP dram base from 0-128MB to 128-256MB
//This function will be called by HAL_TSP_HWPatch() in halTSP.c of K1
MS_BOOL HAL_FQ_SetDramBase(MS_U32 dramBase)
{
    if(dramBase == _RASP_DRAM_BASE_0MB_128MB)
    {
        _dramRASPBase = dramBase;
        return TRUE;
    }
    if(dramBase == _RASP_DRAM_BASE_128MB_256MB)
    {
        _dramRASPBase = dramBase;
        return TRUE;
    }
    else
    {
        _dramRASPBase = 0;
        return FALSE;
    }
}

void HAL_FQ_PVR_SetBuf(MS_U32 u32FQEng, MS_PHY phyStartAddr, MS_U32 u32BufSize)
{
    MS_PHY phyEndAddr = phyStartAddr + u32BufSize;

    _phyFQMiuOffset[u32FQEng] = _HAL_FQ_MIU_OFFSET(phyStartAddr);

    FQ32_W(&(_REGFIQ[u32FQEng].str2mi_head), MIU_FQ((MS_U32)(phyStartAddr-_phyFQMiuOffset[u32FQEng])) & FIQ_STR2MI2_ADDR_MASK);
    FQ32_W(&(_REGFIQ[u32FQEng].str2mi_tail), MIU_FQ((MS_U32)(phyEndAddr-_phyFQMiuOffset[u32FQEng])) & FIQ_STR2MI2_ADDR_MASK);
    FQ32_W(&(_REGFIQ[u32FQEng].str2mi_mid), MIU_FQ((MS_U32)(phyStartAddr-_phyFQMiuOffset[u32FQEng])) & FIQ_STR2MI2_ADDR_MASK);
}

void HAL_FQ_PVR_SetRushAddr(MS_U32 u32FQEng, MS_PHY phyRushAddr)
{
    _phyFQMiuOffset[u32FQEng] = _HAL_FQ_MIU_OFFSET(phyRushAddr);
    FQ32_W(&(_REGFIQ[u32FQEng].rush_addr), MIU_FQ((MS_U32)(phyRushAddr-_phyFQMiuOffset[u32FQEng])) & FIQ_STR2MI2_ADDR_MASK);
}

void _HAL_FQ_PVR_Reset(MS_U32 u32FQEng, MS_BOOL bReset)
{
	if(bReset)
	{
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_RESET_WR_PTR));
	}
	else
	{
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_RESET_WR_PTR));
	}
}

void HAL_FQ_PVR_Start(MS_U32 u32FQEng)
{
    //reset write address
	_HAL_FQ_PVR_Reset(u32FQEng, TRUE);
	_HAL_FQ_PVR_Reset(u32FQEng, FALSE);

    //enable string to miu
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_PVR_ENABLE));
}

void HAL_FQ_PVR_Stop(MS_U32 u32FQEng)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_PVR_ENABLE));
}

void HAL_FQ_Rush_Enable(MS_U32 u32FQEng)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_RUSH_ENABLE));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_RUSH_ENABLE));
}

void HAL_FQ_Bypass(MS_U32 u32FQEng, MS_U8 u8Bypass)
{
    if(u8Bypass)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config11), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config11)), FIQ_CFG11_FIQ_BYPASS));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config11), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config11)), FIQ_CFG11_FIQ_BYPASS));
    }
}

void HAL_FQ_SWReset(MS_U32 u32FQEng, MS_U8 u8Reset)
{
    if(u8Reset)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_SW_RSTZ));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_SW_RSTZ));
        HAL_FQ_SetMuxSwitch(0, REG_TSP5_FIQ_SRC_PATH0);
    }
}

void HAL_FQ_AddrMode(MS_U32 u32FQEng, MS_U8 u8AddrMode)
{
    if(u8AddrMode)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_ADDR_MODE));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_ADDR_MODE));
    }
}
/*
#define MIU_BUS                     4
MS_U32 HAL_FQ_GetRead(MS_U32 u32FQEng)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_LOAD_WR_PTR));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_LOAD_WR_PTR));
    return REG32_R(&(_REGFIQ[u32FQEng].Fiq2mi2_radr_r)) << MIU_BUS;
}

MS_U32 HAL_FQ_GetWrite(MS_U32 u32FQEng)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_LOAD_WR_PTR));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_LOAD_WR_PTR));
    return REG32_R(&(_REGFIQ[u32FQEng].str2mi2_wadr_r)) << MIU_BUS;
}

MS_U32 HAL_FQ_GetPktAddrOffset(MS_U32 u32FQEng)
{
    return REG32_R(&(_REGFIQ[u32FQEng].pkt_addr_offset)) << MIU_BUS;
}
*/

void HAL_FQ_SkipRushData(MS_U32 u32FQEng, MS_U32 u32SkipPath)
{
    MS_U16 data = 0;

    if(u32SkipPath & HAL_FQ_SKIP_CFG1_MASK)
    {
        data = (MS_U16)(u32SkipPath & ~HAL_FQ_SKIP_CFG1_MASK);
        FQ16_W(&(_REGFIQ[1].Reg_fiq_config11),
            (_HAL_REG16_R(&(_REGFIQ[1].Reg_fiq_config11)) & ~FIQ_CFG11_SKIP_RUSH_DATA_PATH1_MASK) | (data & FIQ_CFG11_SKIP_RUSH_DATA_PATH1_MASK));
    }
    else
    {
        data = (MS_U16)(u32SkipPath);
        FQ16_W(&(_REGFIQ[0].Reg_fiq_config11),
            (_HAL_REG16_R(&(_REGFIQ[0].Reg_fiq_config11)) & ~FIQ_CFG11_SKIP_RUSH_DATA_PATH_MASK) | (data & FIQ_CFG11_SKIP_RUSH_DATA_PATH_MASK));
    }

}

#if 0
void HAL_FQ_INT_Enable(MS_U32 u32FQEng, MS_U16 u16Mask)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config16),  _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config16)),  u16Mask & FIQ_CFG16_INT_ENABLE_MASK));
}

void HAL_FQ_INT_Disable(MS_U32 u32FQEng, MS_U16 u16Mask)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config16), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config16)), u16Mask & FIQ_CFG16_INT_ENABLE_MASK));
}

MS_U16 HAL_FQ_INT_GetHW(MS_U32 u32FQEng)
{
    return _HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config16)) & FIQ_CFG16_INT_STATUS_MASK;
}

void HAL_FQ_INT_ClrHW(MS_U32 u32FQEng, MS_U16 u16Mask)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config16), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config16)), u16Mask & FIQ_CFG16_INT_STATUS_MASK));
}
#endif

MS_U32 HAL_FQ_GetPVRTimeStamp(MS_U32 u32FQEng)
{
    //not inplemented
    return 0;
}

void HAL_FQ_SetPVRTimeStamp(MS_U32 u32FQEng , MS_U32 u32Stamp)
{
    //not inplemented
}

MS_BOOL HAL_FQ_SetMuxSwitch(MS_U32 u32FQEng, MS_U32 u32FQSrc)
{
    MS_U16 u16Src;

    if(u32FQEng != 0)
        return FALSE;

    switch(u32FQSrc)
    {
        case 0:
            u16Src = REG_TSP5_FIQ_SRC_PATH0;
            break;
        case 1:
            u16Src = REG_TSP5_FIQ_SRC_PATH1;
            break;
        case 2:
            u16Src = REG_TSP5_FIQ_SRC_PATH2;
            break;
        case 7:
            u16Src = REG_TSP5_FIQ_SRC_PATHFI;
            break;
        default:
            return FALSE;
    }

    TSP5_REG(REG_TSP5_FIQ_MUX) = (TSP5_REG(REG_TSP5_FIQ_MUX) & ~REG_TSP5_FIQ_MUX_MASK) | u16Src;

    return TRUE;
}

MS_U32 HAL_FQ_GetMuxSwitch(MS_U32 u32FQEng)
{
    MS_U16 u16Src;
    MS_U32 u32Ret;

    if(u32FQEng != 0)
        return 0xFFUL;

    u16Src = TSP5_REG(REG_TSP5_FIQ_MUX) & REG_TSP5_FIQ_MUX_MASK;

    switch(u16Src)
    {
        case REG_TSP5_FIQ_SRC_PATH0:
            u32Ret = 0;
            break;
        case REG_TSP5_FIQ_SRC_PATH1:
            u32Ret = 1;
            break;
        case REG_TSP5_FIQ_SRC_PATH2:
            u32Ret = 2;
            break;
        case REG_TSP5_FIQ_SRC_PATHFI:
            u32Ret = 7;
            break;
        default:
            u32Ret = 0xFF;
            break;
    }
    return u32Ret;
}

#ifdef MSOS_TYPE_LINUX_KERNEL

MS_BOOL HAL_FQ_SaveRegs(void)
{
    MS_U32 u32ii = 0;

    for(u32ii = 0; u32ii <= 0x10; u32ii++)
    {
        _u16FQRegArray[0][u32ii] = FIQ_REG(u32ii);
    }

    _u16FQCfgReg[0] = TSP5_REG(REG_TSP5_FIQ_MUX);

    //stop rush data
    if((_HAL_REG16_R(&(_REGFIQ[0].Reg_fiq_config0)) & FIQ_CFG0_RUSH_ENABLE) == 0)
    {
        FQ16_W(&(_REGFIQ[0].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[0].Reg_fiq_config0)), FIQ_CFG0_RUSH_ENABLE));
    }
    //stop pvr
    if(_HAL_REG16_R(&(_REGFIQ[0].Reg_fiq_config0)) & FIQ_CFG0_PVR_ENABLE)
    {
        FQ16_W(&(_REGFIQ[0].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[0].Reg_fiq_config0)), FIQ_CFG0_PVR_ENABLE));
    }

    HAL_FQ_SWReset(0, TRUE);

    return TRUE;
}

MS_BOOL HAL_FQ_RestoreRegs(void)
{
    MS_U32 u32ii = 0;

    HAL_FQ_SWReset(0, FALSE);

    TSP5_REG(REG_TSP5_FIQ_MUX) = _u16FQCfgReg[0];

    FIQ_REG(0)= (_u16FQRegArray[0][0] | FIQ_CFG0_RUSH_ENABLE) & ~FIQ_CFG0_PVR_ENABLE;
    for(u32ii = 1; u32ii <= 0x10; u32ii++)
    {
        FIQ_REG(u32ii)= _u16FQRegArray[0][u32ii];
    }

    // clear dirty data
    _HAL_FQ_PVR_Reset(0, TRUE);
    _HAL_FQ_PVR_Reset(0, FALSE);

    if(_u16FQRegArray[0][0] & FIQ_CFG0_PVR_ENABLE)
    {
        FQ16_W(&(_REGFIQ[0].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[0].Reg_fiq_config0)), FIQ_CFG0_PVR_ENABLE));
    }

    return TRUE;
}

#endif  //MSOS_TYPE_LINUX_KERNEL

