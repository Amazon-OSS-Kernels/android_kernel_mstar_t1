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
/// file    XC_ModeParse_adp.c
/// @brief  Scaler API layer Interface.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC_MODEPARSE_ADP_C_
#define _XC_MODEPARSE_ADP_C_

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
// Common Definition
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsOS.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_ModeParse.h"
#include "apiXC_ModeParse_v2.h"
#include "apiXC_v2.h"
#include "XC_ModeParse_adp.h"
#include "XC_adp.h"


#define MAX_MODEPARSER_TAB_NUM 100
//------------------------------------------------------------------------------
//  Driver Compiler Options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER(a,b,c)                                    if(copy_from_user(a,b,c) != 0) { break; }
#define CPY_to_USER(a,b,c)                                      if(copy_to_user(a,b,c) != 0) { break; }
#else
#define CPY_FROM_USER                                           memcpy
#define CPY_to_USER                                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define COMPAT_PTR(a)                                           compat_ptr(a)
#define COMPAT_NEXT_PTR(a)                                      (*((MS_U32*)compat_ptr((unsigned long)a)))
#define IS_CMP_TASK()                                             is_compat_task()

#define CMP_CPY_FROM_USER(a,b,c)                                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  break; }
#define CMP_CPY_TO_USER(a,b,c)                                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { break;  }

#else
#define COMPAT_PTR(a)                                           (a)
#define COMPAT_NEXT_PTR(a)                                      (*((MS_U32*)a))
#define IS_CMP_TASK()                                             (FALSE)
#define CMP_CPY_FROM_USER                                       CPY_FROM_USER
#define CMP_CPY_TO_USER                                         CPY_to_USER
#endif //CONFIG_COMPAT
//------------------------------------------------------------------------------
//  Local Structurs
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------
#ifdef CONFIG_COMPAT
typedef struct DLL_PACKED
{
    compat_uptr_t ModeDB;
    MS_U8 u8NumberOfItems;
    compat_uptr_t psInputInfo;
    XC_MODEPARSE_RESULT eReturnValue;
} _compat_stXC_MODEPARSE_MATCHMODE;
#endif
//Function parameter
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_INIT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_MATCHMODE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_MATCHMODEEX[5];

//Function parameter's pointer
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_MODETABLE_TYPE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_INPUT_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_MODEPARSE_MODETABLE_TYPE_EX[5];


//------------------------------------------------------------------------------
//  Local Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U32 XC_MODEPARSE_adp_Init(void)
{

//set table
    MS_U32 temp=0;

    //Function parameter
    //pstXC_MODEPARSE_INIT         pXC_MODEPARSE_INIT        = (pstXC_MODEPARSE_INIT)temp;
    pstXC_MODEPARSE_MATCHMODE    pXC_MODEPARSE_MATCHMODE   = (pstXC_MODEPARSE_MATCHMODE)temp;
    pstXC_MODEPARSE_MATCHMODEEX  pXC_MODEPARSE_MATCHMODEEX = (pstXC_MODEPARSE_MATCHMODEEX)temp;


    //Function parameter's pointer
    //MS_PCADC_MODETABLE_TYPE*     pXC_MODEPARSE_ModeDB      = (MS_PCADC_MODETABLE_TYPE*)temp;
    //XC_MODEPARSE_INPUT_INFO*     pXC_MODEPARSE_InputInfo   = (XC_MODEPARSE_INPUT_INFO*)temp;
    //MS_PCADC_MODETABLE_TYPE_EX*  pXC_MODEPARSE_ModeDB_EX   = (MS_PCADC_MODETABLE_TYPE_EX*)temp;

    //Function parameter's pointer
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MODETABLE_TYPE[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_PCADC_MODETABLE_TYPE)*100);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MODETABLE_TYPE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_MODEPARSE_INPUT_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_MODEPARSE_INPUT_INFO));
    UADPBypassSetSPT(&spt_XC_MODEPARSE_INPUT_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_MODEPARSE_MODETABLE_TYPE_EX[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_PCADC_MODETABLE_TYPE_EX)*MAX_MODEPARSER_TAB_NUM);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MODETABLE_TYPE_EX[1],UADP_SPT_END , 0, 0);

    //Function parameter
    UADPBypassSetSPT(&spt_XC_MODEPARSE_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_MODEPARSE_INIT));
    UADPBypassSetSPT(&spt_XC_MODEPARSE_INIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_MODEPARSE_MATCHMODE));
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODE[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_MODEPARSE_MATCHMODE->ModeDB)) - (unsigned long)pXC_MODEPARSE_MATCHMODE), (unsigned long)spt_XC_MODEPARSE_MODETABLE_TYPE);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODE[2],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_MODEPARSE_MATCHMODE->psInputInfo)) - (unsigned long)pXC_MODEPARSE_MATCHMODE), (unsigned long)spt_XC_MODEPARSE_INPUT_INFO);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODE[3],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODEEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_MODEPARSE_MATCHMODEEX));
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODEEX[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_MODEPARSE_MATCHMODEEX->ModeDB)) - (unsigned long)pXC_MODEPARSE_MATCHMODEEX), (unsigned long)spt_XC_MODEPARSE_MODETABLE_TYPE_EX);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODEEX[2],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_MODEPARSE_MATCHMODEEX->psInputInfo)) - (unsigned long)pXC_MODEPARSE_MATCHMODEEX), (unsigned long)spt_XC_MODEPARSE_INPUT_INFO);
    UADPBypassSetSPT(&spt_XC_MODEPARSE_MATCHMODEEX[3],UADP_SPT_END , 0, 0);


    return 0;

}

