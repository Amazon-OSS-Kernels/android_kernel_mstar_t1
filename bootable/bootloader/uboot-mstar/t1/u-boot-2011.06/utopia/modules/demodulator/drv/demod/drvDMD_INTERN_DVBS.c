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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvDMD_INTERN_DVBS.c
/// @brief  DVBS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "drvDMD_INTERN_DVBS.h"
#include "drvDMD_INTERN_DVBS_v2.h"
//#include <string.h>
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvSYS.h"
#include "drvDMD_VD_MBX.h"
#include "halDMD_INTERN_DVBS.h"
#include "halDMD_INTERN_common.h"
#include "../../include/drvSAR.h"  // for Utopia2
#include "utopia.h"   
#include "utopia_dapi.h"
#include "ULog.h"
#include "../../include/drvDMD_INTERN_DVBS_v2.h"  // for Utopia2
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DVBS2FEC_REG_BASE            0x3300 
//#define MS_DEBUG 1
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_S16      _s16CurrentCFO;
extern MS_U16      _u16ChannelInfoArray[2][1000];
extern MS_U16      _u16ChannelInfoIndex;
extern MS_U16      _u16LockedCenterFreq;
extern MS_U16      _u16LockedSymbolRate;
extern MS_U8       modulation_order;
extern DMD_DVBS_Info sDMD_DVBS_Info;
extern MS_U8 u8DemodLockFlag;
extern MS_U8        g_dvbs_lock;
extern MS_BOOL     _bDemodType;
extern DMD_DVBS_DbgLv _u8DMD_DVBS_DbgLevel;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/*
static MS_U16             _u16SignalLevel[185][2]=
{//AV2028 SR=22M, 2/3 CN=5.9
    {32100,    920},{32200,    915},{32350,    910},{32390,    905},{32480,    900},{32550,    895},{32620,    890},{32680,    885},{32750,    880},{32830,    875},
    {32930,    870},{33010,    865},{33100,    860},{33200,    855},{33310,    850},{33410,    845},{33520,    840},{33640,    835},{33770,    830},{33900,    825},
    {34030,    820},{34150,    815},{34290,    810},{34390,    805},{34490,    800},{34580,    795},{34700,    790},{34800,    785},{34880,    780},{34940,    775},
    {35030,    770},{35130,    765},{35180,    760},{35260,    755},{35310,    750},{35340,    745},{35380,    740},{35400,    735},{35450,    730},{35550,    725},
    {35620,    720},{35700,    715},{35800,    710},{35890,    705},{36000,    700},{36120,    695},{36180,    690},{36280,    685},{36400,    680},{36570,    675},
    {36730,    670},{36910,    665},{37060,    660},{37100,    655},{37260,    650},{37340,    645},{37410,    640},{37580,    635},{37670,    630},{37700,    625},
    {37750,    620},{37800,    615},{37860,    610},{37980,    605},{38050,    600},{38170,    595},{38370,    590},{38540,    585},{38710,    580},{38870,    575},
    {39020,    570},{39070,    565},{39100,    560},{39180,    555},{39280,    550},{39460,    545},{39510,    540},{39600,    535},{39620,    530},{39680,    525},
    {39720,    520},{39830,    515},{39880,    510},{39930,    505},{39960,    500},{40000,    495},{40200,    490},{40360,    485},{40540,    480},{40730,    475},
    {40880,    470},{41020,    465},{41150,    460},{41280,    455},{41410,    450},{41520,    445},{41620,    440},{41730,    435},{41840,    430},{41930,    425},
    {42010,    420},{42100,    415},{42180,    410},{42260,    405},{42350,    400},{42440,    395},{42520,    390},{42580,    385},{42660,    380},{42730,    375},
    {42800,    370},{42870,    365},{42940,    360},{43000,    355},{43060,    350},{43130,    345},{43180,    340},{43250,    335},{43310,    330},{43370,    325},
    {43420,    320},{43460,    315},{43520,    310},{43570,    305},{43620,    300},{43660,    295},{43710,    290},{43750,    285},{43810,    280},{43860,    275},
    {43910,    270},{43940,    265},{43990,    260},{44020,    255},{44060,    250},{44110,    245},{44140,    240},{44190,    235},{44230,    230},{44270,    225},
    {44320,    220},{44370,    215},{44400,    210},{44450,    205},{44490,    200},{44530,    195},{44590,    190},{44630,    185},{44660,    180},{44720,    175},
    {44750,    170},{44790,    165},{44830,    160},{44880,    155},{44910,    150},{44960,    145},{45000,    140},{45030,    135},{45070,    130},{45100,    125},
    {45130,    120},{45160,    115},{45200,    110},{45240,    105},{45270,    100},{45300,     95},{45330,     90},{45360,     85},{45400,     80},{45430,     75},
    {45460,     70},{45490,     65},{45530,     60},{45560,     55},{45590,     50},{45630,     45},{45670,     40},{45690,     35},{45740,     30},{45760,     25},
    {45800,     20},{45830,     15},{45860,     10},{45880,      5},{45920,      0}
};
*/
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

static    void* ppDVBSInstant = NULL;
static MS_U32 u32DVBSopen = 0;
static MS_U8 u8DVBSUtopiaOpen = 0;   //for SetStillImagePara is earlier called than Init  

static DMD_DVBS_InitData   AgcSsi_Para;
//static float intern_dvb_c_qam_ref[] = {3.0, 0.0, 0.0, 0.0, 0.0, 80.0}; //16q,32q,64q,128q,256q, and others

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX
#if 1
static float _LogApproxTableX[80] =
{ 1.00, 1.30, 1.69, 2.20, 2.86, 3.71, 4.83, 6.27, 8.16, 10.60, 13.79,
  17.92, 23.30, 30.29, 39.37, 51.19, 66.54, 86.50, 112.46, 146.19,
  190.05, 247.06, 321.18, 417.54, 542.80, 705.64, 917.33, 1192.53,
  1550.29, 2015.38, 2620.00, 3405.99, 4427.79, 5756.13, 7482.97,
  9727.86, 12646.22, 16440.08, 21372.11, 27783.74, 36118.86,
  46954.52, 61040.88, 79353.15, 103159.09, 134106.82, 174338.86,
  226640.52, 294632.68, 383022.48, 497929.22, 647307.99, 841500.39, 1093950.50,
  1422135.65, 1848776.35, 2403409.25, 3124432.03, 4061761.64, 5280290.13,
  6864377.17, 8923690.32, 11600797.42, 15081036.65, 19605347.64, 25486951.94,
  33133037.52, 43072948.77, 55994833.40, 72793283.42, 94631268.45,
  123020648.99, 159926843.68, 207904896.79, 270276365.82, 351359275.57,
  456767058.24, 593797175.72, 771936328.43, 1003517226.96
};

static float _LogApproxTableY[80] =
{ 0.00, 0.11, 0.23, 0.34, 0.46, 0.57, 0.68, 0.80, 0.91, 1.03, 1.14, 1.25,
  1.37, 1.48, 1.60, 1.71, 1.82, 1.94, 2.05, 2.16, 2.28, 2.39, 2.51, 2.62,
  2.73, 2.85, 2.96, 3.08, 3.19, 3.30, 3.42, 3.53, 3.65, 3.76, 3.87, 3.99,
  4.10, 4.22, 4.33, 4.44, 4.56, 4.67, 4.79, 4.90, 5.01, 5.13, 5.24, 5.36,
  5.47, 5.58, 5.70, 5.81, 5.93, 6.04, 6.15, 6.27, 6.04, 6.15, 6.27, 6.38,
  6.49, 6.61, 6.72, 6.84, 6.95, 7.06, 7.18, 7.29, 7.41, 7.52, 7.63, 7.75,
  7.86, 7.98, 8.09, 8.20, 8.32, 8.43, 8.55, 8.66
};

