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

#ifndef _HAL_PCMCIA_C_
#define _HAL_PCMCIA_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvPCMCIA.h"

// Internal Definition
#include "regCHIP.h"
#include "regPCMCIA.h"
#include "halPCMCIA.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// Address bus of RIU is 16 bits.
#define PCMCIA_READ_WORD(addr)         READ_WORD(PCMCIA_BASE_ADDRESS + ((addr)<<2))
#define PCMCIA_WRITE_WORD(addr, val)   WRITE_WORD((PCMCIA_BASE_ADDRESS + ((addr)<<2)), (val))

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_VIRT u32PCMCIA_RIU_BaseAdd;
MS_U16  _u16PCM_PE[3] = { 0, 0, 0};
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
// Put this function here because hwreg_utility2 only for hal.
void HAL_PCMCIA_Set_RIU_base(MS_VIRT ptrRIU_base)
{
    u32PCMCIA_RIU_BaseAdd = ptrRIU_base;
}

void HAL_PCMCIA_Write_Byte(MS_VIRT ptrAddr, MS_U8 u8Val)
{
    WRITE_BYTE(PCMCIA_BASE_ADDRESS+((ptrAddr<<1)-(ptrAddr&0x01)), u8Val);
}

MS_U8 HAL_PCMCIA_Read_Byte(MS_VIRT ptrAddr)
{
    return READ_BYTE(PCMCIA_BASE_ADDRESS+((ptrAddr<<1)-(ptrAddr&0x01)));    
}

MS_BOOL HAL_PCMCIA_GetIntStatus(ISR_STS* isr_status)
{
    MS_U8 u8Tmp = 0;
    u8Tmp = HAL_PCMCIA_Read_Byte(REG_PCMCIA_STAT_INT_RAW_INT1);
    if(u8Tmp & 0x01)
    {
        isr_status->bCardAInsert = TRUE;
    }
    if(u8Tmp & 0x02)
    {
        isr_status->bCardARemove = TRUE;
    }
    if(u8Tmp & 0x04)
    {
        isr_status->bCardAData = TRUE;
    }
    if(u8Tmp & 0x08)
    {
        isr_status->bCardBInsert = TRUE;
    }
    if(u8Tmp & 0x10)
    {
        isr_status->bCardBRemove = TRUE;
    }
    if(u8Tmp & 0x20)
    {
        isr_status->bCardBData = TRUE;
    }        
    
    
    return TRUE;
}

void   HAL_PCMCIA_ClrInt(MS_U32 bits)
{
    MS_U8 u8Tmp = 0;
    u8Tmp = (0x1 << 0) | // cardA insert
            (0x1 << 1) | // cardA remove
            (0x1 << 2) | // IRQ from cardA
            (0x1 << 3) | // cardB insert
            (0x1 << 4) | // cardB remove
            (0x1 << 5);  // IRQ from cardB
    
    HAL_PCMCIA_Write_Byte(REG_PCMCIA_INT_MASK_CLEAR1, u8Tmp);        
    u8Tmp = 0;
    HAL_PCMCIA_Write_Byte(REG_PCMCIA_INT_MASK_CLEAR1, u8Tmp);            
}

void   HAL_PCMCIA_MaskInt(MS_U32 bits, MS_BOOL bMask)
{
    MS_U8 u8Tmp = 0;
    
    if(bMask == TRUE)
    {
        u8Tmp = (0x1 << 0) | // cardA insert
                (0x1 << 1) | // cardA remove
                (0x1 << 2) | // IRQ from cardA
                (0x1 << 3) | // cardB insert            
                (0x1 << 4) | // cardB remove
                (0x1 << 5);  // IRQ from cardB
    }
    else // unmask
    {
        u8Tmp = 0x0;
    }
    HAL_PCMCIA_Write_Byte(REG_PCMCIA_INT_MASK_CLEAR, u8Tmp);        
}

void   HAL_PCMCIA_ClkCtrl(MS_BOOL bEnable)
{
    if(bEnable)
    {
        PCM_CLKGEN0_REG(REG_CLKGEN0_PCM_CLK) &= ~REG_CLKGEN0_PCM_CLK_MASK;

        _u16PCM_PE[0] = PCM_TOP_REG(REG_TOP_PCM_PE_0);
        _u16PCM_PE[1] = PCM_TOP_REG(REG_TOP_PCM_PE_1);
        _u16PCM_PE[2] = PCM_TOP_REG(REG_TOP_PCM_PE_2);
        PCM_TOP_REG(REG_TOP_PCM_PE_0) |= REG_TOP_PCM_PE_0_MASK;
        PCM_TOP_REG(REG_TOP_PCM_PE_1) |= REG_TOP_PCM_PE_1_MASK;
        PCM_TOP_REG(REG_TOP_PCM_PE_2) |= REG_TOP_PCM_PE_2_MASK;
    }
    else
    {
        PCM_TOP_REG(REG_TOP_PCM_PE_0) = _u16PCM_PE[0];
        PCM_TOP_REG(REG_TOP_PCM_PE_1) = _u16PCM_PE[1];
        PCM_TOP_REG(REG_TOP_PCM_PE_2) = _u16PCM_PE[2];

        PCM_CLKGEN0_REG(REG_CLKGEN0_PCM_CLK) |= REG_CLKGEN0_PCM_CLK_DIS;
    }                
}

#endif
