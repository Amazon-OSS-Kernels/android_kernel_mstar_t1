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
//************************************************************
//***             RIU Base address of each IP              ***
//***                (Byte address [23:0])                 ***
//************************************************************

#ifndef _C_RIUBASE_H_
#define _C_RIUBASE_H_

#define     RIUBASE_RIU_DBG                 0x100100    //0x100100 ~ 0x1001ff
#define     RIUBASE_VD_MHEG5                0x100300    //0x100300 ~ 0x1003ff
#define     RIUBASE_POR_STATUS              0x100500    //0x100500 ~ 0x10053f
#define     RIUBASE_INTR_CPUINT             0x100540    //0x100540 ~ 0x10057f
#define     RIUBASE_USB0                    0x100700    //0x100700 ~ 0x10077f
#define     RIUBASE_BDMA                    0x100900    //0x100900 ~ 0x10097f
#define     RIUBASE_UART0                   0x100980    //0x100980 ~ 0x1009ff
#define     RIUBASE_CLKGEN0                 0x100b00    //0x100b00 ~ 0x100bff
#define     RIUBASE_MVD                     0x101100    //0x101100 ~ 0x1011ff
#define     RIUBASE_MIU                     0x101200    //0x101200 ~ 0x1012ff
#define     RIUBASE_MVOP                    0x101400    //0x101400 ~ 0x1014ff
#define     RIUBASE_TSP0                    0x101500    //0x101500 ~ 0x1015ff
#define     RIUBASE_TSP1                    0x101600    //0x101600 ~ 0x1016ff
#define     RIUBASE_JPD                     0x101700    //0x101700 ~ 0x1017ff
#define     RIUBASE_SEMAPH                  0x101800    //0x101800 ~ 0x10183f
#define     RIUBASE_MAU1                    0x101860    //0x101860 ~ 0x10187f
#define     RIUBASE_ECBRIDGE                0x101880    //0x101880 ~ 0x1018ff
#define     RIUBASE_INTR_CTRL               0x101900    //0x101900 ~ 0x1019ff
#define     RIUBASE_HDMI2                   0x101a00    //0x101a00 ~ 0x101aff
#define     RIUBASE_MIPS                    0x101d00    //0x101d00 ~ 0x101dff
#define     RIUBASE_CHIP                    0x101e00    //0x101e00 ~ 0x101eff
#define     RIUBASE_GOP                     0x101f00    //0x101f00 ~ 0x101fff
#define     RIUBASE_UHC0                    0x102400    //0x102400 ~ 0x1024ff
#define     RIUBASE_ADC_ATOP                0x102500    //0x102500 ~ 0x1025ff
#define     RIUBASE_ADC_DTOP                0x102600    //0x102600 ~ 0x1026ff
#define     RIUBASE_HDMI                    0x102700    //0x102700 ~ 0x1027ff
#define     RIUBASE_GE0                     0x102800    //0x102800 ~ 0x1028ff
#define     RIUBASE_CHIPGPIO                0x102b00    //0x102b00 ~ 0x102bff
#define     RIUBASE_SC0                     0x102e00    //0x102e00 ~ 0x102eff
#define     RIUBASE_SC1                     0x102f00    //0x102f00 ~ 0x102fff
#define     RIUBASE_SC2                     0x103000    //0x103000 ~ 0x1030ff
#define     RIUBASE_SC3                     0x103100    //0x103100 ~ 0x1031ff
#define     RIUBASE_SC4                     0x103200    //0x103200 ~ 0x1032ff
#define     RIUBASE_CLKGEN1                 0x103300    //0x103300 ~ 0x10337f
#define     RIUBASE_MAILBOX                 0x103380    //0x103380 ~ 0x1033ff
#define     RIUBASE_MIIC                    0x103420    //0x103420 ~ 0x10343f
#define     RIUBASE_VDMCU51_IF              0x103460    //0x103460 ~ 0x10347f
#define     RIUBASE_DMDMCU51_IF             0x103480    //0x103480 ~ 0x10349f
#define     RIUBASE_URDMA                   0x1034c0    //0x1034c0 ~ 0x1034ff
#define     RIUBASE_AFEC                    0x103500    //0x103500 ~ 0x1035ff
#define     RIUBASE_COMB                    0x103600    //0x103600 ~ 0x1036ff
#define     RIUBASE_VBI                     0x103700    //0x103700 ~ 0x1037ff
#define     RIUBASE_SCM                     0x103800    //0x103800 ~ 0x1038ff
#define     RIUBASE_PATGEN                  0x103980    //0x103980 ~ 0x1039ff
#define     RIUBASE_UTMI                    0x103a80    //0x103a80 ~ 0x103aff
#define     RIUBASE_REG_PIU_NONPM           0x103c00    //0x103c00 ~ 0x103cff
#define     RIUBASE_UART1                   0x110600    //0x110600 ~ 0x11063f
#define     RIUBASE_FUART                   0x110680    //0x110680 ~ 0x1106bf
#define     RIUBASE_HDCPKEY                 0x110800    //0x110800 ~ 0x1108ff
#define     RIUBASE_DVI_ATOP                0x110900    //0x110900 ~ 0x1109ff
#define     RIUBASE_DVI_DTOP                0x110a00    //0x110a00 ~ 0x110a7f
#define     RIUBASE_DVIEQ                   0x110a80    //0x110a80 ~ 0x110abf
#define     RIUBASE_HDCP                    0x110ac0    //0x110ac0 ~ 0x110aff
#define     RIUBASE_ANA_MISC                0x110c00    //0x110c00 ~ 0x110c7f
#define     RIUBASE_MIU_ATOP                0x110d00    //0x110d00 ~ 0x110dff
#define     RIUBASE_ADC_DTOPB               0x111200    //0x111200 ~ 0x1112ff
#define     RIUBASE_CLKGEN_DMD              0x111f00    //0x111f00 ~ 0x111fff
#define     RIUBASE_DMD_TOP_HK              0x112000    //0x112000 ~ 0x1120ff
#define     RIUBASE_VIF_RF                  0x112100    //0x112100 ~ 0x1121ff
#define     RIUBASE_VIF_DBB1                0x112200    //0x112200 ~ 0x1122ff
#define     RIUBASE_VIF_DBB2                0x112300    //0x112300 ~ 0x1123ff
#define     RIUBASE_VIF_DBB3                0x112400    //0x112400 ~ 0x1124ff
#define     RIUBASE_DMD_ADCDMA              0x112500    //0x112500 ~ 0x1125ff
#define     RIUBASE_DMD_MAILBOX_HK          0x112600    //0x112600 ~ 0x1126ff
#define     RIUBASE_DEMOD_7                 0x112700    //0x112700 ~ 0x1127ff
#define     RIUBASE_DMD_ANA_MISC_HK         0x112800    //0x112800 ~ 0x1128ff
#define     RIUBASE_VIVALDI0                0x112a00    //0x112a00 ~ 0x112aff
#define     RIUBASE_VIVALDI2                0x112c00    //0x112c00 ~ 0x112cff
#define     RIUBASE_VIVALDI3                0x112d00    //0x112d00 ~ 0x112dff
#define     RIUBASE_DVI_ATOP_1              0x113200    //0x113200 ~ 0x1132ff
#define     RIUBASE_DVI_DTOP_1              0x113300    //0x113300 ~ 0x11337f
#define     RIUBASE_DVIEQ_1                 0x113380    //0x113380 ~ 0x1133bf
#define     RIUBASE_HDCP_1                  0x1133c0    //0x1133c0 ~ 0x1133ff
#define     RIUBASE_DVI_PS                  0x113600    //0x113600 ~ 0x1136ff
#define     RIUBASE_DVI_DTOP_3              0x113700    //0x113700 ~ 0x11377f
#define     RIUBASE_DVIEQ_3                 0x113780    //0x113780 ~ 0x1137bf
#define     RIUBASE_HDCP_3                  0x1137c0    //0x1137c0 ~ 0x1137ff
#define     RIUBASE_GOP4G_0                 0x120200    //0x120200 ~ 0x1202ff
#define     RIUBASE_GOP4G_1                 0x120300    //0x120300 ~ 0x1203ff
#define     RIUBASE_GOP4G_ST                0x120400    //0x120400 ~ 0x1204ff
#define     RIUBASE_GOP2G_0                 0x120500    //0x120500 ~ 0x1205ff
#define     RIUBASE_GOP2G_1                 0x120600    //0x120600 ~ 0x1206ff
#define     RIUBASE_GOP2G_ST                0x120700    //0x120700 ~ 0x1207ff
#define     RIUBASE_GOP1G_0                 0x120800    //0x120800 ~ 0x1208ff
#define     RIUBASE_SPARE0                  0x121000    //0x121000 ~ 0x1210ff
#define     RIUBASE_SPARE1                  0x121100    //0x121100 ~ 0x1211ff
#define     RIUBASE_MSC                     0x130000    //0x130000 ~ 0x13ffff

#endif  //_C_RIUBASE_H_
