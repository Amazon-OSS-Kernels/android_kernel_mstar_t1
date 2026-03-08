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
/// file    drvAVD.c
/// @brief  AVD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "drvDMD_INTERN_DVBC.h"
#include "drvDMD_INTERN_DVBC_v2.h"
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
#include "halDMD_INTERN_DVBC.h"
#include "halDMD_INTERN_common.h"
#include "../../include/drvSAR.h"  // for Utopia2
#include "utopia.h"
#include "utopia_dapi.h"
#include "ULog.h"
#include "../../include/drvDMD_INTERN_DVBC_v2.h"  // for Utopia2
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

static    void* ppDVBCInstant = NULL;
static MS_U32 u32DVBCopen = 0;
static MS_U8 u8DVBCUtopiaOpen = 0;   //for SetStillImagePara is earlier called than Init

static DMD_DVBC_InitData   AgcSsi_Para;
static float intern_dvb_c_qam_ref[] = {3.0, 0.0, 0.0, 0.0, 0.0, 80.0}; //16q,32q,64q,128q,256q, and others

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
#ifdef CHIP_KAISER
MS_BOOL MDrv_DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel)
{
   void* pAttribte = NULL;
  DVBC_Dual_Public_Init_PARAM Drv_DVBC_Dual_Public_Init_PARAM;
  Drv_DVBC_Dual_Public_Init_PARAM.u8AGC_Tristate_Ctrl=u8AGC_Tristate_Ctrl;
  Drv_DVBC_Dual_Public_Init_PARAM.u8Sar_Channel=u8Sar_Channel;

     if(u8DVBCUtopiaOpen == 0)  // First time open
    {
        if(UtopiaOpen(MODULE_DVBC/*|KERNEL_MODE*/ , &ppDVBCInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //kernel space
//if(UtopiaOpen(MODULE_DVBC , &ppDVBCInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //user space
        {
            u32DVBCopen = 1;
      //return_val=true;
            //ULOGD("DEMOD","\r\n ======== DVBT Open Successful %x =========", (WORD)u32DVBTopen);
        }
        else
        {
           // ULOGD("DEMOD","\r\n ======== DVBT Open Fail %x =========", (WORD)u32DVBTopen);
       //return_val=false;

          return false;
        }

        u8DVBCUtopiaOpen = 1;
    }

   if(u32DVBCopen==1)
        UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_Dual_Public_Init,&Drv_DVBC_Dual_Public_Init_PARAM);
      else
        return false;

      return Drv_DVBC_Dual_Public_Init_PARAM.ret;
}


MS_BOOL MDrv_DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
{
  DVBC_Dual_Individual_Init_PARAM Drv_DVBC_Dual_Individual_Init_PARAM;
  DMD_DVBC_InitData_Transform Init_Para_Temp;


  Init_Para_Temp.u8SarChannel=pDMD_DVBC_InitData->u8SarChannel;
  Init_Para_Temp.u8DMD_DVBC_DSPRegInitExt=pDMD_DVBC_InitData->u8DMD_DVBC_DSPRegInitExt;
  Init_Para_Temp.u8DMD_DVBC_DSPRegInitSize=pDMD_DVBC_InitData->u8DMD_DVBC_DSPRegInitSize;
  Init_Para_Temp.u8DMD_DVBC_InitExt=pDMD_DVBC_InitData->u8DMD_DVBC_InitExt;
  /*
  Init_Para_Temp.pTuner_RfagcSsi=pDMD_DVBC_InitData->pTuner_RfagcSsi;
  Init_Para_Temp.u16Tuner_RfagcSsi_Size=pDMD_DVBC_InitData->u16Tuner_RfagcSsi_Size;
  Init_Para_Temp.pTuner_IfagcSsi_LoRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_LoRef;
  Init_Para_Temp.u16Tuner_IfagcSsi_LoRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcSsi_LoRef_Size;
  Init_Para_Temp.pTuner_IfagcSsi_HiRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_HiRef;
  Init_Para_Temp.u16Tuner_IfagcSsi_HiRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcSsi_HiRef_Size;
  Init_Para_Temp.pTuner_IfagcErr_LoRef=pDMD_DVBC_InitData->pTuner_IfagcErr_LoRef;
  Init_Para_Temp.u16Tuner_IfagcErr_LoRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcErr_LoRef_Size;
  Init_Para_Temp.pTuner_IfagcSsi_HiRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_HiRef;
  Init_Para_Temp.u16Tuner_IfagcErr_HiRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcErr_HiRef_Size;
  Init_Para_Temp.pSqiCnNordigP1=pDMD_DVBC_InitData->pSqiCnNordigP1;
  Init_Para_Temp.u16SqiCnNordigP1_Size=pDMD_DVBC_InitData->u16SqiCnNordigP1_Size;
  */

  Drv_DVBC_Dual_Individual_Init_PARAM.pDMD_DVBC_InitData=&Init_Para_Temp;
  Drv_DVBC_Dual_Individual_Init_PARAM.u32InitDataLen=sizeof(Init_Para_Temp);

  if(u32DVBCopen==1)
      UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_Dual_Individual_Init,&Drv_DVBC_Dual_Individual_Init_PARAM);
  else
      return false;

  return Drv_DVBC_Dual_Individual_Init_PARAM.ret;
}
#else
MS_BOOL MDrv_DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
{
        ULOGD("DEMOD","Doesn't support DVBC_Dual_Individual_Init function!!!\n");
        return false;
}

