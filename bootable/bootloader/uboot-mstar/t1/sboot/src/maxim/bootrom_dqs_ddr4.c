/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define MIU_RIU_REG_BASE                   0x1F000000

#define MIU0_RIU_DTOP                      0x1012
#define MIU1_RIU_DTOP                      0x1006
#define MIU2_RIU_DTOP                      0x1620
#define MIU0_RIU_ARB                       0x1615
#define MIU1_RIU_ARB                       0x1622
#define MIU2_RIU_ARB                       0x1623
#define MIU0_RIU_ATOP                      0x110d
#define MIU1_RIU_ATOP                      0x1616
#define MIU2_RIU_ATOP                      0x1621
#define MIU0_RIU_DTOP_E                    0x152b
#define MIU1_RIU_DTOP_E                    0x152c
#define MIU2_RIU_DTOP_E                    0x152d

#define EFUSE_RIU_BANK                     0x0020
#define REG_ATOP_REGION                    0x2F
#define DEBUG
#define VREF_DQ_EN
#define MIU_ENABLE_AUTO_WRITE_PHASE
#define MIU_ENABLE_AUTO_READ_PHASE

typedef unsigned char   u8;
typedef unsigned int   u16;
typedef unsigned long  u32;

#define MHal_MIU_ReadReg16(u32bank, u32reg ) *( ( volatile u16* ) (MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1)) )
#define MHal_MIU_WritReg16(u32bank, u32reg, u16val ) *( ( volatile u16* ) (MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1))  ) = (u16val)

#define MHal_EFUSE_ReadReg16(u32reg) *(( volatile u16* ) (MIU_RIU_REG_BASE + EFUSE_RIU_BANK*0x100*2 + ((u32reg) << 1)))
#define MHal_EFUSE_WritReg16(u32reg, u16val) *(( volatile u16* ) (MIU_RIU_REG_BASE + EFUSE_RIU_BANK*0x100*2 + ((u32reg) << 1))) = (u16val)



//-------------------------------------------------------------------------------------------------
//  Prototypes
//-------------------------------------------------------------------------------------------------
void putn_ddr4( u8 n );
void putk_ddr4( char c );
void delayus_ddr4(u32 us);
void BootRom_MiuDdr4Dqs(u8 u8MiuDev);
void single_cmd(u16 u16RegMiuDtop, u16 mrx, u8 CmdVal);
u16 VrefDQ_train_loop(u16 u16RegMiuDtop, u16 u16RegMiuArb, u8 Indx, u8 step, u8 max);
void BootRom_MiuDdrTrim(void);
u8 CalculateDrvStength(u8 DrvStength, u8 DefDrv);
//-------------------------------------------------------------------------------------------------
//  Local variables
//-------------------------------------------------------------------------------------------------
const u32 VREF_VAL[10] = {0x1c, 0x1e, 0x20, 0x22,
                          0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e};

