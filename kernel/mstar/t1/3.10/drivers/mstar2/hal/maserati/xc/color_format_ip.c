/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    color_format_ip.c
/// @brief  MStar XC Driver DDI HAL Level
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_COLOR_FORMAT_IP_C
#define _HAL_COLOR_FORMAT_IP_C
#endif
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include "mdrv_mstypes.h"
#include "color_format_input.h"
#include "color_format_ip.h"
#include "color_format_driver.h"
#include "mhal_dlc.h"
#include "mhal_xc.h"

#define AUTODOWNLOAD
#define THREEDLUTAUTODOWNLOAD
//#define VOP2AUTODOWNLOAD
////////Golbal Variable For UI control//////////////
#define MAIN_WINDOW     0
#define SUB_WINDOW      1
#define DWORD MS_U32
#define WORD MS_U16
#define BYTE MS_U8
#define BOOL MS_BOOL
#define U8 MS_U8
MS_S32 Typ3x3Array[3][3] =
{
    {  78740, 50000,        0},
    {-23406, 50000, -9365},
    {         0, 50000, 92780}
};//Initial Y2Rfull709
MS_U8 u8CscIpType = E_CSC_MANHATTAN_OUTPUT;

typedef struct
{
    WORD wContrast;
    WORD wRCon;
    WORD wGCon;
    WORD wBCon;
    BYTE ucSaturation;
    BYTE ucHue;
    MS_S32 sContrastRGBMatrix[3][3];
    short sVideoSatHueMatrix[3][3];
    short* psColorCorrectionMatrix; // this variable must pointer to a array[3][3]
    short* psPCsRGBMatrix; // sRGB matrix for PC, this variable must pointer to a array[3][3]
    MS_S32* psYVUtoRGBMatrix; // this variable must pointer to a array[3][3]
    BYTE bForceYUVtoRGB : 1;
    BYTE ucYUVtoRGBMatrixSel : 2;
    MS_U8 u8CscIpType;
} StruAceInfo;
#define ENABLE_SINE_COSINE_USING_LOOKUP_TABLE   1
static StruAceInfo s_AceInfo[2];
static BOOL bBypassColorMatrix = FALSE;
extern ST_KDRV_XC_CFD_PANEL _stCfdPanel;
MS_U8 u8InEqualOutCP;
void mD3dLutWrite2Register(MS_U16* pu16Lut)
{

    //pRegTable->u32Size = DOVI_3DLUT_SIZE;
    /*pRegTable->s16MinY = pConfig->s16Lut3DMin[0];
    pRegTable->s16MaxY = pConfig->s16Lut3DMax[0];
    pRegTable->u32InvY = ((MS_U32)1 << 30) / (pRegTable->s16MaxY - pRegTable->s16MinY);
    pRegTable->s16MinC = pConfig->s16Lut3DMin[1];
    pRegTable->s16MaxC = pConfig->s16Lut3DMax[1];
    pRegTable->u32InvC = ((MS_U32)1 << 30) / (pRegTable->s16MaxC - pRegTable->s16MinC);*/


    write2Bytes(DoVi_Addr_L(0x7A, 0x51), 0xC001); //iMinC
/*
    writeByteMask(_PK_L_(0x7A,0x02),0x10,0x00); //3D LUT enable
    write2Bytes(DoVi_Addr_L(0x7A, 0x50), 0x3fff); //iMaxC
    write2Bytes(DoVi_Addr_L(0x7A, 0x51), 0xC001); //iMinC
    write4Bytes(DoVi_Addr_L(0x7A, 0x52), 0x8000); //idistc_invs



    write2Bytes(DoVi_Addr_L(0x7A, 0x55), 0x7fff); //iMaxY
    write2Bytes(DoVi_Addr_L(0x7A, 0x54), 0x0000); //iMinY
    write4Bytes(DoVi_Addr_L(0x7A, 0x56), 0x8001); //idistc_invs
*/
#ifdef THREEDLUTAUTODOWNLOAD//    if (E_HdrWriteLutMethod == EN_HDR_WRITE_LUT_AUTODOWNLOAD)
    {

        ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO stAutoDownloadFormat;
        stAutoDownloadFormat.enSubClient = E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_3DLUT;
        stAutoDownloadFormat.bEnableRange = 1;
        stAutoDownloadFormat.u16StartAddr = 0;
        stAutoDownloadFormat.u16EndAddr = DOVI_3DLUT_SIZE - 1;
        writeByteMask(DoVi_Addr_H(0x7A, 0x70),0x80, 0x80);  // AUTODOWNLOAD En
        if(1 == MHal_XC_WriteAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR, (MS_U8*)pu16Lut,3* DOVI_3DLUT_SIZE * sizeof(MS_U16), (void*)&stAutoDownloadFormat) )
        {
            //printk("3D Lut WriteAutodownload 1\n");
#if 0
            if(1 == MHal_XC_FireAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR))
                printk("3D LUT TMO AutoDownload successful!\n");
            else
                printk("#D LUT Gamma AutoDownload FAILED!\n");
#endif
        }
        else
        {
            printk("3D Lut WriteAutodownload fail\n");
        }




    }
#else//    else if (E_HdrWriteLutMethod == EN_HDR_WRITE_LUT_RIU)
    {
        writeByteMask(DoVi_Addr_H(0x7A, 0x70),0x80, 0); // AutoDownload off

        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<6, 0x0); // lut_rd_en
        writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x3, 0x3); // lut_sel
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<5, 0x0<<5); // lut_wd_dup_md=0
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x10, 0x0); // lut_fast_md off
        //writeByteMask(DoVi_Addr_H(7A, 60), 0x1, 0x1, 4); // lut_fast_md
        //write2Bytes(DoVi_Addr_L(7A, 69), 0); // lut_init_addr
        //FILE *fopen("mstar_3dlut.bin", "r");
        //FILE *fp;
        //fp=fopen("D:/projects/3dLutGenerator/Release/mstar_3dlut.bin", "r");
        //fp=fopen("C:/Users/nawwar.ali/Downloads/colorMapMD/mstar_3dlut_full2limit_vuy.bin", "r");

        //MS_U16 pu16Lut[DOVI_3DLUT_SIZE*3];
        //fread(pu16Lut, sizeof(pu16Lut[0]), sizeof(pu16Lut)/sizeof(pu16Lut[0]), fp);
        //*pu16Lut = *u16Full2limit3dLut;
        //MS_U16* pu16Lut = pRegTable->pu16Table;
        MS_U32 u32Idx = 0, u32Idx2 = 0;

        // LUT 0
        writeByteMask(DoVi_Addr_L(0x7A, 0x68), 0xF, 0x0); // lut_sel_ip
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x1<<7); // lut_load_en
        for (u32Idx = 0; u32Idx < DOVI_3DLUT_S0_SIZE; u32Idx++)
        {
            write2Bytes(DoVi_Addr_L(0x7A, 0x61), u32Idx); // lut_addr

            write2Bytes(DoVi_Addr_L(0x7A, 0x66), pu16Lut[u32Idx2]); // wd0
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x64), pu16Lut[u32Idx2]); // wd1
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x62), pu16Lut[u32Idx2]); // wd2
            u32Idx2++;
            writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x1<<4, 0x1<<4); // lut_w_pulse
            //DoVi_WaitByteMask(DoVi_Addr_H(7A, 60), 0x1, 0); // wait lut_flag_rb
            writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x2, 0x2); // lut_flag_clr
        }
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x0); // lut_load_en

        // LUT 1
        writeByteMask(DoVi_Addr_L(0x7A, 0x68), 0xF, 0x1); // lut_sel_ip
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x1<<7); // lut_load_en
        for (u32Idx = 0; u32Idx < DOVI_3DLUT_S1_SIZE; u32Idx++)
        {
            write2Bytes(DoVi_Addr_L(0x7A, 0x61), u32Idx); // lut_addr
            write2Bytes(DoVi_Addr_L(0x7A, 0x66), pu16Lut[u32Idx2]); // wd0
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x64), pu16Lut[u32Idx2]); // wd1
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x62), pu16Lut[u32Idx2]); // wd2
            u32Idx2++;
            writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x1<<4, 0x1<<4); // lut_w_pulse
            //DoVi_WaitByteMask(DoVi_Addr_H(7A, 60), 0x1, 0); // wait lut_flag_rb
            writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x2, 0x2); // lut_flag_clr
        }
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x0); // lut_load_en

        // LUT 2
        writeByteMask(DoVi_Addr_L(0x7A, 0x68), 0xF, 0x2); // lut_sel_ip
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x1<<7); // lut_load_en
        for (u32Idx = 0; u32Idx < DOVI_3DLUT_S2_SIZE; u32Idx++)
        {
            write2Bytes(DoVi_Addr_L(0x7A, 0x61), u32Idx); // lut_addr
            write2Bytes(DoVi_Addr_L(0x7A, 0x66), pu16Lut[u32Idx2]); // wd0
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x64), pu16Lut[u32Idx2]); // wd1
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x62), pu16Lut[u32Idx2]); // wd2
            u32Idx2++;
            writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x1<<4, 0x1<<4); // lut_w_pulse
            //DoVi_WaitByteMask(DoVi_Addr_H(7A, 60), 0x1, 0); // wait lut_flag_rb
            writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x2, 0x2); // lut_flag_clr
        }
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x0); // lut_load_en

        // LUT 3
        writeByteMask(DoVi_Addr_L(0x7A, 0x68), 0xF, 0x3); // lut_sel_ip
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x1<<7); // lut_load_en
        for (u32Idx = 0; u32Idx < DOVI_3DLUT_S3_SIZE; u32Idx++)
        {
            write2Bytes(DoVi_Addr_L(0x7A, 0x61), u32Idx); // lut_addr
            write2Bytes(DoVi_Addr_L(0x7A, 0x66), pu16Lut[u32Idx2]); // wd0
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x64), pu16Lut[u32Idx2]); // wd1
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x62), pu16Lut[u32Idx2]); // wd2
            u32Idx2++;
            writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x1<<4, 0x1<<4); // lut_w_pulse
            //DoVi_WaitByteMask(DoVi_Addr_H(7A, 60), 0x1, 0); // wait lut_flag_rb
            writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x2, 0x2); // lut_flag_clr
        }
        writeByteMask(DoVi_Addr_H(0x7A,0x60), 0x1<<7, 0x0); // lut_load_en

        // LUT 4
        writeByteMask(DoVi_Addr_L(0x7A, 0x68), 0xF, 0x4); // lut_sel_ip
        writeByteMask(DoVi_Addr_H(0x7A,0x60), 0x1<<7, 0x1<<7); // lut_load_en
        for (u32Idx = 0; u32Idx < DOVI_3DLUT_S4_SIZE; u32Idx++)
        {
            write2Bytes(DoVi_Addr_L(0x7A, 0x61), u32Idx); // lut_addr
            write2Bytes(DoVi_Addr_L(0x7A, 0x66), pu16Lut[u32Idx2]); // wd0
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x64), pu16Lut[u32Idx2]); // wd1
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x62), pu16Lut[u32Idx2]); // wd2
            u32Idx2++;
            writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x1<<4, 0x1<<4); // lut_w_pulse
            //DoVi_WaitByteMask(DoVi_Addr_H(7A, 60), 0x1, 0); // wait lut_flag_rb
            writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x2, 0x2); // lut_flag_clr
        }
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x0); // lut_load_en

        // LUT 5
        writeByteMask(DoVi_Addr_L(0x7A, 0x68), 0xF, 0x5); // lut_sel_ip
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x1<<7); // lut_load_en
        for (u32Idx = 0; u32Idx < DOVI_3DLUT_S5_SIZE; u32Idx++)
        {
            write2Bytes(DoVi_Addr_L(0x7A, 0x61), u32Idx); // lut_addr
            write2Bytes(DoVi_Addr_L(0x7A, 0x66), pu16Lut[u32Idx2]); // wd0
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x64), pu16Lut[u32Idx2]); // wd1
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x62), pu16Lut[u32Idx2]); // wd2
            u32Idx2++;
            writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x1<<4, 0x1<<4); // lut_w_pulse
            //DoVi_WaitByteMask(DoVi_Addr_H(7A, 60), 0x1, 0); // wait lut_flag_rb
            writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x2, 0x2); // lut_flag_clr
        }
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x0); // lut_load_en

        // LUT 6
        writeByteMask(DoVi_Addr_L(0x7A, 0x68), 0xF, 0x6); // lut_sel_ip
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x1<<7); // lut_load_en
        for (u32Idx = 0; u32Idx < DOVI_3DLUT_S6_SIZE; u32Idx++)
        {
            write2Bytes(DoVi_Addr_L(0x7A, 0x61), u32Idx); // lut_addr
            write2Bytes(DoVi_Addr_L(0x7A, 0x66), pu16Lut[u32Idx2]); // wd0
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x64), pu16Lut[u32Idx2]); // wd1
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x62), pu16Lut[u32Idx2]); // wd2
            u32Idx2++;
            writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x1<<4, 0x1<<4); // lut_w_pulse
            //DoVi_WaitByteMask(DoVi_Addr_H(7A, 60), 0x1, 0); // wait lut_flag_rb
            writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x2, 0x2); // lut_flag_clr
        }
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x0); // lut_load_en

        // LUT 7
        writeByteMask(DoVi_Addr_L(0x7A, 0x68), 0xF, 0x7); // lut_sel_ip
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x1<<7); // lut_load_en
        for (u32Idx = 0; u32Idx < DOVI_3DLUT_S7_SIZE; u32Idx++)
        {
            write2Bytes(DoVi_Addr_L(0x7A, 0x61), u32Idx); // lut_addr
            write2Bytes(DoVi_Addr_L(0x7A, 0x66), pu16Lut[u32Idx2]); // wd0
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x64), pu16Lut[u32Idx2]); // wd1
            u32Idx2++;
            write2Bytes(DoVi_Addr_L(0x7A, 0x62), pu16Lut[u32Idx2]); // wd2
            u32Idx2++;
            writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x1<<4, 0x1<<4); // lut_w_pulse
            //DoVi_WaitByteMask(DoVi_Addr_H(7A, 60), 0x1, 0); // wait lut_flag_rb
            writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x2, 0x2); // lut_flag_clr
        }
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x1<<7, 0x0); // lut_load_en
    }
#endif
}
const MS_S16 cosine[101] =
{
    658, // 0
    671, // 1
    685, // 2
    698, // 3
    711, // 4
    724, // 5
    736, // 6
    748, // 7
    760, // 8
    772, // 9
    784, // 10
    795, // 11
    806, // 12
    817, // 13
    828, // 14
    838, // 15
    848, // 16
    858, // 17
    868, // 18
    877, // 19
    886, // 20
    895, // 21
    904, // 22
    912, // 23
    920, // 24
    928, // 25
    935, // 26
    942, // 27
    949, // 28
    955, // 29
    962, // 30
    968, // 31
    973, // 32
    979, // 33
    984, // 34
    989, // 35
    993, // 36
    997, // 37
    1001, // 38
    1005, // 39
    1008, // 40
    1011, // 41
    1014, // 42
    1016, // 43
    1018, // 44
    1020, // 45
    1021, // 46
    1022, // 47
    1023, // 48
    1023, // 49
    1024, // 50
    1023, // 51
    1023, // 52
    1022, // 53
    1021, // 54
    1020, // 55
    1018, // 56
    1016, // 57
    1014, // 58
    1011, // 59
    1008, // 60
    1005, // 61
    1001, // 62
    997, // 63
    993, // 64
    989, // 65
    984, // 66
    979, // 67
    973, // 68
    968, // 69
    962, // 70
    955, // 71
    949, // 72
    942, // 73
    935, // 74
    928, // 75
    920, // 76
    912, // 77
    904, // 78
    895, // 79
    886, // 80
    877, // 81
    868, // 82
    858, // 83
    848, // 84
    838, // 85
    828, // 86
    817, // 87
    806, // 88
    795, // 89
    784, // 90
    772, // 91
    760, // 92
    748, // 93
    736, // 94
    724, // 95
    711, // 96
    698, // 97
    685, // 98
    671, // 99
    658 // 100
};
const MS_S16 sine[101] =
{
    784, // 0
    772, // 1
    760, // 2
    748, // 3
    736, // 4
    724, // 5
    711, // 6
    698, // 7
    685, // 8
    671, // 9
    658, // 10
    644, // 11
    630, // 12
    616, // 13
    601, // 14
    587, // 15
    572, // 16
    557, // 17
    542, // 18
    527, // 19
    511, // 20
    496, // 21
    480, // 22
    464, // 23
    448, // 24
    432, // 25
    416, // 26
    400, // 27
    383, // 28
    366, // 29
    350, // 30
    333, // 31
    316, // 32
    299, // 33
    282, // 34
    265, // 35
    247, // 36
    230, // 37
    212, // 38
    195, // 39
    177, // 40
    160, // 41
    142, // 42
    124, // 43
    107, // 44
    89, // 45
    71, // 46
    53, // 47
    35, // 48
    17, // 49
    0, // 50
    -17, // 51
    -35, // 52
    -53, // 53
    -71, // 54
    -89, // 55
    -107, // 56
    -124, // 57
    -142, // 58
    -160, // 59
    -177, // 60
    -195, // 61
    -212, // 62
    -230, // 63
    -247, // 64
    -265, // 65
    -282, // 66
    -299, // 67
    -316, // 68
    -333, // 69
    -350, // 70
    -366, // 71
    -383, // 72
    -400, // 73
    -416, // 74
    -432, // 75
    -448, // 76
    -464, // 77
    -480, // 78
    -496, // 79
    -512, // 80
    -527, // 81
    -542, // 82
    -557, // 83
    -572, // 84
    -587, // 85
    -601, // 86
    -616, // 87
    -630, // 88
    -644, // 89
    -658, // 90
    -671, // 91
    -685, // 92
    -698, // 93
    -711, // 94
    -724, // 95
    -736, // 96
    -748, // 97
    -760, // 98
    -772, // 99
    -784 // 100
};
const MS_U16 u16PQCurveExpLut_ali[705] =
{
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
    0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
    0x1, 0x1, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3,
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
    0x3, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
    0x4, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5,
    0x5, 0x5, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7,
    0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x8, 0x8,
    0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0x9,
    0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0xA, 0xA, 0xA, 0xA, 0xA,
    0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xB, 0xB, 0xB, 0xB, 0xB,
    0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xC, 0xC, 0xC, 0xC,
    0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD,
    0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
    0x13
};

const MS_U16 u16PQCurveMantLut_ali[705] =
{
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
    0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x5, 0x5, 0x5,
    0x5, 0x6, 0x6, 0x6, 0x7, 0x7, 0x7, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0xA, 0xA, 0xA,
    0xB, 0xB, 0xB, 0xC, 0xC, 0xD, 0xD, 0xD, 0xE, 0xE, 0xF, 0xF, 0x10, 0x10, 0x11, 0x11,
    0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x17, 0x18, 0x18, 0x19,
    0x19, 0x1A, 0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1F, 0x1F, 0x20, 0x20, 0x21, 0x21, 0x22,
    0x23, 0x23, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x28, 0x28, 0x29, 0x2A, 0x2A, 0x2B, 0x2C, 0x2C,
    0x2D, 0x2E, 0x2E, 0x2F, 0x30, 0x31, 0x31, 0x32, 0x33, 0x33, 0x34, 0x35, 0x36, 0x36, 0x37, 0x38,
    0x39, 0x3A, 0x3C, 0x3D, 0x3F, 0x40, 0x42, 0x44, 0x45, 0x47, 0x48, 0x4A, 0x4C, 0x4E, 0x4F, 0x51,
    0x53, 0x55, 0x56, 0x58, 0x5A, 0x5C, 0x5E, 0x60, 0x62, 0x63, 0x65, 0x67, 0x69, 0x6B, 0x6D, 0x6F,
    0x71, 0x73, 0x75, 0x77, 0x7A, 0x7C, 0x7E, 0x80, 0x82, 0x84, 0x86, 0x89, 0x8B, 0x8D, 0x8F, 0x91,
    0x94, 0x96, 0x98, 0x9B, 0x9D, 0x9F, 0xA2, 0xA4, 0xA7, 0xA9, 0xAB, 0xAE, 0xB0, 0xB3, 0xB5, 0xB8,
    0xBA, 0xBF, 0xC5, 0xCA, 0xCF, 0xD4, 0xDA, 0xDF, 0xE5, 0xEA, 0xF0, 0xF6, 0xFB, 0x101, 0x107, 0x10D,
    0x113, 0x119, 0x11F, 0x126, 0x12C, 0x132, 0x139, 0x13F, 0x146, 0x14C, 0x153, 0x159, 0x160, 0x167, 0x16E, 0x175,
    0x17C, 0x183, 0x18A, 0x191, 0x198, 0x1A0, 0x1A7, 0x1AF, 0x1B6, 0x1BE, 0x1C5, 0x1CD, 0x1D5, 0x1DC, 0x1E4, 0x1EC,
    0x1F4, 0x1FC, 0x204, 0x20D, 0x215, 0x21D, 0x225, 0x22E, 0x236, 0x23F, 0x247, 0x250, 0x259, 0x262, 0x26A, 0x273,
    0x27C, 0x28E, 0x2A1, 0x2B4, 0x2C6, 0x2DA, 0x2ED, 0x301, 0x314, 0x329, 0x33D, 0x352, 0x366, 0x37C, 0x391, 0x3A7,
    0x3BD, 0x3D3, 0x3E9, 0x400, 0x417, 0x42E, 0x445, 0x45D, 0x475, 0x48D, 0x4A6, 0x4BF, 0x4D8, 0x4F1, 0x50A, 0x524,
    0x53E, 0x559, 0x573, 0x58E, 0x5A9, 0x5C4, 0x5E0, 0x5FC, 0x618, 0x635, 0x651, 0x66E, 0x68B, 0x6A9, 0x6C7, 0x6E5,
    0x703, 0x722, 0x741, 0x760, 0x77F, 0x79F, 0x7BF, 0x7DF, 0x800, 0x820, 0x841, 0x863, 0x884, 0x8A6, 0x8C8, 0x8EB,
    0x90E, 0x954, 0x99B, 0x9E4, 0xA2E, 0xA79, 0xAC5, 0xB12, 0xB60, 0xBB0, 0xC00, 0xC52, 0xCA5, 0xCF9, 0xD4F, 0xDA6,
    0xDFD, 0xE56, 0xEB1, 0xF0C, 0xF69, 0xFC7, 0x1026, 0x1086, 0x10E8, 0x114B, 0x11AF, 0x1214, 0x127B, 0x12E3, 0x134C, 0x13B7,
    0x1423, 0x1490, 0x14FE, 0x156E, 0x15DF, 0x1651, 0x16C5, 0x173A, 0x17B1, 0x1828, 0x18A1, 0x191C, 0x1998, 0x1A15, 0x1A93, 0x1B13,
    0x1B95, 0x1C17, 0x1C9B, 0x1D21, 0x1DA8, 0x1E30, 0x1EBA, 0x1F45, 0x1FD2, 0x2060, 0x20EF, 0x2180, 0x2213, 0x22A7, 0x233C, 0x23D3,
    0x246B, 0x25A1, 0x26DC, 0x281D, 0x2965, 0x2AB2, 0x2C06, 0x2D60, 0x2EC1, 0x3028, 0x3195, 0x3309, 0x3483, 0x3604, 0x378C, 0x391A,
    0x3AB0, 0x3C4C, 0x3DEF, 0x3F99, 0x20A5, 0x2181, 0x2260, 0x2344, 0x242A, 0x2515, 0x2603, 0x26F5, 0x27EA, 0x28E3, 0x29E1, 0x2AE1,
    0x2BE6, 0x2CEF, 0x2DFB, 0x2F0C, 0x3020, 0x3139, 0x3256, 0x3376, 0x349B, 0x35C4, 0x36F1, 0x3822, 0x3958, 0x3A91, 0x3BCF, 0x3D12,
    0x3E58, 0x3FA4, 0x207A, 0x2124, 0x21D0, 0x227F, 0x232F, 0x23E3, 0x2498, 0x2550, 0x260A, 0x26C6, 0x2785, 0x2847, 0x290A, 0x29D0,
    0x2A99, 0x2C32, 0x2DD4, 0x2F81, 0x3139, 0x32FA, 0x34C7, 0x369E, 0x3880, 0x3A6D, 0x3C66, 0x3E6A, 0x203D, 0x214A, 0x225E, 0x2378,
    0x2497, 0x25BD, 0x26EA, 0x281C, 0x2955, 0x2A95, 0x2BDB, 0x2D28, 0x2E7C, 0x2FD7, 0x3138, 0x32A1, 0x3411, 0x3588, 0x3707, 0x388D,
    0x3A1B, 0x3BB0, 0x3D4D, 0x3EF2, 0x2050, 0x212A, 0x2209, 0x22EC, 0x23D3, 0x24BE, 0x25AD, 0x26A1, 0x2799, 0x2896, 0x2997, 0x2A9D,
    0x2BA8, 0x2CB7, 0x2DCB, 0x2EE3, 0x3001, 0x3123, 0x324B, 0x3377, 0x34A9, 0x35E0, 0x371C, 0x385D, 0x39A4, 0x3AF0, 0x3C42, 0x3D99,
    0x3EF6, 0x20E0, 0x2251, 0x23CE, 0x2558, 0x26ED, 0x2890, 0x2A3F, 0x2BFC, 0x2DC7, 0x2F9F, 0x3186, 0x337C, 0x3581, 0x3795, 0x39B8,
    0x3BEC, 0x3E30, 0x2043, 0x2176, 0x22B2, 0x23F6, 0x2544, 0x269C, 0x27FC, 0x2967, 0x2ADB, 0x2C5A, 0x2DE3, 0x2F77, 0x3116, 0x32C0,
    0x3475, 0x3635, 0x3802, 0x39DA, 0x3BBF, 0x3DB1, 0x3FAF, 0x20DD, 0x21EA, 0x22FD, 0x2417, 0x2539, 0x2661, 0x2791, 0x28C9, 0x2A08,
    0x2B4F, 0x2C9F, 0x2DF6, 0x2F56, 0x30BE, 0x322F, 0x33A9, 0x352C, 0x36B9, 0x384E, 0x39EE, 0x3B97, 0x3D4A, 0x3F08, 0x2068, 0x2151,
    0x2240, 0x242E, 0x2633, 0x284F, 0x2A85, 0x2CD4, 0x2F3F, 0x31C5, 0x3468, 0x3729, 0x3A09, 0x3D09, 0x2016, 0x21B9, 0x236D, 0x2535,
    0x2710, 0x2900, 0x2B04, 0x2D1E, 0x2F4F, 0x3198, 0x33F9, 0x3673, 0x3908, 0x3BB7, 0x3E83, 0x20B7, 0x223A, 0x23CE, 0x2572, 0x2727,
    0x28ED, 0x2AC6, 0x2CB1, 0x2EB0, 0x30C4, 0x32EC, 0x352A, 0x377F, 0x39EB, 0x3C6F, 0x3F0D, 0x20E2, 0x224C, 0x23C3, 0x2548, 0x26DD,
    0x2881, 0x2A36, 0x2BFB, 0x2DD1, 0x2FB9, 0x31B4, 0x33C2, 0x35E3, 0x381A, 0x3A66, 0x3CC8, 0x3F40, 0x20E9, 0x223D, 0x239E, 0x250D,
    0x2689, 0x29AC, 0x2D0C, 0x30AC, 0x3493, 0x38C4, 0x3D46, 0x210F, 0x23A9, 0x2675, 0x2975, 0x2CAF, 0x3026, 0x33DE, 0x37DD, 0x3C27,
    0x2061, 0x22DA, 0x2582, 0x285B, 0x2B6A, 0x2EB3, 0x323A, 0x3603, 0x3A14, 0x3E71, 0x2190, 0x2414, 0x26C7, 0x29AE, 0x2CCB, 0x3023,
    0x33BA, 0x3796, 0x3BBB, 0x2017, 0x227C, 0x250E, 0x27D1, 0x2AC8, 0x2DF9, 0x3167, 0x3518, 0x390F, 0x3D53, 0x20F5, 0x236D, 0x2616,
    0x28F2, 0x2C06, 0x2F57, 0x32EA, 0x36C2, 0x3AE8, 0x3F60, 0x2219, 0x24B2, 0x277F, 0x2A85, 0x2DC8, 0x314D, 0x351B, 0x3936, 0x3DA6,
    0x2129
};
void StoreY2Rmatrix(MS_BOOL bWindow,cscParametersStructure* cscParameters)
{
    MS_U8 i,j;
    //printk("Store Original Out CSC :%s :%d\n",__FUNCTION__,__LINE__);
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
        {
            Typ3x3Array[i][j]= (cscParameters->cscCoeff[i][j]);
            //printk("%8d",Typ3x3Array[i][j]);
        }
        //printk("\n");
    }
}
static void ArrayMultiply(MS_S32 sFirst[3][3], MS_S32 sSecond[3][3], MS_S32 sResult[3][3])
{
    BYTE ucRow, ucCol;

    //go line by line
    for(ucRow=0; ucRow!=3; ucRow++)
    {
        // go column by column
        for(ucCol=0; ucCol!=3; ucCol++)
        {
            sResult[ucRow][ucCol] = (((long)sFirst[ucRow][0] * sSecond[0][ucCol]) +
                                     ((long)sFirst[ucRow][1] * sSecond[1][ucCol]) +
                                     ((long)sFirst[ucRow][2] * sSecond[2][ucCol])) >> 10;
        } // for
    } // for

}

static void SetVideoSatHueMatrix(MS_U8 bWindow )
{
    short sTmp;
    MS_U16 u16Hue;

#if( ENABLE_SINE_COSINE_USING_LOOKUP_TABLE )
    u16Hue = s_AceInfo[bWindow].ucHue;
#else
    u16Hue = ((s_AceInfo[bWindow].ucHue <= 50) ? (50 - s_AceInfo[bWindow].ucHue) : (360-(s_AceInfo[bWindow].ucHue-50)));
#endif

    sTmp = ((short)s_AceInfo[bWindow].ucSaturation * 8);
    u16Hue = max(min(u16Hue,100),0);
    s_AceInfo[bWindow].sVideoSatHueMatrix[2][2] = ((((long)cosine[u16Hue] * sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][0] = ((( (long)cosine[u16Hue]* sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[2][0] = ((((long)sine[u16Hue]   * sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][2] = (((-(long)sine[u16Hue]  * sTmp))>>10);

    s_AceInfo[bWindow].sVideoSatHueMatrix[1][1] = 1024;
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][1] = s_AceInfo[bWindow].sVideoSatHueMatrix[1][0] = s_AceInfo[bWindow].sVideoSatHueMatrix[1][2] = s_AceInfo[bWindow].sVideoSatHueMatrix[2][1] = 0;
}
static void SetVideoContrastMatrix( MS_U8 bWindow )
{
    s_AceInfo[bWindow].sContrastRGBMatrix[0][0] = ( (DWORD)s_AceInfo[bWindow].wRCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[1][1] = ( (DWORD)s_AceInfo[bWindow].wGCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[2][2] = ( (DWORD)s_AceInfo[bWindow].wBCon * (s_AceInfo[bWindow].wContrast)) >> 10;
}
void msSetVideoColorMatrix( MS_U8 bScalerWin,cscParametersStructure* cscParameters)
{
    MS_S32 sResultTmp1[3][3];
    MS_S32 sResultTmp2[3][3];
    //MS_S32* psLastResult;
    //DECLARA_BANK_VARIABLE

    U8 i,j;
    MS_S32 ColorMatrix_1[3][3];
    MS_S32 ColorMatrix_2[3][3];

    if( TRUE == bBypassColorMatrix)
    {
        return ;
    }

    //BACKUP_SC_BANK


    // Adjust hue&saturation, and then YUV to RGB
    //ArrayMultiply( (Typ3x3Array)(s_AceInfo[bScalerWin].psYVUtoRGBMatrix), s_AceInfo[bScalerWin].sVideoSatHueMatrix, sResultTmp1 );

    for ( i=0; i<3; i++ )
    {
        for ( j=0; j<3; j++ )
        {
            ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psYVUtoRGBMatrix +(i*3)+j);;
            //ColorMatrix_1[i][j]= Typ3x3Array[i][j];
            ColorMatrix_2[i][j]= s_AceInfo[bScalerWin].sVideoSatHueMatrix[i][j];
        }
    }
    ArrayMultiply(ColorMatrix_1,ColorMatrix_2, sResultTmp1 );

    ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, sResultTmp1, sResultTmp2 );
    //psLastResult = (MS_S32*)sResultTmp2;
    //printk("OSD control CSC :%s :%d\n",__FUNCTION__,__LINE__);
    for ( i=0; i<3; i++ )
    {
        for ( j=0; j<3; j++ )
        {
            cscParameters->cscCoeff[i][j] = sResultTmp2[i][j];
            //printk("%8d",cscParameters->cscCoeff[i][j]);
        }
        //printk("\n");
    }
#if 0
    // Do color correction
    if( NULL != s_AceInfo[bScalerWin].psColorCorrectionMatrix )
    {
        // ArrayMultiply( (Typ3x3Array)(s_AceInfo[bScalerWin].psColorCorrectionMatrix), sResultTmp1, sResultTmp2 );
        for ( i=0; i<3; i++ )
        {
            for ( j=0; j<3; j++ )
            {
                ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psColorCorrectionMatrix +(i*3)+j);
            }
        }
        ArrayMultiply( ColorMatrix_1, sResultTmp1, sResultTmp2 );


        // Adjust contrast-RGB
        ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, sResultTmp2, sResultTmp1 );

        psLastResult = (short*)sResultTmp1;
    }
    else // No color correction
    {
        // Adjust contrast-RGB
        ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, sResultTmp1, sResultTmp2 );
        psLastResult = (short*)sResultTmp2;
    }

    if (MDrv_ACE_IsSupportMLoad() && (!MDrv_ACE_GetSkipWaitVsync(bScalerWin)))
    {
        MDrv_ACE_WriteColorMatrixBurst( bScalerWin, (MS_U16*)psLastResult );
    }
    else

    {

        if(Mdrv_GetBT2020Mode())
        {
            msWriteColorMatrix(bScalerWin, s_AceInfo[bScalerWin].psYVUtoRGBMatrix);
        }
        else
        {
            msWriteColorMatrix( bScalerWin, psLastResult );
        }
    }


    if( bScalerWin == MAIN_WINDOW )
    {

        SET_SC_BANK(BANK_CM_MAIN);
#if(ENABLE_SW_DOUBLE_BUFFER)
        MDrv_SWDB_Write2ByteMask(REG_ADDR_CM_CTL_MAIN, ENABLE_CM_CTL1, MASK_CM_CTL);
#else
        ACE_WRITE_CM_CTL( REG_ADDR_CM_CTL_MAIN, ENABLE_CM_CTL1, MASK_CM_CTL ); // Enable color matrix&Change R/B range
#endif
        //ACE_WRITE_CM_CTL( REG_ADDR_CM_CTL_MAIN, 0x35, MASK_CM_CTL ); // Enable color matrix&Change R/B range

    }
    else // Sub window
    {
        SET_SC_BANK(BANK_CM_SUB);
#if(ENABLE_SW_DOUBLE_BUFFER)
        MDrv_SWDB_Write2ByteMask(REG_ADDR_CM_CTL_SUB, ENABLE_CM_CTL1, MASK_CM_CTL);
#else
        ACE_WRITE_CM_CTL( REG_ADDR_CM_CTL_SUB, ENABLE_CM_CTL1, MASK_CM_CTL ); // Enable color matrix&Change R/B range
#endif
        //ACE_WRITE_CM_CTL( REG_ADDR_CM_CTL_SUB, 0x35, MASK_CM_CTL ); // Enable color matrix&Change R/B range
    }

#if(ENABLE_SW_DOUBLE_BUFFER)
    MDrv_SWDB_Triger();
#endif
    RECOVER_SC_BANK

#endif
}
void msAdjustHSC(MS_U8 bScalerWin, MS_U16 ucHue, MS_U16 ucSaturation, MS_U16 ucContrast,cscParametersStructure* cscParameters)
{
    memset(&s_AceInfo[0], 0, sizeof(StruAceInfo));
    memset(&s_AceInfo[1], 0, sizeof(StruAceInfo));
    MS_U8 i;
    for( i = 0; i < 2; ++ i )
    {
        s_AceInfo[i].wRCon = 1024;
        s_AceInfo[i].wGCon = 1024;
        s_AceInfo[i].wBCon = 1024;
        s_AceInfo[i].wContrast = 1024;
        s_AceInfo[i].ucSaturation = 0x80;
        s_AceInfo[i].ucHue = 50;

        s_AceInfo[i].sContrastRGBMatrix[0][0] =
            s_AceInfo[i].sContrastRGBMatrix[1][1] =
                s_AceInfo[i].sContrastRGBMatrix[2][2] = 1024;
        s_AceInfo[i].sContrastRGBMatrix[0][1] =
            s_AceInfo[i].sContrastRGBMatrix[1][0] =
                s_AceInfo[i].sContrastRGBMatrix[2][0] =
                    s_AceInfo[i].sContrastRGBMatrix[0][2] =
                        s_AceInfo[i].sContrastRGBMatrix[1][2] =
                            s_AceInfo[i].sContrastRGBMatrix[2][1] = 0;

        s_AceInfo[i].sVideoSatHueMatrix[0][0] =
            s_AceInfo[i].sVideoSatHueMatrix[1][1] =
                s_AceInfo[i].sVideoSatHueMatrix[2][2] = 1024;
        s_AceInfo[i].sVideoSatHueMatrix[0][1] =
            s_AceInfo[i].sVideoSatHueMatrix[1][0] =
                s_AceInfo[i].sVideoSatHueMatrix[2][0] =
                    s_AceInfo[i].sVideoSatHueMatrix[0][2] =
                        s_AceInfo[i].sVideoSatHueMatrix[1][2] =
                            s_AceInfo[i].sVideoSatHueMatrix[2][1] = 0;
    }
    s_AceInfo[bScalerWin].ucSaturation = (MS_U8)ucSaturation;
    s_AceInfo[bScalerWin].wContrast = (WORD)ucContrast;
    s_AceInfo[bScalerWin].ucHue = (MS_U8)ucHue;
    s_AceInfo[bScalerWin].psYVUtoRGBMatrix = *Typ3x3Array;
    s_AceInfo[bScalerWin].u8CscIpType = u8CscIpType;
    SetVideoSatHueMatrix( bScalerWin );
    SetVideoContrastMatrix( bScalerWin );

    msSetVideoColorMatrix( bScalerWin ,cscParameters);
}

void writeMdTmo(U16* u16Lut,MS_U8 debugMode)
{
    //static MS_U8 u8FirstRun = 0,u8State = 0;
    //MS_U8 u8BlackScreen = 0;
    U32 u16Idx;
    MS_U16 SramData;
#if RealChip
#else
    FILE *f;
    if(2 == debugMode)
    {
        f = fopen("TmoMD.txt", "w");
        if (f == NULL)
        {
            printk("Error opening TmoMD output file!\n");

        }
    }
#endif
#ifdef CMODEL
    fprintf(fi,"wriu -b 0x100A80 0x1 0\n");
    fprintf(fi,"wriu -b 0x100A82 0x1 0\n");
    fprintf(fi,"wriu -b 0x100A84 0x1 0\n");
#else
    writeByteMask(0x100A80, 0x1,0);
    writeByteMask(0x100A82, 0x1,0);
    writeByteMask(0x100A84, 0x1,0);
#endif//CMODEL

#ifdef AUTODOWNLOAD
    ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO stAutoDownloadFormat;
    stAutoDownloadFormat.enSubClient = E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_TMO;
    stAutoDownloadFormat.bEnableRange = 1;
    stAutoDownloadFormat.u16StartAddr = 0;
    stAutoDownloadFormat.u16EndAddr = 511;
    writeByteMask(DoVi_Addr_H(0x7A, 0x70),0x80, 0x80);  // AUTODOWNLOAD En
    if(1 == MHal_XC_WriteAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR, (MS_U16*)&(u16Lut[0]),512 * sizeof(MS_U16), (void*)&stAutoDownloadFormat) )
    {
#if 0
        if(1 == MHal_XC_FireAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR))
        {
            //printk("M+D TMO AutoDownload successful!\n");
        }
        else
        {
            printk("M+D Gamma AutoDownload FAILED!\n");
        }
#endif
    }
    else
    {
        printk("Write Tmo AutoDownload FAILED!\n");
    }
    /*
        u8BlackScreen = MApi_GFLIP_XC_R2BYTE(REG_SC_BK67_28_L);

         if(1 == u8BlackScreen && 0 == u8State)
        {
            u8State = 1;
        }
        else if(0 == u8BlackScreen && 1 == u8State)
        {
            u8State = 2;
        }
        printk("Autodownload :%d u8FirstRun:%d u8State:%d\n",u8BlackScreen,u8FirstRun,u8State);
        if(0 == u8FirstRun)
        {
           if(2 == u8State)
           {
                printk("Autodownload 0\n");
                MApi_GFLIP_XC_W2BYTEMSK(_PK_H_(0x7A,0x42),0x1000,0xFFFF);
                MApi_GFLIP_XC_W2BYTEMSK(_PK_H_(0x7A,0x54),0,0xFFFF);
                MApi_GFLIP_XC_W2BYTEMSK(_PK_H_(0x7A,0x55),0x7FFFF,0xFFFF);
                u8FirstRun = 1;
           }
           else
           {
                printk("Autodownload 1\n");
                MApi_GFLIP_XC_W2BYTEMSK(_PK_H_(0x7A,0x42),0,0xFFFF);
                MApi_GFLIP_XC_W2BYTEMSK(_PK_H_(0x7A,0x54),0,0xFFFF);
                MApi_GFLIP_XC_W2BYTEMSK(_PK_H_(0x7A,0x55),0,0xFFFF);
           }
        }
    */
#else //RIU
    writeByteMask(DoVi_Addr_H(0x7A, 0x70),0x80, 0); // AutoDownload off
    writeByteMask(DoVi_Addr_L(0x7A, 0x60),  0x3, 0x2); // lut_sel Gamma
    writeByteMask(DoVi_Addr_L(0x7A, 0x68),  0x0F, 0x0); // lut_sel_ip
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x10, 0x0); // lut_fast_md off
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x20, 0x20); // lut_wd_dup_md
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x80, 0x80); // lut_load_en
    //write2Bytes(DoVi_Addr_L(0x7B, 0x69), 0); // lut_init_addr

    //MS_U32* pu32Lut = (MS_U32*) pstDataInfo->pu8Data;
    for (u16Idx  = 0; u16Idx < 512; u16Idx++)
    {
        SramData = min( u16Lut[u16Idx],0x0FFF);
        if(2 == debugMode)
        {
#if RealChip
#else
            fprintf(f,"%d: %0#x\n", u16Idx,SramData);
#endif
        }
        if(1 == debugMode)
        {
            printk("TMO table to be written is %d: %0#x\n", u16Idx,SramData);
        }
        write2Bytes(DoVi_Addr_L(0x7A, 0x61), u16Idx); // lut_addr
        write4Bytes(DoVi_Addr_L(0x7A, 0x62), SramData); // wd0
        //write2Bytes(DoVi_Addr_L(0x7A, 0x63), 0); // wd0
        writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x10, 0x10); // lut_write_pulse