static float Log10Approx(float flt_x)
{
    MS_U8  indx = 0;

    do {
        if (flt_x < _LogApproxTableX[indx])
            break;
        indx++;
    }while (indx < 79);   //stop at indx = 80

    return _LogApproxTableY[indx];
}
#else
static float Log10Approx(float flt_x)
{
    MS_U32       u32_temp = 1;
    MS_U8        indx = 0;

    do {
        u32_temp = u32_temp << 1;
        if (flt_x < (float)u32_temp)
            break;
    }while (++indx < 32);

    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (float)0.3 * indx;
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_DVBS_Init(DMD_DVBS_InitData *pDMD_DVBS_InitData, MS_U32 u32InitDataLen)
{
    void* pAttribte = NULL;

    #ifdef MS_DEBUG
    ULOGD("DEMOD","******check driver layer DVBS init!!*******\n");
    #endif
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_Init\n"));

    DVBS_Init_PARAM Drv_DVBS_Init_PARAM;
    DMD_DVBS_InitData_Transform Init_Para_Temp;

    Init_Para_Temp.u8SarChannel=pDMD_DVBS_InitData->u8SarChannel;
    Init_Para_Temp.u8DMD_DVBS_DSPRegInitExt=pDMD_DVBS_InitData->u8DMD_DVBS_DSPRegInitExt;
    Init_Para_Temp.u8DMD_DVBS_DSPRegInitSize=pDMD_DVBS_InitData->u8DMD_DVBS_DSPRegInitSize;
    Init_Para_Temp.u8DMD_DVBS_InitExt=pDMD_DVBS_InitData->u8DMD_DVBS_InitExt;
    /*
    Init_Para_Temp.pTuner_RfagcSsi=pDMD_DVBS_InitData->pTuner_RfagcSsi;
    Init_Para_Temp.u16Tuner_RfagcSsi_Size=pDMD_DVBS_InitData->u16Tuner_RfagcSsi_Size;
    Init_Para_Temp.pTuner_IfagcSsi_LoRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_LoRef;
    Init_Para_Temp.u16Tuner_IfagcSsi_LoRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcSsi_LoRef_Size;  
    Init_Para_Temp.pTuner_IfagcSsi_HiRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef;
    Init_Para_Temp.u16Tuner_IfagcSsi_HiRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcSsi_HiRef_Size;
    Init_Para_Temp.pTuner_IfagcErr_LoRef=pDMD_DVBS_InitData->pTuner_IfagcErr_LoRef;
    Init_Para_Temp.u16Tuner_IfagcErr_LoRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcErr_LoRef_Size;
    Init_Para_Temp.pTuner_IfagcSsi_HiRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef;
    Init_Para_Temp.u16Tuner_IfagcErr_HiRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcErr_HiRef_Size;
    Init_Para_Temp.pSqiCnNordigP1=pDMD_DVBS_InitData->pSqiCnNordigP1;
    Init_Para_Temp.u16SqiCnNordigP1_Size=pDMD_DVBS_InitData->u16SqiCnNordigP1_Size;
    */
    
    AgcSsi_Para.pTuner_RfagcSsi=pDMD_DVBS_InitData->pTuner_RfagcSsi;
    AgcSsi_Para.u16Tuner_RfagcSsi_Size=pDMD_DVBS_InitData->u16Tuner_RfagcSsi_Size;
    AgcSsi_Para.pTuner_IfagcSsi_LoRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_LoRef;
    AgcSsi_Para.u16Tuner_IfagcSsi_LoRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcSsi_LoRef_Size;  
    AgcSsi_Para.pTuner_IfagcSsi_HiRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef;
    AgcSsi_Para.u16Tuner_IfagcSsi_HiRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcSsi_HiRef_Size;
    AgcSsi_Para.pTuner_IfagcErr_LoRef=pDMD_DVBS_InitData->pTuner_IfagcErr_LoRef;
    AgcSsi_Para.u16Tuner_IfagcErr_LoRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcErr_LoRef_Size;
    AgcSsi_Para.pTuner_IfagcSsi_HiRef=pDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef;
    AgcSsi_Para.u16Tuner_IfagcErr_HiRef_Size=pDMD_DVBS_InitData->u16Tuner_IfagcErr_HiRef_Size;

    Drv_DVBS_Init_PARAM.u32InitDataLen=sizeof(Init_Para_Temp);
    Drv_DVBS_Init_PARAM.pDMD_DVBS_InitData=&Init_Para_Temp;
    Drv_DVBS_Init_PARAM.ret=false;    
     
     if(u8DVBSUtopiaOpen == 0)  // First time open
    { 
        if(UtopiaOpen(MODULE_DVBS/*|KERNEL_MODE*/ , &ppDVBSInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //kernel space
        //if(UtopiaOpen(MODULE_DVBS , &ppDVBSInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //user space
        {
            u32DVBSopen = 1;
            ULOGD("DEMOD","Leo check DVBS utopia open sucessful!!\n");
      //return_val=true;
           // ULOGD("DEMOD","\r\n ======== DVBS Open Successful %x =========", (WORD)u32DVBSopen);
        }
        else
        {
          //  ULOGD("DEMOD","\r\n ======== DVBS Open Fail %x =========", (WORD)u32DVBSopen);
       //return_val=false;
          ULOGD("DEMOD","DVBS utopia open fail!!\n");
          return false;
        }

        u8DVBSUtopiaOpen = 1;
    }
    
   if(u32DVBSopen==1)
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_Init,&Drv_DVBS_Init_PARAM);
      else
        return false;

    return Drv_DVBS_Init_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_Exit(void)
{
    DVBS_EXIT_PARAM_PARAM Drv_DVBS_EXIT_PARAM_PARAM;
    Drv_DVBS_EXIT_PARAM_PARAM.ret=false;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_Exit\n"));

       if(u32DVBSopen==1)
        UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_Exit,&Drv_DVBS_EXIT_PARAM_PARAM);
      else
        return false;

    return Drv_DVBS_EXIT_PARAM_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_SetDbgLevel(DMD_DVBS_DbgLv u8DbgLevel)
{
    DVBS_SetDbgLevel_PARAM Drv_DVBS_SetDbgLevel_PARAM;
    Drv_DVBS_SetDbgLevel_PARAM.u8DbgLevel=u8DbgLevel;
    Drv_DVBS_SetDbgLevel_PARAM.ret=false;
  
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_SetDbgLevel\n"));
    if(u32DVBSopen==1)
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetDbgLevel,&Drv_DVBS_SetDbgLevel_PARAM);
    else
      return false;
      
    return Drv_DVBS_SetDbgLevel_PARAM.ret;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
const DMD_DVBS_Info* MDrv_DMD_DVBS_GetInfo(void)
{
  DVBS_GetInfo_PARAM Drv_DVBS_GetInfo_PARAM;
  Drv_DVBS_GetInfo_PARAM.ret_info=NULL;
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetInfo,&Drv_DVBS_GetInfo_PARAM);
  }
  else
  {
    return false;
  }


  return Drv_DVBS_GetInfo_PARAM.ret_info;

}
#endif

MS_BOOL MDrv_DMD_DVBS_GetLibVer(const MSIF_Version **ppVersion)
{
   DVBS_GetLibVer_PARAM Drv_DVBS_GetLibVer_PARAM;
   Drv_DVBS_GetLibVer_PARAM.ppVersion=ppVersion;
    Drv_DVBS_GetLibVer_PARAM.ret= false;


   if(u32DVBSopen==1)
   {
       UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetLibVer,&Drv_DVBS_GetLibVer_PARAM);
	    Drv_DVBS_GetLibVer_PARAM.ret= true;
   }
   else
   {
       return false;
   }
  
   return Drv_DVBS_GetLibVer_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_GetFWVer(MS_U16 *ver)
{
  DVBS_GetFWVer_PARAM Drv_DVBS_GetFWVer_PARAM;
  Drv_DVBS_GetFWVer_PARAM.ver=ver;
  Drv_DVBS_GetFWVer_PARAM.ret=false;
  
  if(u32DVBSopen==1)
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetFWVer,&Drv_DVBS_GetFWVer_PARAM);
  else
  {
    return false;
  }

       return Drv_DVBS_GetFWVer_PARAM.ret;

}

MS_BOOL MDrv_DMD_DVBS_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
#if defined(CHIP_K1)
  DVBS_GetDSPReg_PARAM Drv_DVBS_GetDSPReg_PARAM;
  Drv_DVBS_GetDSPReg_PARAM.u16Addr=u16Addr;
  Drv_DVBS_GetDSPReg_PARAM.pu8Data=pu8Data;
  
   if(u32DVBSopen==1)
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetDSPReg,&Drv_DVBS_GetDSPReg_PARAM);
  else
  {
    return false;
  }

  return Drv_DVBS_GetDSPReg_PARAM.ret;
#else
    ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBS_GetDSPReg\n");
  return FALSE;
#endif
}

MS_BOOL MDrv_DMD_DVBS_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data)
{
#if defined(CHIP_K1)
  DVBS_SetDSPReg_PARAM Drv_DVBS_SetDSPReg_PARAM;

  Drv_DVBS_SetDSPReg_PARAM.pu8Data=pu8Data;
  Drv_DVBS_SetDSPReg_PARAM.u16Addr=u16Addr;
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetDSPReg,&Drv_DVBS_SetDSPReg_PARAM);
  }
  else
  {
    return false;
  }

    return Drv_DVBS_SetDSPReg_PARAM.ret;
#else
    ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBS_SetDSPReg\n");
  return FALSE;
#endif
}


MS_BOOL MDrv_DMD_DVBS_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
  DVBS_GetReg_PARAM Drv_DVBS_GetReg_PARAM;
  Drv_DVBS_GetReg_PARAM.u16Addr=u16Addr;
  Drv_DVBS_GetReg_PARAM.pu8Data=pu8Data;
  Drv_DVBS_GetReg_PARAM.ret=false;

  if(u32DVBSopen==1)
    {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetReg,&Drv_DVBS_GetReg_PARAM);
   }
    else
    {
      return false;
    }

    return Drv_DVBS_GetReg_PARAM.ret;
  
}

MS_BOOL MDrv_DMD_DVBS_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
  DVBS_SetReg_PARAM Drv_DVBS_SetReg_PARAM;
  Drv_DVBS_SetReg_PARAM.u16Addr=u16Addr;
  Drv_DVBS_SetReg_PARAM.u8Data=u8Data;
  Drv_DVBS_SetReg_PARAM.ret=false; 

  if(u32DVBSopen==1)
    {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetReg,&Drv_DVBS_SetReg_PARAM);
   }
    else
    {
      return false;
    }

    return Drv_DVBS_SetReg_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_SetSerialControl(MS_BOOL bEnable)
{
      DVBS_SetSerialControl_PARAM Drv_DVBS_SetSerialControl_PARAM;
      Drv_DVBS_SetSerialControl_PARAM.bEnable=bEnable;
  Drv_DVBS_SetSerialControl_PARAM.ret=false;

  if(u32DVBSopen==1)
    {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetSerialControl,&Drv_DVBS_SetSerialControl_PARAM);
   }
    else
    {
      return false;
    }    

    
    return Drv_DVBS_SetSerialControl_PARAM.ret;
}