MS_BOOL MDrv_DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel)
{
        ULOGD("DEMOD","Doesn't support DVBC_Dual_Public_Init function!!!\n");
        return false;
}
#endif
MS_BOOL MDrv_DMD_DVBC_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
{
    void* pAttribte = NULL;

    #ifdef MS_DEBUG
    ULOGD("DEMOD","******check driver layer DVBC init!!*******\n");
    #endif
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC.c]MDrv_DMD_DVBC_Init\n"));

    DVBC_Init_PARAM Drv_DVBC_Init_PARAM;
    DMD_DVBC_InitData_Transform Init_Para_Temp;

    Init_Para_Temp.u8SarChannel=pDMD_DVBC_InitData->u8SarChannel;
    Init_Para_Temp.u8DMD_DVBC_DSPRegInitExt=pDMD_DVBC_InitData->u8DMD_DVBC_DSPRegInitExt;
    Init_Para_Temp.u8DMD_DVBC_DSPRegInitSize=pDMD_DVBC_InitData->u8DMD_DVBC_DSPRegInitSize;
    Init_Para_Temp.u8DMD_DVBC_InitExt=pDMD_DVBC_InitData->u8DMD_DVBC_InitExt;
    /*
    Init_Para_Temp.pTuner_RfagcSsi=pDMD_DVBC_InitData->pTuner_RfagcSsi;
    Init_Para_Temp.u16Tuner_RfagcSsi_Size=pDMD_DVBC_InitData->u16Tuner_RfagcSsi_Size;
    Init_Para_Temp.pTuner_IfagcSsi_LoRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_LoRef;
    Init_Para_Temp.u16Tuner_IfagcSsi_LoRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcSsi_LoRef_Size;
    Init_Para_Temp.pTuner_IfagcSsi_HiRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_HiRef;
    Init_Para_Temp.u16Tuner_IfagcSsi_HiRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcSsi_HiRef_Size;
    Init_Para_Temp.pTuner_IfagcErr_LoRef=pDMD_DVBC_InitData->pTuner_IfagcErr_LoRef;
    Init_Para_Temp.u16Tuner_IfagcErr_LoRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcErr_LoRef_Size;
    Init_Para_Temp.pTuner_IfagcSsi_HiRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_HiRef;
    Init_Para_Temp.u16Tuner_IfagcErr_HiRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcErr_HiRef_Size;
    Init_Para_Temp.pSqiCnNordigP1=pDMD_DVBC_InitData->pSqiCnNordigP1;
    Init_Para_Temp.u16SqiCnNordigP1_Size=pDMD_DVBC_InitData->u16SqiCnNordigP1_Size;
    */

    AgcSsi_Para.pTuner_RfagcSsi=pDMD_DVBC_InitData->pTuner_RfagcSsi;
    AgcSsi_Para.u16Tuner_RfagcSsi_Size=pDMD_DVBC_InitData->u16Tuner_RfagcSsi_Size;
    AgcSsi_Para.pTuner_IfagcSsi_LoRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_LoRef;
    AgcSsi_Para.u16Tuner_IfagcSsi_LoRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcSsi_LoRef_Size;
    AgcSsi_Para.pTuner_IfagcSsi_HiRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_HiRef;
    AgcSsi_Para.u16Tuner_IfagcSsi_HiRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcSsi_HiRef_Size;
    AgcSsi_Para.pTuner_IfagcErr_LoRef=pDMD_DVBC_InitData->pTuner_IfagcErr_LoRef;
    AgcSsi_Para.u16Tuner_IfagcErr_LoRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcErr_LoRef_Size;
    AgcSsi_Para.pTuner_IfagcSsi_HiRef=pDMD_DVBC_InitData->pTuner_IfagcSsi_HiRef;
    AgcSsi_Para.u16Tuner_IfagcErr_HiRef_Size=pDMD_DVBC_InitData->u16Tuner_IfagcErr_HiRef_Size;

    Drv_DVBC_Init_PARAM.u32InitDataLen=sizeof(Init_Para_Temp);
    Drv_DVBC_Init_PARAM.pDMD_DVBC_InitData=&Init_Para_Temp;
    Drv_DVBC_Init_PARAM.ret=false;

     if(u8DVBCUtopiaOpen == 0)  // First time open
    {
        if(UtopiaOpen(MODULE_DVBC/*|KERNEL_MODE*/ , &ppDVBCInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //kernel space
        //if(UtopiaOpen(MODULE_DVBC , &ppDVBCInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //user space
        {
            u32DVBCopen = 1;
            ULOGD("DEMOD","bryan check DVBC utopia open sucessful!!\n");
      //return_val=true;
           // ULOGD("DEMOD","\r\n ======== DVBC Open Successful %x =========", (WORD)u32DVBCopen);
        }
        else
        {
          //  ULOGD("DEMOD","\r\n ======== DVBC Open Fail %x =========", (WORD)u32DVBCopen);
       //return_val=false;
          ULOGD("DEMOD","DVBC utopia open fail!!\n");
          return false;
        }

        u8DVBCUtopiaOpen = 1;
    }

   if(u32DVBCopen==1)
        UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_Init,&Drv_DVBC_Init_PARAM);
      else
        return false;

    return Drv_DVBC_Init_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBC_Exit(void)
{
    DVBC_EXIT_PARAM_PARAM Drv_DVBC_EXIT_PARAM_PARAM;
    Drv_DVBC_EXIT_PARAM_PARAM.ret=false;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC.c]MDrv_DMD_DVBC_Exit\n"));

       if(u32DVBCopen==1)
        UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_Exit,&Drv_DVBC_EXIT_PARAM_PARAM);
      else
        return false;

    return Drv_DVBC_EXIT_PARAM_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBC_SetDbgLevel(DMD_DVBC_DbgLv u8DbgLevel)
{
    DVBC_SetDbgLevel_PARAM Drv_DVBC_SetDbgLevel_PARAM;
    Drv_DVBC_SetDbgLevel_PARAM.u8DbgLevel=u8DbgLevel;
    Drv_DVBC_SetDbgLevel_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC.c]MDrv_DMD_DVBC_SetDbgLevel\n"));
      if(u32DVBCopen==1)
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_SetDbgLevel,&Drv_DVBC_SetDbgLevel_PARAM);
      else
        return false;

    return Drv_DVBC_SetDbgLevel_PARAM.ret;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