#ifdef CMODEL
        //fprintf(fi,"wait 10\n") ;
#else
        while  (MApi_GFLIP_XC_R2BYTEMSK( DoVi_Addr_L(0x7A, 0x60) , 0x0100 ) != 0x0100)
        {
            printk("wait a moment...\n");

        };//
#endif// CMODEL

        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x0F, 0x00); // lut_flag_clr
    }
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0xFF, 0x00); // lut_load_en
#endif// AUTPDOWNLOAD or  RIU

}
void writeMdGamma(U32* u32Lut,MS_U8 debugMode,MS_U8 transferCharacterstics)
{

    U32 u16Idx;
    MS_U16 SramData;
#if RealChip
#else
    FILE *f;
    if(2 == debugMode)
    {
        f = fopen("GammaMD.txt", "w");
        if (f == NULL)
        {
            printk("Error opening GammaMD output file!\n");

        }
    }
#endif

#ifdef CMODEL
    fprintf(fi,"wriu -b 0x100A80 0x1 0\n");
    fprintf(fi,"wriu -b 0x100A82 0x1 0\n");
    fprintf(fi,"wriu -b 0x100A84 0x1 0\n");
#else
    writeByteMask(0x100A80, 0x1,0);
    writeByteMask(0x100A82, 0x1,0);
    writeByteMask(0x100A84, 0x1,0);
#endif//CMODEL

#if 1
    if(16 == transferCharacterstics)
    {
        write4Bytes(DoVi_Addr_L(0x7C, 0x02), 0xFFFFFFFF);
        /*write2Bytes(DoVi_Addr_L(0x7C, 0x04), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x06), 0xFFFF);
        write2Bytes(DoVi_Addr_L(0x7C, 0x08), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x0A), 0xFFFF);
        write2Bytes(DoVi_Addr_L(0x7C, 0x0C), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x0E), 0xFFFF);*/
        write2Byte(DoVi_Addr_L(0x7C, 0x10), 0x0008);
        //writeByte(DoVi_Addr_H(7C, 10), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x11), 0x0201);
        //writeByte(DoVi_Addr_H(7C, 11), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x12), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 12), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x13), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 13), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x14), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 14), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x15), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 15), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x16), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 16), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x17), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 17), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x18), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 18), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x19), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 19), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1A), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 1A), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1B), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 1B), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1C), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 1C), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1D), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 1D), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1E), 0x0403);
        //writeByte(DoVi_Addr_H(7C, 1E), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1F), 0x0404);
        //writeByte(DoVi_Addr_H(7C, 1F), 7);
        write2Bytes(DoVi_Addr_L(0x7C, 0x20), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x21), 2);
        write2Bytes(DoVi_Addr_L(0x7C, 0x22), 4);
        write2Bytes(DoVi_Addr_L(0x7C, 0x23), 8);
        write2Bytes(DoVi_Addr_L(0x7C, 0x24), 16);
        write2Bytes(DoVi_Addr_L(0x7C, 0x25), 32);
        write2Bytes(DoVi_Addr_L(0x7C, 0x26), 48);
        write2Bytes(DoVi_Addr_L(0x7C, 0x27), 64);
        write2Bytes(DoVi_Addr_L(0x7C, 0x28), 80);
        write2Bytes(DoVi_Addr_L(0x7C, 0x29), 96);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2A), 112);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2B), 128);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2C), 144);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2D), 160);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2E), 176);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2F), 192);
        write2Bytes(DoVi_Addr_L(0x7C, 0x30), 208);
        write2Bytes(DoVi_Addr_L(0x7C, 0x31), 224);
        write2Bytes(DoVi_Addr_L(0x7C, 0x32), 240);
        write2Bytes(DoVi_Addr_L(0x7C, 0x33), 256);
        write2Bytes(DoVi_Addr_L(0x7C, 0x34), 272);
        write2Bytes(DoVi_Addr_L(0x7C, 0x35), 288);
        write2Bytes(DoVi_Addr_L(0x7C, 0x36), 304);
        write2Bytes(DoVi_Addr_L(0x7C, 0x37), 320);
        write2Bytes(DoVi_Addr_L(0x7C, 0x38), 336);
        write2Bytes(DoVi_Addr_L(0x7C, 0x39), 352);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3A), 368);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3B), 483);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3C), 400);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3D), 416);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3E), 448);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3F), 480);
    }
    else
    {
        //set up the sampling rate, 512 entry with equal spacing
        write4Bytes(DoVi_Addr_L(0x7C, 0x02), 0xFF800000);
        /*write2Bytes(DoVi_Addr_L(0x7C, 0x04), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x06), 0xFFFF);
        write2Bytes(DoVi_Addr_L(0x7C, 0x08), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x0A), 0xFFFF);
        write2Bytes(DoVi_Addr_L(0x7C, 0x0C), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x0E), 0xFFFF);*/
        write2Byte(DoVi_Addr_L(0x7C, 0x10), 0);
        //writeByte(DoVi_Addr_H(7C, 10), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x11), 0);
        //writeByte(DoVi_Addr_H(7C, 11), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x12), 0);
        //writeByte(DoVi_Addr_H(7C, 12), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x13), 0);
        //writeByte(DoVi_Addr_H(7C, 13), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x14), 0);
        //writeByte(DoVi_Addr_H(7C, 14), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x15), 0);
        //writeByte(DoVi_Addr_H(7C, 15), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x16), 0);
        //writeByte(DoVi_Addr_H(7C, 16), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x17), 0);
        //writeByte(DoVi_Addr_H(7C, 17), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x18), 0);
        //writeByte(DoVi_Addr_H(7C, 18), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x19), 0);
        //writeByte(DoVi_Addr_H(7C, 19), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1A), 0);
        //writeByte(DoVi_Addr_H(7C, 1A), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1B), 0x0800);
        //writeByte(DoVi_Addr_H(7C, 1B), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1C), 0x0100);
        //writeByte(DoVi_Addr_H(7C, 1C), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1D), 0x0302);
        //writeByte(DoVi_Addr_H(7C, 1D), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1E), 0x0504);
        //writeByte(DoVi_Addr_H(7C, 1E), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x1F), 0x0706);
        //writeByte(DoVi_Addr_H(7C, 1F), 7);
        write2Bytes(DoVi_Addr_L(0x7C, 0x20), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x21), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x22), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x23), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x24), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x25), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x26), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x27), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x28), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x29), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2A), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2B), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2C), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2D), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2E), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x2F), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x30), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x31), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x32), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x33), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x34), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x35), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x36), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x37), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x38), 2);
        write2Bytes(DoVi_Addr_L(0x7C, 0x39), 4);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3A), 8);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3B), 16);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3C), 32);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3D), 64);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3E), 128);
        write2Bytes(DoVi_Addr_L(0x7C, 0x3F), 256);
    }

#endif



#ifdef AUTODOWNLOAD
    U16* u16LutData = (U16*) kmalloc( 512 * sizeof(U16), GFP_KERNEL);
    for(u16Idx = 0; u16Idx<512; u16Idx++)
    {
        u16LutData[u16Idx] = min( u32Lut[u16Idx],0xFFFF);
    }

    ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO stAutoDownloadFormat;
    stAutoDownloadFormat.enSubClient = E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_GAMMA;
    stAutoDownloadFormat.bEnableRange = 1;
    stAutoDownloadFormat.u16StartAddr = 0;
    stAutoDownloadFormat.u16EndAddr = 511;
    writeByteMask(DoVi_Addr_H(0x7A, 0x70),0x80, 0x80); // AutoDownload on
    if(1 == MHal_XC_WriteAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR, (MS_U8*)&(u16LutData[0]),512 * sizeof(MS_U16), (void*)&stAutoDownloadFormat) )
    {
        //if(1 == MHal_XC_FireAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR))            //Fire at TMO, once is enough
        //     printk("M+D Gamma AutoDownload successful!\n");
        //else
        //  printk("M+D Gamma AutoDownload FAILED!\n");
    }
    else
    {
        printk("Write Gamma AutoDownload FAILED!\n");
    }
#else //RIU
    writeByteMask(DoVi_Addr_H(0x7A, 0x70),0x80, 0); // AutoDownload off
    writeByteMask(DoVi_Addr_L(0x7A, 0x60),  0x3, 0x1); // lut_sel Gamma
    writeByteMask(DoVi_Addr_L(0x7A, 0x68),  0x0F, 0x0); // lut_sel_ip
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x10, 0x0); // lut_fast_md off
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x20, 0x20); // lut_wd_dup_md
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x80, 0x80); // lut_load_en
    //write2Bytes(DoVi_Addr_L(0x7B, 0x69), 0); // lut_init_addr

    //MS_U32* pu32Lut = (MS_U32*) pstDataInfo->pu8Data;
    for (u16Idx  = 0; u16Idx < 512; u16Idx++)
    {
        SramData = min( u32Lut[u16Idx]/*+(1<<3))>>4*/,0xFFFF);
        if(2 == debugMode)
        {
#if RealChip
#else
            fprintf(f,"%d: %0#x\n", u16Idx,SramData);
#endif
        }
        if(1 == debugMode)
        {
            printk("%d: %0#x\n", u16Idx,SramData);
        }
        write2Bytes(DoVi_Addr_L(0x7A, 0x61), u16Idx); // lut_addr
        write2Bytes(DoVi_Addr_L(0x7A, 0x62), SramData); // wd0
        write2Bytes(DoVi_Addr_L(0x7A, 0x63), 0); // wd0
        writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x10, 0x10); // lut_write_pulse

#ifdef CMODEL
        //fprintf(fi,"wait 10\n") ;
#else
        while  (MApi_GFLIP_XC_R2BYTEMSK( DoVi_Addr_L(0x7A, 0x60) , 0x0100 ) != 0x0100)
        {
            printk("wait a moment...\n");

        };//
#endif

        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x0F, 0x00); // lut_flag_clr
    }
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0xFF, 0x00); // lut_load_en
#endif // autodownload or RIU

#ifdef AUTODOWNLOAD
    kfree(u16LutData);
#endif
    MS_U32 u32LastEntry = u32Lut[512]-u32Lut[511];// << 3;

    if(2 == debugMode)
    {
#if RealChip
#else


        fprintf(f,"%d\n",u32Lut[512]);
#endif
    }
    if(1 == debugMode)
    {
        printf("%d: %0#x\n", 512,u32Lut[512]);
    }

    write4Bytes(DoVi_Addr_L(0x7C, 0x04), u32LastEntry);
    write4Bytes(DoVi_Addr_L(0x7C, 0x08), u32LastEntry);
    write4Bytes(DoVi_Addr_L(0x7C, 0x0c), u32LastEntry);
}

void writeMdDeGamma(U32* u32Lut,MS_U8 debugMode,MS_U8 transferCharacterstics,MS_U8 lut_en)
{

    U32 u16Idx;
    MS_U16 u16I;
    MS_U32 SramData;
#if RealChip
#else
    FILE *f;
    FILE *f32;
    if(2 == debugMode)
    {
        f = fopen("DeGammaMD.txt", "w");
        if (f == NULL)
        {
            printk("Error opening DeGammaMD output file!\n");
        }
        f32 = fopen("DeGammaMD32.txt", "w");
        if (f32 == NULL)
        {
            printk("Error opening DeGammaMD32 output file!\n");
        }
    }
#endif
#ifdef CMODEL
    fprintf(fi,"wriu -b 0x100A80 0x1 0\n");
    fprintf(fi,"wriu -b 0x100A82 0x1 0\n");
    fprintf(fi,"wriu -b 0x100A84 0x1 0\n");
#else
    writeByteMask(0x100A80, 0x1,0);
    writeByteMask(0x100A82, 0x1,0);
    writeByteMask(0x100A84, 0x1,0);
#endif

#if 1
    if(16 == transferCharacterstics)
    {
        //set up the sampling rate, 512 entry with equal spacing
        write2Bytes(DoVi_Addr_L(0x7C, 0x42), 0xFFFF);
        /*write4Bytes(DoVi_Addr_L(0x7C, 0x44), 0xFFFFFFFF);
        write4Bytes(DoVi_Addr_L(0x7C, 0x46), 0);
        write4Bytes(DoVi_Addr_L(0x7C, 0x48), 0xFFFFFFFF);
        write4Bytes(DoVi_Addr_L(0x7C, 0x4A), 0);
        write4Bytes(DoVi_Addr_L(0x7C, 0x4C), 0xFFFFFFFF);
        write4Bytes(DoVi_Addr_L(0x7C, 0x4E), 0);*/
        write2Byte(DoVi_Addr_L(0x7C, 0x50), 0x0008);
        //writeByte(DoVi_Addr_H(7C, 50), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x51), 0x0201);
        //writeByte(DoVi_Addr_H(7C, 51), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x52), 0x0303);
        //writeByte(DoVi_Addr_H(7C, 52), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x53), 0x0403);
        //writeByte(DoVi_Addr_H(7C, 53), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x54), 0x0404);
        //writeByte(DoVi_Addr_H(7C, 54), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x55), 0x0404);
        //writeByte(DoVi_Addr_H(7C, 55), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x56), 0x0504);
        //writeByte(DoVi_Addr_H(7C, 56), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x57), 0x0605);
        //writeByte(DoVi_Addr_H(7C, 57), 7);
        write2Bytes(DoVi_Addr_L(0x7C, 0x60), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x61), 2);
        write2Bytes(DoVi_Addr_L(0x7C, 0x62), 4);
        write2Bytes(DoVi_Addr_L(0x7C, 0x63), 8);
        write2Bytes(DoVi_Addr_L(0x7C, 0x64), 16);
        write2Bytes(DoVi_Addr_L(0x7C, 0x65), 32);
        write2Bytes(DoVi_Addr_L(0x7C, 0x66), 48);
        write2Bytes(DoVi_Addr_L(0x7C, 0x67), 64);
        write2Bytes(DoVi_Addr_L(0x7C, 0x68), 96);
        write2Bytes(DoVi_Addr_L(0x7C, 0x69), 128);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6A), 160);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6B), 192);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6C), 224);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6D), 256);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6E), 320);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6F), 384);
    }
    else
    {
        //set up the sampling rate, 512 entry with equal spacing
        write2Bytes(DoVi_Addr_L(0x7C, 0x42), 0xC000);
        /*write4Bytes(DoVi_Addr_L(0x7C, 0x44), 0xFFFFFFFF);
        write4Bytes(DoVi_Addr_L(0x7C, 0x46), 0);
        write4Bytes(DoVi_Addr_L(0x7C, 0x48), 0xFFFFFFFF);
        write4Bytes(DoVi_Addr_L(0x7C, 0x4A), 0);
        write4Bytes(DoVi_Addr_L(0x7C, 0x4C), 0xFFFFFFFF);
        write4Bytes(DoVi_Addr_L(0x7C, 0x4E), 0);*/
        write2Byte(DoVi_Addr_L(0x7C, 0x50), 0);
        //writeByte(DoVi_Addr_H(7C, 50), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x51), 0);
        //writeByte(DoVi_Addr_H(7C, 51), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x52), 0);
        //writeByte(DoVi_Addr_H(7C, 52), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x53), 0);
        //writeByte(DoVi_Addr_H(7C, 53), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x54), 0);
        //writeByte(DoVi_Addr_H(7C, 54), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x55), 0);
        //writeByte(DoVi_Addr_H(7C, 55), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x56), 0);
        //writeByte(DoVi_Addr_H(7C, 56), 0);
        write2Byte(DoVi_Addr_L(0x7C, 0x57), 0x070F);
        //writeByte(DoVi_Addr_H(7C, 57), 7);
        write2Bytes(DoVi_Addr_L(0x7C, 0x60), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x61), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x62), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x63), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x64), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x65), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x66), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x67), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x68), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x69), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6A), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6B), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6C), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6D), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6E), 0);
        write2Bytes(DoVi_Addr_L(0x7C, 0x6F), 256);
    }


#endif


#ifdef AUTODOWNLOAD
    U32* u32AutoDownloadArray = (U32*) kmalloc( 512 * sizeof(U32), GFP_KERNEL);
    for (u16Idx  = 0; u16Idx < 512; u16Idx++)
    {
        if(0 == lut_en)
        {
            // convert 32bit to mantissa(14b)+exponent(5b)
            for (u16I = 0; u16I < 18; u16I++)
            {
                if (u32Lut[u16Idx] < (0x1 << (u16I+14)))
                    break;
            }
            u32AutoDownloadArray[u16Idx] = ( (u32Lut[u16Idx] >> u16I) << 5) | u16I;
        }
    }

    ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO stAutoDownloadFormat;
    stAutoDownloadFormat.enSubClient = E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_DEGAMMA;
    stAutoDownloadFormat.bEnableRange = 1;
    stAutoDownloadFormat.u16StartAddr = 0;
    stAutoDownloadFormat.u16EndAddr = 511;
    MHal_XC_W2BYTE(REG_SC_BK7A_70_L, 0x8000);
    if(1 == MHal_XC_WriteAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR, (MS_U8*)&(u32AutoDownloadArray[0]),512 * sizeof(MS_U32), (void*)&stAutoDownloadFormat) )
    {
        //printk(" DeGamma autodownload successful!\n");

        //MHal_XC_FireAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR);//Fire at TMO, once is enough
    }
    else
    {
        printk("Write DeGamma AutoDownload FAILED!\n");
    }
	kfree(u32AutoDownloadArray);
#else //RIU


    writeByteMask(DoVi_Addr_H(0x7A, 0x70),0x80, 0); // AutoDownload off
    writeByteMask(DoVi_Addr_L(0x7A, 0x60),  0x3, 0x0); // lut_sel Gamma
    writeByteMask(DoVi_Addr_L(0x7A, 0x68),  0x0F, 0x0); // lut_sel_ip
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x10, 0x0); // lut_fast_md off
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x20, 0x20); // lut_wd_dup_md
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x80, 0x80); // lut_load_en
    //write2Bytes(DoVi_Addr_L(0x7B, 0x69), 0); // lut_init_addr

    //MS_U32* pu32Lut = (MS_U32*) pstDataInfo->pu8Data;
    for (u16Idx  = 0; u16Idx < 512; u16Idx++)
    {

        // convert 32bit to mantissa(14b)+exponent(5b)
        for (u16I = 0; u16I < 18; u16I++)
        {
            if (u32Lut[u16Idx] < (0x1 << (u16I+14)))
                break;
        }
        SramData = ( (u32Lut[u16Idx] >> u16I) << 5) | u16I;

        if(2 == debugMode)
        {
#if RealChip
#else
            fprintf(f,"%d: %0#x\n", u16Idx,SramData);
            fprintf(f32,"%d: %0#x\n", u16Idx,u32Lut[u16Idx]);
#endif
        }
        if(1 == debugMode)
        {
            printk("%d: %0#x\n", u16Idx,SramData);
        }
        write2Bytes(DoVi_Addr_L(0x7A, 0x61), u16Idx); // lut_addr
        write4Bytes(DoVi_Addr_L(0x7A, 0x62), SramData); // wd0
        //write2Bytes(DoVi_Addr_L(0x7A, 0x63), 0); // wd0
        writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x10, 0x10); // lut_write_pulse

#ifdef CMODEL
        //fprintf(fi,"wait 10\n") ;
#else
        while  (MApi_GFLIP_XC_R2BYTEMSK( DoVi_Addr_L(0x7A, 0x60) , 0x0100 ) != 0x0100)
        {
            printk("wait a moment...\n");

        };//
#endif

        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x0F, 0x00); // lut_flag_clr
    }
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0xFF, 0x00); // lut_load_en
#endif //RIU-Autodownload


    MS_U32 u32LastEntry = u32Lut[512];// << 3;

    if(2 == debugMode)
    {
#if RealChip
#else

        //fprintf(f32,"%d: %0#x\n", 512,u32LastEntry);
        //fprintf(f32,"%lld\n", u64LastEntry);
        fprintf(f32,"%d\n", u32Lut[u16Idx]);
#endif
    }
    if(1 == debugMode)
    {
        printf("%d: %0#x\n", 512,u32LastEntry);
    }

    write4Bytes(DoVi_Addr_L(0x7C, 0x44), u32LastEntry);
    write4Bytes(DoVi_Addr_L(0x7C, 0x48), u32LastEntry);
    write4Bytes(DoVi_Addr_L(0x7C, 0x4c), u32LastEntry);


}
void writeMdGammaFast(U32* u32Lut)
{
    writeByteMask(DoVi_Addr_L(0x7A, 0x60),  0x3, 0x0); // lut_sel
    writeByteMask(DoVi_Addr_L(0x7A, 0x68),  0x0F, 0x0); // lut_sel_ip
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x10, 0x10); // lut_fast_md
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x20, 0x20); // lut_wd_dup_md
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x80, 0x80); // lut_load_en
    write2Bytes(DoVi_Addr_L(0x7A, 0x69), 0); // lut_init_addr

    U32 u16Idx = 0;
    //MS_U32* pu32Lut = (MS_U32*) pstDataInfo->pu8Data;
    for ( ; u16Idx < 512; u16Idx++)
    {
        //write2Bytes(DoVi_Addr_L(0x7A, 0x61), u16Idx); // lut_addr
        write4Bytes(DoVi_Addr_L(0x7A, 0x62), u16Idx); // wd0
        //write2Bytes(DoVi_Addr_L(0x7A, 0x63), 0); // wd0
        //writeByteMask(DoVi_Addr_L(0x7A, 0x60), 0x10, 0x10); // lut_write_pulse
        //while ((DoVi_ReadByte(DoVi_Addr_H(7A, 60)) & 1) == 0); // wait lut_flag_rb{}

        //write2ByteMask(DoVi_Addr_H(0x7A, 0x60), 0x00, 0xFF);//waste time
        writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0x0F, 0x00); // lut_flag_clr
    }
    writeByteMask(DoVi_Addr_H(0x7A, 0x60), 0xFF, 0x00); // lut_load_en

}
void inputCscHdrWrite2RegisterMaserati(cscParametersStructure* cscParameters)
{
    S16 s16Regs[3][3];
    U16 u16Regs[3][3];
    MS_U8 i,j;
    S16 temp;
    MS_U32 offset1,offset2,offset3;
    MS_U32 offsetY,offsetCb,offsetCr;
    if (FALSE == cscParameters->cscEnable)
    {
        writeByteMask(DoVi_Addr_H(0x7B, 0x7), (0x3 << 0), 0x1);
        writeByteMask(DoVi_Addr_L(0x7B, 0x7), (0x1 << 1), (0x1 << 1));
	write2Bytes(DoVi_Addr_L(0x7B, 0x08), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x09), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x0A), 0x2000);
	write2Bytes(DoVi_Addr_L(0x7B, 0x0B), 0x2000);
	write2Bytes(DoVi_Addr_L(0x7B, 0x0C), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x0D), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x0E), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x0F), 0x2000);
	write2Bytes(DoVi_Addr_L(0x7B, 0x10), 0x0);
        write4Bytes(DoVi_Addr_L(0x7B, 0x11), 0x0);
        write4Bytes(DoVi_Addr_L(0x7B, 0x13), 0x0);
        write4Bytes(DoVi_Addr_L(0x7B, 0x15), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x17), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x18), 0xffff);
        write4Bytes(DoVi_Addr_L(0x7B, 0x19), 0x10000);
    }
    else
    {

        for( i = 0; i < 3; i++)
            for( j = 0; j < 3; j++)
            {
                temp = ((( abs(cscParameters->cscCoeff[i][j]) << 13)+ 25000) / 50000);
                s16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0xFFFF : (-1*temp) &0xFFFF;
                u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0xFFFF : (-1*temp) &0xFFFF;
            }

        offsetCr = (1 == cscParameters->rOrCrSub16) ? 16<<6 : (1 == cscParameters->rOrCrRangeSub128) ? 128<<6 : 0;
        offsetY = (1 == cscParameters->gOrYSub16) ? 16<<6 : (1 == cscParameters->gOrYRangeSub128) ? 128<<6 : 0;
        offsetCb = (1 == cscParameters->bOrCbSub16) ? 16<<6 : (1 == cscParameters->bOrCbRangeSub128) ? 128<<6 : 0;
        offset1 = (s16Regs[0][1] * offsetY) + (s16Regs[0][2] * offsetCr) + (s16Regs[0][0] * offsetCb);
        offset2 = (s16Regs[1][1] * offsetY) + (s16Regs[1][2] * offsetCr) + (s16Regs[1][0] * offsetCb);
        offset3 = (s16Regs[2][1] * offsetY) + (s16Regs[2][2] * offsetCr) + (s16Regs[2][0] * offsetCb);
        /*
                    offset1 *= 64;
                    offset2 *= 64;
                    offset3 *= 64;*/
        /*MS_BOOL rOrCrSub16;
        MS_BOOL gOrYSub16;
        MS_BOOL bOrCbSub16;
        MS_BOOL rOrCrRangeSub128;
        MS_BOOL gOrYRangeSub128;
        MS_BOOL bOrCbRangeSub128;*/
        /*
                    write2Bytes(DoVi_Addr_L(0x7B, 0x08), u16Regs[0][1]);
                    write2Bytes(DoVi_Addr_L(0x7B, 0x09), u16Regs[1][1]);
                    write2Bytes(DoVi_Addr_L(0x7B, 0x0A), u16Regs[2][1]);
                    write2Bytes(DoVi_Addr_L(0x7B, 0x0B), u16Regs[0][2]);
                    write2Bytes(DoVi_Addr_L(0x7B, 0x0C), u16Regs[1][2]);
                    write2Bytes(DoVi_Addr_L(0x7B, 0x0D), u16Regs[2][2]);
                    write2Bytes(DoVi_Addr_L(0x7B, 0x0E), u16Regs[0][0]);
                    write2Bytes(DoVi_Addr_L(0x7B, 0x0F), u16Regs[1][0]);
                    write2Bytes(DoVi_Addr_L(0x7B, 0x10), u16Regs[2][0]);
                    */
        writeByteMask(DoVi_Addr_H(0x7B, 0x7), (0x3 << 0), 0x1);
        writeByteMask(DoVi_Addr_L(0x7B, 0x7), (0x1 << 1), (0x1 << 1));
        write2Bytes(DoVi_Addr_L(0x7B, 0x08), u16Regs[0][1]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x09), u16Regs[0][2]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x0A), u16Regs[0][0]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x0B), u16Regs[1][1]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x0C), u16Regs[1][2]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x0D), u16Regs[1][0]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x0E), u16Regs[2][1]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x0F), u16Regs[2][2]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x10), u16Regs[2][0]);
        write4Bytes(DoVi_Addr_L(0x7B, 0x11), offset1);
        write4Bytes(DoVi_Addr_L(0x7B, 0x13), offset2);
        write4Bytes(DoVi_Addr_L(0x7B, 0x15), offset3);
        write2Bytes(DoVi_Addr_L(0x7B, 0x17), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x18), 0xffff);
        write4Bytes(DoVi_Addr_L(0x7B, 0x19), 0x10000);
    }
}
MS_BOOL hdrCscMaxRgbWrite2RegisterMaserati(cscParametersStructure* cscParameters,MS_U8 mode,MS_U8 matrixCoefficents,MS_U8 ratio,MS_U8 debugMatrixMode,MS_U8 registerDebugEn,MS_U8 IpType)
{
    S16 s16Regs[3][3];
    U16 u16Regs[3][3];
    MS_U8 i,j;
    S16 temp;
    MS_U32 offset1,offset2,offset3;
    MS_U32 offsetY,offsetCb,offsetCr;
    if (E_CFD_IP_CSC_YFULL_TO_RFULL != mode)
    {
        printk("CSC warning! IpType HDR MAX RGB CSC does not support selected mode, switching to YFULL_TO_RFULL\n");
        CSC(E_CFD_IP_CSC_YFULL_TO_RFULL, matrixCoefficents, ratio, debugMatrixMode, registerDebugEn, IpType);
        return 0;
    }
    else
    {

        for( i = 0; i < 3; i++)
            for( j = 0; j < 3; j++)
            {
                temp = ((( abs(cscParameters->cscCoeff[i][j]) << 6 )+ 25000) / 50000);
                s16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0xFFFF : (-1*temp) &0xFFFF;
                u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0xFFFF : (-1*temp) &0xFFFF;
            }


        offsetCr = (1 == cscParameters->rOrCrSub16) ? 16<<1 : (1 == cscParameters->rOrCrRangeSub128) ? 128<<1 : 0;
        offsetY = (1 == cscParameters->gOrYSub16) ? 16<<1 : (1 == cscParameters->gOrYRangeSub128) ? 128<<1 : 0;
        offsetCb = (1 == cscParameters->bOrCbSub16) ? 16<<1 : (1 == cscParameters->bOrCbRangeSub128) ? 128<<1 : 0;


        offset1 = (s16Regs[0][1] * offsetY) + (s16Regs[0][2] * offsetCr) + (s16Regs[0][0] * offsetCb);
        offset2 = (s16Regs[1][1] * offsetY) + (s16Regs[1][2] * offsetCr) + (s16Regs[1][0] * offsetCb);
        offset3 = (s16Regs[2][1] * offsetY) + (s16Regs[2][2] * offsetCr) + (s16Regs[2][0] * offsetCb);



        //write2Bytes(DoVi_Addr_L(0x7B, 0x08), u16Regs[0][1]);
        //write2Bytes(DoVi_Addr_L(0x7B, 0x09), u16Regs[0][2]);
        write2Bytes(DoVi_Addr_L(0x7A, 0x74), u16Regs[0][0]);
        //write2Bytes(DoVi_Addr_L(0x7B, 0x0B), u16Regs[1][1]);
        write2Bytes(DoVi_Addr_L(0x7A, 0x75), u16Regs[1][2]);
        write2Bytes(DoVi_Addr_L(0x7A, 0x76), u16Regs[1][0]);
        //write2Bytes(DoVi_Addr_L(0x7B, 0x0E), u16Regs[2][1]);
        //write2Bytes(DoVi_Addr_L(0x7B, 0x0F), u16Regs[2][2]);
        write2Bytes(DoVi_Addr_L(0x7A, 0x77), u16Regs[2][2]);
        write2Bytes(DoVi_Addr_L(0x7A, 0x78), offset1 & 0xFFFF);
        write2ByteMask(DoVi_Addr_L(0x7A, 0x79), 0x000F, offset1 >> 16);
        write2Bytes(DoVi_Addr_L(0x7A, 0x7A), offset2 & 0xFFFF);
        write2ByteMask(DoVi_Addr_L(0x7A, 0x7B), 0x000F, offset2 >> 16);
        write2Bytes(DoVi_Addr_L(0x7A, 0x7C), offset3 & 0xFFFF);
        write2ByteMask(DoVi_Addr_L(0x7A, 0x7D), 0x000F, offset3 >> 16);
        //write2Bytes(DoVi_Addr_L(0x7B, 0x17), 0x0);
        //write2Bytes(DoVi_Addr_L(0x7B, 0x18), 0xffff);
        //write4Bytes(DoVi_Addr_L(0x7B, 0x19), 0x10000);
        return 1;
    }
}
void outputCscHdrWrite2RegisterMaserati(cscParametersStructure* cscParameters,MS_U8 mode)
{
    U16 u16Regs[3][3];
    MS_U8 i,j;
    S16 temp;
    MS_U32 offset1,offset2,offset3;
    MS_U32 offsetY,offsetCb,offsetCr;
    if (FALSE == cscParameters->cscEnable)
    {
        //writeByteMask(DoVi_Addr_H(0x7B, 0x7), (0x3 << 4) , (0x1  << 4) );
        writeByteMask(DoVi_Addr_H(0x7B, 0x7), (0x30) , (0x10) );
        writeByteMask(DoVi_Addr_L(0x7B, 0x7),(0x1 << 3) ,0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x29), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x2A), 0x1000);
	write2Bytes(DoVi_Addr_L(0x7B, 0x2B), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x2C), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x2D), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x2E), 0x1000);
	write2Bytes(DoVi_Addr_L(0x7B, 0x2F), 0x1000);
        write2Bytes(DoVi_Addr_L(0x7B, 0x30), 0x0);
	write2Bytes(DoVi_Addr_L(0x7B, 0x31), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x32), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x33), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x34), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x35), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x36), 0x8001);
        write2Bytes(DoVi_Addr_L(0x7B, 0x37), 0x8001);
        write2Bytes(DoVi_Addr_L(0x7B, 0x38), 0x7FFF);
        write2Bytes(DoVi_Addr_L(0x7B, 0x39), 0x7FFF);
        write2Bytes(DoVi_Addr_L(0x7B, 0x3A), 0x7FFF);
    }
    else
    {

        for( i = 0; i < 3; i++)
            for( j = 0; j < 3; j++)
            {
                temp = ((( abs(cscParameters->cscCoeff[i][j]) << 12)+ 25000) / 50000);
                //s16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0xFFFF : (-1*temp) &0xFFFF;
                u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0xFFFF : (-1*temp) &0xFFFF;
            }


        offsetCr = (1 == cscParameters->rOrCrAdd16) ? 16<<7 : (1 == cscParameters->rOrCrAdd128) ? 128<<7 : 0;
        offsetY  = (1 == cscParameters->gOrYAdd16)  ? 16<<7 :  0;
        offsetCb = (1 == cscParameters->bOrCbAdd16) ? 16<<7 : (1 == cscParameters->bOrCbAdd128) ? 128<<7 : 0;


        offset1 = offsetY;//(s16Regs[0][1] * offsetY) + (s16Regs[0][2] * offsetCr) + (s16Regs[0][0] * offsetCb);
        offset2 = offsetCb;//(s16Regs[1][1] * offsetY) + (s16Regs[1][2] * offsetCr) + (s16Regs[1][0] * offsetCb);
        offset3 = offsetCb;//(s16Regs[2][1] * offsetY) + (s16Regs[2][2] * offsetCr) + (s16Regs[2][0] * offsetCb);
        writeByteMask(DoVi_Addr_H(0x7B, 0x7), (0x30) , (0x10) );
        writeByteMask(DoVi_Addr_L(0x7B, 0x7),(0x08) ,0x08);
		if (0)/*E_CFD_IP_CSC_RLIMIT_TO_RFULL == mode || E_CFD_IP_CSC_RFULL_TO_RLIMIT == mode)*/
		{
			write2Bytes(DoVi_Addr_L(0x7B, 0x29), u16Regs[0][0]);
			write2Bytes(DoVi_Addr_L(0x7B, 0x2A), u16Regs[0][1]);
			write2Bytes(DoVi_Addr_L(0x7B, 0x2B), u16Regs[0][2]);
			write2Bytes(DoVi_Addr_L(0x7B, 0x2C), u16Regs[1][0]);
			write2Bytes(DoVi_Addr_L(0x7B, 0x2D), u16Regs[1][1]);
			write2Bytes(DoVi_Addr_L(0x7B, 0x2E), u16Regs[1][2]);
			write2Bytes(DoVi_Addr_L(0x7B, 0x2F), u16Regs[2][0]);
			write2Bytes(DoVi_Addr_L(0x7B, 0x30), u16Regs[2][1]);
			write2Bytes(DoVi_Addr_L(0x7B, 0x31), u16Regs[2][2]);
		}
		else  // re-order from CrYCb to YCbCr
		{
        write2Bytes(DoVi_Addr_L(0x7B, 0x29), u16Regs[1][0]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x2A), u16Regs[1][1]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x2B), u16Regs[1][2]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x2C), u16Regs[2][0]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x2D), u16Regs[2][1]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x2E), u16Regs[2][2]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x2F), u16Regs[0][0]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x30), u16Regs[0][1]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x31), u16Regs[0][2]);
		}

        write2Bytes(DoVi_Addr_L(0x7B, 0x32), offset1);
        write2Bytes(DoVi_Addr_L(0x7B, 0x33), offset2);
        write2Bytes(DoVi_Addr_L(0x7B, 0x34), offset3);
        write2Bytes(DoVi_Addr_L(0x7B, 0x35), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x36), 0x8001);
        write2Bytes(DoVi_Addr_L(0x7B, 0x37), 0x8001);
        write2Bytes(DoVi_Addr_L(0x7B, 0x38),  0x7FFF);
        write2Bytes(DoVi_Addr_L(0x7B, 0x39),  0x7FFF);
        write2Bytes(DoVi_Addr_L(0x7B, 0x3A),  0x7FFF);
    }
}
void gamutMappingHdrWrite2registrer(MS_S32 *M3x3,MS_U16 g_u16Hdr3x3Ratio,MS_U8 debugMode)
{

    MS_U16 u16Regs[9];
    MS_U32 ratio = (50000*g_u16Hdr3x3Ratio)>>6;
    MS_U32 halfRatio = ratio>>1;
    MS_U8 i;

    if ( 0/* FALSE == pRegTable->stCSC.bEnable*/)
    {
        //writeByteMask(DoVi_Addr_H(0x7B, 0x7), 0x2, (0x3 << 2));
        writeByteMask(DoVi_Addr_H(0x7B, 0x7),  0xC, 0x8);
        writeByteMask(DoVi_Addr_L(0x7B, 0x7), 0x0, (0x1 << 2));
        write2Bytes(DoVi_Addr_L(0x7B, 0x20), 0x4000);
        write2Bytes(DoVi_Addr_L(0x7B, 0x21), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x22), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x23), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x24), 0x4000);
        write2Bytes(DoVi_Addr_L(0x7B, 0x25), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x26), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x27), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x28), 0x4000);
        write2Bytes(DoVi_Addr_L(0x7B, 0x40), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x41), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x42), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x43), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x44), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x45), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x46), 0xFFFF);
        write2Bytes(DoVi_Addr_L(0x7B, 0x47), 0xFFFF);
        write2Bytes(DoVi_Addr_L(0x7B, 0x48), 0xFFFF);
    }
    else
    {
        for (i = 0; i < 9; i++)
        {
            MS_U16 temp = ((( abs(M3x3[i]) << 14) + halfRatio) / (ratio));
            u16Regs[i] = (M3x3[i] > 0) ? temp & 0xFFFF : (-1 * temp) & 0xFFFF;
        }
        writeByteMask(DoVi_Addr_H(0x7B, 0x7),  0xC, 0x8);
        writeByteMask(DoVi_Addr_L(0x7B, 0x7),0x4, 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x20), u16Regs[0]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x21), u16Regs[1]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x22), u16Regs[2]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x23), u16Regs[3]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x24), u16Regs[4]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x25), u16Regs[5]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x26), u16Regs[6]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x27), u16Regs[7]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x28), u16Regs[8]);
        write2Bytes(DoVi_Addr_L(0x7B, 0x40), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x41), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x42), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x43), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x44), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x45), 0x0);
        write2Bytes(DoVi_Addr_L(0x7B, 0x46), 0xFFFF);
        write2Bytes(DoVi_Addr_L(0x7B, 0x47), 0xFFFF);
        write2Bytes(DoVi_Addr_L(0x7B, 0x48), 0xFFFF);
    }
}