#ifdef UFO_DEMOD_BLINDSCAN_NEW_FLOW
#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_BlindScan_Config(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS)
{
    return MDrv_DMD_DVBS_BlindScan_Config_symbol_rate_list(u32SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, NULL, 0);
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_Config_symbol_rate_list(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{
  MS_U32   u32CurrentSR;
  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBS BlindScan_Config_symbol_rate_list!!\n");
  #endif

  DVBS_BlindScan_Config_Symbol_rate_list_PARAM Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.u32SymbolRate=u32SymbolRate;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.eQamMode=eQamMode;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.u32IFFreq=u32IFFreq;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.bSpecInv=bSpecInv;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.bSerialTS=bSerialTS;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.pu16_symbol_rate_list=pu16_symbol_rate_list;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.u8_symbol_rate_list_num=u8_symbol_rate_list_num;
  Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret=false;  
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_Config_symbol_rate_list,&Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM);
    g_dvbs_lock = 0;
    u8DemodLockFlag=0;
    u32CurrentSR = u32SymbolRate/1000;  //KHz
    Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, u32CurrentSR&0xff);
    Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, (u32CurrentSR>>8)&0xff);
    Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, (u32CurrentSR>>16)&0xff);
    ULOGD("DEMOD","drv_DMD_DVBS_BlindScan_Config u32SymbolRate = %d \n", u32CurrentSR);
  }
  else
  {
    return false;
  }

  return Drv_DVBS_BlindScan_Config_Symbol_rate_list_PARAM.ret;
}
#endif
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_SetConfig(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS)
{
  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBS SetConfig!!\n");
  #endif
  
   return MDrv_DMD_DVBS_SetConfig_symbol_rate_list(u32SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, NULL, 0);
}