const DMD_DVBC_Info* MDrv_DMD_DVBC_GetInfo(void)
{
  DVBC_GetInfo_PARAM Drv_DVBC_GetInfo_PARAM;
  Drv_DVBC_GetInfo_PARAM.ret_info=NULL;
  DMD_DVBC_MODULATION_TYPE Qam_mode;
  float fSNR;
  MS_U16 u16Quality;

  MS_U16 SymbolRate;
  float FreqOff;

  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetInfo,&Drv_DVBC_GetInfo_PARAM);
    MDrv_DMD_DVBC_GetSNR(&fSNR);
    MDrv_DMD_DVBC_GetSignalQuality(&u16Quality);
    MDrv_DMD_DVBC_GetStatus(&Qam_mode, &SymbolRate, &FreqOff);

  }
  else
  {
  }


  return Drv_DVBC_GetInfo_PARAM.ret_info;

}
#endif

MS_BOOL MDrv_DMD_DVBC_GetLibVer(const MSIF_Version **ppVersion)
{
   DVBC_GetLibVer_PARAM Drv_DVBC_GetLibVer_PARAM;
   Drv_DVBC_GetLibVer_PARAM.ppVersion=ppVersion;
    Drv_DVBC_GetLibVer_PARAM.ret= false;


   if(u32DVBCopen==1)
   {
       UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetLibVer,&Drv_DVBC_GetLibVer_PARAM);
	    Drv_DVBC_GetLibVer_PARAM.ret= true;
   }
   else
   {
       return false;
   }

   return Drv_DVBC_GetLibVer_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBC_GetFWVer(MS_U16 *ver)
{
  DVBC_GetFWVer_PARAM Drv_DVBC_GetFWVer_PARAM;
  Drv_DVBC_GetFWVer_PARAM.ver=ver;
  Drv_DVBC_GetFWVer_PARAM.ret=false;

        if(u32DVBCopen==1)
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetFWVer,&Drv_DVBC_GetFWVer_PARAM);
  else
  {
    return false;
  }

       return Drv_DVBC_GetFWVer_PARAM.ret;

}

MS_BOOL MDrv_DMD_DVBC_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
#if defined(CHIP_K1)||defined(CHIP_KENYA)
  DVBC_GetDSPReg_PARAM Drv_DVBC_GetDSPReg_PARAM;
  Drv_DVBC_GetDSPReg_PARAM.u16Addr=u16Addr;
  Drv_DVBC_GetDSPReg_PARAM.pu8Data=pu8Data;

   if(u32DVBCopen==1)
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetDSPReg,&Drv_DVBC_GetDSPReg_PARAM);
  else
  {
    return false;
  }

  return Drv_DVBC_GetDSPReg_PARAM.ret;
#else
    ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBC_GetDSPReg\n");
  return FALSE;
#endif
}

MS_BOOL MDrv_DMD_DVBC_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data)
{
#if defined(CHIP_K1)||defined(CHIP_KENYA)
  DVBC_SetDSPReg_PARAM Drv_DVBC_SetDSPReg_PARAM;

  Drv_DVBC_SetDSPReg_PARAM.pu8Data=pu8Data;
  Drv_DVBC_SetDSPReg_PARAM.u16Addr=u16Addr;

  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_SetDSPReg,&Drv_DVBC_SetDSPReg_PARAM);
  }
  else
  {
    return false;
  }

    return Drv_DVBC_SetDSPReg_PARAM.ret;
#else
    ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBC_SetDSPReg\n");
  return FALSE;
#endif
}


MS_BOOL MDrv_DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
  DVBC_GetReg_PARAM Drv_DVBC_GetReg_PARAM;
  Drv_DVBC_GetReg_PARAM.u16Addr=u16Addr;
  Drv_DVBC_GetReg_PARAM.pu8Data=pu8Data;
  Drv_DVBC_GetReg_PARAM.ret=false;

  if(u32DVBCopen==1)
    {
      UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetReg,&Drv_DVBC_GetReg_PARAM);
   }
    else
    {
      return false;
    }

    return Drv_DVBC_GetReg_PARAM.ret;

}

