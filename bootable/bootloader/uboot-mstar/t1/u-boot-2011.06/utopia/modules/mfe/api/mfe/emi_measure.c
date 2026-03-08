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
#include <stdio.h>
#include <string.h>

#if defined(_MIPS_PLATFORM_) && defined(_M4VE_BIG2_)
#include "shellcfg.h"


#include "com_type.h"

//#define EMI_MEASURE_OFF 0
//#define EMI_MEASURE_ON  1
#define EMI_TIMER_INTERVAL  10  //ms
int reg_ind = 0;
U16 times_prt=0; // print message
U16 times_mnt=0; // change monitor client ID
U32 measure[2][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0}};
U32 emi_report[2] = {0, 0};

const unsigned short emi_client_input[6][5]={ {0x0, 0x4100, 0x4342, 0x4544, 0x4746}
                            , {0x090e, 0x5100, 0x5352, 0x5554, 0x5756}
                            , {0x38fe, 0x6000, 0x6261, 0x6463, 0x6665}
                            , {0xc2f2, 0x6800, 0x6a69, 0x6c6b, 0x706d}
                            , {0xbcfe, 0x7100, 0x7473, 0x7675, 0x7877}
                            , {0x141c, 0x4f00, 0x7a79, 0x7c7b, 0x007d}};

void mp4rec_emi_measure(U8 mode)
{
    static U32 avgtimes = 0;
    U16 temp1=0, temp2=0, i;
    U32 bps, me_cnt = 1, me_clk = 156;//104;//52; // MHz
/*
    if(EMI_MEASURE_OFF == mode)
    {
        WRITE_REG(0xbf8020c0, U16, 0x00);
        diag_printf("=== BANDWIDTH REPORT === \n");
        diag_printf("avg: %d max: %d\n", (emi_report[0]*me_clk/me_cnt)>>10, (emi_report[1]*me_clk/me_cnt)>>10 );
        emi_report[0] = 0;
        emi_report[1] = 0;
        avgtimes = 0;
        return;
    }
*/
//    diag_printf("in mp4rec_emi_measure %d %d %d %d %d %d %d\n", times_mnt
//        , reg_ind, emi_client_input[0][0], emi_client_input[1][0], emi_client_input[2][0], measure[0][0], measure[0][1]);
    // report for every one second
    if(times_prt >= (1000/EMI_TIMER_INTERVAL))
    {
        emi_report[0] = (emi_report[0]*avgtimes + (measure[0][0]/times_prt))/(avgtimes+1);
        avgtimes++;
        emi_report[1] = emi_report[1] > measure[1][0] ? emi_report[1] : measure[1][0];
        for(i=0; i<8; i++)
        {
            diag_printf("[%x] avg: %d max: %d\n", (emi_client_input[reg_ind][(i/2)+1]>>((i%2)*8))&0x3F, ((measure[0][i]/times_prt)*me_clk/me_cnt)>>10, (measure[1][i]*me_clk/me_cnt)>>10 );
            measure[0][i] = 0;
            measure[1][i] = 0;
        }
        times_prt = 0;
    }

    // change monitor ID every 10 seconds
    if (0 == (times_mnt % (10000/EMI_TIMER_INTERVAL))) {
        reg_ind = (reg_ind+1)%6;
        READ_REG(0xbf802084, U16, temp1);
        diag_printf("reg_ind: %d   PSRAM clk reg: %d (%x)\n", reg_ind, temp1, *(U16*)0xBF810740);
        WRITE_REG(0xbf8020c0, U16, 0x100); //*(short *)(0xbf8020c0) = 0x100;
        WRITE_REG(0xbf8020c0, U16, 0x00); //*(short *)(0xbf8020c0) = 0;
        WRITE_REG(0xbf8020c4, U16, emi_client_input[reg_ind][0]);
        WRITE_REG(0xbf8020c8, U16, emi_client_input[reg_ind][1]);
        WRITE_REG(0xbf8020cc, U16, emi_client_input[reg_ind][2]);
        WRITE_REG(0xbf8020d0, U16, emi_client_input[reg_ind][3]);
        WRITE_REG(0xbf8020d4, U16, emi_client_input[reg_ind][4]);
        WRITE_REG(0xbf8020d8, U16, 0x1111); //DRAM,  0000: PSRAM
        WRITE_REG(0xbf8020dC, U16, 0x1111); //DRAM,  0000: PSRAM
        //set  clk  (M+1) -> 52Mhz
        WRITE_REG(0xbf8020e0, U16, ((me_cnt-1)<<16) || ((me_cnt-1)<<16));
        WRITE_REG(0xbf8020e4, U16, ((me_cnt-1)<<16) || ((me_cnt-1)<<16));
        WRITE_REG(0xbf8020e8, U16, ((me_cnt-1)<<16) || ((me_cnt-1)<<16));
        WRITE_REG(0xbf8020ec, U16, ((me_cnt-1)<<16) || ((me_cnt-1)<<16));
        //start to measure
        WRITE_REG(0xbf8020b0, U16, 0xFFFF); //*(short *)(0xbf8020b0) = 0xFFFF;
        times_mnt = 0;
        // clear statics info
        times_prt = 0;
        for(i=0; i<8; i++)
        {
            measure[0][i] = 0;
            measure[1][i] = 0;
        }
    }

    for(i=0; i<8; i++)
    {
        READ_REG((0xbf802100+i*8), U16, temp1); //low bytes
        READ_REG((0xbf802104+i*8), U16, temp2); //high bytes
        bps = (U32)(temp2<<16)|(U32)temp1;
        measure[0][i] += bps;
        measure[1][i] = measure[1][i] > bps ? measure[1][i] : bps;
    }

    times_prt++;
    times_mnt++;
    return;
}

#endif
