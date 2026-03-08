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
//#define DEBUG
//#define MIU_ENABLE_AUTO_WRITE_PHASE
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
void putn( u8 n );
void putk( char c );
void delayus(u32 us);
void BootRom_MiuDdr3Dqs(u8 u8MiuDev);
void BootRom_MiuDdrTrim(void);
u8 CalculateDrvStength(u8 DrvStength, u8 DefDrv);
//-------------------------------------------------------------------------------------------------
//  Local variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Functions
//-------------------------------------------------------------------------------------------------
void putn(u8 n)
{
    char c = '0' + n;

   *(volatile unsigned int*)(0x1F201300) = c;
}

void putk(char c)
{
   *(volatile unsigned int*)(0x1F201300) = c;
}

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

void BootRom_MiuDdr3Dqs(u8 u8MiuDev)
{
    u16 u16RegVal0;
#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    u16 u16RegVal1;
#endif
    u16 u16RegMiuArb;
    u16 u16RegMiuAtop;
    u16 u16RegMiuDtop;
    u16 u16RegMiuDtopE;
    u16 u16DQSMaxCenter[4];
    u16 u16KCode;
    u16 u16KCodeOffset[4];
    u8 i = 0;
    u32 u32KcodeOffsetValue;

    if(u8MiuDev == 0)
    {
        u16RegMiuArb  = MIU0_RIU_ARB;
        u16RegMiuAtop = MIU0_RIU_ATOP;
        u16RegMiuDtop = MIU0_RIU_DTOP;
        u16RegMiuDtopE = MIU0_RIU_DTOP_E;
    }
    else if(u8MiuDev == 1)
    {
        u16RegMiuArb  = MIU1_RIU_ARB;
        u16RegMiuAtop = MIU1_RIU_ATOP;
        u16RegMiuDtop = MIU1_RIU_DTOP;
        u16RegMiuDtopE = MIU1_RIU_DTOP_E;
    }
    else if(u8MiuDev == 2)
    {
        u16RegMiuArb  = MIU2_RIU_ARB;
        u16RegMiuAtop = MIU2_RIU_ATOP;
        u16RegMiuDtop = MIU2_RIU_DTOP;
        u16RegMiuDtopE = MIU2_RIU_DTOP_E;
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
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x8000);

    /* DPAT engine pre-setting */
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0000);

    //sw patch for 4x mode hang bug
    if (u8MiuDev == 0)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
        MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x0000);
    }
    else if (u8MiuDev == 1)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
        MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x0000);
    }
    else
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
        MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x8000);
    }

    MHal_MIU_WritReg16(u16RegMiuDtopE, 0x80, 0x0040);
    MHal_MIU_WritReg16(u16RegMiuDtopE, 0x80, 0x8040);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0200);  // bist length
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);  // bist length
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x20);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0001);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0);
    }while((u16RegVal0 & 0x8000) == 0);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x7c, 0x800b);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c61);

    /* STEP 1: Set RX DQS Phase = 1/2* Kcode + offset */
    //MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    //MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);


#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    /* STEP 2 : Enable TX DQ deskew scan (by DPAT engine) */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0090);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc8, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xca, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xcc, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0002);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuArb, 0x78);
    }while((u16RegVal0 & 0x8000) == 0);

    /* STEP 3 : Read TX DQ deskew result & write to REGs */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0004);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd2, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16RegVal1 = 0x0000 | (u16RegVal0 & 0x00ff);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd8, u16RegVal1);
    u16RegVal1 = 0x0000 | (u16RegVal0 >> 8);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe8, u16RegVal1);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0005);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd6, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x2e, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0006);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe2, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0007);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe6, u16RegVal0);
#endif

