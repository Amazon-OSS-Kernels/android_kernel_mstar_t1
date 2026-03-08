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

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#else
#include <linux/string.h>
#endif
#include "MsCommon.h"
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_dapi.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"

#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"

#include "QualityMap_Text.c"
#include "drvPQ_Bin.h"
#include "PQ_private.h"
#include "apiPQ_v2.h"


MS_PQTextBin_Header_Info stPQTextBinHeaderInfo[MAX_PQ_TEXT_BIN_NUM];
MS_BOOL gbEnablePQTextBin = 0;

char* MDrv_PQ_GetSrcTypeName_U2(void *pInstance,PQ_WIN eWindow)
{
    if(gbEnablePQTextBin)
    {
#if defined (__aarch64__)
        MS_U64 u32Addr;
#else
        MS_U32 u32Addr;
#endif
        MS_U16 u16SrcIDx = MDrv_PQ_GetSrcType_U2(pInstance, eWindow);

        u32Addr = MDrv_PQTextBin_GetSrcType(pInstance,u16SrcIDx, &stPQTextBinHeaderInfo[eWindow]);
        return ((char *)u32Addr);
    }
    else
    {
        return PQ_INPUTTYPE_TEXT_Main[MDrv_PQ_GetSrcType_U2(pInstance, eWindow)];
    }
}
char* MDrv_PQ_GetSrcTypeName(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
		printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
	if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetSrcTypeName PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u8Ret = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_SRC_TYPE_NAME, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.u8Ret;
    }
}

char* MDrv_PQ_GetIPName_U2(void *pInstance,MS_U8 u8PQIPIdx)
{
    if(gbEnablePQTextBin)
    {
#if defined (__aarch64__)
        MS_U64 u32Addr;
#else
        MS_U32 u32Addr;
#endif
        u32Addr = MDrv_PQTextBin_GetIPName(pInstance,(MS_U16)u8PQIPIdx,  &stPQTextBinHeaderInfo[PQ_MAIN_WINDOW]);
        return ((char *)u32Addr);
    }
    else
    {
        return PQ_IP_Text_Main[u8PQIPIdx];
    }
}
char* MDrv_PQ_GetIPName(MS_U8 u8PQIPIdx)
{
#ifdef TRACE_DBG
		printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
	if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetIPName PQArgs;
    PQArgs.u8PQIPIdx = u8PQIPIdx;
    PQArgs.u8Ret = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_IP_NAME, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.u8Ret;
    }
}

char* MDrv_PQ_GetTableName_U2(void *pInstance,MS_U8 u8PQIPIdx, MS_U8 u8TabIdx )
{
    if(gbEnablePQTextBin)
    {
#if defined (__aarch64__)
        MS_U64 u32Addr;
#else
        MS_U32 u32Addr;
#endif
        u32Addr = MDrv_PQTextBin_GetTableName(pInstance,(MS_U16)u8PQIPIdx,
                                              (MS_U16)u8TabIdx,
                                              &stPQTextBinHeaderInfo[PQ_MAIN_WINDOW]);
        return ((char *)u32Addr);

    }
    else
    {
        char** PQ_Stream;

        PQ_Stream = PQ_IPTAB_TEXT_Main[u8PQIPIdx];
        return PQ_Stream[u8TabIdx];
    }
}
char* MDrv_PQ_GetTableName(MS_U8 u8PQIPIdx, MS_U8 u8TabIdx )
{
#ifdef TRACE_DBG
		printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
	if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetTableName PQArgs;
    PQArgs.u8PQIPIdx = u8PQIPIdx;
    PQArgs.u8TabIdx = u8TabIdx;
    PQArgs.u8Ret = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_TABLE_NAME, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.u8Ret;
    }
}

#if 0
void MDrv_PQ_Text_Demo(SCALER_WIN eWindow)
{
    MS_U8 u8IPIdx, u8TabIdx, u8IPNum, u8TabNum;
    U16 u16PQSrcType;

    u16PQSrcType = MDrv_PQ_GetSrcType(eWindow);
    printf("u16PQSrcType=%u\n", u16PQSrcType);

    u8IPNum = MDrv_PQ_GetIPNum(eWindow);

    for( u8IPIdx = 0; u8IPIdx < u8IPNum; u8IPIdx++)
    {
        u8TabNum = MDrv_PQ_GetTableNum(eWindow, u8IPIdx);
        printf("IP:%u [%s], tabnum=%u\n", u8IPIdx, MDrv_PQ_GetIPName(u8IPIdx), u8TabNum);

        u8TabIdx = MDrv_PQ_GetCurrentTableIndex(eWindow, u8IPIdx);
        printf("TAB:%u [%s]\n", u8TabIdx,
        (u8TabIdx == PQ_IP_NULL) ? "null" : MDrv_PQ_GetTableName(u8IPIdx, u8TabIdx));
    }
}
#endif