MS_BOOL MDrv_DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
  DVBC_SetReg_PARAM Drv_DVBC_SetReg_PARAM;
  Drv_DVBC_SetReg_PARAM.u16Addr=u16Addr;
  Drv_DVBC_SetReg_PARAM.u8Data=u8Data;
  Drv_DVBC_SetReg_PARAM.ret=false;

  if(u32DVBCopen==1)
    {
      UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_SetReg,&Drv_DVBC_SetReg_PARAM);
   }
    else
    {
      return false;
    }

    return Drv_DVBC_SetReg_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBC_SetSerialControl(MS_BOOL bEnable)
{
      DVBC_SetSerialControl_PARAM Drv_DVBC_SetSerialControl_PARAM;
      Drv_DVBC_SetSerialControl_PARAM.bEnable=bEnable;
  Drv_DVBC_SetSerialControl_PARAM.ret=false;

  if(u32DVBCopen==1)
    {
      UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_SetSerialControl,&Drv_DVBC_SetSerialControl_PARAM);
   }
    else
    {
      return false;
    }


    return Drv_DVBC_SetSerialControl_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBC_SetConfig(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS)
{
  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBC SetConfig!!\n");
  #endif

   return MDrv_DMD_DVBC_SetConfig_symbol_rate_list(u16SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, NULL, 0);
}

MS_BOOL MDrv_DMD_DVBC_SetConfig_symbol_rate_list(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{
  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBC SetConfig_symbol_rate_list!!\n");
  #endif

  DVBC_SetConfig_Symbol_rate_list_PARAM Drv_DVBC_SetConfig_Symbol_rate_list_PARAM;
  Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.u16SymbolRate=u16SymbolRate;
  Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.eQamMode=eQamMode;
  Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.u32IFFreq=u32IFFreq;
  Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.bSpecInv=bSpecInv;
  Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.bSerialTS=bSerialTS;
  Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.pu16_symbol_rate_list=pu16_symbol_rate_list;
  Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.u8_symbol_rate_list_num=u8_symbol_rate_list_num;
  Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.ret=false;

  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_SetSetConfig_symbol_rate_list,&Drv_DVBC_SetConfig_Symbol_rate_list_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBC_SetConfig_Symbol_rate_list_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBC_SetActive(MS_BOOL bEnable)
{
  DVBC_SetActive_PARAM Drv_DVBC_SetActive_PARAM;
  Drv_DVBC_SetActive_PARAM.bEnable=bEnable;
  Drv_DVBC_SetActive_PARAM.ret=false;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC.c]MDrv_DMD_DVBC_SetActive\n"));

  #ifdef MS_DEBUG
  ULOGD("DEMOD","check driver layer DVBC SetActive!!\n");
  #endif

  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_SetActive,&Drv_DVBC_SetActive_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBC_SetActive_PARAM.ret;
}
#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK_TYPE eType, DMD_DVBC_LOCK_STATUS *eLockStatus)
{
    return MDrv_DMD_DVBC_GetLockWithRFPower(eType, eLockStatus, 200.0f, -200.0f);
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetLockWithRFPower(DMD_DVBC_GETLOCK_TYPE eType, DMD_DVBC_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm)
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

  DVBC_GetLockWithRFPower Drv_DVBC_GetLockWithRFPower;

  Drv_DVBC_GetLockWithRFPower.eType=eType;
  Drv_DVBC_GetLockWithRFPower.eLockStatus=eLockStatus;
  //Drv_DVBC_GetLockWithRFPower.u32CurrRFPowerDbm=(MS_U32)(fCurrRFPowerDbm*10);
  //Drv_DVBC_GetLockWithRFPower.u32NoChannelRFPowerDbm=(MS_U32)(fNoChannelRFPowerDbm*10);
  Drv_DVBC_GetLockWithRFPower.u32TimeInterval=0;
  Drv_DVBC_GetLockWithRFPower.ret=false;

  DVBC_GetIFAGC_PARAM Drv_DVBC_GetIFAGC_PARAM;
  Drv_DVBC_GetIFAGC_PARAM.ifagc_reg=&ifagc_reg;
  Drv_DVBC_GetIFAGC_PARAM.ifagc_reg_lsb=&ifagc_reg_lsb;
  Drv_DVBC_GetIFAGC_PARAM.ifagc_err_reg=&ifagc_err_reg;
  Drv_DVBC_GetIFAGC_PARAM.ret=false;

  if(u32DVBCopen==1)
  {
  	if((AgcSsi_Para.pTuner_IfagcSsi_HiRef != NULL) && (AgcSsi_Para.pTuner_IfagcSsi_LoRef != NULL))
    {
  	    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetIFAGC,&Drv_DVBC_GetIFAGC_PARAM);
        status &= Drv_DVBC_GetIFAGC_PARAM.ret;

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

  	Drv_DVBC_GetLockWithRFPower.u32CurrRFPowerDbm=(MS_U32)(ch_power_db*10);
    Drv_DVBC_GetLockWithRFPower.u32NoChannelRFPowerDbm=(MS_U32)(fNoChannelRFPowerDbm*10);
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetLockWithRFPower,&Drv_DVBC_GetLockWithRFPower);
    status &= Drv_DVBC_GetLockWithRFPower.ret;
  }
  else
  {
    return false;
  }

  #ifdef MS_DEBUG
  //if(*(Drv_DVBC_GetLockWithRFPower_Transform.eLockStatus)==DMD_DVBC_LOCK)
  if(*(Drv_DVBC_GetLockWithRFPower.eLockStatus)==DMD_DVBC_LOCK)
  {
    ULOGD("DEMOD","check in drv layer DVBC demod locked!!\n");
  }
  else
  {
    ULOGD("DEMOD","check in drv layer DVBC demod unlock!!\n");
    return false;
  }
  #endif

  return status;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(u16Strength, 200.0f);
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
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
  DMD_DVBC_MODULATION_TYPE Qam_mode;
  MS_U16 SymbolRate;
  float FreqOff;

  //DVBC_GetSignalStrengthWithRFPower_PARAM Drv_DVBC_GetSignalStrengthWithRFPower_PARAM;
  //Drv_DVBC_GetSignalStrengthWithRFPower_PARAM.u16Strength=u16Strength;
  //Drv_DVBC_GetSignalStrengthWithRFPower_PARAM.fRFPowerDbm=fRFPowerDbm;

  DVBC_GetIFAGC_PARAM Drv_DVBC_GetIFAGC_PARAM;
  Drv_DVBC_GetIFAGC_PARAM.ifagc_reg=&ifagc_reg;
  Drv_DVBC_GetIFAGC_PARAM.ifagc_reg_lsb=&ifagc_reg_lsb;
  Drv_DVBC_GetIFAGC_PARAM.ifagc_err_reg=&ifagc_err_reg;
  Drv_DVBC_GetIFAGC_PARAM.ret=false;

  if(u32DVBCopen==1)
  {
    if((AgcSsi_Para.pTuner_IfagcSsi_HiRef != NULL) && (AgcSsi_Para.pTuner_IfagcSsi_LoRef != NULL))
    {
    	  UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetIFAGC,&Drv_DVBC_GetIFAGC_PARAM);
        status &= Drv_DVBC_GetIFAGC_PARAM.ret;
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

    status &= MDrv_DMD_DVBC_GetStatus(&Qam_mode, &SymbolRate, &FreqOff);

    if( (MS_U8)Qam_mode <= (MS_U8)DMD_DVBC_QAM256)
    {
        ch_power_db_rel = ch_power_db + intern_dvb_c_qam_ref[(MS_U8)Qam_mode];
    }
    else
    {
        ch_power_db_rel = -100.0f;
    }

    if(ch_power_db_rel <= -85.0f)
        {*u16Strength = 0;}
    else if (ch_power_db_rel <= -80.0f)
        {*u16Strength = (MS_U16)(0.0f + (ch_power_db_rel+85.0f)*10.0f/5.0f);}
    else if (ch_power_db_rel <= -75.0f)
        {*u16Strength = (MS_U16)(10.0f + (ch_power_db_rel+80.0f)*20.0f/5.0f);}
    else if (ch_power_db_rel <= -70.0f)
        {*u16Strength = (MS_U16)(30.0f + (ch_power_db_rel+75.0f)*30.0f/5.0f);}
    else if (ch_power_db_rel <= -65.0f)
        {*u16Strength = (MS_U16)(60.0f + (ch_power_db_rel+70.0f)*10.0f/5.0f);}
    else if (ch_power_db_rel <= -55.0f)
        {*u16Strength = (MS_U16)(70.0f + (ch_power_db_rel+65.0f)*20.0f/10.0f);}
    else if (ch_power_db_rel <= -45.0f)
        {*u16Strength = (MS_U16)(90.0f + (ch_power_db_rel+55.0f)*10.0f/10.0f);}
    else
        {*u16Strength = 100;}

    DMD_DBG(ULOGD("DEMOD",">>> SSI_CH_PWR(dB) = %f , Score = %d<<<\n", ch_power_db, *u16Strength));
    DMD_DBG(ULOGD("DEMOD",">>> SSI = %d <<<\n", (int)*u16Strength));

    return status;
  }
  else
  {
    return false;
  }

  return status;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetSignalQuality(MS_U16 *u16Quality)
{
    return MDrv_DMD_DVBC_GetSignalQualityWithRFPower(u16Quality, 200.0f);
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
  float       fber;
  float       log_ber;
  MS_BOOL status = true;
  DMD_DVBC_MODULATION_TYPE Qam_mode;
  float f_snr;

  DMD_DVBC_LOCK_STATUS eLockStatus;
  MS_U16 SymbolRate;
  float FreqOff;


  if(u32DVBCopen==1)
  {
    status &= MDrv_DMD_DVBC_GetSNR(&f_snr);
    if (TRUE == MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK_PSYNC_LOCK, &eLockStatus))
    {
    	  if (MDrv_DMD_DVBC_GetPostViterbiBer(&fber) == FALSE)
        {
            DMD_DBG(ULOGD("DEMOD","\nGetPostViterbiBer Fail!"));
            return FALSE;
        }

        // log_ber = log10(fber)
        #ifdef MSOS_TYPE_LINUX
          log_ber = (-1.0f)*log10f(1.0f/fber);
        #else
          log_ber = (-1.0f)*Log10Approx(1.0f/fber); // Log10Approx() provide 1~2^32 input range only
        #endif

        DMD_DBG(ULOGD("DEMOD","\nLog(BER) = %f",log_ber));
        status &= MDrv_DMD_DVBC_GetStatus(&Qam_mode, &SymbolRate, &FreqOff);
        if (Qam_mode == DMD_DVBC_QAM16)
        {
            if(log_ber  <= (-5.5f))
                *u16Quality = 100;
            else if(log_ber  <= (-5.1f))
                *u16Quality = (MS_U16)(90.0f + ((-5.1f)-log_ber)*10.0f/((-5.1f)-(-5.5f)));
            else if(log_ber  <= (-4.9f))
                *u16Quality = (MS_U16)(80.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.5f))
                *u16Quality = (MS_U16)(70.0f + ((-4.5f)-log_ber)*10.0f/((-4.5f)-(-4.9f)));
            else if(log_ber  <= (-3.7f))
                *u16Quality = (MS_U16)(60.0f + ((-3.7f)-log_ber)*10.0f/((-3.7f)-(-4.5f)));
            else if(log_ber  <= (-3.2f))
                *u16Quality = (MS_U16)(50.0f + ((-3.2f)-log_ber)*10.0f/((-3.2f)-(-3.7f)));
            else if(log_ber  <= (-2.9f))
                *u16Quality = (MS_U16)(40.0f + ((-2.9f)-log_ber)*10.0f/((-2.9f)-(-3.2f)));
            else if(log_ber  <= (-2.5f))
                *u16Quality = (MS_U16)(30.0f + ((-2.5f)-log_ber)*10.0f/((-2.5f)-(-2.9f)));
            else if(log_ber  <= (-2.2f))
                *u16Quality = (MS_U16)(20.0f + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.5f)));
            else if(log_ber  <= (-2.0f))
                *u16Quality = (MS_U16)(0.0f + ((-2.0f)-log_ber)*10.0f/((-2.0f)-(-2.2f)));
            else
                *u16Quality = 0;
        }
        else if (Qam_mode == DMD_DVBC_QAM32)
        {
            if(log_ber  <= (-5.0f))
                *u16Quality = 100;
            else if(log_ber  <= (-4.7f))
                *u16Quality = (MS_U16)(90.0f  + ((-4.7f)-log_ber)*10.0f/((-4.7f)-(-5.0f)));
            else if(log_ber  <= (-4.5f))
                *u16Quality = (MS_U16)(80.0f  + ((-4.5f)-log_ber)*10.0f/((-4.5f)-(-4.7f)));
            else if(log_ber  <= (-3.8f))
                *u16Quality = (MS_U16)(70.0f  + ((-3.8f)-log_ber)*10.0f/((-3.8f)-(-4.5f)));
            else if(log_ber  <= (-3.5f))
                *u16Quality = (MS_U16)(60.0f  + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-3.8f)));
            else if(log_ber  <= (-3.0f))
                *u16Quality = (MS_U16)(50.0f  + ((-3.0f)-log_ber)*10.0f/((-3.0f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
                *u16Quality = (MS_U16)(40.0f  + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.0f)));
            else if(log_ber  <= (-2.4f))
                *u16Quality = (MS_U16)(30.0f  + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.7f)));
            else if(log_ber  <= (-2.2f))
                *u16Quality = (MS_U16)(20.0f  + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.4f)));
            else if(log_ber  <= (-2.0f))
                *u16Quality = (MS_U16)(0.0f  + ((-2.0f)-log_ber)*10.0f/((-2.0f)-(-2.2f)));
            else
                *u16Quality = 0;
        }
        else if (Qam_mode == DMD_DVBC_QAM64)
        {
            if(log_ber  <= (-5.4f))
                *u16Quality = 100;
            else if(log_ber  <= (-5.1f))
                *u16Quality = (MS_U16)(90.0f + ((-5.1f)-log_ber)*10.0f/((-5.1f)-(-5.4f)));
            else if(log_ber  <= (-4.9f))
                *u16Quality = (MS_U16)(80.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.3f))
                *u16Quality = (MS_U16)(70.0f + ((-4.3f)-log_ber)*10.0f/((-4.3f)-(-4.9f)));
            else if(log_ber  <= (-3.7f))
                *u16Quality = (MS_U16)(60.0f + ((-3.7f)-log_ber)*10.0f/((-3.7f)-(-4.3f)));
            else if(log_ber  <= (-3.2f))
                *u16Quality = (MS_U16)(50.0f + ((-3.2f)-log_ber)*10.0f/((-3.2f)-(-3.7f)));
            else if(log_ber  <= (-2.9f))
                *u16Quality = (MS_U16)(40.0f + ((-2.9f)-log_ber)*10.0f/((-2.9f)-(-3.2f)));
            else if(log_ber  <= (-2.4f))
                *u16Quality = (MS_U16)(30.0f + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.9f)));
            else if(log_ber  <= (-2.2f))
                *u16Quality = (MS_U16)(20.0f + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.4f)));
            else if(log_ber  <= (-2.05f))
                *u16Quality = (MS_U16)(0.0f + ((-2.05f)-log_ber)*10.0f/((-2.05f)-(-2.2f)));
            else
                *u16Quality = 0;
        }
        else if (Qam_mode == DMD_DVBC_QAM128)
        {
            if(log_ber  <= (-5.1f))
            *u16Quality = 100;
            else if(log_ber  <= (-4.9f))
            *u16Quality = (MS_U16)(90.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.7f))
            *u16Quality = (MS_U16)(80.0f + ((-4.7f)-log_ber)*10.0f/((-4.7f)-(-4.9f)));
            else if(log_ber  <= (-4.1f))
            *u16Quality = (MS_U16)(70.0f + ((-4.1f)-log_ber)*10.0f/((-4.1f)-(-4.7f)));
            else if(log_ber  <= (-3.5f))
            *u16Quality = (MS_U16)(60.0f + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-4.1f)));
            else if(log_ber  <= (-3.1f))
            *u16Quality = (MS_U16)(50.0f + ((-3.1f)-log_ber)*10.0f/((-3.1f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
            *u16Quality = (MS_U16)(40.0f + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.1f)));
            else if(log_ber  <= (-2.5f))
            *u16Quality = (MS_U16)(30.0f + ((-2.5f)-log_ber)*10.0f/((-2.5f)-(-2.7f)));
            else if(log_ber  <= (-2.06f))
            *u16Quality = (MS_U16)(20.0f + ((-2.06f)-log_ber)*10.0f/((-2.06f)-(-2.5f)));
        //else if(log_ber  <= (-2.05))
        else
        {
            if (f_snr >= 27.2f)
            *u16Quality = 20;
            else if (f_snr >= 25.1f)
            *u16Quality = (MS_U16)(0.0f + (f_snr - 25.1f)*20.0f/(27.2f-25.1f));
            else
            *u16Quality = 0;
        }
        }
        else //256QAM
        {
            if(log_ber  <= (-4.8f))
                *u16Quality = 100;
            else if(log_ber  <= (-4.6f))
                *u16Quality = (MS_U16)(90.0f + ((-4.6f)-log_ber)*10.0f/((-4.6f)-(-4.8f)));
            else if(log_ber  <= (-4.4f))
                *u16Quality = (MS_U16)(80.0f + ((-4.4f)-log_ber)*10.0f/((-4.4f)-(-4.6f)));
            else if(log_ber  <= (-4.0f))
                *u16Quality = (MS_U16)(70.0f + ((-4.0f)-log_ber)*10.0f/((-4.0f)-(-4.4f)));
            else if(log_ber  <= (-3.5f))
                *u16Quality = (MS_U16)(60.0f + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-4.0f)));
            else if(log_ber  <= (-3.1f))
                *u16Quality = (MS_U16)(50.0f + ((-3.1f)-log_ber)*10.0f/((-3.1f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
                *u16Quality = (MS_U16)(40.0f + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.1f)));
            else if(log_ber  <= (-2.4f))
                *u16Quality = (MS_U16)(30.0f + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.7f)));
            else if(log_ber  <= (-2.06f))
                *u16Quality = (MS_U16)(20.0f + ((-2.06f)-log_ber)*10.0f/((-2.06f)-(-2.4f)));
        //else if(log_ber  <= (-2.05))
        else
        {
            if (f_snr >= 29.6f)
                *u16Quality = 20;
            else if (f_snr >= 27.3f)
                *u16Quality = (MS_U16)(0.0f + (f_snr - 27.3f)*20.0f/(29.6f-27.3f));
            else
                *u16Quality = 0;
        }
        }
    }
    else
    {
        *u16Quality = 0;
    }

    DMD_DBG(ULOGD("DEMOD","BER = %8.3e\n", fber));
    DMD_DBG(ULOGD("DEMOD","Signal Quility = %d\n", *u16Quality));
    return TRUE;
  }
  else
  {
    return false;
  }

  return status;
}
#endif