#ifdef MIU_ENABLE_AUTO_READ_PHASE
    /* STEP 4 : Enable RX DQ/DQS deskew (by DPAT engine) */
    //MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d80);

    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c0);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);


    //MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf8, MHal_MIU_ReadReg16(MIU1_RIU_DTOP, 0xf8) & ~0x8000);
    //if (u8MiuDev == 0)
    //{
    //    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    //    MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x0000);
    //}
    //else if (u8MiuDev == 1)
    //{
    //    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
    //    MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x0000);
    //}
    //else
    //{
    //    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    //    MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x8000);
    //}
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0001);
    //
    //do{
    //    u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0);
    //}while((u16RegVal0 & 0x8000) == 0);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xc0, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xc2, 0x1fff);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x2e, 0x0001);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x20, 0x0001);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xd2, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd2) | 0x0001);

    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c0);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x2000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c61);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0x00f0);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0xffff);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0xffff);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x000f);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    //sw patch for 4x mode hang bug
    do{
        u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e);
    }while((u16RegVal0 & 0xf) == 0x0d);

    delayus(1000);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c60);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuArb, 0x78);
    }while((u16RegVal0 & 0x8000) == 0);

    MHal_MIU_WritReg16(u16RegMiuDtopE, 0x80, 0x0040);
    MHal_MIU_WritReg16(u16RegMiuDtopE, 0x80, 0x0010);

    /* STEP 5 : Read RX DQ deskew result & write to REGs */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0000);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa2, u16RegVal0);
    u16DQSMaxCenter[0] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[0] = u16DQSMaxCenter[0] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0001);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa6, u16RegVal0);
    u16DQSMaxCenter[1] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[1] = u16DQSMaxCenter[1] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0002);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa8, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xaa, u16RegVal0);
    u16DQSMaxCenter[2] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[2] = u16DQSMaxCenter[2] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0003);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xac, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
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
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c60);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x0000);

    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xc0, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xc2, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x2e, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x20, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xd2, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd2) & ~(0x0001));



    /* Disable SI mode */
//    u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
//    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 | 0x0800);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) & ~0x01);

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

u8 CalculateDrvStength(u8 DrvStength, u8 DefDrv)
{
    u8 DefTemp;
    if ((DrvStength == 0x7) || (DrvStength == 0x3) || (DrvStength == 0x0))
    {
        return DefDrv;
    }

    DefTemp = DefDrv;
    if ((DrvStength & 0x4) == 0x4)
    {
        DefTemp += (DrvStength & 0x03);
    }
    else
    {
        DefTemp -= (DrvStength & 0x03);
    }

    if ((DefDrv < 8) && (DefTemp >= 8))
    {
        DefTemp += 5;
    }
    else if ((DefDrv >= 8) && (DefTemp < 8))
    {
        DefTemp -= 5;
    }

    return DefTemp;
}

void BootRom_MiuDdrTrim(void)
{
    u8  u8MiuDrvCmd_n=0;
    u8  u8MiuDrvCs_n=0;
    u8  u8MiuDrvCmd_p=0;
    u8  u8MiuDrvCs_p=0;

    u16 u16EfuseVale=0;

    // read efuse
    MHal_EFUSE_WritReg16(0x50, 0x0148);
    MHal_EFUSE_WritReg16(0x50, 0x2148);
    while((MHal_EFUSE_ReadReg16(0x50) & 0x2000) != 0);

    u16EfuseVale = MHal_EFUSE_ReadReg16(0x58);

//MIU_DRVP_0:
    if ((u16EfuseVale & 0x8) == 0)
    {
        goto MIU_DRVN_0;
    }
    u8MiuDrvCmd_p = ((MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0x5E) >> 9) & 0x7) | ((MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xDE) >> 9) & 0x8);
    u8MiuDrvCmd_p = CalculateDrvStength(u16EfuseVale & 0x7, u8MiuDrvCmd_p);
    MHal_MIU_WritReg16(MIU0_RIU_ATOP, 0x5E, (MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0x5E)
                       & (~(0x7 << 9))) | ((u8MiuDrvCmd_p & 0x7) << 9));
    MHal_MIU_WritReg16(MIU0_RIU_ATOP, 0xDE, (MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xDE)
                       & (~(0x8 << 9))) | ((u8MiuDrvCmd_p & 0x8) << 9));

    u8MiuDrvCs_p = ((MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0x58) >> 9) & 0x7) | ((MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xDE) >> 11) & 0x8);
    u8MiuDrvCs_p = CalculateDrvStength(u16EfuseVale & 0x7, u8MiuDrvCs_p);
    MHal_MIU_WritReg16(MIU0_RIU_ATOP, 0x58, (MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0x58)
                       & (~(0x7 << 9))) | ((u8MiuDrvCs_p & 0x7) << 9));
    MHal_MIU_WritReg16(MIU0_RIU_ATOP, 0xDE, (MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xDE)
                       & (~(0x8 << 11))) | ((u8MiuDrvCs_p & 0x8) << 11));