MS_BOOL MDrv_DMD_DVBS_SetConfig_symbol_rate_list(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{ 
  MS_U32   u32CurrentSR;
  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBS SetConfig_symbol_rate_list!!\n");
  #endif

  DVBS_SetConfig_Symbol_rate_list_PARAM Drv_DVBS_SetConfig_Symbol_rate_list_PARAM;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.u16SymbolRate=u32SymbolRate;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.eQamMode=eQamMode;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.u32IFFreq=u32IFFreq;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.bSpecInv=bSpecInv;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.bSerialTS=bSerialTS;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.pu16_symbol_rate_list=pu16_symbol_rate_list;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.u8_symbol_rate_list_num=u8_symbol_rate_list_num;
  Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret=false;  
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetConfig_symbol_rate_list,&Drv_DVBS_SetConfig_Symbol_rate_list_PARAM);
    g_dvbs_lock = 0;
    u8DemodLockFlag=0;
    u32CurrentSR = u32SymbolRate/1000;  //KHz
    Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, u32CurrentSR&0xff);
    Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, (u32CurrentSR>>8)&0xff);
    Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, (u32CurrentSR>>16)&0xff);
    ULOGD("DEMOD","drv_DMD_DVBS_SetConfig u32SymbolRate = %d \n", (int)u32CurrentSR);
  }
  else
  {
    return false;
  }

  return Drv_DVBS_SetConfig_Symbol_rate_list_PARAM.ret;
}
#endif
MS_BOOL MDrv_DMD_DVBS_SetActive(MS_BOOL bEnable)
{
  DVBS_SetActive_PARAM Drv_DVBS_SetActive_PARAM;
  Drv_DVBS_SetActive_PARAM.bEnable=bEnable;
  Drv_DVBS_SetActive_PARAM.ret=false;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_SetActive\n"));

  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBS SetActive!!\n");
  #endif

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetActive,&Drv_DVBS_SetActive_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBS_SetActive_PARAM.ret;
}
#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetLock(DMD_DVBS_GETLOCK_TYPE eType, DMD_DVBS_LOCK_STATUS *eLockStatus)
{
    return MDrv_DMD_DVBS_GetLockWithRFPower(eType, eLockStatus, 200.0f, -200.0f);
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetLockWithRFPower(DMD_DVBS_GETLOCK_TYPE eType, DMD_DVBS_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm)
{
  MS_BOOL status = true;
  
  DMD_IFAGC_SSI   *ifagc_ssi;
  DMD_IFAGC_ERR   *ifagc_err;
  float   ch_power_rf=0.0f;
  float   ch_power_db=0.0f;
  float   ch_power_if=0.0f, ch_power_ifa = 0.0f, ch_power_ifb =0.0f;
  MS_U16  if_agc_val =0, if_agc_vala =0, if_agc_valb =0, if_agc_val_lsb =0, i;
  float   ch_power_takeover=0.0f;
  MS_U8   ssi_tbl_len = 0, err_tbl_len = 0;
  
  MS_U8 ifagc_reg;
  MS_U8 ifagc_reg_lsb;
  MS_U16 ifagc_err_reg;
    MS_U32      u32SymbolRate=0;
    //float       fSymbolRate;
    //MS_U8 ISSY_EN = 0;
    MS_U8 code_rate_idx = 0;
    MS_U8 pilot_flag = 0;
    MS_U8 fec_type_idx = 0;
    MS_U16 k_bch_array[2][11] ={
                {16008, 21408, 25728, 32208, 38688, 43040, 48408, 51648, 53840, 57472, 58192},
                { 3072,  5232,  6312,  7032,  9552, 10632, 11712, 12432, 13152, 14232,     0}};
    MS_U16 n_ldpc_array[2] = {64800, 16200};
    MS_FLOAT pilot_term = 0;
    MS_FLOAT k_bch;    
    MS_FLOAT n_ldpc;
    MS_FLOAT ts_div_num_offset = 2.0;
    //MS_U32 u32Time_start,u32Time_end;
    MS_U32 u32temp=0;
    //MS_FLOAT pkt_interval;
    MS_U8 system_type_reg=0;
    MS_U8 code_rate_reg=0;
    MS_FLOAT fTSDivNum;
    MS_U8 u8Data;

 
  DVBS_GetLockWithRFPower Drv_DVBS_GetLockWithRFPower;

  Drv_DVBS_GetLockWithRFPower.eType=eType;
  Drv_DVBS_GetLockWithRFPower.eLockStatus=eLockStatus;
  //Drv_DVBS_GetLockWithRFPower.u32CurrRFPowerDbm=(MS_U32)(fCurrRFPowerDbm*10);
  //Drv_DVBS_GetLockWithRFPower.u32NoChannelRFPowerDbm=(MS_U32)(fNoChannelRFPowerDbm*10);
  Drv_DVBS_GetLockWithRFPower.ret=false;
  
  DVBS_GetIFAGC_PARAM Drv_DVBS_GetIFAGC_PARAM;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_reg=&ifagc_reg;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_reg_lsb=&ifagc_reg_lsb;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_err_reg=&ifagc_err_reg;
  Drv_DVBS_GetIFAGC_PARAM.ret=false;

  DVBS_GetTsDivNum_PARAM Drv_DVBS_GetTsDivNum_PARAM;
  Drv_DVBS_GetTsDivNum_PARAM.u32SymbolRate=&u32SymbolRate;
  Drv_DVBS_GetTsDivNum_PARAM.system_type_reg=&system_type_reg;
  Drv_DVBS_GetTsDivNum_PARAM.code_rate_idx=&code_rate_idx;
  Drv_DVBS_GetTsDivNum_PARAM.fec_type_idx=&fec_type_idx;
  Drv_DVBS_GetTsDivNum_PARAM.pilot_flag=&pilot_flag;
  Drv_DVBS_GetTsDivNum_PARAM.u32temp=&u32temp;
  Drv_DVBS_GetTsDivNum_PARAM.code_rate_reg=&code_rate_reg;
  Drv_DVBS_GetTsDivNum_PARAM.bRet=false;
  
  if(u32DVBSopen==1)
  {
  	if((AgcSsi_Para.pTuner_IfagcSsi_HiRef != NULL) && (AgcSsi_Para.pTuner_IfagcSsi_LoRef != NULL))
    {
  	    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetIFAGC,&Drv_DVBS_GetIFAGC_PARAM);
        status &= Drv_DVBS_GetIFAGC_PARAM.ret;
    
        ch_power_rf = fCurrRFPowerDbm;
        if_agc_val = ifagc_reg;
        if_agc_val_lsb = ifagc_reg_lsb;
    
        ifagc_ssi = AgcSsi_Para.pTuner_IfagcSsi_LoRef;
        ssi_tbl_len = AgcSsi_Para.u16Tuner_IfagcSsi_LoRef_Size;
        ifagc_err = AgcSsi_Para.pTuner_IfagcErr_LoRef;
        err_tbl_len = AgcSsi_Para.u16Tuner_IfagcErr_LoRef_Size;   
    
        ch_power_if=ifagc_ssi[0].power_db;
        if (if_agc_val >=ifagc_ssi[0].agc_val)
        {
                for(i = 1; i < ssi_tbl_len; i++)
                {
                    if (if_agc_val < ifagc_ssi[i].agc_val)
                    {
                        if_agc_valb = ifagc_ssi[i].agc_val;
                        ch_power_ifb = ifagc_ssi[i].power_db;
        
                        i--;
                        if_agc_vala = ifagc_ssi[i].agc_val;
                        ch_power_ifa=ifagc_ssi[i].power_db;
                        while ((i>1) && (if_agc_vala==ifagc_ssi[i-1].agc_val))
                        {
                            ch_power_ifa=ifagc_ssi[i-1].power_db;
                            i--;
                        }
                        ch_power_if = ch_power_ifa+(ch_power_ifb-ch_power_ifa)*(float)((if_agc_val-if_agc_vala)*256+if_agc_val_lsb)/((if_agc_valb-if_agc_vala)*256);
                        break;
                    }
                }
        }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","if prev %f %x\n", ch_power_ifa, if_agc_vala);
            ULOGD("DEMOD","if next %f %x\n", ch_power_ifb, if_agc_valb);  
            #endif

            
            for(i = 0; i < ssi_tbl_len; i++)
            {
                if (ifagc_ssi[i].agc_val <= ifagc_ssi[i+1].agc_val)
                {
                    ch_power_takeover = ifagc_ssi[i+1].power_db;
                    break;
                }
            }

            #ifdef MS_DEBUG
            //ULOGD("DEMOD","ch_power_rf = %f\n", ch_power_rf);
            ULOGD("DEMOD","ch_power_if = %f\n", ch_power_if);
            ULOGD("DEMOD","ch_power_takeover = %f\n", ch_power_takeover);
            #endif

            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_rf : ch_power_if;

            if(ch_power_rf > (ch_power_takeover + 0.5))
            {
                ch_power_db = ch_power_rf;
            }
            else if(ch_power_if < (ch_power_takeover - 0.5))
            {
                ch_power_db = ch_power_if;
            }
            else
            {
                ch_power_db = (ch_power_if + ch_power_rf)/2;
            }
          
            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_if : ch_power_rf;
    
        if(if_agc_val == 0xff)
        {
            for(i = 0; i < err_tbl_len; i++)
            {
                    if ( ifagc_err_reg <= ifagc_err[i].agc_err )        // signed char comparison
                    {
                        ch_power_db += ifagc_err[i].attn_db;
                        break;
                    }
            }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","if_agc_err = 0x%x\n", ifagc_err_reg);
            #endif
        }
    }
    else
    {
    	  #ifdef MS_DEBUG
        if (fCurrRFPowerDbm>=100.0) // unreasonable input value, get RF level from RFAGG
        {
            ULOGD("DEMOD","Error!! please add AGC table\n");
        }
        #endif
        ch_power_db = fCurrRFPowerDbm;
    }
    
    Drv_DVBS_GetLockWithRFPower.u32CurrRFPowerDbm=(MS_U32)(ch_power_db);
    Drv_DVBS_GetLockWithRFPower.u32NoChannelRFPowerDbm=(MS_U32)(fNoChannelRFPowerDbm);
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetLockWithRFPower,&Drv_DVBS_GetLockWithRFPower);	
    status &= Drv_DVBS_GetLockWithRFPower.ret;
    if(u8DemodLockFlag==1)
    {
         UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetTsDivNum, &Drv_DVBS_GetTsDivNum_PARAM);
         status &= Drv_DVBS_GetTsDivNum_PARAM.bRet;
         if(!system_type_reg)//DVBS2
         {
                k_bch = k_bch_array[fec_type_idx][code_rate_idx];
                n_ldpc = n_ldpc_array[fec_type_idx];
                pilot_term = ((float) n_ldpc / modulation_order / 1440 * 36) * pilot_flag;
                if(sDMD_DVBS_Info.bSerialTS)//serial mode
                {
                    fTSDivNum =288000.0/(k_bch/((n_ldpc/modulation_order+90+pilot_term)/u32SymbolRate));
                    fTSDivNum = fTSDivNum/2-1;// since  288/(2(fTSDivNum+1)) = 288/TS_RATE = A  ==> fTSDivNum = A/2-1
                }
                else//parallel mode
                {
                    fTSDivNum = (288000.0/(k_bch/((n_ldpc/modulation_order+90+pilot_term)/u32SymbolRate)/8) - ts_div_num_offset);
                    fTSDivNum = (fTSDivNum-1)/2;
                }
                fTSDivNum-=ts_div_num_offset;

         }
         else                                            //S
         {
             switch (code_rate_reg)
             {
                case 0x00: //CR 1/2
                    if(sDMD_DVBS_Info.bSerialTS)
                      fTSDivNum = (288000.0/((1.0/2.0)*(188.0/204.0)*u32SymbolRate*2));
                    else
                      fTSDivNum = (288000.0/((1.0/2.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                    fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x01: //CR 2/3
                    if(sDMD_DVBS_Info.bSerialTS)
                      fTSDivNum = (MS_U8)(288000.0/((2.0/3.0)*(188.0/204.0)*u32SymbolRate*2));
                    else
                      fTSDivNum = (MS_U8)(288000.0/((2.0/3.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                    fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x02: //CR 3/4
                    if(sDMD_DVBS_Info.bSerialTS)
                      fTSDivNum = (288000.0/((3.0/4.0)*(188.0/204.0)*u32SymbolRate*2));
                    else
                      fTSDivNum = (288000.0/((3.0/4.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                      fTSDivNum = fTSDivNum/2-1-5;
                    break;
                case 0x03: //CR 5/6
                    if(sDMD_DVBS_Info.bSerialTS)
                      fTSDivNum = (288000.0/((5.0/6.0)*(188.0/204.0)*u32SymbolRate*2));
                    else
                      fTSDivNum = (288000.0/((5.0/6.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                    fTSDivNum = fTSDivNum/2-1-5;

                    break;
                case 0x04: //CR 7/8
                    if(sDMD_DVBS_Info.bSerialTS)
                      fTSDivNum =(288000.0/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*2));
                    else
                      fTSDivNum = (288000.0/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                    fTSDivNum = fTSDivNum/2-1-5;

                    break;
                default:
                    if(sDMD_DVBS_Info.bSerialTS)
                      fTSDivNum = (288000.0/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*2));
                    else
                      fTSDivNum = (288000.0/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                    fTSDivNum = fTSDivNum/2-1-5;

                    break;
             }
         } //printf("INTERN_DVBS_GetTsDivNum u8TSClk = 0x%x\n", *u8TSDivNum);
         u8Data = (MS_U8)fTSDivNum;
	  #ifdef MS_DEBUG
         ULOGD(">>>INTERN_DVBS_GetLock TsClkDivNum = 0x%x<<<\n", u8Data);
         #endif
         if (u8Data > 0x1F) 
             u8Data=0x1F;
             //if (u8Data < 0x05) u8Data=0x05;
             HAL_DMD_RIU_WriteByte(0x103300, u8Data);
                        
             //Ts Output Enable
             HAL_DMD_RIU_WriteByte(0x101eaa,0x10);
    }
    	
  }
  else
  {
    return false;
  }

  #ifdef MS_DEBUG 
  //if(*(Drv_DVBS_GetLockWithRFPower_Transform.eLockStatus)==DMD_DVBS_LOCK)
  if(*(Drv_DVBS_GetLockWithRFPower.eLockStatus)==DMD_DVBS_LOCK)
  {
    ULOGD("DEMOD","check in drv layer DVBS demod locked!!\n");
  }
  else
  {
    ULOGD("DEMOD","check in drv layer DVBS demod unlock!!\n");
    return false;
  }
  #endif

  return status;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_DVBS_GetSignalStrengthWithRFPower(u16Strength, 200.0f);
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
{ 
  MS_BOOL status = true;

  DMD_IFAGC_SSI   *ifagc_ssi;
  DMD_IFAGC_ERR   *ifagc_err;
  float   ch_power_rf=0.0f;
  float   ch_power_db=0.0f, ch_power_db_rel=0.0f;
  float   ch_power_if=0.0f, ch_power_ifa = 0.0f, ch_power_ifb =0.0f;
  MS_U16  if_agc_val =0, if_agc_vala =0, if_agc_valb =0, if_agc_val_lsb =0, i;
  float   ch_power_takeover=0.0f;
  MS_U8   ssi_tbl_len = 0, err_tbl_len = 0;
  
  MS_U8 ifagc_reg;
  MS_U8 ifagc_reg_lsb;
  MS_U16 ifagc_err_reg;

 // float   ch_power_db=0.0f, ch_power_db_rel=0.0f;
  DMD_DVBS_DEMOD_TYPE pDemodType;
  MS_U8  u8_DVBS2_CurrentCodeRateLocal;
  MS_U8  u8_DVBS2_CurrentConstellationLocal;
  //MS_U16 u16Data=0;
  //MS_U8 u8Index;
  //DMD_DVBS_MODULATION_TYPE Qam_mode;
 // MS_U16 SymbolRate;
 // float FreqOff;
  
  //DVBS_GetSignalStrengthWithRFPower_PARAM Drv_DVBS_GetSignalStrengthWithRFPower_PARAM;
  //Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.u16Strength=u16Strength;
  //Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.fRFPowerDbm=fRFPowerDbm;

  DVBS_GetIFAGC_PARAM Drv_DVBS_GetIFAGC_PARAM;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_reg=&ifagc_reg;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_reg_lsb=&ifagc_reg_lsb;
  Drv_DVBS_GetIFAGC_PARAM.ifagc_err_reg=&ifagc_err_reg;
  Drv_DVBS_GetIFAGC_PARAM.ret=false;

/*
  DVBS_GetTunrSignalLevel_PWR_PARAM Drv_DVBS_GetTunrSignalLevel_PWR_PARAM;
  Drv_DVBS_GetTunrSignalLevel_PWR_PARAM.u16Data=&u16Data;
  Drv_DVBS_GetTunrSignalLevel_PWR_PARAM.bRet=false;
  */
  DVBS_GetSignalStrengthWithRFPower_PARAM Drv_DVBS_GetSignalStrengthWithRFPower_PARAM;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.fRFPowerDbm=(MS_U16)fRFPowerDbm;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.pDemodType=&pDemodType;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.u8_DVBS2_CurrentCodeRateLocal=&u8_DVBS2_CurrentCodeRateLocal;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.u8_DVBS2_CurrentConstellationLocal=&u8_DVBS2_CurrentConstellationLocal;
  Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.ret=false;
  
  if(u32DVBSopen==1)
  {

    if((AgcSsi_Para.pTuner_IfagcSsi_HiRef != NULL) && (AgcSsi_Para.pTuner_IfagcSsi_LoRef != NULL))
    {
    	  UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetIFAGC,&Drv_DVBS_GetIFAGC_PARAM);
        status &= Drv_DVBS_GetIFAGC_PARAM.ret;
        ch_power_rf = fRFPowerDbm;
    
        if_agc_val = ifagc_reg;
        if_agc_val_lsb = ifagc_reg_lsb;
    
        ifagc_ssi = AgcSsi_Para.pTuner_IfagcSsi_LoRef;
        ssi_tbl_len = AgcSsi_Para.u16Tuner_IfagcSsi_LoRef_Size;
        ifagc_err = AgcSsi_Para.pTuner_IfagcErr_LoRef;
        err_tbl_len = AgcSsi_Para.u16Tuner_IfagcErr_LoRef_Size;

        ch_power_if=ifagc_ssi[0].power_db;
        if (if_agc_val >=ifagc_ssi[0].agc_val)
        {
                for(i = 1; i < ssi_tbl_len; i++)
                {
                    if (if_agc_val < ifagc_ssi[i].agc_val)
                    {
                        if_agc_valb = ifagc_ssi[i].agc_val;
                        ch_power_ifb = ifagc_ssi[i].power_db;
        
                        i--;
                        if_agc_vala = ifagc_ssi[i].agc_val;
                        ch_power_ifa=ifagc_ssi[i].power_db;
                        while ((i>1) && (if_agc_vala==ifagc_ssi[i-1].agc_val))
                        {
                            ch_power_ifa=ifagc_ssi[i-1].power_db;
                            i--;
                        }
                        ch_power_if = ch_power_ifa+(ch_power_ifb-ch_power_ifa)*(float)((if_agc_val-if_agc_vala)*256+if_agc_val_lsb)/((if_agc_valb-if_agc_vala)*256);
                        break;
                    }
                }
        }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","if prev %f %x\n", ch_power_ifa, if_agc_vala);
            ULOGD("DEMOD","if next %f %x\n", ch_power_ifb, if_agc_valb);  
            #endif
            
            for(i = 0; i < ssi_tbl_len; i++)
            {
                if (ifagc_ssi[i].agc_val <= ifagc_ssi[i+1].agc_val)
                {
                    ch_power_takeover = ifagc_ssi[i+1].power_db;
                    break;
                }
            }

            #ifdef MS_DEBUG
            ULOGD("DEMOD","ch_power_rf = %f\n", ch_power_rf);
            ULOGD("DEMOD","ch_power_if = %f\n", ch_power_if);
            ULOGD("DEMOD","ch_power_takeover = %f\n", ch_power_takeover);
            #endif

            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_rf : ch_power_if;

            if(ch_power_rf > (ch_power_takeover + 0.5))
            {
                ch_power_db = ch_power_rf;
            }
            else if(ch_power_if < (ch_power_takeover - 0.5))
            {
                ch_power_db = ch_power_if;
            }
            else
            {
                ch_power_db = (ch_power_if + ch_power_rf)/2;
            }
          
            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_if : ch_power_rf;
    
        if(if_agc_val == 0xff)
        {
            for(i = 0; i < err_tbl_len; i++)
            {
                    if ( ifagc_err_reg <= ifagc_err[i].agc_err )        // signed char comparison
                    {
                        ch_power_db += ifagc_err[i].attn_db;
                        break;
                    }
            }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","if_agc_err = 0x%x\n", ifagc_err_reg);
           #endif
        }
    }
    else
    {
    	  #ifdef MS_DEBUG
        if (fRFPowerDbm>=100.0) // unreasonable input value, get RF level from RFAGG
        {
            ULOGD("DEMOD","Error!! please add AGC table\n");
        }
        #endif
        ch_power_db = fRFPowerDbm;
    }
    /*
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetTunrSignalLevel_PWR,&Drv_DVBS_GetTunrSignalLevel_PWR_PARAM);
        status &= Drv_DVBS_GetTunrSignalLevel_PWR_PARAM.bRet;
    if (u8DemodLockFlag == 0 || status == FALSE)//Demod unlock
    {
        ch_power_db = 0;
	 DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]INTERN_DVBS GetSignalStrength failed\n"));
    }
    else
    {
        for (u8Index=0; u8Index < (sizeof(_u16SignalLevel)/sizeof(_u16SignalLevel[0])); u8Index++)
        {
            if ((65535 - u16Data) <= _u16SignalLevel[u8Index][0])
            {
                if (u8Index >=1)
                {
                    ch_power_db = (float)(_u16SignalLevel[u8Index][1])+((float)(_u16SignalLevel[u8Index][0] - (65535 - u16Data)) / (float)(_u16SignalLevel[u8Index][0] - _u16SignalLevel[u8Index-1][0]))*(float)(_u16SignalLevel[u8Index-1][1] - _u16SignalLevel[u8Index][1]);
                }
                else
                {
                    ch_power_db = _u16SignalLevel[u8Index][1];
                }
                break;
            }
        }
//----------
        if (ch_power_db >= 350)
            ch_power_db = ch_power_db - 35;
        else if ((ch_power_db < 350) && (ch_power_db >= 250))
            ch_power_db = ch_power_db - 25;
        else
            ch_power_db = ch_power_db - 5;

        if (ch_power_db < 0)
            ch_power_db = 0;
        if (ch_power_db > 920)
            ch_power_db = 920;

        ch_power_db = (-1.0)*(ch_power_db/10.0);
        //DBG_INTERN_DVBS(printf("INTERN_DVBS GetSignalStrength %f\n", ch_power_db));
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]INTERN_DVBS GetSignalStrength %f\n", ch_power_db));
    }
    */
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetSignalStrengthWithRFPower,&Drv_DVBS_GetSignalStrengthWithRFPower_PARAM);
    status &= Drv_DVBS_GetSignalStrengthWithRFPower_PARAM.ret;
    if((MS_U8)pDemodType == (MS_U8)DMD_SAT_DVBS)//S
    {
        float fDVBS_SSI_Pref[]=
        {
            //0,       1,       2,       3,       4
            -78.9,   -77.15,  -76.14,  -75.19,  -74.57,//QPSK
        };
        //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE + 0x84, &u8Data);
        //u8_DVBS2_CurrentCodeRateLocal = (u8Data & 0x07);
        ch_power_db_rel = ch_power_db - fDVBS_SSI_Pref[u8_DVBS2_CurrentCodeRateLocal];
    }
    else
    {
        float fDVBS2_SSI_Pref[][11]=
        {
            //  0,    1,       2,       3,       4,       5,       6,       7,       8,        9,       10
            //1/4,    1/3,     2/5,     1/2,     3/5,     2/3,     3/4,     4/5,     5/6,      8/9,     9/10
            {-85.17, -84.08,  -83.15,  -81.86,  -80.63,  -79.77,  -78.84,  -78.19,  -77.69,   -76.68,  -76.46}, //QPSK
            {   0.0,    0.0,     0.0,     0.0,  -77.36,  -76.24,  -74.95,     0.0,  -73.52,   -72.18,  -71.84}  //8PSK
        };
        /*
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD7, &u8Data);
        u8_DVBS2_CurrentCodeRateLocal = (u8Data & 0x3C)>>2;

        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD7, &u8Data);
       status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD6, &u8Data2);

        if(((u8Data & 0x03)==0x01) && ((u8Data2 & 0x80)==0x00))
        {
            _u8_DVBS2_CurrentConstellationLocal = DMD_DVBS_QPSK;
        }
        else if (((u8Data & 0x03)==0x01) && ((u8Data2 & 0x80)==0x80))
        {
            _u8_DVBS2_CurrentConstellationLocal = DMD_DVBS_8PSK;//8PSK
        }
        */
        ch_power_db_rel = ch_power_db - fDVBS2_SSI_Pref[u8_DVBS2_CurrentConstellationLocal][u8_DVBS2_CurrentCodeRateLocal];
    }

    if(ch_power_db_rel <= -15.0f)
    {
        *u16Strength= 0;
    }
    else if (ch_power_db_rel <= 0.0f)
    {
        *u16Strength = (MS_U16)(2.0f/3 * (ch_power_db_rel+15.0f));
    }
    else if (ch_power_db_rel <= 20.0f)
    {
        *u16Strength = (MS_U16)(4.0f * ch_power_db_rel + 10.0f);
    }
    else if (ch_power_db_rel <= 35.0f)
    {
        *u16Strength = (MS_U16)(2.0f/3 * (ch_power_db_rel-20.0f) + 90.0);
    }
    else
    {
        *u16Strength = 100;
    }

    //printf("SSI_CH_PWR(dB) = %f \n", ch_power_db_rel);
    //DBG_INTERN_DVBS(printf(">>>>>Signal Strength(SSI) = %d\n", (int)*u16Strength));
	ULOGD("DEMOD","Signal Strength(SSI) = %d \n", (int)*u16Strength);
	sDMD_DVBS_Info.u16Strength=*u16Strength;
  }

  else
  {
    return false;
  }

  return status;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSignalQuality(MS_U16 *u16Quality)
{
    return MDrv_DMD_DVBS_GetSignalQualityWithRFPower(u16Quality, 200.0f);
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
    MS_BOOL bRet;
    float       fber = 0.0;
    //float       log_ber;
    MS_BOOL     status = TRUE;
    float       f_snr = 0.0, ber_sqi = 0.0, cn_rel = 0.0;
    //MS_U8       u8Data =0;
    DMD_DVBS_CODE_RATE_TYPE       _u8_DVBS2_CurrentCodeRateLocal ;
    MS_U16     bchpkt_error,BCH_Eflag2_Window;
    //fRFPowerDbm = fRFPowerDbm;
    float snr_poly =0.0;
    float Fixed_SNR =0.0;
    double eFlag_PER=0.0;
	

  if(u32DVBSopen==1)
  {
    if (u8DemodLockFlag == 1)	
   // if (TRUE == INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK, 200.0f, -200.0f, 0))
    {
        if(_bDemodType)  //S2
        {

           MDrv_DMD_DVBS_GetSNR(&f_snr);
           snr_poly = 0.005261367463671*pow(f_snr, 3)-0.116517828301214*pow(f_snr, 2)+0.744836970505452*pow(f_snr, 1)-0.86727609780167;
           Fixed_SNR = f_snr + snr_poly;

           if (Fixed_SNR < 17.0)
              Fixed_SNR = Fixed_SNR;
           else if ((Fixed_SNR < 20.0) && (Fixed_SNR >= 17.0))
              Fixed_SNR = Fixed_SNR - 0.8;
           else if ((Fixed_SNR < 22.5) && (Fixed_SNR >= 20.0))
              Fixed_SNR = Fixed_SNR - 2.0;
           else if ((Fixed_SNR < 27.0) && (Fixed_SNR >= 22.5))
              Fixed_SNR = Fixed_SNR - 3.0;
           else if ((Fixed_SNR < 29.0) && (Fixed_SNR >= 27.0))
              Fixed_SNR = Fixed_SNR - 3.5;
           else if (Fixed_SNR >= 29.0)
              Fixed_SNR = Fixed_SNR - 3.0;


           if (Fixed_SNR < 1.0)
              Fixed_SNR = 1.0;
           if (Fixed_SNR > 30.0)
              Fixed_SNR = 30.0;

            //BCH EFLAG2_Window,  window size 0x2000
            BCH_Eflag2_Window=0x2000;
            MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2FEC_REG_BASE + 0x25*2 + 1, (BCH_Eflag2_Window>>8));
            MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2FEC_REG_BASE + 0x25*2 , (BCH_Eflag2_Window&0xff));
            MDrv_DMD_DVBS_GetPacketErr(&bchpkt_error);
            eFlag_PER = (float)(bchpkt_error)/(float)(BCH_Eflag2_Window);
            if(eFlag_PER>0)
              fber = 0.089267531133002*pow(eFlag_PER, 2) + 0.019640560289510*eFlag_PER + 0.0000001;
            else
              fber = 0;

#ifdef MSOS_TYPE_LINUX
                    //log_ber = ( - 1) *log10f(1 / fber);
                    if (fber > 1.0E-1)
                        ber_sqi = (log10f(1.0f/fber))*20.0f + 8.0f;
                    else if(fber > 8.5E-7)
                        ber_sqi = (log10f(1.0f/fber))*20.0f - 30.0f;
                    else
                        ber_sqi = 100.0;
#else
                    //log_ber = ( - 1) *Log10Approx(1 / fber);
                    if (fber > 1.0E-1)
                        ber_sqi = (Log10Approx(1.0f/fber))*20.0f + 8.0f;
                    else if(fber > 8.5E-7)
                        ber_sqi = (Log10Approx(1.0f/fber))*20.0f - 30.0f;
                    else
                        ber_sqi = 100.0;

#endif

            *u16Quality = Fixed_SNR/30*ber_sqi;
            //DBG_INTERN_DVBS(printf(" Fixed_SNR %f\n",Fixed_SNR));
            //DBG_INTERN_DVBS(printf(" BCH_Eflag2_Window %d\n",BCH_Eflag2_Window));
            //DBG_INTERN_DVBS(printf(" eFlag_PER [%f]\n fber [%8.3e]\n ber_sqi [%f]\n",eFlag_PER,fber,ber_sqi));
        }
        else  //S
        {
            if (MDrv_DMD_DVBS_GetPostViterbiBer(&fber) == FALSE)//ViterbiBer
            {         
                ULOGD("DEMOD","GetPostViterbiBer Fail! \n");
                bRet= FALSE;
            }
            //_fPostBer=fber;


            if (status==FALSE)
            {
                ULOGD("DEMOD","Maserati_GetSignalQuality GetPostViterbiBer Fail! \n");
                bRet= FALSE;
            }
            float fDVBS_SQI_CNref[]=
            {   //0,    1,    2,    3,    4
                4.2,   5.9,  6,  6.9,  7.5,//QPSK
            };

           DVBS_GetCurrentDemodCodeRate_PARAM Drv_DVBS_GetCurrentDemodCodeRate_PARAM;
           Drv_DVBS_GetCurrentDemodCodeRate_PARAM.pCodeRate=&_u8_DVBS2_CurrentCodeRateLocal;
           Drv_DVBS_GetCurrentDemodCodeRate_PARAM.bRet=false;
	   UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetCurrentDemodCodeRate,&Drv_DVBS_GetCurrentDemodCodeRate_PARAM);
#if 0
#ifdef MSOS_TYPE_LINUX
            log_ber = ( - 1.0f) *log10f(1.0f / fber);           //BY modify
#else
            log_ber = ( - 1.0f) *Log10Approx(1.0f / fber);      //BY modify
#endif
            DBG_INTERN_DVBS(printf("\nLog(BER) = %f\n",log_ber));
#endif
            if (fber > 2.5E-2)
                ber_sqi = 0.0;
            else if(fber > 8.5E-7)
#ifdef MSOS_TYPE_LINUX
                ber_sqi = (log10f(1.0f/fber))*20.0f - 32.0f; //40.0f;
#else
                ber_sqi = (Log10Approx(1.0f/fber))*20.0f - 32.0f;//40.0f;
#endif
            else
                ber_sqi = 100.0;

            status &= MDrv_DMD_DVBS_GetSNR(&f_snr);
            ULOGD("DEMOD","drv_DVBS_GetSNR = %d \n", (int)f_snr );
            cn_rel = f_snr - fDVBS_SQI_CNref[_u8_DVBS2_CurrentCodeRateLocal];
			
            ULOGD("DEMOD","fber = %f\n", fber );
            ULOGD("DEMOD","f_snr = %f\n", f_snr );
            ULOGD("DEMOD","cn_nordig_s1 = %f\n", fDVBS_SQI_CNref[_u8_DVBS2_CurrentCodeRateLocal] );
	     ULOGD("DEMOD","cn_rel = %f\n", cn_rel );
            ULOGD("DEMOD","ber_sqi = %f\n", ber_sqi );

            if (cn_rel < -7.0f)
            {
                *u16Quality = 0;
            }
            else if (cn_rel < 3.0)
            {
                *u16Quality = (MS_U16)(ber_sqi*((cn_rel - 3.0)/10.0 + 1.0));
            }
            else
            {
                *u16Quality = (MS_U16)ber_sqi;
            }


        }
            //INTERN_DVBS_GetTunrSignalLevel_PWR();//For Debug.
            ULOGD("DEMOD","Signal Quility(SQI) = %d\n", *u16Quality );
            bRet= TRUE;
    }
    else
    {
        *u16Quality = 0;
    }
     bRet= TRUE;
     sDMD_DVBS_Info.u16Quality=*u16Quality;
  }
  else
  {
    return false;
  }
  return bRet;
}
#endif

#ifdef CHIP_KAISER
MS_BOOL MDrv_DMD_DVBS_ActiveDmdSwitch(MS_U8 demod_no)
{
  DVBS_ActiveDmdSwitch_PARAM Drv_DVBS_ActiveDmdSwitch_PARAM;
  Drv_DVBS_ActiveDmdSwitch_PARAM.demod_no=demod_no;

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_ActiveDmdSwitch,&Drv_DVBS_ActiveDmdSwitch_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBS_ActiveDmdSwitch_PARAM.ret;
 }
#else
MS_BOOL MDrv_DMD_DVBS_ActiveDmdSwitch(MS_U8 demod_no)
{
  ULOGD("DEMOD","Doesn't support DVBS_ActiveDmdSwitch function!!!\n");
  return false;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetSNR(float *fSNR)
{
    MS_U32 u32NDA_SNR_A =0;
    MS_U32 u32NDA_SNR_AB =0;
    float NDA_SNR_A =0.0;
    float NDA_SNR_AB =0.0;
    float NDA_SNR =0.0;
    double NDA_SNR_LINEAR=0.0;

  DVBS_GetSNR_PARAM Drv_DVBS_GetSNR_PARAM;
  Drv_DVBS_GetSNR_PARAM.u32NDA_SNR_A=&u32NDA_SNR_A;
  Drv_DVBS_GetSNR_PARAM.u32NDA_SNR_AB=&u32NDA_SNR_AB;
  Drv_DVBS_GetSNR_PARAM.ret = false;

  if(u32DVBSopen==1)
  {
     if  (u8DemodLockFlag == 0)
    {
    	Drv_DVBS_GetSNR_PARAM.ret=FALSE;
    }
    else
    {
       UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetSNR,&Drv_DVBS_GetSNR_PARAM);
    }

     if (Drv_DVBS_GetSNR_PARAM.ret== FALSE)
    {
       ULOGD("DEMOD","DVBS_GetSNR Fail! \n");
    }
    else
    {
    //NDA SNR
    NDA_SNR_A=(float)u32NDA_SNR_A/65536;
    NDA_SNR_AB=(float)u32NDA_SNR_AB/4194304;
    //
    //since support 16,32APSK we need to add judgement
    if(modulation_order==4)
        NDA_SNR_AB=(float)sqrt(NDA_SNR_AB/(2-1.252295758529242));//for 16APSK CR2/3
    else if(modulation_order==5)//(2-1.41333232789)
        NDA_SNR_AB=(float)sqrt(NDA_SNR_AB/(2-1.41333232789));//for 32APSK CR3/4
    else
        NDA_SNR_AB=(float)sqrt(NDA_SNR_AB);

    NDA_SNR_LINEAR =(1/((NDA_SNR_A/NDA_SNR_AB)-1)) ;

    if(NDA_SNR_LINEAR<=0)
        NDA_SNR=1.0;
    else
         NDA_SNR=10*log10(NDA_SNR_LINEAR);

    //printf("[DVBS]: NDA_SNR ================================: %.1f\n", NDA_SNR);
    //_f_DVBS_CurrentSNR = NDA_SNR;
    /*
        //[DVBS/S2, QPSK/8PSK, 1/2~9/10 the same CN]
        snr_poly = 0.0;     //use Polynomial curve fitting to fix SNR
        snr_poly = 0.005261367463671*pow(NDA_SNR, 3)-0.116517828301214*pow(NDA_SNR, 2)+0.744836970505452*pow(NDA_SNR, 1)-0.86727609780167;
        Fixed_SNR = NDA_SNR + snr_poly;
        //printf("[DVBS]: NDA_SNR + snr_poly =====================: %.1f\n", Fixed_SNR);

        if (Fixed_SNR < 17.0)
            Fixed_SNR = Fixed_SNR;
        else if ((Fixed_SNR < 20.0) && (Fixed_SNR >= 17.0))
            Fixed_SNR = Fixed_SNR - 0.8;
        else if ((Fixed_SNR < 22.5) && (Fixed_SNR >= 20.0))
            Fixed_SNR = Fixed_SNR - 2.0;
        else if ((Fixed_SNR < 27.0) && (Fixed_SNR >= 22.5))
            Fixed_SNR = Fixed_SNR - 3.0;
        else if ((Fixed_SNR < 29.0) && (Fixed_SNR >= 27.0))
            Fixed_SNR = Fixed_SNR - 3.5;
        else if (Fixed_SNR >= 29.0)
            Fixed_SNR = Fixed_SNR - 3.0;

        if (Fixed_SNR < 1.0)
            Fixed_SNR = 1.0;
        if (Fixed_SNR > 30.0)
            Fixed_SNR = 30.0;
    */
    *fSNR = NDA_SNR;
    #ifdef MS_DEBUG
     ULOGD("[DVBS]: NDA_SNR=============================: %.1f\n", NDA_SNR);
    #endif
    	}

	Drv_DVBS_GetSNR_PARAM.ret = true;
  }
  else
  {
    return false;
  }   

  return Drv_DVBS_GetSNR_PARAM.ret;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetPostViterbiBer(float *ber)
{
  MS_U16 BitErrPeriod_reg;
  MS_U32 BitErr_reg;
  
  DVBS_GetPostViterbiBer_PARAM Drv_DVBS_GetPostViterbiBer_PARAM;
  Drv_DVBS_GetPostViterbiBer_PARAM.BitErr_reg=&BitErr_reg;
  Drv_DVBS_GetPostViterbiBer_PARAM.BitErrPeriod_reg=&BitErrPeriod_reg;
  Drv_DVBS_GetPostViterbiBer_PARAM.ret=false; 
  

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetPostViterbiBer,&Drv_DVBS_GetPostViterbiBer_PARAM);
    if (BitErrPeriod_reg == 0 )    //PRD
        BitErrPeriod_reg = 1;

    if (BitErr_reg <= 0 )
        *ber = 0.5f / ((float)BitErrPeriod_reg*128*188*8);
    else
        *ber = (float)BitErr_reg / ((float)BitErrPeriod_reg*128*188*8);

    if (*ber <= 0.0f)
        *ber = 1.0e-10f;

    DMD_DBG(ULOGD("DEMOD","PostVitBER = %8.3e \n ", *ber));
	Drv_DVBS_GetPostViterbiBer_PARAM.ret=true; 
  }
  else
  {
    return false;
  }   


  return Drv_DVBS_GetPostViterbiBer_PARAM.ret;
}
#endif

MS_BOOL MDrv_DMD_DVBS_GetPacketErr(MS_U16 *pktErr)
{
    DVBS_GetPacketErr_PARAM Drv_DVBS_GetPacketErr_PARAM;
    Drv_DVBS_GetPacketErr_PARAM.pktErr=pktErr;
  Drv_DVBS_GetPacketErr_PARAM.ret=false;

    if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetPacketErr,&Drv_DVBS_GetPacketErr_PARAM);
  }
  else
  {
    return false;
  }   

  return Drv_DVBS_GetPacketErr_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBS_GetCellID(MS_U16 *u16CellID)
{
  DVBS_GetCellID_PARAM Drv_DVBS_GetCellID_PARAM;
  Drv_DVBS_GetCellID_PARAM.u16CellID=u16CellID;
  Drv_DVBS_GetCellID_PARAM.ret=false;

  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetCellID,&Drv_DVBS_GetCellID_PARAM);
  }
  else
  {
    return false;
  }   

  return Drv_DVBS_GetCellID_PARAM.ret;
  
}


#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_GetStatus(DMD_DVBS_MODULATION_TYPE *pQAMMode, MS_U32 *u32SymbolRate, float *pFreqOff)
{
  MS_S16 s16CFO=0;
  
  DVBS_GetStatus_PARAM Drv_DVBS_GetStatus_PARAM;
  Drv_DVBS_GetStatus_PARAM.pQAMMode=pQAMMode;
  Drv_DVBS_GetStatus_PARAM.u32SymbolRate=u32SymbolRate;
  Drv_DVBS_GetStatus_PARAM.s16CFO=&s16CFO;
  Drv_DVBS_GetStatus_PARAM.ret=false;
  
  //float f_Fc, FreqCfo_offset;
  
  if(u32DVBSopen==1)
  {
    UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_GetStatus,&Drv_DVBS_GetStatus_PARAM);
      if(abs(s16CFO)%1000 >= 500)
      {
    	   if(s16CFO < 0)
    		*pFreqOff=(s16CFO/1000)-1.0;
    	   else
    		*pFreqOff=(s16CFO/1000)+1.0;	
      }
      else
    	   *pFreqOff = s16CFO/1000;
    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetStatus %d %d %f \n", *pQAMMode, *u32SymbolRate, *pFreqOff);
    }
    #endif
  }
  else
  {
    return false;
  }   

  return Drv_DVBS_GetStatus_PARAM.ret; 
}
#endif

MS_U32 MDrv_DMD_DVBS_SetPowerState(EN_POWER_MODE u16PowerState)
{
    DVBS_SetPowerState_PARAM Drv_DVBS_SetPowerState_PARAM;
    Drv_DVBS_SetPowerState_PARAM.u16PowerState=u16PowerState;
    Drv_DVBS_SetPowerState_PARAM.ret_U32=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_SetPowerState\n"));
   if(u32DVBSopen==1)
   {
  UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_SetPowerState,&Drv_DVBS_SetPowerState_PARAM);
   }
   else
   {
  return false;
    }   

    return Drv_DVBS_SetPowerState_PARAM.ret_U32;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
{
    MS_S16 s16CFO=0;
    DVBS_Get_FreqOffset_PARAM Drv_DVBS_Get_FreqOffset_PARAM;
    Drv_DVBS_Get_FreqOffset_PARAM.s16CFO=&s16CFO;
    Drv_DVBS_Get_FreqOffset_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_Get_FreqOffset\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_Get_FreqOffset,&Drv_DVBS_Get_FreqOffset_PARAM);
      if(abs(s16CFO)%1000 >= 500)
      {
    	   if(s16CFO < 0)
    		*pFreqOff=(s16CFO/1000)-1.0;
    	   else
    		*pFreqOff=(s16CFO/1000)+1.0;	
      }
      else
    	   *pFreqOff = s16CFO/1000;
	  #ifdef MS_DEBUG
	   ULOGD("DEMOD","MDrv_DMD_DVBS_Get_FreqOffset %f \n", *pFreqOff);
         #endif 
          //DBG_INTERN_DVBS(printf(">>> INTERN_DVBS_Get_FreqOffset *pFreqOff = %d[MHz] <<<\n", (MS_S16)*pFreqOff));
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_Get_FreqOffset_PARAM.bRet;
}
#endif

MS_BOOL MDrv_DMD_DVBS_BlindScan_Start(MS_U16 u16StartFreq, MS_U16 u16EndFreq)
{
    DVBS_BlindScan_Start_PARAM Drv_DVBS_BlindScan_Start_PARAM;
    Drv_DVBS_BlindScan_Start_PARAM.u16StartFreq=u16StartFreq;
    Drv_DVBS_BlindScan_Start_PARAM.u16EndFreq=u16EndFreq;
    Drv_DVBS_BlindScan_Start_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_Start\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_Start,&Drv_DVBS_BlindScan_Start_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_Start_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd)
{
    DVBS_BlindScan_NextFreq_PARAM Drv_DVBS_BlindScan_NextFreq_PARAM;
    Drv_DVBS_BlindScan_NextFreq_PARAM.bBlindScanEnd=bBlindScanEnd;
    Drv_DVBS_BlindScan_NextFreq_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_NextFreq\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_NextFreq,&Drv_DVBS_BlindScan_NextFreq_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_NextFreq_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_Cancel(void)
{
    DVBS_BlindScan_Cancel_PARAM Drv_DVBS_BlindScan_Cancel_PARAM;
    Drv_DVBS_BlindScan_Cancel_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_Cancel\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_Cancel,&Drv_DVBS_BlindScan_Cancel_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_Cancel_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_End(void)
{
    DVBS_BlindScan_End_PARAM Drv_DVBS_Blindscan_End_PARAM;
    Drv_DVBS_Blindscan_End_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_End\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_End,&Drv_DVBS_Blindscan_End_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_Blindscan_End_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_GetChannel(MS_U16 u16ReadStart, MS_U16* u16TPNum, HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    DVBS_BlindScan_GetChannel_PARAM Drv_DVBS_BlindScan_Getchannel_PARAM;
    Drv_DVBS_BlindScan_Getchannel_PARAM.u16ReadStart=u16ReadStart;
    Drv_DVBS_BlindScan_Getchannel_PARAM.u16TPNum=u16TPNum;
    Drv_DVBS_BlindScan_Getchannel_PARAM.pTable=pTable;
    Drv_DVBS_BlindScan_Getchannel_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_GetChannel\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_GetChannel,&Drv_DVBS_BlindScan_Getchannel_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_Getchannel_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFreq)
{
    DVBS_BlindScan_GetCurrentFreq_PARAM Drv_DVBS_BlindScan_GetCurrentFreq_PARAM;
    Drv_DVBS_BlindScan_GetCurrentFreq_PARAM.u32CurrentFreq=u32CurrentFreq;
    Drv_DVBS_BlindScan_GetCurrentFreq_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_GetCurrentFreq\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_GetCurrentFreq,&Drv_DVBS_BlindScan_GetCurrentFreq_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_GetCurrentFreq_PARAM.bRet;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    MS_U8 substate_reg=2;
    MS_U32 u32Data=0;
    MS_U16 symbolrate_reg=0;
    MS_U16 CFO_reg;
    DVBS_BlindScan_WaitCurFreqFinished_PARAM Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.u8Progress=u8Progress;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.u8FindNum=u8FindNum;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.substate_reg=&substate_reg;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.u32Data=&u32Data;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.symbolrate_reg=&symbolrate_reg;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.CFO_reg=&CFO_reg;
    Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_WaitCurFreqFinished,&Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM);
      if(substate_reg==0)
      {
        _u16ChannelInfoArray[0][_u16ChannelInfoIndex]=((u32Data+500)/1000);
        _u16LockedCenterFreq=((u32Data+500)/1000);                //Center Freq
        _u16ChannelInfoArray[1][_u16ChannelInfoIndex]=(symbolrate_reg);//Symbol Rate
        _u16LockedSymbolRate=symbolrate_reg;
        _u16ChannelInfoIndex++;
        *u8FindNum=_u16ChannelInfoIndex;
	 if (CFO_reg*1000 >= 0x8000)
        {
           CFO_reg=0x10000- CFO_reg*1000;
           _s16CurrentCFO=-1*CFO_reg/1000;
        }
        else
        {
           _s16CurrentCFO=CFO_reg;
        }
        DMD_DBG(ULOGD("DEMOD","Current Locked CF:%d BW:%d CFO:%d \n ", _u16LockedCenterFreq, _u16LockedSymbolRate, _s16CurrentCFO));
      }
      else if(substate_reg==1) 
      {
         if (CFO_reg*1000 >= 0x8000)
         {
                CFO_reg=0x1000- CFO_reg*1000;
                _s16CurrentCFO=-1*CFO_reg/1000;
         }
         else
         {
                _s16CurrentCFO=CFO_reg;
         }
         DMD_DBG(ULOGD("DEMOD","Pre Locked CF:%d BW:%d CFO:%d \n ", _u16LockedCenterFreq, _u16LockedSymbolRate, _s16CurrentCFO));
      }	
      DMD_DBG(ULOGD("DEMOD","MDrv_Demod_BlindScan_WaitCurFreqFinished- u8Progress: %d u8FindNum %d\n", *u8Progress, *u8FindNum));

   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_WaitCurFreqFinished_PARAM.bRet;
}
#endif

MS_BOOL MDrv_DMD_DVBS_BlindScan_GetTunerFreq(MS_U16 *u16TunerCenterFreq, MS_U16 *u16TunerCutOffFreq)
{
    DVBS_BlindScan_GetTunerFreq_PARAM Drv_DVBS_BlindScan_GetTunerFreq_PARAM;
    Drv_DVBS_BlindScan_GetTunerFreq_PARAM.u16TunerCenterFreq=u16TunerCenterFreq;
    Drv_DVBS_BlindScan_GetTunerFreq_PARAM.u16TunerCutOffFreq=u16TunerCutOffFreq;
    Drv_DVBS_BlindScan_GetTunerFreq_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_BlindScan_GetTunerFreq\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_BlindScan_GetTunerFreq,&Drv_DVBS_BlindScan_GetTunerFreq_PARAM);
   }
   else
   {
      return false;
   }   

   return Drv_DVBS_BlindScan_GetTunerFreq_PARAM.bRet;
}

//------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_DVBS_DiSEqC_Init(void)
{
    DVBS_DiSEqC_Init_PARAM Drv_DVBS_DiSEqC_Init_PARAM;
    Drv_DVBS_DiSEqC_Init_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_Init\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_Init,&Drv_DVBS_DiSEqC_Init_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_Init_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetLNBOut(MS_BOOL bLow)
{
    DVBS_DiSEqC_SetLNBOut_PARAM Drv_DVBS_DiSEqC_SetLNBOut_PARAM;
    Drv_DVBS_DiSEqC_SetLNBOut_PARAM.bLow=bLow;
    Drv_DVBS_DiSEqC_SetLNBOut_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_SetLNBOut\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_SetLNBOut,&Drv_DVBS_DiSEqC_SetLNBOut_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_SetLNBOut_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow)
{
    DVBS_DiSEqC_GetLNBOut_PARAM Drv_DVBS_DiSEqC_GetLNBOut_PARAM;
    Drv_DVBS_DiSEqC_GetLNBOut_PARAM.bLNBOutLow=bLNBOutLow;
    Drv_DVBS_DiSEqC_GetLNBOut_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_GetLNBOut\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_GetLNBOut,&Drv_DVBS_DiSEqC_GetLNBOut_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_GetLNBOut_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_Set22kOnOff(MS_BOOL b22kOn)
{
    DVBS_DiSEqC_Set22kOnOff_PARAM Drv_DVBS_DiSEqC_Set22kOnOff_PARAM;
    Drv_DVBS_DiSEqC_Set22kOnOff_PARAM.b22kOn=b22kOn;
    Drv_DVBS_DiSEqC_Set22kOnOff_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_Set22kOnOff\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_Set22kOnOff,&Drv_DVBS_DiSEqC_Set22kOnOff_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_Set22kOnOff_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn)
{
    DVBS_DiSEqC_Get22kOnOff_PARAM Drv_DVBS_DiSEqC_Get22kOnOff_PARAM;
    Drv_DVBS_DiSEqC_Get22kOnOff_PARAM.b22kOn=b22kOn;
    Drv_DVBS_DiSEqC_Get22kOnOff_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_Get22kOnOff\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_Get22kOnOff,&Drv_DVBS_DiSEqC_Get22kOnOff_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_Get22kOnOff_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
    DVBS_DiSEqC_SendCmd_PARAM Drv_DVBS_DiSEqC_SendCmd_PARAM;
    Drv_DVBS_DiSEqC_SendCmd_PARAM.pCmd=pCmd;
    Drv_DVBS_DiSEqC_SendCmd_PARAM.u8CmdSize=u8CmdSize;
    Drv_DVBS_DiSEqC_SendCmd_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_SendCmd\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_SendCmd,&Drv_DVBS_DiSEqC_SendCmd_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_SendCmd_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetTxToneMode(MS_BOOL bTxTone22kOff)
{
    DVBS_DiSEqC_SetTxToneMode_PARAM Drv_DVBS_DiSEqC_SetTxToneMode_PARAM;
    Drv_DVBS_DiSEqC_SetTxToneMode_PARAM.bTxTone22kOff=bTxTone22kOff;
    Drv_DVBS_DiSEqC_SetTxToneMode_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_SetTxToneMode\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_SetTxToneMode,&Drv_DVBS_DiSEqC_SetTxToneMode_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_SetTxToneMode_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_DiSEqC_SetTone(MS_BOOL bTone1)
{
    DVBS_DiSEqC_SetTone_PARAM Drv_DVBS_DiSEqC_SetTone_PARAM;
    Drv_DVBS_DiSEqC_SetTone_PARAM.bTone1=bTone1;
    Drv_DVBS_DiSEqC_SetTone_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_DiSEqC_SetTone\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_DiSEqC_SetTone,&Drv_DVBS_DiSEqC_SetTone_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_DiSEqC_SetTone_PARAM.bRet;
}

MS_BOOL MDrv_DMD_DVBS_UnicableAGCCheckPower(MS_BOOL pbAGCCheckPower)
{
    DVBS_UnicableAGCCheckPower_PARAM Drv_DVBS_UnicableAGCCheckPower_PARAM;
    Drv_DVBS_UnicableAGCCheckPower_PARAM.pbAGCCheckPower=pbAGCCheckPower;
    Drv_DVBS_UnicableAGCCheckPower_PARAM.bRet=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS.c]MDrv_DMD_DVBS_UnicableAGCCheckPower\n"));
   if(u32DVBSopen==1)
   {
      UtopiaIoctl(ppDVBSInstant,DMD_DVBS_DRV_CMD_UnicableAGCCheckPower,&Drv_DVBS_UnicableAGCCheckPower_PARAM);
   }
   else
   {
      return false;
   }   
   return Drv_DVBS_UnicableAGCCheckPower_PARAM.bRet;
}
