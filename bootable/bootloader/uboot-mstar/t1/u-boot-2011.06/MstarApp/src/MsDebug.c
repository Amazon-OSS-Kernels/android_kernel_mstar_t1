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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <command.h>
#include <MsDebug.h>
#include <MsTypes.h>
#include <ShareType.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define ENV_DEBUG_LEVLE "dbgLevel"

#define STR_ERROR "ERROR"
#define STR_INFO "INFO"
#define STR_TRACE "TRACE"
#define STR_DEBUG "DEBUG"
#define STR_BOOTTIME "BOOTTIME"

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
EN_DEBUG_LEVEL dbgLevel=DEFAULT_DEBUG_LEVEL;
EN_DEBUG_MODULE dbgModule=DEFAULT_DEBUG_MODULE;


//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);


//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
static int _initDebugLevel(void);

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
int do_set_debugging_message_level(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buffer[CMD_BUF] = "\0";
    int ret=0;

    if(argc!=2)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    if(argv[1]==NULL)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    snprintf((char *)buffer,CMD_BUF,"setenv %s %s",ENV_DEBUG_LEVLE,argv[1]);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        printf("\033[0;31m[ERROR]%s:0x%x:\033[0m",__FUNCTION__,__LINE__);
        printf("set %s to env fail.\n",ENV_DEBUG_LEVLE);
        return -1;
    }

    snprintf((char *)buffer,CMD_BUF,"saveenv");
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        printf("\033[0;31m[ERROR]%s:0x%x::\033[0m",__FUNCTION__,__LINE__);
        printf("save %s to env fail\n",ENV_DEBUG_LEVLE);
        return -1;
    }

    _initDebugLevel();

    return 0;
}

int do_initDebugLevel(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
     if(argc!=1)
     {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return -1;
     }

    _initDebugLevel();
    return 0;
}

static int _initDebugLevel(void)
{
    char *pEnv=NULL;

    pEnv=getenv(ENV_DEBUG_LEVLE);
    if(pEnv==NULL)
    {
        dbgLevel=DEFAULT_DEBUG_LEVEL;
        return 0;
    }

    if(strcmp(pEnv,STR_ERROR)==0)
    {
        dbgLevel=EN_DEBUG_LEVEL_ERROR;
        return 0;
    }
    else if(strcmp(pEnv,STR_INFO)==0)
    {
        dbgLevel=(EN_DEBUG_LEVEL_INFO+EN_DEBUG_LEVEL_ERROR);
        return 0;
    }
    else if(strcmp(pEnv,STR_TRACE)==0)
    {
        dbgLevel=(EN_DEBUG_LEVEL_INFO+EN_DEBUG_LEVEL_ERROR+EN_DEBUG_LEVEL_TRACE);
        return 0;
    }
    else if(strcmp(pEnv,STR_DEBUG)==0)
    {
        dbgLevel=(EN_DEBUG_LEVEL_INFO+EN_DEBUG_LEVEL_ERROR+EN_DEBUG_LEVEL_TRACE+EN_DEBUG_LEVEL_DEBUG+EN_DEBUG_LEVEL_BOOTTIME);
        return 0;
    }
    else if(strcmp(pEnv,STR_BOOTTIME)==0)
    {
        dbgLevel=EN_DEBUG_LEVEL_BOOTTIME;
        return 0;
    }
    else
    {
        dbgLevel=DEFAULT_DEBUG_LEVEL;
    }

    return 0;
}

void _dump(unsigned int addr, unsigned int size)
{
    char buffer[CMD_BUF]="\0";
    snprintf(buffer,CMD_BUF,"md.b %x %x",addr,size);
    run_command(buffer,0);
}


