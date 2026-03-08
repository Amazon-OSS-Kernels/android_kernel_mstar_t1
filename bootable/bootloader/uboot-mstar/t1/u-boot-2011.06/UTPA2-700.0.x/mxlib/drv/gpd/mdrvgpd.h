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
#ifndef _GPDAPI_H
#define _GPDAPI_H

#include "gpd.h"

typedef struct DLL_PACKED
{
    MS_PHY    u32PA_ReadBufferAddr;
    MS_VIRT    u32VA_ReadBufferAddr;
    MS_U32    u32ReadBufferSize;
    MS_PHY    u32PA_WriteBufferAddr;
    MS_VIRT    u32VA_WriteBufferAddr;
    MS_U32    u32WriteBufferSize;
    MS_PHY	   u32PA_DepthBufferAddr;
    MS_VIRT    u32VA_DepthBufferAddr;
    MS_PHY	   u32PA_ColorBufferAddr;
    MS_VIRT	   u32VA_ColorBufferAddr;
    MS_VIRT    RegBaseAddr;
    MS_VIRT    ReadBitBaseAddr;
} gpd_addr_info;


typedef struct DLL_PACKED
{
    char   *filename;
    MS_U32    u32Width;
    MS_U32    u32Height;
    MS_U8     u8BitPP;
    MS_U8     u8Interlace;
    MS_S32    enGPDStatus;    //Error Code
    MS_U8     u8MGIF;
} gpd_pic_info;

typedef struct DLL_PACKED
{
    MS_U32    hstart;
    MS_U32    vstart;
    MS_U32    width;
    MS_U32    height;

} gpd_roi_info;


extern void mdrv_gpd_setbuf(gpd_addr_info* addr_info);
extern MS_S32 mdrv_gpd_init(gpd_addr_info* param);
extern MS_S32 mdrv_gpd_decode(gpd_pic_info* pic_info, MS_U32 Ocolor, MS_U32 mode,gpd_roi_info* roi);
extern void mdrv_gpd_scaling_init(MS_U32 enable, MS_U32 mode);
//extern void mdrv_gpd_seek(FILE *hFile);

#endif