void writeDeGammaNormalMode(U32* u32Lut,MS_U16 g_u16Hdr3x3Ratio)
{
    MS_U32 u32InputData1;
    MS_U32 u32Ref, u32Delta1, u32Delta2;
    MS_U32 u32XVYCC_DGT_DATA;
    MS_U16 u16SRamCode_L, u16SRamCode_H;
    MS_U16 u16WriteCount, u16SpLevel, u16SpAddr[4];
    MS_U16 u16Delay = 0xFFFF;
    MS_U16 testRef[300],testDelta[300];
    MS_U16 u16ReadCount ;
    MS_U32 enReadData[300];
    MS_U16 diffSum = 0;
    MS_U32 readTable[600];
    MS_U16 u16Index;

#ifdef VOP2AUTODOWNLOAD
    U32* u32LutData = (U32*) kmalloc( 600 * sizeof(U32), GFP_KERNEL);
    for(u16Index = 0; u16Index<600; u16Index++)
    {
        u32LutData[u16Index] =  ( (( u32Lut[u16Index] + (1<<9)  )>>10) * g_u16Hdr3x3Ratio + (1<<8))>>(3+6);
        printk("Degamma to be downloaded is %d: %d\n",u16Index,u32LutData[u16Index] );
    }

    ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO stAutoDownloadFormat;
    stAutoDownloadFormat.enSubClient = E_KDRV_XC_AUTODOWNLOAD_XVYCC_SUB_DEGAMMA;
    stAutoDownloadFormat.bEnableRange = 1;
    stAutoDownloadFormat.u16StartAddr = 0;
    stAutoDownloadFormat.u16EndAddr = 599;
    //MHal_XC_W2BYTE(REG_SC_BK7A_70_L, 0x8000);
    if(1 == MHal_XC_WriteAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_XVYCC, (MS_U8*)&(u32LutData[0]),600 * sizeof(MS_U32), (void*)&stAutoDownloadFormat) )
    {
        printk("VOP2 DEGamma autodownload Ready!\n");
        //MHal_XC_W2BYTE(0x133082, 0x444);
    }
    else
    {
        printk("VOP2 DEGamma autodownload failed!\n");
    }
	kfree(u32LutData);

#else

    // RGB write together and write one time for clear write bit.
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK2D_60_L, _BIT5 | _BIT6, _BIT5 | _BIT6);
    //if( MApi_XC_W2BYTEMSK(REG_SC_Ali_BK2D_60_L, _BIT5|_BIT6, _BIT5|_BIT6) != (_BIT5|_BIT6))
    //{
    //MApi_XC_W2BYTEMSK(REG_SC_Ali_BK2D_60_L, _BIT5|_BIT6, _BIT5|_BIT6 );
    //}

    u16SpLevel = 0;
    for(u16WriteCount = 0; u16WriteCount < 600; u16WriteCount++)
    {

        u32InputData1 = ( (( u32Lut[u16WriteCount] + (1<<9)  )>>10) * g_u16Hdr3x3Ratio + (1<<8))>>(3+6);


        u16SRamCode_L = u32InputData1&0xFFFF;
        u16SRamCode_H = u32InputData1>>16;


        if(u16Delay==0)
        {
            printk("\n Kernel HDR WriteGamma timeout \n");
        }

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK2D_61_L, u16WriteCount,0xFFFF); // address
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK2D_63_L, u16SRamCode_H,0xFFFF); //data
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK2D_62_L, u16SRamCode_L,0xFFFF); //data
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK2D_60_L, _BIT3, _BIT3); // io_w enable
    }

#endif




    /*
        for(u16ReadCount = 0; u16ReadCount < 300; u16ReadCount++)
        {
            write2ByteMask(REG_SC_Ali_BK2D_61_L,0xFFFF, u16ReadCount); // address
            write2ByteMask(REG_SC_Ali_BK2D_60_L, _BIT4, _BIT4); // io_r enable
            u16SRamCode_H = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_Ali_BK2D_65_L,0xFFFF); //data
            u16SRamCode_L = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_Ali_BK2D_64_L,0xFFFF); //data
            //u32XVYCC_DGT_DATA = (u32Ref<<10) + (u32Delta1<<5) + u32Delta2;
            u32XVYCC_DGT_DATA = (u16SRamCode_H<<16) +  u16SRamCode_L;
            testRef[u16ReadCount] = u32XVYCC_DGT_DATA>>10;
            testDelta[u16ReadCount] = (u32XVYCC_DGT_DATA>>5) & 0x001F;
        }*/

    /*for(u16ReadCount = 0; u16ReadCount < 300; u16ReadCount++)
    {
        readTable[u16ReadCount*2] = testRef[u16ReadCount];
        readTable[u16ReadCount*2 + 1] = testRef[u16ReadCount] + testDelta[u16ReadCount];

        if (u16ReadCount*2 >= u16SpAddr[0])
            readTable[u16ReadCount*2]+=32;
        if (u16ReadCount*2 >= u16SpAddr[1])
            readTable[u16ReadCount*2]+=32;
        if (u16ReadCount*2 >= u16SpAddr[2])
            readTable[u16ReadCount*2]+=32;

        if ( (u16ReadCount*2 +1) >= u16SpAddr[0])
            readTable[u16ReadCount*2 + 1]+=32;
        if ( (u16ReadCount*2 +1) >= u16SpAddr[1])
            readTable[u16ReadCount*2 + 1]+=32;
        if ( (u16ReadCount*2 +1) >= u16SpAddr[2])
            readTable[u16ReadCount*2 + 1]+=32;

        diffSum += u32Lut[u16ReadCount*2] - readTable[u16ReadCount*2];
        diffSum += u32Lut[u16ReadCount*2 + 1] - readTable[u16ReadCount*2 + 1];
    }

    printk("DeGamma Table as read from Sram\n");
    for(u16ReadCount = 0; u16ReadCount < 600; u16ReadCount+=60)
    {
        printk("%d: %0#x\n", u16ReadCount,readTable[u16ReadCount]);
    }*/

    /*
        FILE *f = fopen("deGammaReadTable.txt", "w");
        if (f == NULL)
        {
            printk("Error opening file!\n");
            return;
        }
        for(int i = 0; i < 600; i++)
        {
            fprintf(f,"%d: %0#x\n", i,readTable[i]);
        }*/
}
MS_U8 setCscRegisters(cscParametersStructure* cscParameters,ST_CSC_REGISTERS* stCscRegisters,MS_U8 IpType,MS_U32 *mainBank)
{
    switch(IpType)
    {
        case E_CSC_MANHATTAN_INPUT:
            *mainBank = 0x102f00;
            //*mainBank = 0x130000;

            stCscRegisters[E_cscEnable].bExists = 1;
            stCscRegisters[E_cscEnable].u16Bank = 0x2;
            stCscRegisters[E_cscEnable].u16Address = 0x2B;
            stCscRegisters[E_cscEnable].u32Mask = 0x10;
            stCscRegisters[E_ditherEnable].bExists = 1;
            stCscRegisters[E_ditherEnable].u16Bank = 0x2;
            stCscRegisters[E_ditherEnable].u16Address = 0x2B;
            stCscRegisters[E_ditherEnable].u32Mask = 0x20;
            stCscRegisters[E_rOrCrSub16].bExists = 1;
            stCscRegisters[E_rOrCrSub16].u16Bank = 0x2;
            stCscRegisters[E_rOrCrSub16].u16Address = 0x2B;
            stCscRegisters[E_rOrCrSub16].u32Mask = 0x400;
            stCscRegisters[E_gOrYSub16].bExists = 1;
            stCscRegisters[E_gOrYSub16].u16Bank = 0x2;
            stCscRegisters[E_gOrYSub16].u16Address = 0x2B;
            stCscRegisters[E_gOrYSub16].u32Mask = 0x08;
            stCscRegisters[E_bOrCbSub16].bExists = 1;
            stCscRegisters[E_bOrCbSub16].u16Bank = 0x2;
            stCscRegisters[E_bOrCbSub16].u16Address = 0x2B;
            stCscRegisters[E_bOrCbSub16].u32Mask = 0x200;
            stCscRegisters[E_rOrCrRangeSub128].bExists = 1;
            stCscRegisters[E_rOrCrRangeSub128].u16Bank = 0x2;
            stCscRegisters[E_rOrCrRangeSub128].u16Address = 0x2B;
            stCscRegisters[E_rOrCrRangeSub128].u32Mask = 0x4;
            stCscRegisters[E_gOrYRangeSub128].bExists = 1;
            stCscRegisters[E_gOrYRangeSub128].u16Bank = 0x2;
            stCscRegisters[E_gOrYRangeSub128].u16Address = 0x2B;
            stCscRegisters[E_gOrYRangeSub128].u32Mask = 0x2;
            stCscRegisters[E_bOrCbRangeSub128].bExists = 1;
            stCscRegisters[E_bOrCbRangeSub128].u16Bank = 0x2;
            stCscRegisters[E_bOrCbRangeSub128].u16Address = 0x2B;
            stCscRegisters[E_bOrCbRangeSub128].u32Mask = 0x1;
            stCscRegisters[E_rOrCrAdd16].bExists = 1;
            stCscRegisters[E_rOrCrAdd16].u16Bank = 0x2;
            stCscRegisters[E_rOrCrAdd16].u16Address = 0x2B;
            stCscRegisters[E_rOrCrAdd16].u32Mask = 0x1000;
            stCscRegisters[E_gOrYAdd16].bExists = 1;
            stCscRegisters[E_gOrYAdd16].u16Bank = 0x2;
            stCscRegisters[E_gOrYAdd16].u16Address = 0x2B;
            stCscRegisters[E_gOrYAdd16].u32Mask = 0x8000;
            stCscRegisters[E_bOrCbAdd16].bExists = 1;
            stCscRegisters[E_bOrCbAdd16].u16Bank = 0x2;
            stCscRegisters[E_bOrCbAdd16].u16Address = 0x2B;
            stCscRegisters[E_bOrCbAdd16].u32Mask = 0x800;
            stCscRegisters[E_rOrCrAdd128].bExists = 1;
            stCscRegisters[E_rOrCrAdd128].u16Bank = 0x2;
            stCscRegisters[E_rOrCrAdd128].u16Address = 0x2B;
            stCscRegisters[E_rOrCrAdd128].u32Mask = 0x2000;
            stCscRegisters[E_bOrCbAdd128].bExists = 1;
            stCscRegisters[E_bOrCbAdd128].u16Bank = 0x2;
            stCscRegisters[E_bOrCbAdd128].u16Address = 0x2B;
            stCscRegisters[E_bOrCbAdd128].u32Mask = 0x4000;
            stCscRegisters[E_cscCoeff0].bExists = 1;
            stCscRegisters[E_cscCoeff0].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff0].u16Address = 0x2D;
            stCscRegisters[E_cscCoeff0].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff1].bExists = 1;
            stCscRegisters[E_cscCoeff1].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff1].u16Address = 0x2E;
            stCscRegisters[E_cscCoeff1].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff2].bExists = 1;
            stCscRegisters[E_cscCoeff2].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff2].u16Address = 0x2F;
            stCscRegisters[E_cscCoeff2].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff3].bExists = 1;
            stCscRegisters[E_cscCoeff3].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff3].u16Address = 0x30;
            stCscRegisters[E_cscCoeff3].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff4].bExists = 1;
            stCscRegisters[E_cscCoeff4].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff4].u16Address = 0x31;
            stCscRegisters[E_cscCoeff4].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff5].bExists = 1;
            stCscRegisters[E_cscCoeff5].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff5].u16Address = 0x32;
            stCscRegisters[E_cscCoeff5].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff6].bExists = 1;
            stCscRegisters[E_cscCoeff6].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff6].u16Address = 0x33;
            stCscRegisters[E_cscCoeff6].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff7].bExists = 1;
            stCscRegisters[E_cscCoeff7].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff7].u16Address = 0x34;
            stCscRegisters[E_cscCoeff7].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff8].bExists = 1;
            stCscRegisters[E_cscCoeff8].u16Bank = 0x2;
            stCscRegisters[E_cscCoeff8].u16Address = 0x35;
            stCscRegisters[E_cscCoeff8].u32Mask = 0x1FFF;
            stCscRegisters[E_bt2020En].bExists = 0;
            stCscRegisters[E_bt2020_cr0_11].bExists = 0;
            stCscRegisters[E_bt2020_cr1_11].bExists = 0;
            stCscRegisters[E_bt2020_cb0_33].bExists = 0;
            stCscRegisters[E_bt2020_cb1_33].bExists = 0;
            break;

        case E_CSC_MANHATTAN_OUTPUT:
            *mainBank = 0x102f00;
            //*mainBank = 0x130000;

            stCscRegisters[E_cscEnable].bExists = 1;
            stCscRegisters[E_cscEnable].u16Bank = 0x10;
            stCscRegisters[E_cscEnable].u16Address = 0x2F;
            stCscRegisters[E_cscEnable].u32Mask = 0x10;
            stCscRegisters[E_ditherEnable].bExists = 1;
            stCscRegisters[E_ditherEnable].u16Bank = 0x10;
            stCscRegisters[E_ditherEnable].u16Address = 0x2F;
            stCscRegisters[E_ditherEnable].u32Mask = 0x20;
            stCscRegisters[E_rOrCrSub16].bExists = 1;
            stCscRegisters[E_rOrCrSub16].u16Bank = 0x0F;
            stCscRegisters[E_rOrCrSub16].u16Address = 0x18;
            stCscRegisters[E_rOrCrSub16].u32Mask = 0x200;
            stCscRegisters[E_gOrYSub16].bExists = 1;
            stCscRegisters[E_gOrYSub16].u16Bank = 0x0F;
            stCscRegisters[E_gOrYSub16].u16Address = 0x57;
            stCscRegisters[E_gOrYSub16].u32Mask = 0x40;
            stCscRegisters[E_bOrCbSub16].bExists = 1;
            stCscRegisters[E_bOrCbSub16].u16Bank = 0x0F;
            stCscRegisters[E_bOrCbSub16].u16Address = 0x18;
            stCscRegisters[E_bOrCbSub16].u32Mask = 0x08;
            stCscRegisters[E_rOrCrRangeSub128].bExists = 1;
            stCscRegisters[E_rOrCrRangeSub128].u16Bank = 0x10;
            stCscRegisters[E_rOrCrRangeSub128].u16Address = 0x2F;
            stCscRegisters[E_rOrCrRangeSub128].u32Mask = 0x4;
            stCscRegisters[E_gOrYRangeSub128].bExists = 1;
            stCscRegisters[E_gOrYRangeSub128].u16Bank = 0x10;
            stCscRegisters[E_gOrYRangeSub128].u16Address = 0x2F;
            stCscRegisters[E_gOrYRangeSub128].u32Mask = 0x2;
            stCscRegisters[E_bOrCbRangeSub128].bExists = 1;
            stCscRegisters[E_bOrCbRangeSub128].u16Bank = 0x10;
            stCscRegisters[E_bOrCbRangeSub128].u16Address = 0x2F;
            stCscRegisters[E_bOrCbRangeSub128].u32Mask = 0x1;
            stCscRegisters[E_rOrCrAdd16].bExists = 1;
            stCscRegisters[E_rOrCrAdd16].u16Bank = 0x0F;
            stCscRegisters[E_rOrCrAdd16].u16Address = 0x6B;
            stCscRegisters[E_rOrCrAdd16].u32Mask = 0x8000;
            stCscRegisters[E_gOrYAdd16].bExists = 1;
            stCscRegisters[E_gOrYAdd16].u16Bank = 0x10;
            stCscRegisters[E_gOrYAdd16].u16Address = 0x50;
            stCscRegisters[E_gOrYAdd16].u32Mask = 0x80;
            stCscRegisters[E_bOrCbAdd16].bExists = 1;
            stCscRegisters[E_bOrCbAdd16].u16Bank = 0x0F;
            stCscRegisters[E_bOrCbAdd16].u16Address = 0x6B;
            stCscRegisters[E_bOrCbAdd16].u32Mask = 0x4000;
            stCscRegisters[E_rOrCrAdd128].bExists = 1;
            stCscRegisters[E_rOrCrAdd128].u16Bank = 0x10;
            stCscRegisters[E_rOrCrAdd128].u16Address = 0x50;
            stCscRegisters[E_rOrCrAdd128].u32Mask = 0x20;
            stCscRegisters[E_bOrCbAdd128].bExists = 1;
            stCscRegisters[E_bOrCbAdd128].u16Bank = 0x10;
            stCscRegisters[E_bOrCbAdd128].u16Address = 0x50;
            stCscRegisters[E_bOrCbAdd128].u32Mask = 0x40;
            stCscRegisters[E_cscCoeff0].bExists = 1;
            stCscRegisters[E_cscCoeff0].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff0].u16Address = 0x26;
            stCscRegisters[E_cscCoeff0].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff1].bExists = 1;
            stCscRegisters[E_cscCoeff1].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff1].u16Address = 0x27;
            stCscRegisters[E_cscCoeff1].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff2].bExists = 1;
            stCscRegisters[E_cscCoeff2].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff2].u16Address = 0x28;
            stCscRegisters[E_cscCoeff2].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff3].bExists = 1;
            stCscRegisters[E_cscCoeff3].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff3].u16Address = 0x29;
            stCscRegisters[E_cscCoeff3].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff4].bExists = 1;
            stCscRegisters[E_cscCoeff4].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff4].u16Address = 0x2A;
            stCscRegisters[E_cscCoeff4].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff5].bExists = 1;
            stCscRegisters[E_cscCoeff5].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff5].u16Address = 0x2B;
            stCscRegisters[E_cscCoeff5].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff6].bExists = 1;
            stCscRegisters[E_cscCoeff6].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff6].u16Address = 0x2C;
            stCscRegisters[E_cscCoeff6].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff7].bExists = 1;
            stCscRegisters[E_cscCoeff7].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff7].u16Address = 0x2D;
            stCscRegisters[E_cscCoeff7].u32Mask = 0x1FFF;
            stCscRegisters[E_cscCoeff8].bExists = 1;
            stCscRegisters[E_cscCoeff8].u16Bank = 0x10;
            stCscRegisters[E_cscCoeff8].u16Address = 0x3E;
            stCscRegisters[E_cscCoeff8].u32Mask = 0x1FFF;
            stCscRegisters[E_bt2020En].bExists = 1;
            stCscRegisters[E_bt2020En].u16Bank = 0x25;
            stCscRegisters[E_bt2020En].u16Address = 0x38;
            stCscRegisters[E_bt2020En].u32Mask = 0x1FFF;
            stCscRegisters[E_bt2020_cr0_11].bExists = 1;
            stCscRegisters[E_bt2020_cr0_11].u16Bank = 0x25;
            stCscRegisters[E_bt2020_cr0_11].u16Address = 0x38;
            stCscRegisters[E_bt2020_cr0_11].u32Mask = 0x8000;
            stCscRegisters[E_bt2020_cr1_11].bExists = 1;
            stCscRegisters[E_bt2020_cr1_11].u16Bank = 0x25;
            stCscRegisters[E_bt2020_cr1_11].u16Address = 0x39;
            stCscRegisters[E_bt2020_cr1_11].u32Mask = 0x1FFF;
            stCscRegisters[E_bt2020_cb0_33].bExists = 1;
            stCscRegisters[E_bt2020_cb0_33].u16Bank = 0x25;
            stCscRegisters[E_bt2020_cb0_33].u16Address = 0x3A;
            stCscRegisters[E_bt2020_cb0_33].u32Mask = 0x1FFF;
            stCscRegisters[E_bt2020_cb1_33].bExists = 1;
            stCscRegisters[E_bt2020_cb1_33].u16Bank = 0x25;
            stCscRegisters[E_bt2020_cb1_33].u16Address = 0x3B;
            stCscRegisters[E_bt2020_cb1_33].u32Mask = 0x1FFF;
            break;
        default:
            printk("current CSC IP type unsupported!\n");
            return 0;
            break;
    }

    stCscRegisters[E_cscEnable].s32Val = cscParameters->cscEnable *  stCscRegisters[E_cscEnable].u32Mask;
    stCscRegisters[E_ditherEnable].s32Val = cscParameters->ditherEnable * stCscRegisters[E_ditherEnable].u32Mask;
    stCscRegisters[E_rOrCrSub16].s32Val = cscParameters->rOrCrSub16 * stCscRegisters[E_rOrCrSub16].u32Mask;
    stCscRegisters[E_gOrYSub16].s32Val = cscParameters->gOrYSub16 * stCscRegisters[E_gOrYSub16].u32Mask;
    stCscRegisters[E_bOrCbSub16].s32Val = cscParameters->bOrCbSub16 * stCscRegisters[E_bOrCbSub16].u32Mask;
    stCscRegisters[E_rOrCrRangeSub128].s32Val = cscParameters->rOrCrRangeSub128 * stCscRegisters[E_rOrCrRangeSub128].u32Mask;
    stCscRegisters[E_gOrYRangeSub128].s32Val = cscParameters->gOrYRangeSub128 * stCscRegisters[E_gOrYRangeSub128].u32Mask;
    stCscRegisters[E_bOrCbRangeSub128].s32Val = cscParameters->bOrCbRangeSub128 * stCscRegisters[E_bOrCbRangeSub128].u32Mask;
    stCscRegisters[E_rOrCrAdd16].s32Val = cscParameters->rOrCrAdd16 * stCscRegisters[E_rOrCrAdd16].u32Mask;
    stCscRegisters[E_gOrYAdd16].s32Val = cscParameters->gOrYAdd16 * stCscRegisters[E_gOrYAdd16].u32Mask;
    stCscRegisters[E_bOrCbAdd16].s32Val = cscParameters->bOrCbAdd16 * stCscRegisters[E_bOrCbAdd16].u32Mask;
    stCscRegisters[E_rOrCrAdd128].s32Val = cscParameters->rOrCrAdd128 * stCscRegisters[E_rOrCrAdd128].u32Mask;
    stCscRegisters[E_bOrCbAdd128].s32Val = cscParameters->bOrCbAdd128 * stCscRegisters[E_bOrCbAdd128].u32Mask;
    stCscRegisters[E_cscCoeff0].s32Val = cscParameters->cscCoeff[0][0];
    stCscRegisters[E_cscCoeff1].s32Val = cscParameters->cscCoeff[0][1];
    stCscRegisters[E_cscCoeff2].s32Val = cscParameters->cscCoeff[0][2];
    stCscRegisters[E_cscCoeff3].s32Val = cscParameters->cscCoeff[1][0];
    stCscRegisters[E_cscCoeff4].s32Val = cscParameters->cscCoeff[1][1];
    stCscRegisters[E_cscCoeff5].s32Val = cscParameters->cscCoeff[1][2];
    stCscRegisters[E_cscCoeff6].s32Val = cscParameters->cscCoeff[2][0];
    stCscRegisters[E_cscCoeff7].s32Val = cscParameters->cscCoeff[2][1];
    stCscRegisters[E_cscCoeff8].s32Val = cscParameters->cscCoeff[2][2];
    stCscRegisters[E_bt2020En].s32Val = cscParameters->bt2020En * stCscRegisters[E_bt2020En].u32Mask;
    stCscRegisters[E_bt2020_cr0_11].s32Val = cscParameters->bt2020_cr0_11;
    stCscRegisters[E_bt2020_cr1_11].s32Val = cscParameters->bt2020_cr1_11;
    stCscRegisters[E_bt2020_cb0_33].s32Val = cscParameters->bt2020_cb0_33;
    stCscRegisters[E_bt2020_cb1_33].s32Val = cscParameters->bt2020_cb1_33;


    return 1;
}
MS_U8 cscWrite2registersAll(cscParametersStructure* cscParameters,MS_U8 IpType)
{


    MS_U8 status = 1;
    ST_CSC_REGISTERS stCscRegisters[E_CSC_REGISTERS_NUM];
    MS_U32 mainBank;
    MS_U32 i = 0;
    status = setCscRegisters(cscParameters,stCscRegisters, IpType,&mainBank);
    if(0 == status)
    {
        return 0;
    }
    if(1 == stCscRegisters[E_cscEnable].s32Val)
    {
        for ( i = 0; i < E_CSC_REGISTERS_NUM-1; i++)
        {
            if(stCscRegisters[i].bExists == 0 && stCscRegisters[i].s32Val != 0 )
            {
                printk("This CSC does not support this mode, CSC has not been updated.\n");
                printk("Error value is%0#x\n", stCscRegisters[i].s32Val );
                printk("Error value is number:%d\n", i );
                return 0;
            }
        }
    }


    for(i = E_cscCoeff0; i <= E_cscCoeff8; i++)
    {
        S16 temp = ((( abs(stCscRegisters[i].s32Val) << 10)+ 25000) / 50000);
        stCscRegisters[i].s32Val = (stCscRegisters[i].s32Val >= 0) ? temp & 0x1FFF : (-1*temp) &0x1FFF;
    }

#ifdef  CMODEL


    FILE *f = fopen("CscScript.txt", "w");
    if (f == NULL)
    {
        printk("Error opening CSC write file!\n");
        return 0;
    }
    //stCscRegisters[E_cscEnable].u32Mask = cscParameters->cscEnable;
    printk("EEnable val  is %0#x\n", stCscRegisters[E_cscEnable].s32Val );
#endif
    for ( i = 0; i < E_CSC_REGISTERS_NUM; i++)
    {
        if(1 == stCscRegisters[i].bExists)
        {
            //write2ByteMask(mainBank + stCscRegisters[i].u16Bank<<8 + stCscRegisters[i].u16Address*2, stCscRegisters[i].u32Mask, stCscRegisters[i].s32Val);
#ifdef  CMODEL

#if 0

            fprintf(f,"wriu  %0#x %0#x\n", mainBank,
                    stCscRegisters[i].u16Bank);
            fprintf(f,"wriu -b %0#x %0#x %0#x\n", mainBank+ /*(stCscRegisters[i].u16Bank<<8)  +*/ stCscRegisters[i].u16Address*2,
                    stCscRegisters[i].u32Mask & 0x00FF,
                    stCscRegisters[i].s32Val & 0x00FF);
            fprintf(f,"wriu -b %0#x %0#x %0#x\n", mainBank + /*(stCscRegisters[i].u16Bank<<8) +*/ stCscRegisters[i].u16Address*2+1,
                    stCscRegisters[i].u32Mask >> 8,
                    stCscRegisters[i].s32Val >> 8);
#endif

            /*
                        fprintf(f,"wriu -b %0#x %0#x %0#x\n", mainBank+ (stCscRegisters[i].u16Bank<<8)  + stCscRegisters[i].u16Address*2,
                            stCscRegisters[i].u32Mask & 0x00FF,
                            stCscRegisters[i].s32Val & 0x00FF);
                        fprintf(f,"wriu -b %0#x %0#x %0#x\n", mainBank + (stCscRegisters[i].u16Bank<<8) + stCscRegisters[i].u16Address*2+1,
                            stCscRegisters[i].u32Mask >> 8,
                            stCscRegisters[i].s32Val >> 8);*/
#endif
        }

    }





    return 1;

}
void inputCscWrtite2registerMaseratiSub(cscParametersStructure* cscParameters)
{
    U16 u16Regs[3][3];
    MS_U8 i,j;
    U16 flags = cscParameters->bOrCbRangeSub128
                | (cscParameters->gOrYRangeSub128 << 1)
                | (cscParameters->rOrCrRangeSub128 << 2)
                | (cscParameters->gOrYSub16 << 3)
                | (cscParameters->cscEnable << 4)
                | (cscParameters->ditherEnable << 5)
                | (cscParameters->bOrCbSub16 << 9)
                | (cscParameters->rOrCrSub16 << 10)
                | (cscParameters->bOrCbAdd16 << 11)
                | (cscParameters->rOrCrAdd16 << 12)
                | (cscParameters->rOrCrAdd128 << 13)
                | (cscParameters->bOrCbAdd128 << 14)
                | (cscParameters->gOrYAdd16 << 15);


    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
        {
            S16 temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10)+ 25000) / 50000);
            u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0x1FFF : (-1*temp) &0x1FFF;
        }

    write2ByteMask(_PK_L_(0x82,0x2B), 0xFE3F, flags);
    write2Byte(_PK_L_(0x82,0x2D), u16Regs[0][0]);
    write2Byte(_PK_L_(0x82,0x2E), u16Regs[0][1]);
    write2Byte(_PK_L_(0x82,0x2F), u16Regs[0][2]);
    write2Byte(_PK_L_(0x82,0x30), u16Regs[1][0]);
    write2Byte(_PK_L_(0x82,0x31), u16Regs[1][1]);
    write2Byte(_PK_L_(0x82,0x32), u16Regs[1][2]);
    write2Byte(_PK_L_(0x82,0x33), u16Regs[2][0]);
    write2Byte(_PK_L_(0x82,0x34), u16Regs[2][1]);
    write2Byte(_PK_L_(0x82,0x35), u16Regs[2][2]);
}
void inputCscWrtite2registerManhattan(cscParametersStructure* cscParameters)
{
    U16 u16Regs[3][3];
    MS_U8 i,j;
    U16 flags = cscParameters->bOrCbRangeSub128
                | (cscParameters->gOrYRangeSub128 << 1)
                | (cscParameters->rOrCrRangeSub128 << 2)
                | (cscParameters->gOrYSub16 << 3)
                | (cscParameters->cscEnable << 4)
                | (cscParameters->ditherEnable << 5)
                | (cscParameters->bOrCbSub16 << 9)
                | (cscParameters->rOrCrSub16 << 10)
                | (cscParameters->bOrCbAdd16 << 11)
                | (cscParameters->rOrCrAdd16 << 12)
                | (cscParameters->rOrCrAdd128 << 13)
                | (cscParameters->bOrCbAdd128 << 14)
                | (cscParameters->gOrYAdd16 << 15);


    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
        {
            S16 temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10)+ 25000) / 50000);
            u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0x1FFF : (-1*temp) &0x1FFF;
        }
    write2ByteMask(REG_SC_Ali_BK02_2B_L, 0xFE3F, flags);
    write2Byte(REG_SC_Ali_BK02_2D_L, u16Regs[0][0]);
    write2Byte(REG_SC_Ali_BK02_2E_L, u16Regs[0][1]);
    write2Byte(REG_SC_Ali_BK02_2F_L, u16Regs[0][2]);
    write2Byte(REG_SC_Ali_BK02_30_L, u16Regs[1][0]);
    write2Byte(REG_SC_Ali_BK02_31_L, u16Regs[1][1]);
    write2Byte(REG_SC_Ali_BK02_32_L, u16Regs[1][2]);
    write2Byte(REG_SC_Ali_BK02_33_L, u16Regs[2][0]);
    write2Byte(REG_SC_Ali_BK02_34_L, u16Regs[2][1]);
    write2Byte(REG_SC_Ali_BK02_35_L, u16Regs[2][2]);
}
void outputCscWrtite2registerMaseratiSUb(cscParametersStructure* cscParameters)
{
    U16 u16Regs[3][3];
    U16    u16Bt2020_cr0_11;
    U16    u16Bt2020_cr1_11;
    U16    u16Bt2020_cb0_33;
    U16    u16Bt2020_cb1_33;
    S16 s16Temp;

    U16 flags0F26 = cscParameters->rOrCrRangeSub128
                    | (cscParameters->gOrYRangeSub128 << 1)
                    | (cscParameters->bOrCbRangeSub128 << 2)
                    | (cscParameters->cscEnable << 4)
                    | (cscParameters->ditherEnable << 5);

    U16 flags0f18 = (cscParameters->rOrCrAdd128 << 5)
                    | (cscParameters->bOrCbAdd128 << 6)
                    | (cscParameters->gOrYAdd16 << 7)
                    | (cscParameters->bOrCbSub16 << 1)
                    | (cscParameters->rOrCrSub16 << 2);

    U16 flags0F6B = (cscParameters->bOrCbAdd16 << 12)
                    | (cscParameters->rOrCrAdd16 << 13);

    U16 flags0f57 = (cscParameters->gOrYSub16 << 7);

    MS_U8 i,j;

    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
        {
            s16Temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10) + 25000) / 50000);
            u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;
        }

    s16Temp = ((( abs(cscParameters->bt2020_cr0_11) << 10) + 25000) / 50000);
    u16Bt2020_cr0_11 = (cscParameters->bt2020_cr0_11 >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;
    s16Temp = ((( abs(cscParameters->bt2020_cr1_11) << 10) + 25000) / 50000);
    u16Bt2020_cr1_11 = (cscParameters->bt2020_cr1_11 >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;
    s16Temp = ((( abs(cscParameters->bt2020_cb0_33) << 10) + 25000) / 50000);
    u16Bt2020_cb0_33 = (cscParameters->bt2020_cb0_33 >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;
    s16Temp = ((( abs(cscParameters->bt2020_cb1_33) << 10) + 25000) / 50000);
    u16Bt2020_cb1_33 = (cscParameters->bt2020_cb1_33 >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;

    write2ByteMask(_PK_L_(0x0f,0x26), 0x0037, flags0F26);
    write2ByteMask(_PK_L_(0x0f,0x18), 0x00E6, flags0f18);
    write2ByteMask(_PK_L_(0x0f,0x6B), 0x0300, flags0F6B);
    write2ByteMask(_PK_L_(0x0f,0x57), 0x0080, flags0f57);
    write2Byte(_PK_L_(0x0f,0x1D), u16Regs[0][0]);
    write2Byte(_PK_L_(0x0f,0x1E), u16Regs[0][1]);
    write2Byte(_PK_L_(0x0f,0x1F), u16Regs[0][2]);
    write2Byte(_PK_L_(0x0f,0x20), u16Regs[1][0]);
    write2Byte(_PK_L_(0x0f,0x21), u16Regs[1][1]);
    write2Byte(_PK_L_(0x0f,0x22), u16Regs[1][2]);
    write2Byte(_PK_L_(0x0f,0x23), u16Regs[2][0]);
    write2Byte(_PK_L_(0x0f,0x24), u16Regs[2][1]);
    write2Byte(_PK_L_(0x0f,0x25), u16Regs[2][2]);

    //2020CL settings
    write2Byte(REG_SC_Ali_BK25_3C_L, u16Bt2020_cr0_11 | (cscParameters->bt2020En<<15));
    write2Byte(REG_SC_Ali_BK25_3D_L, u16Bt2020_cr1_11);
    write2Byte(REG_SC_Ali_BK25_3E_L, u16Bt2020_cb0_33);
    write2Byte(REG_SC_Ali_BK25_3F_L, u16Bt2020_cb1_33);


}
void outputCscWrtite2registerManhattan(cscParametersStructure* cscParameters)
{

    if(0 == cscParameters->cscEnable) // use identity matrix instead of bypass for OSD sat hue contrast adjustment purposes
    {
        write2ByteMask(REG_SC_Ali_BK10_2F_L, 0x0037, 0x30);
        write2ByteMask(REG_SC_Ali_BK10_50_L, 0x00E0, 0xe0);
        write2ByteMask(REG_SC_Ali_BK0F_6B_L, 0xC000, 0xc000);
        write2ByteMask(REG_SC_Ali_BK0F_18_L, 0x0208, 0x208);
        write2ByteMask(REG_SC_Ali_BK0F_57_L, 0x0040, 0x40);
        write2Byte(REG_SC_Ali_BK10_26_L, 0x0400);
        write2Byte(REG_SC_Ali_BK10_27_L, 0x0);
        write2Byte(REG_SC_Ali_BK10_28_L, 0x0);
        write2Byte(REG_SC_Ali_BK10_29_L, 0x0);
        write2Byte(REG_SC_Ali_BK10_2A_L, 0x0400);
        write2Byte(REG_SC_Ali_BK10_2B_L, 0x0);
        write2Byte(REG_SC_Ali_BK10_2C_L, 0x0);
        write2Byte(REG_SC_Ali_BK10_2D_L, 0x0);
        write2Byte(REG_SC_Ali_BK10_2E_L, 0x0400);

        //2020CL settings
        write2Byte(REG_SC_Ali_BK25_38_L, 0x0);
        write2Byte(REG_SC_Ali_BK25_39_L, 0x0);
        write2Byte(REG_SC_Ali_BK25_3A_L, 0x0);
        write2Byte(REG_SC_Ali_BK25_3B_L, 0x0);

    }
    else
    {
        U16 u16Regs[3][3];
        U16    u16Bt2020_cr0_11;
        U16    u16Bt2020_cr1_11;
        U16    u16Bt2020_cb0_33;
        U16    u16Bt2020_cb1_33;
        S16 s16Temp;
        U16 flags102F = cscParameters->rOrCrRangeSub128 //
                        | (cscParameters->gOrYRangeSub128 << 1)//
                        | (cscParameters->bOrCbRangeSub128 << 2)//
                        | (cscParameters->cscEnable << 4) //
                        | (cscParameters->ditherEnable << 5); //

        U16 flags1050 = (cscParameters->rOrCrAdd128 << 5)
                        | (cscParameters->bOrCbAdd128 << 6)
                        | (cscParameters->gOrYAdd16 << 7);

        U16 flags0F6B = (cscParameters->bOrCbAdd16 << 14) //
                        | (cscParameters->rOrCrAdd16 << 15); //

        U16 flags0f18 = (cscParameters->bOrCbSub16 << 3) //
                        | (cscParameters->rOrCrSub16 << 9); //

        U16 flags0f57 = (cscParameters->gOrYSub16 << 6);//

        MS_U8 i,j;

        for( i = 0; i < 3; i++)
            for( j = 0; j < 3; j++)
            {
                s16Temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10) + 25000) / 50000);
                u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;
            }

        s16Temp = ((( abs(cscParameters->bt2020_cr0_11) << 10) + 25000) / 50000);
        u16Bt2020_cr0_11 = (cscParameters->bt2020_cr0_11 >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;
        s16Temp = ((( abs(cscParameters->bt2020_cr1_11) << 10) + 25000) / 50000);
        u16Bt2020_cr1_11 = (cscParameters->bt2020_cr1_11 >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;
        s16Temp = ((( abs(cscParameters->bt2020_cb0_33) << 10) + 25000) / 50000);
        u16Bt2020_cb0_33 = (cscParameters->bt2020_cb0_33 >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;
        s16Temp = ((( abs(cscParameters->bt2020_cb1_33) << 10) + 25000) / 50000);
        u16Bt2020_cb1_33 = (cscParameters->bt2020_cb1_33 >= 0) ? s16Temp & 0x1FFF : -1*s16Temp &0x1FFF;

        write2ByteMask(REG_SC_Ali_BK10_2F_L, 0x0037, flags102F);
        write2ByteMask(REG_SC_Ali_BK10_50_L, 0x00E0, flags1050);
        write2ByteMask(REG_SC_Ali_BK0F_6B_L, 0xC000, flags0F6B);
        write2ByteMask(REG_SC_Ali_BK0F_18_L, 0x0208, flags0f18);
        write2ByteMask(REG_SC_Ali_BK0F_57_L, 0x0040, flags0f57);
        write2Byte(REG_SC_Ali_BK10_26_L, u16Regs[0][0]);
        write2Byte(REG_SC_Ali_BK10_27_L, u16Regs[0][1]);
        write2Byte(REG_SC_Ali_BK10_28_L, u16Regs[0][2]);
        write2Byte(REG_SC_Ali_BK10_29_L, u16Regs[1][0]);
        write2Byte(REG_SC_Ali_BK10_2A_L, u16Regs[1][1]);
        write2Byte(REG_SC_Ali_BK10_2B_L, u16Regs[1][2]);
        write2Byte(REG_SC_Ali_BK10_2C_L, u16Regs[2][0]);
        write2Byte(REG_SC_Ali_BK10_2D_L, u16Regs[2][1]);
        write2Byte(REG_SC_Ali_BK10_2E_L, u16Regs[2][2]);

        //2020CL settings
        write2Byte(REG_SC_Ali_BK25_38_L, u16Bt2020_cr0_11 | (cscParameters->bt2020En<<15));
        write2Byte(REG_SC_Ali_BK25_39_L, u16Bt2020_cr1_11);
        write2Byte(REG_SC_Ali_BK25_3A_L, u16Bt2020_cb0_33);
        write2Byte(REG_SC_Ali_BK25_3B_L, u16Bt2020_cb1_33);


    }
}
MS_U8 outputCscWrtite2registerVOP2(cscParametersStructure* cscParameters)
{
    U16 u16Regs[3][3];
    MS_U32 u32checkSum = 0;
    MS_U8 i,j;

    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
        {
            S16 temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10) + 25000) / 50000);
			u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? ((min(temp, 0x0FFF)) & 0x0FFF) : ((min(-1*temp, 0x1FFF)) & 0x1FFF);
            u32checkSum+=u16Regs[i][j];
        }

    if( 0 == u32checkSum)
    {
        return 0;
    }
    else
    {
        write2Byte(REG_SC_Ali_BK10_26_L, u16Regs[0][0]);
        write2Byte(REG_SC_Ali_BK10_27_L, u16Regs[0][1]);
        write2Byte(REG_SC_Ali_BK10_28_L, u16Regs[0][2]);
        write2Byte(REG_SC_Ali_BK10_29_L, u16Regs[1][0]);
        write2Byte(REG_SC_Ali_BK10_2A_L, u16Regs[1][1]);
        write2Byte(REG_SC_Ali_BK10_2B_L, u16Regs[1][2]);
        write2Byte(REG_SC_Ali_BK10_2C_L, u16Regs[2][0]);
        write2Byte(REG_SC_Ali_BK10_2D_L, u16Regs[2][1]);
        write2Byte(REG_SC_Ali_BK10_2E_L, u16Regs[2][2]);

        //2020CL off
        write2Byte(REG_SC_Ali_BK25_38_L, 0x0000);
        write2Byte(REG_SC_Ali_BK25_39_L, 0x0000);
        write2Byte(REG_SC_Ali_BK25_3A_L, 0x0000);
        write2Byte(REG_SC_Ali_BK25_3B_L, 0x0000);
        return 1;
    }

}
MS_U8 outputCscWrtite2registerVOP2SUb(cscParametersStructure* cscParameters)
{
    U16 u16Regs[3][3];
    S16 s16Temp;
    MS_U32 u32checkSum = 0;
    MS_U8 i,j;

    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
        {
            s16Temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10) + 25000) / 50000);
			u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? ((min(s16Temp, 0x0FFFF)) & 0x0FFF) : ((min(-1*s16Temp, 0x1FFF)) & 0x1FFF);
            u32checkSum+=u16Regs[i][j];
        }
    if( 0 == u32checkSum)
    {
        return 0;
    }
    else
    {
        write2Byte(_PK_L_(0x0f,0x1D), u16Regs[0][0]);
        write2Byte(_PK_L_(0x0f,0x1E), u16Regs[0][1]);
        write2Byte(_PK_L_(0x0f,0x1F), u16Regs[0][2]);
        write2Byte(_PK_L_(0x0f,0x20), u16Regs[1][0]);
        write2Byte(_PK_L_(0x0f,0x21), u16Regs[1][1]);
        write2Byte(_PK_L_(0x0f,0x22), u16Regs[1][2]);
        write2Byte(_PK_L_(0x0f,0x23), u16Regs[2][0]);
        write2Byte(_PK_L_(0x0f,0x24), u16Regs[2][1]);
        write2Byte(_PK_L_(0x0f,0x25), u16Regs[2][2]);

        //2020CL off
        write2Byte(REG_SC_Ali_BK25_3C_L, 0x0000);
        write2Byte(REG_SC_Ali_BK25_3D_L, 0x0000);
        write2Byte(REG_SC_Ali_BK25_3E_L, 0x0000);
        write2Byte(REG_SC_Ali_BK25_3F_L, 0x0000);
        return 1;
    }


}
MS_U8 CscWrite2Register(cscParametersStructure* cscParameters,MS_U8 IpType)
{
    switch(IpType)
    {
        case E_CSC_MANHATTAN_INPUT:
            inputCscWrtite2registerManhattan(cscParameters);
            break;
        case E_CSC_MANHATTAN_OUTPUT:
            outputCscWrtite2registerManhattan(cscParameters);
            break;
        case E_CSC_MASERTATI_HDR_INPUT:
            inputCscHdrWrite2RegisterMaserati(cscParameters);
            break;
    //case E_CSC_MASERTATI_HDR_OUTPUT:
	    //outputCscHdrWrite2RegisterMaserati(cscParameters);
		//break;
        case E_CSC_MASERATI_INPUT_SUB:
            inputCscWrtite2registerMaseratiSub(cscParameters);
            break;
        case E_CSC_MASERATI_OUTPUT_SUB:
            outputCscWrtite2registerMaseratiSUb(cscParameters);
            break;
        //case E_CSC_MASERATI_HDR_MAXRGB:
        // hdrCscMaxRgbWrite2RegisterMaserati(cscParameters, mode, matrixCoefficents, ratio, debugMatrixMode, registerDebugEn, IpType);
        // break;

        default:
            printk("current CSC IP type unsupported!\n");
            return 0;
            break;
    }
    return 1;
}
MS_U8 CscWrite2RegisterRecursive(cscParametersStructure* cscParameters,MS_U8 mode,MS_U8 matrixCoefficents,MS_U8 ratio,MS_U8 debugMatrixMode,MS_U8 registerDebugEn,MS_U8 IpType)
{
    MS_BOOL ubStatus = 1;
    switch(IpType)
    {
        case E_CSC_MANHATTAN_INPUT:
            inputCscWrtite2registerManhattan(cscParameters);
            break;
        case E_CSC_MANHATTAN_OUTPUT:
            outputCscWrtite2registerManhattan(cscParameters);
            break;
        case E_CSC_MASERTATI_HDR_INPUT:
            inputCscHdrWrite2RegisterMaserati(cscParameters);
            break;
        case E_CSC_MASERTATI_HDR_OUTPUT:
		outputCscHdrWrite2RegisterMaserati(cscParameters, mode);
            break;
        case E_CSC_MASERATI_INPUT_SUB:
            inputCscWrtite2registerMaseratiSub(cscParameters);
            break;
        case E_CSC_MASERATI_OUTPUT_SUB:
            outputCscWrtite2registerMaseratiSUb(cscParameters);
            break;
        case E_CSC_MASERATI_HDR_MAXRGB:
            ubStatus = hdrCscMaxRgbWrite2RegisterMaserati(cscParameters, mode, matrixCoefficents, ratio, debugMatrixMode, registerDebugEn, IpType);
            break;

        default:
            printk("current CSC IP type unsupported!\n");
            return 0;
            break;
    }
    return ubStatus;
}