//-------------------------------------------------------------------------------------------------
//  Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void putn_ddr4(u8 n)
{
    char c = '0' + n;

    *(volatile unsigned int*)(0x1F201300) = c;
}
//-------------------------------------------------------------------------------------------------
void putk_ddr4(char c)
{
   *(volatile unsigned int*)(0x1F201300) = c;
}
//-------------------------------------------------------------------------------------------------
void single_cmd(u16 u16RegMiuDtop, u16 mrx, u8 CmdVal)
{
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x18, mrx); //Pre-charge all command
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x1e00) | (CmdVal << 9));   //[12:9] : reg_single_cmd =  2 (Pre-charge)
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x0100);                      //[8] : reg_single_cmd_en = 1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x0100);                     //[8] : reg_single_cmd_en = 0
}
//-------------------------------------------------------------------------------------------------
u16 VrefDQ_train_loop(u16 u16RegMiuDtop, u16 u16RegMiuArb, u8 Indx, u8 step, u8 max)
{
    u16 loop;

    u16 u16temp1=0;
    u16 u16temp2=0;
    u16 u16data[3]={0,0,0};

    u8  u8temp;
    u8  u8temp1;
    u8  u8temp2;

    u8 loop_cnt=0;

    for (loop=Indx; loop<((max*step)+Indx); loop=loop+step)
    {
        if (loop > 10)
        {
            break;
        }
        MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x20);       //[05] : reg_auto_ref_off = 1

        //Enable & Change DQ Vref
        //Set MRx[6:0] = Choose VrefDQ value & Range
        single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[loop] | 0x80, 0x00); // = 1 (Enable VrefDQ training)
        delayus_ddr4(1); //wait 150ns for tVREFDQE

        single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[loop] | 0x80, 0x00); // = 1 (Enable VrefDQ training)
        delayus_ddr4(1); //wait 150ns for tVREFDQE

        //Enter DQ Vref training Mode.
        MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);       //[05] : reg_auto_ref_off = 0
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x8009); //SWITCH dpat deb_out = min. margin
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0004);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);                                                 //dpat_rst

        // Enable TX DQ deskew scan (by DPAT engine)
        MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0002);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

        while((MHal_MIU_ReadReg16(u16RegMiuArb, 0x78) & 0x8000) == 0x0);


        u16data[2]=u16data[1];
        u16data[1]=u16data[0];
        //u16data[0]=MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e);

        u16data[0]=MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) << 4;
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x800e); //SWITCH dpat deb_out = min. margin
        u16data[0] += MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e);
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x800f); //SWITCH dpat deb_out = min. margin
        u16data[0] += MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e);
        //Final Min. margin = (Total Min. Margin)* 4 + Low word Min. Margin + High word Min. Margin
        u16temp1 = u16data[0] + (u16data[1] << 1) + u16data[2];
        if (u16temp1 >= u16temp2)
        {
            //if (u16temp1 > u16temp2)
            //{
            //    u8temp2=loop-1;
            //}
            u16temp2=u16temp1;
            u8temp1=loop-1;
        }
        #ifdef DEBUG
        if(loop_cnt >= 8)  {
        MHal_MIU_WritReg16(u16RegMiuArb, (0xa0 + ((loop_cnt-8)<<1) ), (u16data[0]) );
        }
        else if(loop_cnt >= 4)  {
        MHal_MIU_WritReg16(u16RegMiuArb, (0x90 + ((loop_cnt-4)<<1) ), (u16data[0]) );
        }
        else {
        MHal_MIU_WritReg16(u16RegMiuArb, (0x80 + (loop_cnt<<1) ), (u16data[0]) );
        }
        loop_cnt++;
        #endif
    }
    //u8temp = ((u8temp1 + u8temp2 + 1) >> 1);
    return u8temp1;
}
//-------------------------------------------------------------------------------------------------
void delayus_ddr4(u32 us)
{
    u16 u16RegVal0;

    u16RegVal0 = ((us* 12) & 0xffff);
    MHal_MIU_WritReg16(0x30, 0x24, u16RegVal0);

    u16RegVal0 = ((us* 12) >> 16);
    MHal_MIU_WritReg16(0x30, 0x26, u16RegVal0);

    u16RegVal0 = 0x0002;
    MHal_MIU_WritReg16(0x30, 0x20, u16RegVal0);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(0x30, 0x22);
    }while((u16RegVal0 & 0x0001) == 0);
}

