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
/// file    drvSYS.c
/// @brief  System Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <string.h>
#endif
#include "regCHIP.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include <unistd.h>     //close
#include <fcntl.h>      // open
#include <sys/ioctl.h>  // ioctl
#include <sys/mman.h>
#include "drvSysInfo.h"




//--------------------------------------------------------------------------------------------------
////  Internal Variable
////--------------------------------------------------------------------------------------------------

static MS_S32              _s32SysFd = -1;


//-------------------------------------------------------------------------------------------------
/// System information
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_SYS_Info(IO_Sys_Info_t *SysInfo)
{
    IO_Sys_Info_t LclSysInfo;  //local system information
    MS_S32 result;

    _s32SysFd = open("/dev/system", O_RDWR);

    if (0 > _s32SysFd)
    {
        return FALSE;
    }

    result = ioctl(_s32SysFd, IOCTL_SYS_INFO, &LclSysInfo);

    if(result)
    {
       return FALSE;
    }

    SysInfo->LX_MEM_ADDR = LclSysInfo.LX_MEM_ADDR;
    SysInfo->LX_MEM_LENGTH = LclSysInfo.LX_MEM_LENGTH;
    SysInfo->LX_MEM2_ADDR = LclSysInfo.LX_MEM2_ADDR;
    SysInfo->LX_MEM2_LENGTH = LclSysInfo.LX_MEM2_LENGTH;
    SysInfo->EMAC_ADDR = LclSysInfo.EMAC_ADDR;
    SysInfo->EMAC_LENGTH = LclSysInfo.EMAC_LENGTH;
    SysInfo->DRAM_ADDR = LclSysInfo.DRAM_ADDR;
    SysInfo->DRAM_LENGTH = LclSysInfo.DRAM_LENGTH;
    SysInfo->BB_ADDR = LclSysInfo.BB_ADDR;
    SysInfo->BB_LENGTH = LclSysInfo.BB_LENGTH;
    SysInfo->MPOOL_MEM_ADDR = LclSysInfo.MPOOL_MEM_ADDR;
    SysInfo->MPOOL_MEM_LENGTH = LclSysInfo.MPOOL_MEM_LENGTH;
    SysInfo->G3D_MEM0_ADDR = LclSysInfo.G3D_MEM0_ADDR;
    SysInfo->G3D_MEM0_LENGTH = LclSysInfo.G3D_MEM0_LENGTH;
    SysInfo->G3D_MEM1_ADDR = LclSysInfo.G3D_MEM1_ADDR;
    SysInfo->G3D_MEM1_LENGTH = LclSysInfo.G3D_MEM1_LENGTH;
    SysInfo->G3D_CMDQ_ADDR = LclSysInfo.G3D_CMDQ_ADDR;
    SysInfo->G3D_CMDQ_LENGTH = LclSysInfo.G3D_CMDQ_LENGTH;
    return TRUE;
}
