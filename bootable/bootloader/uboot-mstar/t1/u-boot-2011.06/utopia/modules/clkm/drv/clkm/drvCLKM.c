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
#define _DRVBYTEDMA_C

////////////////////////////////////////////////////////////////////////////////
/// file drvBDMA.c
/// @brief DRAM byte DMA control driver
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvCLKM.h"
#include "drvclkm_io.h"
//#include "halclkm.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "halCHIP.h"
#include "utopia.h"
#include "drvSEM.h"
#include "MsTypes.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>

////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////

static CLKM_DbgLvl _gCLKMDbgLevel = E_CLKM_DBGLVL_ERROR;

#define CLKM_UTOPIA20       (1)

#define ERR_HANDLE (-1)
#define NOT_FOUND (-1)

#define SUCCESS_SET_SRC 0
#define ERR_NOT_FOUND_TABLE -1
#define ERR_NOT_SUPPORT_SRC -2

#define CLK_GATE_ENABLE 1
#define CLK_GATE_DISABLE 0


#define CLKM_DBG_FUNC()               if (_gCLKMDbgLevel >= E_CLKM_DBGLVL_ALL) \
                                        {MS_DEBUG_MSG(printf("\t====   %s   ====\n", __FUNCTION__));}
#define CLKM_DBG_INFO(x, args...)     if (_gCLKMDbgLevel >= E_CLKM_DBGLVL_INFO ) \
                                        {MS_DEBUG_MSG(printf("[%s]: ", __FUNCTION__); printf(x, ##args));}
#define CLKM_DBG_ERR(x, args...)      if (_gCLKMDbgLevel >= E_CLKM_DBGLVL_ERROR) \
                                        {MS_DEBUG_MSG(printf("[%s]: ", __FUNCTION__); printf(x, ##args);)}
#define CLKM_DBG_WARN(x, args...)     if (_gCLKMDbgLevel >= E_CLKM_DBGLVL_WARNING) \
                                        {MS_DEBUG_MSG(printf("[%s]: ", __FUNCTION__); printf(x, ##args);)}

#define CLKM_OFFSET(x)       ((x)<<1)

#define CHK_BIT(x, n) ( ((x) & (1<< (n)))!=0 )

#define CLKM_MUTEX_WAIT_TIME        (5000)


static MSIF_Version _clkmVersion = {
    .DDI = { CLKM_DRV_VERSION },
};

static CLKM_Info _gsInfo;

#define CLKM_MODULE_KERNAL_NAME       "/dev/clkm"
      
MS_BOOL Drv_Clkm_Init(void)
{

    MS_PHY u32BaseSize = 0;

     if (_gsInfo.bInit)
     { 
         return _gsInfo.bInit;
     }

      _gsInfo.s32Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex CLKM", MSOS_PROCESS_SHARED);
      MS_ASSERT(_gsInfo.s32Mutex >= 0);

      _gsInfo.s32fd = open(CLKM_MODULE_KERNAL_NAME, O_RDWR);

      if( _gsInfo.s32fd < 0)
      {
           CLKM_DBG_ERR("Fail to open CLKM Kernal Module!\n");
	    _gsInfo.bInit = FALSE;
           return E_CLKM_FAIL;
       }
	
 

    _gsInfo.bInit = TRUE;
    
   
    return true;
}


static MS_BOOL _Drv_CLKM_Is_Init(void)
{
   
    if (!_gsInfo.bInit)
        CLKM_DBG_ERR("Call Drv_Clkm_Init first!\n");
    return _gsInfo.bInit;
}


CLKM_Result Drv_CLKM_Exit()
{

     if (!_gsInfo.bInit)
     {
        CLKM_DBG_INFO("CLKM not init!\n");
        return E_CLKM_FAIL;
     }

      close(_gsInfo.s32fd);
      MsOS_DeleteMutex(_gsInfo.s32Mutex);
       _gsInfo.bInit = FALSE;

      return E_CLKM_OK;


}

CLKM_Result Drv_CLKM_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return E_CLKM_FAIL;
    }

    *ppVersion = &_clkmVersion;

    return E_CLKM_OK;
}




 MS_S32 Drv_Clkm_Get_Handle(char *name)
{

       CLKM_GetHandle_PARAM gethndleparam;
	   	
       MS_S32 s32Handle = ERR_HANDLE;

	int iResult;

	if (!_Drv_CLKM_Is_Init())
       {
            Drv_Clkm_Init();
       }

	 if (FALSE == MsOS_ObtainMutex(_gsInfo.s32Mutex, CLKM_MUTEX_WAIT_TIME))
        {
             CLKM_DBG_ERR("%s ENTRY fails!\n", __FUNCTION__);
             return E_CLKM_FAIL;
        }

	 gethndleparam.s8_Handle_Name = name;

	 if(ioctl(_gsInfo.s32fd, CMD_CLKM_GET_HANDLE, &gethndleparam) < 0)
        {
           CLKM_DBG_ERR("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
           
        }

	  s32Handle = gethndleparam.s32_Handle;

	  MsOS_ReleaseMutex(_gsInfo.s32Mutex);

	 return s32Handle;
}




void Drv_Clkm_Clk_Gate_Disable(MS_S32 s32Handle)
{
      
	
	CLKM_CLK_GATE_DISABLE_PARAM clkgatedisable;

	 if (!_Drv_CLKM_Is_Init())
        {
            Drv_Clkm_Init();
        }


	 if (FALSE == MsOS_ObtainMutex(_gsInfo.s32Mutex, CLKM_MUTEX_WAIT_TIME))
        {
             CLKM_DBG_ERR("%s ENTRY fails!\n", __FUNCTION__);
             return E_CLKM_FAIL;
        }

        clkgatedisable.s32_Handle = s32Handle;

        if(ioctl(_gsInfo.s32fd, CMD_CLKM_CLK_GATE_DISABLE, &clkgatedisable) <0 )
        {
           CLKM_DBG_ERR("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);  
         }

        MsOS_ReleaseMutex(_gsInfo.s32Mutex);
	
}


void Drv_Clkm_Set_Clk_Source(MS_S32 s32Handle,char *clk_src_name)
{
       MS_BOOL ret = TRUE;
       char *s8_Src_Name;
       MS_S32 src_offset;
       s8_Src_Name = clk_src_name;
       MS_U32 u32RegAddr;
       CLKM_SET_CLK_SRC_PARAM setclksrcparam;

       if (!_Drv_CLKM_Is_Init())
       { 
           Drv_Clkm_Init();
       }

       if (FALSE == MsOS_ObtainMutex(_gsInfo.s32Mutex, CLKM_MUTEX_WAIT_TIME))
       {
           CLKM_DBG_ERR("%s ENTRY fails!\n", __FUNCTION__);
           return E_CLKM_FAIL;
       }

       setclksrcparam.s32_Handle = s32Handle;
       setclksrcparam.clk_src_name = clk_src_name;

       if(ioctl(_gsInfo.s32fd, CMD_CLKM_SET_CLK_SOURCE, &setclksrcparam) < 0)
       {
           CLKM_DBG_ERR("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);    
       }
    

	MsOS_ReleaseMutex(_gsInfo.s32Mutex);

	return ret;
}


MS_U16 Drv_Get_Sram_Sd_Info(char* name)
{
     /*  
       MS_U32 u32i;
	sram_sd_table *ptr = sram_sd_param->sram_sd_table_array_ref;
       sram_sd_table *p_sd_table = NULL;
       MS_U32 u32_regaddr ;
	MS_U16 u16Value; 
	MS_S8* s8tmpname = name;

	if (!_Drv_CLKM_Is_Init())
       {
           Drv_Clkm_Init();
        }


	return u16Value;
*/

return 0;

}


