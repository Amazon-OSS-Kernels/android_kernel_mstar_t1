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

#ifndef _API_XC_LD_V2_H_
#define _API_XC_LD_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//******** ENUM DEFINITIONS ********//
typedef enum
{
    E_LD_POOL_ID_INTERNAL = 0,
    E_LD_POOL_ID_NUM,
    E_LD_POOL_ID_MAX = E_LD_POOL_ID_NUM,
} E_LD_POOL_ID;

/// Define CMD ID
typedef enum
{
    E_LD_CMD_INIT,
    //E_LD_CMD_INIT_LDF,
    //E_LD_CMD_INIT_LDB,
    //E_LD_CMD_INIT_SW,
    E_LD_CMD_SETDBGLEVEL,
    E_LD_CMD_NUM,
    E_LD_CMD_MAX = E_LD_CMD_NUM,
} E_LD_IOCTL_CMDS;



typedef struct _stLD_SetDbgLevel
{
    MS_U16 u16DbgSwitch;
    MS_U16 u16Return;
} stLD_SetDbgLevel, *pstLD_SetDbgLevel;

typedef struct _stLD_CommInit
{
    MS_U16 u16Return;
} stLD_CommInit, *pstLD_CommInit;

typedef struct _stLD_Init_Parameter
{
    pstLD_Init_Parameters pLD_InitData;
    MS_U16 u16DataLen;
    MS_U16 u16Return;
} stLD_Init, *pstLD_Init;

#ifdef __cplusplus
}
#endif

#endif