MIU_DRVN_0:
    if ((u16EfuseVale & 0x80) == 0)
    {
        goto MIU_DRVP_1;
    }

    u8MiuDrvCmd_n = ((MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0x5C) >> 9) & 0x7) | ((MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xDE) >> 1) & 0x8);
    u8MiuDrvCmd_n = CalculateDrvStength((u16EfuseVale >> 4) & 0x7, u8MiuDrvCmd_n);
    MHal_MIU_WritReg16(MIU0_RIU_ATOP, 0x5C, (MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0x5C)
                       & (~(0x7 << 9))) | ((u8MiuDrvCmd_n & 0x7) << 9));
    MHal_MIU_WritReg16(MIU0_RIU_ATOP, 0xDE, (MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xDE)
                       & (~(0x8 << 1))) | ((u8MiuDrvCmd_n & 0x8) << 1));

    u8MiuDrvCs_n = ((MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0x58) >> 1) & 0x7) | ((MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xDE) >> 3) & 0x8);
    u8MiuDrvCs_n = CalculateDrvStength((u16EfuseVale >> 4) & 0x7, u8MiuDrvCs_n);
    MHal_MIU_WritReg16(MIU0_RIU_ATOP, 0x58, (MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0x58)
                       & (~(0x7 << 1))) | ((u8MiuDrvCs_n & 0x7) << 1));
    MHal_MIU_WritReg16(MIU0_RIU_ATOP, 0xDE, (MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xDE)
                       & (~(0x8 << 3))) | ((u8MiuDrvCs_n & 0x8) << 3));

MIU_DRVP_1:
    if ((u16EfuseVale & 0x800) == 0)
    {
        goto MIU_DRVN_1;
    }

    u8MiuDrvCmd_p = ((MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0x5E) >> 9) & 0x7) | ((MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0xDE) >> 9) & 0x8);
    u8MiuDrvCmd_p = CalculateDrvStength((u16EfuseVale >> 8) & 0x7, u8MiuDrvCmd_p);
    MHal_MIU_WritReg16(MIU1_RIU_ATOP, 0x5E, (MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0x5E)
                       & (~(0x7 << 9))) | ((u8MiuDrvCmd_p & 0x7) << 9));
    MHal_MIU_WritReg16(MIU1_RIU_ATOP, 0xDE, (MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0xDE)
                       & (~(0x8 << 9))) | ((u8MiuDrvCmd_p & 0x8) << 9));

    u8MiuDrvCs_p = ((MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0x58) >> 9) & 0x7) | ((MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0xDE) >> 11) & 0x8);
    u8MiuDrvCs_p = CalculateDrvStength((u16EfuseVale >> 8) & 0x7, u8MiuDrvCs_p);
    MHal_MIU_WritReg16(MIU1_RIU_ATOP, 0x58, (MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0x58)
                       & (~(0x7 << 9))) | ((u8MiuDrvCs_p & 0x7) << 9));
    MHal_MIU_WritReg16(MIU1_RIU_ATOP, 0xDE, (MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0xDE)
                       & (~(0x8 << 11))) | ((u8MiuDrvCs_p & 0x8) << 11));

MIU_DRVN_1:
    if ((u16EfuseVale & 0x8000) == 0)
    {
        goto exit;
    }

    u8MiuDrvCmd_n = ((MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0x5C) >> 9) & 0x7) | ((MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0xDE) >> 1) & 0x8);
    u8MiuDrvCmd_n = CalculateDrvStength((u16EfuseVale >> 12)  & 0x7, u8MiuDrvCmd_n);
    MHal_MIU_WritReg16(MIU1_RIU_ATOP, 0x5C, (MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0x5C)
                       & (~(0x7 << 9))) | ((u8MiuDrvCmd_n & 0x7) << 9));
    MHal_MIU_WritReg16(MIU1_RIU_ATOP, 0xDE, (MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0xDE)
                       & (~(0x8 << 1))) | ((u8MiuDrvCmd_n & 0x8) << 1));

    u8MiuDrvCs_n = ((MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0x58) >> 1) & 0x7) | ((MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0xDE) >> 3) & 0x8);
    u8MiuDrvCs_n = CalculateDrvStength((u16EfuseVale >> 12)  & 0x7, u8MiuDrvCs_n);
    MHal_MIU_WritReg16(MIU1_RIU_ATOP, 0x58, (MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0x58)
                       & (~(0x7 << 1))) | ((u8MiuDrvCs_n & 0x7) << 1));
    MHal_MIU_WritReg16(MIU1_RIU_ATOP, 0xDE, (MHal_MIU_ReadReg16(MIU1_RIU_ATOP, 0xDE)
                       & (~(0x8 << 3))) | ((u8MiuDrvCs_n & 0x8) << 3));
exit:
    return;
}