void parseInput4Csc(const MS_U8 *mode,const MS_U8 *matrixCoefficents,MS_U8 *inputColorSpace,MS_U8  *outputColorSpace,MS_U8  *inputRange,MS_U8 *outputRange,MS_U8 *colorType,MS_U8 *status, MS_U8 *cscMode)
{

    switch(*mode)
    {

        case E_CFD_IP_CSC_RFULL_TO_RLIMIT:
            *inputColorSpace = Rgb;
            *outputColorSpace = Rgb;
            *inputRange = fullRange;
            *outputRange = limitedRange;
            break;
        case E_CFD_IP_CSC_RFULL_TO_YFULL:
            *inputColorSpace = Rgb;
            *outputColorSpace = Ycbcr;
            *inputRange = fullRange;
            *outputRange = fullRange;
            break;
        case E_CFD_IP_CSC_RFULL_TO_YLIMIT:
            *inputColorSpace = Rgb;
            *outputColorSpace = Ycbcr;
            *inputRange = fullRange;
            *outputRange = limitedRange;
            break;
        case E_CFD_IP_CSC_RLIMIT_TO_RFULL:
            *inputColorSpace = Rgb;
            *outputColorSpace = Rgb;
            *inputRange = limitedRange;
            *outputRange = fullRange;
            break;

        case E_CFD_IP_CSC_RLIMIT_TO_YFULL:
            *inputColorSpace = Rgb;
            *outputColorSpace = Ycbcr;
            *inputRange = limitedRange;
            *outputRange = fullRange;
            break;
        case E_CFD_IP_CSC_RLIMIT_TO_YLIMIT:
            *inputColorSpace = Rgb;
            *outputColorSpace = Ycbcr;
            *inputRange = limitedRange;
            *outputRange = limitedRange;
            break;
        ////////
        case E_CFD_IP_CSC_YFULL_TO_RFULL:
            *inputColorSpace = Ycbcr;
            *outputColorSpace = Rgb;
            *inputRange = fullRange;
            *outputRange = fullRange;
            break;
        case E_CFD_IP_CSC_YFULL_TO_RLIMIT:
            *inputColorSpace = Ycbcr;
            *outputColorSpace = Rgb;
            *inputRange = fullRange;
            *outputRange = limitedRange;
            break;

        case E_CFD_IP_CSC_YFULL_TO_YLIMIT:
            *inputColorSpace = Ycbcr;
            *outputColorSpace = Ycbcr;
            *inputRange = fullRange;
            *outputRange = limitedRange;
            break;
        case E_CFD_IP_CSC_YLIMIT_TO_RFULL:
            *inputColorSpace = Ycbcr;
            *outputColorSpace = Rgb;
            *inputRange = limitedRange;
            *outputRange = fullRange;
            break;
        case E_CFD_IP_CSC_YLIMIT_TO_RLIMIT:
            *inputColorSpace = Ycbcr;
            *outputColorSpace = Rgb;
            *inputRange = limitedRange;
            *outputRange = limitedRange;
            break;
        case E_CFD_IP_CSC_YLIMIT_TO_YFULL:
            *inputColorSpace = Ycbcr;
            *outputColorSpace = Ycbcr;
            *inputRange = limitedRange;
            *outputRange = fullRange;
            break;

        case E_CFD_IP_CSC_OFF:
            *inputColorSpace = 0xFF;
            *outputColorSpace = 0xFF;
            *inputRange = 0xFF;
            *outputRange = 0xFF;
            *cscMode = E_CSCOFF;
            break;
        default:
            *inputColorSpace = 0xFF;
            *outputColorSpace = 0xFF;
            *inputRange = 0xFF;
            *outputRange = 0xFF;
            *cscMode = E_CSCOFF;
            //printk("Csc ERROR! mode out of bound, CSC is turned off.\n");
            *status = 0;
            break;
    }

    switch(*matrixCoefficents)
    {
        case 0:
        case 2:
        case 3:
            *colorType = 0;
            if (inputColorSpace != outputColorSpace)
            {
                //printk("Csc ERROR! matrix coefficient is unspecific, switching to Bt709.\n");
                *status = 0;
            }
        case 1:
            *colorType = 0;
            break;
        case 4:
            *colorType = 1;
            break;
        case 5:
        case 6:
            *colorType = 2;
            break;
        case 7:
            *colorType = 3;
            break;
        case 9:
            *colorType = 4;
            break;
        case 8:
            *colorType = 5;
            break;
        case 10:
            if(*inputColorSpace == *outputColorSpace)
            {
                *colorType = 0;
            }
            else
            {
                *colorType = 6;
            }
            break;
        default:
            *colorType = 0x0;
            //printk("Csc ERROR! matrix coefficient out of bound, switching to Bt709.\n");
            *status = 0;
            break;
    }
}
void matrixInvGen(S32 Minv[3][3], S32 M[3][3])
{
    S32 a = M[0][0];
    S32 b = M[0][1];
    S32 c = M[0][2];
    S32 d = M[1][0];
    S32 e = M[1][1];
    S32 f = M[1][2];
    S32 g = M[2][0];
    S32 h = M[2][1];
    S32 i = M[2][2];

    S64 ei_fh = (S64)e*i - (S64)f*h; // a b c a
    S64 fg_di = (S64)f*g - (S64)d*i; // d e f d
    S64 dh_eg = (S64)d*h - (S64)e*g; // g h i g
    S64 hc_ib = (S64)h*c - (S64)i*b; // a b c a
    S64 ia_gc = (S64)i*a - (S64)g*c;
    S64 gb_ha = (S64)g*b - (S64)h*a;
    S64 bf_ce = (S64)b*f - (S64)c*e;
    S64 cd_af = (S64)c*d - (S64)a*f;
    S64 ae_bd = (S64)a*e - (S64)b*d;

    S64 det = a*ei_fh + b*fg_di + c*dh_eg;

    Minv[0][0] = ei_fh * 2500000000LL / det;
    Minv[1][0] = fg_di * 2500000000LL  / det;
    Minv[2][0] = dh_eg * 2500000000LL / det;
    Minv[0][1] = hc_ib * 2500000000LL  / det;
    Minv[1][1] = ia_gc * 2500000000LL / det;
    Minv[2][1] = gb_ha * 2500000000LL / det;
    Minv[0][2] = bf_ce * 2500000000LL  / det;
    Minv[1][2] = cd_af * 2500000000LL  / det;
    Minv[2][2] = ae_bd * 2500000000LL  / det;

}
MS_U8  cscGenerate3x3(cscParametersStructure* cscParameters,MS_U8 inputColorSpace, MS_U8 outputColorSpace, MS_U8 colorType, bool inputRange, bool outputRange,MS_U8 ratio, MS_U8 cscMode)
{
    //double kb[5] = {0.0722,0.11,0.114,0.087,0.0593};
    //double kr[5] = {0.2126,0.3,0.299,0.212,0.2627};
    //709  -- FCC -- 601 -- SMPTE -- 2020NCL
    MS_U16 kbArray[5] = {3610,5500,5700,4350,2965};
    MS_U16 krArray[5] = {10630,15000,14950, 10600,13135};
    MS_U16 kb = kbArray[colorType];
    MS_U16 kr = krArray[colorType];
    MS_U16 kg = 50000 - kb - kr;
    MS_U16 inRange_gOrY, inRange_rbOrCrCb;
    MS_U16 outRange_gOrY, outRange_rbOrCrCb;

    switch (cscMode)
    {
        case E_CSCOFF:
            cscParameters->cscEnable = 0;
            cscParameters->ditherEnable = 0;
            cscParameters->rOrCrSub16 = 0;
            cscParameters->gOrYSub16 = 0;
            cscParameters->bOrCbSub16 = 0;
            cscParameters->rOrCrRangeSub128 = 0;
            cscParameters->gOrYRangeSub128 = 0;
            cscParameters->bOrCbRangeSub128 = 0;
            cscParameters->rOrCrAdd16 = 0;
            cscParameters->gOrYAdd16 = 0;
            cscParameters->bOrCbAdd16 = 0;
            cscParameters->rOrCrAdd128 = 0;
            cscParameters->bOrCbAdd128 = 0;
            cscParameters->cscCoeff[1][0] = 0;
            cscParameters->cscCoeff[1][1] = 0;
            cscParameters->cscCoeff[1][2] = 0;
            cscParameters->cscCoeff[0][0] = 0;
            cscParameters->cscCoeff[0][1] = 0;
            cscParameters->cscCoeff[0][2] = 0;
            cscParameters->cscCoeff[2][0] = 0;
            cscParameters->cscCoeff[2][1] = 0;
            cscParameters->cscCoeff[2][2] = 0;
            cscParameters->bt2020En = 0;
            cscParameters->bt2020_cr0_11 = 0;
            cscParameters->bt2020_cr1_11 = 0;
            cscParameters->bt2020_cb0_33 = 0;
            cscParameters->bt2020_cb1_33 = 0;
            break;

        case E_KBKR:
            cscParameters->cscEnable = 1;
            cscParameters->ditherEnable = 1;
            cscParameters->rOrCrSub16 = 0;
            cscParameters->gOrYSub16 = 0;
            cscParameters->bOrCbSub16 = 0;
            cscParameters->rOrCrRangeSub128 = 0;
            cscParameters->gOrYRangeSub128 = 0;
            cscParameters->bOrCbRangeSub128 = 0;
            cscParameters->rOrCrAdd16 = 0;
            cscParameters->gOrYAdd16 = 0;
            cscParameters->bOrCbAdd16 = 0;
            cscParameters->rOrCrAdd128 = 0;
            cscParameters->bOrCbAdd128 = 0;
            cscParameters->bt2020En = 0;
            cscParameters->bt2020_cr0_11 = 0;
            cscParameters->bt2020_cr1_11 = 0;
            cscParameters->bt2020_cb0_33 = 0;
            cscParameters->bt2020_cb1_33 = 0;

            // calculate CSC matrix parameters ; 50000 = 1
            if(inputColorSpace != outputColorSpace)
            {
                // calculate R2Y matrix
                if (colorType < 5)  //709  -- FCC -- 601 -- SMPTE -- 2020NCL
                {
                    cscParameters->cscCoeff[0][0] =  25000;
                    cscParameters->cscCoeff[0][1] = -25000 * (kg) / (50000 - kr);
                    cscParameters->cscCoeff[0][2] = -25000 * (kb) / (50000 - kr);
                    cscParameters->cscCoeff[1][0] =  kr;
                    cscParameters->cscCoeff[1][1] =  kg;
                    cscParameters->cscCoeff[1][2] =  kb;
                    cscParameters->cscCoeff[2][0] = -25000 * (kr) / (50000 - kb);
                    cscParameters->cscCoeff[2][1] = -25000 * (kg) / (50000 - kb);
                    cscParameters->cscCoeff[2][2] =  25000 ;
                }
                else if (colorType == 5)  // YCgCo
                {
                    cscParameters->cscCoeff[0][0] =  25000 ;
                    cscParameters->cscCoeff[0][1] =  0;
                    cscParameters->cscCoeff[0][2] = -25000 ;
                    cscParameters->cscCoeff[1][0] =  12500 ;
                    cscParameters->cscCoeff[1][1] =  25000 ;
                    cscParameters->cscCoeff[1][2] =  12500 ;
                    cscParameters->cscCoeff[2][0] = -12500 ;
                    cscParameters->cscCoeff[2][1] =  25000 ;
                    cscParameters->cscCoeff[2][2] = -12500 ;
                }
                else
                {
                    cscParameters->cscCoeff[0][0] = 50000;
                    cscParameters->cscCoeff[0][1] = 0;
                    cscParameters->cscCoeff[0][2] = 0;
                    cscParameters->cscCoeff[1][0] = 0;
                    cscParameters->cscCoeff[1][1] = 50000;
                    cscParameters->cscCoeff[1][2] = 0;
                    cscParameters->cscCoeff[2][0] = 0;
                    cscParameters->cscCoeff[2][1] = 0;
                    cscParameters->cscCoeff[2][2] = 50000;
                }

                // Y2R = inv(R2Y)
                if(inputColorSpace == Ycbcr)
                {
                    matrixInvGen(cscParameters->cscCoeff,cscParameters->cscCoeff);
                }


                if (colorType == 6 && inputColorSpace == Ycbcr) // BT.2020 CL only support Y2R
                {

                    cscParameters->cscCoeff[0][0] = 0;
                    cscParameters->cscCoeff[0][1] = 50000;
                    cscParameters->cscCoeff[0][2] = 0;
                    cscParameters->cscCoeff[1][0] = 0;
                    cscParameters->cscCoeff[1][1] = 50000;
                    cscParameters->cscCoeff[1][2] = 0;
                    cscParameters->cscCoeff[2][0] = 0;
                    cscParameters->cscCoeff[2][1] = 50000;
                    cscParameters->cscCoeff[2][2] = 0;

                    cscParameters->bt2020En = 1;
                    cscParameters->bt2020_cr0_11 = 85910; // Cr =< 0
                    cscParameters->bt2020_cr1_11 = 49690; // Cr > 0
                    cscParameters->bt2020_cb0_33 = 97020; // Cb =< 0
                    cscParameters->bt2020_cb1_33 = 79100; // Cb > 0
                }

            }
            else // uniform matrix
            {
                cscParameters->cscCoeff[0][0] = 50000;
                cscParameters->cscCoeff[0][1] = 0;
                cscParameters->cscCoeff[0][2] = 0;
                cscParameters->cscCoeff[1][0] = 0;
                cscParameters->cscCoeff[1][1] = 50000;
                cscParameters->cscCoeff[1][2] = 0;
                cscParameters->cscCoeff[2][0] = 0;
                cscParameters->cscCoeff[2][1] = 0;
                cscParameters->cscCoeff[2][2] = 50000;
            }

            // change matrix accroding to input/output data range
            {

                if (inputRange == fullRange)
                {
                    inRange_gOrY = 1023;//255;
                    inRange_rbOrCrCb = 1023;//255;
                }
                else//inputRange == limitedRange
                {
                    inRange_gOrY = 876;
                    inRange_rbOrCrCb = (inputColorSpace == Ycbcr) ? 896 : 876;
                }

                if (outputRange == fullRange)
                {
                    outRange_gOrY = 1023;
                    outRange_rbOrCrCb = 1023;
                }
                else//outputRange == limitedRange
                {
                    outRange_gOrY = 876;
                    outRange_rbOrCrCb = (outputColorSpace == Ycbcr) ? 896 : 876;
                }

                if ( inputColorSpace == Ycbcr)
                {
                    cscParameters->cscCoeff[0][0] = cscParameters->cscCoeff[0][0] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[0][1] = cscParameters->cscCoeff[0][1] * outRange_gOrY / inRange_gOrY;
                    cscParameters->cscCoeff[0][2] = cscParameters->cscCoeff[0][2] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[1][0] = cscParameters->cscCoeff[1][0] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[1][1] = cscParameters->cscCoeff[1][1] * outRange_gOrY / inRange_gOrY;
                    cscParameters->cscCoeff[1][2] = cscParameters->cscCoeff[1][2] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[2][0] = cscParameters->cscCoeff[2][0] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[2][1] = cscParameters->cscCoeff[2][1] * outRange_gOrY / inRange_gOrY;
                    cscParameters->cscCoeff[2][2] = cscParameters->cscCoeff[2][2] * outRange_rbOrCrCb / inRange_rbOrCrCb;

                    cscParameters->bt2020_cr0_11 = cscParameters->bt2020_cr0_11* outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->bt2020_cr1_11 = cscParameters->bt2020_cr1_11* outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->bt2020_cb0_33 = cscParameters->bt2020_cb0_33* outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->bt2020_cb1_33 = cscParameters->bt2020_cb1_33* outRange_rbOrCrCb / inRange_rbOrCrCb;
                }
                else
                {
                    cscParameters->cscCoeff[0][0] = cscParameters->cscCoeff[0][0] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[0][1] = cscParameters->cscCoeff[0][1] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[0][2] = cscParameters->cscCoeff[0][2] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[1][0] = cscParameters->cscCoeff[1][0] * outRange_gOrY / inRange_gOrY;
                    cscParameters->cscCoeff[1][1] = cscParameters->cscCoeff[1][1] * outRange_gOrY / inRange_gOrY;
                    cscParameters->cscCoeff[1][2] = cscParameters->cscCoeff[1][2] * outRange_gOrY / inRange_gOrY;
                    cscParameters->cscCoeff[2][0] = cscParameters->cscCoeff[2][0] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[2][1] = cscParameters->cscCoeff[2][1] * outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->cscCoeff[2][2] = cscParameters->cscCoeff[2][2] * outRange_rbOrCrCb / inRange_rbOrCrCb;

                    cscParameters->bt2020_cr0_11 = cscParameters->bt2020_cr0_11* outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->bt2020_cr1_11 = cscParameters->bt2020_cr1_11* outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->bt2020_cb0_33 = cscParameters->bt2020_cb0_33* outRange_rbOrCrCb / inRange_rbOrCrCb;
                    cscParameters->bt2020_cb1_33 = cscParameters->bt2020_cb1_33* outRange_rbOrCrCb / inRange_rbOrCrCb;
                }

                // change matrix value according to HDR request changing data range
                {
                    cscParameters->cscCoeff[0][0] = ((cscParameters->cscCoeff[0][0] * ratio) >> 6);
                    cscParameters->cscCoeff[0][1] = ((cscParameters->cscCoeff[0][1] * ratio) >> 6);
                    cscParameters->cscCoeff[0][2] = ((cscParameters->cscCoeff[0][2] * ratio) >> 6);
                    cscParameters->cscCoeff[1][0] = ((cscParameters->cscCoeff[1][0] * ratio) >> 6);
                    cscParameters->cscCoeff[1][1] = ((cscParameters->cscCoeff[1][1] * ratio) >> 6);
                    cscParameters->cscCoeff[1][2] = ((cscParameters->cscCoeff[1][2] * ratio) >> 6);
                    cscParameters->cscCoeff[2][0] = ((cscParameters->cscCoeff[2][0] * ratio) >> 6);
                    cscParameters->cscCoeff[2][1] = ((cscParameters->cscCoeff[2][1] * ratio) >> 6);
                    cscParameters->cscCoeff[2][2] = ((cscParameters->cscCoeff[2][2] * ratio) >> 6);

                    cscParameters->bt2020_cr0_11 = ((cscParameters->bt2020_cr0_11* ratio) >> 6);
                    cscParameters->bt2020_cr1_11 = ((cscParameters->bt2020_cr1_11* ratio) >> 6);
                    cscParameters->bt2020_cb0_33 = ((cscParameters->bt2020_cb0_33* ratio) >> 6);
                    cscParameters->bt2020_cb1_33 = ((cscParameters->bt2020_cb1_33* ratio) >> 6);

                }
            }

            // decide -16/-128 before 3x3
            {
                if (inputColorSpace == Rgb)
                {
                    if(inputRange == fullRange)
                    {
                        cscParameters->rOrCrSub16 = 0;
                        cscParameters->gOrYSub16 = 0;
                        cscParameters->bOrCbSub16 = 0;
                    }
                    else // inputRange == limitedRange
                    {
                        //pre R/G/B-16
                        cscParameters->rOrCrSub16 = 1;
                        cscParameters->gOrYSub16 = 1;
                        cscParameters->bOrCbSub16 = 1;
                    }
                }
                else if(inputColorSpace == Ycbcr)
                {
                    if (inputRange == fullRange)
                    {
                        cscParameters->gOrYSub16 = 0;
                        cscParameters->rOrCrRangeSub128 = 1;
                        cscParameters->bOrCbRangeSub128 = 1;
                    }
                    else // inputRange == limitedRange
                    {
                        //pre Y-16/CbCr-128
                        cscParameters->gOrYSub16 = 1;
                        cscParameters->rOrCrRangeSub128 = 1;
                        cscParameters->bOrCbRangeSub128 = 1;
                    }
                }
            }

            // decide +16 +128 after 3x3
            {
                if (outputColorSpace == Rgb)
                {
                    if (outputRange == fullRange)
                    {
                        cscParameters->rOrCrAdd16 = 0;
                        cscParameters->gOrYAdd16 = 0;
                        cscParameters->bOrCbAdd16 = 0;
                    }
                    else //outputRange == limitedRange
                    {
                        //post R/G/B+16
                        cscParameters->rOrCrAdd16 = 1;
                        cscParameters->gOrYAdd16 = 1;
                        cscParameters->bOrCbAdd16 = 1;
                    }
                }
                else if (outputColorSpace == Ycbcr)
                {
                    if (outputRange == fullRange)
                    {
                        //post CrCb+128
                        cscParameters->gOrYAdd16 = 0;
                        cscParameters->rOrCrAdd128 = 1;
                        cscParameters->bOrCbAdd128 = 1;
                    }
                    else //outputRange == limitedRange
                    {
                        //post Y+16/CrCb+128
                        cscParameters->gOrYAdd16 = 1;
                        cscParameters->rOrCrAdd128 = 1;
                        cscParameters->bOrCbAdd128 = 1;
                    }
                }
            }

            break;

    }
    return 0;
}
void cscPrint2ScreenGolden(cscParametersStructure* cscParameters,bool inputColorSpace, bool outputColorSpace, int colorType, bool inputRange, bool outputRange,MS_U8 cscMode, MS_U8 debugMatrixMode)
{
#ifdef  CMODEL
    //debugMatrixMode 0->off
    //1->CrYCb style calculated values
    //2->CrYCb style final register values
    //3->YCbCr style calculated values
    //4->YCbCr style final register values
    MS_U8 i,j;
    if(0 == debugMatrixMode)
    {
        return;
    }
    else if(E_CSCOFF == cscMode)
    {
        printk("CSC is OFF :%s :%d\n\n",__FUNCTION__,__LINE__);

    }
    else
    {
        char iCS[10];
        if (inputColorSpace == Rgb) strcpy(iCS, "RGB");
        else strcpy(iCS, "YCbCr");


        char oCS[10];
        if (outputColorSpace == Rgb) strcpy(oCS, "RGB");
        else strcpy(oCS, "YCbCr");


        char iRange[10];
        if (fullRange == inputRange) strcpy(iRange, "full");
        else strcpy(iRange, "limit");
        char oRange[10];
        if (fullRange == outputRange) strcpy(oRange, "full");
        else strcpy(oRange, "limit");
        char cT[6];//709  -- FCC -- 601 -- SMPTE -- 2020
        switch(colorType)
        {
            case 0:
                strcpy(cT , "709");
                break;
            case 1:
                strcpy(cT , "FCC");
                break;
            case 2:
                strcpy(cT , "601");
                break;
            case 3:
                strcpy(cT , "SMPTE");
                break;
            case 4:
                strcpy(cT , "2020");
                break;
            default:
                break;

        }
        if(inputColorSpace == outputColorSpace)
        {
            strcpy(cT , "all");
        }
        char preY[10];
        if (1 == cscParameters->gOrYSub16) strcpy(preY, "-16");
        else strcpy(preY, " ");
        char preCbCr[10];
        if (1 == cscParameters->rOrCrSub16) strcpy(preCbCr, "-16");
        else if (1 == cscParameters->rOrCrRangeSub128)strcpy(preCbCr, "-128");
        else strcpy(preCbCr, " ");

        char postY[10];
        if (1 == cscParameters->gOrYAdd16) strcpy(postY, "+16");
        else strcpy(postY, " 0 ");

        char postCbCr[10];
        if (1 == cscParameters->rOrCrAdd16) strcpy(postCbCr, "+16");
        else if (1 == cscParameters->rOrCrAdd128)strcpy(postCbCr, "+128");
        else strcpy(postCbCr, " 0 ");

        char input1[5],input2[5],input3[5],output1[5],output2[5],output3[5];

        if(inputColorSpace == Rgb)
        {
            strcpy(input1 , "R");
            strcpy(input2 , "G");
            strcpy(input3 , "B");
        }
        else
        {
            strcpy(input1 , "Cr");
            strcpy(input2 , "Y");
            strcpy(input3 , "Cb");
        }
        if(outputColorSpace == Rgb)
        {
            strcpy(output1 , "R");
            strcpy(output2 , "G");
            strcpy(output3 , "B");
        }
        else
        {
            strcpy(output1 , "Cr");
            strcpy(output2 , "Y");
            strcpy(output3 , "Cb");
        }

        printk("%s_%s2%s_%s_%s\n",iRange,iCS,oCS,oRange,cT);



        double u16Regs[3][3];
        if(1 == debugMatrixMode || 3 == debugMatrixMode) //calculated values
        {

            for( i = 0; i < 3; i++)
                for( j = 0; j < 3; j++)
                {
                    //S16 temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10)+ 25000) / 50000);
                    //u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0x1FFF : (-1*temp)/*& 0x1FFF*/ ;
                    u16Regs[i][j] =(cscParameters->cscCoeff[i][j])/ 50000.0;

                }
        }

        else //final values as read from registers
        {
            for( i = 0; i < 3; i++)
                for( j = 0; j < 3; j++)
                {

                    S16 temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10)+ 25000) / 50000);
                    u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp /*& 0x1FFF*/ : (-1*temp)/*& 0x1FFF*/ ;
                    u16Regs[i][j] /= 1024.0;

                }
        }

        if(1 == debugMatrixMode || 2 == debugMatrixMode) //CrYCb style
        {
            printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output1,postCbCr,u16Regs[0][0],u16Regs[0][1],u16Regs[0][2],input1,preCbCr);
            printk("|%2s | = |%4s| + |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output2,postY   ,u16Regs[1][0],u16Regs[1][1],u16Regs[1][2],input2,preY);
            printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output3,postCbCr,u16Regs[2][0],u16Regs[2][1],u16Regs[2][2],input3,preCbCr);
        }
        else
        {
            if(inputColorSpace == Ycbcr)
            {
                strcpy(input3 , "Cr");
                strcpy(input1 , "Y");
                strcpy(input2 , "Cb");
            }

            if(outputColorSpace == Ycbcr)
            {
                strcpy(output3 , "Cr");
                strcpy(output1 , "Y");
                strcpy(output2 , "Cb");
            }
            if(inputColorSpace == Rgb && outputColorSpace == Ycbcr)
            {
                printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output1,postY   ,u16Regs[1][0],u16Regs[1][1],u16Regs[1][2],input1,preY);
                printk("|%2s | = |%4s| + |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output2,postCbCr,u16Regs[2][0],u16Regs[2][1],u16Regs[2][2],input2,preCbCr);
                printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output3,postCbCr,u16Regs[0][0],u16Regs[0][1],u16Regs[0][2],input3,preCbCr);
            }
            else if (inputColorSpace == outputColorSpace)
            {
                printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output1,postY   ,u16Regs[1][1],u16Regs[1][0],u16Regs[1][2],input1,preY);
                printk("|%2s | = |%4s| + |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output2,postCbCr,u16Regs[2][0],u16Regs[2][2],u16Regs[2][1],input2,preCbCr);
                printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output3,postCbCr,u16Regs[0][2],u16Regs[0][1],u16Regs[0][0],input3,preCbCr);
            }
            else
            {
                printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output1,postY   ,u16Regs[0][1],u16Regs[0][2],u16Regs[0][0],input1,preY);
                printk("|%2s | = |%4s| + |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output2,postCbCr,u16Regs[1][1],u16Regs[1][2],u16Regs[1][0],input2,preCbCr);
                printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output3,postCbCr,u16Regs[2][1],u16Regs[2][2],u16Regs[2][0],input3,preCbCr);
            }

        }
    }
