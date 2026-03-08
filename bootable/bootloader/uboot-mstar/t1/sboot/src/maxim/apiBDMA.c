/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#define RIU_MAP                         0x1F200000
#define LOCAL_BDMA_REG_BASE             0x900
#define RIU                             ((unsigned short volatile *) RIU_MAP)
#define RIU8                            ((unsigned char volatile *) RIU_MAP)
#define __loader_2nd
#define __loader_2nd_bss

typedef unsigned char                   U8;
typedef unsigned int                    U16;
typedef unsigned long                   U32;
#ifndef BOOL
typedef unsigned int                    BOOL;
#endif

void __BDMA_FlashCopy2MIU0(U32 FlashPA, U32 DRAMPA, U32 Len);
void __loader_2nd __BDMA_FlashCopy2MIU0(U32 FlashPA, U32 DRAMPA, U32 Len)
{
    while (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x01)
    {
        // do nothing
    }
    //set source address
    RIU[(LOCAL_BDMA_REG_BASE + 0x08 )] = (U16)(FlashPA & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x0A )] = (U16)(FlashPA >> 16);
    //set destination address
    RIU[(LOCAL_BDMA_REG_BASE + 0x0C )] = (U16)(DRAMPA & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x0E )] = (U16)(DRAMPA >> 16);
    // set length
    RIU[(LOCAL_BDMA_REG_BASE + 0x10 )] = (U16)(Len & 0x0000ffff);
    RIU[(LOCAL_BDMA_REG_BASE + 0x12 )] = (U16)(Len >> 16);
    //increasing
    RIU8[(LOCAL_BDMA_REG_BASE + 0x06)*2] = 0x0;   //increasing

    //((DstDataWidth|Dsttype)  <<8) |(SrcDataWidth|srctype)
    RIU[(LOCAL_BDMA_REG_BASE+0x04)] = 0x4045;

    //clear status
    RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] = RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] | 0x1c;
    //fire once
    RIU8[ (LOCAL_BDMA_REG_BASE*2)] = 0x01 ;
    while(1)
    {
        if((RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08)
            break;
    }
}
