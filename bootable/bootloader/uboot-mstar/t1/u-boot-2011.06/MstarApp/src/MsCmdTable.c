/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

#include <common.h>
#include <command.h>
#include <MsCmdTable.h>
#include <MsDebug.h>
#include <exports.h>


static struct list_head cmd_list;


void Table_Init(void)
{
    UBOOT_TRACE("IN\n");

    INIT_LIST_HEAD(&cmd_list);

    UBOOT_TRACE("OK\n");
}

void Add_Command_Table(char * cmd, int flag, int stage)
{

    ST_CMD_RECORED *pcmd=NULL;
    UBOOT_TRACE("IN\n");
    pcmd=malloc(sizeof(ST_CMD_RECORED));
    if(pcmd==NULL)
    {
        UBOOT_ERROR("malloc for pcmd fail==========\n");
        return;
    }
    memset(pcmd,0,sizeof(ST_CMD_RECORED));
    INIT_LIST_HEAD(&pcmd->list);
    pcmd->cmd=malloc(strlen(cmd)+1);
    if(pcmd->cmd==NULL)
    {
        UBOOT_ERROR("malloc for cmd fail\n");
        free(pcmd);
        return;
    }
    memset(pcmd->cmd,0,strlen(cmd)+1);
    strcpy (pcmd->cmd, cmd);

    pcmd->flag = flag;
    pcmd->stage = stage;
    list_add_tail(&pcmd->list,&cmd_list);

    UBOOT_TRACE("OK\n");
    return;
}

ST_CMD_RECORED *getFirstCmd(void)
{

    ST_CMD_RECORED *pcmd=NULL;
    UBOOT_TRACE("IN\n");


    if(list_empty(&cmd_list)==1)
    {
        UBOOT_DEBUG("This is an empty list\n");
        return NULL;
    }

    pcmd=list_entry((cmd_list.next),ST_CMD_RECORED,list);
    UBOOT_TRACE("OK\n");

    return pcmd;
}

ST_CMD_RECORED *getNextCmd(ST_CMD_RECORED *pCmd)
{
    ST_CMD_RECORED *pcmd=NULL;
    UBOOT_TRACE("IN\n");
    if(pCmd==NULL)
    {
        UBOOT_ERROR("The input parameter 'pCmd' is a numm pointer\n");
        return NULL;
    }
    if(list_is_last(&pCmd->list,&cmd_list)==1)
    {
        UBOOT_DEBUG("This is the last cmd\n");
        return NULL;
    }

    pcmd=list_entry((pCmd->list.next),ST_CMD_RECORED,list);
    UBOOT_TRACE("OK\n");

    return pcmd;
}


int Show_Command_Table(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int index=0;
    int ShowStage=0;
    ST_CMD_RECORED *pCmd=NULL;

    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
       return cmd_usage(cmdtp);
    }
    ShowStage= (int)argv[1][0]-'0';

    UBOOT_INFO("No.  CmdName    Stage\n");

    pCmd=getFirstCmd();
    if(pCmd==NULL)
    {
        UBOOT_DEBUG("There are no any cmds in table\n");
        return 0;
    }

    while(1)
    {
        if((pCmd->stage == STAGE_MSINIT) && (ShowStage == 0 ||  ShowStage == 1))
        {
           UBOOT_INFO("%d  %s  STAGE_MSINIT\n",(index+1),pCmd->cmd);
        }
        if ((pCmd->stage == STAGE_PROCESS) && (ShowStage == 0 ||  ShowStage == 2))
        {
           UBOOT_INFO("%d  %s  STAGE_PROCESS\n",(index+1),pCmd->cmd);
        }
        if ((pCmd->stage == STAGE_TOKERNEL) && (ShowStage == 0 ||  ShowStage == 3))
        {
           UBOOT_INFO("%d  %s  STAGE_TOKERNEL\n",(index+1),pCmd->cmd);
        }
        pCmd=getNextCmd(pCmd);
        if(pCmd==NULL)
        {
            UBOOT_DEBUG("It's the last cmd\n");
            break;
        }
        index++;
    }
    return 0;
}

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
    showtb,    3,    1,     Show_Command_Table,
    "Show register command table."
    "    -  showtb [stage]\n",
    "0:ALL , 1:MsInit , 2:MsProcess , 3:MsToKernel\n"
);
#endif

