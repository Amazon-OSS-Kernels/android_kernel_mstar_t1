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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVGOP_PRIV_H_
#define _DRVGOP_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvGFLIP.h"
#include "drvGOP.h"

typedef struct DLL_PACKED _GOP_RESOURCE_PRIVATE
{

}GOP_RESOURCE_PRIVATE;

typedef struct DLL_PACKED _GOP_INSTANT_PRIVATE
{
    void* pResource;
    GOP_CTX_DRV_LOCAL g_gopDrvCtxLocal;
    MS_GOP_CTX_LOCAL *g_pGOPCtxLocal;
    GFLIP_REGS_SAVE_AREA pGOP_STRPrivate;
}GOP_INSTANT_PRIVATE;


void GOPRegisterToUtopia(void);
MS_U32 GOPOpen(void** ppInstance, const void* const pAttribute);
MS_U32 GOPClose(void* pInstance);
MS_U32 GOPIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);


#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
