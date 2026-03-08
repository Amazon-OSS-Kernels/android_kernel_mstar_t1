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
#if(CONFIG_HDMITX_MSTAR_ROCKET ==1)
#include <apiHDMITx.h>
#elif(CONFIG_HDMITX_MSTAR_ROCKET2 ==1)
#include <hdmitx/mstar/rocket2/MsDrvRocket.h>
#endif

#if (ENABLE_HDMITX_MSTAR==1)
extern int msHdmitx_Disp_Init(void);
#elif (CONFIG_HDMITX_MSTAR_ROCKET==1)
extern MS_BOOL msHdmitx_Rocket_Init(HDMITX_VIDEO_TIMING outPutType );
#elif (CONFIG_TV_HDMITX_MSTAR==1)
extern int MsDrvTvHdmiTxInit(void);
#endif

int do_hdmi(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
        cmd_usage(cmdtp);
    }
    else
    {
       if(strncmp(argv[1], "init", 5) == 0)
       {
        #if(CONFIG_HDMITX_MSTAR_ROCKET ==1 ||CONFIG_HDMITX_MSTAR_ROCKET2 ==1)

            char * p_str = NULL;
        #if (CONFIG_HDMITX_MSTAR_ROCKET==1)
            HDMITX_VIDEO_TIMING u8ResolutionEnv = HDMITX_RES_1920x1080p_60Hz;
        #else
            EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING u8ResolutionEnv = E_MAPI_ROCKY_RES_1920x1080p_60Hz;
        #endif

            p_str = getenv ("resolution");
            if(NULL != p_str)
            {
                u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
            }
            UBOOT_DEBUG("ROCKET resolution Type=%d \n",u8ResolutionEnv);
            ret =msHdmitx_Rocket_Init(u8ResolutionEnv);
            if(ret == TRUE)
                ret = 0;
            else
                ret = -1;
        #elif (CONFIG_TV_HDMITX_MSTAR==1)
            ret = MsDrvTvHdmiTxInit();
        #else

            ret = msHdmitx_Disp_Init();
        #endif
       }

       else
       {
           cmd_usage(cmdtp);
       }
    }

    UBOOT_TRACE("OK\n");
    return ret;
}