#ifdef CHIP_KAISER
MS_BOOL MDrv_DMD_DVBC_ActiveDmdSwitch(MS_U8 demod_no)
{
  DVBC_ActiveDmdSwitch_PARAM Drv_DVBC_ActiveDmdSwitch_PARAM;
  Drv_DVBC_ActiveDmdSwitch_PARAM.demod_no=demod_no;

  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_ActiveDmdSwitch,&Drv_DVBC_ActiveDmdSwitch_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBC_ActiveDmdSwitch_PARAM.ret;
 }
#else
MS_BOOL MDrv_DMD_DVBC_ActiveDmdSwitch(MS_U8 demod_no)
{
  ULOGD("DEMOD","Doesn't support DVBC_ActiveDmdSwitch function!!!\n");
  return false;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetSNR(float *fSNR)
{
	MS_U16 snr_reg;

  DVBC_GetSNR_PARAM Drv_DVBC_GetSNR_PARAM;
  Drv_DVBC_GetSNR_PARAM.snr_reg=&snr_reg;
  Drv_DVBC_GetSNR_PARAM.ret = false;

  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetSNR,&Drv_DVBC_GetSNR_PARAM);

#ifdef MSOS_TYPE_LINUX
    *fSNR = 10.0f*log10f(65536.0f/(float)(snr_reg));
#else
    *fSNR = 10.0f*Log10Approx(65536.0f/(float)(snr_reg));
#endif

	Drv_DVBC_GetSNR_PARAM.ret = true;
  }
  else
  {
    return false;
  }

  return Drv_DVBC_GetSNR_PARAM.ret;
}
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetPostViterbiBer(float *ber)
{
  MS_U16 BitErrPeriod_reg;
  MS_U32 BitErr_reg;
  MS_U16 PktErr;

  DVBC_GetPostViterbiBer_PARAM Drv_DVBC_GetPostViterbiBer_PARAM;
  Drv_DVBC_GetPostViterbiBer_PARAM.BitErr_reg=&BitErr_reg;
  Drv_DVBC_GetPostViterbiBer_PARAM.BitErrPeriod_reg=&BitErrPeriod_reg;
  Drv_DVBC_GetPostViterbiBer_PARAM.ret=false;


  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetPostViterbiBer,&Drv_DVBC_GetPostViterbiBer_PARAM);
    MDrv_DMD_DVBC_GetPacketErr(&PktErr);

    if (BitErrPeriod_reg == 0 )    //protect 0
        BitErrPeriod_reg = 1;

    if (BitErr_reg <=0 )
    	{
        *ber = 0.5f / ((float)(BitErrPeriod_reg)*128*188*8);
    	}
    else
    	{
        *ber = (float)BitErr_reg / ((float)(BitErrPeriod_reg)*128*188*8);
    	}
    DMD_DBG(ULOGD("DEMOD","PostVitBER = %8.3e \n ", *ber));
    DMD_DBG(ULOGD("DEMOD","PktErr = %d \n ", (int)PktErr));
	Drv_DVBC_GetPostViterbiBer_PARAM.ret=true;
  }
  else
  {
    return false;
  }


  return Drv_DVBC_GetPostViterbiBer_PARAM.ret;
}
#endif

