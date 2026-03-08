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
////////////////////////////////////////////////////////////////////////////////
//****************************************************
//   Quality Map CodeGen Version 4.0
// 1920
// Maxim
// 6/20/2016  5:17:11 PM
// 42541.7202662
//****************************************************

#ifndef _MAXIM_UFSC_H_
#define _MAXIM_UFSC_H_

#define PQ_IP_NUM_UFSC 16

typedef enum
{
    #if PQ_QM_HMDI
    QM_4K2K_RGB444_UFSC, //0
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_YUV444_UFSC, //1
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_YUV422_UFSC, //2
    #endif
    #if PQ_QM_HMDI
    QM_FHD_RGB444_UFSC, //3
    #endif
    #if PQ_QM_HMDI
    QM_FHD_YUV444_UFSC, //4
    #endif
    #if PQ_QM_HMDI
    QM_FHD_YUV422_UFSC, //5
    #endif
    #if PQ_QM_HMDI
    QM_FSC_3D_UFSC, //6
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_RGB444_DS_UFSC, //7
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_YUV444_DS_UFSC, //8
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_YUV422_DS_UFSC, //9
    #endif
    #if PQ_QM_HMDI
    QM_FHD_RGB444_DS_UFSC, //10
    #endif
    #if PQ_QM_HMDI
    QM_FHD_YUV444_DS_UFSC, //11
    #endif
    #if PQ_QM_HMDI
    QM_FHD_YUV422_DS_UFSC, //12
    #endif
    #if PQ_QM_HMDI
    QM_FSC_3D_DS_UFSC, //13
    #endif
    QM_INPUTTYPE_NUM_UFSC, // 14
} SC_QUALITY_MAP_INDEX_e_UFSC;

typedef enum
{
PQ_IP_INIT_FAC_UFSC,  //0
PQ_IP_VSP_Y_UFSC,  //1
PQ_IP_VSP_C_UFSC,  //2
PQ_IP_VSP_CoRing_UFSC,  //3
PQ_IP_VSP_Dither_UFSC,  //4
PQ_IP_VSP_PreVBound_UFSC,  //5
PQ_IP_HSP_Y_UFSC,  //6
PQ_IP_HSP_C_UFSC,  //7
PQ_IP_HSP_CoRing_UFSC,  //8
PQ_IP_HSP_DeRing_UFSC,  //9
PQ_IP_HSP_Dither_UFSC,  //10
PQ_IP_HnonLinear_UFSC,  //11
PQ_IP_HSP_Y_COEFF_UFSC,  //12
PQ_IP_HSP_C_COEFF_UFSC,  //13
PQ_IP_VIP_4K_UFSC,  //14
PQ_IP_SC_End_UFSC,  //15
 }   PQ_IPTYPE_UFSC;

typedef enum
{
PQ_IP_INIT_FAC_COM_UFSC,  //0
PQ_IP_VSP_Y_COM_UFSC,  //1
PQ_IP_VSP_C_COM_UFSC,  //2
PQ_IP_VSP_CoRing_COM_UFSC,  //3
PQ_IP_VSP_Dither_COM_UFSC,  //4
PQ_IP_VSP_PreVBound_COM_UFSC,  //5
PQ_IP_HSP_Y_COM_UFSC,  //6
PQ_IP_HSP_C_COM_UFSC,  //7
PQ_IP_HSP_CoRing_COM_UFSC,  //8
PQ_IP_HSP_DeRing_COM_UFSC,  //9
PQ_IP_HSP_Dither_COM_UFSC,  //10
PQ_IP_HnonLinear_COM_UFSC,  //11
PQ_IP_HSP_Y_COEFF_COM_UFSC,  //12
PQ_IP_HSP_C_COEFF_COM_UFSC,  //13
PQ_IP_VIP_4K_COM_UFSC,  //14
PQ_IP_SC_End_COM_UFSC,  //15
PQ_IP_COM_NUMS_UFSC
} PQ_IP_COM_Group_UFSC;

typedef enum
{
PQ_IP_INIT_FAC_Type0_UFSC,
PQ_IP_INIT_FAC_Type1_UFSC,
PQ_IP_INIT_FAC_Type2_UFSC,
PQ_IP_INIT_FAC_NUMS_UFSC
} PQ_IP_INIT_FAC_Group_UFSC;

typedef enum
{
PQ_IP_VSP_Y_Bypass_UFSC,
PQ_IP_VSP_Y_Bilinear_UFSC,
PQ_IP_VSP_Y_Y_4Tap_UFSC,
PQ_IP_VSP_Y_Y_6Tap_UFSC,
PQ_IP_VSP_Y_NUMS_UFSC
} PQ_IP_VSP_Y_Group_UFSC;

typedef enum
{
PQ_IP_VSP_C_Bypass_UFSC,
PQ_IP_VSP_C_Bilinear_UFSC,
PQ_IP_VSP_C_Y_4Tap_UFSC,
PQ_IP_VSP_C_C_4Tap_UFSC,
PQ_IP_VSP_C_NUMS_UFSC
} PQ_IP_VSP_C_Group_UFSC;

