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
#ifndef DRV_GOP_SC_H
#define DRV_GOP_SC_H

#ifdef DRV_GOP_SC_C
#define INTERFACE
#else
#define INTERFACE extern
#endif


typedef enum
{
    EN_DST_FB=0,
    EN_DST_FBL=1,
    EN_DST_NUM,
}DRV_GOP_SC_Dst;

//External Info
typedef struct
{
    //op to ve timing
    MS_U16 u16VttIn;
    MS_U16 u16HttIn;
    MS_U16 u16HdeIn;
    MS_U16 u16VdeIn;

    MS_U16 u16Vde_St;
    MS_U16 u16Hde_St;

}DRV_GOP_SC_InCfg;

typedef struct
{
    //ve out timing
    MS_U16 u16VttOut;   //VttOut
    MS_U16 u16HttOut;   //HttOut
    MS_U16 u16HdeOut;    //HdeOut
    MS_U16 u16VdeOut;    //Vdeout

    MS_BOOL bInterlace;

}DRV_GOP_SC_OutCfg;

typedef struct
{
    MS_U8  u8MuxSel;
    MS_U8 u8TVSystem;
    DRV_GOP_SC_Dst stDst;

    DRV_GOP_SC_InCfg InCfg;
    DRV_GOP_SC_OutCfg OutCfg;

}DRV_GOP_SC_INFO;

//Internal Parameter
typedef struct
{
    MS_U16 u16ValidV;
    MS_U16 u16ValidH;
    MS_U8 u8SkipV;
    MS_U8 u8SkipH;

    MS_U16 u16VFacInt;
    MS_U16 u16VFacFrac;
    MS_U16 u16VE_RefY;
    MS_U16 u16VE_RefX;

    MS_U32 u32IDclk;
    MS_U32 u32ODclk;

}GOP_SC_REF;

MS_BOOL MDrv_GOP_SC_Init(void);
VOID MDrv_GOP_SC_Init_riu_base( MS_VIRT vriu_base );
MS_BOOL MDrv_GOP_SC_MuxSel(MS_U8 u8Source);
VOID MDrv_GOP_SC_SetParams(DRV_GOP_SC_InCfg *pInCfg,DRV_GOP_SC_OutCfg *pOutCfg,MS_U8 u8TvSys);
MS_BOOL MDrv_GOP_SC_SetCfg(VOID);
MS_BOOL MDrv_GOP_SC_SetDst(DRV_GOP_SC_Dst stDst);
MS_BOOL MDrv_GOP_SC_SetLock(VOID);
MS_BOOL MDrv_GOP_SC_SetFPLL_Enable(MS_BOOL bEnable);

#undef INTERFACE

#endif
