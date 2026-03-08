/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiGOP_SC.h
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_GOP_SC_H_
#define _API_GOP_SC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define MSIF_GOP_SC_LIBVER              {'0','0'}            //LIB version
#define MSIF_GOP_SC_BUILDNUM            {'1','1'}            //Build Number
#define MSIF_GOP_SC_CHANGELIST          {'0','0','3','9','1','8','8','7'} //P4 ChangeList Number

typedef enum
{
    EN_GOP_ARGB =0,
    EN_SC_RGB
}EN_GOP_SC_MUX_SEL;

typedef enum
{
    EN_GOP_SC_NTSC =0,
    EN_GOP_SC_PAL,
    EN_GOP_SC_PAL_M,
    EN_GOP_SC_TVSYS_NUM
} EN_GOP_SC_TVSYS;

typedef enum
{
    EN_GOP_SC_DST_FB=0,
    EN_GOP_SC_DST_FBL=1,
    EN_GOP_SC_DST_NUM,

}MAPI_GOP_SC_Dst;

typedef struct
{
    //op to ve timing
    MS_U16 u16VTotal;   //VttIn for GOP_SC
    MS_U16 u16HTotal;   //HttIn for GOP_SC
    MS_U16 u16Width;    //HdeIn for GOP_SC
    MS_U16 u16Hight;    //VdeIn for GOP_SC

    MS_U16 u16Vde_St;
    MS_U16 u16Hde_St;

}MAPI_GOP_SC_InCfg;

typedef struct
{
    EN_GOP_SC_MUX_SEL  enSrcSel;

    MAPI_GOP_SC_InCfg stInputCfg;
    EN_GOP_SC_TVSYS u8TVSys;
    MAPI_GOP_SC_Dst stDst;

}MAPI_GOP_SC_INFO;

void MApi_GOP_SC_SkipInit(MS_BOOL bEnable);
MS_BOOL MApi_GOP_SC_Init(void);
MS_BOOL MApi_GOP_SC_SetCfg(MAPI_GOP_SC_INFO *pGopSC_Info);
MS_BOOL MApi_GOP_SC_MuxSel(EN_GOP_SC_MUX_SEL u8Sel);
MS_BOOL MApi_GOP_SC_SetFPLL_Enable(MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif
