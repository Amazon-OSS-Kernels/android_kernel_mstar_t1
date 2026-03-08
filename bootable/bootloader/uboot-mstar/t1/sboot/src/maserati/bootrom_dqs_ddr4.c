/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

#define REG_ATOP_REGION                    0x2F
//#define DEBUG
//#define DDR4_READ_DBI_EN
//#define MPR_TRAINING_EN
//#define VREF_DQ_EN
//#define MIU_ENABLE_AUTO_WRITE_PHASE
//#define TRIGGER_LEVEL_EN
#define MIU_ENABLE_AUTO_READ_PHASE

typedef unsigned char   u8;
typedef unsigned int   u16;
typedef unsigned long  u32;

#define MHal_MIU_ReadReg16(u32bank, u32reg ) *( ( volatile u16* ) (MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1)) )
#define MHal_MIU_WritReg16(u32bank, u32reg, u16val ) *( ( volatile u16* ) (MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1))  ) = (u16val)


//-------------------------------------------------------------------------------------------------
//  Prototypes
//-------------------------------------------------------------------------------------------------
void putn( u8 n );
void putk( char c );
void delayus(u32 us);
void BootRom_MiuDdr4(u8 u8MiuDev);
u16 VrefDQ_train_loop(u16 u16RegMiuDtop, u16 u16RegMiuArb, u8 Indx, u8 step, u8 max);
u32 trigger_level_trian_loop(u16 u16RegMiuAtop, u16 u16RegMiuArb, u8 u8Start[2], u8 step, u8 max);
void single_cmd(u16 u16RegMiuDtop, u16 mrx, u8 CmdVal);
void BootRom_MiuDdr4Dqs(u8 u8MiuDev);
//-------------------------------------------------------------------------------------------------
//  Local variables
//-------------------------------------------------------------------------------------------------
/*
const u32 VREF_VAL[37] = {0x44, 0x46, 0x48, 0x4a,
                          0x4c, 0x4e, 0x50, 0x52,
                          0x54, 0x56, 0x58, 0x5a,
                          0x5c, 0x5e, 0x60, 0x62,
                          0x64, 0x0e, 0x10, 0x12,
                          0x14, 0x16, 0x18, 0x1a,
                          0x1c, 0x1e, 0x20, 0x22,
                          0x24, 0x26, 0x28, 0x2a,
                          0x2c, 0x2e, 0x30, 0x32, 0x34};
*/
const u32 VREF_VAL[10] = {0x1c, 0x1e, 0x20, 0x22,
                          0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e};
//-------------------------------------------------------------------------------------------------
//  Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void putn(u8 n)
{
    char c;
    c = '0' + n;

    *(volatile unsigned int*)(0x1F201300) = c;
}
//-------------------------------------------------------------------------------------------------
void putk(char c)
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
u32 trigger_level_trian_loop(u16 u16RegMiuAtop, u16 u16RegMiuArb, u8 u8Start[2], u8 step, u8 max)
{
    u8 loop;
    u16 u16temp=0;
    u16 u16temp1=0;
    u16 u16temp2[2]={0, 0};
    u32 u32temp3=0;

    u16 u16data[2]={0, 0};
    u16 u16data1[2]={0, 0};
    u16 u16data2[2]={0, 0};

    for (loop=0; loop<(max*step); loop=loop+step)
    {
        if ((u8Start[0] + loop) < 32)
        {
            u16temp  = (u8Start[0] + loop) + ((u8Start[0] + loop) << 8);
        }
        else
        {
            u16temp  = (31) + (31 << 8);
        }
        if ((u8Start[1] + loop) < 32)
        {
            u16temp1 = (u8Start[1] + loop) + ((u8Start[1] + loop) << 8);
        }
        else
        {
            u16temp1  = (31) + (31 << 8);
        }

        MHal_MIU_WritReg16(u16RegMiuAtop, 0x74, u16temp);
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x76, u16temp1);

        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f);                //reg_dqsm_rst_sel = 1
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005);                //reg_dqsm_rst_sel = 0
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x02);        //reg_rx_ff_rst = 1
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & ~0x02);       //reg_rx_ff_rst = 0

        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0004);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0012);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

        while((MHal_MIU_ReadReg16(u16RegMiuArb, 0x78) & 0x8000) != 0x8000);

        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x800e);

        u16data2[0]=u16data1[0];
        u16data1[0]=u16data[0];

        if ((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) & 0x7f) > ((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) >> 8) & 0x7f))
        {
            u16data[0] = ((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) >> 8) & 0x7f);
        }
        else
        {
            u16data[0] = (MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) & 0x7f);
        }

        u16temp = u16data[0] + u16data1[0] + u16data2[0];

        if (u16temp >= u16temp2[0])
        {
            u16temp2[0] = u16temp;
            u32temp3  = (u32temp3 & ~0xff) + ((loop + u8Start[0]) - 1);
        }

        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x800f);

        u16data2[1]=u16data1[1];
        u16data1[1]=u16data[1];

        if ((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) & 0x7f) > ((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) >> 8) & 0x7f))
        {
            u16data[1] = ((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) >> 8) & 0x7f);
        }
        else
        {
            u16data[1] = (MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) & 0x7f);
        }

        u16temp = u16data[1] + u16data1[1] + u16data2[1];
        if (u16temp >= u16temp2[1])
        {
            u16temp2[1] = u16temp;
            u32temp3  = (u32temp3 & ~0xff00) + (((loop + u8Start[1] - 1) << 8));
        }
    }
    return u32temp3;
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
        MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);       //[05] : reg_auto_ref_off = 1

        //Enable & Change DQ Vref
        //Set MRx[6:0] = Choose VrefDQ value & Range
        single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[loop] | 0x80, 0x00); // = 1 (Enable VrefDQ training)
        delayus(1); //wait 150ns for tVREFDQE

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
void delayus(u32 us)
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
    u16 u16RegVal1;
    u16 u16RegMiuArb;
    u16 u16RegMiuAtop;
    u16 u16RegMiuDtop;
    u16 u16RegMiuDtopE;
    u32 u32KcodeOffsetValue;
    u16 u16DQSMaxCenter[4];
    u16 u16KCodeOffset[4];
    u16 u16KCode;
    u8 i = 0;
    u8 u8Cnt;
    u8 u8Cnt1;
    u16 u16RegVal;
    u32 u32RegVal;
    u8 Start[2];

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
    MHal_MIU_WritReg16(u16RegMiuArb, 0x06, 0x7fff);

    /* Enable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) | 0x01);

    /*Enable miu_test limit mask*/
    /* STEP 0: STEP0. DPAT engine pre-setting  */
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x8000);
#ifdef MPR_TRAINING_EN
    /* STEP2. MPR training   (determine initial trigger level setting) */
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x20);    //[05] : reg_auto_ref_off = 1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x06, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x06) | 0x1000);  //[12] : reg_cke_always_on = 1


    //--------------------------------------------------------
    // SW set MPR pattens. (pattern can be changed here!)
    // Set 'mpr0'     = 8'h55 (DRAM default)
    // Set 'mpr1'     = 8'h33 (DRAM default)
    // Set 'mpr2'     = 8'h0f (DRAM default)
    // Set 'mpr3'     = 8'h00 (DRAM default)
    // Set 'mpr_mode' = 2'h2
    //--------------------------------------------------------
    /* MPR Write begin */
    //Insert precharge all
    single_cmd(u16RegMiuDtop, 0x0400, 0x02);

    //Program MPR Mode to Staggered mode & Enable MPR Mode
    single_cmd(u16RegMiuDtop, 0x7004, 0x00); //{3'b011,mpr_mode,11'h004}

    //------------------------------------------------------------------------------
    // If MPR pattern need change, Insert these settings (begin)
    // SW set MPR pattens. (pattern can be changed here!)
    // Set 'mpr0'     = 8'haa
    // Set 'mpr1'     = 8'h55
    // Set 'mpr2'     = 8'hf0
    // Set 'mpr3'     = 8'h00
    // Set 'mpr_mode' = 2'h2
    //--------------------------------------------------------------------------------
    //Program MPR0 register
    single_cmd(u16RegMiuDtop, 0x00aa, 0x04);  //{3'b000,5'b00000,mpr0}
    delayus(1); //wait tWR_MPR(~=25ns)

    //Program MPR1 register
    single_cmd(u16RegMiuDtop, 0x20cc, 0x04); //{3'b001,5'b00000,mpr1}
    delayus(1); //wait tWR_MPR(~=25ns)

    //Program MPR2 register
    single_cmd(u16RegMiuDtop, 0x40f0, 0x04); //{3'b010,5'b00000,mpr2}
    delayus(1); //wait tWR_MPR(~=25ns)

    //Program MPR3 register
    single_cmd(u16RegMiuDtop, 0x6000, 0x04); //{3'b011,5'b00000,mpr3}
    delayus(1); //wait tWR_MPR(~=25ns)


    //Enable BIST MPR check mode
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x34, 0x8000);    //reg_mpr_test_on = 1
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0001);    //reg_test_base = MPRx start
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0021);    //reg_test_length = MPR test length


    // Loop begin
    Start[0] = 0;
    u8Cnt    = 0;
    u8Cnt1   = 0;
    for (i = 0; i < 32; i = i + 4)
    {
        u16RegVal0 = (i + (i << 8)) & 0x1f1f;

        MHal_MIU_WritReg16(u16RegMiuAtop, 0x74, u16RegVal0);
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x76, u16RegVal0);

        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f);                //reg_dqsm_rst_sel = 1
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005);                //reg_dqsm_rst_sel = 0
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x02);        //reg_rx_ff_rst = 1
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & ~0x02);       //reg_rx_ff_rst = 0

        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0180);
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0181);

        if ((MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0) & 0xE000) == 0x8000)
        {
            if (u8Cnt == 0)
            {
                u8Cnt1 = 0;
                Start[0] = i;
            }
            u8Cnt++;
            u8Cnt1++;
        }
        else
        {
            u8Cnt = 0;
        }
    }

    u16RegVal0 =  (Start[0] + (u8Cnt1 << 2)) >> 1;
    u16RegVal0 = u16RegVal0 + (u16RegVal0 << 8);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x74, u16RegVal0 & 0x1f1f);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x76, u16RegVal0 & 0x1f1f);

    //Disable MPR Mode
    single_cmd(u16RegMiuDtop, 0x6000, 0x00);            //[12:9] : reg_single_cmd =  0 (MR setting)
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x34 ,0x0000);    //reg_mpr_test_on = 0
    delayus(1); //wait tWR_MPR(~=25ns)

    /* STEP3. Update trigger level */
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20); //[05] : reg_auto_ref_off = 0
#endif

    /* STEP4. VrefDQ training   (determine optimized DRAM VredDQ value) */
    //DPAT engine pre-setting (DQ TX auto-phase)
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x8001);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0084);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0086);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) & ~0x01);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0010);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c61);
    //MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x8009);


