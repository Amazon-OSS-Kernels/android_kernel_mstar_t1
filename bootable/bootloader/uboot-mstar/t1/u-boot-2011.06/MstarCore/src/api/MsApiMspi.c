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
#include <malloc.h>
#include <MsDebug.h>
#include <drvMSPI.h>
#include <drvSYS.h>
#include <ShareType.h>
#include <MsVfs.h>
#include <CusConfig.h>
#include <drvGPIO.h>

//Dolby
#if(ENABLE_DOLBY==1)

int do_dolby_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    MSPI_CLKConfig ptCLKConfig;
    MS_U8 * dolby_data=NULL;//0x20400000;
    MS_U8 * dolby_temp=NULL;//0x20400000;
    MS_U32 dolby_size=0;//0x81F2A8;

    UBOOT_TRACE("IN\n");
    char PathBuf[CMD_BUF] = "\0";
    mdrv_gpio_init();
    mdrv_gpio_set_high(PAD_GPIO2);
    mdelay(10);
    mdrv_gpio_set_low(PAD_GPIO2);
    mdelay(10);
    mdrv_gpio_set_high(PAD_GPIO2);
    vfs_mount(CONFIG);
    snprintf(PathBuf, sizeof(PathBuf), "%s/fpga/dolby_fpga.sof",CONFIG_PATH);
    UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
    dolby_size = vfs_getsize(PathBuf);
    UBOOT_DEBUG("u32dolby_size=0x%x\n",(unsigned int)dolby_size);
    dolby_data=(MS_U8 *)malloc(dolby_size);
    if(dolby_data==NULL)
    {
        UBOOT_ERROR("malloc for dolby_data fail==========\n");
        return 0;
    }
    if(dolby_size==0 || (vfs_read(dolby_data,PathBuf,0,dolby_size)!=0))
    {
        UBOOT_ERROR("Error: vfs_read Fail\n");
        free(dolby_data);
        return 0;
    }
    dolby_temp=dolby_data;
    MDrv_SYS_SetPadMux(E_TS0_PAD_SET,E_MSPI_PAD_ON);
    memset(&ptCLKConfig, 0, sizeof(ptCLKConfig));
    ptCLKConfig.BClkPolarity=0;
    ptCLKConfig.BClkPhase=0;
    ptCLKConfig.U8Clock=5;

    ret=MDrv_MSPI_Init_Ext(0);
    if(ret!=E_MSPI_OK)
    {
        UBOOT_ERROR("MDrv_MSPI_Init_Ext error %d\n",ret);
        free(dolby_data);
        return 0;
    }
    MDrv_MasterSPI_CsPadConfig(0,0xFF);
    MDrv_MSPI_CLKConfig(&ptCLKConfig);
    MDrv_MasterSPI_MaxClkConfig(0,10);
    MDrv_MSPI_SlaveEnable(true);
    while(dolby_size>0)
        {
            if(dolby_size>0xFFFF)
            {
                UBOOT_DEBUG("dolby addr [0x%x] size[0x%x] \n",(unsigned int)dolby_temp,(unsigned int)dolby_size);
                MDrv_MSPI_SlaveEnable(false);
                MDrv_MSPI_Write(dolby_temp,0xFFFF);
                dolby_temp=dolby_temp+(0xFFFF);
                dolby_size=dolby_size-(0xFFFF);

            }
            else
            {
                UBOOT_DEBUG("---dolby addr [0x%x] size[0x%x] \n",(unsigned int)dolby_temp,(unsigned int)dolby_size);
                MDrv_MSPI_SlaveEnable(false);
                MDrv_MSPI_Write(dolby_temp,dolby_size);
                dolby_size=0;
            }
        }
    MDrv_MSPI_SlaveEnable(true);

    free(dolby_data);
    UBOOT_TRACE("OK\n");
    return ret;
}

#endif

