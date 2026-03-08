/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#include <ShareType.h>
#include <MsDevice.h>
#include <common.h>
#include <command.h>
#include <MsCmdTable.h>
#include <MsAppRegister.h>
#include <MsCustomerRegister.h>
#include <MsDebug.h>
#include <MsStart.h>
#include <MsSystem.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  MstarToKernel
/// @brief         :  This function is always executed when mboot booting.
/// @author        :  MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN MstarProcess(void)
{
#if CONFIG_MINIUBOOT
#else
    ST_CMD_RECORED *pCmd=NULL;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m --------------------MstarProcess---------------------------\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");

    Customer_Register_Process();
    Customer_Register_ToKernel();


    pCmd=getFirstCmd();
    if(pCmd==NULL)
    {
        UBOOT_DEBUG("There are no any cmds in table\n");
        return TRUE;
    }

    while(1)
    {

        if(pCmd->stage == STAGE_PROCESS)
        {
            UBOOT_BOOTTIME("[AT][MB][%s][%lu]_start\n",pCmd->cmd, MsSystemGetBootTime());
            run_command(pCmd->cmd,  pCmd->flag);
            UBOOT_BOOTTIME("[AT][MB][%s][%lu]_end\n",pCmd->cmd, MsSystemGetBootTime());
        }

        pCmd=getNextCmd(pCmd);
        if(pCmd==NULL)
        {
            UBOOT_DEBUG("It's the last cmd\n");
            break;
        }
    }

    UBOOT_TRACE("OK\n");
 #endif
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  MstarToKernel
/// @brief         :  This function is only executed when mboot jumps to kernel.
/// @author        :  MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN MstarToKernel(void)
{
#if CONFIG_MINIUBOOT
    run_command("bootcheck", 0);
#else
    ST_CMD_RECORED *pCmd=NULL;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m ------------------ MstarToKernel---------------------------\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");


    pCmd=getFirstCmd();
    if(pCmd==NULL)
    {
        UBOOT_DEBUG("There are no any cmds in table\n");
        return TRUE;
    }

    while(1)
    {
        if(pCmd->stage == STAGE_TOKERNEL)
        {
            UBOOT_BOOTTIME("[AT][MB][%s][%lu]_start\n",pCmd->cmd, MsSystemGetBootTime());
            run_command(pCmd->cmd,  pCmd->flag);
            UBOOT_BOOTTIME("[AT][MB][%s][%lu]_end\n",pCmd->cmd, MsSystemGetBootTime());
        }
        pCmd=getNextCmd(pCmd);
        if(pCmd==NULL)
        {
            UBOOT_DEBUG("It's the last cmd\n");
            break;
        }
    }

    UBOOT_TRACE("OK\n");
 #endif
    return TRUE;
}

