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
#ifndef _MStar_Command_Table_
#define _MStar_Command_Table_
#include <linux/list.h>

// MsStart Register Command Stage
#define STAGE_MSINIT 0
#define STAGE_PROCESS 1
#define STAGE_TOKERNEL 2


typedef struct{
    char *cmd;
    int flag;
    int stage;
    struct list_head list;
}ST_CMD_RECORED;


void Table_Init(void);
void Add_Command_Table(char *, int, int);
ST_CMD_RECORED *getFirstCmd(void);
ST_CMD_RECORED *getNextCmd(ST_CMD_RECORED *pCmd);
#endif