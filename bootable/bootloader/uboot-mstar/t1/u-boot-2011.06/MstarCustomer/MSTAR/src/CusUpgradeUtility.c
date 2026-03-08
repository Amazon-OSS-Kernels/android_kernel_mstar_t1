/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include <stdio.h>
#include <common.h>
#include <MsTypes.h>
#include <MsUpgrade.h>
#include <MsUpgradeUtility.h>
#include <CusUpgrade.h>
#include <CusUpgradeUtility.h>
#include <MsDebug.h>

int do_checkVersion(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define MAX_SUB_VERSION 2

    int updateVersion[MAX_SUB_VERSION] = {0};
    int currentVersion[MAX_SUB_VERSION] = {0};
    int i = 0;
    int ret = 0;
    char buffer[CMD_BUF] = "\0";

    if(argc!=3)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    // if find current version, compare with upgrade version.
    if(getenv(argv[1])!=NULL)
    {
        sscanf(getenv(argv[1]),"%d.%d", &currentVersion[0], &currentVersion[1]);
        sscanf(argv[2],"%d.%d", &updateVersion[0], &updateVersion[1]);

        UBOOT_DEBUG("currentVersion = %d.%d\n",currentVersion[0],currentVersion[1]);
        UBOOT_DEBUG("updateVersion = %d.%d\n",updateVersion[0],updateVersion[1]);

        for(i=0; i<MAX_SUB_VERSION; i++)
        {
            UBOOT_DEBUG("currentVersion[%d] = %d\n",i,currentVersion[i]);
            UBOOT_DEBUG(" updateVersion[%d] = %d\n",i,updateVersion[i]);

            if(updateVersion[i] < currentVersion[i])
            {
                UBOOT_ERROR("[%s] CurrentVersion=%s is newer than UpdateVersion=%s\n", argv[1],getenv(argv[1]),argv[2]);
                setenv("upgrade_status", "ERROR");
                saveenv();
                return -1;
            }

            if(updateVersion[i] > currentVersion[i])
            {
                snprintf(buffer,CMD_BUF, "setenv %s %s",argv[1],argv[2]);
                UBOOT_DEBUG("cmd: %s \n",buffer);
                ret=run_command(buffer,0);
                break;
            }
        }
    }
    else
    {
        // never set version before
        snprintf(buffer,CMD_BUF, "setenv %s %s",argv[1],argv[2]);
        UBOOT_DEBUG("cmd: %s \n",buffer);
        ret=run_command(buffer,0);
    }

    return ret;
}

