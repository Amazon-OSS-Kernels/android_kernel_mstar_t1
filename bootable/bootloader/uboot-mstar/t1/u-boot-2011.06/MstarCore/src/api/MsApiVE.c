/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

#include <command.h>
#include <common.h>
#include <MsDebug.h>
#include <ve/MsDrvVE.h>

#if(ENABLE_CVBS_OUT==1)
int do_CVBS_OUT(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    UBOOT_TRACE("IN\n");


    char * p_str = NULL;
    HDMITX_VIDEO_TIMING u8ResolutionEnv = HDMITX_RES_1920x1080p_60Hz;
    p_str = getenv ("resolution");
    if(NULL != p_str)
    {
        u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
    }
    UBOOT_DEBUG("resolution Type=%d \n",u8ResolutionEnv);
    ret =MsDrv_VE_init(u8ResolutionEnv);

    UBOOT_TRACE("OK\n");
    return ret;
}
#endif