//-------------------------------------------------------------------------------------------------
void BootRom_MiuDdr4Dqs(u8 u8MiuDev)
{
    u16 u16RegVal0;
#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    u16 u16RegVal1;
#endif
    u16 u16RegMiuArb;
    u16 u16RegMiuAtop;
    u16 u16RegMiuDtop;
    u16 u16RegMiuDtopE;
    u32 u32KcodeOffsetValue;
    u16 u16DQSMaxCenter[4];
    u16 u16KCodeOffset[4];
    u16 u16KCode;
    u8 i = 0;

    u16 u16DQPos0 = 0;
    u16 u16DQPos1 = 0;
    u16 u16DQPos2 = 0;
    u16 u16DQPos3 = 0;
    u16 u16DQPosRes = 0;

    u16 u16RegVal;

    if(u8MiuDev == 0)
    {
        u16RegMiuArb  = MIU0_RIU_ARB;
        u16RegMiuAtop = MIU0_RIU_ATOP;
        u16RegMiuDtop = MIU0_RIU_DTOP;
        u16RegMiuDtopE = MIU0_RIU_DTOP_E;
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
        MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x0000);
    }
    else if(u8MiuDev == 1)
    {
        u16RegMiuArb  = MIU1_RIU_ARB;
        u16RegMiuAtop = MIU1_RIU_ATOP;
        u16RegMiuDtop = MIU1_RIU_DTOP;
        u16RegMiuDtopE = MIU1_RIU_DTOP_E;
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
        MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x0000);
    }
    else if(u8MiuDev == 2)
    {
        u16RegMiuArb  = MIU2_RIU_ARB;
        u16RegMiuAtop = MIU2_RIU_ATOP;
        u16RegMiuDtop = MIU2_RIU_DTOP;
        u16RegMiuDtopE = MIU2_RIU_DTOP_E;
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
        MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x8000);
    }
    else
    {
        return;
    }

    u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 & ~(0x0800));
    MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf8, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0000);

    /* Unmask MIU_group_4 bit15 */
    if((MHal_MIU_ReadReg16(u16RegMiuDtop, 0x02) & 0x0c) == 0x00)
            {
        //16bit
        MHal_MIU_WritReg16(u16RegMiuDtop, 0x46, 0x7ffe);
        /*Enable miu_test limit mask*/
        MHal_MIU_WritReg16(u16RegMiuArb, 0x5c, 0x8000);
        }
        else
        {
    //32bit
    MHal_MIU_WritReg16(u16RegMiuArb, 0x06, 0x7fff);
    /*Enable miu_test limit mask*/
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x8000);
    }

    /* high way mask */
    MHal_MIU_WritReg16(u16RegMiuArb, 0xe6, 0x0000);

    /* Enable SI mode */
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) | 0x01);

    /* STEP4. VrefDQ training   (determine optimized DRAM VredDQ value) */
    //DPAT engine pre-setting (DQ TX auto-phase)
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x1a00);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);      //length
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd6, 0xa0a0); //reg_test_logic need set to 0xa0a0 for SI mode
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0086);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) & ~0x01);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x2010); //reg_cont_pass_num = 2 (for pass number = 5)
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d81);

#ifdef VREF_DQ_EN
    if((MHal_MIU_ReadReg16(u16RegMiuArb, 0x66) & 0x08) == 0x08)
    {
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x18, (MHal_MIU_ReadReg16(u16RegMiuArb, 0x64) & 0xff00) | (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff00));       //Copy From 0x161565[7:0] to 0x101219[7:0]

    // Fine tune -->step :  +/- 2 codes twice
    u16RegVal = 4;//TDB
    u16RegVal = VrefDQ_train_loop(u16RegMiuDtop, u16RegMiuArb, u16RegVal-4, 1, 9);

    //SW need calculate Vref_Val = The Vref_val that have Max. minimum margin
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x20); //[05] : reg_auto_ref_off = 1

    //Enable & Change DQ Vref
    //Set MRx[6:0] = Choose VrefDQ value & Range
    single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal]  | 0x80, 0x00);
    delayus_ddr4(1); //wait 150ns for tVREFDQE

    //Enable & Change DQ Vref
    //Set MRx[6:0] = Choose VrefDQ value & Range
    single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal]  | 0x80, 0x00);
    delayus_ddr4(1); //wait 150ns for tVREFDQE

    //Exit DQ Vref training Mode.
    single_cmd(u16RegMiuDtop, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0x80, 0x00);                    // [7]= 0 (Disable VrefDQ training)
    delayus_ddr4(1); //wait 150ns for tVREFDQE

    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);           //[05] : reg_auto_ref_off = 0
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x06, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x06) & ~0x1000);         //[12] : reg_cke_always_on = 0
    }
#endif