typedef enum
{
PQ_IP_VSP_CoRing_OFF_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_1_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_2_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_3_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_4_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_5_UFSC,
PQ_IP_VSP_CoRing_NUMS_UFSC
} PQ_IP_VSP_CoRing_Group_UFSC;

typedef enum
{
PQ_IP_VSP_Dither_OFF_UFSC,
PQ_IP_VSP_Dither_ON_UFSC,
PQ_IP_VSP_Dither_NUMS_UFSC
} PQ_IP_VSP_Dither_Group_UFSC;

typedef enum
{
PQ_IP_VSP_PreVBound_OFF_UFSC,
PQ_IP_VSP_PreVBound_ON_UFSC,
PQ_IP_VSP_PreVBound_NUMS_UFSC
} PQ_IP_VSP_PreVBound_Group_UFSC;

typedef enum
{
PQ_IP_HSP_Y_Bypass_UFSC,
PQ_IP_HSP_Y_Bilinear_UFSC,
PQ_IP_HSP_Y_Y_4Tap_UFSC,
PQ_IP_HSP_Y_Y_6Tap_UFSC,
PQ_IP_HSP_Y_NUMS_UFSC
} PQ_IP_HSP_Y_Group_UFSC;

typedef enum
{
PQ_IP_HSP_C_Bypass_UFSC,
PQ_IP_HSP_C_Bilinear_UFSC,
PQ_IP_HSP_C_C_4Tap_UFSC,
PQ_IP_HSP_C_Y_4Tap_UFSC,
PQ_IP_HSP_C_NUMS_UFSC
} PQ_IP_HSP_C_Group_UFSC;

typedef enum
{
PQ_IP_HSP_CoRing_OFF_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_1_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_2_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_3_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_4_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_5_UFSC,
PQ_IP_HSP_CoRing_NUMS_UFSC
} PQ_IP_HSP_CoRing_Group_UFSC;

typedef enum
{
PQ_IP_HSP_DeRing_OFF_UFSC,
PQ_IP_HSP_DeRing_DR1_UFSC,
PQ_IP_HSP_DeRing_NUMS_UFSC
} PQ_IP_HSP_DeRing_Group_UFSC;

typedef enum
{
PQ_IP_HSP_Dither_OFF_UFSC,
PQ_IP_HSP_Dither_ON_UFSC,
PQ_IP_HSP_Dither_NUMS_UFSC
} PQ_IP_HSP_Dither_Group_UFSC;

typedef enum
{
PQ_IP_HnonLinear_OFF_UFSC,
PQ_IP_HnonLinear_H_1366_0_UFSC,
PQ_IP_HnonLinear_H_1366_1_UFSC,
PQ_IP_HnonLinear_H_1366_2_UFSC,
PQ_IP_HnonLinear_H_1440_UFSC,
PQ_IP_HnonLinear_H_1680_UFSC,
PQ_IP_HnonLinear_H_1920_0_UFSC,
PQ_IP_HnonLinear_H_1920_1_UFSC,
PQ_IP_HnonLinear_H_1920_2_UFSC,
PQ_IP_HnonLinear_NUMS_UFSC
} PQ_IP_HnonLinear_Group_UFSC;

typedef enum
{
PQ_IP_HSP_Y_COEFF_OFF_UFSC,
PQ_IP_HSP_Y_COEFF_InvSinc4Tc4p4Fc95Apass0001Astop40_UFSC,
PQ_IP_HSP_Y_COEFF_InvSinc4Tc4p4Fc75Apass0001Astop40_UFSC,
PQ_IP_HSP_Y_COEFF_InvSinc4Tc4p4Fc85Apass0001Astop40_UFSC,
PQ_IP_HSP_Y_COEFF_NUMS_UFSC
} PQ_IP_HSP_Y_COEFF_Group_UFSC;

typedef enum
{
PQ_IP_HSP_C_COEFF_OFF_UFSC,
PQ_IP_HSP_C_COEFF_InvSinc4Tc4p4Fc85Apass0001Astop40_UFSC,
PQ_IP_HSP_C_COEFF_InvSinc4Tc4p4Fc75Apass0001Astop40_UFSC,
PQ_IP_HSP_C_COEFF_NUMS_UFSC
} PQ_IP_HSP_C_COEFF_Group_UFSC;

typedef enum
{
PQ_IP_VIP_4K_OFF_UFSC,
PQ_IP_VIP_4K_4K2K_UFSC,
PQ_IP_VIP_4K_NUMS_UFSC
} PQ_IP_VIP_4K_Group_UFSC;

typedef enum
{
PQ_IP_SC_End_End_UFSC,
PQ_IP_SC_End_NUMS_UFSC
} PQ_IP_SC_End_Group_UFSC;

extern code U8 MST_SkipRule_IP_UFSC[PQ_IP_NUM_UFSC];
extern code EN_IPTAB_INFO PQ_IPTAB_INFO_UFSC[];
extern code U8 QMAP_1920_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_IP_NUM_UFSC];
extern code U8 QMAP_1366_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_IP_NUM_UFSC];
extern code U8 QMAP_4K_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_IP_NUM_UFSC];

#endif