MS_U32 XC_MODEPARSE_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
    char buffer_arg[2];

    switch(u32Cmd)
    {

        case E_XC_MODEPARSE_CMD_INIT :
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_XC_MODEPARSE_INIT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_MODEPARSE_CMD_MATCHMODE:
        {
            stXC_MODEPARSE_MATCHMODE tmp;
#ifdef CONFIG_COMPAT
            _compat_stXC_MODEPARSE_MATCHMODE tmp_compat;
            if(IS_CMP_TASK())
            {
                CMP_CPY_FROM_USER(&tmp_compat, pArgs, sizeof(_compat_stXC_MODEPARSE_MATCHMODE));
                tmp.ModeDB = malloc(sizeof(MS_PCADC_MODETABLE_TYPE)*tmp_compat.u8NumberOfItems);
                tmp.psInputInfo = malloc(sizeof(XC_MODEPARSE_INPUT_INFO));
                CMP_CPY_FROM_USER(tmp.ModeDB, tmp_compat.ModeDB, sizeof(MS_PCADC_MODETABLE_TYPE)*tmp_compat.u8NumberOfItems);
                CMP_CPY_FROM_USER(tmp.psInputInfo, tmp_compat.psInputInfo, sizeof(XC_MODEPARSE_INPUT_INFO));
                tmp.u8NumberOfItems=tmp_compat.u8NumberOfItems;
                tmp.eReturnValue=tmp_compat.eReturnValue;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp);
                tmp_compat.eReturnValue = tmp.eReturnValue;
                tmp_compat.u8NumberOfItems = tmp.u8NumberOfItems;
                //CMP_CPY_TO_USER(tmp_compat.ModeDB,tmp.ModeDB,sizeof(MS_PCADC_MODETABLE_TYPE));                //example
                CMP_CPY_TO_USER(tmp_compat.psInputInfo,tmp.psInputInfo,sizeof(XC_MODEPARSE_INPUT_INFO));
                CMP_CPY_TO_USER(pArgs,&tmp_compat,sizeof(_compat_stXC_MODEPARSE_MATCHMODE));
                free(tmp.ModeDB);
                free(tmp.psInputInfo);
            }else
            {
#endif
                void *ptr;
                void *ptr1;
                CPY_FROM_USER(&tmp, pArgs, sizeof(stXC_MODEPARSE_MATCHMODE));
                ptr=tmp.ModeDB;
                ptr1=tmp.psInputInfo;
                tmp.ModeDB = malloc(sizeof(MS_PCADC_MODETABLE_TYPE)*tmp.u8NumberOfItems);
                tmp.psInputInfo = malloc(sizeof(XC_MODEPARSE_INPUT_INFO));
                CPY_FROM_USER(tmp.ModeDB, ptr, sizeof(MS_PCADC_MODETABLE_TYPE)*tmp.u8NumberOfItems);
                CPY_FROM_USER(tmp.psInputInfo, ptr1, sizeof(XC_MODEPARSE_INPUT_INFO));
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp);
                CPY_to_USER(pArgs,&tmp,sizeof(stXC_MODEPARSE_MATCHMODE));
                free(tmp.ModeDB);
                free(tmp.psInputInfo);
#ifdef CONFIG_COMPAT
            }
#endif
            break;
        }

        case E_XC_MODEPARSE_CMD_MATCHMODEEX:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_MODEPARSE_MATCHMODEEX, spt_XC_MODEPARSE_MATCHMODEEX,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_MODEPARSE_CMD_NONE:
        case E_XC_MODEPARSE_CMD_MAX:
        default:
        {
            printf("Command %ld is not existed\n",u32Cmd);
            u32Ret = UTOPIA_STATUS_ERR_INVALID_HANDLE;
            break;
        }

    }

	return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

#endif