#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    if((MHal_MIU_ReadReg16(u16RegMiuArb, 0x66) & 0x08) == 0x08)
    {

    // MIU BIST
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x1a00);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);  //length
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0087);

    while((MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0) & 0xc000) != 0x8000); // check BIST result
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);


    //STEP5.Enable TX DQ Deskew test (determine optimized TX DQ phase)
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) | 0x01);            //reg_si_mode = 1
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x04);       //dpat_rst
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x00);       //dpat_rst
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc8, 0x00);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xca, 0x00);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xcc, 0x00);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x02);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x13);

    while((MHal_MIU_ReadReg16(u16RegMiuArb, 0x78) & 0x8000) == 0x0);

    //STEP6. Read TX deskew result & Write to REGs
    //TX deskew result write to REGs
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0004);
    delayus_ddr4(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ3,DQ2,DQ1,DQ0}

    // DQ2 & DQ3 +1 (Max:5)
    if(((u16RegVal0 & 0x0f00) >> 8) <= 4)
        u16DQPos2 = (((u16RegVal0 & 0x0f00) >> 8)  + 1) << 8;
    else
        u16DQPos2 = 0x0500;

    if(((u16RegVal0 & 0xf000) >> 12) <= 4)
        u16DQPos3 = (((u16RegVal0 & 0xf000) >> 12) + 1) << 12;
    else
        u16DQPos3 = 0x5000;
    u16RegVal0 = (u16RegVal0 & 0x00ff) | u16DQPos2 | u16DQPos3;
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd0, u16RegVal0);


    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ7,DQ6,DQ5,DQ4}

    // DQ4 & DQ7 +1 (Max:5)
    if((u16RegVal0 & 0x000f) <= 4)
        u16DQPos0 = (u16RegVal0 & 0x000f) + 1;
    else
        u16DQPos0 = 0x0005;

    if(((u16RegVal0 & 0xf000) >> 12) <= 4)
        u16DQPos3 = (((u16RegVal0 & 0xf000) >> 12) + 1) << 12;
    else
        u16DQPos3 = 0x5000;
    u16RegVal0 = (u16RegVal0 & 0x0ff0) | u16DQPos0 | u16DQPos3;


    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd2, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);   //reg_rdata = 0x110dc6[15:0] = {DQM3,DQM2,DQM1,DQM0}

    // DQM - 1 (Min:0)
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xce, u16RegVal0);

    u16DQPos0 = (u16RegVal0 & 0x000f);
    u16DQPos1 = (u16RegVal0 & 0x00f0) >> 4;
    u16DQPos2 = (u16RegVal0 & 0x0f00) >> 8;
    u16DQPos3 = (u16RegVal0 & 0xf000) >> 12;

    if(u16DQPos0 <= 0x1)
        u16DQPos0 = 0x0;
    else
        u16DQPos0 = u16DQPos0 - 0x1;

    if(u16DQPos1 <= 0x1)
        u16DQPos1 = 0x0;
    else
        u16DQPos1 = u16DQPos1 - 0x1;

    if(u16DQPos2 <= 0x1)
        u16DQPos2 = 0x0;
    else
        u16DQPos2 = u16DQPos2 - 0x1;

    if(u16DQPos3 <= 0x1)
        u16DQPos3 = 0x0;
    else
        u16DQPos3 = u16DQPos3 - 0x1;

    u16DQPosRes = (u16DQPos3 << 12) | (u16DQPos2 << 8) | (u16DQPos1 << 4) | u16DQPos0;

    u16RegVal1 = u16DQPosRes & 0x00ff;
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd8, u16RegVal1);

    u16RegVal1 = (u16DQPosRes & 0xff00) >> 8;
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe8, u16RegVal1);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0005);
    delayus_ddr4(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ11,DQ10,DQ9,DQ8}

    // DQ10 +1 (Max:5)
    if(((u16RegVal0 & 0x0f00) >> 8) <= 4)
        u16DQPos2 = (((u16RegVal0 & 0x0f00) >> 8)  + 1) << 8;
    else
        u16DQPos2 = 0x0500;
    u16RegVal0 = (u16RegVal0 & 0xf0ff) | u16DQPos2;


    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ15,DQ14,DQ13,DQ12}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd6, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);   //reg_rdata = 0x110dc6[15:0] = {REGION3,REGION2,REGION1,REGION0}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x2e, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0006);
    delayus_ddr4(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ19,DQ18,DQ17,DQ16}

    // DQ19 +1 (Max:5)
    if(((u16RegVal0 & 0xf000) >> 12) <= 4)
        u16DQPos3 = (((u16RegVal0 & 0xf000) >> 12) + 1) << 12;
    else
        u16DQPos3 = 0x5000;

    u16RegVal0 = (u16RegVal0 & 0x0fff) | u16DQPos3;


    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ23,DQ22,DQ21,DQ20}

    // DQ20 -1 (Min:0)
    if(((u16RegVal0 & 0x000f) >> 0) >= 1)
        u16DQPos3 = (((u16RegVal0 & 0x000f) >> 0) - 1) << 0;
    else
        u16DQPos3 = 0x0000;

    u16RegVal0 = (u16RegVal0 & 0xfff0) | u16DQPos3;
        
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe2, u16RegVal0);
    
    // DQ22 -1 (Min:0)
    if(((u16RegVal0 & 0x0f00) >> 8) >= 1)
        u16DQPos3 = (((u16RegVal0 & 0x0f00) >> 8) - 1) << 8;
    else
        u16DQPos3 = 0x0000;

    u16RegVal0 = (u16RegVal0 & 0xf0ff) | u16DQPos3;
        
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe2, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0007);
    delayus_ddr4(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ27,DQ26,DQ25,DQ24}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ31,DQ30,DQ29,DQ28}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe6, u16RegVal0);
    }