#endif
}
void cscPrintRegisters(cscParametersStructure* cscParameters,bool inputColorSpace, bool outputColorSpace, int colorType, bool inputRange, bool outputRange,MS_BOOL cscMode)
{
    MS_U8 i,j;
    if(E_CSCOFF == cscMode)
    {
        printk("CSC is OFF\n\n");

    }
    else
    {
        char iCS[10];
        if (inputColorSpace == Rgb) strcpy(iCS, "RGB");
        else strcpy(iCS, "YCbCr");


        char oCS[10];
        if (outputColorSpace == Rgb) strcpy(oCS, "RGB");
        else strcpy(oCS, "YCbCr");


        char iRange[10];
        if (fullRange == inputRange) strcpy(iRange, "full");
        else strcpy(iRange, "limit");
        char oRange[10];
        if (fullRange == outputRange) strcpy(oRange, "full");
        else strcpy(oRange, "limit");
        char cT[6];//709  -- FCC -- 601 -- SMPTE -- 2020
        switch(colorType)
        {
            case 0:
                strcpy(cT , "709");
                break;
            case 1:
                strcpy(cT , "FCC");
                break;
            case 2:
                strcpy(cT , "601");
                break;
            case 3:
                strcpy(cT , "SMPTE");
                break;
            case 4:
                strcpy(cT , "2020");
                break;
            default:
                break;

        }
        if(inputColorSpace == outputColorSpace)
        {
            strcpy(cT , "all");
        }

        printk("%s_%s2%s_%s_%s\n",iRange,iCS,oCS,oRange,cT);
    }

    printk("0x%02x\n",(cscParameters->cscEnable << 4));
    printk("0x%02x\n",(cscParameters->ditherEnable << 5));
    printk("0x%02x\n",(cscParameters->rOrCrSub16 << 2));
    printk("0x%02x\n",(cscParameters->gOrYSub16 << 3));
    printk("0x%02x\n",(cscParameters->bOrCbSub16 << 1));
    printk("0x%02x\n",(cscParameters->rOrCrRangeSub128 << 2));
    printk("0x%02x\n",(cscParameters->gOrYRangeSub128 << 1));
    printk("0x%02x\n",(cscParameters->bOrCbRangeSub128<<0) );
    printk("0x%02x\n",(cscParameters->rOrCrAdd16 << 4));
    printk("0x%02x\n",(cscParameters->gOrYAdd16 << 7));
    printk("0x%02x\n",(cscParameters->bOrCbAdd16 << 3));
    printk("0x%02x\n",(cscParameters->rOrCrAdd128 << 5));
    printk("0x%02x\n",(cscParameters->bOrCbAdd128 << 6));

    S16 u16Regs[3][3];
    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
        {
            S16 temp = ((( abs(cscParameters->cscCoeff[i][j]) << 10)+ 25000) / 50000);
            u16Regs[i][j] = (cscParameters->cscCoeff[i][j] >= 0) ? temp & 0x1FFF : (-1*temp)& 0x1FFF ;
            printk("0x%02x\n",(u16Regs[i][j]&0x00FF));
            printk("0x%02x\n",(u16Regs[i][j]>>8));
            //out <<"0x"<< hex<<setw(2)<<std::setfill('0')<< (u16Regs[i][j]&0x00FF) <<endl;
            //out <<"0x"<< hex<<setw(2)<<std::setfill('0')<<(u16Regs[i][j]>>8) <<endl;
        }


    //printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output1,postCbCr,u16Regs[0][0]/1024.0,u16Regs[0][1]/1024.0,u16Regs[0][2]/1024.0,input1,preCbCr);

    //printk("|%2s | = |%4s| + |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output2,postY,u16Regs[1][0]/1024.0,u16Regs[1][1]/1024.0,u16Regs[1][2]/1024.0,input2,preY);

    //printk("|%2s |   |%4s|   |%7.4f    %7.4f    %7.4f| |%2s%s|\n",output3,postCbCr,u16Regs[2][0]/1024.0,u16Regs[2][1]/1024.0,u16Regs[2][2]/1024.0,input3,preCbCr);


}

MS_U8 CSC(MS_U8 mode,MS_U8 matrixCoefficents,MS_U8 ratio,MS_U8 debugMatrixMode,MS_U8 registerDebugEn,MS_U8 IpType)
{

    MS_U8 status = 1;
    MS_U8 cscMode = E_KBKR;
    MS_U8 colorType,inputColorSpace,  outputColorSpace,  inputRange,  outputRange;
    cscParametersStructure cscParameters;

    if(ratio > 0x80)
    {
        ratio = 0x80;
        printk("CSC warning: ratio exceeds the limit, clamped to 0x80");
    }
    parseInput4Csc(&mode,&matrixCoefficents,&inputColorSpace,&outputColorSpace,&inputRange,&outputRange,&colorType,&status,&cscMode);

    cscGenerate3x3(&cscParameters, inputColorSpace,  outputColorSpace,  colorType,  inputRange,  outputRange, ratio,cscMode);
    //CscWrite2Register(&cscParameters,IpType);
    CscWrite2RegisterRecursive( &cscParameters, mode, matrixCoefficents, ratio, debugMatrixMode, registerDebugEn, IpType);
    //;cscWrite2registersAll(&cscParameters,IpType);//work in progress
    if((E_CSC_MANHATTAN_OUTPUT == IpType) ||(E_CSC_MASERATI_OUTPUT_SUB == IpType))
    {
        u8CscIpType = IpType;
        StoreY2Rmatrix(0,&cscParameters);
    }
    cscPrint2ScreenGolden(&cscParameters, inputColorSpace,  outputColorSpace,  colorType,  inputRange,  outputRange,cscMode,debugMatrixMode);
    if(1 == registerDebugEn)
    {
        cscPrintRegisters(&cscParameters, inputColorSpace,  outputColorSpace,  colorType,  inputRange,  outputRange,cscMode);
    }
    return status;
}

void parseInput4DeGamma(MS_U8 transferCharacterstics, ST_TRANSFER_FUNCTION_DATA* stDeGammaData,MS_U8 inputFormatFlag)
{
    stDeGammaData->sValid = TRUE;

    //stDeGammaData->tData[0] = 0x8CCD>>2;;//g_HDRinitParameters.DLC_HDRToneMappingData.u16Tmax << 6;

    switch (transferCharacterstics)
    {

        // u32Gamma          : 4.16
        // u32GammaInv       : 4.16
        // u16Alpha          : 1.15
        // u16Beta           : 0.16
        // u16LowerBoundGain : 6.10
        case 1: // 709
        case 6: // 601
        case 14: // 2020 10b
        case 15:
            // 2020 12b
            // u16Gamma : 8.24
            // u16GInv  : 1.12
            // u16A : 1.15
            // u16B : 0.16
            // u16S : 6.10
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0x8E39>>2; // 1/0.45 = 2.222
            stDeGammaData->sData[1] = 0x49C; // 0.018  u32GammaInv
            stDeGammaData->u32Gamma = 0x7333;
            stDeGammaData->u32GammaInv = 0x238E4;
            stDeGammaData->u16Beta = 0x49C; // 0.018;
            stDeGammaData->u16Alpha = 36012;
            stDeGammaData->u16LowerBoundGain  = 0x1200;;
            stDeGammaData->bExtendEn = 0;
            break;
        case 11: // xvYCC
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0x8E39>>2; // 1/0.45 = 2.222
            stDeGammaData->sData[1] = 0x49C; // 0.018
            stDeGammaData->u32Gamma = 0x7333;
            stDeGammaData->u32GammaInv = 0x238E4;
            stDeGammaData->u16Beta = 0x49C; // 0.018;
            stDeGammaData->u16Alpha = 36012;
            stDeGammaData->u16LowerBoundGain  = 0x1200;;
            stDeGammaData->bExtendEn = 1;
            break;
        case 4: // gamma 2.2
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0x8CCD>>2; // 2.2
            stDeGammaData->sData[1] = 0;
            stDeGammaData->u32Gamma = 0x745D;
            stDeGammaData->u32GammaInv = 0x23333;
            stDeGammaData->u16Beta = 0;
            stDeGammaData->u16Alpha = 0x8000;
            stDeGammaData->u16LowerBoundGain  = 0x0100;;
            stDeGammaData->bExtendEn = 0;
            break;
        case 5: // gamma 2.8
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0xB333>>2; // 2.8
            stDeGammaData->sData[1] = 0; // 0
            stDeGammaData->u32Gamma = 0x5B6E;
            stDeGammaData->u32GammaInv = 0x2CCCC;
            stDeGammaData->u16Beta = 0;
            stDeGammaData->u16Alpha = 0x8000;
            stDeGammaData->u16LowerBoundGain  = 0x0100;;
            stDeGammaData->bExtendEn = 0;
            break;
        case 7: // 240M
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0xB333>>2; // 1/0.45 = 2.222
            stDeGammaData->sData[1] = 0x5D6; // 0.0228
            stDeGammaData->u32Gamma = 0x7333;
            stDeGammaData->u32GammaInv = 0x238E4;
            stDeGammaData->u16Beta = 0x5D6;
            stDeGammaData->u16Alpha = 36422;
            stDeGammaData->u16LowerBoundGain  = 0x1000;
            stDeGammaData->bExtendEn = 0;
            break;
        case 8: // Linear
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0x4000>>2; // 1
            stDeGammaData->sData[1] = 0; // 0
            stDeGammaData->u32Gamma = 0x10000;
            stDeGammaData->u32GammaInv = 0x10000;
            stDeGammaData->u16Beta = 0xFFFF;
            stDeGammaData->u16Alpha = 0x8000;
            stDeGammaData->u16LowerBoundGain  = 0x0400;
            stDeGammaData->bExtendEn = 0;
            break;
        case 9: // Log (100:1)
            stDeGammaData->sMode = 3;
            stDeGammaData->sData[0] = 0x2000; // V=1+log10(L)/2
            stDeGammaData->sData[1] = 0x28F; // 0 if L<0.01
            break;
        case 10: // Log (100*sqrt(10) : 1)
            stDeGammaData->sMode = 3;
            stDeGammaData->sData[0] = 0x199A; // V=1+log10(L)/2.5
            stDeGammaData->sData[1] = 0xCF; // 0 if L<sqrt(10)/1000
            break;
        case 12: // 1361 --> actually not supported for negative values
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0x8E39>>2; // 1/0.45 = 2.222
            stDeGammaData->sData[1] = 0x49C; // 0.018
            stDeGammaData->sValid = FALSE;
            printk("Degamma ERROR! Degamma mode (1361) not supported for negative values, Degamma table is not updated.\n");
            break;
        case 13: // sRGB
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0x999A>>2; // 2.4
            stDeGammaData->sData[1] = 0xCD; // 0.0031308  MS_U8 inputFormatFlag
            stDeGammaData->u32Gamma = 0x6AAB;
            stDeGammaData->u32GammaInv = 0x26666;
            stDeGammaData->u16Beta = 0xCD;
            stDeGammaData->u16Alpha = 0x870A;
            stDeGammaData->u16LowerBoundGain  = 0x33AE;
            stDeGammaData->bExtendEn = (E_CFD_CFIO_SRGB == inputFormatFlag) ? 0 : 1;
            break;
        case 16: // 2084
            stDeGammaData->sMode = 2;
            stDeGammaData->sData[0] = 0; // 0
            stDeGammaData->sData[1] = 0; // 0
            break;
        case 17: // 428-1
            stDeGammaData->sMode = 1;
            stDeGammaData->sData[0] = 0x4000>>2; // 1
            stDeGammaData->sData[1] = 0; // 0
            stDeGammaData->u32Gamma = 0x6276;
            stDeGammaData->u32GammaInv = 0x2999D;
            stDeGammaData->u16Beta = 0x0;
            stDeGammaData->u16Alpha = 0x7552;
            stDeGammaData->u16LowerBoundGain  =0x400;//;
            stDeGammaData->bExtendEn = 0;
            break;
        case 18: // adobeRGB
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0x2330; // 2.19921975
            stDeGammaData->sData[1] = 0; // 0
            stDeGammaData->u32Gamma = 0x7468;
            stDeGammaData->u32GammaInv = 0x23300;
            stDeGammaData->u16Beta = 0x0;
            stDeGammaData->u16Alpha = 0x8000;
            stDeGammaData->u16LowerBoundGain  = 0x400;
            stDeGammaData->bExtendEn = 0;
            break;
        case 19: // Bt1886
            stDeGammaData->sMode = 4;
            stDeGammaData->sData[0] = 0x2333; // 2.2
            //stDeGammaData->sData[1] = 0; // 0
            break;
        case 0: // Reserved
        case 2: // Unspecified
        case 3: // Reserved
        default:
            stDeGammaData->sValid = FALSE;
            stDeGammaData->sMode = 0;
            stDeGammaData->sData[0] = 0x8E39>>2; // 1/0.45 = 2.222
            stDeGammaData->sData[1] = 0x49C; // 0.018
            printk("Degamma ERROR! Degamma mode unspecific or out of bound, Degamma table is not updated.\n");
            //break;
            break;
    }

}

U32 newPow125(MS_U32 data_base, MS_U32 data_exp)
{
// returns data_base^data_exp
// data_base : 2.16 (0x10000->1.0)
// data_exp : 4.12  (0x1000->1.0)
// data_out : 3.16  (0x10000->1.0)
    MS_U32 idx, lsb;
    U32  log_res, xlog_res, final_res;
    MS_U32 data_base_temp;
#ifdef ONEIS0X100
    data_base_temp = data_base - (256<<8);
    idx = (data_base_temp >> 8);
    lsb = data_base_temp & 0xFF;
    log_res = ((logLut[idx] * (0x100-lsb)) >> 8) + ((logLut[idx+1] * (lsb)) >> 8);
    printf("log of %f = %f\n" ,data_base/65536.0,log_res/8192.0);
#endif
//data_base+=1;
    data_base_temp = data_base - 0x10000 ;
    idx = (data_base_temp >> 6);
    lsb = data_base_temp & 0x3F;
    log_res = ((logLut[idx] * (0x40-lsb)) >> 6) + ((logLut[idx+1] * (lsb)) >> 6);
    printf("log of %f = %f\n" ,data_base/65536.0,log_res/(8192.0*8));

// x*log(base) : 4.16, x : 4.12, log(base) : 1.13
    xlog_res = (MS_U64)((MS_U64)data_exp * (MS_U64)log_res) >> 13;
//xlog_res = (8 * log_res);
    printf("log X %f = %f\n" ,data_exp/65536.0,xlog_res/(65536.0*8));
//xlog_res = (log_res<0) ? -1*xlog_res : xlog_res;
#ifdef use2048
    xlog_res+= 0xDB7FF; // +12
    idx = xlog_res >> 9;
    lsb = xlog_res & 0x1FF;
    final_res = ((expLut[idx] * (0x1FF-lsb)) >> 9) + ((expLut[idx+1] * (lsb)) >> 9);
#endif
//xlog_res = xlog_res;// + 0xDB6DB; // +(12/14)*2^16
    idx = xlog_res >> 7;//4;
    lsb = xlog_res & 0x07F;//
//final_res = ( (expLut[idx] * (0x80 - lsb))>> 7) + ((expLut[idx+1] * (lsb)) >> 7);
    idx = xlog_res >> 8;//4;
    lsb = xlog_res & 0x0FF;//
    final_res = ( (expLut0[idx] * (0x100 - lsb))>> 8) + ((expLut0[idx+1] * (lsb)) >> 8);
//final_res = expLut[idx];
//printf("idx = %d\n",idx);
//printf("lsb = 0x%x\n",lsb);
//printf("%f ^ %f = %f\n",data_base/65536.0,data_exp/65536.0,final_res/65536.0);
    return final_res;
}
U32 MDrv_HDR_Pow_ali(MS_U32 data_base, MS_U16 data_exp)
{
    // returns data_base^data_exp
    // data_base : 0.16 (0xFFFF->1.0)
    // data_exp : 4.12  (0x1000->1.0)
    // data_out : 0.16  (0xFFFF->1.0)
    MS_U16 idx, lsb, shift;
    U32 data_tmp, log_res, xlog_res, final_res;

    // input 0.16, output 4.16
    // data  4.16, [(0:63); (32:63)*2; (32:63)*2^2; (32:63)*2^3; (32:63)*2^4; (32:63)*2^5; (32:63)*2^6; (32:63)*2^7; (32:63)*2^8; (32:63)*2^9; (32:64)*2^10]
    static const U32 log_lut[385] =
    {
        0xC0000,   0xB1721,   0xA65AF,   0x9FDE3,   0x9B43D,   0x97B1D,   0x94C71,   0x924FA,   0x902CB,   0x8E4A4,   0x8C9AB,   0x8B145,   0x89AFF,   0x88681,   0x87388,   0x861DF,
        0x85159,   0x841D4,   0x83332,   0x8255B,   0x81839,   0x80BBC,   0x7FFD3,   0x7F472,   0x7E98D,   0x7DF19,   0x7D50F,   0x7CB66,   0x7C216,   0x7B91A,   0x7B06D,   0x7A808,
        0x79FE7,   0x79806,   0x79062,   0x788F6,   0x781C0,   0x77ABC,   0x773E9,   0x76D42,   0x766C7,   0x76075,   0x75A4A,   0x75443,   0x74E61,   0x748A0,   0x74300,   0x73D7E,
        0x7381A,   0x732D3,   0x72DA7,   0x72895,   0x7239D,   0x71EBC,   0x719F3,   0x71541,   0x710A4,   0x70C1C,   0x707A8,   0x70348,   0x6FEFB,   0x6FABF,   0x6F696,   0x6F27D,
        0x6EE75,   0x6E694,   0x6DEF0,   0x6D784,   0x6D04E,   0x6C94A,   0x6C277,   0x6BBD0,   0x6B555,   0x6AF03,   0x6A8D7,   0x6A2D1,   0x69CEF,   0x6972E,   0x6918E,   0x68C0C,
        0x686A8,   0x68161,   0x67C35,   0x67723,   0x6722B,   0x66D4A,   0x66881,   0x663CF,   0x65F32,   0x65AAA,   0x65636,   0x651D6,   0x64D88,   0x6494D,   0x64524,   0x6410B,
        0x63D03,   0x63522,   0x62D7E,   0x62612,   0x61EDC,   0x617D8,   0x61104,   0x60A5E,   0x603E3,   0x5FD91,   0x5F765,   0x5F15F,   0x5EB7D,   0x5E5BC,   0x5E01B,   0x5DA9A,
        0x5D536,   0x5CFEF,   0x5CAC3,   0x5C5B1,   0x5C0B9,   0x5BBD8,   0x5B70F,   0x5B25D,   0x5ADC0,   0x5A938,   0x5A4C4,   0x5A064,   0x59C16,   0x597DB,   0x593B1,   0x58F99,
        0x58B91,   0x583B0,   0x57C0C,   0x574A0,   0x56D6A,   0x56666,   0x55F92,   0x558EC,   0x55271,   0x54C1F,   0x545F3,   0x53FED,   0x53A0B,   0x5344A,   0x52EA9,   0x52928,
        0x523C4,   0x51E7D,   0x51951,   0x5143F,   0x50F47,   0x50A66,   0x5059D,   0x500EB,   0x4FC4E,   0x4F7C6,   0x4F352,   0x4EEF2,   0x4EAA4,   0x4E669,   0x4E23F,   0x4DE27,
        0x4DA1F,   0x4D23E,   0x4CA9A,   0x4C32E,   0x4BBF8,   0x4B4F4,   0x4AE20,   0x4A77A,   0x4A0FF,   0x49AAC,   0x49481,   0x48E7B,   0x48898,   0x482D8,   0x47D37,   0x477B6,
        0x47252,   0x46D0B,   0x467DF,   0x462CD,   0x45DD4,   0x458F4,   0x4542B,   0x44F79,   0x44ADC,   0x44654,   0x441E0,   0x43D80,   0x43932,   0x434F7,   0x430CD,   0x42CB5,
        0x428AD,   0x420CC,   0x41927,   0x411BC,   0x40A86,   0x40382,   0x3FCAE,   0x3F608,   0x3EF8D,   0x3E93A,   0x3E30F,   0x3DD09,   0x3D726,   0x3D166,   0x3CBC5,   0x3C644,
        0x3C0E0,   0x3BB99,   0x3B66D,   0x3B15B,   0x3AC62,   0x3A782,   0x3A2B9,   0x39E06,   0x3996A,   0x394E2,   0x3906E,   0x38C0E,   0x387C0,   0x38385,   0x37F5B,   0x37B43,
        0x3773A,   0x36F5A,   0x367B5,   0x3604A,   0x35913,   0x35210,   0x34B3C,   0x34496,   0x33E1B,   0x337C8,   0x3319D,   0x32B97,   0x325B4,   0x31FF4,   0x31A53,   0x314D2,
        0x30F6E,   0x30A27,   0x304FB,   0x2FFE9,   0x2FAF0,   0x2F610,   0x2F147,   0x2EC94,   0x2E7F7,   0x2E370,   0x2DEFC,   0x2DA9B,   0x2D64E,   0x2D213,   0x2CDE9,   0x2C9D0,
        0x2C5C8,   0x2BDE8,   0x2B643,   0x2AED8,   0x2A7A1,   0x2A09E,   0x299CA,   0x29324,   0x28CA8,   0x28656,   0x2802B,   0x27A25,   0x27442,   0x26E81,   0x268E1,   0x26360,
        0x25DFC,   0x258B5,   0x25389,   0x24E77,   0x2497E,   0x2449E,   0x23FD5,   0x23B22,   0x23685,   0x231FD,   0x22D8A,   0x22929,   0x224DC,   0x220A1,   0x21C77,   0x2185E,
        0x21456,   0x20C76,   0x204D1,   0x1FD65,   0x1F62F,   0x1EF2C,   0x1E858,   0x1E1B2,   0x1DB36,   0x1D4E4,   0x1CEB9,   0x1C8B3,   0x1C2D0,   0x1BD0F,   0x1B76F,   0x1B1ED,
        0x1AC8A,   0x1A742,   0x1A216,   0x19D05,   0x1980C,   0x1932C,   0x18E63,   0x189B0,   0x18513,   0x1808B,   0x17C18,   0x177B7,   0x1736A,   0x16F2F,   0x16B05,   0x166EC,
        0x162E4,   0x15B04,   0x1535F,   0x14BF3,   0x144BD,   0x13DBA,   0x136E6,   0x1303F,   0x129C4,   0x12372,   0x11D47,   0x11741,   0x1115E,   0x10B9D,   0x105FD,   0x1007B,
        0xFB18,    0xF5D0,    0xF0A4,    0xEB93,    0xE69A,    0xE1BA,    0xDCF1,    0xD83E,    0xD3A1,    0xCF19,    0xCAA5,    0xC645,    0xC1F8,    0xBDBC,    0xB993,    0xB57A,
        0xB172,    0xA991,    0xA1ED,    0x9A81,    0x934B,    0x8C47,    0x8574,    0x7ECD,    0x7852,    0x7200,    0x6BD5,    0x65CF,    0x5FEC,    0x5A2B,    0x548B,    0x4F09,
        0x49A6,    0x445E,    0x3F32,    0x3A20,    0x3528,    0x3048,    0x2B7F,    0x26CC,    0x222F,    0x1DA7,    0x1933,    0x14D3,    0x1086,     0xC4A,     0x821,     0x408,
        0x0
    };
    // input 4.16, output 0.16
    // data  0.16, 2^4*[(0:63); (32:63)*2; (32:63)*2^2; (32:63)*2^3; (32:63)*2^4; (32:63)*2^5; (32:63)*2^6; (32:63)*2^7; (32:63)*2^8; (32:63)*2^9; (32:64)*2^10]
    static const MS_U16 exp_lut[385] =
    {
        0xFFFF,    0xFFEF,    0xFFDF,    0xFFCF,    0xFFBF,    0xFFAF,    0xFF9F,    0xFF8F,    0xFF7F,    0xFF6F,    0xFF5F,    0xFF4F,    0xFF3F,    0xFF2F,    0xFF1F,    0xFF0F,
        0xFEFF,    0xFEF0,    0xFEE0,    0xFED0,    0xFEC0,    0xFEB0,    0xFEA0,    0xFE90,    0xFE80,    0xFE70,    0xFE60,    0xFE50,    0xFE41,    0xFE31,    0xFE21,    0xFE11,
        0xFE01,    0xFDF1,    0xFDE1,    0xFDD1,    0xFDC2,    0xFDB2,    0xFDA2,    0xFD92,    0xFD82,    0xFD72,    0xFD62,    0xFD53,    0xFD43,    0xFD33,    0xFD23,    0xFD13,
        0xFD03,    0xFCF4,    0xFCE4,    0xFCD4,    0xFCC4,    0xFCB4,    0xFCA5,    0xFC95,    0xFC85,    0xFC75,    0xFC66,    0xFC56,    0xFC46,    0xFC36,    0xFC26,    0xFC17,
        0xFC07,    0xFBE7,    0xFBC8,    0xFBA9,    0xFB89,    0xFB6A,    0xFB4A,    0xFB2B,    0xFB0B,    0xFAEC,    0xFACD,    0xFAAD,    0xFA8E,    0xFA6F,    0xFA4F,    0xFA30,
        0xFA11,    0xF9F2,    0xF9D2,    0xF9B3,    0xF994,    0xF975,    0xF956,    0xF936,    0xF917,    0xF8F8,    0xF8D9,    0xF8BA,    0xF89B,    0xF87C,    0xF85D,    0xF83E,
        0xF81F,    0xF7E1,    0xF7A3,    0xF765,    0xF727,    0xF6E9,    0xF6AC,    0xF66E,    0xF630,    0xF5F3,    0xF5B5,    0xF578,    0xF53B,    0xF4FD,    0xF4C0,    0xF483,
        0xF446,    0xF409,    0xF3CC,    0xF38F,    0xF352,    0xF315,    0xF2D9,    0xF29C,    0xF25F,    0xF223,    0xF1E6,    0xF1AA,    0xF16D,    0xF131,    0xF0F5,    0xF0B9,
        0xF07C,    0xF004,    0xEF8C,    0xEF15,    0xEE9D,    0xEE26,    0xEDAF,    0xED38,    0xECC2,    0xEC4C,    0xEBD6,    0xEB60,    0xEAEA,    0xEA75,    0xEA00,    0xE98B,
        0xE916,    0xE8A2,    0xE82E,    0xE7BA,    0xE746,    0xE6D2,    0xE65F,    0xE5EC,    0xE579,    0xE506,    0xE494,    0xE422,    0xE3B0,    0xE33E,    0xE2CD,    0xE25B,
        0xE1EA,    0xE109,    0xE028,    0xDF49,    0xDE6A,    0xDD8C,    0xDCAF,    0xDBD2,    0xDAF7,    0xDA1C,    0xD943,    0xD86A,    0xD792,    0xD6BB,    0xD5E4,    0xD50F,
        0xD43A,    0xD366,    0xD293,    0xD1C1,    0xD0F0,    0xD01F,    0xCF50,    0xCE81,    0xCDB3,    0xCCE5,    0xCC19,    0xCB4D,    0xCA82,    0xC9B8,    0xC8EF,    0xC826,
        0xC75F,    0xC5D1,    0xC447,    0xC2C0,    0xC13C,    0xBFBB,    0xBE3D,    0xBCC2,    0xBB4A,    0xB9D5,    0xB863,    0xB6F4,    0xB587,    0xB41D,    0xB2B7,    0xB153,
        0xAFF1,    0xAE93,    0xAD37,    0xABDE,    0xAA87,    0xA934,    0xA7E3,    0xA694,    0xA548,    0xA3FF,    0xA2B8,    0xA174,    0xA032,    0x9EF3,    0x9DB7,    0x9C7C,
        0x9B45,    0x98DC,    0x967E,    0x9428,    0x91DC,    0x8F99,    0x8D60,    0x8B2E,    0x8906,    0x86E6,    0x84CF,    0x82C0,    0x80B9,    0x7EBA,    0x7CC3,    0x7AD4,
        0x78EC,    0x770C,    0x7534,    0x7363,    0x7199,    0x6FD6,    0x6E1A,    0x6C65,    0x6AB7,    0x690F,    0x676E,    0x65D4,    0x6440,    0x62B2,    0x612A,    0x5FA8,
        0x5E2D,    0x5B47,    0x5878,    0x55BF,    0x531C,    0x508D,    0x4E13,    0x4BAC,    0x4958,    0x4716,    0x44E6,    0x42C7,    0x40B9,    0x3EBC,    0x3CCE,    0x3AEF,
        0x391F,    0x375D,    0x35A9,    0x3402,    0x3268,    0x30DB,    0x2F5A,    0x2DE5,    0x2C7C,    0x2B1E,    0x29CA,    0x2881,    0x2742,    0x260D,    0x24E1,    0x23BE,
        0x22A5,    0x208C,    0x1E93,    0x1CB9,    0x1AFB,    0x1959,    0x17CF,    0x165E,    0x1503,    0x13BD,    0x128B,    0x116C,    0x105D,     0xF60,     0xE71,     0xD91,
        0xCBF,     0xBF9,     0xB3F,     0xA91,     0x9ED,     0x953,     0x8C2,     0x83B,     0x7BB,     0x743,     0x6D2,     0x669,     0x605,     0x5A8,     0x550,     0x4FE,
        0x4B0,     0x423,     0x3A7,     0x339,     0x2D8,     0x282,     0x237,     0x1F4,     0x1BA,     0x186,     0x158,     0x12F,     0x10C,      0xEC,      0xD1,      0xB8,
        0xA2,      0x8F,      0x7F,      0x70,      0x63,      0x57,      0x4D,      0x44,      0x3C,      0x35,      0x2F,      0x29,      0x24,      0x20,      0x1C,      0x19,
        0x16,      0x11,       0xD,       0xA,       0x8,       0x6,       0x5,       0x4,       0x3,       0x2,       0x2,       0x1,       0x1,       0x1,       0x1,       0x1,
        0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,       0x0,
        0x0
    }; // continued data is all zero

    if (data_base == 0)
        return 0;
    if (data_exp == 0)
        return 0xFFFF;

    // log(base) : 0.16 -> 4.16
    data_tmp = data_base;
    for (shift = 0; data_tmp > 63; shift++)
        data_tmp >>= 1;
    idx = (data_base >> shift) + 32*shift;
    lsb = data_base & ((0x1 << shift) - 1);
    if (idx == 383) // to fix!
        lsb = lsb + 1;
    log_res = log_lut[idx] - ((log_lut[idx] - log_lut[idx+1]) * lsb >> shift);

    // x*log(base) : 4.16, x : 4.12
    xlog_res = data_exp * (U64)log_res >> 12;

    // exp^(x*log(base)) : 4.16 -> 0.16
    data_tmp = xlog_res;
    for (shift = 0; data_tmp > (63 << 4); shift++)
        data_tmp >>= 1;
    idx = (xlog_res >> (shift+4)) + 32*shift;
    lsb = xlog_res & ((0x1 << (shift+4)) - 1);
    final_res = exp_lut[idx] - ((exp_lut[idx] - exp_lut[idx+1]) * lsb >> (shift+4));

    return final_res;
}
U32 newPow04(MS_U32 data_base, MS_U32 data_exp)
{
    if (0xFFFF >= data_base)
    {
        return MDrv_HDR_Pow_ali( data_base,  data_exp>>4);
    }
    else
    {
        //return newPow14( data_base+3,  data_exp);
        //data_base = (data_base & 0x10000 == 0x10000) ? (data_base+1) : (data_base & 0x20000 == 0x20000) ? (data_base+2) : data_base;
        if(data_base>=0xffff)
            data_base++;
        if(data_base>=0x1ffff)
            data_base++;

        return newPow125( data_base,  data_exp);
    }
}
U32 MDrv_HDR_PQ_ali(MS_U32 u16In)
{

    MS_U32 u16Shift, u16Idx, u16Lsb, u16Tmp, u16Exp;
    U32 u32Lut0_out, u32Lut1_out, u32Result;
    if (u16In == 0)
        return 0;
    if (u16In > 0xFFFF)
        u16In = 0xFFFF;

    u16Tmp = u16In;
    for (u16Shift = 0; u16Tmp > 127; u16Shift++)
        u16Tmp >>= 1;
    u16Idx = (u16In >> u16Shift) + 64*u16Shift;
    u16Lsb = u16In & ((0x1 << u16Shift) - 1);
    u32Lut0_out = u16PQCurveMantLut_ali[u16Idx];
    u32Lut1_out = u16PQCurveMantLut_ali[u16Idx+1];
    u16Exp = u16PQCurveExpLut_ali[u16Idx];
    if (u16Exp != u16PQCurveExpLut_ali[u16Idx+1])
        u32Lut1_out <<= 1;
    if (u16Shift > u16PQCurveExpLut_ali[u16Idx])
        u32Result = (u32Lut0_out << u16Exp) + ((u32Lut1_out - u32Lut0_out) * u16Lsb >> (u16Shift - u16Exp));
    else
        u32Result = (u32Lut0_out << u16Exp) + ((u32Lut1_out - u32Lut0_out) * u16Lsb << (u16Exp - u16Shift));
    //printk("%x -> %x; %x %x %x %x %x %x\n", u16In, u32Result, u16Idx, u16Lsb, u32Lut0_out, u32Lut1_out, u16Shift, u16Exp);
    return u32Result;
}

U32 MDrv_HDR_DeGamma_ali(MS_U16 u16In, MS_U16 u16Gamma, MS_U16 u16Ginv, MS_U16 u16A, MS_U16 u16B, MS_U16 u16S)
{
    // u16Gamma : 4.12
    // u16GInv  : 1.12
    // u16A : 1.15
    // u16B : 0.16
    // u16S : 6.10
    U32 u32Th, u32Tmp, u32Out;

    u32Th = (U32)u16S * u16B >> 10;
    if (u16In <= u32Th)
        return ( ( (U64)u16In << 10) /( u16S) )<< (16);
    //return (( ((U64)u16In << 10) /( u16S) )+ 0x8)>>4;//<< 16;
    //u32Tmp = (U32)(u16In + ((u16A - 0x7FFF/*0x7F80*/) << 1)) * 0x7FFF / u16A;
    u32Tmp = ((U64)(u16In + ((u16A - 0x8000/*0x7FFF*//*0x7FF6*//*0x7FF6*/) << 1)) << 15) / u16A;
    u32Out = MDrv_HDR_Pow_ali(u32Tmp /*+ (u32Tmp>>15)*/, u16Gamma);
    return (u32Out)<< (16);
}
U32 MDrv_HDR_Gamma_ali(MS_U16 u16In, MS_U16 u16Gamma, MS_U16 u16Ginv, MS_U16 u16A, MS_U16 u16B, MS_U16 u16S)
{
    // u16Gamma : 4.12
    // u16GInv  : 1.12
    // u16A : 1.15
    // u16B : 0.16
    // u16S : 6.10
    U64 u32Th, u32Tmp, u32Out;

    u32Th = u16B;//(U32)u16S * u16B >> 10;
    if (u16In <= u32Th)
        return ( ( ((U16)u16In ) * u16S + (1<<9) )>> 10);
    //u32Tmp = (U32)(u16In + ((u16A - 0x7F80) << 1)) * 0x7FFF / u16A;
    //u32Out = MDrv_HDR_Pow(u32Tmp + (u32Tmp>>15), u16Ginv);
    u32Out = ( (((MDrv_HDR_Pow_ali(u16In, u16Ginv))*u16A)+(1<<14))>>15) - ((u16A - 0x7FFF/* 0x7F80*/) << 1);
    return  u32Out;//(( (u32Out+(1<<3))>>4)-0) ;//<< 16;
}

