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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: drvRASP_priv_res.h
//  Description: RASP driver (Shared global data) header file
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RASP_PRIV_RES_H_
#define _RASP_PRIV_RES_H_

#include "drvRASP.h"

typedef struct _DrvRASP_State
{
    MS_U32          _RASP_init;
    DrvRASP_FltState _RASP_FltState;
} DrvRASP_State;

typedef struct _RASP_RESOURCE_PRIVATE
{
    MS_S32                       _s32RASPMutexId;
    MS_U32                       _u32LastErr;
    MS_BOOL                      _bSuspend;

    DrvRASP_State                _RASPState[RASP_NUM][RASP_PIDFLT_NUM_ALL];

    #ifdef HW_PVR_ENABLE
    DrvRASP_State                _PVRState[PVR_NUM][PVR_PIDFLT_NUM_ALL];
    #endif

    MS_BOOL                      _bRASPInitReady;
}RASP_RESOURCE_PRIVATE;

#endif //_RASP_PRIV_RES_H_

