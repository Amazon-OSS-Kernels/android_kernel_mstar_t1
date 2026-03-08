/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "compiler.h"
#include "hwreg.h"
#include "datatype.h"
#include "drvGlobal.h"

#define ASSERT(expr)

const char g_str_assert[] = "ASSERT: %s %d\n";

U8 __MDrv_ReadByte( U32 u32Reg )
{
    return RIU8[(u32Reg << 1) - (u32Reg & 1)];
}

U16 __MDrv_Read2Byte( U32 u32Reg )
{
    if (__unlikely(u32Reg & 0x01))
    {
        u32Reg <<= 1;
        return RIU8[u32Reg - 1] + (RIU8[u32Reg + 2] << 8);
    }
    else
    {
        return RIU[u32Reg];
    }
}

U32 __MDrv_Read3Byte( U32 u32Reg )
{
    if (__unlikely(u32Reg & 0x01))
    {
        return (RIU8[(u32Reg << 1) - 1] + (RIU[u32Reg + 1] << 8));
    }
    else
    {
        return (RIU[u32Reg] + (RIU8[(u32Reg + 2) << 1] << 16));
    }
}

U32 __MDrv_Read4Byte( U32 u32Reg )
{
    if (__unlikely(u32Reg & 0x01))
    {
        return (RIU8[(u32Reg << 1) - 1] + (RIU[u32Reg + 1] << 8) + (RIU8[(u32Reg + 3) << 1] << 24));
    }
    else
    {
        return (RIU[u32Reg] + (RIU[u32Reg + 2] << 16));
    }
}

void __MDrv_WriteByte( U32 u32Reg, U8 u8Value )
{
    RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Value;
}

void __MDrv_Write2Byte( U32 u32Reg, U16 u16Value )
{
    if (__unlikely(u32Reg & 0x01))
    {
        RIU8[(u32Reg << 1) - 1] = (U8)((u16Value));
        RIU8[(u32Reg + 1) << 1] = (U8)((u16Value) >> 8);
    }
    else
    {
        RIU[u32Reg] = u16Value;
    }
}

void __MDrv_Write3Byte( U32 u32Reg, U32 u32Value )
{
    if (__unlikely(u32Reg & 0x01))
    {
        RIU8[(u32Reg << 1) - 1] = u32Value;
        RIU[u32Reg + 1] = (u32Value >> 8);
    }
    else
    {
        RIU[u32Reg] = u32Value;
        RIU8[(u32Reg + 2) << 1] = (u32Value >> 16);
    }
}

void __MDrv_Write4Byte( U32 u32Reg, U32 u32Value )
{
    if (__unlikely(u32Reg & 0x01))
    {
        RIU8[(u32Reg << 1) - 1] = u32Value;
        RIU[u32Reg + 1] = (u32Value >> 8);
        RIU8[((u32Reg + 3) << 1)] = (u32Value >> 24);
    }
    else
    {
        RIU[u32Reg] = u32Value;
        RIU[u32Reg + 2] = (u32Value >> 16);
    }
}

void MDrv_WriteRegTbl( const MS_REG_INIT * pRegTable )
{
    U8 u8Length;
    U16 u16Index; // register index
    U16 u16Dummy;

    u16Dummy = 2000;

    do
    {
        u16Index = ((pRegTable[0] << 8) + pRegTable[1]);
        if (u16Index == 0xFFFF) // check end of table
            break;

        u8Length = u16Index >> 14;
        u16Index &= 0x3FFF;

        switch ( u8Length )
        {
        case 3:
            MDrv_Write4Byte( u16Index, pRegTable[2] +
                                      (pRegTable[3] << 8) +
                                      (pRegTable[4] << 16) +
                                      (pRegTable[5] << 24) );
            pRegTable += 6;
            break;

        case 2:
            MDrv_Write3Byte( u16Index, pRegTable[2] +
                                      (pRegTable[3] << 8) +
                                      (pRegTable[4] << 16) );
            pRegTable += 5;
            break;

        case 1:
            MDrv_Write2Byte( u16Index, pRegTable[2] +
                                      (pRegTable[3] << 8) );
            pRegTable += 4;
            break;

        case 0:
            MDrv_WriteByte( u16Index, pRegTable[2] );
            pRegTable += 3;
            break;
        default:
            ASSERT(0);
            break;
        }
    } while (--u16Dummy > 0);
}


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void MDrv_WriteRegTbl_32( const MS_REG_INIT * pRegTable )
{
    U8 u8Length;
    U32 u32Index; // register index
    U16 u16Dummy;

    u16Dummy = 2000;

    do
    {
        u32Index = (((U32)pRegTable[0] << 16) + ((U32)pRegTable[1] << 8) + pRegTable[2] );
        if (u32Index == 0xFFFFFF) // check end of table
            break;

        u8Length = pRegTable[0]>>5;  //u32Index >> 21;
        u32Index &= 0x1FFFFF;

        switch ( u8Length )
        {
        case 3:
            MDrv_Write4Byte( u32Index, pRegTable[3] +
                                      (pRegTable[4] << 8) +
                                      (pRegTable[5] << 16) +
                                      (pRegTable[6] << 24) );
            pRegTable += 7;
            break;

        case 2:
            MDrv_Write3Byte( u32Index, pRegTable[3] +
                                      (pRegTable[4] << 8) +
                                      (pRegTable[5] << 16) );
            pRegTable += 6;
            break;

        case 1:
            MDrv_Write2Byte( u32Index, pRegTable[3] +
                                      (pRegTable[4] << 8) );
            pRegTable += 5;
            break;

        case 0:
            MDrv_WriteByte( u32Index, pRegTable[3] );
            pRegTable += 4;
            break;
        default:
            ASSERT(0);
            break;
        }
    } while (--u16Dummy > 0);
}