MS_BOOL MDrv_DMD_DVBC_GetPacketErr(MS_U16 *pktErr)
{
    DVBC_GetPacketErr_PARAM Drv_DVBC_GetPacketErr_PARAM;
    Drv_DVBC_GetPacketErr_PARAM.pktErr=pktErr;
  Drv_DVBC_GetPacketErr_PARAM.ret=false;

    if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetPacketErr,&Drv_DVBC_GetPacketErr_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBC_GetPacketErr_PARAM.ret;
}

MS_BOOL MDrv_DMD_DVBC_GetCellID(MS_U16 *u16CellID)
{
  DVBC_GetCellID_PARAM Drv_DVBC_GetCellID_PARAM;
  Drv_DVBC_GetCellID_PARAM.u16CellID=u16CellID;
  Drv_DVBC_GetCellID_PARAM.ret=false;

  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetCellID,&Drv_DVBC_GetCellID_PARAM);
  }
  else
  {
    return false;
  }

  return Drv_DVBC_GetCellID_PARAM.ret;

}


#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBC_GetStatus(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, float *pFreqOff)
{
	MS_U32 config_Fc_reg;
	MS_U32 Fc_over_Fs_reg;
	MS_U16 Cfo_offset_reg;

  DVBC_GetStatus_PARAM Drv_DVBC_GetStatus_PARAM;
  Drv_DVBC_GetStatus_PARAM.pQAMMode=pQAMMode;
  Drv_DVBC_GetStatus_PARAM.u16SymbolRate=u16SymbolRate;
  Drv_DVBC_GetStatus_PARAM.config_Fc_reg=&config_Fc_reg;
  Drv_DVBC_GetStatus_PARAM.Fc_over_Fs_reg=&Fc_over_Fs_reg;
  Drv_DVBC_GetStatus_PARAM.Cfo_offset_reg=&Cfo_offset_reg;
  Drv_DVBC_GetStatus_PARAM.ret=false;

  float f_Fc, FreqCfo_offset;

  if(u32DVBCopen==1)
  {
    UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_GetStatus,&Drv_DVBC_GetStatus_PARAM);

    f_Fc = (float)Fc_over_Fs_reg/134217728.0f * 45473.0f;

    FreqCfo_offset = (MS_S32)(Cfo_offset_reg<<4)/16;

    FreqCfo_offset = FreqCfo_offset/0x8000000/8.0f;

    FreqCfo_offset = FreqCfo_offset * (*u16SymbolRate) - (f_Fc-(float)config_Fc_reg);

    *pFreqOff = FreqCfo_offset;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetStatus %d %d\n", *pQAMMode, *u16SymbolRate, *pFreqOff);
    }
    #endif
  }
  else
  {
    return false;
  }

  return Drv_DVBC_GetStatus_PARAM.ret;
}
#endif

MS_U32 MDrv_DMD_DVBC_SetPowerState(EN_POWER_MODE u16PowerState)
{
    DVBC_SetPowerState_PARAM Drv_DVBC_SetPowerState_PARAM;
    Drv_DVBC_SetPowerState_PARAM.u16PowerState=u16PowerState;
    Drv_DVBC_SetPowerState_PARAM.ret_U32=false;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC.c]MDrv_DMD_DVBC_SetPowerState\n"));
   if(u32DVBCopen==1)
   {
  UtopiaIoctl(ppDVBCInstant,DMD_DVBC_DRV_CMD_SetPowerState,&Drv_DVBC_SetPowerState_PARAM);
   }
   else
   {
  return false;
    }

    return Drv_DVBC_SetPowerState_PARAM.ret_U32;
}