U32 MDrv_HDR_Bt1886Gamma_ali(U16 u16In, U16 u16Gamma, U16 u16Ginv, U16 u16White, U16 u16Black)
{
    // u16Gamma : 4.12
    // u16GInv  : 1.12
    // u16White : 0.16
    // u16Black : 0.16
    U32 u32Tmp, u32A, u32B, u32Out;

    u32Tmp = MDrv_HDR_Pow_ali(u16White, u16Ginv) - MDrv_HDR_Pow_ali(u16Black, u16Ginv);
    u32A = MDrv_HDR_Pow_ali((U16)u32Tmp, u16Gamma);
    u32Tmp = ((U32)u16Black << 16) / u32A;
    u32B = MDrv_HDR_Pow_ali(u32Tmp, u16Ginv);
    u32Tmp = ((U32)u16In << 16) / u32A;
    u32Out = (u32Tmp < 0xFFFF) ? MDrv_HDR_Pow_ali(u32Tmp, u16Ginv) : (MDrv_HDR_Pow_ali(u32Tmp >> 1, u16Ginv) << 16) / MDrv_HDR_Pow_ali(0x8000, u16Ginv);
    u32Out = (u32Out > u32B) ? u32Out - u32B : 0; // 16b
    return u32Out;
}
void deGammaGenerator(U32* u32LutData,ST_TRANSFER_FUNCTION_DATA* pData,MS_U16 max_lum_code,MS_U8 g_u16HdrY2RRatio,MS_U8* status, MS_U8 IpType)
{

    MS_U16 u16Val, u16NumEntries, u16LutIdx, u16Index;
    U64 u64Nume, u64Denum;


    //for pq degamma
    MS_U16 u16CurrentRange,u16Adder,u16Counter;
    U32 u32Val;
    MS_U16 u16ResPQ[16] = {2,2,4,8,16,16,16,32,32,32,32,32,32,64,64,128};
    MS_U16 u16RangePQ[17]  = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};
    //MS_U16 u16Res[16];
    //U32 u32LutData[600];

    // u16A : 0.16
    // u16B : 0.16
    // u16S : 6.10
    // u16Gamma : 4.12
    // u16Ginv  : 4.12
    MS_U16 u16Gamma, u16A, u16B, u16S, u16Ginv;

    if(pData->sValid == FALSE)
    {
        //HDR_DBG_HAL(printk("\n    Kernel      MDrv_HDR_TransferFunctionGen    ERROR   u8ColorPrimaries=%d     \n",g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8ColorPrimaries));
        *status = 0;
        return;
    }

    // DeGamma
    if (pData->sMode == 0)
    {
        u16Gamma = pData->sData[0];
        u16B = pData->sData[1];
        u16Ginv = 0x1000000ul / u16Gamma; // (1<<24) / 4.12 -> 1.12
        if (u16B == 0)
        {
            u16S = 0x8000;//////////////////////////////0;
            u16A = 0x8000; // value = 1.0
        }
        else
        {
            u16S = 0x1200; // value = 4.5
            for (u16Index = 0; u16Index < 8; u16Index++)
            {
                u16A = min(((U64)u16S * u16B) * (u16Gamma - 0x1000) >> 23, 0xFFFF) + 0x8000; // 6.10*0.16*4.12 -> 10.38 -> 1.15
                u16S = min(((U32)u16A << (12-5)) / (u16Gamma * MDrv_HDR_Pow_ali(u16B, 0x1000-u16Ginv) >> 16), 0xFFFF); // 1.15/(4.12*0.16>>16)  = 1.15/4.12
            }
        }
        //printk("A = %0#x\n",u16A);
        //printk("S = %0#x\n",u16S);


        // u16Gamma : 4.12
        // u16GInv  : 1.12
        // u16A : 1.15
        // u16B : 0.16
        // u16S : 6.10
        //printk("u16Gamma is:%f\n",u16Gamma/4096.0);
        //printk("B is:%f\n",u16B/65536.0);
        //printk("S is:%f\n",u16S/1024.0);
        //printk("A is:%f\n",u16A/32768.0);



        if(E_DEGAMMA_MASERATI_MD == IpType)
        {
            u16NumEntries = 512;
            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {
                u16Val = ((U32)u16LutIdx << 7);
                u32LutData[u16LutIdx] = MDrv_HDR_DeGamma_ali(u16Val, u16Gamma, u16Ginv, u16A, u16B, u16S);

            }
            for ( ; u16LutIdx < 600; u16LutIdx++)
                u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];

        }
        else
        {
            u16NumEntries = 256;
            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {
                u16Val = ((U32)u16LutIdx << 8);
                u32LutData[u16LutIdx] = MDrv_HDR_DeGamma_ali(u16Val, u16Gamma, u16Ginv, u16A, u16B, u16S);

            }
            // input > 1 case --> to fix
            for ( ; u16LutIdx < 600; u16LutIdx++)
                u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];
        }

    }
    else if (pData->sMode == 1) // DCI-P3
    {
        *status = 0;
        printk("DeGamma ERROR! DeGamma mode (ST 428-1) is currently unsuported, Degamma table has not been updated.\n");
        // todo
        //HDR_DBG_HAL(printk("\n  Kernel Error!! sMode is 1 \n"));
    }
    else if (pData->sMode == 2) // PQ
    {
        MS_U16 i,j;
        if(0 == max_lum_code)
        {
            printk("Degamma E rror! max_lum_code == 0, division by zero, Degamma will not be updated\n");
            *status = 0;
            return;
        }
        if(0 == g_u16HdrY2RRatio)
        {
            printk("Degamma E rror! g_u16HdrY2RRatio == 0, division by zero, Degamma will not be updated\n");
            *status = 0;
            return;
        }
        u64Denum = MDrv_HDR_PQ_ali(max_lum_code);
        if(E_DEGAMMA_MASERATI_MD == IpType)
        {
            u16NumEntries = 512;

            u16Adder = 0;
            u16Counter = 0;
            for( i = 0; i < 16; i++)
            {

                for( j = 0; j < u16ResPQ[i]; j++)
                {
                    u32Val = u16Adder;
                    u64Nume = MDrv_HDR_PQ_ali(u32Val);
                    u32LutData[u16Counter++] = min(u64Nume /** 0xFFF00000ul / u64Denum*/, 0xFFF00000ul);

                    u16CurrentRange = u16RangePQ[i+1] - u16RangePQ[i];
                    u16Adder += ( u16CurrentRange + (u16ResPQ[i]>>1) ) / u16ResPQ[i];

                }
            }


        }
        else
        {
            u16NumEntries = (256 * g_u16HdrY2RRatio)>>6;
            u64Denum = MDrv_HDR_PQ_ali(max_lum_code/*pData->tData[0]*/);

            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {
                //u16Val = ((U32)u16LutIdx << (8+6) ) / g_u16HdrY2RRatio;
                U32 u32Val = ((U32)u16LutIdx << (8+6) ) / g_u16HdrY2RRatio;
                //U32 u32Val = ((U32)u16LutIdx << (8) );

                u64Nume = MDrv_HDR_PQ_ali(u32Val);

                u32LutData[u16LutIdx] = min(u64Nume * 0xFFF00000ul / u64Denum, 0xFFF00000ul);

                //u32LutData[u16LutIdx] = min(u64Nume * 0xFFFFFul / u64Denum, 0xFFFFFul);

            }
            for ( ; u16LutIdx < 600; u16LutIdx++)
                u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];
        }





    }
    else if (pData->sMode == 4)  // Bt1886
    {
        *status = 0;
        printk("DeGamma E RROR! DeGamma mode (Bt1886) is unsuported, Degamma table has not been updated.\n");
    }
    else// if (mode == 3) // Log
    {
        *status = 0;
        printk("DeGamma E RROR! DeGamma mode (Log) is currently unsuported, Degamma table has not been updated.\n");
        // todo
        //HDR_DBG_HAL(printk("\n  Kernel Error!! sMode is 3 \n"));
    }

}
void GammaGenerator(U32* u32LutData,ST_TRANSFER_FUNCTION_DATA* pData,MS_U8 max_lum_code,MS_U16 u16White, MS_U16 u16Black,MS_U8 *status, MS_U8 IpType)
{

    // u16Gamma : 4.12
    // u16GInv  : 1.12
    // u16A : 1.15
    // u16B : 0.16
    // u16S : 6.10
    MS_U16 u16Val, u16NumEntries, u16LutIdx, u16Index;
    U64 u64Nume, u64Denum;
    //U32 u32LutData[600];

    MS_U16 u16Gamma, u16A, u16B, u16S, u16Ginv;


    if(pData->sMode == 2)
    {
        pData->sMode = 0;
        pData->sData[0] = 0x8E39>>2; // 1/0.45 = 2.222
        pData->sData[1] = 0x49C;

        pData->sValid == FALSE;
        *status = 0;
        return;
        printk("Gamma E rror! Gamma mode (PQ) is currently unsuported, Gamma table has not been updated.\n");
    }

    if(pData->sValid == FALSE)
    {
        //HDR_DBG_HAL(printk("\n    Kernel      MDrv_HDR_TransferFunctionGen    ERROR   u8ColorPrimaries=%d     \n",g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8ColorPrimaries));
        *status = 0;
        return;
    }

    // DeGamma
    if (pData->sMode == 0)
    {
        u16Gamma = pData->sData[0];
        u16B = pData->sData[1];
        u16Ginv = 0x1000000ul / u16Gamma; // (1<<24) / 4.12 -> 1.12
        if (u16B == 0)
        {
            u16S = 0;
            u16A = 0x8000; // value = 1.0
        }
        else
        {
            //u16S = 0x1200; // value = 4.5
            u16S = 0x1180; // value = 4.5

            for (u16Index = 0; u16Index < 16; u16Index++)
            {
                u16A = min(((U64)u16S * u16B) * (u16Gamma - 0x1000)+(1<<22) >> 23, 0xFFFF) + 0x8000; // 6.10*0.16*4.12 -> 10.38 -> 1.15
                u16S = min(((U32)u16A << (12-5)) / (u16Gamma * MDrv_HDR_Pow_ali(u16B, 0x1000-u16Ginv)+(1<<15) >> 16), 0xFFFF); // 1.15/(4.12*0.16>>16)  = 1.15/4.12
            }
        }


        // u16A : 0.16
        // u16B : 0.16
        // u16S : 6.10
        // u16Gamma : 4.12
        // u16Ginv  : 4.12



        //u16A =36012;
        //fprintf(fi,"A is %d :\n",u16A);
        //u16B = 1180;
        // u16S = 4608;
        // fprintf(fi,"S is %d :\n",u16S);
        // u16Gamma =  9102;
        // u16Ginv  =1843;

        if (E_GAMMA_MANHATTAN_VOP2 == IpType)
        {
            u16NumEntries = 256;
            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {
                //u16Val = ((U32)u16LutIdx << 8);
                u16Val = (((U32)u16LutIdx * 1023)+2)>>2;
                u32LutData[u16LutIdx] = MDrv_HDR_Gamma_ali(u16Val, u16Gamma, u16Ginv, u16A, u16B, u16S);
            }
            // input > 1 case --> to fix
            for ( ; u16LutIdx < 256; u16LutIdx++)
                u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];
        }
        else if(E_GAMMA_MASERATI_MD == IpType)
        {
            u16NumEntries = 512;
            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {
                //u16Val = ((U32)u16LutIdx << 8);
                u16Val = (((U32)u16LutIdx * 1023)+2)>>3;
                u32LutData[u16LutIdx] = MDrv_HDR_Gamma_ali(u16Val, u16Gamma, u16Ginv, u16A, u16B, u16S);
            }
        }

    }
    else if (pData->sMode == 1) // DCI-P3
    {
        *status = 0;
        printk("Gamma ERROR! Gamma mode (ST 428-1) is currently unsuported, Gamma table has not been updated.\n");
        // todo
        //HDR_DBG_HAL(printk("\n  Kernel Error!! sMode is 1 \n"));
    }
    else if (pData->sMode == 2) // PQ
    {
        u16NumEntries = 256;//* g_u16HdrY2RRatio;
        u64Denum = MDrv_HDR_PQ_ali(max_lum_code/*pData->tData[0]*/);
        //cout<<"u64Denum : "<< u64Denum<<endl;
        for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
        {
            u16Val = ((U32)u16LutIdx << 8);// / g_u16HdrY2RRatio;
            u64Nume = MDrv_HDR_PQ_ali(u16Val);

            u32LutData[u16LutIdx] = min(u64Nume * 0xFFF00000ul / u64Denum, 0xFFF00000ul);

            //u32LutData[u16LutIdx] = min(u64Nume * 0xFFFFFul / u64Denum, 0xFFFFFul);

        }
        for ( ; u16LutIdx < 256; u16LutIdx++)
            u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];
    }
    else if (pData->sMode == 4)  // Bt1886
    {
        u16Gamma = pData->sData[0];
        //u16B = pData->sData[1];
        u16Ginv = 0x1000000ul / u16Gamma;
        if(0 == u16White)
        {
            printk("Gamma warning! u16White == 0, black and white level might be unintilized, changing u16White and u16Black to 0xFF00 and 0x42\n");
            u16White = 0xFF00;
            u16Black - 0x42;
        }


        for(u16Index = 0; u16Index < 256; u16Index++)
        {
            u32LutData[u16Index] = MDrv_HDR_Bt1886Gamma_ali(u16Index<<8, u16Gamma, u16Ginv, u16White, u16Black);
            u32LutData[u16Index] = (u32LutData[u16Index] - (u32LutData[u16Index]>>8)) >> 4;
        }
    }
    else// if (mode == 3) // Log
    {
        *status = 0;
        printk("DeGamma E RROR! DeGamma mode (Log) is currently unsuported, Degamma table has not been updated.\n");
        // todo
        //HDR_DBG_HAL(printk("\n  Kernel Error!! sMode is 3 \n"));
    }

}
void writeGamma(U32* u32LutData,MS_U8 debugMode)
{
#if RealChip
#else
    //FILE *f;
#endif
    MS_U16 SramData;
    MS_U16 u16Index;

    if(2 == debugMode)
    {
#if RealChip
#else
        //f = fopen("Gamma.txt", "w");
        if (f == NULL)
        {
            //printk("Error opening file!\n");
        }
#endif
    }

#ifdef VOP2AUTODOWNLOAD
    U16* u16LutData = (U16*) kmalloc( 256 * sizeof(U16), GFP_KERNEL);
    for(u16Index = 0; u16Index<256; u16Index++)
    {
        u16LutData[u16Index] = min( (u32LutData[u16Index]+(1<<3))>>4,0x0FFF);
    }

    ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO stAutoDownloadFormat;
    stAutoDownloadFormat.enSubClient = E_KDRV_XC_AUTODOWNLOAD_XVYCC_SUB_GAMMA;
    stAutoDownloadFormat.bEnableRange = 1;
    stAutoDownloadFormat.u16StartAddr = 0;
    stAutoDownloadFormat.u16EndAddr = 255;
    //MHal_XC_W2BYTE(REG_SC_BK7A_70_L, 0x8000);
    if(1 == MHal_XC_WriteAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_XVYCC, (MS_U8*)&(u16LutData[0]),256 * sizeof(MS_U16), (void*)&stAutoDownloadFormat) )
    {
        printk("VOP2 Gamma autodownload Ready!\n");
        //MHal_XC_W2BYTE(0x133082, 0x444);

        msleep(50);
        //MHal_XC_FireAutoDownload(E_KDRV_XC_AUTODOWNLOAD_CLIENT_XVYCC);
    }
    else
    {
        printk("VOP2 Gamma autodownload failed!\n");
    }

	kfree(u16LutData);
#else
    for( u16Index = 0; u16Index <256; u16Index++)
    {

        //SramData = ((*(gammaTable+u16Index*2))) + ((*(gammaTable+u16Index*2+1))*256);

        SramData=  min( (u32LutData[u16Index]+(1<<3))>>4,0x0FFF);//MST_LinearRGB_GAMMA_SRAM_Main_1[colorMapSettings[ipNumber]][u16Index*2] + (MST_LinearRGB_GAMMA_SRAM_Main_1[colorMapSettings[ipNumber]][u16Index*2+1]<<8);
        //SramData=  min( u32LutData[u16Index],0xFFFF);//MST_LinearRGB_GAMMA_SRAM_Main_1[colorMapSettings[ipNumber]][u16Index*2] + (MST_LinearRGB_GAMMA_SRAM_Main_1[colorMapSettings[ipNumber]][u16Index*2+1]<<8);
#if RealChip
#else
        if(2 == debugMode)
        {
            //fprintf(f,"%d: %0#x\n", u16Index,SramData);
        }
#endif
        if(1 == debugMode)
        {
            //printk("%d: %0#x\n", u16Index,SramData);
        }

        /*
                U16 i;
                printk("Gamma table is:\n");
                for(i = 0; i < 256; i+=16)
                {
                    printk("%d: %0#x\n",i, ((u32LutData[i])));//+ (1<<19)/ *0x80000* /)>>20));
                }*/
        // waiting ready.
        //while (MApi_XC_R2BYTE(REG_SC_Ali_BK25_78_L) & _BIT3);

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK25_79_L, u16Index,0x0FFF); // address
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK25_7A_L, SramData,0x0FFF); //data
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK25_78_L, 0x60, 0x60); // Select All Channel
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK25_78_L, _BIT3, _BIT3); // io_w enable
    }

    SramData=  min( (u32LutData[256]+(1<<3))>>4,0xFFFFFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK25_7B_L,0xFFFF, SramData); //Max data
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_Ali_BK25_78_L,0x0400, 0400); //Max data enable
#endif



}

void MDrv_HDR_WriteDegammaTable_DeltaMode_600Entries_ali(U32* u32Lut,MS_U16 g_u16Hdr3x3Ratio,MS_U8 lut_en)
{
    MS_U16 u16InputData1, u16InputData2, u16InputData3;
    MS_U32 u32Ref, u32Delta1, u32Delta2;
    MS_U32 u32XVYCC_DGT_DATA;
    MS_U16 u16SRamCode_L, u16SRamCode_H;
    MS_U16 u16WriteCount, u16SpLevel, u16SpAddr[4];
    MS_U16 u16Delay = 0xFFFF;
    MS_U16 u16ReadCount ;
    MS_U16 diffSum = 0;
    //MS_U32 readTable[600];
    //MS_U32 enReadData[300];
    //MS_U16 testRef[300],testDelta[300];
    MS_U16* testRef = (MS_U16*) kmalloc( 300 * sizeof(MS_U16), GFP_KERNEL);
    MS_U16* testDelta = (MS_U16*) kmalloc( 300 * sizeof(MS_U16), GFP_KERNEL);
    MS_U32* readTable = (MS_U32*) kmalloc( 600 * sizeof(MS_U32), GFP_KERNEL);
    MS_U32* enReadData = (MS_U32*) kmalloc( 300 * sizeof(MS_U32), GFP_KERNEL);

    // RGB write together and write one time for clear write bit.
    write2ByteMask(REG_SC_Ali_BK2D_60_L, _BIT5 | _BIT6, _BIT5 | _BIT6 );
    //if( MApi_XC_W2BYTEMSK(REG_SC_Ali_BK2D_60_L, _BIT5|_BIT6, _BIT5|_BIT6) != (_BIT5|_BIT6))
    //{
    //MApi_XC_W2BYTEMSK(REG_SC_Ali_BK2D_60_L, _BIT5|_BIT6, _BIT5|_BIT6 );
    //}

    u16SpAddr[0] = 0xFFFF;
    u16SpAddr[1] = 0xFFFF;
    u16SpAddr[2] = 0xFFFF;
    u16SpLevel = 0;
    for(u16WriteCount = 0; u16WriteCount < 300; u16WriteCount++)
    {

        if(1== lut_en)
        {
            u16InputData1 = u32Lut[u16WriteCount*2];
            u16InputData2 = u32Lut[u16WriteCount*2+1];
            u16InputData3 = u32Lut[u16WriteCount*2+2];
        }
        else
        {
            u16InputData1 = ( (( u32Lut[u16WriteCount*2] + (1<<16)  )>>17) * g_u16Hdr3x3Ratio + (1<<8))>>(3+6);
            u16InputData2 = ( (( u32Lut[u16WriteCount*2+1] + (1<<16)  )>>17) * g_u16Hdr3x3Ratio + (1<<8))>>(3+6);
            u16InputData3 = ( (( u32Lut[u16WriteCount*2+2] + (1<<16)  )>>17) * g_u16Hdr3x3Ratio + (1<<8))>>(3+6);
        }

        u32Ref = min(u16InputData1, 0x7FFF); // 15bis
        u32Delta1 = min(u16InputData2 - u16InputData1, 0x7F); // 7 bits
        u32Delta2 = min(u16InputData3 - u16InputData2, 0x7F); // 7 bits

        testRef[u16WriteCount] = u32Ref;
        testDelta[u16WriteCount] = u32Delta1;

        if ((u32Delta1>>5) > u16SpLevel)
        {
            u16SpAddr[u16SpLevel] = u16WriteCount*2;
            u16SpLevel++;
        }
        else if ((u32Delta2>>5) > u16SpLevel)
        {
            u16SpAddr[u16SpLevel] = u16WriteCount*2+1;
            u16SpLevel++;
        }

        u32Delta1 = u32Delta1 & 0x1F; // 5 bits
        u32Delta2 = u32Delta2 & 0x1F; // 5 bits
        u32XVYCC_DGT_DATA = (u32Ref<<10) + (u32Delta1<<5) + u32Delta2;

        u16SRamCode_L = u32XVYCC_DGT_DATA&0xFFFF;
        u16SRamCode_H = u32XVYCC_DGT_DATA>>16;
        //cout<<dec<<u16WriteCount<<": "<<hex<< u32XVYCC_DGT_DATA <<endl;
        // waiting ready.
        //while(MApi_XC_W2BYTEMSK(REG_SC_Ali_BK2D_60_L, BIT(3))&& (--u16Delay)){;}
        //while(MApi_XC_R2BYTE(REG_SC_Ali_BK2D_60_L) && _BIT3){;}

        if(u16Delay==0)
        {
            printk("\n Kernel HDR WriteGamma timeout \n");
        }

        write2ByteMask(REG_SC_Ali_BK2D_61_L,0xFFFF, u16WriteCount); // address
        write2ByteMask(REG_SC_Ali_BK2D_63_L,0xFFFF, u16SRamCode_H); //data
        write2ByteMask(REG_SC_Ali_BK2D_62_L,0xFFFF, u16SRamCode_L); //data
        write2ByteMask(REG_SC_Ali_BK2D_60_L, _BIT3, _BIT3); // io_w enable
    }

    //write offset value separate point
    write2ByteMask(REG_SC_Ali_BK2D_66_L,0xFFFF, u16SpAddr[0]);
    write2ByteMask(REG_SC_Ali_BK2D_67_L,0xFFFF, u16SpAddr[1]);
    write2ByteMask(REG_SC_Ali_BK2D_68_L,0xFFFF, u16SpAddr[2]);
    write2ByteMask(REG_SC_Ali_BK2D_69_L,0xFFFF, u16SpAddr[0]);
    write2ByteMask(REG_SC_Ali_BK2D_6A_L,0xFFFF, u16SpAddr[1]);
    write2ByteMask(REG_SC_Ali_BK2D_6B_L,0xFFFF, u16SpAddr[2]);
    write2ByteMask(REG_SC_Ali_BK2D_6C_L,0xFFFF, u16SpAddr[0]);
    write2ByteMask(REG_SC_Ali_BK2D_6D_L,0xFFFF, u16SpAddr[1]);
    write2ByteMask(REG_SC_Ali_BK2D_6E_L,0xFFFF, u16SpAddr[2]);

    //write offset value
    write2ByteMask(REG_SC_Ali_BK2D_6F_L,0xFFFF, 0x00E4);
    write2ByteMask(REG_SC_Ali_BK2D_70_L,0xFFFF, 0x00E4);
    write2ByteMask(REG_SC_Ali_BK2D_71_L,0xFFFF, 0x00E4);


    for(u16ReadCount = 0; u16ReadCount < 300; u16ReadCount++)
    {
        //write2ByteMask(REG_SC_Ali_BK2D_61_L,0xFFFF, u16ReadCount); // address
        //write2ByteMask(REG_SC_Ali_BK2D_60_L, _BIT4, _BIT4); // io_r enable
        u16SRamCode_H = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_Ali_BK2D_65_L,0xFFFF); //data
        u16SRamCode_L = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_Ali_BK2D_64_L,0xFFFF); //data
        //u32XVYCC_DGT_DATA = (u32Ref<<10) + (u32Delta1<<5) + u32Delta2;
        u32XVYCC_DGT_DATA = (u16SRamCode_H<<16) +  u16SRamCode_L;
        testRef[u16ReadCount] = u32XVYCC_DGT_DATA>>10;
        testDelta[u16ReadCount] = (u32XVYCC_DGT_DATA>>5) & 0x001F;
    }

    for(u16ReadCount = 0; u16ReadCount < 300; u16ReadCount++)
    {
        readTable[u16ReadCount*2] = testRef[u16ReadCount];
        readTable[u16ReadCount*2 + 1] = testRef[u16ReadCount] + testDelta[u16ReadCount];
        /*
                if (u16ReadCount*2 >= u16SpAddr[0])
                    readTable[u16ReadCount*2]+=32;
                if (u16ReadCount*2 >= u16SpAddr[1])
                    readTable[u16ReadCount*2]+=32;
                if (u16ReadCount*2 >= u16SpAddr[2])
                    readTable[u16ReadCount*2]+=32;
        */
        if(0 != testDelta[u16ReadCount])
        {
            if ( (u16ReadCount*2 +1) >= u16SpAddr[0] &&  u16SpAddr[0]>0)
                readTable[u16ReadCount*2 + 1]+=32;
            if ( (u16ReadCount*2 +1) >= u16SpAddr[1] && u16SpAddr[1]>u16SpAddr[0])
                readTable[u16ReadCount*2 + 1]+=32;
            if ( (u16ReadCount*2 +1) >= u16SpAddr[2] && u16SpAddr[2]>u16SpAddr[1])
                readTable[u16ReadCount*2 + 1]+=32;
        }
    }

    printk("DeGamma Table as read from Sram\n");
    for(u16ReadCount = 0; u16ReadCount < 600; u16ReadCount+=60)
    {
        printk("%d: %0#x\n", u16ReadCount,readTable[u16ReadCount]);
    }

    kfree(readTable);
    kfree(enReadData);
    kfree(testRef);
    kfree(testDelta);

    /*
        FILE *f = fopen("deGammaReadTable.txt", "w");
        if (f == NULL)
        {
            printk("Error opening file!\n");
            return;
        }
        for(int i = 0; i < 600; i++)
        {
            fprintf(f,"%d: %0#x\n", i,readTable[i]);
        }*/
}
void deGammaDebug(U32* u32Lut,MS_U8 debugMode,MS_U16 ratio2,MS_U8 lut_en)
{
    MS_U32 i;
    if(1 == debugMode)
    {
        for(i = 0; i < 600; i++)
        {
            printk("%d: %0#x\n", i,(u32Lut[i]/*+0x80000)>>20*/));
        }
    }
#if RealChip
#else
    if(2 == debugMode)
    {
        //FILE *f = fopen("deGamma.txt", "w");
        if (0)//f == NULL)
        {
            //printk("Error opening file!\n");
            return;
        }
        for(i = 0; i < 600; i++)
        {
            //fprintf(f,"%d: %0#x\n", i,((u32Lut[i]+ (1<<19)/*0x80000*/)>>20));
            //fprintf(f,"%d: %0#x\n", i,((u32Lut[i]/*+ (1<<19)*/*0x8)>>20));
            if(0 == lut_en)
            {
                //fprintf(f,"%d: %0#x\n", i,( (( u32Lut[i] + (1<<16)  )>>17) * ratio2 + (1<<8))>>(3+6));
            }
            else
            {
                //fprintf(f,"%d: %0#x\n", i,( (( u32Lut[i]   )) * ratio2 + (1<<5))>>(6));
            }

            //fprintf(f,"%d: %0#x\n", i,( (( u32Lut[i] + (1<<16)  )>>17) * ratio2 + (1<<8))>>(3+6));

        }
    }
#endif
}
MS_U8 Mhal_CFD_OldDeGamma(MS_U8 transferCharacterstics,
                          MS_U16 max_lum_code,
                          MS_U8 ratio1, MS_U16 ratio2,MS_U8 lut_en, U32* lut_idx, MS_U16 Lut_length, MS_U8 IpType,MS_U8 inputFormatFlag,MS_U8 debugMode) //debug mode: 0 = off, 1 = print2screen, 2 = print2file
{
    MS_U8 status = 1;
    MS_U16 u16ArrayLength = (2 == IpType) ? 600 : 600;
    U32* u32LutData = (U32*) kmalloc( u16ArrayLength * sizeof(U32), GFP_KERNEL);
    //U32 u32LutData[600];
    MS_U16 i;
    //U32* deGammaTable;
    ST_TRANSFER_FUNCTION_DATA stDeGammaData;

    if (lut_en)
    {

        for( i = 0; i < u16ArrayLength; i++)
        {
            u32LutData[i] = lut_idx[min(i,Lut_length-1)];
            printk("%d\n", lut_idx[min(i,Lut_length-1)]) ;
        }
        //deGammaTable = lut_idx;
    }

    else
    {
        parseInput4DeGamma(transferCharacterstics,&stDeGammaData, inputFormatFlag);
        if(ratio1 > 0x80)
        {
            ratio1 = 0x80;
            printk("deGamma warning: ratio1 exceeds the limit, clamped to 0x80");
        }
        if(ratio2 > 0x200)
        {
            ratio2 = 0x200;
            printk("deGamma warning: ratio2 exceeds the limit, clamped to 0x200");
        }
        deGammaGenerator(u32LutData, &stDeGammaData, max_lum_code,ratio1,&status,  IpType);
        //deGammaTable = u32LutData;
    }
    if(1 == status)
    {
        if(E_DEGAMMA_MANHATTAN_DELTA_VOP2 == IpType)
        {
            MDrv_HDR_WriteDegammaTable_DeltaMode_600Entries_ali(u32LutData,ratio2,lut_en);
        }
        else if(E_DEGAMMA_MASERATI_NORMAL_VOP2 == IpType)
        {
            writeDeGammaNormalMode(u32LutData,ratio2);
        }
        else if(E_DEGAMMA_MASERATI_MD == IpType)
        {
            writeMdDeGamma(u32LutData,debugMode,transferCharacterstics,lut_en);
        }
        else
        {
            printk("Degamma IP type is unsupported, Degamma will not be updated\n");
            status = 0;
        }



        deGammaDebug(u32LutData,debugMode, ratio2, lut_en);

        U16 i;
#ifdef CFD_debug
        printk("DeGamma table is:\n");
        for(i = 0; i < 600; i+=60)
        {
            printk("%d: %0#x\n",i, ((u32LutData[i]+ (1<<19)/*0x80000*/)>>20));
            //u16InputData1 = ( (( u32LutData[i*2] + (1<<16)  )>>17) * g_u16Hdr3x3Ratio + (1<<8))>>(3+6);
            printk("%d: %0#x\n",i, ( (( u32LutData[i] + (1<<16)  )>>17) * ratio2 + (1<<8))>>(3+6));
        }
#endif
    }


    kfree(u32LutData);
    return status;
}

MS_U8 oldGamma(MS_U8 transferCharacterstics,
               MS_U16 max_lum_code,
               MS_U16 u16White, MS_U16 u16Black, MS_U8 lut_en, U32* lut_idx, MS_U16 Lut_length, MS_U8 IpType,MS_U8 inputFormatFlag,MS_U8 debugMode)
{
    MS_U8 status = 1;
    MS_U16 u16ArrayLength = (0 == IpType) ? 256 : 512;
    U32* u32LutData = (U32*) kmalloc( u16ArrayLength * sizeof(U32), GFP_KERNEL);
    //U32 u32LutData[256];
    //U32 u32LutData[512];
    MS_U16 i;

    //U32* GammaTable;
    ST_TRANSFER_FUNCTION_DATA stDeGammaData;

    if (lut_en)
    {

        for( i = 0; i < u16ArrayLength; i++)
        {
            u32LutData[i] = lut_idx[min(i,Lut_length-1)];
        }
        //deGammaTable = lut_idx;
    }

    else
    {
        parseInput4DeGamma(transferCharacterstics,&stDeGammaData, inputFormatFlag);
        GammaGenerator(u32LutData, &stDeGammaData, max_lum_code, u16White,  u16Black,&status,  IpType);
        //GammaTable = u32LutData;
    }
    if(1 == status)
    {
        switch(IpType)
        {
            case E_GAMMA_MANHATTAN_VOP2:
                writeGamma(u32LutData,debugMode);
                break;
            case E_GAMMA_MASERATI_MD:
                writeMdGamma(u32LutData,debugMode,transferCharacterstics);
                break;
            default:
                printk("Current Gamma IpType is unsupported,Gamma table will not be updated.\n");
                status = 0;
                break;
        }



    }


    kfree(u32LutData);
    return status;
}
void gamutMappingFlagProccessing(gamutMappingDataStruct* gamutMappingData, MS_U8 colorPrimaries, MS_BOOL isInput)
{

    MS_BOOL isValid = 1; //  data valid
    MS_U16 Rx; //  Rx
    MS_U16 Ry; //  Ry
    MS_U16 Gx; //  Gx
    MS_U16 Gy; //  Gy
    MS_U16 Bx; //  Bx
    MS_U16 By; //  By
    MS_U16 Wx; //  Wx
    MS_U16 Wy; //  Wy
    switch(colorPrimaries)
    {
        case 1:
            Gx = 15000; //0.30
            Gy = 30000; //0.60
            Bx =  7500; //0.15
            By =  3000; //0.06
            Rx = 32000; //0.64
            Ry = 16500; //0.33
            Wx = 15635; //0.3127
            Wy = 16450; //0.3290
            break;
        case 4:
            Gx = 10500; //0.21
            Gy = 35500; //0.71
            Bx =  7000; //0.14
            By =  4000; //0.08
            Rx = 33500; //0.67
            Ry = 16500; //0.33
            Wx = 15500; //0.310
            Wy = 15800; //0.316
            break;
        case 5:
            Gx = 14500; //0.29
            Gy = 30000; //0.60
            Bx =  7500; //0.15
            By =  3000; //0.06
            Rx = 32000; //0.64
            Ry = 16500; //0.33
            Wx = 15635; //0.3127
            Wy = 16450; //0.3290
            break;
        case 6:
        case 7:
            Gx = 15500; //0.31
            Gy = 29750; //0.595
            Bx =  7750; //0.155
            By =  3500; //0.07
            Rx = 31500; //0.63
            Ry = 17000; //0.34
            Wx = 15635; //0.3127
            Wy = 16450; //0.3290
            break;
        case 8:
            Gx = 12150; //0.243
            Gy = 34600; //0.692
            Bx =  7250; //0.145
            By =  2450; //0.049
            Rx = 34050; //0.681
            Ry = 15950; //0.319
            Wx = 15500; //0.310
            Wy = 15800; //0.316
            break;
        case 9:
            Gx =  8500; //0.17
            Gy = 39850; //0.797
            Bx =  6550; //0.131
            By =  2300; //0.046
            Rx = 35400; //0.708
            Ry = 14600; //0.292
            Wx = 15635; //0.3127
            Wy = 16450; //0.3290
            break;
        case 10:
            Gx =     0; //0.0
            Gy = 50000; //1.0
            Bx =     0; //0.0
            By =     0; //0.0
            Rx = 50000; //1.0
            Ry =     0; //0.0
            Wx = 16650; //0.333
            Wy = 16650; //0.333
            break;
        case 11:
            Gx = 10500; //  0.21
            Gy = 35500; // 0.71
            Bx = 7500; //0.15
            By = 3000; //0.06
            Rx = 32000; //0.64
            Ry = 16500; // 0.33
            Wx = 15635; // 0.3127
            Wy = 16450; //0.329
            break;
        case 2: // Unspecified
        default:
            Gx = NULL; //0.0
            Gy = NULL; //1.0
            Bx = NULL; //0.0
            By = NULL; //0.0
            Rx = NULL; //1.0
            Ry = NULL; //0.0
            Wx = NULL; //0.333
            Wy = NULL; //0.333
            isValid = FALSE;
            break;
    }

    if (1 == isInput)
    {
        gamutMappingData->sourceValid = isValid;
        gamutMappingData->sourceRx = Rx;
        gamutMappingData->sourceRy = Ry;
        gamutMappingData->sourceGx = Gx;
        gamutMappingData->sourceGy = Gy;
        gamutMappingData->sourceBx = Bx;
        gamutMappingData->sourceBy = By;
        if (_stCfdPanel.bCustomerColorPrimaries == TRUE && u8InEqualOutCP != 1)
        {
            gamutMappingData->sourceWx = _stCfdPanel.u16SourceWx;
            gamutMappingData->sourceWy = _stCfdPanel.u16SourceWy;
        }
        else
        {
            gamutMappingData->sourceWx = Wx;
            gamutMappingData->sourceWy = Wy;
        }
    }
    else
    {
        gamutMappingData->targetValid = isValid;
        gamutMappingData->targetRx = Rx;
        gamutMappingData->targetRy = Ry;
        gamutMappingData->targetGx = Gx;
        gamutMappingData->targetGy = Gy;
        gamutMappingData->targetBx = Bx;
        gamutMappingData->targetBy = By;
        gamutMappingData->targetWx = Wx;
        gamutMappingData->targetWy = Wy;
    }
}
void parseInput4gamutMapping(gamutMappingDataStruct* gamutMappingData, MS_U8 inputColorcolorPrimaries,MS_U8 outputMode, /*MS_U16 *ouputColormetry*/STU_CFD_COLORIMETRY *pt_ouput_colormetry, MS_U8 outputColorcolorPrimaries)
{
    if(inputColorcolorPrimaries == outputColorcolorPrimaries)
    {
        u8InEqualOutCP = 1;
    }
    else
    {
        u8InEqualOutCP = 0;
    }
    gamutMappingFlagProccessing(gamutMappingData,inputColorcolorPrimaries,1);
    if(1 == outputMode)
    {
        gamutMappingFlagProccessing(gamutMappingData,outputColorcolorPrimaries,0);
    }
    else
    {


        gamutMappingData->targetValid = 1;
        gamutMappingData->targetRx = pt_ouput_colormetry->u16Display_Primaries_x[0];
        gamutMappingData->targetRy = pt_ouput_colormetry->u16Display_Primaries_y[0];
        gamutMappingData->targetGx = pt_ouput_colormetry->u16Display_Primaries_x[1];
        gamutMappingData->targetGy = pt_ouput_colormetry->u16Display_Primaries_y[1];
        gamutMappingData->targetBx = pt_ouput_colormetry->u16Display_Primaries_x[2];
        gamutMappingData->targetBy = pt_ouput_colormetry->u16Display_Primaries_y[2];
        gamutMappingData->targetWx = pt_ouput_colormetry->u16White_point_x;
        gamutMappingData->targetWy = pt_ouput_colormetry->u16White_point_y;
    }
}
S64 MDrv_HDR_MatrixDetGen_ali(MS_U32 a, MS_U32 b, MS_U32 c, MS_U32 d, MS_U32 e, MS_U32 f, MS_U32 g, MS_U32 h, MS_U32 i)
{
    S64 ei_fh = (S64)e*i - (S64)f*h;
    S64 fg_di = (S64)f*g - (S64)d*i;
    S64 dh_eg = (S64)d*h - (S64)e*g;
    S64 det = a*ei_fh + b*fg_di + c*dh_eg;
    return det;
}
void MDrv_HDR_RGB2XYZGen_ali(MS_U32 M[3][3], MS_U16 Rx, MS_U16 Ry, MS_U16 Gx, MS_U16 Gy, MS_U16 Bx, MS_U16 By, MS_U16 Wx, MS_U16 Wy)
{
    // 0 <= (R/G/B)(x/y/z) <= 50000
    MS_U16 Rz = 50000 - Rx - Ry;
    MS_U16 Gz = 50000 - Gx - Gy;
    MS_U16 Bz = 50000 - Bx - By;
    MS_U16 Wz = 50000 - Wx - Wy;

    MS_U32 WX = ((U32)Wx * 50000) / Wy;
    MS_U32 WY = 50000;
    MS_U32 WZ = ((U32)Wz * 50000) / Wy;

    S64 det0 = MDrv_HDR_MatrixDetGen_ali(Rx, Gx, Bx, Ry, Gy, By, Rz, Gz, Bz);
    S64 det1 = MDrv_HDR_MatrixDetGen_ali(WX, Gx, Bx, WY, Gy, By, WZ, Gz, Bz);
    S64 det2 = MDrv_HDR_MatrixDetGen_ali(Rx, WX, Bx, Ry, WY, By, Rz, WZ, Bz);
    S64 det3 = MDrv_HDR_MatrixDetGen_ali(Rx, Gx, WX, Ry, Gy, WY, Rz, Gz, WZ);
    if(det0 == 0)
    {
        det0 = 1;
        printk("gamutMapping warning: det0 = 0,output color primaries format might be incorrect. Replacing det0 with 1\n");
    }
    U32 RS = (MS_U32)((MS_U64) det1 * 50000 / det0);
    U32 GS = (MS_U32)((MS_U64) det2 * 50000 / det0);
    U32 BS = (MS_U32)((MS_U64) det3 * 50000 / det0);

    M[0][0] = (MS_U32)((MS_U64) Rx * RS / 50000);
    M[1][0] = (MS_U32)((MS_U64) Ry * RS / 50000);
    M[2][0] = (MS_U32)((MS_U64) Rz * RS / 50000);
    M[0][1] = (MS_U32)((MS_U64) Gx * GS / 50000);
    M[1][1] = (MS_U32)((MS_U64) Gy * GS / 50000);
    M[2][1] = (MS_U32)((MS_U64) Gz * GS / 50000);
    M[0][2] = (MS_U32)((MS_U64) Bx * BS / 50000);
    M[1][2] = (MS_U32)((MS_U64) By * BS / 50000);
    M[2][2] = (MS_U32)((MS_U64) Bz * BS / 50000);

}
void MDrv_HDR_RGB2RGBGen_ali(S32 Mout[9], MS_U32 Msrc[3][3], MS_U32 Mdst[3][3])
{
    S32 Minv[3][3], M[3][3];

    M[0][0] = Mdst[0][0];
    M[0][1] = Mdst[0][1];
    M[0][2] = Mdst[0][2];
    M[1][0] = Mdst[1][0];
    M[1][1] = Mdst[1][1];
    M[1][2] = Mdst[1][2];
    M[2][0] = Mdst[2][0];
    M[2][1] = Mdst[2][1];
    M[2][2] = Mdst[2][2];
    matrixInvGen(Minv, M);

    // Mout = Mdst^-1 * Msrc
    Mout[0] = ((S64)Minv[0][0]*Msrc[0][0] + (S64)Minv[0][1]*Msrc[1][0] + (S64)Minv[0][2]*Msrc[2][0]) / 50000;
    Mout[1] = ((S64)Minv[0][0]*Msrc[0][1] + (S64)Minv[0][1]*Msrc[1][1] + (S64)Minv[0][2]*Msrc[2][1]) / 50000;
    Mout[2] = ((S64)Minv[0][0]*Msrc[0][2] + (S64)Minv[0][1]*Msrc[1][2] + (S64)Minv[0][2]*Msrc[2][2]) / 50000;
    Mout[3] = ((S64)Minv[1][0]*Msrc[0][0] + (S64)Minv[1][1]*Msrc[1][0] + (S64)Minv[1][2]*Msrc[2][0]) / 50000;
    Mout[4] = ((S64)Minv[1][0]*Msrc[0][1] + (S64)Minv[1][1]*Msrc[1][1] + (S64)Minv[1][2]*Msrc[2][1]) / 50000;
    Mout[5] = ((S64)Minv[1][0]*Msrc[0][2] + (S64)Minv[1][1]*Msrc[1][2] + (S64)Minv[1][2]*Msrc[2][2]) / 50000;
    Mout[6] = ((S64)Minv[2][0]*Msrc[0][0] + (S64)Minv[2][1]*Msrc[1][0] + (S64)Minv[2][2]*Msrc[2][0]) / 50000;
    Mout[7] = ((S64)Minv[2][0]*Msrc[0][1] + (S64)Minv[2][1]*Msrc[1][1] + (S64)Minv[2][2]*Msrc[2][1]) / 50000;
    Mout[8] = ((S64)Minv[2][0]*Msrc[0][2] + (S64)Minv[2][1]*Msrc[1][2] + (S64)Minv[2][2]*Msrc[2][2]) / 50000;
}
void MDrv_HDR_RGB2RGBGen_2020CL_ali(MS_S32 Mout[9], MS_S32 Mr2r[9])
{
    S32 M1[3][3], M2[3][3];

    M1[0][0] = Mr2r[0];
    M1[0][1] = Mr2r[1];
    M1[0][2] = Mr2r[2];
    M1[1][0] = Mr2r[3];
    M1[1][1] = Mr2r[4];
    M1[1][2] = Mr2r[5];
    M1[2][0] = Mr2r[6];
    M1[2][1] = Mr2r[7];
    M1[2][2] = Mr2r[8];

    // M RYB to RGB bt. 2020CL
    // [ 1      0       0
    //  -0.3875 1.4749 -0.0875
    //   0      0       1     ]

    M2[0][0] = 50000;
    M2[0][1] = 0;
    M2[0][2] = 0;
    M2[1][0] = -19373;
    M2[1][1] = 73746;
    M2[1][2] = -4373;
    M2[2][0] = 0;
    M2[2][1] = 0;
    M2[2][2] = 50000;

    // Mout = Mr2r * Mryb2rgb
    Mout[0] = ((S64)M1[0][0] * M2[0][0] + (S64)M1[0][1] * M2[1][0] + (S64)M1[0][2] * M2[2][0]) / 50000;
    Mout[1] = ((S64)M1[0][0] * M2[0][1] + (S64)M1[0][1] * M2[1][1] + (S64)M1[0][2] * M2[2][1]) / 50000;
    Mout[2] = ((S64)M1[0][0] * M2[0][2] + (S64)M1[0][1] * M2[1][2] + (S64)M1[0][2] * M2[2][2]) / 50000;
    Mout[3] = ((S64)M1[1][0] * M2[0][0] + (S64)M1[1][1] * M2[1][0] + (S64)M1[1][2] * M2[2][0]) / 50000;
    Mout[4] = ((S64)M1[1][0] * M2[0][1] + (S64)M1[1][1] * M2[1][1] + (S64)M1[1][2] * M2[2][1]) / 50000;
    Mout[5] = ((S64)M1[1][0] * M2[0][2] + (S64)M1[1][1] * M2[1][2] + (S64)M1[1][2] * M2[2][2]) / 50000;
    Mout[6] = ((S64)M1[2][0] * M2[0][0] + (S64)M1[2][1] * M2[1][0] + (S64)M1[2][2] * M2[2][0]) / 50000;
    Mout[7] = ((S64)M1[2][0] * M2[0][1] + (S64)M1[2][1] * M2[1][1] + (S64)M1[2][2] * M2[2][1]) / 50000;
    Mout[8] = ((S64)M1[2][0] * M2[0][2] + (S64)M1[2][1] * M2[1][2] + (S64)M1[2][2] * M2[2][2]) / 50000;

}
void MDrv_HDR_GamutMapping3x3Gen_ali(gamutMappingDataStruct* pData,MS_S32 *M3x3, MS_U8 inputFormatFlag)
{
    MS_U32 Msrc[3][3], Mdst[3][3];
    MS_S32 Mr2r[9];

    MDrv_HDR_RGB2XYZGen_ali(Msrc, pData->sourceRx, pData->sourceRy, pData->sourceGx, pData->sourceGy, pData->sourceBx, pData->sourceBy, pData->sourceWx, pData->sourceWy);
    MDrv_HDR_RGB2XYZGen_ali(Mdst, pData->targetRx, pData->targetRy, pData->targetGx, pData->targetGy, pData->targetBx, pData->targetBy, pData->targetWx, pData->targetWy);

    if (inputFormatFlag != 12)
    {
        MDrv_HDR_RGB2RGBGen_ali(M3x3, Msrc, Mdst);
    }
    else  // bt.2020 CL
    {
        MDrv_HDR_RGB2RGBGen_ali(Mr2r, Msrc, Mdst);
        MDrv_HDR_RGB2RGBGen_2020CL_ali(M3x3, Mr2r);
    }

}
void gamutMappingWrite2register(MS_S32 *M3x3,MS_U16 g_u16Hdr3x3Ratio,MS_U8 debugMode)
{
    MS_U16 u16Regs[9];
    MS_U32 ratio = (50000*g_u16Hdr3x3Ratio)>>6;
    MS_U32 halfRatio = ratio>>1;
    MS_U8 i;
    for (i = 0; i < 9; i++)
    {
        MS_U16 temp = ((( abs(M3x3[i]) << 10) + halfRatio) / (ratio));
        u16Regs[i] = (M3x3[i] > 0) ? temp & 0x1FFF : (-1 * temp) & 0x1FFF;
    }


    write2Byte(REG_SC_Ali_BK25_02_L, u16Regs[0]);
    write2Byte(REG_SC_Ali_BK25_03_L, u16Regs[1]);
    write2Byte(REG_SC_Ali_BK25_04_L, u16Regs[2]);
    write2Byte(REG_SC_Ali_BK25_05_L, u16Regs[3]);
    write2Byte(REG_SC_Ali_BK25_06_L, u16Regs[4]);
    write2Byte(REG_SC_Ali_BK25_07_L, u16Regs[5]);
    write2Byte(REG_SC_Ali_BK25_08_L, u16Regs[6]);
    write2Byte(REG_SC_Ali_BK25_09_L, u16Regs[7]);
    write2Byte(REG_SC_Ali_BK25_0A_L, u16Regs[8]);

    if(1 ==  debugMode)
    {
        printk("%0#x\n",u16Regs[0]);
        printk("%0#x\n",u16Regs[1]);
        printk("%0#x\n",u16Regs[2]);
        printk("%0#x\n",u16Regs[3]);
        printk("%0#x\n",u16Regs[4]);
        printk("%0#x\n",u16Regs[5]);
        printk("%0#x\n",u16Regs[6]);
        printk("%0#x\n",u16Regs[7]);
        printk("%0#x\n",u16Regs[8]);

    }

}

