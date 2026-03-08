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

///// Table Utility

#define BW_REG(reg) ((reg>>8)&0xFF), (reg&0xFF)
#define REG(reg)    ((reg>>16)&0xFF), ((reg>>8)&0xFF), (reg&0xFF)

#define REG_BW_ADDR_SIZE   3
#define REG_BW_MASK_SIZE   1
#define REG_BW_BANK_SIZE   1

///// Table
#include "Maserati_Bandwidth_RegTable.h"
#include "Maserati_Bandwidth_RegTable.c"

///// Dumper
///// Different dumper for different table
static void _MDrv_BW_DumpTable(TAB_Info* ptab_Info)
{

    MS_U32 u32Addr;
    MS_U8 u8Bank;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8BankGOP_Bak;

    if (ptab_Info->pIPTable == NULL){
        return;
    }

    if (ptab_Info->u8TabIdx >= ptab_Info->u8TabNums){
        MS_ASSERT(0);
        return;
    }

    u8BankGOP_Bak = MApi_XC_ReadByte(REG_GOP_BANK) & 0x0F;

    while (1)
    {

        u32Addr = (ptab_Info->pIPTable[0]<<16) + (ptab_Info->pIPTable[1]<<8) + ptab_Info->pIPTable[2];
        u8Bank    = ptab_Info->pIPTable[3];
        u8Mask    = ptab_Info->pIPTable[4];
        u8Value = ptab_Info->pIPTable[REG_BW_ADDR_SIZE+REG_BW_BANK_SIZE+REG_BW_MASK_SIZE+ptab_Info->u8TabIdx];
        //printf("@@#@@[%s][%d]u32Addr=%x, u8Bank=%x, u8Mask=%x, u8Value=%x\n", __FUNCTION__, __LINE__, u32Addr, u8Bank, u8Mask, u8Value);

        if (u32Addr == _END_OF_BW_TBL_) // check end of table
            break;

	if (u32Addr == 0x000000) // check 0x000000 for error address
            break;

			
        if ((u32Addr >> 8 ) == 0x101F)     // GOP
        {
            if (( MApi_XC_ReadByte(REG_GOP_BANK) & 0x0F) != u8Bank)
            {
                MApi_XC_WriteByteMask(REG_GOP_BANK,u8Bank,0x0F);
            }
        }

        if ((u32Addr >> 8) == 0x102F)  // Scaler
        {
            if (u32Addr % 2) // Odd address
            {
                MApi_XC_W2BYTEMSK( ( (MS_U16)u8Bank << 8 ) | ( (u32Addr & 0x00FF) - 1 ) ,(MS_U16)u8Value << 8 ,(MS_U16)u8Mask << 8);
            }
            else
            {
                MApi_XC_W2BYTEMSK( ( (MS_U16)u8Bank << 8 ) | ( (u32Addr & 0x00FF) ) ,(MS_U16)u8Value ,(MS_U16)u8Mask );
            }
        }
        else //Others
        {
            MApi_XC_WriteByteMask( (MS_U32)u32Addr , u8Value, u8Mask);
        }

        //BW_DBG(printk("[addr=%06x, bank=%02x, msk=%04x, val=%04x]\n", u16Addr, u8Bank, u8Mask, u8Value));
        //printk(" [addr=%06x bank=%02x, msk=%04x, val=%04x]\n",(MS_U32)u32Addr , u8Bank, u8Mask, u8Value);

        ptab_Info->pIPTable+=(REG_BW_ADDR_SIZE+REG_BW_BANK_SIZE+REG_BW_MASK_SIZE+ptab_Info->u8TabNums); // next
    }

    MApi_XC_WriteByteMask(REG_GOP_BANK,u8BankGOP_Bak,0x0F);

}


static void _MDrv_BW_PushBuffer(MS_U8 u8TabIdx, MS_U8 *pu8Buffer)
{
    TAB_Info tab_Info;
    tab_Info.pIPTable  = (void*)BWTABLE;
    tab_Info.u8TabNums = BWTABLE_NUMS;
    tab_Info.u8TabIdx = u8TabIdx;
    MS_U32 u32Index = 0;

    if(pu8Buffer != NULL)
    {
        tab_Info.pIPTable = pu8Buffer;

        while(!(((pu8Buffer[u32Index]&0xFF) == 0xFF) && ((pu8Buffer[u32Index+1]&0xFF) == 0xFF) && ((pu8Buffer[u32Index+2]&0xFF) == 0xFF) && ((pu8Buffer[u32Index+3]&0xFF) == 0x00) && ((pu8Buffer[u32Index+4]&0xFF) == 0x00)))
        {
            u32Index++;
            //printf("@@#@@[%s][%d]u32Index=%u\n", __FUNCTION__, __LINE__,u32Index);
        }

        tab_Info.pIPTable = pu8Buffer +u32Index + 5;

        printf("Read Bandwidthtable.bin\n");
    }

    _MDrv_BW_DumpTable(&tab_Info);

}