#endif

#ifdef MIU_ENABLE_AUTO_READ_PHASE

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f0);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0004);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    while((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x78) & 0x8000) == 0x0);

    //STEP9. Read RX DQ deskew result & write to REGs
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0000);
    delayus_ddr4(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DQ3,DQ2,DQ1,DQ0}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);               //reg_rdata = 0x110dc4[15:0] = {DQ7,DQ6,DQ5,DQ4}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa2, u16RegVal0);
    u16DQSMaxCenter[0] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);       //reg_rdata = 0x110dc6[6:0] = DQS0_max_center
    u16DQSMaxCenter[0] = u16DQSMaxCenter[0] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0001);                    //reg_rdata = 0x110dc6[6:0] = DQS0_max_center
    delayus_ddr4(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DQ11,DQ10,DQ9,DQ8}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);               //reg_rdata = 0x110dc4[15:0] = {DQ15,DQ14,DQ13,DQ12}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa6, u16RegVal0);
    u16DQSMaxCenter[1] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[1] = u16DQSMaxCenter[1] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0002);                    //reg_rdata = 0x110dc6[6:0] = DQS1_max_center
    delayus_ddr4(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DQ19,DQ18,DQ17,DQ16}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa8, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);               //reg_rdata = 0x110dc4[15:0] = {DQ23,DQ22,DQ21,DQ20}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xaa, u16RegVal0);
    u16DQSMaxCenter[2] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[2] = u16DQSMaxCenter[2] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0003);                    //reg_rdata = 0x110dc6[6:0] = DQS2_max_center
    delayus_ddr4(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DQ27,DQ26,DQ25,DQ24}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xac, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);               //reg_rdata = 0x110dc4[15:0] = {DQ31,DQ30,DQ29,DQ28}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xae, u16RegVal0);

	u16DQSMaxCenter[3] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[3] = u16DQSMaxCenter[3] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x60, 0x33c8);
    u16KCode = MHal_MIU_ReadReg16(u16RegMiuAtop, 0x66);
    u16KCode = ((u16KCode & 0x007F) >> 1);

    u32KcodeOffsetValue = (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x96) << 16) | MHal_MIU_ReadReg16(u16RegMiuAtop, 0x94);
    for(i= 0; i < 4; i++)
    {
        if(u16DQSMaxCenter[i] > u16KCode)
        {
            u16RegVal0 = u16DQSMaxCenter[i] - u16KCode;
            if (u16RegVal0 >= 0xf)
            {
                u16KCodeOffset[i] = 0x0f;
            }
            else
            {
                u16KCodeOffset[i] = u16RegVal0;
            }
        }
        else
        {
            u16RegVal0 = u16KCode - u16DQSMaxCenter[i];
            if (u16RegVal0 >= 0xf)
            {
                u16KCodeOffset[i] = 0x0f;
            }
            else
            {
                u16KCodeOffset[i] = u16RegVal0;
            }
            u16KCodeOffset[i] = u16KCodeOffset[i] | 0x0010;
        }
    }
    u16RegVal0 = (u16KCodeOffset[1] << 8) | u16KCodeOffset[0];
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x94, u16RegVal0);
    u16RegVal0 = (u16KCodeOffset[3] << 8) | u16KCodeOffset[2];
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x96, u16RegVal0);
#endif

    /* STEP 6 : Disable DPAT engine & Set DQS Phase = 1/2* Kcode+offset (ratio mode) */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d80);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x0000);

    /* Disable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) & ~0x01);

    putk_ddr4('M');
    putk_ddr4('I');
    putk_ddr4('U');
    putn_ddr4(u8MiuDev);
    putk_ddr4('_');
    putk_ddr4('D');
    putk_ddr4('Q');
    putk_ddr4('S');
    putk_ddr4('-');
    putk_ddr4('O');
    putk_ddr4('K');
    putk_ddr4('\n');
    putk_ddr4('\r');
}