void gamutMappingWrite2registerMaseratiSub(MS_S32 *M3x3,MS_U16 g_u16Hdr3x3Ratio)
{
    MS_U16 u16Regs[9];
    MS_U32 ratio = (50000*g_u16Hdr3x3Ratio)>>6;
    MS_U32 halfRatio = ratio>>1;
    MS_U8 i;
    for (i = 0; i < 9; i++)
    {
        MS_U16 temp = ((( abs(M3x3[i]) << 10) + halfRatio) / (ratio));
        u16Regs[i] = (M3x3[i] > 0) ? temp & 0x1FFF : (-1 * temp) & 0x1FFF;
    }


    write2Byte(REG_SC_Ali_BK25_12_L, u16Regs[0]);
    write2Byte(REG_SC_Ali_BK25_13_L, u16Regs[1]);
    write2Byte(REG_SC_Ali_BK25_14_L, u16Regs[2]);
    write2Byte(REG_SC_Ali_BK25_15_L, u16Regs[3]);
    write2Byte(REG_SC_Ali_BK25_16_L, u16Regs[4]);
    write2Byte(REG_SC_Ali_BK25_17_L, u16Regs[5]);
    write2Byte(REG_SC_Ali_BK25_18_L, u16Regs[6]);
    write2Byte(REG_SC_Ali_BK25_19_L, u16Regs[7]);
    write2Byte(REG_SC_Ali_BK25_1A_L, u16Regs[8]);



}
MS_U8 gamutMapping(MS_U8 inputColorPrimaries, MS_U8 outputMode,STU_CFD_COLORIMETRY* pt_ouput_colorimetry , MS_U8 outputColorPrimaries, MS_U8 inputFormatFlag, MS_U16 ratio,MS_U8 IpType,MS_U8 debugMode)

{

    S32 M3x3[9];
    gamutMappingDataStruct gamutMappingData;

    if(ratio > 0x200)
    {
        ratio = 0x200;
        printk("gamutMapping warning: ratio exceeds the limit, clamped to 0x200");
    }

    parseInput4gamutMapping(&gamutMappingData,inputColorPrimaries,outputMode,/*ouputColormetry*/ pt_ouput_colorimetry,outputColorPrimaries);


    if(1 == gamutMappingData.sourceValid  && 1 == gamutMappingData.targetValid )
    {
        MDrv_HDR_GamutMapping3x3Gen_ali(&gamutMappingData,M3x3, inputFormatFlag);

        switch(IpType)
        {
            case E_GAMUT_MAPPING_VOP2:
                gamutMappingWrite2register(M3x3,ratio, debugMode);
                break;
            case E_GAMUT_MAPPING_MD:
                gamutMappingHdrWrite2registrer(M3x3,ratio, debugMode);
                break;
            case E_GAMUT_MAPPING_VOP2_SUB_MASERATI:
                gamutMappingWrite2registerMaseratiSub(M3x3, ratio);
                break;

            default:
                printk("current gamut mapping IP type unsupported!\n");
                return 0;
                break;
        }

        /*
                printk("source\n%d\n",gamutMappingData.targetGx);
                printk("%d\n",gamutMappingData.sourceGy);
                printk("%d\n",gamutMappingData.sourceBx);
                printk("%d\n",gamutMappingData.sourceBy);
                printk("%d\n",gamutMappingData.sourceRx);
                printk("%d\n",gamutMappingData.sourceRy);
                printk("%d\n",gamutMappingData.sourceWx);
                printk("%d\n",gamutMappingData.sourceWy);
                printk("terget\n%d\n",gamutMappingData.targetGx);
                printk("%d\n",gamutMappingData.targetGy);
                printk("%d\n",gamutMappingData.targetBx);
                printk("%d\n",gamutMappingData.targetBy);
                printk("%d\n",gamutMappingData.targetRx);
                printk("%d\n",gamutMappingData.targetRy);
                printk("%d\n",gamutMappingData.targetWx);
                printk("%d\n",gamutMappingData.targetWy);
                */
        ///*printk("%f    %f    %f\n",M3x3[0]/50000.0,M3x3[1]/50000.0,M3x3[2]/50000.0);
        //printk("%f    %f    %f\n",M3x3[3]/50000.0,M3x3[4]/50000.0,M3x3[5]/50000.0);
        //printk("%f    %f    %f\n",M3x3[6]/50000.0,M3x3[7]/50000.0,M3x3[8]/50000.0);//*/
        return 1;
    }

    if(0 == gamutMappingData.sourceValid)
    {
        printk("Source color primaries not valid, gamutMapping3x3 will not be updated\n");
    }
    if(0 == gamutMappingData.targetValid)
    {
        printk("Target color primaries not valid, gamutMapping3x3 will not be updated\n");

    }




    return 0;
}
MS_BOOL AdjustHueSatContrast(MS_U8 u8window,MS_U16 u16Hue,MS_U16 u16Saturation,MS_U16 u16Contrast)
{
    cscParametersStructure cscParameters;
    msAdjustHSC(u8window, u16Hue, u16Saturation, u16Contrast,&cscParameters);
    MS_U8 u8check =0;
    //printk("HueSatContrast IPtype:%d\n",s_AceInfo[bwindow].u8CscIpType);
    if(0 == u8window)
    {
        u8check = outputCscWrtite2registerVOP2(&cscParameters);
    }
    else
    {
        u8check = outputCscWrtite2registerVOP2SUb(&cscParameters);
    }

    if(1 == u8check)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//manhattanOnOffIp.cpp start
MS_U8 rgbClip(MS_BOOL bMode, MS_U8 u8IpType)
{

    if(E_MAIN_WINDOW == u8IpType) //main window
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK0F_57_L,0x0001,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_58_L,0x1FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_59_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_5A_L,0x1FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_5B_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_5C_L,0x1FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_5D_L,0x0FFF,0x0000);
        }
        else if(E_ON == bMode)
        {
			write2ByteMask(REG_SC_Ali_BK0F_57_L, 0x0001, 0x0001);
			write2ByteMask(REG_SC_Ali_BK0F_58_L, 0x1FFF, 0x13FF);
			write2ByteMask(REG_SC_Ali_BK0F_59_L, 0x0FFF, 0x03FF);
			write2ByteMask(REG_SC_Ali_BK0F_5A_L, 0x1FFF, 0x13FF);
			write2ByteMask(REG_SC_Ali_BK0F_5B_L, 0x0FFF, 0x03FF);
			write2ByteMask(REG_SC_Ali_BK0F_5C_L, 0x1FFF, 0x13FF);
			write2ByteMask(REG_SC_Ali_BK0F_5D_L, 0x0FFF, 0x03FF);
        }
        else
            return 0;
    }
    else if(E_SUB_WINDOW == u8IpType) //sub window
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK0F_57_L,0x0010,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_5E_L,0x1FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_5F_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_60_L,0x1FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_61_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_62_L,0x1FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_63_L,0x0FFF,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK0F_57_L,0x0010,0x0010);
			write2ByteMask(REG_SC_Ali_BK0F_5E_L, 0x1FFF, 0x13FF);
			write2ByteMask(REG_SC_Ali_BK0F_5F_L, 0x0FFF, 0x03FF);
			write2ByteMask(REG_SC_Ali_BK0F_60_L, 0x1FFF, 0x13FF);
			write2ByteMask(REG_SC_Ali_BK0F_61_L, 0x0FFF, 0x03FF);
			write2ByteMask(REG_SC_Ali_BK0F_62_L, 0x1FFF, 0x13FF);
			write2ByteMask(REG_SC_Ali_BK0F_63_L, 0x0FFF, 0x03FF);
        }
        else
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}

MS_U8 linearRgbDeGamma(MS_BOOL bMode, MS_U8 u8IpType)
{

    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0001,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0001,0x0001);
        }
        else
            return 0;
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0001,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0001,0x0001);
        }
        else
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}
MS_U8 linearRgbDeGammaDither(MS_BOOL bMode, MS_U8 u8IpType)
{


    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x1000,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x1000,0x1000);
        }
        else
        {
            return 0;
        }
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x1000,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x1000,0x1000);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    return 1;
}
MS_U8 linearRgbComp(MS_BOOL bMode, MS_U8 u8IpType)
{



    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x8600,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_6C_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK0F_6D_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_40_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_41_L,0xFF7F,0x8000);//
            write2ByteMask(REG_SC_Ali_BK25_44_L,0xFF7F,0x8000);//
            write2ByteMask(REG_SC_Ali_BK25_45_L,0xFF7F,0x8000);//
            write2ByteMask(REG_SC_Ali_BK25_48_L,0xF3FF,0x0000);//
            write2ByteMask(REG_SC_Ali_BK25_4A_L,0x0F07,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_4B_L,0x1FFF,0x8000);
        }
        else
        {
            return 0;
        }
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x8600,0x0000);
            write2ByteMask(REG_SC_Ali_BK0F_6E_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK0F_6F_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_42_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_43_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_46_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_47_L,0xFF7F,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_49_L,0xF3FF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_4C_L,0x0F07,0x8000);
            write2ByteMask(REG_SC_Ali_BK25_4D_L,0x1FFF,0x8000);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    return 1;
}

MS_U8 linearRgbCompDither(MS_BOOL bMode, MS_U8 u8IpType)
{

    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0400,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0400,0x0400);
        }
        else
            return 0;
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0400,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0400,0x0400);
        }
        else
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}
MS_U8 linearRgbClip(MS_BOOL bMode, MS_U8 u8IpType)
{


    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0008,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_0B_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_0C_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_0D_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_0E_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_0F_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_10_L,0x0FFF,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0008,0x0008);
            write2ByteMask(REG_SC_Ali_BK25_0B_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_0C_L,0x0FFF,0x0FFF);
            write2ByteMask(REG_SC_Ali_BK25_0D_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_0E_L,0x0FFF,0x0FFF);
            write2ByteMask(REG_SC_Ali_BK25_0F_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_10_L,0x0FFF,0x0FFF);
        }
        else
            return 0;
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0008,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_1B_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_1C_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_1D_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_1E_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_1F_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_20_L,0x0FFF,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0008,0x0008);
            write2ByteMask(REG_SC_Ali_BK25_1B_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_1C_L,0x0FFF,0x0FFF);
            write2ByteMask(REG_SC_Ali_BK25_1D_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_1E_L,0x0FFF,0x0FFF);
            write2ByteMask(REG_SC_Ali_BK25_1F_L,0x0FFF,0x0000);
            write2ByteMask(REG_SC_Ali_BK25_20_L,0x0FFF,0x0FFF);
        }
        else
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}

MS_U8 linearRgb3x3(MS_BOOL bMode, MS_U8 u8IpType)
{

    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0002,0x0000);

        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0002,0x0002);

        }
        else
            return 0;
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0002,0x0000);

        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0002,0x0002);

        }
        else
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}

MS_U8 linearRgbGamma(MS_BOOL bMode, MS_U8 u8IpType)
{

    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0004,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0004,0x0004);
        }
        else
            return 0;
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0004,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0004,0x0004);
        }
        else
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}
MS_U8 linearRgbGammaDither(MS_BOOL bMode, MS_U8 u8IpType)
{


    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0800,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0800,0x0800);
        }
        else
            return 0;
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0800,0x0000);
        }
        else if(E_ON == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0800,0x0800);
        }
        else
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}

MS_U8 linearRgbBypass(MS_BOOL bMode, MS_U8 u8IpType)
{


    if(E_MAIN_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_01_L,0x0180,0x0080);
        }
        else if(E_ON == bMode)
        {
			write2ByteMask(REG_SC_Ali_BK25_01_L, 0x0180, 0x0100);
        }
        else
            return 0;
    }
    else if(E_SUB_WINDOW == u8IpType)
    {
        if(E_OFF == bMode)
        {
            write2ByteMask(REG_SC_Ali_BK25_11_L,0x0180,0x0080);
        }
        else if(E_ON == bMode)
        {
			write2ByteMask(REG_SC_Ali_BK25_11_L, 0x0180, 0x0100);
        }
        else
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}







MS_U8 colorMapVipDisable()
{
    //vip csc
    write2ByteMask(REG_SC_Ali_BK18_06_L,0x0001,0x0080);

    //pre y offset
    write2ByteMask(REG_SC_Ali_BK1A_0F_L,0x00FF,0x0000);
    write2ByteMask(REG_SC_Ali_BK1A_0E_L,0x0003,0x0000);

    //pre y gain
    write2ByteMask(REG_SC_Ali_BK1A_16_L,0x007F,0x0040);
    write2ByteMask(REG_SC_Ali_BK1A_02_L,0x00F0,0x0000);

    //PRE Y GAIN DITHER
    write2ByteMask(REG_SC_Ali_BK1A_04_L,0x0800,0x0000);

    //POST Y OFFSET
    write2ByteMask(REG_SC_Ali_BK1A_18_L,0x00FF,0x0000);
    write2ByteMask(REG_SC_Ali_BK1A_17_L,0x0003,0x0000);

    //POST Y GAIN
    write2ByteMask(REG_SC_Ali_BK1A_14_L,0x007F,0x0040);
    write2ByteMask(REG_SC_Ali_BK1A_02_L,0x000F,0x0000);

    //POST Y OFFSET 2
    write2ByteMask(REG_SC_Ali_BK1A_19_L,0x00FF,0x0000);
    write2ByteMask(REG_SC_Ali_BK1A_17_L,0x000C,0x0000);

    //YCBCR CLIP
    write2ByteMask(REG_SC_Ali_BK1B_70_L,0x0001,0x0000);
    write2ByteMask(REG_SC_Ali_BK1B_72_L,0x03FF,0x03FF);
    write2ByteMask(REG_SC_Ali_BK1B_73_L,0x03FF,0x03FF);
    write2ByteMask(REG_SC_Ali_BK1B_74_L,0x03FF,0x03FF);
    write2ByteMask(REG_SC_Ali_BK1B_75_L,0x03FF,0x03FF);
    write2ByteMask(REG_SC_Ali_BK1B_76_L,0x03FF,0x03FF);
    write2ByteMask(REG_SC_Ali_BK1B_77_L,0x03FF,0x03FF);

    return 1;
}


void HdrEnableW(MS_BOOL bIsOn)
{
    if(0 == bIsOn)
    {
        writeByteMask(_PK_H_(0x79,0x06),0x80,0x80);
        writeByteMask(_PK_L_(0x79,0x07),0xC3,0x00);
        writeByteMask(_PK_H_(0x79,0x07),0x8C,0x00);
        //write2ByteMask(_PK_L_(0x79,0x08),0x1FFF,0x00);
        //write2ByteMask(_PK_L_(0x79,0x09),0x0FFF,0x00);
        writeByteMask(_PK_H_(0x79,0x03),0xC0,0x00);
        writeByteMask(_PK_H_(0x79,0x04),0x9F,0x05);
    }
    else
    {
        writeByteMask(_PK_H_(0x79,0x06),0x80,0x00);
        writeByteMask(_PK_L_(0x79,0x07),0xC3,0x22);
        writeByteMask(_PK_H_(0x79,0x07),0x8C,0x80);
        //write2ByteMask(_PK_L_(0x79,0x08),0x1FFF,0x00);
        //write2ByteMask(_PK_L_(0x79,0x09),0x0FFF,0x00);
        writeByteMask(_PK_H_(0x79,0x03),0xC0,0x00);
        writeByteMask(_PK_H_(0x79,0x04),0x9F,0x05);
    }

}
MS_U8 HdrEnable(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode)
{
    if(0 == bWriteRegister)
        return 1;
    if(1 == bIsOn)
        bIsOn = u8Mode;
    HdrEnableW(bIsOn);
    return 1;
}
void HdrB01EnW(MS_BOOL bIsOn)
{
    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x01),0x01,0x01);
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x01),0x01,0x00);
    }
}
MS_U8 HdrB01En(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode)
{
    if(0 == bWriteRegister)
        return 1;
    if(1 == bIsOn)
        bIsOn = u8Mode;
    HdrB01EnW(bIsOn);
    return 1;
}
void HdrCupB0101W(MS_BOOL bIsOn, MS_BOOL b420RepEn, MS_BOOL bCup420RepEn, MS_BOOL bCup420RepEnVs, MS_BOOL b422to444En)
{
    if(0 == bIsOn)
    {
        writeByteMask(_PK_H_(0x7A,0x01),0xF0,0x00);
    }
    else
    {
        writeByteMask(_PK_L_(0x79,0x07),0x10, (b420RepEn<<4));
        if(1 == b422to444En)
        {
            writeByteMask(_PK_H_(0x7A,0x01),0xE0, (b422to444En<<7));
        }
        else
        {
            writeByteMask(_PK_H_(0x7A,0x01),0xE0, 0x00);
        }
    }
}

MS_U8 HdrCupB0101(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_CUP* Struct)
{
    MS_BOOL b420RepEn,  bCup420RepEn,  bCup420RepEnVs,  b422to444En;


    switch(u8Mode)
    {
        case 0xFF:
            b420RepEn = Struct->b420RepEn;
            bCup420RepEn = Struct->bCup420RepEn;
            bCup420RepEnVs = Struct->bCup420RepEnVs;
            b422to444En = Struct->b422to444En;
            break;
        case 0: //422
            b420RepEn = 0;
            bCup420RepEn = 0;
            bCup420RepEnVs = 0;
            b422to444En = 1;
            break;
        case 1: //444
            b420RepEn = 1;
            bCup420RepEn = 0;
            bCup420RepEnVs = 0;
            b422to444En = 0;
            break;
        default:
            printk("HdrCupB0101 error! current mode unsupported!\n");
            return 0;
    }

    if(1 == bWriteRegister)
        HdrCupB0101W( bIsOn,  b420RepEn,  bCup420RepEn,  bCup420RepEnVs,  b422to444En);

    return 1;
}
void b107Write2RegisterW(MS_BOOL bIsOn, MS_U16 u16ChromaWeight)
{
    if(0 == bIsOn)
    {
        write2Byte(_PK_L_(0x7B,0x3F),0x0000); //u16ChromaWeight bypass
        //    writeByteMask(_PK_L_(0x7A,0x39),0x3F,0x10); //u8TmoInGain bypass
    }
    else
    {
        write2Byte(_PK_L_(0x7B,0x3F),u16ChromaWeight); //u16ChromaWeight
        //    writeByteMask(_PK_L_(0x7A,0x39),0x3F,u8TmoInGain); //u8TmoInGain
    }
}

MS_U8 b107Write2Register(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_B107* Struct)
{
    MS_U16 u16ChromaWeight;
    switch(u8Mode)
    {
        case 0xFF:
            u16ChromaWeight = Struct->u16ChromaWeight;
            break;
        case 0:
            u16ChromaWeight = 0;
            break;
        default:
            printk("b107Write2Register error! current mode unsupported!\n");
            return 0;
    }

    if(1 == bWriteRegister)
        b107Write2RegisterW( bIsOn,  u16ChromaWeight);

    return 1;
}


void NlmWrite2RegisterW(MS_BOOL bIsOn, MS_U8 u8yAvgGain, MS_U8 u8yAvgOffset,  MS_U8 u8NlmLut[32])
{
    MS_U8 i;
    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x01,0x01); //reg_nlm_byp_en
        writeByteMask(_PK_L_(0x7A,0x30),0x3F,0x20);
        writeByteMask(_PK_L_(0x7A,0x31),0x0F,0x00);
        for (i = 0; i <16; i++)
        {
            writeByteMask(_PK_L_(0x7A,(0x20+i)),0x3F,0x20);
            writeByteMask(_PK_H_(0x7A,(0x20+i)),0x3F,0x20);
        }
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x01,0x00); //reg_nlm_byp_en

        writeByteMask(_PK_L_(0x7A,0x30),0x3F,u8yAvgGain);
        writeByteMask(_PK_L_(0x7A,0x31),0x0F,u8yAvgOffset);
        for (i = 0; i <16; i++)
        {
            writeByteMask(_PK_L_(0x7A,(0x20+i)),0x3F,u8NlmLut[i*2]);
            writeByteMask(_PK_H_(0x7A,(0x20+i)),0x3F,u8NlmLut[i*2+1]);
        }
    }
}

MS_U8 NlmWrite2Register(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_NLM* Struct)
{
    MS_U8 u8yAvgGain;
    MS_U8 u8yAvgOffset;
    MS_U8 u8NlmLut[32];
    MS_U8 u8i;

    switch(u8Mode)
    {
        case 0xFF:
            u8yAvgGain = Struct->u8yAvgGain;
            u8yAvgOffset = Struct->u8yAvgOffset;
            for(u8i = 0; u8i < 32; u8i++)
            {
                u8NlmLut[u8i] = Struct->u8NlmLut[u8i];
            }
            break;
        case 0:
            u8yAvgGain = 0x20;
            u8yAvgOffset = 0x20;
            u8NlmLut[0] = 0x20;
            u8NlmLut[1] = 0x1C;
            u8NlmLut[2] = 0x18;
            u8NlmLut[3] = 0x15;
            u8NlmLut[4] = 0x13;
            u8NlmLut[5] = 0x11;
            u8NlmLut[6] = 0x0F;
            u8NlmLut[7] = 0xD;
            u8NlmLut[8] = 0xB;
            u8NlmLut[9] = 0xA;
            u8NlmLut[10] = 0x9;
            u8NlmLut[11] = 0x8;
            u8NlmLut[12] = 0x7;
            u8NlmLut[13] = 0x6;
            u8NlmLut[14] = 0x5;
            u8NlmLut[15] = 0x4;
            u8NlmLut[16] = 0x4;
            u8NlmLut[17] = 0x3;
            u8NlmLut[18] = 0x3;
            u8NlmLut[19] = 0x2;
            u8NlmLut[20] = 0x2;
            u8NlmLut[21] = 0x2;
            u8NlmLut[22] = 0x2;
            u8NlmLut[23] = 0x1;
            u8NlmLut[24] = 0x1;
            u8NlmLut[25] = 0x1;
            u8NlmLut[26] = 0x1;
            u8NlmLut[27] = 0x0;
            u8NlmLut[28] = 0x0;
            u8NlmLut[29] = 0x0;
            u8NlmLut[30] = 0x0;
            u8NlmLut[31] = 0x0;
            break;
        case 1:
            u8yAvgGain = 0x20;
            u8yAvgOffset = 0x20;
            u8NlmLut[0] = 0x20;
            u8NlmLut[1] = 0x19;
            u8NlmLut[2] = 0x14;
            u8NlmLut[3] = 0x10;
            u8NlmLut[4] = 0xC;
            u8NlmLut[5] = 0xA;
            u8NlmLut[6] = 0x8;
            u8NlmLut[7] = 0x6;
            u8NlmLut[8] = 0x5;
            u8NlmLut[9] = 0x4;
            u8NlmLut[10] = 0x3;
            u8NlmLut[11] = 0x3;
            u8NlmLut[12] = 0x2;
            u8NlmLut[13] = 0x2;
            u8NlmLut[14] = 0x1;
            u8NlmLut[15] = 0x1;
            u8NlmLut[16] = 0x1;
            u8NlmLut[17] = 0x1;
            u8NlmLut[18] = 0x1;
            u8NlmLut[19] = 0x1;
            u8NlmLut[20] = 0x1;
            u8NlmLut[21] = 0x1;
            u8NlmLut[22] = 0x1;
            u8NlmLut[23] = 0x1;
            u8NlmLut[24] = 0x1;
            u8NlmLut[25] = 0x1;
            u8NlmLut[26] = 0x1;
            u8NlmLut[27] = 0x1;
            u8NlmLut[28] = 0x1;
            u8NlmLut[29] = 0x1;
            u8NlmLut[30] = 0x1;
            u8NlmLut[31] = 0x1;
            break;
        case 2:
            u8yAvgGain = 0x20;
            u8yAvgOffset = 0x20;
            u8NlmLut[0] = 0x20;
            u8NlmLut[1] = 0x20;
            u8NlmLut[2] = 0x20;
            u8NlmLut[3] = 0x20;
            u8NlmLut[4] = 0x20;
            u8NlmLut[5] = 0x20;
            u8NlmLut[6] = 0x20;
            u8NlmLut[7] = 0x20;
            u8NlmLut[8] = 0x20;
            u8NlmLut[9] = 0x20;
            u8NlmLut[10] =0x20;
            u8NlmLut[11] =0x20;
            u8NlmLut[12] =0x20;
            u8NlmLut[13] =0x20;
            u8NlmLut[14] =0x20;
            u8NlmLut[15] =0x20;
            u8NlmLut[16] =0x20;
            u8NlmLut[17] =0x20;
            u8NlmLut[18] =0x20;
            u8NlmLut[19] =0x20;
            u8NlmLut[20] =0x20;
            u8NlmLut[21] =0x20;
            u8NlmLut[22] =0x20;
            u8NlmLut[23] =0x20;
            u8NlmLut[24] =0x20;
            u8NlmLut[25] =0x20;
            u8NlmLut[26] =0x20;
            u8NlmLut[27] =0x20;
            u8NlmLut[28] =0x20;
            u8NlmLut[29] =0x20;
            u8NlmLut[30] =0x20;
            u8NlmLut[31] =0x20;
            break;
        default:
            printk("NlmWrite2Register error! current mode unsupported!\n");
            return 0;
    }
    if(1 == bWriteRegister)
        NlmWrite2RegisterW( bIsOn,  u8yAvgGain,  u8yAvgOffset,   u8NlmLut);

    return 1;

}
void HdrAcGainWrite2RegisterW(MS_BOOL bIsOn, MS_U8 u8uGain, MS_U16 u16max, MS_U16 u16min,  MS_U32 u8acGainLut[24])
{
    MS_U8 i;
    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x02,0x02); //reg_acgain_byp_en
        /*writeByteMask(_PK_L_(0x7A,0x30),0x3F,0x20);
        writeByteMask(_PK_H_(0x7A,0x30),0x3F,0x20);
        for (i = 0; i <32; i+2)
        {
            writeByteMask(_PK_L_(0x7A,0x20+i),0x3F,0x20);
            writeByteMask(_PK_H_(0x7A,0x20+i+1),0x3F,0x20);
        }*/
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x02,0x00); //reg_acgain_byp_en

        writeByteMask(_PK_L_(0x7A,0x03),0xFF,u8uGain);

        write2Byte(_PK_L_(0x7A,0x04),u16max);
        write2Byte(_PK_L_(0x7A,0x05),u16min);
        for (i = 0; i <12; i++)
        {
            write2Byte(_PK_L_(0x7A,(0x06+i)),u8acGainLut[i*2]);
            write2Byte(_PK_H_(0x7A,(0x06+i)),u8acGainLut[i*2+1]);

        }
    }
}

MS_U8 HdrAcGainWrite2Register(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_AC_GAIN* Struct)
{

    MS_U8 u8uGain;
    MS_U16 u16max;
    MS_U16 u16min;
    MS_U32 u8acGainLut[24];
    MS_U8 u8i;
    switch(u8Mode)
    {
        case 0xFF:
            u8uGain = Struct->u8uGain;
            u16max = Struct->u16max;
            u16min = Struct->u16min;
            for(u8i = 0; u8i < 24; u8i++)
            {
                u8acGainLut[u8i] = Struct->u8acGainLut[u8i];
            }
            break;
        case 0:
            u8uGain = 0x20;
            u16max = 0xFFFF;
            u16min = 0x8000;
            for(u8i = 0; u8i < 24; u8i++)
            {
                u8acGainLut[u8i] = 0;
            }
            break;
        default:
            printk("HdrAcGainWrite2Register error! current mode unsupported!\n");
            return 0;
    }

    if(1 == bWriteRegister)
        HdrAcGainWrite2RegisterW( bIsOn,  u8uGain,  u16max,  u16min,   u8acGainLut);

    return 1;
}
void HdrTmoSettingsW(MS_BOOL bIsOn,MS_BOOL bCiDitherEn,MS_BOOL bBiDitherEn, MS_U8 u8TmoInGain, MS_U8 u8TmoOutGain, MS_U8 u8UserAlpha)
{
    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x04,0x04); //reg_b02_byp_en
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x04,0x00); //reg_b02_byp_en
        writeByteMask(_PK_L_(0x7A,0x38),0x01,bCiDitherEn);
        writeByteMask(_PK_L_(0x7A,0x38),0x02,bBiDitherEn<<1);

        writeByteMask(_PK_L_(0x7A,0x39),0x3F,u8TmoInGain);
        writeByteMask(_PK_H_(0x7A,0x39),0x3F,u8TmoOutGain);
        writeByteMask(_PK_L_(0x7A,0x3A),0x1F,u8UserAlpha);
    }

}
MS_U8 HdrTmoSettings(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_TMO_SETTINGS* Struct)
{

    MS_BOOL bCiDitherEn;
    MS_BOOL bBiDitherEn;
    MS_U8 u8TmoInGain;
    MS_U8 u8TmoOutGain;
    MS_U8 u8UserAlpha;
    switch(u8Mode)
    {
        case 0xFF:
            bCiDitherEn = Struct->bCiDitherEn;
            bBiDitherEn = Struct->bBiDitherEn;
            u8TmoInGain = Struct->u8TmoInGain;
            u8TmoOutGain = Struct->u8TmoOutGain;
            u8UserAlpha = Struct->u8UserAlpha;
            break;
        case 0:
            bCiDitherEn = 0;
            bBiDitherEn = 0;
            u8TmoInGain = 0x10;
            u8TmoOutGain = 0x10;
            u8UserAlpha = 0x10;
            break;
        case 1:
            bCiDitherEn = 1;
            bBiDitherEn = 0;
            u8TmoInGain = 0x10;
            u8TmoOutGain = 0x10;
            u8UserAlpha = 0x7;
            break;
        case 2:
            bCiDitherEn = 1;
            bBiDitherEn = 0;
            u8TmoInGain = 0x10;
            u8TmoOutGain = 0x10;
            u8UserAlpha = 0x10;
            break;
        default:
            printk("HdrTmoSettings error! current mode unsupported!\n");
            return 0;
    }
    if(1 == bWriteRegister)
        HdrTmoSettingsW( bIsOn, bCiDitherEn, bBiDitherEn,  u8TmoInGain,  u8TmoOutGain,  u8UserAlpha);
    return 1;

}


void HdrAceWrite2RegisterW(MS_BOOL bIsOn,MS_BOOL bDitherEn, MS_U16 u16WeightEdge, MS_U16 u16Weight)
{

    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x08,0x08); //reg_b03_byp_en

        writeByteMask(_PK_H_(0x7A,0x3d),0x80,0);
        write2ByteMask(_PK_L_(0x7A,0x3b),0x0FFF,0);
        write2ByteMask(_PK_L_(0x7A,0x3c),0x0FFF,0);
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x08,0x00); //reg_b03_en

        writeByteMask(_PK_H_(0x7A,0x3d),0x80,bDitherEn<<7);
        write2Byte(_PK_L_(0x7A,0x3b),u16WeightEdge);
        write2ByteMask(_PK_L_(0x7A,0x3c),0x0FFF,u16Weight);

    }
}

