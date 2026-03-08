/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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
/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#include <common.h>
#include <MsMath.h>
#include <ShareType.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  rRand
/// @brief         :  This function is implement for get random value.
/// @author        :  MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
int rRand(unsigned int seed0, unsigned int seed1)
{
    static const unsigned short para_16=0x8000;
    static const unsigned char para_8=0x43;
    static unsigned short valu_16=0x0111;
    static unsigned char valu_8=0x10;

    unsigned short t_16;
    unsigned char t_8;
    int i,next_8,next_16;

    if(seed0>0)
        valu_16=(seed0 & 0xffff);
    if(seed1>0)
        valu_8=(seed1 & 0xff);

    t_16=para_16&valu_16;
    t_8=para_8&valu_8;
    next_8=0;
    for(i=0;i<8;++i)
    {
      if(t_8%2)
       ++next_8;
      t_8/=2;
    }
    for(;i<24;++i)
    {
      if(t_16%2)
       ++next_8;
      t_16/=2;
    }
    next_8%=2;
    next_16=valu_8/0x7f;
    valu_8=valu_8-next_16*0x7f;
    valu_8=valu_8*2+next_8;
    valu_16=valu_16-(valu_16/0x7fff)*0x7fff;
    valu_16=valu_16*2+next_16;
    return valu_16*0x7f*2+valu_8;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  ascii_to_hex
/// @brief         :  This function is implement for ascii_to_hex
/// @author        :  MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short ascii_to_hex(const char *buffer, int length)
{
    unsigned short u16temp;
    U8 tempbuffer[4];
    U8 i = 0;

    while(i<4)
    {
        //printf("%c ", *buffer);
        if((*buffer >= 0x30)&&(*buffer <= 0x39)) // 0 ~ 9
            tempbuffer[i] = *buffer - 0x30;
        else if((*buffer >= 0x41)&&(*buffer <= 0x46)) // A ~ E
            tempbuffer[i] = *buffer - 0x37;
        else if((*buffer >= 0x61)&&(*buffer <= 0x66)) // a ~ e
            tempbuffer[i] = *buffer - 0x57;
        else
            printf("Error> wrong character\n");

        //printf("0x%x \n", tempbuffer[i]);
        i++;
        buffer++;
    }

    u16temp = (U16)(tempbuffer[0]<<12|tempbuffer[1]<<8|tempbuffer[2]<<4|tempbuffer[3]);
    //printf("%04x\n", u16temp);

    return u16temp;
}