#ifdef VREF_DQ_EN

    MHal_MIU_WritReg16(u16RegMiuDtop, 0x18, (MHal_MIU_ReadReg16(u16RegMiuArb, 0x64) & 0xff00) | (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff00));       //Copy From 0x161565[7:0] to 0x101219[7:0]

    // Fine tune -->step :  +/- 2 codes twice
    u16RegVal = 4;//TDB
    u16RegVal = VrefDQ_train_loop(u16RegMiuDtop, u16RegMiuArb, u16RegVal-4, 1, 9);


    //SW need calculate Vref_Val = The Vref_val that have Max. minimum margin
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000); //[05] : reg_auto_ref_off = 1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20); //[05] : reg_auto_ref_off = 1

    //Enable & Change DQ Vref
    //Set MRx[6:0] = Choose VrefDQ value & Range
    single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal]  | 0x80, 0x00);
    delayus(1); //wait 150ns for tVREFDQE

    //Exit DQ Vref training Mode.
    single_cmd(u16RegMiuDtop, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0x80, 0x00);                    // [7]= 0 (Disable VrefDQ training)
    delayus(1); //wait 150ns for tVREFDQE

    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);           //[05] : reg_auto_ref_off = 0
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x06, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x06) & ~0x1000);         //[12] : reg_cke_always_on = 0
#endif

#ifdef MIU_ENABLE_AUTO_WRITE_PHASE

    //STEP5.Enable TX DQ Deskew test (determine optimized TX DQ phase)
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) | 0x01);            //reg_si_mode = 1
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x04);       //dpat_rst
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x00);       //dpat_rst
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc8, 0x00);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xca, 0x00);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xcc, 0x00);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x02);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x13);

    while((MHal_MIU_ReadReg16(u16RegMiuArb, 0x78)) == 0x0);

    //STEP6. Read TX deskew result & Write to REGs
    //TX deskew result write to REGs
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0004);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ3,DQ2,DQ1,DQ0}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ7,DQ6,DQ5,DQ4}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd2, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);   //reg_rdata = 0x110dc6[15:0] = {DQM3,DQM2,DQM1,DQM0}
    u16RegVal1 = u16RegVal0 & 0x00ff;
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd8, u16RegVal1);
    u16RegVal1 = (u16RegVal0 & 0xff00) >> 8;
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe8, u16RegVal1);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0005);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ11,DQ10,DQ9,DQ8}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ15,DQ14,DQ13,DQ12}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd6, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);   //reg_rdata = 0x110dc6[15:0] = {REGION3,REGION2,REGION1,REGION0}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x2e, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0006);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ19,DQ18,DQ17,DQ16}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ23,DQ22,DQ21,DQ20}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe2, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0007);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ27,DQ26,DQ25,DQ24}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ31,DQ30,DQ29,DQ28}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe6, u16RegVal0);
#endif

    //STEP7. Trigger level training   (determine optimize trigger level setting)

