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
#include <MsTypes.h>
#include <version.h>
#include <MsEnvironment.h>

#define MB_VER_STR                                  "MB_mboot_ver"
#define DO_BOOTM_STR                             "MB_do_bootm"
#define INFO_EN_CHECKPOINTS_CFG          "en_chk_p"
#define CMD_BUF                                       128
#define MAX_SAVING_FILE_NUMBERS          16
extern char aPrePoints[CMD_BUF];
extern unsigned int start_ub_time;
extern int del_bootargs_cfg(char *,MS_BOOL);
extern int set_bootargs_cfg(char *,char *,MS_BOOL);

void do_points_handle(void)
{
    char* pInfo = getenv(INFO_EN_CHECKPOINTS_CFG);

    //printf("\n...[%s, %d] pInfo=%s, val=%d...\n", __FUNCTION__, __LINE__, pInfo, val);
    if (pInfo != NULL)
    {
        int val = 0;

        val = atoi(pInfo);

        //printf("\n...[%s, %d] pInfo=%s, val=%d...\n", __FUNCTION__, __LINE__, pInfo, val);
        if (val > 0 && val < MAX_SAVING_FILE_NUMBERS)
        {
            char _buf[10] = "";

            sprintf(_buf, "%s=%d", INFO_EN_CHECKPOINTS_CFG, val++);
            set_bootargs_cfg(INFO_EN_CHECKPOINTS_CFG, _buf, 0);

            sprintf(_buf, "%d", val);
            setenv(INFO_EN_CHECKPOINTS_CFG, _buf);
            saveenv();

            //printf("\n...[%s, %d] set %s to val=%d ok...\n", __FUNCTION__, __LINE__, INFO_EN_CHECKPOINTS_CFG, val);
        }
        else
        {
            int ret =0;

            setenv(INFO_EN_CHECKPOINTS_CFG, "");
            saveenv();

            ret = del_bootargs_cfg(INFO_EN_CHECKPOINTS_CFG,0);
            if(ret == 0)
            {
                if(val == MAX_SAVING_FILE_NUMBERS)
                {
                    printf("\n\n <<<<< customer performance index is done >>>>>\n");
                    printf(" <<<<< customer performance index is done >>>>>\n");
                    printf(" <<<<< customer performance index is done >>>>>\n\n");
                }
                //printf("\n...[%s, %d] del %s ok (after catch %d times)...\n", __FUNCTION__, __LINE__, INFO_EN_CHECKPOINTS_CFG, MAX_SAVING_FILE_NUMBERS-1);
            }
            else
            {
                printf("\n...[%s, %d] %s del failed...\n", __FUNCTION__, __LINE__, INFO_EN_CHECKPOINTS_CFG);
            }
        }
    }
}

void Points_Write(MS_U32 val)
{
    extern int snprintf(char *str, size_t size, const char *fmt, ...);
    char strEnv[CMD_BUF];

    // "start_ub" to bootargs
    if(0 != del_bootargs_cfg(aPrePoints,0))
    {
        printf("\n...[%s, %d] del failed for  %s ...\n", __FUNCTION__, __LINE__, aPrePoints);
    }

    memset(strEnv , 0, CMD_BUF);
    snprintf(strEnv , CMD_BUF-1, "%s=%lu", aPrePoints, start_ub_time);
    if(0 != set_bootargs_cfg((char*)aPrePoints, strEnv, 0))
    {
        printf("\n%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
    }

    // "do_bootm" to bootargs
    if(0 != del_bootargs_cfg(DO_BOOTM_STR,0))
    {
        printf("\n...[%s, %d] del failed for  %s ...\n", __FUNCTION__, __LINE__, DO_BOOTM_STR);
    }

    memset(strEnv , 0, CMD_BUF);
    snprintf(strEnv , CMD_BUF-1, "%s=%lu", DO_BOOTM_STR, val);

    if(0 != set_bootargs_cfg((char*)DO_BOOTM_STR, strEnv, 0))
    {
        printf("\n%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
    }

    // "mboot changelist version" to bootargs
    if(0 != del_bootargs_cfg(MB_VER_STR,0))
    {
        printf("\n...[%s, %d] del failed for  %s ...\n", __FUNCTION__, __LINE__, MB_VER_STR);
    }

    memset(strEnv , 0, CMD_BUF);
    snprintf(strEnv , CMD_BUF-1, "%s=%s", MB_VER_STR,  (char *)CHANGELIST_STRING);

    if(0 != set_bootargs_cfg((char*)MB_VER_STR, strEnv, 0))
    {
        printf("\n%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
    }

    // add the count value
    do_points_handle();

    return;
}