MS_U8 HdrAceWrite2Register(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_ACE* Struct)
{

    MS_BOOL bDitherEn;
    MS_U16 u16WeightEdge;
    MS_U16 u16Weight;
    switch(u8Mode)
    {
        case 0xFF:
            bDitherEn = Struct->bDitherEn;
            u16WeightEdge = Struct->u16WeightEdge;
            u16Weight = Struct->u16Weight;
            break;
        case 0:
            bDitherEn = 0;
            u16WeightEdge = 0x3FFC;
            u16Weight = 0;
            break;
        case 1:
            bDitherEn = 0;
            u16WeightEdge = 0x3FFC;
            u16Weight = 0;
            break;
        default:
            printk("HdrAceWrite2Register error! current mode unsupported!\n");
            return 0;
    }
    if(1 == bWriteRegister)
        HdrAceWrite2RegisterW( bIsOn, bDitherEn,  u16WeightEdge,  u16Weight);
    return 1;
}
void HdrUvcWrite2RegisterW(MS_BOOL bIsOn,MS_U16 u16HuntGain, MS_U16 u16HuntOffset,MS_U16 u16SatGain,MS_U32 u32HkHightThreshold, MS_U32 u32HklowThreshold, MS_U32 u32HkZeroThreshold, MS_BOOL bOutClampEn, MS_U16 u16UvOffset,MS_BOOL bNormalizeEn, MS_U16 u16PostOffset, MS_U16 u16iMaxC)
{
    if(0 == bIsOn) // check the setting, make sure they are bypass settings
    {
#if 0
        write2ByteMask(_PK_L_(0x7A,0x40),0x0FFF,0x0800); //reg_b04_hunt_gain
        write2ByteMask(_PK_L_(0x7A,0x41),0x0FFF,0x0FFF); //reg_b04_hunt_offset
        write2ByteMask(_PK_L_(0x7A,0x42),0xFFFF,0x1000); //reg_b04_sat_gain

        write2Byte(_PK_L_(0x7A,0x43),0xFFFF); //reg_b04_hk_high_th
        write2ByteMask(_PK_L_(0x7A,0x44),0x00FF,0x0000); //reg_b04_hk_high_th

        write2Byte(_PK_L_(0x7A,0x45),0x4000); //reg_b04_hk_low_th
        write2ByteMask(_PK_L_(0x7A,0x46),0x00FF,0x0000); //reg_b04_hk_low_th

        write2Byte(_PK_L_(0x7A,0x48),0x7FFF); //reg_b04_hk_0_th
        write2ByteMask(_PK_L_(0x7A,0x49),0x00FF,0x0000); //reg_b04_hk_0_th

        writeByteMask(_PK_H_(0x7a,0x4a),0x80,0x00); //reg_b04_out_clamp_en
        write2ByteMask(_PK_L_(0x7a,0x4a),0x3FFF,0x0000)//reg_b04_uv_offs_0
#endif
        //control by Qmap 20160303
        //write2ByteMask(_PK_L_(0x7A,0x40),0x0FFF,0x0000); //reg_b04_hunt_gain
        //write2ByteMask(_PK_L_(0x7A,0x41),0x0FFF,0x0800); //reg_b04_hunt_offset
        //write2ByteMask(_PK_L_(0x7A,0x42),0xFFFF,0x1000); //reg_b04_sat_gain

        //write2Byte(_PK_L_(0x7A,0x43),0xFFFF); //reg_b04_hk_high_th
        //write2ByteMask(_PK_L_(0x7A,0x44),0x000F,0x0000); //reg_b04_hk_high_th

        //write2Byte(_PK_L_(0x7A,0x45),0x0000); //reg_b04_hk_low_th
        //write2ByteMask(_PK_L_(0x7A,0x46),0x00FF,0x0000); //reg_b04_hk_low_th

        write2Byte(_PK_L_(0x7A,0x48),0x0000); //reg_b04_hk_0_th
        write2ByteMask(_PK_L_(0x7A,0x49),0x00FF,0x0000); //reg_b04_hk_0_th

        writeByteMask(_PK_H_(0x7a,0x4a),0x80,0x00); //reg_b04_out_clamp_en
        write2ByteMask(_PK_L_(0x7a,0x4a),0x3FFF,0x3800);//reg_b04_uv_offs_0

        writeByteMask(_PK_L_(0x7A,0x58),0x20,0x20); //reg_b05_norm_en
        write2ByteMask(_PK_L_(0x7A,0x59),0x1FFF,0x0800); //reg_b05_uv_post_ofs

        //write2Byte(_PK_L_(0x7A,0x50),0x3FFF); //u16iMaxC
    }
    else
    {
        //write2ByteMask(_PK_L_(0x7A,0x40),0x0FFF,u16HuntGain); //reg_b04_hunt_gain
        //write2ByteMask(_PK_L_(0x7A,0x41),0x0FFF,u16HuntOffset); //reg_b04_hunt_offset
        //write2ByteMask(_PK_L_(0x7A,0x42),0xFFFF,u16SatGain); //reg_b04_sat_gain

        //write2Byte(_PK_L_(0x7A,0x43),u32HkHightThreshold&0x0000FFFF); //reg_b04_hk_high_th
        //write2ByteMask(_PK_L_(0x7A,0x44),0x00FF,u32HkHightThreshold>>16); //reg_b04_hk_high_th

        //write2Byte(_PK_L_(0x7A,0x45),u32HklowThreshold&0x0000FFFF); //reg_b04_hk_low_th
        //write2ByteMask(_PK_L_(0x7A,0x46),0x00FF,u32HklowThreshold>>16); //reg_b04_hk_low_th

        write2Byte(_PK_L_(0x7A,0x48),u32HkZeroThreshold&0x0000FFFF); //reg_b04_hk_0_th
        write2ByteMask(_PK_L_(0x7A,0x49),0x00FF,u32HkZeroThreshold>>16); //reg_b04_hk_0_th

        writeByteMask(_PK_H_(0x7a,0x4a),0x80,bOutClampEn<<7); //reg_b04_out_clamp_en
        write2ByteMask(_PK_L_(0x7a,0x4a),0x3FFF,u16UvOffset);//reg_b04_uv_offs_0

        writeByteMask(_PK_L_(0x7A,0x58),0x20,bNormalizeEn<<5); //reg_b05_norm_en
        write2ByteMask(_PK_L_(0x7A,0x59),0x1FFF,u16PostOffset); //reg_b05_uv_post_ofs

        //write2Byte(_PK_L_(0x7A,0x50),u16iMaxC);

    }
}
MS_U8 HdrUvcWrite2Register(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_UVC* Struct)
{

    MS_U16 u16HuntGain;
    MS_U16 u16HuntOffset;
    MS_U16 u16SatGain;
    MS_U32 u32HkHightThreshold;
    MS_U32 u32HklowThreshold;
    MS_U32 u32HkZeroThreshold;
    MS_BOOL bOutClampEn;
    MS_U16 u16UvOffset;

    MS_BOOL bNormalizeEn;
    MS_U16 u16PostOffset;

    MS_U16 u16iMaxC;
    MS_U16 u16iMinC;

    switch(u8Mode)
    {
        case 0xFF:
            u16HuntGain = Struct->u16HuntGain;
            u16HuntOffset = Struct->u16HuntOffset;
            u16SatGain = Struct->u16SatGain;
            u32HkHightThreshold = Struct->u32HkHightThreshold;
            u32HklowThreshold = Struct->u32HklowThreshold;
            u32HkZeroThreshold = Struct->u32HkZeroThreshold;
            bOutClampEn = Struct->bOutClampEn;
            u16UvOffset = Struct->u16UvOffset;
            bNormalizeEn = Struct->bNormalizeEn;
            u16PostOffset = Struct->u16PostOffset;

            u16iMaxC = Struct->u16iMaxC;
            break;
        case 0:
            u16HuntGain = 0x800;
            u16HuntOffset = 0xFFF;
            u16SatGain = 0x1000;
            u32HkHightThreshold = 0xFFFF;
            u32HklowThreshold = 0x4000;
            u32HkZeroThreshold = 0x7FFF;
            bOutClampEn = 1;
            u16UvOffset = 0x3800;

            bNormalizeEn = 1;
            u16PostOffset = 0x800;

            u16iMaxC = 0x30C6;
            break;
        case 1:
            u16HuntGain = 0x800;
            u16HuntOffset = 0xFFF;
            u16SatGain = 0x1000;
            u32HkHightThreshold = 0xFFFFF;
            u32HklowThreshold = 0x0000;
            u32HkZeroThreshold = 0x7FFF;
            bOutClampEn = 0;
            u16UvOffset = 0x3800;

            bNormalizeEn = 1;
            u16PostOffset = 0x800;

            u16iMaxC = 0x3FFF;
            break;
    case 2:
        u16HuntGain = 0x800;
        u16HuntOffset = 0xFFF;
        u16SatGain = 0x1000;
        u32HkHightThreshold = 0x8000;
        u32HklowThreshold = 0x8000;
        u32HkZeroThreshold = 0x8000;
        bOutClampEn = 0;
	u16UvOffset = 0x3800;
	bNormalizeEn = 1;
	u16PostOffset = 0x800;
        break;
        default:
            printk("HdrUvcWrite2Register error! current mode unsupported!\n");
            return 0;
    }
    if(1 == bWriteRegister)
        HdrUvcWrite2RegisterW( bIsOn, u16HuntGain,  u16HuntOffset, u16SatGain, u32HkHightThreshold,  u32HklowThreshold,  u32HkZeroThreshold,  bOutClampEn,  u16UvOffset, bNormalizeEn,  u16PostOffset,  u16iMaxC);
    return 1;

}
void hdrDither1_b501W(MS_BOOL bIsOn,MS_BOOL bDitherMode/*,MS_BOOL bNormalizeEn, MS_U16 u16PostOffset*/)
{
    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x58),0x10,0x00); //reg_b05_dith_md
        /*writeByteMask(_PK_L_(0x7A,0x58),0x20,0x20); //reg_b05_norm_en
        write2ByteMask(_PK_L_(0x7A,0x59),0x1FFF,0x0800); //reg_b05_uv_post_ofs*/
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x58),0x10,bDitherMode<<4); //reg_b05_dith_md
        /*writeByteMask(_PK_L_(0x7A,0x58),0x20,bNormalizeEn<<5); //reg_b05_norm_en
        write2ByteMask(_PK_L_(0x7A,0x59),0x1FFF,u16PostOffset); //reg_b05_uv_post_ofs*/
    }
}

MS_U8 hdrDither1_b501(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_DITHER1* Struct)
{
    MS_BOOL bDitherMode;
    /*MS_BOOL bNormalizeEn;
    MS_U16 u16PostOffset;*/
    switch(u8Mode)
    {
        case 0xFF:
            bDitherMode = Struct->bDitherMode;
            /*bNormalizeEn = Struct->bNormalizeEn;
            u16PostOffset = Struct->u16PostOffset;*/
            break;
        case 0:
            bDitherMode = 0;
            /*bNormalizeEn = 1;
            u16PostOffset = 0;*/
            break;
        case 1:
            bDitherMode = 0;
            /*bNormalizeEn = 1;
            u16PostOffset = 0;*/
            break;
        default:
            printk("hdrDither1_b501 error! current mode unsupported!\n");
            return 0;
    }
    if(1 == bWriteRegister)
        hdrDither1_b501W( bIsOn, bDitherMode/*, bNormalizeEn,  u16PostOffset*/);
    return 1;
}
void hdr3dLutSettings_b502W(MS_BOOL bIsOn, MS_U16 u16iMaxC, MS_U16 u16iMinC, MS_U32 u32iDistiCInv, MS_U16 u16iMinY, MS_U16 u16iMaxY, MS_U32 u32iDistiYInv, MS_BOOL bAlphaRound, MS_U8 u8OutControl, MS_BOOL bOutControlRound, MS_BOOL bLutBlendMode, MS_BOOL b3dLutDitherMode)
{
    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x10,0x10); //reg_b06_byp_en
        write2Byte(_PK_L_(0x7A,0x50),u16iMaxC);
        write2Byte(_PK_L_(0x7A,0x51),u16iMinC);
        write4Bytes(_PK_L_(0x7A,0x52),u32iDistiCInv);
        write2Byte(_PK_L_(0x7A,0x54),u16iMinY);
        write2Byte(_PK_L_(0x7A,0x55),u16iMaxY);
        write4Bytes(_PK_L_(0x7A,0x56),u32iDistiYInv);
        writeByteMask(_PK_L_(0x7A,0x58),0x01,bAlphaRound);
        writeByteMask(_PK_L_(0x7A,0x58),0x06,u8OutControl<<1);
        writeByteMask(_PK_L_(0x7A,0x58),0x08,bOutControlRound<<3);
        writeByteMask(_PK_H_(0x7A,0x58),0x08,bLutBlendMode<<3);
        writeByteMask(_PK_H_(0x7A,0x58),0x10,b3dLutDitherMode<<4);
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x10,0x00); //reg_b06_byp_en
        write2Byte(_PK_L_(0x7A,0x50),u16iMaxC);
        //write2Byte(_PK_L_(0x7A,0x51),u16iMinC);
        write4Bytes(_PK_L_(0x7A,0x52),u32iDistiCInv);
        write2Byte(_PK_L_(0x7A,0x54),u16iMinY);
        write2Byte(_PK_L_(0x7A,0x55),u16iMaxY);
        write4Bytes(_PK_L_(0x7A,0x56),u32iDistiYInv);
        writeByteMask(_PK_L_(0x7A,0x58),0x01,bAlphaRound);
        writeByteMask(_PK_L_(0x7A,0x58),0x06,u8OutControl<<1);
        writeByteMask(_PK_L_(0x7A,0x58),0x08,bOutControlRound<<3);
        writeByteMask(_PK_H_(0x7A,0x58),0x08,bLutBlendMode<<3);
        writeByteMask(_PK_H_(0x7A,0x58),0x10,b3dLutDitherMode<<4);
    }
    mD3dLutWrite2Register(u16Full2limit3dLut);
}

MS_U8 hdr3dLutSettings_b502(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_3DLUT_SETTINGS* Struct)
{

    MS_U16 u16iMaxC;
    MS_U16 u16iMinC;
    MS_U32 u32iDistiCInv;
    MS_U16 u16iMinY;
    MS_U16 u16iMaxY;
    MS_U32 u32iDistiYInv;
    MS_BOOL bAlphaRound;
    MS_U8 u8OutControl;
    MS_BOOL bOutControlRound;
    MS_BOOL bLutBlendMode;
    MS_BOOL b3dLutDitherMode;
    switch(u8Mode)
    {
        case 0xFF:
            u16iMaxC = Struct->u16iMaxC;
            u16iMinC = Struct->u16iMinC;
            u32iDistiCInv = Struct->u32iDistiCInv;
            u16iMinY = Struct->u16iMinY;
            u16iMaxY = Struct->u16iMaxY;
            u32iDistiYInv = Struct->u32iDistiYInv;
            bAlphaRound = Struct->bAlphaRound;
            u8OutControl = Struct->u8OutControl;
            bOutControlRound = Struct->bOutControlRound;
            bLutBlendMode = Struct->bLutBlendMode;
            b3dLutDitherMode = Struct->b3dLutDitherMode;
            break;
        case 0:
            u16iMaxC = 0x7FFF;
            u16iMinC = 0x0000;
            u32iDistiCInv = 0x00008000;
            u16iMinY = 0x0000;
            u16iMaxY = 0x7FFF;
            u32iDistiYInv = 0X00008000;
            bAlphaRound = 0;
            u8OutControl = 2;
            bOutControlRound = 0;
            bLutBlendMode = 0;
            b3dLutDitherMode = 0;
            break;
        case 1:
            u16iMaxC = 0x30C6;
            u16iMinC = 0xCF3A;
            u32iDistiCInv = 0x0000A7F5;
            u16iMinY = 0x01EE;
            u16iMaxY = 0x4108;
            u32iDistiYInv = 0X000103A5;
            bAlphaRound = 0;
            u8OutControl = 2;
            bOutControlRound = 0;
            bLutBlendMode = 0;
            b3dLutDitherMode = 0;
            break;
        case 2: //bypass setting
            u16iMaxC = 0x7FFF;
            u16iMinC = 0x0000;
            u32iDistiCInv = 0x00008000;
            u16iMinY = 0x0000;
            u16iMaxY = 0x7FFF;
            u32iDistiYInv = 0X00008000;
            bAlphaRound = 0;
            u8OutControl = 2;
            bOutControlRound = 0;
            bLutBlendMode = 0;
            b3dLutDitherMode = 0;
            break;
        default:
            //printk("hdr3dLutSettings_b502 error! current mode unsupported!\n");
            return 0;
    }
    if(1 == bWriteRegister)
        hdr3dLutSettings_b502W( bIsOn, u16iMaxC,  u16iMinC,  u32iDistiCInv,  u16iMinY,  u16iMaxY,  u32iDistiYInv,  bAlphaRound,  u8OutControl,  bOutControlRound,  bLutBlendMode,  b3dLutDitherMode);
    return 1;

}
U32 deGammaSampling(MS_U32 u32In, MS_U32 u32Ginv, MS_U16 u16A, MS_U16 u16B, MS_U16 u16S,MS_U8 u8BitDepth,MS_U8 sMode)
{
// u16Gamma : 4.15
// u16GInv  : 4.15
// u16A : 1.15
// u16B : 0.16
// u16S : 6.10
    U32 u32Th, u32Tmp, u32Out;

    u32Th = (U32)u16S * u16B >> 10;
    if (u32In <= u32Th)
    {
        u32Out = (( (( (U64)u32In << 13) /( u16S) )<< (10))*( ((1<<u8BitDepth) -1) ))>>(u8BitDepth);
        return  u32Out;
    }
    if (sMode == 0) // normal degamma
    {
        u32Tmp = (((U64)(u32In + ((u16A - 0x8000) << 1)) << 15)+(1<<14)) / u16A;
    }
    else
    {
        // SMPTE 428-1
        u32Tmp = (((U64)(u32In) << 15)+(1<<14)) / u16A;
    }


    u32Out = newPow04(u32Tmp , u32Ginv);
    return ( (u32Out<<max((13 - u8BitDepth),0))*( (1<<u8BitDepth) - 1 )>>abs(min((13 - u8BitDepth),0)) );
//return ( (u32Out<<max((13 - u8BitDepth),0))*( (1<<u8BitDepth) - 1 ) ); //u32Out 2.30  //3.29
}
void NewDeGammaGenerator(U32* u32LutData,ST_TRANSFER_FUNCTION_DATA* pData,MS_U16 max_lum_code,MS_U8 g_u16HdrY2RRatio,MS_U8* status, MS_U8 IpType,MS_U16 u16ArrayLength,MS_U8 u8BitDepth,MS_U8 transferCharacterstics)
{

    MS_U16 u16Val, u16NumEntries, u16LutIdx, u16Index;
    U64 u64Nume, u64Denum;
//MS_U32 u32Val;


//for pq degamma
    MS_U16 u16CurrentRange,u16Adder,u16Counter;
    U32 u32Val;




    if(pData->sValid == FALSE)
    {
        //HDR_DBG_HAL(printk("\n    Kernel      MDrv_HDR_TransferFunctionGen    ERROR   u8ColorPrimaries=%d     \n",g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8ColorPrimaries));
        *status = 0;
        return;
    }

// DeGamma
    if (pData->sMode == 0 || pData->sMode == 1)
    {


        if(E_DEGAMMA_MASERATI_MD == IpType)
        {
            //u16NumEntries = 512;
            for (u16LutIdx = 0; u16LutIdx < u16ArrayLength-1; u16LutIdx++)
            {
                //u16Val 1.16
                u32Val = (((U64)u16LutIdx <<(7+u8BitDepth))/( (1<<u8BitDepth) - 1));
                u32LutData[u16LutIdx] = deGammaSampling(u32Val, pData->u32GammaInv, pData->u16Alpha, pData->u16Beta, pData->u16LowerBoundGain, u8BitDepth,pData->sMode)<<3;

            }
            u32Val = (((U64)u16LutIdx <<(7+u8BitDepth))/( (1<<u8BitDepth) - 1));
            u32LutData[u16LutIdx] = (deGammaSampling(u32Val,pData->u32GammaInv, pData->u16Alpha, pData->u16Beta, pData->u16LowerBoundGain, u8BitDepth,pData->sMode)-u32LutData[u16LutIdx-1])<<3;
        }
        else
        {
            U32 test;
            u16NumEntries = (1 == pData->bExtendEn) ? 600 : 257;
            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {
                //MS_U8 u8BitDepth
                //u32Val = (((U32)u16LutIdx <<18)/1023);
                //u32Val = (((U32)u16LutIdx <<(8+u8BitDepth))/( (2<<u8BitDepth) - 1));

                u32Val = (((U32)u16LutIdx <<(8+u8BitDepth))/( (1<<u8BitDepth) - 1));
                test =deGammaSampling(u32Val,  pData->u32GammaInv, pData->u16Alpha, pData->u16Beta, pData->u16LowerBoundGain, u8BitDepth,pData->sMode);
                u32LutData[u16LutIdx] =  deGammaSampling(u32Val,  pData->u32GammaInv, pData->u16Alpha, pData->u16Beta, pData->u16LowerBoundGain, u8BitDepth,pData->sMode);
            }

            // fill the rest of the table
            for ( ; u16LutIdx < u16ArrayLength; u16LutIdx++)
                u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];
        }


    }
    /*else if (pData->sMode == 1) // DCI-P3
    {
     *status = 0;
     printf("DeGamma ERROR! DeGamma mode (ST 428-1) is currently unsuported, Degamma table has not been updated.\n");
     // todo
     //HDR_DBG_HAL(printk("\n  Kernel Error!! sMode is 1 \n"));
    }*/
    else if (pData->sMode == 2) // PQ
    {
        MS_U16 i,j;
        if(0 == max_lum_code)
        {
            printf("Degamma E rror! max_lum_code == 0, division by zero, Degamma will not be updated\n");
            *status = 0;
            return;
        }
        if(0 == g_u16HdrY2RRatio)
        {
            printf("Degamma E rror! g_u16HdrY2RRatio == 0, division by zero, Degamma will not be updated\n");
            *status = 0;
            return;
        }
        u64Denum = MDrv_HDR_PQ_ali(max_lum_code);
        if(E_DEGAMMA_MASERATI_MD == IpType)
        {
            u16NumEntries = 512;

            u16Adder = 0;
            u16Counter = 0;
            for( i = 0; i < 16; i++)
            {

                MS_U16 u16ResPQ[16] =    {2,2,4,8,16,16,16, 32, 32, 32,  32,  32,  32,  64,  64,   128};
                MS_U16 u16RangePQ[17]  = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};
                for( j = 0; j < u16ResPQ[i]; j++)
                {
                    u32Val = u16Adder;
                    u64Nume = MDrv_HDR_PQ_ali(u32Val);
                    //u32LutData[u16Counter++] = min(u64Nume /** 0xFFF00000ul / u64Denum*/, 0xFFF00000ul);
                    u32LutData[u16Counter++] =min(u64Nume /** 0xFFF00000ul / u64Denum*/, 0xFFFF0000ul);
                    u16CurrentRange = u16RangePQ[i+1] - u16RangePQ[i];
                    u16Adder += ( u16CurrentRange + (u16ResPQ[i]>>1) ) / u16ResPQ[i];

                }
                u32LutData[u16Counter] = u32LutData[u16Counter-1]-u32LutData[u16Counter-2];
            }



        }
        else
        {
            u16NumEntries = (256 * g_u16HdrY2RRatio)>>6;
            u64Denum = MDrv_HDR_PQ_ali(max_lum_code/*pData->tData[0]*/);

            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {

                u32Val = ((U32)u16LutIdx << (8+6) ) / g_u16HdrY2RRatio;


                u64Nume = MDrv_HDR_PQ_ali(u32Val);

                u32LutData[u16LutIdx] = min(u64Nume * 0xFFc00000ul / u64Denum, 0xFFc00000ul)>>3;
                //u32LutData[u16LutIdx] = min(u64Nume * 0xFFFF0000ul / u64Denum, 0xFFFF0000ul)>>2;

                //u32LutData[u16LutIdx] = min(u64Nume * 0xFFFFFul / u64Denum, 0xFFFFFul);

            }

            for ( ; u16LutIdx < u16ArrayLength; u16LutIdx++)
                u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];

        }
    }
    else if (pData->sMode == 4)  // Bt1886
    {
        *status = 0;
        printf("DeGamma E RROR! DeGamma mode (Bt1886) is unsuported, Degamma table has not been updated.\n");
    }
    else// if (mode == 3) // Log
    {
        if(E_DEGAMMA_MASERATI_MD == IpType)
        {
            for (u16LutIdx = 0; u16LutIdx < 512; u16LutIdx++)
            {

                u32LutData[u16LutIdx] =  (E_CFD_CFIO_TR_LOG0 == transferCharacterstics) ? Eotf9LutDeGamma_14bit_31Res[u16LutIdx]<<1 : Eotf10LutDeGamma_14bit_31Res[u16LutIdx]<<1; //31 << 1 == 32
            }
            u32LutData[u16LutIdx] =  (E_CFD_CFIO_TR_LOG0 == transferCharacterstics) ? (Eotf9LutDeGamma_14bit_31Res[u16LutIdx]-Eotf9LutDeGamma_14bit_31Res[u16LutIdx-1])<<1 : (Eotf10LutDeGamma_14bit_31Res[u16LutIdx]-Eotf10LutDeGamma_14bit_31Res[u16LutIdx-1])<<1; //31 << 1 == 32
        }
        else
        {
            u16NumEntries = (1 == pData->bExtendEn) ? 600 : 257;
            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {

                u32LutData[u16LutIdx] =  (E_CFD_CFIO_TR_LOG0 == transferCharacterstics) ? Eotf9LutDeGamma_10bit_31Res[u16LutIdx]>>2 : Eotf10LutDeGamma_10bit_31Res[u16LutIdx]>>2; //31 >> 2 == 29
            }

            // fill the rest of the table
            for ( ; u16LutIdx < u16ArrayLength; u16LutIdx++)
                u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];
        }

    }
}
MS_U8 deGammaHal(MS_U8 IpType,U32* u32LutData,MS_U16 ratio2,MS_U8 lut_en,MS_U8 debugMode,MS_U8 transferCharacterstics)
{
    MS_U8 status = 1;
    if(E_DEGAMMA_MANHATTAN_DELTA_VOP2 == IpType)
    {
        MDrv_HDR_WriteDegammaTable_DeltaMode_600Entries_ali(u32LutData,ratio2,lut_en);
    }
    else if(E_DEGAMMA_MASERATI_NORMAL_VOP2 == IpType)
    {
        writeDeGammaNormalMode(u32LutData,ratio2);
    }
    else if(E_DEGAMMA_MASERATI_MD == IpType)
    {
        writeMdDeGamma(u32LutData,debugMode,transferCharacterstics,lut_en);
    }
    /*
     else if(E_DEGAMMA_U13 == IpType)
     {
      writeDeGammaNormalModeUniverse(u32LutData,ratio2);
     }
    */
    else
    {
        printf("Degamma IP type is unsupported, Degamma will not be updated\n");
        status = 0;
    }
    return status;
}
MS_U8 deGamma(MS_U8 transferCharacterstics,
              MS_U16 max_lum_code,
              MS_U8 ratio1, MS_U16 ratio2,MS_U8 lut_en, U32* lut_idx, MS_U16 Lut_length, MS_U8 IpType,MS_U8 inputFormatFlag,MS_U8 debugMode) //debug mode: 0 = off, 1 = print2screen, 2 = print2file
{
    MS_U8 status = 1;
    MS_U16 u16ArrayLength = (E_DEGAMMA_MASERATI_MD != IpType) ? 513 : 600;
    U32* u32LutData = (U32*) kmalloc( u16ArrayLength * sizeof(U32), GFP_KERNEL);
    MS_U16 i;
    MS_U8 u8BitDepth  = (E_DEGAMMA_MASERATI_MD == IpType) ? 14 : 10;
    ST_TRANSFER_FUNCTION_DATA stDeGammaData;

    if (1 == lut_en) // use externally generated DeGamma curve
    {
        if(NULL == lut_idx)
        {
            printf("DeGamma e rror! lut_idx == NULL, deGamma will not be updated\n");
            return 0;
        }
        for( i = 0; i < u16ArrayLength; i++)
        {
            u32LutData[i] = lut_idx[min(i,Lut_length-1)];
        }
    }
    else // Generate DeGamma curve
    {
        parseInput4DeGamma(transferCharacterstics,&stDeGammaData, inputFormatFlag);

        if(ratio1 > 0x80)
        {
            ratio1 = 0x80;
            printf("deGamma warning: ratio1 exceeds the limit, clamped to 0x80");
        }
        if(ratio2 > 0x200)
        {
            ratio2 = 0x200;
            printf("deGamma warning: ratio2 exceeds the limit, clamped to 0x200");
        }

        NewDeGammaGenerator(u32LutData, &stDeGammaData, max_lum_code,ratio1,&status,  IpType, u16ArrayLength, u8BitDepth,transferCharacterstics);
    }
    if(1 == status) //Write DeGamma curve
    {

        status = deGammaHal( IpType, u32LutData, ratio2, lut_en, debugMode, transferCharacterstics);
    }


    kfree(u32LutData);
    return status;
}
U32 Mhal_CFD_GammaSampling(MS_U32 u32In,  MS_U32 u32Gamma,  MS_U16 u16A, MS_U16 u16B, MS_U16 u16S,MS_U8 u8BitDepth,MS_U8 sMode)
{
// u16Gamma : 4.16
// u16GInv  : 4.16
// u16A : 1.15
// u16B : 0.16
// u16S : 6.10
    U64 u32Th, u32Out;


    u32Th = u16B;
    if (u32In <= u32Th)
        return ( (( ((U32)u32In ) * u16S + (1<<9) )>> 10)*( ((1<<u8BitDepth) -1) ))>>(u8BitDepth);

    if(0 == sMode)
    {
        u32Out = ( (((newPow04(u32In, u32Gamma))*u16A)+(1<<14))>>15) - ((u16A - 0x8000) << 1);// * ( ((1<<u8BitDepth)-1)/(1<<u8BitDepth) ) ;
    }
    else
    {
        u32Out =  newPow04(u32In, u32Gamma) - ((u16A - 0x8000) << 1);// * ( ((1<<u8BitDepth)-1)/(1<<u8BitDepth) ) ;
    }


    return ( (u32Out*( ((1<<u8BitDepth) -1) ))>>(u8BitDepth) );


}
void newGammaGenerator(U32* u32LutData,ST_TRANSFER_FUNCTION_DATA* pData,MS_U16 u16White, MS_U16 u16Black,MS_U8 *status, MS_U8 IpType,MS_U16 u16NumEntries,MS_U8 u8BitDepth,MS_U8 transferCharacterstics)
{

// u16Gamma : 4.12
// u16GInv  : 1.12
// u16A : 1.15
// u16B : 0.16
// u16S : 6.10
    MS_U16  u16LutIdx, u16Index;
    MS_U32 u32Val;

//U32 u32LutData[600];

    MS_U16 u16Gamma,  u16Ginv;




    if(pData->sValid == FALSE)
    {
        //HDR_DBG_HAL(printk("\n    Kernel      MDrv_HDR_TransferFunctionGen    ERROR   u8ColorPrimaries=%d     \n",g_HDRinitParameters.DLC_HDRMetadataMpegVUI.u8ColorPrimaries));
        *status = 0;
        return;
    }


    if (pData->sMode == 0 || pData->sMode == 1)
    {

        if(E_GAMMA_MASERATI_MD == IpType)
        {
            //u16NumEntries = 512;
            MS_U8 u8Number = 0;
            MS_U16 u16Temp = u16NumEntries;
            while( u16Temp  > 0 )
            {
                u16Temp = u16Temp>>1;
                u8Number++;
            }
            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {
                u32Val = ( (((U64)u16LutIdx <<(17-u8Number+u8BitDepth)) + (1<<(u8BitDepth-1)) )/( (1<<u8BitDepth) - 1));
                u32LutData[u16LutIdx] = Mhal_CFD_GammaSampling(u32Val, pData->u32Gamma, pData->u16Alpha, pData->u16Beta, pData->u16LowerBoundGain , u8BitDepth,pData->sMode);
            }
        }
        else
        {
            //u16NumEntries = 256;
            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {

                u32Val = ( (((U64)u16LutIdx <<(8+u8BitDepth)) + (1<<(u8BitDepth-1)) )/( (1<<u8BitDepth) - 1));


                u32LutData[u16LutIdx] = Mhal_CFD_GammaSampling(u32Val, pData->u32Gamma, pData->u16Alpha, pData->u16Beta, pData->u16LowerBoundGain , u8BitDepth,pData->sMode);

            }
            // input > 1 case --> to fix
            for ( ; u16LutIdx < u16NumEntries; u16LutIdx++)
                u32LutData[u16LutIdx] = u32LutData[u16NumEntries-1];
        }


    }

    else if (pData->sMode == 2) // PQ
    {
        pData->sValid == FALSE;
        *status = 0;
        return;
        printf("Gamma E rror! Gamma mode (PQ) is currently unsuported, Gamma table has not been updated.\n");
    }
    else if (pData->sMode == 4)  // Bt1886
    {
        u16Gamma = pData->sData[0];
        //u16B = pData->sData[1];
        u16Ginv = 0x1000000ul / u16Gamma;
        if(0 == u16White)
        {
            printf("Gamma warning! u16White == 0, black and white level might be unintilized, changing u16White and u16Black to 0xFF00 and 0x42\n");
            u16White = 0xFF00;
            u16Black - 0x42;
        }


        for(u16Index = 0; u16Index < 256; u16Index++)
        {
            u32LutData[u16Index] = MDrv_HDR_Bt1886Gamma_ali(u16Index<<8, u16Gamma, u16Ginv, u16White, u16Black);
            u32LutData[u16Index] = (u32LutData[u16Index] - (u32LutData[u16Index]>>8)) >> 4;
        }
    }
    else// if (mode == 3) // Log
    {
        if(E_GAMMA_MASERATI_MD == IpType)
        {
            for (u16LutIdx = 0; u16LutIdx < 513; u16LutIdx++)
            {
                u32LutData[u16LutIdx] =  (E_CFD_CFIO_TR_LOG0 == transferCharacterstics) ? E0TF9LutGamma_14bit[u16LutIdx]>>16 : E0TF10LutGamma_14bit[u16LutIdx]>>16; //32 >> 15 == 16
            }
            //u32LutData[u16LutIdx] =  (E_CFD_CFIO_TR_LOG0 == transferCharacterstics) ? (E0TF9LutGamma_14bit[u16LutIdx]-E0TF9LutGamma_14bit[u16LutIdx-1])>>15 : (E0TF10LutGamma_14bit[u16LutIdx]-E0TF10LutGamma_14bit[u16LutIdx-1])>>15;  //31 >> 15 == 16
        }
        else
        {

            for (u16LutIdx = 0; u16LutIdx < u16NumEntries; u16LutIdx++)
            {

                u32LutData[u16LutIdx] =  (E_CFD_CFIO_TR_LOG0 == transferCharacterstics) ? E0TF9LutGamma_10bit[u16LutIdx]>>16 : E0TF10LutGamma_10bit[u16LutIdx]>>16; //31 >> 15 == 16
            }

        }
    }
}


MS_U8 gammaHal(MS_U8 IpType,U32* u32LutData,MS_U8 lut_en,MS_U8 debugMode,MS_U8 transferCharacterstics)
{
    MS_U8 status = 1;
    switch(IpType)
    {
        case E_GAMMA_MANHATTAN_VOP2:
            writeGamma(u32LutData,debugMode);
            break;
        case E_GAMMA_MASERATI_MD:
            writeMdGamma(u32LutData,debugMode,transferCharacterstics);
            break;
        /*
        case E_GAMMA_U13:
        writeGammaUniverse(u32LutData,debugMode);
        break;
        */
        default:
            printf("Current Gamma IpType is unsupported,Gamma table will not be updated.\n");
            status = 0;
            break;
    }
    return status;
}
MS_U8 gamma(MS_U8 transferCharacterstics,
            MS_U16 max_lum_code,
            MS_U16 u16White, MS_U16 u16Black, MS_U8 lut_en, U32* lut_idx, MS_U16 Lut_length, MS_U8 IpType,MS_U8 inputFormatFlag,MS_U8 debugMode)
{
    MS_U8 status = 1;
    MS_U16 u16ArrayLength = (E_GAMMA_MASERATI_MD == IpType) ? 513 : 257;
    MS_U16 u8BitDepth = (E_GAMMA_MASERATI_MD == IpType) ? 14 : 10;
    U32* u32LutData = (U32*) kmalloc( u16ArrayLength * sizeof(U32), GFP_KERNEL);
    MS_U16 i;
    ST_TRANSFER_FUNCTION_DATA stDeGammaData;

    if (1 == lut_en)
    {
        if(NULL == lut_idx)
        {
            printf("Gamma e rror! lut_idx == NULL, gamma will not be updated\n");
            return 0;
        }
        for( i = 0; i < u16ArrayLength; i++)
        {
            u32LutData[i] = lut_idx[min(i,Lut_length-1)];
        }
    }
    else
    {
        parseInput4DeGamma(transferCharacterstics,&stDeGammaData, inputFormatFlag);
        newGammaGenerator(u32LutData, &stDeGammaData, u16White,  u16Black,&status,  IpType,u16ArrayLength,u8BitDepth, transferCharacterstics);
    }
    if(1 == status)
    {
        status = gammaHal( IpType,u32LutData, lut_en, debugMode, transferCharacterstics);
    }

    kfree(u32LutData);
    return status;
}
void Hdr444to442_b601W(MS_BOOL bIsOn)
{
    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x20,0x20); //reg_444to422_byp_en
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x20,0x00); //reg_444to422_byp_en
    }
}
MS_U8 Hdr444to442_b601(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode)
{
    if(0 == bWriteRegister)
        return 1;
    if(1 == bIsOn)
        bIsOn = u8Mode;
    Hdr444to442_b601W(bIsOn);
    return 1;
}
void hdrDither2_b602W(MS_BOOL bIsOn,MS_BOOL bDitherForceWindow, MS_U8 u8DitherWindow)
{
    if(0 == bIsOn)
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x40,0x40); //reg_dith_win_byp_en
    }
    else
    {
        writeByteMask(_PK_L_(0x7A,0x02),0x40,0x00); //reg_dith_win_byp_en
        writeByteMask(_PK_L_(0x7A,0x5B),0x10,bDitherForceWindow<<4);

        writeByteMask(_PK_L_(0x7A,0x5B),0x07,u8DitherWindow);
    }
}
MS_U8 hdrDither2_b602(MS_BOOL bIsOn,MS_BOOL bWriteRegister, MS_U8 u8Mode, ST_HDR_DITHER2* Struct)
{

    MS_BOOL bDitherForceWindow;
    MS_U8 u8DitherWindow;
    switch(u8Mode)
    {
        case 0xFF:
            bDitherForceWindow = Struct->bDitherForceWindow;
            u8DitherWindow = Struct->u8DitherWindow;
            break;
        case 0:
            bDitherForceWindow = 0;
            u8DitherWindow = 0;
            break;
        case 1:
            bDitherForceWindow = 0;
            u8DitherWindow = 0;
            break;
        default:
            printk("hdrDither2_b602W error! current mode unsupported!\n");
            return 0;
    }
    if(1 == bWriteRegister)
        hdrDither2_b602W( bIsOn, bDitherForceWindow,  u8DitherWindow);
    return 1;

}
