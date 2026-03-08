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

#ifndef __MS_STR__
#define __MS_STR__

#include <ShareType.h>
#include <MsTypes.h>
/*
MIU0_GROUP_PRIORITY=1:0:2:3
MIU1_GROUP_PRIORITY=1:0:2:3
MIU2_GROUP_PRIORITY=0:0:0:0
MIU0_GROUP_SELMIU=1140:0000:0190:0000:005D:0000:0000
MIU1_GROUP_SELMIU=4E08:70F8:0A20:0000:0002:7FFF:29FF
MIU2_GROUP_SELMIU=0000:0000:0000:0000:0000:0000:0000
*/

#define MIU_COUNT 4         // MIU0,MIU1,MIU2,MIU3.....
#define MIU_GROUP_COUNT 7

typedef struct
{
    U32 u32MiuPriority[MIU_COUNT];
    U32 u32MiuSel[MIU_COUNT][MIU_GROUP_COUNT];
}ST_MIU_INFO;


typedef struct
{
    U32 u32NuttxAddr;
    U32 u32AESAddr;
    ST_MIU_INFO stMiuInfo;
}ST_FSTR_BACKUP;



int do_check_str_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
void CRCCheck(void);
int check_str_resume(void);
void fast_STR_miu_Backup(ST_MIU_INFO * stMiuInfo);

#endif