#ifdef DDR4_READ_DBI_EN
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x34, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0x34) | 0x80);   //reg_dbi_bit_test_en=1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x34, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x34) | 0x80);
#endif

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);      //test length for RX
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0090);      //test length for RX
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);      //length
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0080);      //test mode
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);      //test mode
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) | 0x01);    //reg_si_mode = 1
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x00);      //reg_test_base
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x2000);     //pass condition window extend to 5 phase (reg_cont_pass_num = 2) , No jump scan (reg_rx_ph_scan_step = 0)

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f0);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);

#ifdef TRIGGER_LEVEL_EN
    Start[0]=(MS_U8) MHal_MIU_ReadReg16(u16RegMiuAtop, 0x74);
    if (Start[0] > 4)
    {
        Start[0] -= 4;
    }
    else
    {
        Start[0] = 0;
    }
    Start[1]=(MS_U8) MHal_MIU_ReadReg16(u16RegMiuAtop, 0x76);
    if (Start[1] > 4)
    {
        Start[1] -= 4;
    }
    else
    {
        Start[1] = 0;
    }
    u32RegVal = trigger_level_trian_loop(u16RegMiuAtop, u16RegMiuArb, Start, 1, 9);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x74, (u32RegVal & 0xff) | ((u32RegVal & 0xff) << 8));
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x76, ((u32RegVal >> 8) & 0xff) | (((u32RegVal >> 8) & 0xff) << 8));

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f);                  //reg_dqsm_rst_sel = 1
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005);                  //reg_dqsm_rst_sel = 0
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x02);        //reg_rx_ff_rst = 1
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & ~0x02);       //reg_rx_ff_rst = 0
#endif

#ifdef MIU_ENABLE_AUTO_READ_PHASE
    //STEP8. Enable RX DQ/DQS deskew   (determine optimized RX per-bit deskew phase)
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0004);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f0);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    while((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x78) & 0x8000) == 0x0);

    //STEP9. Read RX DQ deskew result & write to REGs
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0000);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DQ3,DQ2,DQ1,DQ0}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);               //reg_rdata = 0x110dc4[15:0] = {DQ7,DQ6,DQ5,DQ4}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa2, u16RegVal0);
    u16DQSMaxCenter[0] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);       //reg_rdata = 0x110dc6[6:0] = DQS0_max_center
    u16DQSMaxCenter[0] = u16DQSMaxCenter[0] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0001);                    //reg_rdata = 0x110dc6[6:0] = DQS0_max_center
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DQ11,DQ10,DQ9,DQ8}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);               //reg_rdata = 0x110dc4[15:0] = {DQ15,DQ14,DQ13,DQ12}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa6, u16RegVal0);
    u16DQSMaxCenter[1] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[1] = u16DQSMaxCenter[1] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0002);                    //reg_rdata = 0x110dc6[6:0] = DQS1_max_center
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DQ19,DQ18,DQ17,DQ16}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa8, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);               //reg_rdata = 0x110dc4[15:0] = {DQ23,DQ22,DQ21,DQ20}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xaa, u16RegVal0);
    u16DQSMaxCenter[2] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[2] = u16DQSMaxCenter[2] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0003);                    //reg_rdata = 0x110dc6[6:0] = DQS2_max_center
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DQ27,DQ26,DQ25,DQ24}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xac, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);               //reg_rdata = 0x110dc4[15:0] = {DQ31,DQ30,DQ29,DQ28}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xae, u16RegVal0);

	u16DQSMaxCenter[3] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[3] = u16DQSMaxCenter[3] & 0x007F;

#ifdef DDR4_READ_DBI_EN
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0010);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);               //reg_rdata = 0x110dc2[15:0] = {DM3,DM2,DM1,DM0}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xb4, u16RegVal0);

#endif

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

    //STEP10. Set DQS Phase (K/2 + offset)
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);            //[11] reg_dline_gated_control  = 0
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);             //[0]  reg_dpat_en = 0
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c60);             //[0]  reg_live_test_en = 0
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);            //[11] reg_dline_gated_control  = 1

    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x0000);

    /* Disable SI mode */
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) & ~0x01);

#ifdef DDR4_READ_DBI_EN
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x34, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0x34) & ~0x80);   //reg_dbi_bit_test_en=0
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x34, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x34) & ~0x80);
#endif

    putk('M');
    putk('I');
    putk('U');
    putn(u8MiuDev);
    putk('_');
    putk('D');
    putk('Q');
    putk('S');
    putk('-');
    putk('O');
    putk('K');
    putk('\n');
    putk('\r');
}
